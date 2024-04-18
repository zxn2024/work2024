/**
 * **********************************************************************************
 * @copyright Copyright (c) 2023 Shaanxi Tianji Communication Technologies Co., Ltd.
 *                         
 * @file        tim.c   
 * @author      WangJun
 * @version     None
 * @date        2023-09-06    
 * @brief       None   
 * 
 * **********************************************************************************
 */

#include "tim.h"


/*---------------------- 端口定义 ----------------------*/


/*---------------------- 宏 定 义 ----------------------*/


/*---------------------- 结 构 体 ----------------------*/


/*---------------------- 常    量 ----------------------*/


/*---------------------- 全局变量 ----------------------*/


/*---------------------- 局部变量 ----------------------*/


/*---------------------- 函    数 ----------------------*/
/*****************************************************************************
* @brief  : configure timer0
* @param  : None
* @retval : None
* @date   : 2023-09-06
* @note   : 
*****************************************************************************/
void tim0_init(void)
{
    /* enable the port clock */
    rcu_periph_clock_enable(RCU_GPIOE);
    /* enable TIM clock */
    rcu_periph_clock_enable(RCU_TIMER0);

    /**TIM0 GPIO Configuration
    PE9     ------> TIMER0_CH0  B3 UP 1
    PE11    ------> TIMER0_CH1  B39 UP 1
    PE13    ------> TIMER0_CH2  B39 UP 2
    PE14    ------> TIMER0_CH3  B8 UP 1
    */
    /* connect port to TIMER0_CH0 */
    gpio_mode_set(ATT_B3_UP_1_GPIO_Port, GPIO_MODE_AF, GPIO_PUPD_NONE, ATT_B3_UP_1_Pin);
    gpio_output_options_set(ATT_B3_UP_1_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, ATT_B3_UP_1_Pin);
    gpio_af_set(ATT_B3_UP_1_GPIO_Port, GPIO_AF_1, ATT_B3_UP_1_Pin);

    /* connect port to TIMER0_CH1 */
    gpio_mode_set(ATT_B39_UP_1_GPIO_Port, GPIO_MODE_AF, GPIO_PUPD_NONE, ATT_B39_UP_1_Pin);
    gpio_output_options_set(ATT_B39_UP_1_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, ATT_B39_UP_1_Pin);
    gpio_af_set(ATT_B39_UP_1_GPIO_Port, GPIO_AF_1, ATT_B39_UP_1_Pin);
    
    /* connect port to TIMER0_CH2 */
    gpio_mode_set(ATT_B39_UP_2_GPIO_Port, GPIO_MODE_AF, GPIO_PUPD_NONE, ATT_B39_UP_2_Pin);
    gpio_output_options_set(ATT_B39_UP_2_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, ATT_B39_UP_2_Pin);
    gpio_af_set(ATT_B39_UP_2_GPIO_Port, GPIO_AF_1, ATT_B39_UP_2_Pin);

    /* connect port to TIMER0_CH3 */
    gpio_mode_set(ATT_B8_UP_1_GPIO_Port, GPIO_MODE_AF, GPIO_PUPD_NONE, ATT_B8_UP_1_Pin);
    gpio_output_options_set(ATT_B8_UP_1_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, ATT_B8_UP_1_Pin);
    gpio_af_set(ATT_B8_UP_1_GPIO_Port, GPIO_AF_1, ATT_B8_UP_1_Pin);
    
    timer_oc_parameter_struct timer_ocintpara;
    timer_parameter_struct timer_initpara;

    rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);

    timer_deinit(TIMER0);

    //TIMER0CLK = SystemCoreClock / 1 = 200MHz, T = 5us
    /* TIMER0 configuration */
    timer_initpara.prescaler         = 0U;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 998U;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0U;
    timer_init(TIMER0, &timer_initpara);

    /* CH1~CH3 configuration in PWM mode */
    timer_ocintpara.outputstate = TIMER_CCX_ENABLE;
    timer_ocintpara.outputnstate = TIMER_CCXN_DISABLE;
    timer_ocintpara.ocpolarity = TIMER_OC_POLARITY_HIGH;
    timer_ocintpara.ocnpolarity = TIMER_OCN_POLARITY_HIGH;
    timer_ocintpara.ocidlestate  = TIMER_OC_IDLE_STATE_LOW;
    timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;

    timer_channel_output_config(TIMER0, TIMER_CH_0, &timer_ocintpara);
    timer_channel_output_config(TIMER0, TIMER_CH_1, &timer_ocintpara);
    timer_channel_output_config(TIMER0, TIMER_CH_2, &timer_ocintpara);
    timer_channel_output_config(TIMER0, TIMER_CH_3, &timer_ocintpara);
    
    /* CH0 configuration in PWM mode0,duty cycle 100% */
    //timer_channel_output_pulse_value_config(TIMER0, TIMER_CH_0, 999);
    timer_channel_output_mode_config(TIMER0, TIMER_CH_0, TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER0, TIMER_CH_0, TIMER_OC_SHADOW_DISABLE);
    
    /* CH1 configuration in PWM mode0,duty cycle 100% */
    //timer_channel_output_pulse_value_config(TIMER0, TIMER_CH_1, 999);
    timer_channel_output_mode_config(TIMER0, TIMER_CH_1, TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER0, TIMER_CH_1, TIMER_OC_SHADOW_DISABLE);

    /* CH2 configuration in PWM mode0,duty cycle 100% */
    //timer_channel_output_pulse_value_config(TIMER0, TIMER_CH_2, 999);
    timer_channel_output_mode_config(TIMER0, TIMER_CH_2, TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER0, TIMER_CH_2, TIMER_OC_SHADOW_DISABLE);

    /* CH3 configuration in PWM mode0,duty cycle 100% */
    //timer_channel_output_pulse_value_config(TIMER0, TIMER_CH_3, 999);
    timer_channel_output_mode_config(TIMER0, TIMER_CH_3, TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER0, TIMER_CH_3, TIMER_OC_SHADOW_DISABLE);

    /* TIMER0 primary output enable */
    timer_primary_output_config(TIMER0, ENABLE);    // only TIM0 and TIM7 

    /* auto-reload preload enable */
    timer_auto_reload_shadow_enable(TIMER0);
    /* enable timer0 */
    timer_enable(TIMER0);
}


/*****************************************************************************
* @brief  : configure timer1
* @param  : None
* @retval : None
* @date   : 2023-09-06
* @note   : 
*****************************************************************************/
void tim1_init(void)
{
    /* enable the port clock */
    rcu_periph_clock_enable(RCU_GPIOA);
    /* enable TIM clock */
    rcu_periph_clock_enable(RCU_TIMER1);

    /**TIM0 GPIO Configuration
    PA5    ------> TIMER1_CH0  MIMO DW
    PB9    ------> TIMER1_CH1  B40 UP 2
    PA2    ------> TIMER1_CH2  B8 UP 2
    PA3    ------> TIMER1_CH3  B3 UP 2
    */
    /* connect port to TIMER1_CH0 */
    gpio_mode_set(ATT_MIMO_DW_ATT_GPIO_Port, GPIO_MODE_AF, GPIO_PUPD_NONE, ATT_MIMO_DW_ATT_Pin);
    gpio_output_options_set(ATT_MIMO_DW_ATT_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, ATT_MIMO_DW_ATT_Pin);
    gpio_af_set(ATT_MIMO_DW_ATT_GPIO_Port, GPIO_AF_1, ATT_MIMO_DW_ATT_Pin);

    /* connect port to TIMER1_CH1 */
    gpio_mode_set(ATT_B40_UP_2_GPIO_Port, GPIO_MODE_AF, GPIO_PUPD_NONE, ATT_B40_UP_2_Pin);
    gpio_output_options_set(ATT_B40_UP_2_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, ATT_B40_UP_2_Pin);
    gpio_af_set(ATT_B40_UP_2_GPIO_Port, GPIO_AF_1, ATT_B40_UP_2_Pin);

    /* connect port to TIMER1_CH2 */
    gpio_mode_set(ATT_B8_UP_2_GPIO_Port, GPIO_MODE_AF, GPIO_PUPD_NONE, ATT_B8_UP_2_Pin);
    gpio_output_options_set(ATT_B8_UP_2_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, ATT_B8_UP_2_Pin);
    gpio_af_set(ATT_B8_UP_2_GPIO_Port, GPIO_AF_1, ATT_B8_UP_2_Pin);

    /* connect port to TIMER1_CH3 */
    gpio_mode_set(ATT_B3_UP_2_GPIO_Port, GPIO_MODE_AF, GPIO_PUPD_NONE, ATT_B3_UP_2_Pin);
    gpio_output_options_set(ATT_B3_UP_2_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, ATT_B3_UP_2_Pin);
    gpio_af_set(ATT_B3_UP_2_GPIO_Port, GPIO_AF_1, ATT_B3_UP_2_Pin);
    
    timer_oc_parameter_struct timer_ocintpara;
    timer_parameter_struct timer_initpara;

    rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);

    timer_deinit(TIMER1);

    //TIMER1CLK = SystemCoreClock / 1 = 200MHz, T = 5us
    /* TIMER1 configuration */
    timer_initpara.prescaler         = 0U;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 998U;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0U;
    timer_init(TIMER1, &timer_initpara);

    /* CH0~CH3 configuration in PWM mode */
    timer_ocintpara.outputstate = TIMER_CCX_ENABLE;
    timer_ocintpara.outputnstate = TIMER_CCXN_DISABLE;
    timer_ocintpara.ocpolarity = TIMER_OC_POLARITY_HIGH;
    timer_ocintpara.ocnpolarity = TIMER_OCN_POLARITY_HIGH;
    timer_ocintpara.ocidlestate  = TIMER_OC_IDLE_STATE_LOW;
    timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;

    timer_channel_output_config(TIMER1, TIMER_CH_0, &timer_ocintpara);
    timer_channel_output_config(TIMER1, TIMER_CH_1, &timer_ocintpara);
    timer_channel_output_config(TIMER1, TIMER_CH_2, &timer_ocintpara);
    timer_channel_output_config(TIMER1, TIMER_CH_3, &timer_ocintpara);
    
    /* CH0 configuration in PWM mode0,duty cycle 100% */
    //timer_channel_output_pulse_value_config(TIMER1, TIMER_CH_0, 999);
    timer_channel_output_mode_config(TIMER1, TIMER_CH_0, TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER1, TIMER_CH_0, TIMER_OC_SHADOW_DISABLE);

    /* CH1 configuration in PWM mode0,duty cycle 100% */
    //timer_channel_output_pulse_value_config(TIMER1, TIMER_CH_1, 999);
    timer_channel_output_mode_config(TIMER1, TIMER_CH_1, TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER1, TIMER_CH_1, TIMER_OC_SHADOW_DISABLE);
    
    /* CH2 configuration in PWM mode0,duty cycle 100% */
    //timer_channel_output_pulse_value_config(TIMER1, TIMER_CH_2, 999);
    timer_channel_output_mode_config(TIMER1, TIMER_CH_2, TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER1, TIMER_CH_2, TIMER_OC_SHADOW_DISABLE);

    /* CH3 configuration in PWM mode0,duty cycle 100% */
    //timer_channel_output_pulse_value_config(TIMER1, TIMER_CH_3, 999);
    timer_channel_output_mode_config(TIMER1, TIMER_CH_3, TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER1, TIMER_CH_3, TIMER_OC_SHADOW_DISABLE);
    
    /* TIMER0 primary output enable */
    //timer_primary_output_config(TIMER0, ENABLE);    // only TIM0 and TIM7 

    /* auto-reload preload enable */
    timer_auto_reload_shadow_enable(TIMER1);
    /* enable timer1 */
    timer_enable(TIMER1);
}


/*****************************************************************************
* @brief  : configure timer2
* @param  : None
* @retval : None
* @date   : 2023-09-06
* @note   : 
*****************************************************************************/
void tim2_init(void)
{
    /* enable the port clock */
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOC);
    /* enable TIM clock */
    rcu_periph_clock_enable(RCU_TIMER2);
    
    /**TIM0 GPIO Configuration
    PA6    ------> TIMER2_CH0  NR DW
    PA7    ------> TIMER2_CH1  4G DW
    PC8    ------> TIMER2_CH2  MIMO UP 2
    */
    
    /* connect port to TIMER2_CH0 */
    gpio_mode_set(ATT_NR_DW_ATT_GPIO_Port, GPIO_MODE_AF, GPIO_PUPD_NONE, ATT_NR_DW_ATT_Pin);
    gpio_output_options_set(ATT_NR_DW_ATT_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, ATT_NR_DW_ATT_Pin);
    gpio_af_set(ATT_NR_DW_ATT_GPIO_Port, GPIO_AF_2, ATT_NR_DW_ATT_Pin);

    /* connect port to TIMER2_CH1 */
    gpio_mode_set(ATT_4G_DW_ATT_GPIO_Port, GPIO_MODE_AF, GPIO_PUPD_NONE, ATT_4G_DW_ATT_Pin);
    gpio_output_options_set(ATT_4G_DW_ATT_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, ATT_4G_DW_ATT_Pin);
    gpio_af_set(ATT_4G_DW_ATT_GPIO_Port, GPIO_AF_2, ATT_4G_DW_ATT_Pin);
    
    /* connect port to TIMER2_CH2 */
    gpio_mode_set(ATT_MIMO_UP_2_GPIO_Port, GPIO_MODE_AF, GPIO_PUPD_NONE, ATT_MIMO_UP_2_Pin);
    gpio_output_options_set(ATT_MIMO_UP_2_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, ATT_MIMO_UP_2_Pin);
    gpio_af_set(ATT_MIMO_UP_2_GPIO_Port, GPIO_AF_2, ATT_MIMO_UP_2_Pin);
    
    timer_oc_parameter_struct timer_ocintpara;
    timer_parameter_struct timer_initpara;

    rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);

    timer_deinit(TIMER2);

    //TIMER2CLK = SystemCoreClock / 1 = 200MHz, T = 5us
    /* TIMER2 configuration */
    timer_initpara.prescaler         = 0U;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 998U;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0U;
    timer_init(TIMER2, &timer_initpara);

    /* CH0~CH3 configuration in PWM mode */
    timer_ocintpara.outputstate = TIMER_CCX_ENABLE;
    timer_ocintpara.outputnstate = TIMER_CCXN_DISABLE;
    timer_ocintpara.ocpolarity = TIMER_OC_POLARITY_HIGH;
    timer_ocintpara.ocnpolarity = TIMER_OCN_POLARITY_HIGH;
    timer_ocintpara.ocidlestate  = TIMER_OC_IDLE_STATE_LOW;
    timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;

    timer_channel_output_config(TIMER2, TIMER_CH_0, &timer_ocintpara);
    timer_channel_output_config(TIMER2, TIMER_CH_1, &timer_ocintpara);
    timer_channel_output_config(TIMER2, TIMER_CH_2, &timer_ocintpara);

    /* CH0 configuration in PWM mode0,duty cycle 100% */
    //timer_channel_output_pulse_value_config(TIMER2, TIMER_CH_0, 999);
    timer_channel_output_mode_config(TIMER2, TIMER_CH_0, TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER2, TIMER_CH_0, TIMER_OC_SHADOW_DISABLE);

    /* CH1 configuration in PWM mode0,duty cycle 100% */
    //timer_channel_output_pulse_value_config(TIMER2, TIMER_CH_1, 999);
    timer_channel_output_mode_config(TIMER2, TIMER_CH_1, TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER2, TIMER_CH_1, TIMER_OC_SHADOW_DISABLE);

    /* CH2 configuration in PWM mode0,duty cycle 100% */
    //timer_channel_output_pulse_value_config(TIMER2, TIMER_CH_2, 999);
    timer_channel_output_mode_config(TIMER2, TIMER_CH_2, TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER2, TIMER_CH_2, TIMER_OC_SHADOW_DISABLE);
    
    /* TIMER0 primary output enable */
    //timer_primary_output_config(TIMER0, ENABLE);    // only TIM0 and TIM7 

    /* auto-reload preload enable */
    timer_auto_reload_shadow_enable(TIMER2);
    /* enable timer2 */
    timer_enable(TIMER2);
}


/*****************************************************************************
* @brief  : configure timer3
* @param  : None
* @retval : None
* @date   : 2023-09-06
* @note   : 
*****************************************************************************/
void tim3_init(void)
{
    /* enable the port clock */
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOD);
    /* enable TIM clock */
    rcu_periph_clock_enable(RCU_TIMER3);

    /**TIM0 GPIO Configuration
    PB8    ------> TIMER3_CH2  B40 UP 1
    PD15   ------> TIMER3_CH3  MIMO UP 1
    */

    /* connect port to TIMER3_CH2 */
    gpio_mode_set(ATT_B40_UP_1_GPIO_Port, GPIO_MODE_AF, GPIO_PUPD_NONE, ATT_B40_UP_1_Pin);
    gpio_output_options_set(ATT_B40_UP_1_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, ATT_B40_UP_1_Pin);
    gpio_af_set(ATT_B40_UP_1_GPIO_Port, GPIO_AF_2, ATT_B40_UP_1_Pin);

    /* connect port to TIMER3_CH3 */
    gpio_mode_set(ATT_MIMO_UP_1_GPIO_Port, GPIO_MODE_AF, GPIO_PUPD_NONE, ATT_MIMO_UP_1_Pin);
    gpio_output_options_set(ATT_MIMO_UP_1_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, ATT_MIMO_UP_1_Pin);
    gpio_af_set(ATT_MIMO_UP_1_GPIO_Port, GPIO_AF_2, ATT_MIMO_UP_1_Pin);
    
    timer_oc_parameter_struct timer_ocintpara;
    timer_parameter_struct timer_initpara;

    rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);

    timer_deinit(TIMER3);

    //TIMER3CLK = SystemCoreClock / 1 = 200MHz, T = 5us
    /* TIMER3 configuration */
    timer_initpara.prescaler         = 0U;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 998U;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0U;
    timer_init(TIMER3, &timer_initpara);

    /* CH0~CH3 configuration in PWM mode */
    timer_ocintpara.outputstate = TIMER_CCX_ENABLE;
    timer_ocintpara.outputnstate = TIMER_CCXN_DISABLE;
    timer_ocintpara.ocpolarity = TIMER_OC_POLARITY_HIGH;
    timer_ocintpara.ocnpolarity = TIMER_OCN_POLARITY_HIGH;
    timer_ocintpara.ocidlestate  = TIMER_OC_IDLE_STATE_LOW;
    timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;

    timer_channel_output_config(TIMER3, TIMER_CH_2, &timer_ocintpara);
    timer_channel_output_config(TIMER3, TIMER_CH_3, &timer_ocintpara);
    
    /* CH2 configuration in PWM mode0,duty cycle 100% */
    //timer_channel_output_pulse_value_config(TIMER3, TIMER_CH_2, 999);
    timer_channel_output_mode_config(TIMER3, TIMER_CH_2, TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER3, TIMER_CH_2, TIMER_OC_SHADOW_DISABLE);

    /* CH3 configuration in PWM mode0,duty cycle 100% */
    //timer_channel_output_pulse_value_config(TIMER3, TIMER_CH_3, 999);
    timer_channel_output_mode_config(TIMER3, TIMER_CH_3, TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER3, TIMER_CH_3, TIMER_OC_SHADOW_DISABLE);
    
    /* TIMER0 primary output enable */
    //timer_primary_output_config(TIMER0, ENABLE);    // only TIM0 and TIM7 

    /* auto-reload preload enable */
    timer_auto_reload_shadow_enable(TIMER3);
    /* enable timer3 */
    timer_enable(TIMER3);
}


/*****************************************************************************
* @brief  : configure timer4
* @param  : None
* @retval : None
* @date   : 2023-09-06
* @note   : 
*****************************************************************************/
void tim4_init(void)
{
    /* enable the port clock */
    rcu_periph_clock_enable(RCU_GPIOA);
    /* enable TIM clock */
    rcu_periph_clock_enable(RCU_TIMER4);

    /**TIM0 GPIO Configuration
    PA0    ------> TIMER4_CH0  NR UP 1
    PA1    ------> TIMER4_CH1  NR UP 2
    */

    /* connect port to TIMER4_CH0 */
    gpio_mode_set(ATT_NR_UP_1_GPIO_Port, GPIO_MODE_AF, GPIO_PUPD_NONE, ATT_NR_UP_1_Pin);
    gpio_output_options_set(ATT_NR_UP_1_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, ATT_NR_UP_1_Pin);
    gpio_af_set(ATT_NR_UP_1_GPIO_Port, GPIO_AF_2, ATT_NR_UP_1_Pin);

    /* connect port to TIMER4_CH1 */
    gpio_mode_set(ATT_NR_UP_2_GPIO_Port, GPIO_MODE_AF, GPIO_PUPD_NONE, ATT_NR_UP_2_Pin);
    gpio_output_options_set(ATT_NR_UP_2_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, ATT_NR_UP_2_Pin);
    gpio_af_set(ATT_NR_UP_2_GPIO_Port, GPIO_AF_2, ATT_NR_UP_2_Pin);
    
    timer_oc_parameter_struct timer_ocintpara;
    timer_parameter_struct timer_initpara;

    rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);

    timer_deinit(TIMER4);

    //TIMER4CLK = SystemCoreClock / 1 = 200MHz, T = 5us
    /* TIMER4 configuration */
    timer_initpara.prescaler         = 0U;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 998U;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0U;
    timer_init(TIMER4, &timer_initpara);

    /* CH0~CH3 configuration in PWM mode */
    timer_ocintpara.outputstate = TIMER_CCX_ENABLE;
    timer_ocintpara.outputnstate = TIMER_CCXN_DISABLE;
    timer_ocintpara.ocpolarity = TIMER_OC_POLARITY_HIGH;
    timer_ocintpara.ocnpolarity = TIMER_OCN_POLARITY_HIGH;
    timer_ocintpara.ocidlestate  = TIMER_OC_IDLE_STATE_LOW;
    timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;

    timer_channel_output_config(TIMER4, TIMER_CH_0, &timer_ocintpara);
    timer_channel_output_config(TIMER4, TIMER_CH_1, &timer_ocintpara);
    
    /* CH0 configuration in PWM mode0,duty cycle 100% */
    //timer_channel_output_pulse_value_config(TIMER4, TIMER_CH_0, 999);
    timer_channel_output_mode_config(TIMER4, TIMER_CH_0, TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER4, TIMER_CH_0, TIMER_OC_SHADOW_DISABLE);

    /* CH1 configuration in PWM mode0,duty cycle 100% */
    //timer_channel_output_pulse_value_config(TIMER4, TIMER_CH_1, 999);
    timer_channel_output_mode_config(TIMER4, TIMER_CH_1, TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER4, TIMER_CH_1, TIMER_OC_SHADOW_DISABLE);
    
    /* TIMER0 primary output enable */
    //timer_primary_output_config(TIMER0, ENABLE);    // only TIM0 and TIM7 

    /* auto-reload preload enable */
    timer_auto_reload_shadow_enable(TIMER4);
    /* enable timer4 */
    timer_enable(TIMER4);
}


/*****************************************************************************
* @brief  : 设置PWM占空比
* @param  : channel: TIMER channel[TIMER_CH_0 ~ TIMER_CH_3]
*      		value: 0~999
* @retval : 
* @date   : 2023-09-06
* @note   : 
*****************************************************************************/
void tim0_set(uint16_t channel, uint16_t value)
{
    if(value > 999)
        value = 999;
    
    timer_channel_output_pulse_value_config(TIMER0, channel, value);
}


/*****************************************************************************
* @brief  : 设置PWM占空比
* @param  : channel: TIMER channel[TIMER_CH_0 ~ TIMER_CH_3]
*      		value: 0~999
* @retval : 
* @date   : 2023-09-06
* @note   : 
*****************************************************************************/
void tim1_set(uint16_t channel, uint16_t value)
{
    if(value > 999)
        value = 999;
    
    timer_channel_output_pulse_value_config(TIMER1, channel, value);
}


/*****************************************************************************
* @brief  : 设置PWM占空比
* @param  : channel: TIMER channel[TIMER_CH_0 ~ TIMER_CH_3]
*      		value: 0~999
* @retval : 
* @date   : 2023-09-06
* @note   : 
*****************************************************************************/
void tim2_set(uint16_t channel, uint16_t value)
{
    if(value > 999)
        value = 999;
    
    timer_channel_output_pulse_value_config(TIMER2, channel, value);
}


/*****************************************************************************
* @brief  : 设置PWM占空比
* @param  : channel: TIMER channel[TIMER_CH_0 ~ TIMER_CH_3]
*      		value: 0~999
* @retval : 
* @date   : 2023-09-06
* @note   : 
*****************************************************************************/
void tim3_set(uint16_t channel, uint16_t value)
{
    if(value > 999)
        value = 999;
    
    timer_channel_output_pulse_value_config(TIMER3, channel, value);
}


/*****************************************************************************
* @brief  : 设置PWM占空比
* @param  : channel: TIMER channel[TIMER_CH_0 ~ TIMER_CH_3]
*      		value: 0~999
* @retval : 
* @date   : 2023-09-06
* @note   : 
*****************************************************************************/
void tim4_set(uint16_t channel, uint16_t value)
{
    if(value > 999)
        value = 999;
    
    timer_channel_output_pulse_value_config(TIMER4, channel, value);
}
