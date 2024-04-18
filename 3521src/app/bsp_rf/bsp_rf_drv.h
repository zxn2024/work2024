/*************************************** Copyright ******************************
*
* (C) Copyright 2023, Shaanxi Tianji Communication
* All Rights Reserved
*                         
* FileName   : bsp_rf_drv.h   
* Version    : none		
* Author     : 			
* Date       : 2023-05-11         
* Description: none   
*******************************************************************************/
//
#ifndef __BSP_RF_DRV_H_
#define __BSP_RF_DRV_H_

#include <stdint.h>
#include "bsp_gpio_drv.h"

//控制开关类结构体
typedef struct
{
    uint8_t  *state; 	//状态
    Port_T   tPort;		//端口
} CTRL_PORT_SWITCH_T;


void set_ctrl_pin_state_flag( CTRL_PORT_SWITCH_T *pt, uint8_t state );
void set_ctrl_pin( CTRL_PORT_SWITCH_T *pt );
	

#endif





