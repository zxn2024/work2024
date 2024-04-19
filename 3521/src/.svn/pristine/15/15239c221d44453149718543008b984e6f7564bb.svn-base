/*************************************** Copyright ******************************
 *
 * (C) Copyright 2023, Shaanxi Tianji Communication
 * All Rights Reserved
 *
 * FileName   : app_mrc.h
 * Version    : none
 * Author     :
 * Date       : 2024-3-5
 * Description: none
 *******************************************************************************/

#ifndef __RU_DEVICE_LIST_H__
#define __RU_DEVICE_LIST_H__

#include "bsp_config.h"
#include "mcp_core.h"
#include "mu_ru_comm_para_list.h"
#include "ble.h"
#include "mcpMRC_user.h"
#include "bsp_warn_user.h"
/*---------------------- 端口定义 --------------------------------------*/

/*---------------------- 宏定义  ---------------------------------------*/

#define DEV_TYPE_UNKNOWN (0xff)
#define DEV_TYPE_MU _MU
#define DEV_TYPE_RU _RU
#define DEV_TYPE_HUB _HUB

/*---------------------- 结构体 ----------------------------------------*/

typedef enum __ru_conn_state_t
{
    RU_CONN_STATE_UNCONNECTED,    // 蓝牙未连接
    RU_CONN_STATE_CONNECTED,      // 蓝牙连接
    RU_CONN_STATE_GET_DEV_INFO,   // 获取设备类型
    RU_CONN_STATE_GET_DEV_PARAMS, // 获取设备信息
    RU_CONN_STATE_SYNCED,         // 时间已同步
    RU_CONN_STATE_CONFIGURED,     // 主从配对完成状态(正常工作状态)
    RU_CONN_STATE_ERR             // 错误状态
} ru_conn_state_e;

typedef struct __ru_device_t
{
    ru_conn_state_e ru_conn_state; // 从机蓝牙连接状态
    uint8_t dev_type;              // 从机设备类型: DEV_TYPE_UNKNOWN DEV_TYPE_MU DEV_TYPE_RU DEV_TYPE_HUB
    uint8_t ble_conn_id;           // 从机蓝牙通信id 从01开始,固定数值等于本数组下标+1
    uint8_t ru_dev_id;             // 监控设备地址,HUB固定为230,RU设备为从1开头的数字编号
    uint32_t router_addr;          // 设备路由号
    slave_conn_t *pt_slave_conn;   // 指向从机的蓝牙连接信息
    MRC_RU_DATA_T *pt_mrc_ru_data; // 指向从机的主从通信数据结构
    dev_warn_para_t *pt_warn_para; // 指向从机的告警参数结构体
    uint8_t send_err_count;        // 发送错误计数
    uint32_t relink_tick;          // 重连计时
} ru_device_t, *pt_ru_device_t;    // 从机设备结构体,包含hub和ru设备

extern ru_device_t ru_device_list[BLE_CONNECT_MAX_NUM]; // 蓝牙从机设备列表

/*---------------------- 函数声明 ------------------------------------------*/
/*****************************************************************************
 * @brief    : 初始化从机设备列表
 * @param    : 无
 * @return   : 无
 * @note     : 无
 */
void ru_device_list_init(void);

/*****************************************************************************
 * @brief    : 根据从机蓝牙mac地址获取从机设备结构体指针
 * @param    : mac: 从机蓝牙mac地址
 * @return   : 成功: 指向从机设备结构体指针, 失败: NULL
 * @note     : 无
 */
pt_ru_device_t get_ru_device_ptr_by_ble_mac(uint8_t *mac);

/*****************************************************************************
 * @brief    : 根据从机蓝牙id获取从机设备结构体指针
 * @param    : ble_conn_id: 从机蓝牙id
 * @return   : 成功: 指向从机设备结构体指针, 失败: NULL
 * @note     : 无
 */
pt_ru_device_t get_ru_device_ptr_by_slave_id(uint8_t ble_conn_id);

/*****************************************************************************
 * @brief    : 根据从机监控设备地址获取从机设备结构体指针
 * @param    : ru_dev_id: 从机设备地址
 * @return   : 成功: 指向从机设备结构体指针, 失败: NULL
 * @note     : 无
 */
pt_ru_device_t get_ru_device_ptr_by_monitor_dev_id(uint8_t monitor_dev_id);

/*****************************************************************************
 * @Name	  : get_ble_slave_index_by_dev_id
 * @Brief  : 根据从监控设备id获取蓝牙数组索引
 * @Param  : ru_dev_id: 从设备蓝牙ID
 * @Retval : 返回数据索引,0xff表示无效id
 * @Author :
 * @Data   : 2024-1-11
 * @Note   :
*/
uint8_t get_ble_slave_index_by_dev_id(uint8_t ru_dev_id);

/*****************************************************************************
 * @brief    : 获取hub设备结构体指针
 * @return   : 成功: 指向从机设备结构体指针, 失败: NULL
 * @note     : 无
 */
pt_ru_device_t get_hub_device_ptr(void);
#endif /* __APP_MRC_H__ */
