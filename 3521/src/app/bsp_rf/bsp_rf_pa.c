/***************************** Copyright ****************************************
*
* (C) Copyright 2023, Shaanxi Tianji Communication
*  All Rights Reserved
*
* FileName   : bsp_rf_pa.c
* Version    : none
* Author     :
* Date       : 2023-11-16
* Description: none
*******************************************************************************/
#include "main.h"
#include "system_config.h"
#include "bsp_rf_pa.h"
#include "bi_pa.h"
#include "bsp_rf_global.h"
#include "fpga.h"
#include "rtt.h"

/*---------------------- 端口定义 --------------------------------------*/


/*---------------------- 宏定义  ---------------------------------------*/


/*---------------------- 结构体 ----------------------------------------*/


/*---------------------- 常量 ------------------------------------------*/


/*---------------------- 全局变量 --------------------------------------*/


/*---------------------- 局部变量 --------------------------------------*/


/*---------------------- 声明 ------------------------------------------*/



/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ HAL ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/





/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DRV ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*****************************************************************************
* @Name	  : rf_band_pa_init
* @Brief  : PA初始化
* @Param  : band: 频段
* @Retval : 
* @Author : 
* @Data   : 2024-01-09
* @Note   : 
*****************************************************************************/
void rf_band_pa_init(RF_BAND_E band)
{
	switch(band)
	{
		case BAND_3:
            fpga_set_4g_freqband(BAND_3); 		//开PA
            rtt_printf(RTT_RF, "\r\nset b3 band, open pa\n");
			break;

		case BAND_8:
			fpga_set_4g_freqband(BAND_8); 		//开PA
			rtt_printf(RTT_RF, "\r\nset b8 band, open pa\n");
			break;

		case BAND_39:
            fpga_set_4g_freqband(BAND_39);     //开PA
            rtt_printf(RTT_RF, "\r\nset b39 band, open up pa\n");
			break;

		case BAND_40:
            fpga_set_4g_freqband(BAND_40);     //开PA
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
* @Name	  : rf_4g_pa_state_set
* @Brief  : 设置4g pa
* @Param  : state: 状态 0:关 / 1:开 / 2:上行单通 / 3:下行单通
* @Retval : 
* @Author : 
* @Data   : 2024-01-09
* @Note   : 
*****************************************************************************/
void rf_4g_pa_state_set(RF_SWITCH_E state)
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





/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ APP ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*****************************************************************************
* @Name	  : pa_init
* @Brief  : pa初始化
* @Param  : sys: [输入/出] 
* @Retval : 
* @Author : 
* @Data   : 2024-01-23
* @Note   :
*****************************************************************************/
void pa_init( uint8_t sys )
{
	RF_BAND_E band = BAND_3;
	
    switch(sys)
    {
        case RF_SYS_1:
            band = (RF_BAND_E)get_valid_band(sys);
			rf_band_pa_init( band );
            break;

        case RF_SYS_2:
            break;

        case RF_SYS_3:
            break;

        default:
            break;
    }	
}


/*****************************************************************************
* @Name	  : pa_set
* @Brief  : PA开关设置
* @Param  : sys: [输入/出]
**			 ch: [输入/出]
**			 state: PA_ON  /  PA_OFF
* @Retval :
* @Author :
* @Data   : 2023-11-16
* @Note   :
*****************************************************************************/
void pa_set(uint8_t sys, uint8_t state)
{
    if( sys >= RF_SYS_NUM )
        return;

    if((state != PA_ON) && (state != PA_OFF))
        return;

    switch(sys)
    {
        case RF_SYS_1:
            break;

        case RF_SYS_2:
            break;

        case RF_SYS_3:
            break;

        default:
            break;
    }
}

