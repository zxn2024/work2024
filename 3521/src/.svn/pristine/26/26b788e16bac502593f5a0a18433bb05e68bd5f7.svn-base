/***************************** Copyright ****************************************
*
* (C) Copyright 2022, Shaanxi Tianji Comunication
*  All Rights Reserved
*
* FileName   : bsp_rf_lna_pa.c
* Version    : none
* Author     : none
* Date       : 2023-10-10
* Description:
*   	1. 在4G频段获取之后初始化 lna_pa_pt_init( void ), 若切换了频段, 必须重新初始化
*		2.
*******************************************************************************/
#include "main.h"
#include "system_config.h"
#include "bsp_public_drv.h"
#include "bsp_config.h"
#include "bsp_rf_lna.h"
#include "lna.h"
#include "pa.h"
#include "bsp_rf_global.h"
#include "bsp_rf_4g_band_sel.h"

/*---------------------- 端口定义 --------------------------------------*/


/*---------------------- 宏定义  ---------------------------------------*/
typedef struct
{
    LNA_t * lna1;
    LNA_t * lna2;
} CH_LNA_T;

/*---------------------- 结构体 ----------------------------------------*/
CH_LNA_T lna_ch[RF_SYS_NUM][RF_CH_NUM];

/*---------------------- 常量 ------------------------------------------*/


/*---------------------- 全局变量 --------------------------------------*/


/*---------------------- 局部变量 --------------------------------------*/


/*---------------------- 函数声明 --------------------------------------*/


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*****************************************************************************
* @Name	  : pa_pt_init
* @Brief  : 功放指针初始化
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2023-10-31
* @Note   :
*****************************************************************************/
#if LOCAL_MODULE_TYPE == _MU
void lna_pt_init(void)
{
    uint8_t band = get_valid_band(RF_SYS_1);
    
    lna_ch[RF_SYS_1][RF_CH_UP].lna1 = pt_lna_4g_up;
    lna_ch[RF_SYS_1][RF_CH_UP].lna2 = NULL;

    lna_ch[RF_SYS_1][RF_CH_DW].lna1 = pt_lna_4g_dw_1;
    lna_ch[RF_SYS_1][RF_CH_DW].lna2 = pt_lna_4g_dw_2;

    lna_ch[RF_SYS_2][RF_CH_UP].lna1 = &lna_nr_up;
    lna_ch[RF_SYS_2][RF_CH_UP].lna2 = NULL;

    lna_ch[RF_SYS_2][RF_CH_DW].lna1 = &lna_nr_dw_1;
    lna_ch[RF_SYS_2][RF_CH_DW].lna2 = &lna_nr_dw_2;

    lna_ch[RF_SYS_3][RF_CH_UP].lna1 = &lna_mimo_up;
    lna_ch[RF_SYS_3][RF_CH_UP].lna2 = NULL;

    lna_ch[RF_SYS_3][RF_CH_DW].lna1 = &lna_mimo_dw_1;
    lna_ch[RF_SYS_3][RF_CH_DW].lna2 = &lna_mimo_dw_2;
}
#else
void lna_pt_init(void)
{
    uint8_t band = get_valid_band(RF_SYS_1);

    lna_ch[RF_SYS_1][RF_CH_UP].lna1 = pt_lna_4g_up_1;
    lna_ch[RF_SYS_1][RF_CH_UP].lna2 = pt_lna_4g_up_2;

    lna_ch[RF_SYS_1][RF_CH_DW].lna1 = pt_lna_4g_dw;
    lna_ch[RF_SYS_1][RF_CH_DW].lna2 = NULL;

    lna_ch[RF_SYS_2][RF_CH_UP].lna1 = &lna_nr_up_1;
    lna_ch[RF_SYS_2][RF_CH_UP].lna2 = &lna_nr_up_2;

    lna_ch[RF_SYS_2][RF_CH_DW].lna1 = &lna_nr_dw;
    lna_ch[RF_SYS_2][RF_CH_DW].lna2 = NULL;

    lna_ch[RF_SYS_3][RF_CH_UP].lna1 = &lna_mimo_up_1;
    lna_ch[RF_SYS_3][RF_CH_UP].lna2 = &lna_mimo_up_2;

    lna_ch[RF_SYS_3][RF_CH_DW].lna1 = &lna_mimo_dw;
    lna_ch[RF_SYS_3][RF_CH_DW].lna2 = NULL;
}
#endif



/*****************************************************************************
* @Name	  : lna_set
* @Brief  : 设置低噪放状态
* @Param  : sys: [输入/出]
**			 ch: [输入/出]
**			 id: LNA编号, 取值1, 2, 最大为2
**			 state: [输入/出]
* @Retval :
* @Author :
* @Data   : 2023-10-10
* @Note   :
*****************************************************************************/
void lna_set(uint8_t sys, uint8_t ch, uint8_t id, uint8_t state)
{

    if((sys >= RF_SYS_NUM) || (ch >= RF_CH_NUM))
        return;

    if((state != LNA_BYS) && (state != LNA_RF))
        return;

    if((id == 0) || (id > 2))
        return;

    LNA_t * pt = (id == 1) ? lna_ch[sys][ch].lna1 : lna_ch[sys][ch].lna2;


    if(NULL == pt)
        return;
	
    BI_LNA_BYPASS(pt, (LNA_BYPASS_E)state);
}

