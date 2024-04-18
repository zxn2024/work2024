/**
 * **********************************************************************************
 * @copyright Copyright (c) 2023 Shaanxi Tianji Communication Technologies Co., Ltd.
 *
 * @file        os_timer.c
 * @author      WangJun
 * @version     None
 * @date        2023-06-16
 * @brief       None
 *
 * **********************************************************************************
 */

#include "os_timer.h"

#include "system_config.h"
#include "rtt.h"

#include "bsp_rf_time.h"
#include "bsp_config.h"
#include "bsp_reboot.h"
#include "bsp_temperature.h"
#include "warn_core.h"
#include "bsp_rf_iso.h"
#include "app_monitor.h"
/*---------------------- 端口定义 ----------------------*/


/*---------------------- 宏 定 义 ----------------------*/


/*---------------------- 结 构 体 ----------------------*/


/*---------------------- 常    量 ----------------------*/


/*---------------------- 全局变量 ----------------------*/


/*---------------------- 局部变量 ----------------------*/
static TimerHandle_t xTimer0 = NULL;

/*---------------------- 函    数 ----------------------*/
/*****************************************************************************
* @brief  : 软件定时器计数
* @param  : None
* @retval : None
* @date   : 2023-06-20
* @note   :
*****************************************************************************/
static void vTimeCnt(void)
{
    rf_ctr_in_time_isr();
    cat1_in_timer_isr();
    reboot_in_time_isr();
	tmp_cnt_in_time_isr();
	modem_in_timer_isr();
    warnTimeAddInIsr();    
    vIsoTimeAddInTimeIsr();
    monitor_in_timer_isr();
}


/*****************************************************************************
* @brief  : 软件定时器回调函数
* @param  : pxTimer: [输入/出] 
* @retval : 
* @date   : 2023-06-20
* @note   : 
*****************************************************************************/
void vTimerCallback(xTimerHandle pxTimer)
{
    uint32_t ulTimerID;

    configASSERT(pxTimer);

    /* 获取定时器ID */
    ulTimerID = (uint32_t)pvTimerGetTimerID(pxTimer);

    /* 处理定时器0任务 */
    if(0 == ulTimerID)
    {
        vTimeCnt();
    }
}


/*****************************************************************************
* @brief  : 创建软件定时器
* @param  : None
* @retval : None
* @date   : 2023-06-20
* @note   :
*****************************************************************************/
void os_timer_create(void)
{
    const TickType_t xTimerPer = TIMER_PERIOD_TICKS;    /* 100ms */
    const TickType_t xTimerId = 0;

    xTimer0 = xTimerCreate("Timer0",            /* 定时器名字 */
                            xTimerPer,          /* 定时器周期 */
                            pdTRUE,             /* 周期性 */
                            (void *)xTimerId,   /* 定时器ID */
                            vTimerCallback);    /* 定时器回调函数 */

	rf_ctr_time_init();
    if(xTimer0 == NULL)
    {
        /* 定时器创建失败 */
        rtt_printf(RTT_OS, RTT_TEXT_RED"xTimer0 create fail\r\n");
    }
    else
    {
        /* 启动定时器, 系统启动后开始工作 */
        if(xTimerStart(xTimer0, 100) != pdPASS)
        {
            /* 定时器还没有进入激活状态 */
            rtt_printf(RTT_OS, RTT_TEXT_RED"xTimer0 inactivated\r\n");
        }
    }
}
