/***************************** Copyright ****************************************
*
* (C) Copyright 2023, Shaanxi Tianji Communication
*  All Rights Reserved
*
* FileName   : bsp_rf_5g_modem.c
* Version    : none
* Author     :
* Date       : 2023-10-19
* Description: none
*******************************************************************************/
#include "main.h"
//#include "stdlib.h"
//#include "stdio.h"
//#include "string.h"

#include "system_config.h"
#include "bsp_rf_5g_modem.h"
#include "rtt.h"
#include "bsp_config.h"
#include "bsp_rf_4g_band_sel.h"
#include "bsp_rf_global.h"
#include "eswin_sync_modem.h"
#include "bsp_rf_866x_default_set.h"
#include "bsp_testmode_hal_set.h"
//#include "bsp_rf_global.h"
#include "bsp_rf_scan_net.h"


/*---------------------- 端口定义 --------------------------------------*/


/*---------------------- 宏定义  ---------------------------------------*/


/*---------------------- 结构体 ----------------------------------------*/


/*---------------------- 常量 ------------------------------------------*/


/*---------------------- 全局变量 --------------------------------------*/
NetConfig_T t_5g_modem_config;	//5G 模组配置参数

/*---------------------- 局部变量 --------------------------------------*/


/*---------------------- 声明 ------------------------------------------*/
extern sync_modem_t hModem; // MODEM参数信息
nr_modem_edge_t t_edge = { .lte_ul_rising  = 0,
						   .lte_ul_falling = -100,
						   .lte_dl_rising  = 100,
						   .lte_dl_falling = 0,
						   .nr_ul_rising   = 0,
						   .nr_ul_falling  = -100,
						   .nr_dl_rising   = 100,
						   .nr_dl_falling  = 0,
						 };

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ HAL ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/





/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DRV ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/





/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ APP ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*****************************************************************************
* @Name	  : rf_set_5g_modem_state
* @Brief  : 设置5G MODEM 状态
* @Param  : sys: 系统选择
**			state: 0: 关, 1: 开, 2: 上行单通, 3: 下行单通
* @Retval :
* @Author :
* @Data   : 2024-01-24
* @Note   :
*****************************************************************************/
/**
* @brief   设置提前滞后量   
* @param   None
* @return    
* @data    2024-03-27
* @note    
*/
void rf_5g_modem_set_edge( void )
{
	pc_config_modem_buf.lte_ul_rising  = t_edge.lte_ul_rising;
    pc_config_modem_buf.lte_ul_falling = t_edge.lte_ul_falling;
    pc_config_modem_buf.lte_dl_rising  = t_edge.lte_dl_rising;
    pc_config_modem_buf.lte_dl_falling = t_edge.lte_dl_falling;
    pc_config_modem_buf.nr_ul_rising   = t_edge.nr_ul_rising;
    pc_config_modem_buf.nr_ul_falling  = t_edge.nr_ul_falling;
    pc_config_modem_buf.nr_dl_rising   = t_edge.nr_dl_rising;
    pc_config_modem_buf.nr_dl_falling  = t_edge.nr_dl_falling;
	pc_config_modem_buf.flg_set_timing_offset = 0;
}

/**
* @brief   频段扫描初始化   
* @param   None
* @return    
* @data    2024-03-26
* @note    
*/
void rf_5g_modem_scan_init(void)
{
	pc_config_modem_buf.startup = 0;
	pc_config_modem_buf.lte_ul_rising  = t_edge.lte_ul_rising;
    pc_config_modem_buf.lte_ul_falling = t_edge.lte_ul_falling;
    pc_config_modem_buf.lte_dl_rising  = t_edge.lte_dl_rising;
    pc_config_modem_buf.lte_dl_falling = t_edge.lte_dl_falling;
    pc_config_modem_buf.nr_ul_rising   = t_edge.nr_ul_rising;
    pc_config_modem_buf.nr_ul_falling  = t_edge.nr_ul_falling;
    pc_config_modem_buf.nr_dl_rising   = t_edge.nr_dl_rising;
    pc_config_modem_buf.nr_dl_falling  = t_edge.nr_dl_falling;
}



/**
* @brief   模组重新扫描
* @param   None
* @return
* @data    2024-03-18
* @note    hModem.modem_reboot_or_reset=2;//模块重新上电标志
*		   hModem.modem_reboot_or_reset=1;//模块重启标志
*		   重新上电后，配置命令需要延时5S再发，如果是重启标志则不需要延时。
*		   当应用程序检测到hModem.modem_reboot_or_reset非0，可以做响应的模块配置流程；
*/
BOOL_E rf_5g_modem_rescan(void)
{
    if(hModem.modem_reboot_or_reset)	//模块重新上电或者重启, 需要相关的配置
    {
        hModem.modem_reboot_or_reset = 0;
        return B_TRUE;
    }
    else
        return B_FALSE;
}


/**
* @brief   模组准备好
* @param   None
* @return  B_TRUE: 准备好, 可以发送指令, B_FALSE: 未准备好, 等待
* @data    2024-03-18
* @note
*/
BOOL_E rf_5g_modem_ready(void)
{
//	if( MODEM_IDLE ==modem_get_state() )
    if(CONFIG_IDLE == hModem.modem_config_state || CONFIG_FINE == hModem.modem_config_state)  
        return B_TRUE;
    else
        return B_FALSE;
}


/**
* @brief   获取扫描结果状态
* @param   None
* @return  B_TRUE: 扫描完成,  B_FALSE: 扫描进行中
* @data    2024-03-18
* @note
*/
BOOL_E rf_5g_modem_sacn_done(void)
{
    if(CONFIG_FINE == hModem.modem_config_state)
        return B_TRUE;
    else
        return B_FALSE;
}


/**
* @brief   启动扫描
* @param   None
* @return
* @data    2024-03-19
* @note
*/
void rf_5g_modem_scan_start(void)
{
    pc_config_modem_buf.startup = 1;
}


/**
* @brief   停止扫描
* @param   None
* @return
* @data    2024-03-19
* @note
*/
void rf_5g_modem_scan_stop(void)
{
    pc_config_modem_buf.startup = 0;
}

/**
* @brief   配置扫描参数
* @param   rat:  网络制式, NR:0, LTE:1, LTE_NR:2
*		   band: 频段, BAND_3, BAND_8.....
* @return
* @data    2024-03-19
* @note
*/
#if 0
void rf_5g_modem_scan_config(NET_MODE rat, uint8_t band)
{
    pc_config_modem_buf.startup = 0;

    char  buffer_freq[64];
    char * p_freq_init = buffer_freq;
    memset(p_freq_init, 0, sizeof(buffer_freq[0])*FREQ_BUF_SIZE);
    memset(pc_config_modem_buf.band, 0, sizeof(pc_config_modem_buf.band));

    pc_config_modem_buf.modem_network = rat;
    pc_config_modem_buf.device_mode   = DEVICE_RUN_MODE;

    if((LTE == rat) || (LTE_NR == rat))
    {
        if(BAND_3 == band)
        {
            pc_config_modem_buf.band[0] = 3;

            memset(pc_config_modem_buf.freq_3, 0, sizeof(pc_config_modem_buf.freq_3[0])*FREQ_BUF_SIZE);
            memcpy(pc_config_modem_buf.freq_3, &t_rf_4g_save_para[BAND_3].scan_freq, strlen((const char *)t_rf_4g_save_para[BAND_3].scan_freq));
        }
        else if(BAND_8 == band)
        {
            pc_config_modem_buf.band[1] = 8;
            memset(pc_config_modem_buf.freq_8, 0, sizeof(pc_config_modem_buf.freq_8[0])*FREQ_BUF_SIZE);
            memcpy(pc_config_modem_buf.freq_8, &t_rf_4g_save_para[BAND_8].scan_freq, strlen((const char *)t_rf_4g_save_para[BAND_8].scan_freq));
        }
        else if(BAND_39 == band)
        {
            pc_config_modem_buf.band[2] = 39;
            memset(pc_config_modem_buf.freq_39, 0, sizeof(pc_config_modem_buf.freq_39[0])*FREQ_BUF_SIZE);
            memcpy(pc_config_modem_buf.freq_39, &t_rf_4g_save_para[BAND_39].scan_freq, strlen((const char *)t_rf_4g_save_para[BAND_39].scan_freq));
        }
        else if(BAND_40 == band)
        {
            pc_config_modem_buf.band[3] = 40;
            memset(pc_config_modem_buf.freq_40, 0, sizeof(pc_config_modem_buf.freq_40[0])*FREQ_BUF_SIZE);
            memcpy(pc_config_modem_buf.freq_40, &t_rf_4g_save_para[BAND_40].scan_freq, strlen((const char *)t_rf_4g_save_para[BAND_40].scan_freq));
        }
    }

    if((NR == rat) || (LTE_NR == rat))
    {
        pc_config_modem_buf.band[4] = 41;
        memset(pc_config_modem_buf.freq_41, 0, sizeof(pc_config_modem_buf.freq_41[0])*FREQ_BUF_SIZE);
        memcpy(pc_config_modem_buf.freq_41, &t_rf_sys[ RF_SYS_2 ].t_public.scan_freq, strlen((const char *)t_rf_sys[ RF_SYS_2 ].t_public.scan_freq));
    }
    else
    {}
}
#endif

void rf_5g_modem_scan_config(NET_MODE rat, uint8_t band_lte, uint8_t band_nr )
{
    pc_config_modem_buf.startup = 0;

    char  buffer_freq[64];
    char * p_freq_init = buffer_freq;
    memset(p_freq_init, 0, sizeof(buffer_freq[0])*FREQ_BUF_SIZE);
    memset(pc_config_modem_buf.band, 0, sizeof(pc_config_modem_buf.band));
    memset(pc_config_modem_buf.band_net_mode_set, 0, sizeof(pc_config_modem_buf.band_net_mode_set));

    //pc_config_modem_buf.modem_network = rat;
    pc_config_modem_buf.device_mode   = DEVICE_RUN_MODE;

    if((LTE == rat) || (LTE_NR == rat))
    {
        if(BAND_3 == band_lte)
        {
            pc_config_modem_buf.band[0] = 3;
            pc_config_modem_buf.band_net_mode_set[0] = LTE;

            memset(pc_config_modem_buf.freq_3, 0, sizeof(pc_config_modem_buf.freq_3[0])*FREQ_BUF_SIZE);
            memcpy(pc_config_modem_buf.freq_3, &t_rf_4g_save_para[BAND_3].scan_freq, strlen((const char *)t_rf_4g_save_para[BAND_3].scan_freq));
        }
        else if(BAND_8 == band_lte)
        {
            pc_config_modem_buf.band[1] = 8;
            pc_config_modem_buf.band_net_mode_set[1] = LTE;
            memset(pc_config_modem_buf.freq_8, 0, sizeof(pc_config_modem_buf.freq_8[0])*FREQ_BUF_SIZE);
            memcpy(pc_config_modem_buf.freq_8, &t_rf_4g_save_para[BAND_8].scan_freq, strlen((const char *)t_rf_4g_save_para[BAND_8].scan_freq));
        }
        else if(BAND_39 == band_lte)
        {
            pc_config_modem_buf.band[2] = 39;
            pc_config_modem_buf.band_net_mode_set[2] = LTE;
            memset(pc_config_modem_buf.freq_39, 0, sizeof(pc_config_modem_buf.freq_39[0])*FREQ_BUF_SIZE);
            memcpy(pc_config_modem_buf.freq_39, &t_rf_4g_save_para[BAND_39].scan_freq, strlen((const char *)t_rf_4g_save_para[BAND_39].scan_freq));
        }
        else if(BAND_40 == band_lte)
        {
            pc_config_modem_buf.band[3] = 40;
            pc_config_modem_buf.band_net_mode_set[3] = LTE;
            memset(pc_config_modem_buf.freq_40, 0, sizeof(pc_config_modem_buf.freq_40[0])*FREQ_BUF_SIZE);
            memcpy(pc_config_modem_buf.freq_40, &t_rf_4g_save_para[BAND_40].scan_freq, strlen((const char *)t_rf_4g_save_para[BAND_40].scan_freq));
        }
    }

    if((NR == rat) || (LTE_NR == rat))
    {
        if(BAND_3 == band_nr)
        {
            pc_config_modem_buf.band[0] = 3;
            pc_config_modem_buf.band_net_mode_set[0] = NR;

            memset(pc_config_modem_buf.freq_3, 0, sizeof(pc_config_modem_buf.freq_3[0])*FREQ_BUF_SIZE);
            memcpy(pc_config_modem_buf.freq_3, &t_rf_4g_save_para[BAND_3].scan_freq, strlen((const char *)t_rf_4g_save_para[BAND_3].scan_freq));
        }
        else if(BAND_39 == band_nr)
        {
            pc_config_modem_buf.band[2] = 39;
            pc_config_modem_buf.band_net_mode_set[2] = NR;
            memset(pc_config_modem_buf.freq_39, 0, sizeof(pc_config_modem_buf.freq_39[0])*FREQ_BUF_SIZE);
            memcpy(pc_config_modem_buf.freq_39, &t_rf_4g_save_para[BAND_39].scan_freq, strlen((const char *)t_rf_4g_save_para[BAND_39].scan_freq));
        }
        else if(BAND_40 == band_nr)
        {
            pc_config_modem_buf.band[3] = 40;
            pc_config_modem_buf.band_net_mode_set[3] = NR;
            memset(pc_config_modem_buf.freq_40, 0, sizeof(pc_config_modem_buf.freq_40[0])*FREQ_BUF_SIZE);
            memcpy(pc_config_modem_buf.freq_40, &t_rf_4g_save_para[BAND_40].scan_freq, strlen((const char *)t_rf_4g_save_para[BAND_40].scan_freq));
        }
        else if(BAND_41 == band_nr)
        {
            pc_config_modem_buf.band[4] = 41;
            pc_config_modem_buf.band_net_mode_set[4] = NR;
            memset(pc_config_modem_buf.freq_41, 0, sizeof(pc_config_modem_buf.freq_41[0])*FREQ_BUF_SIZE);
            memcpy(pc_config_modem_buf.freq_41, &t_rf_sys[ RF_SYS_2 ].t_public.scan_freq, strlen((const char *)t_rf_sys[ RF_SYS_2 ].t_public.scan_freq));
        }
    }
    else
    {}
}

/**
* @brief   获取4G网络同步状态
* @param   None
* @return  SYNC / UNSYNC
* @data    2024-03-21
* @note
*/
SYNC_STATE_E rf_5g_modem_get_lte_sync(void)
{
    uint32_t cur_time = hi_get_tick();
    if((BAND_3 == t_rf_sys[1].t_public.valid_band) || (BAND_8 == t_rf_sys[1].t_public.valid_band))
        return SYNC;
    else
    {
		if( LTE == t_rf_sys[RF_SYS_1].t_public.net_mode )
		{
			if(SYNC_SYNC == hModem.LTE_net_info.LTE_Sync) //同步信号正常
				return SYNC;
			else if(SCAN_UNSYNC == hModem.LTE_net_info.LTE_Sync && cur_time - hModem.LTE_net_info.sync_time < 30000) //失步30s内同步时序还能保持正确输出, 认为是同步
				return SYNC;
			else
				return UNSYNC;
		}
		else
		{
			if(SYNC_SYNC == hModem.NR_net_info.NR_Sync) //同步信号正常
				return SYNC;
			else if(SCAN_UNSYNC == hModem.NR_net_info.NR_Sync && cur_time - hModem.NR_net_info.sync_time < 30000) //失步30s内同步时序还能保持正确输出, 认为是同步
				return SYNC;
			else
				return UNSYNC;
		}
    }
}

/**
* @brief   获取5G网络同步状态
* @param   None
* @return  SYNC / UNSYNC
* @data    2024-03-21
* @note
*/
SYNC_STATE_E rf_5g_modem_get_nr_sync(void)
{
    uint32_t cur_time = hi_get_tick();

    if(SYNC_SYNC == hModem.NR_net_info.NR_Sync) //同步信号正常
        return SYNC;
    else if(SCAN_UNSYNC == hModem.NR_net_info.NR_Sync && (cur_time - hModem.NR_net_info.sync_time) < 30000) //失步30s内同步时序还能保持正确输出, 认为是同步
        return SYNC;
    else
        return UNSYNC;
}


/**
* @brief   设置提前滞后量
* @param   rat: [输入/出] 
* @return    
* @data    2024-03-24
* @note    
*/
void rf_5g_modem_edge_cfg( NET_MODE rat )
{
	pc_config_modem_buf.lte_ul_rising  = t_edge.lte_ul_rising;
    pc_config_modem_buf.lte_ul_falling = t_edge.lte_ul_falling;
    pc_config_modem_buf.lte_dl_rising  = t_edge.lte_dl_rising;
    pc_config_modem_buf.lte_dl_falling = t_edge.lte_dl_falling;
    pc_config_modem_buf.nr_ul_rising   = t_edge.nr_ul_rising;
    pc_config_modem_buf.nr_ul_falling  = t_edge.nr_ul_falling;
    pc_config_modem_buf.nr_dl_rising   = t_edge.nr_dl_rising;
    pc_config_modem_buf.nr_dl_falling  = t_edge.nr_dl_falling;
	
	if( LTE == rat )
		modem_set_gpio_timing_offset(   hModem.hw_config.lte_gpio_group, 
										pc_config_modem_buf.lte_ul_rising, 
										pc_config_modem_buf.lte_ul_falling,
										pc_config_modem_buf.lte_dl_rising,
										pc_config_modem_buf.lte_dl_falling );
	else
		modem_set_gpio_timing_offset(   hModem.hw_config.nr_gpio_group, 
										pc_config_modem_buf.lte_ul_rising, 
										pc_config_modem_buf.lte_ul_falling,
										pc_config_modem_buf.lte_dl_rising,
										pc_config_modem_buf.lte_dl_falling );
}

/**
* @brief   设置5G 模组的时隙配比   
* @param   rat: NR / LTE / LTE_NR
* @return    
* @data    2024-03-24
* @note    
*/
Status_t rf_5g_modem_frame_cfg( NET_MODE rat, uint8_t group )
{
	uint8_t frame     = t_5g_modem_config.frame;		    //帧格式
	uint8_t frame_cfg = t_5g_modem_config.specialTime;	//特殊帧配比
    uint8_t gpio_group = 0;
	
	timing_config_t t_cfg;	//时序配置
	uint8_t cycle = 0;	//0:单周期, 1:双周期
//	uint8_t frame_dat[30] = {0};
//	uint8_t *pt = frame_dat;
	
	uint8_t retry = 0;
	Status_t res  = STAT_ERROR;
	
	if((frame > 6) || (frame == 0) || (frame_cfg > 3) || (frame_cfg == 0) )
		return STAT_ERROR;

//    if( NR == rat )
//        gpio_group = hModem.hw_config.nr_gpio_group;
	
	gpio_group = group;
	
	switch( frame )
	{
		case 1:	//DDDDDDDDSUU 单 00050202
			
			t_cfg.num_of_ul_subframe_1  = 0x02; 
			t_cfg.num_of_dl_subframe_1  = 0x07; 
			t_cfg.num_of_ul_subframe_2  = 0x00; 
			t_cfg.num_of_dl_subframe_2  = 0x00; 
			cycle = 0;
			break;
		
		case 2:	//DS 单 00010000
			t_cfg.num_of_ul_subframe_1  = 0x00; 
			t_cfg.num_of_dl_subframe_1  = 0x01; 
			t_cfg.num_of_ul_subframe_2  = 0x00; 
			t_cfg.num_of_dl_subframe_2  = 0x00; 
			cycle = 0;
			break;
		
		case 3:	//DDDSU 单 01030000
			t_cfg.num_of_ul_subframe_1  = 0x01; 
			t_cfg.num_of_dl_subframe_1  = 0x03; 
			t_cfg.num_of_ul_subframe_2  = 0x00; 
			t_cfg.num_of_dl_subframe_2  = 0x00; 
			cycle = 0;
			break;
		
		case 4: //DDSUU	单 02020000
			t_cfg.num_of_ul_subframe_1  = 0x02; 
			t_cfg.num_of_dl_subframe_1  = 0x02; 
			t_cfg.num_of_ul_subframe_2  = 0x00; 
			t_cfg.num_of_dl_subframe_2  = 0x00; 
			cycle = 0;
			break;
		
		case 5:	//DSUUU 单 03010000
			t_cfg.num_of_ul_subframe_1  = 0x03; 
			t_cfg.num_of_dl_subframe_1  = 0x01; 
			t_cfg.num_of_ul_subframe_2  = 0x00; 
			t_cfg.num_of_dl_subframe_2  = 0x00; 
			cycle = 0;
			break;
		
		case 6:	//DDDSUDDSUU 双 01030202
			t_cfg.num_of_ul_subframe_1  = 0x01; 
			t_cfg.num_of_dl_subframe_1  = 0x03; 
			t_cfg.num_of_ul_subframe_2  = 0x02; 
			t_cfg.num_of_dl_subframe_2  = 0x02; 
			cycle = 1;
			break;
		
		default:
			break;
	}
	
	switch( frame_cfg )
	{
		case 1: // 6:4:4 040406
			t_cfg.num_of_ul_symbles_sp1 = 0x04; 
			t_cfg.num_of_gp_symbles_sp1 = 0x04; 
			t_cfg.num_of_dl_symbles_sp1 = 0x06;
			t_cfg.num_of_ul_symbles_sp2 = 0x04; 
			t_cfg.num_of_gp_symbles_sp2 = 0x04; 
			t_cfg.num_of_dl_symbles_sp2 = 0x06;
			break;
		
		case 2: // 2:2:10 0A0202 
			t_cfg.num_of_ul_symbles_sp1 = 0x0A; 
			t_cfg.num_of_gp_symbles_sp1 = 0x02; 
			t_cfg.num_of_dl_symbles_sp1 = 0x02;
			t_cfg.num_of_ul_symbles_sp2 = 0x00; 
			t_cfg.num_of_gp_symbles_sp2 = 0x00; 
			t_cfg.num_of_dl_symbles_sp2 = 0x00;
			break;
		
		case 3:	// 10:2:2 02020A
			t_cfg.num_of_ul_symbles_sp1 = 0x02; 
			t_cfg.num_of_gp_symbles_sp1 = 0x02; 
			t_cfg.num_of_dl_symbles_sp1 = 0x0A;
			t_cfg.num_of_ul_symbles_sp2 = 0x02; 
			t_cfg.num_of_gp_symbles_sp2 = 0x02; 
			t_cfg.num_of_dl_symbles_sp2 = 0x0A;
			break;
		
		default:
			break;
	}
	
	retry = 3;
	while( retry-- )
	{
		Status_t res = modem_set_gpio_timing_config( gpio_group, t_cfg, cycle );
		if( STAT_OK == res )
			break;
	}
	
	return res;
}


/**
* @brief   设置5G模组开关输出状态   
* @param   band: BAND_3 / BAND_8 ......
**			 net_mode: IS_FDD / IS_TDD
**			 state: RF_SWITCH_CLOSE / RF_SWITCH_OPEN / RF_SWITCH_UP / RF_SWITCH_DW
* @return    
* @data    2024-03-23
* @note    
*/
/*
void rf_5g_modem_switch(RF_BAND_E band, RF_SWITCH_E state)
{
	if((BAND_3 == band) || ( BAND_8 == band ))
	{
		rf_set_modem_cmd( RF_SYS_1, state, IS_FDD );
	}
	else if( (BAND_39 == band) || (BAND_40 == band) )
	{
		rf_set_modem_cmd( RF_SYS_1, state, IS_TDD );
	}
	else
	{
		rf_5g_modem_edge_cfg( NR );
		rf_5g_modem_frame_cfg( NR );
		rf_set_modem_cmd( RF_SYS_2, state, IS_TDD );
	}
}
*/
void rf_5g_modem_switch(RF_BAND_E band, RF_SWITCH_E state)
{
	if((BAND_3 == band) || ( BAND_8 == band ))
	{
		rf_set_modem_cmd( RF_SYS_1, state, IS_FDD );
	}
	else if( BAND_39 == band )
	{
		if( NR == band_net_mode[BAND_39] )
		{
			rf_5g_modem_edge_cfg( LTE );
			rf_5g_modem_frame_cfg( NR, hModem.hw_config.lte_gpio_group );
		}
		else
		{
			rf_5g_modem_edge_cfg( LTE );
			rf_5g_modem_frame_cfg( LTE, hModem.hw_config.lte_gpio_group );
		}
		rf_set_modem_cmd( RF_SYS_1, state, IS_TDD );
	}
	else if(BAND_40 == band)
	{
		if( NR == band_net_mode[BAND_40] )
		{
			rf_5g_modem_edge_cfg( LTE );
			rf_5g_modem_frame_cfg( NR, hModem.hw_config.lte_gpio_group );
		}
		else
		{
			rf_5g_modem_edge_cfg( LTE );
			rf_5g_modem_frame_cfg( LTE, hModem.hw_config.lte_gpio_group );
		}
		rf_set_modem_cmd( RF_SYS_1, state, IS_TDD );
	}
	else
	{
		rf_5g_modem_edge_cfg( NR );
		rf_5g_modem_frame_cfg( NR, hModem.hw_config.nr_gpio_group );
		rf_set_modem_cmd( RF_SYS_2, state, IS_TDD );
	}
}

