/**
 * **********************************************************************************
 * @copyright Copyright (c) 2023 Shaanxi Tianji Communication Technologies Co., Ltd.
 *                         
 * @file        systick.h   
 * @author      WangJun
 * @version     None
 * @date        2023-09-04     
 * @brief       None   
 * 
 * **********************************************************************************
 */

#ifndef __SYSTICK_H_
#define __SYSTICK_H_


#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>

/* configure systick */
void systick_config(void);
/* delay a time in milliseconds */
void delay_1ms(uint32_t count);
/* delay decrement */
void delay_decrement(void);


void inc_tick(void);
uint32_t get_tick(void);

#ifdef __cplusplus
}
#endif

#endif
