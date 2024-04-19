/**
 ******************************************************************************
 * @file    modem.h
 * @author
 * @version V1.0.0
 * @date    2023-10-14
 * @brief   This file provides code for the modem interface.
 ******************************************************************************
 */

#ifndef __AT_CMD_H
#define __AT_CMD_H
#include "MCU_config.h"

Status_t BSP_uart_AT_cmd_process(uint8_t *cmd_data, uint16_t len);

#endif /* modem_SPI_H */
