/***************************** Copyright ****************************************
*
* (C) Copyright 2022, Shaanxi Tianji Communication
* All Rights Reserved
*
* FileName   : bsp_cat1.c
* Version    : None
* Author     : WangJun
* Date       : 2022-08-16
* Description:
***/
#include "cat1_N706_driver.h"
#include "os_timer.h"
#include "rtt.h"
#include "crc.h"
#include "bsp_reboot.h"
#include "bsp_wdg.h"
#include "hi_gpio.h"
#include "hi_systick.h"
#include "hi_iwdg.h"
#include "rtt.h"
#include "ascii_hex.h"

/*---------------------- 宏定义  ---------------------------------------*/
#define _USER_RTOS_ (1)

// xSocketTimeEgp
#define BIT_SOCKET_TIME_RTC_QUERY (1U << 3)           // 查询RTC时间
#define BIT_SOCKET_TIME_PDP_CHECK (1U << 4)           // 检查PDP激活状态
#define BIT_SOCKET_TIME_LINK_CHECK (1U << 5)          // 检查Socket连接状态
#define BIT_SOCKET_TIME_CELLINFO_QUARY (1U << 6)      // 查询小区信息状态
#define BIT_SOCKET_TIME_DELAY_EFFECT (1U << 10)       // 参数设置延迟生效开始计数
#define BIT_SOCKET_TIME_DELAY_EFFECT_ALARM (1U << 11) // 参数设置延迟生效到时闹钟
#define BIT_SOCKET_SKIP_PDP_CHECK (1U << 12)           // 收到数据包,跳过一次pdp查询
#define BIT_SOCKET_SKIP_SOCKET0_CHECK (1U << 13)        // 收到数据包,跳过一次socket0查询
#define BIT_SOCKET_SKIP_SOCKET1_CHECK (1U << 14) // 收到数据包,跳过一次socket1查询
#define BIT_SOCKET_SKIP_RTC_QUARY (1U << 15) // 收到数据包,跳过一次RTC查询

#define BIT_SOCKET_TIME_ALL (0x00FFFFFF)
/*---------------------- 结构体 ----------------------------------------*/

cat1_n706_T hCat1; // CAT1参数信息
uart_func_t *cat1_uart = NULL;
static uint8_t *p_tx_buf = NULL;
static uint8_t *p_rx_buf = NULL;

/*---------------------- 常量 ------------------------------------------*/
/* 定时器计数 */
#define RTC_QUERY_CNT (MIN_TO_TICKS(2))       // 间隔固定时间查询模块RTC时间, 2MIN 1次
#define CELL_INFO_QUARY_CNT (MIN_TO_TICKS(5)) // 间隔固定时间查询小区信息, 2MIN 1次
#define SOCKET_RELINK_CNT (MIN_TO_TICKS(30))  // 间隔固定时间重新连接服务器, 30MIN 1次
#define PDP_CHECK_CNT (S_TO_TICKS(59))        // 间隔固定时间查询PDP连接状况, 30s 1次
#define LINK_CHECK_CNT (S_TO_TICKS(61))       // 间隔固定时间查询socket连接状态, 30s 1次

#define SOCKET_PARA_EFFECT_CNT (MIN_TO_TICKS(10)) // 监控参数设置后延迟10min生效

#define RESTART_RETRY_TICK (10 * 1000) // 重启延时

#define AT_CMD_MAX_ERROR_CNT (5)   // AT指令无回复最大次数
#define SEND_CMD_MAX_ERROR_CNT (5) // Socket send data最大错误次数
/*---------------------- 全局变量 --------------------------------------*/

/**
 * @brief 监控链接时间事件组
 *     @arg：bit0 无
 *     @arg：bit1 Socket0重连服务器计时使能
 *     @arg：bit2 Socket0重连服务器
 *     @arg：bit3 查询RTC时间
 */
EventGroupHandle_t xSocketTimeEgp = NULL;

/*---------------------- 局部变量 --------------------------------------*/

/*---------------------- 声明 ------------------------------------------*/
/**********************************************************************
* 函数名称：UTCToBeijing
* 功能描述：UTCToBeijing
* 访问的表：无
* 修改的表：无
* 输入参数：uint32_t y,uint32_t m,uint32_t d,uint32_t h,uint32_t min,uint32_t s
* 输出参数：无
* 返 回 值：无
* 其它说明：无
* 修改日期      版本号  修改人      修改内容
* ---------------------------------------------------------------------
* 2021/12/13    V1.0                                          创建
************************************************************************/
//UTC转换为北京时间 
void UTCToBeijing(uint32_t y,uint32_t m,uint32_t d,uint32_t h,uint32_t min,uint32_t s)
{
		int year=0,month=0,day=0,hour=0;
		int lastday = 0;// 月的最后一天日期
				 
		year=2000+y;
		month=m;
		day=d;
		hour=h+8;//UTC+8转换为北京时间
		 
		if(month==1 || month==3 || month==5 || month==7 || month==8 || month==10 || month==12)
		{
				 lastday = 31;
		}else	if(month == 4 || month == 6 || month == 9 || month == 11)
		{
						lastday = 30;
		}else
		{
				if((year%400 == 0)||(year%4 == 0 && year%100 != 0))//闰年的2月为29天，平年为28天
								lastday = 29;
				else
								lastday = 28;
		}
		if(hour >= 24)//当算出的时大于或等于24：00时，应减去24：00，日期加一天
		{
					hour -= 24;
					day += 1; 
					if(day > lastday)//当算出的日期大于该月最后一天时，应减去该月最后一天的日期，月份加上一个月
					{ 
					  day -= lastday;
					  month += 1;
					 
						if(month > 12)//当算出的月份大于12，应减去12，年份加上1年
						{
						  month -= 12;
						  year += 1;
						}
					}
		}

		hCat1.tRtcTime.year = year-2000;
	  hCat1.tRtcTime.month=(uint8_t)month;
	  hCat1.tRtcTime.day =(uint8_t)day;
	  hCat1.tRtcTime.hour =(uint8_t)hour;
	  hCat1.tRtcTime.minute =(uint8_t)min;
	  hCat1.tRtcTime.second =(uint8_t)s;
}
/*****************************************************************************
 * @Name	  : cat1_n706_register
 * @Brief  : 注册cat1设备
 * @Param  : hw_config cat1设备硬件资源分配,包括GPIO Uart
 * @Retval : None
 * @Note   :
 */
void cat1_n706_register(cat1_n706_hw_config_t *hw_config)
{
    cat1_uart = hw_config->uart_func;
    p_tx_buf = cat1_uart->tpt_data->pt_tx_data;
    p_rx_buf = cat1_uart->tpt_data->pt_rx_data;

    memset(&hCat1, 0, sizeof(hCat1));
    memcpy(&hCat1.cat1_hw_config, hw_config, sizeof(cat1_n706_hw_config_t));
    hCat1.ucSimState = SIM_OUT;

    hCat1.cat1_stat = CAT1_INIT;
    hCat1.cat1_en = B_TRUE;
    hCat1.socketStat[0].socket_stat = SOCKET_START;
    hCat1.socketStat[1].socket_stat = SOCKET_START;
}
/*****************************************************************************
 * @Name	  : vOmcParaInit
 * @Brief  : OMC相关参数初始化
 * @Param  : None
 * @Retval : None
 * @Author :
 * @Date   : 2022-08-16
 * @Note   :
 */

#if (CAT1_HW_TEST == 1)
void cat1_omc_para_set(omc_t *omc_param) // 指定一个测试服务器
{
    memcpy(hCat1.apn, "cmnet", strlen("cmnet"));
	 //memcpy(hCat1.apn, "ctnet", strlen("ctnet"));
    memset(&hCat1.socketConfig[0], 0, sizeof(omc_param->ip[0]));
    memset(&hCat1.socketConfig[1], 0, sizeof(omc_param->ip[1]));
	
    hCat1.socketConfig[0].ip_type = 1;      // 1:ipv4     2:ipv6
    hCat1.socketConfig[0].tcp_or_udp = TCP; // TCP
    hCat1.socketConfig[0].ipv4_addr[0] = 39;
    hCat1.socketConfig[0].ipv4_addr[1] = 106;
    hCat1.socketConfig[0].ipv4_addr[2] = 42;
    hCat1.socketConfig[0].ipv4_addr[3] = 94;
    hCat1.socketConfig[0].port = 6666;
    hCat1.socketConfig[0].use_password_en = B_FALSE;

  	hCat1.socketConfig[1].ip_type = 1;      // 1:ipv4     2:ipv6    3：ipv4v6
    hCat1.socketConfig[1].tcp_or_udp = TCP; // TCP 
    hCat1.socketConfig[1].ipv4_addr[0] = 123;
    hCat1.socketConfig[1].ipv4_addr[1] = 139;
    hCat1.socketConfig[1].ipv4_addr[2] = 59;
    hCat1.socketConfig[1].ipv4_addr[3] = 166;
    hCat1.socketConfig[1].port = 6666;
    hCat1.socketConfig[1].use_password_en = B_FALSE;

    char *s1 = "123.139.59.166";
    memcpy(&hCat1.socketConfig[0].ipv6_addr[0], s1, sizeof(hCat1.socketConfig[0].ipv6_addr));
    rtt_printf(RTT_CAT1, RTT_CTRL_TEXT_BRIGHT_YELLOW "hCat1.socketConfig[0].ipv6_addr=%s\r\n", hCat1.socketConfig[0].ipv6_addr);

    char *s2 = "123.139.59.165";
    memcpy(&hCat1.socketConfig[1].ipv6_addr[0], s2, sizeof(hCat1.socketConfig[1].ipv6_addr));
    rtt_printf(RTT_CAT1, RTT_CTRL_TEXT_BRIGHT_YELLOW "hCat1.socketConfig[1].ipv6_addr=%s\r\n", hCat1.socketConfig[1].ipv6_addr);
}
#else
void cat1_omc_para_set(omc_t *omc_param) // flash参数读取放到flash配置读取功能里调用
{
    memcpy(hCat1.apn, omc_param->apn, sizeof(omc_param->apn));
    memcpy(&hCat1.socketConfig[0], &omc_param->ip[0], sizeof(omc_param->ip[0]));
    memcpy(&hCat1.socketConfig[1], &omc_param->ip[1], sizeof(omc_param->ip[1]));
    
}
#endif

/*****************************************************************************
 * @Name	  : cat1_open
 * @Brief  : 开机
 * @Param  : None
 * @Retval : None
 * @Author :
 * @Date   : 2022-08-16
 * @Note   :
 */
static void cat1_open(void)
{
    hi_gpio_write(hCat1.cat1_hw_config.cat1_open_port, hCat1.cat1_hw_config.cat1_open_pin, LEVEL_H);
#if _USER_RTOS_
    vTaskDelay(2500);
#else
    hi_delay_ms(2000);
#endif
    hi_gpio_write(hCat1.cat1_hw_config.cat1_open_port, hCat1.cat1_hw_config.cat1_open_pin, LEVEL_L);
}

/*****************************************************************************
 * @Name	  : cat1_rst
 * @Brief  : 复位
 * @Param  : None
 * @Retval : None
 * @Author :
 * @Date   : 2022-08-16
 * @Note   : 低电平复位
 */
static void cat1_rst(void)
{
    hi_gpio_write(hCat1.cat1_hw_config.cat1_reset_port, hCat1.cat1_hw_config.cat1_reset_pin, LEVEL_H);
#if _USER_RTOS_
    vTaskDelay(500);
#else
    hi_delay_ms(100);
#endif
    hi_gpio_write(hCat1.cat1_hw_config.cat1_reset_port, hCat1.cat1_hw_config.cat1_reset_pin, LEVEL_L);
}

/*****************************************************************************
 * @Name	  : cat1_pwron
 * @Brief  : 复位
 * @Param  : None
 * @Retval : None
 * @Author :
 * @Date   : 2022-08-16
 * @Note   : 低电平复位
 */
void cat1_pwron(void) 
{
    hi_gpio_write(hCat1.cat1_hw_config.cat1_dc_ctrl_port, hCat1.cat1_hw_config.cat1_dc_ctrl_pin, LEVEL_L);
    vTaskDelay(1500);

    hi_gpio_write(hCat1.cat1_hw_config.cat1_dc_ctrl_port, hCat1.cat1_hw_config.cat1_dc_ctrl_pin, LEVEL_H);
    vTaskDelay(1500);
}

/*****************************************************************************
 * @Name	  : cat1_on_off
 * @Brief  : cat1开关控制,用于测试模式,关闭后彻底停止cat1工作
 * @Param  : None
 * @Retval : None
 * @Author :
 * @Date   : 2022-08-16
 */
void cat1_on_off(uint8_t on_off) 
{
    if(on_off == 0)
    {
        hi_gpio_write(hCat1.cat1_hw_config.cat1_dc_ctrl_port, hCat1.cat1_hw_config.cat1_dc_ctrl_pin, LEVEL_L);
        hCat1.cat1_en = B_FALSE;
        hCat1.cat1_stat = CAT1_INIT;
    }else
    {
        hCat1.cat1_en = B_TRUE;
        hCat1.cat1_stat = CAT1_INIT;
        hCat1.socketStat[0].socket_stat = SOCKET_START;
        hCat1.socketStat[1].socket_stat = SOCKET_START;
    }
}

/*****************************************************************************
 * @Name	  : cat1_state_read
 * @Brief  : 状态检测
 * @Param  : None
 * @Retval : CAT1状态
 * @Author :
 * @Date   : 2022-08-16
 * @Note   :
 */
static ELEC_LEVEL_E cat1_state_read(void)
{
    return hi_gpio_read(hCat1.cat1_hw_config.cat1_stat_port, hCat1.cat1_hw_config.cat1_stat_pin);
}

/*****************************************************************************
 * @Name	  : vCatRxDataClear
 * @Brief  : 清空接收数据缓冲区
 * @Param  : None
 * @Retval : None
 * @Author :
 * @Date   : 2022-08-16
 * @Note   :
 */
static void cat1_rx_data_clear(void)
{
    cat1_uart->rx_data_clr(cat1_uart->tpt_data);
}

/*****************************************************************************
 * @Name	  : vCatTxDataClear
 * @Brief  : 清空发送数据缓冲区
 * @Param  : None
 * @Retval : None
 * @Author :
 * @Date   : 2022-08-16
 * @Note   :
 */
static void cat1_tx_data_clear(void)
{
    cat1_uart->tx_data_clr(cat1_uart->tpt_data);
}

/*****************************************************************************
 * @Name	  : vCatTxDataClear
 * @Brief  : 清空发送数据缓冲区
 * @Param  : None
 * @Retval : None
 * @Author :
 * @Date   : 2022-08-16
 * @Note   :
 */
static void cat1_store_Atcmd_rx_pack(uint8_t *cmd_data, uint16_t len)
{
    if (len > sizeof(hCat1.rx_cmd_buf.buf))
    {
        rtt_printf(RTT_CAT1, RTT_CTRL_TEXT_BRIGHT_RED "ERROR: cat1 rx data len =%d\r\n", len);
        return;
    }
    memset(&hCat1.rx_cmd_buf.buf, 0, sizeof(hCat1.rx_cmd_buf.buf));
    memcpy(&hCat1.rx_cmd_buf.buf, cmd_data, len);
    hCat1.rx_cmd_buf.rx_len = len;
}


/**********************************************************
 *函数名：   cat1_try_lock
 *函数功能： 锁定
 *返回值：   STAT_ERROR STAT_OK
 */
Status_t cat1_try_lock(cat1_n706_T * cat1)
{
    if (cat1->lock == B_TRUE)
    {
        return STAT_ERROR;
    }else
    {
        cat1->lock = B_TRUE;
        return STAT_OK;
    }    
}

/**********************************************************
 *函数名：   cat1_wait_lock
 *函数功能： 锁等待
 *返回值：   STAT_ERROR STAT_OK
 */
Status_t cat1_wait_lock(cat1_n706_T * cat1, uint32_t timeout_ms)
{
    uint32_t time_cnt = 0;

    while(cat1->lock != B_FALSE)
    {
        vTaskDelay(10);
        time_cnt += 10;
        if(time_cnt >= timeout_ms)
        {
            break;
        }
    }

    if (cat1->lock == B_FALSE)
    {
        cat1->lock = B_TRUE;
        return STAT_OK;
    }else
    {
        return STAT_ERROR;
    }
}
/**********************************************************
 *函数名：   cat1_unlock
 *函数功能： 解锁
 *返回值：   无
 */
void cat1_unlock(cat1_n706_T * cat1)
{
    cat1->lock = B_FALSE;
}




/*****************************************************************************
 * @Name	  : vCatTxDataClear
 * @Brief  : 清空发送数据缓冲区
 * @Param  : None
 * @Retval : None
 * @Author :
 * @Date   : 2022-08-16
 * @Note   :
 */
static void cat1_clear_ATcmd_rx_pack(void)
{
    memset(&hCat1.rx_cmd_buf, 0, sizeof(hCat1.rx_cmd_buf));
}

/*****************************************************************************
 * @Name	  : cat1_wait_ATcmd_rx_pack
 * @Brief  : 等待串口AT指令的回包,超时时间wait_ms,返回剩余时间.
 * @Param  : None
 * @Retval : 返回剩余时间,超时返回0
 * @Author :
 * @Date   : 2022-08-16
 * @Note   :
 */
static uint32_t cat1_wait_ATcmd_rx_pack(uint32_t wait_ms)
{
    uint32_t time_cnt = 0, remain_ms = 1;
    while (hCat1.rx_cmd_buf.rx_len == 0)
    {
        vTaskDelay(10);
        time_cnt += 10;

        if (time_cnt >= wait_ms)
            return 0;
    }
    if (wait_ms > time_cnt)
        remain_ms = wait_ms - time_cnt;
    else
        remain_ms = 1;
    return remain_ms;
}

/*****************************************************************************
 * @Name	  : vCat1InitEgpCreat
 * @Brief  : 创建Cat1初始化事件组
 * @Param  : None
 * @Retval : None
 * @Author :
 * @Date   : 2022-08-17
 * @Note   :
 */
void cat1_egp_creat(void)
{
    xSocketTimeEgp = xEventGroupCreate();

    // Socket计时事件组
    if (xSocketTimeEgp == NULL)
    {
        /* 任务创建失败 */
        rtt_printf(RTT_CAT1, RTT_CTRL_TEXT_BRIGHT_RED "ERROR: xSocketTimeEgp Create Fail...\r\n");
    }
    else
    {
        rtt_printf(RTT_CAT1, RTT_CTRL_TEXT_BRIGHT_GREEN "xSocketTimeEgp Create Success!\r\n");
        xEventGroupClearBits(xSocketTimeEgp, BIT_SOCKET_TIME_ALL); // 清零
        rtt_printf(RTT_CAT1, RTT_CTRL_TEXT_BRIGHT_GREEN "xSocketTimeEgp Init Success!\r\n");
    }
}

/*****************************************************************************
 * @Name	  : cat1_send_AT_cmd
 * @Brief  : 监控模块AT交互指令
 * @Param  : ucCmdType: 指令类别
 * @Retval :
 * @Author :
 * @Date   : 2022-08-16
 * @Note   :
 */
static eRETURN cat1_send_AT_cmd(uint8_t ucCmdType)
{
    uint32_t MsToWait = 1000, wait_time_ms = 0;
    char *pStart = NULL, *pEnd = NULL;
    int32_t ul_para1 = 0; int32_t ul_para2=0;
    uint8_t *pServerIP = NULL;                          // IP
    uint16_t usPort = 0;                                // 端口号
//    uint32_t n = 0, stat = 0, lac = 0, ci = 0, act = 0; // 通过CREG获取CELL ID
//    uint32_t remain_time_ms_t0 = 0;
//    uint32_t remain_time_ms_t1 = 0;
    uint32_t remain_time_ms = 0;
    eRETURN RetValue = RET_ERROR;
	char freq_band_ind[20];
	char *pStart1 =NULL;
    char *pEnd1 =NULL;
	char *buff=NULL;
    
    if(STAT_OK != cat1_try_lock(&hCat1))    //上锁
    {
        rtt_printf(RTT_CAT1, "msg send lock!!!\r\n");
        if(STAT_OK != cat1_wait_lock(&hCat1,1000))  //等待超时退出
            return RET_NO_RESPONSE;
    }
	
    cat1_clear_ATcmd_rx_pack(); // 清空之前未处理的指令回包

    switch (ucCmdType)
    {
    case CAT1_CMD_CHECK_AT:
        sprintf((char *)p_tx_buf, "%s", "AT\r\n");
        wait_time_ms = MsToWait;
        break;

    case CAT1_CMD_CLOSE_ECHO:
        sprintf((char *)p_tx_buf, "%s", "ATE0\r\n");
        wait_time_ms = MsToWait;
        break;

    case CAT1_CMD_QUERY_CPIN:
        sprintf((char *)p_tx_buf, "%s", "AT+CPIN?\r\n");
        wait_time_ms = MsToWait * 5;
        break;

    case CAT1_CMD_SET_CREG:
        sprintf((char *)p_tx_buf, "%s", "AT+CREG=2\r\n");
        wait_time_ms = MsToWait;
        break;

    case CAT1_CMD_QUERY_CREG:
        sprintf((char *)p_tx_buf, "%s", "AT+CREG?\r\n");
        wait_time_ms = MsToWait;
        break;

	 case CAT1_CMD_QUERY_CGREG:  
	      sprintf((char *)p_tx_buf, "%s", "AT+CGREG?\r\n"); //查询PS业务
        wait_time_ms = MsToWait;
        break;

    case CAT1_CMD_SET_APN:
        if (B_TRUE == hCat1.socketConfig[0].use_password_en)
        {
            sprintf((char *)p_tx_buf, "AT+NETAPN=\"%s\",\"%s\",\"%s\"\r\n", hCat1.apn, hCat1.socketConfig[0].username, hCat1.socketConfig[0].password);
        }
        else
        {
            sprintf((char *)p_tx_buf, "AT+NETAPN=\"%s\",\"\",\"\"\r\n", hCat1.apn);
        }
        wait_time_ms = MsToWait;
        break;

    case CAT1_CMD_QUARY_PDPACT:
        sprintf((char *)p_tx_buf, "%s", "AT+XIIC?\r\n");
        wait_time_ms = MsToWait * 1;
        break;

    case CAT1_CMD_SET_PDPACT:
        sprintf((char *)p_tx_buf, "%s", "AT+XIIC=1\r\n");
        wait_time_ms = MsToWait;
        break;

    case CAT1_CMD_SET_PDPDEACT:
        sprintf((char *)p_tx_buf, "%s", "AT+XIIC=0\r\n");
        wait_time_ms = MsToWait * 40;
        break;

		case CAT1_CMD_SOCKET_OPEN0:
        if (TCP == hCat1.socketConfig[0].tcp_or_udp) // 连接类型判断
        {
            switch (hCat1.socketConfig[0].ip_type) // IP类型
            {
             case IPV4: // IPv4
                pServerIP = hCat1.socketConfig[0].ipv4_addr;
                usPort = hCat1.socketConfig[0].port;
                sprintf((char *)p_tx_buf, "AT+TCPSETUP=0,%d.%d.%d.%d,%d\r\n",pServerIP[0], pServerIP[1], pServerIP[2], pServerIP[3],usPort);
                break;

             case IPV6: // IPv6
						    pServerIP = hCat1.socketConfig[0].ipv6_addr;
                usPort = hCat1.socketConfig[0].port;
						    sprintf((char *)p_tx_buf, "AT+TCPSETUP=0,%s,%d\r\n",pServerIP,usPort);
                break;

            default: // None
                RetValue = RET_CMD_ERROR;
                goto send_at_end;
            }
            wait_time_ms = MsToWait * 150;  //150
            break;
        }
        else if (UDP == hCat1.socketConfig[0].tcp_or_udp)
        {
					  switch (hCat1.socketConfig[0].ip_type) // IP类型
            {
            case IPV4: // IPv4
                pServerIP = hCat1.socketConfig[0].ipv4_addr;
                usPort = hCat1.socketConfig[0].port;
                sprintf((char *)p_tx_buf, "AT+UDPSETUP=0,%d.%d.%d.%d,%d\r\n",pServerIP[0], pServerIP[1], pServerIP[2], pServerIP[3],usPort);
                break;

            case IPV6: // IPv6
						    pServerIP = hCat1.socketConfig[0].ipv6_addr;
                usPort = hCat1.socketConfig[0].port;
						    sprintf((char *)p_tx_buf, "AT+UDPSETUP=0,%s,%d\r\n",pServerIP,usPort);
                break;

            default: // None
                RetValue = RET_CMD_ERROR;
                goto send_at_end;
            }
            wait_time_ms = MsToWait * 150;
	
            break;
        }
        else
        {
            RetValue = RET_CMD_ERROR;
            goto send_at_end;
        }
			

    case CAT1_CMD_SOCKET_OPEN1:
        if (TCP == hCat1.socketConfig[1].tcp_or_udp) // 连接类型判断
        {
					switch (hCat1.socketConfig[1].ip_type) // IP类型
            {
            case IPV4: // IPv4
                pServerIP = hCat1.socketConfig[1].ipv4_addr;
                usPort = hCat1.socketConfig[1].port;
						    
                sprintf((char *)p_tx_buf, "AT+TCPSETUP=1,%d.%d.%d.%d,%d\r\n",pServerIP[0], pServerIP[1], pServerIP[2], pServerIP[3],usPort);
                break;

            case IPV6: // IPv6
							  pServerIP = hCat1.socketConfig[1].ipv6_addr;
                usPort = hCat1.socketConfig[1].port;
						
                sprintf((char *)p_tx_buf, "AT+TCPSETUP=1,%s,%d\r\n",pServerIP,usPort);
                break;

            default: // None
                RetValue = RET_CMD_ERROR;
                goto send_at_end;
            }
            wait_time_ms = MsToWait * 150;
            break;
        }
        else if (UDP == hCat1.socketConfig[1].tcp_or_udp)
        {
            //TODO: UDP连接
					  switch (hCat1.socketConfig[1].ip_type) // IP类型
            {
            case IPV4: // IPv4
                pServerIP = hCat1.socketConfig[1].ipv4_addr;
                usPort = hCat1.socketConfig[1].port;
                sprintf((char *)p_tx_buf, "AT+UDPSETUP=1,%d.%d.%d.%d,%d\r\n",pServerIP[0], pServerIP[1], pServerIP[2], pServerIP[3],usPort);
                break;

            case IPV6: // IPv6
						    pServerIP = hCat1.socketConfig[1].ipv6_addr;
                usPort = hCat1.socketConfig[1].port;
						    sprintf((char *)p_tx_buf, "AT+UDPSETUP=1,%s,%d\r\n",pServerIP,usPort);
                break;

            default: // None
                RetValue = RET_CMD_ERROR;
                goto send_at_end;
            }
            wait_time_ms = MsToWait * 150;

            break;
        }
        else
        {
            RetValue = RET_CMD_ERROR;
            goto send_at_end;
        }

    case CAT1_CMD_SOCKET_CLOSE0:
			    if (TCP == hCat1.socketConfig[0].tcp_or_udp) // 连接类型判断
					{
						//sprintf((char *)p_tx_buf, "AT+TCPCLOSE=0\r\n");	 
						sprintf((char *)p_tx_buf, "%s%d%s", "AT+TCPCLOSE=", hCat1.socketStat[0].socketID, "\r\n");
					}else if (UDP == hCat1.socketConfig[0].tcp_or_udp)
					{
						//sprintf((char *)p_tx_buf, "AT+UDPCLOSE=0\r\n");	
					  sprintf((char *)p_tx_buf, "%s%d%s", "AT+UDPCLOSE=", hCat1.socketStat[0].socketID, "\r\n");
					}	else{}		 
								 
        
        wait_time_ms = MsToWait * 2;
        break;

    case CAT1_CMD_SOCKET_CLOSE1:
					 if (TCP == hCat1.socketConfig[1].tcp_or_udp) // 连接类型判断
					{
						//sprintf((char *)p_tx_buf, "AT+UDPCLOSE=1\r\n");	 
						sprintf((char *)p_tx_buf, "%s%d%s", "AT+TCPCLOSE=", hCat1.socketStat[1].socketID, "\r\n");
					}else if (UDP == hCat1.socketConfig[1].tcp_or_udp)
					{
						//sprintf((char *)p_tx_buf, "AT+UDPCLOSE=1\r\n");	
					  sprintf((char *)p_tx_buf, "%s%d%s", "AT+UDPCLOSE=", hCat1.socketStat[1].socketID, "\r\n");
					}	else{}	
						
        wait_time_ms = MsToWait * 2;
        break;

    case CAT1_CMD_SOCKET_STAT0:
        sprintf((char *)p_tx_buf, "AT+IPSTATUS=0\r\n");
        wait_time_ms = MsToWait;
        break;

    case CAT1_CMD_SOCKET_STAT1:
        sprintf((char *)p_tx_buf, "AT+IPSTATUS=1\r\n");
        wait_time_ms = MsToWait;
        break;

    case CAT1_CMD_QUERY_SIM_ID:
        sprintf((char *)p_tx_buf, "%s", "AT+CCID\r\n");
        wait_time_ms = MsToWait;
        break;

    case CAT1_CMD_QUERY_RTC:
        sprintf((char *)p_tx_buf, "%s", "AT+CCLK?\r\n");
        wait_time_ms = MsToWait;
        break;

    case CAT1_CMD_QUERY_SW_VER:
        sprintf((char *)p_tx_buf, "%s", "AT+GMR\r\n");
        wait_time_ms = MsToWait;
        break;

    case CAT1_CMD_QUERY_HW_VER:
        sprintf((char *)p_tx_buf, "%s", "AT+CGMM\r\n");
        wait_time_ms = MsToWait;
        break;

		  case CAT1_CMD_QUERY_QNWINFO:
		    sprintf((char *)p_tx_buf, "%s", "AT+NETMSG\r\n");  //查询注册网络的详细信息
        wait_time_ms = MsToWait;
        break;

     case MONITOR_CMD_QUERY_QENG:
		    sprintf((char *)p_tx_buf, "%s", "AT+NETDMSG\r\n");   //查询当前注册网络信息
        wait_time_ms = MsToWait;
        break;
		
    default:
        rtt_printf(RTT_CAT1, "no such cmd!!!\r\n");
        RetValue = RET_ERROR;
        goto send_at_end;
    }
    cat1_uart->send(cat1_uart->tpt_data, strlen((const char *)p_tx_buf));

    rtt_printf(RTT_CAT1, "Cat1 send cmd:%s", (const char *)p_tx_buf);

    vTaskDelay(500); // 此处需要增加延迟，否则串口接收容易出错

    remain_time_ms = cat1_wait_ATcmd_rx_pack(wait_time_ms);

    if (remain_time_ms > 0)
    {
        // cat1_uart->read(cat1_uart->tpt_data);
        // cat1_uart->clr_newmsg(cat1_uart->tpt_data);
        // rtt_printf(RTT_CAT1, "Cat1 Receive Data: %s\r\n", (const char *)hCat1.rx_cmd_buf.buf); // 打印接收的数据

        switch (ucCmdType)
        {
        case CAT1_CMD_CHECK_AT:
        case CAT1_CMD_CLOSE_ECHO:

        case CAT1_CMD_SET_CREG:
            if (0 != strstr((const char *)hCat1.rx_cmd_buf.buf, "OK"))
            {
                RetValue = RET_OK;
            }
            else
            {
                RetValue = RET_ERROR;
            }
            break;
        case CAT1_CMD_SOCKET_CLOSE0:
					
					 if (TCP == hCat1.socketConfig[0].tcp_or_udp) // 连接类型判断
			     {
					  pStart = strstr((const char *)hCat1.rx_cmd_buf.buf, "+TCPCLOSE: 0,OK");
            if (NULL != pStart)
            {
                RetValue = RET_OK;
            }
            else
            {
                RetValue = RET_ERROR;
            }
					}else if (UDP == hCat1.socketConfig[0].tcp_or_udp)
					 {
					   pStart = strstr((const char *)hCat1.rx_cmd_buf.buf, "+UDPCLOSE: 0,OK");
             if (NULL != pStart) 
             {
                RetValue = RET_OK;
             }
             else
             {
                RetValue = RET_ERROR;
             }					 
					 }else {}
            break;
        case CAT1_CMD_SOCKET_CLOSE1:
						if (TCP == hCat1.socketConfig[1].tcp_or_udp) // 连接类型判断
			     {
					  pStart = strstr((const char *)hCat1.rx_cmd_buf.buf, "+TCPCLOSE: 1,OK");
            if (NULL != pStart)
            {
                RetValue = RET_OK;
            }
            else
            {
                RetValue = RET_ERROR;
            }
					}else if (UDP == hCat1.socketConfig[1].tcp_or_udp)
					 {
					   pStart = strstr((const char *)hCat1.rx_cmd_buf.buf, "+UDPCLOSE: 1,OK");
             if (NULL != pStart) 
             {
                RetValue = RET_OK;
             }
             else
             {
                RetValue = RET_ERROR;
             }					 
					 }else {}
            break;
				
        case CAT1_CMD_QUERY_CPIN:
            if (0 != strstr((const char *)hCat1.rx_cmd_buf.buf, "READY"))
            {
                hCat1.ucSimState = SIM_IN;
                RetValue = RET_OK;
            }
            else
            {
                hCat1.ucSimState = SIM_OUT;
                RetValue = RET_ERROR;
            }
            break;

        case CAT1_CMD_QUERY_CREG:
					  pStart = strstr((const char *)hCat1.rx_cmd_buf.buf, "+CREG:");
            pEnd = strstr((const char *)hCat1.rx_cmd_buf.buf, "OK");
            if ((NULL != pStart) && (NULL != pEnd))
            {
                sscanf((const char *)hCat1.rx_cmd_buf.buf, "%*[^:]:%d,%d",&ul_para2,&ul_para1);
  						 if ((1 == ul_para1) || (5 == ul_para1))
                {
                    RetValue = RET_OK;
                }
                else
                {
                    RetValue = RET_ERROR;
                }
            }
            else
            {
                RetValue = RET_ERROR;
            }
            break;
									
			 case CAT1_CMD_QUERY_CGREG:
            pStart = strstr((const char *)hCat1.rx_cmd_buf.buf, "+CGREG:");
            pEnd = strstr((const char *)hCat1.rx_cmd_buf.buf, "OK");
            if ((NULL != pStart) && (NULL != pEnd))
            {
                sscanf((const char *)hCat1.rx_cmd_buf.buf, "%*[^:]:%*d,%d", &ul_para1);
                if ((1 == ul_para1) || (5 == ul_para1))
                {
                    RetValue = RET_OK;
                }
                else
                {
                    RetValue = RET_ERROR;
                }
            }
            else
            {
                RetValue = RET_ERROR;
            }
            break;

        case CAT1_CMD_SET_APN:
            if (0 != strstr((const char *)hCat1.rx_cmd_buf.buf, "OK"))
            {
                RetValue = RET_OK;
            }
            else
            {
                RetValue = RET_ERROR;
            }
            break;

        case CAT1_CMD_SET_PDPACT:
            if (0 != strstr((const char *)hCat1.rx_cmd_buf.buf, "OK"))
            {
                RetValue = RET_OK;
            }
            else
            {
                RetValue = RET_ERROR;
            }
            break;

        case CAT1_CMD_QUARY_PDPACT:
            if (0 != strstr((const char *)hCat1.rx_cmd_buf.buf, "+XIIC:    1"))
            {
                RetValue = RET_OK;
            }
            else
            {
                RetValue = RET_ERROR;
            }
            break;

        case CAT1_CMD_SET_PDPDEACT:
            if (0 != strstr((const char *)hCat1.rx_cmd_buf.buf, "OK"))
            {
                RetValue = RET_OK;
            }
            else
            {
                RetValue = RET_ERROR;
            }
            break;

						#if 0
        case CAT1_CMD_SOCKET_OPEN0:
            do
            {
							  remain_time_ms_t0 = cat1_wait_ATcmd_rx_pack(remain_time_ms); // 如果收到的不是期望的回包,时间还未完,就继续等
                remain_time_ms=remain_time_ms_t0;
							
                if (strstr((const char *)hCat1.rx_cmd_buf.buf, "ERROR")) // 当返回错误时,退出
                {
                    RetValue = RET_ERROR;
                    break;
                }
                if (strstr((const char *)hCat1.rx_cmd_buf.buf, "FAIL")) // 当返回错误时,退出
                {
                    RetValue = RET_ERROR;
                    break;
                }
								
                if(TCP ==hCat1.socketConfig[0].tcp_or_udp) //  连接类型判断
								{
									if (strstr((const char *)hCat1.rx_cmd_buf.buf, "+TCPSETUP: 0,OK") != NULL) //当收到socket连接成功时,退出并返回ok
									{
											RetValue = RET_OK;
											break;
									}
						  	}
								if(UDP ==hCat1.socketConfig[0].tcp_or_udp)
								{
									if (strstr((const char *)hCat1.rx_cmd_buf.buf, "+UDPSETUP: 0,OK") != NULL) //当收到socket连接成功时,退出并返回ok
									{
											RetValue = RET_OK;
											break;
									}
						  	}
								
                cat1_clear_ATcmd_rx_pack();

						} while (remain_time_ms_t0 > 0);
       					
            if (remain_time_ms_t0 == 0)
						{
							   rtt_printf(RTT_CAT1, RTT_CTRL_TEXT_BRIGHT_CYAN "Socket 0 openlink overtime");
                RetValue = RET_ERROR;
						}

            break; // 超时退出
		

        case CAT1_CMD_SOCKET_OPEN1:
            do
            {
							  remain_time_ms_t1 = cat1_wait_ATcmd_rx_pack(remain_time_ms); // 如果收到的不是期望的回包,时间还未完,就继续等
                remain_time_ms= remain_time_ms_t1;
                if (strstr((const char *)hCat1.rx_cmd_buf.buf, "ERROR")) // 当返回错误时,退出
                {
                    RetValue = RET_ERROR;
                    break;
                }
                if (strstr((const char *)hCat1.rx_cmd_buf.buf, "FAIL")) // 当返回错误时,退出
                {
                    RetValue = RET_ERROR;
                    break;
                }

								
                if(TCP ==hCat1.socketConfig[1].tcp_or_udp) //  连接类型判断
								{
									if (strstr((const char *)hCat1.rx_cmd_buf.buf, "+TCPSETUP: 1,OK") != NULL) //当收到socket连接成功时,退出并返回ok
									{
											RetValue = RET_OK;
											break;
									}
							  }
								if(UDP ==hCat1.socketConfig[1].tcp_or_udp)
								{
									if (strstr((const char *)hCat1.rx_cmd_buf.buf, "+UDPSETUP: 1,OK") != NULL) //当收到socket连接成功时,退出并返回ok
									{
											RetValue = RET_OK;
											break;
									}									
								}
                cat1_clear_ATcmd_rx_pack();

						} while (remain_time_ms_t1 > 0);

            if (remain_time_ms_t1 == 0)
                RetValue = RET_ERROR;

            break; // 超时退出
						
						#endif
		#if 1
        case CAT1_CMD_SOCKET_OPEN0:
            do
            {
					
                if (strstr((const char *)hCat1.rx_cmd_buf.buf, "ERROR")) // 当返回错误时,退出
                {
                    RetValue = RET_ERROR;
                    break;
                }
                if (strstr((const char *)hCat1.rx_cmd_buf.buf, "FAIL")) // 当返回错误时,退出
                {
                    RetValue = RET_ERROR;
                    break;
                }
								
                if(TCP ==hCat1.socketConfig[0].tcp_or_udp) //  连接类型判断
								{
									if (strstr((const char *)hCat1.rx_cmd_buf.buf, "+TCPSETUP: 0,OK") != NULL) //当收到socket连接成功时,退出并返回ok
									{
											RetValue = RET_OK;
											break;
									}
						  	}
								if(UDP ==hCat1.socketConfig[0].tcp_or_udp)
								{
									if (strstr((const char *)hCat1.rx_cmd_buf.buf, "+UDPSETUP: 0,OK") != NULL) //当收到socket连接成功时,退出并返回ok
									{
											RetValue = RET_OK;
											break;
									}
						  	}
								
                        cat1_clear_ATcmd_rx_pack();

						} while (remain_time_ms > 0);
       					
            if (remain_time_ms == 0)
						{
							   rtt_printf(RTT_CAT1, RTT_CTRL_TEXT_BRIGHT_CYAN "Socket 0 openlink overtime");
                RetValue = RET_ERROR;
						}

            break; // 超时退出
		

        case CAT1_CMD_SOCKET_OPEN1:
            do
            {
                if (strstr((const char *)hCat1.rx_cmd_buf.buf, "ERROR")) // 当返回错误时,退出
                {
                    RetValue = RET_ERROR;
                    break;
                }
                if (strstr((const char *)hCat1.rx_cmd_buf.buf, "FAIL")) // 当返回错误时,退出
                {
                    RetValue = RET_ERROR;
                    break;
                }

								
                if(TCP ==hCat1.socketConfig[1].tcp_or_udp) //  连接类型判断
								{
									if (strstr((const char *)hCat1.rx_cmd_buf.buf, "+TCPSETUP: 1,OK") != NULL) //当收到socket连接成功时,退出并返回ok
									{
											RetValue = RET_OK;
											break;
									}
							  }
								if(UDP ==hCat1.socketConfig[1].tcp_or_udp)
								{
									if (strstr((const char *)hCat1.rx_cmd_buf.buf, "+UDPSETUP: 1,OK") != NULL) //当收到socket连接成功时,退出并返回ok
									{
											RetValue = RET_OK;
											break;
									}									
								}
                        cat1_clear_ATcmd_rx_pack();

						} while (remain_time_ms > 0);

            if (remain_time_ms == 0)
                RetValue = RET_ERROR;

            break; // 超时退出
						
						#endif
						
        case CAT1_CMD_SOCKET_STAT0:
        case CAT1_CMD_SOCKET_STAT1:
            pStart = strstr((const char *)hCat1.rx_cmd_buf.buf, ",CONNECT"); // 查询socket连接状态

            if (pStart != NULL)
                RetValue = RET_OK;
            else
                RetValue = RET_ERROR;
            break;

        case CAT1_CMD_QUERY_SIM_ID:
            pStart = strstr((const char *)hCat1.rx_cmd_buf.buf, "+CCID:");
            pEnd = strstr((const char *)hCat1.rx_cmd_buf.buf, "OK");
            if ((pStart != NULL) && (pEnd != NULL))
            {
                pEnd = strstr((const char *)pStart + strlen("+CCID: "), "\r\n");
                if (pEnd != NULL)
                {
                    pStart += strlen("+CCID: ");
                    memcpy(hCat1.ucaSimNum, pStart, ((pEnd - pStart) > 32) ? 32 : (pEnd - pStart));
                    rtt_printf(RTT_CAT1, "SIM CCID %d: %s\r\n\r\n", (pEnd - pStart), (const char *)hCat1.ucaSimNum);
                    RetValue = RET_OK;
                }
                else
                {
                    RetValue = RET_ERROR;
                }
            }
            else
            {
                RetValue = RET_ERROR;
            }
            break;

        case CAT1_CMD_QUERY_RTC:
            pStart = strstr((const char *)hCat1.rx_cmd_buf.buf, "+CCLK:");
            pEnd = strstr((const char *)hCat1.rx_cmd_buf.buf, "OK");
            if ((pStart != NULL) && (pEnd != NULL))
            {
                uint32_t year = 0, month = 0, day = 0, hour = 0, minute = 0, second = 0;

                sscanf((const char *)hCat1.rx_cmd_buf.buf, "%*[^+CCLK:]+CCLK: \"%d/%d/%d,%d:%d:%d", &year, &month, &day, &hour, &minute, &second);

//                hCat1.tRtcTime.year = (uint8_t)year;
//                hCat1.tRtcTime.month = (uint8_t)month;
//                hCat1.tRtcTime.day = (uint8_t)day;
//                hCat1.tRtcTime.hour = ((uint8_t)hour + 8) % 24;
//                hCat1.tRtcTime.minute = (uint8_t)minute;
//                hCat1.tRtcTime.second = (uint8_t)second;
									  	UTCToBeijing(year,month,day,hour,minute,second);

                rtt_printf(RTT_CAT1, "Current Time: 20%d/%02d/%02d %02d:%02d:%02d\r\n\r\n",
                           hCat1.tRtcTime.year, hCat1.tRtcTime.month, hCat1.tRtcTime.day,
                           hCat1.tRtcTime.hour, hCat1.tRtcTime.minute, hCat1.tRtcTime.second);

                RetValue = RET_OK;
            }
            else
            {
                RetValue = RET_ERROR;
            }
            break;
						
        case CAT1_CMD_QUERY_SW_VER: // 获取软件版本信息
            pStart = strstr((const char *)hCat1.rx_cmd_buf.buf, "n706");
            pEnd = strstr((const char *)hCat1.rx_cmd_buf.buf, "OK");
            if ((pStart != NULL) && (pEnd != NULL))
            {
                pEnd = strstr((const char *)pStart, "\r\n");
                if (pEnd != NULL)
                {
                    rtt_printf(RTT_CAT1, "SW Version %d: %s\r\n", (pEnd - pStart), (const char *)pStart);
                    memcpy(hCat1.ucaCatSwVer, pStart, (pEnd - pStart));
                    RetValue = RET_OK;
                }
                else
                {
                    RetValue = RET_ERROR;
                }
            }
            else
            {
                RetValue = RET_ERROR;
            }
            break;

        case CAT1_CMD_QUERY_HW_VER: // 获取硬件版本信息
            pStart = strstr((const char *)hCat1.rx_cmd_buf.buf, "EC");
            pEnd = strstr((const char *)hCat1.rx_cmd_buf.buf, "OK");
            if ((pStart != NULL) && (pEnd != NULL))
            {
                pEnd = strstr((const char *)pStart, "\r\n");
                if (pEnd != NULL)
                {
                    rtt_printf(RTT_CAT1, "HW Version %d: %s\r\n", (pEnd - pStart), (const char *)pStart);
                    memcpy(hCat1.ucaCatHwVer, pStart, (pEnd - pStart));
                    RetValue = RET_OK;
                }
                else
                {
                    RetValue = RET_ERROR;
                }
            }
            else
            {
                RetValue = RET_ERROR;
            }
            break;

						
				case CAT1_CMD_QUERY_QNWINFO:  // 不实现
            pStart = strstr((const char *)hCat1.rx_cmd_buf.buf, "+NETMSG");
            pEnd = strstr((const char *)hCat1.rx_cmd_buf.buf, "OK");
            if ((pStart != NULL) && (pEnd != NULL))
            {
                RetValue = RET_OK;
            }
            else
            {
                RetValue = RET_ERROR;
            }
            break;

        case MONITOR_CMD_QUERY_QENG: //AT+NETMSG   AT+NETDMSG  
					#if 0
					+NETDMSG:<MCC+MNC>,[<LAC>/<TAC>],<CELL_ID>, [<BSIC>/<Phy_cellid>],<net_mode>,<BAND>,<ARFCN>,<RX_dBm>,<TX_dBm>,<SID>,<NID>,<BID>, 
<RSRP>,<RSRQ>,<SINR>
<CR><LF>OK<CR><LF>
				#endif
				    pStart = strstr((const char *)hCat1.rx_cmd_buf.buf, "+NETDMSG:");
            pEnd = strstr((const char *)hCat1.rx_cmd_buf.buf, "OK");
            if ((pStart != NULL) && (pEnd != NULL))
            {
							//处理mcc+mnc
							pStart1 = strstr((const char *)pStart, ":");
							pEnd1 = strstr((const char *)pStart, ",");
										
							buff=pStart;
							memcpy(buff, pStart1+3, (pEnd1-1 -(pStart1+3)));
							sscanf((const char *)buff, "%d+%d",  &hCat1.cell_info.mcc, &hCat1.cell_info.mnc);
										
							//处理其他参数
							sscanf((const char *)pStart, "%*[^: ]:  %*[^,],%x,%x,%d,%*[^,],%[^,],%d,%d,%d,%*[^,],%*[^,],%*[^,],%d,%d,%d",
							&hCat1.cell_info.tac, &hCat1.cell_info.cellid, &hCat1.cell_info.pci, 
							(char *)&freq_band_ind,&hCat1.cell_info.earfcn,&hCat1.cell_info.up_bandwitdh, &hCat1.cell_info.dw_bandwitdh, 
							 &hCat1.cell_info.rsrp, &hCat1.cell_info.rsrq, &hCat1.cell_info.sinr);
 																 
							 if (NULL != strstr((const char *)freq_band_ind, "LTE BAND "))
							 {
									sscanf((const char *)freq_band_ind, "LTE BAND %s", (char *)&freq_band_ind);
							 }else if(NULL != strstr((const char *)freq_band_ind, "GSM "))
							 {
									sscanf((const char *)freq_band_ind, "GSM %s", (char *)&freq_band_ind);
							 }else{}
				 
							hCat1.cell_info.freq_band_ind= atoi(freq_band_ind);
				 
				 
							hCat1.cell_info.rsrp = (int)((float)hCat1.cell_info.rsrp) / 10;
							hCat1.cell_info.rsrq = (int)((float)hCat1.cell_info.rsrq) / 10;
							hCat1.cell_info.sinr = (int)((float)hCat1.cell_info.sinr) / 10;
				 
			      	// tac=%#X, cellid=%#X,
							rtt_printf(RTT_CAT1,"-->mcc=%d,mnc=%d, tac=0x%x, cellid=0x%x, pci=%d, freq_band_ind=%d,earfcn=%d,up_bandwitdh=%d,dw_bandwitdh=%d, rsrp = %d, rsrq = %d, sinr = %d\r\n\r\n",
							hCat1.cell_info.mcc,hCat1.cell_info.mnc,hCat1.cell_info.tac,hCat1.cell_info.cellid, hCat1.cell_info.pci,
							hCat1.cell_info.freq_band_ind, hCat1.cell_info.earfcn,hCat1.cell_info.up_bandwitdh,hCat1.cell_info.dw_bandwitdh, 
							hCat1.cell_info.rsrp, hCat1.cell_info.rsrq, hCat1.cell_info.sinr);
					
							 RetValue = RET_OK;
            }
            else
            {
                RetValue = RET_ERROR;
            }
            break;
						
        default:
            break;
        }
        if (hCat1.AT_cmd_no_response_cnt >= 1)
            hCat1.AT_cmd_no_response_cnt--;
    }
    else
    {
        RetValue = RET_NO_RESPONSE;
        hCat1.AT_cmd_no_response_cnt++;
        rtt_printf(RTT_CAT1, "Cat1 no response!!!\r\n");
    }
    
    cat1_unlock(&hCat1);    //解锁
    return RetValue;
    
    send_at_end:
    cat1_unlock(&hCat1);    //解锁
    return RetValue;
}

/*****************************************************************************
 * @Name	  : is_valid_IP
 * @Brief  : 检测服务器IP、Port是否设置
 * @Param  : ucSocketId: [输入/出]
 * @Retval :
 * @Author :
 * @Date   : 2022-08-17
 * @Note   :
 */
static uint8_t is_valid_IP(LINK_ID id)
{
    uint32_t ServerIpSum = 0, i = 0;
    uint16_t Port = 0;

    switch (hCat1.socketConfig[id].ip_type) // IP类型
    {
    case 1: // IPv4
        ServerIpSum = hCat1.socketConfig[id].ipv4_addr[0] + hCat1.socketConfig[id].ipv4_addr[1] + hCat1.socketConfig[id].ipv4_addr[2] + hCat1.socketConfig[id].ipv4_addr[3];
        Port = hCat1.socketConfig[id].port;
        break;

    case 2: // IPv6
        for (i = 0; i < 16; i++)
        {
            ServerIpSum += hCat1.socketConfig[id].ipv6_addr[i];
        }
        Port = hCat1.socketConfig[id].port;
        break;

    case 3: // IPv4v6
        break;

    default: // None
        return RET_CMD_ERROR;
        // break;
    }

    if ((ServerIpSum > 0) && (Port != 0))
    {
        return B_TRUE;
    }
    else
    {
        return B_FALSE;
    }
}

/*****************************************************************************
 * @Name	  : is_valid_apn
 * @Brief  : 检测apn是否设置
 * @Retval :
 */
static BOOL_E is_valid_apn(void)
{
    uint32_t i = 0;
    BOOL_E ret = B_FALSE;

    for (i = 0; i < sizeof(hCat1.apn); i++)
    {
        if (hCat1.apn[i] != 0 && hCat1.apn[i] != 0xff)
        {
            ret = B_TRUE;
            break;
        }
    }
    return ret;
}
/*****************************************************************************
 * @Name	  : cat1_config
 * @Brief  : 链接初始化
 * @Param  : ucSocketId: [输入/出]
 * @Retval : 成功/失败
 * @Author :
 * @Date   : 2022-08-17
 * @Note   :
 */
eRETURN cat1_config(void)
{
#define INIT_RETRY 3
#define AT_RETRY 3
#define CPIN_RETRY 3
#define CGREG_RETRY 5
#define QICSGP_RETRY 5
    const TickType_t xTicksToWait = 1000;
    eRETURN RetValue = RET_ERROR;
    uint8_t retry_times = INIT_RETRY;
    uint8_t i = 0, cfg_stat = B_FALSE;

    while (0 < retry_times) // 修改判断条件
    {
        retry_times--;
        for (i = 0; i < AT_RETRY; i++)
        {
            RetValue = cat1_send_AT_cmd(CAT1_CMD_CHECK_AT); // 发送AT指令
            if (RetValue == RET_OK)
            {
                break;
            }
            else
            {
                vTaskDelay(xTicksToWait);
            }
        }
        if (i == AT_RETRY)
        {
            goto __EXIT;
        }

        for (i = 0; i < AT_RETRY; i++)
        {
            RetValue = cat1_send_AT_cmd(CAT1_CMD_CLOSE_ECHO); // 关闭AT指令回显
            if (RetValue == RET_OK)
            {
                break;
            }
            else
            {
                vTaskDelay(xTicksToWait);
            }
        }
        if (i == AT_RETRY)
        {
            goto __EXIT;
        }

        RetValue = cat1_send_AT_cmd(CAT1_CMD_QUERY_SW_VER); // 查询版本
        RetValue = cat1_send_AT_cmd(CAT1_CMD_QUERY_HW_VER); // 查询版本

        for (i = 0; i < CPIN_RETRY; i++)
        {
            RetValue = cat1_send_AT_cmd(CAT1_CMD_QUERY_CPIN); // 查询SIM卡状态
            if (RetValue == RET_OK)
            {
                break;
            }
            else
            {
                vTaskDelay(xTicksToWait);
            }
        }
        if (i == CPIN_RETRY)
        {
            goto __EXIT;
        }

        for (i = 0; i < CPIN_RETRY; i++)
        {
            RetValue = cat1_send_AT_cmd(CAT1_CMD_QUERY_SIM_ID); // 查询SIM卡ID
            if (RetValue == RET_OK)
            {
                break;
            }
            else
            {
                vTaskDelay(xTicksToWait);
            }
        }
        if (i == CPIN_RETRY)
        {
            goto __EXIT;
        }

        // RetValue = cat1_send_AT_cmd(CAT1_CMD_SET_CREG);   // 设置CS注册上报格式

        for (i = 0; i < QICSGP_RETRY; i++)
        {
            RetValue = cat1_send_AT_cmd(CAT1_CMD_SET_APN); // 配置PDP场景
            if (RetValue == RET_OK)
            {
                break;
            }
            else
            {
                vTaskDelay(xTicksToWait);
            }
        }
        if (i == QICSGP_RETRY)
        {
            goto __EXIT;
        }

        vTaskDelay(2000);

        for (i = 0; i < CGREG_RETRY; i++)
        {
            RetValue = cat1_send_AT_cmd(CAT1_CMD_QUERY_CGREG); // 查询PS业务是否已注册
            if (RetValue == RET_OK)
            {
                cfg_stat = B_TRUE;
                break;
            }
            else
            {
                vTaskDelay(xTicksToWait * 2); // 2s
            }
        }

        if (cfg_stat == B_TRUE)
            break;
    __EXIT:;
    }

    if (cfg_stat == B_TRUE)
    {
        rtt_printf(RTT_CAT1, "Cat1 config done!!\r\n");
        return RET_OK;
    }
    else
    {
        rtt_printf(RTT_CAT1, "Cat1 config failed!!\r\n");
        return RET_ERROR;
    }
}

/*****************************************************************************
 * @Name	  : cat1_config
 * @Brief  : 链接初始化
 * @Param  : ucSocketId: [输入/出]
 * @Retval : 成功/失败
 * @Author :
 * @Date   : 2022-08-17
 * @Note   :
 */
eRETURN cat1_pdp_active(void)
{

#define QICSGP_RETRY 5
#define QIACT_RETRY 5

    const TickType_t xTicksToWait = 1000;
    eRETURN RetValue = RET_ERROR;

    uint8_t Pdp_stat = B_FALSE;
    uint8_t i = 0;

    for (i = 0; i < QIACT_RETRY; i++)
    {
        RetValue = cat1_send_AT_cmd(CAT1_CMD_SET_PDPACT); // 激活PDP场景
        if (RetValue == RET_OK)
        {
            Pdp_stat = B_TRUE;
            break;
        }
        else
        {
            vTaskDelay(xTicksToWait);
        }
    }

    if (Pdp_stat == B_FALSE) // 当配置失败时,断开
    {
        rtt_printf(RTT_CAT1, "Cat1 Pdp active failed!!\r\n");
        cat1_send_AT_cmd(CAT1_CMD_SET_PDPDEACT);
        return RET_ERROR;
    }
    else
    {
        rtt_printf(RTT_CAT1, "Cat1 Pdp active!!\r\n");
        cat1_send_AT_cmd(CAT1_CMD_QUERY_RTC); // 获取RTC时间
        return RET_OK;
    }
}
/*****************************************************************************
 * @Name	  : vSocketConnect
 * @Brief  : Socket连接
 * @Param  : None
 * @Retval : 连接状态
 * @Note   :
 */
eRETURN vSocketConnect(LINK_ID id)
{
    uint8_t retry_times = 5;
    eRETURN ret = RET_ERROR;
    uint32_t quary_time = 0;

    if (!IS_SOCKET_TYPE(hCat1.socketConfig[id].tcp_or_udp) || !IS_SOCKETID(hCat1.socketStat[id].socketID) || !IS_IP_TYPE(hCat1.socketConfig[id].ip_type)) // 参数有效性判断
        return RET_ERROR;

    if (B_TRUE != is_valid_IP(id)) // 检查ip是否有效
        return RET_ERROR;

    while (0 < retry_times)
    {
        hi_iwdg_feed();
        retry_times--;

        if (id == LINK0)
        {
            // 连接服务器
            if (cat1_send_AT_cmd(CAT1_CMD_SOCKET_OPEN0) == RET_OK)
            {
                rtt_printf(RTT_CAT1, RTT_CTRL_TEXT_BRIGHT_CYAN "Cat1 Socket %d connect\r\n", id);
                ret = RET_OK;
            }
            else
            {
                cat1_send_AT_cmd(CAT1_CMD_SOCKET_CLOSE0); // 断开连接
                hCat1.socketStat[id].ucLinkSeverState = B_FALSE;
                ret = RET_ERROR;
                break;
            }

            while (cat1_send_AT_cmd(CAT1_CMD_SOCKET_STAT0) != RET_OK)
            {
                vTaskDelay(3000);
                quary_time += 3000;
                if (quary_time >= 20 * 1000)
                {
                    ret = RET_ERROR;
                    break;
                }
            }
            if (ret == RET_ERROR)
            {
                break;
            }
            else
            {
                hCat1.socketStat[id].ucLinkSeverState = B_TRUE;
                rtt_printf(RTT_CAT1, RTT_CTRL_TEXT_BRIGHT_CYAN "Cat1 Socket %d connected\r\n", id);
                ret = RET_OK;
                break;
            }
        }
        else if (id == LINK1)
        {
            // 连接服务器
            if (cat1_send_AT_cmd(CAT1_CMD_SOCKET_OPEN1) == RET_OK)
            {
                hCat1.socketStat[id].ucLinkSeverState = B_TRUE;
                rtt_printf(RTT_CAT1, "Cat1 Socket %d connected\r\n", id);
                ret = RET_OK;
            }
            else
            {
							
                cat1_send_AT_cmd(CAT1_CMD_SOCKET_CLOSE1); // 断开连接
                hCat1.socketStat[id].ucLinkSeverState = B_FALSE;
                ret = RET_ERROR;
                break;
            }

            while (cat1_send_AT_cmd(CAT1_CMD_SOCKET_STAT1) != RET_OK)
            {
                vTaskDelay(3000);
                quary_time += 3000;
                if (quary_time >= 20 * 1000)
                {
                    ret = RET_ERROR;
                    break;
                }
            }
            if (ret == RET_ERROR)
            {
                break;
            }
            else
            {
                hCat1.socketStat[id].ucLinkSeverState = B_TRUE;
                rtt_printf(RTT_CAT1, "Cat1 Socket %d connected\r\n", id);
                ret = RET_OK;
                break;
            }
        }
    }

    return ret;
}
/*****************************************************************************
 * @Name	  : vSocketConnect
 * @Brief  : Socket连接
 * @Param  : None
 * @Retval : 连接状态
 * @Note   :
 */
eRETURN socket_disconnect(LINK_ID id)
{
    uint8_t retry_times = 2;

    while (0 < retry_times)
    {
        hi_iwdg_feed();
        retry_times--;

        if (id == LINK0)
        {
            // 断开服务器
            if (cat1_send_AT_cmd(CAT1_CMD_SOCKET_CLOSE0) == RET_OK)
            {
                hCat1.socketStat[id].ucLinkSeverState = B_FALSE;
                return RET_OK;
            }
        }
        else if (id == LINK1)
        {
            // 断开服务器
            if (cat1_send_AT_cmd(CAT1_CMD_SOCKET_CLOSE1) == RET_OK)
            {
                hCat1.socketStat[id].ucLinkSeverState = B_FALSE;
                return RET_OK;
            }
        }
        else
            return RET_ERROR;
    }

    return RET_ERROR;
}

/*****************************************************************************
 * @Name	  : cat1_push_msg_handle
 * @Brief  : 判断监控模块是否主动发送链接断开指令
 * @Param  : ucSocketId: 连接号
 * @Retval :
 * @Author :
 * @Date   : 2022-08-18
 * @Note   :
 */
static void cat1_push_msg_handle(uint8_t *rx_data)
{
    uint8_t id = 0xff;
    char *ptr_start = NULL; 
    //char *ptr_data = NULL;
    uint8_t temp = 0;

    //"closed"通知: 相应socket连接状态置为关闭
    ptr_start = strstr((const char *)rx_data, "+TCPCLOSE:");
    if (ptr_start != NULL)
    {
        if (sscanf((const char *)ptr_start, "+TCPCLOSE: %hhd,Link Closed", &id) != 1)  //推送内容是否含有id??
           return;
       
					if (id == LINK0)
							hCat1.socketStat[0].socket_stat = SOCKET_CONNECT;
					else if (id == LINK1)
							hCat1.socketStat[1].socket_stat = SOCKET_CONNECT;
					else
				   ;
        return;
    }

    //"TCP recv"通知: 解析数据,socketid,长度,并存到收包缓冲区
    ptr_start = strstr((const char *)rx_data, "+TCPRECV:");
		
		if (ptr_start != NULL)
    {
			 rtt_printf(RTT_CAT1, "cat1_push_msg_handle TCPRECVDATA : %s\r\n", rx_data);

        memset(&hCat1.rx_buf, 0, sizeof(hCat1.rx_buf));
			
			if (sscanf((const char *)ptr_start, "+TCPRECV: 0,%d,%s\r\n", (uint32_t *)&hCat1.rx_buf.rx_len_socket0, (char *)&hCat1.rx_buf.rx_data_buf_socket0) != 2) // 格式提取
			{
					memset(&hCat1.rx_buf.rx_data_buf_socket0, 0, sizeof(hCat1.rx_buf.rx_data_buf_socket0));
				  memset(&hCat1.rx_buf.rx_len_socket0, 0, sizeof(hCat1.rx_buf.rx_len_socket0));
			}
            else
            {
                //设置事件组,跳过socket0的检查,和pdp检查
                xEventGroupSetBits(xSocketTimeEgp, BIT_SOCKET_SKIP_PDP_CHECK);
                xEventGroupSetBits(xSocketTimeEgp, BIT_SOCKET_SKIP_SOCKET0_CHECK);
                //xEventGroupSetBits(xSocketTimeEgp, BIT_SOCKET_SKIP_RTC_QUARY);
                if(hCat1.rx_buf.rx_len_socket0 > 999)
                    temp = sizeof("+TCPRECV: 0,") + 4;
                else if(hCat1.rx_buf.rx_len_socket0 > 99)
                    temp = sizeof("+TCPRECV: 0,") + 3;
                else
                    temp = sizeof("+TCPRECV: 0,") + 2;
                memcpy(hCat1.rx_buf.rx_data_buf_socket0,ptr_start+temp,hCat1.rx_buf.rx_len_socket0);
            }
            
			if (sscanf((const char *)ptr_start, "+TCPRECV: 1,%d,%s\r\n", (uint32_t *)&hCat1.rx_buf.rx_len_socket1, (char *)&hCat1.rx_buf.rx_data_buf_socket1) != 2) // 格式提取
			{
                memset(&hCat1.rx_buf.rx_data_buf_socket1, 0, sizeof(hCat1.rx_buf.rx_data_buf_socket1));
                memset(&hCat1.rx_buf.rx_len_socket1, 0, sizeof(hCat1.rx_buf.rx_len_socket1));
			}
            else
            {
                xEventGroupSetBits(xSocketTimeEgp, BIT_SOCKET_SKIP_PDP_CHECK);
                xEventGroupSetBits(xSocketTimeEgp, BIT_SOCKET_SKIP_SOCKET1_CHECK);
                //xEventGroupSetBits(xSocketTimeEgp, BIT_SOCKET_SKIP_RTC_QUARY);
                if(hCat1.rx_buf.rx_len_socket1 > 999)
                    temp = sizeof("+TCPRECV: 1,") + 4;
                else if(hCat1.rx_buf.rx_len_socket1 > 99)
                    temp = sizeof("+TCPRECV: 1,") + 3;
                else
                    temp = sizeof("+TCPRECV: 1,") + 2;
                memcpy(hCat1.rx_buf.rx_data_buf_socket1,ptr_start+temp,hCat1.rx_buf.rx_len_socket1);
            }

//      rtt_printf(RTT_CAT1, "rx_len_socket0=%d,rx_data_buf_socket0=%s\r\n",hCat1.rx_buf.rx_len_socket0,hCat1.rx_buf.rx_data_buf_socket0);
//      rtt_printf(RTT_CAT1, "rx_len_socket1=%d,rx_data_buf_socket1=%s\r\n",hCat1.rx_buf.rx_len_socket1,hCat1.rx_buf.rx_data_buf_socket1);
            //打印接收数据
        #if 0
        uint8_t temp_buf[1200] = {0};
        t_algo_hex_2_ascii((uint8_t *)hCat1.rx_buf.rx_data_buf_socket0, (char*)temp_buf, hCat1.rx_buf.rx_len_socket0);
        rtt_printf(RTT_CAT1,"rx_len_socket0=%d,rx_data_buf_socket0=%s\r\n",hCat1.rx_buf.rx_len_socket0,temp_buf);
        #endif
      	
      return;
    }
		
		
		//"UDP recv"通知: 解析数据,socketid,长度,并存到收包缓冲区
    ptr_start = strstr((const char *)rx_data, "+UDPRECV:");
		if (ptr_start != NULL)
    {
			 rtt_printf(RTT_CAT1, "cat1_push_msg_handle UDPDATA : %s\r\n", rx_data);

        memset(&hCat1.rx_buf, 0, sizeof(hCat1.rx_buf));
			
			if (sscanf((const char *)ptr_start, "+UDPRECV: 0,%d,%s\r\n", (uint32_t *)&hCat1.rx_buf.rx_len_socket0, (char *)&hCat1.rx_buf.rx_data_buf_socket0) != 2) // 格式提取
			{
					memset(&hCat1.rx_buf.rx_data_buf_socket0, 0, sizeof(hCat1.rx_buf.rx_data_buf_socket0));
				  memset(&hCat1.rx_buf.rx_len_socket0, 0, sizeof(hCat1.rx_buf.rx_len_socket0));
			}
			if (sscanf((const char *)ptr_start, "+UDPRECV: 1,%d,%s\r\n", (uint32_t *)&hCat1.rx_buf.rx_len_socket1, (char *)&hCat1.rx_buf.rx_data_buf_socket1) != 2) // 格式提取
			{
					memset(&hCat1.rx_buf.rx_data_buf_socket1, 0, sizeof(hCat1.rx_buf.rx_data_buf_socket1));
				  memset(&hCat1.rx_buf.rx_len_socket1, 0, sizeof(hCat1.rx_buf.rx_len_socket1));
			}					
      return;
    }
}

/**************************************************************************
 * 函 数 名：uint8_t cat1_send_msg(uint8_t ucSocketId, uint8_t* send_buf, uint16_t msg_len)
 * 功能描述：客户端消息发送
 * 参数说明：ucSocketId：连接号
 * 参数说明：send_buf：待发送字符串
 * 参数说明：usMsgLen: 待发送数据长度
 * 函数输出：成功/失败
 */
eRETURN cat1_send_msg(LINK_ID link_id, uint8_t *send_buf, uint16_t msg_len)
{
    uint32_t xTicksToWait = 2000;
    eRETURN RetVal = RET_ERROR;
    uint16_t index = 0;
    uint8_t *ptr_errcnt = NULL;
    
    if(STAT_OK != cat1_try_lock(&hCat1))    //上锁
    {
        rtt_printf(RTT_CAT1, "at send lock!!!\r\n");
        if(STAT_OK != cat1_wait_lock(&hCat1,1000))  //等待超时退出
            return RET_ERROR;
    }

    if (link_id == LINK0)
    {
        ptr_errcnt = &hCat1.socketStat[0].SocketSendErrCnt;
    }
    else if (link_id == LINK1)
    {
        ptr_errcnt = &hCat1.socketStat[1].SocketSendErrCnt;
    }
    else
    {
        rtt_printf(RTT_CAT1, "cat1_send_msg Error: socket ID invalid!\r\n");
//        return RET_ERROR;
        RetVal = RET_ERROR;
        goto send_msg_end;
    }

    if (msg_len <= 15)
    {
        rtt_printf(RTT_CAT1, "Socket:%d message length too short!\r\n", link_id);
//        return RET_ERROR;
        RetVal = RET_ERROR;
        goto send_msg_end;
    }
    if (msg_len % 2 != 0)
    {
        rtt_printf(RTT_CAT1, "Socket:%d message length %d must be even!\r\n", link_id, msg_len);
//        return RET_ERROR;
        RetVal = RET_ERROR;
        goto send_msg_end;
    }

    cat1_tx_data_clear();
    cat1_clear_ATcmd_rx_pack();

  
		if(TCP == hCat1.socketConfig[link_id].tcp_or_udp)
		{
		  sprintf((char *)p_tx_buf, "AT+TCPSEND=%d,%d,", link_id, msg_len/2);
		}else if(UDP == hCat1.socketConfig[link_id].tcp_or_udp)
		{
		 sprintf((char *)p_tx_buf, "AT+UDPSEND=%d,%d,", link_id, msg_len);
		}
		
    index = strlen((char *)p_tx_buf);

    p_tx_buf[index++] = '"';

    memcpy(p_tx_buf + index, send_buf, msg_len);
    index += msg_len;

    p_tx_buf[index++] = '"';
    p_tx_buf[index++] = ',';
    p_tx_buf[index++] = '1';

    p_tx_buf[index++] = 0x0D;
    p_tx_buf[index++] = 0x0A;

    rtt_printf(RTT_CAT1, RTT_CTRL_TEXT_BRIGHT_MAGENTA"Cat1 TX str: %s\r\n", p_tx_buf);

    cat1_uart->send(cat1_uart->tpt_data, strlen((const char *)p_tx_buf));

    while (cat1_wait_ATcmd_rx_pack(xTicksToWait) > 0) // 持续检索结果,直到超时或者收到回复
    {
        if (strstr((const char *)hCat1.rx_cmd_buf.buf, "OK") != NULL)
        {
            RetVal = RET_OK;
            rtt_printf(RTT_CAT1, "Socket send ok!\r\n");
            if (*ptr_errcnt)
            {
                *ptr_errcnt = 0;
            }
            break;
        }
        else if (strstr((const char *)hCat1.rx_cmd_buf.buf, "ERROR") != NULL)
        {
            *ptr_errcnt += 1;
            rtt_printf(RTT_CAT1, "[Error]: Socket%d do not exist!\r\n", link_id);
            RetVal = RET_ERROR;
            break;
        }
    }
//    return RetVal;
    cat1_unlock(&hCat1);    //解锁
    return RetVal;
    
    send_msg_end:
    cat1_unlock(&hCat1);    //解锁
    return RetVal;
}

/*****************************************************************************
* @Name	  : cat1_uart_rx_data_handle
* @Brief  : 用于接收并处理从cat1收到的消息,包括连接断开信息,socket数据,建议放在一个调度周期不低于50ms的任务中
            为了避免AT指令回包等待处理流程中,出现推送消息导致AT收发失败,或丢失推送消息,在这里先统一收包并分类
* @Retval : None
* @Author :
* @Date   :
* @Note   :
*/
void cat1_uart_rx_data_handle(void)
{
    uint16_t usRxLen = 0;

    if (cat1_uart->get_newmsg(cat1_uart->tpt_data) == B_TRUE) // 串口有无新数据包
    {
        usRxLen = cat1_uart->read(cat1_uart->tpt_data);
        cat1_uart->clr_newmsg(cat1_uart->tpt_data);
        if (usRxLen == 0) // 解决串口get_newmsg返回值为B_TRUE，但数据长度为0的异常
            return;
    }
    else
        return;

    rtt_printf(RTT_CAT1, RTT_CTRL_TEXT_BRIGHT_CYAN"Cat1 RX Msg: %s", p_rx_buf);

     if ((strstr((char *)p_rx_buf, "+TCPRECV:") != NULL) || (strstr((char *)p_rx_buf, "+UDPRECV:") != NULL)
			// || (strstr((char *)p_rx_buf, "+TCPSETUP:") != NULL)
    || (strstr((char *)p_rx_buf, "Link Closed") != NULL))
    {
        cat1_push_msg_handle(p_rx_buf);
        cat1_rx_data_clear();
        return;
    }
    else // 非推送消息,存入rx_cmd_buf
    {
        cat1_store_Atcmd_rx_pack(p_rx_buf, usRxLen);
        cat1_rx_data_clear();
    }
}

/*****************************************************************************
 * @Name	  : cat1_recv_msg
 * @Brief  : 从Cat1读取收到的数据,格式为字符串
 * @Param  : link_id :指定要读取的socket连接,LINK0 LINK1
 * @Param  : rx_buf: 出参,用于返回接收的数据
 * @Retval : 返回数据长度,0表示无数据
 * @Note   :
 */
uint16_t cat1_recv_msg(LINK_ID link_id, uint8_t *rx_buf)
{
    uint16_t rx_len = 0;

    if (link_id == LINK0) // 判断收包缓冲区内的数据是否是指定socket的
    {
        if (hCat1.rx_buf.rx_len_socket0 > 0) // 取数据
        {
            memcpy(rx_buf, hCat1.rx_buf.rx_data_buf_socket0, hCat1.rx_buf.rx_len_socket0);
            rx_len = hCat1.rx_buf.rx_len_socket0;

            memset(hCat1.rx_buf.rx_data_buf_socket0, 0, sizeof(hCat1.rx_buf.rx_data_buf_socket0)); // 清空接收缓冲区
            hCat1.rx_buf.rx_len_socket0 = 0;
        }
        else
            rx_len = 0;
    }else  if (link_id == LINK1) // 判断收包缓冲区内的数据是否是指定socket的
    {
        if (hCat1.rx_buf.rx_len_socket1 > 0) // 取数据
        {
            memcpy(rx_buf, hCat1.rx_buf.rx_data_buf_socket1, hCat1.rx_buf.rx_len_socket1);
            rx_len = hCat1.rx_buf.rx_len_socket1;

            memset(hCat1.rx_buf.rx_data_buf_socket1, 0, sizeof(hCat1.rx_buf.rx_data_buf_socket1)); // 清空接收缓冲区
            hCat1.rx_buf.rx_len_socket1 = 0;
        }
        else
            rx_len = 0;
    }
    else
        rx_len = 0;
    return rx_len;
}
/*****************************************************************************
 * @Name	  : rtc_time_query
 * @Brief  : 处理定时获取RTC时间
 * @Param  : None
 * @Retval : 返回查询状态
 * @Author :
 * @Date   : 2022-08-18
 * @Note   :
 */
static eRETURN rtc_time_query(void)
{
    eRETURN result = RET_ERROR;

    if (SIM_IN == hCat1.ucSimState)
    {
        // 插卡获取网络时间
        result = cat1_send_AT_cmd(CAT1_CMD_QUERY_RTC);
        // cat1_send_AT_cmd(CAT1_CMD_QUERY_QNWINFO);  // 查询网络信息
       // cat1_send_AT_cmd(CAT1_CMD_QUERY_CREG);     // 查询CI
    }
    return result;
}

/*****************************************************************************
 * @Name	  : cat1_in_timer_isr
 * @Brief  : 链接定时器，在定时器中调用
 * @Param  : None
 * @Retval : None
 * @Author :
 * @Date   : 2022-08-18
 * @Note   :
 */
void cat1_in_timer_isr(void)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    EventBits_t bitsTimeCntEventGroup = xEventGroupGetBitsFromISR(xSocketTimeEgp);
    static uint32_t cat1_timer_cnt = 0;
    static uint32_t SocketParaEffectCnt = 0;

    cat1_timer_cnt++; // 每次进入计数

    /***********************************************************
        周期业务
    */

    /*获取监控模块网络时间 */
    if (cat1_timer_cnt % RTC_QUERY_CNT == 0)
    {
        xEventGroupSetBitsFromISR(xSocketTimeEgp, BIT_SOCKET_TIME_RTC_QUERY, &xHigherPriorityTaskWoken);
    }
    /*获取pdp激活状态的时间 */
    if (cat1_timer_cnt % PDP_CHECK_CNT == 0)
    {
        xEventGroupSetBitsFromISR(xSocketTimeEgp, BIT_SOCKET_TIME_PDP_CHECK, &xHigherPriorityTaskWoken);
    }
    /*获取socket连接状态的时间 */
    if (cat1_timer_cnt % LINK_CHECK_CNT == 0)
    {
        xEventGroupSetBitsFromISR(xSocketTimeEgp, BIT_SOCKET_TIME_LINK_CHECK, &xHigherPriorityTaskWoken);
    }
    /*获取小区信息的时间 */
    if (cat1_timer_cnt % CELL_INFO_QUARY_CNT == 0)
    {
        xEventGroupSetBitsFromISR(xSocketTimeEgp, BIT_SOCKET_TIME_CELLINFO_QUARY, &xHigherPriorityTaskWoken);
    }

    /***********************************************************
        定时业务
    */

    // 监控参数设置延迟10分钟生效
    if (bitsTimeCntEventGroup & BIT_SOCKET_TIME_DELAY_EFFECT)
    {
        if ((++SocketParaEffectCnt) >= SOCKET_PARA_EFFECT_CNT)
        {
            SocketParaEffectCnt = 0;

            rtt_printf(RTT_CAT1, RTT_CTRL_TEXT_BRIGHT_YELLOW "==>Monitor Relink\r\n");
            xEventGroupClearBitsFromISR(xSocketTimeEgp, BIT_SOCKET_TIME_DELAY_EFFECT); // 清除

            // 网络参数修改需要重连监控
            xEventGroupSetBitsFromISR(xSocketTimeEgp, BIT_SOCKET_TIME_DELAY_EFFECT_ALARM, &xHigherPriorityTaskWoken);
        }
    }
}

/**
 * @brief   设置监控参数延迟生效
 */
void set_cat1_reset_after_10min(void)
{
    xEventGroupSetBits(xSocketTimeEgp, BIT_SOCKET_TIME_DELAY_EFFECT); // 置位
}

/*****************************************************************************
 * @Name	  : cat1_init
 * @Brief  : CAT1初始化
 * @Param  : None
 * @Retval : None
 * @Author :
 * @Date   : 2022-08-16
 * @Note   :
 */
void cat1_init(void)
{
    cat1_pwron();
    cat1_rst();  // 重启
    cat1_open(); // 开机 8s后才能进行串口通信

    cat1_rx_data_clear(); // 接收缓存清空
    cat1_tx_data_clear(); // 发送缓存清空
    // cat1_uart->uart_open(cat1_uart->tpt_data);    // 打开串口

    memset(&hCat1.socketStat[0], 0, sizeof(hCat1.socketStat[0]));
    memset(&hCat1.socketStat[1], 0, sizeof(hCat1.socketStat[1]));

    hCat1.ucSimState = SIM_OUT;

    hCat1.cat1_stat = CAT1_INIT;
    hCat1.socketStat[0].socket_stat = SOCKET_START;
    hCat1.socketStat[1].socket_stat = SOCKET_START;
    
	  hCat1.socketStat[0].socketID = LINK0;
    hCat1.socketStat[1].socketID = LINK1;
	
    rtt_printf(RTT_CAT1, RTT_CTRL_TEXT_BRIGHT_GREEN "Cat1 Open!!!\r\n");
}

/*****************************************************************************
 * @Name	  : cat1_reset
 * @Brief  : 重启CAT1,在系统复位处调用
 * @Param  : None
 * @Retval :
 * @Note   :
 */
void cat1_reset(void)
{
    hCat1.cat1_stat = CAT1_INIT;
}

/*****************************************************************************
 * @Name	  : cat1_read_params
 * @Brief  : 查询监控所需的cat1信息
 * @Param  : eType: 参考eCAT1_CMD_TYPE
 * @Param  : rd_buf: 出参
 * @Retval : 出参长度
 * @Note   :
 */
uint8_t cat1_read_params(eCAT1_API_TYPE eType, uint8_t *rd_buf)
{
    uint8_t len = 0;

    switch (eType)
    {
    case CAT1_GET_SIM_ID: // SIM卡串口
        memcpy(rd_buf, hCat1.ucaSimNum, sizeof(hCat1.ucaSimNum));
        len = sizeof(hCat1.ucaSimNum);
        break;

    case CAT1_GET_SW_VER: // 软件版本
        memcpy(rd_buf, hCat1.ucaSimNum, sizeof(hCat1.ucaCatSwVer));
        len = sizeof(hCat1.ucaCatSwVer);
        break;

    case CAT1_GET_HW_VER: // 硬件版本
        memcpy(rd_buf, hCat1.ucaSimNum, sizeof(hCat1.ucaCatHwVer));
        len = sizeof(hCat1.ucaCatHwVer);
        break;

    case CAT1_GET_LINK0_SEVER_STATE: // 链接服务器状态
        memcpy(rd_buf, &hCat1.socketStat[0].ucLinkSeverState, 1);
        len = 1;
        break;

    case CAT1_GET_LINK1_SEVER_STATE: // 链接服务器状态
        memcpy(rd_buf, &hCat1.socketStat[0].ucLinkSeverState, 1);
        len = 1;
        break;

    default:
        break;
    }
    return len;
}
/*****************************************************************************
 * @Name	  : cat1_set_omc_params
 * @Brief  : 设置cat1的omc参数
 * @Param  : eType: 参考eCAT1_CMD_TYPE
 * @Param  : buf: 写入的数据
 * @Param  : len: 写入的长度
 * @Retval : 设置结果
 * @Note   :
 */
eRETURN cat1_set_omc_params(eCAT1_API_TYPE eType, uint8_t *buf, uint8_t len)
{
    eRETURN ret = RET_ERROR;
    switch (eType)
    {
    case CAT1_SET_APN: // 设置
        if (len > sizeof(hCat1.apn))
        {
            memcpy(hCat1.apn, buf, len);
            ret = RET_OK;
        }
        else
            ret = RET_ERROR;
        break;

    case CAT1_SET_LINK0_IP_TPYE:
        //  hCat1.socketConfig[0].ip_type uint8_t 类型
        if (len == 1)
        {
            hCat1.socketConfig[0].ip_type = buf[0];
            ret = RET_OK;
        }
        break;

    case CAT1_SET_LINK0_SOCKET_TPYE:
        // hCat1.socketConfig[0].tcp_or_udp uint8_t 类型
        if (len == 1)
        {
            hCat1.socketConfig[0].tcp_or_udp = buf[0];
            ret = RET_OK;
        }
        break;

    case CAT1_SET_LINK0_USE_PASSWORD:
        // hCat1.socketConfig[0].use_password_en uint8_t 类型
        if (len == 1)
        {
            hCat1.socketConfig[0].use_password_en = buf[0];
            ret = RET_OK;
        }
        break;

    case CAT1_SET_LINK0_USERNAME:
        // hCat1.socketConfig[0].username 是一个长度为 20 的字符数组
        if (len <= sizeof(hCat1.socketConfig[0].username))
        {
            memcpy(hCat1.socketConfig[0].username, (char *)buf, len);
            ret = RET_OK;
        }
        break;

    case CAT1_SET_LINK0_PASSWORD:
        // hCat1.socketConfig[0].password 是一个长度为 20 的字符数组
        if (len <= sizeof(hCat1.socketConfig[0].password))
        {
            memcpy(hCat1.socketConfig[0].password, (char *)buf, len);
            ret = RET_OK;
        }
        break;

    case CAT1_SET_LINK0_IPV4_ADDR:
        // hCat1.socketConfig[0].ipv4_addr 是一个长度为 4 的 uint8_t 数组
        if (len == 4)
        {
            memcpy(hCat1.socketConfig[0].ipv4_addr, buf, len);
            ret = RET_OK;
        }
        break;

    case CAT1_SET_LINK0_IPV6_ADDR:
        // hCat1.socketConfig[0].ipv6_addr 是一个长度为 16 的 uint8_t 数组
        if (len <= sizeof(hCat1.socketConfig[0].ipv6_addr))
        {
            memcpy(hCat1.socketConfig[0].ipv6_addr, buf, len);
            ret = RET_OK;
        }
        break;

    case CAT1_SET_LINK0_PORT:
        // hCat1.socketConfig[0].port uint16_t 类型
        if (len == 2)
        {
            memcpy(&hCat1.socketConfig[0].port, buf, len);
            ret = RET_OK;
        }
        break;

    case CAT1_SET_LINK1_IP_TPYE:
        //  hCat1.socketConfig[0].ip_type uint8_t 类型
        if (len == 1)
        {
            hCat1.socketConfig[1].ip_type = buf[0];
            ret = RET_OK;
        }
        break;

    case CAT1_SET_LINK1_SOCKET_TPYE:
        // hCat1.socketConfig[0].tcp_or_udp uint8_t 类型
        if (len == 1)
        {
            hCat1.socketConfig[1].tcp_or_udp = buf[0];
            ret = RET_OK;
        }
        break;

    case CAT1_SET_LINK1_USE_PASSWORD:
        // hCat1.socketConfig[0].use_password_en uint8_t 类型
        if (len == 1)
        {
            hCat1.socketConfig[1].use_password_en = buf[0];
            ret = RET_OK;
        }
        break;

    case CAT1_SET_LINK1_USERNAME:
        // hCat1.socketConfig[0].username 是一个长度为 20 的字符数组
        if (len <= sizeof(hCat1.socketConfig[1].username))
        {
            memcpy(hCat1.socketConfig[1].username, (char *)buf, len);
            ret = RET_OK;
        }
        break;

    case CAT1_SET_LINK1_PASSWORD:
        // hCat1.socketConfig[0].password 是一个长度为 20 的字符数组
        if (len <= sizeof(hCat1.socketConfig[1].password))
        {
            memcpy(hCat1.socketConfig[1].password, (char *)buf, len);
            ret = RET_OK;
        }
        break;

    case CAT1_SET_LINK1_IPV4_ADDR:
        // hCat1.socketConfig[0].ipv4_addr 是一个长度为 4 的 uint8_t 数组
        if (len == 4)
        {
            memcpy(hCat1.socketConfig[1].ipv4_addr, buf, len);
            ret = RET_OK;
        }
        break;

    case CAT1_SET_LINK1_IPV6_ADDR:
        // hCat1.socketConfig[0].ipv6_addr 是一个长度为 16 的 uint8_t 数组
        if (len <= sizeof(hCat1.socketConfig[1].ipv6_addr))
        {
            memcpy(hCat1.socketConfig[1].ipv6_addr, buf, len);
            ret = RET_OK;
        }
        break;

    case CAT1_SET_LINK1_PORT:
        // hCat1.socketConfig[0].port uint16_t 类型
        if (len == 2)
        {
            memcpy(&hCat1.socketConfig[1].port, buf, len);
            ret = RET_OK;
        }
        break;

    default:
        break;
    }

    if (ret == RET_OK)
    {
        set_cat1_reset_after_10min(); // 配置omc参数成功,10分钟后复位cat1模组并按新的参数进行连接
    }

    return ret;
}

/*****************************************************************************
 * @Name	  : cat1_get_rtc_time
 * @Brief  : 获取当前RTC时间
 * @Retval : rtc时间
 * @Note   :
 */
Time_T cat1_get_rtc_time(void)
{
    return hCat1.tRtcTime;
}

/*****************************************************************************
 * @Name	  : rtc_time_update
 * @Brief  : 更新当前RTC时间
 * @Retval : rtc时间
 * @Note   :
 */
static void rtc_time_update(void)
{
    EventBits_t bitsTimeCntEventGroup = xEventGroupGetBits(xSocketTimeEgp);
    static uint8_t rtx_err_cnt = 0;

    if (bitsTimeCntEventGroup & BIT_SOCKET_TIME_RTC_QUERY)
    {
        xEventGroupClearBits(xSocketTimeEgp, BIT_SOCKET_TIME_RTC_QUERY);
        if(bitsTimeCntEventGroup & BIT_SOCKET_SKIP_RTC_QUARY)
        {
            xEventGroupClearBits(xSocketTimeEgp, BIT_SOCKET_SKIP_RTC_QUARY);
            return;
        }
        if (rtc_time_query() != RET_OK)
            rtx_err_cnt++;
        else
            rtx_err_cnt = 0;

        if (rtx_err_cnt == 3) // 连续多次读取RTC失败后清空RTC时间 
            memset(&hCat1.tRtcTime, 0, sizeof(hCat1.tRtcTime));
    }
}

/*****************************************************************************
 * @Name	  : cellInfo_update
 * @Brief  : 更新当前小区信息
 * @Retval : 小区信息
 * @Note   :
 */
void cellInfo_update(void)
{
    EventBits_t bitsTimeCntEventGroup = xEventGroupGetBits(xSocketTimeEgp);

    if (bitsTimeCntEventGroup & BIT_SOCKET_TIME_CELLINFO_QUARY)
    {
        if (hCat1.cat1_stat != CAT1_PDP_ACTIVED)
        {
            memset(&hCat1.cell_info, 0, sizeof(hCat1.cell_info));
        }
        else
            cat1_send_AT_cmd(MONITOR_CMD_QUERY_QENG);
        xEventGroupClearBits(xSocketTimeEgp, BIT_SOCKET_TIME_CELLINFO_QUARY);
    }
}

static eRETURN pdp_active_check(void)
{
    return cat1_send_AT_cmd(CAT1_CMD_QUARY_PDPACT);
}

/*********************************************************************
 * 函 数 名：void cat1_run(void)
 * 功能描述：cat1业务处理函数,用于创建和保持sock连接,cat1状态监控,错误处理,收包分类等
 * 参数说明：无
 * 函数输出：无
 *********************************************************************/
#define CAT1_OPEN_OVERTIME_MS (20 * 1000) // cat1 模组上电复位超时时间
#define CAT1_ERROR_MS (30 * 1000)         // cat1 错误复位等待时间
#define CAT1_PDP_REACTIVE_MS (30 * 1000)  // PDP再次激活间隔时间

void cat1_process(void)
{
    static uint8_t pdp_cnt = 0;
    static uint32_t start_tick, err_handle_start_tick, last_pdp_tick;
    EventBits_t bitsTimeCntEventGroup = xEventGroupGetBits(xSocketTimeEgp);
    static uint8_t config_cnt = 0;
    static eCat1_stat_machine last_stat = CAT1_STAT_END;
    static  uint8_t pdp_err_cnt = 0, socket0_err_cnt = 0, socket1_err_cnt = 0;
    eRETURN ret;

    if (last_stat != hCat1.cat1_stat)
        rtt_printf(RTT_CAT1, RTT_CTRL_TEXT_BRIGHT_YELLOW "Info :Cat1 Stat = %d\r\n", hCat1.cat1_stat);
    last_stat = hCat1.cat1_stat;

    switch (hCat1.cat1_stat)
    {
    case CAT1_INIT: // 上电复位初始化
        cat1_init();
        start_tick = hi_get_tick();
        hCat1.cat1_stat = CAT1_WAIT_STATUS_ON;
        break;

    case CAT1_WAIT_STATUS_ON:             // 等待上电状态结果
        if (cat1_state_read() == LEVEL_H) // 上电复位等待status引脚拉高后进入配置阶段
        {
            start_tick = 0;
            hCat1.cat1_stat = CAT1_CONFIG;
            break;
        }
        if (hi_get_tick() - start_tick >= CAT1_OPEN_OVERTIME_MS) // 上电复位超时,重新复位
        {
            hCat1.cat1_stat = CAT1_INIT;
        }
        break;

    case CAT1_CONFIG:                 // cat1基础配置
        if (is_valid_apn() != B_TRUE) // 未设置apn时,不进行cat1配置
            break;
        if (cat1_config() == RET_OK)
        {
            hCat1.cat1_stat = CAT1_PDP_CONFIG;
        }
        else
        {
            config_cnt++;
            if (config_cnt == 10)
                hCat1.cat1_stat = CAT1_ERROR_HANDLE;
            rtt_printf(RTT_CAT1, "Cat1 config fail ,error handle!!\r\n");
        }
        break;

    case CAT1_PDP_CONFIG: // 激活PDP
        if (last_pdp_tick == 0 || hi_get_tick() - last_pdp_tick > CAT1_PDP_REACTIVE_MS)
        {
            ret = cat1_pdp_active();
            last_pdp_tick = hi_get_tick();
            if (ret == RET_OK) // 激活成功,跳转状态
            {
                hCat1.cat1_stat = CAT1_PDP_ACTIVED;
                last_pdp_tick = 0;
                pdp_cnt = 0;
            }
            else
            {
                pdp_cnt++;
                if(pdp_cnt == 5)       //pdp5次激活失败，重启cat1
                    hCat1.cat1_stat = CAT1_INIT;
            }
        }
        break;

    case CAT1_PDP_ACTIVED:                                     // PDP已激活
        if (bitsTimeCntEventGroup & BIT_SOCKET_TIME_PDP_CHECK) // PDP激活状态检测
        {
            xEventGroupClearBits(xSocketTimeEgp, BIT_SOCKET_TIME_PDP_CHECK);
            if(bitsTimeCntEventGroup & BIT_SOCKET_SKIP_PDP_CHECK)
            {
                xEventGroupClearBits(xSocketTimeEgp, BIT_SOCKET_SKIP_PDP_CHECK);
                break;
            }
            ret = pdp_active_check();
            if (ret == RET_ERROR)
                hCat1.cat1_stat = CAT1_PDP_CONFIG;
            else if(ret == RET_NO_RESPONSE)
            {
                pdp_err_cnt++;
                if(pdp_err_cnt == 10) //pdp5次查询无回应, 重配置cat1
                {
                    rtt_printf(RTT_CAT1, "Cat1 pdp no resp ,error handle!!\r\n");
                    hCat1.cat1_stat = CAT1_PDP_CONFIG;
                    pdp_err_cnt = 0;
                }
            }else if(ret == RET_OK)
                pdp_err_cnt = 0;

        }
        break;

    case CAT1_ERROR_HANDLE: // 错误处理
        err_handle_start_tick = hi_get_tick();
        hCat1.AT_cmd_no_response_cnt = 0;
        hCat1.cat1_stat = CAT1_ERROR_HANDLE_DELAY;

    case CAT1_ERROR_HANDLE_DELAY:                                   // 错误复位等待时间
        if (hi_get_tick() - err_handle_start_tick >= CAT1_ERROR_MS) // 错误复位等待30s
            hCat1.cat1_stat = CAT1_INIT;
        break;

    default:
        break;
    }
    // cat1运行过程中状态引脚出现拉低, 进行模组复位处理
    if (cat1_state_read() != LEVEL_H && (hCat1.cat1_stat == CAT1_CONFIG || hCat1.cat1_stat == CAT1_PDP_CONFIG || hCat1.cat1_stat == CAT1_PDP_ACTIVED))
        hCat1.cat1_stat = CAT1_ERROR_HANDLE;

    // cat1运行过程中出现AT指令不回复现象, 连续10次复位处理
    if (hCat1.AT_cmd_no_response_cnt == AT_CMD_MAX_ERROR_CNT)
        hCat1.cat1_stat = CAT1_ERROR_HANDLE;

    // 参数设置后先保持链接, 10分钟后, 对cat1模组进行复位重连
    if (bitsTimeCntEventGroup & BIT_SOCKET_TIME_DELAY_EFFECT_ALARM)
    {
        hCat1.cat1_stat = CAT1_INIT;
        xEventGroupClearBits(xSocketTimeEgp, BIT_SOCKET_TIME_DELAY_EFFECT_ALARM);
    }

    // 参数设置后先保持链接, 10分钟后, 对cat1模组进行复位重连
    if (bitsTimeCntEventGroup & BIT_SOCKET_TIME_CELLINFO_QUARY)
    {
        //cellInfo_update();
        xEventGroupClearBits(xSocketTimeEgp, BIT_SOCKET_TIME_CELLINFO_QUARY);
    }


    // Socket连接检测
    if (bitsTimeCntEventGroup & BIT_SOCKET_TIME_LINK_CHECK)
    {
        xEventGroupClearBits(xSocketTimeEgp, BIT_SOCKET_TIME_LINK_CHECK);
        if(bitsTimeCntEventGroup & BIT_SOCKET_SKIP_SOCKET0_CHECK)
        {
            xEventGroupClearBits(xSocketTimeEgp, BIT_SOCKET_SKIP_SOCKET0_CHECK);
        }else
        {
            if(hCat1.socketStat[0].socket_stat == SCOKET_CONNECTED)
            {
                ret = cat1_send_AT_cmd(CAT1_CMD_SOCKET_STAT0);
                if (ret == RET_ERROR)
                    hCat1.socketStat[0].socket_stat = SCOKET_DISCONNECT;
                else if (ret == RET_NO_RESPONSE)
                {
                    socket0_err_cnt++;
                    if(socket0_err_cnt == 10) //socket0 5次查询无回应, 重配置cat1
                    {
                        rtt_printf(RTT_CAT1, "Cat1 socket0 no resp ,error handle!!\r\n");
                        hCat1.socketStat[0].socket_stat = SCOKET_DISCONNECT;
                        socket0_err_cnt = 0;
                    }
                }else if (ret == RET_OK)
                    socket0_err_cnt = 0;
            }

            if(hCat1.socketStat[1].socket_stat == SCOKET_CONNECTED)
            {
                ret = cat1_send_AT_cmd(CAT1_CMD_SOCKET_STAT1);
                if (ret == RET_ERROR)
                    hCat1.socketStat[1].socket_stat = SCOKET_DISCONNECT;
                else if (ret == RET_NO_RESPONSE)
                {
                    socket1_err_cnt++;
                    if(socket1_err_cnt == 10) //socket1 5次查询无回应, 重配置cat1
                    {
                        rtt_printf(RTT_CAT1, "Cat1 socket1 no resp ,error handle!!\r\n");
                        hCat1.socketStat[1].socket_stat = SCOKET_DISCONNECT;
                        socket1_err_cnt = 0;
                    }
                }else if (ret == RET_OK)
                    socket1_err_cnt = 0;
            }
        }
    }
}

/*********************************************************************
 * 函 数 名：void socket_link_process(void)
 * 功能描述：建立和维护socket连接
 * 参数说明：无
 * 函数输出：无
 *********************************************************************/
#define CAT1_SOCKET_RECONN_MS (10 * 60 * 1000) // Socket重连间隔时间

void socket_link_process(LINK_ID id)
{
    static uint32_t last_socket_conn_tick = 0;
    eRETURN ret;
    static eSocket_stat_machine last_stat_0 = SCOKET_STAT_END, last_stat_1 = SCOKET_STAT_END;
    EventBits_t bitsTimeCntEventGroup = xEventGroupGetBits(xSocketTimeEgp);

    if (last_stat_0 != hCat1.socketStat[0].socket_stat)
        rtt_printf(RTT_CAT1, RTT_CTRL_TEXT_BRIGHT_CYAN"Info :Socket 0 Stat = %d\r\n", hCat1.socketStat[0].socket_stat);
    last_stat_0 = hCat1.socketStat[0].socket_stat;

    if (last_stat_1 != hCat1.socketStat[1].socket_stat)
        rtt_printf(RTT_CAT1, RTT_CTRL_TEXT_BRIGHT_CYAN "Info :Socket 1 Stat = %d\r\n", hCat1.socketStat[1].socket_stat);
    last_stat_1 = hCat1.socketStat[1].socket_stat;
    

    switch (hCat1.socketStat[id].socket_stat)
    {
    case SOCKET_START: // Socket配置
        hCat1.socketStat[id].ucLinkSeverState = 0;
        hCat1.socketStat[id].SocketSendErrCnt = 0;
        hCat1.socketStat[id].socket_stat = SOCKET_CONNECT;
        break;

    case SOCKET_CONNECT:                         // Socket进行连接
        if (hCat1.cat1_stat != CAT1_PDP_ACTIVED) // 当pdp不在激活状态时, 不进行连接
            break;

        if (last_socket_conn_tick == 0 || hi_get_tick() - last_socket_conn_tick > CAT1_PDP_REACTIVE_MS) // 如果连接失败,定时重连
        {
            rtt_printf(RTT_CAT1, RTT_CTRL_TEXT_BRIGHT_CYAN "Info :Socket %d connecting\r\n", id);
            ret = vSocketConnect(id);

            if (ret == RET_OK) // 连接成功,跳转状态
            {
                // vTaskDelay(15000);
                hCat1.socketStat[id].socket_stat = SCOKET_CONNECTED;
                last_socket_conn_tick = 0;
            }
            else // 连接不成功,记录时间,定时重连
                last_socket_conn_tick = hi_get_tick();
        }
        break;

    case SCOKET_CONNECTED:                       // Socket已连接
        if (hCat1.cat1_stat != CAT1_PDP_ACTIVED) // 当pdp不在激活状态时, 关闭连接
            hCat1.socketStat[id].socket_stat = SCOKET_DISCONNECT;
        break;

    case SCOKET_DISCONNECT: // Socket进行断连
			
     //   socket_disconnect(id);
		
		   if(RET_ERROR==socket_disconnect(id))
			 {
			   hCat1.cat1_stat=CAT1_INIT;
			 }
		
		
        hCat1.socketStat[id].SocketSendErrCnt = 0;
        hCat1.socketStat[id].socket_stat = SCOKET_DISCONNECTED;

        break;

    case SCOKET_DISCONNECTED: // Socket已断连

        hCat1.socketStat[id].socket_stat = SOCKET_START;
        break;

    default:
        break;
    }

    // socket发包错误处理, 断开重连
    if (hCat1.socketStat[id].SocketSendErrCnt > SEND_CMD_MAX_ERROR_CNT)
    {
        rtt_printf(RTT_CAT1, "Cat1 socket %d send error max handle!!\r\n",id);
        hCat1.socketStat[id].socket_stat = SCOKET_DISCONNECT;
        hCat1.socketStat[id].SocketSendErrCnt = 0;
    }
}

#if (CAT1_HW_TEST == 1)
char send_str[50] = {0};
uint16_t p_cnt = 0;
void socket0_send_test_packet(void)
{
#define SEND_PACKET_DURATION 1000

    static uint32_t last_packet_tick = 0;
    if (hCat1.socketStat[0].socket_stat != SCOKET_CONNECTED)
        return;
    if (hi_get_tick() - last_packet_tick >= SEND_PACKET_DURATION)
    {
        p_cnt++;
        memset(send_str, 0, sizeof(send_str));
        sprintf(send_str, "000102030405060708090A00%04x", p_cnt);
        cat1_send_msg(LINK0, (uint8_t *)send_str, strlen(send_str));
        last_packet_tick = hi_get_tick();
    }
}
#endif

#if (CAT1_HW_TEST == 1)
char send_str_socket1[50] = {0};
uint16_t p_cnt_socket1 = 0;

void socket1_send_test_packet(void)
{
#define SEND_PACKET_DURATION 1000

    static uint32_t last_packet_tick = 0;
    if (hCat1.socketStat[1].socket_stat != SCOKET_CONNECTED)
        return;
    if (hi_get_tick() - last_packet_tick >= SEND_PACKET_DURATION)
    {
        p_cnt_socket1++;
        memset(send_str_socket1, 0, sizeof(send_str_socket1));
        sprintf(send_str_socket1, "000102030405060708090A00%04x", p_cnt_socket1);
        cat1_send_msg(LINK1, (uint8_t *)send_str_socket1, strlen(send_str_socket1));
        last_packet_tick = hi_get_tick();
    }
}
#endif

/*********************************************************************
 * 函 数 名：void cat1_run(void)
 * 功能描述：cat1业务处理函数,用于器件初始化,创建和保持sock连接,cat1状态监控,错误处理,收包分类等
 * 参数说明：无
 * 函数输出：无
 *********************************************************************/
void cat1_run(void)
{
    // cat1模组未使能, 则不进行业务处理
    if(hCat1.cat1_en == B_FALSE)
        return;

    cat1_process();             // cat1初始化,配置,pdp激活,错误处理
    socket_link_process(LINK0); // 连接0的连接建立和维护
   // socket_link_process(LINK1); // 连接1的连接建立和维护
    rtc_time_update();          // RTC时间更新


//		#if (CAT1_HW_TEST == 1)
//    uint8_t rcv_buf_socket1[50] = {0};
//    int j = 0, len_socket1 = 0;
//    socket1_send_test_packet();
//	  vTaskDelay(500);
//    len_socket1 = cat1_recv_msg(LINK1, rcv_buf_socket1);
//    if (len_socket1)
//    {
//        rtt_printf(RTT_CAT1, RTT_CTRL_TEXT_BRIGHT_CYAN "Socket 1 receive data:");
//        for (j = 0; j < len_socket1; j++)
//        {
//            // rtt_printf(RTT_CAT1, RTT_CTRL_TEXT_BRIGHT_CYAN"%02x", rcv_buf_socket1[j]);
//            rtt_printf(RTT_CAT1, RTT_CTRL_TEXT_BRIGHT_CYAN "%c", rcv_buf_socket1[j]);
//        }
//        rtt_printf(RTT_CAT1, "\r\n");
//	 }
//	#endif
		
		#if (CAT1_HW_TEST == 1)
    uint8_t rcv_buf[50] = {0};
    int i = 0, len = 0;
    socket0_send_test_packet();
			  vTaskDelay(500);
    len = cat1_recv_msg(LINK0, rcv_buf);
    if (len)
    {
        rtt_printf(RTT_CAT1, RTT_CTRL_TEXT_BRIGHT_CYAN "Socket 0 receive data:");
        for (i = 0; i < len; i++)
        {
            rtt_printf(RTT_CAT1, RTT_CTRL_TEXT_BRIGHT_CYAN "%c", rcv_buf[i]);
        }
        rtt_printf(RTT_CAT1, "\r\n");
    }
#endif

}
