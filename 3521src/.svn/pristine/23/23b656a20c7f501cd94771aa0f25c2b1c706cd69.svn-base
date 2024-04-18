/***************************** Copyright ****************************************
*
* (C) Copyright 2023, Shaanxi Tianji Communication
*  All Rights Reserved
*
* FileName   : bsp_rf_4g_band_sel.c
* Version    : none
* Author     :
* Date       : 2023-10-18
* Description:
 *				B3:  1800M  20 / 25
 *				B8:  900M	10 / 15
 *				B39: 1900M	10 / 20 /30
 *				B40: 2300M	20 / 50
 *				B41: 2600M	20 / 60 / 80 / 100
*******************************************************************************/
#include "main.h"
//#include "stdlib.h"
//#include "stdio.h"
//#include "string.h"
#include "system_config.h"
#include "bsp_rf_dc_switch.h"
#include "bsp_rf_global.h"
#include "bsp_config.h"
#include "bsp_rf_4g_band_sel.h"
#include "bsp_rf_oplc.h"
#include "bi_switch_spXt.h"
#include "bsp_rf_lna.h"

/*---------------------- 端口定义 --------------------------------------*/


/*---------------------- 宏定义  ---------------------------------------*/

// U22-4GDL_RX   U24 - 4GDL_TX
#if LOCAL_MODULE_TYPE == _MU
ATT_analog_t * pt_att_analog_4g_up_1 = NULL;
ATT_analog_t * pt_att_analog_4g_up_2 = NULL;
ATT_analog_t * pt_att_analog_4g_dw   = NULL;

LNA_t * pt_lna_4g_up   = NULL;
LNA_t * pt_lna_4g_dw_1 = NULL;
LNA_t * pt_lna_4g_dw_2 = NULL;

PORT_t * pt_4g_dc_ctrl = NULL;

//sp4t_4g_dw_rx / sp4t_4g_dw_tx / sp4t_4g_bts / sp4t_4g_ant

#define SP4D_OFF		SW_OFF

//SP4D
#define SP4D_MU_DR_B3	SW_CH4
#define SP4D_MU_DR_B8	SW_CH2
#define SP4D_MU_DR_B39	SW_CH1
#define SP4D_MU_DR_B40	SW_CH3

#define SP4D_MU_DT_B3	SW_CH1
#define SP4D_MU_DT_B8	SW_CH3
#define SP4D_MU_DT_B39	SW_CH2
#define SP4D_MU_DT_B40	SW_CH4
#if (PRODUCT_MODEL == CMDT3521A || PRODUCT_MODEL == CMDT3521C)
#define SP4D_MU_ANT_B3	SW_CH2
#define SP4D_MU_ANT_B8	SW_CH4
#define SP4D_MU_ANT_B39	SW_CH1
#define SP4D_MU_ANT_B40	SW_CH3
#elif (PRODUCT_MODEL == CMDT3521B)
#define SP4D_MU_ANT_B3	SW_CH2
#define SP4D_MU_ANT_B8	SW_CH3
#define SP4D_MU_ANT_B39	SW_CH1
#define SP4D_MU_ANT_B40	SW_CH4
#endif
#define SP4D_MU_BTS_B3	SW_CH2
#define SP4D_MU_BTS_B8	SW_CH4
#define SP4D_MU_BTS_B39	SW_CH1
#define SP4D_MU_BTS_B40	SW_CH3

//SP2D
#define SP2D_OFF		SW_OFF
#define SP2D_MU_UL_B3	SW_CH2
#define SP2D_MU_UL_B39	SW_CH1
#define SP2D_MU_DL_B3	SW_CH1
#define SP2D_MU_DL_B39	SW_CH2

#else
ATT_analog_t * pt_att_analog_4g_up   = NULL;
ATT_analog_t * pt_att_analog_4g_dw_1 = NULL;
ATT_analog_t * pt_att_analog_4g_dw_2 = NULL;

LNA_t * pt_lna_4g_up_1 = NULL;
LNA_t * pt_lna_4g_up_2 = NULL;
LNA_t * pt_lna_4g_dw   = NULL;

PORT_t * pt_4g_dc_ctrl = NULL;

#define SP4D_OFF        SW_OFF
//SP4D
#define SP4D_RU_UR_B3   SW_CH1
#define SP4D_RU_UR_B8   SW_CH3
#define SP4D_RU_UR_B39  SW_CH4
#define SP4D_RU_UR_B40  SW_CH2

#define SP4D_RU_UT_B3   SW_CH2
#define SP4D_RU_UT_B8   SW_CH3
#define SP4D_RU_UT_B39  SW_CH1
#define SP4D_RU_UT_B40  SW_CH4

#define SP4D_RU_ANT_B3  SW_CH2
#define SP4D_RU_ANT_B8  SW_CH4
#define SP4D_RU_ANT_B39 SW_CH3
#define SP4D_RU_ANT_B40 SW_CH1

#define SP4D_RU_BTS_B3  SW_CH2
#define SP4D_RU_BTS_B8  SW_CH4
#define SP4D_RU_BTS_B39 SW_CH3
#define SP4D_RU_BTS_B40 SW_CH1

//SP2D
#define SP2D_OFF        SW_OFF
#define SP2D_RU_UL_B3   SW_CH2
#define SP2D_RU_UL_B39  SW_CH1
#define SP2D_RU_DL_B3   SW_CH2
#define SP2D_RU_DL_B39  SW_CH1
#endif


/*---------------------- 结构体 ----------------------------------------*/


/*---------------------- 常量 ------------------------------------------*/



/*---------------------- 全局变量 --------------------------------------*/
para_rf_ch_t t_rf_4g_save_para[ 4 ];             //用于flash保存的4g 4选一参数

/*---------------------- 局部变量 --------------------------------------*/


/*---------------------- 声明 ------------------------------------------*/



/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ HAL ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/





/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DRV ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*****************************************************************************
* @Name	  : rf_para_replace_after_4g_band_select
* @Brief  : 4G 切换频段后的射频参数更新
* @Param  : band: [输入/出]
* @Retval :
* @Author :
* @Data   : 2023-12-01
* @Note   :
*****************************************************************************/
void rf_para_replace_after_4g_band_select(uint8_t band)
{
    if(band >= 5)
        band = BAND_3;

    tOplc[ RF_SYS_1 ].link_gate   = t_rf_4g_save_para[ band ].oplc_gate;
    tOplc[ RF_SYS_1 ].pow_offset  = t_rf_4g_save_para[ band ].dw_pow_offset;
    tOplc[ RF_SYS_1 ].gain_offset = t_rf_4g_save_para[ band ].oplc_gain_offset;

    t_rf_sys[RF_SYS_1].t_ch[RF_CH_UP].tAtt.ucManuCal        = t_rf_4g_save_para[ band ].up_gain_offset;
    t_rf_sys[RF_SYS_1].t_ch[RF_CH_UP].tAlc.scAlcPointOffset = t_rf_4g_save_para[ band ].up_pow_offset;
    t_rf_sys[RF_SYS_1].t_ch[RF_CH_DW].tAtt.ucManuCal        = t_rf_4g_save_para[ band ].dw_gain_offset;
    t_rf_sys[RF_SYS_1].t_ch[RF_CH_DW].tAlc.scAlcPointOffset = t_rf_4g_save_para[ band ].dw_pow_offset;
}

/*****************************************************************************
* @Name	  : pt_remap_after_4g_band_select
* @Brief  : 4G 切换频段后的指针重定向
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2023-11-27
* @Note   :
*****************************************************************************/
#if LOCAL_MODULE_TYPE == _MU
void pt_remap_after_4g_band_select(void)
{
    uint8_t rf_4g_valid_band = get_valid_band(RF_SYS_1);	//获取当前频段号
    
    SW_STATE_E sp4t_dr  = SP4D_OFF;
    SW_STATE_E sp4t_dt  = SP4D_OFF;
    SW_STATE_E sp4t_bts = SP4D_OFF;
    SW_STATE_E sp4t_ant = SP4D_OFF;
    SW_STATE_E sp2t_ul  = SP2D_OFF;
    SW_STATE_E sp2t_dl  = SP2D_OFF;

    rf_para_replace_after_4g_band_select(rf_4g_valid_band);

    switch(rf_4g_valid_band)
    {
        case BAND_3:
            pt_4g_dc_ctrl 		  = &rf_b3_dc_ctrl;

            pt_att_analog_4g_up_1 = &att_analog_b3_up_1;
            pt_att_analog_4g_up_2 = &att_analog_b3_up_2;
        
            pt_lna_4g_up 		  = &lna_b3_up;
            pt_lna_4g_dw_1 		  = &lna_b3_dw_1;
            pt_lna_4g_dw_2 		  = &lna_b3_dw_2;

            sp4t_dr  = SP4D_MU_DR_B3;
            sp4t_dt  = SP4D_MU_DT_B3;
            sp4t_bts = SP4D_MU_BTS_B3;
            sp4t_ant = SP4D_MU_ANT_B3;
            sp2t_ul  = SP2D_MU_UL_B3;
            sp2t_dl  = SP2D_MU_DL_B3;
            break;

        case BAND_8:
            pt_4g_dc_ctrl 		  = &rf_b8_dc_ctrl;

            pt_att_analog_4g_up_1 = &att_analog_b8_up_1;
            pt_att_analog_4g_up_2 = &att_analog_b8_up_2;
        
            pt_lna_4g_up 		  = &lna_b8_up;
            pt_lna_4g_dw_1 		  = &lna_b8_dw_1;
            pt_lna_4g_dw_2 		  = NULL;

            sp4t_dr  = SP4D_MU_DR_B8;
            sp4t_dt  = SP4D_MU_DT_B8;
            sp4t_bts = SP4D_MU_BTS_B8;
            sp4t_ant = SP4D_MU_ANT_B8;
            sp2t_ul  = SP2D_OFF;
            sp2t_dl  = SP2D_OFF;
            break;

        case BAND_39:
            pt_4g_dc_ctrl 		  = &rf_b39_dc_ctrl;

            pt_att_analog_4g_up_1 = &att_analog_b39_up_1;
            pt_att_analog_4g_up_2 = &att_analog_b39_up_2;
        
            pt_lna_4g_up 		  = &lna_b39_up;
            pt_lna_4g_dw_1 		  = &lna_b39_dw_1;
            pt_lna_4g_dw_2 		  = &lna_b39_dw_2;

            sp4t_dr  = SP4D_MU_DR_B39;
            sp4t_dt  = SP4D_MU_DT_B39;
            sp4t_bts = SP4D_MU_BTS_B39;
            sp4t_ant = SP4D_MU_ANT_B39;
            sp2t_ul  = SP2D_MU_UL_B39;
            sp2t_dl  = SP2D_MU_DL_B39;
            break;

        case BAND_40:
            pt_4g_dc_ctrl 		  = &rf_b40_dc_ctrl;

            pt_att_analog_4g_up_1 = &att_analog_b40_up_1;
            pt_att_analog_4g_up_2 = &att_analog_b40_up_2;
        
            pt_lna_4g_up 		  = &lna_b40_up;
            pt_lna_4g_dw_1 		  = &lna_b40_dw_1;
            pt_lna_4g_dw_2 		  = &lna_b40_dw_2;

            sp4t_dr  = SP4D_MU_DR_B40;
            sp4t_dt  = SP4D_MU_DT_B40;
            sp4t_bts = SP4D_MU_BTS_B40;
            sp4t_ant = SP4D_MU_ANT_B40;
            sp2t_ul  = SP2D_OFF;
            sp2t_dl  = SP2D_OFF;
            break;

        default:
            pt_4g_dc_ctrl 		  = NULL;
            pt_att_analog_4g_up_1 = NULL;
            pt_att_analog_4g_up_2 = NULL;
            pt_att_analog_4g_dw   = NULL;
            pt_lna_4g_up 		  = NULL;
            pt_lna_4g_dw_1 		  = NULL;
            pt_lna_4g_dw_2 		  = NULL;
            sp4t_dr  			  = SP4D_OFF;
            sp4t_dt  			  = SP4D_OFF;
            sp4t_bts 			  = SP4D_OFF;
            sp4t_ant 			  = SP4D_OFF;
            sp2t_ul  			  = SP2D_OFF;
            sp2t_dl  			  = SP2D_OFF;
            break;
    }

	//低噪放指针重定向
	lna_pt_init();	
	
    //设置开关状态
    BI_Switch_CTL(&sp4t_4g_dw_rx, sp4t_dr);
    BI_Switch_CTL(&sp4t_4g_dw_tx, sp4t_dt);
    BI_Switch_CTL(&sp4t_4g_bts, sp4t_bts);
    BI_Switch_CTL(&sp4t_4g_ant, sp4t_ant);

    BI_Switch_CTL(&spdt_b3_b39_up_ctrl, sp2t_ul);
    BI_Switch_CTL(&spdt_b3_b39_dw_ctrl, sp2t_dl);
}
#else
void pt_remap_after_4g_band_select(void)
{
    uint8_t rf_4g_valid_band = get_valid_band(RF_SYS_1);	//获取当前频段号
    
    SW_STATE_E sp4t_ur  = SP4D_OFF;
    SW_STATE_E sp4t_ut  = SP4D_OFF;
    SW_STATE_E sp4t_ant = SP4D_OFF;
    SW_STATE_E sp4t_bts = SP4D_OFF;
    SW_STATE_E sp2t_ul  = SP2D_OFF;
    SW_STATE_E sp2t_dl  = SP2D_OFF;

    rf_para_replace_after_4g_band_select(rf_4g_valid_band);

    switch(rf_4g_valid_band)
    {
        case BAND_3:
            pt_4g_dc_ctrl         = &rf_b3_dc_ctrl;

            pt_att_analog_4g_dw_1 = &att_analog_b3_dw_1;
            pt_att_analog_4g_dw_2 = &att_analog_b3_dw_2;

            pt_lna_4g_up_1        = &lna_b3_up_1;
            pt_lna_4g_up_2        = &lna_b3_up_2;
            pt_lna_4g_dw          = &lna_b3_dw;

            sp4t_ur  = SP4D_RU_UR_B3;
            sp4t_ut  = SP4D_RU_UT_B3;
            sp4t_ant = SP4D_RU_ANT_B3;
            sp4t_bts = SP4D_RU_BTS_B3;
            sp2t_ul  = SP2D_RU_UL_B3;
            sp2t_dl  = SP2D_RU_DL_B3;
            break;

        case BAND_8:
            pt_4g_dc_ctrl 		  = &rf_b8_dc_ctrl;

            pt_att_analog_4g_dw_1 = &att_analog_b8_dw_1;
            pt_att_analog_4g_dw_2 = &att_analog_b8_dw_2;

            pt_lna_4g_up_1 		  = &lna_b8_up_1;
            pt_lna_4g_up_2 		  = &lna_b8_up_2;
            pt_lna_4g_dw 		  = &lna_b8_dw;

            sp4t_ur  = SP4D_RU_UR_B8;
            sp4t_ut  = SP4D_RU_UT_B8;
            sp4t_ant = SP4D_RU_ANT_B8;
            sp4t_bts = SP4D_RU_BTS_B8;
            sp2t_ul  = SP2D_OFF;
            sp2t_dl  = SP2D_OFF;
            break;

        case BAND_39:
            pt_4g_dc_ctrl 		  = &rf_b39_dc_ctrl;

            pt_att_analog_4g_dw_1 = &att_analog_b39_dw_1;
            pt_att_analog_4g_dw_2 = &att_analog_b39_dw_2;

            pt_lna_4g_up_1 		  = &lna_b39_up_1;
            pt_lna_4g_up_2 		  = &lna_b39_up_2;
            pt_lna_4g_dw 		  = &lna_b39_dw;

            sp4t_ur  = SP4D_RU_UR_B39;
            sp4t_ut  = SP4D_RU_UT_B39;
            sp4t_ant = SP4D_RU_ANT_B39;
            sp4t_bts = SP4D_RU_BTS_B39;
            sp2t_ul  = SP2D_RU_UL_B39;
            sp2t_dl  = SP2D_RU_DL_B39;
            break;

        case BAND_40:
            pt_4g_dc_ctrl 		  = &rf_b40_dc_ctrl;

            pt_att_analog_4g_dw_1 = &att_analog_b40_dw_1;
            pt_att_analog_4g_dw_2 = &att_analog_b40_dw_2;

            pt_lna_4g_up_1 		  = &lna_b40_up_1;
            pt_lna_4g_up_2 		  = &lna_b40_up_2;
            pt_lna_4g_dw 		  = &lna_b40_dw;

            sp4t_ur  = SP4D_RU_UR_B40;
            sp4t_ut  = SP4D_RU_UT_B40;
            sp4t_ant = SP4D_RU_ANT_B40;
            sp4t_bts = SP4D_RU_BTS_B40;
            sp2t_ul  = SP2D_OFF;
            sp2t_dl  = SP2D_OFF;
            break;

        default:
            pt_4g_dc_ctrl 		  = NULL;
            pt_att_analog_4g_up   = NULL;
            pt_att_analog_4g_dw_1 = NULL;
            pt_att_analog_4g_dw_2 = NULL;
            pt_lna_4g_up_1 		  = NULL;
            pt_lna_4g_up_2 		  = NULL;
            pt_lna_4g_dw 		  = NULL;
            sp4t_ur  			  = SP4D_OFF;
            sp4t_ut  			  = SP4D_OFF;
            sp4t_ant 			  = SP4D_OFF;
            sp4t_bts 			  = SP4D_OFF;
            sp2t_ul  			  = SP2D_OFF;
            sp2t_dl  			  = SP2D_OFF;
            break;
    }

	//低噪放指针重定向
	lna_pt_init();	
    
    //设置开关状态
    BI_Switch_CTL(&sp4t_4g_up_rx, sp4t_ur);
    BI_Switch_CTL(&sp4t_4g_up_tx, sp4t_ut);
    BI_Switch_CTL(&sp4t_4g_ant, sp4t_ant);
    BI_Switch_CTL(&sp4t_4g_bts, sp4t_bts);

    BI_Switch_CTL(&spdt_b3_b39_up_ctrl, sp2t_ul);
    BI_Switch_CTL(&spdt_b3_b39_dw_ctrl, sp2t_dl);
}
#endif

