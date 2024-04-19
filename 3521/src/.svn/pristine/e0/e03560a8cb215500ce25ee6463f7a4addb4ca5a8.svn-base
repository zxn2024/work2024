/*************************************** Copyright ******************************
*
* (C) Copyright 2024, Shaanxi Tianji Communication
* All Rights Reserved
*                         
* FileName   : bsp_rf_scan_net.h   
* Version    : none		
* Author     : 			
* Date       : 2024-01-15         
* Description: none   
*******************************************************************************/
//#include <stdint.h>
#ifndef __BSP_RF_SCAN_NET_H_
#define __BSP_RF_SCAN_NET_H_
#include "eswin_sync_modem.h"

extern uint8_t  bandwidth_net[ BAND_END ];
extern uint8_t  frame_net[ BAND_END];		//有效值0 ~ 2
extern uint8_t  sp_frame_net[ BAND_END ];    //有效值0 ~ 8

void rf_5g_modem_in_task( void );	//在任务中调用

NET_MODE get_band_net_mode( RF_BAND_E band );
Status_t set_band_net_mode( RF_BAND_E band, NET_MODE mode );
uint8_t * get_band_freq_list_addr( RF_BAND_E band );
extern NET_MODE band_net_mode[ BAND_END ] ;

void set_band_scan_restart( void );	//重新扫描

Status_t measure_coupling_gain_in_calib_mode( RF_BAND_E band );
SYNC_STATE_E read_sync_in_calib_mode( RF_SYS_E sys );
int16_t read_rsrp_in_calib_mode( NET_MODE mode );

#endif





