/***************************** Copyright ****************************************
 *
 * (C) Copyright 2022, Shaanxi Tianji Communication
 * All Rights Reserved
 *
 * FileName   : bsp_modem.c
 * Version    : None
 * Author     : WangJun
 * Date       : 2022-08-16
 * Description: None
 ***/
#include "eswin_sync_modem.h"
#include "os_timer.h"
#include "rtt.h"
#include "crc.h"
#include "bsp_reboot.h"
#include "bsp_wdg.h"
#include "hi_gpio.h"
#include "hi_systick.h"
#include "hi_iwdg.h"
#include "rtt.h"
#include "bsp_rf_866x_default_set.h"
#include "bsp_rf_scan_net.h"
/*---------------------- 宏定义  ---------------------------------------*/
#define _USER_RTOS_ (1)

// ModemTimeEgp
#define BIT_SOCKET_TIME_CELLINFO_QUARY (1U << 1) // 获取小区信息
#define BIT_SOCKET_TIME_ALL (0x00FFFFFF)

/*---------------------- 结构体 ----------------------------------------*/
sync_modem_t hModem; // MODEM参数信息
uart_func_t *mdm_uart = NULL;
static uint8_t *p_tx_buf = NULL;
static uint8_t *p_rx_buf = NULL;

/*---------------------- 常量 ------------------------------------------*/
/* 定时器计数 */

#define CELL_INFO_QUARY_CNT (S_TO_TICKS(5)) // 间隔固定时间查询小区信息, 20S  1次
#define AT_CMD_MAX_ERROR_CNT (3)             // AT指令无回复最大次数
#define AT_CMD_MAX_REBOOT_CNT (2)            // REBOOT最大次数
#define AT_RETRY 5
/*---------------------- 全局变量 --------------------------------------*/
static uint8_t index = 0;
uint8_t g_cmd_type = MODEM_CMD_END;
uint8_t set_frq_point_buf[64] = {0}; // 锁定频点用的缓存区域, 锁频点需要将频点数据先复制到该区域

char buffer_freq[FREQ_BUF_SIZE];

int8_t max_index_lte = 0;
int8_t max_index_nr = 0;

modem_config_buf_t pc_config_modem_buf; // PC配置同步模块参数

static uint8_t g_modem_upgrade_state = 0; /*1表示模块升级状态*/

/**
 * @brief 监控链接时间事件组
 *     @arg：bit0 无
 *     @arg：bit1 查询小区信息
 */
EventGroupHandle_t ModemTimeEgp = NULL;

/*---------------------- 局部变量 --------------------------------------*/
/*---------------------- 声明 ------------------------------------------*/
/*****************************************************************************
 * @Name	  : modem_ec800_register
 * @Brief  : 注册modem设备
 * @Param  : hw_config modem设备硬件资源分配,包括GPIO Uart
 * @Retval : None
 * @Note   :
 */
void modem_register(modem_config_t *p_modem_config)
{
    memset(&hModem, 0, sizeof(hModem));
    memcpy(&hModem.hw_config, p_modem_config, sizeof(modem_config_t));
    hModem.modem_stat = MODEM_INIT;
    mdm_uart = hModem.hw_config.modem_uart;
    p_tx_buf = mdm_uart->tpt_data->pt_tx_data;
    p_rx_buf = mdm_uart->tpt_data->pt_rx_data;
}

/**
 * @brief   将频点数据写入BUF
 * @param   pt: [输入/出]
 * @return
 * @data    2024-03-14
 * @note
 */
static void modem_write_freq_point_to_buf(char *pt)
{
    memset(set_frq_point_buf, 0, sizeof(set_frq_point_buf));
    uint8_t len = strlen(pt);
    if (len > sizeof(set_frq_point_buf))
        len = sizeof(set_frq_point_buf) - 1;

    memcpy(set_frq_point_buf, pt, len);
}

/*****************************************************************************
 * @Name	  : vModemInitEgpCreat
 * @Brief  : 创建Modem初始化事件组
 * @Param  : None
 * @Retval : None
 * @Author :
 * @Date   : 2022-08-17
 * @Note   :
 */
void vModemEgpCreat(void)
{
    ModemTimeEgp = xEventGroupCreate();

    // 计时事件组
    if (ModemTimeEgp == NULL)
    {
        /* 任务创建失败 */
        rtt_printf(RTT_MODEM, RTT_CTRL_TEXT_BRIGHT_RED "ERROR: ModemTimeEgp Create Fail...\r\n");
    }
    else
    {
        rtt_printf(RTT_MODEM, RTT_CTRL_TEXT_BRIGHT_GREEN "ModemTimeEgp Create Success!\r\n");
        xEventGroupClearBits(ModemTimeEgp, BIT_SOCKET_TIME_ALL); // 清零
        rtt_printf(RTT_MODEM, RTT_CTRL_TEXT_BRIGHT_GREEN "ModemTimeEgp Init Success!\r\n");
    }
}

/*****************************************************************************
 * @Name	  : modem_in_timer_isr
 * @Brief  : 链接定时器，在定时器中调用
 * @Param  : None
 * @Retval : None
 * @Author :
 * @Date   : 2022-08-18
 * @Note   :
 */
void modem_in_timer_isr(void)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    EventBits_t bitsTimeCntEventGroup = xEventGroupGetBitsFromISR(ModemTimeEgp);
    static uint32_t modem_timer_cnt = 0;

    modem_timer_cnt++; // 每次进入计数

    /***********************************************************
        周期业务
    */

    /*获取小区信息的时间 */
    if (modem_timer_cnt % CELL_INFO_QUARY_CNT == 0)
    {
        xEventGroupSetBitsFromISR(ModemTimeEgp, BIT_SOCKET_TIME_CELLINFO_QUARY, &xHigherPriorityTaskWoken);
    }
}

/*****************************************************************************
 * @Name	  : vModemPwrOn
 * @Brief  : 上电
 * @Param  : None
 * @Retval : None
 * @Author :
 * @Date   : 2022-08-16
 * @Note   : 低电平上电
 */
void vModemPwrOn(void) // TODO: 需要硬件给出上电时序
{
#if (PRODUCT_MODEL == CMDT3521A || PRODUCT_MODEL == CMDT3521C)
    hi_gpio_write(hModem.hw_config.dc_ctrl_port, hModem.hw_config.dc_ctrl_pin, LEVEL_H);
    vTaskDelay(3000);
    hi_gpio_write(hModem.hw_config.dc_ctrl_port, hModem.hw_config.dc_ctrl_pin, LEVEL_L);
    
#elif (PRODUCT_MODEL == CMDT3521B)
    hi_gpio_write(hModem.hw_config.dc_ctrl_port, hModem.hw_config.dc_ctrl_pin, LEVEL_L);
    vTaskDelay(3000);
    hi_gpio_write(hModem.hw_config.dc_ctrl_port, hModem.hw_config.dc_ctrl_pin, LEVEL_H);
#endif
    hModem.power_on_mready = B_FALSE;
    hModem.init_done = B_FALSE;
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
static void vModemRxDataClear(void)
{
    mdm_uart->rx_data_clr(mdm_uart->tpt_data);
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
static void vModemTxDataClear(void)
{
    mdm_uart->tx_data_clr(mdm_uart->tpt_data);
}

/*****************************************************************************
 * @Name	  : vModemStorePushPack
 * @Brief  : 将收到的modem推送消息放到推送消息缓冲区
 * @Param  : None
 * @Retval : None
 * @Note   :
 */
void vModemStorePushPack(uint8_t *pData, uint16_t len)
{
    uint32_t cp_len;
    if (len > sizeof(hModem.Modem_push_msg_buf.rx_cmd_buf))
        cp_len = sizeof(hModem.Modem_push_msg_buf.rx_cmd_buf);
    else
        cp_len = len;
    memset(&hModem.Modem_push_msg_buf, 0, sizeof(hModem.Modem_push_msg_buf));
    memcpy(hModem.Modem_push_msg_buf.rx_cmd_buf, pData, cp_len);
    hModem.Modem_push_msg_buf.rx_len = cp_len;
    return;
}
/*****************************************************************************
 * @Name	  : vModemClearPushMsgPack
 * @Brief  : 清空推送消息缓冲区
 * @Param  : None
 * @Retval : None
 * @Note   :
 */
void vModemClearPushMsgPack(void)
{
    memset(&hModem.Modem_push_msg_buf, 0, sizeof(hModem.Modem_push_msg_buf));
    return;
}
/*****************************************************************************
 * @Name	  : vModemWaitPushMsgPack
 * @Brief  : 等待推送消息
 * @Param  : None
 * @Retval : None
 * @Note   :
 */
uint32_t vModemWaitPushMsgPack(uint16_t wait_ms)
{
    uint32_t time_cnt = 0, remain_ms = 1;
    while (hModem.Modem_push_msg_buf.rx_len == 0)
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
 * @Name	  : vModemStoreCmdRxPack
 * @Brief  : 将收到的modem回复AT指令消息放到AT指令消息缓冲区
 * @Param  : None
 * @Retval : None
 * @Note   :
 */
void vModemStoreCmdRxPack(uint8_t *pData, uint16_t len)
{
    uint32_t cp_len;
    if (len > sizeof(hModem.Modem_AT_CMD_rx_buf.rx_cmd_buf))
        cp_len = sizeof(hModem.Modem_AT_CMD_rx_buf.rx_cmd_buf);
    else
        cp_len = len;
    memset(hModem.Modem_AT_CMD_rx_buf.rx_cmd_buf, 0, sizeof(hModem.Modem_AT_CMD_rx_buf.rx_cmd_buf));
    memcpy(hModem.Modem_AT_CMD_rx_buf.rx_cmd_buf, pData, cp_len);

    hModem.Modem_AT_CMD_rx_buf.rx_len = cp_len;
    return;
}
/*****************************************************************************
 * @Name	  : vModemClearAtCmdRxPack
 * @Brief  : 清空AT指令回复消息缓冲区
 * @Param  : None
 * @Retval : None
 * @Note   :
 */
void vModemClearAtCmdRxPack(void)
{
    memset(&hModem.Modem_AT_CMD_rx_buf, 0, sizeof(hModem.Modem_AT_CMD_rx_buf));
    return;
}
/*****************************************************************************
 * @Name   : vModemWaitAtCmdRxPack
 * @Brief  : 等待串口AT指令的回包,超时时间wait_ms,返回剩余时间.
 * @Param  : None
 * @Retval : 返回剩余时间,超时返回0
 * @Note   :
 */
static uint32_t vModemWaitAtCmdRxPack(uint16_t wait_ms)
{
    uint32_t time_cnt = 0, remain_ms = 1;
    while (hModem.Modem_AT_CMD_rx_buf.rx_len == 0)
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
 * @Name   : monitor_send_AT_cmd
 * @Brief  : 监控模块AT交互指令
 * @Param  : cmd_type: 指令类别
 * @Retval :
 * @Author :
 * @Date   : 2022-08-16
 * @Note   :
 */
static Status_t monitor_send_AT_cmd(uint8_t cmd_type, int param1, int param2, int param3, int param4, int param5)
{
    uint32_t MsToWait = 500, wait_time_ms = 0;
    char *pStart = NULL, *pEnd = NULL;
    Status_t RetValue = STAT_ERROR;

    if (g_modem_upgrade_state == 1)
    {
        /*升级状态不处理AT命令*/
        return STAT_ERROR;
    }

    hModem.AT_cmd_response_busy_cnt = 0;

    vModemClearAtCmdRxPack();

    switch (cmd_type)
    {
    case MODEM_CMD_SET_REBOOT: // 重启
        sprintf((char *)p_tx_buf, "AT+REBOOT\r\n");
        wait_time_ms = MsToWait * 5;
        break;

    case MODEM_CMD_QUERY_EGMR: // 查询TA固件版本
        sprintf((char *)p_tx_buf, "AT+EGMR\r\n");
        wait_time_ms = MsToWait;
        break;

    case MODEM_CMD_QUERY_EGAPR: // 查询AP版本
        sprintf((char *)p_tx_buf, "AT+EGAPR\r\n");
        wait_time_ms = MsToWait;
        break;

    case MODEM_CMD_QUERY_EGHWR: // 查询硬件版本
        sprintf((char *)p_tx_buf, "AT+EGHWR\r\n");
        wait_time_ms = MsToWait;
        break;
	  
	case MODEM_CMD_QUERY_ECUSR: // 查询软件版本
        sprintf((char *)p_tx_buf, "AT+ECUSR\r\n");
        wait_time_ms = MsToWait;
        break;

    case MODEM_CMD_SET_CFUN_0: // 设置运行模式0
        hModem.work_mode = MINIMUM_MODE;
        sprintf((char *)p_tx_buf, "AT+CFUN=%d\r\n", hModem.work_mode);
        wait_time_ms = MsToWait * 100;
        break;

    case MODEM_CMD_SET_CFUN_1: // 设置运行模式1
        hModem.work_mode = RUN_MODE;
        sprintf((char *)p_tx_buf, "AT+CFUN=%d\r\n", hModem.work_mode);
        // 重新搜网
        hModem.scan_result.Scan_stat = SCAN_IDLE;
        memset(&hModem.modem_command_reback_info, 0, sizeof(hModem.modem_command_reback_info));
        wait_time_ms = MsToWait * 10;
        break;

    case MODEM_CMD_QUERY_CFUN: // 查询运行模式
        sprintf((char *)p_tx_buf, "AT+CFUN?\r\n");
        wait_time_ms = MsToWait;
        break;

    case MODEM_CMD_QUERY_ERAT: // 查询网络制式 NR LTE NR+LTE
        sprintf((char *)p_tx_buf, "AT+ERAT?\r\n");
        wait_time_ms = MsToWait;
        break;

    case MODEM_CMD_SET_ERAT: // 设置网络制式 NR LTE NR+LTE
        hModem.network = param1;
        sprintf((char *)p_tx_buf, "AT+ERAT=%d\r\n", hModem.network);
        wait_time_ms = MsToWait;
        break;

    case MODEM_CMD_SET_LOCK_EBAND: // 锁频带或频点
    {
        uint32_t network_format, band_id; //, arfcn1, arfcn2, arfcn3, arfcn4, arfcn5;
        network_format = param1;
        band_id = param2;
        char *pt_freq_point = (char *)set_frq_point_buf; // 待锁定频点字符串数据

        if (strlen(pt_freq_point)) // 有频点需要设置
        {
            sprintf((char *)p_tx_buf, "AT+EBAND=%d,1,%d,%s\r\n", network_format, band_id, pt_freq_point);
        }
        else
        {
            sprintf((char *)p_tx_buf, "AT+EBAND=%d,1,%d\r\n", network_format, band_id);
        }
        wait_time_ms = MsToWait * 5;
        break;
    }

    case MODEM_CMD_QUERY_BAND: // 查询BAND
    {
        sprintf((char *)p_tx_buf, "AT+EBAND?\r\n");
        wait_time_ms = MsToWait;
        break;
    }

    case MODEM_CMD_SET_UNLOCK_EBAND: // 解锁频带或频点
    {
        uint32_t network_format, band_id;
        network_format = param1;
        band_id = param2;

        if (network_format != 0xFF)
        {
            sprintf((char *)p_tx_buf, "AT+EBAND=%d,0,%d\r\n", network_format, band_id);
        }
        else
        {
            sprintf((char *)p_tx_buf, "AT+EBAND\r\n");
        }
        wait_time_ms = MsToWait * 5;
        break;
    }
    case MODEM_CMD_SET_ECELLGPIO_LTE: // 设置LTE跟踪小区和GPIO组
    {
        uint32_t arfcn, pci, band;
        arfcn = param1;
        pci = param2;
        band = param3;

        sprintf((char *)p_tx_buf, "AT+ECELLGPIO=1,%d,%d,%d,%d\r\n", hModem.hw_config.lte_gpio_group, arfcn, pci, band);
        wait_time_ms = MsToWait * 10;
        break;
    }
    case MODEM_CMD_SET_ECELLGPIO_NR: // 设置NR跟踪小区和GPIO组
    {
        uint32_t arfcn, pci, band;
        arfcn = param1;
        pci = param2;
        band = param3;
        if (band == 3 || band == 39 || band == 40) // 当使用lte频段搜网NR制式时,需要设置nr制式,lte_gpio_group
            sprintf((char *)p_tx_buf, "AT+ECELLGPIO=0,%d,%d,%d,%d\r\n", hModem.hw_config.lte_gpio_group, arfcn, pci, band);
        else if (band == 41) // 当使用NR频段搜网NR制式时,需要设置nr制式,nr_gpio_group
            sprintf((char *)p_tx_buf, "AT+ECELLGPIO=0,%d,%d,%d,%d\r\n", hModem.hw_config.nr_gpio_group, arfcn, pci, band);

        wait_time_ms = MsToWait * 10;
        break;
    }
    case MODEM_CMD_SET_EL1CFG: // 设置物理层参数,时隙配比 TODO怎么知道是单时隙还是双时隙?先按单时隙
    {
        uint32_t io_group = param1;
        timing_config_t *ptr_timing = NULL;
        uint8_t cfg_mode = param2;
        if (io_group == hModem.hw_config.lte_gpio_group)
        {
            ptr_timing = &hModem.LTE_timing_config;
        }
        else if (io_group == hModem.hw_config.nr_gpio_group)
        {
            ptr_timing = &hModem.NR_timing_config;
        }
        else if (io_group == hModem.hw_config.nr2_gpio_group)
        {
            ptr_timing = &hModem.NR2_timing_config;
        }
        else
            return STAT_ERROR;

        // 单时隙
        if (cfg_mode == 0)
        {
            sprintf((char *)p_tx_buf, "AT+EL1CFG=0,%d,%02x%02x%02x%02x%02x%02x%02x\r\n", io_group, ptr_timing->num_of_ul_subframe_1,
                    ptr_timing->num_of_dl_subframe_1, ptr_timing->num_of_ul_subframe_2, ptr_timing->num_of_dl_subframe_2,
                    ptr_timing->num_of_ul_symbles_sp1, ptr_timing->num_of_gp_symbles_sp1, ptr_timing->num_of_dl_symbles_sp1);
        }
        // 双时隙
        else if (cfg_mode == 1)
        {
            sprintf((char *)p_tx_buf, "AT+EL1CFG=0,%d,%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\r\n", io_group, ptr_timing->num_of_ul_subframe_1,
                    ptr_timing->num_of_dl_subframe_1, ptr_timing->num_of_ul_subframe_2, ptr_timing->num_of_dl_subframe_2,
                    ptr_timing->num_of_ul_symbles_sp1, ptr_timing->num_of_gp_symbles_sp1, ptr_timing->num_of_dl_symbles_sp1,
                    ptr_timing->num_of_ul_symbles_sp2, ptr_timing->num_of_gp_symbles_sp2, ptr_timing->num_of_dl_symbles_sp2);
        }
        else
        {
        }

        wait_time_ms = MsToWait;
        break;
    }
    case MODEM_CMD_SET_ETIMEOFFSET: // 设置NR2通道的时间偏移量,并跟踪和输出同步信号
    {
        int offset = param1;
        sprintf((char *)p_tx_buf, "AT+ETIMEOFFSET=%d,%d\r\n", offset, hModem.hw_config.nr2_gpio_group);
        wait_time_ms = MsToWait;
        break;
    }
    case MODEM_CMD_SET_EGPIOSIGNAL: // 设置GPIO信号输出(配单通或TDD)
    {
        uint8_t io_group = param1;
        uint8_t UL_Signal = param2;
        uint8_t DL_Signal = param3;
        if (io_group == hModem.hw_config.lte_gpio_group)
        {
            sprintf((char *)p_tx_buf, "AT+EGPIOSIGNAL=%d,%d,%d\r\n", hModem.hw_config.lte_gpio_group, UL_Signal, DL_Signal);
        }
        else if (io_group == hModem.hw_config.nr_gpio_group)
        {
            sprintf((char *)p_tx_buf, "AT+EGPIOSIGNAL=%d,%d,%d\r\n", hModem.hw_config.nr_gpio_group, UL_Signal, DL_Signal);
        }
        else if (io_group == hModem.hw_config.nr2_gpio_group)
        {
            sprintf((char *)p_tx_buf, "AT+EGPIOSIGNAL=%d,%d,%d\r\n", hModem.hw_config.nr2_gpio_group, UL_Signal, DL_Signal);
        }
        else
        {
        }
        wait_time_ms = MsToWait;
        break;
    }
    case MODEM_CMD_SET_EGPIOADJ: // 设置GPIO信号调整量0,0,-100,100,0
    {
        uint8_t io_group = param1;
        int ul_rising = param2;
        int ul_falling = param3;
        int dl_rising = param4;
        int dl_falling = param5;

        if (io_group == hModem.hw_config.lte_gpio_group)
        {
            sprintf((char *)p_tx_buf, "AT+EGPIOADJ=%d,%d,%d,%d,%d\r\n", hModem.hw_config.lte_gpio_group, ul_rising, ul_falling, dl_rising, dl_falling);
        }
        else if (io_group == hModem.hw_config.nr_gpio_group)
        {
            sprintf((char *)p_tx_buf, "AT+EGPIOADJ=%d,%d,%d,%d,%d\r\n", hModem.hw_config.nr_gpio_group, ul_rising, ul_falling, dl_rising, dl_falling);
        }
        else if (io_group == hModem.hw_config.nr2_gpio_group)
        {
            sprintf((char *)p_tx_buf, "AT+EGPIOADJ=%d,%d,%d,%d,%d\r\n", hModem.hw_config.nr2_gpio_group, ul_rising, ul_falling, dl_rising, dl_falling);
        }
        else
        {
        }
        wait_time_ms = MsToWait;
        break;
    }

    case MODEM_CMD_QUARY_ECELL: // 查询小区信息

        sprintf((char *)p_tx_buf, "%s", "AT+ECELL\r\n"); // 查询PS业务
        wait_time_ms = MsToWait;
        break;

    default:
        rtt_printf(RTT_MODEM, "no such cmd!!!\r\n");
        return STAT_ERROR;
    }

    mdm_uart->send(mdm_uart->tpt_data, strlen((const char *)p_tx_buf));
    g_cmd_type = cmd_type;

    rtt_printf(RTT_MODEM, "Modem send cmd:%s\r\n", (const char *)p_tx_buf);

    // vTaskDelay(500); // 此处需要增加延迟，否则串口接收容易出错
    uint32_t remain_time_ms = vModemWaitAtCmdRxPack(wait_time_ms);

    if (remain_time_ms > 0)
    {
        rtt_printf(RTT_MODEM, RTT_CTRL_TEXT_BRIGHT_CYAN "Modem Receive cmdData, remain_time_ms: %d\r\n", remain_time_ms); // 打印接收的数据

        if (0 != strstr((const char *)hModem.Modem_AT_CMD_rx_buf.rx_cmd_buf, "+ERROR:"))
        {
            rtt_printf(RTT_MODEM, "MODEM +ERROR:\r\n");
            hModem.AT_cmd_response_busy_cnt = 1;
            RetValue = STAT_BUSY;
        }

        switch (cmd_type)
        {
        case MODEM_CMD_SET_REBOOT: // 重启
            if (0 != strstr((const char *)hModem.Modem_AT_CMD_rx_buf.rx_cmd_buf, "OK"))
            {
                rtt_printf(RTT_CAT1, "MODEM_CMD_SET_REBOOT ok\r\n");
                RetValue = STAT_OK;
            }
            else
            {
                rtt_printf(RTT_CAT1, "MODEM_CMD_SET_REBOOT fail\r\n");
                RetValue = STAT_ERROR;
            }
            break;

        case MODEM_CMD_QUERY_ERAT: // 查询网络制式
            pStart = strstr((const char *)hModem.Modem_AT_CMD_rx_buf.rx_cmd_buf, "+ERAT:");
            pEnd = strstr((const char *)hModem.Modem_AT_CMD_rx_buf.rx_cmd_buf, "OK");
            if ((NULL != pStart) && (NULL != pEnd))
            {
                sscanf((const char *)pStart, "%*[^:]:%hhu", (uint8_t *)&hModem.network);

                RetValue = STAT_OK;
            }
            else
            {
                RetValue = STAT_ERROR;
            }
            break;

        case MODEM_CMD_SET_ERAT:        // 设置网络制式 NR LTE NR+LTE
        case MODEM_CMD_QUERY_BAND:      // 查询BAND
        case MODEM_CMD_SET_EL1CFG:      // 设置物理层参数,时隙配比
        case MODEM_CMD_SET_ETIMEOFFSET: // 设置NR2通道的时间偏移量,并跟踪和输出同步信号
        case MODEM_CMD_SET_EGPIOSIGNAL: // 设置GPIO信号输出(配单通或TDD)
        case MODEM_CMD_SET_EGPIOADJ:    // 设置 GPIO 组对应的信号调整量
        case MODEM_CMD_SET_LOCK_EBAND:
        case MODEM_CMD_SET_CFUN_0: // 设置运行模式
            if (0 != strstr((const char *)hModem.Modem_AT_CMD_rx_buf.rx_cmd_buf, "OK") || 0 != strstr((const char *)hModem.Modem_AT_CMD_rx_buf.rx_cmd_buf, "+SYSINFO") || 0 != strstr((const char *)hModem.Modem_AT_CMD_rx_buf.rx_cmd_buf, "+EUSIMSTATU"))
            {
                RetValue = STAT_OK;
            }
            else
            {
                RetValue = STAT_ERROR;
            }
            break;

        case MODEM_CMD_SET_ECELLGPIO_LTE: // 设置跟踪小区和GPIO组
            if (0 != strstr((const char *)hModem.Modem_AT_CMD_rx_buf.rx_cmd_buf, "OK"))
            {
                hModem.modem_command_reback_info.flg_ecellgpio_lte = 1;
                RetValue = STAT_OK;
            }
            else
            {
                RetValue = STAT_ERROR;
            }
            break;

        case MODEM_CMD_SET_ECELLGPIO_NR: // 设置跟踪小区和GPIO组
            if (0 != strstr((const char *)hModem.Modem_AT_CMD_rx_buf.rx_cmd_buf, "OK"))
            {
                hModem.modem_command_reback_info.flg_ecellgpio_nr = 1;
                RetValue = STAT_OK;
            }
            else
            {
                RetValue = STAT_ERROR;
            }
            break;

        case MODEM_CMD_SET_CFUN_1: // 设置运行模式
        {
            if (0 != strstr((const char *)hModem.Modem_AT_CMD_rx_buf.rx_cmd_buf, "OK")|| 0 != strstr((const char *)hModem.Modem_AT_CMD_rx_buf.rx_cmd_buf, "+SYSINFO") || 0 != strstr((const char *)hModem.Modem_AT_CMD_rx_buf.rx_cmd_buf, "+EUSIMSTATU"))
            {
                RetValue = STAT_OK;
                rtt_printf(RTT_MODEM, RTT_CTRL_TEXT_BRIGHT_YELLOW "MODEM_CMD_SET_CFUN_1 ok!\r\n");
            }
            else
            {
                RetValue = STAT_ERROR;
            }

            break;
        }
        case MODEM_CMD_SET_UNLOCK_EBAND:
            if (0 != strstr((const char *)hModem.Modem_AT_CMD_rx_buf.rx_cmd_buf, "OK"))
            {
                RetValue = STAT_OK;
            }
            else
            {
                RetValue = STAT_ERROR;
            }
            break;

        case MODEM_CMD_QUERY_EGMR: // 查询TA固件版本

            pStart = (char *)&hModem.Modem_AT_CMD_rx_buf.rx_cmd_buf;
            pEnd = strstr((const char *)hModem.Modem_AT_CMD_rx_buf.rx_cmd_buf, "OK");
            if ((pStart != NULL) && (pEnd != NULL))
            {
                pEnd = strstr((const char *)pStart, "\r\n");
                if (pEnd != NULL)
                {
                    rtt_printf(RTT_MODEM, "TA Firmware Version %d: %s\r\n", (pEnd - pStart), (const char *)pStart);
                    memset(hModem.TAFirmVer, 0, sizeof(hModem.TAFirmVer));
                    memcpy(hModem.TAFirmVer, pStart, (pEnd - pStart));
                    RetValue = STAT_OK;
                }
                else
                {
                    RetValue = STAT_ERROR;
                }
            }
            else
            {
                RetValue = STAT_ERROR;
            }
            break;
        case MODEM_CMD_QUERY_EGAPR: // 查询AP版本
            pStart = (char *)&hModem.Modem_AT_CMD_rx_buf.rx_cmd_buf;
            pEnd = strstr((const char *)hModem.Modem_AT_CMD_rx_buf.rx_cmd_buf, "OK");
            if ((pStart != NULL) && (pEnd != NULL))
            {
                pEnd = strstr((const char *)pStart, "\r\n");
                if (pEnd != NULL)
                {
                    rtt_printf(RTT_MODEM, "AP Version %d: %s\r\n", (pEnd - pStart), (const char *)pStart);
                    memset(hModem.APVer, 0, sizeof(hModem.APVer));
                    memcpy(hModem.APVer, pStart, (pEnd - pStart));
                    RetValue = STAT_OK;
                }
                else
                {
                    RetValue = STAT_ERROR;
                }
            }
            else
            {
                RetValue = STAT_ERROR;
            }
            break;

        case MODEM_CMD_QUERY_EGHWR: // 查询硬件版本
            pStart = (char *)&hModem.Modem_AT_CMD_rx_buf.rx_cmd_buf;
            pEnd = strstr((const char *)hModem.Modem_AT_CMD_rx_buf.rx_cmd_buf, "OK");
            if ((pStart != NULL) && (pEnd != NULL))
            {
                pEnd = strstr((const char *)pStart, "\r\n");
                if (pEnd != NULL)
                {
                    rtt_printf(RTT_MODEM, "HW Version %d: %s\r\n", (pEnd - pStart), (const char *)pStart);
                    memset(hModem.HWVer, 0, sizeof(hModem.HWVer));
                    memcpy(hModem.HWVer, pStart, (pEnd - pStart));
                    RetValue = STAT_OK;
                }
                else
                {
                    RetValue = STAT_ERROR;
                }
            }
            else
            {
                RetValue = STAT_ERROR;
            }
            break;
			
		case MODEM_CMD_QUERY_ECUSR: // 查询软件版本
            pStart = (char *)&hModem.Modem_AT_CMD_rx_buf.rx_cmd_buf;
            pEnd = strstr((const char *)hModem.Modem_AT_CMD_rx_buf.rx_cmd_buf, "OK");
            if ((pStart != NULL) && (pEnd != NULL))
            {
                pEnd = strstr((const char *)pStart, "\r\n");
                if (pEnd != NULL)
                {
                    rtt_printf(RTT_MODEM, "SW Version %d: %s\r\n", (pEnd - pStart), (const char *)pStart);
                    memset(hModem.SWVer, 0, sizeof(hModem.SWVer));
                    memcpy(hModem.SWVer, pStart, (pEnd - pStart));
                    RetValue = STAT_OK;
                }
                else
                {
                    RetValue = STAT_ERROR;
                }
            }
            else
            {
                RetValue = STAT_ERROR;
            }
            break;

        case MODEM_CMD_QUERY_CFUN: // 查询运行模式
            pStart = strstr((const char *)hModem.Modem_AT_CMD_rx_buf.rx_cmd_buf, "+CFUN:");
            pEnd = strstr((const char *)hModem.Modem_AT_CMD_rx_buf.rx_cmd_buf, "OK");
            if ((NULL != pStart) && (NULL != pEnd))
            {
                sscanf((const char *)hModem.Modem_AT_CMD_rx_buf.rx_cmd_buf, "%*[^:]:%hhu", (uint8_t *)&hModem.work_mode);
                rtt_printf(RTT_MODEM, "hModem.work_mode= %d\t\n", hModem.work_mode);
                RetValue = STAT_OK;
            }
            else
            {
                RetValue = STAT_ERROR;
            }
            break;

        case MODEM_CMD_QUARY_ECELL: // 查询小区信息
            pStart = strstr((const char *)hModem.Modem_AT_CMD_rx_buf.rx_cmd_buf, "+ECELL:");
            pEnd = strstr((const char *)hModem.Modem_AT_CMD_rx_buf.rx_cmd_buf, "OK");
            if ((pStart != NULL) && (pEnd != NULL))
            {
                pStart = strstr((const char *)hModem.Modem_AT_CMD_rx_buf.rx_cmd_buf, "+ECELL: LTE");

                if (pStart != NULL)
                {
                    sscanf((const char *)pStart, "%*[^: ]: %*[^,],%hhd,%d,%d,%d,%d,%d,%hhd,%hhd,%hhd",
                           &hModem.LTE_net_info.band, &hModem.LTE_net_info.freq, &hModem.LTE_net_info.pci, &hModem.LTE_net_info.bw, &hModem.LTE_net_info.rsrp,
                           &hModem.LTE_net_info.sinr, &hModem.LTE_net_info.TDD_or_FDD, &hModem.LTE_net_info.sfa, &hModem.LTE_net_info.ssfp);
                    hModem.LTE_net_info.rsrp = hModem.LTE_net_info.rsrp - 140;
                    // rtt_printf(RTT_MODEM, "Modem.LTE_net_info   -->pci = %d, rsrp = %d, sinr = %d, bandwitdh = %d\r\n\r\n",
                    //            hModem.LTE_net_info.pci, hModem.LTE_net_info.rsrp, hModem.LTE_net_info.sinr, hModem.LTE_net_info.band);
                }
                pStart = strstr((const char *)hModem.Modem_AT_CMD_rx_buf.rx_cmd_buf, "+ECELL: NR");
                if (pStart != NULL)
                {
                    sscanf((const char *)pStart, "%*[^: ]: %*[^,],%hhd,%d,%d,%d,%d,%d",
                           &hModem.NR_net_info.band, &hModem.NR_net_info.freq, &hModem.NR_net_info.pci, &hModem.NR_net_info.bw, &hModem.NR_net_info.rsrp,
                           &hModem.NR_net_info.sinr);
                    hModem.NR_net_info.rsrp = hModem.NR_net_info.rsrp - 156;
                    // rtt_printf(RTT_MODEM, "Modem.NR_net_info   -->pci = %d, rsrp = %d, sinr = %d, bandwitdh = %d\r\n\r\n",
                    //            hModem.NR_net_info.pci, hModem.NR_net_info.rsrp, hModem.NR_net_info.sinr, hModem.NR_net_info.band);
                }
                RetValue = STAT_OK;
            }
            else
            {
                RetValue = STAT_ERROR;
            }
            break;

        default:
            rtt_printf(RTT_MODEM, "no such cmd!!!\r\n");
            g_cmd_type = MODEM_CMD_END;
            return STAT_ERROR;
        }
        if (hModem.Modem_AT_cmd_no_response_cnt >= 1)
        {
            hModem.Modem_AT_cmd_no_response_cnt = 0;
        }

        if (RetValue == STAT_ERROR)
        {
            hModem.Modem_AT_cmd_error_response_cnt++;
            rtt_printf(RTT_MODEM, "hModem.Modem_AT_cmd_error_response_cnt=%d\r\n", hModem.Modem_AT_cmd_error_response_cnt);
        }
        else
        {
            hModem.AT_cmd_reboot_cnt = 0;
            hModem.Modem_AT_cmd_error_response_cnt = 0;
        }
    }
    else
    {
        RetValue = STAT_ERROR;
        hModem.Modem_AT_cmd_no_response_cnt++;
        rtt_printf(RTT_MODEM, "Modem no response!!!\r\n");
    }
    g_cmd_type = MODEM_CMD_END;
    return RetValue;
}
/*****************************************************************************
 * @Name	 : ucModemPushMsgProcess
 * @Brief  : 解析推送信息内容
 * @Param  : 推送信息指针
 * @Retval :
 * @Author :
 * @Date   : 2022-08-18
 * @Note   :
 */

Status_t ucModemPushMsgProcess(uint8_t *rx_data)
{
    Status_t ret = STAT_OK;
    char *pStart = NULL;

    // rtt_printf(RTT_MODEM, RTT_CTRL_TEXT_BRIGHT_YELLOW "ucModemPushMsgProcess rx_data= %s\r\n", rx_data);

    // 检查数据内容，调用相应的解析函数
    if (strstr((char *)rx_data, "+ECELL: LTE In Sync"))
    {
        // 处理LTE同步信息
        hModem.LTE_net_info.LTE_Sync = SYNC_SYNC; // 驻网成功
        hModem.LTE_net_info.sync_time = hi_get_tick();
    }
    else if (strstr((char *)rx_data, "+ECELL: LTE Out Sync"))
    {
        // 处理LTE失步信息
        hModem.LTE_net_info.LTE_Sync = SCAN_UNSYNC;
    }
    else if (strstr((char *)rx_data, "+ECELL: NR In Sync"))
    {
        // 处理NR同步信息
        hModem.NR_net_info.NR_Sync = SYNC_SYNC; // 驻网成功
        hModem.NR_net_info.sync_time = hi_get_tick();
    }
    else if (strstr((char *)rx_data, "+ECELL: NR Out Sync"))
    {
        // 处理NR失步信息
        hModem.NR_net_info.NR_Sync = SCAN_UNSYNC;
    }
    else if (strstr((char *)rx_data, "+ECELL: Scan LTE start"))
    {
        // 处理信息
        hModem.scan_result.Scan_stat = SCAN_START_LTE;
        index = 0;
        // rtt_printf(RTT_MODEM, RTT_CTRL_TEXT_BRIGHT_YELLOW "ucModemPushMsgProcess SCAN_START_LTE!\r\n");
    }
    else if ((strstr((char *)rx_data, "+ECELL: Scan NR start")) && ((strstr((char *)rx_data, "+ECELL: LTE,")) == NULL)) // 只有+ECELL: Scan NR start
    {
        // 处理信息
        hModem.scan_result.Scan_stat = SCAN_START_NR;
        index = 0;
        // rtt_printf(RTT_MODEM, RTT_CTRL_TEXT_BRIGHT_YELLOW "ucModemPushMsgProcess SCAN_START_NR !\r\n");
    }
    // LTE
    else if ((strstr((char *)rx_data, "+ECELL: LTE,")) && (hModem.LTE_net_info.LTE_Sync != SYNC_SYNC))
    {
        pStart = strstr((const char *)rx_data, "+ECELL: LTE,");

        while ((pStart != NULL) && (hModem.scan_result.Scan_stat == SCAN_START_LTE))
        {
            sscanf((const char *)pStart, "%*[^: ]: %*[^,],%hhd,%d,%d,%d,%d,%d,%hhd,%hhd,%hhd",
                   &hModem.scan_result.LTE_scan_tab[index].band, &hModem.scan_result.LTE_scan_tab[index].freq, &hModem.scan_result.LTE_scan_tab[index].pci, &hModem.scan_result.LTE_scan_tab[index].bw, &hModem.scan_result.LTE_scan_tab[index].rsrp,
                   &hModem.scan_result.LTE_scan_tab[index].sinr, &hModem.scan_result.LTE_scan_tab[index].TDD_or_FDD, &hModem.scan_result.LTE_scan_tab[index].sfa, &hModem.scan_result.LTE_scan_tab[index].ssfp);
            hModem.scan_result.LTE_scan_tab[index].rsrp = hModem.scan_result.LTE_scan_tab[index].rsrp - 140;
            pStart += strlen("+ECELL: LTE,"); // 更新指针
            pStart = strstr((const char *)pStart, "+ECELL: LTE,");

            index++;
            hModem.scan_result.LTE_scan_pci_num = index;
        }
        if (strstr((char *)rx_data, "+ECELL: Scan LTE end"))
        {
            if (strstr((char *)rx_data, "+ECELL: Scan NR start"))
                hModem.scan_result.Scan_stat = SCAN_START_NR;
            else
                hModem.scan_result.Scan_stat = SCAN_DONE_LTE;
            index = 0;
        }
    }
    // NR
    else if ((strstr((char *)rx_data, "+ECELL: NR,")) && (hModem.NR_net_info.NR_Sync != SYNC_SYNC))
    {
        pStart = strstr((const char *)rx_data, "+ECELL: NR,");
        while ((pStart != NULL) && (hModem.scan_result.Scan_stat == SCAN_START_NR))
        {
            sscanf((const char *)pStart, "%*[^: ]: %*[^,],%hhd,%d,%d,%d,%d,%d",
                   &hModem.scan_result.NR_scan_tab[index].band, &hModem.scan_result.NR_scan_tab[index].freq, &hModem.scan_result.NR_scan_tab[index].pci, &hModem.scan_result.NR_scan_tab[index].scs, &hModem.scan_result.NR_scan_tab[index].rsrp, //&hModem.scan_result.NR_scan_tab[index].bw
                   &hModem.scan_result.NR_scan_tab[index].sinr);
            hModem.scan_result.NR_scan_tab[index].rsrp = hModem.scan_result.NR_scan_tab[index].rsrp - 156;
            pStart += strlen("+ECELL: NR,"); // 更新指针
            pStart = strstr((const char *)pStart, "+ECELL: NR,");
            index++;
            hModem.scan_result.NR_scan_pci_num = index;
        }
        if (strstr((char *)rx_data, "+ECELL: Scan NR end"))
        {
            hModem.scan_result.Scan_stat = SCAN_DONE_NR;
            index = 0;
        }
    }
    else if (strstr((char *)rx_data, "+ECELL: Scan LTE end"))
    {
        if (strstr((char *)rx_data, "+ECELL: Scan NR start"))
            hModem.scan_result.Scan_stat = SCAN_START_NR;
        else
            hModem.scan_result.Scan_stat = SCAN_DONE_LTE;

        index = 0;
    }
    else if (strstr((char *)rx_data, "+ECELL: Scan NR end"))
    {
        hModem.scan_result.Scan_stat = SCAN_DONE_NR;
        index = 0;
    }
    else
    {
        ret = STAT_ERROR;
    }

    return ret;
}

/*****************************************************************************
 * @Name	  : modem_uart_rx_data_handle
 * @Brief  : 用于接收并处理从modem收到的消息
 * @Retval : None
 * @Author :
 * @Date   :
 * @Note   :
 */
void modem_uart_rx_data_handle(void)
{
    uint16_t usRxLen = 0;

    if (g_modem_upgrade_state == 1)
    {
        /*升级状态不处理AT命令*/
        return;
    }

    if (mdm_uart->get_newmsg(mdm_uart->tpt_data) == B_TRUE) // 串口有无新数据包
    {
        usRxLen = mdm_uart->read(mdm_uart->tpt_data);
        mdm_uart->clr_newmsg(mdm_uart->tpt_data);
        if (usRxLen == 0)
            return;
    }
    else
    {
        return;
    }

    rtt_printf(RTT_MODEM, RTT_CTRL_TEXT_BRIGHT_GREEN "Modem RX Msg: %s\r\n", p_rx_buf);

    if (strstr((char *)p_rx_buf, "+ECELL: ") != NULL)
    {
        if(g_cmd_type >= MODEM_CMD_END) //当前无命令包在等待回包
        {
            vModemStorePushPack(p_rx_buf, usRxLen);
            ucModemPushMsgProcess((uint8_t *)hModem.Modem_push_msg_buf.rx_cmd_buf); // 存入推送包
        }
        else if (strstr((char *)p_rx_buf, "OK") != NULL || strstr((char *)p_rx_buf, "ERROR") != NULL) // 包内存在推送包和命令包粘包,需要同时处理命令包并处理推送信息
        {
            vModemStoreCmdRxPack(p_rx_buf, usRxLen); // 存入命令包

            vModemStorePushPack(p_rx_buf, usRxLen); // 存入推送包
            ucModemPushMsgProcess((uint8_t *)hModem.Modem_push_msg_buf.rx_cmd_buf); //处理推送包
        }else // 包内无命令包回包,直接以推送包处理
        {
            vModemStorePushPack(p_rx_buf, usRxLen); // 存入推送包
            ucModemPushMsgProcess((uint8_t *)hModem.Modem_push_msg_buf.rx_cmd_buf); //处理推送包
        }
    }
    else if (strstr((char *)p_rx_buf, "+MDReady") != NULL)
    {
        hModem.power_on_mready = B_TRUE;
    }
    else
    {
        vModemStoreCmdRxPack(p_rx_buf, usRxLen); // 存入命令包
    }

    vModemRxDataClear(); // 接收缓存清空
}
/*****************************************************************************
 * @Name	  : modem_init
 * @Brief  : MODEM初始化
 * @Param  : None
 * @Retval : None
 * @Author :
 * @Date   : 2022-08-16
 * @Note   :
 */
void modem_init(void)
{
    uint32_t init_start_tick = 0;

    vModemPwrOn();
    init_start_tick = hi_get_tick();
    while ((hModem.power_on_mready == B_FALSE) && (g_modem_upgrade_state != 1)/*非升级状态*/)
    {
        if ((hi_get_tick() - init_start_tick) > 10000)
        {
            vModemPwrOn();
            init_start_tick = hi_get_tick();
            rtt_printf(RTT_MODEM, RTT_CTRL_TEXT_BRIGHT_RED "Modem init timeout!!\r\n");
        }
        vTaskDelay(100);
    }

    vModemRxDataClear(); // 接收缓存清空
    vModemTxDataClear(); // 发送缓存清空

    hModem.modem_stat = MODEM_INIT;
    hModem.scan_result.Scan_stat = SCAN_IDLE;

    memset(&hModem.modem_command_reback_info, 0, sizeof(hModem.modem_command_reback_info));

    memset(&hModem.NR_net_info, 0, sizeof(hModem.NR_net_info));
    memset(&hModem.LTE_net_info, 0, sizeof(hModem.LTE_net_info));

    hModem.modem_config_state = CONFIG_IDLE;
    pc_config_modem_buf.startup = 0;
    hModem.Modem_AT_cmd_no_response_cnt = 0;
    hModem.Modem_AT_cmd_error_response_cnt = 0;
    hModem.AT_cmd_reboot_cnt = 0;
    hModem.AT_cmd_response_busy_cnt = 0;

    rtt_printf(RTT_MODEM, RTT_CTRL_TEXT_BRIGHT_GREEN "Modem Open!!!\r\n");

    hModem.flg_get_ver = 1;
    hModem.init_done = B_TRUE;
}

/*****************************************************************************
 * @Name	  : modem_reboot
 * @Brief  : 重启MODEM,在系统复位处调用
 * @Param  : None
 * @Retval :
 * @Note   :
 */
Status_t modem_reboot(void)
{
    Status_t RetValue = STAT_ERROR;
    RetValue = monitor_send_AT_cmd(MODEM_CMD_SET_REBOOT, 0, 0, 0, 0, 0); // 发送AT指令
    if (RetValue == STAT_OK)
    {
        // 重启之后 清相关变量
        hModem.AT_cmd_reboot_cnt = 0;
        hModem.Modem_AT_cmd_no_response_cnt = 0;
        memset(&hModem.modem_command_reback_info, 0, sizeof(hModem.modem_command_reback_info));
        memset(&hModem.NR_net_info, 0, sizeof(hModem.NR_net_info));
        memset(&hModem.LTE_net_info, 0, sizeof(hModem.LTE_net_info));
        rtt_printf(RTT_MODEM, RTT_CTRL_TEXT_BRIGHT_GREEN "Modem Reboot ok!!\r\n");
    }
    else
    {
        rtt_printf(RTT_MODEM, RTT_CTRL_TEXT_BRIGHT_GREEN "Modem Reboot fail!!\r\n");
    }
    return RetValue;
}

/*********************************************************************
 * 函 数 名：void vModemRun(void)
 * 功能描述：modem业务处理函数,用于modem状态监控,错误处理等
 * 参数说明：无
 * 函数输出：无
 *********************************************************************/
void vModemProcess(void)
{
    EventBits_t bitsTimeCntEventGroup = xEventGroupGetBits(ModemTimeEgp);

    // rtt_printf(RTT_MODEM, RTT_CTRL_TEXT_BRIGHT_GREEN "hModem.modem_stat=%d\r\n",hModem.modem_stat);
    // rtt_printf(RTT_MODEM, RTT_CTRL_TEXT_BRIGHT_GREEN "hModem.scan_result.Scan_stat=%d\r\n",hModem.scan_result.Scan_stat);
    switch (hModem.modem_stat)
    {
    case MODEM_INIT: // 初始化
        hModem.modem_stat = MODEM_IDLE;
        rtt_printf(RTT_MODEM, RTT_CTRL_TEXT_BRIGHT_GREEN "vModemProcess  modem_init done!!\r\n");
        break;

    case MODEM_IDLE: // 完成基本模式配置,等待搜网指令
        if (hModem.scan_result.Scan_stat == SCAN_START_LTE)
        {
            hModem.modem_stat = MODEM_SCAN_LTE;
        }
        else if (hModem.scan_result.Scan_stat == SCAN_START_NR)
        {
            hModem.modem_stat = MODEM_SCAN_NR;
        }
        else
        {
        }
        break;

    case MODEM_SCAN_LTE: // LTE搜网中

        hi_iwdg_feed();
        if ((hModem.scan_result.Scan_stat == SCAN_DONE_LTE) && (hModem.network == LTE))
        {
            hModem.modem_stat = MODEM_SCAN_DONE;
        }
        else if ((hModem.scan_result.Scan_stat == SCAN_DONE_NR) && ((hModem.network == NR) || (hModem.network == LTE_NR)))
        {
            hModem.modem_stat = MODEM_SCAN_DONE;
        }
        else
        {
        }

        break;

    case MODEM_SCAN_NR: // NR搜网中
        hi_iwdg_feed();
        if (hModem.scan_result.Scan_stat == SCAN_DONE_NR)
        {
            hModem.modem_stat = MODEM_SCAN_DONE;
        }
        break;

    case MODEM_SCAN_DONE: // 搜网完成,等待小区跟踪指令(需要先LTE 后NR)
        if (hModem.modem_command_reback_info.flg_ecellgpio_lte == 1)
        {
            hModem.modem_stat = MODEM_LTE_ATTACHED;
        }
        else if ((hModem.network == NR) && (hModem.modem_command_reback_info.flg_ecellgpio_nr == 1))
        {
            hModem.modem_stat = MODEM_NR_ATTACHED;
        }
        break;

    case MODEM_LTE_ATTACHED: // LTE已跟踪小区
        if (hModem.modem_command_reback_info.flg_ecellgpio_nr == 1)
        {
            hModem.modem_stat = MODEM_NR_ATTACHED;
        }
        if (hModem.LTE_net_info.LTE_Sync == SYNC_SYNC)
            hModem.LTE_net_info.sync_time = hi_get_tick();
        if (hModem.NR_net_info.NR_Sync == SYNC_SYNC)
            hModem.NR_net_info.sync_time = hi_get_tick();
        break;

    case MODEM_NR_ATTACHED: // NR已跟踪小区                                                                                                                                                                                           // NR已跟踪小区
        if (hModem.LTE_net_info.LTE_Sync == SYNC_SYNC)
            hModem.LTE_net_info.sync_time = hi_get_tick();
        if (hModem.NR_net_info.NR_Sync == SYNC_SYNC)
            hModem.NR_net_info.sync_time = hi_get_tick();
        break;

    case MODEM_REBOOT:

        if (hModem.AT_cmd_reboot_cnt >= AT_CMD_MAX_REBOOT_CNT)
        {
            hModem.modem_stat = MODEM_RESET;
            break;
        }

        if (modem_reboot() == STAT_OK)
        {
            rtt_printf(RTT_MODEM, "modem_reboot ok\r\n");
            hModem.modem_stat = MODEM_INIT;
            hModem.scan_result.Scan_stat = SCAN_IDLE;
            hModem.modem_reboot_or_reset = 1; // 模块重启标志
        }

        vTaskDelay(100); // 每一次reboot命令之前添加延时
        hModem.AT_cmd_reboot_cnt++;

        break;

    case MODEM_RESET: // 复位
        hModem.AT_cmd_reboot_cnt = 0;
        hModem.AT_cmd_response_busy_cnt = 0;
        hModem.Modem_AT_cmd_no_response_cnt = 0;
        hModem.Modem_AT_cmd_error_response_cnt = 0;
        modem_init();
        hModem.modem_reboot_or_reset = 2; // 模块重新上电标志
        break;

    default:
        break;
    }

    // modem运行过程中出现AT指令不回复或者回复错误现象, 连续3次复位处理
    if ((hModem.Modem_AT_cmd_no_response_cnt >= AT_CMD_MAX_ERROR_CNT) || (hModem.Modem_AT_cmd_error_response_cnt >= AT_CMD_MAX_ERROR_CNT))
    {
        rtt_printf(RTT_MODEM, RTT_CTRL_TEXT_BRIGHT_RED "Modem_AT_cmd_no_response_cnt %d\r\n", hModem.Modem_AT_cmd_no_response_cnt);
        rtt_printf(RTT_MODEM, RTT_CTRL_TEXT_BRIGHT_RED "Modem_AT_cmd_error_response_cnt %d\r\n", hModem.Modem_AT_cmd_error_response_cnt);
        hModem.Modem_AT_cmd_no_response_cnt = 0;
        hModem.Modem_AT_cmd_error_response_cnt = 0;
        hModem.modem_stat = MODEM_REBOOT;
    }
    // modem运行过程中出现AT指令回复 Com 忙碌 现象
    if (hModem.AT_cmd_response_busy_cnt >= 1) // 回复+ERROR:<err> 通过重启解决
    {
        rtt_printf(RTT_MODEM, RTT_CTRL_TEXT_BRIGHT_RED "AT_cmd_response_busy\r\n");
        hModem.modem_stat = MODEM_RESET;
    }

    // 当在小区跟踪状态时,定时刷新小区信息
    if (bitsTimeCntEventGroup & BIT_SOCKET_TIME_CELLINFO_QUARY)
    {
        xEventGroupClearBits(ModemTimeEgp, BIT_SOCKET_TIME_CELLINFO_QUARY);

        if ((((hModem.network == LTE) && (hModem.LTE_net_info.LTE_Sync == SYNC_SYNC)) || ((hModem.network == NR) && (hModem.NR_net_info.NR_Sync == SYNC_SYNC)) || ((hModem.network == LTE_NR) && ((hModem.LTE_net_info.LTE_Sync == SYNC_SYNC) || (hModem.NR_net_info.NR_Sync == SYNC_SYNC)))) && (g_modem_upgrade_state == 0)) //(hModem.Modem_taupdate_info.flg_modem_update_state == 0)
        {
            modem_refresh_net_info(); // 发送获取小区信息指令
        }
        else
        {
            if (hModem.LTE_net_info.LTE_Sync != SYNC_SYNC)
            {
                hModem.LTE_net_info.sinr = -256;
                hModem.LTE_net_info.rsrp = -256;
            }

            if (hModem.NR_net_info.NR_Sync != SYNC_SYNC)
            {
                hModem.NR_net_info.sinr = -256;
                hModem.NR_net_info.rsrp = -256;
            }
        }
        rtt_printf(RTT_MODEM, RTT_CTRL_TEXT_BRIGHT_YELLOW "--> LTE_Sync = %d NR_Sync = %d (0-unsync 1-sync 2-lost)\r\n", hModem.LTE_net_info.LTE_Sync, hModem.NR_net_info.NR_Sync);
    }
}
/***********************************************************
 *函数功能: 获取TA固件版本
 *参数说明: 无
 *返 回 值: STAT_OK /STAT_ERROR
 */
Status_t modem_get_EGMR()
{
    Status_t RetValue = STAT_ERROR;
    RetValue = monitor_send_AT_cmd(MODEM_CMD_QUERY_EGMR, 0, 0, 0, 0, 0); // 发送AT指令
    return RetValue;
}
/***********************************************************
 *函数功能: 获取AP版本
 *参数说明: 无
 *返 回 值: STAT_OK /STAT_ERROR
 */
Status_t modem_get_EGAPR()
{
    Status_t RetValue = STAT_ERROR;
    RetValue = monitor_send_AT_cmd(MODEM_CMD_QUERY_EGAPR, 0, 0, 0, 0, 0); // 发送AT指令
    return RetValue;
}
/***********************************************************
 *函数功能: 获取硬件版本
 *参数说明: 无
 *返 回 值: STAT_OK /STAT_ERROR
 */
Status_t modem_get_EGHWR()
{
    Status_t RetValue = STAT_ERROR;
    RetValue = monitor_send_AT_cmd(MODEM_CMD_QUERY_EGHWR, 0, 0, 0, 0, 0); // 发送AT指令
    return RetValue;
}

/***********************************************************
 *函数功能: 获取软件版本
 *参数说明: 无
 *返 回 值: STAT_OK /STAT_ERROR
 */
Status_t modem_get_ECUSR()
{
    Status_t RetValue = STAT_ERROR;
    RetValue = monitor_send_AT_cmd(MODEM_CMD_QUERY_ECUSR, 0, 0, 0, 0, 0); // 发送AT指令
    return RetValue;
}
/***********************************************************
 *函数功能: 获取模块的版本信息
 *返 回 值: STAT_OK /STAT_ERROR
 */
Status_t modem_get_ver()
{
    Status_t RetValue = STAT_ERROR;
    uint8_t i = 0;
    for (i = 0; i < 3; i++)
    {
        RetValue = modem_get_EGMR();
        if (RetValue == STAT_OK)
        {
            rtt_printf(RTT_MODEM, RTT_CTRL_TEXT_BRIGHT_YELLOW "modem_get_EGMR ok \r\n");
            break;
        }
        else
        {
            rtt_printf(RTT_MODEM, RTT_CTRL_TEXT_BRIGHT_YELLOW "modem_get_EGMR fail \r\n");
            vTaskDelay(2000);
        }
    }

    for (i = 0; i < 3; i++)
    {

        RetValue = modem_get_EGAPR();
        if (RetValue == STAT_OK)
        {
            rtt_printf(RTT_MODEM, RTT_CTRL_TEXT_BRIGHT_YELLOW "modem_get_EGAPR ok \r\n");
            break;
        }
        else
        {
            rtt_printf(RTT_MODEM, RTT_CTRL_TEXT_BRIGHT_YELLOW "modem_get_EGAPR fail \r\n");
            vTaskDelay(2000);
        }
    }

    for (i = 0; i < 3; i++)
    {

        RetValue = modem_get_EGHWR();
        if (RetValue == STAT_OK)
        {
            rtt_printf(RTT_MODEM, RTT_CTRL_TEXT_BRIGHT_YELLOW "modem_get_EGHWR ok \r\n");
            break;
        }
        else
        {
            rtt_printf(RTT_MODEM, RTT_CTRL_TEXT_BRIGHT_YELLOW "modem_get_EGHWR fail \r\n");
            vTaskDelay(2000);
        }
    }
	
	    for (i = 0; i < 3; i++)
    {
        RetValue = modem_get_ECUSR();
        if (RetValue == STAT_OK)
        {
            rtt_printf(RTT_MODEM, RTT_CTRL_TEXT_BRIGHT_YELLOW "modem_get_ECUSR ok \r\n");
            break;
        }
        else
        {
            rtt_printf(RTT_MODEM, RTT_CTRL_TEXT_BRIGHT_YELLOW "modem_get_ECUSR fail \r\n");
            vTaskDelay(2000);
        }
    }

    return RetValue;
}

/*********************************************************************
 * 函 数 名：void vModemRun(void)
 * 功能描述：modem业务处理函数,用于器件初始化,创建和保持sock连接,modem状态监控,错误处理,收包分类等
 * 参数说明：无
 * 函数输出：无
 *********************************************************************/

void modem_run(void)
{
    if (g_modem_upgrade_state == 1)
    {
        /*升级状态*/
    }
    else
    {
        vModemProcess();                                                               // modem状态机,完成初始化,配置,搜网流程管理,掉网状态监控
        if ((hModem.modem_stat != MODEM_REBOOT) && (hModem.modem_stat != MODEM_RESET)) // 非复位态才响应应用程序触发的配置
        {
            modemconfig_state_control(); // 根据PC参数控制状态机的状态
        }
        // modemconfig_state_control();	 //根据PC参数控制状态机的状态
        ModemConfig();
    }
}

/***********************************************************
 *函数功能: 网络制式设置
 *参数说明: 0-NR 1-LTE 2-NR&LTE
 *返 回 值: STAT_OK /STAT_ERROR
 */
Status_t modem_set_network_format(uint8_t network_format)
{
    Status_t RetValue = STAT_ERROR;
    RetValue = monitor_send_AT_cmd(MODEM_CMD_SET_ERAT, network_format, 0, 0, 0, 0); // 发送AT指令
    return RetValue;
}

/***********************************************************
 *函数功能: 锁band 和 频点
 *参数说明: network_channel  CH_NR 0 CH_LTE 1;
 *          band 频带eg:band=41 则锁定N41;
 *          arfcn: 频点字符串, 多个频点之间用','分隔, 字符串中不得有空格
 *返 回 值: STAT_OK /STAT_ERROR
 */

Status_t modem_lock_band(uint8_t network_channel, uint8_t band, char *arfcn)
{
    Status_t RetValue = STAT_ERROR;

    modem_write_freq_point_to_buf(arfcn);

    RetValue = monitor_send_AT_cmd(MODEM_CMD_SET_LOCK_EBAND, network_channel, band, 0, 0, 0);

    return RetValue;
}

/***********************************************************
 *函数功能: 解锁band
 *参数说明: network_channel  CH_NR 0 CH_LTE 1;band 频带eg:band=41 则解锁N41; 注意如果network_channel=0xFF,则解锁LTE和NR所有BANDS
 *返 回 值: STAT_OK /STAT_ERROR
 */
Status_t modem_unlock_band(uint8_t network_channel, uint8_t band)
{
    Status_t RetValue = STAT_ERROR;
    RetValue = monitor_send_AT_cmd(MODEM_CMD_SET_UNLOCK_EBAND, network_channel, band, 0, 0, 0); // 发送AT指令
    return RetValue;
}
/***********************************************************
 *函数功能: 停止驻网和输出,切换到最小功能模式,配置band(或更改其他配置)前需要先调用此函数,配置完成后重新扫描驻网.
 *返 回 值: STAT_OK,STAT_ERROR
 */
Status_t modem_stop(void) // CFUN=0
{
    Status_t RetValue = STAT_ERROR;
    RetValue = monitor_send_AT_cmd(MODEM_CMD_SET_CFUN_0, 0, 0, 0, 0, 0); // 发送AT指令
    hModem.LTE_net_info.LTE_Sync = SYNC_NULL;
    hModem.NR_net_info.NR_Sync = SYNC_NULL;
    return RetValue;
}

/***********************************************************
 *函数功能: 开始搜网,过程可能需要等待一段时间
 *参数说明: 无
 *返 回 值: STAT_OK /STAT_ERROR
 */
Status_t modem_start_scan()
{
    Status_t RetValue = STAT_ERROR;
    memset(&hModem.scan_result, 0, sizeof(hModem.scan_result));
    RetValue = monitor_send_AT_cmd(MODEM_CMD_SET_CFUN_1, 0, 0, 0, 0, 0); // 发送AT指令
    return RetValue;
}

/***********************************************************
 *函数功能: 返回搜网结果
 *参数说明: ptr_scan_result 出参,指针指向扫描结果
 *参数说明: pci 小区号
 *返 回 值: 搜网结束STAT_OK,正在搜网STAT_BUSY,其他STAT_ERROR
 */
Status_t modem_get_scan_result(net_scan_result_t *ptr_scan_result)
{
    Status_t RetValue = STAT_ERROR;
    uint8_t i = 0;
    uint32_t max_rsrp_lte = 0;
    uint32_t max_rsrp_nr = 0;
    if (ptr_scan_result == NULL)
    {
        return STAT_ERROR;
    }

    if ((hModem.modem_stat == MODEM_SCAN_DONE) || (hModem.modem_stat == MODEM_LTE_ATTACHED) || (hModem.modem_stat == MODEM_NR_ATTACHED)) // 搜网结束
    {
        ptr_scan_result = &hModem.scan_result;

#if 0
        rtt_printf(RTT_MODEM, RTT_CTRL_TEXT_BRIGHT_YELLOW "Scan_stat =%d \r\n", hModem.scan_result.Scan_stat);
        rtt_printf(RTT_MODEM, RTT_CTRL_TEXT_BRIGHT_YELLOW "LTE_scan_pci_num =%d \r\n", hModem.scan_result.LTE_scan_pci_num);
        for(i = 0; i < hModem.scan_result.LTE_scan_pci_num; i++)
        {
            rtt_printf(RTT_MODEM, RTT_CTRL_TEXT_BRIGHT_YELLOW "LTE_scan_tab[%d]=%d,%d,%d,%d,%d,%d,%d \r\n", i, hModem.scan_result.LTE_scan_tab[i].band, hModem.scan_result.LTE_scan_tab[i].freq,
                       hModem.scan_result.LTE_scan_tab[i].pci, hModem.scan_result.LTE_scan_tab[i].bw, hModem.scan_result.LTE_scan_tab[i].rsrp,
                       hModem.scan_result.LTE_scan_tab[i].sinr, hModem.scan_result.LTE_scan_tab[i].TDD_or_FDD,
                       hModem.scan_result.LTE_scan_tab[i].sfa, hModem.scan_result.LTE_scan_tab[i].ssfp);
        }

        rtt_printf(RTT_MODEM, RTT_CTRL_TEXT_BRIGHT_YELLOW "NR_scan_pci_num =%d \r\n", hModem.scan_result.NR_scan_pci_num);
        for(i = 0; i < hModem.scan_result.NR_scan_pci_num; i++)
        {
            rtt_printf(RTT_MODEM, RTT_CTRL_TEXT_BRIGHT_YELLOW "NR_scan_tab[%d]=%d,%d,%d,%d,%d,%d,%d \r\n", i, hModem.scan_result.NR_scan_tab[i].band, hModem.scan_result.NR_scan_tab[i].bw,
                       hModem.scan_result.NR_scan_tab[i].freq, hModem.scan_result.NR_scan_tab[i].pci, hModem.scan_result.NR_scan_tab[i].scs,
                       hModem.scan_result.NR_scan_tab[i].rsrp, hModem.scan_result.NR_scan_tab[i].sinr);
        }

        rtt_printf(RTT_MODEM, RTT_CTRL_TEXT_BRIGHT_YELLOW "ptr_scan_result.Scan_stat =%d \r\n", (*ptr_scan_result).Scan_stat);
        rtt_printf(RTT_MODEM, RTT_CTRL_TEXT_BRIGHT_YELLOW "ptr_scan_result.LTE_scan_pci_num =%d \r\n", ptr_scan_result->LTE_scan_pci_num);
        for(i = 0; i < ptr_scan_result->LTE_scan_pci_num; i++)
        {
            rtt_printf(RTT_MODEM, RTT_CTRL_TEXT_BRIGHT_YELLOW "ptr_scan_result.LTE_scan_tab[%d]=%d,%d,%d,%d,%d,%d,%d \r\n", i, ptr_scan_result->LTE_scan_tab[i].band, ptr_scan_result->LTE_scan_tab[i].freq,
                       ptr_scan_result->LTE_scan_tab[i].pci, ptr_scan_result->LTE_scan_tab[i].bw, ptr_scan_result->LTE_scan_tab[i].rsrp,
                       ptr_scan_result->LTE_scan_tab[i].sinr, ptr_scan_result->LTE_scan_tab[i].TDD_or_FDD,
                       ptr_scan_result->LTE_scan_tab[i].sfa, ptr_scan_result->LTE_scan_tab[i].ssfp);
        }
        rtt_printf(RTT_MODEM, RTT_CTRL_TEXT_BRIGHT_YELLOW "\r\n");
        rtt_printf(RTT_MODEM, RTT_CTRL_TEXT_BRIGHT_YELLOW "ptr_scan_result.NR_scan_pci_num =%d \r\n", ptr_scan_result->NR_scan_pci_num);
        for(i = 0; i < ptr_scan_result->NR_scan_pci_num; i++)
        {
            rtt_printf(RTT_MODEM, RTT_CTRL_TEXT_BRIGHT_YELLOW "ptr_scan_result->NR_scan_tab[%d]=%d,%d,%d,%d,%d,%d,%d \r\n", i, ptr_scan_result->NR_scan_tab[i].band, ptr_scan_result->NR_scan_tab[i].bw,
                       ptr_scan_result->NR_scan_tab[i].freq, ptr_scan_result->NR_scan_tab[i].pci, ptr_scan_result->NR_scan_tab[i].scs,
                       ptr_scan_result->NR_scan_tab[i].rsrp, ptr_scan_result->NR_scan_tab[i].sinr);
        }
#endif

        RetValue = STAT_OK;
        ////找出rsrp最大值
        for (i = 0; i < hModem.scan_result.LTE_scan_pci_num; i++)
        {
            if (hModem.scan_result.LTE_scan_tab[i].rsrp > max_rsrp_lte)
            {
                max_rsrp_lte = hModem.scan_result.LTE_scan_tab[i].rsrp;
                max_index_lte = i;
            }
        }

        rtt_printf(RTT_MODEM, RTT_CTRL_TEXT_BRIGHT_YELLOW "max_rsrp_lte=%d, max_index_lte=%d\r\n", max_rsrp_lte, max_index_lte);

        for (i = 0; i < hModem.scan_result.NR_scan_pci_num; i++)
        {
            if (hModem.scan_result.LTE_scan_tab[i].rsrp > max_rsrp_nr)
            {
                max_rsrp_nr = hModem.scan_result.NR_scan_tab[i].rsrp;
                max_index_nr = i;
            }
        }
        rtt_printf(RTT_MODEM, RTT_CTRL_TEXT_BRIGHT_YELLOW "max_rsrp_nr=%d, max_index_nr=%d\r\n", max_rsrp_nr, max_index_nr);
    }
    else if ((hModem.modem_stat == MODEM_SCAN_LTE) || (hModem.modem_stat == MODEM_SCAN_NR)) // 正在搜网
    {
        RetValue = STAT_BUSY;
    }
    else
    {
    }

    return RetValue;
}
/***********************************************************
 *函数功能: 驻网lte小区,驻网成功后输出时序
 *参数说明: freq 频点,pci 小区号,band 频带
 *返 回 值: STAT_OK /STAT_ERROR
 */
Status_t modem_tracking_lte_cell(uint32_t freq, uint32_t pci, uint8_t band)
{
    Status_t RetValue = STAT_ERROR;
    RetValue = monitor_send_AT_cmd(MODEM_CMD_SET_ECELLGPIO_LTE, freq, pci, band, 0, 0); // 发送AT指令
    return RetValue;
}

/***********************************************************
 *函数功能: 驻网NR小区,驻网成功后输出时序
 *参数说明: freq 频点,pci 小区号,band 频带
 *返 回 值: STAT_OK /STAT_ERROR
 */
Status_t modem_tracking_NR_cell(uint32_t freq, uint32_t pci, uint8_t band) // nr2?
{
    Status_t RetValue = STAT_ERROR;
    RetValue = monitor_send_AT_cmd(MODEM_CMD_SET_ECELLGPIO_NR, freq, pci, band, 0, 0); // 发送AT指令
    return RetValue;
}

/***********************************************************
 *函数功能: 时序GPIO输出模式设置,输出常高/常低/TDD时序
 *参数说明: gpio_group: gpio组号, 0-2    gpio_mode_up/gpio_mode_dw：  HI 0 LO 1 TDD 2
 *返 回 值: STAT_OK /STAT_ERROR
 */
Status_t modem_set_channel_output_mode(uint8_t gpio_group, gpio_sync_mode_E gpio_mode_up, gpio_sync_mode_E gpio_mode_dw)
{
    Status_t RetValue = STAT_ERROR;
    rtt_printf(RTT_MODEM, "modem_set_channel_output_mode \r\n");
    RetValue = monitor_send_AT_cmd(MODEM_CMD_SET_EGPIOSIGNAL, gpio_group, gpio_mode_up, gpio_mode_dw, 0, 0); // 发送AT指令
    return RetValue;
}

/***********************************************************
 *函数功能: 时序GPIO输出帧配比和特殊子帧结构配置
 *参数说明: gpio_group: gpio组号, 0-2
 *参数说明: gpio_timing :设置子帧比例,特殊子帧比例   cfg_mode: 0表示单时隙配置，1表示双时隙配置
 *返 回 值: STAT_OK /STAT_ERROR
 */
Status_t modem_set_gpio_timing_config(uint8_t gpio_group, timing_config_t gpio_timing, uint8_t cfg_mode)
{
    Status_t RetValue = STAT_ERROR;

    if (gpio_group == hModem.hw_config.lte_gpio_group)
    {
        memcpy(&hModem.LTE_timing_config, &gpio_timing, sizeof(hModem.LTE_timing_config));
    }
    else if (gpio_group == hModem.hw_config.nr_gpio_group)
    {
        memcpy(&hModem.NR_timing_config, &gpio_timing, sizeof(hModem.NR_timing_config));
    }
    else if (gpio_group == hModem.hw_config.nr2_gpio_group)
    {
        memcpy(&hModem.NR2_timing_config, &gpio_timing, sizeof(hModem.NR2_timing_config));
    }
    else
    {
    }

    RetValue = monitor_send_AT_cmd(MODEM_CMD_SET_EL1CFG, gpio_group, cfg_mode, 0, 0, 0); // 发送AT指令
    return RetValue;
}

/***********************************************************
 *函数功能: 设置输出时序偏移
 *参数说明: network_channel  CH_NR 0 CH_LTE 1 CH_NR2 2
 *参数说明: ul_rising  上行信号上升沿调整量，有符号整型。负值向左偏移，正值向右偏移。单位us
 *参数说明: ul_falling  上行信号下降沿调整量，有符号整型。负值向左偏移，正值向右偏移。单位us
 *参数说明: dl_rising  下行信号上升沿调整量，有符号整型。负值向左偏移，正值向右偏移。单位us
 *参数说明: dl_falling  下行信号下降沿调整量，有符号整型。负值向左偏移，正值向右偏移。单位us
 *返 回 值: STAT_OK /STAT_ERROR
 */
Status_t modem_set_gpio_timing_offset(uint8_t gpio_group, int ul_rising, int ul_falling, int dl_rising, int dl_falling)
{
    Status_t RetValue = STAT_ERROR;
    RetValue = monitor_send_AT_cmd(MODEM_CMD_SET_EGPIOADJ, gpio_group, ul_rising, ul_falling, dl_rising, dl_falling);
    return RetValue;
}
/***********************************************************
 *函数功能: NR2时序是根据NR时序加指定偏移量生成的
 *参数说明: time_offset  NR2的时序偏移量
 *返 回 值: STAT_OK /STAT_ERROR
 */
Status_t modem_set_NR2_offset(uint32_t time_offset)
{
    Status_t RetValue = STAT_ERROR;
    RetValue = monitor_send_AT_cmd(MODEM_CMD_SET_ETIMEOFFSET, time_offset, 0, 0, 0, 0); // 发送AT指令
    return RetValue;
}

/***********************************************************
 *函数功能: 刷新网络信息
 *返 回 值: STAT_OK /STAT_ERROR
 */
Status_t modem_refresh_net_info(void)
{
    Status_t RetValue = STAT_ERROR;
    if ((hModem.modem_stat == MODEM_LTE_ATTACHED) || (hModem.modem_stat == MODEM_NR_ATTACHED))
    {
        RetValue = monitor_send_AT_cmd(MODEM_CMD_QUARY_ECELL, 0, 0, 0, 0, 0); // 发送AT指令
        //rtt_printf(RTT_MODEM, RTT_CTRL_TEXT_BRIGHT_CYAN "modem_refresh_net_info!\r\n");
    }
    return RetValue;
}

/***********************************************************
 *函数功能: 获取网络信息,注意定时调用刷新modem_refresh_net_info以保证获取更新的状态
 *返 回 值: STAT_OK /STAT_ERROR
 */
Status_t modem_get_net_info(LTE_net_info_t *lte_info, NR_net_info_t *nr_info) // 未使用
{
    if ((lte_info == NULL) || (nr_info == NULL))
    {
        return STAT_ERROR;
    }
    lte_info = &hModem.LTE_net_info;
    nr_info = &hModem.NR_net_info;
    Status_t RetValue = STAT_ERROR;
    RetValue = monitor_send_AT_cmd(MODEM_CMD_QUARY_ECELL, 0, 0, 0, 0, 0); // 发送AT指令
    return RetValue;
}

/***********************************************************
 *函数功能: 获取模块的状态
 *返 回 值: 模块状态机状态
    模块初始态 0,
    模块空闲态 1,
    模块LTE扫描态 2,
    模块NR扫描态 3,
    模块扫描结束态 4,
    模块LTE驻网态 5,
    模块NR驻网态 6,
    模块复位态 7,
 */
Modem_stat_E modem_get_state(void)
{
    Modem_stat_E RetValue = hModem.modem_stat;
    return RetValue;
}
/***********************************************************
 *函数功能: 获取模块的频带band信息
 *返 回 值: STAT_OK /STAT_ERROR
 */
Status_t modem_get_band(void)
{
    Status_t RetValue = STAT_ERROR;
    RetValue = monitor_send_AT_cmd(MODEM_CMD_QUERY_BAND, 0, 0, 0, 0, 0); // 发送AT指令
    return RetValue;
}

/***********************************************************
 *函数功能: 获取模块的运行模式
 *返 回 值: STAT_OK /STAT_ERROR
 */
Status_t modem_get_fun(void)
{
    Status_t RetValue = STAT_ERROR;
    RetValue = monitor_send_AT_cmd(MODEM_CMD_QUERY_CFUN, 0, 0, 0, 0, 0); // 发送AT指令
    return RetValue;
}
/***********************************************************
 *函数功能: 获取模块的网络制式
 *返 回 值: STAT_OK /STAT_ERROR
 */
Status_t modem_get_erat(void)
{
    Status_t RetValue = STAT_ERROR;
    RetValue = monitor_send_AT_cmd(MODEM_CMD_QUERY_ERAT, 0, 0, 0, 0, 0); // 发送AT指令
    return RetValue;
}

/***********************************************************
 *函数功能: 初始化上层应用配置模块工作所需的参数
 ********************************************************/

void init_modem_config_buf()
{
    pc_config_modem_buf.device_mode = DEVICE_RUN_MODE;

    char *p_freq_init = &buffer_freq[0];

    pc_config_modem_buf.band[3] = 40;
    memset(p_freq_init, 0, sizeof(buffer_freq[0]) * FREQ_BUF_SIZE);
    p_freq_init = "38950,39148,39100,39250";
    memset(pc_config_modem_buf.freq_40, 0, sizeof(pc_config_modem_buf.freq_40[0]) * FREQ_BUF_SIZE);
    memcpy(pc_config_modem_buf.freq_40, p_freq_init, strlen(p_freq_init));

    rtt_printf(RTT_MODEM, "pc_config_modem_buf.freq_40=%s!\r\n", pc_config_modem_buf.freq_40);

    pc_config_modem_buf.band[4] = 41;
    memset(p_freq_init, 0, sizeof(buffer_freq[0]) * FREQ_BUF_SIZE);
    p_freq_init = "504990,510990,516990";

    memset(pc_config_modem_buf.freq_41, 0, sizeof(pc_config_modem_buf.freq_41[0]) * FREQ_BUF_SIZE);
    memcpy(pc_config_modem_buf.freq_41, p_freq_init, strlen(p_freq_init));

    rtt_printf(RTT_MODEM, "pc_config_modem_buf.freq_41=%s!\r\n", pc_config_modem_buf.freq_41);

    pc_config_modem_buf.band[0] = 3;
    memset(p_freq_init, 0, sizeof(buffer_freq[0]) * FREQ_BUF_SIZE);
    p_freq_init = "1300,1325,1350";
    memset(pc_config_modem_buf.freq_3, 0, sizeof(pc_config_modem_buf.freq_3[0]) * FREQ_BUF_SIZE);
    memcpy(pc_config_modem_buf.freq_3, p_freq_init, strlen(p_freq_init));

    rtt_printf(RTT_MODEM, "pc_config_modem_buf.freq_3=%s!\r\n", pc_config_modem_buf.freq_3);

    pc_config_modem_buf.band[1] = 8;
    memset(p_freq_init, 0, sizeof(buffer_freq[0]) * FREQ_BUF_SIZE);
    p_freq_init = "3590,3615,3640";
    memset(pc_config_modem_buf.freq_8, 0, sizeof(pc_config_modem_buf.freq_8[0]) * FREQ_BUF_SIZE);
    memcpy(pc_config_modem_buf.freq_8, p_freq_init, strlen(p_freq_init));

    rtt_printf(RTT_MODEM, "pc_config_modem_buf.freq_8=%s!\r\n", pc_config_modem_buf.freq_8);

    pc_config_modem_buf.band[1] = 39;
    memset(p_freq_init, 0, sizeof(buffer_freq[0]) * FREQ_BUF_SIZE);
    p_freq_init = "38400,38544,38450,38500";
    memset(pc_config_modem_buf.freq_39, 0, sizeof(pc_config_modem_buf.freq_39[0]) * FREQ_BUF_SIZE);
    memcpy(pc_config_modem_buf.freq_39, p_freq_init, strlen(p_freq_init));
    rtt_printf(RTT_MODEM, "pc_config_modem_buf.freq_39=%s!\r\n", pc_config_modem_buf.freq_39);

    pc_config_modem_buf.band_invalid = 0;
    // pc_config_modem_buf.startup=1;
    pc_config_modem_buf.lte_ul_rising = 0;
    pc_config_modem_buf.lte_ul_falling = -100;
    pc_config_modem_buf.lte_dl_rising = 100;
    pc_config_modem_buf.lte_dl_falling = 0;
    pc_config_modem_buf.nr_ul_rising = 0;
    pc_config_modem_buf.nr_ul_falling = -100;
    pc_config_modem_buf.nr_dl_rising = 100;
    pc_config_modem_buf.nr_dl_falling = 0;
}

BOOL_E rf_5g_modem_scan_done()
{
    if (MODEM_SCAN_DONE == modem_get_state())
        return B_TRUE;
    else
        return B_FALSE;
}

/**
 * @brief   设置各频段的扫描频点
 * @param   band: BAND_3 / BAND_8 / ....
 **
 * @return  STAT_OK / STAT_ERROR
 * @data    2024-03-22
 * @note
 */

Status_t set_band_freq(RF_BAND_E band)
{
    char *p_freq_init = NULL;
    p_freq_init = (char *)get_band_freq_list_addr(band);
    if (p_freq_init != NULL)
    {
        if (band == 0)
        {
            memset(pc_config_modem_buf.freq_3, 0, sizeof(pc_config_modem_buf.freq_3[0]) * FREQ_BUF_SIZE);
            memcpy(pc_config_modem_buf.freq_3, p_freq_init, strlen(p_freq_init));
        }
        else if (band == 1)
        {
            memset(pc_config_modem_buf.freq_8, 0, sizeof(pc_config_modem_buf.freq_8[0]) * FREQ_BUF_SIZE);
            memcpy(pc_config_modem_buf.freq_8, p_freq_init, strlen(p_freq_init));
        }
        else if (band == 2)
        {
            memset(pc_config_modem_buf.freq_39, 0, sizeof(pc_config_modem_buf.freq_39[0]) * FREQ_BUF_SIZE);
            memcpy(pc_config_modem_buf.freq_39, p_freq_init, strlen(p_freq_init));
        }
        else if (band == 3)
        {
            memset(pc_config_modem_buf.freq_40, 0, sizeof(pc_config_modem_buf.freq_40[0]) * FREQ_BUF_SIZE);
            memcpy(pc_config_modem_buf.freq_40, p_freq_init, strlen(p_freq_init));
        }
        else if ((band == 4) || (band == 5))
        {
            memset(pc_config_modem_buf.freq_41, 0, sizeof(pc_config_modem_buf.freq_41[0]) * FREQ_BUF_SIZE);
            memcpy(pc_config_modem_buf.freq_41, p_freq_init, strlen(p_freq_init));
        }
        else
        {
        }
        return STAT_OK;
    }
    else
    {
        return STAT_ERROR;
    }
}

/**
 * @brief   各频段band编码转换
 * @param   band: BAND_3 / BAND_8 / ....
 **
 * @return  band 值 3/8/39/40/41
 * @data    2024-03-22
 * @note
 */
uint8_t bandcode_to_bandval(RF_BAND_E band)
{
    uint8_t ret_bandval = 0;
    if (band == BAND_3)
    {
        ret_bandval = 3;
    }
    else if (band == BAND_8)
    {
        ret_bandval = 8;
    }
    else if (band == BAND_39)
    {
        ret_bandval = 39;
    }
    else if (band == BAND_40)
    {
        ret_bandval = 40;
    }
    else if (band == BAND_41)
    {
        ret_bandval = 41;
    }
    else if (band == BAND_41M)
    {
        ret_bandval = 41;
    }
    else
    {
    }
    return ret_bandval;
}

/***********************************************************
 *函数功能: 设置模块的上下行切换点间隔
 *返 回 值: STAT_OK /STAT_ERROR
 */
Status_t modem_set_lte_nr_timing_offset()
{
    Status_t RetValue = STAT_ERROR;
    uint8_t i = 0;
    for (i = 0; i < 3; i++)
    {

        RetValue = modem_set_gpio_timing_offset(hModem.hw_config.lte_gpio_group, pc_config_modem_buf.lte_ul_rising, pc_config_modem_buf.lte_ul_falling, pc_config_modem_buf.lte_dl_rising, pc_config_modem_buf.lte_dl_falling); // AT+EGPIOADJ
        if (RetValue == STAT_OK)
        {
            rtt_printf(RTT_MODEM, "modem_set_gpio_timing_offset lte ok!\r\n");
            break;
        }
        else
        {
            rtt_printf(RTT_MODEM, "modem_set_gpio_timing_offset lte fail!\r\n");
            vTaskDelay(2000);
        }
    }

    for (i = 0; i < 3; i++)
    {

        RetValue = modem_set_gpio_timing_offset(hModem.hw_config.nr_gpio_group, pc_config_modem_buf.nr_ul_rising, pc_config_modem_buf.nr_ul_falling, pc_config_modem_buf.nr_dl_rising, pc_config_modem_buf.nr_dl_falling); // AT+EGPIOADJ
        if (RetValue == STAT_OK)
        {
            rtt_printf(RTT_MODEM, "modem_set_gpio_timing_offset nr ok!\r\n");
            break;
        }
        else
        {
            rtt_printf(RTT_MODEM, "modem_set_gpio_timing_offset nr fail!\r\n");
            vTaskDelay(2000);
        }
    }

    return RetValue;
}
/***********************************************************
 *函数功能: 同步模块配置状态机的状态控制，由上层应用通过pc_config_modem_buf.startup触发
 ********************************************************/

void modemconfig_state_control()
{
    if (pc_config_modem_buf.startup == 1) // 启动模块配置
    {
        // 设备模式切换后清除相关变量，状态机状态
        vModemRxDataClear(); // 接收缓存清空
        vModemTxDataClear(); // 发送缓存清空

        rtt_printf(RTT_MODEM, RTT_CTRL_TEXT_BRIGHT_GREEN "modemconfig_state_control\r\n");
        hModem.modem_stat = MODEM_INIT;
        hModem.scan_result.Scan_stat = SCAN_IDLE;
        hModem.modem_config_state = CONFIG_CFUN0;

        memset(&hModem.modem_command_reback_info, 0, sizeof(hModem.modem_command_reback_info));

        memset(&hModem.NR_net_info, 0, sizeof(hModem.NR_net_info));
        memset(&hModem.LTE_net_info, 0, sizeof(hModem.LTE_net_info));
        pc_config_modem_buf.band_invalid = 0;
        pc_config_modem_buf.startup = 2; // 代表已启动
    }
    else if (pc_config_modem_buf.startup == 0) // 未启动模块配置
    {
        hModem.modem_config_state = CONFIG_IDLE;
    }
    else
    {
    }

    /////上下行切换点间隔设置
    if (pc_config_modem_buf.flg_set_timing_offset == 0)
    {
        if (modem_set_lte_nr_timing_offset() == STAT_OK)
        {
            pc_config_modem_buf.flg_set_timing_offset = 1; // 设置标志
        }
    }
}

/***********************************************************
 *函数功能: 同步模块配置流程实现
 ********************************************************/
void ModemConfig()
{
    const TickType_t xTicksToWait = 1000;
    Status_t RetValue = STAT_ERROR;
    uint8_t i = 0;
    BOOL_E flag_nr = B_FALSE, flag_lte = B_FALSE;
    net_scan_result_t *ptr_scan_result1 = &hModem.scan_result;
    LTE_net_info_t *lte_info = &hModem.LTE_net_info;
    NR_net_info_t *nr_info = &hModem.NR_net_info;
    char *p_freq = &buffer_freq[0];

    uint8_t ch_tle_nr = 0;
    uint8_t cnt_invalid_band = 0;
    uint8_t flg_index_invalid = 0;

    UNUSED_PARA(i);
    UNUSED_PARA(lte_info);
    UNUSED_PARA(nr_info);

    switch (hModem.modem_config_state)
    {
    case CONFIG_IDLE: // 空闲态  可配置GPIO信号格式（单通/关闭/开FDD）

        break;

    case CONFIG_CFUN0: // 设置最小工作模式
        // 读取版本
        if (hModem.flg_get_ver == 1)
        {
            if (modem_get_ver() == STAT_OK)
            {
                hModem.flg_get_ver = 0;
                rtt_printf(RTT_MODEM, RTT_CTRL_TEXT_BRIGHT_YELLOW "modem_get_ver ok \r\n");

                rtt_printf(RTT_MODEM, RTT_CTRL_TEXT_BRIGHT_YELLOW "hModem.TAFirmVer=%s\r\nhModem.APVer=%s\r\nhModem.HWVer=%s\r\nhModem.SWVer=%s\r\n", hModem.TAFirmVer, hModem.APVer, hModem.HWVer,hModem.SWVer);
            }
        }

        RetValue = modem_stop();
        if (RetValue == STAT_OK)
        {
            hModem.modem_config_state = CONFIG_FORMAT;
            rtt_printf(RTT_MODEM, RTT_CTRL_TEXT_BRIGHT_GREEN "modem_stop ok!\r\n");
            break;
        }
        else
        {
            rtt_printf(RTT_MODEM, "modem_stop fail!\r\n");
            vTaskDelay(xTicksToWait);
        }

        // hModem.modem_config_state = CONFIG_FORMAT;
        // rtt_printf(RTT_MODEM, RTT_CTRL_TEXT_BRIGHT_GREEN "CONFIG_CFUN0 done!!\r\n");
        break;

    case CONFIG_FORMAT: // 网络制式设置

        rtt_printf(RTT_MODEM, "band[0]=%d,rat=%d\r\n", pc_config_modem_buf.band[0], pc_config_modem_buf.band_net_mode_set[0]);
        rtt_printf(RTT_MODEM, "band[1]=%d,rat=%d\r\n", pc_config_modem_buf.band[1], pc_config_modem_buf.band_net_mode_set[1]);
        rtt_printf(RTT_MODEM, "band[2]=%d,rat=%d\r\n", pc_config_modem_buf.band[2], pc_config_modem_buf.band_net_mode_set[2]);
        rtt_printf(RTT_MODEM, "band[3]=%d,rat=%d\r\n", pc_config_modem_buf.band[3], pc_config_modem_buf.band_net_mode_set[3]);
        rtt_printf(RTT_MODEM, "band[4]=%d,rat=%d\r\n", pc_config_modem_buf.band[4], pc_config_modem_buf.band_net_mode_set[4]);

        for (i = 0; i < sizeof(pc_config_modem_buf.band); i++)
        {
            if (pc_config_modem_buf.band[i] == 3 || pc_config_modem_buf.band[i] == 8 || pc_config_modem_buf.band[i] == 39 || pc_config_modem_buf.band[i] == 40 || pc_config_modem_buf.band[i] == 41)
            {
                if (pc_config_modem_buf.band_net_mode_set[i] == NR)
                    flag_nr = B_TRUE;
                else if (pc_config_modem_buf.band_net_mode_set[i] == LTE)
                    flag_lte = B_TRUE;
            }
        }

        if (flag_nr == B_TRUE && flag_lte == B_TRUE)
        {
            pc_config_modem_buf.modem_network = LTE_NR;
        }
        else if (flag_nr == B_TRUE)
        {
            pc_config_modem_buf.modem_network = NR;
        }
        else if (flag_lte == B_TRUE)
        {
            pc_config_modem_buf.modem_network = LTE;
        }
        else
        {
            hModem.modem_config_state = CONFIG_ERRHANDLE;
            ; // 无效网络制式
            break;
        }

        RetValue = modem_set_network_format(pc_config_modem_buf.modem_network); // 设置网络制式0-NR 1-LTE 2-NR&LTE   LTE_NR
        if (RetValue == STAT_OK)
        {
            hModem.modem_config_state = CONFIG_UNLOCKBAND;
            rtt_printf(RTT_MODEM, "modem_set_network_format ok!\r\n");
            break;
        }
        else
        {
            rtt_printf(RTT_MODEM, "modem_set_network_format fail!\r\n");
            vTaskDelay(xTicksToWait);
        }
        break;

    case CONFIG_UNLOCKBAND: // 解锁所有频点

        RetValue = modem_unlock_band(0xFF, 0);
        vTaskDelay(xTicksToWait);

        if (RetValue == STAT_OK)
        {
            hModem.modem_config_state = CONFIG_LOCKBAND;
            rtt_printf(RTT_MODEM, "modem_unlock_band lte ok!\r\n");
            break;
        }
        else
        {
            rtt_printf(RTT_MODEM, "modem_unlock_band lte fail!\r\n");
            vTaskDelay(xTicksToWait);
        }

        break;

    case CONFIG_LOCKBAND: // 配置频带、频点

        for (index = 0; index < 5; index++) // BAND_END
        {
            memset(p_freq, 0, sizeof(buffer_freq[0]) * FREQ_BUF_SIZE);
            flg_index_invalid = 0;

            ch_tle_nr = pc_config_modem_buf.band_net_mode_set[index]; // 获取band对应的LTE/NR制式

            if (ch_tle_nr != NR && ch_tle_nr != LTE) // 无效网络制式,跳过
            {
                cnt_invalid_band++;
                continue;
            }

            rtt_printf(RTT_MODEM, "ch_tle_nr=%d,index=%d\r\n", ch_tle_nr, index);
            // if(STAT_OK!=set_band_freq(index))
            //{
            //     rtt_printf(RTT_MODEM, "get_set bandfreq fail\r\n"); //获取频点并设置进pc_config_modem_buf变量
            // }
            switch (pc_config_modem_buf.band[index])
            {
            case 3:
                memcpy(p_freq, &pc_config_modem_buf.freq_3[0], strlen((char *)&pc_config_modem_buf.freq_3[0]));
                break;

            case 8:
                memcpy(p_freq, &pc_config_modem_buf.freq_8[0], strlen((char *)&pc_config_modem_buf.freq_8[0]));
                break;

            case 39:
                memcpy(p_freq, &pc_config_modem_buf.freq_39[0], strlen((char *)&pc_config_modem_buf.freq_39[0]));
                break;

            case 40:
                memcpy(p_freq, &pc_config_modem_buf.freq_40[0], strlen((char *)&pc_config_modem_buf.freq_40[0]));
                break;

            case 41:
                memcpy(p_freq, &pc_config_modem_buf.freq_41[0], strlen((char *)&pc_config_modem_buf.freq_41[0]));
                break;

            default:
                flg_index_invalid = 1;
                cnt_invalid_band++;
                break;
            }
            // rtt_printf(RTT_MODEM, "pc_config_modem_buf.band[index]=%d \r\n",pc_config_modem_buf.band[index]);
            // rtt_printf(RTT_MODEM, "p_freq=%s \r\n",p_freq);
            if (flg_index_invalid == 0) // band 有效则设置
            {
                uint8_t cnt = 0;
                for (cnt = 0; cnt < 5; cnt++)
                {
                    RetValue = modem_lock_band(ch_tle_nr, pc_config_modem_buf.band[index], p_freq); // 配置频带、频点
                    if (RetValue == STAT_OK)
                    {
                        rtt_printf(RTT_MODEM, "modem_lock_band %d %dok!\r\n", ch_tle_nr, pc_config_modem_buf.band[index]);
                        break;
                    }
                    else
                    {
                        rtt_printf(RTT_MODEM, "modem_lock_band %d %dfail!\r\n", ch_tle_nr, pc_config_modem_buf.band[index]);
                        vTaskDelay(xTicksToWait);
                    }
                }
                if (cnt >= 5)
                {
                    break;
                }
            }
        }

        if (cnt_invalid_band == 5) //  所有band均无效
        {
            rtt_printf(RTT_MODEM, "cnt_invalid_band==5!\r\n");
            cnt_invalid_band = 0;
            flg_index_invalid = 0;
            pc_config_modem_buf.band_invalid = 0;
            hModem.modem_config_state = CONFIG_ERRHANDLE;
        }

        if (hModem.modem_config_state != CONFIG_ERRHANDLE)
        {
            hModem.modem_config_state = CONFIG_CFUN1;
        }

        break;
    case CONFIG_ERRHANDLE:
        rtt_printf(RTT_MODEM, "band invalid!\r\n");
        break;

    case CONFIG_CFUN1: // 设置全功能模式 搜网

        // 根据对应的制式，设置对应通道的下行单通
        if (hModem.network == LTE)
        {
            rf_set_modem_cmd(0, 3, IS_TDD); // 先配下行单通
            rtt_printf(RTT_MODEM, "LTE down !\r\n");
        }
        else if (hModem.network == NR)
        {
            rf_set_modem_cmd(1, 3, IS_TDD); // 先配下行单通
            rtt_printf(RTT_MODEM, "NR down !\r\n");
        }
        else if (hModem.network == LTE_NR)
        {
            rf_set_modem_cmd(1, 3, IS_TDD); // 先配下行单通
            rf_set_modem_cmd(0, 3, IS_TDD); // 先配下行单通
            rtt_printf(RTT_MODEM, "LTE NR down !\r\n");
        }
        else
        {
        }

        RetValue = modem_start_scan();
        if (RetValue == STAT_OK)
        {
            hModem.modem_config_state = CONFIG_GETNETINF;
            rtt_printf(RTT_MODEM, RTT_CTRL_TEXT_BRIGHT_GREEN "modem_start_scan  ok!\r\n");
            break;
        }
        else
        {
            rtt_printf(RTT_MODEM, "modem_start_scan fail!\r\n");
            vTaskDelay(xTicksToWait);
        }
        break;

    case CONFIG_GETNETINF: // 获取搜网结果 hModem.scan_result.NR_scan_pci_num    hModem.scan_result.LTE_scan_pci_num
        if (hModem.modem_stat == MODEM_SCAN_DONE)
        {
            RetValue = modem_get_scan_result(ptr_scan_result1);
            if (RetValue == STAT_OK)
            {
                hModem.modem_config_state = CONFIG_FINE;
                hModem.LTE_net_info.LTE_Sync = SCAN_UNSYNC;
                hModem.NR_net_info.NR_Sync = SCAN_UNSYNC;
                rtt_printf(RTT_MODEM, RTT_CTRL_TEXT_BRIGHT_GREEN "modem_get_scan_result  ok!\r\n");
                break;
            }
            else
            {
                rtt_printf(RTT_MODEM, RTT_CTRL_TEXT_BRIGHT_YELLOW "modem_get_scan_result fail!\r\n");
                vTaskDelay(xTicksToWait);
            }
        }

        break;

    case CONFIG_FINE:
        // 待上位机驻网 、配置子帧配比、 输出时序
        break;

    default:
        break;
    }
}

/***********************************************************
 *函数功能:  获取模组上电信息
 *返 回 值: B_TRUE 已上电 B_FALSE 未上电
 */
BOOL_E modem_get_pwron_stat(void)
{
    if(hModem.init_done == B_TRUE)
        return B_TRUE;
    else
        return B_FALSE;

}

/***********************************************************
 *函数功能: 模块版本升级接收ACK
 *返 回 值: BURN_OK 成功
 */
int modem_upgrade_recv_ack(uint8_t pkt_type, uint8_t *ack)
{
    uint8_t crc, crc_recv;
    uint16_t data_len, i, usRxLen;

    if (g_modem_upgrade_state != 1)
    {
        return BURN_ERR;
    }

    if (mdm_uart->get_newmsg(mdm_uart->tpt_data) == B_TRUE) /*串口有无新数据包*/
    {
        usRxLen = mdm_uart->read(mdm_uart->tpt_data);
        mdm_uart->clr_newmsg(mdm_uart->tpt_data);
        if (usRxLen == 0)
            return BURN_ERR;
    }
    else
    {
        return BURN_ERR;
    }

    /*check magic*/
    if ((usRxLen < 1) || (*p_rx_buf != PKT_MAGIC_RECV))
    {
        /*rtt_printf(RTT_UPGRADE, RTT_TEXT_RED"modem_upgrade_recv_ack: check magic fail, rx len:%d 0x%x\r\n", usRxLen, *p_rx_buf);*/
        return BURN_ERR_TIMEOUT;
    }

    /*check pkt_type*/
    if ((usRxLen < 2) || (*(p_rx_buf + 1) != pkt_type))
    {
        /*rtt_printf(RTT_UPGRADE, RTT_TEXT_RED"modem_upgrade_recv_ack: check pkt_type fail, rx len:%d 0x%x\r\n", usRxLen, *(p_rx_buf+1));*/
        return BURN_ERR_TIMEOUT;
    }

    /*check data_len*/
    data_len = ((*(p_rx_buf + 3) << 8) & 0xFF00) | ((*(p_rx_buf + 2)) & 0x00FF);
    if ((usRxLen < 4) || (data_len > 5))
    {
        /*rtt_printf(RTT_UPGRADE, RTT_TEXT_RED"modem_upgrade_recv_ack: check data_len fail, rx len:%d data_len:%d\r\n", usRxLen, data_len);*/
        return BURN_ERR_TIMEOUT;
    }

    if (usRxLen != (6 + data_len)) /*magic(1B) pkt_type(1B) data_len(2B) data(<5B) ack(1B)  crc(1B)*/
    {
        /*rtt_printf(RTT_UPGRADE, RTT_TEXT_RED"modem_upgrade_recv_ack: check data_len fail, rx len:%d != %d\r\n", usRxLen, (6+data_len));*/
        return BURN_ERR_TIMEOUT;
    }

    /*check crc*/
    crc = 0;
    crc_recv = *(p_rx_buf + usRxLen - 1);
    for (i = 0; i < (usRxLen - 1); i++)
    {
        crc += *(p_rx_buf + i);
    }
    if (crc != crc_recv)
    {
        /*rtt_printf(RTT_UPGRADE, RTT_TEXT_RED"modem_upgrade_recv_ack: check crc fail, crc:0x%x crc_recv:0x%x\r\n", crc, crc_recv);*/
        return BURN_ERR_TIMEOUT;
    }

    *ack = *(p_rx_buf + usRxLen - 2);

    vModemRxDataClear(); /*接收缓存清空*/

    return BURN_OK;
}

/***********************************************************
 *函数功能: 模块版本升级发送REQ
 *返 回 值: 无
 */
void modem_upgrade_send_req(uint8_t pkt_type, uint8_t *buf_head, uint16_t buf_head_len, uint8_t *buf, uint16_t buf_len)
{
    pkt_head_t pkt_head;
    uint8_t crc = 0, *ptr = (uint8_t *)&pkt_head;
    uint16_t i, off = 0;

    if (g_modem_upgrade_state != 1)
    {
        return;
    }

    /*make send_buf	*/
    pkt_head.magic = PKT_MAGIC_SEMD;
    pkt_head.type = pkt_type;
    pkt_head.length = buf_head_len + buf_len;
    memcpy(p_tx_buf + off, (uint8_t *)&pkt_head, sizeof(pkt_head_t));
    off += sizeof(pkt_head);
    for (i = 0; i < off; i++)
    {
        crc += ptr[i];
    }

    if (buf_head_len > 0)
    {
        for (i = 0; i < buf_head_len; i++)
        {
            crc += buf_head[i];
        }
        memcpy(p_tx_buf + off, buf_head, buf_head_len);
        off += buf_head_len;
    }

    if (buf_len > 0)
    {
        for (i = 0; i < buf_len; i++)
        {
            crc += buf[i];
        }
        memcpy(p_tx_buf + off, buf, buf_len);
        off += buf_len;
    }

    *(p_tx_buf + off) = crc;
    off += 1;

    mdm_uart->send(mdm_uart->tpt_data, off);
}

/***********************************************************
 *函数功能: 模块版本升级开始
 *返 回 值: 无
 */
void modem_upgrade_start(void)
{
    g_modem_upgrade_state = 1;
}

/***********************************************************
 *函数功能: 模块版本升级结束
 *返 回 值: 无
 */
void modem_upgrade_end(void)
{
    g_modem_upgrade_state = 0;
    hModem.modem_reboot_or_reset = 2; // 模块重新上电标志
    hModem.flg_get_ver = 1;
    hModem.modem_stat = MODEM_RESET;//modem_init();
}
