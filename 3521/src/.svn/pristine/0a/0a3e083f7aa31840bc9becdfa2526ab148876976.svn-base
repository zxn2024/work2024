/*************************************** Copyright ******************************
*
* (C) Copyright 2023, Shaanxi Tianji Communication
* All Rights Reserved
*                         
* FileName   : bsp_public_enum.h   
* Version    : none		
* Author     : ZhangWei			
* Date       : 2023-02-14         
* Description: none   
*******************************************************************************/
//#include <stdint.h>
#ifndef __BSP_PUBLIC_ENUM_H_
#define __BSP_PUBLIC_ENUM_H_

#include "bsp_dev_config.h"
/******************************枚举******************************/
//#define E_OK                    ( 0 )
//#define E_NOT_OK                ( !E_OK )

typedef enum
{
    OPEN = 0,  //开启
    CLOSE      //关闭
}OPEN_CLOSE_E;

typedef enum
{
    READ = 0,  
    WRITE
}READ_WRITE_E;


typedef enum
{
    LOCK = 0,    //锁定
    UNLOCK,      //失锁
    LOCK_END
}LOCK_STATE_E;

typedef enum
{
    SYNC = 0,    //同步
    UNSYNC,      //未同步
    SYNC_END
}SYNC_STATE_E;

typedef enum
{
    BOOL_TRUE = 0,    //真 / 正确
    BOOL_FALSE,       //假 / 错误
    BOOL_END
}BOOL_E;

typedef enum
{
	WORK_MODE_NORMAL = 0,	//正常模式
	WORK_MODE_STANDLONE,		//特殊的模式: 如单主机模式
    WORK_MODE_END
}WORK_MODE_E;

typedef enum
{
    RF_CH_UP = 0,  //上行
    RF_CH_DW,      //下行
    RF_CH_NUM
}RF_CH_E;


typedef enum
{
	SYS_TDD = 0,
	SYS_FDD,
    SYS_NULL
}TDD_FDD_E;


//系统编号
typedef enum
{
    RF_SYS_1 = 0,  //SYS1系统
    RF_SYS_2,      //SYS2系统
    RF_SYS_NUM
}RF_SYS_E;

typedef enum
{
    RF_SWITCH_CLOSE = 0,  //关闭
    RF_SWITCH_OPEN,       //开启
    RF_SWITCH_UP,         //上行单通
    RF_SWITCH_DW,         //下行单通
    RF_SWITCH_END
}RF_SWITCH_E;

typedef enum
{
    LEVEL_L = 0,     //低电平
    LEVEL_H,         //高电平
    LEVEL_END
}ELEC_LEVEL_E;

typedef enum
{
    NORMAL = 0,     //正常
    WARN,           //告警
    WARN_END
}WARN_STATE_E;

typedef enum
{
    SIM_OUT = 0,    //SIM卡未插入
    SIM_IN,         //SIM卡插入
    SIM_END
}SIM_STATE_E;

typedef enum
{
    NOT_REPORT = 0,     //不上报
    REPORT              //上报
}REPORT_STATE_E;

typedef enum
{
    MSG_SRC_OMC = 0,    //来自于OMC
    MSG_SRC_PC,         //来自于PC
    MSG_SRC_BLE,        //来自于BLE
    MSG_SRC_END
}MSG_SRC_E;

#endif





