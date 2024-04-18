/***************************** Copyright ****************************************
*
* (C) Copyright 2024, Shaanxi Tianji Communication
*  All Rights Reserved
*                         
* FileName   : bsp_rf_4g_scan_net.c   
* Version    : none		
* Author     : 			
* Date       : 2024-01-15         
* Description: none  
*******************************************************************************/
#include "main.h"
//#include "stdlib.h"
//#include "stdio.h"
//#include "string.h"
//#include "MCU_config.h"
#include "public_define.h"
#include "public_enum.h"
#include "public_struct.h"

#include "bsp_rf_scan_net.h"
#include "eswin_sync_modem.h"
#include "bsp_testmode_hal_set.h"
#include "eswin_sync_modem.h"
#include "bsp_rf_global.h"
#include "bsp_rf_4g_band_sel.h"
#include "bsp_rf_dev_state.h"
#include "bsp_rf_fpga.h"
#include "bsp_rf_4g_band_sel.h"
#include "bsp_rf_5g_modem.h"
#include "bsp_rf_4g_band_sel.h"
#include "bsp_rf_lna.h"
#include "bsp_rf_power_on.h"
#include "bsp_rf_ctr.h"
#include "bsp_rf_4g_band_sel.h"
#include "bsp_public_drv.h"
#include "bsp_rf_dc_switch.h"

/*---------------------- 端口定义 --------------------------------------*/


/*---------------------- 宏定义  ---------------------------------------*/
typedef enum
{
	SCAN_STATE_INIT = 0,	//初始化
	SCAN_STATE_IDLE,		//空闲
	SCAN_STATE_4G_SCAN,		//4G扫描
	SCAN_STATE_5G_SCAN,		//5G扫描
	SCAN_STATE_RESULT,		//设置结果
	SCAN_STATE_QUERY,		//查询参数
	SCAN_STATE_EXCEPT		//异常处理
}SCAN_NET_STATE_E;

/*---------------------- 结构体 ----------------------------------------*/
typedef struct
{
	uint8_t  band;
	uint32_t freq;
	uint32_t bw;
	uint32_t pci;
	int32_t  rsrp;
	int32_t  sinr;
} scanf_result_t;

/*---------------------- 常量 ------------------------------------------*/


/*---------------------- 全局变量 --------------------------------------*/
NET_MODE band_net_mode[ BAND_END ] = { LTE, LTE, LTE, LTE, NR, NR };
uint8_t  bandwidth_net[ BAND_END ] = { 25,  15,  30, 50, 100, 100 };
uint8_t  frame_net[ BAND_END] = {2, 2, 2, 2, 1, 1};		//有效值0 ~ 2
uint8_t  sp_frame_net[ BAND_END ] = {7, 7, 7, 7, 1, 1}; //有效值0 ~ 8

static uint8_t           current_band_id = 0;	//用于保存当前使用的4G band

static scanf_result_t    t_4g_scan_sort[4];   //保存lte结果, 用于输出时序
static scanf_result_t    t_lte_scan_result;
static scanf_result_t    t_nr_scan_result;		//保存nr结果, 用于输出时序

sync_modem_t *pt_sync_modem = &hModem;			//指向驱动中定义的sync_modem_t结构体

LTE_net_info_t t_lte_real_time_para;			//定时查询时获取的LTE实时参数
NR_net_info_t  t_nr_real_time_para;				//定时查询时获取的NR实时参数

/*---------------------- 局部变量 --------------------------------------*/
SCAN_NET_STATE_E e_scan_state = SCAN_STATE_IDLE;	//状态机

static NET_MODE scan_net_mode_result;	//最终工作模式, NR / LTE / LTE_NR

static uint8_t rescan_flag = B_FALSE;	//重新扫描标志, B_TRUE: 重新扫描
/*---------------------- 声明 ------------------------------------------*/
void set_dc_switch_by_scan_result( void );


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ HAL ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/





/**
* @brief   获取各频段网络模式
* @param   band: BAND_3 / BAND_8 / .... 
* @return  LTE / NR 
* @data    2024-03-22
* @note    
*/
NET_MODE get_band_net_mode( RF_BAND_E band )
{
	if( band >= BAND_END )
		return NR;
	
	return( band_net_mode[ band ] );
}

/**
* @brief   设置各频段网络模式   
* @param   band: BAND_3 / BAND_8 / .... 
**		   mode: LTE / NR
* @return  STAT_OK / STAT_ERROR
* @data    2024-03-22
* @note    
*/
Status_t set_band_net_mode( RF_BAND_E band, NET_MODE mode )
{
	if((band >= BAND_END) || (mode >= LTE_NR) )
		return STAT_ERROR;
	
	band_net_mode[ band ] = mode;
	return STAT_OK;
}


/**
* @brief   获取各频段的扫描频点地址   
* @param   band: BAND_3 / BAND_8 / .... 
* @return  指针首地址  
* @data    2024-03-22
* @note    
*/
uint8_t * get_band_freq_list_addr( RF_BAND_E band )
{
	switch( band )
	{
		case BAND_3:
		case BAND_8:
		case BAND_39:
		case BAND_40:
			return t_rf_4g_save_para[band].scan_freq;
		
		case BAND_41:
			return t_rf_sys[RF_SYS_2].t_public.scan_freq;
			
		case BAND_41M:
			return t_rf_sys[RF_SYS_2].t_public.scan_freq;
		
		default:
			return 0;
	}
}


/**
* @brief   设置重新扫描标志   
* @param   None
* @return    
* @data    2024-03-22
* @note    
*/
void set_band_scan_restart( void )
{
	rescan_flag = B_TRUE;
	e_scan_state = SCAN_STATE_4G_SCAN;
	t_rf_sys[ RF_SYS_1 ].t_public.scan_net_state  = B_FALSE;
	t_rf_sys[ RF_SYS_1 ].t_public.bin_load_result = B_FALSE;
	t_rf_sys[ RF_SYS_2 ].t_public.scan_net_state  = B_FALSE;
	t_rf_sys[ RF_SYS_2 ].t_public.bin_load_result = B_FALSE;
	t_rf_sys[ RF_SYS_3 ].t_public.scan_net_state  = B_FALSE;
	t_rf_sys[ RF_SYS_3 ].t_public.bin_load_result = B_FALSE;
		rtt_printf( RTT_RF, RTT_TEXT_RED"===>>> rescan .... <<<===\n" );
}


void set_band_scan_next( void )
{
	e_scan_state = SCAN_STATE_RESULT;
	t_rf_sys[ RF_SYS_1 ].t_public.scan_net_state  = B_FALSE;
	t_rf_sys[ RF_SYS_1 ].t_public.bin_load_result = B_FALSE;
	t_rf_sys[ RF_SYS_2 ].t_public.scan_net_state  = B_FALSE;
	t_rf_sys[ RF_SYS_2 ].t_public.bin_load_result = B_FALSE;
	t_rf_sys[ RF_SYS_3 ].t_public.scan_net_state  = B_FALSE;
	t_rf_sys[ RF_SYS_3 ].t_public.bin_load_result = B_FALSE;
	rtt_printf( RTT_RF, RTT_TEXT_RED"===>>> rescan next band .... <<<===\n" );
}


/**
* @brief   重新扫描状态机复位   
* @param   None
* @return    
* @data    2024-03-31
* @note    
*/
void rescan_state_set( void )
{
	if( B_TRUE == rescan_flag )
		e_scan_state = SCAN_STATE_4G_SCAN;
	
	rescan_flag = B_FALSE;
}

/*****************************************************************************
* @Name	  : band_num_to_serial
* @Brief  : band 转化为 序号, 如:40 -> BAND_40
* @Param  : num: [输入/出] 
* @Retval : 
* @Author : 
* @Data   : 2024-01-18
* @Note   :
*****************************************************************************/
static RF_BAND_E band_num_to_serial( uint8_t num )
{
	if( 3 == num )
		return BAND_3;
	else if( 8 == num )
		return BAND_8;
	else if( 39 == num )
		return BAND_39;
	else if( 40 == num )
		return BAND_40;
	else if( 41 == num )
		return BAND_41;
	else
		return BAND_END;
}


static uint8_t band_serial_to_num( RF_BAND_E band )
{
	if( BAND_3 == band )
		return 3;
	else if( BAND_8 == band )
		return 8;
	else if( BAND_39 == band )
		return 39;
	else if( BAND_40 == band )
		return 40;
	else if( BAND_41 == band )
		return 41;
	else if( BAND_41M == band )
		return 41;
	else
		return 0xFF;
}


/*****************************************************************************
* @Name	  : get_scan_data
* @Brief  : 获取扫描结果到待排序数组中
* @Param  : band: 频段, BAND_3 / BAND_8 / ....
**			 pt_src: 扫描结果
**			 pt_tag: 待排序的结构体数组 
**		   valid_band: 有效数量
* @Retval : 
* @Author : 
* @Data   : 2024-01-18
* @Note   :
*****************************************************************************/
uint8_t get_scan_data( RF_BAND_E band, net_scan_result_t *pt_src, scanf_result_t *pt_tag, uint8_t valid_num )
{
	uint8_t i = 0;
	uint8_t n = 0;
	uint8_t band_num;
	
	if( band >= BAND_END )
			return 0;
	
	if( valid_num >= 50 )
			valid_num = 20;
	
	if( (BAND_3 == band)|| (BAND_8 == band)|| (BAND_39 == band) || (BAND_40 == band) )
	{
		band_num = band_serial_to_num( band );
		n = 0;
		if( LTE == band_net_mode[ band ] )
		{
			for( i=0; i<valid_num; i++ )
			{
				if( band_num == pt_src->LTE_scan_tab[i].band )
				{
					pt_tag[n].band = pt_src->LTE_scan_tab[i].band;
					pt_tag[n].freq = pt_src->LTE_scan_tab[i].freq;
					pt_tag[n].bw   = pt_src->LTE_scan_tab[i].bw;
					pt_tag[n].pci  = pt_src->LTE_scan_tab[i].pci;
					pt_tag[n].rsrp = pt_src->LTE_scan_tab[i].rsrp;
					pt_tag[n].sinr = pt_src->LTE_scan_tab[i].sinr;
					n++;
				}
			}
		}
		else
		{
			for( i=0; i<valid_num; i++ )
			{
				if( band_num == pt_src->NR_scan_tab[i].band )
				{
					pt_tag[n].band = pt_src->NR_scan_tab[i].band;
					pt_tag[n].freq = pt_src->NR_scan_tab[i].freq;
					pt_tag[n].bw   = pt_src->NR_scan_tab[i].bw;
					pt_tag[n].pci  = pt_src->NR_scan_tab[i].pci;
					pt_tag[n].rsrp = pt_src->NR_scan_tab[i].rsrp;
					pt_tag[n].sinr = pt_src->NR_scan_tab[i].sinr;
					n++;
				}
			}
		}
	}
	else
	{
		n = 0;
		for( i=0; i<valid_num; i++ )
		{
			pt_tag[n].band = pt_src->NR_scan_tab[i].band;
			pt_tag[n].freq = pt_src->NR_scan_tab[i].freq;
			pt_tag[n].bw   = pt_src->NR_scan_tab[i].bw;
			pt_tag[n].pci  = pt_src->NR_scan_tab[i].pci;
			pt_tag[n].rsrp = pt_src->NR_scan_tab[i].rsrp;
			pt_tag[n].sinr = pt_src->NR_scan_tab[i].sinr;
			n++;
		}
	}
	
	return n;
}


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DRV ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*****************************************************************************
* @Name	  : scan_result_sort
* @Brief  : 按rsrp和sinr 对扫描结果排序    
* @Param  : pt_src: [输入/出] 
**			 num: [输入/出] 
**			 alc_point: [输入/出] 
* @Retval : 
* @Author : 
* @Data   : 2024-01-17
* @Note   :
*****************************************************************************/
void scan_result_sort( scanf_result_t *pt_src, uint8_t num, int8_t alc_point )
{
	
	scanf_result_t t_temp;
	uint8_t  i = 0;	//, j = 0;
	
	rtt_printf( RTT_RF, RTT_TEXT_CYAN"------>>>>>> start sort, num = %d <<<<<<------ \n", num );
	for( i=0; i<num; i++ )
	{
		rtt_printf( RTT_RF, RTT_TEXT_CYAN"----->>>>> %02d: %-2d - %-6d - %-3d, rsrp = %-3d, sinr = %-3d, pci = %-3d\n", i, pt_src[i].band, pt_src[i].freq, pt_src[i].bw, pt_src[i].rsrp, pt_src[i].sinr, pt_src[i].pci);
	}
	rtt_printf( RTT_RF, "\n" );
	
//	uint8_t rsrp_valid_num = 0;	//rsrp 有效的数量
	
	if( num > 1 )	//数量大于1才需要排序
	{
		//根据RSRP排序
		for( i=0; i<num; i++ )
		{
			for( uint8_t j=0; j<num-i-1; j++)
			{
				if( pt_src[j].rsrp < pt_src[j+1].rsrp )
				{
					memcpy( &t_temp, &pt_src[j], 	sizeof( scanf_result_t ) );
					memcpy( &pt_src[j], &pt_src[j+1], sizeof( scanf_result_t ) );
					memcpy( &pt_src[j+1], &t_temp, 	sizeof( scanf_result_t ) );
				}
			}
		}
		
		rtt_printf( RTT_RF, RTT_TEXT_CYAN"------>>>>>> sort by rsrp, num = %d, alc = %d <<<<<<------ \n", num, alc_point );
		for( i=0; i<num; i++ )
		{
			rtt_printf( RTT_RF, RTT_TEXT_YELLOW"----->>>>> %02d: %-2d - %-6d - %-3d, rsrp = %-3d, sinr = %-3d, pci = %-3d\n", i, pt_src[i].band, pt_src[i].freq, pt_src[i].bw, pt_src[i].rsrp, pt_src[i].sinr, pt_src[i].pci );
		}
		
//		//过滤
//		for( i=0; i<num-1; i++ )
//		{
//			if( pt_src[i].rsrp > alc_point )
//				rsrp_valid_num = i+1;
//			else
//				break;
//		}
//		
//		//根据SINR排序
//		if( rsrp_valid_num > 1 )	//需要排序, 找寻sinr最大的
//		{
//			for ( i=0; i<rsrp_valid_num-1; i++ )
//			{
//				for( j=0; j<rsrp_valid_num-i-1; j++)
//				{
//					if( pt_src[j].sinr < pt_src[j+1].sinr )
//					{
//						memcpy( &t_temp, &pt_src[j], 	  sizeof( scanf_result_t ) );
//						memcpy( &pt_src[j], &pt_src[j+1], sizeof( scanf_result_t ) );
//						memcpy( &pt_src[j+1], &t_temp, 	  sizeof( scanf_result_t ) );
//					}
//				}
//			}
//		}
	}
//	
//	rtt_printf( RTT_RF, RTT_TEXT_CYAN"------>>>>>> sort by sinr, rsrp_valid_num = %d <<<<<<------ \n", rsrp_valid_num, alc_point );
//	for( i=0; i<num; i++ )
//	{
//		rtt_printf( RTT_RF, RTT_TEXT_MAGENTA"----->>>>> %02d: %-2d - %-6d - %-3d, rsrp = %-3d, sinr = %-3d\n", i, pt_src[i].band, pt_src[i].freq, pt_src[i].bw, pt_src[i].rsrp, pt_src[i].sinr );
//	}
}


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ APP ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

uint32_t scanf_fpoint_to_freq( NET_MODE rat, RF_BAND_E band, uint32_t point )
{
	uint32_t freq = 0;
	
	uint32_t start_freq = 0;	//频率精确到小数点后1位, 设置数量10倍关系
	uint32_t start_point = 0;
	
	if( BAND_3 == band )
	{
		if( NR == rat )
		{
			freq =(point * 1000 / 200 + 7650)/100;
		}
		else
		{
			start_freq  = 1805;
			start_point = 1200;
			freq = start_freq * 10 + point - start_point;
		}
	}
	else if( BAND_8 == band )
	{
		start_freq  = 925;
		start_point = 3450;
		freq = start_freq * 10 + point - start_point;
	}
	
	else if( BAND_39 == band )
	{
		if( NR == rat )
		{
			freq =(point * 1000 / 200 + 4950)/100;
		}
		else
		{
			start_freq  = 1880;
			start_point = 38250;
			freq = start_freq * 10 + point - start_point;
		}
	}
	else if( BAND_40 == band )
	{
		if( NR == rat )
		{
			freq =(point * 1000 / 200 + 13350)/100;
			
		}
		else
		{
			start_freq  = 2300;
			start_point = 38650;
			freq = start_freq * 10 + point - start_point;
		}
	}
	else if( BAND_41 == band )
	{
		freq = t_rf_sys[RF_SYS_2].t_public.valid_freq;
	}
	else
	{
		freq = t_rf_sys[RF_SYS_3].t_public.valid_freq;
	}

	return freq;
}

/*****************************************************************************
* @Name	  : scan_net_process
* @Brief  : 
* @Param  : network_format: 0: NR, 1:LTE, 2: NR&LTE
**			band: 在双模模式下, band只代表4G
**          band_valid: band有效性性标志, 用于区分3频设备和5频设备
* @Retval : 
* @Author : 
* @Data   : 2024-01-17
* @Note   :
*****************************************************************************/
Status_t scan_net_process( NET_MODE rat, RF_BAND_E band_lte, RF_BAND_E band_nr, uint8_t band_valid  )
{
	Status_t res = STAT_OK;
	
	uint8_t sys = 0;
	
	uint8_t freq_point_4g[64] = { 0 };
	uint8_t freq_point_nr[64] = { 0 };
	
	net_scan_result_t t_scan_result;		//保存原始扫描结果
	
	scanf_result_t    t_scan_data[20];		//缓存扫描数据, 用于排序筛选
	uint8_t           scan_valid_num_lte;		//原始数据中的有效数据数量
	uint8_t           scan_valid_num_nr;		//原始数据中的有效数据数量
	int8_t            alc_point = 0;		//alc启控点
	
	if( B_FALSE == band_valid )	//无效band, 赋值固定值
	{
		t_4g_scan_sort[band_lte].freq = 0;
		t_4g_scan_sort[band_lte].bw   = 0;
		t_4g_scan_sort[band_lte].pci  = 0;
		t_4g_scan_sort[band_lte].rsrp = -10000 - band_lte;
		t_4g_scan_sort[band_lte].sinr = -10000 - band_lte;
        rtt_printf( RTT_MODEM, RTT_TEXT_RED"Modem Scan band_valid Error\r\n");
		return STAT_ERROR;
	}
	
	if( (LTE_NR == rat) || (LTE == rat) )
	{
		memset( &t_lte_scan_result, 0, sizeof( scanf_result_t ));
		switch( band_lte )
		{
			case BAND_3:
				alc_point = -84;
				memcpy( freq_point_4g, t_rf_4g_save_para[0].scan_freq, 64 );
				sys 	  = RF_SYS_1;
				break;
			
			case BAND_8:
				alc_point = -76;
				memcpy( freq_point_4g, t_rf_4g_save_para[1].scan_freq, 64 );
				sys 	  = RF_SYS_1;
				break;
			
			case BAND_39:
				alc_point = -81;
				memcpy( freq_point_4g, t_rf_4g_save_para[2].scan_freq, 64 );
				sys 	  = RF_SYS_1;
				break;
			
			case BAND_40:
				alc_point = -84;
				memcpy( freq_point_4g, t_rf_4g_save_para[3].scan_freq, 64 );
				sys 	  = RF_SYS_1;
				break;
			
			default:
                rtt_printf( RTT_MODEM, RTT_TEXT_RED"Modem Scan band_lte Error\r\n");
				return STAT_ERROR;
		}
		
		tm_sys_open( band_lte );	//通道上电, 4g重定向指针	    
		
		lna_set( sys, RF_CH_DW, 1, LNA_RF ); //下行两级低噪放常开
		lna_set( sys, RF_CH_DW, 2, LNA_RF );
	}
	
	if( (LTE_NR == rat) || (NR == rat) )	//LTE_NR 模式
	{
		memset( &t_nr_scan_result, 0, sizeof( scanf_result_t ));
		switch( band_nr )
		{
			case BAND_3:
				alc_point = -84;
				memcpy( freq_point_4g, t_rf_4g_save_para[0].scan_freq, 64 );
				sys 	  = RF_SYS_1;
				break;
			
			case BAND_39:
				alc_point = -81;
				memcpy( freq_point_4g, t_rf_4g_save_para[2].scan_freq, 64 );
				sys 	  = RF_SYS_1;
				break;
			
			case BAND_40:
				alc_point = -84;
				memcpy( freq_point_4g, t_rf_4g_save_para[3].scan_freq, 64 );
				sys 	  = RF_SYS_1;
				break;
			
			case BAND_41:
				alc_point = -83;
				memcpy( freq_point_nr, t_rf_sys[1].t_public.scan_freq, 64 );
				sys 	  = RF_SYS_2;
				break;
			
			case BAND_41M:
				alc_point = -83;
				memcpy( freq_point_nr, t_rf_sys[2].t_public.scan_freq, 64 );
				sys 	  = RF_SYS_3;
				break;
		
			default:
                rtt_printf( RTT_MODEM, RTT_TEXT_RED"Modem Scan band_nr Error\r\n");
				return STAT_ERROR;
		}
	
		
		tm_sys_open( band_nr );	//通道上电, 4g重定向指针
		lna_set( sys, RF_CH_DW, 1, LNA_RF ); //下行两级低噪放常开
		lna_set( sys, RF_CH_DW, 2, LNA_RF );
	}
	
	rf_5g_modem_scan_config( rat, band_lte, band_nr );
	
	uint8_t retry = 3;
	while( retry-- )
	{
		rf_5g_modem_scan_start();			//开始搜索频点, 需要等待一段时间才能输出数据
		vTaskDelay( 500 );
		
		for( uint16_t n=0; n<=500; n++ )
		{
			if( B_FALSE == rf_5g_modem_sacn_done() )
				vTaskDelay( 150 );
			else
			{
				res = modem_get_scan_result( &t_scan_result );
                if( res != STAT_OK )
                    rtt_printf( RTT_MODEM, RTT_TEXT_RED"Modem Scan result %d\r\n", res);
				break;
			}
		}
		
		if( LTE == rat )
		{
			scan_valid_num_lte = pt_sync_modem->scan_result.LTE_scan_pci_num;		//LTE有效数据数量
			if( pt_sync_modem->scan_result.LTE_scan_pci_num )
				break;
			else 
            {
                rtt_printf( RTT_MODEM, RTT_TEXT_RED"Modem Scan Error lte num err\r\n");
				res = STAT_ERROR;
            }
		}
		else if( NR == rat )
		{
			scan_valid_num_nr = pt_sync_modem->scan_result.NR_scan_pci_num;		//NR有效数据数量
			if( pt_sync_modem->scan_result.NR_scan_pci_num )
				break;
			else 
            {
                rtt_printf( RTT_MODEM, RTT_TEXT_RED"Modem Scan Error nr num err\r\n");
				res = STAT_ERROR;
            }
		}
		else if( LTE_NR == rat )
		{
			scan_valid_num_lte = pt_sync_modem->scan_result.LTE_scan_pci_num;		//LTE_NR有效数据数量
			scan_valid_num_nr  = pt_sync_modem->scan_result.NR_scan_pci_num;
			if( (pt_sync_modem->scan_result.LTE_scan_pci_num > 0) && (pt_sync_modem->scan_result.NR_scan_pci_num>0 ) )
				break;
			else
            {
                rtt_printf( RTT_MODEM, RTT_TEXT_RED"Modem Scan Error LTE_NR num err\r\n");
				res = STAT_ERROR;
            }
		}
		else
		{
			scan_valid_num_lte = 0;
			scan_valid_num_nr = 0;
		}
	}
	
	//获取数据
	if( res == STAT_OK )	//单频段保存指定频段结果
	{
		if((LTE == rat) || (LTE_NR == rat ) )
		{
			memset( t_scan_data, 0, sizeof( t_scan_data ) );
			uint8_t lte_num = get_scan_data( band_lte, &pt_sync_modem->scan_result, t_scan_data, scan_valid_num_lte );
			if( lte_num >= 1 )	
			{
				scan_result_sort( t_scan_data, lte_num, alc_point );
			}
			if( t_scan_data[0].band != 0 )
				memcpy( &t_lte_scan_result, &t_scan_data[0], sizeof( scanf_result_t ) ); 
			else
			{
				t_lte_scan_result.band = 0;
				t_lte_scan_result.rsrp = -11000 - band_lte;
				t_lte_scan_result.sinr = -11000 - band_lte; 
				rtt_printf( RTT_RF, RTT_TEXT_RED"lte scan error, no valid data!\n" );
			}
		}
		
		if((NR == rat) || (LTE_NR == rat ))
		{
			memset( t_scan_data, 0, sizeof(t_scan_data) );
			scan_valid_num_nr = pt_sync_modem->scan_result.NR_scan_pci_num;		//有效数据数量
			uint8_t nr_num = get_scan_data( band_nr, &pt_sync_modem->scan_result, t_scan_data, scan_valid_num_nr );
			if( nr_num >= 1 )	
			{
				scan_result_sort( t_scan_data, nr_num, alc_point );
			}
			if( t_scan_data[0].band != 0 )
				memcpy( &t_nr_scan_result, &t_scan_data[0], sizeof( scanf_result_t ) );	//保存NR 扫描结果
			else
			{
				
				t_nr_scan_result.rsrp = -10005;
				t_nr_scan_result.sinr = -10005; 
			}
		}	
	}
	else
	{
		if((LTE == rat) || (LTE_NR == rat))
		{			
			t_lte_scan_result.rsrp = -10000 - band_lte;
			t_lte_scan_result.sinr = -10000 - band_lte;
		}
		
		if((NR == rat) || (LTE_NR == rat))
		{
			t_nr_scan_result.rsrp = -10005;
			t_nr_scan_result.sinr = -10005;
		}	
	}
	
    if(res != STAT_OK)
    {
        rtt_printf( RTT_MODEM, RTT_TEXT_RED"Modem Scan Error\r\n");
    }
	return res;
}


/*****************************************************************************
* @Name	  : real_time_net_info_analysis
* @Brief  : 实时数据分析
* @Param  : None
* @Retval : 0: 正常 1:切换频段 2:重新扫描
* @Author : 
* @Data   : 2024-01-19
* @Note   : 在任务中调用
*****************************************************************************/
uint8_t real_time_net_info_analysis( void )
{
	#define LOSE_NET_MAX_TIME	( 60 * 1000 )
	#define RE_SYNC_TIME	( 8 * 1000 )

	uint8_t band_end_id = ( DEV_BAND_NUM == 5 ) ? 4 : 2;
	
	static uint32_t start_time = 0, nr_resync_cnt = 0, lte_resync_cnt = 0;
	static uint32_t end_time = 0;
    uint32_t current_time;
	
	int32_t rsrp_lte = hModem.LTE_net_info.rsrp;
	int32_t rsrp_nr = hModem.NR_net_info.rsrp;
    SYNC_STAT_E sync_lte = hModem.LTE_net_info.LTE_Sync;
    SYNC_STAT_E sync_nr = hModem.NR_net_info.NR_Sync;
    uint32_t lte_last_sync_time = hModem.LTE_net_info.sync_time;
    uint32_t nr_last_sync_time = hModem.NR_net_info.sync_time; 
    static uint32_t lte_last_track_time = 0, nr_last_track_time = 0;

    static int lte_index,nr_index;

	RF_BAND_E band = band_num_to_serial( t_4g_scan_sort[ current_band_id ].band );
	
	uint8_t  res = 0;
    
//    UNUSED_PARA(band_end_id);
    UNUSED_PARA(start_time);
    UNUSED_PARA(end_time);
    UNUSED_PARA(rsrp_lte);
    UNUSED_PARA(rsrp_nr);

    if(scan_net_mode_result == MODE_NULL)
        return 2;
    
    // LTE同步异常判断
	if(((LTE == scan_net_mode_result) &&( LTE == band_net_mode[ band ] )) || (LTE_NR == scan_net_mode_result))	//在LTE模式 或者 LTE_NR模式下有效
	{
        if( sync_lte == SYNC_SYNC )
        {
            lte_resync_cnt = 0;
        }
        else if( sync_lte == SCAN_UNSYNC )
        {
            current_time = hi_get_tick();
            // 失步超过丢网时间,重新搜网
            if( lte_resync_cnt >= LOSE_NET_MAX_TIME/RE_SYNC_TIME )
            {
				lte_resync_cnt = 0;
				if((current_band_id + 1 <= band_end_id) && ( t_4g_scan_sort[ current_band_id + 1 ].band != 0 ) )	//LTE 切换到下一个BAND
				{
					current_band_id++;
					res = 1;	//切换下一个网络
					rtt_printf( RTT_MODEM, RTT_TEXT_RED"Modem LTE Lost Net!!! Re-Scan next band\r\n");
				}
				else
				{
					res = 2;//重新扫描
//					lte_resync_cnt = 0;
					rtt_printf( RTT_MODEM, RTT_TEXT_RED"Modem LTE Lost Net!!! Re-Scan\r\n");
				}
            // 失步超过重发时间,重发驻网
            }
			else if( (current_time - lte_last_sync_time) >= RE_SYNC_TIME * (lte_resync_cnt+1) && ((current_time - lte_last_track_time) >= RE_SYNC_TIME ))
            {
                lte_last_track_time = current_time;
                lte_resync_cnt++;
                lte_index++;
				
                if(lte_index>=hModem.scan_result.LTE_scan_pci_num)
                    lte_index=0;
                modem_tracking_lte_cell( hModem.scan_result.LTE_scan_tab[lte_index].freq, hModem.scan_result.LTE_scan_tab[lte_index].pci ,hModem.scan_result.LTE_scan_tab[lte_index].band);
            }
        }else if( sync_lte == SYNC_NULL )
            ;
    }

    // NR同步异常判断
	if((NR == scan_net_mode_result) || (LTE_NR == scan_net_mode_result) || ((LTE == scan_net_mode_result) &&( NR == band_net_mode[ band ] )))	//在NR模式 或者 LTE_NR模式下有效
	{
        if( sync_nr == SYNC_SYNC )
        {
            nr_resync_cnt = 0;
        }
        else if( sync_nr == SCAN_UNSYNC )
        {
            current_time = hi_get_tick();
            // 失步超过丢网时间,重新搜网
            if( nr_resync_cnt >= LOSE_NET_MAX_TIME/RE_SYNC_TIME )
            {
                res = 2;//重新扫描
                nr_resync_cnt = 0;
                rtt_printf( RTT_MODEM, RTT_TEXT_RED"Modem NR Lost Net!!! Re-Scan\r\n");
            // 失步超过重发时间,重发驻网
            }else if( (current_time - nr_last_sync_time) >= RE_SYNC_TIME * (nr_resync_cnt+1) && ((current_time - nr_last_track_time) >= RE_SYNC_TIME ))
            {
                nr_last_track_time = current_time;
                nr_resync_cnt++;
                nr_index++;

                if(nr_index>=hModem.scan_result.NR_scan_pci_num)
                    nr_index=0;
                modem_tracking_NR_cell( hModem.scan_result.NR_scan_tab[nr_index].freq, hModem.scan_result.NR_scan_tab[nr_index].pci ,41);
            }
        }else if( sync_nr == SYNC_NULL )
            ;
	}
#if 0	
	if( NR == scan_net_mode_result )
	{
        //rsrp 异常判断
		// if( rsrp_nr <= -140 )
		// {
		// 	if( start_time )
		// 	{
		// 		end_time = get_run_time();
		// 		if(( end_time - start_time ) >= LOSE_NET_MAX_TIME )
		// 		{
		// 			res = 2;//重新扫描
		// 			end_time = 0;
		// 			start_time = 0;
		// 		}
		// 	}
		// 	else
		// 		start_time = get_run_time();
		// }
		// else
		// {
		// 	start_time = 0;
		// 	end_time   = 0;
		// }
	}
	else if( LTE == scan_net_mode_result )
	{
        //rsrp 异常判断
		// if( rsrp_lte <= -130 ) 
		// {
		// 	if( start_time )
		// 	{
		// 		end_time = get_run_time();
		// 		if(( end_time - start_time ) >= LOSE_NET_MAX_TIME )
		// 		{
		// 			if((current_band_id + 1 <= band_end_id) && ( t_4g_scan_sort[ current_band_id + 1 ].band != 0 ) )
		// 			{
		// 				current_band_id++;
		// 				res = 1;	//切换下一个网络
		// 			}
		// 			else
		// 			{
		// 				res = 2;	//重新扫描
		// 			}
		// 			start_time = 0;
		// 			end_time   = 0;
		// 		}
		// 	}
		// 	else
		// 	{
		// 		start_time = get_run_time();
		// 	}
		// }	
		// else
		// {
		// 	start_time = 0;
		// 	end_time   = 0;
		// }
	}
	else if( LTE_NR == scan_net_mode_result )
	{
        //rsrp 异常判断
		// if( (rsrp_nr <= -140) || (rsrp_lte <= -130) ) 
		// {
		// 	if( start_time )
		// 	{
		// 		end_time = get_run_time();
		// 		if(( end_time - start_time ) >= LOSE_NET_MAX_TIME )
		// 		{
		// 			if((current_band_id + 1 <= band_end_id) && ( t_4g_scan_sort[ current_band_id + 1 ].band != 0 ) )
		// 			{
		// 					current_band_id++;
		// 					res = 1;	//切换下一个网络
		// 			}
		// 			else
		// 			{
		// 				res = 2;	//重新扫描
		// 			}
		// 			start_time = 0;
		// 			end_time   = 0;
		// 		}
		// 	}
		// 	else
		// 		start_time = get_run_time();
		// }
		// else
		// {
		// 	start_time = 0;
		// 	end_time   = 0;
		// }

	}
	else
	{
	
	}
#endif

	return res;
}


/**
* @brief      
* @param   band_id: [输入/出] 
**			 bw: [输入/出] 
* @return    
* @data    2024-03-24
* @note    
*/
static void scan_net_lte_ok( RF_BAND_E band_id, uint8_t bw )
{
	t_rf_sys[ RF_SYS_1 ].t_public.valid_band 	 = band_id;
	t_rf_sys[ RF_SYS_1 ].t_public.valid_bw   	 = bw;
	if( LTE == band_net_mode[band_id] )
		t_rf_sys[ RF_SYS_1 ].t_public.valid_freq     = scanf_fpoint_to_freq( band_net_mode[band_id], band_id, t_lte_scan_result.freq );	//频点转频率
	else
		t_rf_sys[ RF_SYS_1 ].t_public.valid_freq     = scanf_fpoint_to_freq( band_net_mode[band_id], band_id, t_nr_scan_result.freq );	//频点转频率
	t_rf_sys[ RF_SYS_1 ].t_public.net_mode       = band_net_mode[ band_id ];
	t_rf_sys[ RF_SYS_1 ].t_public.scan_net_state = B_TRUE;
	if((BAND_39 == band_id) || (BAND_40 == band_id))
		t_rf_sys[ RF_SYS_1 ].t_public.SysMode = IS_TDD;
	else
		t_rf_sys[ RF_SYS_1 ].t_public.SysMode = IS_FDD;
}

/**
* @brief      
* @param   None
* @return    
* @data    2024-03-24
* @note    
*/
static void scan_net_lte_fail( void )
{
	tm_sys_close( RF_SYS_1 );
	t_rf_sys[ RF_SYS_1 ].t_public.sync           = UNSYNC;
	t_rf_sys[ RF_SYS_1 ].t_public.valid_band 	 = BAND_3;
	t_rf_sys[ RF_SYS_1 ].t_public.valid_bw   	 = 20;
	t_rf_sys[ RF_SYS_1 ].t_public.valid_freq     = 0;	//pt->freq;
	t_rf_sys[ RF_SYS_1 ].t_public.net_mode       = LTE;
	t_rf_sys[ RF_SYS_1 ].t_public.scan_net_state = B_FALSE;
}

/**
* @brief      
* @param   None
* @return    
* @data    2024-03-24
* @note    
*/
static void scan_net_nr_ok( void )
{
	t_rf_sys[ RF_SYS_2 ].t_public.SysMode = IS_TDD;
	t_rf_sys[ RF_SYS_2 ].t_public.scan_net_state = t_rf_sys[ RF_SYS_2 ].t_public.valid_bw ? B_TRUE : B_FALSE;
	
	t_rf_sys[ RF_SYS_3 ].t_public.SysMode = IS_TDD;
	t_rf_sys[ RF_SYS_3 ].t_public.scan_net_state = ( DEV_MIMO_EXIST == 1 ) ? B_TRUE : 0;
}

/**
* @brief      
* @param   None
* @return    
* @data    2024-03-24
* @note    
*/
static void scan_net_nr_fail( void )
{
	t_rf_sys[ RF_SYS_2 ].t_public.SysMode = IS_TDD;
	t_rf_sys[ RF_SYS_2 ].t_public.scan_net_state = B_FALSE;
	
	t_rf_sys[ RF_SYS_3 ].t_public.SysMode = IS_TDD;
	t_rf_sys[ RF_SYS_3 ].t_public.scan_net_state = 0;
}

/*****************************************************************************
* @Name	  : scan_net_set_result_output
* @Brief  : 根据扫描结果设置5G模组输出
* @Param  : None
* @Retval : 
* @Author : 
* @Data   : 2024-01-18
* @Note   :
*****************************************************************************/
void scan_net_set_result_output( void )
{
	uint8_t err = 0;
	Status_t res = STAT_ERROR;
	
	scanf_result_t *pt = NULL;
	
	rf_set_mode_switch_en( B_FALSE );
	//vTaskDelay( 1000 );
	hi_delay_ms( 1000 );
	if((LTE_NR == scan_net_mode_result) || (LTE == scan_net_mode_result))
	{
		RF_BAND_E band_4g_id = band_num_to_serial( t_4g_scan_sort[ current_band_id ].band );
		if( band_4g_id < BAND_END )
		{
			if( B_TRUE == get_band_exist(band_4g_id) )
			{
				if( (LTE == scan_net_mode_result) && (NR==band_net_mode[band_4g_id]) )
				{
					pt = &t_nr_scan_result;
					pt->bw = bandwidth_net[ band_4g_id ];
				}
				else
					pt = &t_lte_scan_result;
				err = 3;
				while( err-- )
				{
					if( (LTE == scan_net_mode_result) && (NR==band_net_mode[band_4g_id]) )
						res = modem_tracking_NR_cell( pt->freq, pt->pci, pt->band);
					else
						res = modem_tracking_lte_cell( pt->freq, pt->pci, pt->band);
					
					if( STAT_OK == res )
					{
						scan_net_lte_ok( band_4g_id, pt->bw );
						break;
					}
					else
					{
						vTaskDelay( 200 );
						if( err == 0 )
						{
							set_band_scan_restart();
							rtt_printf( RTT_RF, RTT_TEXT_YELLOW"--->>> lte scan set result fail freq: %8d  pci: %5d,  band: %2d, <<<---\n", pt->freq, pt->pci, t_4g_scan_sort[ current_band_id ].band );
						}
					}
				}
			}
		}
	}
	else
	{
		scan_net_lte_fail();
	}
	
	vTaskDelay( 1000 );
	if((LTE_NR == scan_net_mode_result) || (NR == scan_net_mode_result))
	{
		pt = &t_nr_scan_result;
		err = 3;
		while( err-- )
		{
			res = modem_tracking_NR_cell( pt->freq, pt->pci ,41);
			if( STAT_OK == res )
			{
				scan_net_nr_ok();
				break;
			}
			else
			{
				vTaskDelay( 200 );
				if( err == 0 )
				{
					scan_net_nr_fail();
					set_band_scan_restart();
					rtt_printf( RTT_RF, RTT_TEXT_YELLOW"--->>> nr scan set result fail freq: %8d  pci: %5d,  band: %2d, <<<---\n", pt->freq, pt->pci, t_4g_scan_sort[ current_band_id ].band );
				}
			}
		}
	}
	else
	{
		scan_net_nr_fail();
	}
	
	vTaskDelay( 1000 );
	
	load_default_para_at_run_start();	//重新载入数据, 重定向指针
	
	rtt_printf( RTT_RF, "scan net end: \n" );
	if((LTE_NR == scan_net_mode_result) || (LTE == scan_net_mode_result)) 
		rtt_printf( RTT_RF, "--->>> SYS_1: band = %-2d, bw = %-3d\n", t_rf_sys[ RF_SYS_1 ].t_public.valid_band, t_rf_sys[ RF_SYS_1 ].t_public.valid_bw );
	if((LTE_NR == scan_net_mode_result) || (NR == scan_net_mode_result))
	{
		rtt_printf( RTT_RF, "--->>> SYS_2: band = %-2d, bw = %-3d\n", t_rf_sys[ RF_SYS_2 ].t_public.valid_band, t_rf_sys[ RF_SYS_2 ].t_public.valid_bw );
		if( DEV_MIMO_EXIST )
		rtt_printf( RTT_RF, "--->>> SYS_3: band = %-2d, bw = %-3d\n", t_rf_sys[ RF_SYS_3 ].t_public.valid_band, t_rf_sys[ RF_SYS_3 ].t_public.valid_bw );
	}
	
	rf_set_mode_switch_en( B_TRUE );
}


/*****************************************************************************
* @Name	  : rf_5g_modem_in_task
* @Brief  : 任务处理流程
* @Param  : None
* @Retval : 
* @Author : 
* @Data   : 2024-01-19
* @Note   :
*****************************************************************************/
void rf_5g_modem_in_task( void )
{
	RF_BAND_E band = BAND_END;
	uint8_t res = B_FALSE;
	uint8_t band_end_id = ( DEV_BAND_NUM == 5 ) ? 4 : 2;
	Status_t scan_result = STAT_ERROR;
	uint8_t retry = 0;
	static BOOL_E scan_lte = B_FALSE;
	static BOOL_E scan_nr  = B_FALSE;
	scanf_result_t *pt_src = NULL;
	
	if((WORK_PL == tDevPublicPara.ucWorkMode) || (WORK_CALIB == tDevPublicPara.ucWorkMode ))
	{
		//rf_set_define_band_in_product_mode( BAND_3 );
		return;
	}

    if(modem_get_pwron_stat() == B_FALSE) //check if modem is power on
        return;

	if(B_TRUE == rf_5g_modem_rescan() && (SCAN_STATE_IDLE != e_scan_state))
    {
        rtt_printf( RTT_RF, RTT_TEXT_RED"--->>>modem reset or reboot, scan restart.... \n" );
        e_scan_state = SCAN_STATE_INIT;
    }

	rescan_state_set();	//判断是否需要重新扫描
	
	switch( e_scan_state )
	{
		case SCAN_STATE_INIT:		//初始化 
				if( B_TRUE == rf_5g_modem_ready() )	//获取5g 模组状态
				{
					rf_5g_modem_set_edge();
					vTaskDelay( 1000 );
					e_scan_state = SCAN_STATE_4G_SCAN;
				}
				break;
				
		case SCAN_STATE_IDLE:		//空闲 
				e_scan_state = SCAN_STATE_INIT;
				break;
		
		case SCAN_STATE_4G_SCAN:	//4G扫描, 排序 
				rf_set_mode_switch_en( B_FALSE );
				if( RF_SWITCH_CLOSE == t_rf_sys[RF_SYS_1].t_public.tRfSwitch.ucNormalState )	//射频关闭
				{
					scan_lte = B_FALSE;
					current_band_id = 0;
					t_4g_scan_sort[ 0 ].band = 0;
					e_scan_state = SCAN_STATE_5G_SCAN;
					break;
				}
				
				rtt_printf( RTT_RF, RTT_TEXT_RED"--->>> start scan lte band.... \n" );

				scan_net_process( band_net_mode[ BAND_3 ], BAND_3,  BAND_3, t_rf_4g_save_para[0].exist );
				if( t_rf_4g_save_para[0].exist )
				{
					pt_src = ( NR == band_net_mode[ BAND_3 ] ) ? &t_nr_scan_result : &t_lte_scan_result;
					memcpy( &t_4g_scan_sort[BAND_3], pt_src, sizeof( scanf_result_t ) );	//保存4g band扫描结果
				}
				
				band_net_mode[ BAND_8 ] = LTE;
				scan_net_process( band_net_mode[ BAND_8 ], BAND_8,  BAND_8, t_rf_4g_save_para[1].exist );
				if( t_rf_4g_save_para[1].exist )
					memcpy( &t_4g_scan_sort[BAND_8], &t_lte_scan_result, sizeof( scanf_result_t ) );	//保存4g band扫描结果

				scan_net_process( band_net_mode[ BAND_39 ], BAND_39, BAND_39, t_rf_4g_save_para[2].exist );
				if( t_rf_4g_save_para[2].exist )
				{
					pt_src = ( NR == band_net_mode[ BAND_39 ] ) ? &t_nr_scan_result : &t_lte_scan_result;
					memcpy( &t_4g_scan_sort[BAND_39], pt_src, sizeof( scanf_result_t ) );	//保存4g band扫描结果
				}
		
				scan_net_process( band_net_mode[ BAND_40 ], BAND_40, BAND_40, t_rf_4g_save_para[3].exist );
				if( t_rf_4g_save_para[3].exist )
				{
					pt_src = ( NR == band_net_mode[ BAND_40 ] ) ? &t_nr_scan_result : &t_lte_scan_result;
					memcpy( &t_4g_scan_sort[BAND_40], pt_src, sizeof( scanf_result_t ) );	//保存4g band扫描结果
				}
		
				scan_result_sort( t_4g_scan_sort, band_end_id, -128 );	//对4个频段排序
				current_band_id = 0;
				if( (0 == t_4g_scan_sort[ current_band_id ].band) || (t_4g_scan_sort[ current_band_id ].rsrp<-120) )
					scan_lte = B_FALSE;
				else
					scan_lte = B_TRUE;
				
				band = band_num_to_serial( t_4g_scan_sort[ current_band_id ].band );
				if( NR == band_net_mode[ band ] )	//LTE处于NR模式
				{
					scan_nr = B_FALSE;
					e_scan_state = SCAN_STATE_RESULT;
				}
				else
					e_scan_state = SCAN_STATE_5G_SCAN;
				break;
		
		case SCAN_STATE_5G_SCAN:	//5G扫描
				if( RF_SWITCH_CLOSE == t_rf_sys[RF_SYS_2].t_public.tRfSwitch.ucNormalState )	//射频关闭
				{
					scan_nr = B_FALSE;
					e_scan_state = SCAN_STATE_RESULT;
					break;
				}
				rtt_printf( RTT_RF, RTT_TEXT_RED"--->>> start scan nr band.... \n" );
				scan_result = scan_net_process( NR, (RF_BAND_E)0, BAND_41, t_rf_sys[RF_SYS_2].t_public.exist );
				if( (STAT_OK == scan_result) && (t_nr_scan_result.rsrp>=-120) )
					scan_nr = B_TRUE;
				else
				{
					scan_nr = B_FALSE;
					rtt_printf( RTT_RF, RTT_TEXT_RED"--->>> scan NR error!\n", retry );
				}
				e_scan_state = SCAN_STATE_RESULT;
				break;
		
		case SCAN_STATE_RESULT:		//设置结果 
				if( (scan_nr==B_TRUE) && (scan_lte == B_TRUE ))
				{
					band = band_num_to_serial( t_4g_scan_sort[ current_band_id ].band );
					scan_net_mode_result = LTE_NR;
					rtt_printf(RTT_RF, RTT_TEXT_YELLOW "--->>>modem work in LTE_NR ....\n" );
				}
				else if( scan_nr == B_TRUE )
				{
					band = BAND_3;
					scan_net_mode_result = NR;
					rtt_printf(RTT_RF, RTT_TEXT_YELLOW "--->>>modem work in NR ....\n" );
				}
				else if( scan_lte == B_TRUE )
				{
					band = band_num_to_serial( t_4g_scan_sort[ current_band_id ].band );
					
					scan_net_mode_result = LTE;
					if ( NR == band_net_mode[ band ] ) 
						rtt_printf(RTT_RF, RTT_TEXT_YELLOW "--->>>modem work in LTE(nr) ....\n" );
					else
						rtt_printf(RTT_RF, RTT_TEXT_YELLOW "--->>>modem work in LTE(lte) ....\n" );
				}
				else
				{
					scan_net_mode_result = MODE_NULL;
					rtt_printf(RTT_RF, RTT_TEXT_RED "--->>>modem scan fail, delay & reset ....\n" );
					vTaskDelay( 5000 );	//delay, reset;	//需要添加异常处理
					e_scan_state = SCAN_STATE_QUERY;
					
					break;
				}
				
				vTaskDelay( 1000 );
				
				retry = 3;
				while( retry-- )
				{
					if( LTE == scan_net_mode_result )
					{		
						if(NR == band_net_mode[ band ])
							scan_result = scan_net_process( NR, band, band, B_TRUE );		//LTE频段 处于 NR 模式
						else
							scan_result = scan_net_process( LTE, band, band, B_TRUE );		//LTE频段 处于 NR 模式
					}
					else if( NR == scan_net_mode_result || LTE_NR == scan_net_mode_result )
						scan_result = scan_net_process( scan_net_mode_result, band, BAND_41, B_TRUE );
                    else
                        scan_result = STAT_ERROR;
					
					if( STAT_OK == scan_result )
					{
						vTaskDelay( 1000 );
						scan_net_set_result_output();
						if( ( LTE == scan_net_mode_result ) || ( LTE_NR == scan_net_mode_result ) )
						{
							flag_fpga_lte_cmd_reload();
						}
						
						if((NR == scan_net_mode_result ) || ( LTE_NR == scan_net_mode_result ) )
						{
							flag_fpga_nr_cmd_reload();
						}
                        //flag_fpga_cmd_reload();			//重载后向FPGA发送4g配置文件
						set_dc_switch_by_scan_result();
						e_scan_state = SCAN_STATE_QUERY;
						break;
					}
					else
					{
						if( 0 == retry )
						{
							set_band_scan_restart();
						}
						rtt_printf( RTT_RF, RTT_TEXT_RED"--->>> scan LTE_NR error!, retry = %d\n", retry );
					}
				}
				break;
		
		case SCAN_STATE_QUERY:		//查询参数 
				rf_set_mode_switch_en( B_TRUE );
				res = real_time_net_info_analysis();
				if( 1 == res )
					set_band_scan_next();	//切换频段
				else if(2 == res) 
					set_band_scan_restart();
				else
					e_scan_state = SCAN_STATE_QUERY;	
				break;
				
		case SCAN_STATE_EXCEPT:		//异常处理 
				set_band_scan_restart();
				break;
		
		default:
				break;
	}
}

/**
* @brief   根据扫描结果设置各通道开关状态  
* @param   None
* @return    
* @data    2024-04-02
* @note    
*/
void set_dc_switch_by_scan_result( void )
{
	if( WORK_NORMAL == tDevPublicPara.ucWorkMode )
	{
		if((LTE == scan_net_mode_result) || (LTE_NR == scan_net_mode_result))
		{
			rf_dc_set( RF_SYS_1, PORT_ON );
			t_rf_sys[RF_SYS_1].t_public.dc_switch = OPEN;
		}
		else
		{
			rf_dc_set( RF_SYS_1, PORT_OFF );
			t_rf_sys[RF_SYS_1].t_public.dc_switch = CLOSE;
		}
		
		if((NR == scan_net_mode_result) || (LTE_NR == scan_net_mode_result))
		{
			rf_dc_set( RF_SYS_2, PORT_ON );
			t_rf_sys[RF_SYS_2].t_public.dc_switch = OPEN;
			if( DEV_MIMO_EXIST )
			{
				rf_dc_set( RF_SYS_3, PORT_ON );
				t_rf_sys[RF_SYS_3].t_public.dc_switch = OPEN;
			}
			else
			{
				rf_dc_set( RF_SYS_3, PORT_OFF );
				t_rf_sys[RF_SYS_3].t_public.dc_switch = CLOSE;
			}
		}
		else
		{
			rf_dc_set( RF_SYS_2, PORT_OFF );
			t_rf_sys[RF_SYS_2].t_public.dc_switch = CLOSE;
			
			rf_dc_set( RF_SYS_3, PORT_OFF );
			t_rf_sys[RF_SYS_3].t_public.dc_switch = CLOSE;
		}
	}
}


/**
* @brief   校准模式下测量耦合增益差值  
* @param   band: [输入/出] 
* @return    
* @data    2024-04-06
* @note    
*/
/*
Status_t measure_coupling_gain_in_calib_mode( RF_BAND_E band )
{
	uint8_t  retry = 50;
	Status_t res   = STAT_ERROR;
	
	if( band >= BAND_40 )
		return STAT_ERROR;
	
	while( retry-- )
	{
		if( B_TRUE == rf_5g_modem_ready() )	//获取5g 模组状态
		{
			rf_5g_modem_set_edge();
			vTaskDelay( 100 );
			res = STAT_OK;
			break;
		}
	}
	
	if( STAT_ERROR == res )
		return STAT_ERROR;

	rf_set_mode_switch_en( B_FALSE );
	res = scan_net_process( LTE, band,  band, t_rf_4g_save_para[0].exist );
	return res;
}
*/

/**
* @brief   校准模式下测量耦合增益差值  
* @param   band: [输入/出] 
* @return    
* @data    2024-04-06
* @note    
*/
Status_t measure_coupling_gain_in_calib_mode( RF_BAND_E band )
{
	uint8_t  retry = 50;
	Status_t res   = STAT_ERROR;
	
	LTE_net_info_t lte_info;
	
	tm_sys_open( band );
	
	while( retry-- )
	{
		if( B_TRUE == rf_5g_modem_ready() )	//获取5g 模组状态
		{
			rf_5g_modem_set_edge();
			vTaskDelay( 100 );
			res = STAT_OK;
			break;
		}
	}
	
	if( STAT_ERROR == res )
		return STAT_ERROR;

	rf_set_mode_switch_en( B_FALSE );
	if( BAND_41 != band )
		res = scan_net_process( LTE, band,  band, t_rf_4g_save_para[0].exist );
	else
		res = scan_net_process( NR, BAND_3,  BAND_41, B_TRUE );
	
	if( STAT_OK == res )
	{
		scan_net_mode_result = ( BAND_41 == band ) ? NR : LTE;
		if( BAND_41 != band )
		{
			current_band_id = 0;
			t_4g_scan_sort[ current_band_id ].band = band_serial_to_num( band );
		}
		scan_net_set_result_output();
		rtt_printf( RTT_RF, RTT_TEXT_YELLOW"scan_rsrp: %d, %d\n", t_lte_scan_result.rsrp, lte_info.rsrp );
	}
	return res;
}

/**
* @brief   校准模式下读取同步状态   
* @param   sys: [输入/出] 
* @return    
* @data    2024-04-06
* @note    
*/
SYNC_STATE_E read_sync_in_calib_mode( RF_SYS_E sys )
{
	SYNC_STAT_E state = SYNC_SYNC;
	if( RF_SYS_1 )
	{
		state = hModem.LTE_net_info.LTE_Sync;
	}
	else
	{
		state = hModem.NR_net_info.NR_Sync;
	}
	
	if( SYNC_SYNC == state )
		return SYNC;
	else
		return UNSYNC;
}

/**
* @brief   校准模式下读取RSRP值
* @param   sys: [输入/出] 
* @return    
* @data    2024-04-06
* @note    
*/
int16_t read_rsrp_in_calib_mode( NET_MODE mode )
{
//	if( WORK_CALIB == tDevPublicPara.ucWorkMode )
//	{
//		if( mode == LTE )
//			return t_lte_scan_result.rsrp;
//		else 
//			return t_nr_scan_result.rsrp;
//	}
//	else
//	{
		if( mode == LTE )
			return hModem.LTE_net_info.rsrp;
		else 
			return hModem.NR_net_info.rsrp;
//	}
}
