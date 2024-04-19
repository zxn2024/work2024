/************************************************************************
* Copyright (c) 2024 Shaanxi Tianji Communication Technologies Co., Ltd.
* 
* @file     bsp_warn_user.c
* @author   ZhangWei
* @version  V1.0
* @date     2024-02-06
* @brief    [描述]
* 			获取数据 -> 判断告警状态 -> 上报告警 -> 清除告警标志 -> 告警状态保存到FLASH
************************************************************************/

#include "stdint.h"
#include "public_enum.h"
#include "FreeRTOS.h"
#include "task.h"
#include "bsp_config.h"
#include "crc.h"
#include "bsp_flash_addr.h"
#include "bsp_rf_global.h"
#include "bsp_warn_user.h"
#include "ru_device_list.h"
#include "rtt.h"

// #define SAVE_WARN_STATE_EN	( 1 )		// 1:保存告警状态 0:不保存告警状态
// #define WARN_SLV_DEV_MAX_NUM    ( 8 )		// 最大从机数量
// typedef struct
// {
// 	uint8_t tdd_or_fdd;
// 	int8_t  ch1_dw_in_pow;
// 	int8_t  ch2_dw_in_pow;
// 	int8_t  ch3_dw_in_pow;
// 	uint8_t iso;
	
// 	ptWarn_T t_warn_ch1_dw_inpow_greater;
// 	ptWarn_T t_warn_ch1_dw_inpow_less;
// 	ptWarn_T t_warn_ch2_dw_inpow_greater;
// 	ptWarn_T t_warn_ch2_dw_inpow_less;
// 	ptWarn_T t_warn_ch3_dw_inpow_greater;
// 	ptWarn_T t_warn_ch3_dw_inpow_less;
// 	ptWarn_T t_warn_iso;
// }dev_warn_para_t;


// typedef struct
// {
// 	uint8_t up_inpow_greater;		//上行输入过功率告警
// 	uint8_t up_inpow_less;			
// 	uint8_t up_outpow_greater;
// 	uint8_t up_outpow_less;
// 	uint8_t up_iso;
	
// 	uint8_t dw_inpow_greater;
// 	uint8_t dw_inpow_less;
// 	uint8_t dw_outpow_greater;
// 	uint8_t dw_outpow_less;
// 	uint8_t dw_iso;
	
// 	uint8_t node_lose;				//节点丢失告警
// 	uint8_t tmp_greater;			//过功率告警
// 	uint8_t reserve1;
// 	uint8_t reserve2;
// 	uint8_t reserve3;
// 	uint8_t reserve4;
// }dev_warn_save_t;

uint8_t           dev_num;			//从机数量
uint8_t           warn_save_en = ENABLE;		//保存标志, B_ENABLE: 允许, B_DISABLE: 禁止
dev_warn_para_t   t_dev_warn[ WARN_SLV_DEV_MAX_NUM + 1 ];	//计算告警需要用到的参数, 0: 主机, 1~8: 从机, 下标代表监控从设备编号
dev_warn_save_t   t_warn_save[ WARN_SLV_DEV_MAX_NUM + 1 ];	//保存主从机的告警参数, 0:主机, 1:从机

uint8_t           warn_rpt_pdu[64];	//告警报文参数, 最多16个告警, 返回报文有效的字符串长度

static uint8_t    save_flag;		//存储标志, 1: 存储, 0: 不存储

/**
* @brief   获取设备告警参数    
* @param   dev_id: 设备编号
* @return    
* @data    2024-02-20
* @note    
*/
static void get_dev_warn_para( uint8_t dev_id )
{
	warn_save_en = ENABLE;
	//warn_save_en = tDevPublicPara.ucWarnSaveEn;
	
	for( uint8_t i=1; i<WARN_SLV_DEV_MAX_NUM; i++ )
	{
		dev_warn_para_t *pt = &t_dev_warn[ i ];
        pt->tdd_or_fdd = t_rf_sys[0].t_public.SysMode;
	}
	//获取主机参数和从机参数, 以及从机数量
}


/************************************************************************
* @brief   初始化
* @param   None
* @return    
* @data    2024-02-22
* @note    在从flash读取初始化参数完成之后,再调用本函数完成告警初始化
************************************************************************/
void dev_warn_init( void )
{
	dev_warn_para_t *pt = &t_dev_warn[0];

    pt->dev_id = 0;
	vWarnInit( &pt->t_warn_ch1_dw_inpow_greater,  127,  WARN_TYPE_GREATER, 180, 180 );
	vWarnInit( &pt->t_warn_ch1_dw_inpow_less,    -128,  WARN_TYPE_LESS,    180, 180 );
	vWarnInit( &pt->t_warn_ch2_dw_inpow_greater,  127,  WARN_TYPE_GREATER, 180, 180 );
	vWarnInit( &pt->t_warn_ch2_dw_inpow_less,    -128,  WARN_TYPE_LESS,    180, 180 );
	vWarnInit( &pt->t_warn_ch3_dw_inpow_greater,  127,  WARN_TYPE_GREATER, 180, 180 );
	vWarnInit( &pt->t_warn_ch3_dw_inpow_less,    -128,  WARN_TYPE_LESS,    180, 180 );
	vWarnInit( &pt->t_warn_iso, 				   30,  WARN_TYPE_EQUAL,   180, 180 );
	
	for( uint8_t i=1; i<WARN_SLV_DEV_MAX_NUM; i++ )
	{
		dev_warn_para_t *pt = &t_dev_warn[ i ];
        pt->dev_id = i;
		vWarnInit( &pt->t_warn_ch1_dw_inpow_greater,  127,  WARN_TYPE_GREATER, 180, 180 );
		vWarnInit( &pt->t_warn_ch1_dw_inpow_less,    -128,  WARN_TYPE_LESS,    180, 180 );
		vWarnInit( &pt->t_warn_ch2_dw_inpow_greater,  127,  WARN_TYPE_GREATER, 180, 180 );
		vWarnInit( &pt->t_warn_ch2_dw_inpow_less,    -128,  WARN_TYPE_LESS,    180, 180 );
		vWarnInit( &pt->t_warn_ch3_dw_inpow_greater,  127,  WARN_TYPE_GREATER, 180, 180 );
		vWarnInit( &pt->t_warn_ch3_dw_inpow_less,    -128,  WARN_TYPE_LESS,    180, 180 );
		vWarnInit( &pt->t_warn_iso, 				   30,  WARN_TYPE_EQUAL,   180, 180 );
	}

    dev_warn_state_read_from_flash();
}


/**
* @brief   计算单个设备的告警状态
* @param   dev_id: [输入/出] 
* @return    
* @data    2024-02-20
* @note    
*/
void dev_warn_state_calc( uint8_t dev_id )
{
    pt_ru_device_t pt_ru_dev = NULL;

	if( dev_id >= WARN_SLV_DEV_MAX_NUM )
		return;
	
	get_dev_warn_para( dev_id );
	dev_warn_para_t *pt = &t_dev_warn[ dev_id ];

    if(dev_id == 0) //TODO 根据设备工作状态判断是否需要计算告警
    {
        pt->ch1_dw_in_pow = t_rf_sys[ RF_SYS_1 ].t_ch[RF_CH_DW].tPow.scInPow;
        pt->ch2_dw_in_pow = t_rf_sys[ RF_SYS_2 ].t_ch[RF_CH_DW].tPow.scInPow;
        pt->ch3_dw_in_pow = t_rf_sys[ RF_SYS_3 ].t_ch[RF_CH_DW].tPow.scInPow;
        //TODO: 获取ISO参数
    }else 
    {
        pt_ru_dev =  get_ru_device_ptr_by_monitor_dev_id(dev_id);
        //从机设备id在连接中,且类型为RU,否则不进行告警计数
        if(pt_ru_dev == NULL || pt_ru_dev->pt_slave_conn->is_dev_connected == B_FALSE || pt_ru_dev->dev_type != DEV_TYPE_RU)
            return;

        pt->ch1_dw_in_pow = pt_ru_dev->pt_mrc_ru_data->mrc_lte_in_pwr;
        pt->ch2_dw_in_pow = pt_ru_dev->pt_mrc_ru_data->mrc_nr_in_pwr;
        pt->ch3_dw_in_pow = pt_ru_dev->pt_mrc_ru_data->mrc_nr2_in_pwr;
        pt->iso = pt_ru_dev->pt_mrc_ru_data->mrc_self_excited;
    }

    warnCheck( &pt->t_warn_ch1_dw_inpow_greater, pt->ch1_dw_in_pow );
    warnCheck( &pt->t_warn_ch1_dw_inpow_less,    pt->ch1_dw_in_pow );
    warnCheck( &pt->t_warn_ch2_dw_inpow_greater, pt->ch2_dw_in_pow );
    warnCheck( &pt->t_warn_ch2_dw_inpow_less,    pt->ch2_dw_in_pow );
    warnCheck( &pt->t_warn_ch3_dw_inpow_greater, pt->ch3_dw_in_pow );
    warnCheck( &pt->t_warn_ch3_dw_inpow_less,    pt->ch3_dw_in_pow );
    warnCheck( &pt->t_warn_iso, 				    pt->iso        );

}

/**
* @brief   单一告警报文数据整理  
* @param   oid: [输入/出] 
*		   tag: [输入/出] 
*		   state: [输入/出] 
* @return    
* @data    2024-02-22
* @note    
*/
static void dev_warn_pdu_set( uint16_t oid, uint8_t *tag, WARN_STATE_E state )
{
	*tag = 0x04;
	*(tag+1) = UINT16_L( oid );
	*(tag+2) = UINT16_H( oid );
	*(tag+3) = state;
}

/************************************************************************
* @brief   向omc上报告警参数   
* @param   dev_id: 设备编号
* @return  告警字符串长度  
* @data    2024-02-20
* @note    
************************************************************************/
uint8_t dev_warn_state_rpt( uint8_t dev_id )
{
	if( dev_id >= WARN_SLV_DEV_MAX_NUM )
		return 0;
	
	memset( warn_rpt_pdu, 0, sizeof( warn_rpt_pdu ) );
	
	dev_warn_para_t *pt = &t_dev_warn[ dev_id ];
	
	uint8_t i = 0;
	
	if( pt->tdd_or_fdd == IS_TDD )
	{
		//TDD 下行输入过功率告警 0x03A0
		//TDD 下行输入欠功率告警 0x03A1
		if( RPT_WARN == vWarnGetEvt( &pt->t_warn_ch1_dw_inpow_greater ) || RPT_QUIT == vWarnGetEvt( &pt->t_warn_ch1_dw_inpow_greater ) )
		{
			dev_warn_pdu_set( 0x03A0, &warn_rpt_pdu[i*4], vWarnGetStat( &pt->t_warn_ch1_dw_inpow_greater ) );
			i++;
		}
		else if( RPT_WARN == vWarnGetEvt( &pt->t_warn_ch1_dw_inpow_less ) || RPT_QUIT == vWarnGetEvt( &pt->t_warn_ch1_dw_inpow_less ) )
		{
			dev_warn_pdu_set( 0x03A0, &warn_rpt_pdu[i*4], NORMAL );
			i++;
		}
		
		//TDD模式下, FDD告警消除
		dev_warn_pdu_set( 0x03A2, &warn_rpt_pdu[i*4], NORMAL );
		i++;
		dev_warn_pdu_set( 0x03A3, &warn_rpt_pdu[i*4], NORMAL );
		i++;
	}
	else
	{
		//FDD模式下, TDD告警消除
		dev_warn_pdu_set( 0x03A0, &warn_rpt_pdu[i*4], NORMAL );
		i++;
		dev_warn_pdu_set( 0x03A1, &warn_rpt_pdu[i*4], NORMAL );
		i++;
		
		//FDD 下行输入过功率告警 0x03A2
		//FDD 下行输入欠功率告警 0x03A3
		if( RPT_WARN == vWarnGetEvt( &pt->t_warn_ch1_dw_inpow_greater )  || RPT_QUIT == vWarnGetEvt( &pt->t_warn_ch1_dw_inpow_greater ) )
		{
			dev_warn_pdu_set( 0x03A2, &warn_rpt_pdu[i*4], vWarnGetStat( &pt->t_warn_ch1_dw_inpow_greater ) );
			i++;
		}
		else if( RPT_WARN == vWarnGetEvt( &pt->t_warn_ch1_dw_inpow_less )  || RPT_QUIT == vWarnGetEvt( &pt->t_warn_ch1_dw_inpow_less ) )
		{
			dev_warn_pdu_set( 0x03A3, &warn_rpt_pdu[(i+1)*4], vWarnGetStat( &pt->t_warn_ch1_dw_inpow_less ) );
			i++;
		}
	}
	
	//CH2 下行输入过功率告警 0x0330
	//CH2 下行输入过功率告警 0x0331
	if( RPT_WARN == vWarnGetEvt( &pt->t_warn_ch2_dw_inpow_greater )  || RPT_QUIT == vWarnGetEvt( &pt->t_warn_ch2_dw_inpow_greater ) )
	{
		dev_warn_pdu_set( 0x0330, &warn_rpt_pdu[i*4], vWarnGetStat( &pt->t_warn_ch2_dw_inpow_greater ) );
		i++;
	}
	else if( RPT_WARN == vWarnGetEvt( &pt->t_warn_ch2_dw_inpow_less )  || RPT_QUIT == vWarnGetEvt( &pt->t_warn_ch2_dw_inpow_less ) )
	{
		dev_warn_pdu_set( 0x0331, &warn_rpt_pdu[(i+1)*4], vWarnGetStat( &pt->t_warn_ch2_dw_inpow_less ) );
		i++;
	}

	//CH3 下行输入过功率告警 0x0336
	//CH3 下行输入过功率告警 0x0337
	if( RPT_WARN == vWarnGetEvt( &pt->t_warn_ch3_dw_inpow_greater )  || RPT_QUIT == vWarnGetEvt( &pt->t_warn_ch3_dw_inpow_greater ) )
	{
		dev_warn_pdu_set( 0x0336, &warn_rpt_pdu[i*4], vWarnGetStat( &pt->t_warn_ch3_dw_inpow_greater ) );
		i++;
	}
	else if( RPT_WARN == vWarnGetEvt( &pt->t_warn_ch3_dw_inpow_less )  || RPT_QUIT == vWarnGetEvt( &pt->t_warn_ch3_dw_inpow_less ))
	{
		dev_warn_pdu_set( 0x0337, &warn_rpt_pdu[(i+1)*4], vWarnGetStat( &pt->t_warn_ch3_dw_inpow_less ) );
		i++;
	}
	
	//隔离度告警
	if( RPT_WARN == vWarnGetEvt( &pt->t_warn_iso )  || RPT_QUIT == vWarnGetEvt( &pt->t_warn_iso ) )
	{
		dev_warn_pdu_set( 0x0329, &warn_rpt_pdu[i*4], vWarnGetStat( &pt->t_warn_iso ) );
		i++;
	}
	
	return i*4;
}

/************************************************************************
* @brief    [描述]
* @param    dev_id         设备编号
* @param    tag            数据存储首地址
************************************************************************/
uint8_t dev_warn_copy_warn_msg( uint8_t dev_id, uint8_t *tag )
{
	uint8_t len = dev_warn_state_rpt( dev_id );
	memcpy( tag, warn_rpt_pdu, len);
	return len;
}

/************************************************************************
* @brief    获取是否有告警需要上报
* @param    dev_id  设备ID
* @return   BOOL_E  B_TRUE:需要上报,  B_FALSE: 不需要上报
************************************************************************/
BOOL_E dev_warn_rpt_flag( uint8_t dev_id )
{
	if( dev_id >= WARN_SLV_DEV_MAX_NUM )
		return B_FALSE;
	
	dev_warn_para_t *pt = &t_dev_warn[ dev_id ];

	if( ( RPT_WARN == vWarnGetEvt( &pt->t_warn_ch1_dw_inpow_greater )) || ( RPT_WARN == vWarnGetEvt( &pt->t_warn_ch1_dw_inpow_less )) ||
		( RPT_WARN == vWarnGetEvt( &pt->t_warn_ch2_dw_inpow_greater )) || ( RPT_WARN == vWarnGetEvt( &pt->t_warn_ch2_dw_inpow_less )) ||
		( RPT_WARN == vWarnGetEvt( &pt->t_warn_ch3_dw_inpow_greater )) || ( RPT_WARN == vWarnGetEvt( &pt->t_warn_ch3_dw_inpow_less )) ||
		( RPT_WARN == vWarnGetEvt( &pt->t_warn_iso )) || ( RPT_QUIT == vWarnGetEvt( &pt->t_warn_ch1_dw_inpow_greater )) || 
		( RPT_QUIT == vWarnGetEvt( &pt->t_warn_ch1_dw_inpow_less )) || ( RPT_QUIT == vWarnGetEvt( &pt->t_warn_ch2_dw_inpow_greater )) ||
		( RPT_QUIT == vWarnGetEvt( &pt->t_warn_ch2_dw_inpow_less )) || ( RPT_QUIT == vWarnGetEvt( &pt->t_warn_ch3_dw_inpow_greater )) ||
		( RPT_QUIT == vWarnGetEvt( &pt->t_warn_ch3_dw_inpow_less )) || ( RPT_QUIT == vWarnGetEvt( &pt->t_warn_iso )) )
			return B_TRUE;
	else
		return B_FALSE;
}

/************************************************************************
* @brief   上报之后清除告警事件状态
* @param   dev_id: [输入/出] 
* @return    
* @data    2024-02-22
* @note    
************************************************************************/
void dev_clr_warn_state_after_rpt( uint8_t dev_id )
{
	if( dev_id >= WARN_SLV_DEV_MAX_NUM )
		return;
	
	dev_warn_para_t *pt = &t_dev_warn[ dev_id ];
	
	warnClearOmcRptEvt( &pt->t_warn_ch1_dw_inpow_greater );
	warnClearOmcRptEvt( &pt->t_warn_ch1_dw_inpow_less    );
	warnClearOmcRptEvt( &pt->t_warn_ch2_dw_inpow_greater );
	warnClearOmcRptEvt( &pt->t_warn_ch2_dw_inpow_less    );
	warnClearOmcRptEvt( &pt->t_warn_ch3_dw_inpow_greater );
	warnClearOmcRptEvt( &pt->t_warn_ch3_dw_inpow_less    );
	warnClearOmcRptEvt( &pt->t_warn_iso					);	
}

/************************************************************************
* @brief   上报成功之后清除告警状态
* @param   dev_id: [输入/出] 
* @return    
* @data    2024-02-22
* @note    
************************************************************************/
void dev_clr_warn_state_after_rpt_success( uint8_t dev_id )
{
	if( dev_id >= WARN_SLV_DEV_MAX_NUM )
		return;
	
	dev_warn_para_t *pt = &t_dev_warn[ dev_id ];
	
	warnOmcRptSuccessed( &pt->t_warn_ch1_dw_inpow_greater );
	warnOmcRptSuccessed( &pt->t_warn_ch1_dw_inpow_less    );
	warnOmcRptSuccessed( &pt->t_warn_ch2_dw_inpow_greater );
	warnOmcRptSuccessed( &pt->t_warn_ch2_dw_inpow_less    );
	warnOmcRptSuccessed( &pt->t_warn_ch3_dw_inpow_greater );
	warnOmcRptSuccessed( &pt->t_warn_ch3_dw_inpow_less    );
	warnOmcRptSuccessed( &pt->t_warn_iso				  );

    rtt_printf(RTT_MONITOR, RTT_CTRL_TEXT_BRIGHT_YELLOW "Report warn Response : dev %d \r\n", dev_id);

	if( ENABLE == warn_save_en )
		save_flag = 1;
	else
		save_flag = 0;
}

/************************************************************************
* @brief      
* @param   None
* @return    
* @data    2024-02-28
* @note    
************************************************************************/
static void  dev_warn_state_data_clr( void )
{
	memset( t_warn_save, NORMAL, sizeof( dev_warn_save_t )*(WARN_SLV_DEV_MAX_NUM + 1) );
}

/************************************************************************
* @brief   整理告警数据格式, 准备将数据写入flash
************************************************************************/
static void  dev_warn_state_data_alignment( void )
{
	dev_warn_state_data_clr();
	
	dev_warn_save_t *pt_warn_mu = &t_warn_save[0];
    dev_warn_save_t *pt_warn_ru = NULL;
    //pt_ru_device_t pt_ru_dev = NULL;
	
	pt_warn_mu->t_warn_ch1_dw_inpow_greater = t_dev_warn[0].t_warn_ch1_dw_inpow_greater.ucStateOriginal;
	pt_warn_mu->t_warn_ch1_dw_inpow_less    = t_dev_warn[0].t_warn_ch1_dw_inpow_less.ucStateOriginal;
	pt_warn_mu->t_warn_ch2_dw_inpow_greater = t_dev_warn[0].t_warn_ch2_dw_inpow_greater.ucStateOriginal;
	pt_warn_mu->t_warn_ch2_dw_inpow_less    = t_dev_warn[0].t_warn_ch2_dw_inpow_less.ucStateOriginal;
	pt_warn_mu->t_warn_ch3_dw_inpow_greater = t_dev_warn[0].t_warn_ch3_dw_inpow_greater.ucStateOriginal;
	pt_warn_mu->t_warn_ch3_dw_inpow_less    = t_dev_warn[0].t_warn_ch3_dw_inpow_less.ucStateOriginal;
	pt_warn_mu->t_warn_iso				    = t_dev_warn[0].t_warn_iso.ucStateOriginal;
	
	for( uint8_t i=1; i<WARN_SLV_DEV_MAX_NUM; i++ )
	{
        // pt_ru_dev =  get_ru_device_ptr_by_monitor_dev_id(i);
        // //存在从机设备且类型为RU,否则不处理
        // if(pt_ru_dev == NULL || pt_ru_dev->dev_type != DEV_TYPE_RU)
        //     continue;
		pt_warn_ru = &t_warn_save[i];

        pt_warn_ru->t_warn_ch1_dw_inpow_greater = t_dev_warn[i].t_warn_ch1_dw_inpow_greater.ucStateOriginal;
        pt_warn_ru->t_warn_ch1_dw_inpow_less    = t_dev_warn[i].t_warn_ch1_dw_inpow_less.ucStateOriginal;
        pt_warn_ru->t_warn_ch2_dw_inpow_greater = t_dev_warn[i].t_warn_ch2_dw_inpow_greater.ucStateOriginal;
        pt_warn_ru->t_warn_ch2_dw_inpow_less    = t_dev_warn[i].t_warn_ch2_dw_inpow_less.ucStateOriginal;
        pt_warn_ru->t_warn_ch3_dw_inpow_greater = t_dev_warn[i].t_warn_ch3_dw_inpow_greater.ucStateOriginal;
        pt_warn_ru->t_warn_ch3_dw_inpow_less    = t_dev_warn[i].t_warn_ch3_dw_inpow_less.ucStateOriginal;
        pt_warn_ru->t_warn_iso				    = t_dev_warn[i].t_warn_iso.ucStateOriginal;
	}
}

/************************************************************************
* @brief   将告警状态写入FLASH保存
* @param   None
* @return    
* @data    2024-02-20
* @note    
************************************************************************/
void dev_warn_state_save_in_flash( void )
{
    uint32_t addr = get_dev_warn_state_save_addr();
    uint16_t len  = sizeof(dev_warn_save_t ) * ( WARN_SLV_DEV_MAX_NUM + 1);

    uint16_t flash_sector = spi_flash.flash_addr_to_sector(addr);        //bi_flash_addr_to_sector( addr );	//flash 扇区地址

	uint8_t err = 0;
	uint8_t result = B_TRUE;
	dev_warn_save_t   t_warn_save_read[ WARN_SLV_DEV_MAX_NUM + 1 ];
	uint8_t *pt_w = (uint8_t *)&t_warn_save[0];
	uint8_t *pt_r = (uint8_t *)&t_warn_save_read[0];
	
	if( save_flag )
	{
		dev_warn_state_data_alignment();

        while(err < 3)
        {
            taskENTER_CRITICAL();
            spi_flash.flash_erase_sector(flash_sector);
            spi_flash.flash_write((uint8_t *)pt_w, addr, len);  //写入数据
            spi_flash.flash_read((uint8_t *)pt_r, addr, len);   //读出数据
            taskEXIT_CRITICAL();
			
			for( uint8_t i=0; i<len; i++ )
			{
				if( *(pt_w+i) != *(pt_r + i) )
				{
					result = B_FALSE;
					break;
				}
			}
			
			if( B_FALSE == result )
				err++;
			else
				break;
        }
        rtt_printf(RTT_MONITOR, RTT_TEXT_YELLOW"Warn save\r\n");
    }
	
	save_flag = 0;
}

WARN_STATE_E get_valid_warn_stat(uint8_t stat)
{
    if(stat == NORMAL || stat == WARN)
        return (WARN_STATE_E)stat;
    else
        return NORMAL;
}

/************************************************************************
* @brief   从FLASH中读取告警状态
* @param   None
* @return    
* @data    2024-02-20
* @note    
************************************************************************/
void dev_warn_state_read_from_flash( void )
{
    uint32_t addr = get_dev_warn_state_save_addr();
    uint16_t len  = sizeof(dev_warn_save_t ) * ( WARN_SLV_DEV_MAX_NUM + 1);

    uint16_t flash_sector = spi_flash.flash_addr_to_sector(addr);        //bi_flash_addr_to_sector( addr );	//flash 扇区地址

	uint8_t *pt_r = (uint8_t *)&t_warn_save[0];
    dev_warn_save_t *pt_warn_mu = &t_warn_save[0];
    dev_warn_save_t *pt_warn_ru = NULL;
	
	dev_warn_state_data_clr();	//清除所有的告警数据
	
	if( ENABLE == warn_save_en )
	{
		taskENTER_CRITICAL();
		spi_flash.flash_read((uint8_t *)pt_r, addr, len);   //读出数据
		taskEXIT_CRITICAL();
		
        //pt_ru_device_t pt_ru_dev = NULL;
        t_dev_warn[0].t_warn_ch1_dw_inpow_greater.ucStateOriginal = get_valid_warn_stat(pt_warn_mu->t_warn_ch1_dw_inpow_greater);
        t_dev_warn[0].t_warn_ch1_dw_inpow_less.ucStateOriginal = get_valid_warn_stat(pt_warn_mu->t_warn_ch1_dw_inpow_less);
        t_dev_warn[0].t_warn_ch2_dw_inpow_greater.ucStateOriginal = get_valid_warn_stat(pt_warn_mu->t_warn_ch2_dw_inpow_greater);
        t_dev_warn[0].t_warn_ch2_dw_inpow_less.ucStateOriginal = get_valid_warn_stat(pt_warn_mu->t_warn_ch2_dw_inpow_less);
        t_dev_warn[0].t_warn_ch3_dw_inpow_greater.ucStateOriginal = get_valid_warn_stat(pt_warn_mu->t_warn_ch3_dw_inpow_greater);
        t_dev_warn[0].t_warn_ch3_dw_inpow_less.ucStateOriginal = get_valid_warn_stat(pt_warn_mu->t_warn_ch3_dw_inpow_less);
        t_dev_warn[0].t_warn_iso.ucStateOriginal = get_valid_warn_stat(pt_warn_mu->t_warn_iso);

        t_dev_warn[0].t_warn_ch1_dw_inpow_greater.ucWarnState = get_valid_warn_stat(pt_warn_mu->t_warn_ch1_dw_inpow_greater);
        t_dev_warn[0].t_warn_ch1_dw_inpow_less.ucWarnState = get_valid_warn_stat(pt_warn_mu->t_warn_ch1_dw_inpow_less);
        t_dev_warn[0].t_warn_ch2_dw_inpow_greater.ucWarnState = get_valid_warn_stat(pt_warn_mu->t_warn_ch2_dw_inpow_greater);
        t_dev_warn[0].t_warn_ch2_dw_inpow_less.ucWarnState = get_valid_warn_stat(pt_warn_mu->t_warn_ch2_dw_inpow_less);
        t_dev_warn[0].t_warn_ch3_dw_inpow_greater.ucWarnState = get_valid_warn_stat(pt_warn_mu->t_warn_ch3_dw_inpow_greater);
        t_dev_warn[0].t_warn_ch3_dw_inpow_less.ucWarnState = get_valid_warn_stat(pt_warn_mu->t_warn_ch3_dw_inpow_less);
        t_dev_warn[0].t_warn_iso.ucWarnState = get_valid_warn_stat(pt_warn_mu->t_warn_iso);
        
        for( uint8_t i=1; i<WARN_SLV_DEV_MAX_NUM; i++ )
        {
            // pt_ru_dev =  get_ru_device_ptr_by_monitor_dev_id(i);
            // //存在从机设备且类型为RU,否则不处理
            // if(pt_ru_dev == NULL || pt_ru_dev->dev_type != DEV_TYPE_RU)
            //     continue;
            pt_warn_ru = &t_warn_save[i];

            t_dev_warn[i].t_warn_ch1_dw_inpow_greater.ucStateOriginal = get_valid_warn_stat(pt_warn_ru->t_warn_ch1_dw_inpow_greater);
            t_dev_warn[i].t_warn_ch1_dw_inpow_less.ucStateOriginal = get_valid_warn_stat(pt_warn_ru->t_warn_ch1_dw_inpow_less);
            t_dev_warn[i].t_warn_ch2_dw_inpow_greater.ucStateOriginal = get_valid_warn_stat(pt_warn_ru->t_warn_ch2_dw_inpow_greater);
            t_dev_warn[i].t_warn_ch2_dw_inpow_less.ucStateOriginal = get_valid_warn_stat(pt_warn_ru->t_warn_ch2_dw_inpow_less);
            t_dev_warn[i].t_warn_ch3_dw_inpow_greater.ucStateOriginal = get_valid_warn_stat(pt_warn_ru->t_warn_ch3_dw_inpow_greater);
            t_dev_warn[i].t_warn_ch3_dw_inpow_less.ucStateOriginal = get_valid_warn_stat(pt_warn_ru->t_warn_ch3_dw_inpow_less);
            t_dev_warn[i].t_warn_iso.ucStateOriginal = get_valid_warn_stat(pt_warn_ru->t_warn_iso);

            t_dev_warn[i].t_warn_ch1_dw_inpow_greater.ucWarnState = get_valid_warn_stat(pt_warn_ru->t_warn_ch1_dw_inpow_greater);
            t_dev_warn[i].t_warn_ch1_dw_inpow_less.ucWarnState = get_valid_warn_stat(pt_warn_ru->t_warn_ch1_dw_inpow_less);
            t_dev_warn[i].t_warn_ch2_dw_inpow_greater.ucWarnState = get_valid_warn_stat(pt_warn_ru->t_warn_ch2_dw_inpow_greater);
            t_dev_warn[i].t_warn_ch2_dw_inpow_less.ucWarnState = get_valid_warn_stat(pt_warn_ru->t_warn_ch2_dw_inpow_less);
            t_dev_warn[i].t_warn_ch3_dw_inpow_greater.ucWarnState = get_valid_warn_stat(pt_warn_ru->t_warn_ch3_dw_inpow_greater);
            t_dev_warn[i].t_warn_ch3_dw_inpow_less.ucWarnState = get_valid_warn_stat(pt_warn_ru->t_warn_ch3_dw_inpow_less);
            t_dev_warn[i].t_warn_iso.ucWarnState = get_valid_warn_stat(pt_warn_ru->t_warn_iso);
        }
    }
}
	

/************************************************************************
* @brief    判断告警是否需要重新上报
************************************************************************/
void dev_warn_rpt_check( uint8_t dev_id )
{
	dev_warn_para_t *pt = &t_dev_warn[ dev_id ];
	
	warnOmcRptCheck( &pt->t_warn_ch1_dw_inpow_greater );
	warnOmcRptCheck( &pt->t_warn_ch1_dw_inpow_less    );
	warnOmcRptCheck( &pt->t_warn_ch2_dw_inpow_greater );
	warnOmcRptCheck( &pt->t_warn_ch2_dw_inpow_less    );
	warnOmcRptCheck( &pt->t_warn_ch3_dw_inpow_greater );
	warnOmcRptCheck( &pt->t_warn_ch3_dw_inpow_less    );
	warnOmcRptCheck( &pt->t_warn_iso				  );
}

/************************************************************************
* @brief    告警处理流程
* @note     在任务中调用
************************************************************************/
void warn_flow_in_task( void )
{
	for( uint8_t dev_id=0; dev_id<=WARN_SLV_DEV_MAX_NUM; dev_id++ )
	{
		dev_warn_state_calc( dev_id );	//获取参数, 设置告警状态
		dev_warn_rpt_check( dev_id );	
		dev_warn_state_save_in_flash(); //保存历史告警状态
	}
}


/************************************************************************
* @brief    测试函数
************************************************************************/


/**
* @brief   计算单个设备的告警状态
* @param   dev_id: [输入/出] 
* @return    
* @data    2024-02-20
* @note    
*/
void test_dev_warn_state_cal( uint8_t dev_id )
{
	if( dev_id >= WARN_SLV_DEV_MAX_NUM )
		return;
	
	get_dev_warn_para( dev_id );
	dev_warn_para_t *pt = &t_dev_warn[ dev_id ];

    if(dev_id == 0) 
    {
        pt->ch1_dw_in_pow = -40;
        pt->ch2_dw_in_pow = -40;
        pt->ch3_dw_in_pow = -40;
        pt->iso = 30;
    }else 
    {
        pt->ch1_dw_in_pow = -40;
        pt->ch2_dw_in_pow = -40;
        pt->ch3_dw_in_pow = -40;
        pt->iso = 30;
    }

    warnCheck( &pt->t_warn_ch1_dw_inpow_greater, pt->ch1_dw_in_pow);
    warnCheck( &pt->t_warn_ch1_dw_inpow_less,    pt->ch1_dw_in_pow);
    warnCheck( &pt->t_warn_ch2_dw_inpow_greater, pt->ch2_dw_in_pow);
    warnCheck( &pt->t_warn_ch2_dw_inpow_less,    pt->ch2_dw_in_pow);
    warnCheck( &pt->t_warn_ch3_dw_inpow_greater, pt->ch3_dw_in_pow);
    warnCheck( &pt->t_warn_ch3_dw_inpow_less,    pt->ch3_dw_in_pow);
    warnCheck( &pt->t_warn_iso, 				    pt->iso       );

}

void test_warn_flow_in_task( void )
{
//	for( uint8_t dev_id=0; dev_id<=WARN_SLV_DEV_MAX_NUM; dev_id++ )
//	{
//        test_dev_warn_state_cal( dev_id );	//测试获取参数, 设置告警状态
//		//dev_warn_state_calc( dev_id );	//获取参数, 设置告警状态
//		dev_warn_rpt_check( dev_id );	
//	}
    
    test_dev_warn_state_cal( 0 );	//测试获取参数, 设置告警状态
    dev_warn_rpt_check( 0 );	
    dev_warn_state_save_in_flash();
}
