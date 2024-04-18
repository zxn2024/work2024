/**
 * **********************************************************************************
 * @copyright Copyright (c) 2023 Shaanxi Tianji Communication Technologies Co., Ltd.
 *                         
 * @file        os_event.c   
 * @author      WangJun
 * @version     None
 * @date        2023-06-16     
 * @brief       None   
 * 
 * **********************************************************************************
 */

#include "os_event.h"
#include "rtt.h"
#include "bsp_rf_time.h"
#include "bsp_config.h"
#include "bsp_reboot.h"
/*---------------------- 端口定义 ----------------------*/


/*---------------------- 宏 定 义 ----------------------*/
/**
  * @brief 串口接收事件组
  */
EventGroupHandle_t xUartNewMsgEgp = NULL;

/**
  * @brief adc事件组
  */
EventGroupHandle_t xAdcEgp = NULL;  //adc事件组
/*---------------------- 结 构 体 ----------------------*/


/*---------------------- 常    量 ----------------------*/


/*---------------------- 全局变量 ----------------------*/


/*---------------------- 局部变量 ----------------------*/


/*---------------------- 函    数 ----------------------*/
/*****************************************************************************
* @brief  : 创建事件组
* @param  : None
* @retval : None
* @date   : 2023-06-20
* @note   : 
*****************************************************************************/
void os_eventgroup_create(void)
{
    /* UART新消息事件组 */
    xUartNewMsgEgp  = xEventGroupCreate();
    
	reboot_egp_creat();
    
    if(xUartNewMsgEgp == NULL)
    {
        rtt_printf(RTT_OS, RTT_TEXT_RED"[ERROR] xUartNewMsgEgp create fail\r\n");
    }
    else
    {
        xEventGroupClearBits(xUartNewMsgEgp, BIT_UART_ALL_MSG);
    }
    
    /* ADC采样事件组 */
    xAdcEgp  = xEventGroupCreate();
    
    if(xAdcEgp == NULL)
    {
        rtt_printf(RTT_OS, RTT_TEXT_RED"[ERROR] xAdcEgp create fail\r\n");
    }
    else
    {
        xEventGroupClearBits(xAdcEgp, BIT_ADC_ALL);
    }

    cat1_egp_creat();
    vModemEgpCreat();
}
