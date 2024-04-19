/*************************************** Copyright ******************************
*
* (C) Copyright 2023, Shaanxi Tianji Communication
* All Rights Reserved
*
* FileName   : bsp_testmode_ergodic.h
* Version    : none
* Author     :
* Date       : 2023-12-14
* Description: none
*******************************************************************************/
//
#ifndef __BSP_TESTMODE_ERGODIC_H_
#define __BSP_TESTMODE_ERGODIC_H_

#include <stdint.h>
#include "bsp_testmode_define.h"
uint8_t tm_pdu_public_data_ergodic(uint8_t dev_id, uint8_t cmd, tm_msg_t * pt_msg, tm_oid_data_t * pt, tm_save_flag_t * t_save);
//uint8_t tm_pdu_public_data_ergodic( uint8_t dev_id, uint8_t cmd, tm_oid_data_t *pt, tm_save_flag_t *t_save );
uint8_t tm_pdu_monitor_data_ergodic(uint8_t dev_id, uint8_t cmd, tm_oid_data_t * pt, tm_save_flag_t * t_save);
uint8_t tm_pdu_cat1_data_ergodic(uint8_t dev_id, uint8_t cmd, tm_oid_data_t * pt, tm_save_flag_t * t_save);
uint8_t tm_pdu_modem_data_ergodic(uint8_t dev_id, uint8_t cmd, tm_oid_data_t * pt, tm_save_flag_t * t_save);
uint8_t tm_pdu_fpga_data_ergodic(uint8_t dev_id, uint8_t cmd, tm_oid_data_t * pt, tm_save_flag_t * t_save);
uint8_t tm_pdu_rf_data_ergodic(uint8_t dev_id, uint8_t cmd, tm_oid_data_t * pt, tm_save_flag_t * t_save);
uint8_t tm_pdu_ble_data_ergodic(uint8_t dev_id, uint8_t cmd, tm_oid_data_t * pt, tm_save_flag_t * t_save);
uint8_t tm_pdu_hub_data_ergodic(uint8_t dev_id, uint8_t cmd, tm_oid_data_t * pt, tm_save_flag_t * t_save);

#endif





