/*************************************** Copyright ******************************
*
* (C) Copyright 2023, Shaanxi Tianji Communication
* All Rights Reserved
*                         
* FileName   : bsp_para_load.h   
* Version    : none		
* Author     : 			
* Date       : 2023-11-13         
* Description: none   
*******************************************************************************/
//
#ifndef __BSP_PARA_LOAD_H_
#define __BSP_PARA_LOAD_H_
#include <stdint.h>
#include "bsp_flash_struct.h"

extern tm_cali_rf_para  t_rf_4g_cali_para[ BAND_END ];	//用于flash保存的4g 4选一校准参数

void load_4g_factory_cali_data(void);		//载入4G通道出厂校准参数
void load_5g_factory_cali_data(void);		//载入5G通道出厂校准参数

uint8_t get_rf_cali_para_lna_att( uint8_t band, uint8_t ch, uint8_t lna_id );	//获取LNA ATT抵消值

#endif





