/**
 * **********************************************************************************
 * @copyright Copyright (c) 2023 Shaanxi Tianji Communication Technologies Co., Ltd.
 *                         
 * @file        adc.h   
 * @author      WangJun
 * @version     None
 * @date        2023-09-07     
 * @brief       None   
 * 
 * **********************************************************************************
 */

#ifndef __ADC_H_
#define __ADC_H_

#include "main.h"

#ifdef __cplusplus
 extern "C" {
#endif

void adc0_init(uint16_t *pbuf, uint16_t len);
void adc2_init(uint16_t *pbuf, uint16_t len);

#ifdef __cplusplus
}
#endif

#endif
