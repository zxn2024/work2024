/***************************** Copyright ****************************************
*
* (C) Copyright 2022, Shaanxi Tianji Communication
*  All Rights Reserved
*
* FileName   : bsp_rf_slave.c
* Version    : none
* Author     : ZhangWei
* Date       : 2022-09-20
* Description: none
*******************************************************************************/
#include "main.h"

#include "bsp_config.h"
#include "rtt.h"

#include "bsp_rf_slave.h"
#include "bsp_dev_para.h"
#include "hi_systick.h"
#include "hi_uart.h"

/*---------------------- 端口定义 --------------------------------------*/


/*---------------------- 宏定义  ---------------------------------------*/
#define __BLE_PRINTF_EN__   (0)

/*---------------------- 结构体 ----------------------------------------*/
typedef struct
{
    uint8_t  id;			//设备编号
    uint8_t  sn[32];		//sn
    uint8_t  mac[48];
    uint32_t mcu_ver;		//MCU软件版本
    uint32_t fpga_ver;		//fpga软件版本
    uint8_t  rf_switch;		//射频开关
    uint8_t  led_switch;	//指示灯开关
} slave_dev_public_t;


typedef struct
{
    uint8_t theo_gain;
    int8_t  theo_pow;
    uint8_t actual_gain;
    int8_t  actual_pow;
    int8_t  gain_offset;
    int8_t  pow_offset;
    int8_t  oplc_offset;            // 开环工控偏移量
    int8_t  set_gain_offset;        // CH1上行增益偏移量, 有效值-128 ~127
    int8_t  set_pow_offset;         // CH1上行功率偏移量, 有效值-128 ~127
    int8_t  set_oplc_offset;        // 开环工控偏移量
} slave_dev_rf_ch_t;


typedef struct
{
    uint8_t frame;

} slave_dev_net_t;


typedef struct
{
    uint8_t port_state;  //端口检测状态 正常 / 空载 / 故障
    uint8_t port_switch; //端口开关状态 开启 / 关闭
} hub_t;


typedef struct
{
    slave_dev_public_t t_dev_public;
    slave_dev_net_t    t_net;
    slave_dev_rf_ch_t  t_ch_para[RF_SYS_NUM][RF_CH_NUM];
} slave_para_t;


/*---------------------- 常量 ------------------------------------------*/


/*---------------------- 全局变量 --------------------------------------*/
slave_dev_rf_ch_t    t_slave_dev_para[RF_SYS_NUM][RF_CH_NUM];   //射频静态参数
uint8_t ucSlaveDevNumFromFlash; //从FLASH中读取到的从机数量

/*---------------------- 局部变量 --------------------------------------*/



/*---------------------- 声明 ------------------------------------------*/



/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ HAL ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


