/*************************************** Copyright ******************************
*
* (C) Copyright 2023, Shaanxi Tianji Communication
* All Rights Reserved
*                         
* FileName   : bsp_rf_4g_band_sel.h   
* Version    : none		
* Author     : 			
* Date       : 2023-10-18         
* Description: none   
*******************************************************************************/
#ifndef __BSP_RF_4G_BAND_SEL_H_
#define __BSP_RF_4G_BAND_SEL_H_

#include <stdint.h>
#include "bsp_config.h"
#include "bi_port.h"
#include "ATT_analog.h"

typedef struct
{
	uint8_t  exist;				//通道是否存在, B_TRUE: 存在, B_FALSE: 不存在
	int8_t   up_alc;			//上行ALC起控点
	int8_t   up_8668_alc;		//上行8668启控点
	int8_t   dw_alc;			//下行ALC起控点
	int8_t   dw_8668_alc;		//下行8668起控点
	int8_t   up_gain_offset;	//上行增益偏移量
	int8_t   dw_gain_offset;	//下行增益偏移量
	int8_t   up_pow_offset;		//上行输出功率偏移量
	int8_t   dw_pow_offset;		//下行输出功率偏移量
	int8_t   oplc_gate;			//开环工控门限
	int8_t   oplc_gain_offset;	//开环工控增益偏移量
	int8_t   oplc_pow_offset;	//开环工控功率偏移量
	uint8_t  band_width;		//带宽
	uint32_t freq;				//频点
	uint8_t  scan_freq[64];		//搜网频点
	uint8_t  reserve[47];		
}para_rf_ch_t;

extern para_rf_ch_t t_rf_4g_save_para[4];	//用于flash保存的4g 4选一参数typedef struct;

#if LOCAL_MODULE_TYPE == _MU
extern ATT_analog_t *pt_att_analog_4g_up_1; 
extern ATT_analog_t *pt_att_analog_4g_up_2; 
extern ATT_analog_t *pt_att_analog_4g_dw; 
extern LNA_t 		*pt_lna_4g_up;    
extern LNA_t 		*pt_lna_4g_dw_1;     
extern LNA_t 		*pt_lna_4g_dw_2; 
extern PORT_t 		*pt_4g_dc_ctrl; 
#else
extern ATT_analog_t *pt_att_analog_4g_up; 
extern ATT_analog_t *pt_att_analog_4g_dw_1; 
extern ATT_analog_t *pt_att_analog_4g_dw_2; 
extern LNA_t 		*pt_lna_4g_up_1;    
extern LNA_t 		*pt_lna_4g_up_2;     
extern LNA_t 		*pt_lna_4g_dw; 
extern PORT_t 		*pt_4g_dc_ctrl; 
#endif

void rf_para_replace_after_4g_band_select( uint8_t band );	//4G更换频段后的射频参数更新
void pt_remap_after_4g_band_select( void );					//4G更换频段后的指针重定向

#endif





