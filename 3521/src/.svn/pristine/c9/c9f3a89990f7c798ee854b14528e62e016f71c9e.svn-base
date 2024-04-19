/***************************** Copyright ****************************************
*
* (C) Copyright 2022, Shaanxi Tianji Communication
*  All Rights Reserved
*
* FileName   : bsp_mach_para.c
* Version    : none
* Author     : ZhangWei
* Date       : 2023-10-16
* Description: none
*******************************************************************************/
#include "main.h"

#include "string.h"
#include "bsp_dev_para.h"
#include "bsp_public_drv.h"
#include "bsp_testmode_save.h"
#include "bsp_rf_global.h"
#include "bsp_para_load.h"
#include "bsp_rf_att.h"

/*---------------------- 端口定义 --------------------------------------*/


/*---------------------- 宏定义  ---------------------------------------*/


/*---------------------- 结构体 ----------------------------------------*/


/*---------------------- 常量 ------------------------------------------*/


/*---------------------- 全局变量 --------------------------------------*/
// #if(PRODUCT_MODEL == CMDT3521C)
// __align(32) Device_T        tDevFactoryPara                 __attribute__((section("CCMRAM"))); // 设备出厂参数信息
// __align(32) omc_t           tOmcPara                        __attribute__((section("CCMRAM"))); // 网管参数信息
// __align(32) RfStaticPara_T  tRfSysStaticPara[RF_SYS_NUM]    __attribute__((section("CCMRAM"))); // 设备参数信息
// #else
// Device_T        tDevFactoryPara;                // 设备出厂参数信息
// omc_t           tOmcPara;                       // 网管参数信息
// RfStaticPara_T  tRfSysStaticPara[RF_SYS_NUM];   // 设备参数信息
// #endif
Device_T        tDevFactoryPara;                // 设备出厂参数信息
omc_t           tOmcPara;                       // 网管参数信息
RfStaticPara_T  tRfSysStaticPara[RF_SYS_NUM];   // 设备参数信息
/*---------------------- 局部变量 --------------------------------------*/


/*---------------------- 声明 ------------------------------------------*/



/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ HAL ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


/**
* @brief   3频率设备默认设置, 屏蔽了MIMO和BAND39 / BAND40   
* @param   None
* @return    
* @data    2024-03-05
* @note    
*/
void vBasicDev3BandDefaultSet( void )
{
	t_rf_sys[RF_SYS_3].t_public.exist  = DEV_MIMO_EXIST;
	t_rf_sys[RF_SYS_3].t_public.enable = (DEV_MIMO_EXIST == 1) ? ENABLE: DISABLE;
	t_rf_sys[RF_SYS_3].t_public.SysMode = IS_TDD;
	
	t_rf_sys[RF_SYS_2].t_public.SysMode = IS_TDD;
	
	if(( BAND_3 == t_rf_sys[RF_SYS_1].t_public.valid_band ) || ( BAND_8 == t_rf_sys[RF_SYS_1].t_public.valid_band ))
		t_rf_sys[RF_SYS_1].t_public.SysMode = IS_FDD;
	else
		t_rf_sys[RF_SYS_1].t_public.SysMode = IS_TDD;
}


/*****************************************************************************
* @Name	  : vBasicDevStaticParaInit
* @Brief  : 静态参数初始化
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2022-09-02
* @Note   :
*****************************************************************************/
void vBasicDevStaticParaInit( void )
{
    RfStaticPara_T * pt = NULL;
    uint8_t sys = 0;
	uint8_t ch  = 0;
	
    //SYS1
    sys = RF_SYS_1;
    pt = &tRfSysStaticPara[sys];
	uint8_t band = get_valid_band( sys );
	
	pt->sysMode                       = (( BAND_3 == band ) || ( BAND_8 == band ) ) ? IS_FDD : IS_TDD;
	pt->chPara[RF_CH_UP].theoryGain   = ( t_rf_4g_cali_para[ band ].up_theo_gain < 127 )   ? t_rf_4g_cali_para[ band ].up_theo_gain    : 80;    //理论增益, 上行理论增益主从模式为70, 单机模式为65
	pt->chPara[RF_CH_UP].theoryPow    = ( t_rf_4g_cali_para[ band ].up_theo_out_pow < 30 ) ? t_rf_4g_cali_para[ band ].up_theo_out_pow : 30;	//理论输出功率
	pt->chPara[RF_CH_DW].theoryGain   = ( t_rf_4g_cali_para[ band ].dw_theo_gain < 127 )   ? t_rf_4g_cali_para[ band ].dw_theo_gain    : 80;    //理论增益
	pt->chPara[RF_CH_DW].theoryPow    = ( t_rf_4g_cali_para[ band ].dw_theo_out_pow < 30 ) ? t_rf_4g_cali_para[ band ].dw_theo_out_pow : 30; 	//理论输出功率
	pt->chPara[RF_CH_DW].ecr8668AlcPoint = t_ch_pow_8668[sys].ptList->start_pow + 1;
	
	for( ch=RF_CH_UP; ch<RF_CH_NUM; ch++ )
	{
		memset(&t_rf_sys[sys].t_ch[ch].tAtt, 0, sizeof(Att_T)); //重新加载后清空att结构体
        memset(&t_rf_sys[sys].t_ch[ch].tIso, 0, sizeof(Iso_T)); //重新加载后清空ISO结构体
		
		t_rf_sys[sys].t_ch[ch].tIso.iso                   = 80;
		t_rf_sys[sys].t_ch[ch].tAlc.scAlcTheoreticalPoint = pt->chPara[ch].theoryPow;
		t_rf_sys[sys].t_ch[ch].tPow.scTheoOutPow          = pt->chPara[ch].theoryPow;
		t_rf_sys[sys].t_ch[ch].tAlc.scOplcPowPoint        = t_rf_sys[sys].t_ch[ch].tAlc.scAlcTheoreticalPoint;
		t_rf_sys[sys].t_ch[ch].tAtt.ucPresent             = ( RF_CH_UP == ch ) ? (0 - analog_table_4g_up_2.min_number) : (0 - analog_table_4g_dw.min_number);
	}
	
	t_rf_sys[RF_SYS_1].t_ch[RF_CH_UP].tAtt.ucManuCal  		  = t_rf_4g_save_para[ band ].up_gain_offset;
	t_rf_sys[RF_SYS_1].t_ch[RF_CH_UP].tAlc.scAlcPointOffset   = t_rf_4g_save_para[ band ].up_pow_offset;
	t_rf_sys[RF_SYS_1].t_ch[RF_CH_DW].tAtt.ucManuCal          = t_rf_4g_save_para[ band ].dw_gain_offset;
	t_rf_sys[RF_SYS_1].t_ch[RF_CH_DW].tAlc.scAlcPointOffset   = t_rf_4g_save_para[ band ].dw_pow_offset;


    //SYS2
    sys = RF_SYS_2;
    pt = &tRfSysStaticPara[sys];
	
	pt->sysMode                       = IS_TDD;
	
	pt->chPara[RF_CH_UP].theoryGain   = ( t_rf_4g_cali_para[ BAND_41 ].up_theo_gain < 127 )   ? t_rf_4g_cali_para[ BAND_41 ].up_theo_gain    : 80;    	//理论增益, 上行理论增益主从模式为70, 单机模式为65
	pt->chPara[RF_CH_UP].theoryPow    = ( t_rf_4g_cali_para[ BAND_41 ].up_theo_out_pow < 30 ) ? t_rf_4g_cali_para[ BAND_41 ].up_theo_out_pow : 30;		//理论输出功率
	pt->chPara[RF_CH_DW].theoryGain   = ( t_rf_4g_cali_para[ BAND_41 ].dw_theo_gain < 127 )   ? t_rf_4g_cali_para[ BAND_41 ].dw_theo_gain    : 80;    	//理论增益
	pt->chPara[RF_CH_DW].theoryPow    = ( t_rf_4g_cali_para[ BAND_41 ].dw_theo_out_pow < 30 ) ? t_rf_4g_cali_para[ BAND_41 ].dw_theo_out_pow : 30; 		//理论输出功率
	pt->chPara[RF_CH_DW].ecr8668AlcPoint = t_ch_pow_8668[sys].ptList->start_pow + 1;
	for( ch=RF_CH_UP; ch<RF_CH_NUM; ch++ )
	{
		//memset(&t_rf_sys[sys].t_ch[ch].tAtt, 0, sizeof(Att_T)); //重新加载后清空att结构体
		memset(&t_rf_sys[sys].t_ch[ch].tIso, 0, sizeof(Iso_T)); //重新加载后清空ISO结构体
			
		t_rf_sys[sys].t_ch[ch].tIso.iso                   = 80;
		
		t_rf_sys[sys].t_ch[ch].tAlc.scAlcTheoreticalPoint = pt->chPara[ch].theoryPow;
		t_rf_sys[sys].t_ch[ch].tAlc.scOplcPowPoint        = t_rf_sys[sys].t_ch[ch].tAlc.scAlcTheoreticalPoint;
		t_rf_sys[sys].t_ch[ch].tPow.scTheoOutPow          = pt->chPara[ch].theoryPow;
		t_rf_sys[sys].t_ch[ch].tAtt.ucPresent             = ( RF_CH_UP == ch ) ? (0 - analog_table_nr_up_2.min_number) : (0 - analog_table_nr_dw.min_number);
	}
	
    //SYS3
    sys = RF_SYS_3;
    pt = &tRfSysStaticPara[sys];

	pt->sysMode                      = IS_TDD;
	pt->chPara[RF_CH_UP].theoryGain  = ( t_rf_4g_cali_para[ BAND_41M ].up_theo_gain < 127 )   ? t_rf_4g_cali_para[ BAND_41M ].up_theo_gain    : 80;    	//理论增益, 上行理论增益主从模式为70, 单机模式为65
	pt->chPara[RF_CH_UP].theoryPow   = ( t_rf_4g_cali_para[ BAND_41M ].up_theo_out_pow < 30 ) ? t_rf_4g_cali_para[ BAND_41M ].up_theo_out_pow : 30;		//理论输出功率
	pt->chPara[RF_CH_DW].theoryGain  = ( t_rf_4g_cali_para[ BAND_41M ].dw_theo_gain < 127 )   ? t_rf_4g_cali_para[ BAND_41M ].dw_theo_gain    : 80;    	//理论增益
	pt->chPara[RF_CH_DW].theoryPow   = ( t_rf_4g_cali_para[ BAND_41M ].dw_theo_out_pow < 30 ) ? t_rf_4g_cali_para[ BAND_41M ].dw_theo_out_pow : 30; 	//理论输出功率
	pt->chPara[RF_CH_DW].ecr8668AlcPoint = t_ch_pow_8668[sys].ptList->start_pow + 1;
	for( ch=RF_CH_UP; ch<RF_CH_NUM; ch++ )
	{
		//memset(&t_rf_sys[sys].t_ch[ch].tAtt, 0, sizeof(Att_T)); //重新加载后清空att结构体
		memset(&t_rf_sys[sys].t_ch[ch].tIso, 0, sizeof(Iso_T)); //重新加载后清空ISO结构体
			
		t_rf_sys[sys].t_ch[ch].tIso.iso                   = 80;
		t_rf_sys[sys].t_ch[ch].tAlc.scAlcTheoreticalPoint = pt->chPara[ch].theoryPow;
		t_rf_sys[sys].t_ch[ch].tPow.scTheoOutPow          = pt->chPara[ch].theoryPow;
		t_rf_sys[sys].t_ch[ch].tAlc.scOplcPowPoint        = t_rf_sys[sys].t_ch[ch].tAlc.scAlcTheoreticalPoint;
		t_rf_sys[sys].t_ch[ch].tAtt.ucPresent             = ( RF_CH_UP == ch ) ? (0 - analog_table_mimo_up_2.min_number) : (0 - analog_table_mimo_dw.min_number);
	}
	
}

/*****************************************************************************
* @Name	  : vBasicDevFactoryParaInit
* @Brief  : 出厂参数初始化
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2022-11-08
* @Note   :
*****************************************************************************/
void vBasicDevFactoryParaInit(void)
{
    memcpy(&tDevFactoryPara.mcuType, (uint8_t *)DEFAULT_MCU_NAME, strlen((char *)DEFAULT_MCU_NAME) );   //MCU型号
    memcpy(&tDevFactoryPara.flashType, (uint8_t *)FLASH_NAME, strlen((char *)FLASH_NAME) );             //FLASH型号
}


/*****************************************************************************
* @Name	  : vBasicDeviceParaInit
* @Brief  : 设备基础参数初始化
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2023-11-18
* @Note   :
*****************************************************************************/
void vBasicDeviceParaInit(void)
{
    vBasicDevFactoryParaInit();
    vBasicDevStaticParaInit();
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DRV ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/





/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ APP ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/



