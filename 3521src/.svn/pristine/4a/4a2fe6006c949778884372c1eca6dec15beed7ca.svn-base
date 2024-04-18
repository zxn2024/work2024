/*************************************** Copyright ******************************
 *
 * (C) Copyright 2023, Shaanxi Tianji Communication
 * All Rights Reserved
 *
 * FileName   : app_monitor.h
 * Version    : none
 * Author     :
 * Date       : 2024-3-5
 * Description: none
 *******************************************************************************/

#ifndef _APP_MONITOR_H_
#define _APP_MONITOR_H_

#include "bsp_config.h"
#include "mcp_core.h"


#define SOCKET_LINK_USER LINK0
#define SOCKET_LINK_TJC LINK1

/*****************************************************************************
 * @Name	  : monitor_msg_in_task
 * @Brief  : 在任务中调用的监控消息处理函数
 * @Param  : None
 * @Retval :
 * @Author :
 * @Data   : 2024-3-5
 * @Note   :
 *****************************************************************************/
void monitor_msg_in_task(void);

/*****************************************************************************
 * @Name   : monitor_in_timer_isr
 * @Brief  : 链接定时器，在定时器中调用
 * @Param  : None
 * @Retval : None
 * @Author :
 * @Date   : 2024-04-14
 * @Note   :
 */
void monitor_in_timer_isr(void);

/*****************************************************************************
 * @Name   : monitor_reconnect
 * @Brief  : 监控重连
 * @Param  : None
 * @Retval : None
 * @Author :
 * @Date   : 2024-04-14
 * @Note   :
 */
void monitor_reconnect(void);

#endif /* _APP_MONITOR_H_ */
