/***************************** Copyright ****************************************
*
* (C) Copyright 2023, Shaanxi Tianji Communication
*  All Rights Reserved
*
* FileName   : bsp_pc.c
* Version    : none
* Author     :
* Date       : 2023-12-22
* Description: none
*******************************************************************************/
#include "main.h"
//#include "stdlib.h"
//#include "stdio.h"
//#include "string.h"
//#include "MCU_config.h"
#include "public_define.h"
#include "public_enum.h"
#include "public_struct.h"
#include "bsp_pc.h"
#include "hi_usb.h"
#include "bsp_testmode.h"
#include "msg_handle.h"
#include "mcp_core.h"

#include "rtt.h"
#include "bsp_testmode_analysis.h"
/*---------------------- 端口定义 --------------------------------------*/


/*---------------------- 宏定义  ---------------------------------------*/
typedef enum
{
    PDU_NULL = 0,
    PDU_MCP,
    PDU_TM,
    PDU_YMODEM
} PDU_TYPE_E;

/*---------------------- 结构体 ----------------------------------------*/


/*---------------------- 常量 ------------------------------------------*/


/*---------------------- 全局变量 --------------------------------------*/


/*---------------------- 局部变量 --------------------------------------*/
#define PDU_MAX_LEN	( 1200 )		//报文最长支持 1200 Bytes
uint8_t pc_rx_buf[ PDU_MAX_LEN ];	//存储pc发送过来的数据
uint8_t pc_mid_buf[ PDU_MAX_LEN ];	//pc缓存数据
uint8_t pc_tx_buf[ PDU_MAX_LEN ];	//存储从pc_rx_buf中提取出来的有效报文
uint8_t msg_pdu_type;	            //PC消息类型, 1:MCPA, 2:TM, 3:YMODEM



/*---------------------- 声明 ------------------------------------------*/



/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ APP ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*****************************************************************************
* @Name	  : pc_rx_buf_clr
* @Brief  : 清空pc_rx_buf
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2023-12-22
* @Note   :
*****************************************************************************/
void pc_rx_buf_clr(void)
{
    memset(pc_rx_buf, 0x00, PDU_MAX_LEN);
}

/*****************************************************************************
* @Name	  : pc_tx_buf_clr
* @Brief  : 清空pc_rx_buf
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2023-12-22
* @Note   :
*****************************************************************************/
void pc_tx_buf_clr(void)
{
    memset(pc_tx_buf, 0x00, PDU_MAX_LEN);
}

/*****************************************************************************
* @Name	  : pc_mid_buf_clr
* @Brief  : 清空pc_rx_buf
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2023-12-22
* @Note   :
*****************************************************************************/
void pc_mid_buf_clr(void)
{
    memset(pc_mid_buf, 0x00, PDU_MAX_LEN);
}


/*****************************************************************************
* @Name	  : get_pc_new_msg_data
* @Brief  : 获取PC发送的数据并提取有效的报文
* @Param  : None
* @Retval : 有效报文的长度
* @Author :
* @Data   : 2023-12-22
* @Note   :
*****************************************************************************/
uint16_t get_pc_new_msg_data(void)
{
    uint16_t mcpa_start = 0;
    uint16_t mcpa_end   = 0;
    uint16_t tm_start   = 0;
//    uint16_t tm_end     = 0;
    uint16_t pdu_len    = 0;	//保存从PC发送数据中提起的报文长度
    uint16_t msg_rx_len = 0;	//保存PC发送的数据长度

    pc_rx_buf_clr();
    pc_tx_buf_clr();
    pc_mid_buf_clr();
    Status_t res = hi_usb_receive(pc_rx_buf, &msg_rx_len);

    msg_rx_len = (msg_rx_len <= PDU_MAX_LEN) ? msg_rx_len : PDU_MAX_LEN;
    msg_pdu_type = PDU_NULL;

    if(STAT_OK == res)   //received new msg
    {
        for(uint16_t i = 0; i <= msg_rx_len; i++)
        {
            if(0x7E == pc_rx_buf[i])
            {
                if( ((B_FALSE == mcpa_start) && (0x03 == pc_rx_buf[i + 1]) && (0x01 == pc_rx_buf[i + 2])) || \
                    ((B_FALSE == mcpa_start) && (0x01 == pc_rx_buf[i + 1]) && (0x01 == pc_rx_buf[i + 2])) )
                    mcpa_start = i;
                else
                {
                    mcpa_end = i;
                    pdu_len  = mcpa_end - mcpa_start + 1;
                    memcpy(pc_mid_buf, &pc_rx_buf[ mcpa_start ],  pdu_len);
                    msg_pdu_type = PDU_MCP;
                    break;
                }
            }
            else if('!' == *pc_rx_buf)
            {
                uint16_t tm_len = pc_rx_buf[i + 1] + (pc_rx_buf[i + 2] << 8);
                tm_len = (i + tm_len < PDU_MAX_LEN) ? tm_len : (PDU_MAX_LEN - i);
                if('!' == pc_rx_buf[ i + tm_len - 1 ])
                {
                    tm_start = i;
                    pdu_len  = tm_len;
                    memcpy(pc_mid_buf, &pc_rx_buf[ tm_start ],  tm_len);
                    msg_pdu_type = PDU_TM;
                    break;
                }
            }
            else
            {}
        }
        
        if(PDU_NULL == msg_pdu_type)
            rtt_printf(RTT_PC, RTT_TEXT_RED"[ERROR]PcRx data error\r\n");
    }

    return pdu_len;
}

/*****************************************************************************
* @Name	  : pc_msg_analysis
* @Brief  : 解析PC报文
* @Param  : pdu_type: [输入/出]
**			src: [输入/出]
**			len: [输入/出]
* @Retval : 待发送报文的长度
* @Author :
* @Data   : 2023-12-22
* @Note   :
*****************************************************************************/
uint16_t pc_msg_analysis(uint8_t pdu_type, uint8_t * src, uint16_t len)
{
    uint16_t tx_pdu_len = 0;

    if((0 == pdu_type) || (0 == len))
        return 0;

    if(PDU_MCP == pdu_type)
    {
        tx_pdu_len =  MCP_rx_pkt_analysis(src, len, pc_tx_buf, MCP_PC);
    }
    else if(PDU_TM == pdu_type)
    {
        tx_pdu_len = tm_msg_parse_in_task(src, pc_tx_buf);
    }
    else
    {}

    return 	tx_pdu_len;
}


/*****************************************************************************
* @Name	  : pc_msg_in_task
* @Brief  : 在任务中调用的PC消息处理函数
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2023-12-22
* @Note   :
*****************************************************************************/
void pc_msg_in_task(void)
{
    uint16_t tx_msg_len = 0;
    uint16_t rx_msg_len = 0;

    rx_msg_len = get_pc_new_msg_data();
    tx_msg_len = pc_msg_analysis(msg_pdu_type, pc_mid_buf, rx_msg_len);

    if(tx_msg_len)
    {
        hi_usb_send(pc_tx_buf, tx_msg_len);
    }
}




