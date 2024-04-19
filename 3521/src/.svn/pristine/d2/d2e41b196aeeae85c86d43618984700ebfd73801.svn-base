/*************************************** Copyright ******************************
*
* (C) Copyright 2023, Shaanxi Tianji Communication
* All Rights Reserved
*
* FileName   : bsp_rf_global.h
* Version    : none
* Author     :
* Date       : 2023-10-18
* Description: none
*******************************************************************************/
//#include <stdint.h>
#ifndef __BSP_RF_GLOBAL_H_
#define __BSP_RF_GLOBAL_H_

#include "system_config.h"

typedef struct
{
    RfSysPublic_T 	t_public;
    RfChPara_T  	t_ch[RF_CH_NUM];
} RfSystem_T;

extern DevPublicPara_T tDevPublicPara;   //与硬件相关的公共参数
extern RfSystem_T      t_rf_sys[ RF_SYS_NUM ];

uint8_t  set_valid_bw(uint8_t sys, uint8_t bw);			//设置有效的频段
uint8_t  set_valid_band(uint8_t sys, uint8_t band);		//设置有效的带宽
uint32_t set_valid_freq(uint8_t sys, uint32_t freq);	//设置有效的频点

uint8_t  get_valid_band(uint8_t sys);			//获取有效的频段
uint8_t  get_valid_bw(uint8_t sys);				//获取有效的带宽
uint32_t get_valid_freq( uint8_t sys );			//获取有效的频点	
#endif





