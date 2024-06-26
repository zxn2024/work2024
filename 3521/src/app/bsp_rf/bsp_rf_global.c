/***************************** Copyright ****************************************
*
* (C) Copyright 2023, Shaanxi Tianji Communication
*  All Rights Reserved
*
* FileName   : bsp_rf_global.c
* Version    : none
* Author     :
* Date       : 2023-10-18
* Description: none
*******************************************************************************/
#include "main.h"
#include "system_config.h"
#include "bsp_rf_global.h"


/*---------------------- 端口定义 --------------------------------------*/


/*---------------------- 宏定义  ---------------------------------------*/


/*---------------------- 结构体 ----------------------------------------*/


/*---------------------- 常量 ------------------------------------------*/


/*---------------------- 全局变量 --------------------------------------*/
DevPublicPara_T tDevPublicPara;   //与硬件相关的公共参数

RfSystem_T          t_rf_sys[ RF_SYS_NUM ];

/*---------------------- 局部变量 --------------------------------------*/


/*---------------------- 声明 ------------------------------------------*/



/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ HAL ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/





/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DRV ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/





/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ APP ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*****************************************************************************
* @Name	  : set_valid_band
* @Brief  : 设置有效的频段
* @Param  : sys: [输入/出]
**			 band: [输入/出]
* @Retval :
* @Author :
* @Data   : 2023-12-28
* @Note   :
*****************************************************************************/
uint8_t set_valid_band(uint8_t sys, RF_BAND_E band)
{
    t_rf_sys[ sys ].t_public.valid_band = band;
 
    return t_rf_sys[ sys ].t_public.valid_band;
}

/*****************************************************************************
* @Name	  : set_valid_bw
* @Brief  : 设置有效的带宽
* @Param  : sys: [输入/出]
**			bw: [输入/出]
* @Retval :
* @Author :
* @Data   : 2023-12-28
* @Note   :
*****************************************************************************/
uint8_t set_valid_bw(uint8_t sys, uint8_t bw)
{
    t_rf_sys[ sys ].t_public.valid_bw = bw;
	
    return t_rf_sys[ sys ].t_public.valid_bw;
}

/*****************************************************************************
* @Name	  : set_valid_band
* @Brief  : 设置有效的频段
* @Param  : sys: [输入/出]
**			 band: [输入/出]
* @Retval :
* @Author :
* @Data   : 2024-01-12
* @Note   :
*****************************************************************************/
uint32_t set_valid_freq(uint8_t sys, uint32_t freq)
{
    t_rf_sys[ sys ].t_public.valid_freq = freq;
	
	return t_rf_sys[ sys ].t_public.valid_freq;
}

/*****************************************************************************
* @Name	  : get_valid_band
* @Brief  : 获取有效的频段
* @Param  : sys: [输入/出]
* @Retval : BAND_3 / BAND_8 / BAND_39 / BAND_40 / BAND_41
* @Author :
* @Data   : 2023-11-16
* @Note   :
*****************************************************************************/
uint8_t get_valid_band(uint8_t sys)
{
    return t_rf_sys[ sys ].t_public.valid_band;
}

/*****************************************************************************
* @Name	  : get_valid_bw
* @Brief  : 获取有效的带宽
* @Param  : sys: [输入/出]
* @Retval :
* @Author :
* @Data   : 2023-11-16
* @Note   :
*****************************************************************************/
uint8_t get_valid_bw(uint8_t sys)
{
    return t_rf_sys[ sys ].t_public.valid_bw;
}

/*****************************************************************************
* @Name	  : get_valid_freq
* @Brief  : 获取有效的频点
* @Param  : sys: [输入/出]
* @Retval :
* @Author :
* @Data   : 2024-01-12
* @Note   :
*****************************************************************************/
uint32_t get_valid_freq(uint8_t sys)
{
    return t_rf_sys[ sys ].t_public.valid_freq;
}

