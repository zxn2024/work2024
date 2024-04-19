/*************************************** Copyright ******************************
*
* (C) Copyright 2023, Shaanxi Tianji Communication
* All Rights Reserved
*                         
* FileName   : bsp_rf_5g_modem.h   
* Version    : none		
* Author     : 			
* Date       : 2023-10-19         
* Description: none   
*******************************************************************************/
//
#ifndef __BSP_RF_5G_MODEM_H_
#define __BSP_RF_5G_MODEM_H_

#include <stdint.h>
#include "public_struct.h"
#include "eswin_sync_modem.h"

typedef struct
{
    int  lte_ul_rising;
    int  lte_ul_falling;
    int  lte_dl_rising;
    int  lte_dl_falling;
    int  nr_ul_rising;
    int  nr_ul_falling;
    int  nr_dl_rising;
    int  nr_dl_falling;
} nr_modem_edge_t;

extern NetConfig_T t_5g_modem_config;	//5G 模组配置参数
extern nr_modem_edge_t t_edge;

//void rf_set_5g_modem_state(uint8_t sys, uint8_t state);
void rf_5g_modem_set_edge( void );				//设置提前滞后量
void rf_5g_modem_scan_init(void);				//频段扫描初始化
BOOL_E rf_5g_modem_rescan( void );				//检测模组状态
BOOL_E rf_5g_modem_ready( void );				//模组准备好状态检测
BOOL_E rf_5g_modem_sacn_done( void );			//模组扫描结束

void rf_5g_modem_scan_config( NET_MODE rat, uint8_t band_lte, uint8_t band_nr );	//设置扫描参数
void rf_5g_modem_scan_start( void );						//启动扫描
void rf_5g_modem_scan_stop( void );							//停止扫描
SYNC_STATE_E rf_5g_modem_get_nr_sync( void );				//获取5G模组同步状态
SYNC_STATE_E rf_5g_modem_get_lte_sync( void );				//获取4G模组同步状态

void rf_5g_modem_switch(RF_BAND_E band, RF_SWITCH_E state);	//设置模组射频开关状态

void rf_5g_modem_edge_cfg( NET_MODE rat );

#endif





