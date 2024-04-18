/***************************** Copyright ****************************************
*
* (C) Copyright 2022, Shaanxi Tianji Communication
*  All Rights Reserved
*
* FileName   : bsp_rf_oplc.c
* Version    : none
* Author     : ZhangWei
* Date       : 2022-08-09
* Description: none
//不同制式下开环工控的基站参数:
//900M:               bs_max_out_pow = 55       bs_min_in_pow = -40
*******************************************************************************/
#include "main.h"
#include "system_config.h"
#include "bsp_rf_oplc.h"
#include "bsp_rf_global.h"
#include "bsp_para_load.h"
#include "bsp_rf_4g_band_sel.h"

/*---------------------- 端口定义 --------------------------------------*/


/*---------------------- 宏定义  ---------------------------------------*/


/*---------------------- 结构体 ----------------------------------------*/


/*---------------------- 常量 ------------------------------------------*/


/*---------------------- 全局变量 --------------------------------------*/
OPLC_PARA_T tOplc[ RF_SYS_NUM ];

/*---------------------- 局部变量 --------------------------------------*/


/*---------------------- 声明 ------------------------------------------*/




/*****************************************************************************
* @Name	  : oplc_init
* @Brief  :
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2023-11-14
* @Note   :
*****************************************************************************/
void oplc_init(void)
{
    uint8_t sys;
    OPLC_PARA_T * pt = NULL;

    for(sys = 0; sys < RF_SYS_NUM; sys++)
    {
        pt = &tOplc[sys];
        memset(&tOplc[sys], 0, sizeof(OPLC_PARA_T));
		
        pt->pt_en = &t_rf_sys[ sys ].t_public.opleEn;
        pt->pt_pow_point = &t_rf_sys[ sys ].t_ch[RF_CH_UP].tAlc.scOplcPowPoint;
        pt->pt_rsrp = &t_rf_sys[ sys ].t_public.tNet.rsrp;
        //pt->pt_att_rsrp = &t_rf_sys[ sys ].t_ch[RF_CH_DW].tAtt.ucRsrpAtt;	//测试开环工控屏蔽
		
		pt->pt_att_rsrp = &t_rf_sys[ sys ].t_ch[RF_CH_UP].tAtt.ucUpDownLink;	//测试使用
		
        pt->pt_alc_point        = &t_rf_sys[ sys ].t_ch[RF_CH_UP].tAlc.scAlcTheoreticalPoint;
		pt->pt_alc_point_offset = &t_rf_sys[ sys ].t_ch[RF_CH_UP].tAlc.scAlcPointOffset;
    }
}


/*****************************************************************************
* @Name	  : get_4g_oplc_para
* @Brief  : 获取4G开环工控相关参数
* @Param  : sys: [输入/出]
* @Retval :
* @Author :
* @Data   : 2023-12-01
* @Note   : 由于4G 为4选1通道, 需要根据有效通道来选择合适的数据
*****************************************************************************/
static void get_4g_oplc_para(uint8_t sys)
{
    if(RF_SYS_1 != sys)
        return;

    uint8_t band = get_valid_band(RF_SYS_1);

    if(band >= 5)
        band = BAND_3;

    tOplc[ RF_SYS_1 ].link_gate   = t_rf_4g_save_para[ band - 1 ].oplc_gate;
    tOplc[ RF_SYS_1 ].pow_offset  = t_rf_4g_save_para[ band - 1 ].dw_pow_offset;
    tOplc[ RF_SYS_1 ].gain_offset = t_rf_4g_save_para[ band - 1 ].oplc_gain_offset;
}


/*****************************************************************************
* @Name	  : oplc_cal
* @Brief  :
* @Param  : sys: [输入/出]
* @Retval :
* @Author :
* @Data   : 2023-11-14
* @Note   :
*****************************************************************************/
void oplc_cal(uint8_t sys)
{
    if(sys >= RF_SYS_NUM)
        return;

    get_4g_oplc_para(sys);		//获取开环工控相关参数, 仅4G有效, 其他频段无效

    OPLC_PARA_T * pt = &tOplc[ sys ];

    uint8_t att_use = 0;

    if(ENABLE == *pt->pt_en)	//使能允许, 计算开环工控
    {
		#if 0
        att_use = (*pt->pt_att_rsrp > pt->link_gate) ? (*pt->pt_att_rsrp - pt->link_gate) : 0;
        if(att_use)
        {
            pt->gain_offset   = (att_use > pt->gain_offset) ? (att_use - pt->gain_offset) : 0;
            pt->pow_offset    = (att_use > pt->pow_offset) ? (att_use - pt->pow_offset) : 0;
            uint8_t temp      = *pt->pt_alc_point - pt->pow_offset;
            *pt->pt_pow_point = (temp >= 5) ? temp : 5;	//更新开环工控启控点
        }
		#endif
		
		//以下程序, 仅供测试使用
		att_use = *pt->pt_att_rsrp;
		if( att_use >= 15 )
			att_use = 15;

		 *pt->pt_pow_point = *pt->pt_alc_point - att_use;
		
		if( *pt->pt_pow_point < t_pow_table[sys][RF_CH_UP].start_pow + 2 )	//限制有效范围
			*pt->pt_pow_point = t_pow_table[sys][RF_CH_UP].start_pow + 2;
    }
    else
    {
        pt->gain_offset   = 0;
        pt->pow_offset    = 0;
        *pt->pt_pow_point = *pt->pt_alc_point;	//更新开环工控启控点
    }
}


/*****************************************************************************
* @Name	  : set_oplc_en
* @Brief  :
* @Param  : sys: [输入/出]
* @Retval :
* @Author :
* @Data   : 2023-11-14
* @Note   :
*****************************************************************************/
void set_oplc_en(uint8_t sys, uint8_t state)
{
    if(sys >= RF_SYS_NUM)
        return;

    if(ENABLE == state)
        *tOplc[ sys ].pt_en = ENABLE;
    else
        *tOplc[ sys ].pt_en = DISABLE;
}


/*****************************************************************************
* @Name	  : set_oplc_link_para
* @Brief  : 设置开环工控联动参数
* @Param  : sys: [输入/出]
**			 gate: [输入/出]
* @Retval :
* @Author :
* @Data   : 2023-11-14
* @Note   :
*****************************************************************************/
void set_oplc_link_para(uint8_t sys, uint8_t gate, int8_t gain_offset, int8_t pow_offset)
{
    if(sys >= RF_SYS_NUM)
        return;

    if(gate >= 20)
        return;

    tOplc[ sys ].link_gate   = gate;
    tOplc[ sys ].pow_offset  = pow_offset;
    tOplc[ sys ].gain_offset = gain_offset;
}

