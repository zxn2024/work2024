/**
 * **********************************************************************************
 * @copyright Copyright (c) 2023 Shaanxi Tianji Communication Technologies Co., Ltd.
 *                         
 * @file        os_event.h   
 * @author      WangJun
 * @version     None
 * @date        2023-06-16     
 * @brief       None   
 * 
 * **********************************************************************************
 */

#ifndef __OS_EVENT_H_
#define __OS_EVENT_H_
#include "FreeRTOS.h"
#include "event_groups.h"


/**
  * @brief 串口新消息事件组位定义
  */
#define BIT_UART0_MSG       (1U << 0)   // 设置串口0消息提示事件掩码
#define BIT_UART1_MSG       (1U << 1)   // 设置串口1消息提示事件掩码
#define BIT_UART2_MSG       (1U << 1)   // 设置串口2消息提示事件掩码
#define BIT_UART3_MSG       (1U << 3)   // 设置串口3消息提示事件掩码
#define BIT_UART4_MSG       (1U << 4)   // 设置串口4消息提示事件掩码
#define BIT_UART5_MSG       (1U << 5)   // 设置串口5消息提示事件掩码
#define BIT_UART6_MSG       (1U << 6)   // 设置串口6消息提示事件掩码

#define BIT_UART_ALL_MSG    (0x00FFFFFF)

extern EventGroupHandle_t xUartNewMsgEgp;



/*
 * @brief ADC数据事件组位定义
 */
#define BIT_ADC0_DMA_COMPLATE    (1U << 0)   //ADC0 DMA转换完成
#define BIT_ADC0_DATA_VALID      (1U << 1)   //ADC0 数据有效
#define BIT_ADC2_DMA_COMPLATE    (1U << 2)   //ADC2 DMA转换完成
#define BIT_ADC2_DATA_VALID      (1U << 3)   //ADC2 数据有效

#define BIT_ADC_ALL             (0x00FFFFFF)

extern EventGroupHandle_t xAdcEgp;










void os_eventgroup_create(void);

#endif

