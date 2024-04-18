/**
 * **********************************************************************************
 * @copyright Copyright (c) 2023 Shaanxi Tianji Communication Technologies Co., Ltd.
 *                         
 * @file        uart.h   
 * @author      WangJun
 * @version     None
 * @date        2023-09-06    
 * @brief       None   
 * 
 * **********************************************************************************
 */

#ifndef __UART_H_
#define __UART_H_

#include "main.h"

#ifdef __cplusplus
 extern "C" {
#endif

void uart3_init(void);
void uart4_init(void);
void uart3_dma_send(uint8_t * buffer, uint16_t len);
void uart4_dma_send(uint8_t * buffer, uint16_t len);

#ifdef __cplusplus
}
#endif

#endif
