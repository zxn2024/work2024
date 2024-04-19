/***************************** Copyright ****************************************
 *
 * (C) Copyright 2024, Shaanxi Tianji Communication
 *  All Rights Reserved
 *
 * FileName   : app_monitor.c
 * Version    : none
 * Author     :
 * Date       : 2024-3-5
 * Description: none
 *******************************************************************************/
#include "main.h"
#include "app_monitor.h"
#include "bsp_config.h"
#include "msg_handle.h"
#include "monitor_para_list.h"
#include "ascii_hex.h"
#include "app_monitor_report.h"

#include "rtt.h"
/*---------------------- 端口定义 --------------------------------------*/

/*---------------------- 宏定义  ---------------------------------------*/
#define MONITOR_RECONNECT_TIME      MIN_TO_TICKS(1)

/*---------------------- 结构体 ----------------------------------------*/

/*---------------------- 常量 ------------------------------------------*/

/*---------------------- 全局变量 --------------------------------------*/

/*---------------------- 局部变量 --------------------------------------*/
#define PDU_MAX_LEN (1200)         // 报文最长支持 1200 Bytes
uint8_t mon_rcv_buf[PDU_MAX_LEN];  // 存储平台发送过来的数据
uint8_t mon_temp_buf[PDU_MAX_LEN]; // pc缓存数据
uint8_t mon_tx_buf[PDU_MAX_LEN];   // 存储回包
uint8_t mon_recon_flag = B_FALSE;  // 监控重连标志
uint8_t mon_recon_start_count = B_FALSE;    // 监控重连开始计时

/*---------------------- 声明 ------------------------------------------*/

/*---------------------- 函数 ------------------------------------------*/
/*****************************************************************************
 * @Name   : monitor_in_timer_isr
 * @Brief  : 链接定时器，在定时器中调用
 * @Param  : None
 * @Retval : None
 * @Author :
 * @Date   : 2024-04-14
 * @Note   :
 */
void monitor_in_timer_isr(void)
{
    static uint32_t mon_recon_timer_cnt = 0;
    if (mon_recon_start_count == B_TRUE) {
        if (mon_recon_timer_cnt >= MONITOR_RECONNECT_TIME) {
            mon_recon_flag = B_TRUE;
            mon_recon_start_count = B_FALSE;
            mon_recon_timer_cnt = 0;
        }
        mon_recon_timer_cnt++;
    }
}

/*****************************************************************************
 * @Name   : monitor_reconnect
 * @Brief  : 监控重连
 * @Param  : None
 * @Retval : None
 * @Author :
 * @Date   : 2024-04-14
 * @Note   :
 */
void monitor_reconnect(void)
{
    mon_recon_start_count = B_TRUE;
}

/*****************************************************************************
 * @Name	  : monitor_msg_in_task
 * @Brief  : 在任务中调用的监控消息处理函数
 * @Param  : None
 * @Retval :
 * @Author :
 * @Data   : 2024-3-5
 * @Note   :
 *****************************************************************************/
void monitor_msg_in_task(void)
{
    uint16_t tx_pdu_len = 0;
    uint16_t rx_len = 0;
    uint8_t cnt = 0;

    if (mon_recon_flag == B_TRUE) {
        mon_recon_flag = B_FALSE;
        socket_disconnect(LINK0);   // 断开连接
    }

    monitor_report_handle();

    // 接收数据
    rx_len = cat1_recv_msg(SOCKET_LINK_USER, mon_rcv_buf);
    if (rx_len == 0)
        return;

    memset(mon_temp_buf, 0, sizeof(mon_temp_buf));

    // 解析报文类型
    memset(mon_tx_buf, 0, sizeof(mon_tx_buf));

    rtt_printf(RTT_CAT1, RTT_CTRL_TEXT_BRIGHT_GREEN"\r\nCat1 Rx MCP:");
    for (uint16_t i = 0; i < rx_len; i++) {
        rtt_printf(RTT_CAT1, RTT_CTRL_TEXT_BRIGHT_GREEN"%02x", mon_rcv_buf[i]);
    }
    rtt_printf(RTT_CAT1, "\r\n");

    tx_pdu_len = MCP_rx_pkt_analysis(mon_rcv_buf, rx_len, mon_temp_buf, MCP_MONITOR);

    // 将报文发送给监控平台
    if (tx_pdu_len)
    {
        t_algo_hex_2_ascii(mon_temp_buf, (char *)mon_tx_buf, tx_pdu_len);
        while (RET_OK != cat1_send_msg(SOCKET_LINK_USER, (uint8_t *)mon_tx_buf, strlen((char *)mon_tx_buf)))
        {
            if (cnt >= 3)
                break;
            cnt++;
            vTaskDelay(100);
        }
    }
}
