/***************************** Copyright ****************************************
 *
 * (C) Copyright 2023, Shaanxi Tianji Communication
 *  All Rights Reserved
 *
 * FileName   : mcpB_user.c
 * Version    : none
 * Author     : none
 * Date       : 2024-2-20
 * Description: MCP:B协议解析及业务实现
 *******************************************************************************/
#include "monitor_para_list.h"
#include "bsp_dev_para.h"
#include "bsp_config.h"
#include "mcp_core.h"
#include "mcpMRC_user.h"
#include "ascii_hex.h"
#include "bsp_reboot.h"
#include "app_version.h"
#include "app_upgrade.h"
#include "crc.h"
/*---------------------- 宏定义  ---------------------------------------*/

/*---------------------- 结构体 ----------------------------------------*/

/*---------------------- 常量 ------------------------------------------*/

/*---------------------- 全局变量 --------------------------------------*/
uint32_t g_ulVerUpgradePackageSeq=0;                      /*升级包的编号*/
uint32_t g_ulVerUpgradePackageSeqBk=0;                    /*升级包的编号*/
uint8_t  g_ucVerUpgradeFlag = 0;                            /*固件升级标记位*/
static uint8_t  g_ucVerUpgradeStatus = EN_VER_UPGRADE_SUCCESS; /*当前固件升级状态*/
static uint8_t  g_ucVerFileTransEnd;

/*---------------------- 声明 ------------------------------------------*/


/*****************************************************************************
* @Name	  :  get_mcpB_msg_info
* @Brief  :  接收监控信息初始化
* @Param  :  mcp_msg: 
**           rx_buf: 
**           rx_buf_len: 
* @Retval :  无
* @Author :
* @Data   :  2024-02-26
* @Note   :  
*****************************************************************************/
void get_mcpB_msg_info(MCP_MSG_T *mcp_msg, uint8_t *rx_buf, uint16_t rx_buf_len, uint8_t msg_src)
{
    uint16_t temp = 0;
    memset(mcp_msg, 0, sizeof(MCP_MSG_T));

    /*包头参数提取*/ 
    mcp_msg->start = *(rx_buf);
    mcp_msg->ap_proto_type = *(rx_buf + ADDR_AP_PROTO_TYPE);
    mcp_msg->vp_proto_type = *(rx_buf + ADDR_VP_PROTO_TYPE);
    mcp_msg->site_id[0] = *(rx_buf + ADDR_VP_SITE_ID + 3);
    mcp_msg->site_id[1] = *(rx_buf + ADDR_VP_SITE_ID + 2);
    mcp_msg->site_id[2] = *(rx_buf + ADDR_VP_SITE_ID + 1);
    mcp_msg->site_id[3] = *(rx_buf + ADDR_VP_SITE_ID);
    mcp_msg->dev_id = *(rx_buf + ADDR_VP_DEVICE_ID);
    temp = *(rx_buf + ADDR_VP_PKT_NUM + 1);
    temp = (temp << 8) + *(rx_buf + ADDR_VP_PKT_NUM);
    mcp_msg->vp_pack_num = temp;
    mcp_msg->vp_sign = *(rx_buf + ADDR_VP_SIGN);
    mcp_msg->mcp_type = *(rx_buf + ADDR_MCP_PROTO_TYPE);
    mcp_msg->cmd_type = *(rx_buf + ADDR_MCP_CMD_TYPE);
    mcp_msg->mcp_resp_sign = *(rx_buf + ADDR_MCP_RESP_SIGN);
    mcp_msg->data_len = *(rx_buf + ADDR_MCP_DATA_FIELD);
    mcp_msg->rx_buf_len = rx_buf_len;
    mcp_msg->tx_buf_len = ADDR_MCP_DATA_FIELD;

    /*指向源数据中回包时需要修改的部分,用于填充回复标志等*/ 
    mcp_msg->ptr_src_buf = rx_buf;
    mcp_msg->ptr_site_resp = rx_buf + ADDR_VP_SITE_ID;
    mcp_msg->ptr_vp_resp = rx_buf + ADDR_VP_SIGN;
    mcp_msg->ptr_mcp_resp = (rx_buf + ADDR_MCP_RESP_SIGN);

    /*指向源数据中MCP数据块的起始位置,用于解析数据块*/ 
    mcp_msg->ptr_data_unit = rx_buf + ADDR_MCP_DATA_FIELD;
    mcp_msg->ptr_mcp_data_code = rx_buf + ADDR_MCP_DATA_FIELD + OFFSET_MCPB_DATA_CODE;

    mcp_msg->msg_src = msg_src;
}

/*****************************************************************************
* @Name	  :  mcpB_parase_msg_local_dev
* @Brief  :  mcpB协议解析及处理
* @Param  :  mcp_msg: 指向源数据
* @Retval :  执行结果
* @Author :
* @Data   :  2024-02-26
* @Note   :  
*****************************************************************************/
uint8_t mcpB_parase_msg_local_dev(MCP_MSG_T *mcp_msg)
{
    uint8_t ret = MCP_RESP_SIGN_OK, err_flag = MCP_RESP_SIGN_OK; /*错误标志*/  
    uint16_t data_id;                                            /*数据标识符*/ 
    uint16_t Num = ADDR_MCP_DATA_FIELD;                          /*有效的监控标识前有14个字节的数据*/ 
    uint16_t data_block_len = 0;   /*数据块长度*/ 
    uint8_t *pt_data;              /*指向数据*/ 
	uint32_t ulUpgradeSeq;

    while ((Num + OFFSET_MCPB_DATA) < mcp_msg->rx_buf_len)
    {
        data_block_len = ((*mcp_msg->ptr_data_unit) & 0x00FF) | ((*(mcp_msg->ptr_data_unit + 1) << 8) & 0xFF00);/*低字节在前，高字节在后*/
        data_id = (*(mcp_msg->ptr_data_unit + 2) & 0x00FF) | ((*(mcp_msg->ptr_data_unit + 3) << 8) & 0xFF00);/*低字节在前，高字节在后*/
        pt_data = mcp_msg->ptr_data_unit + OFFSET_MCPB_DATA; 
				
        /*rtt_printf(RTT_UPGRADE, RTT_TEXT_RED"mcpB_parase_msg_local_dev: mcp_type=%d    data_id=%x    data_block_len=%d\r\n", mcp_msg->cmd_type, data_id, data_block_len);*/	
		
        if(mcp_msg->cmd_type == MCP_CMD_TYPE_QUERY)		
        {
            switch(data_id)
            {
                case MCP_ID_MCP_MODE_B: /*设备监控软件运行模式*/
                    {
                        pt_data[0] = 0x01;
                    }
                    break;
            
                case MCP_ID_OTA_PACKET_NUM:/*下一个文件数据块序号*/
                    break;
            
                case MCP_ID_OTA_PACKET_LEN:/*数据块长度*/
                    break;
            
                case MCP_ID_OTA_FILE_CRC:   /*文件标识码*/
                    break;
            
                case MCP_ID_OTA_TRANS_CTRL: /*文件传输控制*/
                    break;
            
                case MCP_ID_OTA_PACK_ACK:/*文件数据包应答*/
                    break;
            
                case MCP_ID_OTA_PACK_NUM:   /*文件数据块序号*/
                    break;
            
                case MCP_ID_OTA_PACK:  /*文件数据块*/
                    break;
                
                case MCP_ID_MCP_B_FILE_VER:   /*当前待升级软件版本*/
                    {
                        pt_data[0] = 0;
                        pt_data[1] = 0;
                    }					
                    break;
            
                default:
                    break;
            }        	
        }
        else if(mcp_msg->cmd_type == MCP_CMD_TYPE_SET)		
        {
            switch(data_id)
            {
                case MCP_ID_MCP_MODE_B:
                    break;
            
                case MCP_ID_OTA_PACKET_NUM:
                    {
                        if(0 != g_ulVerUpgradePackageSeq)
                        {
                            ulUpgradeSeq = g_ulVerUpgradePackageSeq + 1;
                            memcpy(pt_data, &ulUpgradeSeq, 4);
                        }					
                    }
                    break;
            
                case MCP_ID_OTA_PACKET_LEN:
                    {
                        pt_data[0] = VER_UPGRADE_PACKET_SIZE & 0xFF;
                        pt_data[1] = (VER_UPGRADE_PACKET_SIZE >> 8) & 0xFF;
                    }
                    break;
            
                case MCP_ID_OTA_FILE_CRC:
                    break;
            
                case MCP_ID_OTA_TRANS_CTRL:   /*文件传输控制*/
                    {
						rtt_printf(RTT_UPGRADE, RTT_TEXT_YELLOW"--->> : MCP_ID_OTA_TRANS_CTRL %d\r\n", pt_data[0]);
						
                        if(MCPB_TRANS_CTRL_START == pt_data[0])   /*表示文件传输开始*/
                        {
							/*if(g_ucVerUpgradeFlag != 1)*/
							{
                                g_ucVerUpgradeStatus = ver_FileTransInit();
                                ver_UpgradeTimeOutCtrl(ENABLE);/*开启升级超时检测*/
                                g_ucVerUpgradeFlag = 1;        /*切换为升级模式*/
                                g_ulVerUpgradePackageSeqBk = 0;
                                g_ulVerUpgradePackageSeq = 0;
							    g_ucVerFileTransEnd = 0;
							}
                        }
                        else if(MCPB_TRANS_CTRL_END == pt_data[0])/*表示文件传输结束*/
                        {
                            if(g_ucVerUpgradeStatus == EN_VER_UPGRADE_SUCCESS)
                            {
								if(g_ucVerFileTransEnd != 1)/*避免收到重复消息重复处理*/
								{
									g_ucVerFileTransEnd = 1;
                                	g_ucVerUpgradeStatus = ver_FileTransEnd();
                                	if(g_ucVerUpgradeStatus == EN_VER_UPGRADE_SUCCESS)
										rtt_printf(RTT_UPGRADE, RTT_TEXT_YELLOW"--->> : update file ok\r\n");
                                	else
                                	    rtt_printf(RTT_UPGRADE, RTT_TEXT_RED"--->> : update file %s ERROR!!!\r\n", (g_ucVerUpgradeStatus == EN_VER_UPGRADE_FILE_CRC_FAIL) ? "CRC32" : "write end");
								}
                            }
                    
                            ver_UpgradeTimeValueClear();    /*清除升级超时时间*/
                        }
                        else if(MCPB_TRANS_CTRL_CANCEL == pt_data[0])/*表示监控中心取消软件升级*/
                        {
                            g_ucVerUpgradeFlag = 0;
                            g_ulVerUpgradePackageSeqBk = 0;
                            g_ulVerUpgradePackageSeq = 0;							
                            ver_UpgradeTimeOutCtrl(DISABLE);/*关闭升级超时检测*/
                            rtt_printf(RTT_UPGRADE, RTT_TEXT_YELLOW"--->> : Exit update!!!\r\n");
                        }
                        else if(MCPB_TRANS_CTRL_COMPLETE == pt_data[0])/*表示软件升级正常结束*/
                        {
                            g_ucVerUpgradeFlag = 0;
                            g_ulVerUpgradePackageSeqBk = 0;
                            g_ulVerUpgradePackageSeq = 0;								
                            ver_UpgradeTimeOutCtrl(DISABLE);/*关闭升级超时检测*/
                            rtt_printf(RTT_UPGRADE, RTT_TEXT_YELLOW"[INFO]: update end, %s!\r\n", (g_ucVerUpgradeStatus == EN_VER_UPGRADE_SUCCESS) ? "[OK]" : "[ERROR]");
                            if(g_ucVerUpgradeStatus == EN_VER_UPGRADE_SUCCESS)
                            {					
                                if (g_ucVerFileTransEnd == 1)	
                                {				
                                    g_ucVerFileTransEnd = 2;/*升级完成，避免收到重复消息重复处理*/	
                                    g_ucVerUpgradeStatus = ver_FileLoad();
                                    if(g_ucVerUpgradeStatus == EN_VER_UPGRADE_SUCCESS)
                                        rtt_printf(RTT_UPGRADE, RTT_TEXT_YELLOW"[Ver File Load]: success!\r\n");                               
                                    else
            	    			    	rtt_printf(RTT_UPGRADE, RTT_TEXT_RED"[Ver File Load]: fail!\r\n");								
                                }
								else if (g_ucVerFileTransEnd == 0)
								{
									/*未收到文件传输完成，而收到了升级结束*/
									g_ucVerUpgradeStatus = EN_VER_UPGRADE_TSP_CANCLE_UPGRADE_SERVICE;
								}
                            }
                            /*g_ucVerUpgradeStatus = EN_VER_UPGRADE_SUCCESS;*/
                        }
                    }
                    break;
            
                case MCP_ID_OTA_PACK_ACK:/*文件数据包应答*/
                    {
                        if(0x03 == pt_data[0])          /*表示请求监控中心取消软件升级*/
                        {
                        }
                        else if(0x04 == pt_data[0])     /*表示由于文件中的厂家标识错误，设备终止软件升级*/
                        {
                        }
                        else if(0x05 == pt_data[0])     /*表示由于文件中的设备标识错误，设备终止软件升级*/
                        {
                        }
                        else if(0x06 == pt_data[0])     /*表示由于文件中的其它错误，设备终止软件升级*/
                        {
                        }
                        else
                        {
                            /* no nothing */
                        }
                    }
                    break;
            
                case MCP_ID_OTA_PACK_NUM:   /*文件数据块序号*/
                    {
                        ulUpgradeSeq = (uint32_t)(pt_data[0] | (pt_data[1]<<8) | (pt_data[2]<<16) | (pt_data[3]<<24));
						if (ulUpgradeSeq > 65535)/*本地升级和远程升级的顺序不同*/
							ulUpgradeSeq = (uint32_t)(pt_data[3] | (pt_data[2]<<8) | (pt_data[1]<<16) | (pt_data[0]<<24));
                        g_ulVerUpgradePackageSeq = ulUpgradeSeq;
                        rtt_printf(RTT_UPGRADE, RTT_TEXT_YELLOW"[INFO]: packet %d\r\n", ulUpgradeSeq);
                        ver_UpgradeTimeValueClear();        /*清除升级超时时间*/							
                    }
                    break;
            
                case MCP_ID_OTA_PACK:   /*文件数据块*/
                    {
						if(g_ulVerUpgradePackageSeqBk == 0)
						{
                            if(g_ucVerUpgradeStatus == EN_VER_UPGRADE_SUCCESS)
                            {
								/*rtt_printf(RTT_UPGRADE, RTT_TEXT_YELLOW"ver_FileDataSave: %x%x\r\n", pt_data[0], pt_data[1]);*/
                                g_ucVerUpgradeStatus = ver_FileDataSave(pt_data, data_block_len - 4);
                                if(g_ucVerUpgradeStatus)
                                    rtt_printf(RTT_UPGRADE, RTT_TEXT_RED"[ERROR]: packet %d write fail res=%d\r\n", ulUpgradeSeq, g_ucVerUpgradeStatus);
                                else
                                    g_ulVerUpgradePackageSeqBk = g_ulVerUpgradePackageSeq;
                            }							
						}
                        else if(g_ulVerUpgradePackageSeq > g_ulVerUpgradePackageSeqBk)
                        {
                            if(g_ucVerUpgradeStatus == EN_VER_UPGRADE_SUCCESS)
                            {
								/*rtt_printf(RTT_UPGRADE, RTT_TEXT_YELLOW"ver_FileDataSave: %x%x\r\n", pt_data[0], pt_data[1]);*/
                                g_ucVerUpgradeStatus = ver_FileDataSave(pt_data, data_block_len - 4);
                                if(g_ucVerUpgradeStatus)
                                    rtt_printf(RTT_UPGRADE, RTT_TEXT_RED"[ERROR]: packet %d write fail res=%d\r\n", ulUpgradeSeq, g_ucVerUpgradeStatus);
                                else
                                    g_ulVerUpgradePackageSeqBk = g_ulVerUpgradePackageSeq;
                            }
                        }
                        else
                        {
                            rtt_printf(RTT_UPGRADE, RTT_TEXT_YELLOW"[WARN]: packet %d repeat\r\n", g_ulVerUpgradePackageSeq);
                        }
                    }
                    break;

                case MCP_ID_MCP_B_FILE_VER:   /*当前待升级软件版本*/
                    {
                        pt_data[0] = 0;
                        pt_data[1] = 0;
                    }					
                    break;					
					
                default:
                    break;
            }        	
        }
        else
        {
        	err_flag = MCP_RESP_SIGN_PROTO_ERROR;
        }
		
		if (data_id != MCP_ID_OTA_PACK)
		{
        	memcpy(mcp_msg->ptr_src_buf + mcp_msg->tx_buf_len, mcp_msg->ptr_data_unit, data_block_len);
        	mcp_msg->tx_buf_len += data_block_len;
		}

        /*移动指针到下一个数据块*/ 
        Num = Num + data_block_len;
        if ((Num + OFFSET_MCPB_DATA) < mcp_msg->rx_buf_len)
        {
            mcp_msg->ptr_data_unit += data_block_len;
        }
        /*处理错误码*/ 
        if (ret == MCP_RESP_SIGN_OK && err_flag != MCP_RESP_SIGN_OK) /*设置当任何一个参数解析失败时，设返回值为错误码*/ 
            ret = err_flag;
        else if (ret == MCP_RESP_SIGN_CONDITION && (err_flag != MCP_RESP_SIGN_OK && err_flag != MCP_RESP_SIGN_CONDITION)) /*其他错误码覆盖条件错误码*/ 
        {
            ret = err_flag;
            break;
        }
    }
	
	mcp_msg->tx_buf_len += 3;/*增加校验和和结束符的长度*/

    return ret;
}

#if (_MU == LOCAL_MODULE_TYPE)  
/*****************************************************************************
 * @Name	  : mcpB_parase_msg_slave_dev
 * @Brief  : mcpB协议主从机业务解析及处理,解析从机业务数据包,包括:
 *           主查从回包处理,存储查询项
 * @Param  : mcp_msg: 指向源数据
 * @Retval : 执行结果
 * @Author :
 * @Data   : 2024-1-13
 * @Note   :
 *****************************************************************************/
extern uint8_t get_ble_slave_index_by_slave_id(uint8_t slv_ble_conn_id);
uint8_t mcpB_parase_msg_slave_dev(MCP_MSG_T * mcp_msg)
{
    uint8_t ret = MCP_RESP_SIGN_OK, err_flag = MCP_RESP_SIGN_OK; /*错误标志*/ 
    uint16_t data_id;                                            /*数据标识符*/ 
    uint16_t Num = ADDR_MCP_DATA_FIELD;                          /*有效的监控标识前有14个字节的数据*/ 
    uint8_t data_block_len = 0;                                  /*数据块长度*/ 
    uint8_t ble_conn_id = 0;                                    /*子站点ble_slave_id*/ 
    uint8_t ble_slave_index = 0;                                 /*蓝牙数组索引*/ 

    uint8_t *pt_data;              /*指向数据*/ 	
    uint32_t ulUpgradeSeq;
	
    /*通过子站点编号查蓝牙从设备id*/ 
    ble_conn_id = mcp_msg->dev_id; 
    if(ble_conn_id == 0)
        return MCP_RESP_SIGN_OTHER_ERROR;

    ble_slave_index = get_ble_slave_index_by_slave_id(ble_conn_id);	

    while ((Num + OFFSET_MCPB_DATA) < mcp_msg->rx_buf_len)
    {
        data_block_len = ((*mcp_msg->ptr_data_unit) & 0x00FF) | ((*(mcp_msg->ptr_data_unit + 1) << 8) & 0xFF00);/*低字节在前，高字节在后*/
        data_id = (*(mcp_msg->ptr_data_unit + 2) & 0x00FF) | ((*(mcp_msg->ptr_data_unit + 3) << 8) & 0xFF00);/*低字节在前，高字节在后*/
        pt_data = mcp_msg->ptr_data_unit + OFFSET_MCPB_DATA; 	
	
        /*rtt_printf(RTT_UPGRADE, RTT_TEXT_RED"mcpB_parase_msg_slave_dev: mcp_type=%d    data_id=%x    data_block_len=%d\r\n", mcp_msg->cmd_type, data_id, data_block_len);*/
		
        if(mcp_msg->cmd_type == MCP_CMD_TYPE_QUERY)		
        {     	
        }
        else if(mcp_msg->cmd_type == MCP_CMD_TYPE_SET)		
        {
            switch(data_id)
            {            
                case MCP_ID_OTA_TRANS_CTRL:   /*文件传输控制*/
					Upgrade_recv_trans_ctrl(ble_slave_index, pt_data[0]);
                    break;
            
                case MCP_ID_OTA_PACK_NUM:   /*文件数据块序号*/
                    {
                        ulUpgradeSeq = (uint32_t)(pt_data[0] | (pt_data[1]<<8) | (pt_data[2]<<16) | (pt_data[3]<<24));
						Upgrade_recv_pack_num(ble_slave_index, ulUpgradeSeq);					
						rtt_printf(RTT_UPGRADE, RTT_TEXT_YELLOW"[INFO]: dev_id %d ack packet %d\r\n", ble_conn_id, ulUpgradeSeq);						
                    }
                    break;
	
                default:
                    break;
            }        	
        }
        else
        {
        	err_flag = MCP_RESP_SIGN_PROTO_ERROR;
        }

        /*移动指针到下一个数据块*/ 
        Num = Num + data_block_len;
        if ((Num + OFFSET_MCPB_DATA) < mcp_msg->rx_buf_len)
        {
            mcp_msg->ptr_data_unit += data_block_len;
        }
        /*处理错误码*/ 
        if (ret == MCP_RESP_SIGN_OK && err_flag != MCP_RESP_SIGN_OK) /*设置当任何一个参数解析失败时，设返回值为错误码*/ 
            ret = err_flag;
        else if (ret == MCP_RESP_SIGN_CONDITION && (err_flag != MCP_RESP_SIGN_OK && err_flag != MCP_RESP_SIGN_CONDITION)) /*其他错误码覆盖条件错误码*/ 
        {
            ret = err_flag;
            break;
        }
    }

    return ret;	
}
#endif

/*****************************************************************************
 * @Name	  : mcpB_msg_handler
 * @Brief  : MCP_B协议发送消息处理
 * @Param  : rx_data:接收数据
 *           len:接收数据长度
 *           reply_data:回复数据
 * @Retval : 回复数据长度
 * @Author :
 * @Data   : 2024-1-10
 * @Note   :
 *****************************************************************************/
uint16_t mcpB_msg_handler(uint8_t *rx_data, uint16_t len, uint8_t *reply_data, uint8_t msg_src)
{
    uint8_t ret = 0;
    uint16_t crc_result;
    MCP_MSG_T mcp_msg;
    //uint8_t device_id = 0;
	
    /*rtt_printf(RTT_UPGRADE, RTT_TEXT_RED"mcpB_msg_handler: len=%d\r\n", len);*/
	
    if (!len)
        return 0;

    get_mcpB_msg_info(&mcp_msg, rx_data, len, msg_src);

    /*判断是否是MCP_B协议*/ 
    if (mcp_msg.mcp_type != MCP_B)
        return 0;

#if (_MU == LOCAL_MODULE_TYPE)                            /*本设备为主机*/ 
    if(mcp_msg.vp_sign == VP_SIGN_REPLY && mcp_msg.msg_src == MCP_BLE) /*从机应答包 */ 
    {
        ret = mcpB_parase_msg_slave_dev(&mcp_msg);
    }
    else if (mcp_msg.vp_sign == VP_SIGN_SEND) /*请求包*/ 
    {
        ret = mcpB_parase_msg_local_dev(&mcp_msg);
    }
#elif (_RU == LOCAL_MODULE_TYPE) /*本设备为从机,从机仅支持查本机,收到任何id都当做本机查询*/ 

    ret = mcpB_parase_msg_local_dev(&mcp_msg);
#endif
	
    /*rtt_printf(RTT_UPGRADE, RTT_TEXT_RED"mcpB_msg_handler: vp_sign=%d\r\n", mcp_msg.vp_sign);*/	
    /*当报文是回包时,不需要再回复*/ 
    if (mcp_msg.vp_sign == VP_SIGN_REPLY)
        return 0;

    /*设置回复数据标志*/ 
    *mcp_msg.ptr_mcp_resp = ret;
    *mcp_msg.ptr_vp_resp = VP_SIGN_REPLY;

    crc_result = crc16((mcp_msg.ptr_src_buf + 1), mcp_msg.tx_buf_len - 4);
	
    mcp_msg.ptr_crc_L_resp = mcp_msg.ptr_src_buf + mcp_msg.tx_buf_len - 3;
    mcp_msg.ptr_crc_H_resp = mcp_msg.ptr_src_buf + mcp_msg.tx_buf_len - 2;

    *mcp_msg.ptr_crc_L_resp = UINT16_L(crc_result); /*L位*/ 
    *mcp_msg.ptr_crc_H_resp = UINT16_H(crc_result); /*H位*/
	
	*(mcp_msg.ptr_src_buf + mcp_msg.tx_buf_len - 1) = MCP_EOP;

	/*rtt_printf(RTT_UPGRADE, RTT_TEXT_RED"mcpB_msg_handler: tx_buf_len=%d\r\n", mcp_msg.tx_buf_len);*/
	
    memcpy(reply_data, mcp_msg.ptr_src_buf, mcp_msg.tx_buf_len); /*将发送数据存放缓存数组*/

    mcp_msg.tx_buf_len = MsgEscape(reply_data, mcp_msg.tx_buf_len); /*转义处理*/

    return mcp_msg.tx_buf_len;
}

/*****************************************************************************
* @Name	  :  mcpB_upgrade_result
* @Brief  :  mcpB升级结果
* @Param  :  无
* @Retval :  返回升级结果
                0：表示已经成功完成升级，
                1：表示设备终止软件升级，（比如硬件故障）
                2：表示监控中心取消软件升级，
                3：表示文件检查失败，
                4：表示保存文件失败，
                5：表示远程通信链路异常，
                6：表示远程通信超时，
                7：表示文件FTP下载超时，
                8：表示文件登录FTP服务器失败，
                9：表示正在通过FTP方式升级，
                17：表示其它异常中断软件升级
* @Author :
* @Data   :  2024-02-26
* @Note   :  
*****************************************************************************/
uint8_t mcpB_upgrade_result(void)
{
	if(g_ucVerUpgradeStatus == EN_VER_UPGRADE_SUCCESS)                          /*升级成功               */
		return 0;
	else if(g_ucVerUpgradeStatus == EN_VER_UPGRADE_WE_FLASH_FAIL)              /*写擦flash失败          */
		return 4;
	else if(g_ucVerUpgradeStatus == EN_VER_UPGRADE_FILE_TOO_LAGER)             /*固件过大               */
		return 4;
	else if(g_ucVerUpgradeStatus == EN_VER_UPGRADE_FILE_CRC_FAIL)              /*CRC校验失败            */
		return 3;
	else if(g_ucVerUpgradeStatus == EN_VER_UPGRADE_FILE_INFO_ERROR)            /*固件信息错误           */
		return 3;
	else if(g_ucVerUpgradeStatus == EN_VER_UPGRADE_FILE_MOVE_FAIL)             /*固件搬移失败           */
		return 4;
	else if(g_ucVerUpgradeStatus == EN_VER_UPGRADE_MCU_TYPE_ERROR)             /*MCU类型错误            */
		return 3;
	else if(g_ucVerUpgradeStatus == EN_VER_UPGRADE_FILE_LOAF_FAIL)             /*固件加载失败           */
		return 1;
	else if(g_ucVerUpgradeStatus == EN_VER_UPGRADE_UNDONE)                     /*升级未完成             */
		return 17;
	else if(g_ucVerUpgradeStatus == EN_VER_UPGRADE_BOOTLOADER_COPY_ERR)        /*bootloader拷贝失败     */
		return 4;	
	else if(g_ucVerUpgradeStatus == EN_VER_UPGRADE_TSP_CANCLE_UPGRADE_SERVICE) /*TSP取消升级            */
		return 2;	
	else if(g_ucVerUpgradeStatus == EN_VER_UPGRADE_UPGRADE_STOP_BY_MAN_NUM_ERR)/*升级终止由于厂商号错误 */
		return 1;	
	else if(g_ucVerUpgradeStatus == EN_VER_UPGRADE_UPGRADE_STOP_BY_DEV_NUM_ERR)/*升级终止由于设备号错误 */
		return 1;	
	else if(g_ucVerUpgradeStatus == EN_VER_UPGRADE_UPGRADE_STOP_BY_OTHER)      /*升级终止由于其他问题   */
		return 1;	
	else if(g_ucVerUpgradeStatus == EN_VER_UPGRADE_UPGRADE_APP_RUN_TIMEOUT)    /*APP运行超时            */
		return 17;	
	else
		return 17;
}

#if (LOCAL_MODULE_TYPE == _MU)
/*****************************************************************************
 * @Name	  : mcpB_send_trans_ctrl
 * @Brief  : MCP_B协议发送消息
 * @Param  : ble_conn_id
 *           ctrl:MCPB_TRANS_CTRL_START    0x01 文件传输开始
                  MCPB_TRANS_CTRL_END      0x02 文件传输结束
                  MCPB_TRANS_CTRL_CANCEL   0x03 监控中心取消软件升级
                  MCPB_TRANS_CTRL_COMPLETE 0x04 软件升级正常结束
 * @Retval : 无
 * @Author :
 * @Data   : 2024-3-12
 * @Note   :
 *****************************************************************************/
void mcpB_send_trans_ctrl(uint8_t ble_conn_id, uint8_t ctrl)
{
	mcp_send_pkt_t tx_pkt;
	uint8_t value[1];
		
    memset(&tx_pkt, 0, sizeof(mcp_send_pkt_t));
    new_mrc_send_pkt(&tx_pkt, ble_conn_id, MCP_B, MCP_CMD_TYPE_SET);	
	
	value[0] = ctrl;
	add_mrc_pkt_data(&tx_pkt, MCP_ID_OTA_TRANS_CTRL, value, 1);
	
	mrc_add_tail_and_escape(&tx_pkt);
	
	mrc_send_pkt_to_ru(&tx_pkt, ble_conn_id);
}

/*****************************************************************************
 * @Name	  : mcpB_send_file_data
 * @Brief  : MCP_B协议发送消息
 * @Param  : ble_conn_id
 *           seq:数据块序号
 *           pbuf:数据buf
 *           len:数据长度
 * @Retval : 无
 * @Author :
 * @Data   : 2024-3-12
 * @Note   :
 *****************************************************************************/
void mcpB_send_file_data(uint8_t ble_conn_id, uint32_t seq, uint8_t *pbuf, uint8_t len)
{
	mcp_send_pkt_t tx_pkt;
	uint8_t value[4];

    memset(&tx_pkt, 0, sizeof(mcp_send_pkt_t));
    new_mrc_send_pkt(&tx_pkt, ble_conn_id, MCP_B, MCP_CMD_TYPE_SET);	

	value[0] = 0;
	add_mrc_pkt_data(&tx_pkt, MCP_ID_OTA_PACK_ACK, value, 1);	
	
	value[0] = (uint8_t)((seq & 0x000000FF));
	value[1] = (uint8_t)((seq & 0x0000FF00) >> 8);
	value[2] = (uint8_t)((seq & 0x00FF0000) >> 16);
	value[3] = (uint8_t)((seq & 0xFF000000) >> 24);	
    add_mrc_pkt_data(&tx_pkt, MCP_ID_OTA_PACK_NUM, value, 4);	
	
	add_mrc_pkt_data(&tx_pkt, MCP_ID_OTA_PACK, pbuf, len);
	
	mrc_add_tail_and_escape(&tx_pkt);

	mrc_send_pkt_to_ru(&tx_pkt, ble_conn_id);
}
#endif
