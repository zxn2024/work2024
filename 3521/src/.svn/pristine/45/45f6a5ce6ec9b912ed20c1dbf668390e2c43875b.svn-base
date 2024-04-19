/*************************************** Copyright ******************************
*
* (C) Copyright 2022, Shaanxi Tianji Communication
* All Rights Reserved
*                         
* FileName   : bsp_rf_att.h   
* Version    : none		
* Author     : ZhangWei			
* Date       : 2022-08-05         
* Description: none   
*******************************************************************************/
#ifndef __BSP_RF_ATT_H_
#define __BSP_RF_ATT_H_

#include <stdint.h>
#include "system_config.h"

typedef struct bsp_rf_att
{
	uint8_t      en;				//ENABLE: 有效,  DISABLE: 无效. 用于区分是否使用该衰减器
	uint8_t      refresh;			//刷新标识, TRUE: 刷新, FALSE: 不刷新
	uint8_t      max_num;			//最大可用衰减数量, LNA为可抵消的衰减器数量
	uint8_t      new_value;			//新设置的衰减值, lna为相应旁路状态
	uint8_t      cur_value;			//当前数据, 设置后保存, 用于判断衰减器状态是否有变化
	void         *handle; 			
}BSP_ATT_T;

typedef struct
{
	BSP_ATT_T lna1;
	BSP_ATT_T lna2;
	BSP_ATT_T digit_att1;
	BSP_ATT_T digit_att2;
	BSP_ATT_T analog_att1;
	BSP_ATT_T analog_att2;
}ATT_CH_T;


extern ATT_CH_T  *pt_ch_att[RF_SYS_NUM][RF_CH_NUM];	//根据系统指向不同的衰减器typedef struct

void att_init( void );
void att_set( uint8_t sys, uint8_t ch );

uint8_t get_att_num_can_use_for_alc( uint8_t sys, uint8_t ch );		//获取ALC可使用的衰减器剩余数量, 若为0, 则无多余的衰减可用
uint8_t get_att_num_can_use_for_8668( uint8_t sys );	//获取8668后面ADC可使用的衰减数量, 若为0, 则无多余的衰减可用

void att_set_force_fresh( uint8_t sys );

void att_all_set_max_value( uint8_t sys );	//所有att设置为最大值

#endif




