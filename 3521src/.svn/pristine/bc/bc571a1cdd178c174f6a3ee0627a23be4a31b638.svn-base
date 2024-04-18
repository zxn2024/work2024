/**
 * **********************************************************************************
 * @copyright Copyright (c) 2023 Shaanxi Tianji Communication Technologies Co., Ltd.
 *                         
 * @file        gpio.c   
 * @author      WangJun
 * @version     None
 * @date        2023-09-07     
 * @brief       None   
 * 
 * **********************************************************************************
 */

#include "gpio.h"


/*---------------------- 端口定义 ----------------------*/


/*---------------------- 宏 定 义 ----------------------*/


/*---------------------- 结 构 体 ----------------------*/


/*---------------------- 常    量 ----------------------*/


/*---------------------- 全局变量 ----------------------*/


/*---------------------- 局部变量 ----------------------*/


/*---------------------- 函    数 ----------------------*/

/*****************************************************************************
* @brief  : configure gpio
* @param  : None
* @retval : None
* @date   : 2023-09-07
* @note   : 
*****************************************************************************/
void gpio_init(void)
{
    /* enable the port clock */
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_GPIOD);
    rcu_periph_clock_enable(RCU_GPIOE);
    rcu_periph_clock_enable(RCU_GPIOF);
    rcu_periph_clock_enable(RCU_GPIOG);
    
    rcu_periph_clock_enable(RCU_SYSCFG);
    
    /*****************************LED*****************************/
    //PG8
    gpio_mode_set(LED_PG8_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_PG8_Pin);
    gpio_output_options_set(LED_PG8_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, LED_PG8_Pin);
    GPIO_BOP(LED_PG8_GPIO_Port) = LED_PG8_Pin;   // 置高电平
    //PC9
    gpio_mode_set(LED_PC9_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_PC9_Pin);
    gpio_output_options_set(LED_PC9_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, LED_PC9_Pin);
    GPIO_BOP(LED_PC9_GPIO_Port) = LED_PC9_Pin;   // 置高电平
    //PC11
    gpio_mode_set(LED_PC11_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_PC11_Pin);
    gpio_output_options_set(LED_PC11_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, LED_PC11_Pin);
    GPIO_BOP(LED_PC11_GPIO_Port) = LED_PC11_Pin;   // 置高电平
    //PD0
    gpio_mode_set(LED_PD0_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_PD0_Pin);
    gpio_output_options_set(LED_PD0_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, LED_PD0_Pin);
    GPIO_BOP(LED_PD0_GPIO_Port) = LED_PD0_Pin;   // 置高电平
    //PD1
    gpio_mode_set(LED_PD1_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_PD1_Pin);
    gpio_output_options_set(LED_PD1_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, LED_PD1_Pin);
    GPIO_BOP(LED_PD1_GPIO_Port) = LED_PD1_Pin;   // 置高电平
    //PD2
    gpio_mode_set(LED_PD2_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_PD2_Pin);
    gpio_output_options_set(LED_PD2_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, LED_PD2_Pin);
    GPIO_BOP(LED_PD2_GPIO_Port) = LED_PD2_Pin;   // 置高电平
    
    /*****************************DC CTRL*****************************/
    //PG12
    gpio_mode_set(RF_B3_DC_CTRL_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, RF_B3_DC_CTRL_Pin);
    gpio_output_options_set(RF_B3_DC_CTRL_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, RF_B3_DC_CTRL_Pin);
    GPIO_BOP(RF_B3_DC_CTRL_GPIO_Port) = RF_B3_DC_CTRL_Pin;   // 置高电平
    //PG14
    gpio_mode_set(RF_B8_DC_CTRL_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, RF_B8_DC_CTRL_Pin);
    gpio_output_options_set(RF_B8_DC_CTRL_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, RF_B8_DC_CTRL_Pin);
    GPIO_BOP(RF_B8_DC_CTRL_GPIO_Port) = RF_B8_DC_CTRL_Pin;   // 置高电平
    //PD10
    gpio_mode_set(RF_B39_DC_CTRL_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, RF_B39_DC_CTRL_Pin);
    gpio_output_options_set(RF_B39_DC_CTRL_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, RF_B39_DC_CTRL_Pin);
    GPIO_BOP(RF_B39_DC_CTRL_GPIO_Port) = RF_B39_DC_CTRL_Pin;   // 置高电平
    //PD3
    gpio_mode_set(RF_B40_DC_CTRL_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, RF_B40_DC_CTRL_Pin);
    gpio_output_options_set(RF_B40_DC_CTRL_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, RF_B40_DC_CTRL_Pin);
    GPIO_BOP(RF_B40_DC_CTRL_GPIO_Port) = RF_B40_DC_CTRL_Pin;   // 置高电平
    //PF2
    gpio_mode_set(RF_4G_DC_EN_CTRL_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, RF_4G_DC_EN_CTRL_Pin);
    gpio_output_options_set(RF_4G_DC_EN_CTRL_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, RF_4G_DC_EN_CTRL_Pin);
    GPIO_BOP(RF_4G_DC_EN_CTRL_GPIO_Port) = RF_4G_DC_EN_CTRL_Pin;   // 置高电平
    //PE0
    gpio_mode_set(RF_NR_DC_EN_CTRL_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, RF_NR_DC_EN_CTRL_Pin);
    gpio_output_options_set(RF_NR_DC_EN_CTRL_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, RF_NR_DC_EN_CTRL_Pin);
    GPIO_BC(RF_NR_DC_EN_CTRL_GPIO_Port) = RF_NR_DC_EN_CTRL_Pin;   // 置低电平
    //PG15
    gpio_mode_set(RF_MIMO_DC_EN_CTRL_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, RF_MIMO_DC_EN_CTRL_Pin);
    gpio_output_options_set(RF_MIMO_DC_EN_CTRL_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, RF_MIMO_DC_EN_CTRL_Pin);
    GPIO_BC(RF_MIMO_DC_EN_CTRL_GPIO_Port) = RF_MIMO_DC_EN_CTRL_Pin;   // 置低电平
    
    
    /*****************************SILENT*****************************/
    /* NR UP SILENT---------*/
    //PG9
    gpio_mode_set(RF_NR_UP_SILENT_CTRL_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, RF_NR_UP_SILENT_CTRL_Pin);
    gpio_output_options_set(RF_NR_UP_SILENT_CTRL_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, RF_NR_UP_SILENT_CTRL_Pin);
    GPIO_BC(RF_NR_UP_SILENT_CTRL_GPIO_Port) = RF_NR_UP_SILENT_CTRL_Pin;   // 置低电平  上升沿有效
    /* MIMO UP SILENT---------*/
    //PE1
    gpio_mode_set(RF_MIMO_UP_SILENT_CTRL_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, RF_MIMO_UP_SILENT_CTRL_Pin);
    gpio_output_options_set(RF_MIMO_UP_SILENT_CTRL_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, RF_MIMO_UP_SILENT_CTRL_Pin);
    GPIO_BC(RF_MIMO_UP_SILENT_CTRL_GPIO_Port) = RF_MIMO_UP_SILENT_CTRL_Pin;   // 置低电平  上升沿有效

    /*****************************ECR8668*****************************/
    //PC15
    gpio_mode_set(ECR8668_4G_DW_RST_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, ECR8668_4G_DW_RST_Pin);
    gpio_output_options_set(ECR8668_4G_DW_RST_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, ECR8668_4G_DW_RST_Pin);
    GPIO_BOP(ECR8668_4G_DW_RST_GPIO_Port) = ECR8668_4G_DW_RST_Pin;   // 置高电平
    //PC2
    gpio_mode_set(ECR8668_NR_RST_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, ECR8668_NR_RST_Pin);
    gpio_output_options_set(ECR8668_NR_RST_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, ECR8668_NR_RST_Pin);
    GPIO_BOP(ECR8668_NR_RST_GPIO_Port) = ECR8668_NR_RST_Pin;   // 置高电平
    //PD5
    gpio_mode_set(ECR8668_MIMO_RST_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, ECR8668_MIMO_RST_Pin);
    gpio_output_options_set(ECR8668_MIMO_RST_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, ECR8668_MIMO_RST_Pin);
    GPIO_BOP(ECR8668_MIMO_RST_GPIO_Port) = ECR8668_MIMO_RST_Pin;   // 置高电平
    
    /*****************************LNA*****************************/
    //PE3
    gpio_mode_set(LNA_B3_UP_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LNA_B3_UP_Pin);
    gpio_output_options_set(LNA_B3_UP_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, LNA_B3_UP_Pin);
    GPIO_BOP(LNA_B3_UP_GPIO_Port) = LNA_B3_UP_Pin;   // 置高电平
    //PE2
    gpio_mode_set(LNA_B3_DW_1_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LNA_B3_DW_1_Pin);
    gpio_output_options_set(LNA_B3_DW_1_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, LNA_B3_DW_1_Pin);
    GPIO_BOP(LNA_B3_DW_1_GPIO_Port) = LNA_B3_DW_1_Pin;   // 置高电平
    //PE4
    gpio_mode_set(LNA_B3_DW_2_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LNA_B3_DW_2_Pin);
    gpio_output_options_set(LNA_B3_DW_2_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, LNA_B3_DW_2_Pin);
    GPIO_BOP(LNA_B3_DW_2_GPIO_Port) = LNA_B3_DW_2_Pin;   // 置高电平
    //PF1
    gpio_mode_set(LNA_B8_UP_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LNA_B8_UP_Pin);
    gpio_output_options_set(LNA_B8_UP_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, LNA_B8_UP_Pin);
    GPIO_BOP(LNA_B8_UP_GPIO_Port) = LNA_B8_UP_Pin;   // 置高电平
    //PE5
    gpio_mode_set(LNA_B8_DW_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LNA_B8_DW_Pin);
    gpio_output_options_set(LNA_B8_DW_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, LNA_B8_DW_Pin);
    GPIO_BOP(LNA_B8_DW_GPIO_Port) = LNA_B8_DW_Pin;   // 置高电平
    //PF13
    gpio_mode_set(LNA_B39_UP_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LNA_B39_UP_Pin);
    gpio_output_options_set(LNA_B39_UP_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, LNA_B39_UP_Pin);
    GPIO_BOP(LNA_B39_UP_GPIO_Port) = LNA_B39_UP_Pin;   // 置高电平
    //PF12
    gpio_mode_set(LNA_B39_DW_1_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LNA_B39_DW_1_Pin);
    gpio_output_options_set(LNA_B39_DW_1_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, LNA_B39_DW_1_Pin);
    GPIO_BOP(LNA_B39_DW_1_GPIO_Port) = LNA_B39_DW_1_Pin;   // 置高电平
    //PF14
    gpio_mode_set(LNA_B39_DW_2_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LNA_B39_DW_2_Pin);
    gpio_output_options_set(LNA_B39_DW_2_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, LNA_B39_DW_2_Pin);
    GPIO_BOP(LNA_B39_DW_2_GPIO_Port) = LNA_B39_DW_2_Pin;   // 置高电平
    //PG0
    gpio_mode_set(LNA_B40_UP_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LNA_B40_UP_Pin);
    gpio_output_options_set(LNA_B40_UP_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, LNA_B40_UP_Pin);
    GPIO_BOP(LNA_B40_UP_GPIO_Port) = LNA_B40_UP_Pin;   // 置高电平
    //PF15
    gpio_mode_set(LNA_B40_DW_1_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LNA_B40_DW_1_Pin);
    gpio_output_options_set(LNA_B40_DW_1_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, LNA_B40_DW_1_Pin);
    GPIO_BOP(LNA_B40_DW_1_GPIO_Port) = LNA_B40_DW_1_Pin;   // 置高电平
    //PG1
    gpio_mode_set(LNA_B40_DW_2_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LNA_B40_DW_2_Pin);
    gpio_output_options_set(LNA_B40_DW_2_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, LNA_B40_DW_2_Pin);
    GPIO_BOP(LNA_B40_DW_2_GPIO_Port) = LNA_B40_DW_2_Pin;   // 置高电平
    //PE12
    gpio_mode_set(LNA_NR_UP_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LNA_NR_UP_Pin);
    gpio_output_options_set(LNA_NR_UP_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, LNA_NR_UP_Pin);
    GPIO_BOP(LNA_NR_UP_GPIO_Port) = LNA_NR_UP_Pin;   // 置高电平
    //PE10
    gpio_mode_set(LNA_NR_DW_1_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LNA_NR_DW_1_Pin);
    gpio_output_options_set(LNA_NR_DW_1_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, LNA_NR_DW_1_Pin);
    GPIO_BOP(LNA_NR_DW_1_GPIO_Port) = LNA_NR_DW_1_Pin;   // 置高电平
    //PE15
    gpio_mode_set(LNA_NR_DW_2_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LNA_NR_DW_2_Pin);
    gpio_output_options_set(LNA_NR_DW_2_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, LNA_NR_DW_2_Pin);
    GPIO_BOP(LNA_NR_DW_2_GPIO_Port) = LNA_NR_DW_2_Pin;   // 置高电平
    //PG11
    gpio_mode_set(LNA_MIMO_UP_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LNA_MIMO_UP_Pin);
    gpio_output_options_set(LNA_MIMO_UP_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, LNA_MIMO_UP_Pin);
    GPIO_BOP(LNA_MIMO_UP_GPIO_Port) = LNA_MIMO_UP_Pin;   // 置高电平
    //PG10
    gpio_mode_set(LNA_MIMO_DW_1_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LNA_MIMO_DW_1_Pin);
    gpio_output_options_set(LNA_MIMO_DW_1_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, LNA_MIMO_DW_1_Pin);
    GPIO_BOP(LNA_MIMO_DW_1_GPIO_Port) = LNA_MIMO_DW_1_Pin;   // 置高电平
    //PG13
    gpio_mode_set(LNA_MIMO_DW_2_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LNA_MIMO_DW_2_Pin);
    gpio_output_options_set(LNA_MIMO_DW_2_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, LNA_MIMO_DW_2_Pin);
    GPIO_BOP(LNA_MIMO_DW_2_GPIO_Port) = LNA_MIMO_DW_2_Pin;   // 置高电平
    
    /*****************************SPDT*****************************/
    //PE6
    gpio_mode_set(SPDT_B3_B39_UP_CTRL_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, SPDT_B3_B39_UP_CTRL_Pin);
    gpio_output_options_set(SPDT_B3_B39_UP_CTRL_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, SPDT_B3_B39_UP_CTRL_Pin);
    GPIO_BOP(SPDT_B3_B39_UP_CTRL_GPIO_Port) = SPDT_B3_B39_UP_CTRL_Pin;   // 置高电平
    //PC13
    gpio_mode_set(SPDT_B3_B39_DW_CTRL_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, SPDT_B3_B39_DW_CTRL_Pin);
    gpio_output_options_set(SPDT_B3_B39_DW_CTRL_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, SPDT_B3_B39_DW_CTRL_Pin);
    GPIO_BOP(SPDT_B3_B39_DW_CTRL_GPIO_Port) = SPDT_B3_B39_DW_CTRL_Pin;   // 置高电平
    
    /*****************************SP4T*****************************/
    //PB0
    gpio_mode_set(SP4T_4G_DW_RX_V2_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, SP4T_4G_DW_RX_V2_Pin);
    gpio_output_options_set(SP4T_4G_DW_RX_V2_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, SP4T_4G_DW_RX_V2_Pin);
    GPIO_BOP(SP4T_4G_DW_RX_V2_GPIO_Port) = SP4T_4G_DW_RX_V2_Pin;   // 置高电平
    //PB1
    gpio_mode_set(SP4T_4G_DW_RX_V3_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, SP4T_4G_DW_RX_V3_Pin);
    gpio_output_options_set(SP4T_4G_DW_RX_V3_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, SP4T_4G_DW_RX_V3_Pin);
    GPIO_BOP(SP4T_4G_DW_RX_V3_GPIO_Port) = SP4T_4G_DW_RX_V3_Pin;   // 置高电平
    //PB2
    gpio_mode_set(SP4T_4G_DW_TX_V2_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, SP4T_4G_DW_TX_V2_Pin);
    gpio_output_options_set(SP4T_4G_DW_TX_V2_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, SP4T_4G_DW_TX_V2_Pin);
    GPIO_BOP(SP4T_4G_DW_TX_V2_GPIO_Port) = SP4T_4G_DW_TX_V2_Pin;   // 置高电平
    //PE7
    gpio_mode_set(SP4T_4G_DW_TX_V3_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, SP4T_4G_DW_TX_V3_Pin);
    gpio_output_options_set(SP4T_4G_DW_TX_V3_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, SP4T_4G_DW_TX_V3_Pin);
    GPIO_BOP(SP4T_4G_DW_TX_V3_GPIO_Port) = SP4T_4G_DW_TX_V3_Pin;   // 置高电平
    //PE8
    gpio_mode_set(SP4T_4G_BTS_V2_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, SP4T_4G_BTS_V2_Pin);
    gpio_output_options_set(SP4T_4G_BTS_V2_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, SP4T_4G_BTS_V2_Pin);
    GPIO_BOP(SP4T_4G_BTS_V2_GPIO_Port) = SP4T_4G_BTS_V2_Pin;   // 置高电平
    //PD12
    gpio_mode_set(SP4T_4G_BTS_V3_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, SP4T_4G_BTS_V3_Pin);
    gpio_output_options_set(SP4T_4G_BTS_V3_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, SP4T_4G_BTS_V3_Pin);
    GPIO_BOP(SP4T_4G_BTS_V3_GPIO_Port) = SP4T_4G_BTS_V3_Pin;   // 置高电平
    //PD8
    gpio_mode_set(SP4T_4G_ANT_V2_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, SP4T_4G_ANT_V2_Pin);
    gpio_output_options_set(SP4T_4G_ANT_V2_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, SP4T_4G_ANT_V2_Pin);
    GPIO_BOP(SP4T_4G_ANT_V2_GPIO_Port) = SP4T_4G_ANT_V2_Pin;   // 置高电平
    //PD9
    gpio_mode_set(SP4T_4G_ANT_V3_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, SP4T_4G_ANT_V3_Pin);
    gpio_output_options_set(SP4T_4G_ANT_V3_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, SP4T_4G_ANT_V3_Pin);
    GPIO_BOP(SP4T_4G_ANT_V3_GPIO_Port) = SP4T_4G_ANT_V3_Pin;   // 置高电平
    
    /*****************************BLE*****************************/
    //PC4
    gpio_mode_set(BLE_DC_CTRL_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, BLE_DC_CTRL_Pin);
    gpio_output_options_set(BLE_DC_CTRL_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, BLE_DC_CTRL_Pin);
    GPIO_BOP(BLE_DC_CTRL_GPIO_Port) = BLE_DC_CTRL_Pin;   // 置高电平
    
    /*****************************CAT1*****************************/
    //PG2
    gpio_mode_set(CAT1_DC_CTRL_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, CAT1_DC_CTRL_Pin);
    gpio_output_options_set(CAT1_DC_CTRL_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, CAT1_DC_CTRL_Pin);
    GPIO_BOP(CAT1_DC_CTRL_GPIO_Port) = CAT1_DC_CTRL_Pin;   // 置高电平
    //PG6
    gpio_mode_set(CAT1_OPEN_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, CAT1_OPEN_Pin);
    gpio_output_options_set(CAT1_OPEN_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, CAT1_OPEN_Pin);
    GPIO_BOP(CAT1_OPEN_GPIO_Port) = CAT1_OPEN_Pin;   // 置高电平
    //PG7
    gpio_mode_set(CAT1_RESET_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, CAT1_RESET_Pin);
    gpio_output_options_set(CAT1_RESET_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, CAT1_RESET_Pin);
    GPIO_BOP(CAT1_RESET_GPIO_Port) = CAT1_RESET_Pin;   // 置高电平
    //PG5
    gpio_mode_set(CAT1_STATE_GPIO_Port, GPIO_MODE_INPUT, GPIO_PUPD_NONE, CAT1_STATE_Pin);

    /*****************************exit*****************************/
    /*****4G 8668*****/
    // PC0
    /* configure pin as input */
    gpio_mode_set(ECR8668_4G_DW_ADC_OVERLOAD_GPIO_Port, GPIO_MODE_INPUT, GPIO_PUPD_NONE, ECR8668_4G_DW_ADC_OVERLOAD_Pin);
    /* enable and set interrupt to the lowest priority */
    nvic_irq_enable(ECR8668_4G_DW_ADC_OVERLOAD_EXTI_IRQn, 15U, 0U);
    /* connect line to key GPIO pin */
    syscfg_exti_line_config(EXTI_SOURCE_GPIOC, EXTI_SOURCE_PIN0);
    /* configure EXTI line */
    exti_init(EXTI_0, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
    exti_interrupt_flag_clear(EXTI_0);
    
    // PC1
    /* configure pin as input */
    gpio_mode_set(ECR8668_4G_DW_TIA_OVERLOAD_GPIO_Port, GPIO_MODE_INPUT, GPIO_PUPD_NONE, ECR8668_4G_DW_TIA_OVERLOAD_Pin);
    /* enable and set interrupt to the lowest priority */
    nvic_irq_enable(ECR8668_4G_DW_TIA_OVERLOAD_EXTI_IRQn, 15U, 0U);
    /* connect line to key GPIO pin */
    syscfg_exti_line_config(EXTI_SOURCE_GPIOC, EXTI_SOURCE_PIN1);
    /* configure EXTI line */
    exti_init(EXTI_1, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
    exti_interrupt_flag_clear(EXTI_1);

    /*****NR 8668*****/
    //nr 
    // PD13
    /* configure pin as input */
    gpio_mode_set(ECR8668_NR_ADC_OVERLOAD_GPIO_Port, GPIO_MODE_INPUT, GPIO_PUPD_NONE, ECR8668_NR_ADC_OVERLOAD_Pin);
    /* enable and set interrupt to the lowest priority */
    nvic_irq_enable(ECR8668_NR_ADC_OVERLOAD_EXTI_IRQn, 15U, 0U);
    /* connect line to key GPIO pin */
    syscfg_exti_line_config(EXTI_SOURCE_GPIOD, EXTI_SOURCE_PIN13);
    /* configure EXTI line */
    exti_init(EXTI_13, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
    exti_interrupt_flag_clear(EXTI_13);

    // PD14
    /* configure pin as input */
    gpio_mode_set(ECR8668_NR_TIA_OVERLOAD_GPIO_Port, GPIO_MODE_INPUT, GPIO_PUPD_NONE, ECR8668_NR_TIA_OVERLOAD_Pin);
    /* enable and set interrupt to the lowest priority */
    nvic_irq_enable(ECR8668_NR_TIA_OVERLOAD_EXTI_IRQn, 15U, 0U);
    /* connect line to key GPIO pin */
    syscfg_exti_line_config(EXTI_SOURCE_GPIOD, EXTI_SOURCE_PIN14);
    /* configure EXTI line */
    exti_init(EXTI_14, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
    exti_interrupt_flag_clear(EXTI_14);
    
    /*****MIMO 8668*****/
    // PD6
    /* configure pin as input */
    gpio_mode_set(ECR8668_MIMO_ADC_OVERLOAD_GPIO_Port, GPIO_MODE_INPUT, GPIO_PUPD_NONE, ECR8668_MIMO_ADC_OVERLOAD_Pin);
    /* enable and set interrupt to the lowest priority */
    nvic_irq_enable(ECR8668_MIMO_ADC_OVERLOAD_EXTI_IRQn, 15U, 0U);
    /* connect line to key GPIO pin */
    syscfg_exti_line_config(EXTI_SOURCE_GPIOD, EXTI_SOURCE_PIN6);
    /* configure EXTI line */
    exti_init(EXTI_6, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
    exti_interrupt_flag_clear(EXTI_6);

    // PD7
    /* configure pin as input */
    gpio_mode_set(ECR8668_MIMO_TIA_OVERLOAD_GPIO_Port, GPIO_MODE_INPUT, GPIO_PUPD_NONE, ECR8668_MIMO_TIA_OVERLOAD_Pin);
    /* enable and set interrupt to the lowest priority */
    nvic_irq_enable(ECR8668_MIMO_TIA_OVERLOAD_EXTI_IRQn, 15U, 0U);
    /* connect line to key GPIO pin */
    syscfg_exti_line_config(EXTI_SOURCE_GPIOD, EXTI_SOURCE_PIN7);
    /* configure EXTI line */
    exti_init(EXTI_7, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
    exti_interrupt_flag_clear(EXTI_7);

    /*****4G up_down*****/
    // PG3
    /* configure pin as input */
    gpio_mode_set(MODEM_4G_UP_GPIO_Port, GPIO_MODE_INPUT, GPIO_PUPD_NONE, MODEM_4G_UP_Pin);
    /* enable and set interrupt to the lowest priority */
    nvic_irq_enable(MODEM_4G_UP_EXTI_IRQn, 15U, 0U);
    /* connect line to key GPIO pin */
    syscfg_exti_line_config(EXTI_SOURCE_GPIOG, EXTI_SOURCE_PIN3);
    /* configure EXTI line */
    exti_init(EXTI_3, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
    exti_interrupt_flag_clear(EXTI_3);
    
    // PA8
    /* configure pin as input */
    gpio_mode_set(MODEM_4G_DW_GPIO_Port, GPIO_MODE_INPUT, GPIO_PUPD_NONE, MODEM_4G_DW_Pin);
    /* enable and set interrupt to the lowest priority */
    nvic_irq_enable(MODEM_4G_DW_EXTI_IRQn, 15U, 0U);
    /* connect line to key GPIO pin */
    syscfg_exti_line_config(EXTI_SOURCE_GPIOA, EXTI_SOURCE_PIN8);
    /* configure EXTI line */
    exti_init(EXTI_8, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
    exti_interrupt_flag_clear(EXTI_8);
    
    /*****5g up_down*****/
    // // PB7
    // /* configure pin as input */
    // gpio_mode_set(MODEM_5G_UP_GPIO_Port, GPIO_MODE_INPUT, GPIO_PUPD_NONE, MODEM_5G_UP_Pin);
    // /* enable and set interrupt to the lowest priority */
    // nvic_irq_enable(MODEM_5G_UP_EXTI_IRQn, 15U, 0U);
    // /* connect line to key GPIO pin */
    // syscfg_exti_line_config(EXTI_SOURCE_GPIOB, EXTI_SOURCE_PIN7);
    // /* configure EXTI line */
    // exti_init(EXTI_7, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
    // exti_interrupt_flag_clear(EXTI_7);
    
    // PD4
    /* configure pin as input */
    gpio_mode_set(MODEM_5G_DW_GPIO_Port, GPIO_MODE_INPUT, GPIO_PUPD_NONE, MODEM_5G_DW_Pin);
    /* enable and set interrupt to the lowest priority */
    nvic_irq_enable(MODEM_5G_DW_EXTI_IRQn, 15U, 0U);
    /* connect line to key GPIO pin */
    syscfg_exti_line_config(EXTI_SOURCE_GPIOD, EXTI_SOURCE_PIN4);
    /* configure EXTI line */
    exti_init(EXTI_4, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
    exti_interrupt_flag_clear(EXTI_4);
    
    /*****************************5G MODEM*****************************/
    //PB6
    gpio_mode_set(MODEM_DC_CTRL_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, MODEM_DC_CTRL_Pin);
    gpio_output_options_set(MODEM_DC_CTRL_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, MODEM_DC_CTRL_Pin);
    GPIO_BC(MODEM_DC_CTRL_GPIO_Port) = MODEM_DC_CTRL_Pin;   // 置低电平
    //PC5
    gpio_mode_set(MODEM_SYN_GPIO_Port, GPIO_MODE_INPUT, GPIO_PUPD_NONE, MODEM_SYN_Pin);
    
    /*****************************FPGA DC CTRL*****************************/
    //PF11
    gpio_mode_set(FPGA_DC_CTRL_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, FPGA_DC_CTRL_Pin);
    gpio_output_options_set(FPGA_DC_CTRL_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, FPGA_DC_CTRL_Pin);
    GPIO_BOP(FPGA_DC_CTRL_GPIO_Port) = FPGA_DC_CTRL_Pin;   // 置高电平
    
    /*****************************FPGA STATUS*****************************/
    //PD11
    gpio_mode_set(FPGA_MCU_STATUS_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, FPGA_MCU_STATUS_Pin);
    gpio_output_options_set(FPGA_MCU_STATUS_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, FPGA_MCU_STATUS_Pin);
    //GPIO_BOP(FPGA_MCU_STATUS_GPIO_Port) = FPGA_MCU_STATUS_Pin;   // 置高电平		

    /*****************************FPGA JTAG EN*****************************/
    //PG4
    gpio_mode_set(FPGA_JTAG_EN_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, FPGA_JTAG_EN_Pin);
    gpio_output_options_set(FPGA_JTAG_EN_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, FPGA_JTAG_EN_Pin);
    GPIO_BOP(FPGA_JTAG_EN_GPIO_Port) = FPGA_JTAG_EN_Pin;   // 置高电平

    /*****************************SPI CS*****************************/
    //PA15
    gpio_mode_set(FLASH_CS_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, FLASH_CS_Pin);
    gpio_output_options_set(FLASH_CS_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, FLASH_CS_Pin);
    GPIO_BOP(FLASH_CS_GPIO_Port) = FLASH_CS_Pin;        // 置高电平
    //PB12
    gpio_mode_set(FPGA_MCU_CS_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, FPGA_MCU_CS_Pin);
    gpio_output_options_set(FPGA_MCU_CS_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, FPGA_MCU_CS_Pin);
    GPIO_BOP(FPGA_MCU_CS_GPIO_Port) = FPGA_MCU_CS_Pin;        // 置高电平

    /*****************************FPGA PS*****************************/
    //PC14
    gpio_mode_set(FPGA_N_STATUS_GPIO_Port, GPIO_MODE_INPUT, GPIO_PUPD_NONE, FPGA_N_STATUS_Pin);
    //gpio_output_options_set(FPGA_N_STATUS_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, FPGA_N_STATUS_Pin);
    //GPIO_BOP(FPGA_N_STATUS_GPIO_Port) = FPGA_N_STATUS_Pin;   // 置高电平
    //PF0
    gpio_mode_set(FPGA_N_CONFIG_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, FPGA_N_CONFIG_Pin);
    gpio_output_options_set(FPGA_N_CONFIG_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, FPGA_N_CONFIG_Pin);
    GPIO_BOP(FPGA_N_CONFIG_GPIO_Port) = FPGA_N_CONFIG_Pin;   // 置高电平
    //PA4
    gpio_mode_set(FPGA_PS_CS_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, FPGA_PS_CS_Pin);
    gpio_output_options_set(FPGA_PS_CS_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, FPGA_PS_CS_Pin);
    GPIO_BOP(FPGA_PS_CS_GPIO_Port) = FPGA_PS_CS_Pin;        // 置高电平
    //PC10
    gpio_mode_set(FPGA_PS_DCLK_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, FPGA_PS_DCLK_Pin);
    gpio_output_options_set(FPGA_PS_DCLK_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, FPGA_PS_DCLK_Pin);
    GPIO_BOP(FPGA_PS_DCLK_GPIO_Port) = FPGA_PS_DCLK_Pin;    // 置高电平
    //PC12
    gpio_mode_set(FPGA_PS_DATA0_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, FPGA_PS_DATA0_Pin);
    gpio_output_options_set(FPGA_PS_DATA0_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, FPGA_PS_DATA0_Pin);
    GPIO_BOP(FPGA_PS_DATA0_GPIO_Port) = FPGA_PS_DATA0_Pin;  // 置高电平
}
