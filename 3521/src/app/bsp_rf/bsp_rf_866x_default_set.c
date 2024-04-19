/***************************** Copyright ****************************************
*
* (C) Copyright 2024, Shaanxi Tianji Communication
*  All Rights Reserved
*
* FileName   : bsp_rf_866x_default_set.c
* Version    : none
* Author     :
* Date       : 2024-01-02
* Description: none
*******************************************************************************/
#include "main.h"

#include "bsp_testmode_hal_set.h"
#include "bsp_rf_866x_default_set.h"
#include "ecr8668_driver.h"
#include "ecr8668.h"
#include "bsp_rf_866x.h"
#include "bsp_public_drv.h"
#include "rtt.h"
#include "bsp_rf_lna.h"
#include "bsp_rf_att_set.h"
#include "bsp_config.h"
#include "fpga.h"
#include "bsp_rf_global.h"
#include "bsp_rf_att.h"

/*---------------------- 端口定义 --------------------------------------*/


/*---------------------- 宏定义  ---------------------------------------*/
#define USE_ESWIN_CAT1	( 1 )	// 5G 模组选择, 1: 自研模组, 0: 其他模组

/*---------------------- 结构体 ----------------------------------------*/
typedef struct
{
    uint8_t  on_off_page;	//页内 / 页外 0: 页内, 1:业外
    uint32_t addr;			//地址
    uint32_t dat;			//数据
} ecr866x_data_t;


/*---------------------- 常量 ------------------------------------------*/


/*---------------------- 全局变量 --------------------------------------*/


/*---------------------- 局部变量 --------------------------------------*/


/*---------------------- 声明 ------------------------------------------*/
extern void Ecr866xRssiEn(TRX8668_FPGA_t * h8668, uint32_t ch);


/*****************************************************************************
* @Name	  : rf_866x_default_set
* @Brief  : 8668默认数据设置
* @Param  : sys: [输入/出]
**			 band: [输入/出]
**			 ch: [输入/出]
* @Retval :
* @Author :
* @Data   : 2024-01-02
* @Note   :
*****************************************************************************/
#if LOCAL_MODULE_TYPE == _MU
void rf_866x_default_set(uint8_t band)
{
    uint32_t state = 0;

    switch(band)
    {
        case BAND_3:
			//rf_set_modem_cmd(RF_SYS_1, 0, IS_FDD);
            rtt_printf(RTT_RF, "B3 OPEN RF...\n");
            rtt_printf(RTT_RF, "B3 866x init...\n");
            state = BSP_866x_Ex_Read(pt_866x_sys[RF_SYS_1], 0x20000004);
            rtt_printf(RTT_RF, "B3 866x state = %d\n", state);

			Ecr866xRssiEn(pt_866x_sys[RF_SYS_1], pt_866x_sys[RF_SYS_1]->ch);

            fpga_set_4g_freqband(BAND_3);
            rtt_printf(RTT_RF, "set b3 mipi ok\n");
			//rf_set_modem_cmd(RF_SYS_1, 1, IS_FDD);
            rtt_printf(RTT_RF, "B3 OPEN RF...\n");
            break;

        case BAND_8:
			//rf_set_modem_cmd(RF_SYS_1, 0,IS_FDD);
            rtt_printf(RTT_RF, "B8 OPEN RF...\n");
            rtt_printf(RTT_RF, "B8 866x init...\n");
            state = BSP_866x_Ex_Read(pt_866x_sys[RF_SYS_1], 0x20000004);
            rtt_printf(RTT_RF, "B8 866x state = %d\n", state);

			Ecr866xRssiEn(pt_866x_sys[RF_SYS_1], pt_866x_sys[RF_SYS_1]->ch);
		
            fpga_set_4g_freqband(BAND_8);
            //rtt_printf(RTT_RF, "set b8 mipi ok\n");
			//rf_set_modem_cmd(RF_SYS_1, 1,IS_FDD);
            rtt_printf(RTT_RF, "B8 OPEN RF...\n");
            break;

        case BAND_39:
			//rf_set_modem_cmd(RF_SYS_1, 0,IS_TDD);
            rtt_printf(RTT_RF, "B39 OPEN RF...\n");
            rtt_printf(RTT_RF, "B39 866x init...\n");
            state = BSP_866x_Ex_Read(pt_866x_sys[RF_SYS_1], 0x20000004);
            rtt_printf(RTT_RF, "B39 866x state = %d\n", state);

			Ecr866xRssiEn(pt_866x_sys[RF_SYS_1], pt_866x_sys[RF_SYS_1]->ch);

            fpga_set_4g_freqband(BAND_39);
            rtt_printf(RTT_RF, "set 39 mipi ok\n");
            break;

        case BAND_40:
			//rf_set_modem_cmd(RF_SYS_1, 0,IS_TDD);
            rtt_printf(RTT_RF, "B40 OPEN RF...");
            rtt_printf(RTT_RF, "B40 866x init...\n");
            state = BSP_866x_Ex_Read(pt_866x_sys[RF_SYS_1], 0x20000004);
            rtt_printf(RTT_RF, "B40 866x state = %d\n", state);
		
			Ecr866xRssiEn(pt_866x_sys[RF_SYS_1], pt_866x_sys[RF_SYS_1]->ch);

            fpga_set_4g_freqband(BAND_40);
            rtt_printf(RTT_RF, "set 40 mipi ok\n");
            break;

        case BAND_41:
            rtt_printf(RTT_RF, "NR 866x init...\n");
            state = BSP_866x_Ex_Read(pt_866x_sys[RF_SYS_2], 0x20000004);
            rtt_printf(RTT_RF, "NR 866x state = %d\n", state);

			Ecr866xRssiEn(pt_866x_sys[RF_SYS_2], pt_866x_sys[RF_SYS_1]->ch);
			
			rtt_printf(RTT_RF, "ext page: 0x00200938, 0x00001082\n");	//设置RSSI可读取模式指令1
            rtt_printf(RTT_RF, "ext page: 0x00200960, 0x0000B260\n");	//设置RSSI可读取模式指令2
            break;

        case BAND_41M:
            rtt_printf(RTT_RF, "MIMO 866x init...\n");
            state = BSP_866x_Ex_Read(pt_866x_sys[RF_SYS_3], 0x20000004);
            rtt_printf(RTT_RF, "MIMO 866x state = %d\n", state);

			Ecr866xRssiEn(pt_866x_sys[RF_SYS_3], pt_866x_sys[RF_SYS_1]->ch);
            break;

        default:
            break;
    }
}
#else
void rf_866x_default_set(uint8_t band)
{
    uint32_t state = 0;

    switch(band)
    {
        case BAND_3:
            rtt_printf(RTT_RF, "B3 866x init...\n");
            state = BSP_866x_Ex_Read(pt_866x_sys[RF_SYS_1], 0x20000004);
            rtt_printf(RTT_RF, "B3 866x state = %d\n", state);
        
            Ecr866xRssiEn(pt_866x_sys[RF_SYS_1], pt_866x_sys[RF_SYS_1]->ch);
                        
            tm_sys_fpga_pa_init(BAND_3);
            tm_sys_fpga_switch(IS_4G, IS_FDD, RF_SWITCH_OPEN);   // FDD默认上下行全开
            break;
            
        case BAND_8:
            rtt_printf(RTT_RF, "B8 866x init...\n");
            state = BSP_866x_Ex_Read(pt_866x_sys[RF_SYS_1], 0x20000004);
            rtt_printf(RTT_RF, "B8 866x state = %d\n", state);
        
            Ecr866xRssiEn(pt_866x_sys[RF_SYS_1], pt_866x_sys[RF_SYS_1]->ch);
                        
            tm_sys_fpga_pa_init(BAND_8);
            tm_sys_fpga_switch(IS_4G, IS_FDD, RF_SWITCH_OPEN);   // FDD默认上下行全开
            break;

        case BAND_39:
            rtt_printf(RTT_RF, "B39 866x init...\n");
            state = BSP_866x_Ex_Read(pt_866x_sys[RF_SYS_1], 0x20000004);
            rtt_printf(RTT_RF, "B39 866x state = %d\n", state);

            Ecr866xRssiEn(pt_866x_sys[RF_SYS_1], pt_866x_sys[RF_SYS_1]->ch);
                        
            tm_sys_fpga_pa_init(BAND_39);
            tm_sys_fpga_switch(IS_4G, IS_TDD, RF_SWITCH_DW);   // TDD默认开下行
            break;

        case BAND_40:
            rtt_printf(RTT_RF, "B40 866x init...\n");
            state = BSP_866x_Ex_Read(pt_866x_sys[RF_SYS_1], 0x20000004);
            rtt_printf(RTT_RF, "B40 866x state = %d\n", state);

            Ecr866xRssiEn(pt_866x_sys[RF_SYS_1], pt_866x_sys[RF_SYS_1]->ch);
                        
            tm_sys_fpga_pa_init(BAND_40);
            tm_sys_fpga_switch(IS_4G, IS_TDD, RF_SWITCH_DW);   // TDD默认开下行
            break;

        case BAND_41:
            rtt_printf(RTT_RF, "N41 866x init...\n");
            state = BSP_866x_Ex_Read(pt_866x_sys[RF_SYS_2], 0x20000004);
            rtt_printf(RTT_RF, "N41 866x state = %d\n", state);

            Ecr866xRssiEn(pt_866x_sys[RF_SYS_2], pt_866x_sys[RF_SYS_2]->ch);
                        
            tm_sys_fpga_switch(IS_5G, IS_TDD, RF_SWITCH_DW);   // TDD默认开下行
            break;

        case BAND_41M:
            rtt_printf(RTT_RF, "N41_MIMO 866x init...\n");
            state = BSP_866x_Ex_Read(pt_866x_sys[RF_SYS_3], 0x20000004);
            rtt_printf(RTT_RF, "N41_MIMO 866x state = %d\n", state);

            Ecr866xRssiEn(pt_866x_sys[RF_SYS_3], pt_866x_sys[RF_SYS_3]->ch);
                        
            tm_sys_fpga_switch(IS_5G, IS_TDD, RF_SWITCH_DW);   // TDD默认开下行
            break;

        default:
            break;
    }
}
#endif


/*****************************************************************************
* @Name	  : rf_ch_lna_att_set
* @Brief  :
* @Param  : band: [输入/出]
**			 ch: [输入/出]
* @Retval :
* @Author :
* @Data   : 2024-01-02
* @Note   :
*****************************************************************************/
#if LOCAL_MODULE_TYPE == _MU
void rf_ch_lna_att_default_set(uint8_t band, uint8_t ch)
{
//	if( WORK_CALIB == tDevPublicPara.ucWorkMode )	//非校准模式下, 不需要设置默认值
//		return;
	
    switch(band)
    {
        case BAND_3:
        case BAND_8:
			rf_set_modem_cmd(RF_SYS_1, 0,IS_FDD);
			hi_delay_ms( 50 );
			if( WORK_CALIB == tDevPublicPara.ucWorkMode )
			{
				lna_set(RF_SYS_1, RF_CH_DW, 1, LNA_RF);
				lna_set(RF_SYS_1, RF_CH_DW, 2, LNA_RF);
				lna_set(RF_SYS_1, RF_CH_UP, 1, LNA_RF);
				att_analog_pwm_set(RF_SYS_1, RF_CH_UP, 1, 0);
				att_analog_pwm_set(RF_SYS_1, RF_CH_UP, 2, 0);
				att_analog_pwm_set(RF_SYS_1, RF_CH_DW, 1, 0);
			}
			else if( WORK_PL == tDevPublicPara.ucWorkMode )
			{
				att_set_force_fresh( RF_SYS_1 );
			}
			rf_set_modem_cmd(RF_SYS_1, 1,IS_FDD);
            break;

        case BAND_39:
            if(RF_CH_UP == ch)
            {
				rf_set_modem_cmd(RF_SYS_1, 0,IS_TDD);
				hi_delay_ms( 50 );
				if( WORK_CALIB == tDevPublicPara.ucWorkMode )
				{
					lna_set(RF_SYS_1, RF_CH_DW, 1, LNA_BYS);
					lna_set(RF_SYS_1, RF_CH_DW, 2, LNA_BYS);
					lna_set(RF_SYS_1, RF_CH_UP, 1, LNA_RF);
					att_analog_pwm_set(RF_SYS_1, RF_CH_UP, 1, 0);
					att_analog_pwm_set(RF_SYS_1, RF_CH_UP, 2, 0);
					att_analog_pwm_set(RF_SYS_1, RF_CH_DW, 1, 0);
				}
				else if( WORK_PL == tDevPublicPara.ucWorkMode )
				{
					att_set_force_fresh( RF_SYS_1 );
				}
				rf_set_modem_cmd(RF_SYS_1, 2,IS_TDD);
                rtt_printf(RTT_RF, "B39 UP OPEN RF...\n");
            }
            else
            {
				rf_set_modem_cmd(RF_SYS_1, 0,IS_TDD);
				hi_delay_ms( 50 );
				if( WORK_CALIB == tDevPublicPara.ucWorkMode )
				{
					lna_set(RF_SYS_1, RF_CH_UP, 1, LNA_BYS);
					lna_set(RF_SYS_1, RF_CH_DW, 1, LNA_RF);
					lna_set(RF_SYS_1, RF_CH_DW, 2, LNA_RF);
					att_analog_pwm_set(RF_SYS_1, RF_CH_UP, 1, 0);
					att_analog_pwm_set(RF_SYS_1, RF_CH_UP, 2, 0);
					att_analog_pwm_set(RF_SYS_1, RF_CH_DW, 1, 0);
				}
				else if( WORK_PL == tDevPublicPara.ucWorkMode )
				{
					att_set_force_fresh( RF_SYS_1 );
				}
				rf_set_modem_cmd(RF_SYS_1, 3,IS_TDD);
                rtt_printf(RTT_RF, "B39 DW OPEN RF...\n");
            }
            break;


        case BAND_40:
            if(RF_CH_UP == ch)
            {
				rf_set_modem_cmd(RF_SYS_1, 0,IS_TDD);
				hi_delay_ms( 50 );
				if( WORK_CALIB == tDevPublicPara.ucWorkMode )
				{
					lna_set(RF_SYS_1, RF_CH_DW, 1, LNA_BYS);
					lna_set(RF_SYS_1, RF_CH_DW, 2, LNA_BYS);
					lna_set(RF_SYS_1, RF_CH_UP, 1, LNA_RF);
					att_analog_pwm_set(RF_SYS_1, RF_CH_UP, 1, 0);
					att_analog_pwm_set(RF_SYS_1, RF_CH_UP, 2, 0);
					att_analog_pwm_set(RF_SYS_1, RF_CH_DW, 1, 0);
				}
				else if( WORK_PL == tDevPublicPara.ucWorkMode )
				{
					att_set_force_fresh( RF_SYS_1 );
				}
				rf_set_modem_cmd(RF_SYS_1, 2,IS_TDD);
                rtt_printf(RTT_RF, "B40 UP OPEN RF...\n");
            }
            else
            {
				rf_set_modem_cmd(RF_SYS_1, 0,IS_TDD);
				hi_delay_ms( 50 );
				if( WORK_CALIB == tDevPublicPara.ucWorkMode )
				{
					lna_set(RF_SYS_1, RF_CH_UP, 1, LNA_BYS);
					lna_set(RF_SYS_1, RF_CH_DW, 1, LNA_RF);
					lna_set(RF_SYS_1, RF_CH_DW, 2, LNA_RF);
					att_analog_pwm_set(RF_SYS_1, RF_CH_UP, 1, 0);
					att_analog_pwm_set(RF_SYS_1, RF_CH_UP, 2, 0);
					att_analog_pwm_set(RF_SYS_1, RF_CH_DW, 1, 0);
				}
				else if( WORK_PL == tDevPublicPara.ucWorkMode )
				{
					att_set_force_fresh( RF_SYS_1 );
				}
				rf_set_modem_cmd(RF_SYS_1, 3,IS_TDD);
                rtt_printf(RTT_RF, "B40 DW OPEN RF...\n");
            }
            break;

        case BAND_41:
            rf_nr_up_silent_ctrl.ctl(&rf_nr_up_silent_ctrl, PORT_OFF);
            if(RF_CH_UP == ch)
            {
				rf_set_modem_cmd(RF_SYS_2, 2,IS_TDD);
                rtt_printf(RTT_RF, "B41 UP OPEN RF...\n");
				
				if( WORK_CALIB == tDevPublicPara.ucWorkMode )
				{
					lna_set(RF_SYS_2, RF_CH_DW, 1, LNA_BYS);
					lna_set(RF_SYS_2, RF_CH_DW, 2, LNA_BYS);
					lna_set(RF_SYS_2, RF_CH_UP, 1, LNA_RF);
					
					
					att_analog_pwm_set(RF_SYS_2, RF_CH_DW, 1, 999);
					att_analog_pwm_set(RF_SYS_2, RF_CH_UP, 1, 999);
					att_analog_pwm_set(RF_SYS_2, RF_CH_UP, 2, 999);
				}
				else if( WORK_PL == tDevPublicPara.ucWorkMode )
				{
					att_set_force_fresh( RF_SYS_2 );
				}
            }
            else
            {
				rf_set_modem_cmd(RF_SYS_2, 3,IS_TDD);
                rtt_printf(RTT_RF, "B41 DW OPEN RF...\n");
				if( WORK_CALIB == tDevPublicPara.ucWorkMode )
				{
					lna_set(RF_SYS_2, RF_CH_UP, 1, LNA_BYS);
					lna_set(RF_SYS_2, RF_CH_DW, 1, LNA_RF);
					lna_set(RF_SYS_2, RF_CH_DW, 2, LNA_RF);
					att_analog_pwm_set(RF_SYS_2, RF_CH_UP, 1, 0);
					att_analog_pwm_set(RF_SYS_2, RF_CH_UP, 2, 0);
					att_analog_pwm_set(RF_SYS_2, RF_CH_DW, 1, 0);
					//att_analog_pwm_set( RF_SYS_2, RF_CH_DW, 2, 0 );
				}
				else if( WORK_PL == tDevPublicPara.ucWorkMode )
				{
					att_set_force_fresh( RF_SYS_2 );
				}
            }
            break;

        case BAND_41M:
            rf_mimo_up_silent_ctrl.ctl(&rf_mimo_up_silent_ctrl, PORT_OFF);
            if(RF_CH_UP == ch)
            {
				rf_set_modem_cmd(RF_SYS_3, 2,IS_TDD);
                rtt_printf(RTT_RF, "B41_MIMO UP OPEN RF...\n");
				if( WORK_CALIB == tDevPublicPara.ucWorkMode )
				{
					lna_set(RF_SYS_3, RF_CH_DW, 1, LNA_BYS);
					lna_set(RF_SYS_3, RF_CH_DW, 2, LNA_BYS);
					lna_set(RF_SYS_3, RF_CH_UP, 1, LNA_RF);
					att_analog_pwm_set(RF_SYS_3, RF_CH_DW, 1, 0);
					//att_analog_pwm_set( RF_SYS_2, RF_CH_DW, 2, 0 );
					att_analog_pwm_set(RF_SYS_3, RF_CH_UP, 1, 0);
					att_analog_pwm_set(RF_SYS_3, RF_CH_UP, 2, 0);
				}
				else if( WORK_PL == tDevPublicPara.ucWorkMode )
				{
					att_set_force_fresh( RF_SYS_3 );
				}
            }
            else
            {
				rf_set_modem_cmd(RF_SYS_2, 3,IS_TDD);
                rtt_printf(RTT_RF, "B41_MIMO DW OPEN RF...\n");
				if( WORK_CALIB == tDevPublicPara.ucWorkMode )
				{
					lna_set(RF_SYS_3, RF_CH_UP, 1, LNA_BYS);
					lna_set(RF_SYS_3, RF_CH_DW, 1, LNA_RF);
					lna_set(RF_SYS_3, RF_CH_DW, 2, LNA_RF);
					att_analog_pwm_set(RF_SYS_3, RF_CH_UP, 1, 0);
					att_analog_pwm_set(RF_SYS_3, RF_CH_UP, 2, 0);
					att_analog_pwm_set(RF_SYS_3, RF_CH_DW, 1, 0);
					//att_analog_pwm_set( RF_SYS_2, RF_CH_DW, 2, 0 );
				}
				else if( WORK_PL == tDevPublicPara.ucWorkMode )
				{
					att_set_force_fresh( RF_SYS_3 );
				}
            }
            break;

        default:
            break;
    }
}

#else
void rf_ch_lna_att_default_set(uint8_t band, uint8_t ch)
{
    switch(band)
    {
        case BAND_3:
        case BAND_8:
            lna_set(RF_SYS_1, RF_CH_UP, 1, LNA_RF);
            lna_set(RF_SYS_1, RF_CH_UP, 2, LNA_RF);
            lna_set(RF_SYS_1, RF_CH_DW, 1, LNA_RF);
            att_analog_pwm_set(RF_SYS_1, RF_CH_UP, 1, 200); //衰一部分
            att_analog_pwm_set(RF_SYS_1, RF_CH_DW, 1, 999); //不衰
            att_analog_pwm_set(RF_SYS_1, RF_CH_DW, 2, 0);   //全衰
            break;

        case BAND_39:
            if(RF_CH_UP == ch)
            {
                lna_set(RF_SYS_1, RF_CH_UP, 1, LNA_RF);
                lna_set(RF_SYS_1, RF_CH_UP, 2, LNA_RF);
                lna_set(RF_SYS_1, RF_CH_DW, 1, LNA_BYS);
                att_analog_pwm_set(RF_SYS_1, RF_CH_UP, 1, 999); //不衰
                att_analog_pwm_set(RF_SYS_1, RF_CH_DW, 1, 999); //不衰
                att_analog_pwm_set(RF_SYS_1, RF_CH_DW, 2, 0);   //全衰
            }
            else
            {
                lna_set(RF_SYS_1, RF_CH_UP, 1, LNA_BYS);
                lna_set(RF_SYS_1, RF_CH_UP, 2, LNA_BYS);
                lna_set(RF_SYS_1, RF_CH_DW, 1, LNA_RF);
                att_analog_pwm_set(RF_SYS_1, RF_CH_UP, 1, 0);
                att_analog_pwm_set(RF_SYS_1, RF_CH_DW, 1, 999);
                att_analog_pwm_set(RF_SYS_1, RF_CH_DW, 2, 0);
            }
            break;

        case BAND_40:
            if(RF_CH_UP == ch)
            {
                lna_set(RF_SYS_1, RF_CH_UP, 1, LNA_RF);
                lna_set(RF_SYS_1, RF_CH_UP, 2, LNA_RF);
                lna_set(RF_SYS_1, RF_CH_DW, 1, LNA_BYS);
                att_analog_pwm_set(RF_SYS_1, RF_CH_UP, 1, 999); //不衰
                att_analog_pwm_set(RF_SYS_1, RF_CH_DW, 1, 999); //不衰
                att_analog_pwm_set(RF_SYS_1, RF_CH_DW, 2, 0);   //全衰
            }
            else
            {
                lna_set(RF_SYS_1, RF_CH_UP, 1, LNA_BYS);
                lna_set(RF_SYS_1, RF_CH_UP, 2, LNA_BYS);
                lna_set(RF_SYS_1, RF_CH_DW, 1, LNA_RF);
                att_analog_pwm_set(RF_SYS_1, RF_CH_UP, 1, 0);
                att_analog_pwm_set(RF_SYS_1, RF_CH_DW, 1, 999);
                att_analog_pwm_set(RF_SYS_1, RF_CH_DW, 2, 0);
            }
            break;

        case BAND_41:
            if(RF_CH_UP == ch)
            {
                lna_set(RF_SYS_2, RF_CH_UP, 1, LNA_RF);
                lna_set(RF_SYS_2, RF_CH_UP, 2, LNA_RF);
                lna_set(RF_SYS_2, RF_CH_DW, 1, LNA_BYS);
                att_analog_pwm_set(RF_SYS_2, RF_CH_UP, 1, 999);
                att_analog_pwm_set(RF_SYS_2, RF_CH_DW, 1, 0);
                att_analog_pwm_set(RF_SYS_2, RF_CH_DW, 2, 0);
            }
            else
            {
                lna_set(RF_SYS_2, RF_CH_UP, 1, LNA_BYS);
                lna_set(RF_SYS_2, RF_CH_UP, 2, LNA_BYS);
                lna_set(RF_SYS_2, RF_CH_DW, 1, LNA_RF);
                att_analog_pwm_set(RF_SYS_2, RF_CH_UP, 1, 0);
                att_analog_pwm_set(RF_SYS_2, RF_CH_DW, 1, 999);
                att_analog_pwm_set(RF_SYS_2, RF_CH_DW, 2, 0);
            }
            break;

        case BAND_41M:
            if(RF_CH_UP == ch)
            {
                lna_set(RF_SYS_3, RF_CH_UP, 1, LNA_RF);
                lna_set(RF_SYS_3, RF_CH_UP, 2, LNA_RF);
                lna_set(RF_SYS_3, RF_CH_DW, 1, LNA_BYS);
                att_analog_pwm_set(RF_SYS_3, RF_CH_UP, 1, 999);
                att_analog_pwm_set(RF_SYS_3, RF_CH_DW, 1, 0);
                att_analog_pwm_set(RF_SYS_3, RF_CH_DW, 2, 0);
            }
            else
            {
                lna_set(RF_SYS_3, RF_CH_UP, 1, LNA_BYS);
                lna_set(RF_SYS_3, RF_CH_UP, 2, LNA_BYS);
                lna_set(RF_SYS_3, RF_CH_DW, 1, LNA_RF);
                att_analog_pwm_set(RF_SYS_3, RF_CH_UP, 1, 0);
                att_analog_pwm_set(RF_SYS_3, RF_CH_DW, 1, 999);
                att_analog_pwm_set(RF_SYS_3, RF_CH_DW, 2, 0);
            }
            break;

        default:
            break;
    }
}
#endif



#if LOCAL_MODULE_TYPE == _MU
//3: 关闭, 4:开  1:下行单通 2: 上行单通
/*****************************************************************************
* @Name	  : rf_set_modem_cmd
* @Brief  :
* @Param  : sys: [输入/出]
**			 state: 0:关, 1:开, 2:上行单通, 3:下行单通
* @Retval :
* @Author :
* @Data   : 2024-01-04
* @Note   :
*****************************************************************************/
#if( USE_ESWIN_CAT1 == 1 )
void rf_set_modem_cmd(uint8_t sys, uint8_t state, IS_TDD_E tdd_or_fdd )
{
	  gpio_sync_mode_E gpio_up   = GPIO_ALL_LO;
	  gpio_sync_mode_E gpio_down = GPIO_ALL_LO;

		if(0 == state)//关
		{
			gpio_up = GPIO_ALL_LO;
			gpio_down=GPIO_ALL_LO;
		}
		else if(1 == state)//开
		{					
			if(IS_TDD == tdd_or_fdd)   // TDD
			{
				gpio_up = GPIO_TDD;
				gpio_down=GPIO_TDD;
			}else 
			{
				gpio_up = GPIO_ALL_HI;
				gpio_down=GPIO_ALL_HI;
			}
		}
		else if(2 == state)//上行单通
		{
			 gpio_up = GPIO_ALL_HI;
			 gpio_down=GPIO_ALL_LO;
		}
		else if(3 == state) //下行单通
		{
			 gpio_up = GPIO_ALL_LO;
			 gpio_down=GPIO_ALL_HI;
		}else {}
		
		if(RF_SYS_1 == sys)
		{
			modem_set_channel_output_mode(hModem.hw_config.lte_gpio_group, gpio_up, gpio_down); //CH_LTE
		}else 
		{
		    modem_set_channel_output_mode(hModem.hw_config.nr_gpio_group, gpio_up, gpio_down);	//CH_NR
	    }
}
#else
void rf_set_modem_cmd(uint8_t sys, uint8_t state, IS_TDD_E tdd_or_fdd )
{
    char acBuf[50] = "AT*L1DEBUG=0201000000003\r\n";	//5G下行

    if(RF_SYS_1 == sys)
    {
        acBuf[14] = 0x31;
        if(0 == state)
        {
            acBuf[23] = 0x33;
        }
        else if(2 == state)
        {
            acBuf[23] = 0x32;
        }
        else if(3 == state)
        {
            acBuf[23] = 0x31;
        }
        else
        {
            acBuf[23] = 0x34;
        }
    }
    else	//5G
    {
        acBuf[14] = 0x30;
        if(0 == state)
        {
            acBuf[23] = 0x33;
        }
        else if(2 == state)
        {
            acBuf[23] = 0x32;
        }
        else if(3 == state)
        {
            acBuf[23] = 0x31;
        }
        else
        {
            acBuf[23] = 0x34;
        }
    }

    uint8_t len = strlen(acBuf);
    memcpy(uart_modem_fun.tpt_data->pt_tx_data, acBuf, len);

    rtt_printf(RTT_RF, "sent to modem: %s", acBuf);
    uart_modem_fun.send(uart_modem_fun.tpt_data, len);
}
#endif

#endif


/*****************************************************************************
* @Name	  : get_bwid_by_bw
* @Brief  : 从从带宽数据获取带宽编号
* @Param  : band: [输入/出]
**			bw: 带宽, 10 / 15 / 20 / .....
**			bw_id: 计算后的带宽编号
* @Retval :
* @Author :
* @Data   : 2024-01-04
* @Note   :
*	PLC_LTE_20MHZ  <--->  B3_20M   = 0
*	PLC_LTE_25MHZ  <--->  B3_25M   = 1
*	PLC_LTE_10MHZ  <--->  B8_10M   = 0
*	PLC_LTE_15MHZ  <--->  B8_15M   = 1
*	PLC_LTE_10MHZ  <--->  B39_10M  = 0
*	PLC_LTE_20MHZ  <--->  B39_20M  = 1
*	PLC_LTE_30MHZ  <--->  B39_30M  = 2
*	PLC_LTE_20MHZ  <--->  B40_20M  = 0
*	PLC_LTE_50MHZ  <--->  B40_50M  = 1
*	PLC_NR_20MHZ   <--->  B41_20M  = 0
*	PLC_NR_60MHZ   <--->  B41_60M  = 1
*	PLC_NR_60MHZ   <--->  B41_80M  = 2
*	PLC_NR_100MHZ  <--->  B41_100M = 3
*****************************************************************************/
Status_t get_bwid_by_bw(uint8_t band, uint8_t bw, uint8_t *bw_id)
{
    Status_t res = STAT_ERROR;

    switch(bw)
    {
        case PLC_LTE_10MHZ: 	//(10)         // LTE 10 MHz
            if(BAND_8 == band)
            {
                *bw_id = B8_10M;
                res    = STAT_OK;
            }
            else if(BAND_39 == band)
            {
                *bw_id = B39_10M;
                res = STAT_OK;
            }
            else
                res = STAT_ERROR;
            break;

        case PLC_LTE_15MHZ: 	//(15)         // LTE 15 MHz
            if(BAND_8 == band)
            {
                *bw_id = B8_15M;
                res = STAT_OK;
            }
            else
                res = STAT_ERROR;
            break;

        case PLC_LTE_20MHZ: 	//(20)         // LTE 20 MHz
            if(BAND_3 == band)
            {
                *bw_id = B3_20M;
                res = STAT_OK;
            }
            else if(BAND_39 == band)
            {
                *bw_id = B39_20M;
                res = STAT_OK;
            }
            else if(BAND_40 == band)
            {
                *bw_id = B40_20M;
                res = STAT_OK;
            }
            else if((BAND_41 == band) || (BAND_41M == band))
            {
                *bw_id = B41_20M;
                res = STAT_OK;
            }
            else
                res = STAT_ERROR;
            break;

        case PLC_LTE_25MHZ: 	//(25)         // LTE 15 MHz
            if(BAND_3 == band)
            {
                *bw_id = B3_25M;
                res = STAT_OK;
            }
            else
                res = STAT_ERROR;
            break;

        case PLC_LTE_30MHZ: 	//(30)         // LTE 20 MHz
            if(BAND_39 == band)
            {
                *bw_id = B39_30M;
                res = STAT_OK;
            }
            else
                res = STAT_ERROR;
            break;

        case PLC_LTE_50MHZ: 	//(50)         // LTE 20 MHz
            if(BAND_40 == band)
            {
                *bw_id = B40_50M;
                res = STAT_OK;
            }
            else
                res = STAT_ERROR;
            break;


        case PLC_NR_60MHZ: 		//(60)         // NR 60 MHz
            if((BAND_41 == band) || (BAND_41M == band))
            {
                *bw_id = B41_60M;
                res = STAT_OK;
            }
            else
                res = STAT_ERROR;
            break;

        case PLC_NR_80MHZ: 		//(80)         // NR 80 MHz
            if((BAND_41 == band) || (BAND_41M == band))
            {
                *bw_id = B41_80M;
                res = STAT_OK;
            }
            else
                res = STAT_ERROR;
            break;

        case PLC_NR_100MHZ: 	//(100)        // NR 100 MHz
            if((BAND_41 == band) || (BAND_41M == band))
            {
                *bw_id = B41_100M;
                res = STAT_OK;
            }
            else
                res = STAT_ERROR;
            break;

        default:
            res = STAT_ERROR;
            break;
    }
    return res;
}


