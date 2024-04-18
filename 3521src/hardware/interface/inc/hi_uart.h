/*************************************** Copyright ******************************
*
* (C) Copyright 2023, Shaanxi Tianji Communication
* All Rights Reserved
*
* FileName   : hi_uart.h
* Version    : none
* Author     :
* Date       : 2023-05-26
* Description: uart的通用操作接口，支持多型号MCU，MCU列表请看MCU_config.h

*******************************************************************************/
//
#ifndef __HI_UART_H__
#define __HI_UART_H__


#include "usart.h"
#include "MCU_config.h"

#include "FreeRTOS.h"
#include "event_groups.h"
#include "queue.h"

#include "hi_cbuffer.h"
/*---------------------- 端口定义 --------------------------------------*/
#if(MCU_SERIES == MCU_GD32F4XX)

#elif(MCU_SERIES == MCU_STM32F4XX || MCU_SERIES == MCU_STM32H5XX)

#endif
/*---------------------- 宏定义  ---------------------------------------*/

/*---------------------- 结 构 体 ---------------------------------------*/
typedef struct
{
    uint8_t		        name_des[10];   // 模块描述
    char                rtt_index;		// RTT打印窗口编号
    UART_CH_T  	        uart_id;        // 串口句柄

    EventGroupHandle_t  msg_egp;        // 事件组
    EventBits_t         msg_egp_bit;    // 事件组位

    QueueHandle_t       msg_queue;      // 消息队列
    UBaseType_t         msg_queue_len;  // 消息队列长度

    uint16_t 	        rx_data_len;    // 接收缓冲区长度
    uint16_t 	        tx_data_len;    // 发送缓冲区长度

    uint8_t 	   *    pt_rx_data;     // 接收缓存
    uint8_t 	   *    pt_tx_data;     // 发送缓存

    Cbuffer_T 	   *    tpt_buffer;     // 监控端口循环队列

} uart_data_t;


typedef struct
{
    uart_data_t   *  tpt_data;
    void (*init)(const uart_data_t * tpt);
    void (*rx_data_clr)(const uart_data_t * tpt);
    void (*tx_data_clr)(const uart_data_t * tpt);
    void (*uart_open)(const uart_data_t * tpt);
    void (*uart_close)(const uart_data_t * tpt);
    void (*callback)(const uart_data_t * tpt);
    uint16_t (*read)(const uart_data_t * tpt);
    void (*send)(const uart_data_t * tpt, uint16_t len);
    uint8_t (*get_newmsg)(const uart_data_t * tpt);
    void (*clr_newmsg)(const uart_data_t * tpt);
    uint8_t (*wait_newmsg)(const uart_data_t * tpt, uint8_t timeout_second);
} uart_func_t;


/*---------------------- 声明 ------------------------------------------*/
void hi_uart_init(void);
void hi_uart_func_struct_init(uart_func_t * tpt_fun, const uart_data_t * tpt_data);

#endif
