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

        从机设备列表,包括从机设备的参数,连接状态,蓝牙连接状态,设备类型等等所需的从机信息

*******************************************************************************/
#include "main.h"
#include "bsp_config.h"
#include "ru_device_list.h"

/*---------------------- 端口定义 --------------------------------------*/

/*---------------------- 宏定义  ---------------------------------------*/

/*---------------------- 结构体 ----------------------------------------*/

ru_device_t ru_device_list[BLE_CONNECT_MAX_NUM];

/*---------------------- 常量 ------------------------------------------*/

/*---------------------- 全局变量 --------------------------------------*/

/*---------------------- 局部变量 --------------------------------------*/
/*
 * @Name	  : ru_device_list_init
 * @Brief  : 初始化主从通信从机列表
 * @Retval : None
 * @Note   :
 */
void ru_device_list_init(void)
{
    int i = 0;
    // 初始化从机信息
    for (i = 0; i < BLE_CONNECT_MAX_NUM; i++)
    {
        memset(&ru_device_list[i], 0, sizeof(ru_device_t));
        ru_device_list[i].ru_conn_state = RU_CONN_STATE_UNCONNECTED;
        ru_device_list[i].dev_type = DEV_TYPE_UNKNOWN;
        ru_device_list[i].ble_conn_id = i + 1;
        ru_device_list[i].ru_dev_id = 0;
        ru_device_list[i].pt_slave_conn = &hBle.slave_conn[get_ble_index_by_slave_id(ru_device_list[i].ble_conn_id)];
        ru_device_list[i].pt_mrc_ru_data = &mrc_ru_data[i];
        ru_device_list[i].pt_warn_para = NULL;

        memset(ru_device_list[i].pt_mrc_ru_data, 0, sizeof(MRC_RU_DATA_T));
    }
}

/*****************************************************************************
 * @brief    : 根据从机蓝牙mac地址获取从机设备结构体指针
 * @param    : mac: 从机蓝牙mac地址
 * @return   : 成功: 指向从机设备结构体指针, 失败: NULL
 * @note     : 无
 */
pt_ru_device_t get_ru_device_ptr_by_ble_mac(uint8_t *mac)
{
    int i = 0;
    for (i = 0; i < BLE_CONNECT_MAX_NUM; i++)
    {
        if (memcmp(ru_device_list[i].pt_slave_conn->mac_addr, mac, 6) == 0)
        {
            return &ru_device_list[i];
        }
    }
    return NULL;
}

/*****************************************************************************
 * @brief    : 根据从机蓝牙id获取从机设备结构体指针
 * @param    : ble_conn_id: 从机蓝牙id
 * @return   : 成功: 指向从机设备结构体指针, 失败: NULL
 * @note     : 无
 */
pt_ru_device_t get_ru_device_ptr_by_slave_id(uint8_t ble_conn_id)
{
    int i = 0;
    for (i = 0; i < BLE_CONNECT_MAX_NUM; i++)
    {
        if (ru_device_list[i].ble_conn_id == ble_conn_id)
        {
            return &ru_device_list[i];
        }
    }
    return NULL;
}

/*****************************************************************************
 * @brief    : 根据从机监控设备地址获取从机设备结构体指针
 * @param    : ru_dev_id: 从机设备地址
 * @return   : 成功: 指向从机设备结构体指针, 失败: NULL
 * @note     : 无
 */
pt_ru_device_t get_ru_device_ptr_by_monitor_dev_id(uint8_t monitor_dev_id)
{
    int i = 0;
    for (i = 0; i < BLE_CONNECT_MAX_NUM; i++)
    {
        if (ru_device_list[i].ru_dev_id == monitor_dev_id)
        {
            return &ru_device_list[i];
        }
    }
    return NULL;
}

/*****************************************************************************
 * @brief    : 获取hub设备结构体指针
 * @return   : 成功: 指向从机设备结构体指针, 失败: NULL
 * @note     : 无
 */
pt_ru_device_t get_hub_device_ptr(void)
{
    int i = 0;
    for (i = 0; i < BLE_CONNECT_MAX_NUM; i++)
    {
        if (ru_device_list[i].dev_type == DEV_TYPE_HUB)
        {
            return &ru_device_list[i];
        }
    }
    return NULL;
}

/*****************************************************************************
 * @Name	  : get_ble_slave_index_by_dev_id
 * @Brief  : 根据从监控设备id获取蓝牙数组索引
 * @Param  : ru_dev_id: 从设备蓝牙ID
 * @Retval : 返回数据索引,0xff表示无效id
 * @Author :
 * @Data   : 2024-1-11
 * @Note   :
*/
uint8_t get_ble_slave_index_by_dev_id(uint8_t ru_dev_id)
{
    int i = 0;
    for (i = 0; i < BLE_CONNECT_MAX_NUM; i++)
    {
        if (ru_device_list[i].ru_dev_id == ru_dev_id)
        {
            return i;
        }
    }
    return 0xff;
}
