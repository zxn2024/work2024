/*
 * **********************************************************************************
 * @copyright Copyright (c) 2023 Shaanxi Tianji Communication Technologies Co., Ltd.
 *
 * @file        rtt.c
 * @author      WangJun
 * @version     None
 * @date        2023-06-16
 * @brief       RTT Viewer Printf
 *
 * **********************************************************************************
 */

#include "rtt.h"


/*---------------------- 端口定义 ----------------------*/


/*---------------------- 宏 定 义 ----------------------*/


/*---------------------- 结 构 体 ----------------------*/


/*---------------------- 常    量 ----------------------*/


/*---------------------- 全局变量 ----------------------*/


/*---------------------- 局部变量 ----------------------*/


/*---------------------- 函    数 ----------------------*/


/*****************************************************************************
* @brief  : RTT Viewer打印
* @param  : Terminalx: 端口号
*		    sFormat: 参数
*			...:
* @retval : int
* @date   : 2023-06-16
* @note   : None
*****************************************************************************/
int rtt_printf(char Terminalx, const char * sFormat, ...)
{
    int r;

#if _RTT_VIEWER_EN_
    va_list ParamList;

    SEGGER_RTT_SetTerminal(Terminalx);
    va_start(ParamList, sFormat);

    r = SEGGER_RTT_vprintf(0, sFormat, &ParamList);

    SEGGER_RTT_printf(0, RTT_CTRL_TEXT_BRIGHT_WHITE"");

    va_end(ParamList);
#endif

    return r;
}



/*****************************************************************************
* @brief  : RTT Viewer初始化
* @param  : None
* @retval : None
* @date   : 2023-06-16
* @note   : None
*****************************************************************************/
void rtt_init(void)
{
	SEGGER_RTT_Init();
	
    /* 配置通道 0，上行配置*/
    SEGGER_RTT_ConfigUpBuffer(0, "RTT_UP", NULL, 0, SEGGER_RTT_MODE_NO_BLOCK_SKIP);

    /* 配置通道 0，下行配置*/
    SEGGER_RTT_ConfigDownBuffer(0, "RTT_DW", NULL, 0, SEGGER_RTT_MODE_NO_BLOCK_SKIP);
    
    rtt_printf(RTT_OS, "\n*****rtt viewer list*****\n");
    rtt_printf(RTT_OS, "  RTT_OS --------- %d\n", RTT_OS);
    rtt_printf(RTT_OS, "  RTT_ADC -------- %d\n", RTT_ADC);
    rtt_printf(RTT_OS, "  RTT_RF --------- %d\n", RTT_RF);
    rtt_printf(RTT_OS, "  RTT_CAT1 ------- %d\n", RTT_CAT1);
    rtt_printf(RTT_OS, "  RTT_PC --------- %d\n", RTT_PC);
    rtt_printf(RTT_OS, "  RTT_FPGA ------- %d\n", RTT_FPGA);
    rtt_printf(RTT_OS, "  RTT_MODEM ------ %d\n", RTT_MODEM);
    rtt_printf(RTT_OS, "  RTT_BLE -------- %d\n", RTT_BLE);
    rtt_printf(RTT_OS, "  RTT_FLASH ------ %d\n", RTT_FLASH);
    rtt_printf(RTT_OS, "  RTT_UPGRADE ---- %d\n", RTT_UPGRADE);
    rtt_printf(RTT_OS, "  RTT_TESTMODE --- %d\n", RTT_TESTMODE);
    rtt_printf(RTT_OS, "  RTT_4G_ISO ----- %d\n", RTT_4G_ISO);
    rtt_printf(RTT_OS, "  RTT_MONITOR ----- %d\n", RTT_MONITOR);
    rtt_printf(RTT_OS, "  RTT_DRIVERS ---- %d\n", RTT_DRIVERS);
    rtt_printf(RTT_OS, "  RTT_8668 ------- %d\n", RTT_8668);
    rtt_printf(RTT_OS, "  RTT_LAST ------- %d\n", RTT_LAST);
    rtt_printf(RTT_OS, "*************************\n");
}
