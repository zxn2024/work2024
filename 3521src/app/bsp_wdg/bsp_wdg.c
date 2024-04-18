/***************************** Copyright ****************************************
*
* (C) Copyright 2023, Shaanxi Tianji Communication
*  All Rights Reserved
*
* FileName   : bsp_wdg.c
* Version    : none
* Author     :
* Date       : 2023-10-07
* Description: 最多支持24个任务, 在中断中间隔1S喂狗,
**             在每个任务中置位相应的事件组位, 在中断中检测所有的事件组是否置位,
**             若都置位, 则复位事件组, 否则在超时后, 停止喂狗,触发看门狗复位
**             引起复位的最大时间可以通过 WDT_MAX_TIMEOUT 参数修改, 最大65S
**
**             wdg_para_init(): 初始化看门狗参数, 在任务创建之后调用一次
**             wdg_egp_creat(): 事件组创建和初始化
**			   wdg_in_time_isr( const uint16_t period ): 定时器中调用, 计时和喂狗, period: 定时器周期, 单位ms,
**             wdg_in_task(): 在每个任务中调用
*******************************************************************************/
#include "FreeRTOS.h"
#include "event_groups.h"
#include "rtt.h"
#include "hi_iwdg.h"

//#include "bsp_os_task.h"


/*---------------------- 端口定义 --------------------------------------*/


/*---------------------- 宏定义  ---------------------------------------*/

#define WDT_MAX_TIMEOUT 	( 30 )  //喂狗最大超时间隔, 单位S

#define WDG_MAX_TASK_NUM 	( 24 )	//支持的最大任务数量, 最大24, 不可更改


/*---------------------- 结构体 ----------------------------------------*/


/*---------------------- 常量 ------------------------------------------*/


/*---------------------- 全局变量 --------------------------------------*/


/*---------------------- 局部变量 --------------------------------------*/


/*---------------------- 声明 ------------------------------------------*/



/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ HAL ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/





/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DRV ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/





/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ APP ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static EventGroupHandle_t xWdgEgp = NULL;

static uint8_t TaskNum = 1;			//任务总数量, 最大支持24个, 有效值范围 1 ~ 24
static EventBits_t BitWdgAll = 0;	//对应使用到的bit位

/*****************************************************************************
* @Name	  : wdg_para_init
* @Brief  : 初始化
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2022-11-03
* @Note   :
*****************************************************************************/
void wdg_para_init(void)
{
    TaskNum = (uint8_t)uxTaskGetNumberOfTasks();    //获取任务数量

    if(TaskNum >= WDG_MAX_TASK_NUM)   //一个事件组最多建立24个事件位
        TaskNum = WDG_MAX_TASK_NUM;

    BitWdgAll = 0;
    for(uint8_t i = 1; i <= TaskNum - 1; i++)
    {
        BitWdgAll |= (0x01 << i);
    }
}

/*****************************************************************************
* @Name	  : wdg_egp_init
* @Brief  : 事件组初始化
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2022-11-03
* @Note   :
*****************************************************************************/
static void wdg_egp_init(void)
{
    xEventGroupClearBits(xWdgEgp, 0x00FFFFFF);
}


/*****************************************************************************
* @Name	  : wdg_egp_creat
* @Brief  : 创建事件组
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2022-11-03
* @Note   :
*****************************************************************************/
void wdg_egp_creat(void)
{
    xWdgEgp = xEventGroupCreate();

    if(xWdgEgp == NULL) /* 任务创建失败 */
    {
        rtt_printf(RTT_OS, RTT_TEXT_RED"[ERROR] xWdgEgp Create FAIL...\r\n");
        return;
    }

    wdg_egp_init();
}


/*****************************************************************************
* @Name	  : wdg_in_time_isr
* @Brief  : 定时器中调用的WDT相关计数器
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2022-11-03
* @Note   :
*****************************************************************************/
void wdg_in_time_isr(const uint16_t period)
{
    /* 定时器计数 */
#define WDT_AUTO_CNT	     ( ( 1 * 1000 ) / period )    //定时喂狗, 1S 1次
#define WDT_FEED_TIMEOUT	 ( ( WDT_MAX_TIMEOUT * 1000 ) / period )    //定时喂狗, 1S 1次

    EventBits_t xEgpBit = xEventGroupGetBitsFromISR(xWdgEgp);

    static uint16_t usWdgAutoCnt = 0;
    static uint16_t usWdgOverTimeCnt = 0;

    ++usWdgOverTimeCnt;

    if((xEgpBit & BitWdgAll) == BitWdgAll)	//当所有的任务均设置了喂狗标识
    {
        usWdgOverTimeCnt = 0;
        xEventGroupClearBitsFromISR(xWdgEgp, BitWdgAll);
    }

    //喂狗超时定时器
    if(usWdgOverTimeCnt <= WDT_FEED_TIMEOUT)
        ++usWdgOverTimeCnt;
    else
        return;

    if(++usWdgAutoCnt >= WDT_AUTO_CNT)
    {
        usWdgAutoCnt = 0;
        hi_iwdg_feed();
    }
}


/*****************************************************************************
* @Name	  : wdg_in_task
* @Brief  : 在任务中设置运行正常标识
* @Param  : taskPriority: 当前任务定义的优先级
* @Retval :
* @Author :
* @Data   : 2022-11-03
* @Note   :
*****************************************************************************/
void wdg_in_task(void)
{
    EventBits_t bitsTask;
    TaskHandle_t current_handle = xTaskGetCurrentTaskHandle();	//获取当前任务句柄
    if(NULL == current_handle)
        return;

    uint8_t priority = (uint8_t)uxTaskPriorityGet(current_handle);	//获取当前任务的优先级

    if(priority > WDG_MAX_TASK_NUM)
        return;

    bitsTask = (1 << (priority - 1));

    xEventGroupSetBits(xWdgEgp, bitsTask);
}

