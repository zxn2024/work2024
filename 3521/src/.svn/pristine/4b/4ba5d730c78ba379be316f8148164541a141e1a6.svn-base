/*************************************** Copyright ******************************
*
* (C) Copyright 2022, Shaanxi Tianji Communication
* All Rights Reserved
*                         
* FileName   : bsp_rf_slave_struct.h   
* Version    : none		
* Author     : ZhangWei			
* Date       : 2022-09-27         
* Description: none   
*******************************************************************************/
#ifndef __BSP_RF_SLAVE_STRUCT_H_
#define __BSP_RF_SLAVE_STRUCT_H_

#include <stdint.h>
#include <bsp_dev_config.h>
//#include <bsp_public_enum.h>

typedef struct
{
    uint8_t  id;             // 设备编号, 有效值0~15, 0代表FSRU设备蓝牙模块, 1~15代表FARU模块
    uint8_t  sn[30];         // SN, 字符串型, 最长30字节
    uint8_t  factoryId;      // 厂商代码
    uint8_t  machType;       // 设备类别
    uint8_t  machModel[20];  // 设备型号
    uint8_t  run_state;      // 运行状态, 0:设备不存在, 1:正常, 2:升级模式
    uint8_t  led_switch;     // 指示灯开关状态, 0:关闭, 1打开
    uint8_t  slave_num;      // 从机数量 , 有效值1~15
    uint32_t sw_ver;         // 软件版本, 有效值21000~65535
    uint8_t  temperature;    // 温度值, 有效值-128 ~ 127
    uint8_t  frame;          // 帧结构, 有效值1~3
    uint8_t  sfg;            // 特殊子帧结构, 有效值0~9
    uint8_t  timeOffset;     // 时间偏移量,有效值-100~100
    uint8_t  ookFreqPoint;   //OOK频点,1:915MHz,2:632MHz
    
    uint8_t  set_led_switch;    // 指示灯开关状态, 0:关闭, 1打开
    uint8_t  set_frame;         // 帧结构, 有效值1~3
    uint8_t  set_sfg;           // 特殊子帧结构, 有效值0~9
    uint8_t  set_timeOffset;    // 时间偏移量,有效值-100~100
    uint8_t  set_ookFreq;       //ook频点, 1:915MHz, 2:632MHz
}SlaveDevPublic_T;

typedef struct
{
    uint8_t  mac;               // 运行状态, 0:设备不存在, 1:正常, 2:升级模式
    uint8_t  slave_num;         // 从机数量, 有效值1~15
    uint8_t  bleAnt_switch;     // 蓝牙天线开关状态, 0:RF, 1:ANT
    int8_t   ble_rx_pow;        // 蓝牙接收功率, 有效值-128 ~127
    int8_t   ble_tx_pow;        // 蓝牙发设功率, 有效值-128 ~127
    uint8_t  ook_freq;          // OOK频点, 1:915MHz,2:632MHz
    uint8_t  whiteListEn;       // 白名单使能
}BlePara_T;

typedef struct
{
    uint8_t sw_ver;             // FPGA软件版本, 有效值21000~65535
    uint8_t communication_ver;  // 通信协议版本有效值1~255
    uint8_t warn1;              // FPGA告警1, 按bit表示, 0:正常, 1:告警
    uint8_t warn2;              // FPGA告警2, 按bit表示, 0:正常, 1:告警
}FpgaPara_T;

//UP/DOWN 链路参数
typedef struct
{
    int8_t theory_gain;            // CH1上行理论增益, 有效值-128 ~127
    int8_t throry_pow;             // CH1上行理论输出功率, 有效值-128 ~127
    int8_t actual_gain;            // CH1上行实时增益, 有效值-128 ~127
    int8_t actual_pow;             // CH1上行实时输出功率, 有效值-128 ~127
    int8_t gain_temp_offset;       // CH1上行增益温补值, 有效值-128 ~127
    int8_t pow_temp_offset;        // CH1上行功率温补值, 有效值-128 ~127
    int8_t gain_offset;            // CH1上行增益偏移量, 有效值-128 ~127
    int8_t pow_offset;             // CH1上行功率偏移量, 有效值-128 ~127
    int8_t oplc_offset;            // 开环工控偏移量
    int8_t set_gain_offset;        // CH1上行增益偏移量, 有效值-128 ~127
    int8_t set_pow_offset;         // CH1上行功率偏移量, 有效值-128 ~127
    int8_t set_oplc_offset;         // 开环工控偏移量
}SlaveDevCh_T;

typedef struct
{
    uint8_t sync;       // 同步状态, 0:未同步, 1:同步
    uint8_t rf_switch;  // 下行射频开关状态, 有效值0~3, 0:关闭, 1:开启, 2:上行单通, 3:下行单通
    uint8_t warn1;      // 告警参数1, 按bit表示, 0:正常, 1:告警
    uint8_t warn2;      // 告警参数2, 按bit表示, 0:正常, 1:告警
    SlaveDevCh_T ch[ 2 ];  // ch[0]: UP, ch[1]: DOWN
}SlaveDevSysPara_T;


typedef struct
{
    SlaveDevPublic_T s_public;  //公共参数
    BlePara_T        ble;       //BLE参数
    FpgaPara_T       fpga;      //FPGA参数
    SlaveDevSysPara_T rf[ RF_SYS_NUM ];    //rf[0]: SYS1, rf[1]: SYS2
}SlaveDevPara_T;

#endif



