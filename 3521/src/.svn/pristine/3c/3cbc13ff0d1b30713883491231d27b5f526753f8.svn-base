/***************************** Copyright ****************************************
*
* (C) Copyright 2023, Shaanxi Tianji Communication
*  All Rights Reserved
*
* FileName   : rf_init.c
* Version    : none
* Author     :
* Date       : 2023-11-20
* Description: none
*******************************************************************************/
#include "main.h"

#include "system_config.h"

#include "bsp_dev_para.h"
#include "bsp_rf_alc.h"
#include "bsp_rf_att.h"
#include "bsp_rf_pow.h"
#include "bsp_rf_oplc.h"
#include "bsp_rf_global.h"
#include "bsp_rf_time.h"
#include "bsp_up_silence.h"

/*---------------------- 端口定义 --------------------------------------*/


/*---------------------- 事件组  ----------------------------------------*/

/*---------------------- 宏定义  ---------------------------------------*/
#define SLAVE_OPLC_EN               ( 0 )                               //系统工控使能

/*---------------------- 结构体 ----------------------------------------*/


/*---------------------- 常量 ------------------------------------------*/


/*---------------------- 全局变量 --------------------------------------*/


/*---------------------- 局部变量 --------------------------------------*/


/*---------------------- 声明 ------------------------------------------*/




/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ APP ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


/*****************************************************************************
* @Name	  : vRfInit
* @brief  : 射频参数初始化
* @param  : None
* @retval : None
* @author :
* @Data   : 2023-10-18
*****************************************************************************/
void vRfInit(void)
{
    uint8_t sys;
    uint8_t ch;

    vBasicDeviceParaInit(); //设备参数初始化
    pow_init();
    att_init();
    oplc_init();
    silence_init();
    rf_ctr_time_init();

    tDevPublicPara.scTemp 					= 0;
    tDevPublicPara.scTempOffset 			= 0;
    tDevPublicPara.tSlaveDevNum.ucMaxNum 	= SLAVE_DEV_PERMIT_MAX_NUM;
    tDevPublicPara.tSlaveDevNum.ucTheoNum 	= 1;
    tDevPublicPara.tSlaveDevNum.ucOnlineNum = 0;
    tDevPublicPara.ucRstByRunTime 			= 25; //默认间隔25小时重启
    tDevPublicPara.ucRstByNetTime 			= 3;  //默认凌晨3点重启
    tDevPublicPara.ucAcVol 					= 0;
    tDevPublicPara.ucCommFrePoint 			= 0;
    tDevPublicPara.ucLedSwitch 				= (0U == DEFAULT_LED_SILENCE) ? DISABLE : ENABLE;
//	tDevPublicPara.ucLedDisplayMode = ;
    tDevPublicPara.ucRunState 				= RUN_STATE_NORMAL;
    tDevPublicPara.ucWarnEn 				= ENABLE;
//	tDevPublicPara.ucDevMode               = WORK_MODE_NORMAL;

//    tNetConfig.firstTransitionPointOffset = 0x02;
//    tNetConfig.firstProtectTime = 0x04;
//    tNetConfig.secondTransitionPointOffset = 0x02;
//    tNetConfig.secondProtectTime = 0x04;

    for(sys = 0; sys < RF_SYS_NUM; sys++)
    {
        RfSystem_T * pt_sys = &t_rf_sys[sys];

        pt_sys->t_public.opleEn 			     = (0U == DEFAULT_OPLC_EN) ? DISABLE : ENABLE;
        pt_sys->t_public.rsrpEn 			     = DISABLE;
        pt_sys->t_public.tcEn 				     = ENABLE;
        pt_sys->t_public.tRfSwitch.ucNormalState = RF_SWITCH_OPEN;
        pt_sys->t_public.tRfSwitch.ucTestState   = RF_SWITCH_CLOSE;
        pt_sys->t_public.sysRunState		     = OPEN;
        //    pt_sys->t_public.IsoCloseGate 		     = tRfChStaticPara[sys][RF_CH_DW].isoCloseGate;

        for(ch = 0; ch < RF_CH_NUM; ch++)
        {
            RfChPara_T * pt_ch = &t_rf_sys[sys].t_ch[ch];

            pt_ch->tGain.scTheoreticalGain 		    = tRfSysStaticPara[sys].chPara[ch].theoryGain;
            pt_ch->tAlc.scAlcTheoreticalPoint 	    = tRfSysStaticPara[sys].chPara[ch].theoryPow;
            pt_ch->tAlc.scAlcUpperLimit 			= pt_ch->tAlc.scAlcTheoreticalPoint + 1;
            pt_ch->tAlc.scAlcLowerLimit 			= pt_ch->tAlc.scAlcTheoreticalPoint - 1;
            pt_ch->tAlc.scOplcOffset 				= 0;
            pt_ch->tAlc.scOplcPowPoint 			    = pt_ch->tAlc.scAlcTheoreticalPoint;
            pt_ch->tAtt.scCaliGainOffset 			= 0;    //正数减小增益, 负数增大增益
            pt_ch->tAtt.scTempOffset 				= 0;
            pt_ch->tAtt.ucManuCal 				    = 0;
            pt_ch->tPow.scTheoOutPow 				= tRfSysStaticPara[sys].chPara[ch].theoryPow;
            pt_ch->tPowList.scCaliOffset 			= 0;
            pt_ch->tPowList.scTempOffset 			= 0;
            pt_ch->tIso.iso 					    = pt_ch->tGain.scTheoreticalGain + 10;
            pt_ch->tIso.lockAlc                     = DISABLE;
        }
    }
}

