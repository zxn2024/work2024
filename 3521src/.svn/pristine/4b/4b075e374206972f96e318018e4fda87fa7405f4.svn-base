/**
 * **********************************************************************************
 * @copyright Copyright (c) 2023 Shaanxi Tianji Communication Technologies Co., Ltd.
 *
 * @file        rtt.h
 * @author      WangJun
 * @version     None
 * @date        2023-06-16
 * @brief       None
 *
 * **********************************************************************************
 */

#ifndef __RTT_H_
#define __RTT_H_

#include <stdint.h>
#include "SEGGER_RTT.h"
#include "SEGGER_RTT_Conf.h"

/*---------------------- 宏 定 义 ----------------------*/
#define _RTT_VIEWER_EN_     1           /* RTT Viewer输出使能 */

/* 文本颜色 */
#define RTT_TEXT_RED        RTT_CTRL_TEXT_BRIGHT_RED
#define RTT_TEXT_GREEN      RTT_CTRL_TEXT_BRIGHT_GREEN
#define RTT_TEXT_YELLOW     RTT_CTRL_TEXT_BRIGHT_YELLOW
#define RTT_TEXT_BLUE       RTT_CTRL_TEXT_BRIGHT_BLUE
#define RTT_TEXT_MAGENTA    RTT_CTRL_TEXT_BRIGHT_MAGENTA    /* 洋红 */
#define RTT_TEXT_CYAN 	    RTT_CTRL_TEXT_BRIGHT_CYAN       /* 蓝绿色 */ 
#define RTT_TEXT_WHITE      RTT_CTRL_TEXT_BRIGHT_WHITE

/* 文本背景颜色 */
#define RTT_BG_RED 	  	    RTT_CTRL_BG_RED
#define RTT_BG_GREEN 	    RTT_CTRL_BG_GREEN
#define RTT_BG_YELLOW 	    RTT_CTRL_BG_YELLOW
#define RTT_BG_BLUE 	    RTT_CTRL_BG_BLUE
#define RTT_BG_MAGENTA      RTT_CTRL_BG_MAGENTA
#define RTT_BG_CYAN 	    RTT_CTRL_BG_CYAN
#define RTT_BG_WHITE 	    RTT_CTRL_BG_WHITE


/*---------------------- 枚    举 ----------------------*/
enum
{
    RTT_OS = 0,        
    RTT_ADC,
    RTT_RF,
    RTT_CAT1,
    RTT_PC,
    RTT_FPGA,           // 5
    RTT_MODEM,
    RTT_BLE,
    RTT_FLASH,
    RTT_UPGRADE,
    RTT_TESTMODE,       // 10
    RTT_4G_ISO,
    RTT_MONITOR,
    RTT_DRIVERS,
    RTT_8668,
	RTT_LAST			//15
};

/*---------------------- 函    数 ----------------------*/
int rtt_printf(char Terminalx, const char * sFormat, ...);
void rtt_init(void);


#endif

