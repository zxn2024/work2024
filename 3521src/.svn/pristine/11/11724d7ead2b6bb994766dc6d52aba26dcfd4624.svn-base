/*************************************** Copyright ******************************
*
* (C) Copyright 2023, Shaanxi Tianji Communication
* All Rights Reserved
*                         
* FileName   : bsp_led_drv.h   
* Version    : none		
* Author     : ZhangWei			
* Date       : 2023-03-09         
* Description: none   
*******************************************************************************/
//
#ifndef __BSP_LED_DRV_H_
#define __BSP_LED_DRV_H_

#include <stdint.h>

void drv_led_cnt_in_time_isr(void);			//定时器状态设置, 只能在在定时器中断中调用

void drv_close_all_led(void);				//所有的灯均设置为灭的状态
void drv_fresh_all_led_state( void );				//设置所有的指示灯状态
void drv_led_water( void );					//流水灯
void drv_led_set_state_in_testmode( void );				//测试模式指示灯显示, 所有的灯1s闪烁
void drv_led_init(void);					//状态初始化, 包括寄存器, 初始状态
uint8_t get_led_water_end_sign( void );	//获取流水灯结束标识

#endif





