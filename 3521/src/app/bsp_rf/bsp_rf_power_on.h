/*************************************** Copyright ******************************
*
* (C) Copyright 2023, Shaanxi Tianji Communication
* All Rights Reserved
*                         
* FileName   : rf_init.h   
* Version    : none		
* Author     : 			
* Date       : 2023-11-20         
* Description: none   
*******************************************************************************/
//
#ifndef __RF_INIT_H_
#define __RF_INIT_H_
#include <stdint.h>

void load_default_para_at_pow_on( void );		//上电时加载默认的配置文件, 在任务切换使能前调用
void load_default_para_at_run_start( void );	//射频运行前 或 切换频段后的默认参数载入	

uint8_t load_8668_bin_in_task( uint8_t sys );	//任务中加载8668 bin 文件 在任务中 while 循环前调用, 

#endif





