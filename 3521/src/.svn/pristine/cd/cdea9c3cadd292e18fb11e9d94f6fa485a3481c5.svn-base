/*************************************** Copyright ******************************
*
* (C) Copyright 2023, Shaanxi Tianji Communication
* All Rights Reserved
*                         
* FileName   : bsp_rf_cali.h   
* Version    : none		
* Author     : 			
* Date       : 2023-11-15         
* Description: none   
*******************************************************************************/
#ifndef __BSP_RF_CALI_H_
#define __BSP_RF_CALI_H_

#include <stdint.h>
#include "system_config.h"

typedef struct
{
	int8_t 	gain_offset;	//增益校准
	int8_t 	pow_offset1;	//功率校准1
	int8_t 	pow_offset2;	//功率校准2
	uint8_t lna_att1;		//lna1 旁路校准
	uint8_t lna_att2;		//lna2 旁路校准
}rf_cali_para_t;

extern rf_cali_para_t t_cali_para[RF_SYS_NUM][RF_CH_NUM];

#endif





