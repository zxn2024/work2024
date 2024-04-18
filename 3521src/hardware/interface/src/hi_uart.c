/**
 * **********************************************************************************
 * @copyright Copyright (c) 2023 Shaanxi Tianji Communication Technologies Co., Ltd.
 *
 * @file        hi_uart.c
 * @author      WangJun
 * @version     None
 * @date        2023-11-7
 * @brief       None
 *
 * **********************************************************************************
 */

#include "hi_uart.h"
#include "hi_systick.h"
#include <string.h>

#include "rtt.h"

/*---------------------- 端口定义 ----------------------*/


/*---------------------- 宏 定 义 ----------------------*/


/*---------------------- 结 构 体 ----------------------*/


/*---------------------- 常    量 ----------------------*/


/*---------------------- 全局变量 ----------------------*/


/*---------------------- 局部变量 ----------------------*/


/*---------------------- 声    明 ----------------------*/


/*---------------------- 函    数 ----------------------*/
/*****************************************************************************
* @Name	  : uart_open
* @Brief  : 初始化串口和环形队列
* @Param  : usart_periph: 串口号
*			uartRxBuffer: 接收缓冲区地址
*			uartRxBufLen: 缓冲区大小
* @Retval : 成功/失败
* @Author :
* @Data   : 2022-08-16
* @Note   :
*****************************************************************************/
static int8_t uart_open(UART_CH_T usart_periph, Cbuffer_T * uartRxBuffer, uint16_t uartRxBufLen)
{
    if(NULL == usart_periph || NULL == uartRxBuffer)
        return B_FALSE;

    if(B_TRUE == cbuffer_init(uartRxBuffer, uartRxBufLen))
        return B_TRUE;
    else
        return B_FALSE;
}


/*****************************************************************************
* @Name	  : uart_close
* @Brief  : 清空环形队列
* @Param  : usart_periph: 串口号
*			uartRxBuffer: 接收缓冲区地址
* @Retval : 成功/失败
* @Author :
* @Data   : 2022-08-16
* @Note   :
*****************************************************************************/
static int8_t uart_close(UART_CH_T usart_periph, Cbuffer_T * uartRxBuffer)
{
    if(NULL == usart_periph || NULL == uartRxBuffer)
        return B_FALSE;

#if(STM32 == MCU_MANUFACTURER)

#elif(GD32 == MCU_MANUFACTURER)
    usart_interrupt_disable(usart_periph, USART_INT_IDLE);
    usart_interrupt_disable(usart_periph, USART_INT_RBNE);
    usart_disable(usart_periph);
#endif

    if(B_TRUE == cbuffer_clear(uartRxBuffer))
        return B_TRUE;
    else
        return B_FALSE;
}


/*****************************************************************************
* @Name	  : uart_read
* @Brief  : 从环形队列中读一串数据到目标区域
* @Param  : uartRxBuffer: 接收缓冲区地址
*			pBuf: 读缓存区
*			len: 长度
* @Retval : 读取的数据长度
* @Author :
* @Data   : 2022-08-16
* @Note   :
*****************************************************************************/
static int16_t uart_read(Cbuffer_T * uartRxBuffer, uint8_t * pBuf, uint16_t len)
{
    uint16_t i = 0;
    uint16_t usAreaAvail = 0;
    uint16_t usTemp = 0;

    if(NULL == uartRxBuffer || NULL == pBuf)
        return B_FALSE;

    if(uartRxBuffer->usWptrEnd >= uartRxBuffer->usRptr)
        usAreaAvail = uartRxBuffer->usWptrEnd - uartRxBuffer->usRptr;
    else
        usAreaAvail = uartRxBuffer->usMaxLen - uartRxBuffer->usRptr + uartRxBuffer->usWptrEnd;
    if(len > usAreaAvail)
        len = usAreaAvail;

    usTemp = uartRxBuffer->usRptr;
    for(i = 0; i < len; i++)
    {
        if(B_FALSE == cbuffer_read(uartRxBuffer, pBuf + i))
        {
            uartRxBuffer->usRptr = usTemp;
            return B_FALSE;
        }
    }
    return len;
}

/*****************************************************************************
* @brief  : uart发送
* @param  : uart: 串口句柄
*			pbuf: 发送缓存数据指针
*			len: 发送长度
* @retval :
* @date   : 2023-09-08
* @note   :
*****************************************************************************/
static void uart_send(UART_CH_T uart, uint8_t * pbuf, uint16_t len)
{
#if(STM32 == MCU_MANUFACTURER)
    HAL_UART_Transmit_DMA(uart, pbuf, len);
    hi_delay_ms(len/10);
#elif(GD32 == MCU_MANUFACTURER)
    switch(uart)
    {
        case USART0:
            usart0_dma_send(pbuf, len);
            break;
        case USART1:
            usart1_dma_send(pbuf, len);
            break;
        case USART2:
            usart2_dma_send(pbuf, len);
            break;
        case USART5:
            usart5_dma_send(pbuf, len);
            break;
        case UART3:
            //uart3_dma_send(pbuf, len);
            break;
        case UART4:
            //uart4_dma_send(pbuf, len);
            break;
        default:
            break;
    }
#endif
}


/*****************************************************************************
* @Name	  : hi_uart_callback
* @Brief  : 从指定的串口接收数据, 并给出新消息提示
* @Param  : usart_periph: 串口号
*			uartRxBuffer: 接收缓冲区地址
* @Retval :
* @Author :
* @Data   : 2022-08-16
* @Note   :
*****************************************************************************/
static void uart_callback(UART_CH_T usart_periph, Cbuffer_T * uartRxBuffer,  EventGroupHandle_t xEgp, EventBits_t xEgpBit)
{
    uint8_t ucData = 0;
    BaseType_t xResult;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

#if(STM32 == MCU_MANUFACTURER)
    /*接收中断触发后，将串口接收到的数据写入环形队列*/
    if(usart_periph->Instance->SR & (1 << 5))
    {
        ucData = (uint8_t)(usart_periph->Instance->DR & (uint8_t)0x00FF);
        cbuffer_write(uartRxBuffer, ucData);
    }

    /*空闲中断触发后，将环形队列中的数据读到目标数组*/
    if(usart_periph->Instance->SR & (1 << 4))
    {
        __HAL_UART_CLEAR_IDLEFLAG(usart_periph);
        uartRxBuffer->usWptrEnd = uartRxBuffer->usWptrIng;

        xResult = xEventGroupSetBitsFromISR(xEgp,
                xEgpBit,
                &xHigherPriorityTaskWoken);

        if(xResult != pdFALSE)
        {
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
    }
#elif(GD32 == MCU_MANUFACTURER)
    /*接收中断触发后，将串口接收到的数据写入环形队列*/
    if(RESET != usart_interrupt_flag_get(usart_periph, USART_INT_FLAG_RBNE))
    {
        ucData = usart_data_receive(usart_periph);
        cbuffer_write(uartRxBuffer, ucData);
    }

    /*空闲中断触发后，将环形队列中的数据读到目标数组*/
    if(RESET != usart_interrupt_flag_get(usart_periph, USART_INT_FLAG_IDLE))
    {
        // 空闲中断清除步骤：软件先读USART_STAT0，再读USART_DATA可清除该位。
        usart_flag_get(usart_periph, USART_FLAG_IDLE);
        ucData = usart_data_receive(usart_periph);
        //cbuffer_write(uartRxBuffer, ucData);
        uartRxBuffer->usWptrEnd = uartRxBuffer->usWptrIng;

        xResult = xEventGroupSetBitsFromISR(xEgp,
                xEgpBit,
                &xHigherPriorityTaskWoken);

        if(xResult != pdFALSE)
        {
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
    }
#endif
}

/*****************************************************************************
* @Name	  : uart_it_flag_init
* @Brief  : 串口中断标识初始化
* @Param  : usart_periph: [输入/出]
* @Retval :
* @Author :
* @Data   : 2024-01-18
* @Note   :
*****************************************************************************/
static void uart_it_flag_init(UART_CH_T usart_periph)
{
    #if(MCU_SERIES == MCU_GD32F4XX)
    
    #else
    __HAL_UART_ENABLE_IT(usart_periph, UART_IT_IDLE | UART_IT_RXNE);    // 允许串口空闲中断、接收中断
    __HAL_UART_DISABLE_IT(usart_periph, UART_IT_TC);                    // 禁止串口发送完成中断
    __HAL_UART_CLEAR_FLAG(usart_periph, UART_IT_TC);
    #endif
}


/*****************************************************************************
* @Name	  : vCatRxDataClear
* @Brief  : 清空接收数据缓冲区
* @Param  : None
* @Retval : None
* @Author :
* @Data   : 2022-08-16
* @Note   :
*****************************************************************************/
static void uart_func_rx_data_clr(const uart_data_t * tpt)
{
    memset(tpt->pt_rx_data, 0, tpt->rx_data_len);
//	rtt_printf( tpt->rtt_index, LOG_BG_RED"%s Rx data clear!\n\n", tpt->name_des );
}


/*****************************************************************************
* @Name	  : vCatTxDataClear
* @Brief  : 清空发送数据缓冲区
* @Param  : None
* @Retval : None
* @Author :
* @Data   : 2022-08-16
* @Note   :
*****************************************************************************/
static void uart_func_tx_data_clr(const uart_data_t * tpt)
{
    memset(tpt->pt_tx_data, 0, tpt->tx_data_len);
//	rtt_printf( tpt->rtt_index, "%s Tx data clear!\n\n", tpt->name_des );
}


/*****************************************************************************
* @Name	  : uart_func_init
* @Brief  : CAT1初始化
* @Param  : None
* @Retval : None
* @Author :
* @Data   : 2022-08-16
* @Note   :
*****************************************************************************/
static void uart_func_init(const uart_data_t * tpt)
{
    uart_func_rx_data_clr(tpt);       // 接收缓存清空
    uart_func_tx_data_clr(tpt);       // 发送缓存清空
    uart_open(tpt->uart_id, tpt->tpt_buffer, tpt->rx_data_len);     // 打开串口, 循环队列长度与接收缓冲区大小一致
    rtt_printf(tpt->rtt_index, RTT_TEXT_GREEN"mcu to %s uart open!!!\r\n", tpt->name_des);

    uart_it_flag_init(tpt->uart_id);
}


/*****************************************************************************
* @Name	  : vMonitorModemInit
* @Brief  : CAT1初始化
* @Param  : None
* @Retval : None
* @Author :
* @Data   : 2022-08-16
* @Note   :
*****************************************************************************/
static void uart_func_open(const uart_data_t * tpt)
{
    uart_open(tpt->uart_id, tpt->tpt_buffer, tpt->rx_data_len);     // 打开串口
}

/*****************************************************************************
* @Name	  : vMonitorModemInit
* @Brief  : CAT1初始化
* @Param  : None
* @Retval : None
* @Author :
* @Data   : 2022-08-16
* @Note   :
*****************************************************************************/
static void uart_func_close(const uart_data_t * tpt)
{
    uart_close(tpt->uart_id, tpt->tpt_buffer);     // 关闭串口
}

/*****************************************************************************
* @Name	  : MonitorModemUartCallBack
* @Brief  : 串口回调函数
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2023-03-23
* @Note   :
*****************************************************************************/
static void uart_func_callback(const uart_data_t * tpt)
{
    uart_callback(tpt->uart_id,  tpt->tpt_buffer, tpt->msg_egp, tpt->msg_egp_bit);
}


/*****************************************************************************
* @Name	  : usMonitorModemReadData
* @Brief  : 读取接收数据
* @Param  : None
* @Retval : 实际读取的数据长度
* @Author :
* @Data   : 2023-03-28
* @Note   :
*****************************************************************************/
static uint16_t uart_func_read_data(const uart_data_t * tpt)
{
    uint16_t len = uart_read(tpt->tpt_buffer, tpt->pt_rx_data, tpt->rx_data_len);
    //rtt_printf( tpt->rtt_index, LOG_TEXT_MAGENTA"%s Rx: %s\n", tpt->name_des, (const char *)tpt->pt_rx_data );
    return len;
}


/*****************************************************************************
* @Name	  : vMonitorModemSendData
* @Brief  : 发送数据
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2023-03-28
* @Note   :
*****************************************************************************/
static void uart_func_send_data(const uart_data_t * tpt, uint16_t len)
{
    uart_send(tpt->uart_id, tpt->pt_tx_data, len);
    //rtt_printf( tpt->rtt_index, "%s Tx: %s\n", tpt->name_des, (const char *)tpt->pt_tx_data );
}

/*****************************************************************************
* @Name	  : vMonitorModemGetNewMsgBit
* @Brief  : 获取新消息标志
* @Param  : None
* @Retval : B_TRUE: 有新消息,  B_FALSE: 没有新消息
* @Author :
* @Data   : 2023-03-28
* @Note   :
*****************************************************************************/
static uint8_t uart_func_get_new_msg_bit(const uart_data_t * tpt)
{
    EventBits_t bitNewMsgEgp = xEventGroupGetBits(tpt->msg_egp);

    if((bitNewMsgEgp & tpt->msg_egp_bit) == tpt->msg_egp_bit)
        return B_TRUE;
    else
        return B_FALSE;
}

/*****************************************************************************
* @Name	  : vMonitorModemClearNewMsgBit
* @Brief  : 清除新消息事件组标志位
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2023-03-28
* @Note   :
*****************************************************************************/
static void uart_func_clr_new_msg_bit(const uart_data_t * tpt)
{
    xEventGroupClearBits(tpt->msg_egp, tpt->msg_egp_bit);      // 清除消息接收标志
}

/*****************************************************************************
* @Name	  : vMonitorModemWaitNewMsgBit
* @Brief  : 等待新消息事件组置位
* @Param  : timeout_second: 最长等待时间, 单位S
* @Retval :
* @Author :
* @Data   : 2023-03-28
* @Note   :
*****************************************************************************/
static uint8_t uart_func_wait_new_msg_bit(const uart_data_t * tpt, uint8_t timeout_second)
{
    EventBits_t bitsUartNewMsgEgp;
    bitsUartNewMsgEgp = xEventGroupWaitBits(tpt->msg_egp,       /* 标志事件组句柄 */
                                            tpt->msg_egp_bit,   /* 等待标志置位 */
                                            pdTRUE,             /* 退出前标志清零 */
                                            pdFALSE,            /* 是否等待所有的标志均被置位 */
                                            timeout_second * 1000);           /* 最长等待时间 */

    if((bitsUartNewMsgEgp & tpt->msg_egp_bit) == tpt->msg_egp_bit)
    {
        return B_TRUE;
    }
    else
        return B_FALSE;
}



/*****************************************************************************
* @brief  : uart初始化
* @param  : None
* @retval : None
* @date   : 2023-09-12
* @note   :
*****************************************************************************/
void hi_uart_init(void)
{
#if(PRODUCT_MODEL == CMDT3521A)
    usart0_init();
    usart2_init();
    usart5_init();
#elif(PRODUCT_MODEL == CMDT3521B)
    usart0_init();
    usart2_init();
    usart5_init();
#elif(PRODUCT_MODEL == CMDT3521C)
    MX_USART1_UART_Init();
    MX_USART3_UART_Init();
    MX_USART6_UART_Init();
#elif(PRODUCT_MODEL == CMDT3521D)

#elif(PRODUCT_MODEL == CMDT3521E)

#elif(PRODUCT_MODEL == CMDT3521F)

#endif
}


/*****************************************************************************
* @Name	  : uart_func_struct_init
* @Brief  : 函数指针初始化
* @Param  : tpt_fun: [输入/出]
**			 tpt_data: [输入/出]
* @Retval :
* @Author :
* @Data   : 2023-05-29
* @Note   :
*****************************************************************************/
void hi_uart_func_struct_init(uart_func_t * tpt_fun, const uart_data_t * tpt_config_data)
{
    tpt_fun->tpt_data    = (uart_data_t *)tpt_config_data;
    tpt_fun->init        = uart_func_init;
    tpt_fun->rx_data_clr = uart_func_rx_data_clr;
    tpt_fun->tx_data_clr = uart_func_tx_data_clr;
    tpt_fun->uart_open 	 = uart_func_open;
    tpt_fun->uart_close  = uart_func_close;
    tpt_fun->callback 	 = uart_func_callback;
    tpt_fun->read 		 = uart_func_read_data;
    tpt_fun->send 		 = uart_func_send_data;
    tpt_fun->get_newmsg  = uart_func_get_new_msg_bit;
    tpt_fun->clr_newmsg  = uart_func_clr_new_msg_bit;
    tpt_fun->wait_newmsg = uart_func_wait_new_msg_bit;
}


