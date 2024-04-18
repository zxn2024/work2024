/************************************************************************
* Copyright (c) 2024 Shaanxi Tianji Communication Technologies Co., Ltd.
* 
* @file     bsp_warn_user.h
* @author   
* @version  
* @date     2024-02-29 
* @brief
* 
************************************************************************/

#ifndef __BSP_WARN_USER_H_
#define __BSP_WARN_USER_H_

//#include <stdint.h>
#include "public_enum.h"
#include "warn_core.h"
#if (LOCAL_MODULE_TYPE == _MU)
    #define SAVE_WARN_STATE_EN	( 1 )		// 1:保存告警状态 0:不保存告警状态
    #define WARN_SLV_DEV_MAX_NUM    ( SLAVE_DEV_MAX_NUM )		// 最大从机数量
#elif (LOCAL_MODULE_TYPE == _RU)
    #define SAVE_WARN_STATE_EN	( 1 )		// 1:保存告警状态 0:不保存告警状态
    #define WARN_SLV_DEV_MAX_NUM    ( 1 )		// 最大从机数量
#endif
typedef struct
{
    uint8_t dev_id; //监控设备ID 0 主机, 1~8 从机,等于数组下标

	uint8_t tdd_or_fdd;
	int8_t  ch1_dw_in_pow;
	int8_t  ch2_dw_in_pow;
	int8_t  ch3_dw_in_pow;
	uint8_t iso;
	
	tWarn_T t_warn_ch1_dw_inpow_greater;  //ch1下行输入过功率告警
	tWarn_T t_warn_ch1_dw_inpow_less;     //ch1下行输入欠功率告警
	tWarn_T t_warn_ch2_dw_inpow_greater;  //ch2下行输入过功率告警
	tWarn_T t_warn_ch2_dw_inpow_less;     //ch2下行输入欠功率告警
	tWarn_T t_warn_ch3_dw_inpow_greater;  //ch3下行输入过功率告警
	tWarn_T t_warn_ch3_dw_inpow_less;     //ch3下行输入欠功率告警
	tWarn_T t_warn_iso;
}dev_warn_para_t;

typedef struct
{
	WARN_STATE_E t_warn_ch1_dw_inpow_greater; //ch1下行输入过功率告警
	WARN_STATE_E t_warn_ch1_dw_inpow_less;  //ch1下行输入欠功率告警
	WARN_STATE_E t_warn_ch2_dw_inpow_greater;  //ch2下行输入过功率告警
	WARN_STATE_E t_warn_ch2_dw_inpow_less;   //ch2下行输入欠功率告警
	WARN_STATE_E t_warn_ch3_dw_inpow_greater;   //ch3下行输入过功率告警
	WARN_STATE_E t_warn_ch3_dw_inpow_less;    //ch3下行输入欠功率告警
	WARN_STATE_E t_warn_iso;
	WARN_STATE_E reserve1;
	WARN_STATE_E reserve2;
	WARN_STATE_E reserve3;
	WARN_STATE_E reserve4;
}dev_warn_save_t;


extern dev_warn_para_t   t_dev_warn[ WARN_SLV_DEV_MAX_NUM + 1 ];	//计算告警需要用到的参数, 0: 主机, 1~8: 从机
extern dev_warn_save_t   t_warn_save[ WARN_SLV_DEV_MAX_NUM + 1 ];	//保存主从机的告警参数, 0:主机, 1:从机

void dev_warn_init( void );	//初始化

BOOL_E dev_warn_rpt_flag( uint8_t dev_id );                     //查询是否有新的消息需要上报
uint8_t dev_warn_copy_warn_msg( uint8_t dev_id, uint8_t *tag ); //获取整理成PDU格式后的告警数据

void dev_clr_warn_state_after_rpt( uint8_t dev_id );            //上报之后清除告警事件状态
void dev_clr_warn_state_after_rpt_success( uint8_t dev_id );    //上报成功之后清除告警状态

void dev_warn_state_save_in_flash( void ); //保存告警状态到flash
void dev_warn_state_read_from_flash( void ); //从flash读取告警状态

void warn_flow_in_task( void );     //在任务中调用的告警判断流程


void test_warn_flow_in_task( void );
void warn_report_test(void);
#endif





