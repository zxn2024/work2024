/**
 * **********************************************************************************
 * @copyright Copyright (c) 2023 Shaanxi Tianji Communication Technologies Co., Ltd.
 *                         
 * @file        uart.c   
 * @author      WangJun
 * @version     None
 * @date        2023-09-06    
 * @brief       None   
 * 
 * **********************************************************************************
 */

#include "uart.h"


/*---------------------- 端口定义 ----------------------*/


/*---------------------- 宏 定 义 ----------------------*/


/*---------------------- 结 构 体 ----------------------*/


/*---------------------- 常    量 ----------------------*/


/*---------------------- 全局变量 ----------------------*/


/*---------------------- 局部变量 ----------------------*/


/*---------------------- 函    数 ----------------------*/
/*****************************************************************************
* @brief  : configure uart3
* @param  : None
* @retval : None
* @date   : 2023-09-06
* @note   : 
*****************************************************************************/
void uart3_init(void)
{
    /* enable the port clock */
    rcu_periph_clock_enable(RCU_GPIOC);
    /* enable UART3 clock */
    rcu_periph_clock_enable(RCU_UART3);
    /* enable DMA clock */
    rcu_periph_clock_enable(RCU_DMA0);

    /**UART3 GPIO Configuration
    PC10    ------> UART3_TX
    PC11    ------> UART3_RX
    */
    /* connect port to UART3_Tx */
    gpio_af_set(GPIOC, GPIO_AF_8, GPIO_PIN_10);
    /* connect port to UART3_Rx */
    gpio_af_set(GPIOC, GPIO_AF_8, GPIO_PIN_11);
    /* configure UART Tx as alternate function push-pull */
    gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_10);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);
    /* configure UART Rx as alternate function push-pull */
    gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_11);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_11);

    /* UART DMA enable*/
    usart_dma_receive_config(UART3, USART_DENR_DISABLE);
    //usart_dma_transmit_config(UART3, USART_DENT_ENABLE);

    dma_single_data_parameter_struct dma_init_struct;

    /* deinitialize DMA0 channel4(UART3 TX) */
    dma_single_data_para_struct_init(&dma_init_struct);
    dma_deinit(DMA0, DMA_CH4);
    dma_init_struct.direction = DMA_MEMORY_TO_PERIPH;
    dma_init_struct.memory0_addr = NULL;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.periph_memory_width = DMA_PERIPH_WIDTH_8BIT;
    dma_init_struct.number = 0;
    dma_init_struct.periph_addr = ((uint32_t)&USART_DATA(UART3));
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.priority = DMA_PRIORITY_HIGH;
    dma_single_data_mode_init(DMA0, DMA_CH4, &dma_init_struct);

    /* configure DMA mode */
    dma_circulation_disable(DMA0, DMA_CH4);
    dma_channel_subperipheral_select(DMA0, DMA_CH4, DMA_SUBPERI4);

    /* enable DMA0 channel4 transfer complete interrupt */
    //dma_interrupt_enable(DMA0, DMA_CH4, DMA_CHXCTL_FTFIE);
    /* disable DMA0 channel4 transfer complete interrupt */
    dma_interrupt_disable(DMA0, DMA_CH4, DMA_CHXCTL_FTFIE);

    /* enable DMA0 channel4 */
    //dma_channel_enable(DMA0, DMA_CH4);

    /* UART interrupt configuration */
    nvic_irq_enable(UART3_IRQn, 5, 0);

    /* enable UART IDLE interrupt */
    usart_interrupt_enable(UART3, USART_INT_IDLE);
    /* enable UART RBNE interrupt */
    usart_interrupt_enable(UART3, USART_INT_RBNE);
    /* disable UART TC interrupt */
    usart_interrupt_disable(UART3, USART_INT_TC);

    usart_baudrate_set(UART3, 115200U);
    usart_word_length_set(UART3, USART_WL_8BIT);
    usart_stop_bit_set(UART3, USART_STB_1BIT);
    usart_parity_config(UART3, USART_PM_NONE);
    usart_receive_config(UART3, USART_RECEIVE_ENABLE);
    usart_transmit_config(UART3, USART_TRANSMIT_ENABLE);
    usart_enable(UART3);
}


/*****************************************************************************
* @brief  : configure uart4
* @param  : None
* @retval : None
* @date   : 2023-09-06
* @note   : 
*****************************************************************************/
void uart4_init(void)
{
    /* enable the port clock */
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_GPIOD);
    /* enable UART4 clock */
    rcu_periph_clock_enable(RCU_UART4);
    /* enable DMA clock */
    rcu_periph_clock_enable(RCU_DMA0);

    /**UART4 GPIO Configuration
    PC12    ------> UART4_TX
    PD2     ------> UART4_RX
    */
    /* connect port to UART4_Tx */
    gpio_af_set(GPIOC, GPIO_AF_7, GPIO_PIN_12);
    /* connect port to UART4_Rx */
    gpio_af_set(GPIOD, GPIO_AF_7, GPIO_PIN_2);
    /* configure UART Tx as alternate function push-pull */
    gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_12);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_12);
    /* configure UART Rx as alternate function push-pull */
    gpio_mode_set(GPIOD, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_2);
    gpio_output_options_set(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_2);

    /* UART DMA enable*/
    usart_dma_receive_config(UART4, USART_DENR_DISABLE);
    //usart_dma_transmit_config(UART4, USART_DENT_ENABLE);

    dma_single_data_parameter_struct dma_init_struct;

    /* deinitialize DMA0 channel7(UART4 TX) */
    dma_single_data_para_struct_init(&dma_init_struct);
    dma_deinit(DMA0, DMA_CH7);
    dma_init_struct.direction = DMA_MEMORY_TO_PERIPH;
    dma_init_struct.memory0_addr = NULL;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.periph_memory_width = DMA_PERIPH_WIDTH_8BIT;
    dma_init_struct.number = 0;
    dma_init_struct.periph_addr = ((uint32_t)&USART_DATA(UART4));
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.priority = DMA_PRIORITY_HIGH;
    dma_single_data_mode_init(DMA0, DMA_CH7, &dma_init_struct);

    /* configure DMA mode */
    dma_circulation_disable(DMA0, DMA_CH7);
    dma_channel_subperipheral_select(DMA0, DMA_CH7, DMA_SUBPERI4);

    /* enable DMA0 channel7 transfer complete interrupt */
    //dma_interrupt_enable(DMA0, DMA_CH7, DMA_CHXCTL_FTFIE);
    /* disable DMA0 channel7 transfer complete interrupt */
    dma_interrupt_disable(DMA0, DMA_CH7, DMA_CHXCTL_FTFIE);

    /* enable DMA0 channel7 */
    //dma_channel_enable(DMA0, DMA_CH7);

    /* UART interrupt configuration */
    nvic_irq_enable(UART4_IRQn, 5, 0);

    /* enable UART IDLE interrupt */
    usart_interrupt_enable(UART4, USART_INT_IDLE);
    /* enable UART RBNE interrupt */
    usart_interrupt_enable(UART4, USART_INT_RBNE);
    /* disable UART TC interrupt */
    usart_interrupt_disable(UART4, USART_INT_TC);

    usart_baudrate_set(UART4, 115200U);
    usart_word_length_set(UART4, USART_WL_8BIT);
    usart_stop_bit_set(UART4, USART_STB_1BIT);
    usart_parity_config(UART4, USART_PM_NONE);
    usart_receive_config(UART4, USART_RECEIVE_ENABLE);
    usart_transmit_config(UART4, USART_TRANSMIT_ENABLE);
    usart_enable(UART4);
}



/*****************************************************************************
* @brief  : configure uart3 dma send data
* @param  : buffer: send data buf
*			len: send data len
* @retval : None
* @date   : 2023-09-06
* @note   : 
*****************************************************************************/
void uart3_dma_send(uint8_t * buffer, uint16_t len)
{
    dma_channel_disable(DMA0, DMA_CH4);

    dma_flag_clear(DMA0, DMA_CH4, DMA_FLAG_SDE);
    dma_flag_clear(DMA0, DMA_CH4, DMA_FLAG_TAE);
    dma_flag_clear(DMA0, DMA_CH4, DMA_FLAG_FTF);    // 必须清中断标志，否则发送数据会出错
    dma_memory_address_config(DMA0, DMA_CH4, DMA_MEMORY_0, (uint32_t)buffer);   // 设置要发送数据的内存地址
    dma_transfer_number_config(DMA0, DMA_CH4, len);

    dma_channel_enable(DMA0, DMA_CH4);
    usart_dma_transmit_config(UART3, USART_DENT_ENABLE);   // 使能串口DMA发送

    while(RESET == dma_flag_get(DMA0, DMA_CH4, DMA_FLAG_FTF));  // 等待发送完成
}


/*****************************************************************************
* @brief  : configure uart4 dma send data
* @param  : buffer: send data buf
*			len: send data len
* @retval : None
* @date   : 2023-09-06
* @note   : 
*****************************************************************************/
void uart4_dma_send(uint8_t * buffer, uint16_t len)
{
    dma_channel_disable(DMA0, DMA_CH7);

    dma_flag_clear(DMA0, DMA_CH7, DMA_FLAG_SDE);
    dma_flag_clear(DMA0, DMA_CH7, DMA_FLAG_TAE);
    dma_flag_clear(DMA0, DMA_CH7, DMA_FLAG_FTF);    // 必须清中断标志，否则发送数据会出错
    dma_memory_address_config(DMA0, DMA_CH7, DMA_MEMORY_0, (uint32_t)buffer);   // 设置要发送数据的内存地址
    dma_transfer_number_config(DMA0, DMA_CH7, len);

    dma_channel_enable(DMA0, DMA_CH7);
    usart_dma_transmit_config(UART4, USART_DENT_ENABLE);   // 使能串口DMA发送

    while(RESET == dma_flag_get(DMA0, DMA_CH7, DMA_FLAG_FTF));  // 等待发送完成
}
