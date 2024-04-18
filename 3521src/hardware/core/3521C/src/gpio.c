/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
        * Free pins are configured automatically as Analog (this feature is enabled through
        * the Code Generation settings)
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, LNA_B3_DW_1_Pin|LNA_B3_UP_Pin|LNA_B3_DW_2_Pin|LNA_B8_DW_Pin
                          |SPDT_B3_B39_UP_CTRL_Pin|SP4T_4G_DW_TX_V3_Pin|SP4T_4G_BTS_V2_Pin|LNA_NR_DW_1_Pin
                          |LNA_NR_UP_Pin|LNA_NR_DW_2_Pin|RF_MIMO_UP_SILENT_CTRL_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, SPDT_B3_B39_DW_CTRL_Pin|FPGA_N_STATUS_Pin|ECR8668_4G_DW_RST_Pin|ECR8668_NR_RST_Pin
                          |BLE_DC_CTRL_Pin|FPGA_PS_DCLK_Pin|FPGA_PS_DATA0_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOF, FPGA_N_CONFIG_Pin|LNA_B8_UP_Pin|FPGA_DC_CTRL_Pin|LNA_B39_DW_1_Pin
                          |LNA_B39_UP_Pin|LNA_B39_DW_2_Pin|LNA_B40_DW_1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(RF_4G_DC_EN_CTRL_GPIO_Port, RF_4G_DC_EN_CTRL_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, FPGA_PS_CS_Pin|FLASH_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, SP4T_4G_DW_RX_V2_Pin|SP4T_4G_DW_RX_V3_Pin|SP4T_4G_DW_TX_V2_Pin|FPGA_MCU_CS_Pin, GPIO_PIN_RESET);
						  
  HAL_GPIO_WritePin(GPIOB, MODEM_DC_CTRL_Pin, GPIO_PIN_SET);						  

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOG, LNA_B40_UP_Pin|LNA_B40_DW_2_Pin|CAT1_DC_CTRL_Pin|FPGA_JTAG_EN_Pin
                          |CAT1_OPEN_Pin|CAT1_RESET_Pin|RF_NR_UP_SILENT_CTRL_Pin|LNA_MIMO_DW_1_Pin
                          |LNA_MIMO_UP_Pin|LNA_MIMO_DW_2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, SP4T_4G_ANT_V2_Pin|SP4T_4G_ANT_V3_Pin|FPGA_MCU_STATUS_Pin|SP4T_4G_BTS_V3_Pin
                          |ECR8668_MIMO_RST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, RF_B39_DC_CTRL_Pin|LED_PD0_Pin|LED_PD1_Pin|LED_PD2_Pin
                          |RF_B40_DC_CTRL_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOG, LED_PG8_Pin|RF_B3_DC_CTRL_Pin|RF_B8_DC_CTRL_Pin|RF_MIMO_DC_EN_CTRL_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, LED_PC9_Pin|LED_PC11_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(RF_NR_DC_EN_CTRL_GPIO_Port, RF_NR_DC_EN_CTRL_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : PEPin PEPin PEPin PEPin
                           PEPin PEPin PEPin PEPin
                           PEPin PEPin PEPin PEPin */
  GPIO_InitStruct.Pin = LNA_B3_DW_1_Pin|LNA_B3_UP_Pin|LNA_B3_DW_2_Pin|LNA_B8_DW_Pin
                          |SPDT_B3_B39_UP_CTRL_Pin|SP4T_4G_DW_TX_V3_Pin|SP4T_4G_BTS_V2_Pin|LNA_NR_DW_1_Pin
                          |LNA_NR_UP_Pin|LNA_NR_DW_2_Pin|RF_NR_DC_EN_CTRL_Pin|RF_MIMO_UP_SILENT_CTRL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : PCPin PCPin PCPin PCPin
                           PCPin PCPin PCPin */
  GPIO_InitStruct.Pin = SPDT_B3_B39_DW_CTRL_Pin|FPGA_N_STATUS_Pin|ECR8668_4G_DW_RST_Pin|ECR8668_NR_RST_Pin
                          |BLE_DC_CTRL_Pin|LED_PC9_Pin|LED_PC11_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PFPin PFPin PFPin PFPin
                           PFPin PFPin PFPin PFPin */
  GPIO_InitStruct.Pin = FPGA_N_CONFIG_Pin|LNA_B8_UP_Pin|RF_4G_DC_EN_CTRL_Pin|FPGA_DC_CTRL_Pin
                          |LNA_B39_DW_1_Pin|LNA_B39_UP_Pin|LNA_B39_DW_2_Pin|LNA_B40_DW_1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pins : PCPin PCPin */
  GPIO_InitStruct.Pin = ECR8668_4G_DW_ADC_OVERLOAD_Pin|ECR8668_4G_DW_TIA_OVERLOAD_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PAPin PAPin */
  GPIO_InitStruct.Pin = FPGA_PS_CS_Pin|FLASH_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = MODEM_SYN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(MODEM_SYN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PBPin PBPin PBPin PBPin
                           PBPin */
  GPIO_InitStruct.Pin = SP4T_4G_DW_RX_V2_Pin|SP4T_4G_DW_RX_V3_Pin|SP4T_4G_DW_TX_V2_Pin|FPGA_MCU_CS_Pin
                          |MODEM_DC_CTRL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PGPin PGPin PGPin PGPin
                           PGPin PGPin PGPin PGPin
                           PGPin PGPin PGPin PGPin
                           PGPin PGPin */
  GPIO_InitStruct.Pin = LNA_B40_UP_Pin|LNA_B40_DW_2_Pin|CAT1_DC_CTRL_Pin|FPGA_JTAG_EN_Pin
                          |CAT1_OPEN_Pin|CAT1_RESET_Pin|LED_PG8_Pin|RF_NR_UP_SILENT_CTRL_Pin
                          |LNA_MIMO_DW_1_Pin|LNA_MIMO_UP_Pin|RF_B3_DC_CTRL_Pin|LNA_MIMO_DW_2_Pin
                          |RF_B8_DC_CTRL_Pin|RF_MIMO_DC_EN_CTRL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pins : PDPin PDPin PDPin PDPin
                           PDPin PDPin PDPin PDPin
                           PDPin PDPin */
  GPIO_InitStruct.Pin = SP4T_4G_ANT_V2_Pin|SP4T_4G_ANT_V3_Pin|RF_B39_DC_CTRL_Pin|FPGA_MCU_STATUS_Pin
                          |SP4T_4G_BTS_V3_Pin|LED_PD0_Pin|LED_PD1_Pin|LED_PD2_Pin
                          |RF_B40_DC_CTRL_Pin|ECR8668_MIMO_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : PDPin PDPin PDPin PDPin
                           PDPin */
  GPIO_InitStruct.Pin = ECR8668_NR_ADC_OVERLOAD_Pin|ECR8668_NR_TIA_OVERLOAD_Pin|MODEM_5G_DW_Pin|ECR8668_MIMO_ADC_OVERLOAD_Pin
                          |ECR8668_MIMO_TIA_OVERLOAD_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = MODEM_4G_UP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(MODEM_4G_UP_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = CAT1_STATE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(CAT1_STATE_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = MODEM_4G_DW_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(MODEM_4G_DW_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PCPin PCPin */
  GPIO_InitStruct.Pin = FPGA_PS_DCLK_Pin|FPGA_PS_DATA0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = MODEM_5G_UP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(MODEM_5G_UP_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI1_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

  HAL_NVIC_SetPriority(EXTI3_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);

  HAL_NVIC_SetPriority(EXTI4_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */
