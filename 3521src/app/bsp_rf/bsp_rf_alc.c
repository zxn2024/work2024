/***************************** Copyright ****************************************
*
* (C) Copyright 2022, Shaanxi Tianji Communication
*  All Rights Reserved
*
* FileName   : bsp_rf_alc.c
* Version    : none
* Author     : ZhangWei
* Date       : 2022-08-09
* Description: none
*******************************************************************************/
#include "main.h"
#include "system_config.h"

#include "bsp_rf_global.h"
#include "bsp_rf_pow.h"
#include "rtt.h"
#include "bsp_rf_att.h"
#include "bsp_rf_alc.h"
#include "math.h"
#include "bsp_dev_para.h"
#include "bsp_public_drv.h"
#include "lna.h"


/*---------------------- 端口定义 --------------------------------------*/


/*---------------------- 宏定义  ---------------------------------------*/


/*---------------------- 结构体 ----------------------------------------*/


/*---------------------- 常量 ------------------------------------------*/


/*---------------------- 全局变量 --------------------------------------*/


/*---------------------- 局部变量 --------------------------------------*/


/*---------------------- 声明 ------------------------------------------*/


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DRV ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


/*****************************************************************************
* @Name	  : up_att_link_down_att
* @Brief  : 将下行的ALC联动数据发送给上行联动数据
* @Param  : sys: [输入/出]
* @Retval :
* @Author :
* @Data   : 2023-11-01
* @Note   :
*****************************************************************************/
static void up_att_link_down_att(uint8_t sys)
{
    Att_T * ptu = &t_rf_sys[sys].t_ch[RF_CH_UP].tAtt;
    Att_T * ptd = &t_rf_sys[sys].t_ch[RF_CH_DW].tAtt;
//	ATT_CH_T *pt_att = pt_ch_att[sys][RF_CH_DW];
	
	
	uint8_t att_link =  ptd->uc8668Att;
	
	att_link = ( att_link >= 10 ) ? (att_link - 10) : 0;
	
	if( WORK_NORMAL == tDevPublicPara.ucWorkMode )
	{
		ptu->ucUpDownLink = ptd->ucAlc + att_link;
	}
	else
	{
		ptu->ucUpDownLink = 0;
	}
    ptd->ucUpDownLink = 0;
}



/*****************************************************************************
* @Name	  : alc_ch_cal
* @Brief  : 通过末端输出功率计算衰减值
* @Param  : eCh: RF_CH_UP / RF_CH_DW
*           pt: [输入/出]
* @Retval :
* @Author :
* @Data   : 2023-11-10
* @Note   :
*****************************************************************************/
static void alc_ch_cal(uint8_t sys, uint8_t ch)
{
    if((sys >= RF_SYS_NUM) || (ch >= RF_CH_NUM))
        return;

    if (t_rf_sys[sys].t_ch[ch].tIso.lockAlc == ENABLE) {
        return;
    }

    Att_T * pt_att = &t_rf_sys[sys].t_ch[ch].tAtt;
    Alc_T * pt_alc = &t_rf_sys[sys].t_ch[ch].tAlc;

    float   real_point = 0.0f;
	float   alc_point = (float)pt_alc->scAlcTheoreticalPoint + (float)pt_alc->scAlcPointOffset/10;
	float   oplc_point = (float)(pt_alc->scOplcPowPoint + (float)pt_alc->scAlcPointOffset/10);
	
    if((alc_point > oplc_point) && (1U == DEFAULT_OPLC_EN)&&( RF_CH_UP == ch ))
        real_point = oplc_point;
    else
        real_point = alc_point;
	
	pt_alc->scAlcUpperLimit = real_point + 1.0f;
	pt_alc->scAlcLowerLimit = real_point - 1.0f;
	
	if( pt_alc->scAlcUpperLimit > t_pow_table[sys][ch].start_pow + t_pow_table[sys][ch].valid_num )
	{
		pt_alc->scAlcUpperLimit = t_pow_table[sys][ch].start_pow + t_pow_table[sys][ch].valid_num - 1;
		pt_alc->scAlcLowerLimit = pt_alc->scAlcUpperLimit - 2;
	}
	else if( pt_alc->scAlcLowerLimit < t_pow_table[sys][ch].start_pow )
	{
		pt_alc->scAlcLowerLimit = t_pow_table[sys][ch].start_pow + 1;
		pt_alc->scAlcUpperLimit = pt_alc->scAlcLowerLimit + 2;
	}
	
    //计算可用衰减器数量
    uint8_t useful_value = get_att_num_can_use_for_alc(sys, ch);
    float  pow = get_float_ch_pow_for_alc(sys, ch);
	
    if(pow >= (float)(pt_alc->scAlcUpperLimit))
    {
        if(useful_value > 0)
            pt_att->ucAlc++;
    }
    else if(pow <= (float)(pt_alc->scAlcLowerLimit))
    {
        if(pt_att->ucAlc > 0)
            pt_att->ucAlc--;
    }
	
//	rtt_printf( RTT_RF, RTT_TEXT_YELLOW"alc cal: %s - %s, \tpow = %d, \tuseful_att = %d, \talc_att = %d\n", rf_band_name[t_rf_sys[sys].t_public.valid_band], rf_ch_name[ch], pow, useful_value, pt_att->ucAlc );
}

/*****************************************************************************
* @Name	  : alc_ch_cal_for_8668
* @Brief  : 通过8668后的检波器检测8668应设置的衰减值
* @Param  : sys: [输入/出]
* @Retval :
* @Author :
* @Data   : 2023-11-10
* @Note   :
*****************************************************************************/
static void alc_ch_cal_for_8668(uint8_t sys)
{
    if(sys >= RF_SYS_NUM)
        return;

    uint8_t ch = (LOCAL_MODULE_TYPE == _MU) ? RF_CH_DW : RF_CH_UP;

    if (t_rf_sys[sys].t_ch[ch].tIso.lockAlc == ENABLE) {
        return;
    }

    Att_T * pt_att = &t_rf_sys[sys].t_ch[ch].tAtt;
    float  pow   = get_float_ch_pow_for_8668(sys, ch);

    int8_t  real_point = tRfSysStaticPara[sys].chPara[ch].ecr8668AlcPoint;	//可改为从外部获取, 最大-5, 系统不一样, 此数据可能不一样.

    //计算可用衰减器数量
    uint8_t useful_value = get_att_num_can_use_for_8668(sys);

    if(pow >= (float)(real_point + 1))
    {
        if(useful_value > 0)
            pt_att->uc8668Att++;
    }
    else if(pow <= (float)(real_point - 1))
    {
        if(pt_att->uc8668Att > 0)
            pt_att->uc8668Att--;
    }
//	if( RF_SYS_1 == sys )
//		rtt_printf( RTT_RF, RTT_TEXT_CYAN"8668 alc: %s, \tpow = %d, \tuseful_att = %d, \talc_att = %d\n", rf_band_name[t_rf_sys[sys].t_public.valid_band], pow, useful_value, pt_att->uc8668Att );
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ APP ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


/*****************************************************************************
* @Name	  : coverage_stability_by_rsrp
* @Brief  : 覆盖稳定性 ( 主机下行根据RSRP设置ATT )
* @Param  : sys
* @Retval :
* @Author :
* @Data   : 2023-11-10
* @Note   : 初始衰减值=额定增益-（额定输出功率-（RSRP+10log10（W/子载波带宽）））
*****************************************************************************/
void coverage_stability_by_rsrp(uint8_t sys, uint8_t ch)
{
    if((sys >= RF_SYS_NUM) || (ch >= RF_CH_NUM))
        return;

    if((LOCAL_MODULE_TYPE == _RU) || (RF_CH_UP == ch))
    {
        t_rf_sys[sys].t_ch[ch].tAtt.ucRsrpAtt = 0;
        return;
    }

    NetPara_T * pt_net = &t_rf_sys[sys].t_public.tNet;

    uint8_t rsrp    = pt_net->rsrp;			//获取RSRP数值
    uint8_t bw_thro = pt_net->bandWidth;	//带宽
    uint8_t bw_sub  = pt_net->subBw;		//子载波带宽
    double  d_res   = bw_thro / bw_sub;

    Alc_T * pt_alc = &t_rf_sys[sys].t_ch[RF_CH_DW].tAlc;
    Att_T * pt_att = &t_rf_sys[sys].t_ch[RF_CH_DW].tAtt;

    int8_t pow = (pt_alc->scOplcPowPoint < pt_alc->scAlcTheoreticalPoint) ? pt_alc->scOplcPowPoint : pt_alc->scAlcTheoreticalPoint;
    int8_t att_res = (int8_t)(pow - (rsrp + (int8_t)(10 * log10(d_res))));

    if(att_res < 0)
        att_res = 0;

    pt_att->ucRsrpAtt = att_res;
}


/*****************************************************************************
* @Name	  : alc_cal_8668
* @Brief  : ALC计算
* @Param  : sys: [输入/出]
* @Retval :
* @Author :
* @Data   : 2023-11-10
* @Note   :
*****************************************************************************/
void alc_cal_8668(uint8_t sys)
{
    if(sys >=  RF_SYS_NUM)
        return;

    alc_ch_cal_for_8668(sys);		//计算8668衰减器
}
#if LOCAL_MODULE_TYPE == _MU
/*****************************************************************************
* @Name	  : 上下行输出功率ALC计算
* @Brief  : 上下行输出功率ALC计算
* @Param  : sys: [输入/出]
* @Retval :
* @Author :
* @Data   : 2023-11-10
* @Note   :
*****************************************************************************/
void alc_cal_for_up_and_down(uint8_t sys)
{
    if(sys >=  RF_SYS_NUM)
        return;

    alc_ch_cal(sys, RF_CH_DW);	    //计算下行总衰减
    up_att_link_down_att(sys);      //联动
    alc_ch_cal(sys, RF_CH_UP);	    //计算上行总衰减
}

#else
/*****************************************************************************
* @Name	  : 下行输出功率ALC计算
* @Brief  : 下行输出功率ALC计算
* @Param  : sys: [输入/出]
* @Retval :
* @Author :
* @Data   : 2023-11-10
* @Note   :
*****************************************************************************/
void alc_cal_ru_for_down(uint8_t sys)
{
    if(sys >=  RF_SYS_NUM)
        return;

    alc_ch_cal(sys, RF_CH_DW);	    //计算下行总衰减
}

/*****************************************************************************
* @Name	  : 从机RU联动
* @Brief  : 从机RU联动
* @Param  : sys: [输入/出]
* @Retval :
* @Author :
* @Data   : 2023-11-10
* @Note   :
*****************************************************************************/
void alc_cal_ru_for_up_and_down(uint8_t sys)
{
    if(sys >=  RF_SYS_NUM)
        return;

    up_att_link_down_att(sys);      //联动
    alc_ch_cal(sys, RF_CH_UP);	    //计算上行总衰减
}
#endif


/*****************************************************************************
* @Name	  : alc_cal
* @Brief  : ALC计算
* @Param  : sys: [输入/出]
* @Retval :
* @Author :
* @Data   : 2023-11-10
* @Note   :
*****************************************************************************/
void alc_cal(uint8_t sys)
{
    if(sys >=  RF_SYS_NUM)
        return;

    alc_ch_cal_for_8668(sys);		//计算8668衰减器
    alc_cal_for_up_and_down( sys );
}

