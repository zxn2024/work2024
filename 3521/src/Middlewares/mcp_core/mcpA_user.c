/***************************** Copyright ****************************************
 *
 * (C) Copyright 2023, Shaanxi Tianji Communication
 *  All Rights Reserved
 *
 * FileName   : mcpA_user.c
 * Version    : none
 * Author     : none
 * Date       : 2024-2-20
 * Description: MCP:A协议解析及业务实现
 *******************************************************************************/
#include "bsp_config.h"
#include "mcp_core.h"
#include "ascii_hex.h"
#include "crc.h"
#include "monitor_para_list.h"
#include "bsp_reboot.h"
#include "bsp_dev_flash.h"
#include "bsp_dev_para.h"
#include "bsp_rf_global.h"
#include "mcpMRC_user.h"
#include "mcpA_user.h"
#include "app_monitor_report.h"
#include "app_monitor.h"

#if (LOCAL_MODULE_TYPE == _MU)

#include "bsp_rf_5g_modem.h"
#include "eswin_sync_modem.h"
#include "bsp_warn_user.h"
#include "ru_device_list.h"
extern sync_modem_t hModem; 

#elif (LOCAL_MODULE_TYPE == _RU)


#endif

/*---------------------- 宏定义  ---------------------------------------*/
/*---------------------- 结构体 ----------------------------------------*/

/*---------------------- 常量 ------------------------------------------*/

/*---------------------- 全局变量 --------------------------------------*/

/*---------------------- 声明 ------------------------------------------*/
#if (LOCAL_MODULE_TYPE == _MU)

extern sync_modem_t hModem; // MODEM参数信息

#elif (LOCAL_MODULE_TYPE == _RU)


#endif


/*---------------------- 函数 ------------------------------------------*/
/*****************************************************************************
 * @Name   : mcpA_rcv_data_proc
 * @Brief  : mcpa监控协议,接收对端发起包数据单元读写处理
 *          1.解析收到的设置报文
 *          2.解析收到的查询报文
 * @Param  : pt_msg: 指向收到的消息
 *			pt_param: 读本地数据时是入参,写本地数据时是出参
 *			strlen: 字符串长度,非字符串类型时此参数忽略,建议传0
 *			save: 指向保存监控数据的地址
 * @Retval : 返回错误码
 * @Author :
 * @Data   : 2024-1-11
 * @Note   :
 *****************************************************************************/

uint8_t mcpA_rcv_data_proc(MCP_MSG_T * pt_msg, void * pt_param, uint16_t strlen, uint8_t * save)
{
    MCP_TYPE_E data_type = pt_msg->mcp_data_type;   // 数据类型
    MCP_RW_E cmd_rw = pt_msg->mcp_data_rw;          // 读写类型
    int min = pt_msg->mcp_data_min;                 // 最小值
    int max = pt_msg->mcp_data_max;                 // 最大值
    uint8_t mcp_cmd = pt_msg->cmd_type;             // 指令类型
    //uint8_t vp_sign = pt_msg->vp_sign;            // 发包回包标志

    uint8_t * pt_data_unit = pt_msg->ptr_data_unit;                 // 指向当前处理的数据单元
    uint8_t data_len = *(uint8_t *)pt_data_unit - 3;                // 数据单元中的数据长度
    uint8_t * pt_id_h = (pt_data_unit + OFFSET_MCPA_DATA_CODE_H);   // 指向ID高字节
    uint8_t * pt_data = (pt_data_unit + OFFSET_MCPA_DATA);          // 指向数据

    uint8_t resp_errcode = 0;
    uint16_t i = 0;
    int val = 0;
    READ_WRITE_E data_operate = READ;
    
    //数据长度判断--防止设置时数据越界
    if(strlen > 0)
    {
        if(data_len != strlen)
            return MCP_RESP_SIGN_LEN_ERROR;
    }
    
    // 获取数据
    switch(data_type)
    {
        case MCP_BOOL:
            if(data_len != 1) {
                *pt_id_h |= MCP_ERR_LEN_ERR;
                return MCP_RESP_SIGN_CONDITION;
            }
            val = (uint8_t)(*pt_data);
            if (val > 1) {
                *pt_id_h |= MCP_ERR_WIDTH_ERR;
                return MCP_RESP_SIGN_CONDITION;
            }
            break;
        case MCP_UINT1:
            if(data_len != 1) {
                *pt_id_h |= MCP_ERR_LEN_ERR;
                return MCP_RESP_SIGN_CONDITION;
            }
            val = (uint8_t)(*pt_data);
            break;
            
        case MCP_INT1:
            if(data_len != 1) {
                *pt_id_h |= MCP_ERR_LEN_ERR;
                return MCP_RESP_SIGN_CONDITION;
            }
            val = (int8_t)(*pt_data);
            break;

        case MCP_INT2:
            if(data_len != 2) {
                *pt_id_h |= MCP_ERR_LEN_ERR;
                return MCP_RESP_SIGN_CONDITION;
            }
            val = (int16_t)H_AND_L(*(pt_data + 1), *pt_data);
            break;

        case MCP_UINT2:
            if(data_len != 2) {
                *pt_id_h |= MCP_ERR_LEN_ERR;
                return MCP_RESP_SIGN_CONDITION;
            }
            val = (uint16_t)H_AND_L(*(pt_data + 1), *pt_data);
            break;

        case MCP_INT4:
            if(data_len != 4) {
                *pt_id_h |= MCP_ERR_LEN_ERR;
                return MCP_RESP_SIGN_CONDITION;
            }
            val = (int32_t)(((*(pt_data + 3)) << 24) + ((*(pt_data + 2)) << 16) + ((*(pt_data + 1)) << 8) + (*pt_data));
            break;
        
        case MCP_UINT4:
            if(data_len != 4) {
                *pt_id_h |= MCP_ERR_LEN_ERR;
                return MCP_RESP_SIGN_CONDITION;
            }
            val = (uint32_t)(((*(pt_data + 3)) << 24) + ((*(pt_data + 2)) << 16) + ((*(pt_data + 1)) << 8) + (*pt_data));
            break;

        case MCP_STRING:
            val = 0;
            break;

        default:
            rtt_printf(RTT_PC, RTT_CTRL_TEXT_BRIGHT_YELLOW "Mcp data_type error:%d\r\n", data_type);
            return MCP_RESP_SIGN_PROTO_ERROR;
    }

    // 根据包类型确定数据处理方式
    if(pt_msg->vp_sign == VP_SIGN_REPLY && mcp_cmd == MCP_CMD_TYPE_QUERY && pt_msg->msg_src != MCP_MONITOR)  // 当收到的是查询的回复报文
    {
        // 回复包数据没有写操作,只有读
        data_operate = READ;
        // 处理回复包错误码,当回包错误码不为0时,不处理数据,直接返回错误
        resp_errcode = *pt_id_h & 0xf0;
        if(resp_errcode != MCP_ERR_OK)
            return MCP_RESP_SIGN_OTHER_ERROR;
    }
    else if(pt_msg->vp_sign == VP_SIGN_SEND)  // 当收到的是发起报文
    {
        if(mcp_cmd == MCP_CMD_TYPE_QUERY)  // 查询命令
        {
            // 当收查询时,写入数据
            data_operate = WRITE;
        }
        else if(mcp_cmd == MCP_CMD_TYPE_SET)  // 设置命令
        {
            // 判断设置值范围是否合法,min和max都为0时,表示不限制范围
            if(((val < min) || (val > max)) && (!(min == 0 && max == 0)))
            {
                *pt_id_h |= MCP_ERR_SET_VALUE_OVER;
                return MCP_RESP_SIGN_CONDITION;
            }
            // 当收到设置时,接收数据
            data_operate = READ;
            // 发起报文&&类型是设置&&读写标志为RW时,保存数据
            if(cmd_rw == MCP_RW)
                *save |= 1;
        }
        else if(mcp_cmd == MCP_CMD_TYPE_RPT)  // 上报命令
        {
            // 当收上报时,接收数据
            data_operate = READ;
        }
        else
            ;
    }
	else if(pt_msg->vp_sign == VP_SIGN_REPLY && mcp_cmd == MCP_CMD_TYPE_RPT)// 设置上报类型后的回包处理
	{
		 data_operate = READ;
	}
	else //其他情况,包括上报回包,设置回包,不处理数据.
        return MCP_RESP_SIGN_OK;

    switch(data_type)
    {
        case MCP_BOOL:
            if(READ == data_operate)
            {
                *(uint8_t *)pt_param = (*((uint8_t *)pt_data) == 0) ? 0 : 1;
            }
            else
            {
                *((uint8_t *)pt_data) = *(uint8_t *)pt_param == 0 ? 0 : 1;
            }
            break;

        case MCP_UINT1:
        case MCP_UINT2:
        case MCP_UINT4:
            if(READ == data_operate)
            {
                for(i = 0; i < data_len; i++)
                    *((uint8_t *)pt_param + data_len - 1 - i) = *((uint8_t *)pt_data + (data_len - 1 - i));
            }
            else
            {
                for(i = 0; i < data_len; i++)
                    *((uint8_t *)pt_data + (data_len - 1 - i)) = *((uint8_t *)pt_param + data_len - 1 - i);
            }
            break;

        case MCP_INT1:
        case MCP_INT2:
        case MCP_INT4:
            if(READ == data_operate)
            {
                for(i = 0; i < data_len; i++)
                    *((int8_t *)pt_param + data_len - 1 - i) = *((int8_t *)pt_data + (data_len - 1 - i));
            }
            else
            {
                for(i = 0; i < data_len; i++)
                    *((int8_t *)pt_data + (data_len - 1 - i)) = *((int8_t *)pt_param + data_len - 1 - i);
            }
            break;

        case MCP_STRING:
            // pdu字符串长度超过了参数字符串长度,以参数字符串长度为准避免指针越界
            if(data_len > strlen)
                data_len = strlen;
            if(READ == data_operate)
            {
                memset(pt_param, 0, strlen);
                for(i = 0; i < data_len; i++)
                    *((uint8_t *)pt_param + i) = *((uint8_t *)pt_data + i);
            }
            else
            {
                memset(pt_data, 0, data_len);
                for(i = 0; i < data_len; i++)
                    *((uint8_t *)pt_data + i) = *((uint8_t *)pt_param + i);
            }
            break;

        default:
            break;
    }
    return MCP_RESP_SIGN_OK;
}
#if (LOCAL_MODULE_TYPE == _MU)


/*****************************************************************************
 * @Name	  : mcpA_msg_parase_and_handle
 * @Brief  : mcpA协议解析及处理,解析本机业务数据包
 * @Param  : pScr: 指向源数据
 *			pLen: 源数据长度
 *			pdu_cmd: PDU命令
 * @Retval : 执行结果
 * @Author :
 * @Data   : 2024-1-13
 * @Note   :
 *****************************************************************************/
uint8_t mcpA_parase_msg_local_dev(MCP_MSG_T * mcp_msg)
{
    uint8_t ret = MCP_RESP_SIGN_OK, err_flag = MCP_RESP_SIGN_OK, resp_err_flag = MCP_RESP_SIGN_OK; // 错误标志
    int16_t ssTemp;                                                                                // 临时变量
    uint8_t ucTemp;                                                                                // 临时变量
    uint16_t data_id;                                                                              // 数据标识符
    uint32_t site_id_temp;
    uint16_t Num = 14;                                                                             // 有效的监控标识前有14个字节的数据
    uint8_t save_monitor = 0;
    uint8_t recon_monitor = 0;
    uint8_t data_block_len = 0; // 数据块长度
    uint8_t str_temp[32] = {0};
	
	if (mcp_msg->cmd_type == MCP_CMD_TYPE_SWITCH_UPGRADE_MODE)// 转换到软件升级模式
	{
		return MCP_RESP_SIGN_OK;
	}
	
	if (mcp_msg->cmd_type == MCP_CMD_TYPE_SWITCH_SOFE_VERSION) // 切换监控软件版本
	{
		return MCP_RESP_SIGN_OK;
	}
	
    while((Num + 3) < mcp_msg->rx_buf_len)
    {
        data_block_len = *(mcp_msg->ptr_data_unit);
        data_id = (*(mcp_msg->ptr_data_unit + 1) & 0x00FF);
        data_id |= ((*(mcp_msg->ptr_data_unit + 2) << 8) & 0xFF00);

        // 获取监控码对应的数据类型,上下限,读写权限等信息
        get_MU_MCP_data_id_config(data_id, mcp_msg);

        switch(data_id)
        {
            //设备信息
            case MCP_ID_QUARY_ID_LIST: // 监控列表
                ssTemp = vMpRptParaList(mcp_msg->dev_id, mcp_msg->ptr_data_unit, data_id);
                if(ssTemp >= 5)
                    mcp_msg->tx_buf_len = ssTemp + 14; // 从起始字节到结束字节的总长度
                break;

            case MCP_ID_QUARY_PRIVATE_ID_LIST: // 自定义监控列表
                // 从机列表,用于pc主查从
                break;

            case MCP_ID_MANUFACTURER_CODE: // 厂商代码
                err_flag = mcpA_rcv_data_proc(mcp_msg, &tDevFactoryPara.factoryId, 20, &save_monitor);
                break;

            case MCP_ID_DEV_TYPE: // 设备类别
                err_flag = mcpA_rcv_data_proc(mcp_msg, &tDevFactoryPara.devType, 0, &save_monitor);
                break;

            case MCP_ID_DEVICE_MODEL: // 设备型号
                err_flag = mcpA_rcv_data_proc(mcp_msg, tDevFactoryPara.devMode, 20, &save_monitor);
                break;

            case MCP_ID_SN: // sn号
                err_flag = mcpA_rcv_data_proc(mcp_msg, tDevFactoryPara.sn, 30, &save_monitor);
                break;

            case MCP_ID_SW_VERSION: // 监控版本信息
                memcpy(str_temp, SW_MON_VERSION, strlen(SW_MON_VERSION));
                err_flag = mcpA_rcv_data_proc(mcp_msg, str_temp, 20, &save_monitor);
                break;

            case MCP_ID_TXMK: // 远程数据通信模块
                ucTemp = CONST_PDU_TXMK;
                err_flag = mcpA_rcv_data_proc(mcp_msg, &ucTemp, 0, &save_monitor);
                break;

            case MCP_ID_DATA_MODEL_VERSION: // 数据模型版本
            {
                char str_temp[10] = {0};
                memcpy(str_temp,CONST_PDU_DATA_MODEL_VERSION,strlen(CONST_PDU_DATA_MODEL_VERSION)); 
                err_flag = mcpA_rcv_data_proc(mcp_msg, str_temp, 10, &save_monitor);
                break;
            }
            case MCP_ID_ROUTER_ADDRESS: // 设备路由登记地址
                tDevPublicPara.devRoutingAddr = 0; //主机固定为0
                err_flag = mcpA_rcv_data_proc(mcp_msg, &tDevPublicPara.devRoutingAddr, 0, &save_monitor);
                break;

            case MCP_ID_DEV_RESET: // 设备重启
                ucTemp = 0;
                err_flag = mcpA_rcv_data_proc(mcp_msg, &ucTemp, 0, &save_monitor);
                if(ucTemp == 1)
                {
                    reboot_set_egp_bit(RESET_MASTER);
                }
                break;

            //网管参数
            case MCP_ID_SITE_ID: // 站点编号 *
                site_id_temp = tOmcPara.bs_id;
                err_flag = mcpA_rcv_data_proc(mcp_msg, &tOmcPara.bs_id, 0, &save_monitor);
                if((mcp_msg->msg_src == MCP_PC)&&(mcp_msg->cmd_type == MCP_CMD_TYPE_SET) && (site_id_temp != tOmcPara.bs_id))//本地软件或本地操作界面修改了设备的站点编号
                {
                    socket_disconnect(LINK0);// 断开连接
                }
                break;

            case MCP_ID_DEV_ID: // 设备编号 *
                err_flag = mcpA_rcv_data_proc(mcp_msg, &tOmcPara.dev_id, 0, &save_monitor);
                *mcp_msg->ptr_devid_resp = tOmcPara.dev_id;
                break;

            case MCP_ID_IP_VERSION: // ipv4/ipv6选择
                err_flag = mcpA_rcv_data_proc(mcp_msg, &tOmcPara.ip[0].ip_type, 0, &save_monitor);
                break;

            case MCP_ID_IPV4: // 网管系统IP地址（ipv4）
                err_flag = mcpA_rcv_data_proc(mcp_msg, tOmcPara.ip[0].ipv4_addr, 4, &save_monitor);
                recon_monitor = save_monitor;
                break;

            case MCP_ID_PORT: // 网管系统IP端口
                err_flag = mcpA_rcv_data_proc(mcp_msg, &tOmcPara.ip[0].port, 0, &save_monitor);
                recon_monitor = save_monitor;
                break;

            case MCP_ID_IPV6: // 网管系统IPv6地址
                err_flag = mcpA_rcv_data_proc(mcp_msg, tOmcPara.ip[0].ipv6_addr, 48, &save_monitor);
                break;

            case MCP_ID_APN: // 无线ip通信方式接入点名称
                err_flag = mcpA_rcv_data_proc(mcp_msg, tOmcPara.apn, 20, &save_monitor);
                recon_monitor = save_monitor;
                break;

            case MCP_ID_HEARTBEAT_INTERVAL: // 心跳包间隔时间 *
                err_flag = mcpA_rcv_data_proc(mcp_msg, &tOmcPara.heart, 0, &save_monitor);
                break;

            case MCP_ID_USERNAME: // PS域参数, 用户标识 *
                err_flag = mcpA_rcv_data_proc(mcp_msg, tOmcPara.ip[0].username, 20, &save_monitor);
                recon_monitor = save_monitor;
                break;

            case MCP_ID_PASSWD: // PS域参数, 用户口令 *
                err_flag = mcpA_rcv_data_proc(mcp_msg, tOmcPara.ip[0].password, 20, &save_monitor);
                recon_monitor = save_monitor;
                break;

            case MCP_ID_TCP_UDP: // PS域传输协议 *tcp_or_udp
                err_flag = mcpA_rcv_data_proc(mcp_msg, &tOmcPara.ip[0].tcp_or_udp, 0, &save_monitor);
                break;

            case MCP_ID_DEV_PORT: // 设备端接收口号 *
                err_flag = mcpA_rcv_data_proc(mcp_msg, &tOmcPara.dev_port, 0, &save_monitor);
                break;

            case MCP_ID_REPORT_TYPE:                                                            // 上报类型,为主动上报报文,此处仅解析平台回包
                if(mcp_msg->cmd_type == MCP_CMD_TYPE_RPT && mcp_msg->vp_sign == VP_SIGN_REPLY)  // 处理平台回复的告警确认包
                {
                    resp_err_flag = mcpA_rcv_data_proc(mcp_msg, &ucTemp, 0, &save_monitor);
                    if(resp_err_flag == MCP_ERR_OK)
                    {
                       report_reply_process(mcp_msg->dev_id,ucTemp); // 处理上报回复
                    }
                }
				else if(mcp_msg->cmd_type == MCP_CMD_TYPE_SET && mcp_msg->vp_sign == VP_SIGN_SEND)  // 处理平台回复的告警确认包
                {
                    resp_err_flag = mcpA_rcv_data_proc(mcp_msg, &ucTemp, 0, &save_monitor);
					if(resp_err_flag == MCP_ERR_OK)
                    {
                       report_process(ucTemp); // 处理上报回复
                    }
                }
                break;

            case MCP_ID_COMM_MODE: // 通信方式 *
                ucTemp = CONST_PDU_TXFS;
                err_flag = mcpA_rcv_data_proc(mcp_msg, &ucTemp, 0, &save_monitor);
                break;

            //采样参数
            case MCP_ID_FDD_UL_A_GAIN: //FDD上行实际增益 
                err_flag = mcpA_rcv_data_proc(mcp_msg, &t_rf_sys[RF_SYS_1].t_ch[RF_CH_UP].tGain.scRealGain, 0, &save_monitor);
                break;

            case MCP_ID_FDD_BANDWIDTH: //FDD带宽 
                err_flag = mcpA_rcv_data_proc(mcp_msg, &hModem.LTE_net_info.bw, 0, &save_monitor);
                break;

            case MCP_ID_FDD_BAND: //FDD Band 
                err_flag = mcpA_rcv_data_proc(mcp_msg, &hModem.LTE_net_info.band, 0, &save_monitor);
                break;

            case MCP_ID_FDD_CHANNEL: //FDD Channel 
                err_flag = mcpA_rcv_data_proc(mcp_msg, &hModem.LTE_net_info.freq, 0, &save_monitor);
                break;

            case MCP_ID_FDD_PCI: //FDD PCI
                err_flag = mcpA_rcv_data_proc(mcp_msg, &hModem.LTE_net_info.pci, 0, &save_monitor);
                break;

            case MCP_ID_FDD_SINR: //FDD SINR 
                err_flag = mcpA_rcv_data_proc(mcp_msg, &hModem.LTE_net_info.sinr, 0, &save_monitor);
                break;

            case MCP_ID_FDD_RSRP: //FDD RSRP 
                err_flag = mcpA_rcv_data_proc(mcp_msg, &hModem.LTE_net_info.rsrp, 0, &save_monitor);
                break;

            case MCP_ID_TDD_UL_A_GAIN: //TDD上行实际增益 
                err_flag = mcpA_rcv_data_proc(mcp_msg, &t_rf_sys[RF_SYS_1].t_ch[RF_CH_UP].tGain.scRealGain, 0, &save_monitor);
                break;

            case MCP_ID_TDD_BANDWIDTH: //TDD带宽 
                err_flag = mcpA_rcv_data_proc(mcp_msg, &hModem.LTE_net_info.bw, 0, &save_monitor);
                break;

            case MCP_ID_TDD_BAND: //TDD Band 
                err_flag = mcpA_rcv_data_proc(mcp_msg, &hModem.LTE_net_info.band, 0, &save_monitor);
                break;

            case MCP_ID_TDD_CHANNEL: //TDD Channel 
                err_flag = mcpA_rcv_data_proc(mcp_msg, &hModem.LTE_net_info.freq, 0, &save_monitor);
                break;

            case MCP_ID_TDD_PCI: //TDD PCI 
                err_flag = mcpA_rcv_data_proc(mcp_msg, &hModem.LTE_net_info.pci, 0, &save_monitor);
                break;

            case MCP_ID_TDD_SINR: //TDD SINR 
                err_flag = mcpA_rcv_data_proc(mcp_msg, &hModem.LTE_net_info.sinr, 0, &save_monitor);
                break;

            case MCP_ID_TDD_RSRP: //TDD RSRP 
                err_flag = mcpA_rcv_data_proc(mcp_msg, &hModem.LTE_net_info.rsrp, 0, &save_monitor);
                break;

            case MCP_ID_NR1_UL_A_GAIN: //5G上行实际增益 (通道1) 
                err_flag = mcpA_rcv_data_proc(mcp_msg, &t_rf_sys[RF_SYS_2].t_ch[RF_CH_UP].tGain.scRealGain, 0, &save_monitor);
                break;

            case MCP_ID_NR2_UL_A_GAIN: //5G上行实际增益 (通道2) 
                err_flag = mcpA_rcv_data_proc(mcp_msg, &t_rf_sys[RF_SYS_3].t_ch[RF_CH_UP].tGain.scRealGain, 0, &save_monitor);
                break;

            case MCP_ID_NR_BANDWIDTH: //5G带宽 
                err_flag = mcpA_rcv_data_proc(mcp_msg, &hModem.NR_net_info.bw, 0, &save_monitor);
                break;

            case MCP_ID_NR_CHANNEL: //5G Band 
                err_flag = mcpA_rcv_data_proc(mcp_msg, &hModem.NR_net_info.freq, 0, &save_monitor);
                break;

            case MCP_ID_NR_BAND: //5G Channel 
                err_flag = mcpA_rcv_data_proc(mcp_msg, &hModem.NR_net_info.band, 0, &save_monitor);
                break;

            case MCP_ID_NR_PCI: //5G PCI 
                err_flag = mcpA_rcv_data_proc(mcp_msg, &hModem.NR_net_info.pci, 0, &save_monitor);
                break;

            case MCP_ID_NR_SINR: //5G SINR 
                err_flag = mcpA_rcv_data_proc(mcp_msg, &hModem.NR_net_info.sinr, 0, &save_monitor);
                break;

            case MCP_ID_NR_RSRP: //5G RSRP 
                err_flag = mcpA_rcv_data_proc(mcp_msg, &hModem.NR_net_info.rsrp, 0, &save_monitor);
                break;

            case MCP_ID_TIMESLOT_RATIO: //5G上下行配比 
                //设置5G上下行配比,缺少执行函数
                err_flag = mcpA_rcv_data_proc(mcp_msg, &t_5g_modem_config.frame, 0, &save_monitor);
                break;

            case MCP_ID_SPECIAL_SUBFRAME: //5G特殊子帧配置
                //设置5G上下行配比,缺少执行函数
                err_flag = mcpA_rcv_data_proc(mcp_msg, &t_5g_modem_config.specialTime, 0, &save_monitor);
                break;

            case MCP_ID_FRAMEOFFSET: //5G帧头时间调整量
                //设置5G上下行配比,缺少执行函数
                err_flag = mcpA_rcv_data_proc(mcp_msg, &t_5g_modem_config.timeOffset, 0, &save_monitor);
                break;
            
            // 设置参数
            //XXX告警fdd与tdd冲突
            case MCP_ID_FDD_WARN_TH_DL_IN_OP: //FDD下行输入功率过功率门限 
                err_flag = mcpA_rcv_data_proc(mcp_msg, &t_dev_warn[0].t_warn_ch1_dw_inpow_greater.scGate, 0, &save_monitor);
                break;

            case MCP_ID_FDD_WARN_TH_DL_IN_LP: //FDD下行输入功率欠功率门限 
                err_flag = mcpA_rcv_data_proc(mcp_msg, &t_dev_warn[0].t_warn_ch1_dw_inpow_less.scGate, 0, &save_monitor);
                break;

            case MCP_ID_TDD_WARN_TH_DL_IN_OP: //TDD下行输入功率过功率门限 
                err_flag = mcpA_rcv_data_proc(mcp_msg, &t_dev_warn[0].t_warn_ch1_dw_inpow_greater.scGate, 0, &save_monitor);
                break;

            case MCP_ID_TDD_WARN_TH_DL_IN_LP: //TDD下行输入功率欠功率门限
                err_flag = mcpA_rcv_data_proc(mcp_msg, &t_dev_warn[0].t_warn_ch1_dw_inpow_less.scGate, 0, &save_monitor);
                break;

            case MCP_ID_NR1_WARN_TH_DL_IN_OP: //5G 通道1下行输入功率过功率门限 
                err_flag = mcpA_rcv_data_proc(mcp_msg, &t_dev_warn[0].t_warn_ch2_dw_inpow_greater.scGate, 0, &save_monitor);
                break;

            case MCP_ID_NR1_WARN_TH_DL_IN_LP: //5G 通道1下行输入功率欠功率门限 
                err_flag = mcpA_rcv_data_proc(mcp_msg, &t_dev_warn[0].t_warn_ch2_dw_inpow_less.scGate, 0, &save_monitor);
                break;

            case MCP_ID_NR2_WARN_TH_DL_IN_OP: //5G 通道2下行输入功率过功率门限 
                err_flag = mcpA_rcv_data_proc(mcp_msg, &t_dev_warn[0].t_warn_ch3_dw_inpow_greater.scGate, 0, &save_monitor);
                break;

            case MCP_ID_NR2_WARN_TH_DL_IN_LP: //5G 通道2下行输入功率欠功率门限 
                err_flag = mcpA_rcv_data_proc(mcp_msg, &t_dev_warn[0].t_warn_ch3_dw_inpow_less.scGate, 0, &save_monitor);
                break;

            /**************主机告警业务*************
             *
             * 告警业务处理:
             * 1. 平台查询,填充数据并回复平台
             * 2. 主机上报的平台回复处理,设置告警信息
            */
            case MCP_ID_TDD_WARN_DL_IN_OP: //TDD下行输入过功率告警 
                if(mcp_msg->vp_sign == VP_SIGN_REPLY)  // 处理平台回复的告警确认包
                {
                    resp_err_flag = mcpA_rcv_data_proc(mcp_msg, &ucTemp, 0, &save_monitor);
                    if(resp_err_flag == MCP_ERR_OK)
                    {
                        // 处理告警确认
                        dev_clr_warn_state_after_rpt_success(mcp_msg->dev_id);
                    }
                }
                else // 处理告警查询
                {
                    if(t_dev_warn[mcp_msg->dev_id].tdd_or_fdd == IS_TDD)
                        ucTemp = vWarnGetStat(&t_dev_warn[mcp_msg->dev_id].t_warn_ch1_dw_inpow_greater);
                    else
                        ucTemp = 0;
                    err_flag = mcpA_rcv_data_proc(mcp_msg, &ucTemp, 0, &save_monitor);
                }
                break;

            case MCP_ID_TDD_WARN_DL_IN_LP: //FDD下行输入欠功率告警 
                if(mcp_msg->vp_sign == VP_SIGN_REPLY)  // 处理平台回复的告警确认包
                {
                    resp_err_flag = mcpA_rcv_data_proc(mcp_msg, &ucTemp, 0, &save_monitor);
                    if(resp_err_flag == MCP_ERR_OK)
                    {
                        // 处理告警确认
                        dev_clr_warn_state_after_rpt_success(mcp_msg->dev_id);
                    }
                }
                else // 处理告警查询
                {
                    if(t_dev_warn[mcp_msg->dev_id].tdd_or_fdd == IS_TDD)
                        ucTemp = vWarnGetStat(&t_dev_warn[mcp_msg->dev_id].t_warn_ch1_dw_inpow_less);
                    else
                        ucTemp = 0;
                    
                    err_flag = mcpA_rcv_data_proc(mcp_msg, &ucTemp, 0, &save_monitor);
                }
                break;

            case MCP_ID_FDD_WARN_DL_IN_OP: //FDD下行输入过功率告警 
                if(mcp_msg->vp_sign == VP_SIGN_REPLY)  // 处理平台回复的告警确认包
                {
                    resp_err_flag = mcpA_rcv_data_proc(mcp_msg, &ucTemp, 0, &save_monitor);
                    if(resp_err_flag == MCP_ERR_OK)
                    {
                        // 处理告警确认
                        dev_clr_warn_state_after_rpt_success(mcp_msg->dev_id);
                    }
                }
                else // 处理告警查询
                {
                    if(t_dev_warn[mcp_msg->dev_id].tdd_or_fdd == IS_FDD)
                        ucTemp = vWarnGetStat(&t_dev_warn[mcp_msg->dev_id].t_warn_ch1_dw_inpow_greater);
                    else
                        ucTemp = 0;
                    err_flag = mcpA_rcv_data_proc(mcp_msg, &ucTemp, 0, &save_monitor);
                }
                break;

            case MCP_ID_FDD_WARN_DL_IN_LP: //FDD下行输入欠功率告警 
                if(mcp_msg->vp_sign == VP_SIGN_REPLY)  // 处理平台回复的告警确认包
                {
                    resp_err_flag = mcpA_rcv_data_proc(mcp_msg, &ucTemp, 0, &save_monitor);
                    if(resp_err_flag == MCP_ERR_OK)
                    {
                        // 处理告警确认
                        dev_clr_warn_state_after_rpt_success(mcp_msg->dev_id);
                    }
                }
                else // 处理告警查询
                {
                    if(t_dev_warn[mcp_msg->dev_id].tdd_or_fdd == IS_FDD)
                        ucTemp = vWarnGetStat(&t_dev_warn[mcp_msg->dev_id].t_warn_ch1_dw_inpow_less);
                    else
                        ucTemp = 0;
                    err_flag = mcpA_rcv_data_proc(mcp_msg, &ucTemp, 0, &save_monitor);
                }
                break;

            case MCP_ID_NR1_WARN_DL_IN_OP: //5G通道1下行输入过功率告警 
                if(mcp_msg->vp_sign == VP_SIGN_REPLY)  // 处理平台回复的告警确认包
                {
                    resp_err_flag = mcpA_rcv_data_proc(mcp_msg, &ucTemp, 0, &save_monitor);
                    if(resp_err_flag == MCP_ERR_OK)
                    {
                        // 处理告警确认
                        dev_clr_warn_state_after_rpt_success(mcp_msg->dev_id);
                    }
                }
                else // 处理告警查询
                {
                    ucTemp = vWarnGetStat(&t_dev_warn[mcp_msg->dev_id].t_warn_ch2_dw_inpow_greater);
                    err_flag = mcpA_rcv_data_proc(mcp_msg, &ucTemp, 0, &save_monitor);
                }
                break;

            case MCP_ID_NR1_WARN_DL_IN_LP: //5G通道1下行输入欠功率告警 
                if(mcp_msg->vp_sign == VP_SIGN_REPLY)  // 处理平台回复的告警确认包
                {
                    resp_err_flag = mcpA_rcv_data_proc(mcp_msg, &ucTemp, 0, &save_monitor);
                    if(resp_err_flag == MCP_ERR_OK)
                    {
                        // 处理告警确认
                        dev_clr_warn_state_after_rpt_success(mcp_msg->dev_id);
                    }
                }
                else // 处理告警查询
                {
                    ucTemp = vWarnGetStat(&t_dev_warn[mcp_msg->dev_id].t_warn_ch2_dw_inpow_less);
                    err_flag = mcpA_rcv_data_proc(mcp_msg, &ucTemp, 0, &save_monitor);
                }
                break;

            case MCP_ID_NR2_WARN_DL_IN_OP: //5G通道2下行输入过功率告警 
                if(mcp_msg->vp_sign == VP_SIGN_REPLY)  // 处理平台回复的告警确认包
                {
                    resp_err_flag = mcpA_rcv_data_proc(mcp_msg, &ucTemp, 0, &save_monitor);
                    if(resp_err_flag == MCP_ERR_OK)
                    {
                        // 处理告警确认
                        dev_clr_warn_state_after_rpt_success(mcp_msg->dev_id);
                    }
                }
                else // 处理告警查询
                {
                    ucTemp = vWarnGetStat(&t_dev_warn[mcp_msg->dev_id].t_warn_ch3_dw_inpow_greater);
                    err_flag = mcpA_rcv_data_proc(mcp_msg, &ucTemp, 0, &save_monitor);
                }
                break;

            case MCP_ID_NR2_WARN_DL_IN_LP: //5G通道2下行输入欠功率告警 
                if(mcp_msg->vp_sign == VP_SIGN_REPLY)  // 处理平台回复的告警确认包
                {
                    resp_err_flag = mcpA_rcv_data_proc(mcp_msg, &ucTemp, 0, &save_monitor);
                    if(resp_err_flag == MCP_ERR_OK)
                    {
                        // 处理告警确认
                        dev_clr_warn_state_after_rpt_success(mcp_msg->dev_id);
                    }
                }
                else // 处理告警查询
                {
                    ucTemp = vWarnGetStat(&t_dev_warn[mcp_msg->dev_id].t_warn_ch3_dw_inpow_less);
                    err_flag = mcpA_rcv_data_proc(mcp_msg, &ucTemp, 0, &save_monitor);
                }
                break;

            case MCP_ID_FDD_WARN_SELF_EXECITATION: //自激告警 
                if(mcp_msg->vp_sign == VP_SIGN_REPLY)  // 处理平台回复的告警确认包
                {
                    resp_err_flag = mcpA_rcv_data_proc(mcp_msg, &ucTemp, 0, &save_monitor);
                    if(resp_err_flag == MCP_ERR_OK)
                    {
                        // 处理告警确认
                        dev_clr_warn_state_after_rpt_success(mcp_msg->dev_id);
                    }
                }
                else // 处理告警查询
                {
                    ucTemp = vWarnGetStat(&t_dev_warn[mcp_msg->dev_id].t_warn_iso);
                    err_flag = mcpA_rcv_data_proc(mcp_msg, &ucTemp, 0, &save_monitor);
                }
                break;


            case MCP_ID_MCP_MODE_A:	//设备监控软件运行模式 		
            	*(mcp_msg->ptr_data_unit + 3) = 0;
            	break;
            
            case MCP_ID_APC_MAX_LENGTH:	//AP:C协议最大长度 		
            	*(mcp_msg->ptr_data_unit + 3) = (uint8_t)(CONST_PDU_APCZDCD & 0x00FF); 
            	*(mcp_msg->ptr_data_unit + 4) = (uint8_t)(CONST_PDU_APCZDCD >> 8); 
            	break;		
            
            case MCP_ID_MCP_B_INTERACTION_MODE:   //MCP:B交互机制
            	*(mcp_msg->ptr_data_unit + 3) = CONST_PDU_MCPBJHJZ; 
                break;
                    
            case MCP_ID_NUM_OF_CONTINUOUS_PACKS:   //连发系数
            	*(mcp_msg->ptr_data_unit + 3) = CONST_PDU_LFXS;
                break;
                
            case MCP_ID_DEV_RESP_TIMEOUT: //设备响应超时
            	*(mcp_msg->ptr_data_unit + 3) = CONST_PDU_SBXYCS;
                break;
                
            case MCP_ID_TX_INTERVAL: //发送间隔时间
                *(mcp_msg->ptr_data_unit + 3) = (uint8_t)(CONST_PDU_FSJGSJ & 0x00FF); 
                *(mcp_msg->ptr_data_unit + 4) = (uint8_t)(CONST_PDU_FSJGSJ >> 8); 				
                break;
                
            case MCP_ID_OTA_PAUSE_TIMEOUT: //暂停传输等待时间
            	*(mcp_msg->ptr_data_unit + 3) = CONST_PDU_ZTDDSJ;
                break;
                
            case MCP_ID_OTA_MODE_TIMEOUT:   //监控模式/升级模式转换时间
                *(mcp_msg->ptr_data_unit + 3) = (uint8_t)(CONST_PDU_JKMSZHSJ & 0x00FF); 
                *(mcp_msg->ptr_data_unit + 4) = (uint8_t)(CONST_PDU_JKMSZHSJ >> 8); 			
                break;
            
            case MCP_ID_OTA_RESULT: //设备执行软件升级的结果 
            	*(mcp_msg->ptr_data_unit + 3) = mcpB_upgrade_result();
                break;			
			
            case MCP_ID_OTA_MODE:			
            	*(mcp_msg->ptr_data_unit + 3) = CONST_PDU_YCSJFS;
            	break;						
				
				
            default:
                *(mcp_msg->ptr_data_unit + OFFSET_MCPA_DATA_CODE_H) |= MCP_ERR_IDERR;
                err_flag = MCP_RESP_SIGN_CONDITION;
                break;
        }

        // 处理错误码
        if(ret == MCP_RESP_SIGN_OK && err_flag != MCP_RESP_SIGN_OK) {  // 设置当任何一个参数解析失败时，设返回值为错误码
            ret = err_flag;
        }
        else if(ret == MCP_RESP_SIGN_CONDITION && (err_flag != MCP_RESP_SIGN_OK && err_flag != MCP_RESP_SIGN_CONDITION))  // 其他错误码覆盖条件错误码,并退出循环
        {
            ret = err_flag;
            break;
        }

        // 移动指针到下一个数据块
        Num = Num + data_block_len;
        if((Num + 3) < mcp_msg->rx_buf_len)
        {
            mcp_msg->ptr_data_unit += data_block_len;
            if (*mcp_msg->ptr_data_unit < 4 || *mcp_msg->ptr_data_unit > 51) {
                ret = MCP_RESP_SIGN_LEN_ERROR;
                break;
            }
        }
    }

    if(save_monitor)
    {
        flash_dev_para_rw(WRITE); // 出厂参数
    }
    
    if (recon_monitor) {
        monitor_reconnect();
    }

    return ret;
}

/*****************************************************************************
 * @Name	  : get_ble_conn_id_by_monitor_ru_id
 * @Brief  : 根据监控的从站编号查询从机的ble_conn_id
 * @Param  : ru_id: 监控的从站编号
 * @Retval : ble_conn_id: 子站点ble_conn_id, 0表示未找到
 * @Author :
 * @Data   : 2024-1-13
 * @Note   :
 */
uint8_t get_ble_conn_id_by_monitor_ru_id(uint8_t ru_id)
{
    uint8_t ret = 0;
    uint8_t i = 0;

    if(ru_id == 0 )
        return 0;

    for(i = 0; i < BLE_CONNECT_MAX_NUM; i++)
    {
        if(ru_device_list[i].ru_dev_id == ru_id )
        {
            ret = ru_device_list[i].ble_conn_id;
            break;
        }
    }

    return ret;
}
/*****************************************************************************
 * @Name	  : get_ble_slave_index_by_slave_id
 * @Brief  : 根据从设备蓝牙ID获取蓝牙数组索引
 * @Param  : slv_ble_conn_id: 从设备蓝牙ID
 * @Retval : 返回数据索引
 * @Author :
 * @Data   : 2024-1-11
 * @Note   :
*/
uint8_t get_ble_slave_index_by_slave_id(uint8_t slv_ble_conn_id)
{
    if(slv_ble_conn_id >= BLE_CONNECT_MAX_NUM)
        return 0;
    else
        return slv_ble_conn_id - 1;
}

/*****************************************************************************
 * @Name    : update_mrc_pkt_send_stat
 * @Brief  : 更新数据接收状态
 * @Param  : pt_msg: 指向收到的消息
 * @Retval : 无
 * @Author :
 * @Data   : 2024-1-13
 * @Note   : 主设备收到从机回复包时,如果有发包方在等待信息,发包信息和收包的包序号一致,更新数据接收状态,用于发包方查询
*/
void update_mcpA_pkt_send_stat(MCP_MSG_T *mcp_msg)
{
    uint8_t ru_index;
    ru_index = get_ble_slave_index_by_slave_id(mcp_msg->dev_id);
    
    if(ru_index!= 0xff && mcp_msg->vp_sign == VP_SIGN_REPLY && mcp_msg->mcp_resp_sign == MCP_RESP_SIGN_OK && mrc_ru_data[ru_index].pkt_send_stat.mrc_pkt_id == mcp_msg->vp_pack_num)
    {
        mrc_ru_data[ru_index].pkt_send_stat.mrc_pkt_send_stat = MRC_DATA_RECV_OK;
    }else if(ru_index!= 0xff && mcp_msg->vp_sign == VP_SIGN_REPLY && mcp_msg->mcp_resp_sign != MCP_RESP_SIGN_OK && mrc_ru_data[ru_index].pkt_send_stat.mrc_pkt_id == mcp_msg->vp_pack_num)
    {
        mrc_ru_data[ru_index].pkt_send_stat.mrc_pkt_send_stat = MRC_DATA_RECV_ERR;
    }
}

/*****************************************************************************
 * @Name	  : mcpA_msg_parase_and_handle
 * @Brief  : mcpA协议主从机业务解析及处理,解析从机业务数据包,包括:
 *           平台下发主查从,填充数据并回复平台
 *           主查从回包处理,存储查询项
 *           从机上报处理,设置告警信息
 * @Param  : pScr: 指向源数据
 *			pLen: 源数据长度
 *			pdu_cmd: PDU命令
 * @Retval : 执行结果
 * @Author :
 * @Data   : 2024-1-13
 * @Note   :
 *****************************************************************************/
uint8_t mcpA_parase_msg_slave_dev(MCP_MSG_T * mcp_msg)
{
    uint8_t ret = MCP_RESP_SIGN_OK, err_flag = MCP_RESP_SIGN_OK, resp_err_flag = MCP_RESP_SIGN_OK; // 错误标志
    int16_t ssTemp;                                                                                // 临时变量
    uint8_t ucTemp;                                                                                // 临时变量
    uint16_t data_id;                                                                              // 数据标识符
    uint16_t Num = 14;                                                                             // 有效的监控标识前有14个字节的数据
    uint8_t save_monitor = 0;                                                                      // 是否保存参数到flash
    uint8_t data_block_len = 0;                                                                    // 数据块长度
    uint8_t ble_conn_id = 0;                                                                       // 子站点ble_conn_id
    uint8_t ble_slave_index = 0;                                                                   // 蓝牙数组索引
    int8_t s8_temp;                                                                                // 临时变量
    uint8_t str_temp[32] = {0};

    //主从通信设备编号都是用ble连接id,平台查询设备号按监控设备号
    //先以包类型区分,查询设置发送和上报回包都是平台通信,查询设置回复和上报包都是从机通信
    //TODO 后续增加一个消息来源参数来区分
    //if((mcp_msg->vp_sign == VP_SIGN_SEND && mcp_msg->cmd_type == MCP_CMD_TYPE_SET) || (mcp_msg->vp_sign == VP_SIGN_SEND && mcp_msg->cmd_type == MCP_CMD_TYPE_QUERY) || (mcp_msg->vp_sign == VP_SIGN_REPLY && mcp_msg->cmd_type == MCP_CMD_TYPE_RPT))
    if (mcp_msg->msg_src == MCP_MONITOR || mcp_msg->msg_src == MCP_PC)
    {
        ///TODO 根据平台从机编号获取索引
        ble_slave_index = get_ble_slave_index_by_dev_id(mcp_msg->dev_id);
        if(ble_slave_index == 0xff)
            return MCP_RESP_SIGN_OTHER_ERROR;

        if(mcp_msg->cmd_type == MCP_CMD_TYPE_SET) // 处理主机收到的主从设置命令,直接透传,但是部分设置指令,如告警门限,需要保存到flash,所以还需要本地处理
        {
            // 当数据不超过蓝牙最大传输长度时,直接透传数据到蓝牙从设备
            if(mcp_msg->rx_buf_len <= MAX_MRC_DATA_LEN)
            {
                ble_slave_msg_send_by_id( mcp_msg->ptr_src_buf, mcp_msg->rx_buf_len, ble_conn_id);
            }
        }
    } else if (mcp_msg->msg_src == MCP_BLE) //if((mcp_msg->vp_sign == VP_SIGN_SEND && mcp_msg->cmd_type == MCP_CMD_TYPE_RPT) || (mcp_msg->vp_sign == VP_SIGN_REPLY && mcp_msg->cmd_type == MCP_CMD_TYPE_SET && mcp_msg->msg_src != MCP_MONITOR) || (mcp_msg->vp_sign == VP_SIGN_REPLY && mcp_msg->cmd_type == MCP_CMD_TYPE_QUERY && mcp_msg->msg_src != MCP_MONITOR))
    {
        ble_conn_id = mcp_msg->dev_id;
        if(ble_conn_id == 0)
            return MCP_RESP_SIGN_OTHER_ERROR;

        ble_slave_index = get_ble_slave_index_by_slave_id(ble_conn_id);
    } else {
        return MCP_RESP_SIGN_OK;
    }

    // 检查是否在等待此回包,更新数据接收状态
    if(mcp_msg->vp_sign == VP_SIGN_REPLY)
        update_mcpA_pkt_send_stat(mcp_msg);

    while((Num + 3) < mcp_msg->rx_buf_len)
    {
        data_block_len = *(mcp_msg->ptr_data_unit);
        data_id = (*(mcp_msg->ptr_data_unit + 1) & 0x00FF);
        data_id |= ((*(mcp_msg->ptr_data_unit + 2) << 8) & 0xFF00);

        // 获取监控码对应的数据类型,上下限,读写权限等信息
        get_MU_MCP_data_id_config(data_id, mcp_msg);

        switch(data_id)
        {
            case MCP_ID_MANUFACTURER_CODE: // 厂商代码  str20的数组
                err_flag = mcpA_rcv_data_proc(mcp_msg, mrc_ru_data[ble_slave_index].manu_id, 20, &save_monitor);
                break;

            case MCP_ID_DEV_TYPE: // 设备类别
                err_flag = mcpA_rcv_data_proc(mcp_msg, &mrc_ru_data[ble_slave_index].mcp_dev_type, 0, &save_monitor);
                break;

            case MCP_ID_DEVICE_MODEL: // 设备型号
                err_flag = mcpA_rcv_data_proc(mcp_msg, mrc_ru_data[ble_slave_index].mcp_dev_model, 20, &save_monitor);
                break;

            case MCP_ID_SN: // sn号
                err_flag = mcpA_rcv_data_proc(mcp_msg, mrc_ru_data[ble_slave_index].sn, 30, &save_monitor);
                break;

            case MCP_ID_QUARY_ID_LIST: // 监控列表
                ssTemp = vMpRptParaList(mcp_msg->dev_id, mcp_msg->ptr_data_unit, data_id);
                if(ssTemp >= 5)
                    mcp_msg->tx_buf_len = ssTemp + 14; // 从起始字节到结束字节的总长度
                break;

            case MCP_ID_SW_VERSION: // 监控版本信息
                err_flag = mcpA_rcv_data_proc(mcp_msg, &mrc_ru_data[ble_slave_index].mcu_sw_ver_str, 20, &save_monitor);
                break;
            
            case MCP_ID_DATA_MODEL_VERSION:     //数据模型版本
                memcpy(str_temp,CONST_PDU_DATA_MODEL_VERSION,strlen(CONST_PDU_DATA_MODEL_VERSION)); 
                err_flag = mcpA_rcv_data_proc(mcp_msg, str_temp, 10, &save_monitor);
                break;

            case MCP_ID_ROUTER_ADDRESS: // 设备路由登记地址
                err_flag = mcpA_rcv_data_proc(mcp_msg, &ru_device_list[ble_slave_index].router_addr, 0, &save_monitor);
                break;

            //网管参数
            case MCP_ID_SITE_ID: // 站点编号 *
                err_flag = mcpA_rcv_data_proc(mcp_msg, &tOmcPara.bs_id, 0, &save_monitor);
                break;

            case MCP_ID_DEV_ID: // 设备编号 *
                err_flag = mcpA_rcv_data_proc(mcp_msg, &ru_device_list[ble_slave_index].ru_dev_id, 0, &save_monitor);
                *mcp_msg->ptr_devid_resp = ru_device_list[ble_slave_index].ru_dev_id;
                break;
                
            case MCP_ID_DEV_RESET: // 设备重启
                ucTemp = 0;
                err_flag = mcpA_rcv_data_proc(mcp_msg, &ucTemp, 0, &save_monitor);
                if(ucTemp == 1)
                {
                    reboot_set_egp_bit(RESET_SLAVE);
                }
                break;
			case MCP_ID_REPORT_TYPE:                                                            // 上报类型,为主动上报报文,此处仅解析平台回包
                
				if(mcp_msg->cmd_type == MCP_CMD_TYPE_RPT && mcp_msg->vp_sign == VP_SIGN_REPLY)  // 处理平台回复的告警确认包
                {
                    resp_err_flag = mcpA_rcv_data_proc(mcp_msg, &ucTemp, 0, &save_monitor);
                    if(resp_err_flag == MCP_ERR_OK)
                    {
                       report_reply_process(mcp_msg->dev_id,ucTemp); // 处理上报回复
                    }
                }
				break;
            case MCP_ID_FDD_WARN_TH_DL_IN_OP: // 下行输入过功率告警门限
                if(mcp_msg->cmd_type == MCP_CMD_TYPE_SET)
                {
                    err_flag = mcpA_rcv_data_proc(mcp_msg, &s8_temp, 0, &save_monitor);
                    for(int i=1; i<=WARN_SLV_DEV_MAX_NUM; i++)
                    {
                        t_dev_warn[i].t_warn_ch1_dw_inpow_greater.scGate = s8_temp;
                    }
                }else
                    err_flag = mcpA_rcv_data_proc(mcp_msg, &t_dev_warn[mcp_msg->dev_id].t_warn_ch1_dw_inpow_greater.scGate, 0, &save_monitor);
                break;

            case MCP_ID_FDD_WARN_TH_DL_IN_LP: // 下行输入欠功率告警门限
                if(mcp_msg->cmd_type == MCP_CMD_TYPE_SET)
                {
                    err_flag = mcpA_rcv_data_proc(mcp_msg, &s8_temp, 0, &save_monitor);
                    for(int i=1; i<=WARN_SLV_DEV_MAX_NUM; i++)
                    {
                        t_dev_warn[i].t_warn_ch1_dw_inpow_less.scGate = s8_temp;
                    }
                }else
                    err_flag = mcpA_rcv_data_proc(mcp_msg, &t_dev_warn[mcp_msg->dev_id].t_warn_ch1_dw_inpow_less.scGate, 0, &save_monitor);
                break;

            case MCP_ID_TDD_WARN_TH_DL_IN_OP: // 下行输入过功率告警门限
                if(mcp_msg->cmd_type == MCP_CMD_TYPE_SET)
                {
                    err_flag = mcpA_rcv_data_proc(mcp_msg, &s8_temp, 0, &save_monitor);
                    for(int i=1; i<=WARN_SLV_DEV_MAX_NUM; i++)
                    {
                        t_dev_warn[i].t_warn_ch1_dw_inpow_greater.scGate = s8_temp;
                    }
                }else
                    err_flag = mcpA_rcv_data_proc(mcp_msg, &t_dev_warn[mcp_msg->dev_id].t_warn_ch1_dw_inpow_greater.scGate, 0, &save_monitor);
                break;

            case MCP_ID_TDD_WARN_TH_DL_IN_LP: // 下行输入欠功率告警门限
                if(mcp_msg->cmd_type == MCP_CMD_TYPE_SET)
                {
                    err_flag = mcpA_rcv_data_proc(mcp_msg, &s8_temp, 0, &save_monitor);
                    for(int i=1; i<=WARN_SLV_DEV_MAX_NUM; i++)
                    {
                        t_dev_warn[i].t_warn_ch1_dw_inpow_less.scGate = s8_temp;
                    }
                }else
                    err_flag = mcpA_rcv_data_proc(mcp_msg, &t_dev_warn[mcp_msg->dev_id].t_warn_ch1_dw_inpow_less.scGate, 0, &save_monitor);
                break;

            case MCP_ID_NR1_WARN_TH_DL_IN_OP: // 下行输入过功率告警门限
                if(mcp_msg->cmd_type == MCP_CMD_TYPE_SET)
                {
                    err_flag = mcpA_rcv_data_proc(mcp_msg, &s8_temp, 0, &save_monitor);
                    for(int i=1; i<=WARN_SLV_DEV_MAX_NUM; i++)
                    {
                        t_dev_warn[i].t_warn_ch2_dw_inpow_greater.scGate = s8_temp;
                    }
                }else
                    err_flag = mcpA_rcv_data_proc(mcp_msg, &t_dev_warn[mcp_msg->dev_id].t_warn_ch2_dw_inpow_greater.scGate, 0, &save_monitor);
                break;

            case MCP_ID_NR1_WARN_TH_DL_IN_LP: // 下行输入欠功率告警门限
                if(mcp_msg->cmd_type == MCP_CMD_TYPE_SET)
                {
                    err_flag = mcpA_rcv_data_proc(mcp_msg, &s8_temp, 0, &save_monitor);
                    for(int i=1; i<=WARN_SLV_DEV_MAX_NUM; i++)
                    {
                        t_dev_warn[i].t_warn_ch2_dw_inpow_less.scGate = s8_temp;
                    }
                }else
                    err_flag = mcpA_rcv_data_proc(mcp_msg, &t_dev_warn[mcp_msg->dev_id].t_warn_ch2_dw_inpow_less.scGate, 0, &save_monitor);
                break;

            case MCP_ID_NR2_WARN_TH_DL_IN_OP: // 下行输入过功率告警门限
                if(mcp_msg->cmd_type == MCP_CMD_TYPE_SET)
                {
                    err_flag = mcpA_rcv_data_proc(mcp_msg, &s8_temp, 0, &save_monitor);
                    for(int i=1; i<=WARN_SLV_DEV_MAX_NUM; i++)
                    {
                        t_dev_warn[i].t_warn_ch3_dw_inpow_greater.scGate = s8_temp;
                    }
                }else
                    err_flag = mcpA_rcv_data_proc(mcp_msg, &t_dev_warn[mcp_msg->dev_id].t_warn_ch3_dw_inpow_greater.scGate, 0, &save_monitor);
                break;

            case MCP_ID_NR2_WARN_TH_DL_IN_LP: // 下行输入欠功率告警门限
                if(mcp_msg->cmd_type == MCP_CMD_TYPE_SET)
                {
                    err_flag = mcpA_rcv_data_proc(mcp_msg, &s8_temp, 0, &save_monitor);
                    for(int i=1; i<=WARN_SLV_DEV_MAX_NUM; i++)
                    {
                        t_dev_warn[i].t_warn_ch3_dw_inpow_less.scGate = s8_temp;
                    }
                }else
                    err_flag = mcpA_rcv_data_proc(mcp_msg, &t_dev_warn[mcp_msg->dev_id].t_warn_ch3_dw_inpow_less.scGate, 0, &save_monitor);
                break;

            /**************主从告警业务*************
             *
             * 告警业务处理:
             * 1. 平台下发主查从,填充数据并回复平台
             * 2. 主查从,从机回包处理,存储查询项
             * 3. 从机上报的平台回复处理,设置告警信息
            */
            case MCP_ID_FDD_WARN_DL_IN_OP: // 4G FDD下行输入过功率告警
                if(mcp_msg->cmd_type == MCP_CMD_TYPE_RPT && mcp_msg->vp_sign == VP_SIGN_REPLY)  // 处理平台回复的告警确认包
                {
                    resp_err_flag = mcpA_rcv_data_proc(mcp_msg, &ucTemp, 0, &save_monitor);
                    if(resp_err_flag == MCP_ERR_OK)
                    {
                        //  处理告警确认
                        dev_clr_warn_state_after_rpt_success(mcp_msg->dev_id);
                    }
                }
                else if(mcp_msg->cmd_type == MCP_CMD_TYPE_QUERY && mcp_msg->vp_sign == VP_SIGN_REPLY) // 处理主从告警查询包回包
                {
                    resp_err_flag = mcpA_rcv_data_proc(mcp_msg, &ucTemp, 0, &save_monitor);
                }
                else if(mcp_msg->cmd_type == MCP_CMD_TYPE_QUERY && mcp_msg->vp_sign == VP_SIGN_SEND)// 处理平台告警查询包
                {
                    if(t_dev_warn[mcp_msg->dev_id].tdd_or_fdd == IS_FDD)
                        ucTemp = vWarnGetStat(&t_dev_warn[mcp_msg->dev_id].t_warn_ch1_dw_inpow_greater);
                    else
                        ucTemp = 0;
                    err_flag = mcpA_rcv_data_proc(mcp_msg, &ucTemp, 0, &save_monitor);
                }
                break;

            case MCP_ID_FDD_WARN_DL_IN_LP: // 4G FDD下行输入欠功率告警
                if(mcp_msg->cmd_type == MCP_CMD_TYPE_RPT && mcp_msg->vp_sign == VP_SIGN_REPLY)  // 处理平台回复的告警确认包
                {
                    resp_err_flag = mcpA_rcv_data_proc(mcp_msg, &ucTemp, 0, &save_monitor);
                    if(resp_err_flag == MCP_ERR_OK)
                    {
                        //  处理告警确认
                        dev_clr_warn_state_after_rpt_success(mcp_msg->dev_id);
                    }
                }
                else if(mcp_msg->cmd_type == MCP_CMD_TYPE_QUERY && mcp_msg->vp_sign == VP_SIGN_REPLY) // 处理主从告警查询包回包
                {
                    resp_err_flag = mcpA_rcv_data_proc(mcp_msg, &ucTemp, 0, &save_monitor);
                }
                else if(mcp_msg->cmd_type == MCP_CMD_TYPE_QUERY && mcp_msg->vp_sign == VP_SIGN_SEND)// 处理平台告警查询包
                {
                    if(t_dev_warn[mcp_msg->dev_id].tdd_or_fdd == IS_FDD)
                        ucTemp = vWarnGetStat(&t_dev_warn[mcp_msg->dev_id].t_warn_ch1_dw_inpow_less);
                    else
                        ucTemp = 0;
                    err_flag = mcpA_rcv_data_proc(mcp_msg, &ucTemp, 0, &save_monitor);
                }
                break;

            case MCP_ID_TDD_WARN_DL_IN_OP: // 4G TDD下行输入过功率告警
                if(mcp_msg->cmd_type == MCP_CMD_TYPE_RPT && mcp_msg->vp_sign == VP_SIGN_REPLY)  // 处理平台回复的告警确认包
                {
                    resp_err_flag = mcpA_rcv_data_proc(mcp_msg, &ucTemp, 0, &save_monitor);
                    if(resp_err_flag == MCP_ERR_OK)
                    {
                        //  处理告警确认
                        dev_clr_warn_state_after_rpt_success(mcp_msg->dev_id);
                    }
                }
                else if(mcp_msg->cmd_type == MCP_CMD_TYPE_QUERY && mcp_msg->vp_sign == VP_SIGN_REPLY) // 处理主从告警查询包回包
                {
                    resp_err_flag = mcpA_rcv_data_proc(mcp_msg, &ucTemp, 0, &save_monitor);
                }
                else if(mcp_msg->cmd_type == MCP_CMD_TYPE_QUERY && mcp_msg->vp_sign == VP_SIGN_SEND)// 处理平台告警查询包
                {
                    if(t_dev_warn[mcp_msg->dev_id].tdd_or_fdd == IS_TDD)
                        ucTemp = vWarnGetStat(&t_dev_warn[mcp_msg->dev_id].t_warn_ch1_dw_inpow_greater);
                    else
                        ucTemp = 0;
                    err_flag = mcpA_rcv_data_proc(mcp_msg, &ucTemp, 0, &save_monitor);
                }
                break;
 
            case MCP_ID_TDD_WARN_DL_IN_LP: // 4G TDD下行输入欠功率告警
                if(mcp_msg->cmd_type == MCP_CMD_TYPE_RPT && mcp_msg->vp_sign == VP_SIGN_REPLY)  // 处理平台回复的告警确认包
                {
                    resp_err_flag = mcpA_rcv_data_proc(mcp_msg, &ucTemp, 0, &save_monitor);
                    if(resp_err_flag == MCP_ERR_OK)
                    {
                        //  处理告警确认
                        dev_clr_warn_state_after_rpt_success(mcp_msg->dev_id);
                    }
                }
                else if(mcp_msg->cmd_type == MCP_CMD_TYPE_QUERY && mcp_msg->vp_sign == VP_SIGN_REPLY) // 处理主从告警查询包回包
                {
                    resp_err_flag = mcpA_rcv_data_proc(mcp_msg, &ucTemp, 0, &save_monitor);
                }
                else if(mcp_msg->cmd_type == MCP_CMD_TYPE_QUERY && mcp_msg->vp_sign == VP_SIGN_SEND)// 处理平台告警查询包
                {
                    if(t_dev_warn[mcp_msg->dev_id].tdd_or_fdd == IS_TDD)
                        ucTemp = vWarnGetStat(&t_dev_warn[mcp_msg->dev_id].t_warn_ch1_dw_inpow_less);
                    else
                        ucTemp = 0;
                    err_flag = mcpA_rcv_data_proc(mcp_msg, &ucTemp, 0, &save_monitor);
                }
                break;

            case MCP_ID_NR1_WARN_DL_IN_OP: // 5G NR1下行输入过功率告警
                if(mcp_msg->cmd_type == MCP_CMD_TYPE_RPT && mcp_msg->vp_sign == VP_SIGN_REPLY)  // 处理平台回复的告警确认包
                {
                    resp_err_flag = mcpA_rcv_data_proc(mcp_msg, &ucTemp, 0, &save_monitor);
                    if(resp_err_flag == MCP_ERR_OK)
                    {
                        //  处理告警确认
                        dev_clr_warn_state_after_rpt_success(mcp_msg->dev_id);
                    }
                }
                else if(mcp_msg->cmd_type == MCP_CMD_TYPE_QUERY && mcp_msg->vp_sign == VP_SIGN_REPLY) // 处理主从告警查询包回包
                {
                    resp_err_flag = mcpA_rcv_data_proc(mcp_msg, &ucTemp, 0, &save_monitor);
                }
                else if(mcp_msg->cmd_type == MCP_CMD_TYPE_QUERY && mcp_msg->vp_sign == VP_SIGN_SEND)// 处理平台告警查询包
                {
                    ucTemp = vWarnGetStat(&t_dev_warn[mcp_msg->dev_id].t_warn_ch2_dw_inpow_greater);
                    err_flag = mcpA_rcv_data_proc(mcp_msg, &ucTemp, 0, &save_monitor);
                }
                break;

            case MCP_ID_NR1_WARN_DL_IN_LP: // 5G NR1下行输入欠功率告警
                if(mcp_msg->cmd_type == MCP_CMD_TYPE_RPT && mcp_msg->vp_sign == VP_SIGN_REPLY)  // 处理平台回复的告警确认包
                {
                    resp_err_flag = mcpA_rcv_data_proc(mcp_msg, &ucTemp, 0, &save_monitor);
                    if(resp_err_flag == MCP_ERR_OK)
                    {
                        //  处理告警确认
                        dev_clr_warn_state_after_rpt_success(mcp_msg->dev_id);
                    }
                }
                else if(mcp_msg->cmd_type == MCP_CMD_TYPE_QUERY && mcp_msg->vp_sign == VP_SIGN_REPLY) // 处理主从告警查询包回包
                {
                    resp_err_flag = mcpA_rcv_data_proc(mcp_msg, &ucTemp, 0, &save_monitor);
                }
                else if(mcp_msg->cmd_type == MCP_CMD_TYPE_QUERY && mcp_msg->vp_sign == VP_SIGN_SEND)// 处理平台告警查询包
                {
                    ucTemp = vWarnGetStat(&t_dev_warn[mcp_msg->dev_id].t_warn_ch2_dw_inpow_less);
                    err_flag = mcpA_rcv_data_proc(mcp_msg, &ucTemp, 0, &save_monitor);
                }
                break;

            case MCP_ID_NR2_WARN_DL_IN_OP: // 5G NR2下行输入过功率告警
                if(mcp_msg->cmd_type == MCP_CMD_TYPE_RPT && mcp_msg->vp_sign == VP_SIGN_REPLY)  // 处理平台回复的告警确认包
                {
                    resp_err_flag = mcpA_rcv_data_proc(mcp_msg, &ucTemp, 0, &save_monitor);
                    if(resp_err_flag == MCP_ERR_OK)
                    {
                        //  处理告警确认
                        dev_clr_warn_state_after_rpt_success(mcp_msg->dev_id);
                    }
                }
                else if(mcp_msg->cmd_type == MCP_CMD_TYPE_QUERY && mcp_msg->vp_sign == VP_SIGN_REPLY) // 处理主从告警查询包回包
                {
                    resp_err_flag = mcpA_rcv_data_proc(mcp_msg, &ucTemp, 0, &save_monitor);
                }
                else if(mcp_msg->cmd_type == MCP_CMD_TYPE_QUERY && mcp_msg->vp_sign == VP_SIGN_SEND)// 处理平台告警查询包
                {
                    ucTemp = vWarnGetStat(&t_dev_warn[mcp_msg->dev_id].t_warn_ch3_dw_inpow_greater);
                    err_flag = mcpA_rcv_data_proc(mcp_msg, &ucTemp, 0, &save_monitor);
                }
                break;

            case MCP_ID_NR2_WARN_DL_IN_LP: // 5G NR2下行输入欠功率告警
                if(mcp_msg->cmd_type == MCP_CMD_TYPE_RPT && mcp_msg->vp_sign == VP_SIGN_REPLY)  // 处理平台回复的告警确认包
                {
                    resp_err_flag = mcpA_rcv_data_proc(mcp_msg, &ucTemp, 0, &save_monitor);
                    if(resp_err_flag == MCP_ERR_OK)
                    {
                        //  处理告警确认
                        dev_clr_warn_state_after_rpt_success(mcp_msg->dev_id);
                    }
                }
                else if(mcp_msg->cmd_type == MCP_CMD_TYPE_QUERY && mcp_msg->vp_sign == VP_SIGN_REPLY) // 处理主从告警查询包回包
                {
                    resp_err_flag = mcpA_rcv_data_proc(mcp_msg, &ucTemp, 0, &save_monitor);
                }
                else if(mcp_msg->cmd_type == MCP_CMD_TYPE_QUERY && mcp_msg->vp_sign == VP_SIGN_SEND)// 处理平台告警查询包
                {
                    ucTemp = vWarnGetStat(&t_dev_warn[mcp_msg->dev_id].t_warn_ch3_dw_inpow_less);
                    err_flag = mcpA_rcv_data_proc(mcp_msg, &ucTemp, 0, &save_monitor);
                }
                break;

            case MCP_ID_FDD_WARN_SELF_EXECITATION: // 自激告警
                if(mcp_msg->cmd_type == MCP_CMD_TYPE_RPT && mcp_msg->vp_sign == VP_SIGN_REPLY)  // 处理平台回复的告警确认包
                {
                    resp_err_flag = mcpA_rcv_data_proc(mcp_msg, &ucTemp, 0, &save_monitor);
                    if(resp_err_flag == MCP_ERR_OK)
                    {
                        //  处理告警确认
                        dev_clr_warn_state_after_rpt_success(mcp_msg->dev_id);
                    }
                }
                else if(mcp_msg->cmd_type == MCP_CMD_TYPE_QUERY && mcp_msg->vp_sign == VP_SIGN_REPLY) // 处理主从告警查询包回包
                {
                    resp_err_flag = mcpA_rcv_data_proc(mcp_msg, &ucTemp, 0, &save_monitor);
                }
                else if(mcp_msg->cmd_type == MCP_CMD_TYPE_QUERY && mcp_msg->vp_sign == VP_SIGN_SEND)// 处理平台告警查询包
                {
                    ucTemp = vWarnGetStat(&t_dev_warn[mcp_msg->dev_id].t_warn_iso);
                    err_flag = mcpA_rcv_data_proc(mcp_msg, &ucTemp, 0, &save_monitor);
                }
                break;
				
            case MCP_ID_MCP_MODE_A:	//设备监控软件运行模式 		
            	*(mcp_msg->ptr_data_unit + 3) = 0;
            	break;
            
            case MCP_ID_APC_MAX_LENGTH:	//AP:C协议最大长度 		
            	*(mcp_msg->ptr_data_unit + 3) = (uint8_t)(CONST_PDU_APCZDCD & 0x00FF); 
            	*(mcp_msg->ptr_data_unit + 4) = (uint8_t)(CONST_PDU_APCZDCD >> 8); 
            	break;		
            
            case MCP_ID_MCP_B_INTERACTION_MODE:   //MCP:B交互机制
            	*(mcp_msg->ptr_data_unit + 3) = CONST_PDU_MCPBJHJZ; 
                break;
                    
            case MCP_ID_NUM_OF_CONTINUOUS_PACKS:   //连发系数
            	*(mcp_msg->ptr_data_unit + 3) = CONST_PDU_LFXS;
                break;
                
            case MCP_ID_DEV_RESP_TIMEOUT: //设备响应超时
            	*(mcp_msg->ptr_data_unit + 3) = CONST_PDU_SBXYCS;
                break;
                
            case MCP_ID_TX_INTERVAL: //发送间隔时间
                *(mcp_msg->ptr_data_unit + 3) = (uint8_t)(CONST_PDU_FSJGSJ & 0x00FF); 
                *(mcp_msg->ptr_data_unit + 4) = (uint8_t)(CONST_PDU_FSJGSJ >> 8); 				
                break;
                
            case MCP_ID_OTA_PAUSE_TIMEOUT: //暂停传输等待时间
            	*(mcp_msg->ptr_data_unit + 3) = CONST_PDU_ZTDDSJ;
                break;
                
            case MCP_ID_OTA_MODE_TIMEOUT:   //监控模式/升级模式转换时间
                *(mcp_msg->ptr_data_unit + 3) = (uint8_t)(CONST_PDU_JKMSZHSJ & 0x00FF); 
                *(mcp_msg->ptr_data_unit + 4) = (uint8_t)(CONST_PDU_JKMSZHSJ >> 8); 			
                break;
            
            case MCP_ID_OTA_RESULT: //设备执行软件升级的结果 
            	*(mcp_msg->ptr_data_unit + 3) = mcpB_upgrade_result();
                break;			
			
            case MCP_ID_OTA_MODE:			
            	*(mcp_msg->ptr_data_unit + 3) = CONST_PDU_YCSJFS;
            	break;					
				

            default:
                *(mcp_msg->ptr_data_unit + 2) |= MCP_ERR_IDERR;
                err_flag = MCP_RESP_SIGN_ID_ERROR;
                break;
        }

        // 处理错误码
        if(ret == MCP_RESP_SIGN_OK && err_flag != MCP_RESP_SIGN_OK) {  // 设置当任何一个参数解析失败时，设返回值为错误码
            ret = err_flag;
        }
        else if(ret == MCP_RESP_SIGN_CONDITION && (err_flag != MCP_RESP_SIGN_OK && err_flag != MCP_RESP_SIGN_CONDITION))  // 其他错误码覆盖条件错误码,并退出循环
        {
            ret = err_flag;
            break;
        }

        // 移动指针到下一个数据块
        Num = Num + data_block_len;
        if((Num + 3) < mcp_msg->rx_buf_len)
        {
            mcp_msg->ptr_data_unit += data_block_len;
            if (*mcp_msg->ptr_data_unit < 4 || *mcp_msg->ptr_data_unit > 51) {
                ret = MCP_RESP_SIGN_LEN_ERROR;
                break;
            }
        }
    }

    if(save_monitor)
    {
        flash_dev_para_rw(WRITE); // 出厂参数
    }
    else
        ;

    return ret;
}



#elif (LOCAL_MODULE_TYPE == _RU)


/*****************************************************************************
 * @Name	  : mcpA_parase_msg_ru_dev
 * @Brief  : mcpA协议解析及处理,解析ru业务数据包
 * @Param  : pScr: 指向源数据
 *			pLen: 源数据长度
 *			pdu_cmd: PDU命令
 * @Retval : 执行结果
 * @Author :
 * @Data   : 2024-1-13
 * @Note   :
 *****************************************************************************/
uint8_t mcpA_parase_msg_ru_dev(MCP_MSG_T * mcp_msg)
{
    uint8_t ret = MCP_RESP_SIGN_OK, err_flag = MCP_RESP_SIGN_OK, resp_err_flag = MCP_RESP_SIGN_OK; // 错误标志
    int16_t ssTemp;                                                                                // 临时变量
    uint8_t ucTemp;                                                                                // 临时变量
    uint8_t arr_temp[32] = {0};                                                                    // 临时变量
    uint16_t data_id;                                                                              // 数据标识符
    uint16_t Num = 14;                                                                             // 有效的监控标识前有14个字节的数据
    uint8_t save_monitor = 0;
    uint8_t data_block_len = 0; // 数据块长度
    
    UNUSED_PARA(resp_err_flag);

    while((Num + 3) < mcp_msg->rx_buf_len)
    {
        data_block_len = *(mcp_msg->ptr_data_unit);
        data_id = (*(mcp_msg->ptr_data_unit + 1) & 0x00FF);
        data_id |= ((*(mcp_msg->ptr_data_unit + 2) << 8) & 0xFF00);

        // 获取监控码对应的数据类型,上下限,读写权限等信息
        get_MU_MCP_data_id_config(data_id, mcp_msg);

        switch(data_id)
        {
            //设备信息
            case MCP_ID_QUARY_ID_LIST: // 监控列表
                ssTemp = vMpRptParaList(mcp_msg->dev_id, mcp_msg->ptr_data_unit, data_id);
                if(ssTemp >= 5)
                    mcp_msg->tx_buf_len = ssTemp + 14; // 从起始字节到结束字节的总长度
                break;

            case MCP_ID_QUARY_PRIVATE_ID_LIST: // 自定义监控列表
                //XXX
                break;

            case MCP_ID_MANUFACTURER_CODE: // 厂商代码
                err_flag = mcpA_rcv_data_proc(mcp_msg, &tDevFactoryPara.factoryId, 20, &save_monitor);
                break;

            case MCP_ID_DEV_TYPE: // 设备类别
                err_flag = mcpA_rcv_data_proc(mcp_msg, &tDevFactoryPara.devType, 0, &save_monitor);
                break;

            case MCP_ID_DEVICE_MODEL: // 设备型号
                err_flag = mcpA_rcv_data_proc(mcp_msg, tDevFactoryPara.devMode, 20, &save_monitor);
                break;

            case MCP_ID_SN: // sn号
                err_flag = mcpA_rcv_data_proc(mcp_msg, tDevFactoryPara.sn, 30, &save_monitor);
                break;

            case MCP_ID_SW_VERSION: // 监控版本信息
                memcpy(arr_temp,SW_MON_VERSION,sizeof(SW_MON_VERSION));
                err_flag = mcpA_rcv_data_proc(mcp_msg, arr_temp, 20, &save_monitor);
                break;

            case MCP_ID_ROUTER_ADDRESS: // 设备路由登记地址
                err_flag = mcpA_rcv_data_proc(mcp_msg, &tDevPublicPara.devRoutingAddr, 0, &save_monitor);
                break;

            case MCP_ID_DEV_RESET: // 设备重启
                ucTemp = 0;
                err_flag = mcpA_rcv_data_proc(mcp_msg, &ucTemp, 0, &save_monitor);
                if(ucTemp == 1)
                {
                    reboot_set_egp_bit(RESET_SYSTEM);
                }
                break;

				
            case MCP_ID_MCP_MODE_A:			
            	*(mcp_msg->ptr_data_unit + 3) = 0;
            	break;
            
            case MCP_ID_APC_MAX_LENGTH:			
            	*(mcp_msg->ptr_data_unit + 3) = (uint8_t)(CONST_PDU_APCZDCD & 0x00FF); 
            	*(mcp_msg->ptr_data_unit + 4) = (uint8_t)(CONST_PDU_APCZDCD >> 8); 
            	break;		
            
            case MCP_ID_MCP_B_INTERACTION_MODE:   //MCP:B交互机制
            	*(mcp_msg->ptr_data_unit + 3) = CONST_PDU_MCPBJHJZ; 
                break;
                    
            case MCP_ID_NUM_OF_CONTINUOUS_PACKS:   //连发系数
            	*(mcp_msg->ptr_data_unit + 3) = CONST_PDU_LFXS;
                break;
                
            case MCP_ID_DEV_RESP_TIMEOUT: //设备响应超时
            	*(mcp_msg->ptr_data_unit + 3) = CONST_PDU_SBXYCS;
                break;
                
            case MCP_ID_TX_INTERVAL: //发送间隔时间
                *(mcp_msg->ptr_data_unit + 3) = (uint8_t)(CONST_PDU_FSJGSJ & 0x00FF); 
                *(mcp_msg->ptr_data_unit + 4) = (uint8_t)(CONST_PDU_FSJGSJ >> 8); 				
                break;
                
            case MCP_ID_OTA_PAUSE_TIMEOUT: //暂停传输等待时间
            	*(mcp_msg->ptr_data_unit + 3) = CONST_PDU_ZTDDSJ;
                break;
                
            case MCP_ID_OTA_MODE_TIMEOUT:   //监控模式/升级模式转换时间
                *(mcp_msg->ptr_data_unit + 3) = (uint8_t)(CONST_PDU_JKMSZHSJ & 0x00FF); 
                *(mcp_msg->ptr_data_unit + 4) = (uint8_t)(CONST_PDU_JKMSZHSJ >> 8); 			
                break;
				
            case MCP_ID_OTA_RESULT: //设备执行软件升级的结果 
            	*(mcp_msg->ptr_data_unit + 3) = mcpB_upgrade_result();
                break; 
 
            case MCP_ID_OTA_MODE:			
            	*(mcp_msg->ptr_data_unit + 3) = CONST_PDU_YCSJFS;
            	break;						
				
				
            // 采样参数

            // 设置参数
            

            default:
                *(mcp_msg->ptr_data_unit + OFFSET_MCPA_DATA_CODE_H) |= MCP_ERR_IDERR;
                err_flag = MCP_RESP_SIGN_ID_ERROR;
                break;
        }

        // 移动指针到下一个数据块
        Num = Num + data_block_len;
        if((Num + 3) < mcp_msg->rx_buf_len)
        {
            mcp_msg->ptr_data_unit += data_block_len;
        }
        // 处理错误码
        if(ret == MCP_RESP_SIGN_OK && err_flag != MCP_RESP_SIGN_OK)  // 设置当任何一个参数解析失败时，设返回值为错误码
            ret = err_flag;
        else if(ret == MCP_RESP_SIGN_CONDITION && (err_flag != MCP_RESP_SIGN_OK && err_flag != MCP_RESP_SIGN_CONDITION))  // 其他错误码覆盖条件错误码
        {
            ret = err_flag;
            break;
        }
    }

    if(save_monitor)
    {
        flash_dev_para_rw(WRITE); // 出厂参数
    }
    else
        ;

    return ret;
}

#endif
/*****************************************************************************
 * @Name	  : get_mcpA_msg_info
 * @Brief  : 接收监控信息初始化
 * @Param  : None
 * @Retval :
 * @Author :
 * @Data   : 2024-1-10
 * @Note   :
 *****************************************************************************/
void get_mcpA_msg_info(MCP_MSG_T * mcp_msg, uint8_t * rx_buf, uint16_t rx_buf_len, uint8_t msg_src)
{
    uint16_t temp = 0;
    memset(mcp_msg, 0, sizeof(MCP_MSG_T));

    // 包头参数提取
    mcp_msg->start = *(rx_buf);
    mcp_msg->ap_proto_type = *(rx_buf + ADDR_AP_PROTO_TYPE);
    mcp_msg->vp_proto_type = *(rx_buf + ADDR_VP_PROTO_TYPE);
    mcp_msg->site_id[0] = *(rx_buf + ADDR_VP_SITE_ID + 3);
    mcp_msg->site_id[1] = *(rx_buf + ADDR_VP_SITE_ID + 2);
    mcp_msg->site_id[2] = *(rx_buf + ADDR_VP_SITE_ID + 1);
    mcp_msg->site_id[3] = *(rx_buf + ADDR_VP_SITE_ID);
    mcp_msg->dev_id = *(rx_buf + ADDR_VP_DEVICE_ID);
    temp = *(uint16_t*)(rx_buf + ADDR_VP_PKT_NUM);
    mcp_msg->vp_pack_num = temp;
    mcp_msg->vp_sign = *(rx_buf + ADDR_VP_SIGN);
    mcp_msg->mcp_type = *(rx_buf + ADDR_MCP_PROTO_TYPE);
    mcp_msg->cmd_type = *(rx_buf + ADDR_MCP_CMD_TYPE);
    mcp_msg->mcp_resp_sign = *(rx_buf + ADDR_MCP_RESP_SIGN);
    mcp_msg->data_len = *(rx_buf + ADDR_MCP_DATA_FIELD);
    mcp_msg->rx_buf_len = rx_buf_len;
    mcp_msg->tx_buf_len = rx_buf_len;

    // 指向源数据中回包时需要修改的部分,用于填充回复标志等
    mcp_msg->ptr_src_buf = rx_buf;
    mcp_msg->ptr_site_resp = rx_buf + ADDR_VP_SITE_ID;
    mcp_msg->ptr_devid_resp = rx_buf + ADDR_VP_DEVICE_ID;
    mcp_msg->ptr_vp_resp = rx_buf + ADDR_VP_SIGN;
    mcp_msg->ptr_mcp_resp = (rx_buf + ADDR_MCP_RESP_SIGN);

    // 指向源数据中MCP数据块的起始位置,用于解析数据块
    mcp_msg->ptr_data_unit = rx_buf + ADDR_MCP_DATA_FIELD;
    mcp_msg->ptr_mcp_data_code = rx_buf + ADDR_MCP_DATA_FIELD + OFFSET_MCPA_DATA_CODE;

    mcp_msg->msg_src = msg_src;

    return;
}

/*****************************************************************************
 * @Name	  : mcpA_msg_handler
 * @Brief  : MCP_A协议发送消息处理
 * @Param  : rx_data:接收数据
 *           len:接收数据长度
 *           reply_data:回复数据
 * @Retval : 回复数据长度
 * @Author :
 * @Data   : 2024-1-10
 * @Note   :
 *****************************************************************************/
uint16_t mcpA_msg_handler(uint8_t * rx_data, uint16_t len, uint8_t * reply_data, uint8_t msg_src)
{
    uint8_t ret = 0;
    uint16_t crc_result;
    MCP_MSG_T mcp_msg;

    if(!len)
        return 0;

    // 解析MCP_A协议数据包的包头信息并填充mcp_msg结构体
    get_mcpA_msg_info(&mcp_msg, rx_data, len, msg_src);

    // 判断是否是MCP_A协议
    if(mcp_msg.mcp_type != MCP_A)
        return 0;

#if (_MU == LOCAL_MODULE_TYPE)                            // 本设备为主机,主机支持对本机和主从的查询等业务
    if(mcp_msg.dev_id == 0xFF || mcp_msg.dev_id == 0x00)  // 查本机,0xff设备id表示本机,0x00设备id表示主机设备
    {
        ret = mcpA_parase_msg_local_dev(&mcp_msg);
    }
    else if (1)//(get_ble_slave_index_by_dev_id(mcp_msg.dev_id) != 0xFF) //  主查从,先检查从机id合法性,是否在本设备的从机列表中
    {
        ret = mcpA_parase_msg_slave_dev(&mcp_msg);
    }
    else // id错误
    {
        rtt_printf(RTT_PC, RTT_CTRL_TEXT_BRIGHT_RED "Mcp device_id error:%d\r\n", mcp_msg.dev_id);
        return 0;
    }
#elif (_RU == LOCAL_MODULE_TYPE) // 本设备为从机,从机仅支持查本机,收到任何id都当做本机查询

    ret = mcpA_parase_msg_ru_dev(&mcp_msg);

#endif

    if (ret == MCP_RESP_SIGN_OK) {
        rtt_printf(RTT_CAT1, RTT_CTRL_TEXT_BRIGHT_YELLOW "McpA Rx: devid=%d, num=%d, ret=%d\r\n", mcp_msg.dev_id,mcp_msg.vp_pack_num,ret);
    } else {
        rtt_printf(RTT_CAT1, RTT_CTRL_TEXT_BRIGHT_RED "McpA Rx: devid=%d, num=%d, ret=%d\r\n", mcp_msg.dev_id,mcp_msg.vp_pack_num,ret);
    }

    // 当报文是回包时,不需要再回复
    if(mcp_msg.vp_sign == VP_SIGN_REPLY)
    {
        return 0;
    }

    // 设置回复数据标志
    *mcp_msg.ptr_mcp_resp = ret;
    *mcp_msg.ptr_vp_resp = VP_SIGN_REPLY;
    *mcp_msg.ptr_site_resp = tOmcPara.bs_id & 0xFF;
    *(mcp_msg.ptr_site_resp + 1) = (tOmcPara.bs_id >> 8) & 0xFF;
    *(mcp_msg.ptr_site_resp + 2) = (tOmcPara.bs_id >> 16) & 0xFF;
    *(mcp_msg.ptr_site_resp + 3) = (tOmcPara.bs_id >> 24) & 0xFF;

    crc_result = crc16((mcp_msg.ptr_src_buf + 1), mcp_msg.tx_buf_len - 4);

    mcp_msg.ptr_crc_L_resp = &mcp_msg.ptr_src_buf[mcp_msg.tx_buf_len - 3];
    mcp_msg.ptr_crc_H_resp = &mcp_msg.ptr_src_buf[mcp_msg.tx_buf_len - 2];

    *mcp_msg.ptr_crc_L_resp = UINT16_L(crc_result); // L位
    *mcp_msg.ptr_crc_H_resp = UINT16_H(crc_result); // H位

    memcpy(reply_data, mcp_msg.ptr_src_buf, mcp_msg.tx_buf_len); // 将发送数据存放缓存数组

    mcp_msg.tx_buf_len = MsgEscape(reply_data, mcp_msg.tx_buf_len); // 转义处理
    if (msg_src == MCP_MONITOR) {
        monitor_heart_beat_last_time_set(mcp_msg.dev_id);
    }
    return mcp_msg.tx_buf_len;
}
