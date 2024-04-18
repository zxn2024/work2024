/***************************** Copyright ****************************************
*
* (C) Copyright 2023, Shaanxi Tianji Communication
*  All Rights Reserved
*
* FileName   : bsp_rf_ble.c
* Version    : none
* Author     :
* Date       : 2023-10-19
* Description: none
*******************************************************************************/
#include "main.h"
#include "stdio.h"
#include "bsp_config.h"
#include "rtt.h"

#include "bsp_rf_slave.h"
#include "bsp_dev_para.h"
#include "hi_systick.h"
#include "hi_uart.h"
#include "ble.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "ascii_hex.h"
/*---------------------- 端口定义 --------------------------------------*/


/*---------------------- 宏定义  ---------------------------------------*/


/*---------------------- 结构体 ----------------------------------------*/


/*---------------------- 常量 ------------------------------------------*/


/*---------------------- 全局变量 --------------------------------------*/

xQueueHandle ble_msg_queue = NULL;
xQueueHandle ble_cmd_queue = NULL;
T_BleData ble_msg_data = {0};
/*---------------------- 局部变量 --------------------------------------*/

uart_func_t * p_ble_uart = NULL;
static uint8_t * p_tx_buf = NULL;
static uint8_t * p_rx_buf = NULL;

ble_T hBle;  // 蓝牙参数
/*---------------------- 声明 ------------------------------------------*/



/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 函数 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*****************************************************************************
* @Name	  : ble_queue_create
* @Brief  : 创建队列
* @Retval : None
* @Note   :
*/
void ble_queue_create(void)
{
    ble_msg_queue = xQueueCreate(5, sizeof(T_BleData));
    ble_cmd_queue = xQueueCreate(2, sizeof(T_BleData));
}
/**********************************************************
 *函数名：   BSP_866x_lock
 *函数功能： 锁定ble芯片,使其不能被其他程序使用,如果已被其他任务锁定立刻返回失败
 *返回值：   STAT_ERROR STAT_OK
 */
Status_t ble_try_lock(ble_T * hble)
{
    if (hble->ble_stat.lock == B_TRUE)
    {
        return STAT_ERROR;
    }else
    {
        hble->ble_stat.lock = B_TRUE;
        return STAT_OK;
    }    
}

/**********************************************************
 *函数名：   BSP_866x_lock
 *函数功能： 锁定ble芯片,使其不能被其他程序使用,超时则返回失败
 *返回值：   STAT_ERROR STAT_OK
 */
Status_t ble_wait_lock(ble_T * hble, uint32_t timeout_ms)
{
    uint32_t time_cnt = 0;

    while(hble->ble_stat.lock != B_FALSE)
    {
        vTaskDelay(10);
        time_cnt += 10;
        if(time_cnt >= timeout_ms)
        {
            break;
        }
    }

    if (hble->ble_stat.lock == B_FALSE)
    {
        hble->ble_stat.lock = B_TRUE;
        return STAT_OK;
    }else
    {
        return STAT_ERROR;
    }
}
/**********************************************************
 *函数名：   BSP_866x_unlock
 *函数功能： 解锁ble芯片,使其可以被其他程序使用
 *返回值：   无
 */
void ble_unlock(ble_T * hble)
{
    hble->ble_stat.lock = B_FALSE;
}
/*****************************************************************************
* @Name	  : ble_register
* @Brief  : ble结构体注册
* @Retval : None
* @Note   :
*/
void ble_register(ble_hw_config_t * ble_config)
{
    memset(&hBle, 0, sizeof(ble_T));
    memcpy(&hBle.hw_config, ble_config, sizeof(ble_hw_config_t));
    p_ble_uart = hBle.hw_config.ble_uart;
    p_tx_buf = p_ble_uart->tpt_data->pt_tx_data;
    p_rx_buf = p_ble_uart->tpt_data->pt_rx_data;
    ble_unlock(&hBle);

    (void )p_tx_buf; //避免编译报警
    hBle.queue_create = B_FALSE;
}


/*****************************************************************************
 * @Name	  : vCatRxDataClear
 * @Brief  : 清空接收数据缓冲区
 * @Param  : None
 * @Retval : None
 * @Author :
 * @Date   : 2022-08-16
 * @Note   :
 */
static void ble_rx_data_clear(void)
{
    p_ble_uart->rx_data_clr(p_ble_uart->tpt_data);
}

/*****************************************************************************
 * @Name	  : vCatTxDataClear
 * @Brief  : 清空发送数据缓冲区
 * @Param  : None
 * @Retval : None
 * @Author :
 * @Date   : 2022-08-16
 * @Note   :
 */
void ble_tx_data_clear(void)
{
    p_ble_uart->tx_data_clr(p_ble_uart->tpt_data);
}

/*****************************************************************************
* @Name	  : ble_rst
* @brief  : BLE复位
* @param  : None
* @retval :
* @author :
* @Data   : 2023-05-23
*****************************************************************************/
void ble_rst(void)
{
    #if LOCAL_MODULE_TYPE == _MU
    BI_PORT_CTL(&hBle.hw_config.ble_dc_ctrl, PORT_ON);
    vTaskDelay(500);
    BI_PORT_CTL(&hBle.hw_config.ble_dc_ctrl, PORT_OFF);
    #else
    BI_PORT_CTL(&hBle.hw_config.ble_dc_ctrl, PORT_OFF);
    vTaskDelay(500);
    BI_PORT_CTL(&hBle.hw_config.ble_dc_ctrl, PORT_ON);
    #endif

    rtt_printf(RTT_BLE, RTT_CTRL_TEXT_BRIGHT_GREEN"vBleHardwareReset!!!\r\n");

}

/*****************************************************************************
* @Name	  : ble_callback
* @Brief  : 回调函数
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2023-11-22
* @Note   :
*****************************************************************************/
void ble_callback(void)
{
    //p_ble_uart->callback(p_ble_uart->tpt_data);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DRV ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#if( LOCAL_MODULE_TYPE == _MU )
/**
 * @brief 获取指定mac地址对应的从设备index
 * @param conn_handle
 * @retval 成功返回索引号，失败返回-1
 */
int get_slave_dev_index_by_mac(uint8_t * mac)
{
    int i;
    for(i = 0; i < BLE_CONNECT_MAX_NUM; i++)
    {
        if(memcmp(hBle.slave_conn[i].mac_addr, mac, 6) == 0)
        {
            return i;
        }
    }
    return -1;
}
/*****************************************************************************
 * @Name	  : get_ru_data_index_by_id
 * @Brief  : 根据从设备ID获取从机索引(ID: 通信id, 索引: 从机结构体数组下标)
 * @Param  : ble_conn_id: 从设备ID, 1~BLE_CONNECT_MAX_NUM
 * @Retval : 返回从机索引
 * @Author :
 * @Data   : 2024-1-11
 * @Note   :
*/
uint8_t get_ble_index_by_slave_id(uint8_t ble_conn_id)
{
    if(ble_conn_id >= 1)
        return ble_conn_id - 1;
    else
        return 0xff;
}
/**
 * @brief 获取一个空闲的保存从机信息的索引号
 * @param None
 * @retval 成功返回索引号，失败返回-1
 */
int t_ble_get_free_slave_device_index(void)
{
    int i;

    for(i = 0; i < BLE_CONNECT_MAX_NUM; i++)
    {
        if(hBle.slave_conn[i].is_dev_configred == B_FALSE && hBle.slave_conn[i].is_dev_connected == B_FALSE)
        {
            return i;
        }
    }
    return -1;
}

/**
 * @brief 获取一个离线的从机信息的索引号
 * @param None
 * @retval 成功返回索引号，失败返回-1
 */
int t_ble_get_offline_slave_device_index(void)
{
    int i;

    for(i = 0; i < BLE_CONNECT_MAX_NUM; i++)
    {
        if(hBle.slave_conn[i].is_dev_configred == B_TRUE && hBle.slave_conn[i].is_dev_connected == B_FALSE)
        {
            return i;
        }
    }
    return -1;
}
#endif

/*****************************************************************************
* @Name	  : searchStartAndEndInCmdBackMsg
* @Brief  : 判断发送命令后的返回数据是否包含指定的起始字节和结束字节
* @Param  : src: [输入/出]
**          str_start: [输入/出]
**          str_end: [输入/出]
* @Retval :
* @Author :
* @Data   : 2022-09-26
* @Note   :
*****************************************************************************/
static uint8_t searchStartAndEndInCmdBackMsg(const char * src, const char * str_start, const char * str_end)
{
    if(((char *)strstr((char *)src, str_start) != NULL)
        && ((char *)strstr((char *)src, str_end) != NULL))
        return B_TRUE;
    else
        return B_FALSE;
}

/****************************************************************************
* 函数名：BleQLINKMsgAnalysis(uint8_t *msgbuf)
* 功  能：解析查询蓝牙组网状态的消息
* 输  入：msgbuf：蓝牙返回组网状态的消息
* 输  出：无
****************************************************************************/
static void BleQLINKMsgAnalysis(uint8_t * msgbuf)
{
    #if( LOCAL_MODULE_TYPE == _MU )
    int slave_index = 0;
    uint8_t slave_num = 0;
    uint8_t i;
    #endif
    uint8_t mac[10] = {0};
    

    BOOL_E slav_conn_stat[BLE_CONNECT_MAX_NUM];
    char * strbuf = NULL;

    memset(slav_conn_stat, 0, sizeof(slav_conn_stat));
    //更新主机连接状态
    hBle.master_conn.is_dev_connected = B_FALSE;
    strbuf = strstr((const char *)msgbuf, "m");
    if(strbuf != NULL)
    {
        hBle.master_conn.is_dev_connected = B_TRUE;
        sscanf((const char *)(strbuf + 2), "%02x%02x%02x%02x%02x%02x", (int*)&mac[0], (int*)&mac[1], (int*)&mac[2], (int*)&mac[3], (int*)&mac[4], (int*)&mac[5]);
        memcpy(hBle.master_conn.mac_addr, mac, 6);
    }

    //更新手机连接状态
    hBle.app_conn.is_dev_connected = B_FALSE;
    strbuf = strstr((const char *)msgbuf, "p");
    if(strbuf != NULL)
    {
        hBle.app_conn.is_dev_connected = B_TRUE;
        sscanf((const char *)(strbuf + 2), "%02x%02x%02x%02x%02x%02x", (int*)&mac[0], (int*)&mac[1], (int*)&mac[2], (int*)&mac[3], (int*)&mac[4], (int*)&mac[5]);
        memcpy(hBle.app_conn.mac_addr, mac, 6);
    }

    #if( LOCAL_MODULE_TYPE == _MU )
    //获取从机mac地址,记录从机连接状态
    for(i = 0; i < BLE_CONNECT_MAX_NUM; i++)
    {
        strbuf = strstr((const char *)msgbuf, "s");
        if(strbuf != NULL)
        {
            sscanf((const char *)(strbuf + 2), "%02x%02x%02x%02x%02x%02x", (int*)&mac[0], (int*)&mac[1], (int*)&mac[2], (int*)&mac[3], (int*)&mac[4], (int*)&mac[5]);
            slave_index = get_slave_dev_index_by_mac(mac);
            if(slave_index == -1)
            {
                slave_index = t_ble_get_free_slave_device_index();
                if(slave_index == -1)
                {
                    slave_index = t_ble_get_offline_slave_device_index();
                    if(slave_index == -1)
                    {
                        rtt_printf(RTT_BLE, RTT_CTRL_TEXT_BRIGHT_RED"> L%d, slave num over the limit\r\n", __LINE__);
                        break;
                    }
                }
            }
            slav_conn_stat[slave_index] = B_TRUE;
            memcpy(hBle.slave_conn[slave_index].mac_addr, mac, 6);
            slave_num ++;
            strbuf[0] = 0x20;
        }
        else
        {
            break;
        }
    }
    hBle.ble_stat.slave_connect_num = slave_num;     // 存储在线从机数量

    //更新从机的连接状态
    for(i = 0; i < BLE_CONNECT_MAX_NUM; i++)
    {
        if(slav_conn_stat[i] == B_TRUE && hBle.slave_conn[i].is_dev_connected == B_FALSE)
        {
            hBle.slave_conn[i].is_dev_connected = B_TRUE;
            hBle.slave_conn[i].disconn_event = B_FALSE;
            hBle.slave_conn[i].conn_event = B_TRUE;
            rtt_printf(RTT_BLE, RTT_CTRL_TEXT_BRIGHT_GREEN"> L%d, slave %d connected\r\n", __LINE__, i);
        }
        else if(slav_conn_stat[i] == B_FALSE && hBle.slave_conn[i].is_dev_connected == B_TRUE)
        {
            hBle.slave_conn[i].is_dev_connected = B_FALSE;
            hBle.slave_conn[i].conn_event = B_FALSE;
            hBle.slave_conn[i].disconn_event = B_TRUE;
            rtt_printf(RTT_BLE, RTT_CTRL_TEXT_BRIGHT_GREEN"> L%d, slave %d disconnected\r\n", __LINE__, i);
        }
    }
    #endif
}

/****************************************************************************
* 函数名：ucBleSetCmd(uint8_t cmdType)
* 功  能：向ble发送AT指令
* 输  入：cmdType：指令类型
* 输  出：成功/失败
****************************************************************************/
Status_t ble_cmd(uint8_t uccmd)
{
    uint8_t error_cnt = 0;  // 参数设置错误次数

    Status_t RetVal = STAT_ERROR;
    int32_t getpara = 0;
    char * pStart = NULL, * pEnd = NULL;

    char * pt = (char *)p_ble_uart->tpt_data->pt_tx_data;
    char * pr = (char *)p_ble_uart->tpt_data->pt_rx_data;

    p_ble_uart->tx_data_clr(p_ble_uart->tpt_data);

    if(ble_wait_lock(&hBle,100) != STAT_OK)
        return STAT_ERROR;
    do
    {
        switch(uccmd)
        {
            case CMD_QUERY_BLE_QLINK:      // 查询组网状态
                sprintf(pt, "%s", "AT+QLINK?\r\n");
                break;

            case CMD_QUERY_BLE_TXPWR:     // 查询蓝牙发射功率
                sprintf(pt, "%s", "AT+TXPWR?\r\n");
                break;

            case CMD_QUERY_BLE_MODE:     // 查询蓝牙模式
                sprintf(pt, "%s", "AT+MODE?\r\n");
                break;

            case CMD_QUERY_BLE_LINKKEY:  // 查询蓝牙匹配秘钥
                sprintf(pt, "%s", "AT+LINKKEY?\r\n");
                break;

            case CMD_QUERY_BLE_AUTOCONN:  // 查询自动连接状态
                sprintf(pt, "%s", "AT+AUTOCONN?\r\n");
                break;

            case CMD_QUERY_BLE_VERSION:  // 查询版本号
                sprintf(pt, "%s", "AT+VER?\r\n");
                break;

            case CMD_QUERY_BLE_RSSI:  // 查询信号接收强度
                sprintf(pt, "%s", "AT+QRSSI?\r\n");
                break;

            case CMD_SET_BLE_AUTOCONN_OPEN: // 开启自动连接
                sprintf(pt, "%s", "AT+AUTOCONN=ON\r\n");
                break;

            case CMD_SET_BLE_AUTOCONN_CLOSE:  // 关闭自动连接
                sprintf(pt, "%s", "AT+AUTOCONN=OFF\r\n");
                break;

            case CMD_SET_BLE_RESET:      // 软件重启
                sprintf(pt, "%s", "AT+RESET\r\n");
                break;

            case CMD_SET_BLE_TXPWR:       // 设置蓝牙发射功率
                sprintf(pt, "AT+TXPWR=%d\r\n", hBle.module_config.ble_tx_pow);
                break;

            case CMD_SET_BLE_MODE:       // 设置蓝牙模式
                if(hBle.module_config.mode == T_BLE_MODE_MS)
                {
                    sprintf(pt, "%s", "AT+MODE=3\r\n");
                }
                else if(hBle.module_config.mode == T_BLE_MODE_M)
                {
                    sprintf(pt, "%s", "AT+MODE=1\r\n");
                }
                else if(hBle.module_config.mode == T_BLE_MODE_S)
                {
                    sprintf(pt, "%s", "AT+MODE=2\r\n");
                }
                break;

            case CMD_SET_BLE_LINKKEY: // 设置蓝牙匹配秘钥
                sprintf(pt, "AT+LINKKEY=%x\r\n", hBle.module_config.linkkey);
                break;

            case CMD_SET_BLE_APP: // 设置蓝牙APP模式
                if(hBle.module_config.autoconn_enable == OPEN)
                {
                    sprintf(pt, "%s", "AT+APP=ON\r\n");
                }
                else if(hBle.module_config.autoconn_enable == CLOSE)
                {
                    sprintf(pt, "%s", "AT+APP=OFF\r\n");
                }
                break;

            default:
                ble_unlock(&hBle);
                return STAT_ERROR;
        }

        //清缓冲区
        xQueueReceive(ble_cmd_queue, &ble_msg_data, (TickType_t)pdMS_TO_TICKS(0));
        memset(&ble_msg_data, 0, sizeof(ble_msg_data));
        //发数据
        p_ble_uart->send(p_ble_uart->tpt_data, strlen((const char *)p_ble_uart->tpt_data->pt_tx_data));

        rtt_printf(RTT_BLE, RTT_CTRL_TEXT_BRIGHT_GREEN"> L%d, cnt = %d, mcu tx ble: %s\r\n", __LINE__, error_cnt, p_ble_uart->tpt_data->pt_tx_data);

        if(xQueueReceive(ble_cmd_queue, &ble_msg_data, (TickType_t)pdMS_TO_TICKS(500)))
        {
            rtt_printf(RTT_BLE, RTT_CTRL_TEXT_BRIGHT_GREEN"> L%d, mcu rx ble: %s\r\n", __LINE__, ble_msg_data.data);
            pr = (char *)ble_msg_data.data;
            switch(uccmd)
            {
                case CMD_QUERY_BLE_QLINK:      // 查询组网状态
                    if(B_TRUE == searchStartAndEndInCmdBackMsg((const char *)pr, "+QLINK:", "OK"))
                    {
                        RetVal = STAT_OK;
                        BleQLINKMsgAnalysis((uint8_t *)pr);
                    }
                    else
                        RetVal = STAT_ERROR;
                    break;

                case CMD_QUERY_BLE_TXPWR:     // 查询蓝牙发射功率
                    if(B_TRUE == searchStartAndEndInCmdBackMsg((const char *)pr, "+TXPWR:", "OK"))
                    {
                        RetVal = STAT_OK;
                        sscanf((const char *)pr, "%*[^:]:%d", &getpara);
                        hBle.module_config.ble_tx_pow = getpara;
                    }
                    else
                        RetVal = STAT_ERROR;
                    break;

                case CMD_QUERY_BLE_MODE:     // 查询蓝牙模式
                    if(B_TRUE == searchStartAndEndInCmdBackMsg((const char *)pr, "+MODE:", "OK"))
                    {
                        RetVal = STAT_OK;
                        sscanf((const char *)pr, "%*[^:]:%d", &getpara);
                        if(getpara == 1)
                        {
                            hBle.module_config.mode = T_BLE_MODE_M;
                        }
                        else if(getpara == 2)
                        {
                            hBle.module_config.mode = T_BLE_MODE_S;
                        }
                        else if(getpara == 3)
                        {
                            hBle.module_config.mode = T_BLE_MODE_MS;
                        }
                    }
                    else
                    {
                        RetVal = STAT_ERROR;
                    }
                    break;

                case CMD_QUERY_BLE_LINKKEY:  // 查询蓝牙匹配秘钥
                    if(B_TRUE == searchStartAndEndInCmdBackMsg((const char *)pr, "+LINKKEY:", "OK"))
                    {
                        RetVal = STAT_OK;
                        sscanf((const char *)pr, "%*[^:]:%d", &getpara);
                        hBle.module_config.linkkey = getpara;
                    }
                    else
                        RetVal = STAT_ERROR;
                    break;

                case CMD_QUERY_BLE_AUTOCONN:  // 查询自动连接状态
                    if(B_TRUE == searchStartAndEndInCmdBackMsg((const char *)pr, "+AUTOCONN:", "OK"))
                    {
                        if(strstr((const char *)pr, "ON") != NULL)
                        {
                            hBle.module_config.autoconn_enable = OPEN;
                        }
                        else if(strstr((const char *)pr, "OFF") != NULL)
                        {
                            hBle.module_config.autoconn_enable = CLOSE;
                        }
                        RetVal = STAT_OK;
                    }
                    else
                        RetVal = STAT_ERROR;
                    break;

                case CMD_QUERY_BLE_VERSION:  // 查询版本号
                    if(B_TRUE == searchStartAndEndInCmdBackMsg((const char *)pr, "+VER:", "OK"))
                    {
                        RetVal = STAT_OK;
                        sscanf((const char *)pr, "%*[^:]:%d", &hBle.module_config.version);
                    }
                    else
                        RetVal = STAT_ERROR;
                    break;

                case CMD_QUERY_BLE_RSSI:  // 查询信号接收强度
                    pStart = strstr((const char *)pr, "+QRSSI:");
                    pEnd = strstr((const char *)pr, "OK");
                    if((pStart != NULL) && (pEnd != NULL))
                    {
                        RetVal = STAT_OK;
                        sscanf((const char *)pStart, "%*[^:]:%d", &getpara);
                        hBle.ble_stat.rssi = getpara;
                    }
                    else if((char *)strstr(pr, "error:07") != NULL)
                    {
                        RetVal = STAT_OK;
                    }
                    else
                    {
                        RetVal = STAT_ERROR;
                    }
                    break;

                case CMD_SET_BLE_AUTOCONN_OPEN: // 开启自动连接
                    if(B_TRUE == searchStartAndEndInCmdBackMsg((const char *)pr, "+AUTOCONN=", "OK"))
                    {
                        hBle.module_config.autoconn_enable = ENABLE;
                        RetVal = STAT_OK;
                    }
                    else
                        RetVal = STAT_ERROR;
                    break;

                case CMD_SET_BLE_AUTOCONN_CLOSE:  // 关闭自动连接
                    if(B_TRUE == searchStartAndEndInCmdBackMsg((const char *)pr, "+AUTOCONN=", "OK"))
                    {
                        hBle.module_config.autoconn_enable = DISABLE;
                        RetVal = STAT_OK;
                    }
                    else
                        RetVal = STAT_ERROR;
                    break;

                case CMD_SET_BLE_RESET:      // 复位
                    RetVal = STAT_OK;
                    break;

                case CMD_SET_BLE_TXPWR:       // 设置蓝牙发射功率
                    if(B_TRUE == searchStartAndEndInCmdBackMsg((const char *)pr, "+TXPWR=", "OK"))
                    {
                        RetVal = STAT_OK;
                    }
                    else
                        RetVal = STAT_ERROR;
                    break;

                case CMD_SET_BLE_MODE:       // 设置蓝牙模式
                    if(B_TRUE == searchStartAndEndInCmdBackMsg((const char *)pr, "+MODE=", "OK"))
                    {
                        RetVal = STAT_OK;
                    }
                    else
                        RetVal = STAT_ERROR;
                    break;
                case CMD_SET_BLE_LINKKEY: // 设置蓝牙匹配秘钥
                    if(B_TRUE == searchStartAndEndInCmdBackMsg((const char *)pr, "+LINKKEY=", "OK"))
                    {
                        RetVal = STAT_OK;
                    }
                    else
                        RetVal = STAT_ERROR;
                    break;

                case CMD_SET_BLE_APP: // 设置蓝牙APP模式

                    break;

                default:
                    error_cnt++;
                    break;
            }
        }
        else
        {
            rtt_printf(RTT_BLE, RTT_CTRL_TEXT_BRIGHT_GREEN"> L%d, ble uart rx no msg, resend = %d\r\n", __LINE__, error_cnt);
        }

        if(RetVal == STAT_OK)
            break;

        //if( RetVal == B_FALSE )
        //    error_cnt++;

    } while(0);   // 连续2次设置失败，则直接返回错误

    ble_unlock(&hBle);
    return RetVal;
}

/*****************************************************************************
* @Name	  : ble_enable
* @Brief  : 开启自组网功能
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2023-03-11
* @Note   :
*****************************************************************************/
void ble_set_conn_en(uint8_t enable)
{
    if(enable == DISABLE)
    {
        ble_cmd(CMD_SET_BLE_AUTOCONN_CLOSE);    // 关闭自组网功能
    }
    else
    {
        ble_cmd(CMD_SET_BLE_AUTOCONN_OPEN);     // 开启自组网功能
    }
}

/*****************************************************************************
* @Name   : ble_get_version
* @Brief  : 获取蓝牙版本
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2023-09-23
* @Note   :
*****************************************************************************/
uint32_t ble_get_version(void)
{
    return hBle.module_config.version;
}

/**************************************************************************
* 函 数 名：uint8_t ble_msg_send_by_mac(uint8_t *msgbuf, uint16_t usMsgLen)
* 功能描述：蓝牙透传数据发送,根据mac地址发送到主机或从机
* 参数说明：msgbuf:消息包 usMsgLen:消息长度 mac_addr:从设备地址
* 函数输出：成功/失败
**************************************************************************/
Status_t ble_msg_send_by_mac(uint8_t * msgbuf, uint16_t usMsgLen, uint8_t * mac_addr)
{
    Status_t RetVal = STAT_ERROR;
    uint16_t index = 0;

    char * pt = (char *)p_ble_uart->tpt_data->pt_tx_data;

    if(ble_wait_lock(&hBle,100) != STAT_OK)
        return STAT_ERROR;

    xQueueReceive(ble_cmd_queue, &ble_msg_data, (TickType_t)pdMS_TO_TICKS(1));
    p_ble_uart->tx_data_clr(p_ble_uart->tpt_data);

    index = sprintf(pt, "AT+TX=%02x%02x%02x%02x%02x%02x,%d,", mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5], usMsgLen);    // 透传组包
    index += t_algo_hex_2_ascii(msgbuf, pt + index, usMsgLen);
    pt[index] = '\r';
    pt[index + 1] = '\n';

    rtt_printf(RTT_BLE, "ucaUartTxData Len %d: %s\r\n", strlen(pt), pt);
    p_ble_uart->send(p_ble_uart->tpt_data, strlen((const char *)pt));

    memset(&ble_msg_data, 0, sizeof(ble_msg_data));
    
    if(xQueueReceive(ble_cmd_queue, &ble_msg_data, (TickType_t)pdMS_TO_TICKS(500)))
        RetVal = STAT_OK;

    ble_unlock(&hBle);
    return RetVal;
}

#if( LOCAL_MODULE_TYPE == _MU )
/*****************************************************************************
 * @Name   : ble_slave_msg_send_by_id
 * @Brief  : 发送自组网透传数据到从机
 * @Retval : None
 * @Note   :
 */
Status_t ble_slave_msg_send_by_id(uint8_t *msgbuf, uint16_t usMsgLen, uint8_t ble_conn_id)
{
    if(ble_conn_id > BLE_CONNECT_MAX_NUM || hBle.slave_conn[ble_conn_id - 1].is_dev_connected == B_FALSE)
        return STAT_ERROR;

    if(BLE_TXRX_MAX_LEN < usMsgLen || usMsgLen == 0 || msgbuf == NULL)
        return STAT_ERROR;
    return ble_msg_send_by_mac(msgbuf, usMsgLen, hBle.slave_conn[ble_conn_id - 1].mac_addr);
}
/*****************************************************************************
 * @Name	  : ble_broadcast_msg_send
 * @Brief  : 主机发送自组网广播数据至所有从机
 * @Retval : None
 * @Note   :
 */
Status_t ble_broadcast_msg_send(uint8_t * msgbuf, uint16_t usMsgLen)
{
    Status_t RetVal = STAT_ERROR;
    uint16_t index = 0;
    char * pt = (char *)p_ble_uart->tpt_data->pt_tx_data;

    if(BLE_TXRX_MAX_LEN < usMsgLen || usMsgLen == 0 || msgbuf == NULL)
        return STAT_ERROR;

    if(ble_wait_lock(&hBle,100) != STAT_OK)
        return STAT_ERROR;
    xQueueReceive(ble_cmd_queue, &ble_msg_data, (TickType_t)pdMS_TO_TICKS(1));
    p_ble_uart->tx_data_clr(p_ble_uart->tpt_data);
    
    index = sprintf(pt, "AT+BC=,%d,", usMsgLen);    // 透传组包
    index += t_algo_hex_2_ascii(msgbuf, pt + index, usMsgLen);
    pt[index] = '\r';
    pt[index + 1] = '\n'; 
    rtt_printf(RTT_BLE, "ucaUartTxData Len %d: %s\r\n", strlen(pt), pt);

    p_ble_uart->send(p_ble_uart->tpt_data, strlen((const char *)pt));

    memset(&ble_msg_data, 0, sizeof(ble_msg_data));
    if(xQueueReceive(ble_cmd_queue, &ble_msg_data, (TickType_t)pdMS_TO_TICKS(500)))
        RetVal = STAT_OK;
    ble_unlock(&hBle);
    return RetVal;
}
#endif

/*****************************************************************************
 * @Name	  : ble_master_msg_send
 * @Brief  : 发送自组网透传数据到主机
 * @param  : msgbuf:消息包hex格式 usMsgLen:消息长度,hex格式
 * @Retval : None
 * @Note   :
 */
Status_t ble_master_msg_send(uint8_t * msgbuf, uint16_t usMsgLen)
{
    Status_t RetVal = STAT_ERROR;
    uint16_t index = 0;
    char * pt = (char *)p_ble_uart->tpt_data->pt_tx_data;

    if(BLE_TXRX_MAX_LEN < usMsgLen || usMsgLen == 0 || msgbuf == NULL)
        return STAT_ERROR;

    if(ble_wait_lock(&hBle,100) != STAT_OK)
        return STAT_ERROR;
    xQueueReceive(ble_cmd_queue, &ble_msg_data, (TickType_t)pdMS_TO_TICKS(1));
    p_ble_uart->tx_data_clr(p_ble_uart->tpt_data);

    if(hBle.master_conn.is_dev_connected == B_TRUE)
    {
        index = sprintf(pt, "AT+TX=%02x%02x%02x%02x%02x%02x,%d,", hBle.master_conn.mac_addr[0], hBle.master_conn.mac_addr[1], hBle.master_conn.mac_addr[2], hBle.master_conn.mac_addr[3], hBle.master_conn.mac_addr[4], hBle.master_conn.mac_addr[5], usMsgLen);    // 透传组包
        index += t_algo_hex_2_ascii(msgbuf, pt + index, usMsgLen);
        pt[index] = '\r';
        pt[index + 1] = '\n'; 
    }

    rtt_printf(RTT_BLE, "ucaUartTxData Len %d: %s\r\n", strlen(pt), pt);

    p_ble_uart->send(p_ble_uart->tpt_data, strlen((const char *)pt));

    memset(&ble_msg_data, 0, sizeof(ble_msg_data));
    if(xQueueReceive(ble_cmd_queue, &ble_msg_data, (TickType_t)pdMS_TO_TICKS(500)))
        RetVal = STAT_OK;
    ble_unlock(&hBle);
    return RetVal;
}

/*****************************************************************************
 * @Name	  : ble_app_msg_send
 * @Brief  : 发送自组网透传数据到手机app
 * @Retval : None
 * @Note   :
 */
Status_t ble_app_msg_send(uint8_t * msgbuf, uint16_t usMsgLen)
{
    Status_t RetVal = STAT_ERROR;
    uint16_t index = 0;

    char * pt = (char *)p_ble_uart->tpt_data->pt_tx_data;

    if(BLE_TXRX_MAX_LEN < usMsgLen || usMsgLen == 0 || msgbuf == NULL)
        return STAT_ERROR;

    if(ble_wait_lock(&hBle,100) != STAT_OK)
        return STAT_ERROR;
    //清缓存
    xQueueReceive(ble_cmd_queue, &ble_msg_data, (TickType_t)pdMS_TO_TICKS(0));
    p_ble_uart->tx_data_clr(p_ble_uart->tpt_data);

    // 手机消息组包
    if(hBle.app_conn.is_dev_connected == B_TRUE)
    {
        index = sprintf(pt, "AT+TXP=%d,", usMsgLen);
        index += t_algo_hex_2_ascii(msgbuf, pt + index, usMsgLen);
        pt[index] = '\r';
        pt[index + 1] = '\n'; 
    }

    rtt_printf(RTT_BLE, "ucaUartTxData Len %d: %s\r\n", strlen(pt), pt);

    p_ble_uart->send(p_ble_uart->tpt_data, strlen((const char *)pt));

    memset(&ble_msg_data, 0, sizeof(ble_msg_data));
    if(xQueueReceive(ble_cmd_queue, &ble_msg_data, (TickType_t)pdMS_TO_TICKS(500)))
        RetVal = STAT_OK;
    ble_unlock(&hBle);
    return RetVal;
}

/*****************************************************************************
* @Name	  : vBleMsgAnalysis
* @Brief  : 蓝牙消息解析
* @Param  : rx_buf 出参,存入接收的数据
* @Retval : data length
* @Author :
* @Data   : 2023-04-10
* @Note   :
*****************************************************************************/
uint16_t ble_msg_receive(T_BleData * rx_buf)
{
    if(rx_buf == NULL || ble_msg_queue == NULL)
        return 0;
    if(xQueueReceive(ble_msg_queue, rx_buf, (TickType_t)pdMS_TO_TICKS(0)))
        return rx_buf->len;
    else
        return 0;
}


/*****************************************************************************
* @Name	  : ble_uart_rx_data_handle
* @Brief  : 所有ble串口收包在这里统一分类,将收到的消息分为AT指令回包和推送消息并分别存储
            ,建议放在一个调度周期不高于50ms的任务中,不要和ble run在同一个任务
* @Retval : None
* @Author :
* @Date   :
* @Note   :
*/
void ble_uart_rx_data_handle(void)
{
    uint16_t bleRxLen = 0;
    uint32_t newusMsgLen = 0;
    char role = 0;
    char * pStart = NULL;
    char * pEnd = NULL;
    uint8_t mac[10] = {0};
    uint8_t msgData[550] = {0};
    #if( LOCAL_MODULE_TYPE == _MU )
    int slave_index = 0;
    #endif

    if(hBle.queue_create == B_FALSE)
        return;

    if(p_ble_uart->get_newmsg(p_ble_uart->tpt_data) == B_TRUE)  // 串口有无新数据包
    {
        ble_rx_data_clear();
        bleRxLen = p_ble_uart->read(p_ble_uart->tpt_data);
        p_ble_uart->clr_newmsg(p_ble_uart->tpt_data);
    }
    else
        return;
    rtt_printf(RTT_BLE, "ble uart RX Msg:len = %d, %s", bleRxLen, p_rx_buf);

    if(strstr((char *)p_rx_buf, "+RX=") != NULL)  // 检查并处理ble收到的透传数据
    {
        pStart = strstr((char *)p_rx_buf, "+RX=");
        pEnd = strstr((const char *)pStart, "\r\n"); // 首个\r\n的位置

        if(pStart != NULL && pEnd != NULL)
        {
            if(sscanf((const char *)(pStart + 4), "%02x%02x%02x%02x%02x%02x,%d,%s", (int*)&mac[0], (int*)&mac[1], (int*)&mac[2], (int*)&mac[3], (int*)&mac[4], (int*)&mac[5], &newusMsgLen, msgData) != 8)
            {
                rtt_printf(RTT_BLE, "Ble msg receive error! Data format error!\r\n");
                return;
            }

            if(strlen((char *)msgData) != newusMsgLen * 2)
            {
                rtt_printf(RTT_BLE, "Ble msg receive error! Data len is not match!\r\n");
                return;
            }

            memset(&ble_msg_data, 0, sizeof(ble_msg_data));
            if(t_algo_ascii_2_hex(msgData, (uint8_t *)ble_msg_data.data, strlen((char *)msgData)) != -1)
            {
                memcpy(ble_msg_data.ble_mac, mac, 6);
                ble_msg_data.len = newusMsgLen;
                ble_msg_data.msg_type = MSG_DATA;
                xQueueSend(ble_msg_queue, &ble_msg_data, (TickType_t)0);
            }
        }
    }
    else if(strstr((char *)p_rx_buf, "+RXP=") != NULL) // 检查并处理ble收到的APP消息
    {
        pStart = strstr((char *)p_rx_buf, "+RXP=");
        pEnd = strstr((const char *)pStart, "\r\n"); // 首个\r\n的位置

        if(pStart != NULL && pEnd != NULL)
        {
            if(sscanf((const char *)(pStart + 5), "%02x%02x%02x%02x%02x%02x,%d,%s", (int*)&mac[0], (int*)&mac[1], (int*)&mac[2], (int*)&mac[3], (int*)&mac[4], (int*)&mac[5], &newusMsgLen, msgData) != 8)
            {
                rtt_printf(RTT_BLE, "Ble app msg receive error! Data format error!\r\n");
                return;
            }

            if(strlen((char *)msgData) != newusMsgLen * 2)
            {
                rtt_printf(RTT_BLE, "Ble app msg receive error! Data len is not match!\r\n");
                return;
            }

            memset(&ble_msg_data, 0, sizeof(ble_msg_data));
            if(t_algo_ascii_2_hex(msgData, (uint8_t *)ble_msg_data.data, strlen((char *)msgData)) != -1)
            {
                memcpy(ble_msg_data.ble_mac, mac, 6);
                ble_msg_data.len = newusMsgLen;
                ble_msg_data.msg_type = MSG_APP;
                xQueueSend(ble_msg_queue, &ble_msg_data, (TickType_t)0);
            }
        }
    }
    else if(strstr((char *)p_rx_buf, "+CONNECT=") != NULL) // 检查并处理ble收到连接状态推送
    {
        pStart = strstr((char *)p_rx_buf, "+CONNECT=");
        pEnd = strstr((const char *)pStart, "\r\n"); // 首个\r\n的位置

        if(pStart != NULL && pEnd != NULL)
        {
            if(sscanf((const char *)(pStart), "+CONNECT=%c,%02x%02x%02x%02x%02x%02x", &role, (int*)&mac[0], (int*)&mac[1], (int*)&mac[2], (int*)&mac[3], (int*)&mac[4], (int*)&mac[5]) != 7)
            {
                rtt_printf(RTT_BLE, "Ble app msg receive error! Data format error!\r\n");
                return;
            }

            if(role == 'm')
            {
                hBle.master_conn.is_dev_connected = B_TRUE;
                memcpy(hBle.master_conn.mac_addr, mac, 6);
                rtt_printf(RTT_BLE, "Ble master connected!\r\n");
                return;
            }
            else if(role == 'p')
            {
                hBle.app_conn.is_dev_connected = B_TRUE;
                memcpy(hBle.app_conn.mac_addr, mac, 6);
                rtt_printf(RTT_BLE, "Ble app connected!\r\n");
                return;
            }
            #if( LOCAL_MODULE_TYPE == _MU )
            else if(role == 's')
            {
                slave_index = get_slave_dev_index_by_mac(mac);
                if(slave_index == -1)
                {
                    slave_index = t_ble_get_free_slave_device_index();
                    if(slave_index == -1)
                    {
                        slave_index = t_ble_get_offline_slave_device_index();
                        if(slave_index == -1)
                        {
                            rtt_printf(RTT_BLE, RTT_CTRL_TEXT_BRIGHT_RED"ble slave num over the limit\r\n");
                            return;
                        }
                    }
                }
                hBle.slave_conn[slave_index].is_dev_connected = B_TRUE;
                hBle.slave_conn[slave_index].disconn_event = B_FALSE;
                hBle.slave_conn[slave_index].conn_event = B_TRUE;
                hBle.ble_stat.slave_connect_num++;
                memcpy(hBle.slave_conn[slave_index].mac_addr, mac, 6);
                rtt_printf(RTT_BLE, "Ble slave connected!\r\n");
            }
            #endif
        }
    }
    else if(strstr((char *)p_rx_buf, "+DISCONNECT=") != NULL) // 检查并处理ble收到的断连状态推送
    {
        pStart = strstr((char *)p_rx_buf, "+DISCONNECT=");
        pEnd = strstr((const char *)pStart, "\r\n"); // 首个\r\n的位置

        if(pStart != NULL && pEnd != NULL)
        {
            if(sscanf((const char *)(pStart), "+DISCONNECT=%c,%02x%02x%02x%02x%02x%02x", &role, (int*)&mac[0], (int*)&mac[1], (int*)&mac[2], (int*)&mac[3], (int*)&mac[4], (int*)&mac[5]) != 7)
            {
                rtt_printf(RTT_BLE, "Ble app msg receive error! Data format error!\r\n");
                return;
            }

            if(role == 'm')
            {
                hBle.master_conn.is_dev_connected = B_FALSE;
                memset(hBle.master_conn.mac_addr, 0, 6);
                rtt_printf(RTT_BLE, "Ble master disconnected!\r\n");
                return;
            }
            else if(role == 'p')
            {
                hBle.app_conn.is_dev_connected = B_FALSE;
                memset(hBle.app_conn.mac_addr, 0, 6);
                rtt_printf(RTT_BLE, "Ble app disconnected!\r\n");
                return;
            }
            #if( LOCAL_MODULE_TYPE == _MU )
            else if(role == 's')
            {
                slave_index = get_slave_dev_index_by_mac(mac);
                if(slave_index == -1)
                {
                    rtt_printf(RTT_BLE, RTT_CTRL_TEXT_BRIGHT_RED"ble slave mac not found\r\n");
                    return;
                }
                hBle.slave_conn[slave_index].is_dev_connected = B_FALSE;
                hBle.slave_conn[slave_index].conn_event = B_FALSE;
                hBle.slave_conn[slave_index].disconn_event = B_TRUE;
                hBle.ble_stat.slave_connect_num--;
                rtt_printf(RTT_BLE, "Ble slave disconnected!\r\n");
            }
            #endif
        }
    }
    else// 检查并处理ble模组的cmd回包
    {
        memset(&ble_msg_data, 0, sizeof(ble_msg_data));
        if(bleRxLen <= sizeof(ble_msg_data.data))
        {
            memcpy(ble_msg_data.data, p_rx_buf, bleRxLen);
            ble_msg_data.len = bleRxLen;
            ble_msg_data.msg_type = MSG_RX_CMD;
            xQueueSend(ble_cmd_queue, &ble_msg_data, (TickType_t)0);
        }
        else
        {
            rtt_printf(RTT_BLE, "Ble msg receive error! Data len is too long!\r\n");
        }
    }
}


/*****************************************************************************
* @Name	  : ble_init
* @Brief  : 端口初始化
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2023-11-22
* @Note   :
*****************************************************************************/
void ble_init(void)
{
    ble_queue_create();
    hBle.queue_create = B_TRUE;
    ble_rst();
    vTaskDelay(500);

    hBle.module_config.autoconn_enable = 1;
    hBle.module_config.ble_tx_pow = 0;
    #if LOCAL_MODULE_TYPE == _MU
    hBle.module_config.mode = T_BLE_MODE_M;
    #else
    hBle.module_config.mode = T_BLE_MODE_S;
    #endif
    hBle.module_config.autoconn_enable = OPEN;
    hBle.module_config.linkkey = BLE_KEY_3521;
    
    ble_set_conn_en(ENABLE);
    ble_cmd(CMD_SET_BLE_MODE);
    ble_cmd(CMD_SET_BLE_TXPWR);
    ble_cmd(CMD_SET_BLE_LINKKEY);
    ble_cmd(CMD_SET_BLE_APP);

    rtt_printf(RTT_BLE, "ble init done\r\n");
}
