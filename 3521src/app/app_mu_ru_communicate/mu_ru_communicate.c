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

        1. 主从组网处理
        2. 接收从机数据,处理MCP协议数据
        3. 查询从机型号,对时处理,轮询设备状态
        4. 提供封装好的通信接口
*******************************************************************************/
#include "main.h"
#include "app_monitor.h"
#include "monitor_para_list.h"
#include "mu_ru_communicate.h"

#include "rtt.h"
/*---------------------- 端口定义 --------------------------------------*/

/*---------------------- 宏定义  ---------------------------------------*/

#define MRC_QUARY_RU_PERIOD_MS (2500)
#define MRC_UPDATA_BLE_LINK_MS (3 * 1000)
#define MRC_RESEND_PERIOD_MS (2 * 1000)
#define MRC_RESEND_MAX_COUNT (10)
#define MRC_SYNC_MAX_COUNT (30)
#define MRC_RELINK_TIMEOUT_MS (1 * 60 * 1000)
#define MRC_CALC_ROUTER_ADDR_TIME (10 * 1000)
/*---------------------- 结构体 ----------------------------------------*/

/*---------------------- 常量 ------------------------------------------*/

/*---------------------- 全局变量 --------------------------------------*/

mrc_mu_para_t mrc_mu_para; // 主从通信模块,主机参数列表

/*---------------------- 局部变量 --------------------------------------*/

T_BleData ble_rx_data_buf;
T_BleData ble_tx_data_buf;

/*---------------------- 声明 ------------------------------------------*/

/*---------------------- 函数 ------------------------------------------*/

/*****************************************************************************
 * @Name	  : mrc_init
 * @Brief  : 初始化主从通信模块
 * @Retval : None
 * @Note   :
 */
void mrc_init(void)
{
    // 初始化从机信息
#if (LOCAL_MODULE_TYPE == _MU)
    ru_device_list_init();
    memset(&mrc_mu_para, 0, sizeof(mrc_mu_para_t));
    mrc_mu_para.hub_led1 = 1;
    mrc_mu_para.hub_led2 = 1;
    mrc_mu_para.hub_led3 = 1;
    mrc_mu_para.hub_led4 = 1;
    mrc_mu_para.hub_led5 = 1;
    mrc_mu_para.hub_led6 = 1;
    mrc_mu_para.hub_led7 = 1;
    mrc_mu_para.hub_led8 = 1;
    mrc_set_system_power_ctrl(B_FALSE,0,0,0,0); //初始化系统功控关闭
#elif (LOCAL_MODULE_TYPE == _RU)
#endif
}

/*****************************************************************************
* @Name	  : app_mrc_get_ru_param
* @Brief  : 查询单个从机参数
* @params :
            ble_conn_id: 主从通信的从机ID
            param_id: 参数ID
            strlen: 参数长度
            timeout: 超时时间单位ms, 输入0表示不等待,发送后立刻返回,并且不返回参数值
* @Note   : 当返回成功后,在mrc_ru_data取对应数据.
*           仅支持查询MCP_MRC协议的参数.
*/
Status_t get_slave_dev_info(uint8_t ble_conn_id, uint32_t timeout)
{
    uint16_t pkt_id = 0;
    Status_t ret = STAT_OK;
    mcp_send_pkt_t tx_pkt;

    memset(&tx_pkt, 0, sizeof(mcp_send_pkt_t));
    new_mrc_send_pkt(&tx_pkt, ble_conn_id, MCP_A, MCP_CMD_TYPE_QUERY);
    add_mrc_pkt_data(&tx_pkt, MCP_ID_MANUFACTURER_CODE, NULL, 20);
    add_mrc_pkt_data(&tx_pkt, MCP_ID_DEV_TYPE, NULL, 0);
    add_mrc_pkt_data(&tx_pkt, MCP_ID_DEVICE_MODEL, NULL, 20);
    add_mrc_pkt_data(&tx_pkt, MCP_ID_SN, NULL, 30);
    add_mrc_pkt_data(&tx_pkt, MCP_ID_SW_VERSION, NULL, 20);
    mrc_add_tail_and_escape(&tx_pkt);

    pkt_id = tx_pkt.pkt_id;

    // 发送数据
    ret = mrc_send_pkt_to_ru(&tx_pkt, ble_conn_id);
    // 发送失败,返回错误
    if (ret == STAT_ERROR)
        return STAT_ERROR;
    else if (timeout == 0) // 发送成功,超时时间为0,立刻返回
        return STAT_OK;
    
    // 发送成功,等待回复
    if (mrc_wait_response(ble_conn_id, pkt_id, timeout) == STAT_OK)
        return STAT_OK;
    else
        return STAT_ERROR;
}

/*****************************************************************************
* @Name	  : get_slave_dev_public_params
* @Brief  : 查询单个从机参数
* @params :
            ble_conn_id: 主从通信的从机ID
            param_id: 参数ID
            strlen: 参数长度
            timeout: 超时时间单位ms, 输入0表示不等待,发送后立刻返回,并且不返回参数值
* @Note   : 当返回成功后,在mrc_ru_data取对应数据.
*           仅支持查询MCP_MRC协议的参数.
*/
Status_t get_slave_dev_public_params(uint8_t ble_conn_id, uint32_t timeout)
{
    uint16_t pkt_id = 0;
    Status_t ret = STAT_OK;
    mcp_send_pkt_t tx_pkt;

    memset(&tx_pkt, 0, sizeof(mcp_send_pkt_t));
    new_mrc_send_pkt(&tx_pkt, ble_conn_id, MCP_MRC, MCP_CMD_TYPE_QUERY);
    add_mrc_pkt_data(&tx_pkt, MRC_DEV_TYPE, NULL, 0);
    add_mrc_pkt_data(&tx_pkt, MRC_MCU_FW_VERSION, NULL, 0);
    add_mrc_pkt_data(&tx_pkt, MRC_BLE_FW_VERSION, NULL, 0);
    add_mrc_pkt_data(&tx_pkt, MRC_FPGA_FW_VERSION, NULL, 0);
    add_mrc_pkt_data(&tx_pkt, MRC_8668_FW1_VERSION, NULL, 0);
    add_mrc_pkt_data(&tx_pkt, MRC_8668_FW2_VERSION, NULL, 0);
    add_mrc_pkt_data(&tx_pkt, MRC_8668_FW3_VERSION, NULL, 0);
    add_mrc_pkt_data(&tx_pkt, MRC_8668_FW4_VERSION, NULL, 0);
    add_mrc_pkt_data(&tx_pkt, MRC_8668_FW5_VERSION, NULL, 0);
    mrc_add_tail_and_escape(&tx_pkt);

    pkt_id = tx_pkt.pkt_id;

    // 发送数据
    ret = mrc_send_pkt_to_ru(&tx_pkt, ble_conn_id);
    // 发送失败,返回错误
    if (ret == STAT_ERROR)
        return STAT_ERROR;
    else if (timeout == 0) // 发送成功,超时时间为0,立刻返回
        return STAT_OK;

    // 发送成功,等待回复
    if (mrc_wait_response(ble_conn_id, pkt_id, timeout) == STAT_OK)
        return STAT_OK;
    else
        return STAT_ERROR;
}

/*****************************************************************************
 * @Name	  : mrc_query_ru_params_periodically
 * @Brief  : 周期性查询从机和hub参数
 * @params :
 * @Note   :
 */
void mrc_query_ru_params_periodically(void)
{
    uint8_t i = 0;
    uint8_t ble_conn_id = 0;
    mcp_send_pkt_t tx_pkt;
    static uint32_t last_tick;
    static uint32_t send_cnt;
    
    if (hi_get_tick() - last_tick >= MRC_QUARY_RU_PERIOD_MS)
    {
        last_tick = hi_get_tick();
        send_cnt++;
    }
    else
        return;

    // 交替执行查询从机和设置从机
    if(send_cnt % 2 == 0)
    {
        for (i = 0; i < BLE_CONNECT_MAX_NUM; i++)
        {
            if (ru_device_list[i].dev_type == DEV_TYPE_RU && ru_device_list[i].ru_conn_state == RU_CONN_STATE_CONFIGURED)
            {
                ble_conn_id = ru_device_list[i].ble_conn_id;
                memset(&tx_pkt, 0, sizeof(mcp_send_pkt_t));
                new_mrc_send_pkt(&tx_pkt, ble_conn_id, MCP_MRC, MCP_CMD_TYPE_QUERY);
                add_mrc_pkt_data(&tx_pkt, MRC_RUN_TIME, NULL, 0);
                add_mrc_pkt_data(&tx_pkt, MRC_LTE_IN_PWR, NULL, 0);
                add_mrc_pkt_data(&tx_pkt, MRC_NR_IN_PWR, NULL, 0);
                add_mrc_pkt_data(&tx_pkt, MRC_NR2_IN_PWR, NULL, 0);
                add_mrc_pkt_data(&tx_pkt, MRC_SELF_EXCITED, NULL, 0);
                mrc_add_tail_and_escape(&tx_pkt);
                mrc_send_pkt_to_ru(&tx_pkt, ble_conn_id);
                vTaskDelay(20);
            }
        }

        // 查询hub
        for (i = 0; i < BLE_CONNECT_MAX_NUM; i++)
        {
            if (ru_device_list[i].dev_type == DEV_TYPE_HUB && ru_device_list[i].ru_conn_state == RU_CONN_STATE_CONFIGURED)
            {
                ble_conn_id = ru_device_list[i].ble_conn_id;
                memset(&tx_pkt, 0, sizeof(mcp_send_pkt_t));
                new_mrc_send_pkt(&tx_pkt, ble_conn_id, MCP_MRC, MCP_CMD_TYPE_QUERY);
                add_mrc_pkt_data(&tx_pkt, MRC_RUN_TIME, NULL, 0);
                add_mrc_pkt_data(&tx_pkt, MRC_HUB_CH1_OPEN_TIME, NULL, 0);
                add_mrc_pkt_data(&tx_pkt, MRC_HUB_CH2_OPEN_TIME, NULL, 0);
                add_mrc_pkt_data(&tx_pkt, MRC_HUB_CH3_OPEN_TIME, NULL, 0);
                add_mrc_pkt_data(&tx_pkt, MRC_HUB_CH4_OPEN_TIME, NULL, 0);
                add_mrc_pkt_data(&tx_pkt, MRC_HUB_CH5_OPEN_TIME, NULL, 0);
                add_mrc_pkt_data(&tx_pkt, MRC_HUB_CH6_OPEN_TIME, NULL, 0);
                add_mrc_pkt_data(&tx_pkt, MRC_HUB_CH7_OPEN_TIME, NULL, 0);
                add_mrc_pkt_data(&tx_pkt, MRC_HUB_CH8_OPEN_TIME, NULL, 0);
                add_mrc_pkt_data(&tx_pkt, MRC_HUB_CH1_OPEN, NULL, 0);
                add_mrc_pkt_data(&tx_pkt, MRC_HUB_CH2_OPEN, NULL, 0);
                add_mrc_pkt_data(&tx_pkt, MRC_HUB_CH3_OPEN, NULL, 0);
                add_mrc_pkt_data(&tx_pkt, MRC_HUB_CH4_OPEN, NULL, 0);
                add_mrc_pkt_data(&tx_pkt, MRC_HUB_CH5_OPEN, NULL, 0);
                add_mrc_pkt_data(&tx_pkt, MRC_HUB_CH6_OPEN, NULL, 0);
                add_mrc_pkt_data(&tx_pkt, MRC_HUB_CH7_OPEN, NULL, 0);
                add_mrc_pkt_data(&tx_pkt, MRC_HUB_CH8_OPEN, NULL, 0);
                mrc_add_tail_and_escape(&tx_pkt);
                mrc_send_pkt_to_ru(&tx_pkt, ble_conn_id);
                vTaskDelay(20);
            }
        }
    }
    else if(send_cnt % 2 == 1)// 交替执行查询从机和设置从机
    {
        // 设置从机
        for (i = 0; i < BLE_CONNECT_MAX_NUM; i++)
        {
            if (ru_device_list[i].dev_type == DEV_TYPE_RU && ru_device_list[i].ru_conn_state == RU_CONN_STATE_CONFIGURED)
            {
                ble_conn_id = ru_device_list[i].ble_conn_id;
                memset(&tx_pkt, 0, sizeof(mcp_send_pkt_t));
                new_mrc_send_pkt(&tx_pkt, ble_conn_id, MCP_MRC, MCP_CMD_TYPE_SET);
                add_mrc_pkt_data(&tx_pkt, MRC_SYSPC_SWITCH, (uint8_t *)&mrc_mu_para.pc_switch, 0);
                add_mrc_pkt_data(&tx_pkt, MRC_SYSPC_LTE_UL_IN_EXPECT, (uint8_t *)&mrc_mu_para.lte_ul_pwr_expect, 0);
                add_mrc_pkt_data(&tx_pkt, MRC_SYSPC_LTE_DL_OUT, (uint8_t *)&mrc_mu_para.lte_tx_pwr, 0);
                add_mrc_pkt_data(&tx_pkt, MRC_SYSPC_NR_UL_IN_EXPECT, (uint8_t *)&mrc_mu_para.nr_ul_pwr_expect, 0);
                add_mrc_pkt_data(&tx_pkt, MRC_SYSPC_NR_DL_OUT, (uint8_t *)&mrc_mu_para.nr_tx_pwr, 0);
                mrc_add_tail_and_escape(&tx_pkt);
                mrc_send_pkt_to_ru(&tx_pkt, ble_conn_id);
                vTaskDelay(20);
            }
        }
        // 设置hub
        if(send_cnt >= 24) //开机一段时间内不做配置,等待从机通道打开且均建立连接
        {
            for (i = 0; i < BLE_CONNECT_MAX_NUM; i++)
            {
                if (ru_device_list[i].dev_type == DEV_TYPE_HUB && ru_device_list[i].ru_conn_state == RU_CONN_STATE_CONFIGURED)
                {
                    ble_conn_id = ru_device_list[i].ble_conn_id;
                    memset(&tx_pkt, 0, sizeof(mcp_send_pkt_t));
                    new_mrc_send_pkt(&tx_pkt, ble_conn_id, MCP_MRC, MCP_CMD_TYPE_SET);
                    add_mrc_pkt_data(&tx_pkt, MRC_HUB_LED1_OPEN, &mrc_mu_para.hub_led1, 0);
                    add_mrc_pkt_data(&tx_pkt, MRC_HUB_LED2_OPEN, &mrc_mu_para.hub_led2, 0);
                    add_mrc_pkt_data(&tx_pkt, MRC_HUB_LED3_OPEN, &mrc_mu_para.hub_led3, 0);
                    add_mrc_pkt_data(&tx_pkt, MRC_HUB_LED4_OPEN, &mrc_mu_para.hub_led4, 0);
                    add_mrc_pkt_data(&tx_pkt, MRC_HUB_LED5_OPEN, &mrc_mu_para.hub_led5, 0);
                    add_mrc_pkt_data(&tx_pkt, MRC_HUB_LED6_OPEN, &mrc_mu_para.hub_led6, 0);
                    add_mrc_pkt_data(&tx_pkt, MRC_HUB_LED7_OPEN, &mrc_mu_para.hub_led7, 0);
                    add_mrc_pkt_data(&tx_pkt, MRC_HUB_LED8_OPEN, &mrc_mu_para.hub_led8, 0);
                    mrc_add_tail_and_escape(&tx_pkt);
                    mrc_send_pkt_to_ru(&tx_pkt, ble_conn_id);
                    vTaskDelay(20);
                }
            }
        }
    }
}

// 周期查询ble连接状态和rssi(从机)
void update_ble_status(void)
{
    static uint32_t last_tick;

    if (hi_get_tick() - last_tick >= MRC_UPDATA_BLE_LINK_MS)
    {
        last_tick = hi_get_tick();
    }
    else
        return;

    // 查询连接
    ble_cmd(CMD_QUERY_BLE_QLINK);
}
/*****************************************************************************
 * @Name	  : mrc_recv_msg_in_task
 * @Brief  : 接收蓝牙数据,处理MCP协议数据,发送蓝牙数据回包
 * @Retval : None
 * @Note   :
 */
void mrc_recv_msg_in_task(void)
{
    uint16_t msg_len = 0, tx_len = 0;

    // 接收蓝牙数据
    memset(&ble_rx_data_buf, 0, sizeof(T_BleData));
    msg_len = ble_msg_receive(&ble_rx_data_buf);
    if (msg_len == 0)
    {
        return;
    }

    // 处理蓝牙数据 
    memset(&ble_tx_data_buf,0,sizeof(T_BleData));
    tx_len = MCP_rx_pkt_analysis(ble_rx_data_buf.data, ble_rx_data_buf.len, ble_tx_data_buf.data, MCP_BLE);

    // 发送蓝牙数据回包
    if (tx_len)
    {
        ble_tx_data_buf.len = tx_len;
        memcpy(ble_tx_data_buf.ble_mac, ble_rx_data_buf.ble_mac, 6);
        ble_msg_send_by_mac(ble_tx_data_buf.data, ble_tx_data_buf.len, ble_tx_data_buf.ble_mac);
    }
}
// 给新接入的从机生成一个新的监控从站编号,以流水号的方式生成,从站编号范围1-255,其中230以后为hub
uint8_t app_mrc_get_new_ru_id(uint8_t dev_type)
{
    uint8_t ret = 0;
    static uint8_t current_ru_id = 1;
    static uint8_t current_hub_id = 231;

    if (dev_type == DEV_TYPE_RU)
    {
        ret = current_ru_id;
        current_ru_id += 1;
    }
    else if (dev_type == DEV_TYPE_HUB)
    {
        ret = current_hub_id;
        current_hub_id += 1;
    }

    return ret;
}

void mrc_connection_handler(void)
{
    uint8_t i = 0;
    uint8_t ble_conn_id = 0;
    uint32_t mu_tick = 0;
    // 连接处理
    for (i = 0; i < BLE_CONNECT_MAX_NUM; i++)
    {
        ble_conn_id = ru_device_list[i].ble_conn_id;

        if (ru_device_list[i].pt_slave_conn->conn_event == B_TRUE)
        {
            // 处理连接事件
            ru_device_list[i].pt_slave_conn->conn_event = B_FALSE;
            ru_device_list[i].ru_conn_state = RU_CONN_STATE_CONNECTED;
        }
        else if (ru_device_list[i].pt_slave_conn->disconn_event == B_TRUE)
        {
            // 处理断开事件
            ru_device_list[i].pt_slave_conn->disconn_event = B_FALSE;
            ru_device_list[i].ru_conn_state = RU_CONN_STATE_UNCONNECTED;
        }

        switch (ru_device_list[i].ru_conn_state)
        {
        case RU_CONN_STATE_CONNECTED:
            // 连接成功,查询设备信息
            if (get_slave_dev_public_params(ble_conn_id, 4000) == STAT_OK)
            {
                if (mrc_ru_data[i].device_type == DEV_TYPE_RU)
                {
                    ru_device_list[i].dev_type = DEV_TYPE_RU;
                    if (ru_device_list[i].ru_dev_id == 0)
                    {
                        ru_device_list[i].ru_dev_id = app_mrc_get_new_ru_id(DEV_TYPE_RU);
                        rtt_printf(RTT_BLE, RTT_CTRL_TEXT_BRIGHT_YELLOW ">RU: ru connected, id: %d\r\n", ru_device_list[i].ru_dev_id);
                    }
                    ru_device_list[i].ru_conn_state = RU_CONN_STATE_GET_DEV_PARAMS;
                }
                else if (mrc_ru_data[i].device_type == DEV_TYPE_HUB)
                {
                    ru_device_list[i].dev_type = DEV_TYPE_HUB;
                    if (ru_device_list[i].ru_dev_id == 0)
                    {
                        ru_device_list[i].ru_dev_id = app_mrc_get_new_ru_id(DEV_TYPE_HUB);
                        rtt_printf(RTT_BLE, RTT_CTRL_TEXT_BRIGHT_YELLOW ">RU: hub connected, id: %d\r\n", ru_device_list[i].ru_dev_id);
                    }
                    ru_device_list[i].ru_conn_state = RU_CONN_STATE_GET_DEV_PARAMS;
                }
                // TODO  类型未识别,如何处理
            }
            else
            {
                ru_device_list[i].send_err_count += 1;
                if (ru_device_list[i].send_err_count >= MRC_RESEND_MAX_COUNT)
                {
                    ru_device_list[i].send_err_count = 0;
                    ru_device_list[i].relink_tick = hi_get_tick() + MRC_RELINK_TIMEOUT_MS;
                    ru_device_list[i].ru_conn_state = RU_CONN_STATE_ERR;
                    rtt_printf(RTT_BLE, RTT_CTRL_TEXT_BRIGHT_RED ">RU: ru/hub connected, but get device info failed\r\n");
                }
            }
            break;

        case RU_CONN_STATE_GET_DEV_PARAMS:
            if (get_slave_dev_info(ble_conn_id, 4000) == STAT_OK)
            {
                ru_device_list[i].ru_conn_state = RU_CONN_STATE_GET_DEV_INFO;
            }
            else
            {
                ru_device_list[i].send_err_count += 1;
                if (ru_device_list[i].send_err_count >= MRC_RESEND_MAX_COUNT)
                {
                    ru_device_list[i].send_err_count = 0;
                    ru_device_list[i].relink_tick = hi_get_tick() + MRC_RELINK_TIMEOUT_MS;
                    ru_device_list[i].ru_conn_state = RU_CONN_STATE_ERR;
                    rtt_printf(RTT_BLE, RTT_CTRL_TEXT_BRIGHT_RED ">RU: get device info failed, id: %d\r\n", ru_device_list[i].ru_dev_id);
                }
            }
            break;

        case RU_CONN_STATE_GET_DEV_INFO:
            // 获取设备信息成功,对时
            mu_tick = hi_get_tick();
            if (mrc_set_ru_param(ble_conn_id, MRC_TIME_SYNC, &mu_tick, 0, 1500) == STAT_OK)
            {
                ru_device_list[i].ru_conn_state = RU_CONN_STATE_SYNCED;
                rtt_printf(RTT_BLE, ">RU: ru/hub synced, id: %d\r\n", ru_device_list[i].ru_dev_id);
            }
            else
            {
                ru_device_list[i].send_err_count += 1;
                if (ru_device_list[i].send_err_count >= MRC_RESEND_MAX_COUNT)
                {
                    ru_device_list[i].send_err_count = 0;
                    ru_device_list[i].relink_tick = hi_get_tick() + MRC_RELINK_TIMEOUT_MS;
                    ru_device_list[i].ru_conn_state = RU_CONN_STATE_ERR;
                    rtt_printf(RTT_BLE, RTT_CTRL_TEXT_BRIGHT_RED ">RU: ru/hub sync failed, id: %d\r\n", ru_device_list[i].ru_dev_id);
                }
            }
            break;

        case RU_CONN_STATE_SYNCED:
            // 对时成功,更新设备连接状态
            ru_device_list[i].send_err_count = 0;
            ru_device_list[i].pt_slave_conn->is_dev_configred = B_TRUE;
            ru_device_list[i].ru_conn_state = RU_CONN_STATE_CONFIGURED;
            hBle.ble_stat.slave_pair_num += 1;
            rtt_printf(RTT_BLE, RTT_CTRL_TEXT_BRIGHT_YELLOW ">RU: ru/hub sync configured done, id: %d\r\n", ru_device_list[i].ru_dev_id);
            break;

        case RU_CONN_STATE_UNCONNECTED:
            // 未连接
            break;
        case RU_CONN_STATE_ERR:
            // 错误状态, 一段时间后重连
            if(ru_device_list[i].relink_tick <= hi_get_tick() )
                ru_device_list[i].ru_conn_state = RU_CONN_STATE_CONNECTED;
            break;
        case RU_CONN_STATE_CONFIGURED:
            // 设备已配对,工作状态

            break;

        default:
            break;
        }
    }
}

void ble_conn_print_info(void)
{
    uint8_t conn_stat = 0xFF;
    int8_t rssi;
    uint8_t dev_cnt = 0;
    static uint32_t last_print_tick = 0;

    if (hi_get_tick() - last_print_tick >= 2000)
        last_print_tick = hi_get_tick();
    else
        return;

    int i = 0;
#if (LOCAL_MODULE_TYPE == _MU)
    for (i = 0; i < BLE_CONNECT_MAX_NUM; i++)
    {
        conn_stat = hBle.slave_conn[i].is_dev_connected;
        if (conn_stat == B_TRUE)
        {
            rtt_printf(RTT_BLE, RTT_CTRL_TEXT_BRIGHT_GREEN "| %2d | %02x%02x%02x%02x%02x%02x | %2d | %2d| %08x|", i,
                       hBle.slave_conn[i].mac_addr[0], hBle.slave_conn[i].mac_addr[1], hBle.slave_conn[i].mac_addr[2],
                       hBle.slave_conn[i].mac_addr[3], hBle.slave_conn[i].mac_addr[4], hBle.slave_conn[i].mac_addr[5],
                       conn_stat, ru_device_list[i].ru_conn_state, ru_device_list[i].router_addr);
            if (ru_device_list[i].dev_type == DEV_TYPE_RU)
                rtt_printf(RTT_BLE, RTT_CTRL_TEXT_BRIGHT_GREEN "  RU |\r\n");
            else if (ru_device_list[i].dev_type == DEV_TYPE_HUB)
                rtt_printf(RTT_BLE, RTT_CTRL_TEXT_BRIGHT_GREEN "  HUB|\r\n");
            else
                rtt_printf(RTT_BLE, RTT_CTRL_TEXT_BRIGHT_GREEN "  UNK|\r\n");
            dev_cnt++;
        }
    }
#endif
    conn_stat = hBle.master_conn.is_dev_connected;
    if (conn_stat == B_TRUE)
    {
        rtt_printf(RTT_BLE, RTT_CTRL_TEXT_BRIGHT_GREEN "| %2d | %02x%02x%02x%02x%02x%02x | %2d | %3d|", i,
                   hBle.master_conn.mac_addr[0], hBle.master_conn.mac_addr[1], hBle.master_conn.mac_addr[2],
                   hBle.master_conn.mac_addr[3], hBle.master_conn.mac_addr[4], hBle.master_conn.mac_addr[5],
                   conn_stat, rssi);
        rtt_printf(RTT_BLE, RTT_CTRL_TEXT_BRIGHT_GREEN "  MU|\r\n");
        dev_cnt++;
    }

    conn_stat = hBle.app_conn.is_dev_connected;
    if (conn_stat == B_TRUE)
    {
        rtt_printf(RTT_BLE, RTT_CTRL_TEXT_BRIGHT_GREEN "| %2d | %02x%02x%02x%02x%02x%02x | %2d | %2d|", i,
                   hBle.app_conn.mac_addr[0], hBle.app_conn.mac_addr[1], hBle.app_conn.mac_addr[2],
                   hBle.app_conn.mac_addr[3], hBle.app_conn.mac_addr[4], hBle.app_conn.mac_addr[5],
                   conn_stat, hBle.app_conn.is_dev_configred);
        rtt_printf(RTT_BLE, RTT_CTRL_TEXT_BRIGHT_GREEN " APP|\r\n");
        dev_cnt++;
    }

    if (dev_cnt == 0)
        rtt_printf(RTT_BLE, RTT_CTRL_TEXT_BRIGHT_YELLOW "NOT CONNECTED!\r\n");
    rtt_printf(RTT_BLE, RTT_CTRL_TEXT_BRIGHT_GREEN "\r\n\r\n");
}

void mrc_cal_router_addr(void)
{
    uint8_t i = 0;
    uint32_t last_tick = 0;
    int diff = 0;
    pt_ru_device_t pt_hub = NULL;

    // 周期计算路由地址
    if(hi_get_tick() - last_tick < MRC_CALC_ROUTER_ADDR_TIME)
        return;
    else
        last_tick = hi_get_tick();

    pt_hub = get_hub_device_ptr();

    // 路由号计算
    for(i = 0; i < BLE_CONNECT_MAX_NUM; i++)
    {
        if(ru_device_list[i].dev_type == DEV_TYPE_HUB && ru_device_list[i].pt_slave_conn->is_dev_configred == B_TRUE)
        {
            ru_device_list[i].router_addr = 0x10000000;
        }
        //是从机,且未分配路由号
        else if(ru_device_list[i].dev_type == DEV_TYPE_RU && (ru_device_list[i].router_addr & 0x0FFFFFFF) == 0 && ru_device_list[i].pt_slave_conn->is_dev_configred == B_TRUE)
        {
            if(pt_hub == NULL)
            {
                ru_device_list[i].router_addr = ((uint32_t)ru_device_list[i].ru_dev_id << 28) & 0xF0000000;
            }else
            {
                if(ru_device_list[i].pt_mrc_ru_data->mcu_run_time != 0 && pt_hub->pt_mrc_ru_data->hub_ch1_open_time != 0)
                {
                    diff = pt_hub->pt_mrc_ru_data->hub_ch1_open_time - ru_device_list[i].pt_mrc_ru_data->mcu_run_time;
                    if(diff >= -2000 && diff <= 2000)
                    {
                        ru_device_list[i].router_addr = 0x12000000;
                        mrc_mu_para.hub_led1 = 2;
                    }
                }
                if(ru_device_list[i].pt_mrc_ru_data->mcu_run_time != 0 && pt_hub->pt_mrc_ru_data->hub_ch2_open_time != 0)
                {
                    diff = pt_hub->pt_mrc_ru_data->hub_ch2_open_time - ru_device_list[i].pt_mrc_ru_data->mcu_run_time;
                    if(diff >= -2000 && diff <= 2000)
                    {
                        ru_device_list[i].router_addr = 0x13000000;
                        mrc_mu_para.hub_led2 = 2;
                    }
                }
                if(ru_device_list[i].pt_mrc_ru_data->mcu_run_time != 0 && pt_hub->pt_mrc_ru_data->hub_ch3_open_time != 0)
                {
                    diff = pt_hub->pt_mrc_ru_data->hub_ch3_open_time - ru_device_list[i].pt_mrc_ru_data->mcu_run_time;
                    if(diff >= -2000 && diff <= 2000)
                    {
                        ru_device_list[i].router_addr = 0x14000000;
                        mrc_mu_para.hub_led3 = 2;
                    }
                }
                if(ru_device_list[i].pt_mrc_ru_data->mcu_run_time != 0 && pt_hub->pt_mrc_ru_data->hub_ch4_open_time != 0)
                {
                    diff = pt_hub->pt_mrc_ru_data->hub_ch4_open_time - ru_device_list[i].pt_mrc_ru_data->mcu_run_time;
                    if(diff >= -2000 && diff <= 2000)
                    {
                        ru_device_list[i].router_addr = 0x20000000;
                        mrc_mu_para.hub_led4 = 2;
                    }
                }
                if(ru_device_list[i].pt_mrc_ru_data->mcu_run_time != 0 && pt_hub->pt_mrc_ru_data->hub_ch5_open_time != 0)
                {
                    diff = pt_hub->pt_mrc_ru_data->hub_ch5_open_time - ru_device_list[i].pt_mrc_ru_data->mcu_run_time;
                    if(diff >= -2000 && diff <= 2000)
                    {
                        ru_device_list[i].router_addr = 0x16000000;
                        mrc_mu_para.hub_led5 = 2;
                    }
                }
                if(ru_device_list[i].pt_mrc_ru_data->mcu_run_time != 0 && pt_hub->pt_mrc_ru_data->hub_ch6_open_time != 0)
                {
                    diff = pt_hub->pt_mrc_ru_data->hub_ch6_open_time - ru_device_list[i].pt_mrc_ru_data->mcu_run_time;
                    if(diff >= -2000 && diff <= 2000)
                    {
                        ru_device_list[i].router_addr = 0x17000000;
                        mrc_mu_para.hub_led6 = 2;
                    }
                }
                if(ru_device_list[i].pt_mrc_ru_data->mcu_run_time != 0 && pt_hub->pt_mrc_ru_data->hub_ch7_open_time != 0)
                {
                    diff = pt_hub->pt_mrc_ru_data->hub_ch7_open_time - ru_device_list[i].pt_mrc_ru_data->mcu_run_time;
                    if(diff >= -2000 && diff <= 2000)
                    {
                        ru_device_list[i].router_addr = 0x18000000;
                        mrc_mu_para.hub_led7 = 2;
                    }
                }
                if(ru_device_list[i].pt_mrc_ru_data->mcu_run_time != 0 && pt_hub->pt_mrc_ru_data->hub_ch8_open_time != 0)
                {
                    diff = pt_hub->pt_mrc_ru_data->hub_ch8_open_time - ru_device_list[i].pt_mrc_ru_data->mcu_run_time;
                    if(diff >= -2000 && diff <= 2000)
                    {
                        ru_device_list[i].router_addr = 0x19000000;
                        mrc_mu_para.hub_led8 = 2;
                    }
                }
            }
        }
    }
}

/*****************************************************************************
 * @Name	  : mrc_reset_ru
 * @Param  : conn_id:蓝牙连接ID, 0xFF:重置所有连接的RU
 * @Retval : None
 * @Note   :
 */
void mrc_reset_ru(uint8_t ble_conn_id)
{
    mcp_send_pkt_t tx_pkt;
    uint8_t reset_val = 1;

    memset(&tx_pkt, 0, sizeof(mcp_send_pkt_t));
    new_mrc_send_pkt(&tx_pkt, ble_conn_id, MCP_A, MCP_CMD_TYPE_SET);
    add_mrc_pkt_data(&tx_pkt, MCP_ID_DEV_RESET, &reset_val, 0);
    mrc_add_tail_and_escape(&tx_pkt);

    //发送数据
    mrc_send_pkt_to_ru(&tx_pkt, ble_conn_id);
}

void mrc_reset_all_ru(void)
{
    for(uint8_t i = 0; i < BLE_CONNECT_MAX_NUM; i++)
    {
        if(ru_device_list[i].pt_slave_conn->is_dev_connected == B_TRUE)
        {
            mrc_reset_ru(i);
        }
    }
    return;
}

/*****************************************************************************
 * @Name	  : mrc_set_system_power_ctrl(B_BOOL pc_switch, int8_t lte_ul_pwr_expect, int8_t lte_tx_pwr, int8_t nr_ul_pwr_expect, int8_t nr_tx_pwr)
 * @Param  : pc_switch: 系统功控开关 0:关 1:开
 * @Param  : lte_ul_pwr_expect:LTE上行功率预期值
 * @Param  : lte_tx_pwr:LTE下行功率
 * @Param  : nr_ul_pwr_expect:NR上行功率预期值
 * @Param  : nr_tx_pwr:NR下行功率
 * @Retval : None
 * @Note   : 设置系统功控参数,设置参数并启用开关后,蓝牙将系统功控参数周期性广播给所有从机
*/

void mrc_set_system_power_ctrl(BOOL_E pc_switch, int8_t lte_ul_pwr_expect, int8_t lte_tx_pwr, int8_t nr_ul_pwr_expect, int8_t nr_tx_pwr)
{
    mrc_mu_para.pc_switch = pc_switch;
    mrc_mu_para.lte_ul_pwr_expect = lte_ul_pwr_expect;
    mrc_mu_para.lte_tx_pwr = lte_tx_pwr;
    mrc_mu_para.nr_ul_pwr_expect = nr_ul_pwr_expect;
    mrc_mu_para.nr_tx_pwr = nr_tx_pwr;
}


/*****************************************************************************
* @Name	  : mrc_process
* @Brief  : 主从通信模块处理函数:
        1. 主从组网处理
        2. 查询从机型号,对时处理,轮询设备状态
        3.打印组网信息

* @Retval : None
* @Retval : None
* @Note   :
*/
void mrc_process(void)
{
    // 组网处理
    mrc_connection_handler();
    // 轮询
    mrc_query_ru_params_periodically();
    // 连接打印
    ble_conn_print_info();
    // 周期查询ble连接状态和rssi(从机)
    update_ble_status();
    // 路由地址计算
    mrc_cal_router_addr();

}
