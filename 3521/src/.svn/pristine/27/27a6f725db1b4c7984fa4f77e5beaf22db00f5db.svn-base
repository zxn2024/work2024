/*************************************** Copyright ******************************
*
* (C) Copyright 2023, Shaanxi Tianji Communication
* All Rights Reserved
*                         
* FileName   : bsp_rf_dev_state.h   
* Version    : none		
* Author     : 			
* Date       : 2023-10-18         
* Description: none   
*******************************************************************************/
#ifndef __BSP_RF_DEV_STATE_H_
#define __BSP_RF_DEV_STATE_H_
#include "system_config.h"
#include <stdint.h>

uint8_t get_band_exist( uint8_t band );
void rf_power_source_set( uint8_t sys, uint8_t ch, uint8_t state );     //设置链路电源开关状态
void rf_sys_open_state_set( uint8_t sys, BOOL_E state );                //设置系统开启状态
void rf_sys_sync_state_set( uint8_t sys, SYNC_STATE_E state );          //设置同步状态
void rf_dev_run_mode_set( uint8_t sys, WORK_MODE_E state );             //设置运行模式
void rf_sys_enable_set( uint8_t sys, uint8_t state );                   //设置链路使能
void rf_sys_run_state_set( uint8_t sys, OPEN_CLOSE_E state );           //设置运行状态
void rf_sys_oplc_en_set( uint8_t sys, uint8_t state );                  //设置开环工控使能
void rf_sys_iso_en_set( uint8_t sys, uint8_t state );                   //设置隔离度检测使能

#endif





