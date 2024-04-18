/***************************** Copyright ****************************************
*
* (C) Copyright 2023, Shaanxi Tianji Communication
*  All Rights Reserved
*
* FileName   : bsp_up_silence.c
* Version    : none
* Author     :
* Date       : 2023-11-13
* Description:
*		1. 调用silence_init()完成初始化
*       2. 载入设置的门限参数
*       3. 在射频处理流程中调用rf_up_silence()函数
*       4. 在8668外部中断中调用 silence_out_detection_sys(x)_callback();	其中, x=1,2,3, 用于退出上行静默
*******************************************************************************/
#include "main.h"
//#include "stdlib.h"
//#include "stdio.h"
//#include "string.h"

#include "system_config.h"
#include "bsp_up_silence.h"
#include "bsp_rf_time.h"
#include "bsp_rf_global.h"
#include "ecr8668_driver.h"
#include "bsp_config.h"
#include "bsp_rf_att.h"
#include "bsp_rf_pa.h"
#include "bsp_rf_866x_default_set.h"
#include "ru_device_list.h"
#include "bsp_public_drv.h"

/*---------------------- 端口定义 --------------------------------------*/


/*---------------------- 宏定义  ---------------------------------------*/


/*---------------------- 结构体 ----------------------------------------*/
#define RSSI_MAX_NUM    ( 5 )



/*---------------------- 常量 ------------------------------------------*/
//enum
//{
//    RUN_STATE_NORMAL = 0,
//    RUN_STATE_SILENCE
//};


/*---------------------- 全局变量 --------------------------------------*/


/*---------------------- 局部变量 --------------------------------------*/
SILENCE_PARA_T t_silence_para[ RF_SYS_NUM ];

/*---------------------- 声明 ------------------------------------------*/




/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ APP ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/**
* @brief   进入静默设置  
* @param   sys: [输入/出] 
* @return    
* @data    2024-04-17
* @note    
*/
void rf_silence_in_set( uint8_t sys )
{
	SILENCE_PARA_T *pt_s = &t_silence_para[ sys ];
	ATT_CH_T       *pt_att     = pt_ch_att[sys][RF_CH_UP];
	
	pt_s->lna1 		  = pt_att->lna1.cur_value;
	pt_s->lna2 		  = pt_att->lna2.cur_value;
	pt_s->digit_att1  = pt_att->digit_att1.cur_value;
	pt_s->digit_att2  = pt_att->digit_att2.cur_value;
	pt_s->analog_att1 = pt_att->analog_att1.cur_value;
	pt_s->analog_att2 = pt_att->analog_att2.cur_value;
	
	pt_att->lna1.cur_value = LNA_BYS;
	pt_att->lna2.cur_value = LNA_BYS;
	pt_att->digit_att1.cur_value = pt_att->digit_att1.max_num;
	pt_att->digit_att2.cur_value = pt_att->digit_att2.max_num;
	pt_att->analog_att1.cur_value = pt_att->analog_att1.max_num;
	pt_att->analog_att2.cur_value = pt_att->analog_att2.max_num;
	
	pt_att->digit_att1.refresh  = B_TRUE;
	pt_att->digit_att2.refresh  = B_TRUE;
	pt_att->analog_att1.refresh = B_TRUE;
	pt_att->analog_att2.refresh = B_TRUE;
	pt_att->lna1.refresh 		= B_TRUE;
	pt_att->lna2.refresh 		= B_TRUE;
	
	att_set( sys, RF_CH_UP );
	
	//关闭PA
	if( RF_SYS_1 )
	{
		rf_4g_pa_state_set( RF_SWITCH_DW );
	}
	
	rf_set_modem_cmd( sys, RF_SWITCH_DW, t_rf_sys[sys].t_public.SysMode );
}


/**
* @brief   退出静默设置
* @param   sys: [输入/出] 
* @return    
* @data    2024-04-17
* @note    
*/
void rf_silence_quit_set( uint8_t sys )
{
	SILENCE_PARA_T *pt_s = &t_silence_para[ sys ];
	ATT_CH_T       *pt_att     = pt_ch_att[sys][RF_CH_UP];
	
	pt_att->lna1.cur_value 		  = pt_s->lna1;
	pt_att->lna2.cur_value 		  = pt_s->lna2;
	pt_att->digit_att1.cur_value  = pt_s->digit_att1;
	pt_att->digit_att2.cur_value  = pt_s->digit_att2;
	pt_att->analog_att1.cur_value = pt_s->analog_att1;
	pt_att->analog_att2.cur_value = pt_s->analog_att2;
	
	pt_att->digit_att1.refresh  = B_TRUE;
	pt_att->digit_att2.refresh  = B_TRUE;
	pt_att->analog_att1.refresh = B_TRUE;
	pt_att->analog_att2.refresh = B_TRUE;
	pt_att->lna1.refresh 		= B_TRUE;
	pt_att->lna2.refresh 		= B_TRUE;
	
	att_set( sys, RF_CH_UP );
	
	//设置PA开
	if( RF_SYS_1 )
	{
		rf_4g_pa_state_set( RF_SWITCH_OPEN );
	}
	
	rf_set_modem_cmd( sys, RF_SWITCH_OPEN, t_rf_sys[sys].t_public.SysMode );
}

/*****************************************************************************
* @Name	  : silence_init
* @Brief  :
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2023-11-13
* @Note   :
*****************************************************************************/
void silence_init(void)
{
    for(uint8_t sys = 0; sys < RF_SYS_NUM; sys++)
    {
        memset(t_silence_para, 0, sizeof(SILENCE_PARA_T));
    }
    t_silence_para[ RF_SYS_1 ].gate = -100;
    t_silence_para[ RF_SYS_2 ].gate = -100;
    t_silence_para[ RF_SYS_3 ].gate = -100;
	
	rf_nr_up_silent_ctrl.ctl(&rf_nr_up_silent_ctrl, PORT_OFF); 
	rf_mimo_up_silent_ctrl.ctl(&rf_mimo_up_silent_ctrl, PORT_OFF);
}



/*****************************************************************************
* @Name	  : silence_in_set
* @Brief  : 进入静默设置
* @Param  : sys: [输入/出]
* @Retval :
* @Author :
* @Data   : 2023-11-13
* @Note   :
*****************************************************************************/

void silence_in_set(uint8_t sys)
{
    if(sys >= RF_SYS_NUM)
        return;

    #if LOCAL_MODULE_TYPE == _MU
		//rf_silence_in_set_att( sys );
    #else
    switch(sys)
    {
        case RF_SYS_1:
            BSP_866x_enter_silent_mode(&h8668_LTE);	//设置8668饱和门限, 接收增益 BSP_866X_enter_silence_mode
            rf_4g_up_silent_ctrl.ctl(&rf_4g_up_silent_ctrl, PORT_OFF);  //关闭从机上行发射PA - 4G需要发送MIPI指令, 直接拉管脚
            break;

        case RF_SYS_2:
            BSP_866x_enter_silent_mode(&h8668_NR);
            rf_5g_up_silent_ctrl.ctl(&rf_5g_up_silent_ctrl, PORT_OFF);     //关闭从机上行发射PA - 5G直接拉低
            break;

        case RF_SYS_3:
			BSP_866x_enter_silent_mode(&h8668_NR_MIMO);
            rf_5g_up_silent_ctrl.ctl(&rf_5g_up_silent_ctrl, PORT_OFF);  //关闭从机上行发射PA - 5G直接拉低
            break;

        default:
            break;
    }
    #endif
}



/*****************************************************************************
* @Name	  : silence_quit_set
* @Brief  : 退出静默设置
* @Param  : sys: [输入/出]
* @Retval :
* @Author :
* @Data   : 2023-11-13
* @Note   :
*****************************************************************************/
static void silence_quit_set(uint8_t sys)
{
    if(sys >= RF_SYS_NUM)
        return;

    TRX8668_FPGA_t * handle_att = NULL;

    #if LOCAL_MODULE_TYPE == _MU
    switch(sys)
    {
        case RF_SYS_1:
            handle_att = &h8668_LTE;
            break;

        case RF_SYS_2:
            handle_att = &h8668_NR;
            rf_nr_up_silent_ctrl.ctl(&rf_nr_up_silent_ctrl, PORT_ON);     //关闭从机上行发射PA - 5G直接拉GPIO 上升沿有效
            break;

        case RF_SYS_3:
            handle_att = &h8668_NR_MIMO;
            rf_mimo_up_silent_ctrl.ctl(&rf_mimo_up_silent_ctrl, PORT_ON);     //关闭从机上行发射PA - 5G直接拉GPIO 上升沿有效
            break;

        default:
            break;
    }
    #else
    switch(sys)
    {
        case RF_SYS_1:
            handle_att = &h8668_LTE;
            rf_4g_up_silent_ctrl.ctl(&rf_4g_up_silent_ctrl, PORT_ON);  //关闭从机上行发射PA - 4G需要发送MIPI指令, 直接拉管脚
            break;

        case RF_SYS_2:
            handle_att = &h8668_NR;
            rf_5g_up_silent_ctrl.ctl(&rf_5g_up_silent_ctrl, PORT_ON);     //关闭从机上行发射PA - 5G直接拉GPIO 上升沿有效
            break;

        case RF_SYS_3:
            handle_att = &h8668_NR_MIMO;
            rf_5g_up_silent_ctrl.ctl(&rf_5g_up_silent_ctrl, PORT_ON);     //关闭从机上行发射PA - 5G直接拉GPIO 上升沿有效
            break;

        default:
            break;
    }
    #endif

    if(NULL != handle_att)
        isr_866x_quit_silent_mode(handle_att);
}


/*****************************************************************************
* @Name	  : silence_detection
* @Brief  : 检测是否进入静默模式
* @Param  : sys:
**			 ch:
* @Retval :
* @Author :
* @Data   : 2023-11-13
* @Note   :
*****************************************************************************/
#if LOCAL_MODULE_TYPE == _MU
static void silence_detection(uint8_t sys )
{
    if( sys >= RF_SYS_NUM )
        return;

    SILENCE_PARA_T * pt = &t_silence_para[ sys ];
//    NetPara_T * pt_net = &t_rf_sys[sys].t_public.tNet;
	
	uint8_t slave_dev_num = 0;
	uint8_t all_in_silence = 0;
	
	if(ENABLE != t_rf_sys[sys].t_public.silenceEn)
    {
        pt->state = RUN_STATE_NORMAL;
        return;
    }
	
	for ( uint8_t id = 0; id < BLE_CONNECT_MAX_NUM; id++)
	{
		if(ru_device_list[id].dev_type == DEV_TYPE_RU )
		{
			if( (ru_device_list[id].pt_mrc_ru_data->mrc_silent == 1) && (ru_device_list[id].ru_conn_state == RU_CONN_STATE_CONFIGURED) )
			{
				slave_dev_num++;
				all_in_silence++;
			}
			else
				slave_dev_num++;
		}
	}
	
    if((all_in_silence != 0) && (slave_dev_num == all_in_silence))	//进入静默
	{
		if( pt->state != RUN_STATE_SILENCE )
		{
			pt->state = RUN_STATE_SILENCE;
			rf_silence_in_set( sys );
			rtt_printf( RTT_RF, RTT_TEXT_RED" === %s in silence! ===\n", rf_sys_name[sys] );
		}
	}
	else	//退出静默
	{
		if( pt->state != RUN_STATE_NORMAL )
		{
			pt->state = RUN_STATE_NORMAL;
			rf_silence_quit_set( sys );
			rtt_printf( RTT_RF, RTT_TEXT_RED" === %s quit silence! ===\n", rf_sys_name[sys] );
		}
	}
	
	
}
#else
static void silence_detection(uint8_t sys, uint8_t ch)
{
    if((sys >= RF_SYS_NUM) || (ch != RF_CH_UP))
        return;

    SILENCE_PARA_T * pt = &t_silence_para[ RF_SYS_1 ];
    NetPara_T * pt_net = &t_rf_sys[sys].t_public.tNet;

    if(ENABLE != t_rf_sys[sys].t_public.silenceEn)
    {
        pt->state = RUN_STATE_NORMAL;
        return;
    }

    if(RUN_STATE_SILENCE == pt->state)     //当前已经处于静默状态, 则不再检测
        return;

    int16_t rssi_value = pt_net->rssi;  //获取RSSI数据

    if(rssi_value > pt->gate)
    {
        pt->cnt = 0;
        pt->state = RUN_STATE_NORMAL;
        return;
    }

    pt->cnt++;
    if(pt->cnt >= RSSI_MAX_NUM)         //进入上行静默
    {
        pt->cnt   = 0;
        pt->state = RUN_STATE_SILENCE;
        silence_in_set(sys);
    }
}
#endif

/*****************************************************************************
* @Name	  : silence_out_detection_sys1_callback
* @Brief  : SYS1退出上行静默回调函数
* @Param  : sys: [输入/出]
* @Retval :
* @Author :
* @Data   : 2023-11-13
* @Note   : 在中断中调用
*****************************************************************************/
void silence_out_detection_sys1_callback(void)
{
    t_silence_para[ RF_SYS_1 ].state = RUN_STATE_NORMAL;
    silence_quit_set(RF_SYS_1);
}


/*****************************************************************************
* @Name	  : silence_out_detection_sys1_callback
* @Brief  : SYS2退出上行静默回调函数
* @Param  : sys: [输入/出]
* @Retval :
* @Author :
* @Data   : 2023-11-13
* @Note   : 在中断中调用
*****************************************************************************/
void silence_out_detection_sys2_callback(void)
{
    t_silence_para[ RF_SYS_2 ].state = RUN_STATE_NORMAL;
    silence_quit_set(RF_SYS_2);
}


/*****************************************************************************
* @Name	  : silence_out_detection_sys1_callback
* @Brief  : SYS3退出上行静默回调函数
* @Param  : sys: [输入/出]
* @Retval :
* @Author :
* @Data   : 2023-11-13
* @Note   : 在中断中调用
*****************************************************************************/
void silence_out_detection_sys3_callback(void)
{
    t_silence_para[ RF_SYS_3 ].state = RUN_STATE_NORMAL;
    silence_quit_set(RF_SYS_3);
}


/*****************************************************************************
* @Name	  : get_silence_state
* @Brief  : 获取上行静默状态
* @Param  : sys: [输入/出]
* @Retval :
* @Author :
* @Data   : 2023-11-13
* @Note   :
*****************************************************************************/
RUN_STATE_E get_silence_state(uint8_t sys)
{
    if(sys >= RF_SYS_NUM)
        return RUN_STATE_NORMAL;

    return t_silence_para[ sys ].state;

}

/*****************************************************************************
* @Name	  : rf_up_silence
* @Brief  : 上行静默
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2023-11-15
* @Note   :
*****************************************************************************/
void rf_silence(uint8_t sys)
{
    silence_detection(sys);
}



