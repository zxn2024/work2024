/*************************************** Copyright ******************************
*
* (C) Copyright 2022, Shaanxi Tianji Communication
* All Rights Reserved
*
* FileName   : bsp_testmode.h
* Version    : none
* Author     : ZhangWei
* Date       : 2022-09-14
* Description: none
*******************************************************************************/

#ifndef __BSP_TESTMODE_H_
#define __BSP_TESTMODE_H_

#include <stdint.h>
#include "bsp_testmode_define.h"
#include "public_struct.h"
#include "FreeRTOS.h"
#include "event_groups.h"

#define TM_PUBLIC_SET_SYS	( 1 << 0 )
#define TM_PUBLIC_SET_BAND	( 1 << 1 )


/*---------------------- 事件组  ---------------------------------------*/

extern EventGroupHandle_t egp_tm_public;
extern EventGroupHandle_t egp_tm_dev; 		//蓝牙
extern EventGroupHandle_t egp_tm_fpga; 		//FPGA
extern EventGroupHandle_t egp_tm_ecr8668; 	//ECR8668
extern EventGroupHandle_t egp_tm_rf; 		//射频
extern EventGroupHandle_t egp_tm_func; 		//功能
extern EventGroupHandle_t egp_tm_ble; 		//蓝牙

extern SlaveDevNum_T * pt_tm_slave_num;	//从机数量参数

#endif





