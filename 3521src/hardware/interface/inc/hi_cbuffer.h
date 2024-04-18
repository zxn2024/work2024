/**
 * **********************************************************************************
 * @copyright Copyright (c) 2023 Shaanxi Tianji Communication Technologies Co., Ltd.
 *                         
 * @file        hi_cbuffer.h   
 * @author      WangJun
 * @version     None
 * @date        2023-09-12     
 * @brief       None   
 * 
 * **********************************************************************************
 */

#ifndef __HI_CBUFFER_H_
#define __HI_CBUFFER_H_

#include <stdint.h>

#ifdef __cplusplus
 extern "C" {
#endif

#define _EN_MALLOC_UART_BUF     1       // 是否使能串口动态内存分配 0：静态分配；1：动态分配
typedef struct
{
    uint8_t *ucpBuf;
    uint16_t usMaxLen;
    uint16_t usRptr;
    uint16_t usWptrIng;
    uint16_t usWptrEnd;
}Cbuffer_T;

int8_t cbuffer_init(Cbuffer_T* uartRxBuffer,uint16_t uartRxBufLen);
int8_t cbuffer_read(Cbuffer_T* uartRxBuffer,uint8_t* pBuf);
int8_t cbuffer_write(Cbuffer_T* uartRxBuffer,uint8_t ucWriteData);
int8_t cbuffer_free(Cbuffer_T* uartRxBuffer);
int8_t cbuffer_clear(Cbuffer_T* uartRxBuffer);

#ifdef __cplusplus
}
#endif

#endif
