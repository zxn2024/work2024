/***************************** Copyright ****************************************
*
* (C) Copyright 2022, Shaanxi Tianji Communication
*  All Rights Reserved
*
* FileName   : bsp_rf_ctr.c
* Version    : none
* Author     : ZhangWei
* Date       : 2022-08-05
* Description: none
*******************************************************************************/
#include "main.h"
#include "system_config.h"
#include "public_enum.h"

#include "bsp_dev_para.h"
#include "bsp_rf_global.h"
#include "rtt.h"
#include "bsp_rf_ctr.h"
#include "bsp_rf_lna.h"
#include "bsp_rf_alc.h"
#include "bsp_rf_att.h"
#include "bsp_rf_pow.h"
#include "bsp_rf_oplc.h"
#include "FreeRTOS.h"                   // ARM.FreeRTOS::RTOS:Core
#include "event_groups.h"               // ARM.FreeRTOS::RTOS:Event Groups
#include "bsp_temperature.h"
#include "bsp_rf_workmode.h"
#include "bsp_rf_time.h"
#include "bsp_up_silence.h"
#include "bsp_rf_get_para.h"
#include "bsp_rf_dc_switch.h"
#include "bsp_public_drv.h"
#include "bsp_testmode_hal_set.h"
#include "bsp_rf_4g_band_sel.h"

#include "bsp_rf_power_on.h"
#include "bsp_rf_866x.h"
#include "bsp_rf_pa.h"
#include "bsp_rf_866x_default_set.h"
#include "bsp_rf_att_set.h"

#include "bsp_rf_fpga.h"
#include "bsp_rf_iso.h"

#if LOCAL_MODULE_TYPE == _MU
#include "bsp_rf_5g_modem.h"
#include "mu_ru_communicate.h"
#else

#endif
/*---------------------- 端口定义 --------------------------------------*/


/*---------------------- 事件组  ----------------------------------------*/


/*---------------------- 宏定义  ---------------------------------------*/
#define SLAVE_OPLC_EN   ( 0 )           //系统工控使能
#define DEBUG_MODE		( 0 )           //正常模式为0, 测试模式为1


/*---------------------- 结构体 ----------------------------------------*/
typedef struct
{
	uint16_t rf_set_pa:1 ;			//B_FALSE: 不设置  B_TRUE:重新设置
	uint16_t rf_set_5g_modem:1;		//B_FALSE: 不设置  B_TRUE:重新设置
	uint16_t rf_set_lte_modem:1;	//B_FALSE: 不设置  B_TRUE:重新设置
	uint8_t  bin_loader_cnt;		//bin文件加载次数
}rf_set_flag_t;

typedef struct
{
	int8_t up_pow_last;
	int8_t dw_pow_last;
	uint8_t up;
	uint8_t dw;
}alc_act_flag_t;

/*---------------------- 常量 ------------------------------------------*/


/*---------------------- 全局变量 --------------------------------------*/


/*---------------------- 局部变量 --------------------------------------*/
rf_set_flag_t t_rf_set_flag[RF_SYS_NUM];

/*---------------------- 声明 ------------------------------------------*/
static void rf_para_printf(void);
extern void test_analog_att( uint8_t sys, uint8_t ch );
extern void att_set_force_fresh( uint8_t sys );

void rf_set_mode_switch_en( BOOL_E en );


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ HAL ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/





/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DRV ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*****************************************************************************
* @Name	  : rf_set_define_band_in_product_mode
* @Brief  : 在没有5G模组的条件下测试使用
* @Param  : band_4g: [输入/出] 
* @Retval : 
* @Author : 
* @Data   : 2024-01-23
* @Note   :
*****************************************************************************/
void rf_set_define_band_in_product_mode( uint8_t band_4g )
{
	
	if( WORK_PL == tDevPublicPara.ucWorkMode )	//测试模式使用
	{
//		set_valid_band( RF_SYS_1, band_4g );	
//		set_valid_bw( RF_SYS_1, rf_866x_get_default_band_width( band_4g ) );
//		set_valid_freq( RF_SYS_1, rf_866x_get_default_freq_point( band_4g ) );
		
		set_valid_band( RF_SYS_2, BAND_41 );	
		set_valid_bw( RF_SYS_2, rf_866x_get_default_band_width( BAND_41 ) );
		set_valid_freq( RF_SYS_2, rf_866x_get_default_freq_point( BAND_41 ) );
		
		set_valid_band( RF_SYS_3, BAND_41 );	//设置默认BAND
		set_valid_bw( RF_SYS_3, rf_866x_get_default_band_width( BAND_41M ) );
		set_valid_freq( RF_SYS_3, rf_866x_get_default_freq_point( BAND_41M ) );
		
		t_rf_sys[RF_SYS_1].t_public.net_mode = LTE;
		t_rf_sys[RF_SYS_2].t_public.net_mode = NR;
		t_rf_sys[RF_SYS_3].t_public.net_mode = NR;
		
		t_rf_sys[RF_SYS_1].t_public.exist = 1;
		t_rf_sys[RF_SYS_2].t_public.exist = 1;
		t_rf_sys[RF_SYS_3].t_public.exist = DEV_MIMO_EXIST;
		
		
		t_rf_sys[RF_SYS_1].t_public.scan_net_state = B_TRUE;
		t_rf_sys[RF_SYS_2].t_public.scan_net_state = B_TRUE;
		t_rf_sys[RF_SYS_3].t_public.scan_net_state = B_TRUE;
		
		t_rf_sys[RF_SYS_1].t_public.openState = B_TRUE;
		t_rf_sys[RF_SYS_2].t_public.openState = B_TRUE;
		t_rf_sys[RF_SYS_3].t_public.openState = B_TRUE;
		
//		rtt_printf( RTT_RF, RTT_TEXT_YELLOW"dev run in product mode....\n" );
	}
	else if( WORK_NORMAL == tDevPublicPara.ucWorkMode )	//正常运行时测试使用
	{
		set_valid_band( RF_SYS_1, band_4g );	
		set_valid_bw( RF_SYS_1, rf_866x_get_default_band_width( band_4g ) );
		set_valid_freq( RF_SYS_1, rf_866x_get_default_freq_point( band_4g ) );
		
		set_valid_band( RF_SYS_2, BAND_41 );	
		set_valid_bw( RF_SYS_2, rf_866x_get_default_band_width( BAND_41 ) );
		set_valid_freq( RF_SYS_2, rf_866x_get_default_freq_point( BAND_41 ) );
		
		set_valid_band( RF_SYS_3, BAND_41M );	//设置默认BAND
		set_valid_bw( RF_SYS_3, rf_866x_get_default_band_width( BAND_41M ) );
		set_valid_freq( RF_SYS_3, rf_866x_get_default_freq_point( BAND_41M ) );
		
		t_rf_sys[RF_SYS_1].t_public.net_mode = LTE;
		t_rf_sys[RF_SYS_2].t_public.net_mode = NR;
		t_rf_sys[RF_SYS_3].t_public.net_mode = NR;
		
		t_rf_sys[RF_SYS_1].t_public.exist = 1;
		t_rf_sys[RF_SYS_2].t_public.exist = 1;
		t_rf_sys[RF_SYS_3].t_public.exist = DEV_MIMO_EXIST;
		
		t_rf_sys[RF_SYS_1].t_public.scan_net_state = B_TRUE;
		t_rf_sys[RF_SYS_2].t_public.scan_net_state = B_TRUE;
		t_rf_sys[RF_SYS_3].t_public.scan_net_state = B_TRUE;
		
		t_rf_sys[RF_SYS_1].t_public.openState = B_TRUE;
		t_rf_sys[RF_SYS_2].t_public.openState = B_TRUE;
		t_rf_sys[RF_SYS_3].t_public.openState = B_TRUE;
	}
}



/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ OS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ APP ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*****************************************************************************
* @Name	  : vRfTmpCompensation
* @Brief  : 更新温度补偿值
* @Param  : sys: 系统选择
* @Retval :
* @Author :
* @Data   : 2023-11-18
* @Note   :
*****************************************************************************/
void rf_tc_cal(uint8_t sys)
{
    RfChPara_T * pt = NULL;
    uint8_t ch = 0;

    if(sys >= RF_SYS_NUM)
        return;

    if(ENABLE == t_rf_sys[sys].t_public.tcEn)
    {
        for(ch = 0; ch < RF_CH_NUM; ch++)
        {
            pt = &t_rf_sys[ sys ].t_ch[ ch ];
            pt->tAtt.scTempOffset = get_gain_tc_value(sys, ch);
            pt->tPowList.scTempOffset = get_pow_tc_value(sys, ch);
        }
    }
    else
    {
        for(ch = 0; ch < RF_CH_NUM; ch++)
        {
            pt = &t_rf_sys[ sys ].t_ch[ ch ];
            pt->tAtt.scTempOffset = 0;
            pt->tPowList.scTempOffset = 0;
        }
    }
}

/**
* @brief   通道关闭设置  
* @param   sys: [输入/出] 
* @return    
* @data    2024-04-06
* @note    
*/
void rf_ch_close_set( uint8_t sys )
{
	// 衰减器设置为全衰减, 低噪放旁路
	att_all_set_max_value( sys );
	
	// 功放关闭
	if( (t_rf_sys[sys].t_public.valid_band == BAND_3) || (t_rf_sys[sys].t_public.valid_band == BAND_8) )
		rf_set_modem_cmd(RF_SYS_1, 0,IS_FDD);
	else if( (t_rf_sys[sys].t_public.valid_band == BAND_39) || (t_rf_sys[sys].t_public.valid_band == BAND_40) )
		rf_set_modem_cmd(RF_SYS_1, 0,IS_TDD);
	else 
		rf_set_modem_cmd(sys, 0,IS_TDD);
		
	RfSysPublic_T *pt_pub = &t_rf_sys[sys].t_public;
	pt_pub->dc_switch       = CLOSE;
	pt_pub->bin_load_result = 0;
	pt_pub->sync            = UNSYNC;
	pt_pub->bin_load_result = 0;
	pt_pub->scan_net_state  = 0;
	t_rf_sys[sys].t_public.sysRunState = RUN_STATE_CLOSE;	//系统关断
	
	rf_dc_set( sys, PORT_OFF );
	
	rtt_printf( RTT_RF, RTT_TEXT_RED" [WARN] %s sys power over closed!\n", rf_sys_name[sys] );
}


/*****************************************************************************
* @Name	  : rf_ctr_ch_close_process
* @Brief  : 通道关断处理流程
* @Param  : sys: [输入/出]
* @Retval :
* @Author :
* @Data   : 2023-10-20
* @Note   :
*****************************************************************************/
void rf_ctr_ch_close_process(uint8_t sys)
{
#if 1
	#define CLOSED_TIME	( 2* 60 * 1000 )

	static uint32_t start_time[RF_SYS_NUM] = {0, 0, 0};
	static uint32_t end_time[RF_SYS_NUM] = {0,0,0};
	
	int8_t dw_in_pow = t_rf_sys[sys].t_ch[RF_CH_DW].tPow.scInPow;
	int8_t up_in_pow = t_rf_sys[sys].t_ch[RF_CH_UP].tPow.scInPow;
	
	if((dw_in_pow > 0) || (up_in_pow > 0))
		end_time[sys] = hi_get_tick();
	else
	{
		start_time[sys] = hi_get_tick();
		end_time[sys]   = start_time[sys];
	}
	
	if( end_time[sys] < start_time[sys] )
	{
		start_time[sys] = hi_get_tick();
		end_time[sys] = start_time[sys];
	}
	
	if( end_time[sys] - start_time[sys] >= CLOSED_TIME )	//关断
	{
		rf_ch_close_set( sys );
		t_rf_sys[sys].t_public.closeReason = CLOSE_POW_OVER;
	}

#endif
}


/*****************************************************************************
* @Name	  : rf_real_gain_cal
* @brief  : 通道实际增益计算
* @param  : pt: [输入/出]
* @retval :
* @author :
* @Data   : 2023-10-20
*****************************************************************************/
/* 实际增益 = 理论增益 - 人工调整增益 - ISO衰减 - ALC衰减值 */
static void rf_real_gain_cal(uint8_t sys)
{
    uint8_t att_num = 0;
    RfChPara_T * pt = NULL;
	ATT_CH_T   * pt_att = NULL;
	
    if(sys >= RF_SYS_NUM)
        return;
	
    pt = &t_rf_sys[sys].t_ch[RF_CH_DW];	//下行实际增益
	pt_att = pt_ch_att[sys][RF_CH_DW];
	pt->tGain.scTheoreticalGain = tRfSysStaticPara[sys].chPara[RF_CH_DW].theoryGain;
	pt->tAtt.constAttNum = pt_att->lna1.max_num + pt_att->lna2.max_num + pt_att->digit_att1.max_num + pt_att->analog_att1.max_num;

	att_num = pt_att->digit_att1.cur_value + pt_att->analog_att1.cur_value - 10 - pt->tAtt.ucPresent;
	if( pt_att->lna1.cur_value == 0 )
		att_num += pt_att->lna1.max_num;
	if( pt_att->lna2.cur_value == 0 )
		att_num += pt_att->lna2.max_num;
	
    if(att_num >= pt->tAtt.constAttNum)
        att_num = pt->tAtt.constAttNum;
	
	if( att_num <= 3 )
		att_num = 0;
	
    pt->tGain.scRealGain = pt->tGain.scTheoreticalGain - att_num;	//下行实际增益
	pt->tPow.scOutPow    = t_ch_pow[sys][RF_CH_DW].pow.ave;
	pt->tPow.scInPow = pt->tPow.scOutPow - pt->tGain.scRealGain;	//下行实际输入功率

    pt     = &t_rf_sys[sys].t_ch[RF_CH_UP];	//上行实际增益
	pt_att = pt_ch_att[sys][RF_CH_UP];
	pt->tGain.scTheoreticalGain = tRfSysStaticPara[sys].chPara[RF_CH_UP].theoryGain;
	pt->tAtt.constAttNum = pt_att->lna1.max_num + pt_att->analog_att1.max_num + pt_att->analog_att2.max_num;
	
	att_num = pt_att->analog_att1.cur_value + pt_att->analog_att2.cur_value;
	if( pt_att->lna1.cur_value == 0 )
		att_num += pt_att->lna1.max_num;
	
	att_num = att_num - pt->tAtt.ucPresent;
	
    if(att_num >= pt->tAtt.constAttNum)
        att_num = pt->tAtt.constAttNum;
    pt->tGain.scRealGain = pt->tGain.scTheoreticalGain - att_num;		//上行实际增益
	pt->tPow.scOutPow    = t_ch_pow[sys][RF_CH_UP].pow.ave;
	pt->tPow.scInPow     = pt->tPow.scOutPow - pt->tGain.scRealGain;	//上行实际输入功率
}

/*****************************************************************************
* @Name	  : rf_ch_pow_cal
* @brief  : 计算指定通道的输入功率
* @param  : pt: 通道参数结构体指针
* @retval :
* @author :
* @Data   : 2022-08-09
*****************************************************************************/
static void rf_ch_pow_cal(uint8_t sys)
{

}


/*****************************************************************************
* @Name	  : rf_sys_oplc_cal_and_brd
* @Brief  : 系统工控计算及广播
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2022-10-21
* @Note   :
*****************************************************************************/
void rf_sys_oplc_cal(RF_SYS_E sys)
{
	Gain_T         * pt_gain = &t_rf_sys[sys].t_ch[RF_CH_UP].tGain;
	Alc_T          * pt_alc  = &t_rf_sys[sys].t_ch[RF_CH_UP].tAlc;
	RfSysPublic_T  * pt_pub  = &t_rf_sys[sys].t_public;
	int8_t           up_real_out_pow = 0;
	RfChPara_T     * pt      = &t_rf_sys[sys].t_ch[RF_CH_UP];
	
	if(pt_alc->scAlcTheoreticalPoint > (pt_alc->scOplcPowPoint + pt_alc->scOplcOffset))
		up_real_out_pow = pt_alc->scOplcPowPoint + pt_alc->scOplcOffset;
	else
		up_real_out_pow = pt_alc->scAlcTheoreticalPoint;

	//期望接收功率 = 最大发射功率 - 实际增益
	if( ENABLE == pt_pub->opleEn )
		pt_pub->upExpectRxPow = up_real_out_pow - pt_gain->scTheoreticalGain + pt->tAtt.ucUpDownLink;
	else
		pt_pub->upExpectRxPow = 0;
}

/**
* @brief   广播系统工控数据   
* @param   None
* @return    
* @data    2024-04-04
* @note    
*/
void rf_sys_oplc_brd( void )
{
#if ( LOCAL_MODULE_TYPE == _MU )
	if( WORK_NORMAL != tDevPublicPara.ucWorkMode )
		return;
	
	BOOL_E flag = get_rf_oplc_brd_flag();
	RfSysPublic_T  * pt_pub_1  = &t_rf_sys[RF_SYS_1].t_public;
	RfSysPublic_T  * pt_pub_2  = &t_rf_sys[RF_SYS_2].t_public;
	
	if( B_TRUE == flag )
	{
		rtt_printf( RTT_RF, "UpExpectRxPow: LTE: %d, %d,  NR: %d, %d\n", pt_pub_1->upExpectRxPow, 0, pt_pub_2->upExpectRxPow, 0 );
		mrc_set_system_power_ctrl( B_TRUE, pt_pub_1->upExpectRxPow, 0, pt_pub_2->upExpectRxPow, 0 );
	}
#endif
}

void test_reset_5g_modem( void )
{
	#define CNT_MAX ( 10000 )
//	static uint16_t time[3] = {0,0,0};
	static uint32_t cnt = 0;
	
	if( cnt<= CNT_MAX )
		cnt++;
	
	if( CNT_MAX == cnt )
	{
		t_rf_set_flag[RF_SYS_1].rf_set_pa       = B_TRUE;
		t_rf_set_flag[RF_SYS_1].rf_set_5g_modem = B_TRUE;
		
		t_rf_set_flag[RF_SYS_2].rf_set_pa       = B_TRUE;
		t_rf_set_flag[RF_SYS_2].rf_set_5g_modem = B_TRUE;
		
		t_rf_set_flag[RF_SYS_3].rf_set_pa       = B_TRUE;
		t_rf_set_flag[RF_SYS_3].rf_set_5g_modem = B_TRUE;
	}
}

/*****************************************************************************
* @Name	  : rf_band_handle
* @brief  : 单通道状态判断
* @param  : pt: [输入/出]
* @retval : STAT_OK / STAT_ERROR
* @author :
* @Data   : 2023-10-19
*****************************************************************************/
Status_t rf_band_state_check(uint8_t sys)
{
    RfSysPublic_T * pt_pub = &t_rf_sys[ sys ].t_public;
	RfChPara_T    * pt_dw  = &t_rf_sys[ sys ].t_ch[RF_CH_DW];
	
    uint8_t switch_state = B_TRUE;	//开关是否已处于正常状态, B_TRUE: 正常状态,  B_FALSE: 单通状态
    uint8_t silence_state = get_silence_state(sys);
	
	vBasicDev3BandDefaultSet();		//3频设备默认设置
	
    if((RF_SWITCH_UP != pt_pub->tRfSwitch.ucTestState) && (RF_SWITCH_DW != pt_pub->tRfSwitch.ucTestState))
        switch_state = B_TRUE;	//B_TRUE: 非单通状态
	else
		switch_state = B_FALSE;	//B_FALSE: 单通状态
	
    if(B_FALSE == pt_pub->exist)      //硬件系统不存在, 直接退出
        return STAT_ERROR;
	
	if( B_FALSE == pt_pub->scan_net_state )	//扫描未结束或者扫描失败, 直接退出
		return STAT_ERROR;
	
	if( pt_pub->valid_band >= BAND_END )	//有效Band 编号错误
		return STAT_ERROR;

	if((RUN_STATE_CLOSE == pt_pub->sysRunState) && (WORK_NORMAL == tDevPublicPara.ucWorkMode))      //系统由于某些原因关闭, 检测是否满足退出关闭条件, 满足则退出, 否则设置关闭退出
	{
		rf_dc_set( sys, PORT_OFF );
		return STAT_ERROR ;
	}
	
	if( (pt_dw->tAtt.ucIso) > 20 && (pt_dw->tIso.iso < 35) )	//隔离度关断
	{
		rf_dc_set( sys, PORT_OFF );
		pt_pub->closeReason = CLOSE_ISO;
		return STAT_ERROR ;
	}
		
	if( B_FALSE == pt_pub->bin_load_result )	//8668未加载
	{
		if(t_rf_set_flag[sys].bin_loader_cnt < 10)
		{
			uint8_t res = load_8668_bin_in_task( sys );	//加载8668 BIN文件
			if( B_TRUE == res )
			{
				t_rf_set_flag[sys].rf_set_pa = 1;
				if( sys <= RF_SYS_2 )
				{
					t_rf_set_flag[sys].rf_set_5g_modem  = 1;
				}
				t_rf_set_flag[sys].bin_loader_cnt = 0;
				att_set_force_fresh( sys );
			}
			else
			{
				t_rf_set_flag[sys].bin_loader_cnt++;
				return STAT_ERROR;
			}
		}
		else
		{
									//关闭电源
			return STAT_ERROR;		//8668 bin 文件加载失败, 直接退出
		}
	}
	
    if(DISABLE == pt_pub->enable)   //通道禁用, 关闭该通道电源, 然后退出
    {
        return STAT_ERROR;
    }
		
	pt_pub->openState = B_TRUE;		//开机完成
	
	if( B_TRUE == t_rf_set_flag[sys].rf_set_pa )	//设置PA状态
	{
		t_rf_set_flag[sys].rf_set_pa = B_FALSE;
		pa_init( sys );
	}
	
	if( B_TRUE == t_rf_set_flag[sys].rf_set_5g_modem )	//设置5g模组状态
	{
		vTaskDelay( 1000 );
		t_rf_set_flag[sys].rf_set_5g_modem = B_FALSE;
        
        #if LOCAL_MODULE_TYPE == _MU
		rf_5g_modem_switch( (RF_BAND_E)pt_pub->valid_band, RF_SWITCH_OPEN );
        #else

        #endif
	}
	
    if(B_TRUE == switch_state)	//射频开关正常, 单通状态会跳过下列判断
    {
        if((IS_TDD == pt_pub->SysMode) && (UNSYNC == pt_pub->sync))   //TDD系统未同步且不处于单通状态, 退出
        {
            return STAT_ERROR;
        }

        if(RF_SWITCH_CLOSE == pt_pub->tRfSwitch.ucNormalState)   //射频关断, 退出
        {
			rf_dc_set( sys, PORT_OFF );	//关闭电源
            return STAT_ERROR;
        }
		else
		{
			rf_dc_set( sys, PORT_ON );
		}

//        if(RUN_STATE_SILENCE == silence_state)	//上行静默模式
//        {
//            return STAT_ERROR;
//        }
		
//		if(RUN_STATE_CLOSE == pt_pub->sysRunState)      //系统由于某些原因关闭, 检测是否满足退出关闭条件, 满足则退出, 否则设置关闭退出
//        {
//            return STAT_ERROR ;
//        }
    }
	
	return STAT_OK;
}

/*****************************************************************************
* @Name	  : rf_band_handle_8668
* @brief  : 处理8668后的功率采集
* @param  : pt: [输入/出]
* @retval :
* @author :
* @Data   : 2023-10-19
*****************************************************************************/
void rf_band_handle_8668(uint8_t sys)
{
	pow_convert();      							//计算输出功率
	alc_cal_8668(sys);								//计算8668衰减
	att_set(sys, RF_CH_DW);							//设置下行衰减
}


/*****************************************************************************
* @Name	  : rf_band_handle
* @brief  : 射频处理流程
* @param  : pt: [输入/出]
* @retval :
* @author :
* @Data   : 2023-10-19
*****************************************************************************/
static void rf_band_handle_alc(RF_SYS_E sys)
{

	pow_convert();      							//计算输出功率
	alc_cal_for_up_and_down( sys );					//计算上下行末端ALC衰减/上下行联动
	
	if( WORK_NORMAL == tDevPublicPara.ucWorkMode )
	{
        vIsoInRfProgress((RF_SYS_E)sys, RF_CH_DW);
//		coverage_stability_by_rsrp(sys, RF_CH_DW);	    //下行覆盖稳定性, 从机无效
		oplc_cal(sys);            					    //计算开环工控
	}
//    rf_tc_cal(sys);								//计算温补
	
	if( RUN_STATE_SILENCE != get_silence_state(sys) )
		att_set(sys, RF_CH_UP);   						//设置上行衰减
	
    att_set(sys, RF_CH_DW);						    //设置下行衰减
    rf_real_gain_cal(sys);						    //计算实际增益
    rf_ch_pow_cal(sys);							    //计算输入功率
	
	if( WORK_NORMAL == tDevPublicPara.ucWorkMode )
	{
		rf_silence(sys);								//上行静默
		rf_ctr_ch_close_process(sys);   			//关断判断
		rf_sys_oplc_cal(sys);						//系统工控广播, 从机无效
	}
}


static void rf_band_alc_adjust( uint8_t sys, uint8_t ch )
{
	uint16_t pwm_value, pwm_max, pwm_min;
	
	uint16_t pwm_set_value = 0;
	
	RfChPara_T *pt_ch = ( RF_CH_UP == ch ) ? &t_rf_sys[sys].t_ch[RF_CH_UP] : &t_rf_sys[sys].t_ch[RF_CH_DW];
	
	float pow     = t_ch_pow[sys][ch].pow.ave_precision;
	float alc_pow = (pt_ch->tAlc.scAlcLowerLimit + pt_ch->tAlc.scAlcUpperLimit) / 2;
	
	ATT_analog_t *pt_up_list[RF_SYS_NUM] = { pt_att_analog_4g_up_2,
											 &att_analog_nr_up_2,
											 &att_analog_mimo_up_2
											};
	
	ATT_analog_t *pt_dw_list[RF_SYS_NUM] = { &att_analog_4g_dw,
											 &att_analog_nr_dw,
											 &att_analog_mimo_dw
											};
	ATT_analog_t * pt_att = NULL;
	
	pt_att = ( RF_CH_UP == ch ) ? pt_up_list[sys] : pt_dw_list[sys];
	
	
	if( 0 == pt_att->attSetValue )
		return;
		
	pwm_value = pt_att->hw_config.att_table[pt_att->attSetValue];
	pwm_max   = pt_att->hw_config.att_table[pt_att->attSetValue-1];
	pwm_min   = pt_att->hw_config.att_table[pt_att->attSetValue+1];

	if( pow > alc_pow )	//功率高, pwm要减小
	{
		pwm_set_value = (uint16_t)(pwm_value - ( pow - alc_pow ) * ( pwm_value - pwm_min ));
	}
	else	//功率低, pwm要增大
	{
		pwm_set_value = (uint16_t)(pwm_value + ( alc_pow - pow ) * ( pwm_max - pwm_value ));
	}
	
	if( RF_CH_UP == ch )
	{
		att_analog_pwm_set( sys, ch, 2, pwm_set_value);
	}
	else
	{
		att_analog_pwm_set( sys, ch, 1, pwm_set_value);
	}
}


/*****************************************************************************
* @Name	  : rf_band_alc_adjust
* @brief  : alc二次微调
* @param  : sys: [输入]
* @retval :
* @author :
* @Data   : 2023-10-19
*****************************************************************************/
#if 0
static void rf_band_alc_adjust(uint8_t sys, uint8_t ch) //TODO 测试
{
    float alc_adjust_value = 0.0f;
    float alc_set_value = 0.0f;
    int8_t  alc_mid_point = 0;

	pow_convert();      						//计算输出功率,是否必要?

    alc_mid_point = (t_rf_sys[sys].t_ch[ch].tAlc.scAlcLowerLimit + t_rf_sys[sys].t_ch[ch].tAlc.scAlcUpperLimit)/2; 
    alc_adjust_value = ((float)alc_mid_point - t_ch_pow[sys][ch].pow.ave_precision);
    //alc_adjust_value = ((float)alc_mid_point - t_ch_pow[sys][ch].pow.ave_precision) + t_rf_sys[sys].t_ch[ch].tAlc.scAlcAdjustValue;

    if( alc_adjust_value <= -1.5f )
        alc_adjust_value = -1.5f;
    else if( alc_adjust_value >= 1.5f )
        alc_adjust_value = 1.5f;

    //t_rf_sys[sys].t_ch[ch].tAlc.scAlcAdjustValue = alc_adjust_value;

    switch(sys)
    {
        case RF_SYS_1:
            if(ch == RF_CH_UP)
            {
                alc_set_value = (float)pt_ch_att[sys][RF_CH_UP]->analog_att2.cur_value - alc_adjust_value;
                if(alc_set_value <= 0.0f)
                    alc_set_value = 0.0f;
                pt_att_analog_4g_up_2->float_set(pt_att_analog_4g_up_2, alc_set_value);	
            }
            else if(ch == RF_CH_DW)
            {
                alc_set_value = (float)pt_ch_att[sys][RF_CH_DW]->analog_att1.cur_value - alc_adjust_value;
                if(alc_set_value <= 0.0f)
                    alc_set_value = 0.0f;
                pt_att_analog_4g_dw->float_set(pt_att_analog_4g_dw, alc_set_value);
            }
            break;

        case RF_SYS_2:
            if(ch == RF_CH_UP)
            {
                alc_set_value = (float)pt_ch_att[sys][RF_CH_UP]->analog_att2.cur_value - alc_adjust_value;
                if(alc_set_value <= 0.0f)
                    alc_set_value = 0.0f;
                att_analog_nr_up_2.float_set(&att_analog_nr_up_2, alc_set_value);	
            }
            else if(ch == RF_CH_DW)
            {
                alc_set_value = (float)pt_ch_att[sys][RF_CH_DW]->analog_att1.cur_value - alc_adjust_value;
                if(alc_set_value <= 0.0f)
                    alc_set_value = 0.0f;
                att_analog_nr_dw.float_set(&att_analog_nr_dw, alc_set_value);		
            }
            break;

        case RF_SYS_3:
            if(ch == RF_CH_UP)
            {
                alc_set_value = (float)pt_ch_att[sys][RF_CH_UP]->analog_att2.cur_value - alc_adjust_value;
                if(alc_set_value <= 0.0f)
                    alc_set_value = 0.0f;
                att_analog_mimo_up_2.float_set(&att_analog_mimo_up_2, alc_set_value);
            }
            else if(ch == RF_CH_DW)
            {
                alc_set_value = (float)pt_ch_att[sys][RF_CH_DW]->analog_att1.cur_value - alc_adjust_value;
                if(alc_set_value <= 0.0f)
                    alc_set_value = 0.0f;
                att_analog_mimo_dw.float_set(&att_analog_mimo_dw, alc_set_value);
            }
            break;

        default:
            break;
    }
}
#endif


/**
* @brief   ALC微调   
* @param   None
* @return    
* @data    2024-04-07
* @note    
*/
void rf_band_alc_fine_tuning( void )
{
	static alc_act_flag_t t_alc_flag[RF_SYS_NUM];
	alc_act_flag_t *pt = NULL;
	
	RfChPara_T *pt_ch = NULL;
	
	RF_SYS_E sys;
	
	for( sys = RF_SYS_1; sys<RF_SYS_NUM; sys++ )
	{
		pt = &t_alc_flag[sys];
		
		pt_ch = &t_rf_sys[sys].t_ch[RF_CH_UP];
		if( (pt->up_pow_last == pt_ch->tAtt.ucAlc) && (pt->up <= 4 ) )
			pt->up++;
		else if(pt->up_pow_last != pt_ch->tAtt.ucAlc)
		{
			pt->up = 0;
			pt->up_pow_last = pt_ch->tAtt.ucAlc;
		}
		else
		{}
			
		pt_ch = &t_rf_sys[sys].t_ch[RF_CH_DW];
		if( (pt->dw_pow_last == pt_ch->tAtt.ucAlc) && (pt->dw<= 4 ) )
			pt->dw++;
		else if( pt->dw_pow_last != pt_ch->tAtt.ucAlc )
		{
			pt->dw = 0;
			pt->dw_pow_last = pt_ch->tAtt.ucAlc;
		}
		else
		{}
		
		if(pt->up == 3)	//上行ALC无动作
		{
			pt_ch = &t_rf_sys[sys].t_ch[RF_CH_UP];
			if((pt_ch->tPow.scOutPow >= pt_ch->tAlc.scAlcLowerLimit) && (pt_ch->tPow.scOutPow <= pt_ch->tAlc.scAlcUpperLimit))
			{
				rtt_printf( RTT_RF, RTT_TEXT_MAGENTA"%s up alc tuning....\n", rf_sys_name[sys] );
				rf_band_alc_adjust(sys,RF_CH_UP);
			}
		}
		
		if(pt->dw == 3)	//下行ALC无动作
		{
			pt_ch = &t_rf_sys[sys].t_ch[RF_CH_DW];
			if((pt_ch->tPow.scOutPow >= pt_ch->tAlc.scAlcLowerLimit) && (pt_ch->tPow.scOutPow <= pt_ch->tAlc.scAlcUpperLimit))
			{
				rtt_printf( RTT_RF, RTT_TEXT_MAGENTA"%s dw alc tuning....\n", rf_sys_name[sys] );
				rf_band_alc_adjust(sys,RF_CH_DW);
			}
		}
	}
}



/*****************************************************************************
* @Name	  : set_run_mode
* @Brief  : 设置运行模式
* @Param  : eMode: [输入/出]
* @Retval :
* @Author :
* @Data   : 2023-11-17
* @Note   :
*****************************************************************************/
void set_work_mode(WORK_MODE_E mode)
{
    if((WORK_NORMAL == mode) || (WORK_CALIB == mode) || (WORK_PL == mode))
        return;

    tDevPublicPara.ucWorkMode = mode;
}

/**
* @brief   设置设备网络模式  
* @param   mode: [输入/出] 
* @return    
* @data    2024-04-02
* @note    
*/
void set_dev_net_mode( NET_MODE mode )
{
	if( (LTE == mode) || (NR == mode) || (LTE_NR == mode) )
		tDevPublicPara.ucNetMode = mode;
}
/*****************************************************************************
* @Name	  : get_run_mode
* @Brief  : 获取运行模式
* @Param  : None
* @Retval :
* @Author :
* @Data   : get_run_mode
* @Note   :
*****************************************************************************/
WORK_MODE_E getRfRunMode(void)
{
    return (WORK_MODE_E)tDevPublicPara.ucRunState;
}


/*****************************************************************************
* @Name	  : rf_run_start_init
* @brief  : 起始运行状态
* @param  : None
* @retval :
* @author :
* @Data   : 2022-08-10
*****************************************************************************/
void rf_run_start_init(void)
{
	load_default_para_at_run_start();
}

/*****************************************************************************
* @Name	  : rf_run_in_test_mode_init
* @brief  : 进入测试模式前的初始化动作
* @param  : None
* @retval :
* @author :
* @Data   : 2022-08-10
*****************************************************************************/
void rf_run_in_test_mode_init(void)
{
	rtt_printf( RTT_RF, RTT_TEXT_RED"[ WARN ] Dev in to test mode, close all!\r\n" );
	
	tm_sys_close(RF_SYS_1);
	tm_sys_close(RF_SYS_2);
	tm_sys_close(RF_SYS_3);
}


/*****************************************************************************
* @Name	  : vRfRunNor
* @brief  : 正常运行模式
* @param  : None
* @retval :
* @author :
* @Data   : 2022-08-10
*****************************************************************************/
void vRfRunNor(void)
{
    RF_SYS_E sys = RF_SYS_1;
	static uint8_t step[RF_SYS_NUM] = {0,0,0};
    static uint8_t alc_order = 0;
	
	Status_t res[RF_SYS_NUM] = {STAT_ERROR, STAT_ERROR, STAT_ERROR};
	
	for(sys = RF_SYS_1; sys < RF_SYS_NUM; sys++)
	{
		res[sys] = rf_band_state_check( sys );
	}
	
	//处理8668后的ADC数据
	if(BIT_ADC2_DATA_VALID == ((xEventGroupGetBits(xAdcEgp) & BIT_ADC2_DATA_VALID)) && alc_order == 0)
    {
        xEventGroupClearBits(xAdcEgp, BIT_ADC2_DATA_VALID);
        alc_order = 1;
        for(sys = RF_SYS_1; sys < RF_SYS_NUM; sys++)
        {
            if((STAT_OK == res[sys]) && (0==step[sys]))
            {
                rf_band_handle_8668(sys);
                step[ sys ] = 1;
            }
        }
    }
	
//	//延时等待
//	if(B_FALSE == get_rf_interval_flag())     //定时刷新数据
//		return;
//	
	//处理末端的ALC
	if(BIT_ADC2_DATA_VALID == ((xEventGroupGetBits(xAdcEgp) & BIT_ADC2_DATA_VALID)))
    {
        xEventGroupClearBits(xAdcEgp, BIT_ADC2_DATA_VALID);
        alc_order = 0;
		for(sys = RF_SYS_1; sys < RF_SYS_NUM; sys++)
		{
			if( STAT_OK == res[sys] )
				rf_band_handle_alc(sys);
			step[ sys ] = 0;
		}
	}
	
	rf_sys_oplc_brd();			//广播系统工控
	rf_set_fpga_cmd();
	rf_para_printf();
}


/*****************************************************************************
* @Name	  : vRfRunTest
* @brief  : 测试模式
* @param  : None
* @retval :
* @author :
* @Data   : 2022-08-22
*****************************************************************************/
void vRfRunTest(void)
{
//    vTestModePorcess();
}




/*****************************************************************************
* @Name	  : rf_ctr_in_task
* @brief  : 射频控制流程
* @param  : None
* @retval :
* @author :
* @Data   : 2022-08-10
*****************************************************************************/
void rf_ctr_in_task(void)
{
    static uint8_t in_calimode  = B_TRUE;	//判断是否首次进入测试模式, B_TRUE: 首次进入, B_FALSE: 非首次进入
    static uint8_t normal_start = B_TRUE;	//判断是否进行开机流程, B_TRUE: 需要开机流程, B_FALSE: 不需要开机流程
	static uint8_t in_plmode    = B_TRUE;	//判断是否首次进入产线模式, B_TRUE: 首次进入, B_FALSE: 非首次进入
	static uint16_t print_cnt = 0;
	
    get_all_para();
	tmp_in_task();	//获取温度, 计算温补数据
	
	#if DEBUG_MODE
		tDevPublicPara.ucWorkMode = WORK_CALIB;	// *#*#*#* 仅用于测试, 正式程序需要删除
	#endif
	
    if(WORK_CALIB == tDevPublicPara.ucWorkMode)	//work in test mode
    {
		normal_start = B_TRUE;
		in_plmode    = B_TRUE;

        if(B_TRUE == in_calimode)
        {
            rf_run_in_test_mode_init(); 			//进入测试模式前的初始化
            in_calimode  = B_FALSE;
        }

        vRfRunTest();	//测试流程
    }
	else	//work in normal
	{
		if( WORK_PL == tDevPublicPara.ucWorkMode )	//产线模式
		{
			normal_start = B_TRUE;
			in_calimode  = B_TRUE;
			rf_set_mode_switch_en( B_TRUE );
			if( B_TRUE == in_plmode )
			{
				rf_set_define_band_in_product_mode( BAND_3 );	//起始
				in_plmode = B_FALSE;
			}
		}
		else
		{
			in_calimode = B_TRUE;
			in_plmode   = B_TRUE;
			if(B_TRUE == normal_start)		
			{
				rf_run_start_init();	//起始
				normal_start = B_FALSE;
			}
		}
		
		vRfRunNor();	//正常运行
	}
	
//	test_reset_5g_modem();
	
	if( ++print_cnt >= 1000 )
	{
		print_cnt = 0;
		printf_run_time( RTT_OS );
	}
}


/*****************************************************************************
* @Name	  : rf_para_printf
* @Brief  : adc信息打印
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2023-03-20
* @Note   :
*****************************************************************************/
#define _PRINT_EN_RF_PARA_ ( 1 )
static void rf_para_printf(void)
{
#if _PRINT_EN_RF_PARA_
#define RF_PARA_PRINTF_TIME_INTERVAL	( 5 )
    static uint32_t rf_para_get_time_save = 0;

    uint32_t time = get_run_time();
    time /= (RF_PARA_PRINTF_TIME_INTERVAL * 1000);
    if(rf_para_get_time_save != time)
    {
        printf_run_time(RTT_RF);
        rf_para_get_time_save = time;
        RfChPara_T * pt       = NULL; //射频独立参数
        RfSysPublic_T    *   ptpublic = NULL;

        //adc, 功率, 衰减器1, 衰减2, lna, pa, 隔离度,
        rtt_printf(RTT_RF, RTT_TEXT_MAGENTA"******** rf active para : %d \n", vGetTmpValue());
        for(uint8_t esys = 0; esys < RF_SYS_NUM; esys++)
        {
			if( (0==DEV_MIMO_EXIST)  && (esys == RF_SYS_3) )
				break;
				
            ptpublic = &t_rf_sys[ esys ].t_public;
			rtt_printf(RTT_RF, RTT_TEXT_WHITE">>> %s_pub: %d - %d - %d, exist:%d, dc:%d, run_state: %d, close_reason:%d, scan_result: %d,  bin_load: %d\n", 
												rf_band_name[t_rf_sys[esys].t_public.valid_band],
												ptpublic->valid_bw,
												ptpublic->valid_freq,
												ptpublic->valid_freq_point,
												ptpublic->exist,
												ptpublic->dc_switch,
												ptpublic->sysRunState,
												ptpublic->closeReason,
												ptpublic->scan_net_state,
												ptpublic->bin_load_result);
			
            for(uint8_t ech = 0; ech < RF_CH_NUM; ech++)
            {
                pt = &t_rf_sys[esys].t_ch[ech]; //射频独立参数
                if(ptpublic->openState == OPEN)
                {
					if( RF_CH_UP == ech )
					{
						rtt_printf(RTT_RF, RTT_TEXT_GREEN">>> %s_%s: p_in: %-3d, p_out: %.2f, alc: %.2f, oplc:%02d, link: %02d,  att_split: %02d - %02d - %02d, rx_expt: %d, silence: %d\n", 
												rf_band_name[t_rf_sys[esys].t_public.valid_band],
												rf_ch_name[ech], 
												pt->tPow.scInPow, 
												get_float_ch_pow_for_alc(esys, RF_CH_UP),
												(float)pt->tAlc.scAlcTheoreticalPoint + pt->tAlc.scAlcPointOffset/10,
												pt->tAlc.scOplcPowPoint,
												pt->tAtt.ucUpDownLink,
												pt_ch_att[esys][ech]->lna1.cur_value,
												pt_ch_att[esys][ech]->analog_att1.cur_value,	
												pt_ch_att[esys][ech]->analog_att2.cur_value,
												ptpublic->upExpectRxPow,
												get_silence_state(esys) );
					}
					else
					{
						rtt_printf(RTT_RF, RTT_TEXT_YELLOW">>> %s_%s: p_in: %-3d, p_out: %.2f, p_8668: %.2f, alc: %.2f  att_split: %02d - %02d - %02d - %02d, iso = %d, iso_att = %d\n", 
																		rf_band_name[t_rf_sys[esys].t_public.valid_band],
																		rf_ch_name[ech], 
																		pt->tPow.scInPow, 
																		get_float_ch_pow_for_alc(esys, RF_CH_DW),
																		get_float_ch_pow_for_8668(esys, RF_CH_DW),
																		(float)pt->tAlc.scAlcTheoreticalPoint + pt->tAlc.scAlcPointOffset/10,
																		pt_ch_att[esys][ech]->lna1.cur_value,
																		pt_ch_att[esys][ech]->lna2.cur_value,
																		pt_ch_att[esys][ech]->digit_att1.cur_value,	
																		pt_ch_att[esys][ech]->analog_att1.cur_value,
																		pt->tIso.iso, 
																		pt->tAtt.ucIso);
					}
                }
            }
			rtt_printf( RTT_RF, "\n" );
        }
#if 0
        bsp_printf(RTT_RF, LOG_TEXT_MAGENTA"\n******** rf calibration para : \n");
        for(uint8_t esys = 0; esys < RF_SYS_NUM; esys++)
            for(uint8_t ech = 0; ech < RF_CH_NUM; ech++)
            {
                pt = &tRfChPara[esys][ech]; //射频独立参数
                bsp_printf(RTT_RF, ">>> %s_%s cali_gain: %-2d, cali_pow: %-2d, manu_gain: %-2d, manu_pow: %-2d, temp_gain: %-2d, temp_pow: %-2d\n", rf_sys_name[esys], rf_ch_name[ech],
                    pt->tAtt.scCaliGainOffset, pt->tPowList.scCaliOffset, pt->tAtt.ucManuCal, pt->tAlc.scAlcTheoreticalPoint, pt->tAtt.scTempOffset, pt->tPowList.scTempOffset);
            }
#endif
    }
#endif
}

/**
* @brief   设置是否允许进行模式切换   
* @param   en: [输入/出] 
* @return    
* @data    2024-03-26
* @note    
*/
void rf_set_mode_switch_en( BOOL_E en )
{
	tDevPublicPara.ucModeSwitchEn = ( 0 == en ) ? B_FALSE : B_TRUE;
}

/**
* @brief   判断开机是否进入测试模式
* @param   None
* @return    
* @data    2024-03-25
* @note    1. 开机插入USB, 则需等待10s, 未插入则不等待
*		   2. 等待中检测是否进入测试模式或者校准模式, 进入, 则跳过等待
*/
void jump_in_test_mode( void )
{
	USB_CONNECT_STATE_E usb_state = hi_usb_get_connect_state();	
	uint8_t cnt = 0;
	
	if( USB_CONNECT == hi_usb_get_connect_state() )
	{
		cnt = 100;
		while( cnt-- )
		{	
			if( cnt < 15 )
				rf_set_mode_switch_en( B_TRUE );
			else
				rf_set_mode_switch_en( B_FALSE );

			if( WORK_NORMAL == tDevPublicPara.ucWorkMode )
			{
				vTaskDelay( 100 );
			}
		}
		rf_set_mode_switch_en( B_FALSE );
	}
}
