/*************************************** Copyright ******************************
*
*           (C) Copyright 2022, Shaanxi Tianji Comunication
*                          All Rights Reserved
*                         
* FileName   : bsp_rf_lna.h   
* Version    : none		
* Author     : none			
* Date       : 2023-10-10         
* Description: none   
*******************************************************************************/

#ifndef __BSP_RF_LNA_H_
#define __BSP_RF_LNA_H_

#include "stdint.h"

void lna_pt_init( void );

void lna_set(uint8_t sys, uint8_t ch, uint8_t id, uint8_t state);

#endif


