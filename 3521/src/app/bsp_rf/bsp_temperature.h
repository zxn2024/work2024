/*************************************** Copyright ******************************
*
* (C) Copyright 2022, Shaanxi Tianji Communication
* All Rights Reserved
*                         
* FileName   : bsp_temperature.h   
* Version    : none		
* Author     : ZhangWei			
* Date       : 2022-10-20         
* Description: none   
*******************************************************************************/

#ifndef __BSP_TEMPERATURE_H_
#define __BSP_TEMPERATURE_H_

#include <stdint.h>

//温度补偿表
typedef struct
{
    int8_t scTemperature; //温度值
    int8_t scGain;        //增益补偿值
    int8_t scPower;       //功率补偿值
} stTempData;


void tmp_cnt_in_time_isr(void); //定时器计数

void tmp_init(void);         //初始化数据
int8_t get_pow_tc_value(uint8_t sys, uint8_t ch);		//获取功率温补值
int8_t get_gain_tc_value(uint8_t sys, uint8_t ch);	//获取增益温补值

void tmp_in_task(void);    //任务中调用, 计算温度补偿值
int8_t vGetTmpValue(void);            //获取温度值
#endif





