/*************************************** Copyright ******************************
 *
 * (C) Copyright 2023, Shaanxi Tianji Communication
 * All Rights Reserved
 *
 * FileName   : bsp_public_macro.h
 * Version    : none
 * Author     :
 * Date       : 2023-02-10
 * Description: none
 *******************************************************************************/
#ifndef _WARN_CORE_H_
#define _WARN_CORE_H_

#include "MCU_config.h"
#include "stdint.h"

/* 定时器周期 */
#define WANR_TIME_PERIOR_MS     ( 100 )

/*告警方式*/
typedef enum
{
    WARN_TYPE_GREATER = 0,   ///< 高于门限
    WARN_TYPE_LESS    = 1,   ///< 低于门限
    WARN_TYPE_EQUAL   = 2    ///< 置于某种状态
} eWarnType;


#pragma pack(4) // 1字节对齐
/*告警数据结构*/
typedef struct tagWarn_T
{
    ENABLE_DISABLE_E ucEn;               ///< 告警使能, ENABLE / DISABLE
    int8_t           scGate;             ///< 门限值 或者 状态值

    eWarnType        WarnType;           ///< 告警类型：WARN_TYPE_GREATER 高于门限 WARN_TYPE_LESS 低于门限 WARN_TYPE_EQUAL 置于某种状态
    
    WARN_STATE_E     ucStateOriginal;    ///< 告警状态, 保存值, WARN_STAT_NORMAL(00):正常, WARN_STAT_WARNING(01):告警
    WARN_STATE_E     ucStateCurrent;     ///< 告警阈值比较后, 当前时刻的状态
    WARN_STATE_E     ucWarnState;            ///< 告警状态

    REPORT_STATE_E   ucWarnEvent;        ///< 告警事件,当告警状态变化时产生事件,向监控平台上报后消除事件: 00 无事件, 01 告警事件

    uint32_t         ulDelayTimeIn;       ///< 告警产生延迟时间, 单位: MS
    uint32_t         ulDelayTimeQuit;     ///< 告警消除延迟时间, 单位: MS
    uint32_t         ulDelayTimeStart;    ///< 告警延迟时间计数器
 
    ENABLE_DISABLE_E ucWarnRptInterEn;    ///< 使能
	uint8_t          ucWarnTotalNum;	  ///< 告警总次数
	uint8_t          ucWarnReportNum;	  ///< 告警已上报到第几轮
    uint32_t         ulRptIntervalStart;  ///< 告警时间间隔计数起始时间
	uint16_t         usRptInterval;       ///< 告警时间间隔, 时间计数到0的时候, 会置位一次告警事件
								          ///< 3分钟, 3分钟, 3分钟, 3小时, 3分钟, 3分钟, 3小时, 3分钟, 3分钟, 0
} tWarn_T, *ptWarn_T;


/* 初始化告警 */
void vWarnInit(ptWarn_T ptWarn, int8_t gate, eWarnType warnType, uint32_t WarnGenerateDelayTime, uint32_t WarnClearDelayTime);

/* 设置告警使能 */
void vWarnSetEn(ptWarn_T ptWarn, ENABLE_DISABLE_E ucEn);

/* 设置告警门限 */
void vWarnSetThreshold(ptWarn_T ptWarn, int8_t gate);

/* 告警状态检测 */
void warnCheck(ptWarn_T ptWarn, int8_t state);

/* 获取告警状态 */
WARN_STATE_E vWarnGetStat(ptWarn_T ptWarn);

/* 获取上报标识 */
REPORT_STATE_E vWarnGetEvt(ptWarn_T ptWarn);

/* 向OMC上报失败后的多次尝试 */
void warnOmcRptCheck( ptWarn_T ptWarn );

/* 向OMC上报成功后更新告警状态 */
void warnOmcRptSuccessed(ptWarn_T ptWarn);

/* 清除上报标志 */
void warnClearOmcRptEvt( ptWarn_T ptWarn );

/* 定时器, 在定时中断中调用 */
void warnTimeAddInIsr( void );

#endif
