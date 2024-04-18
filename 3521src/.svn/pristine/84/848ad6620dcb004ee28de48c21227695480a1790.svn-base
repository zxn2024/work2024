/*************************************** Copyright ******************************
*
*           (C) Copyright 2022, Shaanxi Tianji Comunication
*                          All Rights Reserved
*                         
* FileName   : bsp_rf_att_set.h   
* Version    : none		
* Author     : none			
* Date       : 2023-10-12         
* Description: none   
*******************************************************************************/

#ifndef __BSP_RF_ATT_SET_H_
#define __BSP_RF_ATT_SET_H_

#include <stdint.h>

void att_init(void);
void att_set(uint8_t sys, uint8_t ch);   //设置衰减器
void att_digit_set(uint8_t sys, uint8_t ch, uint8_t value);
void att_analog_set(uint8_t sys, uint8_t ch, uint8_t id, uint8_t value);
void att_analog_pwm_set(uint8_t sys, uint8_t ch, uint8_t id, uint16_t pwm_value);
uint16_t att_analog_pwm_get(uint8_t sys, uint8_t ch, uint8_t id);
#endif




