/*************************************** Copyright ******************************
*
* (C) Copyright 2022, Shaanxi Tianji Communication
* All Rights Reserved
*                         
* FileName   : bsp_rf_pow.h   
* Version    : none		
* Author     : ZhangWei			
* Date       : 2022-08-08         
* Description: none   
*******************************************************************************/
//#include <stdint.h>
#ifndef __BSP_RF_POW_H_
#define __BSP_RF_POW_H_


#include "system_config.h"

/*---------------------- 结构体 ----------------------------------------*/

typedef enum
{
    ADC_VALUE_TYPE_ALC = 0,	//用于ALC的采样值
    ADC_VALUE_TYPE_8668		//8668后的采样值
} ADC_VALUE_TYPE_E;

typedef struct
{
	int8_t   start_pow;			//功率表中的起始功率值
	int8_t   cali_value;		//校准值
	uint8_t  valid_num;			//有效的数据个数
	uint8_t  step;				//步进 1:1dB, 2:0.5dB
	uint16_t table[ 94 ];		//最大储存94个功率数据
}POW_TABLE_PARA_T;

typedef struct
{
    AdcValue_T        * ptAdc;     //ADC采样值, 指向adc数据采样保存位置
    POW_TABLE_PARA_T  * ptList;    //功率表
    PowValue_T          pow;
} AdcToPow_T;

/*----------------------------------------------------------------------*/


extern POW_TABLE_PARA_T t_pow_table[RF_SYS_NUM][RF_CH_NUM];		//保存从FLASH中载入的功率表数据, 仅供载入输
extern POW_TABLE_PARA_T t_pow_table_8668[RF_SYS_NUM];			//用于保存8668后检波的ADC值, 主机的下行 / 从机的上行

void pow_init( void );       //功率参数初始化
void pow_convert( void );    //ADC值转换为功率值

void set_pow_cali_offset( uint8_t sys, uint8_t ch, int8_t calOffset );  //设置校准参数
void set_pow_temp_offset( uint8_t sys, uint8_t ch, int8_t tempOffset ); //设置温补参数
int8_t get_ch_pow_for_alc( uint8_t sys, uint8_t ch );	//获取指定通道的输出功率值
float get_float_ch_pow_for_alc(uint8_t sys, uint8_t ch);

void set_pow_cali_offset_8668( uint8_t sys, int8_t calOffset );  //设置校准参数
void set_pow_temp_offset_8668( uint8_t sys, int8_t tempOffset ); //设置温补参数
int8_t get_ch_pow_for_8668( uint8_t sys, uint8_t ch );	//获取指定通道的输出功率值
float get_float_ch_pow_for_8668(uint8_t sys, uint8_t ch);

uint16_t get_ch_adc_value(uint8_t sys, uint8_t ch, uint8_t type);


extern AdcToPow_T 		 t_ch_pow[RF_SYS_NUM][RF_CH_NUM];       //各系统通道对应的功率值
extern AdcValue_T 		 *t_ch_adc[RF_SYS_NUM][RF_CH_NUM];      //各系统通道对应的检波电压
extern AdcToPow_T 		 t_ch_pow_8668[RF_SYS_NUM];             // 8668功率值
extern AdcValue_T 		 *t_ch_adc_8668[RF_SYS_NUM];            // 8668检波值

#endif

