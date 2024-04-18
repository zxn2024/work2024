/*************************************** Copyright ******************************
*
* (C) Copyright 2023, Shaanxi Tianji Communication
* All Rights Reserved
*                         
* FileName   : bsp_led_dev.h   
* Version    : none		
* Author     : ZhangWei			
* Date       : 2023-03-09         
* Description: none   
*******************************************************************************/
//
#ifndef __BSP_LED_DEV_H_
#define __BSP_LED_DEV_H_

#include <stdint.h>
#include "bsp_public_struct.h"
#include "bsp_dev_config.h"
#include "bsp_gpio_drv.h"

/* LED 闪烁状态定义 */
typedef enum
{
    LED_STATE_OFF = 0,        //灭
    LED_STATE_ON,             //亮 
    LED_STATE_SLOW_FLASH,     //慢闪
    LED_STATE_QUICK_FLASH,    //快闪
    LED_STATE_END
} LED_STATE_E;


typedef struct
{
    int8_t level;       //使用灯数量来显示分级时使用
    int8_t level_off;   //指示灯熄灭门限
    int8_t level_slow;  //指示灯慢闪门限
    int8_t level_quick; //指示灯快闪门限
    int8_t level_on;    //常亮门限
}LedLevel_T;


/*---------------------- 结构体 ----------------------------------------*/
typedef struct 
{
    LED_STATE_E state;      //指示灯状态
    Port_T      tPort;
}Led_T;

#define LED_NUM_MAX   ( 4 )

#define FROM_LEFT_TO_RIGHT	( 0 )
#define FROM_CENTER_TO_EDGE ( 1 )

//指示灯方案选择
#if LED_DISPLAY_PLAN == LED_PLAN_84
	#define LED_WATER_TYPE	 FROM_CENTER_TO_EDGE
#elif LED_DISPLAY_PLAN == LED_PLAN_86
	#define LED_WATER_TYPE	 FROM_LEFT_TO_RIGHT
#elif LED_DISPLAY_PLAN == LED_PLAN_102
	#define LED_WATER_TYPE	 FROM_LEFT_TO_RIGHT
#elif LED_DISPLAY_PLAN == LED_PLAN_103
	#define LED_WATER_TYPE	 FROM_CENTER_TO_EDGE
#endif

#if LED_DISPLAY_PLAN == LED_PLAN_84
	extern Led_T *ptSys2S3;	//
	extern Led_T *ptSys2S2;	//
	extern Led_T *ptSys2S1;	//
	extern Led_T *ptSys2LR;	//
	extern Led_T *ptSys2LA;	//
	extern Led_T *ptSys2LS;	//
	extern Led_T *ptPower; 	//
	extern Led_T *ptSys1GA;	//
	extern Led_T *ptSys1GR;	//
	extern Led_T *ptSys1S1;	//
	extern Led_T *ptSys1S2;	//
	extern Led_T *ptSys1S3;	//

#elif LED_DISPLAY_PLAN == LED_PLAN_86
	extern Led_T *ptSys1Pow;	//PB4
	extern Led_T *ptSys2Pow;	//PB3
	extern Led_T *ptSys1Iso;	//PB5
	extern Led_T *ptSys2Iso;	//PC8

#elif LED_DISPLAY_PLAN == LED_PLAN_102
	extern Led_T *ptSys1Pow;	//PB3
	extern Led_T *ptSys2Pow;	//PB4
	extern Led_T *ptSys1Iso;	//PC8
	extern Led_T *ptSys2Iso;	//PB5

#elif LED_DISPLAY_PLAN == LED_PLAN_103
	extern Led_T *ptLedSync;	//PC8
	extern Led_T *ptSys1S1;		//PC9
	extern Led_T *ptSys1S2;		//PD0
	extern Led_T *ptSys1S3;		//PD1
	extern Led_T *ptSys1S4;		//PD2
	extern Led_T *ptSys2S1;		//PD3
	extern Led_T *ptSys2S2;		//PD4
	extern Led_T *ptSys2S3;		//PD5
	extern Led_T *ptSys2S4;		//PD6
	extern Led_T *ptSys1Alarm;	//PB3
	extern Led_T *ptSys2Alarm;	//PB4
	extern Led_T *ptPower;	    //PB5

#elif LED_DISPLAY_PLAN == LED_PLAN_SHIP
	extern Led_T *ptWarn;		//
	extern Led_T *ptGps;		//
	extern Led_T *ptConnect;	//
	extern Led_T *ptPow;		//
#else

#endif

extern Led_T *tArrayLed[ LED_NUM_MAX ];	//

void dev_led_hal_init(void); 		//LED分配管脚
void dev_led_plan_init( void );	//根据方案进行指示灯重新映射
uint8_t get_dev_led_num( void );	//获取方案使用的指示灯数量
	
#endif





