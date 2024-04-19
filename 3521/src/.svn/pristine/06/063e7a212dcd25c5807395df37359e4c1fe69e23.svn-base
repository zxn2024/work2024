/*************************************** Copyright ******************************
 *
 * (C) Copyright 2023, Shaanxi Tianji Communication
 * All Rights Reserved
 *
 * FileName   : ble.h
 * Version    : none
 * Author     :
 * Date       : 2023-10-19
 * Description: none
 *******************************************************************************/
// #include <stdint.h>
#ifndef __BLE_H_
#define __BLE_H_

#include <stdint.h>
#include "bsp_config.h"

/*---------------------- 宏定义  ---------------------------------------*/
#if (LOCAL_MODULE_TYPE == _MU)
#define BLE_CONNECT_MAX_NUM BLE_SLAVE_DEV_MAX_NUM // 蓝牙支持最大连接数量 从机+HUB
#elif (LOCAL_MODULE_TYPE == _RU)
#define BLE_CONNECT_MAX_NUM 1
#endif

#define BLE_KEY_3521 (0x12345678)


#define BLE_TXRX_MAX_LEN 240 /**< BLE RX buffer size. */
#define BLE_TXRX_BUF_SIZE 256 /**< BLE RX buffer size. */
/**
 * @brief 命令参数序号列表
 */
typedef enum
{
    CMD_QUERY_BLE_QLINK = 1,         // 查询组网状态
    CMD_QUERY_BLE_MAC = 2,           // 查询MAC地址
    CMD_QUERY_BLE_TXPWR = 3,         // 查询蓝牙发射功率
    CMD_QUERY_BLE_MODE = 4,          // 查询蓝牙模式
    CMD_QUERY_BLE_LINKKEY = 5,       // 查询蓝牙匹配秘钥
    CMD_QUERY_BLE_AUTOCONN = 6,      // 查询自动连接状态
    CMD_QUERY_BLE_VERSION = 7,       // 查询蓝牙版本
    CMD_QUERY_BLE_RSSI = 8,          // 查询接收信号强度（从机模式）
    CMD_QUERY_BLE_APP_EN = 9,        // 查询连接手机APP使能
    CMD_SET_BLE_AUTOCONN_OPEN = 10,  // 开启自动连接
    CMD_SET_BLE_AUTOCONN_CLOSE = 11, // 关闭自动连接
    CMD_SET_BLE_RESET = 12,          // 重启
    CMD_SET_BLE_TXPWR = 13,          // 设置蓝牙发射功率
    CMD_SET_BLE_MODE = 14,           // 设置蓝牙模式
    CMD_SET_BLE_LINKKEY = 15,        // 设置蓝牙匹配秘钥
    CMD_SET_BLE_APP = 16,            // 设置APP模式
    CMD_SET_BLE_END
} CmdBleTypeList_E;

typedef enum
{
    MSG_DATA = 0, // 自组网透传数据
    MSG_APP = 0,  // 手机app透传数据
    MSG_RX_CMD,   // 接收到蓝牙指令
} msg_type_E;

typedef enum
{
    SLAVE_CONNECTED = 0,  // 从机已组网
    SLAVE_QUARY_DEV_TYPE, // 查询设备类型
    SLAVE_TIME_SYNC,      // 时间同步
    SLAVE_QUARY_RUNTIME,  // 查询运行时间
    SLAVE_PAIRED,         // 从机已配对
} slave_stat_machine_E;

typedef enum
{
    T_BLE_MODE_M = 1, // 主机模式
    T_BLE_MODE_S = 2, // 从机模式
    T_BLE_MODE_MS = 3 // 主从模式
} T_BleModeType;

typedef enum
{
    UNCONNECT = 0, // 未连接
    CONNECT = 1    // 连接
} T_BLECONNS_E;

/*---------------------- 结构体定义  ---------------------------------------*/
typedef struct
{
    PORT_t ble_dc_ctrl;    /*电源控制*/
    uart_func_t *ble_uart; /*串口*/
} ble_hw_config_t;

typedef struct
{
    uint8_t ble_mac[6];
    msg_type_E msg_type;
    uint8_t len;
    uint8_t data[BLE_TXRX_BUF_SIZE];
} T_BleData;

typedef struct
{
    uint8_t ble_mac[6];
    uint8_t mode;            // 蓝牙模式, ms:主从模式/m:主机模式/s:从机模式
    int8_t ble_tx_pow;       // 蓝牙发设功率, 有效值-128 ~127
    uint32_t linkkey;        // 蓝牙连接秘钥
    uint8_t appstatus;       // app模式开启状态
    uint32_t version;        // 版本号
    uint8_t long_range_en;   // 使能远距离模式
    uint8_t autoconn_enable; // 自动连接功能使能
} ble_module_config_t;       // 蓝牙模组工作参数配置

typedef struct
{
    uint8_t slave_pair_num;    // 从机配对数量(作为主机)
    uint8_t slave_connect_num; // 从机在线数量(作为主机)
    uint8_t is_host_connected; // 是否连接到主机(作为从机)
    uint8_t is_app_connected;  // 是否连接到app
    int8_t rssi;               // 信号接收强度(作为从机)
    volatile BOOL_E lock;             // 互斥锁
} ble_status_t;

typedef struct
{
    uint8_t mac_addr[6];
    BOOL_E is_dev_connected; // 是否连接到设备
    BOOL_E is_dev_configred;    // 从机是否配对成功,配对的从机设备下电前都会保存mac
    BOOL_E conn_event;       // 从机建立连接事件
    BOOL_E disconn_event;    // 从机断开连接事件
} master_conn_t;

typedef struct
{
    uint8_t mac_addr[6];
    uint8_t ble_conn_id;           // 从机蓝牙通信id,1~BLE_CONNECT_MAX_NUM,值等于ble_T中本数组的下标+1
    uint8_t slave_stat_machine; // 从机连接状态机

    BOOL_E is_dev_connected;    // 是否连接到设备
    BOOL_E is_dev_configred;    // 是否配对成功(型号查询,对时,协议握手都成功),配对的设备会记录mac,本次运行掉线和重连都保持ble_conn_id和从机相关数据.但下电不保留.

    BOOL_E conn_event;          // 建立连接事件
    BOOL_E disconn_event;       // 断开连接事件
} slave_conn_t;

typedef struct
{
    ble_hw_config_t hw_config;                  // 硬件配置,串口,GPIO
    ble_module_config_t module_config;          // 蓝牙模组工作参数配置
    ble_status_t ble_stat;                      // ble工作状态
    master_conn_t master_conn;                  // 和主机的连接
    master_conn_t app_conn;                     // 和手机的连接
    #if( LOCAL_MODULE_TYPE == _MU )
    slave_conn_t slave_conn[BLE_CONNECT_MAX_NUM]; // 和从机的连接
    #endif

    BOOL_E queue_create; // 蓝牙初始化标志;  
} ble_T;

/*---------------------- 全局变量  ---------------------------------------*/

extern ble_T hBle; // 蓝牙参数

/*---------------------- 接口函数  ---------------------------------------*/

/*****************************************************************************
 * @Name	  : ble_register
 * @Brief  : ble结构体注册
 * @Retval : None
 * @Note   :
 */
void ble_register(ble_hw_config_t *hw_config);
/*****************************************************************************
* @Name	  : ble_rst
* @brief  : BLE复位
* @param  : None
* @retval :
* @author :
* @Data   : 2023-05-23
*/
void ble_rst(void);
/*****************************************************************************
* @Name	  : ble_queue_create
* @Brief  : 创建队列
* @Retval : None
* @Note   :
*/
void ble_queue_create(void);
/*****************************************************************************
 * @Name	  : ble_init
 * @Brief  : ble初始化
 * @Retval : None
 * @Note   :
 */
void ble_init(void); // 初始化

/*****************************************************************************
 * @Name	  : ble_cmd
 * @Brief  : ble at指令
 * @Retval : None
 * @Note   :
 */
Status_t ble_cmd(uint8_t uccmd); // 蓝牙指令

/*****************************************************************************
 * @Name	  : ble_get_version
 * @Brief  : 获取蓝牙版本
 * @Retval : None
 * @Note   :
 */
uint32_t ble_get_version(void); // 获取蓝牙版本

/*****************************************************************************
 * @Name	  : ble_msg_send_by_mac
 * @Brief  : 发送自组网透传数据到从机
 * @Retval : None
 * @Note   :
 */
Status_t ble_msg_send_by_mac(uint8_t *msgbuf, uint16_t usMsgLen, uint8_t *mac_addr);

/*****************************************************************************
 * @Name	  : ble_slave_msg_send_by_id
 * @Brief  : 发送自组网透传数据到从机
 * @Param  : ble_conn_id: 从机蓝牙通信ID
 * @Retval : None
 * @Note   :
 */
Status_t ble_slave_msg_send_by_id(uint8_t *msgbuf, uint16_t usMsgLen, uint8_t ble_conn_id);

/*****************************************************************************
 * @Name	  : ble_broadcast_msg_send
 * @Brief  : 主机发送自组网广播数据至所有从机
 * @Retval : None
 * @Note   :
 */
Status_t ble_broadcast_msg_send(uint8_t *msgbuf, uint16_t usMsgLen); // 蓝牙消息发送

/*****************************************************************************
 * @Name	  : ble_master_msg_send
 * @Brief  : 发送自组网透传数据到主机
 * @Retval : None
 * @Note   :
 */
Status_t ble_master_msg_send(uint8_t *msgbuf, uint16_t usMsgLen); // 蓝牙消息发送

/*****************************************************************************
 * @Name	  : ble_app_msg_send
 * @Brief  : 发送自组网透传数据到手机app
 * @Retval : None
 * @Note   :
 */
Status_t ble_app_msg_send(uint8_t *msgbuf, uint16_t usMsgLen); // 蓝牙消息发送

/*****************************************************************************
 * @Name	  : ble_msg_receive
 * @Brief  : 接收自组网透传数据
 * @Retval : None
 * @Note   :
 */
uint16_t ble_msg_receive(T_BleData *rx_buf); // 蓝牙消息接收

/*****************************************************************************
 * @Name	  : ble_set_conn_en
 * @Brief  : 使能自组网功能
 * @Retval : None
 * @Note   :
 */
void ble_set_conn_en(uint8_t enable);

/*****************************************************************************
* @Name	  : ble_uart_rx_data_handle
* @Brief  : 所有ble串口收包在这里统一分类,将收到的消息分为AT指令回包和推送消息并分别存储
            ,建议放在一个调度周期不高于50ms的任务中,不要和ble run在同一个任务
* @Retval : None
* @Note   :
*/
void ble_uart_rx_data_handle(void);

/*****************************************************************************
 * @Name	  : get_ru_data_index_by_id
 * @Brief  : 根据从设备ID获取从机索引(ID: 通信id, 索引: 从机结构体数组下标)
 * @Param  : ble_conn_id:   从机蓝牙通信ID, 1~BLE_CONNECT_MAX_NUM
 * @Retval : 返回从机索引
 * @Author :
 * @Data   : 2024-1-11
 * @Note   :
*/
uint8_t get_ble_index_by_slave_id(uint8_t ble_conn_id);


#endif
