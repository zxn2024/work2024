/**
 * @file bsp_ecr8668.h
 * @author tjtanq
 * @brief
 * @date 2023-02-17
 *
 * @copyright Copyright (c) 2023 SHANNXI TIANJI, Or Its Affiliates.All Rights Reserved.
 *
 */
#ifndef __BSP_ECR8668_H__
#define __BSP_ECR8668_H__

#include <stdint.h>
#include "rtt.h"
#include "ECR8660.h"
#include "stdarg.h"
#include "stdlib.h"
#include "string.h"
#include "ECR8660_SPI.h"

#define DFE_CONFIG_DELAY_FLAG 0x0020108C // 配置DFE回环前延时标志

extern const uint32_t  ecr8668_bin_len_b3;
extern const uint8_t   ecr8668_bin_b3[48928];
extern const uint32_t  ecr8668_bin_len_b8;
extern const uint8_t   ecr8668_bin_b8[48928];
extern const uint32_t  ecr8668_bin_len_b39;
extern const uint8_t   ecr8668_bin_b39[48928];
extern const uint32_t  ecr8668_bin_len_b40;
extern const uint8_t   ecr8668_bin_b40[48928];
extern const uint32_t  ecr8668_bin_len_n41;
extern const uint8_t   ecr8668_bin_n41[48928];


/*提供两个参考码表作为未校准前的默认码表,延伸宝的*/
/*正常工作下的码表--RX数字增益 RX模拟增益 TX数字增益 TX模拟增益*/
extern GAIN_TABLE_T gain_tab_b3;
extern GAIN_TABLE_T gain_tab_b8;
extern GAIN_TABLE_T gain_tab_b39;
extern GAIN_TABLE_T gain_tab_b40;
extern GAIN_TABLE_T gain_tab_n41;
/*抗阻塞模式下的码表--RX数字增益 RX模拟增益 TX数字增益 TX模拟增益*/
extern GAIN_TABLE_T gain_tab_b3_antiblocking;
extern GAIN_TABLE_T gain_tab_b8_antiblocking;
extern GAIN_TABLE_T gain_tab_b39_antiblocking;
extern GAIN_TABLE_T gain_tab_b40_antiblocking;
extern GAIN_TABLE_T gain_tab_n41_antiblocking;
/****************************** 结构体定义 ******************************/



/**********************************************单通模式配置**************************************************************/

/**************************CH_MODE************************************************/
// 通道模式选择 地址：ECR866x_CH_MODE_ADDR        寄存器数据：ECR866x_CH_MODE_0 ~17
#define ECR866x_CH_MODE_ADDR 0x20004000 // 通道选择地址
// CH0
/*通道0功能编号*/
#define CH0_1T1R_PLC_TRX_NO_LO 0 // 1T1R、CH0、公网带宽、TRx不共本振
#define CH0_1T1R_PLC_TRX_LO 1    // 1T1R、CH0、公网带宽、TRx共本振
#define CH0_1T_PLC 2             // 1T、CH0、公网带宽
#define CH0_1R_PLC 3             // 1R、CH0、公网带宽
#define CH0_1T1R_OWN_TRX_NO_LO 4 // 1T1R、CH0、自定义带宽、TRx不共本振
#define CH0_1T1R_OWN_TRX_LO 5    // 1T1R、CH0、自定义带宽、TRx共本振
#define CH0_1T_OWN 6             // 1T、CH0、自定义带宽
#define CH0_1R_OWN 7             // 1R、CH0、自定义带宽
/*CH0寄存器值*/
#define ECR866x_CH_MODE_0 0x00020505 // 1T1R、CH0、公网带宽、TRx不共本振
#define ECR866x_CH_MODE_1 0x00024505 // 1T1R、CH0、公网带宽、TRx共本振
#define ECR866x_CH_MODE_2 0x00020500 // 1T、CH0、公网带宽
#define ECR866x_CH_MODE_3 0x00020005 // 1R、CH0、公网带宽
#define ECR866x_CH_MODE_4 0x00030505 // 1T1R、CH0、自定义带宽、TRx不共本振
#define ECR866x_CH_MODE_5 0x00034505 // 1T1R、CH0、自定义带宽、TRx共本振
#define ECR866x_CH_MODE_6 0x00030500 // 1T、CH0、自定义带宽
#define ECR866x_CH_MODE_7 0x00030005 // 1R、CH0、自定义带宽
// CH1
/*通道1功能编号*/
#define CH1_1T1R_PLC_TRX_NO_LO 0x10 // 1T1R、CH0、公网带宽、TRx不共本振
#define CH1_1T1R_PLC_TRX_LO 0x11    // 1T1R、CH0、公网带宽、TRx共本振
#define CH1_1T_PLC 0x12             // 1T、CH0、公网带宽
#define CH1_1R_PLC 0x13             // 1R、CH0、公网带宽
#define CH1_1T1R_OWN_TRX_NO_LO 0x14 // 1T1R、CH0、自定义带宽、TRx不共本振
#define CH1_1T1R_OWN_TRX_LO 0x15    // 1T1R、CH0、自定义带宽、TRx共本振
#define CH1_1T_OWN 0x16             // 1T、CH0、自定义带宽
#define CH1_1R_OWN 0x17             // 1R、CH0、自定义带宽
/*CH1寄存器值*/
#define ECR866x_CH_MODE_10 0x00020909 // 1T1R、CH1、公网带宽、TRx不共本振
#define ECR866x_CH_MODE_11 0x00024900 // 1T1R、CH1、公网带宽、TRx共本振
#define ECR866x_CH_MODE_12 0x00020900 // 1T、CH1、公网带宽
#define ECR866x_CH_MODE_13 0x00020909 // 1R、CH1、公网带宽
#define ECR866x_CH_MODE_14 0x00030909 // 1T1R、CH1、自定义带宽、TRx不共本振
#define ECR866x_CH_MODE_15 0x00034909 //: 1T1R、CH1、自定义带宽、TRx共本振
#define ECR866x_CH_MODE_16 0x00030900 // 1T、CH1、自定义带宽
#define ECR866x_CH_MODE_17 0x00030009 // 1R、CH1、自定义带宽
/***********************************CH_MODE_END************************************/

/*********************************带宽配置******************************/
#define ECR866x_BW_addr 0x20004004 // 带宽配置地址
// 公网带宽
/*公网带宽编号*/
#define PLC_LTE_10MHZ (10)         // LTE 10 MHz
#define PLC_LTE_15MHZ (15)         // LTE 15 MHz
#define PLC_LTE_20MHZ (20)         // LTE 20 MHz
#define PLC_LTE_25MHZ (25)         // LTE 15 MHz
#define PLC_LTE_30MHZ (30)         // LTE 20 MHz
#define PLC_LTE_50MHZ (50)         // LTE 20 MHz
#define PLC_NR_20MHZ (20)          // NR 20 MHz
#define PLC_NR_60MHZ (60)          // NR 60 MHz
#define PLC_NR_80MHZ (80)          // NR 80 MHz
#define PLC_NR_100MHZ (100)        // NR 100 MHz
/*寄存器值*/
#define ECR866x_BW_PUB_NETBAND_0 0x00000000 // LTE 1.4 MHz
#define ECR866x_BW_PUB_NETBAND_1 0x00010001 // LTE 3 MHz/TD-SCDMA
#define ECR866x_BW_PUB_NETBAND_2 0x00020002 // LTE 5 MHz/WCDMA
#define ECR866x_BW_PUB_NETBAND_3 0x00030003 // LTE 10 MHz
#define ECR866x_BW_PUB_NETBAND_4 0x00040004 // LTE 15 MHz
#define ECR866x_BW_PUB_NETBAND_5 0x00050005 // LTE 20 MHz
#define ECR866x_BW_PUB_NETBAND_6 0x00060006 // NR 40 MHz
#define ECR866x_BW_PUB_NETBAND_7 0x00070007 // NR 60 MHz
#define ECR866x_BW_PUB_NETBAND_8 0x00080008 // NR 80 MHz
#define ECR866x_BW_PUB_NETBAND_9 0x00090009 // NR 100 MHz
// 自定义带宽-在自定义带宽通道模式下的带宽命令
/*自定义带宽编号*/
#define ECR866x_BW_CUSTOM_NETBAND_A 0x00000000 // 自定义带宽1,需要和其他通道配置同时设置生效,下同
#define ECR866x_BW_CUSTOM_NETBAND_B 0x00010001 // 自定义带宽2
#define ECR866x_BW_CUSTOM_NETBAND_C 0x00020002 // 自定义带宽3
#define ECR866x_BW_CUSTOM_NETBAND_D 0x00030003 // 自定义带宽4

#define USER_LTE_B3_20MHZ ECR866x_BW_CUSTOM_NETBAND_A// B3 20 MHz
#define USER_LTE_B3_25MHZ ECR866x_BW_CUSTOM_NETBAND_B// B3 25 MHz
#define USER_LTE_B8_10MHZ ECR866x_BW_CUSTOM_NETBAND_A// B8 10 MHz
#define USER_LTE_B8_15MHZ ECR866x_BW_CUSTOM_NETBAND_B// B8 20 MHz
#define USER_LTE_B39_10MHZ ECR866x_BW_CUSTOM_NETBAND_A// B39 10 MHz
#define USER_LTE_B39_20MHZ ECR866x_BW_CUSTOM_NETBAND_B// B39 20 MHz
#define USER_LTE_B39_30MHZ ECR866x_BW_CUSTOM_NETBAND_C// B39 30 MHz
#define USER_LTE_B40_20MHZ ECR866x_BW_CUSTOM_NETBAND_A// B40 20 MHz
#define USER_LTE_B40_50MHZ ECR866x_BW_CUSTOM_NETBAND_B// B40 50 MHz
#define USER_NR_N41_20MHZ ECR866x_BW_CUSTOM_NETBAND_A// N41 20 MHz
#define USER_NR_N41_60MHZ ECR866x_BW_CUSTOM_NETBAND_B// N41 60 MHz
#define USER_NR_N41_80MHZ ECR866x_BW_CUSTOM_NETBAND_C// N41 80 MHz
#define USER_NR_N41_100MHZ ECR866x_BW_CUSTOM_NETBAND_D// N41 100 MHz


#define ECR866x_BW_DIRECT_addr 0x00201180 // 单独设置自定义带宽配置地址

#define ECR866x_BW_CUSTOM_NETBAND_A_DIRECT 0x0110000B // 单独设置自定义带宽1
#define ECR866x_BW_CUSTOM_NETBAND_B_DIRECT 0x1110000B // 单独设置自定义带宽2
#define ECR866x_BW_CUSTOM_NETBAND_C_DIRECT 0x2110000B // 单独设置自定义带宽3
#define ECR866x_BW_CUSTOM_NETBAND_D_DIRECT 0x3110000B // 单独设置自定义带宽4

#define USER_BW_DIRECT_LTE_B3_20MHZ ECR866x_BW_CUSTOM_NETBAND_A_DIRECT// B3 20 MHz
#define USER_BW_DIRECT_LTE_B3_25MHZ ECR866x_BW_CUSTOM_NETBAND_B_DIRECT// B3 25 MHz
#define USER_BW_DIRECT_LTE_B8_10MHZ ECR866x_BW_CUSTOM_NETBAND_A_DIRECT// B8 10 MHz
#define USER_BW_DIRECT_LTE_B8_15MHZ ECR866x_BW_CUSTOM_NETBAND_B_DIRECT// B8 20 MHz
#define USER_BW_DIRECT_LTE_B39_10MHZ ECR866x_BW_CUSTOM_NETBAND_A_DIRECT// B39 10 MHz
#define USER_BW_DIRECT_LTE_B39_20MHZ ECR866x_BW_CUSTOM_NETBAND_B_DIRECT// B39 20 MHz
#define USER_BW_DIRECT_LTE_B39_30MHZ ECR866x_BW_CUSTOM_NETBAND_C_DIRECT// B39 30 MHz
#define USER_BW_DIRECT_LTE_B40_20MHZ ECR866x_BW_CUSTOM_NETBAND_A_DIRECT// B40 20 MHz
#define USER_BW_DIRECT_LTE_B40_50MHZ ECR866x_BW_CUSTOM_NETBAND_B_DIRECT// B40 50 MHz
#define USER_BW_DIRECT_NR_N41_20MHZ ECR866x_BW_CUSTOM_NETBAND_A_DIRECT// N41 20 MHz
#define USER_BW_DIRECT_NR_N41_60MHZ ECR866x_BW_CUSTOM_NETBAND_B_DIRECT// N41 60 MHz
#define USER_BW_DIRECT_NR_N41_80MHZ ECR866x_BW_CUSTOM_NETBAND_C_DIRECT// N41 80 MHz
#define USER_BW_DIRECT_NR_N41_100MHZ ECR866x_BW_CUSTOM_NETBAND_D_DIRECT// N41 100 MHz

/********************************带宽_END*******************************/

/**********************************RX频率设置*************************************/

// 接收频率设置值用户自定义，格式：将频率值转化为十六进制数写入频率设置地址
// 例如：频率为2565 则将2565000转化为16进制->0x00272388 写入寄存器地址即可完成配置

// CH0
#define ECR866x_CH0_RXFREQ_ADDR 0x20004008      // RX的频率设置地址
#define ECR866x_CH0_RXFREQ_SAVE_ADDR 0x00201180 // 单独配置频率生效寄存器地址
#define ECR866x_CH0_RXFREQ_SAVE_DATA 0x00000008 // 单独配置频率生效寄存器值
// CH1
#define ECR866x_CH1_RXFREQ_SAVE_ADDR 0x00201180 // 单独配置频率生效寄存器地址
#define ECR866x_CH1_RXFREQ_SAVE_DATA 0x10000008 // 单独配置频率生效寄存器值
#define ECR866x_CH1_RXFREQ_ADDR 0x20004010      // RX的频率设置地址
/************************************RXEND****************************************/

/********************************TX频率设置***************************/
// 设置方式和RX一样

// CH0
#define ECR866x_CH0_TXFREQ_ADDR 0x20004018      // TX的频率设置地址
#define ECR866x_CH0_TXFREQ_SAVE_ADDR 0x00201180 // 单独配置频率生效寄存器地址
#define ECR866x_CH0_TXFREQ_SAVE_DATA 0x20000008 // 单独配置频率生效寄存器值
// CH1
#define ECR866x_CH1_TXFREQ_ADDR 0x20004020      // TX的频率设置地址
#define ECR866x_CH1_TXFREQ_SAVE_ADDR 0x00201180 // 单独配置频率生效寄存器地址
#define ECR866x_CH1_TXFREQ_SAVE_DATA 0x30000008 // 单独配置频率生效寄存器值
/**********************************TX_END*****************************/

/*************************************RX_GAIN****************************************/
// CH0
#define ECR866x_CH0_RXGAIN_ADDR 0x20004028         // RX的增益偏移量设置：默认0
#define ECR866x_CH0_RXGAIN_DEFAULT_DATA 0x00008025 // 增益偏移量默认0时的寄存器值
#define ECR866x_CH0_RXGAIN_SAVE_ADDR 0x00201180    // 单独配置接受增益生效寄存器地址
#define ECR866x_CH0_RXGAIN_SAVE_DATA 0x00000009    // 单独配置接受增益生效寄存器值
// CH1
#define ECR866x_CH1_RXGAIN_ADDR 0x2000402C         // RX的增益偏移量设置：默认0
#define ECR866x_CH1_RXGAIN_DEFAULT_DATA 0x0000802B // 增益偏移量默认0时的寄存器值
#define ECR866x_CH1_RXGAIN_SAVE_ADDR 0x00201180    // 单独配置接受增益生效寄存器地址
#define ECR866x_CH1_RXGAIN_SAVE_DATA 0x00000009    // 单独配置接受增益生效寄存器值
/*************************************RX_END*****************************************/

/*************************************TX_GAIN****************************************/
// CH0
#define ECR866x_CH0_TXGAIN_ADDR 0x20004030         // TX的增益偏移量设置：默认0
#define ECR866x_CH0_TXGAIN_DEFAULT_DATA 0x00008020 // 增益偏移量默认0时的寄存器值
#define ECR866x_CH0_TXGAIN_SAVE_ADDR 0x00201180    // 单独配置发送增益生效寄存器地址
#define ECR866x_CH0_TXGAIN_SAVE_DATA 0x00000009    // 单独配置发送增益生效寄存器值
// CH1
#define ECR866x_CH1_TXGAIN_ADDR 0x20004034         // TX的增益偏移量设置：默认0
#define ECR866x_CH1_TXGAIN_DEFAULT_DATA 0x00008020 // 增益偏移量默认0时的寄存器值
#define ECR866x_CH1_TXGAIN_SAVE_ADDR 0x00201180    // 单独配置发送增益生效寄存器地址
#define ECR866x_CH1_TXGAIN_SAVE_DATA 0x00000009    // 单独配置发送增益生效寄存器值
/*************************************TX_END*****************************************/

/**************************************PORT******************************************/
// CH0
#define ECR866x_CH0_PORT_ADDR 0x00201180 // RTX端口配置地址
/*CH0 端口编号*/
#define CH0_RXA_TXA 0x11
#define CH0_RXB_TXA 0x21
#define CH0_RXC_TXA 0x31
#define CH0_RXA_TXB 0x12
#define CH0_RXB_TXB 0x22
#define CH0_RXC_TXB 0x32
#define CH0_TXA 0x1
#define CH0_TXB 0x2
#define CH0_RXA 0x10
#define CH0_RXB 0x20
#define CH0_RXC 0x30
/*CH0 寄存器值*/
#define ECR866x_CH0_PORT_011 0x10100001 // RxA-TxA
#define ECR866x_CH0_PORT_021 0x20100001 // RxB-TxA
#define ECR866x_CH0_PORT_031 0x30100001 // RxC-TxA
#define ECR866x_CH0_PORT_012 0x10200001 // RxA-TxB
#define ECR866x_CH0_PORT_022 0x20200001 // RxB-TxB
#define ECR866x_CH0_PORT_032 0x30200001 // RxC-TxB
#define ECR866x_CH0_PORT_001 0x00100001 // TxA
#define ECR866x_CH0_PORT_002 0x00200001 // TxB
#define ECR866x_CH0_PORT_010 0x10000001 // RxA
#define ECR866x_CH0_PORT_020 0x20000001 // RxB
#define ECR866x_CH0_PORT_030 0x30000001 // RxC
// CH1
#define ECR866x_CH1_PORT_ADDR 0x00201180 // RTX端口配置地址
/*CH1 端口编号*/
#define CH1_RXA_TXA 0x111
#define CH1_RXB_TXA 0x121
#define CH1_RXC_TXA 0x131
#define CH1_RXA_TXB 0x112
#define CH1_RXB_TXB 0x122
#define CH1_RXC_TXB 0x132
#define CH1_TXA 0x101
#define CH1_TXB 0x102
#define CH1_RXA 0x110
#define CH1_RXB 0x120
#define CH1_RXC 0x130

#define ECR866x_CH1_PORT_111 0x01010001 // RxA-TxA
#define ECR866x_CH1_PORT_121 0x02010001 // RxB-TxA
#define ECR866x_CH1_PORT_131 0x03010001 // RxC-TxA
#define ECR866x_CH1_PORT_112 0x01020001 // RxA-TxB
#define ECR866x_CH1_PORT_122 0x02020001 // RxB-TxB
#define ECR866x_CH1_PORT_132 0x03020001 // RxC-TxB
#define ECR866x_CH1_PORT_101 0x00010001 // TxA
#define ECR866x_CH1_PORT_102 0x00020001 // TxB
#define ECR866x_CH1_PORT_110 0x01000001 // RxA
#define ECR866x_CH1_PORT_120 0x02000001 // RxB
#define ECR866x_CH1_PORT_130 0x03000001 // RxC
/************************************PORT_END****************************************/

/**************************************LOOP******************************************/
#define ECR866x_LOOP_ADDR 0x0020108C // 回环使能地址
/*loop 编号*/
#define LOOP_ALL_CLOSE 0
#define LOOP_ALL_OPEN 1
#define LOOP_CH0_OPEN 2
#define LOOP_CH1_OPEN 3
#define CH0_NCO_EN 4
#define CH1_NCO_EN 5
#define ECR866x_LOOP_0 0x00000000 // 回环关断
#define ECR866x_LOOP_D 0x080D0000 // 回环全部打开
#define ECR866x_LOOP_E 0x080E0000 // CH0回环打开
#define ECR866x_LOOP_F 0x080F0000 // CH1回环打开
#define ECR866x_LOOP_3 0x08030000 // CH1的NCO使能
#define ECR866x_LOOP_4 0x08040000 // CH0的NCO使能
/************************************LOOP_END****************************************/

/************************************默认寄存器配置**********************************/
// 默认写入的寄存器--上述寄存器写完后需要关闭RBDP
#define ECR866x_CLOSE_RBDP_ADDR 0x00201080 // 关闭RBDP 寄存器地址
#define ECR866x_CLOSE_RBDP_DATA 0x00000000
// 默认校准参数
// ch0
#define ECR866x_CH0_TX_CALI_LO_I_ADDR 0x00200380   // 本振校准TX参数I地址
#define ECR866x_CH0_TX_CALI_LO_I_DATA 0x017FC880   // 本振校准TX参数I默认参数
#define ECR866x_CH0_TX_CALI_LO_Q_ADDR 0x00200388   // 本振校准TX参数Q地址
#define ECR866x_CH0_TX_CALI_LO_Q_DATA 0x017F8099   // 本振校准TX参数Q默认参数

#define ECR866x_CH0_RX_CALI_LO_I_ADDR 0x00200540   // 本振校准RX参数I地址
//#define ECR866x_CH0_RX_CALI_LO_I_DATA 0x017FC880   // 本振校准RX参数I默认参数
#define ECR866x_CH0_RX_CALI_LO_Q_ADDR 0x00200544   // 本振校准RX参数Q地址
//#define ECR866x_CH0_RX_CALI_LO_Q_DATA 0x017F8099   // 本振校准RX参数Q默认参数

#define ECR866x_CH0_CALI_MIRROR_ADDR 0x00200D08 // 镜像校准参数地址
#define ECR866x_CH0_CALI_MIRROR_DATA 0xFEEF02B9 // 镜像校准参数默认参数
// ch1
#define ECR866x_CH1_CALI_LO_I_ADDR 0x00200400   // 本振校准参数I地址
#define ECR866x_CH1_CALI_LO_I_DATA 0x017FC880   // 本振校准参数I默认参数
#define ECR866x_CH1_CALI_LO_Q_ADDR 0x00200408   // 本振校准参数Q地址
#define ECR866x_CH1_CALI_LO_Q_DATA 0x017F8099   // 本振校准参数Q默认参数
#define ECR866x_CH1_CALI_MIRROR_ADDR 0x00200F08 // 镜像校准参数地址
#define ECR866x_CH1_CALI_MIRROR_DATA 0xFEEF02B9 // 镜像校准参数默认参数
/************************************默认寄存器END**********************************/

/*************************************RSSIEN****************************************/
// CH0
#define ECR866x_CH0_RSSI_READ_ADDR 0x002009B0 // rssi下行读取地址
#define ECR866x_CH0_RSSI_EN_ADDR0 0x00200938  // CH0 rssi检测功能打开地址0
#define ECR866x_CH0_RSSI_EN_DATA0 0x00001082
#define ECR866x_CH0_RSSI_EN_ADDR1 0x00200960 // CH0 rssi检测功能打开地址1
#define ECR866x_CH0_RSSI_EN_DATA1 0x0000B260
// CH1
#define ECR866x_CH1_RSSI_READ_ADDR 0x00200BB0 // rssi上行读取地址
#define ECR866x_CH1_RSSI_EN_ADDR0 0x00200B38  // CH1 rssi检测功能打开地址0
#define ECR866x_CH1_RSSI_EN_DATA0 0x00001082
#define ECR866x_CH1_RSSI_EN_ADDR1 0x00200B60 // CH1 rssi检测功能打开地址1
#define ECR866x_CH1_RSSI_EN_DATA1 0x0000B260
/************************************RSSI_END***************************************/

/**************************866x频率查询*******************************/
#define ECR_QUERY_FREQ_CH0_RX 0               // CH0-RX 频率查询编号
#define ECR_QUERY_FREQ_CH0_RX_ADDR 0x20004008 // CH0-RX 频率查询地址
#define ECR_QUERY_FREQ_CH1_RX 1               // CH1-RX 频率查询编号
#define ECR_QUERY_FREQ_CH1_RX_ADDR 0x20004010 // CH1-RX 频率查询地址
#define ECR_QUERY_FREQ_CH0_TX 2               // CH0-TX 频率查询编号
#define ECR_QUERY_FREQ_CH0_TX_ADDR 0x20004018 // CH0-TX 频率查询地址
#define ECR_QUERY_FREQ_CH1_TX 3               // CH1-TX 频率查询编号
#define ECR_QUERY_FREQ_CH1_TX_ADDR 0x20004020 // CH1-TX 频率查询地址
/************************866x频率查询_END*****************************/

/**************************866x增益查询*******************************/
#define ECR_QUERY_GAIN_CH0_RX 0               // CH0-RX 增益查询编号
#define ECR_QUERY_GAIN_CH0_RX_ADDR 0x20004028 // CH0-RX 增益查询地址
#define ECR_QUERY_GAIN_CH1_RX 1               // CH1-RX 增益查询编号
#define ECR_QUERY_GAIN_CH1_RX_ADDR 0x2000402C // CH1-RX 增益查询地址
#define ECR_QUERY_GAIN_CH0_TX 2               // CH0-TX 增益查询编号
#define ECR_QUERY_GAIN_CH0_TX_ADDR 0x20004030 // CH0-TX 增益查询地址
#define ECR_QUERY_GAIN_CH1_TX 3               // CH1-TX 增益查询编号
#define ECR_QUERY_GAIN_CH1_TX_ADDR 0x20004034 // CH1-TX 增益查询地址
/************************866x频率查询_END*****************************/

/*********************************本振校准**********************************/
#define ECR_CH0_CAIL_LO_I_ADDR 0x00200380 // CH0本振校准参数I写入地址
#define ECR_CH0_CAIL_LO_Q_ADDR 0x00200388 // CH0本振校准参数Q写入地址
#define ECR_CH1_CAIL_LO_I_ADDR 0x00200400 // CH1本振校准参数I写入地址
#define ECR_CH1_CAIL_LO_Q_ADDR 0x00200408 // CH1本振校准参数Q写入地址
/*******************************本振校准_END********************************/

/*********************************镜像校准**********************************/
#define ECR_CH0_CAIL_MIROR_ADDR 0x00200D08 // CH0本振校准参数I写入地址
#define ECR_CH1_CAIL_MIROR_ADDR 0x00200F08 // CH1本振校准参数Q写入地址
/*******************************镜像校准_END********************************/

/*****************************Rx动态增益配置*********************************/
#define ECR_CH0_DYN_GAIN_ADDR 0x00200880 // CH0动态增益配置地址
#define ECR_CH1_DYN_GAIN_ADDR 0x00200A80 // CH1动态增益配置地址
/***************************Rx动态增益配置_END*******************************/

/*****************************TX_RX通道打开或关闭*********************************/
#define ECR_CH_STATE_ADDR 0x00201180 // 通道状态控制地址
// 通道状态命令编号
#define OPEN_RX_CLOSE_TX_FLAG 0 // 打开RX关闭TX
#define OPEN_TX_CLOSE_RX_FLAG 1 // 打开TX关闭RX
#define CLOSE_TXRX_FLAG 2       // tx rx全部关闭
#define OPEN_TXRX_FLAG 3         // tx rx全部打开
// 通道状态生效寄存器值
#define ECR_OPEN_RX_CLOSE_TX 0x00000010 // 打开RX关闭TX
#define ECR_OPEN_TX_CLOSE_RX 0x10000010 // 打开TX关闭RX
#define ECR_CLOSE_TXRX 0x20000010       // tx rx全部关闭
#define ECR_OPEN_TXRX 0x30000010         // tx rx全部打开
/***********************************TX_RX_END*************************************/

/***************************数字和模拟增益设置地址**************************/
#define ECR_CH0_SZ_GAIN_EN 0x0020090C      // CH0 数字增益使能地址
#define ECR_CH0_RX_SZ_GAIN_ADDR 0x00200920 // CH0 接收数字增益
#define ECR_CH0_RX_MN_GAIN_ADDR 0x00200880 // CH0 接收模拟增益
#define ECR_CH0_TX_SZ_GAIN_ADDR 0x00200D1C // CH0 发送数字增益
#define ECR_CH0_TX_MN_GAIN_ADDR 0x00200330 // CH0 发送模拟增益

#define ECR_CH1_SZ_GAIN_EN 0x00200B0C      // CH1 数字增益使能地址
#define ECR_CH1_RX_SZ_GAIN_ADDR 0x00200B20 // CH1 接收数字增益
#define ECR_CH1_RX_MN_GAIN_ADDR 0x00200A80 // CH1 接收模拟增益
#define ECR_CH1_TX_SZ_GAIN_ADDR 0x00200F1C // CH1 发送数字增益
#define ECR_CH1_TX_MN_GAIN_ADDR 0x00200370 // CH1 发送模拟增益

/**************************************END**************************/

/***************************其他寄存器**************************/
#define ECR_ADC_RESET_RELSASE_ADDR 0x00200728    // ADC 复位释放寄存器地址

#define ECR_ADC_RESET_RELSASE_VAL 0XFFFFFFFE     // ADC 复位释放寄存器默认值

/***********************************END**************************/

#define USER_LOG_BUFF_SIZE (128)                  // 设置输出 LOG 的最大长度 Byte
#define USER_DBG_LEVEL (LOG_MAX_LEVEL)            // 设置输出 LOG 的最大等级
#define USER_ECR8660_CODE_FLASH_ADDR (0x73000000) // 设置存放 ECR8660 代码的首地址
#define CHECK_MCU_WORK_MODE (1)                   // 设置 User_Check_MCU_Work 的模式


void User_Printf(Ecr8660_Log_Level_E type, CHAR *p_fmt, ...);
INT64U User_SPI_Write_Read(trx8668_port_t spi_periph, Spi_Read_Write_E ReadWrite, pGlobal_Spi_Trans_U pdata);
Status_t User_Check_MCU_Work(void);
extern uint8_t RssiCheckFinish;
void HardReset_8668(PORT_T reset_port, GPIO_PIN_T reset_pin); // 8668硬复位
Status_t ecr8668_init(TRX8668_FPGA_t *h8668);
uint32_t HexCharToHex_LSB(char *addrbuf);
uint32_t HexCharToHex_MSB(char *databuf);

void Spi8668Init(void);
int8_t Ecr8668RssiRead(trx8668_port_t spi_periph,uint8_t ch);

Status_t Ecr8668SetAtt(TRX8668_FPGA_t * h8668, int8_t value);

void Ecr866xNcoCfg(TRX8668_FPGA_t *h8668, Ecr866xNco_T tNcoCfg);
void Ecr866xRssiModeCfg(TRX8668_FPGA_t *h8668, Ecr866xRssi_T tRssCfg);
void Ecr866xSetRxFreq(TRX8668_FPGA_t *h8668, uint32_t ch, uint32_t rx_freq);
void Ecr866xSetTxFreq(TRX8668_FPGA_t *h8668, uint32_t ch, uint32_t tx_freq);
void Ecr866xSetRxGain(TRX8668_FPGA_t *h8668, uint32_t ch, int rx_gain);
void Ecr866xSetTxGain(TRX8668_FPGA_t *h8668, uint32_t ch, int tx_gain);
uint32_t Ecr866xGetFreq(TRX8668_FPGA_t *h8668, uint32_t ch);
uint32_t Ecr866xGetGain(TRX8668_FPGA_t *h8668, uint32_t ch);
void BSP_866xCailLoSet(TRX8668_FPGA_t *h8668, uint32_t ch, uint32_t I, uint32_t Q);
void BSP_866xCailMirrorSet(TRX8668_FPGA_t *h8668, uint32_t ch, uint32_t data);
uint8_t Ecr66xGetRssi(TRX8668_FPGA_t *h8668, uint32_t ch);
void Ecr866xSetGain(TRX8668_FPGA_t *h8668, uint32_t ch, int ch_gain);
void Ecr866xSetChClose(TRX8668_FPGA_t *h8668, uint32_t operate);

void Ecr866xBwChoose(TRX8668_FPGA_t *h8668, RF_BAND_E band, uint32_t bw_choose);//不能单独使用
void Ecr866xBwDirectChoose(TRX8668_FPGA_t *h8668, RF_BAND_E band, uint32_t bw_choose);//能单独使用
void Ecr866xChModeCfg(TRX8668_FPGA_t *h8668,uint32_t ch_mode); //不能单独使用

void Ecr866xRxFreqCfg(TRX8668_FPGA_t *h8668, uint32_t ch, uint32_t rx_freq); //频点单位:100kHz
void Ecr866xTxFreqCfg(TRX8668_FPGA_t *h8668, uint32_t ch, uint32_t tx_freq); //频点单位:100kHz

void Ecr866xRxGainCfg(TRX8668_FPGA_t *h8668,uint32_t ch,int rx_gain);
void Ecr866xTxGainCfg(TRX8668_FPGA_t *h8668,uint32_t ch,int tx_gain);
void Ecr866xPortCfg(TRX8668_FPGA_t *h8668,uint32_t port);
void Ecr866xLoopCfg(TRX8668_FPGA_t *h8668,uint32_t loop);
/**********************************************************
 *函数名：   BSP_866x_check_if_warning(TRX8668_FPGA_t * h8668)
 *函数功能： 判断当前是否存在饱和告警,部分寄存器操作在饱和告警时会导致死机
 *返回值：   无
 */
BOOL_E BSP_866x_check_if_warning(TRX8668_FPGA_t * h8668);

void BSP_866xSetOverloadThreshold(TRX8668_FPGA_t * h8668, uint8_t threshold);

#endif
