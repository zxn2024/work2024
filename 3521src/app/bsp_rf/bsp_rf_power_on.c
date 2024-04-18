/***************************** Copyright ****************************************
*
* (C) Copyright 2023, Shaanxi Tianji Communication
*  All Rights Reserved
*
* FileName   : bsp_rf_power_on.c
* Version    : none
* Author     :
* Date       : 2023-12-28
* Description: 上电开机设置
*******************************************************************************/
#include "main.h"

#include "public_enum.h"
#include "system_config.h"
#include "bsp_dev_para.h"
#include "bsp_rf_alc.h"
#include "bsp_rf_att.h"
#include "bsp_rf_pow.h"
#include "bsp_rf_oplc.h"
#include "bsp_rf_global.h"
#include "bsp_rf_time.h"
#include "bsp_up_silence.h"
#include "bsp_para_load.h"
#include "bsp_rf_dc_switch.h"
#include "bsp_dev_flash.h"
#include "bsp_rf_lna.h"
#include "bsp_testmode_save.h"
#include "bsp_rf_4g_band_sel.h"
#include "bsp_rf_att_set.h"
#include "bsp_rf_global.h"
#include "bsp_rf_866x.h"
#include "bsp_public_drv.h"
#include "bsp_rf_866x_default_set.h"
#include "bsp_rf_ctr.h"
#include "bsp_rf_pa.h"

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
* @Name	  : rf_para_init_pow_on
* @brief  : 射频开机参数设置
* @param  : None
* @retval : None
* @author :
* @Data   : 2023-12-28
*****************************************************************************/
static void rf_para_init_pow_on(void)
{
    uint8_t sys;
    uint8_t ch;
	
	tDevPublicPara.ucNetMode                = LTE_NR;
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
//	tDevPublicPara.ucDevMode                = WORK_MODE_NORMAL;
	tDevPublicPara.ucModeSwitchEn			= B_FALSE;			//默认不允许切换

	//设置默认频段
	set_valid_band( RF_SYS_1, BAND_3 );	
	set_valid_bw( RF_SYS_1, rf_866x_get_default_band_width( BAND_3 ) );
	set_valid_freq( RF_SYS_1, rf_866x_get_default_freq_point( BAND_3 ) );
	
//	set_valid_bw( RF_SYS_1, 10 );		// 正式软件需要删除
	
	set_valid_band( RF_SYS_2, BAND_41 );	
	set_valid_bw( RF_SYS_2, rf_866x_get_default_band_width( BAND_41 ) );
	set_valid_freq( RF_SYS_2, rf_866x_get_default_freq_point( BAND_41 ) );
	
	
	set_valid_band( RF_SYS_3, BAND_41M );	//设置默认BAND
	set_valid_bw( RF_SYS_3, rf_866x_get_default_band_width( BAND_41M ) );
	set_valid_freq( RF_SYS_3, rf_866x_get_default_freq_point( BAND_41M ) );
	
    for(sys = 0; sys < RF_SYS_NUM; sys++)
    {
        RfSystem_T * pt_sys = &t_rf_sys[sys];
		
		pt_sys->t_public.exist                   = B_TRUE;
		pt_sys->t_public.SysMode                 = IS_TDD;
		pt_sys->t_public.enable  				 = ENABLE;
		pt_sys->t_public.bin_load_result		 = B_FALSE;
		pt_sys->t_public.scan_net_state          = B_FALSE;
		pt_sys->t_public.sync					 = UNSYNC;
        pt_sys->t_public.opleEn 			     = (0U == DEFAULT_OPLC_EN) ? DISABLE : ENABLE;
        pt_sys->t_public.rsrpEn 			     = DISABLE;
        pt_sys->t_public.tcEn 				     = ENABLE;
        pt_sys->t_public.tRfSwitch.ucNormalState = RF_SWITCH_OPEN;
        pt_sys->t_public.tRfSwitch.ucTestState   = RF_SWITCH_CLOSE;
        pt_sys->t_public.sysRunState		     = RUN_STATE_NORMAL;
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
            pt_ch->tIso.iso 					    = 80;
        }
    }
	
	vBasicDev3BandDefaultSet();
}


/*****************************************************************************
* @Name	  : rf_switch_set_at_power_on
* @Brief  : 上电设置射频开关
* @Param  : None
* @Retval : 
* @Author : 
* @Data   : 2023-12-28
* @Note   :
*****************************************************************************/
static void rf_switch_set_at_power_on( void )
{
	uint8_t sys;
    uint8_t ch;
	RfSysPublic_T * pt_pub = NULL;
	
	pow_init();
    att_init();
    oplc_init();
	lna_pt_init();
    silence_init();
    rf_ctr_time_init();
	
	for(sys = 0; sys < RF_SYS_NUM; sys++)
    {
		//设置衰减器
		att_analog_pwm_set( sys, RF_CH_UP, 1, 0 );
		att_analog_pwm_set( sys, RF_CH_UP, 2, 0 );
		att_analog_pwm_set( sys, RF_CH_DW, 1, 0 );
		
		//设置低噪放
		for(ch = 0; ch < RF_CH_NUM; ch++)
		{
			lna_set( sys, ch, 1, LNA_RF );	//BYP_ON: 低噪放旁路
			lna_set( sys, ch, 2, LNA_RF );	
		}
		
//		pa_init( sys );
		
		//打开射频电源
		pt_pub = &t_rf_sys[ sys ].t_public;
		if(RF_SWITCH_CLOSE == pt_pub->tRfSwitch.ucNormalState)   //射频关断, 退出
			rf_dc_set( sys, PORT_OFF );							 //关闭电源
		else
			rf_dc_set( sys, PORT_ON );
	}
	
}


/*****************************************************************************
* @Name	  : load_default_para_at_pow_on
* @Brief  : 开机载入默认参数
* @Param  : None
* @Retval : 
* @Author : 
* @Data   : 2023-12-28
* @Note   : 在任务之外, 硬件初始化完成后调用
*****************************************************************************/
void load_default_para_at_pow_on( void )
{	
	rf_para_init_pow_on();				//射频参数初始化
	
	tm_read_cali_factory_data();		//加载出厂的设备参数, 存储在 t_tm_factory 结构体中
	flash_dev_para_rw( READ );			//加载应用中的设备参数
										//设置有效的band
	
	load_4g_factory_cali_data();		//载入4G功率表 & 衰减表
	load_5g_factory_cali_data(); 		//载入5G功率表 & 衰减表
	
	pt_remap_after_4g_band_select();	//指针重定向
	
	rf_switch_set_at_power_on();		//设置射频开关状态
	
	vBasicDeviceParaInit(); 			//设备参数初始化
}

/*****************************************************************************
* @Name	  : load_default_para_at_run_start
* @Brief  : 射频运行前 或 切换频段后的默认参数载入
* @Param  : None
* @Retval : 
* @Author : 
* @Data   : 2024-01-22
* @Note   :
*****************************************************************************/
void load_default_para_at_run_start( void )
{
	tm_read_cali_factory_data();		//加载出厂的设备参数, 存储在 t_tm_factory 结构体中
	flash_dev_para_rw( READ );			//加载应用中的设备参数
										//设置有效的band
	
	load_4g_factory_cali_data();		//载入4G功率表 & 衰减表
	load_5g_factory_cali_data(); 		//载入5G功率表 & 衰减表
	
	pt_remap_after_4g_band_select();	//指针重定向
	
	rf_switch_set_at_power_on();		//设置射频开关状态
	
	vBasicDeviceParaInit(); 			//设备参数初始化
	
	
}


/*****************************************************************************
* @Name	  : load_8668_bin_in_task
* @Brief  : 载入指定系统的8668 BIN文件
* @Param  : sys: [输入/出] 
* @Retval : B_TRUE: 成功 / B_FALSE: 失败
* @Author : 
* @Data   : 2024-01-12
* @Note   :
*****************************************************************************/
uint8_t load_8668_bin_in_task( uint8_t sys )
{
	Status_t load_res = STAT_OK;
	RfSysPublic_T *pt = &t_rf_sys[sys].t_public;

	rtt_printf(RTT_OS, RTT_TEXT_MAGENTA"%s 8668 BIN ready load: band = %s, bw = %d, freq = %d \r\n", &rf_sys_name[sys], rf_band_name[pt->valid_band], pt->valid_bw, pt->valid_freq );
	
	load_res = rf_866x_reload_bin( sys, (RF_BAND_E)pt->valid_band, (uint32_t)pt->valid_bw, (uint32_t)pt->valid_freq );		//8668 BIN 文件加载
	if( STAT_OK == load_res )
	{	
		pt->bin_load_result = B_TRUE;
		
//		lna_set(RF_SYS_1, RF_CH_DW, 1, LNA_RF);
//		lna_set(RF_SYS_1, RF_CH_DW, 2, LNA_RF);
//		lna_set(RF_SYS_1, RF_CH_UP, 1, LNA_RF);
//		att_analog_pwm_set(RF_SYS_1, RF_CH_UP, 1, 0);
//		att_analog_pwm_set(RF_SYS_1, RF_CH_UP, 2, 0);
//		att_analog_pwm_set(RF_SYS_1, RF_CH_DW, 1, 0);
//		
		
		rf_866x_default_set( pt->valid_band );
//		rf_ch_lna_att_default_set( pt->valid_band, RF_CH_DW );
//		rf_set_modem_cmd(pt->valid_band, RF_SWITCH_OPEN, (IS_TDD_E)pt->SysMode);
		
		rtt_printf(RTT_OS, RTT_TEXT_GREEN"%s %s 8668 BIN LOAD OK !\r\n\n", &rf_sys_name[sys], rf_band_name[pt->valid_band] );	//成功
	}
	else
	{
		pt->bin_load_result = B_FALSE;
		rtt_printf(RTT_OS, RTT_TEXT_RED"%s %s 8668 BIN LOAD fail!\r\n\n", &rf_sys_name[sys], rf_band_name[pt->valid_band] );	//失败
	}
	
	return pt->bin_load_result;
}

