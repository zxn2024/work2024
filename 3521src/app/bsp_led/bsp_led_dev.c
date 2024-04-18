/***************************** Copyright ****************************************
*
* (C) Copyright 2023, Shaanxi Tianji Communication
*  All Rights Reserved
*                         
* FileName   : bsp_led_dev.c   
* Version    : none		
* Author     : ZhangWei			
* Date       : 2023-03-09         
* Description: none   
*******************************************************************************/
#include "main.h"

#include "bsp_public_enum.h"
#include "bsp_public_struct.h"
#include "bsp_public_macro.h"

#include "bsp_led_dev.h"
#include "bsp_dev_config.h"

#define LED_PLAN( n )	( n )	//指示灯方案


/*---------------------- 端口定义 --------------------------------------*/

#define LED01_PORT	GPIOB		//
#define LED01_PIN	GPIO_PIN_12	
#define LED02_PORT	GPIOB		//
#define LED02_PIN	GPIO_PIN_13	
#define LED03_PORT	GPIOB		//
#define LED03_PIN	GPIO_PIN_14	
#define LED04_PORT	GPIOB		//
#define LED04_PIN	GPIO_PIN_15	
//#define LED05_PORT	GPIOB		//
//#define LED05_PIN	GPIO_PIN_3	
//#define LED06_PORT	GPIOD		//
//#define LED06_PIN	GPIO_PIN_0	
//#define LED07_PORT	GPIOD		//
//#define LED07_PIN	GPIO_PIN_4	
//#define LED08_PORT	GPIOB		//
//#define LED08_PIN	GPIO_PIN_5	
//#define LED09_PORT	GPIOD		//
//#define LED09_PIN	GPIO_PIN_6	
//#define LED10_PORT	GPIOD		//
//#define LED10_PIN	GPIO_PIN_1	
//#define LED11_PORT	GPIOC		//
//#define LED11_PIN	GPIO_PIN_9	
//#define LED12_PORT	GPIOD		//
//#define LED12_PIN	GPIO_PIN_5	


/*---------------------- 常量 ------------------------------------------*/


/*---------------------- 全局变量 --------------------------------------*/


/*---------------------- 局部变量 --------------------------------------*/
Led_T tLed01, tLed02, tLed03, tLed04;	//, tLed05, tLed06;
//Led_T tLed07, tLed08, tLed09, tLed10, tLed11, tLed12;

Led_T *tArrayLed[ LED_NUM_MAX ];

//LedLevel_T tLevelSys1Iso, tLevelSys2Iso, tLevelSys1Pow, tLevelSys2Pow; 

//LedLevel_T *tArrayLevel[] = { &tLevelSys1Iso,  //L1
//							  &tLevelSys2Iso, //L2
//							  &tLevelSys1Pow,  //L3
//							  &tLevelSys2Pow  //L4
//						    };

							
/*******  指示灯显示方案1  *******/
#if LED_DISPLAY_PLAN == LED_PLAN_84
Led_T *ptSys2S3    = NULL;	//
Led_T *ptSys2S2    = NULL;	//
Led_T *ptSys2S1    = NULL;	//
Led_T *ptSys2LR    = NULL;	//
Led_T *ptSys2LA    = NULL;	//
Led_T *ptSys2LS    = NULL;	//
Led_T *ptPower     = NULL;	//
Led_T *ptSys1GA    = NULL;	//
Led_T *ptSys1GR    = NULL;	//
Led_T *ptSys1S1    = NULL;	//
Led_T *ptSys1S2    = NULL;	//
Led_T *ptSys1S3    = NULL;	//

#elif LED_DISPLAY_PLAN == LED_PLAN_86

Led_T *ptSys1Pow    = NULL;	//PB4
Led_T *ptSys2Pow    = NULL;	//PB3
Led_T *ptSys1Iso    = NULL;	//PB5
Led_T *ptSys2Iso    = NULL;	//PC8

#elif LED_DISPLAY_PLAN == LED_PLAN_102
Led_T *ptSys1Pow    = NULL;	//PB3
Led_T *ptSys2Pow    = NULL;	//PB4
Led_T *ptSys1Iso    = NULL;	//PC8
Led_T *ptSys2Iso    = NULL;	//PB5

#elif LED_DISPLAY_PLAN == LED_PLAN_103
Led_T *ptLedSync   = NULL;	//
Led_T *ptSys1S1    = NULL;	//
Led_T *ptSys1S2    = NULL;	//
Led_T *ptSys1S3    = NULL;	//
Led_T *ptSys1S4    = NULL;	//
Led_T *ptSys2S1    = NULL;	//
Led_T *ptSys2S2    = NULL;	//
Led_T *ptSys2S3    = NULL;	//
Led_T *ptSys2S4    = NULL;	//
Led_T *ptSys1Alarm = NULL;	//
Led_T *ptSys2Alarm = NULL;	//
Led_T *ptPower     = NULL;	//

#elif LED_DISPLAY_PLAN == LED_PLAN_SHIP
Led_T *ptWarn      = NULL;	//
Led_T *ptGps       = NULL;	//
Led_T *ptConnect   = NULL;	//
Led_T *ptPow       = NULL;	//
#else

#endif



/*---------------------- 宏定义  ---------------------------------------*/


/*---------------------- 结构体 ----------------------------------------*/


/*---------------------- 常量 ------------------------------------------*/


/*---------------------- 全局变量 --------------------------------------*/


/*---------------------- 局部变量 --------------------------------------*/
static uint8_t led_num = 0;	//指示灯数量

/*---------------------- 声明 ------------------------------------------*/



/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ HAL ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/





/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DRV ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/





/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ APP ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*****************************************************************************
* @Name   : dev_led_gpio_init
* @Brief  : 
* @Param  : tLed: 指示灯结构体
*   port: 端口
*   pin:  管脚 
*   state: 指示灯状态 
* @Retval : 
* @Author : 
* @Data   : 2022-08-29
* @Note   :
*****************************************************************************/
static void dev_led_gpio_init( Led_T *pt, port_typedef port, pin_typedef pin, LED_STATE_E state )
{
	pt->tPort.port = port;
	pt->tPort.pin  = pin;
	pt->state      = state;
}

/*****************************************************************************
* @Name	  : dev_led_hal_init
* @Brief  : LED分配管脚
* @Param  : None
* @Retval : 
* @Author : 
* @Data   : 2023-03-09
* @Note   :
*****************************************************************************/
void dev_led_hal_init(void) 
{   
    dev_led_gpio_init( &tLed01,  LED01_PORT,  LED01_PIN,  LED_STATE_OFF );     
    dev_led_gpio_init( &tLed02,  LED02_PORT,  LED02_PIN,  LED_STATE_OFF );
	dev_led_gpio_init( &tLed03,  LED03_PORT,  LED03_PIN,  LED_STATE_OFF );
	dev_led_gpio_init( &tLed04,  LED04_PORT,  LED04_PIN,  LED_STATE_OFF );
//	dev_led_gpio_init( &tLed05,  LED05_PORT,  LED05_PIN,  LED_STATE_OFF );
//	dev_led_gpio_init( &tLed06,  LED06_PORT,  LED06_PIN,  LED_STATE_OFF );
//	dev_led_gpio_init( &tLed07,  LED07_PORT,  LED07_PIN,  LED_STATE_OFF );
//	dev_led_gpio_init( &tLed08,  LED08_PORT,  LED08_PIN,  LED_STATE_OFF );
//	dev_led_gpio_init( &tLed09,  LED09_PORT,  LED09_PIN,  LED_STATE_OFF );
//	dev_led_gpio_init( &tLed10,  LED10_PORT,  LED10_PIN,  LED_STATE_OFF );
//	dev_led_gpio_init( &tLed11,  LED11_PORT,  LED11_PIN,  LED_STATE_OFF );
//	dev_led_gpio_init( &tLed12,  LED12_PORT,  LED12_PIN,  LED_STATE_OFF );
}


/*****************************************************************************
* @Name	  : dev_led_plan_init
* @Brief  : 当前方案的指示灯重新映射
* @Param  : None
* @Retval : 
* @Author : 
* @Data   : 2023-03-09
* @Note   :
*****************************************************************************/
void dev_led_plan_init( void )
{
#if LED_DISPLAY_PLAN == LED_PLAN_103
	led_num     = 12;
	
    ptLedSync   = &tLed07;	//PC8
    ptSys1S1    = &tLed04;	//PC9
    ptSys1S2    = &tLed03;	//PD0
    ptSys1S3    = &tLed02;	//PD1
    ptSys1S4    = &tLed01;	//PD2
    ptSys2S1    = &tLed09;	//PD3
    ptSys2S2    = &tLed10;	//PD4
    ptSys2S3    = &tLed11;	//PD5
    ptSys2S4    = &tLed12;	//PD6
    ptSys1Alarm = &tLed05;	//PB3
    ptSys2Alarm = &tLed08;	//PB4
    ptPower     = &tLed06;	//PB5
	
	tArrayLed[0]  = ptSys1S4;  //用于流水灯的点亮顺序
	tArrayLed[1]  = ptSys1S3;
	tArrayLed[2]  = ptSys1S2;
	tArrayLed[3]  = ptSys1S1;
	tArrayLed[4]  = ptSys1Alarm;
	tArrayLed[5]  = ptPower;
	tArrayLed[6]  = ptLedSync;
	tArrayLed[7]  = ptSys2Alarm;
	tArrayLed[8]  = ptSys2S1;
	tArrayLed[9]  = ptSys2S2;
	tArrayLed[10] = ptSys2S3;
	tArrayLed[11] = ptSys2S4;
	
#elif LED_DISPLAY_PLAN == LED_PLAN_86
	led_num     = 4;
	
	ptSys1Iso   = &tLed01;	//PB4
	ptSys2Iso   = &tLed05;	//PB3
	ptSys1Pow   = &tLed08;	//PB5
	ptSys2Pow   = &tLed02;	//PC8
	
	tArrayLed[0]  = ptSys1Iso;  //用于流水灯的点亮顺序
	tArrayLed[1]  = ptSys2Iso;
	tArrayLed[2]  = ptSys1Pow;
	tArrayLed[3]  = ptSys2Pow;
	
	for( uint8_t i=4; i<=11; i++ )
		tArrayLed[i]  = NULL;
	
#elif LED_DISPLAY_PLAN == LED_PLAN_84
	led_num     = 12;
	
	ptSys2S3    = &tLed01;	
	ptSys2S2    = &tLed02;	
	ptSys2S1    = &tLed03;	
	ptSys2LR    = &tLed04;
	ptSys2LA    = &tLed05;
	ptSys2LS    = &tLed06;
	ptPower     = &tLed07;
	ptSys1GA    = &tLed08;
	ptSys1GR    = &tLed09;
	ptSys1S1    = &tLed10;
	ptSys1S2    = &tLed11;
	ptSys1S3    = &tLed12;

	tArrayLed[0]  = ptSys2S3;  //用于流水灯的点亮顺序
	tArrayLed[1]  = ptSys2S2;
	tArrayLed[2]  = ptSys2S1;
	tArrayLed[3]  = ptSys2LR;
	tArrayLed[4]  = ptSys2LA;
	tArrayLed[5]  = ptSys2LS;
	tArrayLed[6]  = ptPower;
	tArrayLed[7]  = ptSys1GA;
	tArrayLed[8]  = ptSys1GR;
	tArrayLed[9]  = ptSys1S1;
	tArrayLed[10] = ptSys1S2;
	tArrayLed[11] = ptSys1S3;
	
#elif LED_DISPLAY_PLAN == LED_PLAN_102
	led_num     = 4;
	
	ptSys2Pow   = &tLed01;	//PB4
	ptSys1Pow   = &tLed05;	//PB3
	ptSys2Iso   = &tLed08;	//PB5
	ptSys1Iso   = &tLed02;	//PC8
	
	tArrayLed[0]  = ptSys2Pow;  //用于流水灯的点亮顺序
	tArrayLed[1]  = ptSys1Pow;
	tArrayLed[2]  = ptSys2Iso;
	tArrayLed[3]  = ptSys1Iso;
	
	for( uint8_t i=4; i<=11; i++ )
		tArrayLed[i]  = NULL;

#elif LED_DISPLAY_PLAN == LED_PLAN_SHIP
	led_num     = 4;
	
	ptWarn    = &tLed01;	
	ptGps     = &tLed02;	
	ptConnect = &tLed03;	
	ptPow     = &tLed04;	
	
	tArrayLed[0]  = ptWarn;  	//用于流水灯的点亮顺序
	tArrayLed[1]  = ptGps;
	tArrayLed[2]  = ptConnect;
	tArrayLed[3]  = ptPow;
	
	for( uint8_t i=4; i<=11; i++ )
		tArrayLed[i]  = NULL;

#else

#endif
	//关闭多余的指示灯
	
}

/*****************************************************************************
* @Name	  : get_dev_led_num
* @Brief  : 获取当前方案的指示灯数量
* @Param  : None
* @Retval : 
* @Author : 
* @Data   : 2023-03-09
* @Note   :
*****************************************************************************/
uint8_t get_dev_led_num( void )
{
	return led_num;
}
