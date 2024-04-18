/*************************************** Copyright ******************************
*
* (C) Copyright 2023, Shaanxi Tianji Communication
* All Rights Reserved
*
* FileName   : bsp_testmode_hal_set.h
* Version    : none
* Author     :
* Date       : 2023-12-19
* Description: none
*******************************************************************************/
//
#ifndef __BSP_TESTMODE_HAL_SET_H_
#define __BSP_TESTMODE_HAL_SET_H_
#include <stdint.h>
#include "system_config.h"

void tm_sys_close( uint8_t sys );	//关闭系统供电
void tm_sys_open(uint8_t band);

void tm_sys_fpga_pa_init(RF_BAND_E band);   // PA初始化
void tm_sys_fpga_4g_pa(RF_SWITCH_E state);      // 设置pa

#if LOCAL_MODULE_TYPE == _RU
void tm_sys_fpga_switch(IS_4G_5G_E mode, IS_TDD_E tdd, RF_SWITCH_E state);  // 设置时序
#endif

#endif




