/**
* Copyright (c) 2024 Shaanxi Tianji Communication Technologies Co., Ltd.
* 
* @file     warn_core.c
* @author   
* @version  V1.0
* @date     2024-02-03
* @brief    [描述]
* 
*/

#include "warn_core.h"
#include "main.h"

#define WARN_DEBUG  ( 1 )   ///< 告警调试打印开关, 0: 不打印, 1: 打印

///反复尝试上报延迟间隔:单位秒 3分钟, 3分钟, 3小时, 3分钟, 3分钟, 3小时, 3分钟, 3分钟, 终止
const uint16_t warnRptTimeArray[9] = {3*60, 3*60, 3*60*60, 3*60, 3*60, 3*60*60, 3*60, 3*60, 0 };
//const uint16_t warnRptTimeArray[9] = {10, 11, 12, 13, 14, 15, 16, 17, 0 };
uint32_t warnTimeCnt = 0;   ///< 告警计时器

/**
* @brief  初始化
* @param  ptWarn       告警结构体
* @param  gate         告警门限, -128 ~ 127
* @param  warnType     告警类型  WARN_GREATER:   高于门限告警 
*                               WARN_TYPE_LESS: 低于门限告警 
*                               WARN_TYPE_EQUAL: 等于告警
*
* @param  WarnGenerateDelayTime 告警产生延迟时间, 单位:S, 最大不超过24小时
*                               0: 无延迟, 其他: 延迟的秒数
* @param  WarnClearDelayTime    告警产生延迟时间, 最大不超过24小时
*                               0: 无延迟, 其他: 延迟的秒数
*/
void vWarnInit(ptWarn_T ptWarn, int8_t gate, eWarnType warnType, uint32_t WarnGenerateDelayTime, uint32_t WarnClearDelayTime)
{
    if( WarnGenerateDelayTime >= 24*60*60 )
        WarnGenerateDelayTime = 24*60*60;

    if( WarnClearDelayTime >= 24*60*60 )
        WarnClearDelayTime = 24*60*60;

    ptWarn->ucEn     = E_ENABLE;
    ptWarn->WarnType = warnType;

    ptWarn->scGate   = gate;

    ptWarn->ucStateOriginal = NORMAL; // 告警状态, 保存值, WARN_STAT_NORMAL(00):正常, WARN_STAT_WARNING(01):告警
    ptWarn->ucStateCurrent  = NORMAL; // 告警状态, 当前值 
    ptWarn->ucWarnState  = NORMAL; // 告警状态, 当前值 

    ptWarn->ucWarnEvent = RPT_NULL;        // 告警事件,当告警状态变化时产生事件,向监控平台上报后消除事件: 00 无事件, 01 告警事件

    ptWarn->ulDelayTimeIn = WarnGenerateDelayTime * 1000 / WANR_TIME_PERIOR_MS;     //告警产生延迟时间, 单位: S
    ptWarn->ulDelayTimeQuit = WarnClearDelayTime * 1000 / WANR_TIME_PERIOR_MS;        //告警消除延迟时间, 单位: S
    ptWarn->ulDelayTimeStart = 0;     // 告警延迟时间计数器
 
	ptWarn->ucWarnTotalNum = 0;	  // 告警总次数
	ptWarn->ucWarnReportNum = 0;	  // 告警总次数
	ptWarn->usRptInterval = warnRptTimeArray[0];
}


/**
* @brief    设置告警使能
* @param    ptWarn         para_doc
* @param    ucEn           para_doc
*/
void vWarnSetEn(ptWarn_T ptWarn, ENABLE_DISABLE_E ucEn)
{
    ptWarn->ucEn = ucEn;
}


/**
* @brief    设置告警门限
* @param    ptWarn         para_doc
* @param    gate           para_doc
*/
void vWarnSetThreshold(ptWarn_T ptWarn, int8_t gate)
{
    ptWarn->scGate = gate;
}


/*
***********************************************************************
* @brief    计时器, 在定时器中调用
***********************************************************************
*/
void warnTimeAddInIsr( void )
{
    warnTimeCnt++;
}


/************************************************************************
* @brief    Get the Warn Time Cnt object[描述]
* @return uint32_t 
************************************************************************/
static uint32_t getWarnTimeCnt( void )
{
    return warnTimeCnt;
}


/************************************************************************
* @brief    设置告警的起始时间, 若告警状态未变化, 则更新时间为最新时间
* @param    ptWarn         para_doc
************************************************************************/
static void vWarnDelayTimeStartSetValue(ptWarn_T ptWarn)
{
    if(ptWarn->ucWarnState == ptWarn->ucStateCurrent)
    {
        ptWarn->ulDelayTimeStart = getWarnTimeCnt();
    }
}


/************************************************************************
* @brief    Get the Wart Delay Time Result object[描述]
* @param    ptWarn         para_doc
* @return   BOOL_E 
************************************************************************/
static REPORT_STATE_E CalcWartRptStat( ptWarn_T ptWarn )
{
    uint32_t cur_time   = getWarnTimeCnt();
    uint32_t start_time = ptWarn->ulDelayTimeStart;
    BOOL_E warn_rpt_chk = B_FALSE;

    if(ptWarn->ucStateCurrent == ptWarn->ucWarnState)   //状态一致, 不需要继续判断
    {
        ptWarn->ucWarnEvent = RPT_NULL;
        return ptWarn->ucWarnEvent;
    }

    if(ptWarn->ulDelayTimeIn == 0)  //0延迟告警
    {
        warn_rpt_chk = B_TRUE;
        ptWarn->ucWarnState = ptWarn->ucStateCurrent;
    }
    else
    {
        if(cur_time > ptWarn->ulDelayTimeStart) 
        {
            uint32_t interval_time = (ptWarn->ucStateCurrent == WARN) ? ptWarn->ulDelayTimeIn : ptWarn->ulDelayTimeQuit; //延迟时间 
            if(cur_time - start_time > interval_time)
            {
                warn_rpt_chk = B_TRUE;
                ptWarn->ucWarnState = ptWarn->ucStateCurrent;
            }
            else
                warn_rpt_chk = B_FALSE;
        }
    }
    if(E_ENABLE != ptWarn->ucEn)
    {
        ptWarn->ucWarnEvent = RPT_NULL;
        return ptWarn->ucWarnEvent;
    }

    switch (ptWarn->ucWarnEvent) 
    {
        case RPT_NULL: //初始状态,或上报已回,检测到告警就上报
            if(B_TRUE == warn_rpt_chk && ptWarn->ucStateCurrent == WARN)
            {
                ptWarn->ucWarnEvent = RPT_WARN;
            }else if(B_TRUE == warn_rpt_chk && ptWarn->ucStateCurrent == NORMAL)
            {
                ptWarn->ucWarnEvent = RPT_QUIT;
            }
            break;
            
        case RPT_WARN: //告警上报
        case RPT_QUIT: //退出告警上报
            break;

        case RPT_WARN_SENT: //告警上报已发未回,重复检测到告警,保持等待回包状态,检测到退出则上报
            if(B_TRUE == warn_rpt_chk && ptWarn->ucStateCurrent == NORMAL)
            {
                ptWarn->ucWarnEvent = RPT_QUIT;
            }else if(B_TRUE == warn_rpt_chk && ptWarn->ucStateCurrent == WARN)
                break;
        case RPT_QUIT_SENT: //退出上报已发未回,重复检测到退出告警,保持等待回包状态,检测到告警则上报
            if(B_TRUE == warn_rpt_chk && ptWarn->ucStateCurrent == NORMAL)
            {
                ptWarn->ucWarnEvent = RPT_QUIT_SENT;
            }else if(B_TRUE == warn_rpt_chk && ptWarn->ucStateCurrent == WARN)
            {
                ptWarn->ucWarnEvent = RPT_WARN;
            }
            break;
        default:
            ptWarn->ucWarnEvent = RPT_NULL;
            break;
    }
    
    return ptWarn->ucWarnEvent;
}


/************************************************************************
* @brief    告警状态检测
* @param    ptWarn          para_doc
* @param    state           当前告警参数状态或者数值
************************************************************************/
void warnCheck(ptWarn_T ptWarn, int8_t state)
{
    //根据类型不同，处理告警值
    switch (ptWarn->WarnType) 
    {
        case WARN_TYPE_GREATER:		//高于门限告警
                ptWarn->ucStateCurrent =  (state > ptWarn->scGate) ? WARN : NORMAL;
                break;

        case WARN_TYPE_LESS:		//低于门限告警
                ptWarn->ucStateCurrent =  (state < ptWarn->scGate) ? WARN : NORMAL;
                break;

        case WARN_TYPE_EQUAL:		//状态一致告警
                ptWarn->ucStateCurrent =  (state == ptWarn->scGate) ? WARN : NORMAL;
                break;

        default:
            break;
    }

    vWarnDelayTimeStartSetValue( ptWarn );      //更新告警变化的起始时间
    CalcWartRptStat( ptWarn );     //获取告警变化的持续时间
}


/************************************************************************
* @brief    获取告警状态
* @param    ptWarn         结构体
* @return   WARN_STATE_E   NORMAL / WARN
************************************************************************/
WARN_STATE_E vWarnGetStat(ptWarn_T ptWarn)
{
    return ptWarn->ucWarnState;
}

/************************************************************************
* @brief    获取上报标识
* @param    ptWarn         告警结构体
* @return   eWarnEvt: REPORT_NULL - 无告警上报
*                     REPORT_WARN - 有告警需要上报
************************************************************************/
REPORT_STATE_E vWarnGetEvt(ptWarn_T ptWarn)
{
    return ptWarn->ucWarnEvent;
}

/************************************************************************
* @brief    向OMC上报失败后的多次尝试
* @param    ptWarn         para_doc
* @note     
************************************************************************/
void warnOmcRptCheck( ptWarn_T ptWarn )
{

    if( ptWarn->ucStateOriginal == ptWarn->ucStateCurrent ) //状态一致
    {
        ptWarn->ucWarnRptInterEn = E_DISABLE;
        ptWarn->ucWarnTotalNum   = 0;
        ptWarn->ucWarnReportNum   = 0;
        ptWarn->usRptInterval    = warnRptTimeArray[0];
        return;
    }

    if( ptWarn->ucWarnTotalNum >= 9 )   //重复了9次, 不再上报
    {
        ptWarn->ucWarnTotalNum = 9;
        return;
    }

    if( E_DISABLE == ptWarn->ucWarnRptInterEn ) //状态不一致时判断
    {
        ptWarn->ucWarnRptInterEn   = E_ENABLE;
        ptWarn->ulRptIntervalStart = getWarnTimeCnt();
        ptWarn->usRptInterval      = warnRptTimeArray[ptWarn->ucWarnTotalNum];
        ptWarn->ucWarnTotalNum++;
    }

    uint32_t cur_time   = getWarnTimeCnt();
//    uint32_t start_time = ptWarn->ulRptIntervalStart;

    if( cur_time - ptWarn->ulRptIntervalStart >= S_TO_TICKS(ptWarn->usRptInterval) && ptWarn->ucWarnTotalNum != ptWarn->ucWarnReportNum)    //时间间隔超时, 本轮告警还未上报
    {
        ptWarn->ucWarnReportNum = ptWarn->ucWarnTotalNum;
        ptWarn->ucWarnEvent      = RPT_NULL;
        ptWarn->ucWarnRptInterEn = E_DISABLE;
    }
}


/************************************************************************
* @brief    清除上报标志
* @param    ptWarn         para_doc
* @note     进行上报操作后, 无论成功与否, 均需要清空上报事件
************************************************************************/
void warnClearOmcRptEvt( ptWarn_T ptWarn )
{
    if(ptWarn->ucWarnEvent == RPT_WARN)
        ptWarn->ucWarnEvent = RPT_WARN_SENT;
    else if(ptWarn->ucWarnEvent == RPT_QUIT)
        ptWarn->ucWarnEvent = RPT_QUIT_SENT;
}

/************************************************************************
* @brief    向OMC上报成功后更新告警状态
* @param    ptWarn         para_doc
************************************************************************/
void warnOmcRptSuccessed(ptWarn_T ptWarn)
{
    ptWarn->ucStateOriginal = ptWarn->ucStateCurrent;
    ptWarn->ucWarnRptInterEn = E_DISABLE;
    ptWarn->ucWarnTotalNum = 0;
    ptWarn->ucWarnReportNum = 0;
    ptWarn->ucWarnEvent = RPT_NULL;
}
