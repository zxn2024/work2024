/*************************************** Copyright ******************************
*
* (C) Copyright 2022, Shaanxi Tianji Communication
* All Rights Reserved
*                         
* FileName   : bsp_reboot.h   
* Version    : None		
* Author     : WangJun	
* Date       : 2022-08-18         
* Description: None   
*******************************************************************************/

#ifndef __BSP_REBOOT_H_
#define __BSP_REBOOT_H_

#include "FreeRTOS.h"
#include "event_groups.h"

typedef enum
{
    RESET_SYSTEM,      // 系统重启
    RESET_MASTER,	   // 重启主机
    RESET_SLAVE,       // 重启从机
    RESET_5GMODEM_SW,  // 5GMODEM软件重启
    RESET_5GMODEM_HW,  // 5GMODEM硬件重启
    RESET_CAT1,        // CAT1重启
    RESET_BLEM,        // BLEM重启
    RESET_BLES,        // BLES重启
    RESET_FPGA,        // FPGA重启
    RESET_BRD		   // 广播重启
} SystemRst_E;


void reboot_egp_creat(void);              // 创建重启事件组
void reboot_in_time_isr(void);             // 定时器计数

void reboot_set_egp_bit(SystemRst_E type);    // 设置重启事件
void reboot_in_task(void);

#endif
