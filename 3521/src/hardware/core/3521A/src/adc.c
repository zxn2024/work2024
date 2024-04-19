/**
 * **********************************************************************************
 * @copyright Copyright (c) 2023 Shaanxi Tianji Communication Technologies Co., Ltd.
 *
 * @file        adc.c
 * @author      WangJun
 * @version     None
 * @date        2023-09-07
 * @brief       None
 *
 * **********************************************************************************
 */

#include "adc.h"


/*---------------------- 端口定义 ----------------------*/


/*---------------------- 宏 定 义 ----------------------*/


/*---------------------- 结 构 体 ----------------------*/


/*---------------------- 常    量 ----------------------*/


/*---------------------- 全局变量 ----------------------*/


/*---------------------- 局部变量 ----------------------*/


/*---------------------- 函    数 ----------------------*/


/*****************************************************************************
* @brief  : configure adc0
* @param  : None
* @retval : None
* @date   : 2023-09-05
* @note   :
*****************************************************************************/
void adc0_init(uint16_t * pbuf, uint16_t len)
{
    /* enable the port clock */
    //rcu_periph_clock_enable(RCU_GPIOA);
    /* enable ADC0 clock */
    rcu_periph_clock_enable(RCU_ADC0);
    /* enable DMA clock */
    rcu_periph_clock_enable(RCU_DMA1);
    /* config ADC clock */
    adc_clock_config(ADC_ADCCK_PCLK2_DIV4); // fclk = APB2 / 4  ;25MHz

    /*******************DMA*******************/
    /* ADC_DMA_channel configuration */
    dma_single_data_parameter_struct dma_single_data_parameter;

    /* ADC DMA_channel configuration */
    dma_deinit(DMA1, DMA_CH0);

    /* initialize DMA single data mode */
    dma_single_data_parameter.periph_addr = (uint32_t)(&ADC_RDATA(ADC0));
    dma_single_data_parameter.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_single_data_parameter.memory0_addr = (uint32_t)(pbuf);
    dma_single_data_parameter.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_single_data_parameter.periph_memory_width = DMA_PERIPH_WIDTH_16BIT;
    dma_single_data_parameter.circular_mode = DMA_CIRCULAR_MODE_ENABLE;
    dma_single_data_parameter.direction = DMA_PERIPH_TO_MEMORY;
    dma_single_data_parameter.number = len;
    dma_single_data_parameter.priority = DMA_PRIORITY_LOW;
    dma_single_data_mode_init(DMA1, DMA_CH0, &dma_single_data_parameter);

    dma_channel_subperipheral_select(DMA1, DMA_CH0, DMA_SUBPERI0);

    /* dma ch0 interrupt configuration */
    nvic_irq_enable(DMA1_Channel0_IRQn, 6, 0);  // 使能DMA中断

    /* enable DMA1 CH0  HTFIE interrupt */
    //dma_interrupt_enable(DMA1, DMA_CH0, DMA_CHXCTL_HTFIE);  // 半传输完成中断

    /* enable DMA1 CH0  FTFIE interrupt */
    //dma_interrupt_enable(DMA1, DMA_CH0, DMA_CHXCTL_FTFIE);  // 全部传输完成中断

    /* clear DMA1 CH0  HTFIE interrupt */
    //dma_interrupt_flag_clear(DMA1, DMA_CH0, DMA_INT_FLAG_HTF);

    /* clear DMA1 CH0  FTFIE interrupt */
    //dma_interrupt_flag_clear(DMA1, DMA_CH0, DMA_INT_FLAG_FTF);

    /* enable DMA circulation mode */
    dma_circulation_enable(DMA1, DMA_CH0);

    /* enable DMA channel */
    dma_channel_enable(DMA1, DMA_CH0);

    /* configure the ADC sync mode */
    adc_sync_mode_config(ADC_SYNC_MODE_INDEPENDENT);

    /* ADC contineous function enable */
    adc_special_function_config(ADC0, ADC_CONTINUOUS_MODE, ENABLE);

    /* ADC SCAN function disable */
    adc_special_function_config(ADC0, ADC_SCAN_MODE, ENABLE);

    /* ADC data alignment config */
    adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);

    /* ADC channel length config */
    adc_channel_length_config(ADC0, ADC_REGULAR_CHANNEL, 1);   // length = 1

    /* ADC regular channel config */
    adc_regular_channel_config(ADC0, 0U, ADC_CHANNEL_16, ADC_SAMPLETIME_480);    // temperature   19.68us

    /* ADC temperature and Vref enable */
    adc_channel_16_to_18(ADC_TEMP_VREF_CHANNEL_SWITCH, ENABLE);

    /* ADC DMA function enable */
    adc_dma_request_after_last_enable(ADC0);
    adc_dma_mode_enable(ADC0);

    /* enable ADC interface */
    adc_enable(ADC0);
    /* wait for ADC stability */
    delay_1ms(1U);
    /* ADC calibration and reset calibration */
    adc_calibration_enable(ADC0);   // 校准复位

    /* enable ADC software trigger */
    adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL);
}


/*****************************************************************************
* @brief  : configure adc2
* @param  : None
* @retval : None
* @date   : 2023-09-07
* @note   :
*****************************************************************************/
void adc2_init(uint16_t * pbuf, uint16_t len)
{
    /* enable the port clock */
    rcu_periph_clock_enable(RCU_GPIOF);
    /* enable ADC2 clock */
    rcu_periph_clock_enable(RCU_ADC2);
    /* enable DMA clock */
    rcu_periph_clock_enable(RCU_DMA1);
    /* config ADC clock */
    adc_clock_config(ADC_ADCCK_PCLK2_DIV4); // fclk = APB2 / 4  ;25MHz

    /* PF5 */
    gpio_mode_set(AD_4G_DW_TRX_POW_GPIO_Port, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, AD_4G_DW_TRX_POW_Pin);
    /* PF4 */
    gpio_mode_set(AD_NR_DW_TRX_POW_GPIO_Port, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, AD_NR_DW_TRX_POW_Pin);
    /* PC3 */
    gpio_mode_set(AD_MIMO_DW_TRX_POW_GPIO_Port, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, AD_MIMO_DW_TRX_POW_Pin);
    /* PF6 */
    gpio_mode_set(AD_4G_UP_GPIO_Port, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, AD_4G_UP_Pin);
    /* PF10 */
    gpio_mode_set(AD_4G_DW_GPIO_Port, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, AD_4G_DW_Pin);
    /* PF8 */
    gpio_mode_set(AD_NR_UP_GPIO_Port, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, AD_NR_UP_Pin);
    /* PF7 */
    gpio_mode_set(AD_NR_DW_GPIO_Port, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, AD_NR_DW_Pin);
    /* PF3 */
    gpio_mode_set(AD_MIMO_UP_GPIO_Port, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, AD_MIMO_UP_Pin);
    /* PF9 */
    gpio_mode_set(AD_MIMO_DW_GPIO_Port, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, AD_MIMO_DW_Pin);
    
    /*******************DMA*******************/
    /* ADC_DMA_channel configuration */
    dma_single_data_parameter_struct dma_single_data_parameter;

    /* ADC DMA_channel configuration */
    dma_deinit(DMA1, DMA_CH1);

    /* initialize DMA single data mode */
    dma_single_data_parameter.periph_addr = (uint32_t)(&ADC_RDATA(ADC2));
    dma_single_data_parameter.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_single_data_parameter.memory0_addr = (uint32_t)(pbuf);
    dma_single_data_parameter.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_single_data_parameter.periph_memory_width = DMA_PERIPH_WIDTH_16BIT;
    dma_single_data_parameter.circular_mode = DMA_CIRCULAR_MODE_ENABLE;
    dma_single_data_parameter.direction = DMA_PERIPH_TO_MEMORY;
    dma_single_data_parameter.number = len;
    dma_single_data_parameter.priority = DMA_PRIORITY_LOW;
    dma_single_data_mode_init(DMA1, DMA_CH1, &dma_single_data_parameter);

    dma_channel_subperipheral_select(DMA1, DMA_CH1, DMA_SUBPERI2);

    /* dma ch1 interrupt configuration */
    nvic_irq_enable(DMA1_Channel1_IRQn, 6, 0);  // 使能DMA中断

    /* enable DMA1 CH1  HTFIE interrupt */
    dma_interrupt_enable(DMA1, DMA_CH1, DMA_CHXCTL_HTFIE);  // 半传输完成中断

    /* enable DMA1 CH1  FTFIE interrupt */
    dma_interrupt_enable(DMA1, DMA_CH1, DMA_CHXCTL_FTFIE);  // 全部传输完成中断

    /* clear DMA1 CH1  HTFIE interrupt */
    dma_interrupt_flag_clear(DMA1, DMA_CH1, DMA_INT_FLAG_HTF);

    /* clear DMA1 CH01  FTFIE interrupt */
    dma_interrupt_flag_clear(DMA1, DMA_CH1, DMA_INT_FLAG_FTF);

    /* enable DMA circulation mode */
    dma_circulation_enable(DMA1, DMA_CH1);

    /* enable DMA channel */
    dma_channel_enable(DMA1, DMA_CH1);

    /* configure the ADC sync mode */
    adc_sync_mode_config(ADC_SYNC_MODE_INDEPENDENT);

    /* ADC contineous function enable */
    adc_special_function_config(ADC2, ADC_CONTINUOUS_MODE, ENABLE);

    /* ADC SCAN function disable */
    adc_special_function_config(ADC2, ADC_SCAN_MODE, ENABLE);

    /* ADC data alignment config */
    adc_data_alignment_config(ADC2, ADC_DATAALIGN_RIGHT);

    /* ADC channel length config */
    adc_channel_length_config(ADC2, ADC_REGULAR_CHANNEL, 9);   // length = 9

    /* ADC regular channel config */
    adc_regular_channel_config(ADC2, 0U, ADC_CHANNEL_15, ADC_SAMPLETIME_28);    // 4g dw trx pwr    1.6us
    adc_regular_channel_config(ADC2, 1U, ADC_CHANNEL_14, ADC_SAMPLETIME_28);    // nr dw trx pwr
    adc_regular_channel_config(ADC2, 2U, ADC_CHANNEL_13, ADC_SAMPLETIME_28);    // mimo dw trx pwr
    adc_regular_channel_config(ADC2, 3U, ADC_CHANNEL_4, ADC_SAMPLETIME_28);     // 4g up
    adc_regular_channel_config(ADC2, 4U, ADC_CHANNEL_8, ADC_SAMPLETIME_28);     // 4g dw 
    adc_regular_channel_config(ADC2, 5U, ADC_CHANNEL_6, ADC_SAMPLETIME_28);     // nr up
    adc_regular_channel_config(ADC2, 6U, ADC_CHANNEL_5, ADC_SAMPLETIME_28);     // nr dw
    adc_regular_channel_config(ADC2, 7U, ADC_CHANNEL_9, ADC_SAMPLETIME_28);     // mimo up
    adc_regular_channel_config(ADC2, 8U, ADC_CHANNEL_7, ADC_SAMPLETIME_28);     // mimo dw
            
    /* ADC DMA function enable */
    adc_dma_request_after_last_enable(ADC2);
    adc_dma_mode_enable(ADC2);

    /* enable ADC interface */
    adc_enable(ADC2);
    /* wait for ADC stability */
    delay_1ms(1U);
    /* ADC calibration and reset calibration */
    adc_calibration_enable(ADC2);   // 校准复位

    /* enable ADC software trigger */
    adc_software_trigger_enable(ADC2, ADC_REGULAR_CHANNEL);
}
