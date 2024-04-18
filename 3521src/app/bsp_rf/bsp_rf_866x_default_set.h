/*************************************** Copyright ******************************
*
* (C) Copyright 2024, Shaanxi Tianji Communication
* All Rights Reserved
*
* FileName   : bsp_rf_866x_dEfault_set.h
* Version    : none
* Author     :
* Date       : 2024-01-02
* Description: none
*******************************************************************************/
//
#ifndef __BSP_RF_866X_DEFAULT_SET_H_
#define __BSP_RF_866X_DEFAULT_SET_H_
#include <stdint.h>
#include "system_config.h"

void rf_866x_default_set(uint8_t band);
void rf_ch_lna_att_default_set(uint8_t band, uint8_t ch);

#if LOCAL_MODULE_TYPE == _MU
void rf_set_modem_cmd(uint8_t sys, uint8_t state, IS_TDD_E tdd_or_fdd );
#endif

Status_t get_bwid_by_bw(uint8_t band, uint8_t bw, uint8_t *bw_id);

#endif





