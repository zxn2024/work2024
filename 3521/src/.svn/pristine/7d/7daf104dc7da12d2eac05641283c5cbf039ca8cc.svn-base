/*************************************** Copyright ******************************
*
* (C) Copyright 2023, Shaanxi Tianji Communication
* All Rights Reserved
*                         
* FileName   : bsp_up_silence.h   
* Version    : none		
* Author     : 			
* Date       : 2023-11-13         
* Description: none   
*******************************************************************************/

#ifndef __BSP_UP_SILENCE_H_
#define __BSP_UP_SILENCE_H_
#include <stdint.h>
#include "system_config.h"


typedef struct
{
    uint8_t 	cnt;
    int16_t 	gate;       //门限
    RUN_STATE_E state;      //状态, 0: 正常, 1:静默
	
	uint8_t lna1;
	uint8_t lna2;
	uint8_t digit_att1;
	uint8_t digit_att2;
	uint8_t analog_att1;
	uint8_t analog_att2;
} SILENCE_PARA_T;

extern SILENCE_PARA_T t_silence_para[ RF_SYS_NUM ];	//

void silence_init(void);                    //初始化
void rf_silence(uint8_t sys);				//上行静默检测
RUN_STATE_E get_silence_state(uint8_t sys);		//获取上行静默状态

//void silence_in_set(uint8_t sys);           //进入静默状态设置

void silence_out_detection_sys1_callback(void);
void silence_out_detection_sys2_callback(void);
void silence_out_detection_sys3_callback(void);

#endif





