/**
 * **********************************************************************************
 * @copyright Copyright (c) 2023 Shaanxi Tianji Communication Technologies Co., Ltd.
 *
 * @file        hi_cbuffer.c
 * @author      WangJun
 * @version     None
 * @date        2023-09-12
 * @brief       None
 *
 * **********************************************************************************
 */

#include "hi_cbuffer.h"
#include <string.h>
#include "rtt.h"
#include "MCU_config.h"

/*---------------------- 端口定义 ----------------------*/


/*---------------------- 宏 定 义 ----------------------*/


/*---------------------- 结 构 体 ----------------------*/


/*---------------------- 常    量 ----------------------*/


/*---------------------- 全局变量 ----------------------*/


/*---------------------- 局部变量 ----------------------*/


/*---------------------- 函    数 ----------------------*/
/*****************************************************************************
* @Name	  : cbuffer_init
* @Brief  : cbuffer初始化
* @Param  : uartRxBuffer: 接收缓冲区地址
*           uartRxBufLen: 缓冲区大小
* @Retval : 成功/失败
* @Author :
* @Data   : 2022-08-16
* @Note   :
*****************************************************************************/
int8_t cbuffer_init(Cbuffer_T * uartRxBuffer, uint16_t uartRxBufLen)
{
    if(NULL == uartRxBuffer)
        return B_FALSE;

    if(uartRxBuffer->ucpBuf != NULL)
    {
        if(uartRxBufLen == uartRxBuffer->usMaxLen)
        {
            uartRxBuffer->usRptr = 0;
            uartRxBuffer->usWptrIng = 0;
            uartRxBuffer->usWptrEnd = 0;
            return B_TRUE;
        }
        else
            free(uartRxBuffer->ucpBuf);
    }

    uartRxBuffer->ucpBuf = malloc(uartRxBufLen);
    if(NULL == uartRxBuffer->ucpBuf)
    {
        rtt_printf(RTT_OS, RTT_TEXT_RED"[ERROR] -->malloc fail!\r\n");
        return B_FALSE;
    }
    memset(uartRxBuffer->ucpBuf, 0, uartRxBufLen);
    uartRxBuffer->usMaxLen = uartRxBufLen;
    uartRxBuffer->usRptr = 0;
    uartRxBuffer->usWptrIng = 0;
    uartRxBuffer->usWptrEnd = 0;
    return B_TRUE;
}


/*****************************************************************************
* @Name   : cbuffer_read
* @Brief  : 从环形队列中读1个字节到目标区域
* @Param  : uartRxBuffer: 接收缓冲区地址
*           pBuf: 读缓存区
* @Retval : 成功/失败
* @Author :
* @Data   : 2022-08-16
* @Note   :
*****************************************************************************/
int8_t cbuffer_read(Cbuffer_T * uartRxBuffer, uint8_t * pBuf)
{
    if((NULL == uartRxBuffer) || (NULL == pBuf))
        return B_FALSE;
    if(NULL == uartRxBuffer->ucpBuf)
        return B_FALSE;
    if(uartRxBuffer->usRptr == uartRxBuffer->usWptrEnd)
        return B_FALSE;

    *pBuf = uartRxBuffer->ucpBuf[uartRxBuffer->usRptr];
    uartRxBuffer->usRptr = (uartRxBuffer->usRptr + 1) % uartRxBuffer->usMaxLen;

    return B_TRUE;
}


/*****************************************************************************
* @Name   : cbuffer_write
* @Brief  : 从数据源写一个字节到环形队列
* @Param  : uartRxBuffer: 接收缓冲区地址
*           ucWriteData: 待写入数据
* @Retval : 成功/失败
* @Author :
* @Data   : 2022-08-16
* @Note   :
*****************************************************************************/
int8_t cbuffer_write(Cbuffer_T * uartRxBuffer, uint8_t ucWriteData)
{
    if(NULL == uartRxBuffer)
        return B_FALSE;
    if(NULL == uartRxBuffer->ucpBuf)
        return B_FALSE;

    uartRxBuffer->ucpBuf[uartRxBuffer->usWptrIng] = ucWriteData;
    uartRxBuffer->usWptrIng = (uartRxBuffer->usWptrIng + 1) % uartRxBuffer->usMaxLen;

    if(uartRxBuffer->usWptrIng == uartRxBuffer->usRptr)
        uartRxBuffer->usRptr = (uartRxBuffer->usRptr + 1) % uartRxBuffer->usMaxLen;
    return B_TRUE;
}


/*****************************************************************************
* @Name   : cbuffer_free
* @Brief  : 释放环形队列空间
* @Param  : uartRxBuffer: 接收缓冲区地址
* @Retval : 成功/失败
* @Author :
* @Data   : 2022-08-16
* @Note   :
*****************************************************************************/
int8_t cbuffer_free(Cbuffer_T * uartRxBuffer)
{
    if(NULL == uartRxBuffer)
        return B_FALSE;

    if(uartRxBuffer->ucpBuf != NULL)
        free(uartRxBuffer->ucpBuf);
    uartRxBuffer->usMaxLen = 0;
    uartRxBuffer->usRptr = 0;
    uartRxBuffer->usWptrIng = 0;
    uartRxBuffer->usWptrEnd = 0;
    return B_TRUE;
}


/*****************************************************************************
* @Name   : cbuffer_clear
* @Brief  : 清空环形队列
* @Param  : uartRxBuffer: 接收缓冲区地址
* @Retval : 成功/失败
* @Author :
* @Data   : 2022-08-16
* @Note   :
*****************************************************************************/
int8_t cbuffer_clear(Cbuffer_T * uartRxBuffer)
{
    if(NULL == uartRxBuffer)
        return B_FALSE;
    if(NULL == uartRxBuffer->ucpBuf)
        return B_FALSE;

    uartRxBuffer->usWptrIng = uartRxBuffer->usRptr;
    uartRxBuffer->usWptrEnd = uartRxBuffer->usRptr;
    return B_TRUE;
}
