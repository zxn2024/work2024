/***************************** Copyright ****************************************
*
* (C) Copyright 2023, Shaanxi Tianji Communication
*  All Rights Reserved
*
* FileName   : bsp_rf_dev_state.c
* Version    : none
* Author     :
* Date       : 2023-10-18
* Description: none
*******************************************************************************/
#include "main.h"
#include "system_config.h"
#include "bsp_public_drv.h"

#include "bsp_rf_global.h"
#include "bsp_rf_dev_state.h"
#include "bsp_rf_4g_band_sel.h"

/*---------------------- 端口定义 --------------------------------------*/


/*---------------------- 宏定义  ---------------------------------------*/


/*---------------------- 结构体 ----------------------------------------*/


/*---------------------- 常量 ------------------------------------------*/


/*---------------------- 全局变量 --------------------------------------*/


/*---------------------- 局部变量 --------------------------------------*/


/*---------------------- 声明 ------------------------------------------*/


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ HAL ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/





/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DRV ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/





/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ APP ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*****************************************************************************
* @Name	  : get_band_exist
* @Brief  : 根据设备频段数量设置有效频段标志
* @Param  : band: 频段
* @Retval : B_TRUE: 存在,  B_FALSE: 不存在
* @Author : 
* @Data   : 2024-01-20
* @Note   :
*****************************************************************************/
uint8_t get_band_exist( uint8_t band )
{
	if( (BAND_3 == band) || (BAND_8 == band) || (BAND_39 == band) || (BAND_40 == band) )
	{
		return t_rf_4g_save_para[band].exist;
	}
	else if( BAND_41 == band )
	{
		return t_rf_sys[RF_SYS_2].t_public.exist;
	}
	else if( BAND_41M == band )
	{
		return t_rf_sys[RF_SYS_3].t_public.exist;
	}
	else
		return B_FALSE;
}

/*****************************************************************************
* @Name	  : rf_power_source_set
* @Brief  : 链路电源控制
* @Param  : sys: RF_SYS_1 .... RF_SYS_NUM
*			ch: RF_CH_UP / RF_CH_DW
*			state: OPEN / CLOSE
* @Retval :
* @Author :
* @Data   : 2023-10-19
* @Note   :
*****************************************************************************/
void rf_power_source_set(uint8_t sys, uint8_t ch, uint8_t state)
{
    if((sys >= RF_SYS_NUM) || (ch >= RF_CH_NUM))
        return;

    if((state != OPEN) && (state != CLOSE))
        return;


}


/*****************************************************************************
* @Name	  : rf_sys_open_state_set
* @Brief  : 链路开机状态设置
* @Param  : sys: RF_SYS_1 .... RF_SYS_NUM
*    		state: B_TRUE : 已完成  /   B_FALSE : 未完成
* @Retval :
* @Author :
* @Data   : 2023-10-19
* @Note   :
*****************************************************************************/
void rf_sys_open_state_set(uint8_t sys, BOOL_E state)
{
    if(sys >= RF_SYS_NUM)
        return;

    if((state != B_FALSE) && (state != B_TRUE))
        return;

    t_rf_sys[ sys ].t_public.openState = state;
}


/*****************************************************************************
* @Name	  : rf_sys_sync_state_set
* @Brief  : 同步状态设置
* @Param  : sys: RF_SYS_1 .... RF_SYS_NUM
*			state: SYNC : 已同步  /   UNSYNC : 未同步
* @Retval :
* @Author :
* @Data   : 2023-10-19
* @Note   :
*****************************************************************************/
void rf_sys_sync_state_set(uint8_t sys, SYNC_STATE_E state)
{
    if(sys >= RF_SYS_NUM)
        return;

    t_rf_sys[ sys ].t_public.sync = state;
}


//
/*****************************************************************************
* @Name	  : rf_dev_run_mode_set
* @Brief  : 设置运行模式
* @Param  : sys: RF_SYS_1 .... RF_SYS_NUM
*			state: WORK_MODE_NORMAL : 正常模式   /   WORK_MODE_STANDLONE : 单设备模式
* @Retval :
* @Author :
* @Data   : 2023-10-19
* @Note   :
*****************************************************************************/
//void rf_dev_run_mode_set( uint8_t sys, WORK_MODE_E state )
//{
////    if( sys >= RF_SYS_NUM || state >= WORK_MODE_END )
////        return;

////    tDevPublicPara.ucDevMode = state;
//}


/*****************************************************************************
* @Name	  : rf_sys_run_state_set
* @Brief  : 设置设备运行状态
* @Param  : sys: RF_SYS_1 .... RF_SYS_NUM
*			state: OPEN / CLOSE
* @Retval :
* @Author :
* @Data   : 2023-10-19
* @Note   :
*****************************************************************************/
void rf_sys_run_state_set(uint8_t sys, OPEN_CLOSE_E state)
{
    if(sys >= RF_SYS_NUM)
        return;

    t_rf_sys[ sys ].t_public.sysRunState = state;
}


/*****************************************************************************
* @Name	  : rf_sys_oplc_en_set
* @Brief  : 设置开环工控使能
* @Param  : sys: RF_SYS_1 .... RF_SYS_NUM
**			state: ENABLE / DISABLE
* @Retval :
* @Author :
* @Data   : 2023-10-19
* @Note   :
*****************************************************************************/
void rf_sys_oplc_en_set(uint8_t sys, uint8_t state)
{
    if(sys >= RF_SYS_NUM)
        return;

    t_rf_sys[ sys ].t_public.opleEn = state;
}


/*****************************************************************************
* @Name	  : rf_sys_iso_en_set
* @Brief  : 设置隔离度使能开关
* @Param  : sys: RF_SYS_1 .... RF_SYS_NUM
**			state: ENABLE / DISABLE
* @Retval :
* @Author :
* @Data   : 2023-10-19
* @Note   :
*****************************************************************************/
void rf_sys_iso_en_set(uint8_t sys, uint8_t state)
{
//    if( sys >= RF_SYS_NUM )
//        return;

//    t_rf_sys[ sys ].t_public. = state;
}



/*****************************************************************************
* @Name	  : get_rf_power_source_set
* @Brief  : 获取链路电源控制状态
* @Param  : sys: RF_SYS_1 .... RF_SYS_NUM
* @Retval : OPEN / CLOSE
* @Author :
* @Data   : 2023-10-19
* @Note   :
*****************************************************************************/
OPEN_CLOSE_E get_rf_power_source_set(uint8_t sys, uint8_t ch)
{
    return OPEN;
}


/*****************************************************************************
* @Name	  : get_rf_sys_open_state
* @Brief  : 获取链路开机状态设置
* @Param  : sys: RF_SYS_1 .... RF_SYS_NUM
* @Retval : B_TRUE : 已完成  /   B_FALSE : 未完成
* @Author :
* @Data   : 2023-10-19
* @Note   :
*****************************************************************************/
BOOL_E get_rf_sys_open_state(uint8_t sys)
{
    return (BOOL_E)t_rf_sys[ sys ].t_public.openState;
}



/*****************************************************************************
* @Name	  : get_rf_sys_sync_state_set
* @Brief  : 获取同步状态
* @Param  : sys: RF_SYS_1 .... RF_SYS_NUM
* @Retval : SYNC : 已同步  /   UNSYNC : 未同步
* @Author :
* @Data   : 2023-10-19
* @Note   :
*****************************************************************************/
SYNC_STATE_E get_rf_sys_sync_state_set(uint8_t sys)
{
    return (SYNC_STATE_E)t_rf_sys[ sys ].t_public.sync;
}


/*****************************************************************************
* @Name	  : get_rf_dev_run_mode
* @Brief  : 获取运行模式
* @Param  : sys: RF_SYS_1 .... RF_SYS_NUM
* @Retval : WORK_MODE_NORMAL : 正常模式   /   WORK_MODE_STANDLONE : 单设备模式
* @Author :
* @Data   : 2023-10-19
* @Note   :
*****************************************************************************/
//WORK_MODE_E get_rf_dev_run_mode( uint8_t sys )
//{

//    return tDevPublicPara.ucDevMode;
//}


/*****************************************************************************
* @Name	  : get_rf_sys_run_state
* @Brief  : 获取设备运行状态
* @Param  : sys: RF_SYS_1 .... RF_SYS_NUM
* @Retval : OPEN / CLOSE
* @Author :
* @Data   : 2023-10-19
* @Note   :
*****************************************************************************/
OPEN_CLOSE_E get_rf_sys_run_state(uint8_t sys, OPEN_CLOSE_E state)
{
    return (OPEN_CLOSE_E)t_rf_sys[ sys ].t_public.sysRunState;
}


/*****************************************************************************
* @Name	  : get_rf_sys_oplc_en
* @Brief  : 获取开环工控使能
* @Param  : sys: RF_SYS_1 .... RF_SYS_NUM
* @Retval : ENABLE / DISABLE
* @Author :
* @Data   : 2023-10-19
* @Note   :
*****************************************************************************/
uint8_t get_rf_sys_oplc_en(uint8_t sys)
{
    return t_rf_sys[ sys ].t_public.opleEn;
}


//设置隔离度使能开关
/*****************************************************************************
* @Name	  : get_rf_sys_iso_en
* @Brief  : 获取隔离度使能开关
* @Param  : sys: RF_SYS_1 .... RF_SYS_NUM
* @Retval : ENABLE / DISABLE
* @Author :
* @Data   : 2023-10-19
* @Note   :
*****************************************************************************/
uint8_t get_rf_sys_iso_en(uint8_t sys)
{
    return ENABLE;
    //return t_rf_sys[ sys ].t_public.;
}
