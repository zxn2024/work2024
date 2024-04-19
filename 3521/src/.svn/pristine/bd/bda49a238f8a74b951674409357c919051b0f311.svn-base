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

#include "usart.h"


/*---------------------- 端口定义 ----------------------*/


/*---------------------- 宏 定 义 ----------------------*/


/*---------------------- 结 构 体 ----------------------*/


/*---------------------- 常    量 ----------------------*/


/*---------------------- 全局变量 ----------------------*/


/*---------------------- 局部变量 ----------------------*/


/*---------------------- 函    数 ----------------------*/
/*****************************************************************************
* @brief  : configure usart0
* @param  : None
* @retval : None
* @date   : 2023-09-06
* @note   : 
*****************************************************************************/
void usart0_init(void)
{
    /* enable the port clock */
    rcu_periph_clock_enable(RCU_GPIOA);
    /* enable USART0 clock */
    rcu_periph_clock_enable(RCU_USART0);
    /* enable DMA clock */
    rcu_periph_clock_enable(RCU_DMA1);

    /**USART0 GPIO Configuration
    PA9     ------> USART0_TX
    PA10    ------> USART0_RX
    */
    
    /* configure USART Tx as alternate function push-pull */
    gpio_mode_set(MODEM_RX_TO_MCU_TX0_GPIO_Port, GPIO_MODE_AF, GPIO_PUPD_PULLUP, MODEM_RX_TO_MCU_TX0_Pin);
    gpio_output_options_set(MODEM_RX_TO_MCU_TX0_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, MODEM_RX_TO_MCU_TX0_Pin);
    /* connect port to USART0_Tx */
    gpio_af_set(MODEM_RX_TO_MCU_TX0_GPIO_Port, GPIO_AF_7, MODEM_RX_TO_MCU_TX0_Pin);

    /* configure USART Rx as alternate function push-pull */
    gpio_mode_set(MODEM_TX_TO_MCU_RX0_GPIO_Port, GPIO_MODE_AF, GPIO_PUPD_PULLUP, MODEM_TX_TO_MCU_RX0_Pin);
    //gpio_output_options_set(MODEM_TX_TO_MCU_RX0_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, MODEM_TX_TO_MCU_RX0_Pin);
    /* connect port to USART0_Rx */
    gpio_af_set(MODEM_TX_TO_MCU_RX0_GPIO_Port, GPIO_AF_7, MODEM_TX_TO_MCU_RX0_Pin);

    /* USART DMA enable*/
    usart_dma_receive_config(USART0, USART_DENR_DISABLE);
    //usart_dma_transmit_config(USART0, USART_DENT_ENABLE);

    dma_single_data_parameter_struct dma_init_struct;

    /* deinitialize DMA1 channel7(USART0 TX) */
    dma_single_data_para_struct_init(&dma_init_struct);
    dma_deinit(DMA1, DMA_CH7);
    dma_init_struct.direction = DMA_MEMORY_TO_PERIPH;
    dma_init_struct.memory0_addr = NULL;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.periph_memory_width = DMA_PERIPH_WIDTH_8BIT;
    dma_init_struct.number = 0;
    dma_init_struct.periph_addr = ((uint32_t)&USART_DATA(USART0));
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.priority = DMA_PRIORITY_HIGH;
    dma_single_data_mode_init(DMA1, DMA_CH7, &dma_init_struct);

    /* configure DMA mode */
    dma_circulation_disable(DMA1, DMA_CH7);
    dma_channel_subperipheral_select(DMA1, DMA_CH7, DMA_SUBPERI4);

    /* enable DMA1 channel7 transfer complete interrupt */
    //dma_interrupt_enable(DMA1, DMA_CH7, DMA_CHXCTL_FTFIE);
    /* disable DMA1 channel7 transfer complete interrupt */
    dma_interrupt_disable(DMA1, DMA_CH7, DMA_CHXCTL_FTFIE);

    /* enable DMA1 channel7 */
    //dma_channel_enable(DMA1, DMA_CH7);

    /* USART interrupt configuration */
    nvic_irq_enable(USART0_IRQn, 5, 0);

    /* enable USART IDLE interrupt */
    usart_interrupt_enable(USART0, USART_INT_IDLE);
    /* enable USART RBNE interrupt */
    usart_interrupt_enable(USART0, USART_INT_RBNE);
    /* disable USART TC interrupt */
    usart_interrupt_disable(USART0, USART_INT_TC);

    usart_baudrate_set(USART0, 921600U);
    usart_word_length_set(USART0, USART_WL_8BIT);
    usart_stop_bit_set(USART0, USART_STB_1BIT);
    usart_parity_config(USART0, USART_PM_NONE);
    usart_hardware_flow_rts_config(USART0, USART_RTS_DISABLE);
    usart_hardware_flow_cts_config(USART0, USART_CTS_DISABLE);
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
    usart_enable(USART0);
}


/*****************************************************************************
* @brief  : configure usart1
* @param  : None
* @retval : None
* @date   : 2023-09-06
* @note   : 
*****************************************************************************/
void usart1_init(void)
{
//    /* enable the port clock */
//    rcu_periph_clock_enable(RCU_GPIOD);
//    /* enable USART1 clock */
//    rcu_periph_clock_enable(RCU_USART1);
//    /* enable DMA clock */
//    rcu_periph_clock_enable(RCU_DMA0);

//    /**USART1 GPIO Configuration
//    PD5     ------> USART1_TX
//    PD6     ------> USART1_RX
//    */
//    
//    /* configure USART Tx as alternate function push-pull */
//    //gpio_mode_set(BLE2_RX_TO_MCU_TX1_GPIO_Port, GPIO_MODE_AF, GPIO_PUPD_PULLUP, BLE2_RX_TO_MCU_TX1_Pin);
//    //gpio_output_options_set(BLE2_RX_TO_MCU_TX1_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, BLE2_RX_TO_MCU_TX1_Pin);
//    /* connect port to USART1_Tx */
//    //gpio_af_set(BLE2_RX_TO_MCU_TX1_GPIO_Port, GPIO_AF_7, BLE2_RX_TO_MCU_TX1_Pin);

//    /* configure USART Rx as alternate function push-pull */
//    //gpio_mode_set(BLE2_TX_TO_MCU_RX1_GPIO_Port, GPIO_MODE_AF, GPIO_PUPD_PULLUP, BLE2_TX_TO_MCU_RX1_Pin);
//    //gpio_output_options_set(BLE2_TX_TO_MCU_RX1_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, BLE2_TX_TO_MCU_RX1_Pin);
//    /* connect port to USART1_Rx */
//    //gpio_af_set(BLE2_TX_TO_MCU_RX1_GPIO_Port, GPIO_AF_7, BLE2_TX_TO_MCU_RX1_Pin);

//    /* USART DMA enable*/
//    usart_dma_receive_config(USART1, USART_DENR_DISABLE);
//    //usart_dma_transmit_config(USART1, USART_DENT_ENABLE);

//    dma_single_data_parameter_struct dma_init_struct;

//    /* deinitialize DMA0 channel6(USART1 TX) */
//    dma_single_data_para_struct_init(&dma_init_struct);
//    dma_deinit(DMA0, DMA_CH6);
//    dma_init_struct.direction = DMA_MEMORY_TO_PERIPH;
//    dma_init_struct.memory0_addr = NULL;
//    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
//    dma_init_struct.periph_memory_width = DMA_PERIPH_WIDTH_8BIT;
//    dma_init_struct.number = 0;
//    dma_init_struct.periph_addr = ((uint32_t)&USART_DATA(USART1));
//    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
//    dma_init_struct.priority = DMA_PRIORITY_HIGH;
//    dma_single_data_mode_init(DMA0, DMA_CH6, &dma_init_struct);

//    /* configure DMA mode */
//    dma_circulation_disable(DMA0, DMA_CH6);
//    dma_channel_subperipheral_select(DMA0, DMA_CH6, DMA_SUBPERI4);

//    /* enable DMA0 channel6 transfer complete interrupt */
//    //dma_interrupt_enable(DMA0, DMA_CH6, DMA_CHXCTL_FTFIE);
//    /* disable DMA0 channel6 transfer complete interrupt */
//    dma_interrupt_disable(DMA0, DMA_CH6, DMA_CHXCTL_FTFIE);

//    /* enable DMA0 channel6 */
//    //dma_channel_enable(DMA0, DMA_CH6);

//    /* USART interrupt configuration */
//    nvic_irq_enable(USART1_IRQn, 5, 0);

//    /* enable USART IDLE interrupt */
//    usart_interrupt_enable(USART1, USART_INT_IDLE);
//    /* enable USART RBNE interrupt */
//    usart_interrupt_enable(USART1, USART_INT_RBNE);
//    /* disable USART TC interrupt */
//    usart_interrupt_disable(USART1, USART_INT_TC);

//    usart_baudrate_set(USART1, 115200U);
//    usart_word_length_set(USART1, USART_WL_8BIT);
//    usart_stop_bit_set(USART1, USART_STB_1BIT);
//    usart_parity_config(USART1, USART_PM_NONE);
//    usart_hardware_flow_rts_config(USART1, USART_RTS_DISABLE);
//    usart_hardware_flow_cts_config(USART1, USART_CTS_DISABLE);
//    usart_receive_config(USART1, USART_RECEIVE_ENABLE);
//    usart_transmit_config(USART1, USART_TRANSMIT_ENABLE);
//    usart_enable(USART1);
}


/*****************************************************************************
* @brief  : configure usart2
* @param  : None
* @retval : None
* @date   : 2023-09-06
* @note   : 
*****************************************************************************/
void usart2_init(void)
{
    /* enable the port clock */
    rcu_periph_clock_enable(RCU_GPIOB);
    /* enable USART0 clock */
    rcu_periph_clock_enable(RCU_USART2);
    /* enable DMA clock */
    rcu_periph_clock_enable(RCU_DMA0);

    /**USART2 GPIO Configuration
    PB10     ------> USART2_TX
    PB11     ------> USART2_RX
    */

    /* configure USART Tx as alternate function push-pull */
    gpio_mode_set(BLE_RX_TO_MCU_TX2_GPIO_Port, GPIO_MODE_AF, GPIO_PUPD_PULLUP, BLE_RX_TO_MCU_TX2_Pin);
    gpio_output_options_set(BLE_RX_TO_MCU_TX2_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, BLE_RX_TO_MCU_TX2_Pin);
    /* connect port to USART2_Tx */
    gpio_af_set(BLE_RX_TO_MCU_TX2_GPIO_Port, GPIO_AF_7, BLE_RX_TO_MCU_TX2_Pin);
    
    /* configure USART Rx as alternate function push-pull */
    gpio_mode_set(BLE_TX_TO_MCU_RX2_GPIO_Port, GPIO_MODE_AF, GPIO_PUPD_PULLUP, BLE_TX_TO_MCU_RX2_Pin);
    //gpio_output_options_set(BLE_TX_TO_MCU_RX2_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, BLE_TX_TO_MCU_RX2_Pin);
    /* connect port to USART2_Rx */
    gpio_af_set(BLE_TX_TO_MCU_RX2_GPIO_Port, GPIO_AF_7, BLE_TX_TO_MCU_RX2_Pin);


    /* USART DMA enable*/
    usart_dma_receive_config(USART2, USART_DENR_DISABLE);
    //usart_dma_transmit_config(USART2, USART_DENT_ENABLE);

    dma_single_data_parameter_struct dma_init_struct;

    /* deinitialize DMA0 channel3(USART2 TX) */
    dma_single_data_para_struct_init(&dma_init_struct);
    dma_deinit(DMA0, DMA_CH3);
    dma_init_struct.direction = DMA_MEMORY_TO_PERIPH;
    dma_init_struct.memory0_addr = NULL;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.periph_memory_width = DMA_PERIPH_WIDTH_8BIT;
    dma_init_struct.number = 0;
    dma_init_struct.periph_addr = ((uint32_t)&USART_DATA(USART2));
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.priority = DMA_PRIORITY_HIGH;
    dma_single_data_mode_init(DMA0, DMA_CH3, &dma_init_struct);

    /* configure DMA mode */
    dma_circulation_disable(DMA0, DMA_CH3);
    dma_channel_subperipheral_select(DMA0, DMA_CH3, DMA_SUBPERI4);

    /* enable DMA0 channel3 transfer complete interrupt */
    //dma_interrupt_enable(DMA0, DMA_CH3, DMA_CHXCTL_FTFIE);
    /* disable DMA0 channel3 transfer complete interrupt */
    dma_interrupt_disable(DMA0, DMA_CH3, DMA_CHXCTL_FTFIE);

    /* enable DMA0 channel3 */
    //dma_channel_enable(DMA0, DMA_CH3);

    /* USART interrupt configuration */
    nvic_irq_enable(USART2_IRQn, 5, 0);

    /* enable USART IDLE interrupt */
    usart_interrupt_enable(USART2, USART_INT_IDLE);
    /* enable USART RBNE interrupt */
    usart_interrupt_enable(USART2, USART_INT_RBNE);
    /* disable USART TC interrupt */
    usart_interrupt_disable(USART2, USART_INT_TC);

    usart_baudrate_set(USART2, 115200U);
    usart_word_length_set(USART2, USART_WL_8BIT);
    usart_stop_bit_set(USART2, USART_STB_1BIT);
    usart_parity_config(USART2, USART_PM_NONE);
    usart_hardware_flow_rts_config(USART2, USART_RTS_DISABLE);
    usart_hardware_flow_cts_config(USART2, USART_CTS_DISABLE);
    usart_receive_config(USART2, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART2, USART_TRANSMIT_ENABLE);
    usart_enable(USART2);
}


/*****************************************************************************
* @brief  : configure usart5
* @param  : None
* @retval : None
* @date   : 2023-09-06
* @note   : 
*****************************************************************************/
void usart5_init(void)
{
    /* enable the port clock */
    rcu_periph_clock_enable(RCU_GPIOC);
    /* enable USART5 clock */
    rcu_periph_clock_enable(RCU_USART5);
    /* enable DMA clock */
    rcu_periph_clock_enable(RCU_DMA1);

    /**USART5 GPIO Configuration
    PC6     ------> USART5_TX
    PC7     ------> USART5_RX
    */

    /* configure USART Tx as alternate function push-pull */
    gpio_mode_set(CAT1_RX_TO_MCU_TX5_GPIO_Port, GPIO_MODE_AF, GPIO_PUPD_PULLUP, CAT1_RX_TO_MCU_TX5_Pin);
    gpio_output_options_set(CAT1_RX_TO_MCU_TX5_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, CAT1_RX_TO_MCU_TX5_Pin);
    /* connect port to USART5_Tx */
    gpio_af_set(CAT1_RX_TO_MCU_TX5_GPIO_Port, GPIO_AF_8, CAT1_RX_TO_MCU_TX5_Pin);
    
    /* configure USART Rx as alternate function push-pull */
    gpio_mode_set(CAT1_TX_TO_MCU_RX5_GPIO_Port, GPIO_MODE_AF, GPIO_PUPD_PULLUP, CAT1_TX_TO_MCU_RX5_Pin);
    gpio_output_options_set(CAT1_TX_TO_MCU_RX5_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, CAT1_TX_TO_MCU_RX5_Pin);
    /* connect port to USART5_Rx */
    gpio_af_set(CAT1_TX_TO_MCU_RX5_GPIO_Port, GPIO_AF_8, CAT1_TX_TO_MCU_RX5_Pin);

    /* USART DMA enable*/
    usart_dma_receive_config(USART5, USART_DENR_DISABLE);
    //usart_dma_transmit_config(USART5, USART_DENT_ENABLE);

    dma_single_data_parameter_struct dma_init_struct;

    /* deinitialize DMA1 channel6(USART5 TX) */
    dma_single_data_para_struct_init(&dma_init_struct);
    
    dma_deinit(DMA1, DMA_CH6);
    
    dma_init_struct.direction = DMA_MEMORY_TO_PERIPH;
    dma_init_struct.memory0_addr = NULL;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.periph_memory_width = DMA_PERIPH_WIDTH_8BIT;
    dma_init_struct.number = 0;
    dma_init_struct.periph_addr = ((uint32_t)&USART_DATA(USART5));
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.priority = DMA_PRIORITY_HIGH;
    dma_single_data_mode_init(DMA1, DMA_CH6, &dma_init_struct);

    /* configure DMA mode */
    dma_circulation_disable(DMA1, DMA_CH6);
    dma_channel_subperipheral_select(DMA1, DMA_CH6, DMA_SUBPERI5);

    /* enable DMA1 channel6 transfer complete interrupt */
    //dma_interrupt_enable(DMA1, DMA_CH6, DMA_CHXCTL_FTFIE);
    /* disable DMA1 channel6 transfer complete interrupt */
    dma_interrupt_disable(DMA1, DMA_CH6, DMA_CHXCTL_FTFIE);

    /* enable DMA1 channel6 */
    //dma_channel_enable(DMA1, DMA_CH6);

    /* USART interrupt configuration */
    nvic_irq_enable(USART5_IRQn, 5, 0);

    /* enable USART IDLE interrupt */
    usart_interrupt_enable(USART5, USART_INT_IDLE);
    /* enable USART RBNE interrupt */
    usart_interrupt_enable(USART5, USART_INT_RBNE);
    /* disable USART TC interrupt */
    usart_interrupt_disable(USART5, USART_INT_TC);

    usart_baudrate_set(USART5, 115200U);
    usart_word_length_set(USART5, USART_WL_8BIT);
    usart_stop_bit_set(USART5, USART_STB_1BIT);
    usart_parity_config(USART5, USART_PM_NONE);
    usart_hardware_flow_rts_config(USART5, USART_RTS_DISABLE);
    usart_hardware_flow_cts_config(USART5, USART_CTS_DISABLE);
    usart_receive_config(USART5, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART5, USART_TRANSMIT_ENABLE);
    usart_enable(USART5);
}


/*****************************************************************************
* @brief  : configure usart0 dma send data
* @param  : buffer: send data buf
*			len: send data len
* @retval : None
* @date   : 2023-09-06
* @note   : 
*****************************************************************************/
void usart0_dma_send(uint8_t * buffer, uint16_t len)
{
    dma_channel_disable(DMA1, DMA_CH7);

    dma_flag_clear(DMA1, DMA_CH7, DMA_FLAG_SDE);
    dma_flag_clear(DMA1, DMA_CH7, DMA_FLAG_TAE);
    dma_flag_clear(DMA1, DMA_CH7, DMA_FLAG_FTF);    // 必须清中断标志，否则发送数据会出错
    dma_memory_address_config(DMA1, DMA_CH7, DMA_MEMORY_0, (uint32_t)buffer);   // 设置要发送数据的内存地址
    dma_transfer_number_config(DMA1, DMA_CH7, len);

    dma_channel_enable(DMA1, DMA_CH7);
    usart_dma_transmit_config(USART0, USART_DENT_ENABLE);   // 使能串口DMA发送

    while(RESET == dma_flag_get(DMA1, DMA_CH7, DMA_FLAG_FTF));  // 等待发送完成
}


/*****************************************************************************
* @brief  : configure usart1 dma send data
* @param  : buffer: send data buf
*			len: send data len
* @retval : None
* @date   : 2023-09-06
* @note   : 
*****************************************************************************/
void usart1_dma_send(uint8_t * buffer, uint16_t len)
{    
    dma_channel_disable(DMA0, DMA_CH6);

    dma_flag_clear(DMA0, DMA_CH6, DMA_FLAG_SDE);
    dma_flag_clear(DMA0, DMA_CH6, DMA_FLAG_TAE);
    dma_flag_clear(DMA0, DMA_CH6, DMA_FLAG_FTF);    // 必须清中断标志，否则发送数据会出错
    dma_memory_address_config(DMA0, DMA_CH6, DMA_MEMORY_0, (uint32_t)buffer);   // 设置要发送数据的内存地址
    dma_transfer_number_config(DMA0, DMA_CH6, len);

    dma_channel_enable(DMA0, DMA_CH6);
    usart_dma_transmit_config(USART1, USART_DENT_ENABLE);   // 使能串口DMA发送

    while(RESET == dma_flag_get(DMA0, DMA_CH6, DMA_FLAG_FTF));  // 等待发送完成
}


/*****************************************************************************
* @brief  : configure usart2 dma send data
* @param  : buffer: send data buf
*			len: send data len
* @retval : None
* @date   : 2023-09-06
* @note   : 
*****************************************************************************/
void usart2_dma_send(uint8_t * buffer, uint16_t len)
{    
    dma_channel_disable(DMA0, DMA_CH3);

    dma_flag_clear(DMA0, DMA_CH3, DMA_FLAG_SDE);
    dma_flag_clear(DMA0, DMA_CH3, DMA_FLAG_TAE);
    dma_flag_clear(DMA0, DMA_CH3, DMA_FLAG_FTF);    // 必须清中断标志，否则发送数据会出错
    dma_memory_address_config(DMA0, DMA_CH3, DMA_MEMORY_0, (uint32_t)buffer);   // 设置要发送数据的内存地址
    dma_transfer_number_config(DMA0, DMA_CH3, len);

    dma_channel_enable(DMA0, DMA_CH3);
    usart_dma_transmit_config(USART2, USART_DENT_ENABLE);   // 使能串口DMA发送

    while(RESET == dma_flag_get(DMA0, DMA_CH3, DMA_FLAG_FTF));  // 等待发送完成
}


/*****************************************************************************
* @brief  : configure usart5 dma send data
* @param  : buffer: send data buf
*			len: send data len
* @retval : None
* @date   : 2023-09-06
* @note   : 
*****************************************************************************/
void usart5_dma_send(uint8_t * buffer, uint16_t len)
{    
    dma_channel_disable(DMA1, DMA_CH6);

    dma_flag_clear(DMA1, DMA_CH6, DMA_FLAG_SDE);
    dma_flag_clear(DMA1, DMA_CH6, DMA_FLAG_TAE);
    dma_flag_clear(DMA1, DMA_CH6, DMA_FLAG_FTF);    // 必须清中断标志，否则发送数据会出错
    dma_memory_address_config(DMA1, DMA_CH6, DMA_MEMORY_0, (uint32_t)buffer);   // 设置要发送数据的内存地址
    dma_transfer_number_config(DMA1, DMA_CH6, len);

    dma_channel_enable(DMA1, DMA_CH6);
    usart_dma_transmit_config(USART5, USART_DENT_ENABLE);   // 使能串口DMA发送

    while(RESET == dma_flag_get(DMA1, DMA_CH6, DMA_FLAG_FTF));  // 等待发送完成
}
