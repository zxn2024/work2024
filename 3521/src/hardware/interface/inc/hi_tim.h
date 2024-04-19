/*************************************** Copyright ******************************
*
* (C) Copyright 2023, Shaanxi Tianji Communication
* All Rights Reserved
*                         
* FileName   : hi_tim.h   
* Version    : none		
* Author     : 			
* Date       : 2023-05-26         
* Description: 定时器的通用操作接口，支持多型号MCU，MCU列表请看MCU_config.h
    
*******************************************************************************/
//
#ifndef __HI_TIM_H__
#define __HI_TIM_H__


#include "tim.h"
#include "MCU_config.h"

/*---------------------- 端口定义 --------------------------------------*/
#if(MCU_SERIES == MCU_GD32F4XX)
    #define HTIM0 TIMER0
    #define HTIM1 TIMER1
    #define HTIM2 TIMER2
    #define HTIM3 TIMER3
    #define HTIM4 TIMER4
    
    #define PWMCH0 (TIMER_CH_0)
    #define PWMCH1 (TIMER_CH_1)
    #define PWMCH2 (TIMER_CH_2)
    #define PWMCH3 (TIMER_CH_3)
#elif(MCU_SERIES == MCU_STM32F4XX)

    #define HTIM1  &htim1
    #define HTIM2  &htim2
    #define HTIM3  &htim3
    #define HTIM4  &htim4
    #define HTIM5  &htim5
    #define HTIM11 &htim11

    #define PWMCH1 (TIM_CHANNEL_1)
    #define PWMCH2 (TIM_CHANNEL_2)
    #define PWMCH3 (TIM_CHANNEL_3)
    #define PWMCH4 (TIM_CHANNEL_4)
    
#elif(MCU_SERIES == MCU_STM32H5XX)
#endif
/*---------------------- 宏定义  ---------------------------------------*/


/*---------------------- 声明 ------------------------------------------*/


/*****************************************************************************
* @Name	  : hiSpiRead
* @brief  : Spi 读函数
* @param  : ch ：端口号,见上方端口定义
* @param  : pBuf ：数据缓冲区
* @param  : len ：数据长度
* @param  : timeout ：超时时间
* @Retval  : Status_t
* @author :
* @Data   :
* @Note   :
*****************************************************************************/
void hi_tim_init(void);

/*****************************************************************************
* @Name	  : hiSpiRead
* @brief  : Spi 读函数
* @param  : ch ：端口号,见上方端口定义
* @param  : pBuf ：数据缓冲区
* @param  : len ：数据长度
* @param  : timeout ：超时时间
* @Retval  : Status_t
* @author :
* @Data   :
* @Note   :
*****************************************************************************/
void hi_tim_pwm_set(TIM_T tim, PWM_CH_T pwmCh, uint16_t value);

/*****************************************************************************
* @Name	  : hi_tim_pwm_get
* @brief  : pwm 比较值读取函数
* @param  : tim ：定时器端口号,见上方端口定义
* @param  : pwmCh ：pwm 通道号
* @param  : value ：读取到的 pwm 值
* @author :
* @Data   :
* @Note   :
*****************************************************************************/
uint16_t hi_tim_pwm_get(TIM_T tim, PWM_CH_T pwmCh);



#endif
