/***************************** Copyright ****************************************
*
* (C) Copyright 2023, Shaanxi Tianji Communication
*  All Rights Reserved
*
* FileName   : bsp_rf_get_para.c
* Version    : none
* Author     :
* Date       : 2023-11-20
* Description: none
*******************************************************************************/
#include "main.h"
//#include "stdlib.h"
//#include "stdio.h"
//#include "string.h"
#include "system_config.h"
#include "bsp_public_drv.h"
#include "bsp_rf_get_para.h"
#include "rtt.h"
#include "bsp_rf_global.h"
#include "bsp_rf_5g_modem.h"


/*---------------------- 端口定义 --------------------------------------*/


/*---------------------- 宏定义  ---------------------------------------*/


/*---------------------- 结构体 ----------------------------------------*/


/*---------------------- 常量 ------------------------------------------*/


/*---------------------- 全局变量 --------------------------------------*/


/*---------------------- 局部变量 --------------------------------------*/


/*---------------------- 声明 ------------------------------------------*/


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ APP ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*****************************************************************************
* @Name	  : get_4g_valid_id
* @Brief  : 获取4G的有效频段和带宽
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2023-11-20
* @Note   :
*****************************************************************************/
void get_band_and_bw(void)
{
//    // 根据模组实际值进行赋值
//    t_rf_sys[RF_SYS_1].t_public.valid_band = BAND_3;
//    t_rf_sys[RF_SYS_1].t_public.valid_bw   = B3_20M;

//    t_rf_sys[RF_SYS_2].t_public.valid_band = BAND_41;
//    t_rf_sys[RF_SYS_2].t_public.valid_bw   = B41_100M;

//    t_rf_sys[RF_SYS_3].t_public.valid_band = BAND_41M;
//    t_rf_sys[RF_SYS_3].t_public.valid_bw   = B41_100M;
}


/*****************************************************************************
* @Name	  : get_sync
* @Brief  : 获取4g/5g同步状态
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2023-11-20
* @Note   :
*****************************************************************************/
void get_sync(void)
{
    // 根据模组实际值进行赋值
	if((WORK_CALIB == tDevPublicPara.ucWorkMode) || (WORK_PL == tDevPublicPara.ucWorkMode))
	{
		t_rf_sys[RF_SYS_1].t_public.sync = SYNC;
		t_rf_sys[RF_SYS_2].t_public.sync = SYNC;
		t_rf_sys[RF_SYS_3].t_public.sync = SYNC;
		return;
	}
	
    if((BAND_39 == t_rf_sys[RF_SYS_1].t_public.valid_band) || (BAND_40 == t_rf_sys[RF_SYS_1].t_public.valid_band))
        t_rf_sys[RF_SYS_1].t_public.sync = rf_5g_modem_get_lte_sync();
    else
        t_rf_sys[RF_SYS_1].t_public.sync = SYNC;

    t_rf_sys[RF_SYS_2].t_public.sync = rf_5g_modem_get_nr_sync();
    t_rf_sys[RF_SYS_3].t_public.sync = rf_5g_modem_get_nr_sync();
}


/*****************************************************************************
* @Name	  : get_net_para
* @Brief  : 获取网络参数
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2023-11-20
* @Note   :
*****************************************************************************/
void get_net_para(void)
{
//    // 根据模组实际值进行赋值
//    t_rf_sys[RF_SYS_1].t_public.tNet.rsrp = 0;
//    t_rf_sys[RF_SYS_1].t_public.tNet.rssi = 0;

//    t_rf_sys[RF_SYS_2].t_public.tNet.rsrp = 0;
//    t_rf_sys[RF_SYS_2].t_public.tNet.rssi = 0;

//    t_rf_sys[RF_SYS_3].t_public.tNet.rsrp = 0;
//    t_rf_sys[RF_SYS_3].t_public.tNet.rssi = 0;
}



/*****************************************************************************
* @Name	  : get_all_para
* @Brief  : 获取所有的参数
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2023-11-20
* @Note   :
*****************************************************************************/
void get_all_para(void)
{
    get_band_and_bw();
    get_sync();
    get_net_para();
}




