/***************************** Copyright ****************************************
*
* (C) Copyright 2022, Shaanxi Tianji Comunication
* All Rights Reserved
*
* FileName   : bsp_rf_att_set.c
* Version    : none
* Author     : none
* Date       : 2023-10-17
* Description: none
*******************************************************************************/

#include "bsp_public_drv.h"

#include "bsp_dev_para.h"
#include "rtt.h"
#include "bsp_rf_att_set.h"

#include "bsp_rf_dc_switch.h"
#include "bsp_rf_global.h"
#include "bsp_config.h"
#include "ATT_analog.h"
#include "bsp_rf_4g_band_sel.h"
#include "ecr8668_driver.h"

/*---------------------- 端口定义 --------------------------------------*/


/*---------------------- 宏定义  ---------------------------------------*/


/*---------------------- 结构体 ----------------------------------------*/


/*---------------------- 常量 ------------------------------------------*/


/*---------------------- 全局变量 --------------------------------------*/


/*---------------------- 局部变量 --------------------------------------*/



/*****************************************************************************
* @Name	  : att_sys1_set
* @brief  : 通道1ATT设置
* @param  : ch : 通道号, RF_CH_UP / RF_CH_DW
* @retval : None
* @author :
* @Data   : 2023-10-17
*****************************************************************************/
void att_digit_set(uint8_t sys, uint8_t ch, uint8_t value)
{
    TRX8668_FPGA_t * handle_att = NULL;


	#if( _MU == LOCAL_MODULE_TYPE )
		if(RF_CH_UP == ch)
			return;
	#else
		if(RF_CH_DW == ch)
			return;
	#endif

    switch(sys)
    {
        case RF_SYS_1:
            handle_att = &h8668_LTE;
            break;

        case RF_SYS_2:
            handle_att = &h8668_NR;
            break;

        case RF_SYS_3:
            handle_att = &h8668_NR_MIMO;
            break;

        default:
            break;
    }

    if(NULL != handle_att)
        BSP_8668AttSet(handle_att, value);
}

/*****************************************************************************
* @Name	  : att_sys2_set
* @brief  : 通道 ATT设置
* @param  : ch : 通道号, RF_CH_UP / RF_CH_DW
* @retval : None
* @author :
* @Data   : 2023-10-17
*****************************************************************************/
#if LOCAL_MODULE_TYPE == _MU    // MU
void att_analog_set(uint8_t sys, uint8_t ch, uint8_t id, uint8_t value)
{
    if((sys >= RF_SYS_NUM) || (ch >= RF_CH_NUM) || (id > 2) || (id == 0))
        return;

    if((ch == RF_CH_DW) && (id > 2))
        return;

    ATT_analog_t * pt = NULL;

    switch(sys)
    {
        case RF_SYS_1:
            if(RF_CH_UP == ch)
            {
                pt = (1 == id) ? pt_att_analog_4g_up_1  : pt_att_analog_4g_up_2;
            }
            else
            {
                pt = &att_analog_4g_dw;
            }
            break;

        case RF_SYS_2:
            if(RF_CH_UP == ch)
            {
                pt = (1 == id) ? &att_analog_nr_up_1 : &att_analog_nr_up_2;
            }
            else
            {
                pt = &att_analog_nr_dw;
            }
            break;

        case RF_SYS_3:
            if(RF_CH_UP == ch)
            {
                pt = (1 == id) ? &att_analog_mimo_up_1 : &att_analog_mimo_up_2;
            }
            else
            {
                pt = &att_analog_mimo_dw;
            }
            break;

        default:
            pt = NULL;
            break;
    }

    if(NULL != pt)
         pt->set(pt, value);
}
#else   // RU
void att_analog_set(uint8_t sys, uint8_t ch, uint8_t id, uint8_t value)
{
    if((sys >= RF_SYS_NUM) || (ch >= RF_CH_NUM) || (id > 2) || (id == 0))
        return;

    if((ch == RF_CH_DW) && (id > 2))
        return;

    ATT_analog_t * pt = NULL;;

    switch(sys)
    {
        case RF_SYS_1:
            if(RF_CH_UP == ch)
            {
                pt = &att_analog_4g_up;
            }
            else
            {
                pt = (1 == id) ? pt_att_analog_4g_dw_1  : pt_att_analog_4g_dw_2;
            }
            break;

        case RF_SYS_2:
            if(RF_CH_UP == ch)
            {
                pt = &att_analog_nr_up;
            }
            else
            {
                pt = (1 == id) ? &att_analog_nr_dw_1 : &att_analog_nr_dw_2;
            }
            break;

        case RF_SYS_3:
            if(RF_CH_UP == ch)
            {
                pt = &att_analog_mimo_up;
            }
            else
            {
                pt = (1 == id) ? &att_analog_mimo_dw_1 : &att_analog_mimo_dw_2;
            }
            break;

        default:
            pt = NULL;
            break;
    }

    if(NULL != pt)
        pt->set(pt, value);
}
#endif

/*****************************************************************************
* @Name	  : att_analog_pwm_set
* @Brief  : 通道 ATT PWM设置
* @Param  : sys: [输入/出]
**			 ch: 通道号, RF_CH_UP / RF_CH_DW
**			 id: [输入/出]
**			 pwm_value: [输入/出]
* @Retval :
* @Author :
* @Data   : 2023-12-19
* @Note   : 用于采集模拟衰减表
*****************************************************************************/
#if LOCAL_MODULE_TYPE == _MU    // MU
void att_analog_pwm_set(uint8_t sys, uint8_t ch, uint8_t id, uint16_t pwm_value)
{
    if((sys >= RF_SYS_NUM) || (ch >= RF_CH_NUM) || (id > 2) || (id == 0))
        return;

    if((ch == RF_CH_DW) && (id > 2))
        return;

    ATT_analog_t * pt = NULL;;

    switch(sys)
    {
        case RF_SYS_1:
            if(RF_CH_UP == ch)
            {
                pt = (1 == id) ? pt_att_analog_4g_up_1  : pt_att_analog_4g_up_2;
            }
            else
            {
                pt = &att_analog_4g_dw;
            }
            break;

        case RF_SYS_2:
            if(RF_CH_UP == ch)
            {
                pt = (1 == id) ? &att_analog_nr_up_1 : &att_analog_nr_up_2;
            }
            else
            {
                pt = &att_analog_nr_dw;
            }
            break;

        case RF_SYS_3:
            if(RF_CH_UP == ch)
            {
                pt = (1 == id) ? &att_analog_mimo_up_1 : &att_analog_mimo_up_2;
            }
            else
            {
                pt = &att_analog_mimo_dw;
            }
            break;
            
        default:
            pt = NULL;
            break;
    }

    if(NULL != pt)
        pt->pwm_set(pt, pwm_value);
}
uint16_t att_analog_pwm_get(uint8_t sys, uint8_t ch, uint8_t id)
{
    uint16_t pwm_value = 0;

    if((sys >= RF_SYS_NUM) || (ch >= RF_CH_NUM) || (id > 2) || (id == 0))
        return 0;

    if((ch == RF_CH_DW) && (id > 2))
        return 0;

    ATT_analog_t * pt = NULL;;

    switch(sys)
    {
        case RF_SYS_1:
            if(RF_CH_UP == ch)
            {
                pt = (1 == id) ? pt_att_analog_4g_up_1  : pt_att_analog_4g_up_2;
            }
            else
            {
                pt = &att_analog_4g_dw;
            }
            break;

        case RF_SYS_2:
            if(RF_CH_UP == ch)
            {
                pt = (1 == id) ? &att_analog_nr_up_1 : &att_analog_nr_up_2;
            }
            else
            {
                pt = &att_analog_nr_dw;
            }
            break;

        case RF_SYS_3:
            if(RF_CH_UP == ch)
            {
                pt = (1 == id) ? &att_analog_mimo_up_1 : &att_analog_mimo_up_2;
            }
            else
            {
                pt = &att_analog_mimo_dw;
            }
            break;
            
        default:
            pt = NULL;
            break;
    }

    if(NULL != pt)
        pwm_value = pt->pwm_get(pt);
    return pwm_value;
}

#else   // RU
void att_analog_pwm_set(uint8_t sys, uint8_t ch, uint8_t id, uint16_t pwm_value)
{
    if((sys >= RF_SYS_NUM) || (ch >= RF_CH_NUM) || (id > 2) || (id == 0))
        return ;

    if((ch == RF_CH_DW) && (id > 2))
        return ;

    ATT_analog_t * pt = NULL;;

    switch(sys)
    {
        case RF_SYS_1:
            if(RF_CH_UP == ch)
            {
                pt = &att_analog_4g_up;
            }
            else
            {
                pt = (1 == id) ? pt_att_analog_4g_dw_1  : pt_att_analog_4g_dw_2;
            }
            break;

        case RF_SYS_2:
            if(RF_CH_UP == ch)
            {
                pt = &att_analog_nr_up;
            }
            else
            {
                pt = (1 == id) ? &att_analog_nr_dw_1 : &att_analog_nr_dw_2;
            }
            break;

        case RF_SYS_3:
            if(RF_CH_UP == ch)
            {
                pt = &att_analog_mimo_up;
            }
            else
            {
                pt = (1 == id) ? &att_analog_mimo_dw_1 : &att_analog_mimo_dw_2;
            }
            break;

        default:
            pt = NULL;
            break;
    }

    if(NULL != pt)
        pt->pwm_set(pt, pwm_value);
}
uint16_t att_analog_pwm_get(uint8_t sys, uint8_t ch, uint8_t id)
{
    uint16_t pwm_value = 0;

    if((sys >= RF_SYS_NUM) || (ch >= RF_CH_NUM) || (id > 2) || (id == 0))
        return 0;

    if((ch == RF_CH_DW) && (id > 2))
        return 0;

    ATT_analog_t * pt = NULL;;

    switch(sys)
    {
        case RF_SYS_1:
            if(RF_CH_UP == ch)
            {
                pt = &att_analog_4g_up;
            }
            else
            {
                pt = (1 == id) ? pt_att_analog_4g_dw_1  : pt_att_analog_4g_dw_2;
            }
            break;

        case RF_SYS_2:
            if(RF_CH_UP == ch)
            {
                pt = &att_analog_nr_up;
            }
            else
            {
                pt = (1 == id) ? &att_analog_nr_dw_1 : &att_analog_nr_dw_2;
            }
            break;

        case RF_SYS_3:
            if(RF_CH_UP == ch)
            {
                pt = &att_analog_mimo_up;
            }
            else
            {
                pt = (1 == id) ? &att_analog_mimo_dw_1 : &att_analog_mimo_dw_2;
            }
            break;

        default:
            pt = NULL;
            break;
    }

    if(NULL != pt)
        pwm_value = pt->pwm_get(pt);

    return pwm_value;
}


#endif


void test_analog_att( uint8_t sys, uint8_t ch )
{
	ATT_analog_t  * pt = NULL;
	static uint8_t att_value = 0;
	
	sys = RF_SYS_1;
	ch  = RF_CH_DW;
	uint8_t id = 0;
	
    switch(sys)
    {
        case RF_SYS_1:
            if(RF_CH_UP == ch)
            {
                pt = (1 == id) ? pt_att_analog_4g_up_1  : pt_att_analog_4g_up_2;
            }
            else
            {
                pt = &att_analog_4g_dw;
            }
            break;

        case RF_SYS_2:
            if(RF_CH_UP == ch)
            {
                pt = (1 == id) ? &att_analog_nr_up_1 : &att_analog_nr_up_2;
            }
            else
            {
                pt = &att_analog_nr_dw;
            }
            break;

        case RF_SYS_3:
            if(RF_CH_UP == ch)
            {
                pt = (1 == id) ? &att_analog_mimo_up_1 : &att_analog_mimo_up_2;
            }
            else
            {
                pt = &att_analog_mimo_dw;
            }
            break;
            
        default:
            pt = NULL;
            break;
    }
	

	for( att_value = 0; att_value<=15; att_value++ )
	{
		pt->set(pt, att_value);
		vTaskDelay( 1000 );
	}
}


void test_analog_att_pwm( uint8_t sys, uint8_t ch )
{
	ATT_analog_t  * pt = NULL;
    uint16_t pwm_val = 0, pwm_get = 0;
	
	sys = RF_SYS_1;
	ch  = RF_CH_DW;
	uint8_t id = 0;
	
    switch(sys)
    {
        case RF_SYS_1:
            if(RF_CH_UP == ch)
            {
                pt = (1 == id) ? pt_att_analog_4g_up_1  : pt_att_analog_4g_up_2;
            }
            else
            {
                pt = &att_analog_4g_dw;
            }
            break;

        case RF_SYS_2:
            if(RF_CH_UP == ch)
            {
                pt = (1 == id) ? &att_analog_nr_up_1 : &att_analog_nr_up_2;
            }
            else
            {
                pt = &att_analog_nr_dw;
            }
            break;

        case RF_SYS_3:
            if(RF_CH_UP == ch)
            {
                pt = (1 == id) ? &att_analog_mimo_up_1 : &att_analog_mimo_up_2;
            }
            else
            {
                pt = &att_analog_mimo_dw;
            }
            break;
            
        default:
            pt = NULL;
            break;
    }
	

	for( pwm_val = 0; pwm_val<=999; pwm_val++ )
	{
		pt->pwm_set(pt, pwm_val);
		vTaskDelay( 200 );
		pwm_get = pt->pwm_get(pt);
        
        rtt_printf(RTT_RF, RTT_CTRL_TEXT_BRIGHT_GREEN "pwm set/get = %d %d\r\n",pwm_val,pwm_get);
	}
}
