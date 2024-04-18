/*************************************** Copyright ******************************
*
* (C) Copyright 2022, Shaanxi Tianji Communication
* All Rights Reserved
*
* FileName   : bsp_rf_oplc.h
* Version    : none
* Author     : ZhangWei
* Date       : 2023-11-14
* Description: none
*******************************************************************************/
//#include <stdint.h>
#ifndef __BSP_RF_OPLC_H_
#define __BSP_RF_OPLC_H_

#include "stdint.h"
#include "system_config.h"

typedef struct
{
    uint8_t * pt_en;            //开环工控使能, 网管可配置
    int16_t * pt_rsrp;			//RSRP数据
    uint8_t * pt_att_rsrp;		//原始的由rsrp计算出来的衰减.
    int8_t  link_gate;       	//联动门限, 网管可配置
    int8_t  gain_offset;		//增益联动偏移量, 网管可配置
    int8_t  pow_offset;				//功率联动偏移量, 网管可配置
//	uint8_t gain_att;				//联动后的增益衰减值
    int8_t * pt_pow_point;			//联动后的功率启控点
    int8_t * pt_alc_point;			//上行ALC理论启控点
	int8_t * pt_alc_point_offset;	//
} OPLC_PARA_T;

extern OPLC_PARA_T tOplc[ RF_SYS_NUM ];

void oplc_init(void);	//线初始化, 然后再从FLASH载入相关参数
void oplc_cal(uint8_t sys);				//计算开环工控

void set_oplc_en(uint8_t sys, uint8_t state);			//设置开环工控使能
void set_oplc_link_para(uint8_t sys, uint8_t gate, int8_t gain_offset, int8_t pow_offset);	//设置开环工控联动参数

#endif





