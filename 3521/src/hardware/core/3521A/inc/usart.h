/**
 * **********************************************************************************
 * @copyright Copyright (c) 2023 Shaanxi Tianji Communication Technologies Co., Ltd.
 *                         
 * @file        usart.h   
 * @author      WangJun
 * @version     None
 * @date        2023-09-06     
 * @brief       None   
 * 
 * **********************************************************************************
 */

#ifndef __USART_H_
#define __USART_H_

#include "main.h"

#ifdef __cplusplus
 extern "C" {
#endif

void usart0_init(void);
void usart1_init(void);
void usart2_init(void);
void usart5_init(void);

void usart0_dma_send(uint8_t * buffer, uint16_t len);
void usart1_dma_send(uint8_t * buffer, uint16_t len);
void usart2_dma_send(uint8_t * buffer, uint16_t len);
void usart5_dma_send(uint8_t * buffer, uint16_t len);

#ifdef __cplusplus
}
#endif

#endif
