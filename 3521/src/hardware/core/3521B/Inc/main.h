/**
 * **********************************************************************************
 * @copyright Copyright (c) 2023 Shaanxi Tianji Communication Technologies Co., Ltd.
 *                         
 * @file        main.h   
 * @author      WangJun
 * @version     None
 * @date        2023-09-06     
 * @brief       None   
 * 
 * **********************************************************************************
 */

#ifndef __MAIN_H_
#define __MAIN_H_

#ifdef __cplusplus
 extern "C" {
#endif

/*gd32f4*/
#if(PRODUCT_MODEL == CMDT3521B)
  #include "gd32f4xx.h"
  #include "systick.h"
  #include "gd32f4xx_libopt.h"
/* Pin defines -----------------------------------------------------------*/

/* LED----------*/
#define LED_PG8_Pin GPIO_PIN_8
#define LED_PG8_GPIO_Port GPIOG

#define LED_PC9_Pin GPIO_PIN_9
#define LED_PC9_GPIO_Port GPIOC

#define LED_PC11_Pin GPIO_PIN_11
#define LED_PC11_GPIO_Port GPIOC

#define LED_PD0_Pin GPIO_PIN_0
#define LED_PD0_GPIO_Port GPIOD

#define LED_PD1_Pin GPIO_PIN_1
#define LED_PD1_GPIO_Port GPIOD

#define LED_PD2_Pin GPIO_PIN_2
#define LED_PD2_GPIO_Port GPIOD


/* DC CTRL---------*/
#define RF_B3_DC_CTRL_Pin GPIO_PIN_12
#define RF_B3_DC_CTRL_GPIO_Port GPIOG

#define RF_B8_DC_CTRL_Pin GPIO_PIN_14
#define RF_B8_DC_CTRL_GPIO_Port GPIOG

#define RF_B39_DC_CTRL_Pin GPIO_PIN_10
#define RF_B39_DC_CTRL_GPIO_Port GPIOD

#define RF_B40_DC_CTRL_Pin GPIO_PIN_3
#define RF_B40_DC_CTRL_GPIO_Port GPIOD

#define RF_4G_DC_EN_CTRL_Pin GPIO_PIN_2
#define RF_4G_DC_EN_CTRL_GPIO_Port GPIOF

#define RF_NR_DC_EN_CTRL_Pin GPIO_PIN_0
#define RF_NR_DC_EN_CTRL_GPIO_Port GPIOE

#define RF_MIMO_DC_EN_CTRL_Pin GPIO_PIN_15
#define RF_MIMO_DC_EN_CTRL_GPIO_Port GPIOG


/* 5G UP SILENT---------*/
#define RF_NR_UP_SILENT_CTRL_Pin GPIO_PIN_9
#define RF_NR_UP_SILENT_CTRL_GPIO_Port GPIOG

#define RF_MIMO_UP_SILENT_CTRL_Pin GPIO_PIN_1
#define RF_MIMO_UP_SILENT_CTRL_GPIO_Port GPIOE


/* ECR8668---------*/
#define ECR8668_4G_DW_RST_Pin GPIO_PIN_15
#define ECR8668_4G_DW_RST_GPIO_Port GPIOC

#define ECR8668_4G_DW_ADC_OVERLOAD_Pin GPIO_PIN_0
#define ECR8668_4G_DW_ADC_OVERLOAD_GPIO_Port GPIOC
#define ECR8668_4G_DW_ADC_OVERLOAD_EXTI_IRQn EXTI0_IRQn

#define ECR8668_4G_DW_TIA_OVERLOAD_Pin GPIO_PIN_1
#define ECR8668_4G_DW_TIA_OVERLOAD_GPIO_Port GPIOC
#define ECR8668_4G_DW_TIA_OVERLOAD_EXTI_IRQn EXTI1_IRQn

#define ECR8668_NR_RST_Pin GPIO_PIN_2
#define ECR8668_NR_RST_GPIO_Port GPIOC

#define ECR8668_NR_ADC_OVERLOAD_Pin GPIO_PIN_13
#define ECR8668_NR_ADC_OVERLOAD_GPIO_Port GPIOD
#define ECR8668_NR_ADC_OVERLOAD_EXTI_IRQn EXTI10_15_IRQn

#define ECR8668_NR_TIA_OVERLOAD_Pin GPIO_PIN_14
#define ECR8668_NR_TIA_OVERLOAD_GPIO_Port GPIOD
#define ECR8668_NR_TIA_OVERLOAD_EXTI_IRQn EXTI10_15_IRQn

#define ECR8668_MIMO_RST_Pin GPIO_PIN_5
#define ECR8668_MIMO_RST_GPIO_Port GPIOD

#define ECR8668_MIMO_ADC_OVERLOAD_Pin GPIO_PIN_6
#define ECR8668_MIMO_ADC_OVERLOAD_GPIO_Port GPIOD
#define ECR8668_MIMO_ADC_OVERLOAD_EXTI_IRQn EXTI5_9_IRQn

#define ECR8668_MIMO_TIA_OVERLOAD_Pin GPIO_PIN_7
#define ECR8668_MIMO_TIA_OVERLOAD_GPIO_Port GPIOD
#define ECR8668_MIMO_TIA_OVERLOAD_EXTI_IRQn EXTI5_9_IRQn


/* LNA----------*/
#define LNA_B3_UP_Pin GPIO_PIN_3
#define LNA_B3_UP_GPIO_Port GPIOE

#define LNA_B3_DW_1_Pin GPIO_PIN_2
#define LNA_B3_DW_1_GPIO_Port GPIOE

#define LNA_B3_DW_2_Pin GPIO_PIN_4
#define LNA_B3_DW_2_GPIO_Port GPIOE

#define LNA_B8_UP_Pin GPIO_PIN_1
#define LNA_B8_UP_GPIO_Port GPIOF

#define LNA_B8_DW_Pin GPIO_PIN_5
#define LNA_B8_DW_GPIO_Port GPIOE

#define LNA_B39_UP_Pin GPIO_PIN_13
#define LNA_B39_UP_GPIO_Port GPIOF

#define LNA_B39_DW_1_Pin GPIO_PIN_12
#define LNA_B39_DW_1_GPIO_Port GPIOF

#define LNA_B39_DW_2_Pin GPIO_PIN_14
#define LNA_B39_DW_2_GPIO_Port GPIOF

#define LNA_B40_UP_Pin GPIO_PIN_0
#define LNA_B40_UP_GPIO_Port GPIOG

#define LNA_B40_DW_1_Pin GPIO_PIN_15
#define LNA_B40_DW_1_GPIO_Port GPIOF

#define LNA_B40_DW_2_Pin GPIO_PIN_1
#define LNA_B40_DW_2_GPIO_Port GPIOG

#define LNA_NR_UP_Pin GPIO_PIN_12
#define LNA_NR_UP_GPIO_Port GPIOE

#define LNA_NR_DW_1_Pin GPIO_PIN_10
#define LNA_NR_DW_1_GPIO_Port GPIOE

#define LNA_NR_DW_2_Pin GPIO_PIN_15
#define LNA_NR_DW_2_GPIO_Port GPIOE

#define LNA_MIMO_UP_Pin GPIO_PIN_11
#define LNA_MIMO_UP_GPIO_Port GPIOG

#define LNA_MIMO_DW_1_Pin GPIO_PIN_10
#define LNA_MIMO_DW_1_GPIO_Port GPIOG

#define LNA_MIMO_DW_2_Pin GPIO_PIN_13
#define LNA_MIMO_DW_2_GPIO_Port GPIOG


/* SPDT----------*/
#define SPDT_B3_B39_UP_CTRL_Pin GPIO_PIN_6 
#define SPDT_B3_B39_UP_CTRL_GPIO_Port GPIOE 

#define SPDT_B3_B39_DW_CTRL_Pin GPIO_PIN_13 
#define SPDT_B3_B39_DW_CTRL_GPIO_Port GPIOC


/* SP4T----------*/
#define SP4T_4G_DW_RX_V2_Pin GPIO_PIN_0
#define SP4T_4G_DW_RX_V2_GPIO_Port GPIOB

#define SP4T_4G_DW_RX_V3_Pin GPIO_PIN_1
#define SP4T_4G_DW_RX_V3_GPIO_Port GPIOB

#define SP4T_4G_DW_TX_V2_Pin GPIO_PIN_2
#define SP4T_4G_DW_TX_V2_GPIO_Port GPIOB

#define SP4T_4G_DW_TX_V3_Pin GPIO_PIN_7 
#define SP4T_4G_DW_TX_V3_GPIO_Port GPIOE

#define SP4T_4G_ANT_V2_Pin GPIO_PIN_8
#define SP4T_4G_ANT_V2_GPIO_Port GPIOD

#define SP4T_4G_ANT_V3_Pin GPIO_PIN_9
#define SP4T_4G_ANT_V3_GPIO_Port GPIOD

#define SP4T_4G_BTS_V2_Pin GPIO_PIN_8
#define SP4T_4G_BTS_V2_GPIO_Port GPIOE

#define SP4T_4G_BTS_V3_Pin GPIO_PIN_12
#define SP4T_4G_BTS_V3_GPIO_Port GPIOD


/* TIM----------*/
#define ATT_4G_DW_ATT_Pin GPIO_PIN_7 
#define ATT_4G_DW_ATT_GPIO_Port GPIOA 

#define ATT_NR_DW_ATT_Pin GPIO_PIN_6  
#define ATT_NR_DW_ATT_GPIO_Port GPIOA 

#define ATT_MIMO_DW_ATT_Pin GPIO_PIN_5 
#define ATT_MIMO_DW_ATT_GPIO_Port GPIOA 

#define ATT_B3_UP_1_Pin GPIO_PIN_9  
#define ATT_B3_UP_1_GPIO_Port GPIOE

#define ATT_B3_UP_2_Pin GPIO_PIN_3  
#define ATT_B3_UP_2_GPIO_Port GPIOA

#define ATT_B8_UP_1_Pin GPIO_PIN_14  
#define ATT_B8_UP_1_GPIO_Port GPIOE

#define ATT_B8_UP_2_Pin GPIO_PIN_2  
#define ATT_B8_UP_2_GPIO_Port GPIOA

#define ATT_B39_UP_1_Pin GPIO_PIN_11  
#define ATT_B39_UP_1_GPIO_Port GPIOE

#define ATT_B39_UP_2_Pin GPIO_PIN_13
#define ATT_B39_UP_2_GPIO_Port GPIOE

#define ATT_B40_UP_1_Pin GPIO_PIN_8
#define ATT_B40_UP_1_GPIO_Port GPIOB

#define ATT_B40_UP_2_Pin GPIO_PIN_9
#define ATT_B40_UP_2_GPIO_Port GPIOB

#define ATT_NR_UP_1_Pin GPIO_PIN_0
#define ATT_NR_UP_1_GPIO_Port GPIOA

#define ATT_NR_UP_2_Pin GPIO_PIN_1  
#define ATT_NR_UP_2_GPIO_Port GPIOA

#define ATT_MIMO_UP_1_Pin GPIO_PIN_15
#define ATT_MIMO_UP_1_GPIO_Port GPIOD

#define ATT_MIMO_UP_2_Pin GPIO_PIN_8  
#define ATT_MIMO_UP_2_GPIO_Port GPIOC

/* UART-------------------*/
/* BLE----------*/
#define BLE_DC_CTRL_Pin GPIO_PIN_4
#define BLE_DC_CTRL_GPIO_Port GPIOC
        
#define BLE_RX_TO_MCU_TX2_Pin GPIO_PIN_10
#define BLE_RX_TO_MCU_TX2_GPIO_Port GPIOB
        
#define BLE_TX_TO_MCU_RX2_Pin GPIO_PIN_11
#define BLE_TX_TO_MCU_RX2_GPIO_Port GPIOB

/* CAT1---------*/
#define CAT1_DC_CTRL_Pin GPIO_PIN_2
#define CAT1_DC_CTRL_GPIO_Port GPIOG

#define CAT1_STATE_Pin GPIO_PIN_5                    // input
#define CAT1_STATE_GPIO_Port GPIOG

#define CAT1_OPEN_Pin GPIO_PIN_6
#define CAT1_OPEN_GPIO_Port GPIOG

#define CAT1_RESET_Pin GPIO_PIN_7
#define CAT1_RESET_GPIO_Port GPIOG

#define CAT1_RX_TO_MCU_TX5_Pin GPIO_PIN_6
#define CAT1_RX_TO_MCU_TX5_GPIO_Port GPIOC

#define CAT1_TX_TO_MCU_RX5_Pin GPIO_PIN_7
#define CAT1_TX_TO_MCU_RX5_GPIO_Port GPIOC

/* 5G MODEM---------*/
#define MODEM_4G_UP_Pin GPIO_PIN_3
#define MODEM_4G_UP_GPIO_Port GPIOG
#define MODEM_4G_UP_EXTI_IRQn EXTI3_IRQn

#define MODEM_4G_DW_Pin GPIO_PIN_8
#define MODEM_4G_DW_GPIO_Port GPIOA
#define MODEM_4G_DW_EXTI_IRQn EXTI5_9_IRQn

#define MODEM_5G_UP_Pin GPIO_PIN_7
#define MODEM_5G_UP_GPIO_Port GPIOB
#define MODEM_5G_UP_EXTI_IRQn EXTI5_9_IRQn

#define MODEM_5G_DW_Pin GPIO_PIN_4
#define MODEM_5G_DW_GPIO_Port GPIOD
#define MODEM_5G_DW_EXTI_IRQn EXTI4_IRQn

#define MODEM_DC_CTRL_Pin GPIO_PIN_6
#define MODEM_DC_CTRL_GPIO_Port GPIOB

#define MODEM_SYN_Pin GPIO_PIN_5                    // input
#define MODEM_SYN_GPIO_Port GPIOC

#define MODEM_RX_TO_MCU_TX0_Pin GPIO_PIN_9
#define MODEM_RX_TO_MCU_TX0_GPIO_Port GPIOA

#define MODEM_TX_TO_MCU_RX0_Pin GPIO_PIN_10
#define MODEM_TX_TO_MCU_RX0_GPIO_Port GPIOA


/* SPI--------------------*/
/* FLASH SPI0--------*/
#define FLASH_CS_Pin GPIO_PIN_15
#define FLASH_CS_GPIO_Port GPIOA

#define FLASH_CLK_Pin GPIO_PIN_3
#define FLASH_CLK_GPIO_Port GPIOB

#define FLASH_MISO_Pin GPIO_PIN_4
#define FLASH_MISO_GPIO_Port GPIOB

#define FLASH_MOSI_Pin GPIO_PIN_5
#define FLASH_MOSI_GPIO_Port GPIOB

/* FPGA MCU SPI1--------*/
#define FPGA_MCU_CS_Pin GPIO_PIN_12
#define FPGA_MCU_CS_GPIO_Port GPIOB

#define FPGA_MCU_CLK_Pin GPIO_PIN_13
#define FPGA_MCU_CLK_GPIO_Port GPIOB

#define FPGA_MCU_MISO_Pin GPIO_PIN_14
#define FPGA_MCU_MISO_GPIO_Port GPIOB

#define FPGA_MCU_MOSI_Pin GPIO_PIN_15
#define FPGA_MCU_MOSI_GPIO_Port GPIOB

/* FPGA CTRL--------*/
#define FPGA_DC_CTRL_Pin GPIO_PIN_11
#define FPGA_DC_CTRL_GPIO_Port GPIOF

/* FPGA STATUS--------*/                    // input
#define FPGA_MCU_STATUS_Pin GPIO_PIN_11
#define FPGA_MCU_STATUS_GPIO_Port GPIOD

/* FPGA JTAG EN----------*/
#define FPGA_JTAG_EN_Pin GPIO_PIN_4
#define FPGA_JTAG_EN_GPIO_Port GPIOG


/* FPGA PS --------*/
#define FPGA_N_STATUS_Pin GPIO_PIN_14
#define FPGA_N_STATUS_GPIO_Port GPIOC

#define FPGA_N_CONFIG_Pin GPIO_PIN_0
#define FPGA_N_CONFIG_GPIO_Port GPIOF

#define FPGA_PS_CS_Pin GPIO_PIN_4
#define FPGA_PS_CS_GPIO_Port GPIOA

#define FPGA_PS_DCLK_Pin GPIO_PIN_10
#define FPGA_PS_DCLK_GPIO_Port GPIOC   // 配置成GPIO

#define FPGA_PS_DATA0_Pin GPIO_PIN_12
#define FPGA_PS_DATA0_GPIO_Port GPIOC


/* ADC----------*/
#define AD_4G_DW_TRX_POW_Pin GPIO_PIN_5
#define AD_4G_DW_TRX_POW_GPIO_Port GPIOF

#define AD_NR_DW_TRX_POW_Pin GPIO_PIN_4
#define AD_NR_DW_TRX_POW_GPIO_Port GPIOF

#define AD_MIMO_DW_TRX_POW_Pin GPIO_PIN_3
#define AD_MIMO_DW_TRX_POW_GPIO_Port GPIOC

#define AD_4G_UP_Pin GPIO_PIN_6
#define AD_4G_UP_GPIO_Port GPIOF

#define AD_4G_DW_Pin GPIO_PIN_10
#define AD_4G_DW_GPIO_Port GPIOF

#define AD_NR_UP_Pin GPIO_PIN_8
#define AD_NR_UP_GPIO_Port GPIOF

#define AD_NR_DW_Pin GPIO_PIN_7
#define AD_NR_DW_GPIO_Port GPIOF

#define AD_MIMO_UP_Pin GPIO_PIN_3
#define AD_MIMO_UP_GPIO_Port GPIOF

#define AD_MIMO_DW_Pin GPIO_PIN_9
#define AD_MIMO_DW_GPIO_Port GPIOF

/* USB---------*/
#define USB_FD_DM_Pin GPIO_PIN_11
#define USB_FD_DM_GPIO_Port GPIOA

#define USB_FD_DP_Pin GPIO_PIN_12
#define USB_FD_DP_GPIO_Port GPIOA

#endif

#ifdef __cplusplus
}
#endif

#endif
