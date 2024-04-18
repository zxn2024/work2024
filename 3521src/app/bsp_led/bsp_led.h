/*************************************** Copyright ******************************
*
* (C) Copyright 2022, Shaanxi Tianji Communication
* All Rights Reserved
*                         
* FileName   : bsp_led.h   
* Version    : none		
* Author     : ZhangWei			
* Date       : 2022-08-24         
* Description: none   
*******************************************************************************/
#ifndef __BSP_LED_H_
#define __BSP_LED_H_

#include <stdint.h>

void led_cnt_in_time_isr( void );
void led_init( void );
void led_in_task( void );    

#endif


