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

#ifndef __APP_MRC_H__
#define __APP_MRC_H__

#include "bsp_config.h"
#include "mcp_core.h"
#include "mu_ru_comm_para_list.h"
#include "ble.h"
#include "mcpMRC_user.h"
#include "ru_device_list.h"
/*---------------------- 端口定义 --------------------------------------*/

/*---------------------- 宏定义  ---------------------------------------*/

/*---------------------- 结构体 ----------------------------------------*/
typedef struct __MRC_MU_PARA_T
{
    BOOL_E pc_switch;         // 系统功控开关 0:关 1:开
    int8_t lte_ul_pwr_expect; // LTE上行输入功率期望值
    int8_t lte_tx_pwr;        // LTE下行输出功率
    int8_t nr_ul_pwr_expect;  // NR上行输入功率期望值
    int8_t nr_tx_pwr;         // NR下行输出功率
    uint8_t hub_led1;         // HUB通道1点灯状态 0:跟随通道电源开关 1:关闭 2:点亮 
    uint8_t hub_led2;         // HUB通道2点灯状态 0:跟随通道电源开关 1:关闭 2:点亮 
    uint8_t hub_led3;         // HUB通道3点灯状态 0:跟随通道电源开关 1:关闭 2:点亮 
    uint8_t hub_led4;         // HUB通道4点灯状态 0:跟随通道电源开关 1:关闭 2:点亮 
    uint8_t hub_led5;         // HUB通道5点灯状态 0:跟随通道电源开关 1:关闭 2:点亮 
    uint8_t hub_led6;         // HUB通道6点灯状态 0:跟随通道电源开关 1:关闭 2:点亮 
    uint8_t hub_led7;         // HUB通道7点灯状态 0:跟随通道电源开关 1:关闭 2:点亮 
    uint8_t hub_led8;         // HUB通道8点灯状态 0:跟随通道电源开关 1:关闭 2:点亮 

} mrc_mu_para_t; // 主从通信模块,主机参数列表

/*---------------------- 全局变量 --------------------------------------*/

extern ru_device_t ru_device_list[BLE_CONNECT_MAX_NUM]; // 蓝牙从机设备列表
extern mrc_mu_para_t mrc_mu_para;                       // 主从通信模块,主机参数列表

/*---------------------- 函数声明 --------------------------------------*/

/*****************************************************************************
 * @Name	  : mrc_init
 * @Brief  : 初始化主从通信模块
 * @Retval : None
 * @Note   :
 */
void mrc_init(void);

/*****************************************************************************
 * @Name	  : mrc_init
 * @Brief  : 初始化主从通信模块
 * @Retval : None
 * @Note   :
 */
void mrc_process(void);

/*****************************************************************************
 * @Name	  : mrc_recv_msg_in_task
 * @Brief  : 接收蓝牙数据,处理MCP协议数据,发送蓝牙数据回包
 * @Retval : None
 * @Note   :
 */
void mrc_recv_msg_in_task(void);

/*****************************************************************************
 * @Name	  : mrc_reset_ru
 * @Param  : ble_conn_id:蓝牙连接ID, 0xFF:重置所有连接的RU
 * @Retval : None
 * @Note   :
 */
void mrc_reset_ru(uint8_t ble_conn_id);
void mrc_reset_all_ru(void);

/*****************************************************************************
 * @Name	  : mrc_set_system_power_ctrl(B_BOOL pc_switch, int8_t lte_ul_pwr_expect, int8_t lte_tx_pwr, int8_t nr_ul_pwr_expect, int8_t nr_tx_pwr)
 * @Param  : pc_switch: 系统功控开关 0:关 1:开
 * @Param  : lte_ul_pwr_expect:LTE上行功率预期值
 * @Param  : lte_tx_pwr:LTE下行功率
 * @Param  : nr_ul_pwr_expect:NR上行功率预期值
 * @Param  : nr_tx_pwr:NR下行功率
 * @Retval : None
 * @Note   : 设置系统功控参数,蓝牙将系统功控参数周期性广播给所有从机
 */

void mrc_set_system_power_ctrl(BOOL_E pc_switch, int8_t lte_ul_pwr_expect, int8_t lte_tx_pwr, int8_t nr_ul_pwr_expect, int8_t nr_tx_pwr);

Status_t get_slave_dev_info(uint8_t ble_conn_id, uint32_t timeout);
#endif /* __APP_MRC_H__ */
