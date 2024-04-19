/***************************** Copyright ****************************************
*
* (C) Copyright 2023, Shaanxi Tianji Communication
*  All Rights Reserved
*
* FileName   : bsp_rf_time.c
* Version    : none
* Author     :
* Date       : 2023-11-13
* Description: none
*       1. 100ms定时中断中调用 void rf_ctr_in_time_isr(void)
*       2. 获取射频参数刷新标识 uint8_t get_rf_interval_flag( void )
*       3. 获取上行静默检测标识 uint8_t get_rf_up_silence_flag( void )
*******************************************************************************/
#include "main.h"
//#include "stdlib.h"
//#include "stdio.h"
//#include "string.h"
#include "FreeRTOS.h"                   // ARM.FreeRTOS::RTOS:Core
#include "event_groups.h"               // ARM.FreeRTOS::RTOS:Event Groups
#include "rtt.h"
#include "bsp_rf_time.h"
#include "bsp_rf_global.h"
#include "bsp_public_drv.h"

/*---------------------- 端口定义 --------------------------------------*/
#define RF_CTR_INTERVAL_TIME_CNT	MS_TO_TICKS(100)
#define RF_CTR_BRD_TIME_CNT			S_TO_TICKS(30)


/*---------------------- 事件组  ---------------------------------------*/


/*---------------------- 结构体 ----------------------------------------*/

typedef struct
{
    uint16_t cnt;
    uint16_t  max;
    uint8_t  state;
} RF_TIME_T;

/*---------------------- 常量 ------------------------------------------*/


/*---------------------- 全局变量 --------------------------------------*/


/*---------------------- 局部变量 --------------------------------------*/
RF_TIME_T t_time_rf_ctr;					//射频控制间隔
RF_TIME_T t_time_oplc_brd;					//开环工控广播计时
RF_TIME_T t_time_reload_bin[ RF_SYS_NUM ];				//reload 8668 bin 文件


/*---------------------- 声明 ------------------------------------------*/



/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ OS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*****************************************************************************
* @Name	  : rf_ctr_time_init
* @Brief  :
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2023-11-20
* @Note   :
*****************************************************************************/
void rf_ctr_time_init(void)
{
	memset(&t_time_rf_ctr, 0, sizeof(RF_TIME_T));
	t_time_rf_ctr.max   = RF_CTR_INTERVAL_TIME_CNT;
	t_time_rf_ctr.cnt   = 0;
	t_time_rf_ctr.state = B_FALSE;
	
	memset(&t_time_oplc_brd, 0, sizeof(RF_TIME_T));
	t_time_oplc_brd.max   = RF_CTR_BRD_TIME_CNT;
	t_time_oplc_brd.cnt   = 0;
	t_time_oplc_brd.state = B_FALSE;
}


/*****************************************************************************
* @Name	  : time_att
* @Brief  :
* @Param  : pt: [输入/出]
* @Retval :
* @Author :
* @Data   : 2023-11-20
* @Note   :
*****************************************************************************/
static void time_add(RF_TIME_T * pt)
{
    pt->cnt++;

    if(pt->cnt >= pt->max)
    {
        pt->state = B_TRUE;
        pt->cnt = 0;
    }
}

/*****************************************************************************
* @Name	  : rf_ctr_in_time_isr
* @Brief  : 定时器状态设置, 只能在在定时器中断中调用
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2023-10-18
* @Note   :
*****************************************************************************/
void rf_ctr_in_time_isr(void)
{
    time_add(&t_time_rf_ctr);
    time_add(&t_time_oplc_brd);
}


/*****************************************************************************
* @Name	  : func_get_rf_egp_state
* @Brief  :
* @Param  : egp: [输入/出]
**			 bit_flag: [输入/出]
* @Retval :
* @Author :
* @Data   : 2023-11-13
* @Note   :
*****************************************************************************/
static BOOL_E func_get_time_run_state( RF_TIME_T *pt )
{
    if( B_TRUE == pt->state )
    {
        pt->state = B_FALSE;
        return B_TRUE;
    }
    else
        return B_FALSE;
}

/*****************************************************************************
* @Name	  : get_rf_interval_flag
* @Brief  : 获取射频刷新标识
* @Param  : None
* @Retval : TURE: 需要刷新   B_FALSE: 不刷新
* @Author :
* @Data   : 2023-10-18
* @Note   :
*****************************************************************************/
BOOL_E get_rf_interval_flag(void)
{
    return func_get_time_run_state( &t_time_rf_ctr );
}


/*****************************************************************************
* @Name	  : get_rf_oplc_brd_flag
* @Brief  :
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2023-11-20
* @Note   :
*****************************************************************************/
BOOL_E get_rf_oplc_brd_flag( void )
{
    return func_get_time_run_state( &t_time_oplc_brd );
}


/*****************************************************************************
* @Name	  : get_rf_reload_bin_flag
* @Brief  : 获取 reload bin 标志
* @Param  : sys: [输入/出] 
* @Retval : 
* @Author : 
* @Data   : 2024-01-25
* @Note   :
*****************************************************************************/
uint8_t get_rf_reload_bin_flag( uint8_t sys )
{
    if( sys >= RF_SYS_NUM )
        return B_FALSE;

    return t_time_reload_bin[sys].state;
}


/*****************************************************************************
* @Name	  : clr_rf_reload_bin_flag
* @Brief  : 清空 reload bin 标志
* @Param  : sys: [输入/出] 
* @Retval : 
* @Author : 
* @Data   : 2024-01-25
* @Note   :
*****************************************************************************/
void clr_rf_reload_bin_flag(uint8_t sys )
{
    if( sys >= RF_SYS_NUM )
		return;

	t_time_reload_bin[sys].cnt   = 0;
    t_time_reload_bin[sys].state = B_FALSE;
}


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ APP ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/



