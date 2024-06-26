/**
 * **********************************************************************************
 * @copyright Copyright (c) 2023 Shaanxi Tianji Communication Technologies Co., Ltd.
 *                         
 * @file        os_queue.c   
 * @author      WangJun
 * @version     None
 * @date        2023-06-16     
 * @brief       None   
 * 
 * **********************************************************************************
 */

#include "os_queue.h"
#include "rtt.h"
#include "app_monitor_report.h"

/*---------------------- 端口定义 ----------------------*/


/*---------------------- 宏 定 义 ----------------------*/


/*---------------------- 结 构 体 ----------------------*/


/*---------------------- 常    量 ----------------------*/


/*---------------------- 全局变量 ----------------------*/


/*---------------------- 局部变量 ----------------------*/


/*---------------------- 函    数 ----------------------*/
/*****************************************************************************
* @brief  : 创建消息队列
* @param  : None
* @retval : None
* @date   : 2023-06-20
* @note   : 
*****************************************************************************/
void os_queue_create(void)
{
    socket_queue_create();
}
