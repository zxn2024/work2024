/**
 * **********************************************************************************
 * @copyright Copyright (c) 2023 Shaanxi Tianji Communication Technologies Co., Ltd.
 *                         
 * @file        msg_handle.h   
 * @author      WangJun
 * @version     None
 * @date        2023-09-14     
 * @brief       None   
 * 
 * **********************************************************************************
 */

#ifndef __MSG_HANDLE_H_
#define __MSG_HANDLE_H_

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
 extern "C" {
#endif

uint16_t msg_split_escape(uint8_t * src, uint8_t * dest, uint16_t len);
uint16_t msg_escape(uint8_t * src, uint16_t len);
uint16_t msg_split_print(uint8_t * src, uint8_t * dst, uint16_t len);

#ifdef __cplusplus
}
#endif

#endif
