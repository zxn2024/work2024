
/***************************** Copyright ****************************************
 *
 * (C) Copyright 2023, Shaanxi Tianji Communication
 *  All Rights Reserved
 *
 * FileName   : mcpMRC_user.c
 * Version    : none
 * Author     : none
 * Date       : 2023-06-20
 * Description: 主从通信协议的用户层代码实现
 *******************************************************************************/
#ifndef __MCP_MRC_USER_H__
#define __MCP_MRC_USER_H__

#include "bsp_config.h"
#include "mcp_core.h"

/*---------------------- 宏定义  ---------------------------------------*/

// typedef union __MCP_DATA_U
// {
//     uint8_t byte;
//     int8_t sbyte;
//     uint16_t word;
//     int16_t sword;
//     uint32_t dword;
//     int32_t sdword;
// } MRC_DATA_U; // Mu Ru communication data union

typedef enum
{
    MRC_DATA_IDLE = 0,
    MRC_DATA_SEND = 1,    // 发包
    MRC_DATA_RECV_OK = 2, // 收包成功
    MRC_DATA_RECV_ERR = 3 // 收包失败
} MRC_SEND_STATE_E;       // Mu Ru communication data state

/*---------------------- 结构体定义  ---------------------------------------*/
// typedef struct __MRC_DATA_T
// {
//     MRC_DATA_U data; // 数据为数值时,存放数值
//     uint8_t *data_str;   // 数据是字符串时,指向字符串首地址
//     MCP_TYPE_E data_type; // 数据类型
//     uint8_t quary_switch; // 轮询开关
//     uint32_t quary_period; // 查询周期
//     MRC_SEND_STATE_E send_stat; // 发包回包标志
// } MRC_DATA_T; // Mu Ru communication data

// typedef struct __MRC_HUB_PKT_T
// {
//     uint8_t device_type; // 单片机对时包设置,用于设置时间同步值给RU
//     uint32_t mcu_sync_time; // 单片机对时包设置,用于设置时间同步值给RU
//     uint32_t mcu_run_time; // 设备单片机运行时间查询
//     uint8_t device_id; // 设备id设置查询
// } MRC_HUB_PKT_T; // Mu hub communication data

typedef struct __resp_pkt_info_t
{
    uint16_t mrc_pkt_id;                // 主从通信包序号
    uint8_t mrc_pkt_type;               // 主从通信包类型 上报:MCP_CMD_TYPE_RPT 查询:MCP_CMD_TYPE_QUERY 设置:MCP_CMD_TYPE_SET
    MRC_SEND_STATE_E mrc_pkt_send_stat; // 包状态
} pkt_send_stat_t;                      // 返回包信息

#if (LOCAL_MODULE_TYPE == _MU)
typedef struct __MRC_RU_DATA_T // ru和hub的数据
{
    /***********************主从通信协议MRC数据**********************/
    // RU&HUB共用数据
    uint32_t mcu_sync_time;     // 单片机对时包设置,用于设置时间同步值给RU
    uint8_t device_type;        // 设备类型 _MU _RU _HUB
    uint32_t mcu_run_time;      // 设备单片机运行时间查询
    uint8_t device_id;          // 设备编号设置查询
    uint32_t mcu_sw_ver_u32;    // MCU软件版本
    uint8_t mcu_sw_ver_str[20]; // MCU软件版本字符串
    uint32_t ble_sw_ver;        // BLE软件版本

    // RU数据
    uint32_t fpga_sw_ver;     // FPGA软件版本
    uint32_t ecr8668_fw1_ver; // 8668固件1版本
    uint32_t ecr8668_fw2_ver; // 8668固件2版本
    uint32_t ecr8668_fw3_ver; // 8668固件3版本
    uint32_t ecr8668_fw4_ver; // 8668固件4版本
    uint32_t ecr8668_fw5_ver; // 8668固件5版本

    int8_t mrc_lte_in_pwr;    // 8668固件3版本
    int8_t mrc_nr_in_pwr;     // 8668固件4版本
    int8_t mrc_nr2_in_pwr;    // 8668固件4版本
    uint8_t mrc_self_excited; // 8668固件5版本
    uint8_t mrc_silent;       // 静默模式


    // HUB数据
    int32_t hub_ch1_open_time; // HUB通道1开启时间查询
    int32_t hub_ch2_open_time; // HUB通道2开启时间查询
    int32_t hub_ch3_open_time; // HUB通道3开启时间查询
    int32_t hub_ch4_open_time; // HUB通道4开启时间查询
    int32_t hub_ch5_open_time; // HUB通道5开启时间查询
    int32_t hub_ch6_open_time; // HUB通道6开启时间查询
    int32_t hub_ch7_open_time; // HUB通道7开启时间查询
    int32_t hub_ch8_open_time; // HUB通道8开启时间查询

    uint8_t hub_ch1_open; // HUB通道1开关
    uint8_t hub_ch2_open; // HUB通道2开关
    uint8_t hub_ch3_open; // HUB通道3开关
    uint8_t hub_ch4_open; // HUB通道4开关
    uint8_t hub_ch5_open; // HUB通道5开关
    uint8_t hub_ch6_open; // HUB通道6开关
    uint8_t hub_ch7_open; // HUB通道7开关
    uint8_t hub_ch8_open; // HUB通道8开关

    /***********************MCPA 主从查询数据**********************/

    uint8_t manu_id[20];       // 厂商ID
    uint8_t mcp_dev_type;      // 设备类别
    uint8_t mcp_dev_model[20]; // 设备类别
    uint8_t sn[30];            // sn号
    uint32_t route_addr;       // 路由登记地址

    uint8_t fdd_warn_th_dl_in_op; // FDD下行输入功率过功率门限
    uint8_t fdd_warn_th_dl_in_lp; // FDD下行输入功率欠功率门限
    uint8_t tdd_warn_th_dl_in_op; // TDD下行输入功率过功率门限
    uint8_t tdd_warn_th_dl_in_lp; // TDD下行输入功率欠功率门限

    uint8_t nr1_warn_th_dl_in_op; // 5G 通道1下行输入功率过功率门限
    uint8_t nr1_warn_th_dl_in_lp; // 5G 通道1下行输入功率欠功率门限
    uint8_t nr2_warn_th_dl_in_op; // 5G 通道2下行输入功率过功率门限
    uint8_t nr2_warn_th_dl_in_lp; // 5G 通道2下行输入功率欠功率门限

    uint8_t fdd_warn_dl_in_op; // FDD下行输入过功率告警
    uint8_t fdd_warn_dl_in_lp; // FDD下行输入欠功率告警
    uint8_t tdd_warn_dl_in_op; // TDD下行输入过功率告警
    uint8_t tdd_warn_dl_in_lp; // TDD下行输入欠功率告警

    uint8_t nr1_warn_dl_in_op;       // 5G通道1下行输入过功率告警
    uint8_t nr1_warn_dl_in_lp;       // 5G通道1下行输入欠功率告警
    uint8_t nr2_warn_dl_in_op;       // 5G通道2下行输入过功率告警
    uint8_t nr2_warn_dl_in_lp;       // 5G通道2下行输入欠功率告警
    uint8_t fdd_warn_self_execition; // 自激告警

    uint8_t ble_conn_id; // 设备主从组网id号 主机: 0  从机: 1~BLE_CONNECT_MAX_NUM
    // 当需要等待回包时,将发包信息保存到resp_pkt_info中,等待回包时,根据resp_pkt_info判断是否是同一包,并更新收包成功状态
    pkt_send_stat_t pkt_send_stat;

} MRC_RU_DATA_T; // Mu Ru communication data

extern MRC_RU_DATA_T mrc_ru_data[BLE_CONNECT_MAX_NUM];
#elif (LOCAL_MODULE_TYPE == _RU)

typedef struct __MRC_RU_DATA_T // ru和hub的数据
{
    /***********************主从通信协议MRC数据**********************/
    // RU&HUB共用数据
    int32_t sync_time_offset; // 时间同步值偏移,记录从机和主机之间的时间差(systick差)= mu - ru;
    // RU数据
    BOOL_E mrc_pc_switch;         // 系统功控开关 0:关 1:开
    int8_t mrc_lte_ul_pwr_expect; // LTE上行输入功率期望值
    int8_t mrc_lte_tx_pwr;        // LTE下行输出功率
    int8_t mrc_nr_ul_pwr_expect;  // NR上行输入功率期望值
    int8_t mrc_nr_tx_pwr;         // NR下行输出功率
    // 当需要等待回包时,将发包信息保存到resp_pkt_info中,等待回包时,根据resp_pkt_info判断是否是同一包,并更新收包成功状态
    pkt_send_stat_t pkt_send_stat;

} MRC_RU_DATA_T; // Mu Ru communication data

extern MRC_RU_DATA_T mrc_ru_data[1];

#endif

/*---------------------- 接口函数  ---------------------------------------*/
/*****************************************************************************
 * @Name	  : mcpMRC_init
 * @Brief  : 初始化mcpa监控协议数据结构
 * @Param  : 无
 * @Retval : 无
 * @Author :
 * @Data   : 2024-1-11
 * @Note   :
 */
void mcpMRC_init(void);

/*****************************************************************************
 * @Name	  : get_ble_conn_id_by_monitor_ru_id
 * @Brief  : 根据监控的从站编号查询从机的ble_conn_id
 * @Param  : ru_id: 监控的从站编号
 * @Retval : ble_conn_id: 子站点ble_conn_id, 0表示未找到
 * @Author :
 * @Data   : 2024-1-13
 * @Note   :
 */
uint8_t get_ble_conn_id_by_monitor_ru_id(uint8_t ru_id);

/*****************************************************************************
 * @Name	  : new_mrc_send_pkt
 * @Brief  : 初始化一个发送报文,用于主设备发给从设备的包,支持mcp A B C MRC
 * @Param  : pkt_buf: 发送数据缓存区
 * @Param  : ble_conn_id: 主从通信的设备ID 主机: 0  从机: 1~BLE_CONNECT_MAX_NUM
 * @Param  : mcp_proto_type: MCP协议类型
 * @Param  : cmd_type: 命令类型
 * @Retval : None
 * @Author :
 * @Data   : 2024-1-10
 * @Note   :
 */
void new_mrc_send_pkt(mcp_send_pkt_t *pkt_buf, uint8_t ble_conn_id, uint8_t mcp_proto_type, uint8_t cmd_type);
/*****************************************************************************
 * @Name	  : add_mrc_pkt_data
 * @Brief  : 添加数据块到发送报文,用于主设备发给从设备的包,支持mcp A B C MRC
 * @Param  : pkt_buf: 发送数据缓存区
 * @Param  : code: 数据ID
 * @Param  : para_buf: 数据值
 * @Param  : strlen: 数据长度
 * @Retval : None
 * @Author :
 * @Data   : 2024-1-10
 * @Note   :
 */
Status_t add_mrc_pkt_data(mcp_send_pkt_t *pkt_buf, uint16_t code, uint8_t *para_buf, uint8_t strlen);
/*****************************************************************************
 * @Name	  : mrc_send_pkt_to_ru
 * @Brief  : 发送数据包给从设备,支持mcp A B C MRC
 * @Param  : pkt_buf: 发送数据缓存区
 * @Param  : ble_conn_id: 主从通信的从机ID
 * @Retval : None
 * @Author :
 * @Data   : 2024-1-10
 * @Note   :
 */
void mrc_add_tail_and_escape(mcp_send_pkt_t *pkt_buf);

/*****************************************************************************
 * @Name	  : mrc_send_pkt_to_ru
 * @Brief  : 发送数据包给从设备,支持mcp A B C MRC
 * @Param  : pkt_buf: 发送数据缓存区
 * @Param  : ble_conn_id: 主从通信的从机ID 从机: 1~BLE_CONNECT_MAX_NUM
 * @Retval : None
 * @Author :
 * @Data   : 2024-1-10
 * @Note   :
 */
Status_t mrc_send_pkt_to_ru(mcp_send_pkt_t *pkt_buf, uint8_t ble_conn_id);

Status_t mrc_send_pkt_to_mu(mcp_send_pkt_t *pkt_buf);

/*****************************************************************************
* @Name	  : app_mrc_set_ru_param
* @Brief  : 设置单个从机参数
* @params :
            ble_conn_id: 主从通信的从机ID  从机: 1~BLE_CONNECT_MAX_NUM
            param_id: 参数ID
            param_val: 输入参数值
            strlen: 参数长度
            timeout: 超时时间单位ms, 输入0表示不等待,发送后立刻返回

* @Note   :
*/
Status_t mrc_set_ru_param(uint8_t ble_conn_id, uint16_t param_id, void *param_val, uint8_t strlen, uint32_t timeout);
/*****************************************************************************
* @Name	  : app_mrc_get_ru_param
* @Brief  : 查询单个从机参数
* @params :
            ble_conn_id: 主从通信的从机ID  从机: 1~BLE_CONNECT_MAX_NUM
            param_id: 参数ID
            strlen: 参数长度
            timeout: 超时时间单位ms, 输入0表示不等待,发送后立刻返回,并且不返回参数值
* @Note   : 当返回成功后,在mrc_ru_data取对应数据
*/
Status_t mrc_query_ru_param(uint8_t ble_conn_id, uint16_t param_id, uint8_t strlen, uint32_t timeout);

// 等待回包
Status_t mrc_wait_response(uint8_t ble_conn_id, uint16_t pkt_id, uint32_t timeout);
#endif // __MCP_MRC_USER_H__
