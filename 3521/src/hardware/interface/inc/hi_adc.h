/*************************************** Copyright ******************************
*
* (C) Copyright 2023, Shaanxi Tianji Communication
* All Rights Reserved
*                         
* FileName   : hi_adc.h   
* Version    : none		
* Author     : 			
* Date       : 2023-05-26         
* Description: adc的通用操作接口，支持多型号MCU，MCU列表请看MCU_config.h
    
*******************************************************************************/
//
#ifndef __HI_ADC_H__
#define __HI_ADC_H__

#include "adc.h"
#include "MCU_config.h"

/*---------------------- 端口定义 --------------------------------------*/

// 是否使能多组求平均：0：多组平均取最大，1：大于多组最大值的0.8倍求平均， 2：多组均值求平均
#define ENABLE_AVE_GROUPS       2
// 是否使能每更新一组数据后求平均：0：10组数据都更新后求平均，1：每更新1组数据后求平均
#define ENABLE_AVE_UPDATA       0


#if(PRODUCT_MODEL == CMDT3521A)
/*---------------------- 宏 定 义 ----------------------*/
#define ADC0_CH_NUM	            ( 1	)       //采样通道数量(含温度通道)
#define ADC0_CONVERTED_NUM	    ( 10 )      //单通道采样次数
#define ADC0_CONVERTEN_TOTAL    (ADC0_CH_NUM * ADC0_CONVERTED_NUM)

#define ADC2_CH_NUM	            ( 9	)	    //采样通道数量
#define ADC2_CONVERTED_NUM	    ( 694 )     //单通道采样次数
#define ADC2_CONVERTEN_TOTAL    (ADC2_CH_NUM * ADC2_CONVERTED_NUM)  // 9.9936ms

#define ADC_CH_TOTAL_NUM        (ADC2_CH_NUM)
#define ADC_CH_ARRAY_NUM        ( 10 )      //10组数据 /*需要采集的组数，修改此值*/

/*---------------------- 结 构 体 ----------------------*/
// ADC采样结构体
// 与射频通道参数相关的ADC采集值
typedef struct
{
    uint16_t cnt;                           // ADC采集次数
    uint16_t AdcArray[ADC_CH_ARRAY_NUM];    // ADC值平均值
    AdcValue_T result;                      // 计算功率使用此值

} AdcArray_T, *psAdcArray_T;

/*---------------------- 全局变量 ----------------------*/
extern AdcArray_T tChAdcValue[ADC_CH_TOTAL_NUM]; // 每个通道10组数值
extern AdcArray_T tAdcTempValue;    


#elif(PRODUCT_MODEL == CMDT3521B)
/*---------------------- 宏 定 义 ----------------------*/
#define ADC0_CH_NUM	            ( 1	)       //采样通道数量(含温度通道)
#define ADC0_CONVERTED_NUM	    ( 10 )      //单通道采样次数
#define ADC0_CONVERTEN_TOTAL    (ADC0_CH_NUM * ADC0_CONVERTED_NUM)

#define ADC2_CH_NUM	            ( 9	)	    //采样通道数量
#define ADC2_CONVERTED_NUM	    ( 694 )     //单通道采样次数
#define ADC2_CONVERTEN_TOTAL    (ADC2_CH_NUM * ADC2_CONVERTED_NUM)  // 9.9936ms

#define ADC_CH_TOTAL_NUM        (ADC2_CH_NUM)
#define ADC_CH_ARRAY_NUM        ( 10 )      //10组数据 /*需要采集的组数，修改此值*/

/*---------------------- 结 构 体 ----------------------*/
// ADC采样结构体
// 与射频通道参数相关的ADC采集值
typedef struct
{
    uint16_t cnt;                           // ADC采集次数
    uint16_t AdcArray[ADC_CH_ARRAY_NUM];    // ADC值平均值
    AdcValue_T result;                      // 计算功率使用此值

} AdcArray_T, *psAdcArray_T;

/*---------------------- 全局变量 ----------------------*/
extern AdcArray_T tChAdcValue[ADC_CH_TOTAL_NUM]; // 每个通道10组数值
extern AdcArray_T tAdcTempValue;    



#elif(PRODUCT_MODEL == CMDT3521C)
/*---------------------- 宏 定 义 ----------------------*/
#define ADC0_CH_NUM	            ( 1	)       //采样通道数量(含温度通道)
#define ADC0_CONVERTED_NUM	    ( 10 )      //单通道采样次数
#define ADC0_CONVERTEN_TOTAL    (ADC0_CH_NUM * ADC0_CONVERTED_NUM)

#define ADC2_CH_NUM	            ( 9	)	    //采样通道数量
#define ADC2_CONVERTED_NUM	    ( 694 )     //单通道采样次数
#define ADC2_CONVERTEN_TOTAL    (ADC2_CH_NUM * ADC2_CONVERTED_NUM)  // 9.9936ms

#define ADC_CH_TOTAL_NUM        (ADC2_CH_NUM)
#define ADC_CH_ARRAY_NUM        ( 10 )      //10组数据 /*需要采集的组数，修改此值*/

/*---------------------- 结 构 体 ----------------------*/
// ADC采样结构体
// 与射频通道参数相关的ADC采集值
typedef struct
{
    uint16_t cnt;                           // ADC采集次数
    uint16_t AdcArray[ADC_CH_ARRAY_NUM];    // ADC值平均值
    AdcValue_T result;                      // 计算功率使用此值

} AdcArray_T, *psAdcArray_T;

/*---------------------- 全局变量 ----------------------*/
extern AdcArray_T tChAdcValue[ADC_CH_TOTAL_NUM]; // 每个通道10组数值
extern AdcArray_T tAdcTempValue;    

#elif(PRODUCT_MODEL == CMDT3521D)

#elif(PRODUCT_MODEL == CMDT3521E)

#elif(PRODUCT_MODEL == CMDT3521F)

#endif


/*---------------------- 声明 ------------------------------------------*/
/*****************************************************************************
* @Name	  : hiAdcInit
* @Brief  : adc初始化函数
* @Retval : 
* @Author : 
* @Data   : 
* @Note   : 
*****************************************************************************/
void hi_adc_init(void);

/*****************************************************************************
* @Name	  : hi_adc_start
* @Brief  : adc开启
* @Retval :
* @Author :
* @Data   :
* @Note   :
*****************************************************************************/
void hi_adc_start(void);
/*****************************************************************************
* @Name	  : hiAdcGetDMAData
* @brief  : adc DMA读取数据
* @param  : ch ：端口号,见上方端口定义
* @param  : pBuf ：取数据缓冲区
* @param  : len ：取数据长度
* @Retval  : Status_t
* @author : 
* @Data   : 
* @Note   : 默认使用循环DMA，双缓冲模式
*****************************************************************************/
Status_t hi_adc_get_DMA_data(ADC_CH_T ch, uint16_t *ptr_buf, uint16_t len);

int8_t adc_read_temperature(uint16_t data);

void hi_adc_get_value(void);

#if(MCU_SERIES == MCU_GD32F4XX)
void ADC_ConvFullCpltCallback(void);
void ADC_ConvHalfCpltCallback(void);
#elif(MCU_SERIES == MCU_STM32F4XX || MCU_SERIES == MCU_STM32H5XX)
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc);
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc);
void adc_start_dma(void);
#endif

#endif
