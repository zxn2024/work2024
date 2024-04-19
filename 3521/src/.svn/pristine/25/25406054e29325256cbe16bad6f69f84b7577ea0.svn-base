/***************************** Copyright ****************************************
*
* (C) Copyright 2023, Shaanxi Tianji Communication
*  All Rights Reserved
*
* FileName   : bsp_gpio_drv.c
* Version    : none
* Author     :
* Date       :
* Description: none
*******************************************************************************/


#include "hi_tim.h"


/*---------------------- 端口定义 --------------------------------------*/


/*---------------------- 宏定义  ---------------------------------------*/


/*---------------------- 结构体 ----------------------------------------*/


/*---------------------- 常量 ------------------------------------------*/


/*---------------------- 全局变量 --------------------------------------*/


/*---------------------- 局部变量 --------------------------------------*/


/*---------------------- 声明 ------------------------------------------*/
#if(STM32 == MCU_MANUFACTURER)
/*****************************************************************************
* @Name	  : hi_tim_pwm_start
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
static void hi_tim_pwm_start(TIM_T tim, PWM_CH_T pwmCh)
{
    HAL_TIM_PWM_Start(tim, pwmCh);
}
#endif


/*****************************************************************************
* @Name	  : hi_tim_pwm_set
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
void hi_tim_pwm_set(TIM_T tim, PWM_CH_T pwmCh, uint16_t value)
{
    if(value > 999)
        value = 999;

#if(PRODUCT_MODEL == CMDT3521A)
    timer_channel_output_pulse_value_config(tim, pwmCh, value);
#elif(PRODUCT_MODEL == CMDT3521B)
    timer_channel_output_pulse_value_config(tim, pwmCh, value);
#elif(PRODUCT_MODEL == CMDT3521C)
    __HAL_TIM_SET_COMPARE(tim, pwmCh, value);
#elif(PRODUCT_MODEL == CMDT3521D)

#elif(PRODUCT_MODEL == CMDT3521E)

#elif(PRODUCT_MODEL == CMDT3521F)

#endif
}


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
uint16_t hi_tim_pwm_get(TIM_T tim, PWM_CH_T pwmCh)
{
    uint16_t value = 0;

#if(PRODUCT_MODEL == CMDT3521A)
    value = timer_channel_capture_value_register_read(tim, pwmCh);
#elif(PRODUCT_MODEL == CMDT3521B)
    value = timer_channel_capture_value_register_read(tim, pwmCh);
#elif(PRODUCT_MODEL == CMDT3521C)
    value = __HAL_TIM_GET_COMPARE(tim, pwmCh);
#endif
    return value;
}
/*****************************************************************************
* @Name	  : hi_tim_init
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
void hi_tim_init(void)
{
#if(PRODUCT_MODEL == CMDT3521A)
    tim0_init();
    tim1_init();
    tim2_init();
    tim3_init();
    tim4_init();
    
    // hi_tim_pwm_set(HTIM0, PWMCH0, 999);
    // hi_tim_pwm_set(HTIM0, PWMCH1, 999);
    // hi_tim_pwm_set(HTIM0, PWMCH2, 999);
    // hi_tim_pwm_set(HTIM0, PWMCH3, 999);

    // hi_tim_pwm_set(HTIM1, PWMCH0, 999);
    // hi_tim_pwm_set(HTIM1, PWMCH1, 999);
    // hi_tim_pwm_set(HTIM1, PWMCH2, 999);
    // hi_tim_pwm_set(HTIM1, PWMCH3, 999);
    
    // hi_tim_pwm_set(HTIM2, PWMCH0, 999);
    // hi_tim_pwm_set(HTIM2, PWMCH1, 999);
    // hi_tim_pwm_set(HTIM2, PWMCH2, 999);
    
    // hi_tim_pwm_set(HTIM3, PWMCH2, 999);
    // hi_tim_pwm_set(HTIM3, PWMCH3, 999);
    
    // hi_tim_pwm_set(HTIM4, PWMCH0, 999);
    // hi_tim_pwm_set(HTIM4, PWMCH1, 999);
    
#elif(PRODUCT_MODEL == CMDT3521B)
    tim0_init();
    tim1_init();
    tim2_init();
    tim3_init();
    tim4_init();
    
    // hi_tim_pwm_set(HTIM0, PWMCH0, 999);
    // hi_tim_pwm_set(HTIM0, PWMCH1, 999);
    // hi_tim_pwm_set(HTIM0, PWMCH2, 999);
    // hi_tim_pwm_set(HTIM0, PWMCH3, 999);

    // hi_tim_pwm_set(HTIM1, PWMCH0, 999);
    // hi_tim_pwm_set(HTIM1, PWMCH1, 999);
    // hi_tim_pwm_set(HTIM1, PWMCH2, 999);
    // hi_tim_pwm_set(HTIM1, PWMCH3, 999);
    
    // hi_tim_pwm_set(HTIM2, PWMCH0, 999);
    // hi_tim_pwm_set(HTIM2, PWMCH1, 999);
    // hi_tim_pwm_set(HTIM2, PWMCH2, 999);
    
    // hi_tim_pwm_set(HTIM3, PWMCH2, 999);
    // hi_tim_pwm_set(HTIM3, PWMCH3, 999);
    
    // hi_tim_pwm_set(HTIM4, PWMCH0, 999);
    // hi_tim_pwm_set(HTIM4, PWMCH1, 999);
    
#elif(PRODUCT_MODEL == CMDT3521C)
    MX_TIM1_Init();
    MX_TIM2_Init();
    MX_TIM3_Init();
    MX_TIM4_Init();
    MX_TIM5_Init();
    MX_TIM11_Init();

    // 使能PWM
    hi_tim_pwm_set(HTIM1, PWMCH1, 999);
    hi_tim_pwm_set(HTIM1, PWMCH2, 999);
    hi_tim_pwm_set(HTIM1, PWMCH3, 999);
    hi_tim_pwm_set(HTIM1, PWMCH4, 999);
    hi_tim_pwm_start(HTIM1, PWMCH1);
    hi_tim_pwm_start(HTIM1, PWMCH2);
    hi_tim_pwm_start(HTIM1, PWMCH3);
    hi_tim_pwm_start(HTIM1, PWMCH4);

    hi_tim_pwm_set(HTIM2, PWMCH1, 999);
    hi_tim_pwm_start(HTIM2, PWMCH1);
    
    hi_tim_pwm_set(HTIM3, PWMCH1, 999);
    hi_tim_pwm_set(HTIM3, PWMCH2, 999);
    hi_tim_pwm_set(HTIM3, PWMCH3, 999);
    hi_tim_pwm_start(HTIM3, PWMCH1);
    hi_tim_pwm_start(HTIM3, PWMCH2);
    hi_tim_pwm_start(HTIM3, PWMCH3);
    
    hi_tim_pwm_set(HTIM4, PWMCH3, 999);
    hi_tim_pwm_set(HTIM4, PWMCH4, 999);
    hi_tim_pwm_start(HTIM4, PWMCH3);
    hi_tim_pwm_start(HTIM4, PWMCH4);
    
    hi_tim_pwm_set(HTIM5, PWMCH1, 999);
    hi_tim_pwm_set(HTIM5, PWMCH2, 999);
    hi_tim_pwm_set(HTIM5, PWMCH3, 999);
    hi_tim_pwm_set(HTIM5, PWMCH4, 999);
    hi_tim_pwm_start(HTIM5, PWMCH1);
    hi_tim_pwm_start(HTIM5, PWMCH2);
    hi_tim_pwm_start(HTIM5, PWMCH3);
    hi_tim_pwm_start(HTIM5, PWMCH4);
    
    hi_tim_pwm_set(HTIM11, PWMCH1, 999);
    hi_tim_pwm_start(HTIM11, PWMCH1);
    
#elif(PRODUCT_MODEL == CMDT3521D)

#elif(PRODUCT_MODEL == CMDT3521E)

#elif(PRODUCT_MODEL == CMDT3521F)

#endif
}



