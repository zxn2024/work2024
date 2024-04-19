/*************************************** Copyright ******************************
*
* (C) Copyright 2022, Shaanxi Tianji Communication
* All Rights Reserved
*                         
* FileName   : bsp_rf_iso.h   
* Version    : none		
* Author     : ZhangWei			
* Date       : 2022-11-21         
* Description: none   
*******************************************************************************/
//#include <stdint.h>
#ifndef __BSP_RF_ISO_H_
#define __BSP_RF_ISO_H_

#include "bsp_public_enum.h"
#include "bsp_dev_config.h"

#include "FreeRTOS.h"                   // ARM.FreeRTOS::RTOS:Core
#include "event_groups.h"               // ARM.FreeRTOS::RTOS:Event Groups


extern EventGroupHandle_t xIsoEgp;

/*---------------------- 结构体 ----------------------------------------*/
//#define BIT_ISO_RF_CLOSE     (1U << 9)   //开机90s内设置RF通道全衰状态
//#define BIT_ISO_SYS1_RESET_EN     (1U << 0)  //进行5G自激重新检测处理，一天最多3次
//#define BIT_ISO_SYS2_RESET_EN     (1U << 1)  //进行5G自激重新检测处理，一天最多3次
//#define BIT_ISO_SYS1_CLOSE_MIMO   (1U << 16)  //自激隔离度不足关断标记
//#define BIT_ISO_SYS2_CLOSE_MIMO   (1U << 17)  //自激隔离度不足关断标记


void vIncTick(void);
void vIsoInRfProgress( RF_SYS_E eSys, RF_CH_E eCh );
void vIsoEgpCreat(void);
void vIsoTimeAddInTimeIsr( void );



#endif






