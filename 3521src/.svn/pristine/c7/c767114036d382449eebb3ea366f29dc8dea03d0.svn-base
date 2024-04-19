/***************************** Copyright ****************************************
 *
 * (C) Copyright 2023, Shaanxi Tianji Communication
 *  All Rights Reserved
 *
 * FileName   : mcpMRC_user.c
 * Version    : none
 * Author     : none
 * Date       : 2023-06-20
 * Description: 主从通信协议的用户层代码实现
 *******************************************************************************/
#include "bsp_config.h"
#include "mcpMRC_user.h"
#include "mu_ru_comm_para_list.h"
#include "bsp_dev_flash.h"
#include "crc.h"
#include "bsp_reboot.h"
#include "bsp_dev_flash.h"
#include "bsp_dev_para.h"
#include "bsp_rf_global.h"
#include "ble.h"
#include "fpga.h"
#include "app_version.h"
/*---------------------- 宏定义  ---------------------------------------*/

/*---------------------- 结构体 ----------------------------------------*/

/*---------------------- 常量 ------------------------------------------*/

/*---------------------- 全局变量 --------------------------------------*/

/*---------------------- 声明 ------------------------------------------*/


/*---------------------- 常量 ------------------------------------------*/

/*---------------------- 全局变量 --------------------------------------*/
#if (LOCAL_MODULE_TYPE == _MU)

MRC_RU_DATA_T mrc_ru_data[BLE_CONNECT_MAX_NUM ]; // Mu Ru communication data,从设备数量 = RU + HUB

#elif (LOCAL_MODULE_TYPE == _RU)

MRC_RU_DATA_T mrc_ru_data[1]; // Mu Ru communication data

#endif
/*---------------------- 函数 ------------------------------------------*/


/*****************************************************************************
 * @Name	  : mcpMRC_init
 * @Brief  : 初始化mcpa监控协议数据结构
 * @Param  : 无
 * @Retval : 无
 * @Author :
 * @Data   : 2024-1-11
 * @Note   :
*/
void mcpMRC_init(void)
{
    uint8_t i = 0;
    for (i = 0; i < BLE_CONNECT_MAX_NUM; i++)
    {
        memset(&mrc_ru_data[i], 0, sizeof(MRC_RU_DATA_T)); // 初始化数据结构
#if (LOCAL_MODULE_TYPE == _MU)

        mrc_ru_data[i].ble_conn_id = i + 1; // 从设备ID
#endif
    }
}

/*****************************************************************************
 * @Name	  : get_ru_data_index_by_id
 * @Brief  : 根据从设备蓝牙ID获取蓝牙数组索引
 * @Param  : slv_ble_conn_id: 从设备蓝牙ID, 范围1-BLE_CONNECT_MAX_NUM
 * @Retval : 返回数据索引
 * @Author :
 * @Data   : 2024-1-11
 * @Note   :
*/
uint8_t get_mrc_ru_device_index_by_conn_id(uint8_t ble_conn_id)
{
    if(ble_conn_id >= BLE_CONNECT_MAX_NUM && ble_conn_id == 0)
        return 0xff;
    else
        return ble_conn_id - 1;
}
/*****************************************************************************
 * @Name	  : mcpMRC_rcv_data_proc
 * @Brief  : mcpa监控协议,接收对端发起包数据单元读写处理
 * @Param  : pt_msg: 指向收到的消息
 *			pt_param: 读本地数据时是入参,写本地数据时是出参
 *			strlen: 字符串长度,非字符串类型时此参数忽略,建议传0
 *			save: 指向保存监控数据的地址
 * @Retval : 返回错误码
 * @Author :
 * @Data   : 2024-1-11
 * @Note   :
 *****************************************************************************/

uint8_t mcpMRC_rcv_data_proc(MCP_MSG_T *pt_msg, void *pt_param, uint16_t strlen, uint8_t *save)
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
    switch (data_type)
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
    case MCP_UINT1:
        if(data_len != 1)
            return MCP_RESP_SIGN_LEN_ERROR;
        val = (uint8_t)(*pt_data);
        break;
        
    case MCP_INT1:
        if(data_len != 1)
            return MCP_RESP_SIGN_LEN_ERROR;
        val = (int8_t)(*pt_data);
        break;

    case MCP_INT2:
        if(data_len != 2)
            return MCP_RESP_SIGN_LEN_ERROR;
        val = (int16_t)H_AND_L(*(pt_data + 1), *pt_data);
        break;

    case MCP_UINT2:
        if(data_len != 2)
            return MCP_RESP_SIGN_LEN_ERROR;
        val = (uint16_t)H_AND_L(*(pt_data + 1), *pt_data);
        break;

    case MCP_INT4:
        if(data_len != 4)
            return MCP_RESP_SIGN_LEN_ERROR;
        val = (int32_t)(((*(pt_data + 3)) << 24) + ((*(pt_data + 2)) << 16) + ((*(pt_data + 1)) << 8) + (*pt_data));
        break;

    case MCP_UINT4:
        if(data_len != 4)
            return MCP_RESP_SIGN_LEN_ERROR;
        val = (uint32_t)(((*(pt_data + 3)) << 24) + ((*(pt_data + 2)) << 16) + ((*(pt_data + 1)) << 8) + (*pt_data));
        break;

    case MCP_STRING:
        val = 0;
        break;

    default:
        rtt_printf(RTT_BLE, RTT_CTRL_TEXT_BRIGHT_YELLOW "Mcp data_type error:%d\r\n", data_type);
        return MCP_RESP_SIGN_ID_ERROR;
    }

    // 根据包类型确定数据处理方式
    if (pt_msg->vp_sign == VP_SIGN_REPLY) // 当收到的是回复报文
    {
        // 回复包数据没有写操作,只有读
        data_operate = READ;
        // 处理回复包错误码,当回包错误码不为0时,不处理数据,直接返回错误
        resp_errcode = *pt_id_h & 0xf0;
        if(resp_errcode != MCP_ERR_OK)
            return MCP_RESP_SIGN_OTHER_ERROR;
    }
    else if (pt_msg->vp_sign == VP_SIGN_SEND) // 当收到的是发起报文
    {

        if (mcp_cmd == MCP_CMD_TYPE_QUERY) // 查询命令
        {
            // 当收查询时,写入数据
            data_operate = WRITE;
        }
        else if (mcp_cmd == MCP_CMD_TYPE_SET) // 设置命令
        {
            // 判断设置值范围是否合法,min和max都为0时,表示不限制范围
            if (((val < min) || (val > max)) && (min != 0 && max != 0))
            {
                *pt_id_h |= MCP_ERR_SET_VALUE_OVER;
                return MCP_RESP_SIGN_CONDITION;
            }
            // 当收到设置时,接收数据
            data_operate = READ;
            // 发起报文&&类型是设置&&读写标志为RW时,保存数据
            if (cmd_rw == MCP_RW)
                *save |= 1;
        }
        else if (mcp_cmd == MCP_CMD_TYPE_RPT) // 上报命令
        {
            // 当收上报时,接收数据
            data_operate = READ;
        }
        else
            ;
    }

    switch (data_type)
    {
    case MCP_BOOL:
        if (READ == data_operate)
        {
            *(uint8_t *)pt_param = (*((uint8_t *)pt_data ) == 0) ? 0 : 1;
        }
        else
        {
            *((uint8_t *)pt_data) = *(uint8_t *)pt_param == 0 ? 0 : 1;
        }
        break;

    case MCP_UINT1:
    case MCP_UINT2:
    case MCP_UINT4:
        if (READ == data_operate)
        {
            for (i = 0; i < data_len; i++)
                *((uint8_t *)pt_param + data_len - 1 - i) = *((uint8_t *)pt_data + (data_len - 1 - i));
        }
        else
        {
            for (i = 0; i < data_len; i++)
                *((uint8_t *)pt_data + (data_len - 1 - i)) = *((uint8_t *)pt_param + data_len - 1 - i);
        }
        break;

    case MCP_INT1:
    case MCP_INT2:
    case MCP_INT4:
        if (READ == data_operate)
        {
            for (i = 0; i < data_len; i++)
                *((int8_t *)pt_param + data_len - 1 - i) = *((int8_t *)pt_data + (data_len - 1 - i));
        }
        else
        {
            for (i = 0; i < data_len; i++)
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

/*****************************************************************************
 * @Name    : update_mrc_pkt_send_stat
 * @Brief  : 更新数据接收状态
 * @Param  : pt_msg: 指向收到的消息
 * @Retval : 无
 * @Author :
 * @Data   : 2024-1-13
 * @Note   : 主设备收到从机回复包时,如果有发包方在等待信息,发包信息和收包的包序号一致,更新数据接收状态,用于发包方查询
*/
void update_mrc_pkt_send_stat(MCP_MSG_T *mcp_msg)
{
    uint8_t data_index;
    if(mcp_msg->dev_id != 0)
        data_index = get_mrc_ru_device_index_by_conn_id(mcp_msg->dev_id);
    else
        data_index = 0;
    
    if(data_index!= 0xff && mcp_msg->vp_sign == VP_SIGN_REPLY && mcp_msg->mcp_resp_sign == MCP_RESP_SIGN_OK && mrc_ru_data[data_index].pkt_send_stat.mrc_pkt_id == mcp_msg->vp_pack_num)
    {
        mrc_ru_data[data_index].pkt_send_stat.mrc_pkt_send_stat = MRC_DATA_RECV_OK;
        rtt_printf(RTT_BLE, RTT_CTRL_TEXT_BRIGHT_GREEN "Mrc data recv ok,dev_id:%d,mrc_pkt_id:%d\r\n", mcp_msg->dev_id, mcp_msg->vp_pack_num);
    }else if(data_index!= 0xff && mcp_msg->vp_sign == VP_SIGN_REPLY && mcp_msg->mcp_resp_sign != MCP_RESP_SIGN_OK && mrc_ru_data[data_index].pkt_send_stat.mrc_pkt_id == mcp_msg->vp_pack_num)
    {
        mrc_ru_data[data_index].pkt_send_stat.mrc_pkt_send_stat = MRC_DATA_RECV_ERR;
        rtt_printf(RTT_BLE, RTT_CTRL_TEXT_BRIGHT_RED "Mrc data recv err,dev_id:%d,mrc_pkt_id:%d,err_code:%d\r\n", mcp_msg->dev_id, mcp_msg->vp_pack_num, mcp_msg->mcp_resp_sign);
    }
}

#if (LOCAL_MODULE_TYPE == _MU)



/*****************************************************************************
 * @Name    : mcpA_msg_parase_and_handle
 * @Brief  : mcpA协议解析及处理,解析MU本机业务数据包
 * @Param  : pScr: 指向源数据
 *			pLen: 源数据长度
 *			pdu_cmd: PDU命令
 * @Retval : 执行结果
 * @Author :
 * @Data   : 2024-1-13
 * @Note   :主机:
 *          从机发来的查询,丢掉
 *          从机发来的查询回包,保存数据
 *          从机发来的设置,丢掉
 *          从机发来的设置回包,丢掉
 *          从机发来的上报数据,保存数据
 *          从机发来的上报回包,丢掉
 *          
 *          总结下,主从通信里:
 *          从机发来的包,只处理上报数据,其他包丢掉
 *          从机回复的包,只处理查询回包存数据,其他包查看错误码即可
 *****************************************************************************/
uint8_t mcp_mrc_mu_parase_msg(MCP_MSG_T *mcp_msg)
{
    uint8_t ret = MCP_RESP_SIGN_OK, err_flag = MCP_RESP_SIGN_OK; // 错误标志
    int16_t ssTemp;                                                                                // 临时变量
    uint8_t ucTemp;                                                                                // 临时变量
    uint16_t data_id;                                                                              // 数据标识符
    uint16_t Num = 14;                                                                             // 有效的监控标识前有14个字节的数据
    uint8_t save_monitor = 0;
    uint8_t data_block_len = 0; // 数据块长度
    uint8_t ru_index;
    
    (void)ssTemp; //避免编译报警
    (void)ucTemp;

    // 检查是否在等待此回包,更新数据接收状态
    if(mcp_msg->vp_sign == VP_SIGN_REPLY)
        update_mrc_pkt_send_stat(mcp_msg);
    if((mcp_msg->dev_id == 0 || mcp_msg->dev_id > BLE_CONNECT_MAX_NUM) )
    {
        return MCP_RESP_SIGN_OTHER_ERROR;
    }

    ru_index = get_mrc_ru_device_index_by_conn_id(mcp_msg->dev_id);

    if(mcp_msg->cmd_type == MCP_CMD_TYPE_RPT  && mcp_msg->vp_sign == VP_SIGN_SEND ) //收到从机上报消息,处理
    {
        while ((Num + 3) < mcp_msg->rx_buf_len)
        {
            data_block_len = *(mcp_msg->ptr_data_unit);
            data_id = (*(mcp_msg->ptr_data_unit + 1) & 0x00FF);
            data_id |= ((*(mcp_msg->ptr_data_unit + 2) << 8) & 0xFF00);

            // 获取监控码对应的数据类型,上下限,读写权限等信息
            get_MRC_data_id_config( data_id, mcp_msg);

            switch (data_id)
            {
            case MRC_UL_SILENT: // 从机上行静默状态上报
                err_flag = mcpMRC_rcv_data_proc(mcp_msg, &mrc_ru_data[ru_index].mrc_silent, 0, &save_monitor);
                break;

            default:
                *(mcp_msg->ptr_data_unit + OFFSET_MCPA_DATA_CODE_H) |= MCP_ERR_IDERR;
                err_flag = MCP_RESP_SIGN_ID_ERROR;
                break;
            }

            // 移动指针到下一个数据块
            Num = Num + data_block_len;
            if ((Num + 3) < mcp_msg->rx_buf_len)
            {
                mcp_msg->ptr_data_unit += data_block_len;
            }
            // 处理错误码
            if (ret == MCP_RESP_SIGN_OK && err_flag != MCP_RESP_SIGN_OK) // 设置当任何一个参数解析失败时，设返回值为错误码
                ret = err_flag;
            else if (ret == MCP_RESP_SIGN_CONDITION && (err_flag != MCP_RESP_SIGN_OK && err_flag != MCP_RESP_SIGN_CONDITION)) // 其他错误码覆盖条件错误码
            {
                ret = err_flag;
                break;
            }
        }
    }else if( mcp_msg->cmd_type == MCP_CMD_TYPE_QUERY && mcp_msg->vp_sign == VP_SIGN_REPLY) //主查从查询报文回包
    {
        while ((Num + 3) < mcp_msg->rx_buf_len)
        {
            data_block_len = *(mcp_msg->ptr_data_unit);
            data_id = (*(mcp_msg->ptr_data_unit + 1) & 0x00FF);
            data_id |= ((*(mcp_msg->ptr_data_unit + 2) << 8) & 0xFF00);

            // 获取监控码对应的数据类型,上下限,读写权限等信息
            get_MRC_data_id_config( data_id, mcp_msg);

            switch (data_id)
            {
            case MRC_TIME_SYNC: // 厂商代码
                err_flag = mcpMRC_rcv_data_proc(mcp_msg, &mrc_ru_data[ru_index].mcu_sync_time, 0, &save_monitor);
                break;
                
            case MRC_DEV_TYPE: // 厂商代码
                err_flag = mcpMRC_rcv_data_proc(mcp_msg, &mrc_ru_data[ru_index].device_type, 0, &save_monitor);
                break;

            case MRC_RUN_TIME: // 运行时间
                err_flag = mcpMRC_rcv_data_proc(mcp_msg, &mrc_ru_data[ru_index].mcu_run_time, 0, &save_monitor);
                break;
                
            case MRC_HUB_CH1_OPEN_TIME: // HUB通道1开启时间查询
                err_flag = mcpMRC_rcv_data_proc(mcp_msg, &mrc_ru_data[ru_index].hub_ch1_open_time, 0, &save_monitor);
                break;
                
            case MRC_HUB_CH2_OPEN_TIME: // HUB通道2开启时间查询
                err_flag = mcpMRC_rcv_data_proc(mcp_msg, &mrc_ru_data[ru_index].hub_ch2_open_time, 0, &save_monitor);
                break;
                
            case MRC_HUB_CH3_OPEN_TIME: // HUB通道3开启时间查询
                err_flag = mcpMRC_rcv_data_proc(mcp_msg, &mrc_ru_data[ru_index].hub_ch3_open_time, 0, &save_monitor);
                break;
                
            case MRC_HUB_CH4_OPEN_TIME: // HUB通道4开启时间查询
                err_flag = mcpMRC_rcv_data_proc(mcp_msg, &mrc_ru_data[ru_index].hub_ch4_open_time, 0, &save_monitor);
                break;
                
            case MRC_HUB_CH5_OPEN_TIME: // HUB通道5开启时间查询
                err_flag = mcpMRC_rcv_data_proc(mcp_msg, &mrc_ru_data[ru_index].hub_ch5_open_time, 0, &save_monitor);
                break;
                
            case MRC_HUB_CH6_OPEN_TIME: // HUB通道6开启时间查询
                err_flag = mcpMRC_rcv_data_proc(mcp_msg, &mrc_ru_data[ru_index].hub_ch6_open_time, 0, &save_monitor);
                break;
                
            case MRC_HUB_CH7_OPEN_TIME: // HUB通道7开启时间查询
                err_flag = mcpMRC_rcv_data_proc(mcp_msg, &mrc_ru_data[ru_index].hub_ch7_open_time, 0, &save_monitor);
                break;
                
            case MRC_HUB_CH8_OPEN_TIME: // HUB通道8开启时间查询
                err_flag = mcpMRC_rcv_data_proc(mcp_msg, &mrc_ru_data[ru_index].hub_ch8_open_time, 0, &save_monitor);
                break;

            case MRC_HUB_CH1_OPEN: // HUB通道1开关
                err_flag = mcpMRC_rcv_data_proc(mcp_msg, &mrc_ru_data[ru_index].hub_ch1_open, 0, &save_monitor);
                break;
                
            case MRC_HUB_CH2_OPEN: // HUB通道2开关
                err_flag = mcpMRC_rcv_data_proc(mcp_msg, &mrc_ru_data[ru_index].hub_ch2_open, 0, &save_monitor);
                break;
                
            case MRC_HUB_CH3_OPEN: // HUB通道3开关
                err_flag = mcpMRC_rcv_data_proc(mcp_msg, &mrc_ru_data[ru_index].hub_ch3_open, 0, &save_monitor);
                break;
                
            case MRC_HUB_CH4_OPEN: // HUB通道4开关
                err_flag = mcpMRC_rcv_data_proc(mcp_msg, &mrc_ru_data[ru_index].hub_ch4_open, 0, &save_monitor);
                break;
                
            case MRC_HUB_CH5_OPEN: // HUB通道5开关
                err_flag = mcpMRC_rcv_data_proc(mcp_msg, &mrc_ru_data[ru_index].hub_ch5_open, 0, &save_monitor);
                break;
                
            case MRC_HUB_CH6_OPEN: // HUB通道6开关
                err_flag = mcpMRC_rcv_data_proc(mcp_msg, &mrc_ru_data[ru_index].hub_ch6_open, 0, &save_monitor);
                break;
                
            case MRC_HUB_CH7_OPEN: // HUB通道7开关
                err_flag = mcpMRC_rcv_data_proc(mcp_msg, &mrc_ru_data[ru_index].hub_ch7_open, 0, &save_monitor);
                break;
                
            case MRC_HUB_CH8_OPEN: // HUB通道8开关
                err_flag = mcpMRC_rcv_data_proc(mcp_msg, &mrc_ru_data[ru_index].hub_ch8_open, 0, &save_monitor);
                break;

            case MRC_MCU_FW_VERSION: // MCU固件版本
                err_flag = mcpMRC_rcv_data_proc(mcp_msg, &mrc_ru_data[ru_index].mcu_sw_ver_u32, 0, &save_monitor);
                break;
                
            case MRC_BLE_FW_VERSION: // BLE固件版本
                err_flag = mcpMRC_rcv_data_proc(mcp_msg, &mrc_ru_data[ru_index].ble_sw_ver, 0, &save_monitor);
                break;
                
            case MRC_FPGA_FW_VERSION: // FPGA固件版本
                err_flag = mcpMRC_rcv_data_proc(mcp_msg, &mrc_ru_data[ru_index].fpga_sw_ver, 0, &save_monitor);
                break;
                
            case MRC_8668_FW1_VERSION: // 8668固件1版本
                err_flag = mcpMRC_rcv_data_proc(mcp_msg, &mrc_ru_data[ru_index].ecr8668_fw1_ver, 0, &save_monitor);
                break;
                
            case MRC_8668_FW2_VERSION: // 8668固件2版本
                err_flag = mcpMRC_rcv_data_proc(mcp_msg, &mrc_ru_data[ru_index].ecr8668_fw2_ver, 0, &save_monitor);
                break;
                
            case MRC_8668_FW3_VERSION: // 8668固件3版本
                err_flag = mcpMRC_rcv_data_proc(mcp_msg, &mrc_ru_data[ru_index].ecr8668_fw3_ver, 0, &save_monitor);
                break;
                
            case MRC_8668_FW4_VERSION: // 8668固件4版本
                err_flag = mcpMRC_rcv_data_proc(mcp_msg, &mrc_ru_data[ru_index].ecr8668_fw4_ver, 0, &save_monitor);
                break;
                
            case MRC_8668_FW5_VERSION: // 8668固件5版本
                err_flag = mcpMRC_rcv_data_proc(mcp_msg, &mrc_ru_data[ru_index].ecr8668_fw5_ver, 0, &save_monitor);
                break;
                
            case MRC_LTE_IN_PWR: // LTE输入功率查询
                err_flag = mcpMRC_rcv_data_proc(mcp_msg, &mrc_ru_data[ru_index].mrc_lte_in_pwr, 0, &save_monitor);
                break;
                
            case MRC_NR_IN_PWR: //  NR输入功率查询
                err_flag = mcpMRC_rcv_data_proc(mcp_msg, &mrc_ru_data[ru_index].mrc_nr_in_pwr, 0, &save_monitor);
                break;
                
            case MRC_NR2_IN_PWR: // NR2输入功率查询
                err_flag = mcpMRC_rcv_data_proc(mcp_msg, &mrc_ru_data[ru_index].mrc_nr2_in_pwr, 0, &save_monitor);
                break;
                
            case MRC_SELF_EXCITED: // 自激查询
                err_flag = mcpMRC_rcv_data_proc(mcp_msg, &mrc_ru_data[ru_index].mrc_self_excited, 0, &save_monitor);
                break;
            
            case MRC_SYSPC_SWITCH: // 系统功控开关
                break;
                
            case MRC_SYSPC_LTE_UL_IN_EXPECT: // 系统功控LTE上行期望接收功率
                break;
                
            case MRC_SYSPC_LTE_DL_OUT: // 系统功控LTE发射功率
                break;
                
            case MRC_SYSPC_NR_UL_IN_EXPECT: // 系统功控NR上行期望接收功率
                break;
                
            case MRC_SYSPC_NR_DL_OUT: // 系统功控NR发射功率
                break;
                
            case MRC_RU_CH_CLOSE: // 从机射频通道关闭 1:LTE 2:NR 3:NRMIMO
                break;
                
            case MRC_RU_CH_OPEN: // 从机射频通道开启 1:LTE 2:NR 3:NRMIMO
                break;

            default:
                *(mcp_msg->ptr_data_unit + OFFSET_MCPA_DATA_CODE_H) |= MCP_ERR_IDERR;
                err_flag = MCP_RESP_SIGN_ID_ERROR;
                break;
            }
            // 处理错误码
            if (ret == MCP_RESP_SIGN_OK && err_flag != MCP_RESP_SIGN_OK) // 设置当任何一个参数解析失败时，设返回值为错误码
                ret = err_flag;
            else if (ret == MCP_RESP_SIGN_CONDITION && (err_flag != MCP_RESP_SIGN_OK && err_flag != MCP_RESP_SIGN_CONDITION)) // 其他错误码覆盖条件错误码
            {
                ret = err_flag;
                break;
            }

            // 移动指针到下一个数据块
            Num = Num + data_block_len;
            if ((Num + 3) < mcp_msg->rx_buf_len)
            {
                mcp_msg->ptr_data_unit += data_block_len;
            }
        }
    }else if ( mcp_msg->cmd_type == MCP_CMD_TYPE_SET )
    {
        ; // 从机对主机的设置,不处理,主对从设置回包,不处理
    }else 
        ; // 其他命令类型,丢掉

    if (save_monitor)
    {
        flash_dev_para_rw(WRITE); // 出厂参数
    }
    else
        ;

    return ret;
}

#elif (LOCAL_MODULE_TYPE == _RU)
/*****************************************************************************
 * @Name   : mcp_mrc_ru_parase_msg
 * @Brief  : mcpA协议解析及处理,解析从机本机业务数据包
 * @Param  : pScr: 指向源数据
 *			pLen: 源数据长度
 *			pdu_cmd: PDU命令
 * @Retval : 执行结果
 * @Author :
 * @Data   : 2024-1-13
 * @Note   :从机:
 *          主机发来的查询,回复
 *          主机发来的设置,执行
 *          主机发来的上报回复,检查包错误码
 *          其他类型的包,丢掉
 *          
 *          总结下,主从通信里:
 *          主机发来的包,处理设置查询
 *          主机回复的包,查看上报回包存数据错误码
 *          从机不用关心设备编号,任何编号都按给本机的处理
 *****************************************************************************/
uint8_t mcp_mrc_ru_parase_msg(MCP_MSG_T *mcp_msg)
{
    uint8_t ret = MCP_RESP_SIGN_OK, err_flag = MCP_RESP_SIGN_OK; // 错误标志
    uint8_t scTemp = 0;                                                                            // 临时变量;                                                                                // 临时变量
    int16_t ssTemp = 0;                                                                                // 临时变量
    uint8_t ucTemp = 0;                                                                            // 临时变量;                                                                                // 临时变量                                                 // 有效的监控标识前有14个字节的数据
    uint32_t u32Temp = 0, ru_sys_tick = 0;                                                                          // 临时变量
    int32_t i32Temp = 0;                                                                           // 临时变量
    uint16_t data_id;                                                                              // 数据标识符
    uint16_t Num = 14;                            
    uint8_t save_monitor = 0;
    uint8_t data_block_len = 0; // 数据块长度
    
    UNUSED_PARA(scTemp); //避免编译报警
    UNUSED_PARA(ssTemp); //避免编译报警
    UNUSED_PARA(ucTemp); //避免编译报警
    UNUSED_PARA(i32Temp); //避免编译报警

    if(mcp_msg->vp_sign == VP_SIGN_SEND)
    {
        while ((Num + 3) < mcp_msg->rx_buf_len)
        {
            data_block_len = *(mcp_msg->ptr_data_unit);
            data_id = (*(mcp_msg->ptr_data_unit + 1) & 0x00FF);
            data_id |= ((*(mcp_msg->ptr_data_unit + 2) << 8) & 0xFF00);

            // 获取监控码对应的数据类型,上下限,读写权限等信息
            get_MRC_data_id_config( data_id, mcp_msg);

            switch (data_id)
            {
            case MRC_TIME_SYNC: // 对时设置/查询
                err_flag = mcpMRC_rcv_data_proc(mcp_msg, &u32Temp, 0, &save_monitor);
                // 对时处理
                ru_sys_tick = hi_get_tick();
                mrc_ru_data[0].sync_time_offset = u32Temp - ru_sys_tick;
                break;
                
            case MRC_DEV_TYPE: // 设备类型
                ucTemp = _RU;
                err_flag = mcpMRC_rcv_data_proc(mcp_msg, &(ucTemp), 0, &save_monitor);
                break;

            case MRC_RUN_TIME: // 运行时间
                // 读取对时差值,上报给主机
                err_flag = mcpMRC_rcv_data_proc(mcp_msg, &mrc_ru_data[0].sync_time_offset, 0, &save_monitor);
                break;

            case MRC_MCU_FW_VERSION: // MCU固件版本
                u32Temp = SW_UPGRADE;
                err_flag = mcpMRC_rcv_data_proc(mcp_msg, &u32Temp, 0, &save_monitor);
                // 软件版本问题, 软件版本是字符串,而升级文件中的软件版本是个uint32
                break;
                
            case MRC_BLE_FW_VERSION: // BLE固件版本
                // 读取数据
                u32Temp = ble_get_version();
                err_flag = mcpMRC_rcv_data_proc(mcp_msg, &u32Temp, 0, &save_monitor);
                break;
                
            case MRC_FPGA_FW_VERSION: // FPGA固件版本
                // 读取数据
                u32Temp = fpga_ver_time();
                err_flag = mcpMRC_rcv_data_proc(mcp_msg, &u32Temp, 0, &save_monitor);
                break;
                
            case MRC_8668_FW1_VERSION: // 8668固件1 版本
                // 读取数据,从哪读?
                u32Temp = 0xbeefbeef;
                err_flag = mcpMRC_rcv_data_proc(mcp_msg, &u32Temp, 0, &save_monitor);
                break;
                
            case MRC_8668_FW2_VERSION: // 8668固件2 版本
                // 读取数据,从哪读?
                u32Temp = 0xbeefbeef;
                err_flag = mcpMRC_rcv_data_proc(mcp_msg, &u32Temp, 0, &save_monitor);
                break;
                
            case MRC_8668_FW3_VERSION: // 8668固件3 版本
                // 读取数据,从哪读?
                u32Temp = 0xbeefbeef;
                err_flag = mcpMRC_rcv_data_proc(mcp_msg, &u32Temp, 0, &save_monitor);
                break;

            case MRC_8668_FW4_VERSION: // 8668固件4 版本
                // 读取数据,从哪读?
                u32Temp = 0xbeefbeef;
                err_flag = mcpMRC_rcv_data_proc(mcp_msg, &u32Temp, 0, &save_monitor);
                break;

            case MRC_8668_FW5_VERSION: // 8668固件5 版本
                // 读取数据,从哪读?
                u32Temp = 0xbeefbeef;
                err_flag = mcpMRC_rcv_data_proc(mcp_msg, &u32Temp, 0, &save_monitor);
                break;
                
            case MRC_LTE_IN_PWR: // LTE下行输入功率
                ucTemp = t_rf_sys[ RF_SYS_1 ].t_ch[RF_CH_DW].tPow.scInPow;
                err_flag = mcpMRC_rcv_data_proc(mcp_msg, &ucTemp, 0, &save_monitor);
                break;
                
            case MRC_NR_IN_PWR: // 5G通道1下行输入功率
                ucTemp = t_rf_sys[ RF_SYS_2 ].t_ch[RF_CH_DW].tPow.scInPow;
                err_flag = mcpMRC_rcv_data_proc(mcp_msg, &ucTemp, 0, &save_monitor);
                break;
                
            case MRC_NR2_IN_PWR: // 5G通道2下行输入功率
                ucTemp = t_rf_sys[ RF_SYS_3 ].t_ch[RF_CH_DW].tPow.scInPow;
                err_flag = mcpMRC_rcv_data_proc(mcp_msg, &ucTemp, 0, &save_monitor);
                break;
                
            case MRC_SELF_EXCITED: // 自激状态
                //TODO 读取自激状态,从哪读?
                break;
                
            case MRC_SYSPC_SWITCH: // 系统功控开关
                err_flag = mcpMRC_rcv_data_proc(mcp_msg, &mrc_ru_data[0].mrc_pc_switch, 0, &save_monitor);
                break;
                
            case MRC_SYSPC_LTE_UL_IN_EXPECT: // 系统功控LTE上行期望接收功率
                err_flag = mcpMRC_rcv_data_proc(mcp_msg, &mrc_ru_data[0].mrc_lte_ul_pwr_expect, 0, &save_monitor);
                break;
                
            case MRC_SYSPC_LTE_DL_OUT: // 系统功控LTE发射功率
                err_flag = mcpMRC_rcv_data_proc(mcp_msg, &mrc_ru_data[0].mrc_lte_tx_pwr, 0, &save_monitor);
                break;
                
            case MRC_SYSPC_NR_UL_IN_EXPECT: // 系统功控NR上行期望接收功率
                err_flag = mcpMRC_rcv_data_proc(mcp_msg, &mrc_ru_data[0].mrc_nr_ul_pwr_expect, 0, &save_monitor);
                break;
                
            case MRC_SYSPC_NR_DL_OUT: // 系统功控NR发射功率
                err_flag = mcpMRC_rcv_data_proc(mcp_msg, &mrc_ru_data[0].mrc_nr_tx_pwr, 0, &save_monitor);
                break;
                
            case MRC_RU_CH_CLOSE: // 从机射频通道关闭 RF_BAND_E
                err_flag = mcpMRC_rcv_data_proc(mcp_msg, &ucTemp, 0, &save_monitor);
                if(ucTemp == BAND_3 || ucTemp == BAND_8 || ucTemp == BAND_39 || ucTemp == BAND_40) // LTE关闭
                {

                }else if(ucTemp == BAND_41)
                {

                }else if(ucTemp == BAND_41M)
                {

                }
                break;
                
            case MRC_RU_CH_OPEN: // 从机射频通道开启 1:LTE 2:NR 3:NRMIMO
                break;

            default:
                *(mcp_msg->ptr_data_unit + OFFSET_MCPA_DATA_CODE_H) |= MCP_ERR_IDERR;
                err_flag = MCP_RESP_SIGN_ID_ERROR;
                break;
            }
            // 处理错误码
            if (ret == MCP_RESP_SIGN_OK && err_flag != MCP_RESP_SIGN_OK) // 设置当任何一个参数解析失败时，设返回值为错误码
                ret = err_flag;
            else if (ret == MCP_RESP_SIGN_CONDITION && (err_flag != MCP_RESP_SIGN_OK && err_flag != MCP_RESP_SIGN_CONDITION)) // 其他错误码覆盖条件错误码
                {
                    ret = err_flag;
                    break;
                }

            // 移动指针到下一个数据块
            Num = Num + data_block_len;
            if ((Num + 3) < mcp_msg->rx_buf_len)
            {
                mcp_msg->ptr_data_unit += data_block_len;
            }
        }
    }else if(mcp_msg->vp_sign == VP_SIGN_REPLY)
    {
        //  当收到回复包时,如果有发包方在等待信息,发包信息和收包的包序号一致,更新数据接收状态,用于发包方查询
        update_mrc_pkt_send_stat(mcp_msg);
    }


    if (save_monitor)
    {
        flash_dev_para_rw(WRITE); // 出厂参数
    }
    else
        ;

    return ret;
}

#endif
/*****************************************************************************
 * @Name	  : get_mcpMRC_msg_info
 * @Brief  : 接收监控信息初始化
 * @Param  : None
 * @Retval :
 * @Author :
 * @Data   : 2024-1-10
 * @Note   :
 *****************************************************************************/
void get_mcpMRC_msg_info(MCP_MSG_T *mcp_msg, uint8_t *rx_buf, uint16_t rx_buf_len, uint8_t msg_src)
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
    mcp_msg->ptr_vp_resp = rx_buf + ADDR_VP_SIGN;
    mcp_msg->ptr_mcp_resp = (rx_buf + ADDR_MCP_RESP_SIGN);

    // 指向源数据中MCP数据块的起始位置,用于解析数据块
    mcp_msg->ptr_data_unit = rx_buf + ADDR_MCP_DATA_FIELD;
    mcp_msg->ptr_mcp_data_code = rx_buf + ADDR_MCP_DATA_FIELD + OFFSET_MCPA_DATA_CODE;

    mcp_msg->msg_src = msg_src;

    return;
}


#if (LOCAL_MODULE_TYPE == _MU)
/*****************************************************************************
 * @Name	  : new_mrc_send_pkt
* @Brief  : 初始化一个发送报文,用于主设备发给从设备的包,支持mcp A B C MRC
* @Param  : pkt_buf: 发送数据缓存区
* @Param  : ble_conn_id: 主从通信的设备ID
* @Param  : mcp_proto_type: MCP协议类型
* @Param  : cmd_type: 命令类型
* @Retval : None
* @Author :
* @Data   : 2024-1-10
* @Note   :
*/
void new_mrc_send_pkt(mcp_send_pkt_t *pkt_buf, uint8_t ble_conn_id, uint8_t mcp_proto_type, uint8_t cmd_type)
{
    MCP_tx_packet_header_make(pkt_buf, mcp_proto_type, ble_conn_id, cmd_type);
}
/*****************************************************************************
 * @Name	  : add_mrc_pkt_data
* @Brief  : 添加数据块到发送报文,用于主设备发给从设备的包,支持mcp A B C MRC
* @Param  : pkt_buf: 发送数据缓存区
* @Param  : code: 数据ID
* @Param  : para_buf: 数据值
* @Param  : strlen: 数据长度
* @Retval : None
* @Author :
* @Data   : 2024-1-10
* @Note   :
*/
Status_t add_mrc_pkt_data(mcp_send_pkt_t *pkt_buf, uint16_t code, uint8_t *para_buf, uint8_t strlen)
{
    MCP_MSG_T mcp_msg;

    if(pkt_buf == NULL)
        return STAT_ERROR;

    memset(&mcp_msg, 0, sizeof(MCP_MSG_T));

    if(pkt_buf->mcp_proto_type == MCP_MRC)
    {
        if(get_MRC_data_id_config( code, &mcp_msg) != STAT_OK)
            return STAT_ERROR;
    }
    else if(pkt_buf->mcp_proto_type == MCP_A)
    {
        if(get_RU_MCP_data_id_config( code, &mcp_msg) != STAT_OK)
            return STAT_ERROR;
    }
    else if(pkt_buf->mcp_proto_type == MCP_B)
    {
        MCP_B_tx_packet_data_add(pkt_buf, strlen, MCP_STRING, code, para_buf);
        return STAT_OK;		
    }	
    else
        return STAT_ERROR;

    MCP_A_MRC_tx_packet_data_add(pkt_buf, strlen, mcp_msg.mcp_data_type, code, para_buf);
    return STAT_OK;
}


/*****************************************************************************
* @Name	  : mrc_send_pkt_to_ru
* @Brief  : 发送数据包给从设备,支持mcp A B C MRC
* @Param  : pkt_buf: 发送数据缓存区
* @Param  : ble_conn_id: 主从通信的从机ID
* @Retval : None
* @Author :
* @Data   : 2024-1-10
* @Note   :
*/
void mrc_add_tail_and_escape(mcp_send_pkt_t *pkt_buf)
{
    MCP_tx_packet_tail_make(pkt_buf); // 添加包尾
    pkt_buf->pkt_len = MsgEscape(pkt_buf->pkt_buf, pkt_buf->pkt_len); // 转义处理
}
/*****************************************************************************
* @Name	  : mrc_send_pkt_to_ru
* @Brief  : 发送数据包给从设备,支持mcp A B C MRC
* @Param  : pkt_buf: 发送数据缓存区
* @Param  : ble_conn_id: 主从通信的从机ID
* @Retval : None
* @Author :
* @Data   : 2024-1-10
* @Note   :
*/
Status_t mrc_send_pkt_to_ru(mcp_send_pkt_t *pkt_buf, uint8_t ble_conn_id)
{

    // 数据发送
    if( ble_slave_msg_send_by_id( pkt_buf->pkt_buf, pkt_buf->pkt_len, ble_conn_id) == STAT_OK)
    {
        return STAT_OK;
    }
    else
    {
        return STAT_ERROR;
    }
}
#elif (LOCAL_MODULE_TYPE == _RU)
//RU发起,发包给MU
void new_mrc_send_pkt(mcp_send_pkt_t *pkt_buf, uint8_t ble_conn_id, uint8_t mcp_proto_type, uint8_t cmd_type)
{
    MCP_tx_packet_header_make(pkt_buf, mcp_proto_type, ble_conn_id, cmd_type);
}

Status_t add_mrc_pkt_data(mcp_send_pkt_t *pkt_buf, uint16_t code, uint8_t *para_buf, uint8_t strlen)
{
    MCP_MSG_T mcp_msg;

    if(pkt_buf == NULL)
        return STAT_ERROR;

    memset(&mcp_msg, 0, sizeof(MCP_MSG_T));

    if(pkt_buf->mcp_proto_type == MCP_MRC)
    {
        if(get_MRC_data_id_config( code, &mcp_msg) != STAT_OK)
            return STAT_ERROR;
    }
    else if(pkt_buf->mcp_proto_type == MCP_A)
    {
        if(get_MU_MCP_data_id_config( code, &mcp_msg) != STAT_OK)
            return STAT_ERROR;
    }
    else
        return STAT_ERROR;

    MCP_A_MRC_tx_packet_data_add(pkt_buf, strlen, mcp_msg.mcp_data_type, code, para_buf);
    return STAT_OK;
}

void mrc_add_tail_and_escape(mcp_send_pkt_t *pkt_buf)
{
    MCP_tx_packet_tail_make(pkt_buf); // 添加包尾
    pkt_buf->pkt_len = MsgEscape(pkt_buf->pkt_buf, pkt_buf->pkt_len); // 转义处理
}

Status_t mrc_send_pkt_to_mu(mcp_send_pkt_t *pkt_buf)
{
    // 数据发送
    if( ble_master_msg_send( pkt_buf->pkt_buf, pkt_buf->pkt_len) == STAT_OK)
    {
        return STAT_OK;
    }
    else
    {
        return STAT_ERROR;
    }
}
#endif

//等待回包
Status_t mrc_wait_response(uint8_t ble_conn_id, uint16_t pkt_id, uint32_t timeout)
{
    uint32_t start_time = hi_get_tick();
    Status_t ret = STAT_ERROR;

#if (LOCAL_MODULE_TYPE == _MU)
    mrc_ru_data[ble_conn_id - 1].pkt_send_stat.mrc_pkt_id = pkt_id; // 更新包序号
    mrc_ru_data[ble_conn_id - 1].pkt_send_stat.mrc_pkt_send_stat = MRC_DATA_SEND; // 更新发送状态
#elif (LOCAL_MODULE_TYPE == _RU)
    if(ble_conn_id != 0)
        return STAT_ERROR;
    mrc_ru_data[0].pkt_send_stat.mrc_pkt_id = pkt_id; // 更新包序号
    mrc_ru_data[0].pkt_send_stat.mrc_pkt_send_stat = MRC_DATA_SEND; // 更新发送状态
#endif

    //查询当前回包的包序号和发送状态
    while(timeout >= (hi_get_tick() - start_time))
    {
        //收到正确回包
        if(mrc_ru_data[ble_conn_id - 1].pkt_send_stat.mrc_pkt_id == pkt_id && mrc_ru_data[ble_conn_id - 1].pkt_send_stat.mrc_pkt_send_stat == MRC_DATA_RECV_OK)
        {
            mrc_ru_data[ble_conn_id - 1].pkt_send_stat.mrc_pkt_send_stat = MRC_DATA_IDLE;
            ret = STAT_OK;
            rtt_printf(RTT_BLE, RTT_CTRL_TEXT_BRIGHT_YELLOW"recieve MRC msg OK id = %d!\r\n", pkt_id);  
            break;
        }//收到错误回包
        else if(mrc_ru_data[ble_conn_id - 1].pkt_send_stat.mrc_pkt_id == pkt_id && mrc_ru_data[ble_conn_id - 1].pkt_send_stat.mrc_pkt_send_stat == MRC_DATA_RECV_ERR)
        {
            mrc_ru_data[ble_conn_id - 1].pkt_send_stat.mrc_pkt_send_stat = MRC_DATA_IDLE;
            ret = STAT_ERROR;
            rtt_printf(RTT_BLE, RTT_CTRL_TEXT_BRIGHT_YELLOW"recieve MRC msg Err id = %d!\r\n", pkt_id);
            break;
        }
        vTaskDelay(5);
    }
    //超时
    if(ret ==STAT_ERROR)
    {
        rtt_printf(RTT_BLE, RTT_CTRL_TEXT_BRIGHT_YELLOW"%d-rcv MRC msg Err id t = %d, %d, stat = %d!\r\n", hi_get_tick(), pkt_id,mrc_ru_data[ble_conn_id - 1].pkt_send_stat.mrc_pkt_id,mrc_ru_data[ble_conn_id - 1].pkt_send_stat.mrc_pkt_send_stat);
        mrc_ru_data[ble_conn_id - 1].pkt_send_stat.mrc_pkt_send_stat = MRC_DATA_IDLE;
    }
    return ret;
}
#if (LOCAL_MODULE_TYPE == _MU)
/*****************************************************************************
* @Name	  : app_mrc_set_ru_param
* @Brief  : 设置单个从机参数
* @params :
            ble_conn_id: 主从通信的从机ID
            param_id: 参数ID
            param_val: 输入参数值
            strlen: 参数长度 
            timeout: 超时时间单位ms, 输入0表示不等待,发送后立刻返回
*           仅支持MCP_MRC协议的参数.
            
* @Note   :
*/
Status_t mrc_set_ru_param(uint8_t ble_conn_id, uint16_t param_id, void *param_val, uint8_t strlen, uint32_t timeout)
{
    uint16_t pkt_id = 0;
    Status_t ret = STAT_OK;
    mcp_send_pkt_t tx_pkt;

    memset(&tx_pkt, 0, sizeof(mcp_send_pkt_t));
    new_mrc_send_pkt(&tx_pkt, ble_conn_id, MCP_MRC, MCP_CMD_TYPE_SET);
    if(add_mrc_pkt_data(&tx_pkt, param_id, param_val, strlen) != STAT_OK)
        return STAT_ERROR;
    mrc_add_tail_and_escape(&tx_pkt);
    pkt_id = tx_pkt.pkt_id;

    //发送数据
    ret = mrc_send_pkt_to_ru(&tx_pkt, ble_conn_id);
    //发送失败,返回错误
    if( ret == STAT_ERROR)
        return STAT_ERROR;
    else if(timeout == 0)//发送成功,超时时间为0,立刻返回
        return STAT_OK;
    else//发送成功,等待回复
    {
        if(mrc_wait_response(ble_conn_id, pkt_id, timeout) == STAT_OK)
            return STAT_OK;
        else
            return STAT_ERROR;
    }
}

/*****************************************************************************
* @Name	  : app_mrc_get_ru_param
* @Brief  : 查询单个从机参数
* @params :
            ble_conn_id: 主从通信的从机ID
            param_id: 参数ID
            strlen: 参数长度
            timeout: 超时时间单位ms, 输入0表示不等待,发送后立刻返回,并且不返回参数值
* @Note   : 当返回成功后,在mrc_ru_data取对应数据.
*           仅支持查询MCP_MRC协议的参数.
*/
Status_t mrc_query_ru_param(uint8_t ble_conn_id, uint16_t param_id, uint8_t strlen, uint32_t timeout)
{
    uint16_t pkt_id = 0;
    Status_t ret = STAT_OK;
    mcp_send_pkt_t tx_pkt;

    memset(&tx_pkt, 0, sizeof(mcp_send_pkt_t));
    new_mrc_send_pkt(&tx_pkt, ble_conn_id, MCP_MRC, MCP_CMD_TYPE_QUERY);
    if(add_mrc_pkt_data(&tx_pkt, param_id, NULL, strlen) != STAT_OK)
        return STAT_ERROR;
    mrc_add_tail_and_escape(&tx_pkt);
    pkt_id = tx_pkt.pkt_id;

    //发送数据
    ret = mrc_send_pkt_to_ru(&tx_pkt, ble_conn_id);
    //发送失败,返回错误
    if( ret == STAT_ERROR)
        return STAT_ERROR;
    else if(timeout == 0)//发送成功,超时时间为0,立刻返回
        return STAT_OK;

    //发送成功,等待回复
    if(mrc_wait_response(ble_conn_id, pkt_id, timeout) == STAT_OK)
        return STAT_OK;
    else
        return STAT_ERROR;
}

#elif (LOCAL_MODULE_TYPE == _RU)

#endif
/*****************************************************************************
 * @Name	  : mcpMRC_msg_handler
 * @Brief  : MCP_A协议发送消息处理
 * @Param  : rx_data:接收数据
 *           len:接收数据长度
 *           reply_data:回复数据
 * @Retval : 回复数据长度
 * @Author :
 * @Data   : 2024-1-10
 * @Note   :
 *****************************************************************************/
uint16_t mcpMRC_msg_handler(uint8_t *rx_data, uint16_t len, uint8_t *reply_data, uint8_t msg_src)
{
    uint8_t ret = 0;
    uint16_t crc_result;
    MCP_MSG_T mcp_msg;

    if (!len)
        return 0;

    get_mcpMRC_msg_info(&mcp_msg, rx_data, len, msg_src);

    // 判断是否是MCP_MRC协议
    if (mcp_msg.mcp_type != MCP_MRC)
        return 0;

    // 协议解析及业务处理

#if (_MU == LOCAL_MODULE_TYPE)   
    ret = mcp_mrc_mu_parase_msg(&mcp_msg);
#elif (_RU == LOCAL_MODULE_TYPE)
    ret = mcp_mrc_ru_parase_msg(&mcp_msg);
#endif

    if(ret == MCP_RESP_SIGN_OK)
        rtt_printf(RTT_BLE, RTT_CTRL_TEXT_BRIGHT_YELLOW "McpMRC Rx: devid=%d, num=%d, ret=%d\r\n", mcp_msg.dev_id,mcp_msg.vp_pack_num,ret);
    else
        rtt_printf(RTT_BLE, RTT_CTRL_TEXT_BRIGHT_RED "McpMRC Rx: devid=%d, num=%d, ret=%d\r\n", mcp_msg.dev_id,mcp_msg.vp_pack_num,ret);
    // 当报文是回包时,不需要再回复
    if(mcp_msg.vp_sign == VP_SIGN_REPLY)
    {
        return 0;
    }

    // 设置回复数据标志
    *mcp_msg.ptr_mcp_resp = ret;
    *mcp_msg.ptr_vp_resp = VP_SIGN_REPLY;

    crc_result = crc16((mcp_msg.ptr_src_buf + 1), mcp_msg.tx_buf_len - 4);

    mcp_msg.ptr_crc_L_resp = mcp_msg.ptr_data_unit + *mcp_msg.ptr_data_unit;
    mcp_msg.ptr_crc_H_resp = mcp_msg.ptr_crc_L_resp + 1;

    *mcp_msg.ptr_crc_L_resp = UINT16_L(crc_result); // L位
    *mcp_msg.ptr_crc_H_resp = UINT16_H(crc_result); // H位

    memcpy(reply_data, mcp_msg.ptr_src_buf, mcp_msg.tx_buf_len); // 将发送数据存放缓存数组

    mcp_msg.tx_buf_len = MsgEscape(reply_data, mcp_msg.tx_buf_len); // 转义处理
    
    return mcp_msg.tx_buf_len;
}
