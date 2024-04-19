#include "app_monitor_report.h"
#include "bsp_dev_para.h"
#include "crc.h"
#include "mcp_core.h"
#include "bsp_public_drv.h"
#include "app_monitor.h"
#include "ru_device_list.h"
#include "bsp_config.h"
#include "rtt.h"
#include "mu_ru_communicate.h"
#include "bsp_flash_struct.h"

/*---------------------- 结构体 ----------------------------------------*/
typedef enum 
{
    NO_REPORT           = 0x00U,
    BE_REPORTING        = 0x01U,
    REPORT_COMPLETE     = 0x02U,
} Login_Report_Start_E;

typedef struct
{
    uint8_t login_report_status;    // 登陆上报状态
    uint8_t heart_beat_report_cnt;  // 心跳包上报次数
    uint32_t last_heart_beat_report_time;   // 心跳包最后一次上报时间
}mon_rep_t;

/*---------------------- 局部变量 --------------------------------------*/
mon_rep_t   mon_rep[BLE_CONNECT_MAX_NUM + 1] = {0};
ru_device_t *ru_para = ru_device_list;

/**********************************全局变量************************************/
extern uint8_t  g_ucVerUpgradeFlag;


/**********************************创建socket上报消息队列************************************/
QueueHandle_t xSocketQueue   = NULL;

/**
 * @brief 创建消息队列
 */
void socket_queue_create(void)
{
    xSocketQueue = xQueueCreate(SOCKET_REPORT_QUEUE_LEN, sizeof(SocketQueue_T));    
    if(NULL == xSocketQueue)
    {
        rtt_printf(RTT_OS, RTT_CTRL_TEXT_BRIGHT_RED"ERROR: Create xSocketQueue Fail!!!\r\n");
    }
}


static uint16_t TimeTcpHeartFrameCnt = 0x8000; // 通信包标识


void warn_report_test(void);
// 主机监控上报
// sendBuf:发送消息存储
// type：上报类型
// id：上报设备编号
uint16_t mu_monitor_report(uint8_t id, uint8_t type, uint8_t *data_in, uint16_t data_len,uint8_t *sendBuf)
{
    uint16_t res_crc;
    uint16_t len = 0;
    uint16_t msgLen = 0;
    uint16_t Socketsendlen = 0;
    uint8_t ucaWarnMsg[SOCKET_REPORT_QUEUE_SIZE] = {0};

    uint8_t *pt_start = ucaWarnMsg;
    uint8_t *pt = ucaWarnMsg;

    if ((TimeTcpHeartFrameCnt >= 0x8000) && (TimeTcpHeartFrameCnt < 0xFFFF))
        TimeTcpHeartFrameCnt++;
    else
        TimeTcpHeartFrameCnt = 0x8000;

    *pt++ = 0x7E;
    *pt++ = 0x03;
    *pt++ = 0x01;
    *pt++ = tOmcPara.bs_id & 0x000000FF;
    *pt++ = (tOmcPara.bs_id >> 8) & 0x000000FF;
    *pt++ = (tOmcPara.bs_id >> 16) & 0x000000FF;
    *pt++ = (tOmcPara.bs_id >> 24) & 0x000000FF;
    *pt++ = (id == 0) ? tOmcPara.dev_id : id;
    *pt++ = (TimeTcpHeartFrameCnt & 0x00FF);
    *pt++ = ((TimeTcpHeartFrameCnt >> 8) & 0x00FF);
    *pt++ = 0x80;
    *pt++ = 0x01;
    *pt++ = 0x01;
    *pt++ = 0xFF;
    *pt++ = 0x04;
    *pt++ = 0x41; // 0x41
    *pt++ = 0x01; // 0x01
    *pt++ = type; // 监控上报类型
    if(type == MCP_REP_WARN && data_in != NULL)
    {
        memcpy(pt, data_in, data_len);
        pt += data_len;
    }

    len = pt - pt_start; // 计算需要进行CRC校验的消息长度, 不包含起始字节
    res_crc = crc16(pt_start + 1, len - 1);

    *pt++ = (uint8_t)res_crc;
    *pt++ = (uint8_t)(res_crc >> 8);
    *pt++ = 0x7E;

    len = pt - pt_start;               // 计算整条消息的长度, 包含起始字节/结束字节
    msgLen = MsgEscape(pt_start, len); // 转义处理

    Socketsendlen = MsgSplit(ucaWarnMsg, sendBuf, msgLen);

    return Socketsendlen;
}

/*****************************************************************************
 * @Name   : monitor_heart_last_time_set
 * @Brief  : 设置设备最后一次心跳包上报时间
 * @Param  : dev_id:设备编号
 * @Retval : 
 * @Author :
 * @Data   : 2024-04-12
 * @Note   :
 *****************************************************************************/
void monitor_heart_beat_last_time_set(uint8_t dev_id)
{
    if (dev_id == 231) {
        mon_rep[BLE_CONNECT_MAX_NUM].last_heart_beat_report_time = get_run_time();
    } else {
        mon_rep[dev_id].last_heart_beat_report_time = get_run_time();
    }
}

// 监控上报处理-开站上报和心跳包上报
void monitor_report_handle(void)
{
    uint8_t warn_buf[100] = {0};
    uint16_t tx_pdu_len = 0, warn_len = 0;
    SocketQueue_T tSocketQueue;

	memset(&tSocketQueue, 0, sizeof(SocketQueue_T));
	
    uint8_t mu_report_cnt,ru_report_cnt,warn_report_cnt;
    uint8_t i = 0,j,ucTemp;

    uint32_t time_cnt = get_run_time();

    if(g_ucVerUpgradeFlag == 1)
        return ;

    // cat1模组未使能, 则不进行业务处理
    if(hCat1.cat1_en == B_FALSE)
        return;
        
    // 检测到初连接服务器，发送登陆上报
    if (hCat1.socketStat[SOCKET_LINK_USER].ucLinkSeverState == B_TRUE)
    {
        if((hCat1.socketConfig[SOCKET_LINK_USER].ipv4_addr[0] == 39) && (hCat1.socketConfig[SOCKET_LINK_USER].ipv4_addr[1] == 106) && (hCat1.socketConfig[SOCKET_LINK_USER].ipv4_addr[2] == 42) && (hCat1.socketConfig[SOCKET_LINK_USER].ipv4_addr[3] == 94))
        {
            if (mon_rep[0].login_report_status == NO_REPORT)
            {
                // 发送登陆上报
                mu_report_cnt = 0;
                tx_pdu_len = mu_monitor_report(0, MCP_REP_LOGIN_MONITOR, NULL, 0, tSocketQueue.ucSocketBuf);
                tSocketQueue.ucSocketSendLen = tx_pdu_len;
                while (xQueueSend(xSocketQueue,&tSocketQueue,0) != pdPASS) // 发送
                {
                    mu_report_cnt++;
                    if (mu_report_cnt >= 3)
                        break;
                    vTaskDelay(1000);
                }
                mon_rep[0].login_report_status = REPORT_COMPLETE;
                mon_rep[0].last_heart_beat_report_time = time_cnt;
            }

            // 从机登陆上报
            for (i = 0; i < BLE_CONNECT_MAX_NUM; i++)
            {
                ucTemp = (ru_para[i].ru_dev_id == 231) ? BLE_CONNECT_MAX_NUM : ru_para[i].ru_dev_id;
                if (ru_para[i].ru_conn_state == RU_CONN_STATE_CONFIGURED)
                {
                    ru_report_cnt = 0;
                    tx_pdu_len = mu_monitor_report(ru_para[i].ru_dev_id, MCP_REP_LOGIN_MONITOR, NULL, 0, tSocketQueue.ucSocketBuf);
                    tSocketQueue.ucSocketSendLen = tx_pdu_len;
                    while (1) // 发送
                    {
                        if (xQueueSend(xSocketQueue,&tSocketQueue,0) == pdPASS)
                        {
                            mon_rep[ucTemp].login_report_status = REPORT_COMPLETE;
                            mon_rep[ucTemp].last_heart_beat_report_time = time_cnt;
                            break;
                        }
                        ru_report_cnt++;
                        if (ru_report_cnt >= 3)
                            break;
                        vTaskDelay(1000);
                    }
                }
            }

            // 主机心跳包定时上报,首次上报为开机60秒,之后按设置间隔上报
            if (time_cnt - mon_rep[0].last_heart_beat_report_time >= (tOmcPara.heart * 1000 ))
            {
                //主机心跳上报
                mu_report_cnt = 0;
                tx_pdu_len = mu_monitor_report(0, MCP_REP_HEARTBEAT, NULL, 0, tSocketQueue.ucSocketBuf);
                tSocketQueue.ucSocketSendLen = tx_pdu_len;
                while (xQueueSend(xSocketQueue,&tSocketQueue,0) != pdPASS) // 发送
                {
                    mu_report_cnt++;
                    if (mu_report_cnt >= 3)
                        break;
                    vTaskDelay(1000);
                }
                mon_rep[0].last_heart_beat_report_time = time_cnt;
            }

            //从机心跳上报
            for(i = 0;i < BLE_CONNECT_MAX_NUM;i++)
            {
                ucTemp = (ru_para[i].ru_dev_id == 231) ? BLE_CONNECT_MAX_NUM : ru_para[i].ru_dev_id;
                if(mon_rep[ucTemp].login_report_status == REPORT_COMPLETE && ru_para[i].ru_conn_state == RU_CONN_STATE_CONFIGURED)
                {
                    if (time_cnt - mon_rep[ucTemp].last_heart_beat_report_time >= (tOmcPara.heart * 1000 )) {
                        ru_report_cnt = 0;
                        tx_pdu_len = mu_monitor_report(ru_para[i].ru_dev_id, MCP_REP_HEARTBEAT, NULL, 0, tSocketQueue.ucSocketBuf);
                        tSocketQueue.ucSocketSendLen = tx_pdu_len;
                        while (xQueueSend(xSocketQueue,&tSocketQueue,2) != pdPASS) // 发送
                        {
                            ru_report_cnt++;
                            if (ru_report_cnt >= 3)
                                break;
                            vTaskDelay(1000);
                        }
                        mon_rep[ucTemp].last_heart_beat_report_time = time_cnt;
                    }
                }
            }
        }
        else
        {
            //登陆上报失败处理
            for(i = 0; i <= BLE_CONNECT_MAX_NUM;i++)
            {
                if(mon_rep[i].login_report_status == BE_REPORTING)
                {
                    if((time_cnt - mon_rep[i].last_heart_beat_report_time) > LOGIN_FAILD_TIME)
                    {
                        mon_rep[i].login_report_status = NO_REPORT;
                    }
                }
            }

            //主机开机登陆上报
            if(mon_rep[0].login_report_status == NO_REPORT)
            {
                tx_pdu_len = mu_monitor_report(0, MCP_REP_LOGIN_MONITOR, NULL, 0, tSocketQueue.ucSocketBuf);
                tSocketQueue.ucSocketSendLen = tx_pdu_len;
                xQueueSend(xSocketQueue,&tSocketQueue,0);
                mon_rep[0].login_report_status = BE_REPORTING;  //主机登陆上报开始
                mon_rep[0].last_heart_beat_report_time = time_cnt;
            }
            
            //从机登陆上报
            if(mon_rep[0].login_report_status == REPORT_COMPLETE)    //主机上报成功
            {
                for(i = 0;i < BLE_CONNECT_MAX_NUM ;i++)
                {
                    ucTemp = (ru_para[i].ru_dev_id == 231) ? BLE_CONNECT_MAX_NUM : ru_para[i].ru_dev_id;
                    if (ru_para[i].ru_conn_state == RU_CONN_STATE_CONFIGURED &&  mon_rep[ucTemp].login_report_status == NO_REPORT)   //需要上报
                    {
                        tx_pdu_len = mu_monitor_report(ru_para[i].ru_dev_id, MCP_REP_LOGIN_MONITOR, NULL, 0, tSocketQueue.ucSocketBuf);
                        tSocketQueue.ucSocketSendLen = tx_pdu_len;
                        xQueueSend(xSocketQueue,&tSocketQueue,0);

                        mon_rep[ucTemp].login_report_status = BE_REPORTING;
                        mon_rep[ucTemp].last_heart_beat_report_time = time_cnt;
                    }
                }
            }

            // 心跳包定时上报,按设置间隔上报
            if (time_cnt - mon_rep[0].last_heart_beat_report_time >= (tOmcPara.heart * 1000 ))
            {
                //主机心跳上报
                if(mon_rep[0].login_report_status == REPORT_COMPLETE)
                {
                    if (mon_rep[0].heart_beat_report_cnt == 3) {
                        socket_disconnect(LINK0);   // 断开连接
                        mon_rep[0].heart_beat_report_cnt = 0;
                        return;
                    }
                    mu_report_cnt = 0;
                    tx_pdu_len = mu_monitor_report(0, MCP_REP_HEARTBEAT, NULL, 0, tSocketQueue.ucSocketBuf);
                    tSocketQueue.ucSocketSendLen = tx_pdu_len;
                    while (xQueueSend(xSocketQueue,&tSocketQueue,0) != pdPASS) // 发送
                    {
                        mu_report_cnt++;
                        if (mu_report_cnt >= 3)
                            break;
                        vTaskDelay(1000);
                    }
                }
                mon_rep[0].last_heart_beat_report_time = time_cnt;
                mon_rep[0].heart_beat_report_cnt++;
            }

            //从机心跳上报
            for(i = 0;i < BLE_CONNECT_MAX_NUM;i++)
            {
                ucTemp = (ru_para[i].ru_dev_id == 231) ? BLE_CONNECT_MAX_NUM : ru_para[i].ru_dev_id;
                if(ru_para[i].ru_conn_state == RU_CONN_STATE_CONFIGURED && mon_rep[ucTemp].login_report_status == REPORT_COMPLETE)
                {
                    if (time_cnt - mon_rep[ucTemp].last_heart_beat_report_time >= (tOmcPara.heart * 1000 )) {
                        if (mon_rep[ucTemp].heart_beat_report_cnt == 3) {
                            socket_disconnect(LINK0);   // 断开连接
                            mon_rep[ucTemp].heart_beat_report_cnt = 0;
                            return;
                        }
                        ru_report_cnt = 0;
                        tx_pdu_len = mu_monitor_report(ru_para[i].ru_dev_id, MCP_REP_HEARTBEAT, NULL, 0, tSocketQueue.ucSocketBuf);
                        tSocketQueue.ucSocketSendLen = tx_pdu_len;
                        while (xQueueSend(xSocketQueue,&tSocketQueue,2) != pdPASS) // 发送
                        {
                            ru_report_cnt++;
                            if (ru_report_cnt >= 3)
                                break;
                            vTaskDelay(1000);
                        }
                        mon_rep[ucTemp].last_heart_beat_report_time = time_cnt;
                        mon_rep[ucTemp].heart_beat_report_cnt++;
                    }
                }
            }

            // 告警上报
            for (i = 0; i < (sizeof(t_dev_warn) / sizeof(t_dev_warn[0])); i++)
            {
                // 判断是否需要上报
                if (dev_warn_rpt_flag(i) == B_TRUE)
                {
                    memset(warn_buf, 0, sizeof(warn_buf));
                    // 复制告警消息
                    warn_len = dev_warn_copy_warn_msg(i, warn_buf);
                    if(i == 0)
                    {
                        tx_pdu_len = mu_monitor_report(0, MCP_REP_WARN, warn_buf, warn_len, tSocketQueue.ucSocketBuf);
                    }
                    else
                    {
                        for(j = 0; j < BLE_CONNECT_MAX_NUM;j++)
                        {
                            if(i == ru_para[j].ru_dev_id)
                            {
                                tx_pdu_len = mu_monitor_report(ru_para[j].ru_dev_id, MCP_REP_WARN, warn_buf, warn_len, tSocketQueue.ucSocketBuf);
                            }
                        }
                    }

                    if(tx_pdu_len < 14)
                        return;
                    
                    // 发送告警上报
                    warn_report_cnt = 0;
                    tSocketQueue.ucSocketSendLen = tx_pdu_len;
                    
                    while (xQueueSend(xSocketQueue,&tSocketQueue,0) != pdPASS) // 发送
                    {
                        warn_report_cnt++;
                        if (warn_report_cnt >= 3)
                            break;
                        vTaskDelay(1000);
                    }
                    rtt_printf(RTT_MONITOR, RTT_CTRL_TEXT_BRIGHT_YELLOW "Report warn: dev %d \r\n", i);
                    dev_clr_warn_state_after_rpt(i);
                }
            }
        }
    }
    else
    {
        memset(mon_rep , 0 , (sizeof(mon_rep_t) * (BLE_CONNECT_MAX_NUM + 1)));
    }
}


//监控上报信息接收处理
void monitor_queue_recv(void)
{
    uint8_t queue_report_cnt;
    BaseType_t xResult = pdFALSE;
    SocketQueue_T tSocketQueue;

    // cat1模组未使能, 则不进行业务处理
    if(hCat1.cat1_en == B_FALSE)
        return;

    xResult = xQueueReceive(xSocketQueue,&tSocketQueue,0);
    
    vTaskDelay(500);
    
    if(xResult == pdPASS)
    {
        queue_report_cnt = 0;
        while(cat1_send_msg(SOCKET_LINK_USER,tSocketQueue.ucSocketBuf,tSocketQueue.ucSocketSendLen) != RET_OK)
        {
            queue_report_cnt++;
            if (queue_report_cnt >= 3)
                break;
            vTaskDelay(1000);
        }
    }  
}



void warn_report_test(void)
{
    uint8_t temp_buf[150];
    uint8_t warn_buf[100] = {0};
    uint16_t warn_len = 0,tx_pdu_len = 0;
    int i = 0;
    // 告警上报
    for (i = 0; i < (sizeof(t_dev_warn) / sizeof(t_dev_warn[0])); i++)
    {
        if (dev_warn_rpt_flag(i) == B_TRUE)
        {
            memset(temp_buf, 0, sizeof(temp_buf));
            memset(warn_buf, 0, sizeof(warn_buf));
            warn_len = dev_warn_copy_warn_msg(i, warn_buf);
            tx_pdu_len = mu_monitor_report(ru_para[i].ru_dev_id, MCP_REP_WARN, warn_buf, warn_len, temp_buf);

            rtt_printf(RTT_MONITOR, RTT_TEXT_GREEN"Warn report: %d, len: %d, msg: %s\r\n", i, tx_pdu_len, temp_buf);
            dev_clr_warn_state_after_rpt(i);
        }
    }
}

//上报类型设置处理
void report_process(uint8_t type)
{
	uint8_t report_cnt_mu = 0;
	uint8_t dev_id = 0;
    uint8_t ble_conn_id,i,rpt_type,rpt_flag;//rpt_flag == 1,上报
	uint16_t tx_pdu_len_mu = 0;
	SocketQueue_T tSocketQueue;
	memset(&tSocketQueue, 0, sizeof(SocketQueue_T));
	
    switch (type)
    {
	    case MCP_REP_SITE_OPEN:
            dev_id = 0;
            rpt_flag = 1;
            rpt_type = MCP_REP_SITE_OPEN;
			break;
		case MCP_REP_CONFIG_CHANGE:
            dev_id = 0;
            rpt_flag = 1;
            rpt_type = MCP_REP_CONFIG_CHANGE;
			break;
		case MCP_REP_SITE_OPEN_HUB:
            dev_id = 231;
            rpt_flag = 1;
            rpt_type = MCP_REP_SITE_OPEN;
			break;
		case MCP_REP_CONFIG_CHANGE_HUB:
			dev_id = 231;
            rpt_flag = 1;
            rpt_type = MCP_REP_CONFIG_CHANGE;
			break;
		case MCP_REP_SITE_OPEN_RU1:
			dev_id = 1;
            rpt_flag = 1;
            rpt_type = MCP_REP_SITE_OPEN;
			break;
		case MCP_REP_CONFIG_CHANGE_RU1:
			dev_id = 1;
            rpt_flag = 1;
            rpt_type = MCP_REP_CONFIG_CHANGE;
			break;
		case MCP_REP_SITE_OPEN_RU2:
            dev_id = 2;
            rpt_flag = 1;
            rpt_type = MCP_REP_SITE_OPEN;
			break;
		case MCP_REP_CONFIG_CHANGE_RU2:
			dev_id = 2;
            rpt_flag = 1;
            rpt_type = MCP_REP_CONFIG_CHANGE;
			break;
		case MCP_REP_SITE_OPEN_RU3:
			dev_id = 3;
            rpt_flag = 1;
            rpt_type = MCP_REP_SITE_OPEN;
			break;
		case MCP_REP_CONFIG_CHANGE_RU3:
			dev_id = 3;
            rpt_flag = 1;
            rpt_type = MCP_REP_CONFIG_CHANGE;
			break;
		case MCP_REP_SITE_OPEN_RU4:
			dev_id = 4;
            rpt_flag = 1;
            rpt_type = MCP_REP_SITE_OPEN;
			break;
		case MCP_REP_CONFIG_CHANGE_RU4:
			dev_id = 4;
            rpt_flag = 1;
            rpt_type = MCP_REP_CONFIG_CHANGE;
			break;
		case MCP_REP_SITE_OPEN_RU5:
			dev_id = 5;
            rpt_flag = 1;
            rpt_type = MCP_REP_SITE_OPEN;
			break;
		case MCP_REP_CONFIG_CHANGE_RU5:
			dev_id = 5;
            rpt_flag = 1;
            rpt_type = MCP_REP_CONFIG_CHANGE;
			break;
		case MCP_REP_SITE_OPEN_RU6:
			dev_id = 6;
            rpt_flag = 1;
            rpt_type = MCP_REP_SITE_OPEN;
			break;
		case MCP_REP_CONFIG_CHANGE_RU6:
			dev_id = 6;
            rpt_flag = 1;
            rpt_type = MCP_REP_CONFIG_CHANGE;
			break;
		case MCP_REP_SITE_OPEN_RU7:
			dev_id = 7;
            rpt_flag = 1;
            rpt_type = MCP_REP_SITE_OPEN;
			break;
		case MCP_REP_CONFIG_CHANGE_RU7:
			dev_id = 7;
            rpt_flag = 1;
            rpt_type = MCP_REP_CONFIG_CHANGE;
			break;
		case MCP_REP_SITE_OPEN_RU8:
			dev_id = 8;
            rpt_flag = 1;
            rpt_type = MCP_REP_SITE_OPEN;
			break;
		case MCP_REP_CONFIG_CHANGE_RU8:
            dev_id = 8;
            rpt_flag = 1;
            rpt_type = MCP_REP_CONFIG_CHANGE;
			break;
		default:
            rpt_flag = 0;
			break;
	}

    if((rpt_type == MCP_REP_CONFIG_CHANGE) && (dev_id > 0))
    {
        for(i = 0; i < BLE_CONNECT_MAX_NUM;i++)
        {
            if(dev_id == ru_para[i].ru_dev_id)
                ble_conn_id = ru_para[i].ble_conn_id;
        }

        if(get_slave_dev_info(ble_conn_id,4000) != STAT_OK) //蓝牙断开
        {
            rpt_flag = 0;
        }
    }
    
    if(rpt_flag == 0)
        return;

    tx_pdu_len_mu = mu_monitor_report(dev_id, rpt_type, NULL, 0, tSocketQueue.ucSocketBuf);
    tSocketQueue.ucSocketSendLen = tx_pdu_len_mu;

    while (xQueueSend(xSocketQueue,&tSocketQueue,0) != pdPASS) // 发送
    {
        report_cnt_mu++;
        if (report_cnt_mu >= 3)
            break;
        vTaskDelay(1000);
    }
}
//上报类型设置后回复处理
void report_reply_process(uint8_t dev_id,uint8_t type)
{
    if((type == MCP_REP_SITE_OPEN)||(type == MCP_REP_LOGIN_MONITOR))
    {
        if(dev_id == 231)
            mon_rep[BLE_CONNECT_MAX_NUM].login_report_status = REPORT_COMPLETE;
        else
            mon_rep[dev_id].login_report_status = REPORT_COMPLETE;   // 上报成功
    } else if (type == MCP_REP_HEARTBEAT) {
        if (dev_id == 231) {
            mon_rep[BLE_CONNECT_MAX_NUM].heart_beat_report_cnt = 0;
        } else {
            mon_rep[dev_id].heart_beat_report_cnt = 0;
        }
    }
}
