/**
 * **********************************************************************************
 * @copyright Copyright (c) 2023 Shaanxi Tianji Communication Technologies Co., Ltd.
 *
 * @file        hi_usb.c
 * @author      WangJun
 * @version     None
 * @date        2023-09-11
 * @brief       None
 *
 * **********************************************************************************
 */

#include "hi_usb.h"
#include "msg_handle.h"
#include "rtt.h"
/*---------------------- 端口定义 ----------------------*/


/*---------------------- 宏 定 义 ----------------------*/


/*---------------------- 结 构 体 ----------------------*/
#if(GD32 == MCU_MANUFACTURER)
usb_core_driver cdc_acm;
#elif(STM32 == MCU_MANUFACTURER)
uint8_t g_usb_new_msg = 0;
extern USBD_HandleTypeDef hUsbDeviceFS;
#endif
/*---------------------- 常    量 ----------------------*/


/*---------------------- 全局变量 ----------------------*/
uint16_t g_usb_rx_len = 0;          // 接收b包长度
uint16_t g_usb_data_len = 0;          // 接收长度

// #if(PRODUCT_MODEL == CMDT3521C)
// __align(32) uint8_t pc_rx_array[PC_MSG_LEN] __attribute__((section("CCMRAM")));    //接收数据缓冲区
// __align(32) uint8_t PcRxDataBuff[PC_MSG_LEN] __attribute__((section("CCMRAM")));   //处理数据缓冲区
// __align(32) uint8_t PcTxDataBuff[PC_MSG_LEN] __attribute__((section("CCMRAM")));   //发送数据缓冲区
// #else
// uint8_t pc_rx_array[PC_MSG_LEN];    //接收数据缓冲区
// uint8_t PcRxDataBuff[PC_MSG_LEN];   //处理数据缓冲区
// uint8_t PcTxDataBuff[PC_MSG_LEN];   //发送数据缓冲区
// #endif
uint8_t pc_rx_array[PC_MSG_LEN];    //接收数据缓冲区
uint8_t PcRxDataBuff[PC_MSG_LEN];   //处理数据缓冲区
uint8_t PcTxDataBuff[PC_MSG_LEN];   //发送数据缓冲区


/*---------------------- 局部变量 ----------------------*/


/*---------------------- 函    数 ----------------------*/
/*****************************************************************************
* @brief  : usb初始化
* @param  : None
* @retval : None
* @date   : 2023-09-12
* @note   : 在task中调用
*****************************************************************************/
void hi_usb_init(void)
{
#if(STM32 == MCU_MANUFACTURER)
    //MX_USB_DEVICE_Init();
#elif(GD32 == MCU_MANUFACTURER)
    usb_gpio_config();
    usb_rcu_config();
    usb_timer_init();

    usbd_init(&cdc_acm, USB_CORE_ENUM_FS, &cdc_desc, &cdc_class);
    usb_intr_config();
#endif
}


/**
* @brief   获取USB连接状态   
* @param   None
* @return  USB_CONNECT: 已连接 / USB_UNCONNECT: 未连接  
* @data    2024-03-25
* @note    
*/
USB_CONNECT_STATE_E hi_usb_get_connect_state( void )
{
	#if(STM32 == MCU_MANUFACTURER)
		if( USBD_STATE_CONFIGURED == hUsbDeviceFS.dev_state )
			return USB_CONNECT;
		else
			return USB_UNCONNECT;
	
#elif(GD32 == MCU_MANUFACTURER)
    if( USBD_CONFIGURED == cdc_acm.dev.cur_status )
			return USB_CONNECT;
		else
			return USB_UNCONNECT;
#endif
}

/*****************************************************************************
* @brief  : usb数据发送
* @param  : pbuf: 发送数据缓存指针
*			len: 发送长度
* @retval :
* @date   : 2023-09-14
* @note   :
*****************************************************************************/
Status_t hi_usb_send(uint8_t * pbuf, uint16_t len)
{
    Status_t retval = STAT_OK;
    uint16_t i = 0;
    
#if(STM32 == MCU_MANUFACTURER)
    usb_send_data(pbuf, len);

    rtt_printf(RTT_PC, "PcTx: len = %d, data = ", len);
    for(i = 0; i < len; i++)
        rtt_printf(RTT_PC, "%02X", *(pbuf++));
    rtt_printf(RTT_PC, "\r\n");
    
    retval = STAT_OK;
#elif(GD32 == MCU_MANUFACTURER)
    if(USBD_CONFIGURED == cdc_acm.dev.cur_status)
    {
        usb_cdc_handler * cdc = (usb_cdc_handler *)cdc_acm.dev.class_data[CDC_COM_INTERFACE];

        if(0U != len)
        {
            cdc->packet_sent = 0U;
            cdc->receive_length = 0;
            if(len > 0)
            {
                usbd_ep_send(&cdc_acm, CDC_DATA_IN_EP, pbuf, len);
                
                rtt_printf(RTT_PC, "PcTx: len = %d, data = ", len);
                for(i = 0; i < len; i++)
                    rtt_printf(RTT_PC, "%02X", *(pbuf++));
                rtt_printf(RTT_PC, "\r\n");
                
                retval = STAT_OK;
            }
            else
            {
                retval = STAT_ERROR;
            }
            memset(pc_rx_array, 0, PC_MSG_LEN);
        }
        else
        {
            retval = STAT_ERROR;
        }
    }
    else
        retval = STAT_ERROR;

#endif
    return retval;
}


/*****************************************************************************
* @brief  : usb数据接收
* @param  : pbuf: 接收数据缓存指针
* @retval : uint16_t: 接收长度
* @date   : 2023-09-14
* @note   :
*****************************************************************************/
Status_t hi_usb_receive(uint8_t * pbuf, uint16_t * len)
{
    Status_t retval = STAT_OK;
    uint16_t i = 0;
    
#if(STM32 == MCU_MANUFACTURER)
    
    if(g_usb_data_len > 0)
    {
        memcpy(pbuf, pc_rx_array, g_usb_data_len);    // 原始接收数据
    
        *len = g_usb_data_len;
        g_usb_data_len = 0;   // 清空接收长度
        g_usb_rx_len = 0;   // 清空接收长度

        rtt_printf(RTT_PC, RTT_TEXT_YELLOW"\r\nPcRx: len = %d, data = ", *len);
        for(i = 0; i < *len; i++)
            rtt_printf(RTT_PC, RTT_TEXT_YELLOW"%02X", *(pbuf++));
        rtt_printf(RTT_PC, "\r\n");
        
        memset(pc_rx_array, 0, PC_MSG_LEN);
        
        retval = STAT_OK;
    }
    else
    {
        retval = STAT_ERROR;
    }
#elif(GD32 == MCU_MANUFACTURER)
    if(USBD_CONFIGURED == cdc_acm.dev.cur_status)
    {
        if(0U == cdc_acm_check_ready(&cdc_acm))
        {
            cdc_acm_data_receive(&cdc_acm);
            retval = STAT_BUSY;
        }
        else
        {
            if(g_usb_data_len > 0)
            {
                memcpy(pbuf, pc_rx_array, g_usb_data_len);    // 原始接收数据
                
                *len = g_usb_data_len;
                g_usb_data_len = 0;   // 清空接收长度
                g_usb_rx_len = 0;   // 清空接收长度

                rtt_printf(RTT_PC, RTT_TEXT_YELLOW"\r\nPcRx: len = %d, data = ", *len);
                for(i = 0; i < *len; i++)
                    rtt_printf(RTT_PC, RTT_TEXT_YELLOW"%02X", *(pbuf++));
                rtt_printf(RTT_PC, "\r\n");
                
                retval = STAT_OK;
            }
            else
            {
                retval = STAT_ERROR;
            }
        }
    }
    else
    {
        retval = STAT_tIMEOUT;
    }
#endif
    return retval;
}

void hi_usb_clear_fifo(void)
{
#if(GD32 == MCU_MANUFACTURER)
	static uint8_t last_stat;
	static uint32_t cnt = 0;
    
	// if(last_stat != cdc_acm.dev.cur_status)
	// 	rtt_printf( RTT_PC, "<usb>stat: %d\r\n",cdc_acm.dev.cur_status);
	if(last_stat != cdc_acm.dev.cur_status && USBD_CONFIGURED == cdc_acm.dev.cur_status)
	{
		usbd_fifo_flush(&cdc_acm,CDC_DATA_IN_EP);
		usbd_fifo_flush(&cdc_acm,CDC_DATA_OUT_EP);
		usbd_fifo_flush(&cdc_acm,CDC_CMD_EP);
	}
	if(USBD_CONFIGURED == cdc_acm.dev.cur_status && ((cdc_acm.regs.er_in[1]->DIEPLEN)!=0x00000000))
	{
		cnt++;
	}else
		cnt = 0;
	
	if(cnt >= 10)
	{
		usbd_fifo_flush(&cdc_acm,CDC_DATA_IN_EP);
		usbd_fifo_flush(&cdc_acm,CDC_DATA_OUT_EP);
		usbd_fifo_flush(&cdc_acm,CDC_CMD_EP);
		cnt = 0;
        rtt_printf(RTT_PC, "USB Clear fifo");
	}
	last_stat = cdc_acm.dev.cur_status;
#elif(STM32 == MCU_MANUFACTURER)

#endif
}

void hi_usb_pkt_cplt_chk(void)
{

#if(GD32 == MCU_MANUFACTURER)
    static uint16_t pre_length = 0;
    
    if(g_usb_rx_len == 0)
        return;

    if(g_usb_rx_len % 64 != 0)
        g_usb_data_len = g_usb_rx_len;
    else
    {
        if(pre_length == g_usb_rx_len)
            g_usb_data_len = g_usb_rx_len;
        else
            pre_length = g_usb_rx_len;
    }
#elif(STM32 == MCU_MANUFACTURER)
    static uint16_t pre_length = 0;
    
    if(g_usb_rx_len == 0)
        return;

    if(g_usb_rx_len % 64 != 0)
        g_usb_data_len = g_usb_rx_len;
    else
    {
        if(pre_length == g_usb_rx_len)
            g_usb_data_len = g_usb_rx_len;
        else
            pre_length = g_usb_rx_len;
    }

#endif
}
