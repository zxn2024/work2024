/***************************** Copyright ****************************************
*
* (C) Copyright 2022, Shaanxi Tianji Communication
*  All Rights Reserved
*
* FileName   : bsp_rf_att.c
* Version    : none
* Author     : none
* Date       : 2022-08-03
* Description: none
*   MU:	ANT - LNA1 -   /  - A-ATT-FULL   -   /    - A_ATT_HALF - (POW1) - BTS
*   MD:	ANT - LNA1 - LNA2 - D_ATT1(8668) - (POW1) -     /      - (POW1) - BTS	B8频段, 没有LNA2
*   RU: ANT - LNA1 - LNA2 - D_ATT1(8668) - (POW1) - A_ATT_FUL  - (POW2) - BTS
*   RD: ANT - LNA1 -   /  - A-ATT-FULL   -   /    - A_ATT_HALF - (POW1) - BTS
*******************************************************************************/
#include "main.h"
#include "system_config.h"

#include "bsp_rf_att_set.h"

#include "bsp_dev_para.h"
#include "rtt.h"
#include "lna.h"
#include "bsp_rf_lna.h"
#include "bsp_rf_global.h"
#include "bsp_rf_att.h"
#include "bsp_para_load.h"
#include "bsp_rf_pow.h"
#include "bsp_rf_866x.h"

/*---------------------- 端口定义 --------------------------------------*/


/*---------------------- 宏定义  ---------------------------------------*/


/*---------------------- 结构体 ----------------------------------------*/


/*---------------------- 常量 ------------------------------------------*/
enum
{
	ATT_TYPE_DIGIT_OR_ANALOG = 0,
	ATT_TYPE_LNA,
};

/*---------------------- 全局变量 --------------------------------------*/

ATT_CH_T  t_4g_up_att;		//定义了所有通道上的衰减器
ATT_CH_T  t_4g_dw_att;
ATT_CH_T  t_5g_up_att;		//定义了所有通道上的衰减器
ATT_CH_T  t_5g_dw_att;
ATT_CH_T  t_5g_m_up_att;	//定义了所有通道上的衰减器
ATT_CH_T  t_5g_m_dw_att;

ATT_CH_T * pt_ch_att[RF_SYS_NUM][RF_CH_NUM];	//根据系统指向不同的衰减器


/*****************************************************************************
* @Name	  : att_struct_init
* @Brief  : 初始化 BSP_ATT_T 结构体数据
* @Param  : pt: [输入/出]
**			 en: [输入/出]
**			 max_num: [输入/出]
**			 value: [输入/出]
* @Retval :
* @Author :
* @Data   : 2023-11-01
* @Note   :
*****************************************************************************/
static void att_struct_init(BSP_ATT_T * pt, uint8_t type, uint8_t en, uint8_t max_num, uint8_t value)
{
    pt->en 		   = en;
    pt->new_value  = (ENABLE == pt->en) ? value : 0;
	if( ATT_TYPE_DIGIT_OR_ANALOG == type )
		pt->max_num    = (ENABLE == pt->en) ? max_num-1 : 0;
	else
		pt->max_num    = (ENABLE == pt->en) ? max_num : 0;
    pt->cur_value  = pt->new_value;
    pt->refresh    = B_TRUE;
}


/*****************************************************************************
* @Name	  : att_init
* @Brief  :
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2023-10-30
* @Note   :
*****************************************************************************/
void att_init(void)
{
    ATT_CH_T * pt = NULL;
	
	uint8_t lna_att1, lna_att2;		
	uint8_t digit_att1, digit_att2;
	uint8_t analog_att1, analog_att2;
	uint8_t band_4g = t_rf_sys[ RF_SYS_1 ].t_public.valid_band;
	
#if( LOCAL_MODULE_TYPE == _MU )
    pt = &t_4g_up_att;
	lna_att1 = get_rf_cali_para_lna_att( band_4g, RF_CH_UP, 1 );
	lna_att2 = 0;
	analog_att1 = analog_table_4g_up_1.valid_number;
	analog_att2 = analog_table_4g_up_2.valid_number;
	digit_att1 = 0;
	digit_att2 = 0;
    att_struct_init(&pt->lna1,   		ATT_TYPE_LNA, 			  ENABLE,   lna_att1,    LNA_BYS);
    att_struct_init(&pt->lna2,   		ATT_TYPE_LNA, 			  DISABLE,  lna_att2,    LNA_BYS);
    att_struct_init(&pt->digit_att1,    ATT_TYPE_DIGIT_OR_ANALOG, DISABLE,  digit_att1,  0);
    att_struct_init(&pt->digit_att2,    ATT_TYPE_DIGIT_OR_ANALOG, DISABLE,  digit_att2,  0);
    att_struct_init(&pt->analog_att1,   ATT_TYPE_DIGIT_OR_ANALOG, ENABLE,   analog_att1, 0);	//全
    att_struct_init(&pt->analog_att2, 	ATT_TYPE_DIGIT_OR_ANALOG, ENABLE,   analog_att2, 0);	//半, 半级仅用于校准和温补
	pt_att_analog_4g_up_1->hw_config.att_max_number = analog_att1;
	pt_att_analog_4g_up_2->hw_config.att_max_number = analog_att2;
	
    pt = &t_4g_dw_att;
	lna_att1 = get_rf_cali_para_lna_att( band_4g, RF_CH_DW, 1 );
	lna_att2 = get_rf_cali_para_lna_att( band_4g, RF_CH_DW, 2 );
	//analog_att1 = analog_table_4g_dw.valid_number;
	
	analog_att2 = 0;
	digit_att1 = BSP_866x_get_att_num( pt_866x_sys[RF_SYS_1] );
	digit_att2 = 0;
    att_struct_init(&pt->lna1,   		ATT_TYPE_LNA, 			  ENABLE,   lna_att1, LNA_BYS);
    att_struct_init(&pt->lna2,   		ATT_TYPE_LNA, 			  ENABLE,   lna_att2, LNA_BYS);
    att_struct_init(&pt->digit_att1, 	ATT_TYPE_DIGIT_OR_ANALOG, ENABLE,   digit_att1, 0);
    att_struct_init(&pt->digit_att2, 	ATT_TYPE_DIGIT_OR_ANALOG, DISABLE,  digit_att2,  0);
    att_struct_init(&pt->analog_att1, 	ATT_TYPE_DIGIT_OR_ANALOG, ENABLE,   analog_att1, 0);	//全
    att_struct_init(&pt->analog_att2, 	ATT_TYPE_DIGIT_OR_ANALOG, DISABLE,  analog_att2, 0);	//半
	att_analog_4g_dw.hw_config.att_max_number = pt->analog_att1.max_num;
	
    pt = &t_5g_up_att;
	lna_att1 = get_rf_cali_para_lna_att( BAND_41, RF_CH_UP, 1 );
	lna_att2 = 0;
	analog_att1 = analog_table_nr_up_1.valid_number;
	analog_att2 = analog_table_nr_up_2.valid_number;
	digit_att1 = 0;
	digit_att2 = 0;
    att_struct_init(&pt->lna1,   		ATT_TYPE_LNA, 			  ENABLE,   lna_att1, LNA_BYS);
    att_struct_init(&pt->lna2,   		ATT_TYPE_LNA, 			  DISABLE,  lna_att2,  LNA_BYS);
    att_struct_init(&pt->digit_att1, 	ATT_TYPE_DIGIT_OR_ANALOG, DISABLE,  digit_att1, 0);
    att_struct_init(&pt->digit_att2, 	ATT_TYPE_DIGIT_OR_ANALOG, DISABLE,  digit_att2,  0);
    att_struct_init(&pt->analog_att1, 	ATT_TYPE_DIGIT_OR_ANALOG, ENABLE,   analog_att1, 0);
    att_struct_init(&pt->analog_att2, 	ATT_TYPE_DIGIT_OR_ANALOG, ENABLE,   analog_att2, 0);
	att_analog_nr_up_1.hw_config.att_max_number = analog_att1;
	att_analog_nr_up_2.hw_config.att_max_number = analog_att2;
	
    pt = &t_5g_dw_att;
	lna_att1 = get_rf_cali_para_lna_att( BAND_41, RF_CH_DW, 1 );
	lna_att2 = get_rf_cali_para_lna_att( BAND_41, RF_CH_DW, 2 );
	analog_att1 = analog_table_nr_dw.valid_number;
	analog_att2 = 0;
	digit_att1 = BSP_866x_get_att_num( pt_866x_sys[RF_SYS_2] );
	digit_att2 = 0;
    att_struct_init(&pt->lna1,   		ATT_TYPE_LNA, 			  ENABLE,   lna_att1, LNA_BYS);
    att_struct_init(&pt->lna2,   		ATT_TYPE_LNA, 			  ENABLE,   lna_att2, LNA_BYS);
    att_struct_init(&pt->digit_att1, 	ATT_TYPE_DIGIT_OR_ANALOG, ENABLE,   digit_att1, 0);
    att_struct_init(&pt->digit_att2, 	ATT_TYPE_DIGIT_OR_ANALOG, DISABLE,  digit_att2,  0);
    att_struct_init(&pt->analog_att1, 	ATT_TYPE_DIGIT_OR_ANALOG, ENABLE,   analog_att1, 0);
    att_struct_init(&pt->analog_att2, 	ATT_TYPE_DIGIT_OR_ANALOG, DISABLE,  analog_att2, 0);
	att_analog_nr_dw.hw_config.att_max_number = pt->analog_att1.max_num;
	
    pt = &t_5g_m_up_att;
	lna_att1 = get_rf_cali_para_lna_att( BAND_41, RF_CH_UP, 1 );
	lna_att2 = 0;
	analog_att1 = analog_table_mimo_up_1.valid_number;
	analog_att2 = analog_table_mimo_up_2.valid_number;
	digit_att1 = 0;
	digit_att2 = 0;
    att_struct_init(&pt->lna1,   		ATT_TYPE_LNA, 			  ENABLE,   lna_att1, LNA_BYS);
    att_struct_init(&pt->lna2,   		ATT_TYPE_LNA, 			  DISABLE,  lna_att2,  LNA_BYS);
    att_struct_init(&pt->digit_att1, 	ATT_TYPE_DIGIT_OR_ANALOG, DISABLE,  digit_att1, 0);
    att_struct_init(&pt->digit_att2, 	ATT_TYPE_DIGIT_OR_ANALOG, DISABLE,  digit_att2,  0);
    att_struct_init(&pt->analog_att1, 	ATT_TYPE_DIGIT_OR_ANALOG, ENABLE,   analog_att1, 0);
    att_struct_init(&pt->analog_att2, 	ATT_TYPE_DIGIT_OR_ANALOG, ENABLE,   analog_att2, 0);
	att_analog_mimo_up_1.hw_config.att_max_number = analog_att1;
	att_analog_mimo_up_2.hw_config.att_max_number = analog_att2;
	
    pt = &t_5g_m_dw_att;
	lna_att1 = get_rf_cali_para_lna_att( BAND_41M, RF_CH_DW, 1 );
	lna_att2 = get_rf_cali_para_lna_att( BAND_41M, RF_CH_DW, 2 );
	analog_att1 = analog_table_mimo_dw.valid_number;
	analog_att2 = 0;
	digit_att1 = BSP_866x_get_att_num( pt_866x_sys[RF_SYS_3] );
	digit_att2 = 0;
    att_struct_init(&pt->lna1,   		ATT_TYPE_LNA, 			  ENABLE,   lna_att1, LNA_BYS);
    att_struct_init(&pt->lna2,   		ATT_TYPE_LNA, 			  ENABLE,   lna_att2, LNA_BYS);
    att_struct_init(&pt->digit_att1, 	ATT_TYPE_DIGIT_OR_ANALOG, ENABLE,   digit_att1, 0);
    att_struct_init(&pt->digit_att2, 	ATT_TYPE_DIGIT_OR_ANALOG, DISABLE,  digit_att2,  0);
    att_struct_init(&pt->analog_att1, 	ATT_TYPE_DIGIT_OR_ANALOG, ENABLE,   analog_att1, 0);
    att_struct_init(&pt->analog_att2, 	ATT_TYPE_DIGIT_OR_ANALOG, DISABLE,  analog_att2, 0);
	att_analog_nr_dw.hw_config.att_max_number = pt->analog_att1.max_num;

#else

    pt = &t_4g_up_att;
	lna_att1 = get_rf_cali_para_lna_att( band_4g, RF_CH_UP, 1 );
	lna_att2 = get_rf_cali_para_lna_att( band_4g, RF_CH_UP, 2 );
	analog_att1 = analog_table_4g_up.valid_number;
	analog_att2 = 0;
	digit_att1 = BSP_866x_get_att_num( pt_866x_sys[RF_SYS_1] );
	digit_att2 = 0;
    att_struct_init(&pt->lna1,   		ATT_TYPE_LNA, 			  ENABLE,   lna_att1,  LNA_BYS);
    att_struct_init(&pt->lna2,   		ATT_TYPE_LNA, 			  ENABLE,   lna_att2,  LNA_BYS);
    att_struct_init(&pt->digit_att1,    ATT_TYPE_DIGIT_OR_ANALOG, ENABLE,   digit_att1, 0);
    att_struct_init(&pt->digit_att2,    ATT_TYPE_DIGIT_OR_ANALOG, DISABLE,  digit_att2,  0);
    att_struct_init(&pt->analog_att1,   ATT_TYPE_DIGIT_OR_ANALOG, ENABLE,   analog_att1, 0);	//全
    att_struct_init(&pt->analog_att2, 	ATT_TYPE_DIGIT_OR_ANALOG, DISABLE,  analog_att2, 0);	//半

    pt = &t_4g_dw_att;
	lna_att1 = get_rf_cali_para_lna_att( band_4g, RF_CH_DW, 1 );
	lna_att2 = 0;
	analog_att1 = analog_table_4g_dw_1.valid_number;
	analog_att2 = analog_table_4g_dw_2.valid_number;
    digit_att1 = 0;
	digit_att2 = 0;
    att_struct_init(&pt->lna1,   		ATT_TYPE_LNA, 			  ENABLE,   lna_att1, LNA_BYS);
    att_struct_init(&pt->lna2,   		ATT_TYPE_LNA, 			  DISABLE,  lna_att2, LNA_BYS);
    att_struct_init(&pt->digit_att1, 	ATT_TYPE_DIGIT_OR_ANALOG, DISABLE,  digit_att1, 0);
    att_struct_init(&pt->digit_att2, 	ATT_TYPE_DIGIT_OR_ANALOG, DISABLE,  digit_att2,  0);
    att_struct_init(&pt->analog_att1, 	ATT_TYPE_DIGIT_OR_ANALOG, ENABLE,   analog_att1, 0);	//全
    att_struct_init(&pt->analog_att2, 	ATT_TYPE_DIGIT_OR_ANALOG, DISABLE,  analog_att2, 0);	//半

    pt = &t_5g_up_att;
	lna_att1 = get_rf_cali_para_lna_att( BAND_41, RF_CH_UP, 1 );
	lna_att2 = get_rf_cali_para_lna_att( BAND_41, RF_CH_UP, 2 );
	analog_att1 = analog_table_nr_up.valid_number;
	analog_att2 = 0;
	digit_att1 = BSP_866x_get_att_num( pt_866x_sys[RF_SYS_2] );
	digit_att2 = 0;
    att_struct_init(&pt->lna1,   		ATT_TYPE_LNA, 			  ENABLE,   lna_att1, LNA_BYS);
    att_struct_init(&pt->lna2,   		ATT_TYPE_LNA, 			  ENABLE,   lna_att2,  LNA_BYS);
    att_struct_init(&pt->digit_att1, 	ATT_TYPE_DIGIT_OR_ANALOG, ENABLE,   digit_att1, 0);
    att_struct_init(&pt->digit_att2, 	ATT_TYPE_DIGIT_OR_ANALOG, DISABLE,  digit_att2,  0);
    att_struct_init(&pt->analog_att1, 	ATT_TYPE_DIGIT_OR_ANALOG, ENABLE,   analog_att1, 0);
    att_struct_init(&pt->analog_att2, 	ATT_TYPE_DIGIT_OR_ANALOG, DISABLE,  analog_att2, 0);

    pt = &t_5g_dw_att;
	lna_att1 = get_rf_cali_para_lna_att( BAND_41, RF_CH_DW, 1 );
	lna_att2 = 0;
	analog_att1 = analog_table_nr_dw_1.valid_number;
	analog_att2 = analog_table_nr_dw_2.valid_number;
    digit_att1 = 0;
	digit_att2 = 0;
    att_struct_init(&pt->lna1,   		ATT_TYPE_LNA, 			  ENABLE,   lna_att1, LNA_BYS);
    att_struct_init(&pt->lna2,   		ATT_TYPE_LNA, 			  DISABLE,  lna_att2, LNA_BYS);
    att_struct_init(&pt->digit_att1, 	ATT_TYPE_DIGIT_OR_ANALOG, DISABLE,  digit_att1, 0);
    att_struct_init(&pt->digit_att2, 	ATT_TYPE_DIGIT_OR_ANALOG, DISABLE,  digit_att2,  0);
    att_struct_init(&pt->analog_att1, 	ATT_TYPE_DIGIT_OR_ANALOG, ENABLE,   analog_att1, 0);
    att_struct_init(&pt->analog_att2, 	ATT_TYPE_DIGIT_OR_ANALOG, DISABLE,  analog_att2, 0);

    pt = &t_5g_m_up_att;
	lna_att1 = get_rf_cali_para_lna_att( BAND_41M, RF_CH_UP, 1 );
	lna_att2 = get_rf_cali_para_lna_att( BAND_41M, RF_CH_UP, 2 );
	analog_att1 = analog_table_mimo_up.valid_number;
	analog_att2 = 0;
	digit_att1 = BSP_866x_get_att_num( pt_866x_sys[RF_SYS_3] );
	digit_att2 = 0;
    att_struct_init(&pt->lna1,   		ATT_TYPE_LNA, 			  ENABLE,   lna_att1, LNA_BYS);
    att_struct_init(&pt->lna2,   		ATT_TYPE_LNA, 			  ENABLE,   lna_att2, LNA_BYS);
    att_struct_init(&pt->digit_att1, 	ATT_TYPE_DIGIT_OR_ANALOG, ENABLE,   digit_att1, 0);
    att_struct_init(&pt->digit_att2, 	ATT_TYPE_DIGIT_OR_ANALOG, DISABLE,  digit_att2,  0);
    att_struct_init(&pt->analog_att1, 	ATT_TYPE_DIGIT_OR_ANALOG, ENABLE,   analog_att1, 0);
    att_struct_init(&pt->analog_att2, 	ATT_TYPE_DIGIT_OR_ANALOG, DISABLE,  analog_att2, 0);

    pt = &t_5g_m_dw_att;
	lna_att1 = get_rf_cali_para_lna_att( BAND_41M, RF_CH_DW, 1 );
	lna_att2 = 0;
	analog_att1 = analog_table_mimo_dw_1.valid_number;
	analog_att2 = analog_table_mimo_dw_2.valid_number;
    digit_att1 = 0;
	digit_att2 = 0;
    att_struct_init(&pt->lna1,   		ATT_TYPE_LNA, 			  ENABLE,   lna_att1, LNA_BYS);
    att_struct_init(&pt->lna2,   		ATT_TYPE_LNA, 			  DISABLE,  lna_att2, LNA_BYS);
    att_struct_init(&pt->digit_att1, 	ATT_TYPE_DIGIT_OR_ANALOG, DISABLE,  digit_att1, 0);
    att_struct_init(&pt->digit_att2, 	ATT_TYPE_DIGIT_OR_ANALOG, DISABLE,  digit_att2,  0);
    att_struct_init(&pt->analog_att1, 	ATT_TYPE_DIGIT_OR_ANALOG, ENABLE,   analog_att1, 0);
    att_struct_init(&pt->analog_att2, 	ATT_TYPE_DIGIT_OR_ANALOG, DISABLE,  analog_att2, 0);
#endif

    pt_ch_att[RF_SYS_1][RF_CH_UP] = &t_4g_up_att;
    pt_ch_att[RF_SYS_1][RF_CH_DW] = &t_4g_dw_att;
    pt_ch_att[RF_SYS_2][RF_CH_UP] = &t_5g_up_att;
    pt_ch_att[RF_SYS_2][RF_CH_DW] = &t_5g_dw_att;
    pt_ch_att[RF_SYS_3][RF_CH_UP] = &t_5g_m_up_att;
    pt_ch_att[RF_SYS_3][RF_CH_DW] = &t_5g_m_dw_att;
}


/*****************************************************************************
* @Name	  : att_set_fresh_flag
* @Brief  : 设置衰减器衰减标志
* @Param  : pt_hal: [输入/出]
**			 pt_val: [输入/出]
* @Retval :
* @Author :
* @Data   : 2023-11-20
* @Note   :
*****************************************************************************/
static void att_set_fresh_flag(ATT_CH_T * pt_hal)
{
    if(pt_hal->analog_att1.cur_value != pt_hal->analog_att1.new_value)
        pt_hal->analog_att1.refresh = B_TRUE;

    if(pt_hal->analog_att2.cur_value != pt_hal->analog_att2.new_value)
        pt_hal->analog_att2.refresh = B_TRUE;

    if(pt_hal->digit_att1.cur_value != pt_hal->digit_att1.new_value)
        pt_hal->digit_att1.refresh = B_TRUE;

    if(pt_hal->digit_att2.cur_value != pt_hal->digit_att2.new_value)
        pt_hal->digit_att2.refresh = B_TRUE;

    if(pt_hal->lna1.cur_value != pt_hal->lna1.new_value)
        pt_hal->lna1.refresh = B_TRUE;

    if(pt_hal->lna2.cur_value != pt_hal->lna2.new_value)
        pt_hal->lna2.refresh = B_TRUE;
}


/*****************************************************************************
* @Name	  : att_set_force_fresh
* @Brief  : 设置衰减器后重新8668增益,
* @Param  : sys: [输入/出]
* @Retval :
* @Author :
* @Data   : 2024-03-16
* @Note   :
*****************************************************************************/
void att_set_force_fresh( uint8_t sys )
{
	t_rf_sys[sys].t_ch[RF_CH_UP].tAtt.ucAlc       = pt_ch_att[sys][RF_CH_UP]->analog_att1.max_num;
	t_rf_sys[sys].t_ch[RF_CH_DW].tAtt.uc8668Att   = pt_ch_att[sys][RF_CH_DW]->digit_att1.max_num;
	pt_ch_att[sys][RF_CH_UP]->lna1.cur_value 	  = LNA_RF;
	pt_ch_att[sys][RF_CH_UP]->lna2.cur_value 	  = LNA_RF;
	pt_ch_att[sys][RF_CH_DW]->lna1.cur_value 	  = LNA_RF;
	pt_ch_att[sys][RF_CH_DW]->lna2.cur_value 	  = LNA_RF;
	
	pt_ch_att[sys][RF_CH_UP]->digit_att1.refresh  = B_TRUE;
	pt_ch_att[sys][RF_CH_UP]->digit_att2.refresh  = B_TRUE;
	pt_ch_att[sys][RF_CH_UP]->analog_att1.refresh = B_TRUE;
	pt_ch_att[sys][RF_CH_UP]->analog_att2.refresh = B_TRUE;
	pt_ch_att[sys][RF_CH_UP]->lna1.refresh 		  = B_TRUE;
	pt_ch_att[sys][RF_CH_UP]->lna2.refresh 		  = B_TRUE;
	pt_ch_att[sys][RF_CH_DW]->digit_att1.refresh  = B_TRUE;
	pt_ch_att[sys][RF_CH_DW]->digit_att2.refresh  = B_TRUE;
	pt_ch_att[sys][RF_CH_DW]->analog_att1.refresh = B_TRUE;
	pt_ch_att[sys][RF_CH_DW]->analog_att2.refresh = B_TRUE;
	pt_ch_att[sys][RF_CH_DW]->lna1.refresh 		  = B_TRUE;
	pt_ch_att[sys][RF_CH_DW]->lna2.refresh 		  = B_TRUE;
}

///*****************************************************************************
//* @Name	  : att_set_fresh_flag
//* @Brief  : 设置衰减器衰减标志
//* @Param  : pt_hal: [输入/出]
//**			pt_val: [输入/出]
//* @Retval :
//* @Author :
//* @Data   : 2023-11-20
//* @Note   :
//*****************************************************************************/
//static void att_set_fresh_flag( ATT_CH_T *pt_hal )
//{
//	if( pt_hal->analog_att1.cur_value != pt_hal->analog_att1.new_value )
//		pt_hal->analog_att1.refresh = B_TRUE;
//
//	if( pt_hal->analog_att2.cur_value != pt_hal->analog_att2.new_value )
//		pt_hal->analog_att2.refresh = B_TRUE;

//	if( pt_hal->digit_att1.cur_value != pt_hal->digit_att1.new_value )
//		pt_hal->digit_att1.refresh = B_TRUE;
//
//	if( pt_hal->digit_att2.cur_value != pt_hal->digit_att2.new_value )
//		pt_hal->digit_att2.refresh = B_TRUE;

//	if( pt_hal->lna1.cur_value != pt_hal->lna1.new_value )
//		pt_hal->lna1.refresh = B_TRUE;

//	if( pt_hal->lna2.cur_value != pt_hal->lna2.new_value )
//		pt_hal->lna2.refresh = B_TRUE;
//}


/*****************************************************************************
* @Name	  : att_clr_fresh_flag
* @Brief  : 清除衰减器衰减标志
* @Param  : pt_hal: [输入/出]
**			pt_val: [输入/出]
* @Retval :
* @Author :
* @Data   : 2023-11-20
* @Note   :
*****************************************************************************/
static void att_clr_fresh_flag(ATT_CH_T * pt_hal)
{
    pt_hal->analog_att1.refresh = B_FALSE;
    pt_hal->analog_att2.refresh = B_FALSE;
    pt_hal->digit_att1.refresh  = B_FALSE;
    pt_hal->digit_att2.refresh  = B_FALSE;
    pt_hal->lna1.refresh     	= B_FALSE;
    pt_hal->lna2.refresh 		= B_FALSE;
}


/*****************************************************************************
* @Name	  : att_update_cut_value
* @Brief  : 更新衰减器当前值
* @Param  : pt_hal: [输入/出]
**			pt_val: [输入/出]
* @Retval :
* @Author :
* @Data   : 2023-11-20
* @Note   :
*****************************************************************************/
static void att_update_cut_value(ATT_CH_T * pt_hal)
{
    pt_hal->analog_att1.cur_value = pt_hal->analog_att1.new_value;
    pt_hal->analog_att2.cur_value = pt_hal->analog_att2.new_value;
    pt_hal->digit_att1.cur_value  = pt_hal->digit_att1.new_value;
    pt_hal->digit_att2.cur_value  = pt_hal->digit_att2.new_value;
    pt_hal->lna1.cur_value 		  = pt_hal->lna1.new_value;
    pt_hal->lna2.cur_value        = pt_hal->lna2.new_value;
}


/*****************************************************************************
* @Name	  : att_calc_for_dw/up
* @Brief  : 计算上行/下行衰减器
* @Param  : pt_hal: 通道上的衰减器指针
**			 pt_val: [输入/出]
* @Retval :
* @Author :
* @Data   : 2023-11-09
* @Note   :
*****************************************************************************/
#if( LOCAL_MODULE_TYPE == _MU )
	#define ATT_8668_PRE	( 10 )
    static void att_calc_for_dw(ATT_CH_T * pt_hal, Att_T * pt_val)
#else
	#define ATT_8668_PRE	( 20 )
    static void att_calc_for_up(ATT_CH_T * pt_hal, Att_T * pt_val)
#endif
{
    //计算模拟衰减器上需要设置的衰减值
    uint8_t analog_res = 0;	//平衡8668后可用于ALC的衰减
    int8_t  analog_set_pre = 0;		//作用于模拟衰减器的温补数据和8668数据
    uint8_t analog_set_alc  = 0;	//作用于模拟衰减器的alc数据
	
	if( pt_hal->lna1.cur_value == LNA_RF && pt_val->ucIso < 10)	//低噪放未旁路且隔离度衰减小于10
	{
		analog_res = (pt_val->uc8668Att <= ATT_8668_PRE) ? (ATT_8668_PRE - pt_val->uc8668Att) : 0;	//平衡到后级模拟衰减器上的8668衰减
		analog_set_pre = pt_val->ucPresent + pt_val->scTempOffset + analog_res - pt_val->ucManuCal;	//温补 + 8668平衡衰减
		analog_set_pre = ( analog_set_pre >= 0 ) ? analog_set_pre : 0;
	}
	else	//低噪放已经旁路
	{
		analog_res 	   = 0;
		analog_set_pre = pt_val->ucPresent + pt_val->scTempOffset;
	}
	
    if(analog_set_pre < pt_hal->analog_att1.max_num)	//作用于模拟衰减器上的alc数据
    {
        analog_set_alc = (pt_val->ucAlc > (pt_hal->analog_att1.max_num - analog_set_pre)) ? (pt_hal->analog_att1.max_num - analog_set_pre) : pt_val->ucAlc;
    }
	
    pt_hal->analog_att1.new_value = analog_set_pre + analog_set_alc;
    pt_hal->analog_att1.new_value = (pt_hal->analog_att1.new_value <= pt_hal->analog_att1.max_num) ? pt_hal->analog_att1.new_value : pt_hal->analog_att1.max_num;

    //计算与LNA联动后各衰减的设置值
    uint8_t att_split = pt_val->scCaliGainOffset		//att_splite: 需要与LNA联动的总衰减数量
        + pt_val->uc8668Att
        + pt_val->ucIso
//        + pt_val->ucManuCal
        + pt_val->ucUpDownLink;

    if(att_split >= (pt_hal->digit_att1.max_num + pt_hal->lna1.max_num + pt_hal->lna2.max_num))
        att_split = pt_hal->digit_att1.max_num + pt_hal->lna1.max_num + pt_hal->lna2.max_num;

    if((att_split <= pt_hal->lna1.max_num) && (ENABLE == pt_hal->lna1.en))
    {
        pt_hal->lna1.new_value 	 	 = LNA_RF;
        pt_hal->lna2.new_value 	 	 = LNA_RF;
        pt_hal->digit_att1.new_value = att_split;
    }
    else if((att_split <= (pt_hal->lna1.max_num + 2)) && (ENABLE == pt_hal->lna1.en))
    {
        if(LNA_RF == pt_hal->lna1.new_value)	//lna1 to rf
        {
            pt_hal->lna1.new_value 	 	 = LNA_RF;
            pt_hal->lna2.new_value 	 	 = LNA_RF;
            pt_hal->digit_att1.new_value = att_split;
        }
        else	//lna1 to bypass
        {
            pt_hal->lna1.new_value 	 	 = LNA_BYS;
            pt_hal->lna2.new_value 	 	 = LNA_RF;
            pt_hal->digit_att1.new_value = att_split - pt_hal->lna1.max_num;
        }
//        pt_hal->analog_att1.new_value = ((ENABLE == pt_hal->analog_att1.en) && (pt_hal->analog_att1.max_num >= att_split)) ? att_split : 0;
    }
    else if((att_split <= (pt_hal->lna1.max_num + pt_hal->lna2.max_num)) && (ENABLE == pt_hal->lna2.en))
    {
        pt_hal->lna1.new_value 	 	 = LNA_BYS;
        pt_hal->lna2.new_value 	 	 = LNA_RF;
        pt_hal->digit_att1.new_value = att_split - pt_hal->lna1.max_num;
    }
    else if((att_split <= (pt_hal->lna1.max_num + pt_hal->lna2.max_num + 2)) && (ENABLE == pt_hal->lna2.en))
    {
        if(pt_hal->lna2.new_value == LNA_RF)	//lna1 on rf
        {
            pt_hal->lna1.new_value 	 	 = LNA_BYS;
            pt_hal->lna2.new_value 	 	 = LNA_RF;
            pt_hal->digit_att1.new_value = att_split - pt_hal->lna1.max_num;
        }
        else	//lna1 on bypass
        {
            pt_hal->lna1.new_value 	 	 = LNA_BYS;
            pt_hal->lna2.new_value 	 	 = LNA_BYS;
            pt_hal->digit_att1.new_value = att_split - pt_hal->lna1.max_num - pt_hal->lna2.max_num;
        }
    }
    else
    {
        pt_hal->lna1.new_value 	 	 = LNA_BYS;
        pt_hal->lna2.new_value 	 	 = LNA_BYS;
        pt_hal->digit_att1.new_value = att_split - pt_hal->lna1.max_num - pt_hal->lna2.max_num;
    }

    pt_hal->digit_att2.new_value  = 0;
    pt_hal->digit_att2.cur_value  = 0;
    pt_hal->analog_att2.new_value = 0;
    pt_hal->analog_att2.cur_value = 0;

    att_set_fresh_flag(pt_hal);
}


/*****************************************************************************
* @Name	  : att_calc_for_up
* @Brief  : 设置衰减器
* @Param  : pt_hal: [输入/出]
**			 pt_val: [输入/出]
* @Retval :
* @Author :
* @Data   : 2023-11-01
* @Note   :
*****************************************************************************/
#if( LOCAL_MODULE_TYPE == _MU )
    static void att_calc_for_up(ATT_CH_T * pt_hal, Att_T * pt_val)
#else
    static void att_calc_for_dw(ATT_CH_T * pt_hal, Att_T * pt_val)
#endif
{
	#if( LOCAL_DEVICE_TYPE == _DTX )
		#define LNA_SWITCH_GATE	( 25 )	
	#else
		#define LNA_SWITCH_GATE ( 15 )
	#endif
    //计算校准数据,第二级模拟衰减器只用于校准+温补+预置衰减+手动调整
    int8_t att_cal = pt_val->ucPresent + pt_val->scCaliGainOffset + pt_val->scTempOffset - pt_val->ucManuCal;

    uint8_t swap_att = 0;
    
    if(pt_hal->lna1.max_num > LNA_SWITCH_GATE)   // 判断LNA1是否大于17
    {
        // 计算提前交换多少dB
        if((pt_val->ucPresent - 3) >= 15)
        {
            swap_att = 15;
        }
        else
        {
            swap_att = pt_val->ucPresent - 3;
        }

        //计算与LNA联动后各衰减的设置值
        uint8_t att_splite = pt_val->ucAlc
            + pt_val->ucIso
//            + pt_val->ucManuCal
            + pt_val->ucUpDownLink;

        if(att_splite >= (pt_hal->analog_att1.max_num + pt_hal->lna1.max_num))
            att_splite = pt_hal->analog_att1.max_num + pt_hal->lna1.max_num;

        if((att_splite + swap_att) <= pt_hal->lna1.max_num)
        {
            pt_hal->lna1.new_value 	      = LNA_RF;
            pt_hal->analog_att1.new_value = att_splite;
        }
        else if((att_splite + swap_att) <= (pt_hal->lna1.max_num + 2))
        {
            if(pt_hal->lna1.new_value == LNA_RF)	//lna1 to rf
            {
                pt_hal->lna1.new_value 	 	  = LNA_RF;
                pt_hal->analog_att1.new_value = att_splite;
            }
            else	//lna1 to bypass
            {
                pt_hal->lna1.new_value 	 	 = LNA_BYS; 
                pt_hal->analog_att1.new_value = (att_splite + swap_att) - pt_hal->lna1.max_num;
                att_cal -= swap_att;
            }
            pt_hal->analog_att1.new_value = ((ENABLE == pt_hal->analog_att1.en) && (pt_hal->analog_att1.max_num >= pt_hal->analog_att1.new_value)) ? pt_hal->analog_att1.new_value : pt_hal->analog_att1.max_num;
        }
        else
        {
            pt_hal->lna1.new_value 	 	  = LNA_BYS;
            pt_hal->analog_att1.new_value = (att_splite + swap_att) - pt_hal->lna1.max_num;
            att_cal -= swap_att;
            
            pt_hal->analog_att1.new_value = ((ENABLE == pt_hal->analog_att1.en) && (pt_hal->analog_att1.max_num >= pt_hal->analog_att1.new_value)) ? pt_hal->analog_att1.new_value : pt_hal->analog_att1.max_num;
        }
        if(att_cal < 0)
            att_cal = 0;
        
        pt_hal->analog_att2.new_value = att_cal;
        
        if(att_cal >= pt_hal->analog_att2.max_num)
            att_cal = pt_hal->analog_att2.max_num;
    }
    else
    {
        //计算与LNA联动后各衰减的设置值
        uint8_t att_splite = pt_val->ucAlc
            + pt_val->ucIso
//            + pt_val->ucManuCal
            + pt_val->ucUpDownLink;

        if(att_splite >= (pt_hal->analog_att1.max_num + pt_hal->lna1.max_num))
            att_splite = pt_hal->analog_att1.max_num + pt_hal->lna1.max_num;

        if(att_splite <= pt_hal->lna1.max_num)
        {
            pt_hal->lna1.new_value 	      = LNA_RF;
            pt_hal->analog_att1.new_value = att_splite;
        }
        else if(att_splite <= (pt_hal->lna1.max_num + 2))
        {
            if(pt_hal->lna1.new_value == LNA_RF)	//lna1 to rf
            {
                pt_hal->lna1.new_value 	 	  = LNA_RF;
                pt_hal->analog_att1.new_value = att_splite;
            }
            else	//lna1 to bypass
            {
                pt_hal->lna1.new_value 	 	 = LNA_BYS;
                pt_hal->analog_att1.new_value = att_splite - pt_hal->lna1.max_num;
            }
            pt_hal->analog_att1.new_value = ((ENABLE == pt_hal->analog_att1.en) && (pt_hal->analog_att1.max_num >= pt_hal->analog_att1.new_value)) ? pt_hal->analog_att1.new_value : pt_hal->analog_att1.max_num;
        }
        else
        {
            pt_hal->lna1.new_value 	 	  = LNA_BYS;
            pt_hal->analog_att1.new_value  = att_splite - pt_hal->lna1.max_num - pt_hal->lna2.max_num;
            pt_hal->analog_att1.new_value = ((ENABLE == pt_hal->analog_att1.en) && (pt_hal->analog_att1.max_num >= pt_hal->analog_att1.new_value)) ? pt_hal->analog_att1.new_value : pt_hal->analog_att1.max_num;
        }
		
		pt_hal->analog_att2.new_value = att_cal;
        
        if(att_cal >= pt_hal->analog_att2.max_num)
            att_cal = pt_hal->analog_att2.max_num;
    }


    pt_hal->lna2.new_value        = LNA_RF;	
    pt_hal->lna2.cur_value        = LNA_RF;
    pt_hal->digit_att1.new_value  = 0;
    pt_hal->digit_att1.cur_value  = 0;
    pt_hal->digit_att2.new_value  = 0;
    pt_hal->digit_att2.cur_value  = 0;
	
    att_set_fresh_flag(pt_hal);
}


/*****************************************************************************
* @Name	  : att_hal_set_action
* @Brief  : 设置衰减器的实际操作
* @Param  : sys: [输入/出]
**			 ch: [输入/出]
**			 pt_hal: [输入/出]
* @Retval :
* @Author :
* @Data   : 2023-11-01
* @Note   :
*****************************************************************************/
static void att_hal_set_action(uint8_t sys, uint8_t ch, ATT_CH_T * pt_hal)
{
    if((ENABLE == pt_hal->digit_att1.en) && (B_TRUE == pt_hal->digit_att1.refresh))
    {
        att_digit_set( sys, ch, pt_hal->digit_att1.new_value );
    }

    if((ENABLE == pt_hal->digit_att2.en) && (B_TRUE == pt_hal->digit_att2.refresh))
    {
        //添加8668衰减控制程序, 本软件中无效
    }

    if((ENABLE == pt_hal->lna1.en) && (B_TRUE == pt_hal->lna1.refresh))
    {
        lna_set(sys, ch, 1, pt_hal->lna1.new_value);
    }

    if((ENABLE == pt_hal->lna2.en) && (B_TRUE == pt_hal->lna2.refresh))
    {
        lna_set(sys, ch, 2, pt_hal->lna2.new_value);
    }

    if((ENABLE == pt_hal->analog_att1.en) && (B_TRUE == pt_hal->analog_att1.refresh))
    {
        att_analog_set( sys, ch, 1, pt_hal->analog_att1.new_value );
    }

    if((ENABLE == pt_hal->analog_att2.en) && (B_TRUE == pt_hal->analog_att2.refresh))
    {
        att_analog_set( sys, ch, 2, pt_hal->analog_att2.new_value );
    }

    att_update_cut_value(pt_hal);
    att_clr_fresh_flag(pt_hal);
}


/*****************************************************************************
* @Name	  : att_set
* @Brief  : 设置衰减器
* @Param  : sys: [输入/出]
**			ch: [输入/出]
* @Retval :
* @Author :
* @Data   : 2023-11-01
* @Note   :
*****************************************************************************/
void att_set(uint8_t sys, uint8_t ch)
{
	static uint8_t cnt = 0;
    if((sys >= RF_SYS_NUM) || (ch >= RF_CH_NUM))
        return;

    ATT_CH_T * pt_hal = pt_ch_att[sys][ch];
    Att_T * pt_val    = &t_rf_sys[sys].t_ch[ch].tAtt;

    switch(ch)
    {
        case RF_CH_UP:
            att_calc_for_up(pt_hal, pt_val);
			if( RF_SYS_1 == sys )
			{
//				rtt_printf( RTT_RF, "up_att_set: att1 = %d, lna 1 = %d, att2 = %d\n", pt_hal->analog_att1.cur_value, pt_hal->lna1.cur_value, pt_hal->analog_att2.cur_value );
			}
            break;

        case RF_CH_DW:
            att_calc_for_dw(pt_hal, pt_val);
			if(( RF_SYS_1 == sys ) && (RF_CH_DW == ch))
			{   
				cnt++;
				if( cnt == 10 )
				{
					cnt = 0;
//					rtt_printf( 9, "pow_alc = %04d - %02d, pow_8668 = %04d - %02d, lna1 = %d, lna2 = %d, att_8668 = %02d, att_alc = %02d\n", t_ch_pow[sys][ch].ptAdc->usAdcAve,t_ch_pow[sys][ch].pow.ave,t_ch_pow_8668[sys].ptAdc->usAdcAve,t_ch_pow_8668[sys].pow.ave, pt_hal->lna1.cur_value, pt_hal->lna2.cur_value, pt_hal->digit_att1.cur_value, pt_hal->analog_att1.cur_value ); 
				}
			}
            break;

        default:
            break;
    }

    att_hal_set_action(sys, ch, pt_hal);
}


/*****************************************************************************
* @Name	  : get_att_num_can_use_for_alc
* @Brief  : 获取可供ALC使用的ATT的剩余数量
* @Param  : sys: [输入/出]
**			 ch: [输入/出]
* @Retval :
* @Author :
* @Data   : 2023-11-01
* @Note   :
*****************************************************************************/
uint8_t get_att_num_can_use_for_alc(uint8_t sys, uint8_t ch)
{
    uint8_t att_sum = 0;
    uint8_t att_use = 0;
    uint8_t res = 0;

    ATT_CH_T * pt_hal = pt_ch_att[sys][ch];
    Att_T   *  pt_val = &t_rf_sys[sys].t_ch[ch].tAtt;

#if( LOCAL_MODULE_TYPE == _MU )
    if(RF_CH_DW == ch)
#else
    if(RF_CH_UP == ch)
#endif
    {
        att_sum = pt_hal->analog_att1.max_num;
        att_use = pt_hal->analog_att1.cur_value;
    }
    else
    {
        att_sum = pt_hal->analog_att1.max_num + pt_hal->lna1.max_num;
        att_use = pt_val->ucAlc
            + pt_val->ucIso
            + pt_val->ucManuCal
            + pt_val->ucPresent
            + pt_val->ucUpDownLink;
    }

    res = (att_sum > att_use) ? (att_sum - att_use) : 0;

    return res;
}


/*****************************************************************************
* @Name	  : get_att_num_can_use_for_8668
* @Brief  : 获取8668可使用的衰减器剩余数量
* @Param  : sys: [输入/出]
**			 ch: [输入/出]
* @Retval :
* @Author :
* @Data   : 2023-11-09
* @Note   :
*****************************************************************************/
uint8_t get_att_num_can_use_for_8668(uint8_t sys)
{
    uint8_t att_sum = 0;
    uint8_t att_use = 0;
    uint8_t res = 0;

    if(sys > RF_SYS_NUM)
        return 0;

    //仅限于主机的下行 或者从机的上行
#if( LOCAL_MODULE_TYPE == _MU )
    uint8_t ch = RF_CH_DW;
#else
    uint8_t ch = RF_CH_UP;
#endif

    ATT_CH_T * pt_hal = pt_ch_att[sys][ch];
    Att_T   *  pt_val = &t_rf_sys[sys].t_ch[ch].tAtt;

//    att_sum = pt_hal->digit_att1.cur_value + pt_hal->lna1.max_num - pt_hal->lna2.cur_value;
	att_sum = pt_hal->digit_att1.max_num + pt_hal->lna1.max_num + pt_hal->lna2.max_num;
    att_use = pt_val->scCaliGainOffset
				+ pt_val->scTempOffset
				+ pt_val->uc8668Att
				+ pt_val->ucIso
				+ pt_val->ucManuCal
				+ pt_val->ucPresent
				+ pt_val->ucUpDownLink;

    res = (att_sum > att_use) ? (att_sum - att_use) : 0;

    return res;
}

/**
* @brief   所有att设置为最大值   
* @param   sys: [输入/出] 
* @return    
* @data    2024-04-06
* @note    
*/
void att_all_set_max_value( uint8_t sys )
{
	ATT_CH_T * pt = pt_ch_att[sys][RF_CH_UP];
	
	for( RF_CH_E ch = RF_CH_UP; ch<RF_CH_DW; ch++ )
	{
		pt = pt_ch_att[sys][ch];
		pt->analog_att1.new_value = pt->analog_att1.max_num;
		pt->analog_att2.new_value = pt->analog_att2.max_num;
		pt->digit_att1.new_value  = pt->digit_att1.max_num;
		pt->digit_att2.new_value  = pt->digit_att2.max_num;
		pt->lna1.new_value        = LNA_BYS;
		pt->lna2.new_value        = LNA_BYS;
		
		pt->analog_att1.refresh   = B_TRUE;
		pt->analog_att2.refresh   = B_TRUE;
		pt->digit_att1.refresh    = B_TRUE;
		pt->digit_att2.refresh    = B_TRUE;
		pt->lna1.refresh          = B_TRUE;
		pt->lna2.refresh          = B_TRUE;
		
		att_set( sys, ch );
	}
}


