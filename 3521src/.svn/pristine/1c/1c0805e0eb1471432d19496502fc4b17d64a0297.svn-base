/***************************** Copyright ****************************************
*
* (C) Copyright 2023, Shaanxi Tianji Communication
*  All Rights Reserved
*                         
* FileName   : bsp_led_drv.c   
* Version    : none		
* Author     : ZhangWei			
* Date       : 2023-03-09         
* Description: none   
*******************************************************************************/
#include "main.h"

#include "bsp_public_enum.h"
#include "bsp_public_struct.h"
#include "bsp_public_macro.h"
#include "bsp_public_drv.h"
#include "bsp_led_dev.h"
#include "bsp_led_drv.h"


/*---------------------- 端口定义 --------------------------------------*/


/*---------------------- 宏定义  ---------------------------------------*/


/*---------------------- 结构体 ----------------------------------------*/
typedef enum
{
    LED_OFF = 0,
    LED_ON,
    LED_ONOFF_END
}LED_ONOFF_E;

typedef struct bsp_led
{
    LED_ONOFF_E slow;   //慢闪状态
    LED_ONOFF_E quick;  //快闪状态
} LedFlash_T;

typedef struct
{
    uint8_t init;     //是否完成初始化, 0:未完成, 1:已经完成
    uint8_t ucLedWaterId;   //流水灯时电路的灯编号.
    uint8_t end;      //流水灯结束标志, 0:未结束, 1:结束
}LedWater_T;


/*---------------------- 常量 ------------------------------------------*/


/*---------------------- 全局变量 --------------------------------------*/
static uint8_t g_led_num = LED_NUM_MAX;	//当前显示方案用到的指示灯数量

//uint8_t g_led_water_group_num = ( LED_NUM_MAX%2 ) ? ( LED_NUM_MAX+1 / 2 ) : ( LED_NUM_MAX / 2 );	//流水灯的分组数量

/*---------------------- 局部变量 --------------------------------------*/
LedFlash_T tLedFlash;   //快闪慢闪的状态
LedWater_T tLedWater = { .init = 0,
                         .ucLedWaterId = 0
                       };

/*---------------------- 声明 ------------------------------------------*/
void vLedTest( void );


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ HAL ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/





/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DRV ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*****************************************************************************
* @Name	  : drv_led_single_on_off
* @Brief  : 单个指示灯的亮灭
* @Param  : pt: 指示灯结构体 
*           state: ON: 亮,  OFF: 灭
* @Retval : 
* @Author : 
* @Data   : 2022-08-24
* @Note   :
*****************************************************************************/
static void drv_led_single_on_off(Led_T *pt, LED_ONOFF_E state)
{
    if ( state == LED_ON )
        vSetPin(&pt->tPort, LEVEL_L);
    else
        vSetPin(&pt->tPort, LEVEL_H);
}


/*****************************************************************************
* @Name	  : drv_led_set_onoff_by_state
* @Brief  : 根据指示灯状态设置指示灯
* @Param  : pt: 指示灯结构体 
* @Retval : 
* @Author : 
* @Data   : 2022-08-24
* @Note   :
*****************************************************************************/
static void drv_led_set_onoff_by_state( Led_T *pt )
{
    LED_ONOFF_E eState = LED_OFF;
    
    if( LED_STATE_OFF == pt->state )
    {
        eState = LED_OFF;
    }
    else if( LED_STATE_ON == pt->state )
    {
        eState = LED_ON;
    }
    else if( LED_STATE_SLOW_FLASH == pt->state )
    {
        eState = tLedFlash.slow ? LED_ON : LED_OFF;
    }
    else if( LED_STATE_QUICK_FLASH == pt->state )
    {
        eState = tLedFlash.quick ? LED_ON : LED_OFF;
    }
    else
    {}
        
    drv_led_single_on_off( pt, eState );
}


/*****************************************************************************
* @Name	  : drv_fresh_all_led_state
* @Brief  : 设置所有的指示灯状态
* @Param  : None
* @Retval : 
* @Author : 
* @Data   : 2022-10-05
* @Note   :
*****************************************************************************/
void drv_fresh_all_led_state( void )
{
    for( uint8_t i=0; i<g_led_num; i++ )
        drv_led_set_onoff_by_state( tArrayLed[i] );
}    

/*****************************************************************************
* @Name	  : drv_close_all_led
* @Brief  : 所有的灯均设置为灭的状态
* @Param  : None
* @Retval : 
* @Author : 
* @Data   : 2022-08-24
* @Note   :
*****************************************************************************/
void drv_close_all_led(void)
{
    for( uint8_t i=0; i<g_led_num; i++ )
        tArrayLed[i]->state = LED_STATE_OFF;

    drv_fresh_all_led_state();
}


/*****************************************************************************
* @Name	  : drv_led_water_init
* @Brief  : led流水灯初始化
* @Param  : None
* @Retval : 
* @Author : 
* @Data   : 2022-10-05
* @Note   :
*****************************************************************************/
void drv_led_water_init( void )
{
    drv_close_all_led();
    tLedWater.init = 0;
    tLedWater.ucLedWaterId = 0;
}

/*****************************************************************************
* @Name	  : drv_led_water
* @Brief  : 流水灯
* @Param  : None
* @Retval : 
* @Author : 
* @Data   : 2022-08-24
* @Note   :
*****************************************************************************/
void drv_led_water( void )
{
    if( tLedWater.init == 0 )
    {
        drv_led_water_init();
        tLedWater.init = 1;
        tLedWater.end = 0;
    }

#if LED_WATER_TYPE == FROM_LEFT_TO_RIGHT
	
    //流水方式: 顺次亮一个指示灯
    drv_close_all_led(); 
	
    if( tLedWater.ucLedWaterId >= g_led_num )
        tLedWater.ucLedWaterId = 0;
    
    tArrayLed[tLedWater.ucLedWaterId]->state = LED_STATE_ON;
    drv_led_set_onoff_by_state( tArrayLed[ tLedWater.ucLedWaterId ] );
	
#else
	
	//流水灯方式: 从中间向两边
	uint8_t led_cyc_num = ( g_led_num % 2 ) ? (g_led_num+1)/2 : (g_led_num/2);

	if( tLedWater.ucLedWaterId >= led_cyc_num )
	{
		tLedWater.ucLedWaterId = 0;
		drv_close_all_led();
	}

	if( g_led_num % 2 )
	{
		tArrayLed[ (g_led_num-1)/2 + tLedWater.ucLedWaterId ]->state = LED_STATE_ON;
		tArrayLed[ (g_led_num-1)/2 - tLedWater.ucLedWaterId ]->state = LED_STATE_ON;
		drv_led_set_onoff_by_state( tArrayLed[ (g_led_num-1)/2 + tLedWater.ucLedWaterId ] );
		drv_led_set_onoff_by_state( tArrayLed[ (g_led_num-1)/2 - tLedWater.ucLedWaterId ] );
	}
	else
	{
		tArrayLed[ g_led_num/2 + tLedWater.ucLedWaterId ]->state = LED_STATE_ON;
		tArrayLed[ g_led_num/2 - tLedWater.ucLedWaterId - 1 ]->state = LED_STATE_ON;
		drv_led_set_onoff_by_state( tArrayLed[ g_led_num/2 + tLedWater.ucLedWaterId ] );
		drv_led_set_onoff_by_state( tArrayLed[ g_led_num/2 - tLedWater.ucLedWaterId - 1 ] );
	}
#endif

}


/*****************************************************************************
* @Name	  : drv_led_set_state_in_testmode
* @Brief  : 测试模式指示灯显示, 所有的灯1s闪烁
* @Param  : None
* @Retval : 
* @Author : 
* @Data   : 2022-08-24
* @Note   :
*****************************************************************************/
void drv_led_set_state_in_testmode( void )
{
    for( uint8_t i=0; i<g_led_num; i++ )
    {
        tArrayLed[i]->state = LED_STATE_SLOW_FLASH;
    }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ APP ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ OS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*****************************************************************************
* @Name	  : drv_led_cnt_in_time_isr
* @Brief  : 定时器状态设置, 只能在在定时器中断中调用
* @Param  : None
* @Retval : 
* @Author : 
* @Data   : 2022-10-05
* @Note   :
*****************************************************************************/
void drv_led_cnt_in_time_isr(void)
{
    /* 定时器计数 */
    #define LED_QUICK_CNT   ( 300 / TIMER_PERIOD_TICKS )   //快闪间隔300ms
    #define LED_SLOW_CNT    ( 1000 / TIMER_PERIOD_TICKS )  //慢闪间隔1000ms
    #define LED_WATER_CNT   ( 500 / TIMER_PERIOD_TICKS )   //流水灯间隔500ms

    #define LED_WATER_LENGTH_TIME   ( g_led_num * 3 * 500 / TIMER_PERIOD_TICKS )   //流水灯持续时间
    
    static uint16_t usLedQuickFlashCnt = 0;
    static uint16_t usLedSlowFlashCnt = 0;
    static uint16_t usLedWaterCnt = 0;
    static uint16_t usLedWaterLengthTime = 0;   //指示灯持续时间

        /* 快闪计数器 */
    if(++usLedQuickFlashCnt >= LED_QUICK_CNT)
    {
        usLedQuickFlashCnt = 0;
        tLedFlash.quick = ( tLedFlash.quick == LED_ON ) ? LED_OFF : LED_ON;
    }

    /* 慢闪计数器 */
    if(++usLedSlowFlashCnt >= LED_SLOW_CNT)
    {
        usLedSlowFlashCnt = 0;
        tLedFlash.slow = ( tLedFlash.slow == LED_ON ) ? LED_OFF : LED_ON;
    }

    /* 流水灯计数器 */
    if(usLedWaterCnt++ >= LED_WATER_CNT )
    {
        usLedWaterCnt = 0;
        tLedWater.ucLedWaterId++;
        tLedWater.ucLedWaterId %= g_led_num;
    }
    
    /* 流水灯时长计数器 */
    if(usLedWaterLengthTime <= LED_WATER_LENGTH_TIME )
    {
        tLedWater.end = 0;
        usLedWaterLengthTime++;
    }
    else
        tLedWater.end = 1;
}


/*****************************************************************************
* @Name	  : drv_led_init
* @Brief  : 状态初始化, 包括寄存器, 初始状态
* @Param  : None
* @Retval : 
* @Author : 
* @Data   : 2022-08-24
* @Note   :
*****************************************************************************/
void drv_led_init(void)
{
    dev_led_hal_init();
	dev_led_plan_init();
	g_led_num = get_dev_led_num();
	
    drv_led_water_init();
	
    vLedTest();
}


/*****************************************************************************
* @Name	  : get_led_water_end_sign
* @Brief  : 获取流水灯结束标识
* @Param  : None
* @Retval : TRUE: 结束,  FALSE:未结束
* @Author : 
* @Data   : 2023-03-09
* @Note   :
*****************************************************************************/
uint8_t get_led_water_end_sign( void )
{
	if( tLedWater.end )
		return TRUE;
	else
		return FALSE;
}

/*****************************************************************************
* @Name	  : vLedTest
* @Brief  : 
* @Param  : None
* @Retval : 
* @Author : 
* @Data   : 2023-04-21
* @Note   :
*****************************************************************************/
void vLedTest( void )
{
//	uint8_t i = 0;
//	drv_close_all_led();
//	
//	for( i=0; i< g_led_num; i++)
//	{
//		tArrayLed[i]->state = LED_STATE_ON;
//		drv_led_set_onoff_by_state( tArrayLed[i] );
//		tArrayLed[i]->state = LED_STATE_OFF;
//		drv_led_set_onoff_by_state( tArrayLed[i] );
//	}
}
