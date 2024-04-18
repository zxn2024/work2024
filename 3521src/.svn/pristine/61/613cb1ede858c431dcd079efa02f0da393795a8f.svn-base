/*************************************** Copyright ******************************
*
* (C) Copyright 2022, Shaanxi Tianji Communication
* All Rights Reserved
*                         
* FileName   : bsp_wdg.h   
* Version    : none		
* Author     : ZhangWei			
* Date       : 2023-10-07         
* Description: none   
*******************************************************************************/

#ifndef __BSP_WDG_H_
#define __BSP_WDG_H_
#include <stdint.h>

void wdg_para_init( void );						//初始化看门狗参数, 在任务创建之后调用一次
void wdg_egp_creat(void);						//事件组创建和初始化
void wdg_in_time_isr( const uint16_t period );	//定时器中调用, 计时和喂狗, period:定时器的周期, 单位ms, 
void wdg_in_task( void );						//在每个任务中调用

#endif

