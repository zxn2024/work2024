/***************************** Copyright ****************************************
*
* (C) Copyright 2023, Shaanxi Tianji Communication
*  All Rights Reserved
*                         
* FileName   : bsp_rf_iso_fd.c   
* Version    : none		
* Author     : 			
* Date       : 2023-04-04         
* Description: 
*            1.修正 iso_set_att() 函数
* 			 2.修正 iso_detection() 函数中的att_min_value 和 att_num 两个参数数值
*******************************************************************************/
#include "main.h"
//#include "stdlib.h"
//#include "stdio.h"
//#include "string.h"
#include "public_types.h"
#include "bsp_public_struct.h"
#include "bsp_public_macro.h"
#include "bsp_public_drv.h"
#include "bsp_dev_config.h"
#include "bsp_rf_iso_fd.h"

#include "FreeRTOS.h"
#include "task.h"
#include "bsp_rtt.h"
#include "bsp_rf_ctr.h"
#include "bsp_rf_lna.h"
#include "bsp_rf_pa.h"
#include "bsp_rf_analog_att.h"
#include "bsp_rf_att.h"
#include "bsp_rf_pow.h"

#include "bsp_cover_switch.h"


/*---------------------- 端口定义 --------------------------------------*/


/*---------------------- 宏定义  ---------------------------------------*/
enum{
	ISO_REQUEST_NONE = 0,		//不检测隔离度
	ISO_REQUEST_ONLY_START,		//仅开机检测
	ISO_REQUEST_ONLY_PROCESS,	//仅过程检测
	ISO_REQUEST_ALL_THE_TIME	//开机/过程均检测
};


enum{
	ISO_RES_OK = 0,		//隔离度正常
	ISO_RES_AE,			//自激
	ISO_RES_POWOVER		//过功率
};

//隔离度发现过程结构体
typedef struct
{
	int8_t  pow_last;						//前一次功率保存值
	uint8_t discovery_num;					//发现次数
	uint8_t cyc_num;						//循环次数
	const uint8_t const_cyc_max_num;		//过程中隔离度检测的循环次数
	const uint8_t const_discovery_min_num;	//过程中隔离度检测ATT动作1dB时功率的跳变阙值, 大于此数据, 表示可能自激
} ch_iso_discovery_t;	

//恢复数据结构体
typedef struct 
{
	uint8_t  retry_num;						//重启次数
	uint8_t  retry_sign;					//重新检测隔离度标识
	uint32_t iso_resumt_time_cnt;			//半小时计数器
	const uint8_t const_retry_max_num;		//最大重新检测隔离度次数
} iso_resume_t;

//诱导程序结构体
typedef struct
{
	uint8_t induce_time_cnt;		//诱导计数器
	uint8_t induce_sign;			//诱导标识
} iso_induce_t;

typedef struct
{
	const uint8_t const_request;		//隔离度检测要求, 0: 不检测, 1: 开机检测, 过程不检测, 2: 开机不检, 过程检测, 3: 开机过程均检测 
	const uint8_t const_iso_att_back;	//隔离度回退值
	const uint8_t const_delta_step5;	//ATT 5dB步进变化时的功率跳动阙值
	const uint8_t const_delta_step1;	//ATT 1dB步进变化时的功率跳动阙值
	
	ch_iso_discovery_t   t_discovery;	//用于发现自激的数据
	EqIndependentPara_T* pt_ch_para;	//通道参数
}ch_iso_para_t;




/*---------------------- 结构体 ----------------------------------------*/


/*---------------------- 常量 ------------------------------------------*/
#if (LOCAL_MODULE_TYPE == MODULE_TYPE_MU)
	#define ISO_BACK_VALUE 	( 10 )	//主机回退10dB
#else
	#define ISO_BACK_VALUE 	( 5 )	//从机回退4dB
#endif

/*---------------------- 全局变量 --------------------------------------*/
ch_iso_para_t t_iso_para[RF_SYS_NUM][RF_CH_END] = { { { .const_request = ISO_REQUEST_ONLY_START,   .const_iso_att_back = ISO_BACK_VALUE, .const_delta_step5 = 8, .const_delta_step1 = 5, .t_discovery.const_cyc_max_num = 50, .t_discovery.const_discovery_min_num = 10 }, 
													  { .const_request = ISO_REQUEST_ALL_THE_TIME, .const_iso_att_back = ISO_BACK_VALUE, .const_delta_step5 = 8, .const_delta_step1 = 5, .t_discovery.const_cyc_max_num = 50, .t_discovery.const_discovery_min_num = 10 } 
													},
													#if RF_SYS_NUM >= 2
												    { { .const_request = ISO_REQUEST_ONLY_START,   .const_iso_att_back = ISO_BACK_VALUE, .const_delta_step5 = 8, .const_delta_step1 = 5, .t_discovery.const_cyc_max_num = 50, .t_discovery.const_discovery_min_num = 10 }, 
													  { .const_request = ISO_REQUEST_ALL_THE_TIME, .const_iso_att_back = ISO_BACK_VALUE, .const_delta_step5 = 8, .const_delta_step1 = 5, .t_discovery.const_cyc_max_num = 50, .t_discovery.const_discovery_min_num = 10 } 
													} 
													#endif
												  };

iso_resume_t t_iso_resume[RF_SYS_NUM] = {
										{ .const_retry_max_num = 3 },
										#if RF_SYS_NUM >= 2
										{ .const_retry_max_num = 3 }
										#endif
									};
							  

/*---------------------- 局部变量 --------------------------------------*/


/*---------------------- 声明 ------------------------------------------*/
extern void vRfGetData( void );                 //获取ADC采集数据
extern void vPowConvert( void );                //ADC转换为输出功率


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ HAL ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/





/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DRV ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/





/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ APP ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*****************************************************************************
* @Name	  : iso_init
* @Brief  : 隔离度参数初始化
* @Param  : None
* @Retval : 
* @Author : 
* @Data   : 2023-04-06
* @Note   :
*****************************************************************************/
void iso_init( void )
{
	ch_iso_para_t *pt_para = NULL;
	iso_resume_t  *pt_resume = NULL;
	
	uint8_t esys, ech;
	
	for( esys = 0; esys<RF_SYS_NUM; esys++ )
	{
		pt_resume = &t_iso_resume[esys];
		pt_resume->retry_num = 0;
		pt_resume->retry_sign = B_FALSE;
		pt_resume->iso_resumt_time_cnt = 0;
		
		for( ech = 0; ech<RF_CH_END; ech++ )
		{
			pt_para = &t_iso_para[esys][ech];
			
			pt_para->t_discovery.cyc_num = 0;
			pt_para->t_discovery.discovery_num = 0;
			pt_para->t_discovery.pow_last = -128;
			
			pt_para->pt_ch_para = &tRfChPara[esys][ech];
		}
	}
}


/*****************************************************************************
* @Name	  : iso_resume_time_cnt_clear
* @Brief  : 清空隔离度半小时计数器
* @Param  : esys: [输入/出] 
**			 ech: [输入/出] 
* @Retval : 
* @Author : 
* @Data   : 2023-04-06
* @Note   :
*****************************************************************************/
static void iso_resume_time_cnt_clear( uint8_t esys )
{
	t_iso_resume[esys].iso_resumt_time_cnt = 0;
}


/*****************************************************************************
* @Name	  : iso_resume_retry_sign_clear
* @Brief  : 清除隔离度尝恢复标识
* @Param  : esys: [输入/出] 
* @Retval : 
* @Author : 
* @Data   : 2023-04-07
* @Note   :
*****************************************************************************/
static void iso_resume_retry_sign_clear( uint8_t esys )
{
	t_iso_resume[esys].retry_sign = B_FALSE;
}


/*****************************************************************************
* @Name	  : iso_time_cnt_in_isr
* @Brief  : 隔离度计数器
* @Param  : None
* @Retval : 
* @Author : 
* @Data   : 2023-04-06
* @Note   : 
*****************************************************************************/
void iso_time_cnt_in_isr( void )
{
	#define ISO_RETRY_TIME	( MIN_TO_TICKS( 30 ) )	//30分计数
	
	uint8_t esys = 0;
	for( esys=0; esys<RF_SYS_NUM; esys++ )
	{
		if( t_iso_resume[esys].retry_num >= t_iso_resume[esys].const_retry_max_num )
		{
			t_iso_resume[esys].retry_sign = B_FALSE;
			return;
		}
		
		if( t_iso_resume[esys].iso_resumt_time_cnt <= ISO_RETRY_TIME )
		{
			t_iso_resume[esys].iso_resumt_time_cnt++;	
		}
		else
		{
			t_iso_resume[esys].retry_sign = B_TRUE;
		}
	}
}


/*****************************************************************************
* @Name	  : iso_set_rf_ch_state
* @Brief  : 设置指定的通道的射频参数状态
* @Param  : esys: [输入/出] 
**			 ech: [输入/出] 
**			 state: [输入/出] 
* @Retval : 
* @Author : 
* @Data   : 2023-04-07
* @Note   : 在 iso_set_rf_state() 中调用
*****************************************************************************/
static void iso_set_rf_ch_state( uint8_t esys, uint8_t ech, uint8_t state )
{
	EqIndependentPara_T *pt_para = t_iso_para[esys][ech].pt_ch_para;
	
	if( state == ENABLE )
	{
		pt_para->tAtt.lnaState = LNA_TO_RF;
		vChLnaSet( esys, ech, LNA_TO_RF );
		vChPaSet( esys, ech, ENABLE );
		
	}
	else
	{
		pt_para->tAtt.lnaState = LNA_TO_BYPASS;
		vChLnaSet( esys, ech, LNA_TO_BYPASS );
		vChPaSet( esys, ech, DISABLE );
	}
	
	vChAnalogAttSet( esys, ech, 1, pt_para->tAtt.constFirstAnalogAttMaxNum );
	vChAnalogAttSet( esys, ech, 2, pt_para->tAtt.constSecondAnalogAttMaxNum );
}


/*****************************************************************************
* @Name	  : iso_set_rf_state
* @Brief  : 设置隔离度检测时候的射频链路状态
* @Param  : esys: [输入/出] 
**			 ech: [输入/出] 
* @Retval : 
* @Author : 
* @Data   : 2023-04-06
* @Note   : 
*****************************************************************************/
static void iso_set_rf_state( uint8_t esys, uint8_t ech )
{
	if( (esys >= RF_SYS_NUM) || (ech >= RF_CH_END) )
		return;
	
	if( ech == RF_CH_UP )
	{
		iso_set_rf_ch_state( esys, RF_CH_UP, ENABLE );
		iso_set_rf_ch_state( esys, RF_CH_DW, DISABLE );
	}
	else
	{
		iso_set_rf_ch_state( esys, RF_CH_UP, DISABLE );
		iso_set_rf_ch_state( esys, RF_CH_DW, ENABLE );
	}
	
	cover_set_rf_switch_to_rpt();	//设置覆盖开关状态
}


/*****************************************************************************
* @Name	  : iso_set_att
* @Brief  : 
* @Param  : esys: [输入/出] 
*			ech: [输入/出] 
*			att: [输入/出] 
* @Retval : 
* @Author : 
* @Data   : 2023-04-06
* @Note   : 在隔离度检测设置衰减器时调用
*****************************************************************************/
static void iso_set_att( RF_SYS_E esys, RF_CH_E ech, uint8_t att )
{
	if( (esys >= RF_SYS_NUM) || (ech >= RF_CH_END) )
		return;
	
	Att_T *pt = &t_iso_para[esys][ech].pt_ch_para->tAtt;
	pt->ucDigitSum = att;
	
//	vAttSpliteAttToAttAndLna( pt );
	
	vChAtt( esys, ech );
	
	vTaskDelay( 150 );
	
	vRfGetData();
	vPowConvert();
}


/*****************************************************************************
* @Name	  : iso_quit_resume_rf_state
* @Brief  : 退出隔离度检测时恢复射频参数
* @Param  : esys: [输入/出] 
* @Retval : 
* @Author : 
* @Data   : 2023-04-07
* @Note   :
*****************************************************************************/
static void iso_quit_resume_rf_state( uint8_t esys )
{
	for( uint8_t ech=0; ech<RF_CH_END; ech++ )
	{
		iso_set_rf_ch_state( esys, ech, ENABLE );
		vAttSpliteAttToAttAndLna( &t_iso_para[esys][ech].pt_ch_para->tAtt );
	}
}


/*****************************************************************************
* @Name	  : iso_confirm
* @Brief  : 隔离度确认
* @Param  : esys: [输入/出] 
**			 ech: [输入/出] 
**			 att_num: 
* @Retval : ISO_RES_OK / ISO_RES_AE
* @Author : 
* @Data   : 2023-04-06
* @Note   :
*****************************************************************************/
static uint8_t iso_confirm( RF_SYS_E esys, RF_CH_E ech, uint8_t att_num, Pow_T *pt_pow, Att_T *pt_att, const uint8_t d_value )
{
    uint8_t point_normal, point_iso;	//检测点衰减器
	int8_t pow_normal, pow_iso;         //检测点功率
    uint8_t count = 0;       			//自激次数计数器
    uint8_t res = ISO_RES_OK; 			//自激标志，1: 自激, 0: 不自激
	
	bsp_printf( RTT_ISO, LOG_TEXT_CYAN"\n-----> L%d, %s - %s iso in confirm, att = %d...\r\n", __LINE__, rf_sys_name[esys], rf_ch_name[ech], att_num );
	
    point_normal = ((att_num + 2) > pt_att->constAttNum) ? pt_att->constAttNum : (att_num + 3);	//弱自激衰减点计算
    point_iso = (att_num > 0) ? (att_num - 1) : 0;		//强自激衰减点计算
	
    for(uint8_t i = 0; i < 10; i++)
    {
        iso_set_att( esys, ech, point_iso ); //设置自激点衰减
        pow_iso = pt_pow->scOutPow;
        
        iso_set_att( esys, ech, point_normal );    //非自激点衰减
        pow_normal = pt_pow->scOutPow;
        
        if( (pow_iso - pow_normal) >= d_value )
            count++;
		
		bsp_printf( RTT_ISO, LOG_TEXT_CYAN"-----> L%d, %s - %s iso in confirm, pow_normal = %d, pow_iso = %d, cyc = %d, count = %d...\r\n", __LINE__, rf_sys_name[esys], rf_ch_name[ech], pow_normal, pow_iso, i, count );
    }
    
    if(count >= 5)
        res = ISO_RES_AE; // 自激
    
    return res;	
}


/*****************************************************************************
* @Name	  : iso_pow_over_confirm
* @Brief  : 过功率确认
* @Param  : esys: [输入/出] 
**			 ech: [输入/出] 
* @Retval : ISO_RES_OK / ISO_RES_POWOVER
* @Author : 
* @Data   : 2023-04-06
* @Note   :
*****************************************************************************/
uint8_t iso_pow_over_confirm( uint8_t esys, uint8_t ech, Pow_T *pt_pow )
{
	uint8_t count = 0;
	
	for( uint8_t i=0; i<10; i++ )
	{
		vTaskDelay( 100 );
		if( pt_pow->scOutPow >= pt_pow->scTheoOutPow )
			count++;
	}
	
	if( count>= 8 )
		return ISO_RES_POWOVER;
	else
		return ISO_RES_OK;
}


/*****************************************************************************
* @Name	  : iso_set_default_value
* @Brief  : 不进行隔离度检测的通道设置默认值
* @Param  : esys: [输入/出] 
**			 ech: [输入/出] 
* @Retval : 
* @Author : 
* @Data   : 2023-05-25
* @Note   :
*****************************************************************************/
void iso_set_default_value( RF_SYS_E esys, RF_CH_E ech )
{
	ch_iso_para_t *pt_iso = &t_iso_para[esys][ech];
	
	pt_iso->pt_ch_para->ucIso = t_iso_para[esys][ech].pt_ch_para->tGain.scTheoreticalGain + 10;
	pt_iso->pt_ch_para->tAtt.ucIso = 0;
}

/*****************************************************************************
* @Name	  : iso_detection
* @Brief  : 隔离度检测
* @Param  : esys: [输入/出] 
**			 ech: [输入/出] 
* @Retval : 
* @Author : 
* @Data   : 2023-04-06
* @Note   :
*****************************************************************************/
void iso_detection( RF_SYS_E esys, RF_CH_E ech )
{
    static uint8_t att_num = 0;        //衰减器设置值
	static uint8_t att_min_value = 0;
    int8_t  pow_save = 0;
    int8_t  pow_new = 0;
	int8_t  d_value = 0;
	uint8_t res = ISO_RES_OK;
    uint8_t iso_value = 0;
	uint8_t iso_att = 0;

	ch_iso_para_t *pt_iso = &t_iso_para[esys][ech];
	Att_T *pt_att = &t_iso_para[esys][ech].pt_ch_para->tAtt;
	Pow_T *pt_pow = &t_iso_para[esys][ech].pt_ch_para->tPow;
	
	att_min_value = 0;
	
	iso_set_rf_state( esys, ech );
    iso_set_att( esys, ech, att_num );
	
	att_num = pt_att->constDigitAttNum + pt_att->constBypassAttNum;
	pow_new = pt_pow->scOutPow;
	pow_save = pow_new;
	
	//此处需要添加温度检测和温度补偿数据, 计算隔离度最小值, 需要减去当前的温度补偿数据
	
    bsp_printf( RTT_ISO, LOG_TEXT_MAGENTA"\n-----> L%d,%s - %s iso detection start...\r\n", __LINE__, rf_sys_name[esys], rf_ch_name[ech] );
	bsp_printf( RTT_ISO, "-----> L%d,%s - %s ATT_NUM = %d, THEO_POW = %d \r\n", __LINE__, rf_sys_name[esys], rf_ch_name[ech], att_num, pt_pow->scTheoOutPow );
	
	res = ISO_RES_OK;
    while( 1 )
    {
        iso_set_att( esys, ech, att_num );

		pow_new = pt_pow->scOutPow;
		d_value = ( pow_new >= pow_save ) ? (pow_new - pow_save ) : 0;
		
		bsp_printf( RTT_ISO, "-----> L%d,%s - %s iso_step_5: att = %d, new = %d, save = %d, d_value = %d \r\n",  \
									__LINE__, rf_sys_name[esys], rf_ch_name[ech], att_num, pow_new, pow_save, d_value );

        if( d_value >= pt_iso->const_delta_step5 ) //跳变满足门限
        {
            res = ISO_RES_AE;
            break;
        }

		if( pow_new > pt_pow->scTheoOutPow )
		{
			res = ISO_RES_POWOVER;
			break;
		}

		if( att_num == att_min_value )
		{
			res = ISO_RES_OK;
			break;
		}
		else
		{
			att_num = ( att_num >= 5 ) ? ( att_num - 5 ) : 0;
			if( att_num < att_min_value )
				att_num = att_min_value;
			pow_save = pow_new;
		}
    }
	
	if( res == ISO_RES_AE )	//检测到自激
	{
		att_num += 8;
		if( att_num >= pt_att->constAttNum )
			att_num = pt_att->constAttNum;
		
		iso_set_att( esys, ech, att_num );
		pow_new = pt_pow->scOutPow;
		
		bsp_printf( RTT_ISO, LOG_TEXT_YELLOW"\n-----> L%d,%s - %s iso detection by step1, start_att = %d, pow_new = %d, pow_save = %d\r\n", \
									__LINE__, rf_sys_name[esys], rf_ch_name[ech], att_num, pow_new, pow_save );
		
		res = ISO_RES_OK;
		while( 1 )
		{
			iso_set_att( esys, ech, att_num );

			pow_new = pt_pow->scOutPow;
			d_value = ( pow_new >= pow_save ) ? (pow_new - pow_save ) : 0;

			bsp_printf( RTT_ISO, LOG_TEXT_YELLOW"-----> L%d,%s - %s iso_step1: att = %d, new = %d, save = %d, d_value = %d \r\n", \
										__LINE__, rf_sys_name[esys], rf_ch_name[ech], att_num, pow_new, pow_save, d_value );

			if( d_value >= pt_iso->const_delta_step1 ) //跳变满足门限
			{
				res = iso_confirm( esys, ech, att_num, pt_pow, pt_att, 8 );
				if( res == ISO_RES_AE )
					break;
			}

			if( pow_new > pt_pow->scTheoOutPow )	//过功率
			{
				res = iso_pow_over_confirm( esys, ech, pt_pow );
				if( res == ISO_RES_POWOVER )
					break;
			}

			if( att_num == att_min_value )
			{
				res = ISO_RES_OK;
				break;
			}
			else
			{
				if( att_num > att_min_value )
					att_num--;
				
				pow_save = pow_new;
			}
		}
	}
	
	if( res == ISO_RES_OK )
	{
		iso_value = pt_iso->pt_ch_para->tGain.scTheoreticalGain + 10;
		iso_att = 0;
		bsp_printf( RTT_ISO, LOG_TEXT_GREEN"-----> L%d,%s - %s iso detection ok\n\n", __LINE__, rf_sys_name[esys], rf_ch_name[ech] );
	}
	else if( res == ISO_RES_AE )	//隔离度不足
	{
		iso_value = pt_iso->pt_ch_para->tGain.scTheoreticalGain - att_num + att_min_value;
		iso_att = att_num + pt_iso->const_iso_att_back - att_min_value;
		if( iso_att >= pt_att->constAttNum )
			iso_att = pt_att->constAttNum;
		
		bsp_printf( RTT_ISO, LOG_TEXT_RED"-----> L%d,%s - %s iso detection end: iso = %d, iso_att = %d \n", __LINE__, rf_sys_name[esys], rf_ch_name[ech], iso_value, iso_att );
	}
	else if( res == ISO_RES_POWOVER )	//过功率
	{	
		iso_value = pt_iso->pt_ch_para->tGain.scTheoreticalGain + 5;
		pt_att->ucAlc = att_num + 5;
		bsp_printf( RTT_ISO, LOG_TEXT_YELLOW"-----> L%d,%s - %s iso detection end: pow over, att = %d \r\n", __LINE__, rf_sys_name[esys], rf_ch_name[ech], att_num );
	}
	else
	{
		iso_value = pt_iso->pt_ch_para->tGain.scTheoreticalGain + 5;
		iso_att = 0;
		bsp_printf( RTT_ISO, LOG_TEXT_RED"[ ERROR ]: -----> L%d,%s - %s SOFTWARE ERROR!!!\r\n", __LINE__, rf_sys_name[esys], rf_ch_name[ech] );
	}
	
	pt_iso->pt_ch_para->tAtt.ucIso = iso_att;	//由隔离度引起的衰减
	pt_iso->pt_ch_para->ucIso = iso_value;
	
	iso_set_att( esys, ech, pt_iso->pt_ch_para->tAtt.ucIso );
}

/*****************************************************************************
* @Name	  : iso_handle
* @Brief  : 隔离度检测后的数据处理
* @Param  : esys: [输入/出] 
* @Retval : 
* @Author : 
* @Data   : 2023-04-06
* @Note   :
*****************************************************************************/
void iso_handle( uint8_t esys )
{
	uint8_t *pt_up_att = &t_iso_para[esys][RF_CH_UP].pt_ch_para->tAtt.ucIso;
	uint8_t *pt_dw_att = &t_iso_para[esys][RF_CH_DW].pt_ch_para->tAtt.ucIso;
	
	if( *pt_up_att > *pt_dw_att )
		*pt_dw_att = *pt_up_att;
	else
		*pt_up_att = *pt_dw_att;
}



/*****************************************************************************
* @Name	  : iso_try_resume
* @Brief  : 隔离度尝试恢复
* @Param  : esys: [输入/出] 
* @Retval : 
* @Author : 
* @Data   : 2023-04-07
* @Note   : 只在下行
*****************************************************************************/
void iso_try_resume( uint8_t esys, uint8_t ech )
{
	uint8_t alc_value = t_iso_para[esys][RF_CH_DW].pt_ch_para->tAtt.ucIso;
	ch_iso_discovery_t *pt = &t_iso_para[esys][RF_CH_DW].t_discovery;
	
	if( RF_CH_UP == ech )
		return;
	
	if( alc_value == 0 )
	{
		iso_resume_time_cnt_clear( esys );
		iso_resume_retry_sign_clear( esys );
		return;
	}
	
	if( t_iso_resume[esys].retry_sign == B_TRUE )
	{
		iso_resume_retry_sign_clear( esys );
		t_iso_resume[esys].retry_num++;
		pt->cyc_num = pt->const_cyc_max_num;		//设置重检隔离度标志.
		pt->discovery_num = pt->const_cyc_max_num;
	}	
}

/*****************************************************************************
* @Name	  : iso_discovery_data_clear
* @Brief  : 清除隔离度发现所需的数据
* @Param  : esys: [输入/出] 
* @Retval : 
* @Author : 
* @Data   : 2023-04-06
* @Note   :
*****************************************************************************/
static void iso_discovery_data_clear( uint8_t esys, uint8_t ech )
{
	ch_iso_discovery_t *pt = &t_iso_para[esys][ech].t_discovery;

	pt->cyc_num = 0;
	pt->discovery_num = 0;
	pt->pow_last = t_iso_para[esys][ech].pt_ch_para->tPow.scOutPow;
}

/*****************************************************************************
* @Name	  : iso_discovery
* @Brief  : 隔离度发现判断
* @Param  : esys: [输入/出] 
**			 ech: [输入/出] 
* @Retval : 
* @Author : 
* @Data   : 2023-04-06
* @Note   :
*****************************************************************************/
static void iso_discovery( RF_SYS_E esys, RF_CH_E ech )
{
	int8_t pow_new = 0;
	
	ch_iso_discovery_t *pt = &t_iso_para[esys][ech].t_discovery;
	Pow_T *pt_pow = &t_iso_para[esys][ech].pt_ch_para->tPow;
	
	pt->cyc_num++;
	pow_new = pt_pow->scOutPow;
	
	bsp_printf( RTT_ISO, "---->L%d %s - %s iso_cyc = %2d, dis_num = %2d, pow_new = %-3d, pow_last = %-3d\n", __LINE__, rf_sys_name[esys], rf_ch_name[ech], \
																					pt->cyc_num, pt->discovery_num, pow_new, pt->pow_last );		
	
	if( (( pow_new > pt->pow_last ) && ( pow_new - pt->pow_last >= 8 )) || 
					(( pt->pow_last > pow_new ) && ( pt->pow_last - pow_new >= 8 )) )
	{
		pt->discovery_num++;
		pt->pow_last = pow_new;
	}
	else 
		pt->pow_last = pow_new;
	
	if( pt->cyc_num >= pt->const_cyc_max_num )
	{
		if( pt->discovery_num >= pt->const_discovery_min_num )	//无论UP/DW哪条链路发现自激, 均重新检测上下行链路自激
		{
			iso_detection( esys, RF_CH_UP );
			iso_detection( esys, RF_CH_DW );
			iso_handle( esys );
			iso_resume_time_cnt_clear( esys );
			iso_quit_resume_rf_state( esys );
		}
		iso_discovery_data_clear( esys, RF_CH_UP );
		iso_discovery_data_clear( esys, RF_CH_DW );
		
		if( ech == RF_CH_DW )
			bsp_printf( RTT_ISO, LOG_TEXT_YELLOW"---->L%d %s - %s iso_resume_retry_num = %d, time: %d\n, ", __LINE__, rf_sys_name[esys], rf_ch_name[ech], t_iso_resume[esys].retry_num, t_iso_resume[esys].iso_resumt_time_cnt/10 );
	}
}


/*****************************************************************************
* @Name	  : iso_induce
* @Brief  : 隔离度诱导程序
* @Param  : esys: [输入/出] 
**			 ech: [输入/出] 
* @Retval : 
* @Author : 
* @Data   : 2023-04-06
* @Note   :
*****************************************************************************/
void iso_induce( uint8_t esys, uint8_t ech )
{
	
}


/*****************************************************************************
* @Name	  : iso_flow_open
* @Brief  : 开机隔离度检测
* @Param  : None
* @Retval : 
* @Author : 
* @Data   : 2023-04-06
* @Note   :
*****************************************************************************/
void iso_flow_open( void )
{
	uint8_t const *pt_request = NULL;
	
	RF_SYS_E esys;
	RF_CH_E  ech;
	
	for( esys=RF_SYS_1; esys<RF_SYS_NUM; esys++ )
	{
		for( ech=RF_CH_UP; ech<RF_CH_END; ech++ )
		{
			pt_request = &t_iso_para[esys][ech].const_request;
			
			if( DISABLE == t_iso_para[esys][ech].pt_ch_para->ucIsoEn )
			{
				iso_set_default_value( esys, ech );
				continue;
			}
			
			if(( *pt_request == ISO_REQUEST_ONLY_START ) || ( *pt_request == ISO_REQUEST_ALL_THE_TIME ) )
			{
				iso_detection( esys, ech );
			}
			else
			{
				iso_set_default_value( esys, ech );
			}
		}
		iso_handle( esys );
		iso_quit_resume_rf_state( esys );
	}
}


/*****************************************************************************
* @Name	  : iso_flow_process
* @Brief  : 过程隔离度检测
* @Param  : esys: [输入/出] 
**			 ech: [输入/出]  
* @Retval : 
* @Author : 
* @Data   : 2023-04-06
* @Note   :
*****************************************************************************/
void iso_flow_process( RF_SYS_E esys, RF_CH_E ech )
{
	uint8_t const *pt_request = &t_iso_para[esys][ech].const_request;
	
	if( DISABLE == t_iso_para[esys][ech].pt_ch_para->ucIsoEn )
	{
		iso_set_default_value( esys, ech );
		iso_discovery_data_clear( esys, ech );
		return;
	}
	
	if(( *pt_request == ISO_REQUEST_ONLY_PROCESS ) || ( *pt_request == ISO_REQUEST_ALL_THE_TIME ) )
	{
		iso_try_resume( esys, ech );
		iso_discovery( esys, ech );
	}
	else
	{
		iso_discovery_data_clear( esys, ech );
	}
}

/*****************************************************************************
* @Name	  : get_iso_value
* @Brief  : 获取隔离度值
* @Param  : esys: [输入/出] 
* @Retval : 
* @Author : 
* @Data   : 2023-04-17
* @Note   :
*****************************************************************************/
uint8_t get_iso_value( RF_SYS_E esys )
{
	if( tRfChPara[esys][RF_CH_DW].ucIso >= tRfChPara[esys][RF_CH_UP].ucIso )
		return tRfChPara[esys][RF_CH_UP].ucIso;
	else
		return tRfChPara[esys][RF_CH_DW].ucIso;
}
