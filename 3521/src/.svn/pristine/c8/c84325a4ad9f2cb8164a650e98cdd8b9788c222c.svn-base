/*************************************** Copyright ******************************
*
* (C) Copyright 2024, Shaanxi Tianji Communication
* All Rights Reserved
*                         
* FileName   : bsp_rf_866x.h   
* Version    : none		
* Author     : 			
* Date       : 2024-01-01         
* Description: none   
*******************************************************************************/
//#include <stdint.h>
#ifndef __BSP_RF_866X_H_
#define __BSP_RF_866X_H_

#include "bsp_config.h"

typedef struct
{	
	uint8_t ch_switch_state;	//通道开关状态
}rf_866x_para_t;

extern TRX8668_FPGA_t *pt_866x_sys[RF_SYS_NUM];
extern rf_866x_para_t t_rf_866x_para[RF_SYS_NUM];

uint32_t rf_866x_get_default_freq_point( uint8_t band );	//获取默认的频点
uint32_t rf_866x_get_default_band_width( uint8_t band );	//获取默认的带宽

uint32_t rf_866x_read_ch_mode_state( TRX8668_FPGA_t *tpt );
uint32_t rf_866x_read_bw( TRX8668_FPGA_t *tpt );
uint32_t rf_866x_read_loop_state( TRX8668_FPGA_t *tpt );
uint32_t rf_866x_get_default_freq_point( uint8_t band );	//获取默认频点
Status_t rf_866x_reload_bin( uint8_t sys, RF_BAND_E band, uint32_t band_width, uint32_t freq );	//过程中重新加载8668 bin 文件

void reg_866x_rw_struct_init( void );       //866x reg 读写结构体初始化
Status_t reg_866x_rw( uint32_t *tag );      //866x reg 读写
void reg_866x_set_addr( uint32_t addr );    //设置地址数据
void reg_866x_set_on_page_dat( uint8_t sys, uint8_t cmd, uint32_t dat );		//设置片内寄存器数据
void reg_866x_set_off_page_dat( uint8_t sys, uint8_t cmd, uint32_t dat );		//设置片外寄存器数据

Status_t reg_8668_get_addr_dat_in_tm( uint8_t cmd, uint8_t sys, uint8_t *pt, uint32_t *temp_dat );		//测试模式下读写寄存器

#endif





