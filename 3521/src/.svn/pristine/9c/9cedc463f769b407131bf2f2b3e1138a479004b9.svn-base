/************************************************************************
* Copyright (c) 2024 Shaanxi Tianji Communication Technologies Co., Ltd.
* 
* @file     led.c
* @author   
* @version  
* @date     2024-03-13 
* @brief
* 
************************************************************************/
#include "main.h"
//#include "stdlib.h"
//#include "stdio.h"
//#include "string.h"
#include "public_enum.h"
//#include "bsp_public_struct.h"
//#include "bsp_public_macro.h"
//#include "bsp_public_drv.h"
//#include "bsp_dev_config.h"
#include "bsp_config.h"
//#include "led_display.h"
#include "bsp_rf_global.h"
#include "bsp_warn_user.h"


/* PLAN_A
丝印	名称		颜色	状态			含义
RUN		运行指示	绿色	慢闪（1s）		系统启动中
							常亮			系统启动完成
ALM		告警指示	红色	常亮			有告警；或大功率关断；或自激关断
							慢闪（1s）		有自激
							常灭			无告警
PWR		功率指示	绿色	常亮			4G或5G输出功率≥Pmax-10
							慢闪（1s）		4G和5G输出功率均<Pmax-10
SYN		同步指示	绿色	常亮			5G和4G TDD均同步；或5G同步，4G为FDD
							慢闪（1s）		5G同步，4G TDD未同步
							快闪（0.3s）	5G未同步，4G TDD同步；或5G未同步，4G为FDD
							常灭			5G和4G TDD均未同步

*/

/* PLAN_B
丝印	名称		颜色	状态		含义
RUN		运行指示	绿色	慢闪（1s）	系统启动中
							常亮		系统启动完成
ALM		告警指示	红色	常亮		有告警；或大功率关断
							常灭		无告警
PWR		功率指示	绿色	常亮		4G或5G输出功率≥Pmax-10
							慢闪（1s）	4G和5G输出功率均<Pmax-10
4GSYN	4G同步指示	绿色	常亮		4G TDD同步
							慢闪（1s）	4G TDD未同步
							常灭		4G为FDD
5GSYN	5G同步指示	绿色	常亮		5G TDD同步；
							慢闪（1s）	5G TDD未同步
ISO		隔离度指示	红色	常亮		自激关断
							慢闪（1s）	有自激
							常灭		无自激
*/

/* PLAN_C
灯名称		说明		颜色	状态		定义
LED_WARN	告警指示		红色	常亮		设备有告警
								灭			无告警
								0.2Hz闪烁	设备自激
LED_POW		5G功率指示	绿色	常亮		5G通道输出功率正常
								灭			5G通道输出功率异常
LED_5GSYNC	5G同步状态	绿色	常亮		5G同步
											0.2Hz闪烁	5G未同步
LED_RUN		运行状态		绿色	1Hz闪烁		系统正常运行
											0.5Hz闪烁	系统启动中
LED_SELF	4G功率指示	绿色	常亮		4G通道输出功率正常
											灭	4G通道输出功率异常
LED_4GSYNC	4G同步状态	绿色	常亮		4G同步
								0.2Hz闪烁	4G是TDD，未同步
								灭			4G是FDD
*/


/*---------------------- 端口定义 --------------------------------------*/
#if(PRODUCT_MODEL == CMDT3521A)
	#define FLASH_SLOW	( 1000 )
	#define FLASH_QUICK ( 300  )

	LED_t *pt_led_run  = &led_pd0;
	LED_t *pt_led_alm  = &led_pg8;
	LED_t *pt_led_pwr  = &led_pc9;
	LED_t *pt_led_sync = &led_pd1;

#elif(PRODUCT_MODEL == CMDT3521B)
	#define FLASH_SLOW	( 1000 )
	#define FLASH_QUICK ( 1000  )

	LED_t *pt_led_run     = &led_pg8;
	LED_t *pt_led_alm     = &led_pc9;
	LED_t *pt_led_pwr     = &led_c11;
	LED_t *pt_led_4g_sync = &led_pd0;
	LED_t *pt_led_5g_sync = &led_pd1;
	LED_t *pt_led_iso     = &led_pd2;

#else
	#define FLASH_SLOW			( 2000  )
	#define FLASH_QUICK 	    ( 1000  )
	#define FLAHS_SUPER_SLOW	( 5000 )

	LED_t *pt_led_run     = &led_pd0;
	LED_t *pt_led_alm     = &led_pg8;
	LED_t *pt_led_5g_pwr  = &led_pd0;
	LED_t *pt_led_5g_sync = &led_pd1;
	LED_t *pt_led_4g_pwr  = &led_pd2;
	LED_t *pt_led_4g_sync = &led_c11;
	
#endif

/*---------------------- 宏定义  ---------------------------------------*/


/*---------------------- 结构体 ----------------------------------------*/
typedef struct
{
	uint8_t 	  tdd_or_fdd;	//TDD OR FDD
	uint8_t 	  run_state;	//运行状态
	uint8_t 	  sync;		//同步状态
	int8_t  	  pow;		//功率
	WARN_STATE_E  warn;		//告警
	uint8_t 	  iso;		//隔离度
	uint8_t 	  theo_gain;	//理论增益
	uint8_t 	  real_gain;	//实际增益
	int8_t  	  theo_pow;	//理论输出功率
	int8_t  	  rsrp;		//rsrp
	uint8_t 	  iso_close;	//隔离度关断
	uint8_t 	  pow_close;	//大功率关断
}led_para_t;


/*---------------------- 常量 ------------------------------------------*/


/*---------------------- 全局变量 --------------------------------------*/


/*---------------------- 局部变量 --------------------------------------*/
led_para_t t_para[3];	//获取运行状态

led_para_t *pt_4g 	= &t_para[0];
led_para_t *pt_nr 	= &t_para[1];
led_para_t *pt_mimo = &t_para[2];

uint8_t    led_work_mode = WORK_NORMAL;

/*---------------------- 声明 ------------------------------------------*/



/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ HAL ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/



/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DRV ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/



/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ APP ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

WARN_STATE_E led_warn_state( RF_SYS_E sys )
{
	dev_warn_para_t  * pt = &t_dev_warn[ 0 ];	
	if( WARN == pt->t_warn_ch1_dw_inpow_greater.ucWarnState || 
		WARN == pt->t_warn_ch1_dw_inpow_less.ucWarnState    ||
		WARN == pt->t_warn_ch1_dw_inpow_greater.ucWarnState || 
		WARN == pt->t_warn_ch1_dw_inpow_less.ucWarnState    ||
		WARN == pt->t_warn_ch1_dw_inpow_greater.ucWarnState || 
	    WARN == pt->t_warn_ch1_dw_inpow_less.ucWarnState )
	{
		return WARN;
	}
	else
		return NORMAL;
	
}




/**
* @brief   获取 LED 显示用参数   
* @param   None
* @return    
* @data    2024-03-13
* @note    
*/
static void get_led_para( void )
{
	
	 RfSysPublic_T 	*pt_pub = NULL;
     RfChPara_T     *pt_ch  = NULL;
	 led_para_t     *pt_led[3] = {  pt_4g,
									pt_nr,
									pt_mimo
								 };
	 
	led_work_mode = tDevPublicPara.ucWorkMode;
	if( WORK_CALIB == led_work_mode )
		return;
							
	for( uint8_t sys=0; sys<RF_SYS_NUM; sys++ )
	{
		if( DEV_MIMO_EXIST == 0 )
		{
			pt_mimo = &t_para[1];
		}
		
		pt_pub = &t_rf_sys[sys].t_public;
		pt_ch  = &t_rf_sys[sys].t_ch[RF_CH_DW];
		
		pt_led[sys]->tdd_or_fdd = pt_pub->SysMode;					//TDD OR FDD
		pt_led[sys]->run_state  = pt_pub->openState; 				//运行状态
		pt_led[sys]->sync 	  	= pt_pub->sync;						//同步状态
		pt_led[sys]->pow 		= pt_ch->tPow.scOutPow;;			//功率
		pt_led[sys]->theo_gain  = pt_ch->tGain.scTheoreticalGain;	//理论增益
		pt_led[sys]->real_gain  = pt_ch->tGain.scRealGain;;			//实际增益
		pt_led[sys]->rsrp 	    = 0;								//rsrp
		
		if( B_TRUE == pt_led[sys]->run_state )	//开机完成
		{
			pt_led[sys]->tdd_or_fdd = pt_pub->SysMode;					//TDD OR FDD
			pt_led[sys]->sync 	  	= pt_pub->sync;						//同步状态
			pt_led[sys]->pow 		= pt_ch->tPow.scOutPow;;			//功率
			pt_led[sys]->theo_gain  = pt_ch->tGain.scTheoreticalGain;	//理论增益
			pt_led[sys]->real_gain  = pt_ch->tGain.scRealGain;;			//实际增益
			pt_led[sys]->rsrp 	    = 0;								//rsrp
			pt_led[sys]->warn 	    = led_warn_state((RF_SYS_E)sys);	//告警
			pt_led[sys]->iso 		= pt_ch->tIso.iso;					//隔离度	
			pt_led[sys]->iso_close  = (CLOSE_ISO == pt_pub->closeReason) ? 1 : 0;		//隔离度关断						//隔离度关断
			pt_led[sys]->pow_close  = (CLOSE_POW_OVER == pt_pub->closeReason) ? 1 : 0;	//过功率关断							//过功率关断
		}
		else
		{
			pt_led[sys]->tdd_or_fdd = IS_TDD;	//TDD OR FDD
			pt_led[sys]->sync 	  	= UNSYNC;	//同步状态
			pt_led[sys]->pow 		= -80;		//功率
			pt_led[sys]->theo_gain  = 65;		//理论增益
			pt_led[sys]->real_gain  = 65;		//实际增益
			pt_led[sys]->rsrp 	    = 0;		//rsrp
			pt_led[sys]->warn 	    = NORMAL;	//告警
			pt_led[sys]->iso 		= 80;		//隔离度	
			pt_led[sys]->iso_close  = 0;		//隔离度关断
			pt_led[sys]->pow_close  = 0;		//过功率关断
		}
	}
}


/**
* @brief   设置指示灯状态  
* @param   None
* @return    
* @data    2024-03-13
* @note    
*/
static void led_run( void )
{
	#if(PRODUCT_MODEL == CMDT3521A)	
		BI_LED_RUN( pt_led_run );
		BI_LED_RUN( pt_led_alm );
		BI_LED_RUN( pt_led_pwr );
		BI_LED_RUN( pt_led_sync );
	
	#elif(PRODUCT_MODEL == CMDT3521B)
		BI_LED_RUN( pt_led_run );
		BI_LED_RUN( pt_led_alm );
		BI_LED_RUN( pt_led_pwr );
		BI_LED_RUN( pt_led_4g_sync );
		BI_LED_RUN( pt_led_5g_sync );
		BI_LED_RUN( pt_led_iso );
		
	#else
		BI_LED_RUN( pt_led_run );
		BI_LED_RUN( pt_led_alm );
		BI_LED_RUN( pt_led_5g_pwr );
		BI_LED_RUN( pt_led_5g_sync );
		BI_LED_RUN( pt_led_4g_pwr );
		BI_LED_RUN( pt_led_4g_sync );
	
	#endif
}

/**
* @brief   开机所有的灯全亮   
* @param   None
* @return    
* @data    2024-03-30
* @note    
*/
void led_all_on( void )
{
	#if(PRODUCT_MODEL == CMDT3521A)	
		pt_led_run->set( pt_led_run,  LED_ON, 0 );
		pt_led_alm->set( pt_led_alm,  LED_ON, 0 );
		pt_led_alm->set( pt_led_pwr,  LED_ON, 0 );
		pt_led_alm->set( pt_led_sync, LED_ON, 0 );
		
	#elif(PRODUCT_MODEL == CMDT3521B)
		pt_led_run->set( pt_led_run,  	 LED_ON, 500 );
		pt_led_alm->set( pt_led_alm,  	 LED_ON, 0 );
		pt_led_alm->set( pt_led_pwr,  	 LED_ON, 0 );
		pt_led_alm->set( pt_led_4g_sync, LED_ON, 0 );
		pt_led_alm->set( pt_led_5g_sync, LED_ON, 0 );
		pt_led_alm->set( pt_led_iso, 	 LED_ON, 0 );
	
	#else
		pt_led_run->set( pt_led_run,  	 LED_ON, 500 );
		pt_led_alm->set( pt_led_alm,  	 LED_ON, 0 );
		pt_led_alm->set( pt_led_5g_pwr,  LED_ON, 0 );
		pt_led_alm->set( pt_led_5g_sync, LED_ON, 0 );
		pt_led_alm->set( pt_led_4g_pwr,  LED_ON, 0 );
		pt_led_alm->set( pt_led_4g_sync, LED_ON, 0 );

	#endif
	led_run();
	vTaskDelay( 1000 );
}

/**
* @brief   指示灯状态初始化   
* @param   None
* @return    
* @data    2024-03-13
* @note    起始状态全灭
*/
void led_init( void )
{
	led_all_on();
	
	#if(PRODUCT_MODEL == CMDT3521A)	
		pt_led_run->set( pt_led_run,  LED_OFF, 0 );
		pt_led_alm->set( pt_led_alm,  LED_OFF, 0 );
		pt_led_alm->set( pt_led_pwr,  LED_OFF, 0 );
		pt_led_alm->set( pt_led_sync, LED_OFF, 0 );
	
	#elif(PRODUCT_MODEL == CMDT3521B)
		pt_led_run->set( pt_led_run,  	 LED_OFF, 0 );
		pt_led_alm->set( pt_led_alm,  	 LED_OFF, 0 );
		pt_led_alm->set( pt_led_pwr,  	 LED_OFF, 0 );
		pt_led_alm->set( pt_led_4g_sync, LED_OFF, 0 );
		pt_led_alm->set( pt_led_5g_sync, LED_OFF, 0 );
		pt_led_alm->set( pt_led_iso, 	 LED_OFF, 0 );
	
	#else
		pt_led_run->set( pt_led_run,  	 LED_OFF, 0 );
		pt_led_alm->set( pt_led_alm,  	 LED_OFF, 0 );
		pt_led_alm->set( pt_led_5g_pwr,  LED_OFF, 0 );
		pt_led_alm->set( pt_led_5g_sync, LED_OFF, 0 );
		pt_led_alm->set( pt_led_4g_pwr,  LED_OFF, 0 );
		pt_led_alm->set( pt_led_4g_sync, LED_OFF, 0 );
	#endif
	
	led_run();
}


/**
* @brief   3521A 显示方案   
* @param   None
* @return    
* @data    2024-03-13
* @note    
*/
#if(PRODUCT_MODEL == CMDT3521A)
static void plan_display( void )
{
	#define LED_POW_LEVEL ( 0 )
	#define LED_ISO_LEVEL ( 10 )
	get_led_para();

	//pt_warn, 告警灯
	if( (pt_4g->iso <= (pt_4g->theo_gain-LED_ISO_LEVEL)) ||  (pt_nr->iso <= (pt_nr->theo_gain-LED_ISO_LEVEL)) || (pt_mimo->iso <= (pt_mimo->theo_gain-LED_ISO_LEVEL)) ) 
	{
		pt_led_alm->set( pt_led_alm, LED_FLASH, FLASH_SLOW );
	}
	else if((WARN == pt_4g->warn) || (WARN == pt_nr->warn) || (WARN == pt_mimo->warn))
	{
		pt_led_alm->set( pt_led_alm, LED_ON, FLASH_SLOW );
	}
	else
	{
		pt_led_alm->set( pt_led_alm, LED_OFF, FLASH_SLOW );
	}
	
	//pt_pwr, 功率灯
	if( (pt_4g->pow >= (pt_4g->theo_pow-10)) || ((pt_nr->pow >= (pt_nr->theo_pow-10)) || (pt_mimo->pow >= (pt_mimo->theo_pow-10))))
	{
		pt_led_pwr->set( pt_led_pwr, LED_ON, FLASH_SLOW );
	}
	else
	{
		pt_led_pwr->set( pt_led_pwr, LED_FLASH, FLASH_SLOW );
	}
	
	//pt_sync, 同步灯
	if( (SYNC == pt_4g->sync) && (SYNC == pt_nr->sync) )
	{
		pt_led_sync->set( pt_led_sync, LED_ON, FLASH_QUICK );
	}
	else if( (UNSYNC == pt_4g->sync) && (SYNC == pt_nr->sync) )
	{
		pt_led_sync->set( pt_led_sync, LED_FLASH, FLASH_SLOW );
	}
	else if( (SYNC == pt_4g->sync) && (UNSYNC == pt_nr->sync) )
	{
		pt_led_sync->set( pt_led_sync, LED_FLASH, FLASH_QUICK );
	}
	else
	{
		pt_led_sync->set( pt_led_sync, LED_OFF, FLASH_SLOW );
	}
	
	//pt_run, 运行灯
	if( (B_TRUE == pt_4g->run_state) && (B_TRUE == pt_4g->run_state) && (B_TRUE == pt_4g->run_state))
		pt_led_run->set( pt_led_run, LED_ON, FLASH_SLOW );
	else
		pt_led_run->set( pt_led_run, LED_FLASH, FLASH_SLOW );
}
#endif

#if(PRODUCT_MODEL == CMDT3521B)
static void plan_display( void )
{
	#define LED_POW_LEVEL ( 0 )
	#define LED_ISO_LEVEL ( 10 )
	get_led_para();

	//pt_warn, 告警灯
	if((WARN == pt_4g->warn) || (WARN == pt_nr->warn) || (WARN == pt_mimo->warn))
	{
		pt_led_alm->set( pt_led_alm, LED_ON, FLASH_SLOW );
	}
	else
	{
		pt_led_alm->set( pt_led_alm, LED_OFF, FLASH_SLOW );
	}
	
	//pt_iso, 隔离度灯
	if( pt_4g->iso_close || pt_nr->iso_close || pt_mimo->iso_close )	//隔离度关断
	{
		pt_led_iso->set( pt_led_iso, LED_ON, FLASH_SLOW );
	}
	else if( (pt_4g->iso <= (pt_4g->theo_gain-LED_ISO_LEVEL)) ||  (pt_nr->iso <= (pt_nr->theo_gain-LED_ISO_LEVEL)) || (pt_mimo->iso <= (pt_mimo->theo_gain-LED_ISO_LEVEL)) ) 
	{
		pt_led_iso->set( pt_led_iso, LED_FLASH, FLASH_SLOW );
	}
	else
	{
		pt_led_iso->set( pt_led_iso, LED_OFF, FLASH_SLOW );
	}
	
	//pt_pwr, 功率灯
		if( (pt_4g->pow >= (pt_4g->theo_pow-10)) || ((pt_nr->pow >= (pt_nr->theo_pow-10)) || (pt_mimo->pow >= (pt_mimo->theo_pow-10))))
	{
		pt_led_pwr->set( pt_led_pwr, LED_ON, FLASH_SLOW );
	}
	else
	{
		pt_led_pwr->set( pt_led_pwr, LED_FLASH, FLASH_SLOW );
	}
	
	//pt_4g_sync, 同步灯
	if( IS_FDD == pt_4g->tdd_or_fdd )
	{
		pt_led_4g_sync->set( pt_led_4g_sync, LED_OFF, FLASH_QUICK );
	}
	else if( SYNC == pt_4g->sync )
	{
		pt_led_4g_sync->set( pt_led_4g_sync, LED_ON, FLASH_SLOW );
	}
	else
	{
		pt_led_4g_sync->set( pt_led_4g_sync, LED_FLASH, FLASH_SLOW );
	}
	
	//pt_5g_sync, 同步灯
	if( SYNC == pt_nr->sync )
	{
		pt_led_5g_sync->set( pt_led_5g_sync, LED_ON, FLASH_SLOW );
	}
	else
	{
		pt_led_5g_sync->set( pt_led_5g_sync, LED_FLASH, FLASH_SLOW );
	}
	
	//pt_run, 运行灯
	if( (B_TRUE == pt_4g->run_state) && (B_TRUE == pt_4g->run_state) && (B_TRUE == pt_4g->run_state))
		pt_led_run->set( pt_led_run, LED_ON, FLASH_SLOW );
	else
		pt_led_run->set( pt_led_run, LED_FLASH, FLASH_SLOW );
}
#endif


#if(PRODUCT_MODEL == CMDT3521C)
static void plan_display( void )
{
	#define LED_POW_LEVEL ( 10 )
	#define LED_ISO_LEVEL ( 10 )
	get_led_para();

	//pt_warn, 告警灯
	if( (WARN == pt_4g->warn) ||  (WARN == pt_nr->warn) || (WARN == pt_mimo->warn) || 
			(CLOSE_POW_OVER == pt_4g->pow_close) || (CLOSE_POW_OVER == pt_nr->pow_close) || (CLOSE_POW_OVER == pt_mimo->pow_close) ) 
	{
		pt_led_alm->set( pt_led_alm, LED_ON, FLASH_SLOW );
	}
	else
	{
		pt_led_alm->set( pt_led_alm, LED_OFF, FLASH_SLOW );
	}
	
	//pt_4g_pwr, 功率灯
	if( pt_4g->pow >= (pt_4g->theo_pow - LED_POW_LEVEL) )
	{
		pt_led_4g_pwr->set( pt_led_4g_pwr, LED_ON, FLASH_SLOW );
	}
	else
	{
		pt_led_4g_pwr->set( pt_led_4g_pwr, LED_OFF, FLASH_SLOW );
	}
	
	//pt_4g_sync, 同步灯
	if( IS_FDD == pt_4g->tdd_or_fdd )
	{
		pt_led_4g_sync->set( pt_led_4g_sync, LED_OFF, FLASH_QUICK );
	}
	else if( SYNC == pt_4g->sync )
	{
		pt_led_4g_sync->set( pt_led_4g_sync, LED_ON, FLASH_SLOW );
	}
	else
	{
		pt_led_4g_sync->set( pt_led_4g_sync, LED_FLASH, FLAHS_SUPER_SLOW );
	}
	
	//pt_5g_pwr, 功率灯
	if( (pt_nr->pow >= (pt_nr->theo_pow - LED_POW_LEVEL )) || (pt_mimo->pow >= (pt_mimo->theo_pow - LED_POW_LEVEL ))  )
	{
		pt_led_5g_pwr->set( pt_led_5g_pwr, LED_ON, FLASH_SLOW );
	}
	else
	{
		pt_led_5g_pwr->set( pt_led_5g_pwr, LED_FLASH, FLASH_SLOW );
	}
	
	//pt_5g_sync, 同步灯
	if( SYNC == pt_nr->sync )
	{
		pt_led_5g_sync->set( pt_led_5g_sync, LED_ON, FLASH_SLOW );
	}
	else
	{
		pt_led_5g_sync->set( pt_led_5g_sync, LED_FLASH, FLAHS_SUPER_SLOW );
	}
	
	//pt_run, 运行灯
	if( (B_TRUE == pt_4g->run_state) && (B_TRUE == pt_4g->run_state) && (B_TRUE == pt_4g->run_state))
		pt_led_run->set( pt_led_run, LED_FLASH, FLASH_QUICK );
	else
		pt_led_run->set( pt_led_run, LED_FLASH, FLASH_SLOW );
}
#endif

/**
* @brief   测试模式下, 仅运行灯500ms闪烁 
* @param   None
* @return    
* @data    2024-03-13
* @note    
*/
void led_work_in_test( void )
{
	#if(PRODUCT_MODEL == CMDT3521A)	
		pt_led_run->set( pt_led_run,  LED_FLASH, 500 );
		pt_led_alm->set( pt_led_alm,  LED_OFF, 0 );
		pt_led_alm->set( pt_led_pwr,  LED_OFF, 0 );
		pt_led_alm->set( pt_led_sync, LED_OFF, 0 );

		
	#elif(PRODUCT_MODEL == CMDT3521B)
		pt_led_run->set( pt_led_run,  	 LED_FLASH, 500 );
		pt_led_alm->set( pt_led_alm,  	 LED_OFF, 0 );
		pt_led_alm->set( pt_led_pwr,  	 LED_OFF, 0 );
		pt_led_alm->set( pt_led_4g_sync, LED_OFF, 0 );
		pt_led_alm->set( pt_led_5g_sync, LED_OFF, 0 );
		pt_led_alm->set( pt_led_iso, 	 LED_OFF, 0 );
	
	#else
		pt_led_run->set( pt_led_run,  	 LED_FLASH, 500 );
		pt_led_alm->set( pt_led_alm,  	 LED_OFF, 0 );
		pt_led_alm->set( pt_led_5g_pwr,  LED_OFF, 0 );
		pt_led_alm->set( pt_led_5g_sync, LED_OFF, 0 );
		pt_led_alm->set( pt_led_4g_pwr,  LED_OFF, 0 );
		pt_led_alm->set( pt_led_4g_sync, LED_OFF, 0 );

	#endif
}


/**
* @brief   在任务中调用的指示灯显示
* @param   None
* @return    
* @data    2024-03-13
* @note    
*/
void led_in_task( void )
{
	if( WORK_CALIB == led_work_mode )
		led_work_in_test();
	else
		plan_display();
	
	led_run();
}
