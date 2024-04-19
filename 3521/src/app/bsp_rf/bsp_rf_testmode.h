/*************************************** Copyright ******************************
*
* (C) Copyright 2022, Shaanxi Tianji Communication
* All Rights Reserved
*                         
* FileName   : bsp_rf_testmode.h   
* Version    : none		
* Author     : ZhangWei			
* Date       : 2022-09-14         
* Description: none   
*******************************************************************************/

#ifndef __BSP_RF_TESTMODE_H_
#define __BSP_RF_TESTMODE_H_

#include <stdint.h>

#define TM_CALI_GAIN_OFFSET_MAX    ( 3  )    
#define TM_CALI_GAIN_OFFSET_MIN    ( -3 )
#define TM_CALI_POWLIST_OFFSET_MAX ( 3  )
#define TM_CALI_POWLIST_OFFSET_MIN ( -3 )


void vTestModeEgpCerate(void);  //创建事件组
void vTestModePorcess( void );  //测试模式下的处理流程
uint8_t vTestModePduData( uint8_t rw, uint16_t uid, uint8_t *src );  //响应PDU指令

#endif





