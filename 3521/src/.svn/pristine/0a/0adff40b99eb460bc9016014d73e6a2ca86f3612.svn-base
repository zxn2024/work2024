/***************************** Copyright ****************************************
 *
 * (C) Copyright 2023, Shaanxi Tianji Communication
 *  All Rights Reserved
 *
 * FileName   : master_packet.c
 * Version    : none
 * Author     : none
 * Date       : 2023-06-20
 * Description: MCP协议核心处理模块,包括:
 *              1. 发起方的协议组包
 *              2. 收包处理
 *              3. 提供解包处理的三个接口,包括:
 *                  mcpA 发起方协议解包
 *                  mcpA 回复方协议解包
 *                  mcpB 发起方协议解包
 *                  mcpB 回复方协议解包
 *                  mcpC 发起方协议解包
 *                  mcpC 回复方协议解包
 *******************************************************************************/
#include "bsp_config.h"
#include "mcp_core.h"
#include "ascii_hex.h"
#include "crc.h"
#include "bsp_dev_para.h"

/*---------------------- 宏定义  ---------------------------------------*/

/*---------------------- 结构体 ----------------------------------------*/

/*---------------------- 常量 ------------------------------------------*/

/*---------------------- 全局变量 --------------------------------------*/

uint16_t pkt_number = 0; // 包序号;
uint8_t  buff_temp[MAX_MCP_DATA_LEN] = {0}; // 包格式处理缓冲区;
/*---------------------- 声明 ------------------------------------------*/


/*****************************************************************************
* @Name	  : MsgUnescape  反转义处理
* @Brief  : 字符反转义，0x5E,0x5D->0x5E, 0x5E,0x7D->0x7E
* @Param  : pSrc: 指向源数据
*           pDest: 指向处理后数据
*           len: 源数据长度
* @Retval :
* @Author :
* @Data   : 2022-08-22
* @Note   :
*****************************************************************************/
uint16_t MsgUnescape(uint8_t * pSrc, uint8_t * pDest, uint16_t len)
{
    uint16_t n, i;

    for(n = 0, i = 0; n < len; n++, i++)
    {
        if((*(pSrc + n) == 0x5E) && (*(pSrc + n + 1) == 0x5D))
        {
            pDest[i] = 0x5E;
            n++;
        }
        else if((*(pSrc + n) == 0x5E) && (*(pSrc + n + 1) == 0x7D))
        {
            pDest[i] = 0x7E;
            n++;
        }
        else
            pDest[i] = *(pSrc + n);
    }

    return i;
}


/*****************************************************************************
* @Name	  : MsgEscape   拆分
* @Brief  : 字符转义，0x5E->0x5E,0x5D, 0x7E->0x5E,0x7D
* @Param  : pBuff: 指向待转换的数据
*           len: 待转换数据长度
* @Retval : 转换后数据长度
* @Author :
* @Data   : 2022-08-22
* @Note   :
*****************************************************************************/
uint16_t MsgEscape(uint8_t * pBuff, uint16_t len)
{
    uint16_t n = 0, i = 0;
    uint16_t len_result = 0;

    if(len <= 10 || len >= MAX_MCP_DATA_LEN)
        return 0;

    if((*pBuff == 0x7E) && (*(pBuff + len - 1) == 0x7E))
    {
        buff_temp[0] = 0x7E;

        for(n = 1, i = 1; n <= len - 2; n++, i++)
        {
            if(*(pBuff + n) == 0x5E)
            {
                buff_temp[i++] = 0x5E;
                buff_temp[i] = 0x5D;
            }
            else if(*(pBuff + n) == 0x7E)
            {
                buff_temp[i++] = 0x5E;
                buff_temp[i] = 0x7D;
            }
            else
                buff_temp[i] = *(pBuff + n);
        }
        buff_temp[i] = 0x7E;
        len_result = i + 1;
        memcpy(pBuff, &buff_temp[0], len_result);

        return len_result;
    }
    else
        return 0;
}


/*****************************************************************************
* @Name	  : MsgSplit
* @Brief  : 将不可见字符串拆分成可见字符串，大于0x39的减去0x37，其余的减去0x30
* @Param  : src: 指向源数据
*           dst: 指向拆分后数据
*           len: 源数据长度
* @Retval :
* @Author :
* @Data   : 2022-08-22
* @Note   :
*****************************************************************************/
uint16_t MsgSplit(uint8_t * src, uint8_t * dst, uint16_t len)
{
    uint16_t n = 0;
    uint16_t i = 0;
    uint8_t dh, dl;

    if((src == NULL) || (dst == NULL) || (len == 0))
    {
        return 0;
    }

    for(i = 0, n = 0; i < len; i++)
    {
        dh = (*(src + i) >> 4) & 0x0F;
        dl = *(src + i) & 0x0F;

        if(dh > 0x09)
            *(dst + n) = dh + 0x37;
        else
            *(dst + n) = dh + 0x30;
        n++;
        if(dl > 0x09)
            *(dst + n) = dl + 0x37;
        else
            *(dst + n) = dl + 0x30;
        n++;
    }

    return n;
}

/**************************************************************************
* 函 数 名：MsgASCIIToHex
* 功能描述：ASCII转换为16进制
* 参数说明：输入参数、输入参数长度、输出参数
* 函数输出：转换后的数据长度
**************************************************************************/
uint16_t MsgASCIIToHex(uint8_t * inbuf, uint16_t inlen, uint8_t * outbuf)
{
    uint16_t outlen = 0;
    uint16_t strlen = 0;
    uint8_t dh = 0;
    uint8_t dl = 0;
    while(1)
    {
        if(*(inbuf + strlen) > 0x39)
        {
            dh = *(inbuf + strlen) - 0x37;
        }
        else
        {
            dh = *(inbuf + strlen) - 0x30;
        }
        strlen++;
        if(*(inbuf + strlen) > 0x39)
        {
            dl = *(inbuf + strlen) - 0x37;
        }
        else
        {
            dl = *(inbuf + strlen) - 0x30;
        }
        strlen++;
        outbuf[outlen++] = (dh << 4) + dl;
        if(strlen == inlen)
        {
            break;
        }
    }
    return outlen;
}

/*****************************************************************************
 * @Name   : void MCP_tx_packet_header_make(uint8_t mcp_proto_type, uint8_t cmd_type, uint8_t *pkt_buf)
 * @Brief  : 填充MCP数据包头
 * @Param  : mcp_proto_type: 消息类型 (1:A, 2:B, 3:C)
 *           dev_id: 设备编号:
                     -和监控通信时 主机 0，从机 0x01~0xFE, 本机 0xFF, 
                     -用于主从通信时,使用从机蓝牙连接序号ble_conn_id: 主机为0 从设备从1起
 *           cmd_type: 命令类型 (1:上报 2:查询 3:设置)
 *           pkt_buf: 数据包组包缓冲区
 * @Retval : 无
 *****************************************************************************/
void MCP_tx_packet_header_make(mcp_send_pkt_t * pkt_buf, uint8_t mcp_proto_type, uint8_t dev_id, uint8_t cmd_type)
{
    pkt_buf->pkt_buf[ADDR_SOP] = MCP_SOP;
    pkt_buf->pkt_buf[ADDR_AP_PROTO_TYPE] = AP_C;
    pkt_buf->pkt_buf[ADDR_VP_PROTO_TYPE] = VP_A;
    pkt_buf->pkt_buf[ADDR_VP_SITE_ID] = 0; // 待获取
    pkt_buf->pkt_buf[ADDR_VP_DEVICE_ID] = dev_id;
    pkt_buf->pkt_buf[ADDR_VP_PKT_NUM] = (uint8_t)(pkt_number & 0x00ff);
    pkt_buf->pkt_buf[ADDR_VP_PKT_NUM+1] = (uint8_t)((pkt_number & 0xff00) >> 8);
    pkt_buf->pkt_buf[ADDR_VP_SIGN] = VP_SIGN_SEND;
    pkt_buf->pkt_buf[ADDR_MCP_PROTO_TYPE] = mcp_proto_type;
    pkt_buf->pkt_buf[ADDR_MCP_CMD_TYPE] = cmd_type;
    pkt_buf->pkt_buf[ADDR_MCP_RESP_SIGN] = MCP_RESP_SIGN_SEND;

    pkt_buf->mcp_proto_type = mcp_proto_type;
    pkt_buf->devid = dev_id;
    pkt_buf->pkt_id = pkt_number;
    pkt_buf->pkt_len = ADDR_MCP_RESP_SIGN + 1;
    pkt_number++;
}

/*****************************************************************************
 * @Name   : void MCP_A_MRC_tx_packet_data_add(uint8_t *pkt_buf, uint16_t paralen, uint16_t code, uint8_t *para_buf)
 * @Brief  : 填充数据包数据, 适用于A,B,MRC协议
 * @Param  : pkt_buf: 数据包缓存区
 *           para_len: 参数长度
 *           code: 监控代码
 *           para_buf: 参数缓存区
 * @Retval : 无
 *****************************************************************************/
void MCP_A_MRC_tx_packet_data_add(mcp_send_pkt_t * pkt_buf, uint16_t para_len, MCP_TYPE_E para_type, uint16_t code, void * para_buf)
{
    uint16_t data_ptr = pkt_buf->pkt_len;
    uint8_t * para_ptr = (uint8_t *)para_buf;
    uint8_t para_length = 0;

    data_ptr++;
    pkt_buf->pkt_buf[data_ptr] = (uint8_t)(code & 0x00FF);
    data_ptr++;
    pkt_buf->pkt_buf[data_ptr] = (uint8_t)((code & 0xFF00) >> 8);
    data_ptr++;

    // 填充参数数据, 当para_buf为NULL时, 认为是查询包,不填充参数数据

    switch(para_type)
    {
        case MCP_BOOL:
        case MCP_UINT1:
        case MCP_INT1:
            
            para_length = 1;
            if(para_buf != NULL)
                pkt_buf->pkt_buf[data_ptr] = (uint8_t)(para_ptr[0]);     // 1字节
            data_ptr++;
            break;
        
        case MCP_UINT2:
        case MCP_INT2:
            para_length = 2;
            if(para_buf != NULL)
                pkt_buf->pkt_buf[data_ptr] = (uint8_t)(para_ptr[0]);
            data_ptr++;
            if(para_buf != NULL)
                pkt_buf->pkt_buf[data_ptr] = (uint8_t)(para_ptr[1]);
            data_ptr++;
            break;
        
        case MCP_UINT4:
        case MCP_INT4:
            para_length = 4;
            if(para_buf != NULL)
                pkt_buf->pkt_buf[data_ptr] = (uint8_t)(para_ptr[0]);
            data_ptr++;
            if(para_buf != NULL)
                pkt_buf->pkt_buf[data_ptr] = (uint8_t)(para_ptr[1]);
            data_ptr++;
            if(para_buf != NULL)
                pkt_buf->pkt_buf[data_ptr] = (uint8_t)(para_ptr[2]);
            data_ptr++;
            if(para_buf != NULL)
                pkt_buf->pkt_buf[data_ptr] = (uint8_t)(para_ptr[3]);
            data_ptr++;
            break;
        
        case MCP_STRING:
            para_length = para_len;
            if(para_buf != NULL)
                memcpy(&pkt_buf->pkt_buf[data_ptr], para_ptr, para_len);
            data_ptr += para_len;
            break;

        default:
            break;
    }
    

    pkt_buf->pkt_buf[pkt_buf->pkt_len] = para_length + 3;
    pkt_buf->pkt_len = data_ptr;
}

/*****************************************************************************
 * @Name   : void MCP_A_MRC_tx_packet_data_add(uint8_t *pkt_buf, uint16_t paralen, uint16_t code, uint8_t *para_buf)
 * @Brief  : 填充数据包数据, 适用于A,B,MRC协议
 * @Param  : pkt_buf: 数据包缓存区
 *           para_len: 参数长度
 *           code: 监控代码
 *           para_buf: 参数缓存区
 * @Retval : 无
 *****************************************************************************/
void MCP_B_tx_packet_data_add(mcp_send_pkt_t * pkt_buf, uint16_t para_len, MCP_TYPE_E para_type, uint16_t code, void * para_buf)
{
    uint16_t data_ptr = pkt_buf->pkt_len;
    uint8_t * para_ptr = (uint8_t *)para_buf;

    pkt_buf->pkt_buf[data_ptr] = (uint8_t)((para_len+4) & 0x00FF);
    data_ptr++;
    pkt_buf->pkt_buf[data_ptr] = (uint8_t)(((para_len+4) & 0xFF00) >> 8);
    data_ptr++;
    pkt_buf->pkt_buf[data_ptr] = (uint8_t)(code & 0x00FF);
    data_ptr++;
    pkt_buf->pkt_buf[data_ptr] = (uint8_t)((code & 0xFF00) >> 8);
    data_ptr++;
    switch(para_type)
    {
        case MCP_BOOL:
        case MCP_UINT1:
        case MCP_INT1:
            pkt_buf->pkt_buf[data_ptr] = (uint8_t)(para_ptr[0]);
            data_ptr++;
            break;
        
        case MCP_UINT2:
        case MCP_INT2:
            pkt_buf->pkt_buf[data_ptr] = (uint8_t)(para_ptr[1]);
            data_ptr++;
            pkt_buf->pkt_buf[data_ptr] = (uint8_t)(para_ptr[0]);
            data_ptr++;
            break;
        
        case MCP_UINT4:
        case MCP_INT4:
            pkt_buf->pkt_buf[data_ptr] = (uint8_t)(para_ptr[3]);
            data_ptr++;
            pkt_buf->pkt_buf[data_ptr] = (uint8_t)(para_ptr[2]);
            data_ptr++;
            pkt_buf->pkt_buf[data_ptr] = (uint8_t)(para_ptr[1]);
            data_ptr++;
            pkt_buf->pkt_buf[data_ptr] = (uint8_t)(para_ptr[0]);
            data_ptr++;
            break;
        
        case MCP_STRING:
            memcpy(&pkt_buf->pkt_buf[data_ptr], para_ptr, para_len);
            data_ptr += para_len;
            break;

        default:
            break;
    }

    pkt_buf->pkt_len = data_ptr;
}
/*****************************************************************************
 * @Name   : void MCP_C_tx_packet_data_add(uint8_t *pkt_buf, uint16_t paralen, uint16_t code, uint8_t *para_buf)
 * @Brief  : 填充数据包数据, 适用于MCPC协议
 * @Param  : pkt_buf: 数据包缓存区
 *           para_len: 参数长度
 *           code: 监控代码
 *           para_buf: 参数缓存区
 * @Retval : 无
 *****************************************************************************/
void MCP_C_tx_packet_data_add(mcp_send_pkt_t * pkt_buf, uint16_t para_len, MCP_TYPE_E para_type, uint32_t code, void * para_buf)
{
    uint16_t data_ptr = pkt_buf->pkt_len;
    uint8_t * para_ptr = (uint8_t *)para_buf;

    pkt_buf->pkt_buf[data_ptr] = (para_len);
    data_ptr++;
    pkt_buf->pkt_buf[data_ptr] = (uint8_t)(code & 0x000000FF);
    data_ptr++;
    pkt_buf->pkt_buf[data_ptr] = (uint8_t)((code & 0x0000FF00) >> 8);
    data_ptr++;
    pkt_buf->pkt_buf[data_ptr] = (uint8_t)((code & 0x00FF0000) >> 16);
    data_ptr++;
    pkt_buf->pkt_buf[data_ptr] = (uint8_t)((code & 0xFF000000) >> 24);
    data_ptr++;

    switch(para_type)
    {
        case MCP_BOOL:
        case MCP_UINT1:
        case MCP_INT1:
            pkt_buf->pkt_buf[data_ptr] = (uint8_t)(para_ptr[0]);     // 1字节
            data_ptr++;
            break;
        
        case MCP_UINT2:
        case MCP_INT2:
            pkt_buf->pkt_buf[data_ptr] = (uint8_t)(para_ptr[1]);
            data_ptr++;
            pkt_buf->pkt_buf[data_ptr] = (uint8_t)(para_ptr[0]);
            data_ptr++;
            break;
        
        case MCP_UINT4:
        case MCP_INT4:
            pkt_buf->pkt_buf[data_ptr] = (uint8_t)(para_ptr[3]);
            data_ptr++;
            pkt_buf->pkt_buf[data_ptr] = (uint8_t)(para_ptr[2]);
            data_ptr++;
            pkt_buf->pkt_buf[data_ptr] = (uint8_t)(para_ptr[1]);
            data_ptr++;
            pkt_buf->pkt_buf[data_ptr] = (uint8_t)(para_ptr[0]);
            data_ptr++;
            break;
        
        case MCP_STRING:
            memcpy(&pkt_buf->pkt_buf[data_ptr], para_ptr, para_len);
            data_ptr += para_len;
            break;

        default:
            break;
    }

    pkt_buf->pkt_len = data_ptr;
}
/*****************************************************************************
 * @Name   : void tx_packet_tail_make(uint8_t *pkt_buf)
 * @Brief  : 填充数据包尾
 * @Param  : pkt_buf: 数据包缓存区
 * @Retval : 无
 *****************************************************************************/
void MCP_tx_packet_tail_make(mcp_send_pkt_t * pkt_buf)
{
    uint16_t data_ptr, res_crc;
    data_ptr = pkt_buf->pkt_len; // 获取数据指针

    res_crc = crc16(pkt_buf->pkt_buf + 1, data_ptr - 1); // 计算CRC校验值,不包含起始字节/结束字节

    pkt_buf->pkt_buf[data_ptr] = (uint8_t)(res_crc & 0x00FF);
    data_ptr++;
    pkt_buf->pkt_buf[data_ptr] = (uint8_t)((res_crc & 0xFF00) >> 8);
    data_ptr++;
    pkt_buf->pkt_buf[data_ptr] = MCP_EOP; // 填充结束符

    pkt_buf->pkt_len = data_ptr + 1;
}
/*****************************************************************************
 * @Name   : void MCP_tx_packet_escape(uint8_t *pkt_buf)
 * @Brief  : 发送前数据包转义
 * @Param  : pkt_buf: 数据包缓存区
 * @Retval : 无
 *****************************************************************************/

void MCP_tx_packet_escape(mcp_send_pkt_t * pkt_buf)
{
    MsgEscape(pkt_buf->pkt_buf, pkt_buf->pkt_len);
}

/*****************************************************************************
 * @Name	  : test_protocol_pkt_valid_chk
 * @Brief  : 协议报文校验,返回校验结果
 * @Param  : rx_data: 原始数据指针
 **			reply_data: 目标返回数据指针
 * @Retval : 若返回数据长度为0, 则不需要发送, 否则从USB发送数据
 * @Author :
 * @Data   : 2023-12-13
 * @Note   :
 *****************************************************************************/
uint8_t MCP_pkt_valid_chk(uint8_t * rx_data, uint16_t len, uint8_t msg_src)
{
    uint32_t mcp_siteid_temp = 0;

    if((rx_data[ADDR_SOP] != MCP_SOP) && (rx_data[len - 1] != MCP_EOP))  // 开始/结束字符检查
        return MCP_RESP_SIGN_LEN_ERROR;

    if(len < 16) // 长度检查
        return MCP_RESP_SIGN_LEN_ERROR;

    if (msg_src == MCP_MONITOR && rx_data[ADDR_AP_PROTO_TYPE] != AP_C) {
        return MCP_RESP_SIGN_OTHER_ERROR;
    } else if((rx_data[ADDR_AP_PROTO_TYPE] != AP_A) && (rx_data[ADDR_AP_PROTO_TYPE] != AP_B) && (rx_data[ADDR_AP_PROTO_TYPE] != AP_C))  // AP协议类型检查
        return MCP_RESP_SIGN_OTHER_ERROR;

    if(rx_data[ADDR_VP_PROTO_TYPE] != VP_A)   // VP协议类型检查
        return MCP_RESP_SIGN_OTHER_ERROR;

    mcp_siteid_temp = (rx_data[ADDR_VP_SITE_ID]) + (rx_data[ADDR_VP_SITE_ID+1]<<8 )+ (rx_data[ADDR_VP_SITE_ID+2]<<16) + (rx_data[ADDR_VP_SITE_ID+3]<<24);
    if(msg_src == MCP_MONITOR && mcp_siteid_temp != tOmcPara.bs_id)//OMC用与设备本身不同的站点编号或设备编号去对设备进行查询或设置操作，设备应鉴权失败，对收到的命令不予回应,站点编号鉴权失败处理
    {
        return MCP_RESP_SIGN_OTHER_ERROR;
    }

    // if ((rx_data[ADDR_VP_DEVICE_ID] >= BLE_CONNECT_MAX_NUM) && (rx_data[ADDR_VP_DEVICE_ID] <= 200)) // 判断设备ID是否合法
    //     return MCP_RESP_SIGN_OTHER_ERROR;

    if((rx_data[ADDR_VP_SIGN] != VP_SIGN_SEND) && (rx_data[ADDR_VP_SIGN] != VP_SIGN_REPLY))  // VP签名检查
        return MCP_RESP_SIGN_OTHER_ERROR;

    if((rx_data[ADDR_MCP_PROTO_TYPE] != MCP_A) && (rx_data[ADDR_MCP_PROTO_TYPE] != MCP_B) && (rx_data[ADDR_MCP_PROTO_TYPE] != MCP_C)&& (rx_data[ADDR_MCP_PROTO_TYPE] != MCP_MRC))  // MCP协议类型检查
        return MCP_RESP_SIGN_OTHER_ERROR;

    if(((rx_data[ADDR_VP_SIGN] == VP_SIGN_SEND) && (rx_data[ADDR_MCP_RESP_SIGN] != MCP_RESP_SIGN_SEND)) || ((rx_data[ADDR_VP_SIGN] != VP_SIGN_SEND) && (rx_data[ADDR_MCP_RESP_SIGN] == MCP_RESP_SIGN_SEND)))  // 发送包应答签名检查
        return MCP_RESP_SIGN_OTHER_ERROR;

    // 判断命令是否合法
    if ((rx_data[ADDR_MCP_CMD_TYPE] != MCP_CMD_TYPE_RPT) && (rx_data[ADDR_MCP_CMD_TYPE] != MCP_CMD_TYPE_QUERY) && (rx_data[ADDR_MCP_CMD_TYPE] != MCP_CMD_TYPE_SET)
		&& (rx_data[ADDR_MCP_CMD_TYPE] != MCP_CMD_TYPE_SWITCH_UPGRADE_MODE) && (rx_data[ADDR_MCP_CMD_TYPE] != MCP_CMD_TYPE_SWITCH_SOFE_VERSION))
    {
        return MCP_RESP_SIGN_ID_ERROR;
    }

    // CRC校验 
    uint16_t crc_msg = (*(rx_data + len - 2) << 8) | (*(rx_data + len - 3));
    uint16_t crc_cal = crc16((rx_data + 1), len - 4);

    if(crc_msg != crc_cal && crc_cal != 0xffff) // CRC校验失败(crc_cal == 0xffff时跳过crc校验)
        return MCP_RESP_SIGN_CRC_ERROR;

    // 校验成功
    return MCP_RESP_SIGN_OK;
}

static uint16_t MCP_pkt_error_reply(uint8_t * rx_data, uint16_t len, uint8_t * reply_data, uint8_t error_code)
{
    if (error_code == MCP_RESP_SIGN_CRC_ERROR || error_code == MCP_RESP_SIGN_OTHER_ERROR) {
        return 0;
    }
    uint16_t crc_result;
    memcpy(reply_data, rx_data, len);
    *(reply_data + 13) = error_code;

    crc_result = crc16((reply_data + 1), len - 4);
    *(reply_data + len - 3) = UINT16_L(crc_result);   // L位
    *(reply_data + len - 2) = UINT16_H(crc_result);   // H位
    return MsgEscape(reply_data, len); // 转义处理
}

/*****************************************************************************
* @Name	  : MCP_rx_pkt_analysis
* @Brief  : 调测协议报文解析,此处仅关注主从通信所用协议
* @Param  : rx_data: 接收数据,hex格式数组
            reply_data: 回复数据
            msg_src: 消息来源
* @Retval : Status_t : 成功/失败
* @Author :
* @Data   : 2023-12-13
* @Note   :
*/

uint16_t MCP_rx_pkt_analysis(uint8_t * rx_data, uint16_t len, uint8_t * reply_data, uint8_t msg_src)
{
    uint8_t chk_code = MCP_RESP_SIGN_OK;
    uint16_t data_len = 0;
    uint16_t tx_len = 0;
    uint16_t pkt_len = 0;
    uint16_t temp = 0;
    uint16_t pkt_start = 0;
    uint16_t pkt_end = 0;
    uint8_t mcp_proto_type;

    while (1) {
        if (temp == len) {
            return 0;
        }
        if (rx_data[temp] == 0x7E) {
            pkt_start = temp;
            temp++;
        } else {
            temp++;
            continue;
        }
        while (1) {
            if (temp == len) {
                return 0;
            }
            if (rx_data[temp] == 0x7E) {
                pkt_end = temp;
                if ((pkt_end - pkt_start) < 16) {
                    pkt_start = 0;
                    pkt_end = 0;
                    break;
                } else {
                    pkt_len = pkt_end - pkt_start + 1;
                    break;
                }
            }
            temp++;
        }
        if (pkt_len > 0) {
            break;
        }
    }
    memcpy(buff_temp, rx_data + pkt_start, pkt_len);
    // 转义处理
    data_len = MsgUnescape(buff_temp, buff_temp, pkt_len);
    memcpy(rx_data, buff_temp, data_len);
    memset(buff_temp, 0, sizeof(buff_temp));

    // 校验数据包的合法性
    chk_code = MCP_pkt_valid_chk(rx_data, data_len, msg_src);

    // 处理错误包:
    if(chk_code != MCP_RESP_SIGN_OK)
    {
        tx_len = MCP_pkt_error_reply(rx_data, data_len, reply_data, chk_code);
        return tx_len;
    }

    // 根据包类型分类处理
    mcp_proto_type = rx_data[ADDR_MCP_PROTO_TYPE];
    switch(mcp_proto_type)
    {
        case MCP_A: // MCP:A数据解析
            tx_len = mcpA_msg_handler(rx_data, data_len, reply_data, msg_src); 
            break;

        case MCP_B: // MCP:B数据解析
            tx_len = mcpB_msg_handler(rx_data, data_len, reply_data, msg_src);  
            break;

        case MCP_C: // MCP:C数据解析
            tx_len = mcpC_msg_handler(rx_data, data_len, reply_data, msg_src);  
            break;

        case MCP_MRC: // MCP:MRC数据解析,用于传输主从通信数据
            tx_len = mcpMRC_msg_handler(rx_data, data_len,reply_data, msg_src);    
            break;

        default:
            break;
    }

    return tx_len;
}
