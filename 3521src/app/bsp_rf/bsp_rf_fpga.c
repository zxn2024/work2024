/***************************** Copyright ****************************************
*
* (C) Copyright 2024, Shaanxi Tianji Communication
*  All Rights Reserved
*                         
* FileName   : bsp_rf_fpga.c   
* Version    : none		
* Author     : 			
* Date       : 2024-01-26         
* Description: none   
*	主机：
*	1、设置4G频段 （包括mipi的设置）
*	   uint8_t fpga_set_4g_freqband(uint8_t fb);  
*		   参数：fb：0 B3(1800M)    1 B8(900M)    2 B39(F频段)    3 B40(E频段)
*	2、设置4G子帧配比
*	   uint8_t fpga_set_4g_subframe(uint8_t subframe);
*		   参数说明: subframe 0~6  目前支持0、1、2
*	3、设置4G特殊子帧配比
*	   uint8_t fpga_set_4g_specical_subframe(uint8_t specical_subframe);
*		   specical_subframe 0~8
*	4、设置4G中心频点
*	   uint8_t fpga_set_4g_center_freq(uint32_t center_freq);
*	5、设置4G带宽
*	   uint8_t fpga_set_4g_bandwidth(uint32_t bandwidth);
*	6、设置5G帧格式
*	   uint8_t fpga_set_5g_frame(char *pf);  
*		   参数：如：02070000040406
*
*	从机:
*	1、读取4G频段
*	   uint8_t fpga_get_4g_freqband(void);  
*		   返回值：fb：0 B3(1800M)    1 B8(900M)    2 B39(F频段)    3 B40(E频段)
*	2、设置4G频段 （mipi设置）
*	   uint8_t fpga_set_4g_freqband(uint8_t fb);  
*		   参数：fb：0 B3(1800M)    1 B8(900M)    2 B39(F频段)    3 B40(E频段)
*	3、读取4G中心频点
*	   uint32_t fpga_get_4g_center_freq(void); 
*	4、读取4G带宽
*	   uint32_t fpga_get_4g_bandwidth(void);
*	5、设置4G上行静默 (关MIPI)
*	   uint8_t fpga_set_4g_ul_silent(void);
*	6、设置TRX增益 (退出静默时设置)
*	   uint8_t fpga_set_trx_gain(uint8_t op_type, uint32_t gain);
*	7、读取TRX RSSI
*	   int8_t fpga_get_trx_rssi(uint8_t op_type);
*******************************************************************************/
#include "main.h"
//#include "stdlib.h"
//#include "stdio.h"
//#include "string.h"
//#include "MCU_config.h"
#include "public_define.h"
#include "public_enum.h"
#include "public_struct.h"
#include "bsp_rf_fpga.h"
#include "bsp_rf_global.h"
#include "fpga.h"
#include "rtt.h"
#if( _MU == LOCAL_MODULE_TYPE )
#include "eswin_sync_modem.h"
#include "bsp_rf_5g_modem.h"
#include "bsp_rf_scan_net.h"
#endif
/*---------------------- 端口定义 --------------------------------------*/


/*---------------------- 宏定义  ---------------------------------------*/


/*---------------------- 结构体 ----------------------------------------*/
typedef struct
{
	uint32_t set_4g_band 			  : 1;	//MU & RU, 设置4G频段
	uint32_t set_4g_subframe 		  : 1;	//MU, 设置4G上下行配比
	uint32_t set_4g_specical_subframe : 1;  //MU, 设置4G特殊子帧格式
	uint32_t set_4g_center_freq 	  : 1;  //MU, 设置4G中心频点
	uint32_t set_4g_bandwidth 		  : 1;  //MU, 设置带宽
	uint32_t set_5g_frame 			  : 1;  //MU, 设置5G帧格式
	uint32_t set_5g_frame_bw		  : 1;  //MU, 设置5G带宽
	uint32_t set_5g_center_freq 	  : 1;  //MU, 中心频点
	uint32_t set_4g_ul_silence		  : 1;	//RU, 设置4G上行静默
	uint32_t set_trx_gain			  : 1;  //设置TRX增益, (退出静默时设置)
	
	uint32_t read_4g_band			  : 1;	//RU, 读取4G频段
	uint32_t read_4g_center_freq	  : 1;	//RU, 读取4G中心频点
	uint32_t read_4g_bw				  : 1;  //RU, 读取带宽
	uint32_t read_trx_rssi			  : 1;  //RU, 读取TRX RSSI
	
}rf_fpga_cmd_t;

/*---------------------- 常量 ------------------------------------------*/


/*---------------------- 全局变量 --------------------------------------*/


/*---------------------- 局部变量 --------------------------------------*/
rf_fpga_cmd_t t_cmd_fpga;

/*---------------------- 声明 ------------------------------------------*/
#if( _MU == LOCAL_MODULE_TYPE )
extern sync_modem_t hModem; // MODEM参数信息
#endif

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ HAL ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/



/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DRV ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*****************************************************************************
* @Name	  : nr_frame_to_string
* @Brief  : 5g帧结构转换为字符串
* @Param  : pt: [输入/出] 
**			 frame: [输入/出] 
**			 special_frame: [输入/出] 
* @Retval : B_TRUE: 执行正常,  B_FALSE: 执行错误, frame 或者 special_frame参数无效
* @Author : 
* @Data   : 2024-01-29
* @Note   :
*****************************************************************************/
static BOOL_E nr_frame_to_string( uint8_t *pt, uint8_t frame, uint8_t special_frame )
{
	BOOL_E res = B_FALSE;
	uint8_t frame_array[6][9] = { "02070000",
								  "00010000",
								  "01030000",
								  "02020000",
								  "03010000",
								  "01030202"
								};
	uint8_t s_frame_array[3][7] = { "040406",
									"100202",
									"020210"
								  };
	
	if( (frame>=1) && (frame<=6) && (special_frame>=1) && (special_frame<=3) )
	{
		memcpy( pt, frame_array[frame-1], 9 );
		memcpy( pt+8, s_frame_array[special_frame-1], 7 );
		rtt_printf( RTT_RF, "5g frame to string: frame = %d, s_frame = %d, cmd = %s\n", frame, special_frame, frame, pt );
		res = B_TRUE;
	}
	else
		res = B_FALSE;
	
	return res;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ APP ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*****************************************************************************
* @Name	  : flag_fpga_cmd_set_4g_band
* @Brief  : 
* @Param  : None
* @Retval : 
* @Author : 
* @Data   : 2024-01-30
* @Note   :
*****************************************************************************/
void flag_fpga_cmd_set_4g_band( void )
{
	t_cmd_fpga.set_4g_band = 1;
}

/*****************************************************************************
* @Name	  : flag_fpga_cmd_set_4g_subframe
* @Brief  : 
* @Param  : None
* @Retval : 
* @Author : 
* @Data   : 2024-01-30
* @Note   :
*****************************************************************************/
void flag_fpga_cmd_set_4g_subframe( void )
{
	t_cmd_fpga.set_4g_subframe = 1;
}

/*****************************************************************************
* @Name	  : flag_fpga_cmd_set_4g_specical_subframe
* @Brief  : 
* @Param  : None
* @Retval : 
* @Author : 
* @Data   : 2024-01-30
* @Note   :
*****************************************************************************/
void flag_fpga_cmd_set_4g_specical_subframe( void )
{
	t_cmd_fpga.set_4g_specical_subframe = 1;
}

/*****************************************************************************
* @Name	  : flag_fpga_cmd_set_4g_center_freq
* @Brief  : 
* @Param  : None
* @Retval : 
* @Author : 
* @Data   : 2024-01-30
* @Note   :
*****************************************************************************/
void flag_fpga_cmd_set_4g_center_freq( void )
{
	t_cmd_fpga.set_4g_center_freq = 1;
}

/*****************************************************************************
* @Name	  : flag_fpga_cmd_set_4g_bandwidth
* @Brief  : 
* @Param  : None
* @Retval : 
* @Author : 
* @Data   : 2024-01-30
* @Note   :
*****************************************************************************/
void flag_fpga_cmd_set_4g_bandwidth( void )
{
	t_cmd_fpga.set_4g_bandwidth = 1;
}

/*****************************************************************************
* @Name	  : flag_fpga_cmd_set_5g_frame
* @Brief  : 
* @Param  : None
* @Retval : 
* @Author : 
* @Data   : 2024-01-30
* @Note   :
*****************************************************************************/
void flag_fpga_cmd_set_5g_frame( void )
{
	t_cmd_fpga.set_5g_frame = 1;
}

/**
* @brief   设置5G带宽
* @param   None
* @return    
* @data    2024-03-28
* @note    
*/
void flag_fpga_cmd_set_5g_bw( void )
{
	t_cmd_fpga.set_5g_frame_bw = 1;
}


/**
* @brief   设置5G 中心频率  
* @param   None
* @return    
* @data    2024-03-29
* @note    
*/
void flag_fpga_cmd_set_5g_center_freq( void )
{
	t_cmd_fpga.set_5g_center_freq = 1;
}

/*****************************************************************************
* @Name	  : flag_fpga_cmd_set_4g_ul_silence
* @Brief  : 
* @Param  : None
* @Retval : 
* @Author : 
* @Data   : 2024-01-30
* @Note   :
*****************************************************************************/
void flag_fpga_cmd_set_4g_ul_silence( void )
{
	t_cmd_fpga.set_4g_ul_silence = 1;
}

/*****************************************************************************
* @Name	  : flag_fpga_cmd_set_trx_gain
* @Brief  : 
* @Param  : None
* @Retval : 
* @Author : 
* @Data   : 2024-01-30
* @Note   :
*****************************************************************************/
void flag_fpga_cmd_set_trx_gain( void )
{
	t_cmd_fpga.set_trx_gain = 1;
}

/*****************************************************************************
* @Name	  : flag_fpga_cmd_read_4g_band
* @Brief  : 
* @Param  : None
* @Retval : 
* @Author : 
* @Data   : 2024-01-30
* @Note   :
*****************************************************************************/
void flag_fpga_cmd_read_4g_band( void )
{
	t_cmd_fpga.read_4g_band = 1;
}

/*****************************************************************************
* @Name	  : flag_fpga_cmd_read_4g_center_freq
* @Brief  : 
* @Param  : None
* @Retval : 
* @Author : 
* @Data   : 2024-01-30
* @Note   :
*****************************************************************************/
void flag_fpga_cmd_read_4g_center_freq( void )
{
	t_cmd_fpga.read_4g_center_freq = 1;
}

/*****************************************************************************
* @Name	  : flag_fpga_cmd_read_4g_bw
* @Brief  : 
* @Param  : None
* @Retval : 
* @Author : 
* @Data   : 2024-01-30
* @Note   :
*****************************************************************************/
void flag_fpga_cmd_read_4g_bw( void )
{
	t_cmd_fpga.read_4g_bw = 1;
}

/*****************************************************************************
* @Name	  : flag_fpga_cmd_read_trx_rssi
* @Brief  : 
* @Param  : None
* @Retval : 
* @Author : 
* @Data   : 2024-01-30
* @Note   :
*****************************************************************************/
void flag_fpga_cmd_read_trx_rssi( void )
{
	t_cmd_fpga.read_trx_rssi = 1;
}

/*****************************************************************************
* @Name	  : flag_fpga_cmd_reload
* @Brief  : 重载后设置FPGA标识, 内部包含4G和5G
* @Param  : None
* @Retval : 
* @Author : 
* @Data   : 2024-01-30
* @Note   :
*****************************************************************************/
void flag_fpga_cmd_reload( void )
{
	flag_fpga_cmd_set_4g_band();
	flag_fpga_cmd_set_4g_subframe();
	flag_fpga_cmd_set_4g_specical_subframe();
	flag_fpga_cmd_set_4g_center_freq();
	flag_fpga_cmd_set_4g_bandwidth();
	flag_fpga_cmd_set_5g_frame();
	flag_fpga_cmd_set_5g_bw();
	flag_fpga_cmd_set_5g_center_freq();
}

/**
* @brief   重载后设置FPGA标识, 仅LTE 
* @param   None
* @return    
* @data    2024-04-02
* @note    
*/
void flag_fpga_lte_cmd_reload( void )
{
	flag_fpga_cmd_set_4g_band();
	flag_fpga_cmd_set_4g_subframe();
	flag_fpga_cmd_set_4g_specical_subframe();
	flag_fpga_cmd_set_4g_center_freq();
	flag_fpga_cmd_set_4g_bandwidth();
}

/**
* @brief   重载后设置FPGA标识, 仅NR
* @param   None
* @return    
* @data    2024-04-02
* @note    
*/
void flag_fpga_nr_cmd_reload( void )
{
	flag_fpga_cmd_set_5g_frame();
	flag_fpga_cmd_set_5g_bw();
	flag_fpga_cmd_set_5g_center_freq();
}

/*****************************************************************************
* @Name	  : rf_set_fpga_cmd
* @Brief  : 
* @Param  : None
* @Retval : 
* @Author : 
* @Data   : 2024-01-29
* @Note   :
*****************************************************************************/
#if( _MU == LOCAL_MODULE_TYPE )
void rf_set_fpga_cmd( void )
{
	BOOL_E b_res = B_FALSE;
	uint8_t res = 0;
	
	rf_fpga_cmd_t *pt_cmd = &t_cmd_fpga;
	RfSysPublic_T *pt_rf = &t_rf_sys[RF_SYS_1].t_public;
	
	if( pt_cmd->set_4g_band )	    //频段
	{
		rtt_printf( RTT_RF, "send cmd to fpga - freqband = %d\n", pt_rf->valid_band );
		res = fpga_set_4g_freqband( pt_rf->valid_band );
		if( 1 == res )
			pt_cmd->set_4g_band = 0;
	}
	
	if( pt_cmd->set_4g_bandwidth )	//带宽
	{
		rtt_printf( RTT_RF, "send cmd to fpga - 4g_bw = %d\n", pt_rf->valid_bw );
		res = fpga_set_4g_bandwidth( pt_rf->valid_bw );
		if( 1 == res )
			pt_cmd->set_4g_bandwidth = 0;
	}
	
	if( pt_cmd->set_4g_center_freq ) //中心频率
	{
		rtt_printf( RTT_RF, "send cmd to fpga - lte_center_freq = %d\n", pt_rf->valid_freq );
		res = fpga_set_4g_center_freq( pt_rf->valid_freq );
		if( 1 == res )
			pt_cmd->set_4g_center_freq = 0;
	}
	
	if((BAND_3 == pt_rf->valid_band) || (BAND_8 == pt_rf->valid_band) )	//频分系统无需设置帧结构和子帧配比
	{
		pt_cmd->set_4g_subframe = 0;
		pt_cmd->set_4g_specical_subframe = 0;
	}
	
	if( pt_cmd->set_4g_subframe )	//帧结构
	{
		uint8_t sfa = 0;
		if( NR == t_rf_sys[RF_SYS_1].t_public.net_mode )
			sfa = frame_net[ t_rf_sys[RF_SYS_1].t_public.valid_band ];
		else
			sfa = hModem.LTE_net_info.sfa;

		rtt_printf( RTT_RF, "send cmd to fpga - lte_frame = %d\n", sfa );
		res = fpga_set_4g_subframe( sfa );	//subframe: 0~6  目前支持0、1、2
		if( 1 == res )
			pt_cmd->set_4g_subframe = 0;
	}
	
	if( pt_cmd->set_4g_specical_subframe )	//特殊子帧
	{
		uint8_t ssfp = 0;
		if( NR == t_rf_sys[RF_SYS_1].t_public.net_mode )
			ssfp = sp_frame_net[ t_rf_sys[RF_SYS_1].t_public.valid_band ];
		else
			ssfp = hModem.LTE_net_info.ssfp;

		rtt_printf( RTT_RF, "send cmd to fpga - lte_spframe = %d\n", ssfp );
		res = fpga_set_4g_specical_subframe( ssfp );	//specical_subframe  0~8
		if( 1 == res )
			pt_cmd->set_4g_specical_subframe = 0;
	}
	
	if( pt_cmd->set_5g_frame )	//5G帧结构
	{
		uint8_t frame_array[20] = {0};
		b_res = nr_frame_to_string( frame_array, t_5g_modem_config.frame, t_5g_modem_config.specialTime );
		if( B_TRUE == b_res )
		{
			rtt_printf( RTT_RF, "send cmd to fpga - nr_frame = %s\n", frame_array );
			res = fpga_set_5g_frame( (char *)frame_array );	//specical_subframe  0~8
			if( 1 == res )
				pt_cmd->set_5g_frame = 0;
		}
	}
	
	if( pt_cmd->set_5g_frame_bw )
	{
		rtt_printf( RTT_RF, "send cmd to fpga - nr_bandwidth = %d\n", t_rf_sys[1].t_public.valid_bw );
		res = fpga_set_5g_bandwidth( t_rf_sys[1].t_public.valid_bw );	//specical_subframe  0~8
		if( 1 == res )
			pt_cmd->set_5g_frame_bw = 0;
	}
	
	if( pt_cmd->set_5g_center_freq )
	{
		rtt_printf( RTT_RF, "send cmd to fpga - nr_freq = %d\n", t_rf_sys[1].t_public.valid_freq );
		res = fpga_set_5g_center_freq( t_rf_sys[1].t_public.valid_freq );	//specical_subframe  0~8
		if( 1 == res )
			pt_cmd->set_5g_center_freq = 0;
	}
}

#else
void rf_set_fpga_cmd( void )
{

}

#endif


