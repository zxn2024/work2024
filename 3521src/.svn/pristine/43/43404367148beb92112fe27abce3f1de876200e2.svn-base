/*************************************** Copyright ******************************
*
*           (C) Copyright 2022, Shaanxi Tianji Comunication
*                          All Rights Reserved
*
* FileName   : bsp_public_io.h
* Version    : none
* Author     : ZhangWei
* Date       : 2022-08-02
* Description: none
*******************************************************************************/

#ifndef __BSP_PUBLIC_DRV_H_
#define __BSP_PUBLIC_DRV_H_

#include "stdint.h"
#include "system_config.h"

extern const uint8_t rf_sys_name[RF_SYS_NUM][10];		//系统名称
extern const uint8_t rf_ch_name[RF_CH_NUM][4];	//通道名称
extern const uint8_t rf_band_name[BAND_END][10];

void name_4g_replace(uint8_t band);		        //根据4G有效频段显示4G频段名称
    
void run_time_add(void);       			        //系统运行时间计时, 1ms时基定时器中调用
uint32_t get_run_time(void);    			    //获取系统运行时间, 单位ms
void printf_run_time(uint8_t windown_id);	    //打印系统运行时间

#endif
