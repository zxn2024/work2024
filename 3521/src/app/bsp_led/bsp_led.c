/***************************** Copyright ****************************************
*
* (C) Copyright 2022, Shaanxi Tianji Communication
*  All Rights Reserved
*                         
* FileName   : bsp_led.c   
* Version    : none		
* Author     : ZhangWei			
* Date       : 2022-08-24         
* Description: *  
*   -----  vLedTimeSet()    定时事件,定时器中调用
*   -----  led_init()       LED初始化, 设备上电初始化调用
*   -----  led_in_task()           指示灯显示, 显示任务中调用  
*******************************************************************************/
#include "main.h"

#include "bsp_public_macro.h"
#include "bsp_public_drv.h"
#include "bsp_led_dev.h"
#include "bsp_led_drv.h"
#include "bsp_led.h"
#include "bsp_rf_ctr.h"
#include "bsp_monitor.h"
#include "bsp_l76k.h"

/*---------------------- 宏定义  ---------------------------------------*/
/* LED 闪烁状态定义 */
typedef enum
{
    LED_PROGRESS_WATER = 0,    //流水灯
    LED_PROGRESS_RUN,          //正常运行
    LED_PROGRESS_END           
} LED_MACH_E;


typedef struct
{
    int8_t   scTheoGain;   //理论增益
    int8_t   scMaxOutPut;  //最大输出功率
    uint8_t  scSync;       //同步状态 SYNC / UNSYNC
    uint8_t  ucRunState;   //运行状态, 正常 / 关断
    uint8_t  ucIso;        //隔离度
    int8_t   scPow;        //输出功率
}LedChPara_T;

typedef struct
{
    uint8_t  ucLedEn;       //开关LED使能, ENABLE / DISABLE
	uint8_t  ucLedDisMode;	//显示模式
    uint8_t  ucMode;        //运行状态, 开机 / 正常工作 / 测试模式
	uint8_t  ucSeverLink;   //服务器连接指示灯
    LedChPara_T tSysPara[RF_SYS_END];
} LedPara_T;


/*---------------------- 局部变量 ----------------------------------------*/

LedPara_T  tLedPara;    //LED获取的射频参数

static LED_MACH_E eLedProgressState = LED_PROGRESS_WATER;


/*---------------------- 声明 ----------------------------------------*/                    


                
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DRV ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/



/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DRV PUBLIC ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*****************************************************************************
* @Name	  : led_plan_select
* @Brief  : 根据设置参数选择LED显示方案
* @Param  : plan: [输入/出] 
* @Retval : 
* @Author : 
* @Data   : 2023-03-09
* @Note   :
*****************************************************************************/
static void led_plan_select( void )
{

}

/*****************************************************************************
* @Name	  : led_get_para
* @Brief  : 获取指示灯计算需要用到的参数
* @Param  : None
* @Retval : 
* @Author : 
* @Data   : 2022-08-24
* @Note   :
*****************************************************************************/
static void led_get_para( void )
{
    EqIndependentPara_T *pt = NULL;
    uint8_t eSys = 0;
    
    tLedPara.ucLedEn      = tDevPublicPara.ucLedSwitch;
	tLedPara.ucLedDisMode = tDevPublicPara.ucLedDisplayMode;
    tLedPara.ucMode       = tDevPublicPara.ucRunState;
    tLedPara.ucSeverLink  = (TRUE == getSockLinkState()) ? 1: 0;
    for( eSys=0; eSys<RF_SYS_END; eSys++ )
    {
        pt = &tRfChPara[eSys][RF_CH_DW];
        tLedPara.tSysPara[eSys].scTheoGain = pt->tGain.scTheoreticalGain;
        tLedPara.tSysPara[eSys].scMaxOutPut = pt->tPow.scTheoOutPow;
        tLedPara.tSysPara[eSys].scSync = SYNC;
        tLedPara.tSysPara[eSys].ucRunState = tRfSysPublic[eSys].ucSysState;
        tLedPara.tSysPara[eSys].ucIso = pt->ucIso;
        tLedPara.tSysPara[eSys].scPow = pt->tPow.scOutPow;
    }
    
 //   tLedPara.tSysPara[RF_SYS_2].scSync = tNetPara.sync;
	led_plan_select();
}



/*---------------------- 声明 ------------------------------------------*/



/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ HAL ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/



/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ OS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/



/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DRV ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/



/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ APP ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*****************************************************************************
* @Name	  : led_init
* @Brief  : led 初始化
* @Param  : None
* @Retval : 
* @Author : 
* @Data   : 2023-07-06
* @Note   :
*****************************************************************************/
void led_init( void )
{
	drv_led_init();
}

/*****************************************************************************
* @Name	  : led_cnt_in_time_isr
* @Brief  : led 指示灯计数器
* @Param  : None
* @Retval : 
* @Author : 
* @Data   : 2023-07-06
* @Note   :
*****************************************************************************/
void led_cnt_in_time_isr( void )
{
	drv_led_cnt_in_time_isr();
}

/*****************************************************************************
* @Name	  : led_iso_state_cal
* @Brief  : 
* @Param  : pt_led: [输入/出] 
**			t_led_ch_para: [输入/出] 
**			max_pow_offset: [输入/出] 
**		    iso_offset1: [输入/出] 
* @Retval : 
* @Author : 
* @Data   : 2023-04-21
* @Note   :
*****************************************************************************/
void led_iso_state_cal( Led_T *pt_led, LedChPara_T t_led_ch_para, uint8_t max_pow_offset, uint8_t iso_offset1 )
{
    LedChPara_T *pt = &t_led_ch_para;
    
        
    if ( pt->ucIso >= (pt->scTheoGain + 5 ) || pt->scPow > (pt->scMaxOutPut - max_pow_offset ))
    {
		#if ((LED_DISPLAY_PLAN == LED_PLAN_103) || (LED_DISPLAY_PLAN == LED_PLAN_84))
			pt_led->state = LED_STATE_OFF;
		#else
			pt_led->state = LED_STATE_ON;   
		#endif
    }
    else if( pt->ucIso >= (pt->scTheoGain - iso_offset1) )
    {
        pt_led->state = LED_STATE_QUICK_FLASH; 
    }
    else
    { 
		#if ((LED_DISPLAY_PLAN == LED_PLAN_103) || (LED_DISPLAY_PLAN == LED_PLAN_84))
			pt_led->state = LED_STATE_ON;
		#else
			pt_led->state = LED_STATE_SLOW_FLASH;
		#endif
    }
}

/*****************************************************************************
* @Name	  : led_pow_state_cal
* @Brief  : 功率灯状态计算
* @Param  : pt_led: 指示灯
*			t_led_ch_para: 通道参数
*			on_offset:     指示灯常亮实的功率偏移设置
*			quick_offset:  指示灯快闪偏移设置
*			slow_offset:   指示灯慢闪偏移设置
* @Retval : 
* @Author : 
* @Data   : 2022-10-05
* @Note   :
*****************************************************************************/
#if ((LED_DISPLAY_PLAN == LED_PLAN_86) || (LED_DISPLAY_PLAN == LED_PLAN_102))
static void led_pow_state_cal( Led_T *pt_led, LedChPara_T t_led_ch_para, int8_t on_offset, int8_t quick_offset, int8_t slow_offset )
{
    LedChPara_T *pt = &t_led_ch_para;
    
    if( (pt->ucRunState == CLOSE) || (pt->scSync == UNSYNC))
    {
        pt_led->state = LED_STATE_OFF;
        return;
    }
    
    if ( pt->scPow > (pt->scMaxOutPut - on_offset) )
    {
        pt_led->state = LED_STATE_ON;   
    }
    else if( pt->scPow > (pt->scMaxOutPut - quick_offset) )
    {
        pt_led->state = LED_STATE_QUICK_FLASH; 
    }
    else if( pt->scPow > (pt->scMaxOutPut - slow_offset) )
    { 
        pt_led->state = LED_STATE_SLOW_FLASH;  
    }
    else
    {
        pt_led->state = LED_STATE_OFF;   
    }
}
#endif

/*****************************************************************************
* @Name	  : vLedFlashStateChange
* @Brief  : 将功率灯的单灯闪烁状态转换为多灯指示状态
* @Param  :  pt_pow:
**			 pt_led_s1: [输入/出] 
**			 pt_led_s2: [输入/出] 
**			 pt_led_s3: [输入/出] 
**			 pt_led_s4: [输入/出] 
* @Retval : 
* @Author : 
* @Data   : 2023-04-04
* @Note   :
*****************************************************************************/
#if LED_DISPLAY_PLAN == LED_PLAN_103 
static void led_pow_state_cal( Led_T *pt_pow, LedChPara_T t_led_ch_para, int8_t led_value4, int8_t led_value3, int8_t led_value2, Led_T *pt_led_s1, Led_T *pt_led_s2, Led_T *pt_led_s3, Led_T *pt_led_s4  )
{
    LedChPara_T *pt = &t_led_ch_para;

    if( (pt->ucRunState == CLOSE) || (pt->scSync == UNSYNC))
    {
        pt_led_s1->state = LED_STATE_SLOW_FLASH;
		pt_led_s2->state = LED_STATE_SLOW_FLASH;
		pt_led_s3->state = LED_STATE_SLOW_FLASH;
		pt_led_s4->state = LED_STATE_SLOW_FLASH;
        return;
    }
    
    if ( pt->scPow > (pt->scMaxOutPut - led_value4) )
    {
        pt_led_s1->state = LED_STATE_ON;
		pt_led_s2->state = LED_STATE_ON;
		pt_led_s3->state = LED_STATE_ON;
		pt_led_s4->state = LED_STATE_ON;   
    }
    else if( pt->scPow > (pt->scMaxOutPut - led_value3) )
    {
        pt_led_s1->state = LED_STATE_ON;
		pt_led_s2->state = LED_STATE_ON;
		pt_led_s3->state = LED_STATE_ON;
		pt_led_s4->state = LED_STATE_OFF;
    }
    else if( pt->scPow > (pt->scMaxOutPut - led_value2) )
    { 
        pt_led_s1->state = LED_STATE_ON;
		pt_led_s2->state = LED_STATE_ON;
		pt_led_s3->state = LED_STATE_OFF;
		pt_led_s4->state = LED_STATE_OFF; 
    }
    else
    {
        pt_led_s1->state = LED_STATE_ON;
		pt_led_s2->state = LED_STATE_OFF;
		pt_led_s3->state = LED_STATE_OFF;
		pt_led_s4->state = LED_STATE_OFF;   
    }
}	


#elif LED_DISPLAY_PLAN == LED_PLAN_84
static void led_pow_state_cal( Led_T *pt_pow, LedChPara_T t_led_ch_para, int8_t led_value3, int8_t led_value2, Led_T *pt_led_s1, Led_T *pt_led_s2, Led_T *pt_led_s3)
{
    LedChPara_T *pt = &t_led_ch_para;

//    if( (pt->ucRunState == CLOSE) || (pt->scSync == UNSYNC))
//    {
//        pt_led_s1->state = LED_STATE_SLOW_FLASH;
//		pt_led_s2->state = LED_STATE_SLOW_FLASH;
//		pt_led_s3->state = LED_STATE_SLOW_FLASH;
//        return;
//    }
    
    if( pt->scPow > (pt->scMaxOutPut - led_value3) )
    {
        pt_led_s1->state = LED_STATE_ON;
		pt_led_s2->state = LED_STATE_ON;
		pt_led_s3->state = LED_STATE_ON;
    }
    else if( pt->scPow > (pt->scMaxOutPut - led_value2) )
    { 
        pt_led_s1->state = LED_STATE_ON;
		pt_led_s2->state = LED_STATE_ON;
		pt_led_s3->state = LED_STATE_OFF;
    }
    else
    {
        pt_led_s1->state = LED_STATE_ON;
		pt_led_s2->state = LED_STATE_OFF;
		pt_led_s3->state = LED_STATE_OFF;
    }
}

#elif LED_DISPLAY_PLAN == LED_PLAN_SHIP

#else

#endif


/*****************************************************************************
* @Name	  : led_display_state_cal
* @Brief  : 指示灯正确的显示状态计算
* @Param  : None
* @Retval : 
* @Author : 
* @Data   : 2022-08-24
* @Note   :
*****************************************************************************/
static void led_display_state_cal( void )
{
    uint8_t eSys = RF_SYS_1;

#if LED_DISPLAY_PLAN == LED_PLAN_86
		#if( LOCAL_MODULE_TYPE == MODULE_TYPE_MU ) 
			eSys = RF_SYS_1;
			led_iso_state_cal( ptSys1Iso,  tLedPara.tSysPara[eSys] , 4, 25 );
			led_pow_state_cal( ptSys1Pow,  tLedPara.tSysPara[eSys], 5, 10, 50 );

			eSys = RF_SYS_2;
			led_iso_state_cal( ptSys2Iso, tLedPara.tSysPara[eSys], 4, 25 );
			led_pow_state_cal( ptSys2Pow, tLedPara.tSysPara[eSys], 5, 10, 50 );

		#else
			eSys = RF_SYS_1;
			led_iso_state_cal( ptSys1Iso,  tLedPara.tSysPara[eSys] , 4, 25 );
			led_pow_state_cal( ptSys1Pow,  tLedPara.tSysPara[eSys], 5, 10, 50 );
			
			eSys = RF_SYS_2;
			led_iso_state_cal( ptSys2Iso, tLedPara.tSysPara[eSys], 4, 25 );
			led_pow_state_cal( ptSys2Pow, tLedPara.tSysPara[eSys], 5, 10, 50 );

		#endif
	
#elif LED_DISPLAY_PLAN == LED_PLAN_84
	static Led_T pt_pow_state;
	#if( LOCAL_MODULE_TYPE == MODULE_TYPE_MU ) 

		eSys = RF_SYS_1;
		led_iso_state_cal( ptSys1GA,  tLedPara.tSysPara[eSys] , 4, 25 );
		led_pow_state_cal( ptSys1S1,  tLedPara.tSysPara[eSys], 5, 10, ptSys1S1, ptSys1S2, ptSys1S3 );
		ptSys1GR->state = ( tLedPara.tSysPara[eSys].ucRunState == CLOSE ) ? LED_STATE_SLOW_FLASH : LED_STATE_ON;
		
		eSys = RF_SYS_2;
		led_iso_state_cal( ptSys2LA, tLedPara.tSysPara[eSys], 4, 25 );
		led_pow_state_cal( ptSys2S1,  tLedPara.tSysPara[eSys], 5, 10, ptSys2S1, ptSys2S2, ptSys2S3 );
		ptSys2LR->state = ( tLedPara.tSysPara[eSys].ucRunState == CLOSE ) ? LED_STATE_SLOW_FLASH : LED_STATE_ON;
		
		ptSys2LS->state = tLedPara.ucSeverLink ? LED_STATE_ON : LED_STATE_OFF;
		ptPower->state   = LED_STATE_ON;

	#else
	
		eSys = RF_SYS_1;
		led_iso_state_cal( ptSys1GA,  tLedPara.tSysPara[eSys] , 4, 25 );
		led_pow_state_cal( ptSys1S1,  tLedPara.tSysPara[eSys], 5, 10, ptSys1S1, ptSys1S2, ptSys1S3 );
		ptSys1GR->state = ( tLedPara.tSysPara[eSys].ucRunState == CLOSE ) ? LED_STATE_SLOW_FLASH : LED_STATE_ON;

		eSys = RF_SYS_2;
		led_iso_state_cal( ptSys2LA, tLedPara.tSysPara[eSys], 4, 25 );
		led_pow_state_cal( ptSys2S1,  tLedPara.tSysPara[eSys], 5, 10, ptSys2S1, ptSys2S2, ptSys2S3 );
		ptSys2LR->state = ( tLedPara.tSysPara[eSys].ucRunState == CLOSE ) ? LED_STATE_SLOW_FLASH : LED_STATE_ON;
		
		ptSys2LS->state = tLedPara.ucSeverLink ? LED_STATE_ON : LED_STATE_OFF;
		ptPower->state   = LED_STATE_ON;
		
	#endif

#elif LED_DISPLAY_PLAN == LED_PLAN_102
		#if( LOCAL_MODULE_TYPE == MODULE_TYPE_MU ) 
			eSys = RF_SYS_1;
			led_iso_state_cal( ptSys1Iso,  tLedPara.tSysPara[eSys] , 4, 25 );
			led_pow_state_cal( ptSys1Pow,  tLedPara.tSysPara[eSys], 5, 10, 50 );

			eSys = RF_SYS_2;
			led_iso_state_cal( ptSys2Iso, tLedPara.tSysPara[eSys], 4, 25 );
			led_pow_state_cal( ptSys2Pow, tLedPara.tSysPara[eSys], 5, 10, 50 );

		#else
			eSys = RF_SYS_1;
			led_iso_state_cal( ptSys1Iso,  tLedPara.tSysPara[eSys] , 4, 25 );
			led_pow_state_cal( ptSys1Pow,  tLedPara.tSysPara[eSys], 5, 10, 50 );
			
			eSys = RF_SYS_2;
			led_iso_state_cal( ptSys2Iso, tLedPara.tSysPara[eSys], 4, 25 );
			led_pow_state_cal( ptSys2Pow, tLedPara.tSysPara[eSys], 5, 10, 50 );

		#endif

#elif LED_DISPLAY_PLAN == LED_PLAN_103
	#if( LOCAL_MODULE_TYPE == MODULE_TYPE_MU ) 

		eSys = RF_SYS_1;
		led_iso_state_cal( ptSys1Alarm,  tLedPara.tSysPara[eSys] , 4, 25 );
		led_pow_state_cal( ptSys1S1,  tLedPara.tSysPara[eSys], 5, 10, 15, ptSys1S1, ptSys1S2, ptSys1S3, ptSys1S4 );
	
		eSys = RF_SYS_2;
		led_iso_state_cal( ptSys2Alarm, tLedPara.tSysPara[eSys], 4, 25 );
		led_pow_state_cal( ptSys2S1,  tLedPara.tSysPara[eSys], 5, 10, 15, ptSys2S1, ptSys2S2, ptSys2S3, ptSys2S4 );
		
		ptLedSync->state = tLedPara.ucSeverLink ? LED_STATE_ON : LED_STATE_OFF;
		ptPower->state   = LED_STATE_ON;

	#else
	
		eSys = RF_SYS_1;
		led_iso_state_cal( ptSys1Alarm,  tLedPara.tSysPara[eSys] , 4, 25 );
		led_pow_state_cal( ptSys1S1,  tLedPara.tSysPara[eSys], 5, 10, 15, ptSys1S1, ptSys1S2, ptSys1S3, ptSys1S4 );
	
		eSys = RF_SYS_2;
		led_iso_state_cal( ptSys2Alarm, tLedPara.tSysPara[eSys], 4, 25 );
		led_pow_state_cal( ptSys2S1,  tLedPara.tSysPara[eSys], 5, 10, 15, ptSys2S1, ptSys2S2, ptSys2S3, ptSys2S4 );
		
		ptLedSync->state = LED_STATE_OFF;
		ptPower->state   = LED_STATE_ON;	
	#endif

#elif LED_DISPLAY_PLAN == LED_PLAN_SHIP
		ptPow->state     = LED_STATE_ON;	//电源灯亮
		ptConnect->state = tLedPara.ucSeverLink ? LED_STATE_ON : LED_STATE_OFF;			//网络连接指示灯
		ptGps->state     = ( TRUE == get_gps_valid() ) ? LED_STATE_ON : LED_STATE_OFF;	//GPS数据是否有效
		
		//告警灯, 长亮:关断,   闪烁: 隔离度不足,    灭: 正常
		if( (CLOSE == tLedPara.tSysPara[eSys].ucRunState) || (tLedPara.tSysPara[eSys].ucIso < tRfSysPublic[eSys].ucIsoCloseGate ) )
			ptWarn->state = LED_STATE_ON;
		else if( tLedPara.tSysPara[eSys].ucIso <  ( tLedPara.tSysPara[eSys].scTheoGain + 5 ) )
			ptWarn->state = LED_STATE_SLOW_FLASH;
		else
			ptWarn->state = LED_STATE_OFF;
#else
	
#endif
}


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DRV ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*****************************************************************************
* @Name	  : led_display_in_normal
* @Brief  : 正常运行时的指示灯显示
* @Param  : None
* @Retval : 
* @Author : 
* @Data   : 2022-08-24
* @Note   :
*****************************************************************************/
static void led_display_in_normal( void )
{
   led_display_state_cal(); 
}

/*****************************************************************************
* @Name	  : led_in_task
* @Brief  : 
* @Param  : None
* @Retval : 
* @Author : 
* @Data   : 2022-08-24
* @Note   :
*****************************************************************************/
void led_in_task( void )
{
    led_get_para();
    led_display_state_cal();
    
	tLedPara.ucLedEn = ENABLE;
    if( DISABLE == tLedPara.ucLedEn )
    {
        drv_close_all_led();
        return;
    }
    
    switch( eLedProgressState )
    {
        case LED_PROGRESS_WATER:
                drv_led_water();
                if( TRUE == get_led_water_end_sign() )
                    eLedProgressState = LED_PROGRESS_RUN;
                break;
        
        case LED_PROGRESS_RUN:
            switch( tLedPara.ucMode )
            {
                case RUN_MODE_START:      //起始
                        break;

                case RUN_MODE_NORMAL:     //正常运行
                        led_display_in_normal();
                        break;
                
                case RUN_MODE_TEST_INIT:  //进入测试模式前的初始化
                case RUN_MODE_TEST:       //测试模式
                        drv_led_set_state_in_testmode();     
                        break;
            
                default:
                    break;
            }
            break;
            
        default:
                 eLedProgressState = LED_PROGRESS_WATER;
                 break;
    }
    
    drv_fresh_all_led_state();   //设置
}
