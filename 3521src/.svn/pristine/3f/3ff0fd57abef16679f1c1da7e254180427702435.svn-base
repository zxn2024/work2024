/***************************** Copyright ****************************************
*
* (C) Copyright 2023, Shaanxi Tianji Communication
*  All Rights Reserved
*
* FileName   : bsp_testmode_hal_set.c
* Version    : none
* Author     :
* Date       : 2023-12-19
* Description: none
*******************************************************************************/
#include "main.h"
//#include "stdlib.h"
//#include "stdio.h"
//#include "string.h"
//#include "MCU_config.h"
#include "bsp_testmode_hal_set.h"
#include "system_config.h"
#include "bsp_testmode.h"
#include "bsp_rf_dc_switch.h"
#include "bsp_rf_att_set.h"
#include "bsp_rf_lna.h"
#include "lna.h"
#include "bsp_rf_global.h"
#include "bsp_rf_4g_band_sel.h"


/*---------------------- 端口定义 --------------------------------------*/


/*---------------------- 宏定义  ---------------------------------------*/


/*---------------------- 结构体 ----------------------------------------*/


/*---------------------- 常量 ------------------------------------------*/


/*---------------------- 全局变量 --------------------------------------*/
uint8_t tm_reload_flag;	//测试重新加载标识

/*---------------------- 局部变量 --------------------------------------*/


/*---------------------- 声明 ------------------------------------------*/

/*****************************************************************************
* @Name	  : tm_reload_para
* @Brief  : 重载参数
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2023-12-19
* @Note   :
*****************************************************************************/
void tm_reload_para(void)
{

}


void tm_save_dev_para(void)
{

}

/*****************************************************************************
* @Name	  : tm_sys_close
* @Brief  : 关闭系统
* @Param  : sys: [输入/出] 
* @Retval : 
* @Author : 
* @Data   : 2024-01-05
* @Note   :
*****************************************************************************/
void tm_sys_close( uint8_t sys )
{
	rf_dc_set( sys, PORT_OFF );
}


/*****************************************************************************
* @Name	  : tm_sys_open
* @Brief  : 测试模式下打开指定通道
* @Param  : sys: [输入/出]
**			band: 频段
* @Retval :
* @Author :
* @Data   : 2023-12-23
* @Note   :
*****************************************************************************/
void tm_sys_open( uint8_t band )
{
	uint8_t sys = 0;
	uint8_t res = B_FALSE;
	
	if((BAND_3 == band) || (BAND_8 == band) || (BAND_39 == band) || (BAND_40 == band) )
	{
		sys = RF_SYS_1;
		res = B_TRUE;
	}
	else if( BAND_41 == band )
	{
		sys = RF_SYS_2;
		res = B_TRUE;
	}
	else if( BAND_41M == band )
	{
		sys = RF_SYS_3;
		res = B_TRUE;
	}
	else
	{}
			
  if( B_FALSE == res )
			return;
	
	set_valid_band( sys, band );
	if( RF_SYS_1 == sys )
		pt_remap_after_4g_band_select();	//重新加载指针
	
	att_analog_pwm_set(sys, RF_CH_UP, 1, 0);
	att_analog_pwm_set(sys, RF_CH_UP, 2, 0);
	att_analog_pwm_set(sys, RF_CH_DW, 1, 0);
	att_analog_pwm_set(sys, RF_CH_DW, 2, 0);

	lna_set(sys, RF_CH_UP, 1, LNA_RF);
	lna_set(sys, RF_CH_UP, 2, LNA_RF);
	lna_set(sys, RF_CH_DW, 1, LNA_RF);
	lna_set(sys, RF_CH_DW, 2, LNA_RF);
	
	rf_dc_set( sys, PORT_ON );
}


/*****************************************************************************
* @Name	  : tm_sys_fpga_pa_init
* @Brief  : PA初始化
* @Param  : band: 频段
* @Retval : 
* @Author : 
* @Data   : 2024-01-09
* @Note   : 
*****************************************************************************/
void tm_sys_fpga_pa_init(RF_BAND_E band)
{
	switch(band)
	{
		case BAND_3:
            fpga_set_4g_freqband(BAND_3); // 开PA
            rtt_printf(RTT_RF, "\r\nset b3 band, open pa\n");
			break;

		case BAND_8:
			fpga_set_4g_freqband(BAND_8); // 开PA
			rtt_printf(RTT_RF, "\r\nset b8 band, open pa\n");
			break;

		case BAND_39:
            //fpga_set_4g_freqband(BAND_40);
            //hi_delay_ms(100);
            fpga_set_4g_freqband(BAND_39);     // 开PA
            rtt_printf(RTT_RF, "\r\nset b39 band, open up pa\n");
			break;

		case BAND_40:
            //fpga_set_4g_freqband(BAND_39);
            //hi_delay_ms(100);
            fpga_set_4g_freqband(BAND_40);     // 开PA
            rtt_printf(RTT_RF, "\r\nset b40 band, open up pa\n");
			break;
            
		case BAND_41:
            
			break;
        
        case BAND_41M:
            
			break;

		default:
			rtt_printf(RTT_RF, RTT_TEXT_RED"\r\nset band error\n");
			break;
	}
}


/*****************************************************************************
* @Name	  : tm_sys_fpga_pa_init
* @Brief  : 设置4g pa
* @Param  : state: 状态 0:关 / 1:开 / 2:上行单通 / 3:下行单通
* @Retval : 
* @Author : 
* @Data   : 2024-01-09
* @Note   : 
*****************************************************************************/
void tm_sys_fpga_4g_pa(RF_SWITCH_E state)
{
    switch(state)
    {
        case RF_SWITCH_CLOSE:   // 关
            fpga_4g_ul_pa_off();
            fpga_4g_dl_pa_off();
            rtt_printf(RTT_RF, "set up pa close, dw pa close\n");     
            break;
        
        case RF_SWITCH_OPEN:    // 开
            fpga_4g_ul_pa_on();
            fpga_4g_dl_pa_on();
            rtt_printf(RTT_RF, "set up pa open, dw pa open\n");     
            break;
        
        case RF_SWITCH_UP:  // 上行单通
            fpga_4g_dl_pa_off();
            fpga_4g_ul_pa_on();
            rtt_printf(RTT_RF, "set up pa open, dw pa close\n");     
            break;
        
        case RF_SWITCH_DW:  // 下行单通
            fpga_4g_ul_pa_off();
            fpga_4g_dl_pa_on();
            rtt_printf(RTT_RF, "set dw pa open, up pa close\n");    
            break;
        
        default:
            break;
    }
}




#if LOCAL_MODULE_TYPE == _RU
/*****************************************************************************
* @Name	  : tm_sys_4g_switch
* @Brief  : 设置4G时序
* @Param  : mode: 模式
* @Param  : tdd:  fdd or tdd
* @Param  : state: 状态 0:关 / 1:开 / 2:上行单通 / 3:下行单通
* @Retval : 
* @Author : 
* @Data   : 2024-01-09
* @Note   : RU
*****************************************************************************/
void tm_sys_fpga_switch(IS_4G_5G_E mode, IS_TDD_E tdd, RF_SWITCH_E state)
{
    if(IS_5G == mode)   // 5G
    {
        if(IS_TDD == tdd)   // TDD
        {
            switch(state)
            {
                case RF_SWITCH_CLOSE:     // 关
                    fpga_set_5g_tdd_mode(0);    // 关设置前先设置开
                    fpga_set_5g_tdd_mode(3);
                    rtt_printf(RTT_RF, "set 5g tdd up close, dw close\n");     
                    break;
                
                case RF_SWITCH_OPEN:     // 开
                    fpga_set_5g_tdd_mode(3);    // 开设置前先设置关
                    fpga_set_5g_tdd_mode(0);
                    rtt_printf(RTT_RF, "set 5g tdd up open, dw open\n");     
                    break;
                
                case RF_SWITCH_UP:      // 上行单通
                    fpga_set_5g_tdd_mode(1);    // 上行设置前先拉成下行
                    fpga_set_5g_tdd_mode(2);
                    rtt_printf(RTT_RF, "set 5g tdd up open, dw close\n");     
                    break;
                
                case RF_SWITCH_DW:      // 下行单通
                    fpga_set_5g_tdd_mode(2);    // 下行设置前先拉成上行
                    fpga_set_5g_tdd_mode(1);
                    rtt_printf(RTT_RF, "set 5g tdd dw open, up close\n");    
                    break;
                
                default:
                    break;
            }
        }
        else       // FDD
        {
            ;
        }
    }
    else                // 4G
    {
        if(IS_TDD == tdd)   // TDD
        {
            switch(state)
            {
                case RF_SWITCH_CLOSE:   // 关
                    fpga_set_4g_tdd_mode(0);    // 关设置前先设置开
                    fpga_set_4g_tdd_mode(3);
                    rtt_printf(RTT_RF, "set 4g tdd up close, dw close\n");     
                    break;
                
                case RF_SWITCH_OPEN:    // 开
                    fpga_set_4g_tdd_mode(3);    // 开设置前先设置关
                    fpga_set_4g_tdd_mode(0);
                    rtt_printf(RTT_RF, "set 4g tdd up open, dw open\n");     
                    break;
                
                case RF_SWITCH_UP:      // 上行单通
                    fpga_set_4g_tdd_mode(1);    // 上行设置前先拉成下行
                    fpga_set_4g_tdd_mode(2);
                    rtt_printf(RTT_RF, "set 4g tdd up open, dw close\n");     
                    break;
                
                case RF_SWITCH_DW:      // 下行单通
                    fpga_set_4g_tdd_mode(2);    // 下行设置前先拉成上行
                    fpga_set_4g_tdd_mode(1);
                    rtt_printf(RTT_RF, "set 4g tdd dw open, up close\n");    
                    break;
                
                default:
                    break;
            }
        }
        else       // FDD
        {
            switch(state)
            {
                case RF_SWITCH_CLOSE:   // 关
                    fpga_set_4g_tdd_mode(3);
                    rtt_printf(RTT_RF, "set 4g fdd up close, dw close\n");     
                    break;
                
                case RF_SWITCH_OPEN:    // 开
                    fpga_set_4g_tdd_mode(4);
                    rtt_printf(RTT_RF, "set 4g fdd up open, dw open\n");     
                    break;
                
                case RF_SWITCH_UP:      // 上行单通
                    fpga_set_4g_tdd_mode(2);
                    rtt_printf(RTT_RF, "set 4g fdd up open, dw close\n");     
                    break;
                
                case RF_SWITCH_DW:      // 下行单通
                    fpga_set_4g_tdd_mode(1);
                    rtt_printf(RTT_RF, "set 4g fdd dw open, up close\n");    
                    break;
                
                default:
                    break;
            }
        }
    }
}    
#endif

