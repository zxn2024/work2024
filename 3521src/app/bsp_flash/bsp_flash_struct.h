/*************************************** Copyright ******************************
*
* (C) Copyright 2023, Shaanxi Tianji Communication
* All Rights Reserved
*
* FileName   : bsp_flash_struct.h
* Version    : none
* Author     :
* Date       : 2023-12-21
* Description: none
*******************************************************************************/
//
#ifndef __BSP_FLASH_STRUCT_H_
#define __BSP_FLASH_STRUCT_H_
#include <stdint.h>
#include "system_config.h"
#include "bsp_rf_4g_band_sel.h"
#include "bsp_rf_pow.h"

/*********************** 正常保存参数 *************************/
#pragma pack(1) // 1字节对齐
typedef struct
{
    uint8_t  work_mode;			//工作模式
    uint8_t  slave_dev_num;		//从机数量
    uint8_t  rst_time_by_net;	//根据网络定时重启时间
    uint8_t  rst_by_run_time;	//重启间隔
    uint8_t	 led_plan;			//指示灯方案
    uint8_t  led_switch;		//指示灯开关
    int8_t   tmp_offset;		//温度校准值
	uint8_t  warn_en;			//告警使能
	uint8_t  warn_save_en;		//是否保存告警状态
    uint8_t  reserve[55];
} para_dev_t;


typedef struct
{
    uint8_t  factory_id[20];    //厂商代码
    uint8_t  dev_type;			//设备类型
    uint8_t  dev_model[20];		//设备型号
	uint8_t  sn[32];
    uint8_t  reserve[23];
} para_fact_t;

typedef struct
{
    int8_t   tmp_offset;	//温度校准值
    uint8_t  warn_en;		//允许告警使能(通道)
    uint8_t  oplc_en;		//开环工控使能
    uint8_t  rsrp_en;		//rsrp使能
    uint8_t  tmp_en;		//温补使能
    uint8_t  silence_en;	//静默使能
    uint8_t  iso_en;		//隔离度检测使能
    uint8_t  rf_switch;		//射频开关
    uint8_t  reserve[8];
} para_rf_sys_t;



//typedef struct
//{
//    int8_t  up_gain_offset;
//    int8_t  dw_gain_offset;
//    int8_t  up_pow_offset;
//    int8_t  dw_pow_offset;
//    int8_t  oplc_gate;
//    int8_t  oplc_gain_offset;
//    int8_t  oplc_pow_offset;
//	uint8_t scan_freq[64];
//} para_rf_ch_t;


//告警参数
typedef struct
{
    uint8_t     en;        //告警总开关
    int8_t      gate;      //开环工控使能
} para_warn_t;

typedef struct
{
	para_warn_t up_inpow_less;
	para_warn_t up_inpow_greater;
	para_warn_t up_outpow_less;
	para_warn_t up_outpow_greater;
	para_warn_t up_iso;
	para_warn_t dw_inpow_less;
	para_warn_t dw_inpow_greater;
	para_warn_t dw_outpow_less;
	para_warn_t dw_outpow_greater;
	para_warn_t dw_iso;
	para_warn_t tmp_greater;
	para_warn_t other;
}para_warn_list_t;

typedef struct
{
	para_warn_list_t mu;
    para_warn_list_t ru;
} para_warn_comb_t;


typedef struct
{
    uint16_t         head;
    para_dev_t  	 t_dev;
    para_fact_t 	 t_fact;
    omc_t       	 t_omc;
	NetConfig_T      t_net_config;			//网络参数配置
    para_rf_sys_t 	 t_sys_pub[RF_SYS_NUM];	
    para_rf_ch_t  	 t_rf_ch_para[6];			//各频段射频参数
    para_warn_comb_t t_warn[RF_SYS_NUM];	//主机告警使能和门限
    uint32_t		 crc;
} flash_dev_para_t;
#pragma pack()



/*********************** 测试模式校准参数 *************************/

#pragma pack(1) // 1字节对齐

typedef struct
{
    int8_t  coupling_gain;		//耦合增益
	int8_t  up_rssi_gate;       //上行静默门限
    int8_t  reserve1[14];
    int8_t  up_theo_gain;		//上行理论增益
    int8_t  up_theo_out_pow;	//上行理论输出功率
    int8_t  up_lna_att1;		//上行LNA1抵消ATT数值
    int8_t  up_lna_att2;		//上行LNA2抵消ATT数值
    int8_t  up_gain_cali;		//上行增益校准
    uint8_t reserve2[18];
    int8_t  dw_theo_gain;		//下行理论增益
    int8_t  dw_theo_out_pow;	//下行理论输出功率
    int8_t  dw_lna_att1;		//下行LNA1抵消ATT数值
    int8_t  dw_lna_att2;		//下行LNA2抵消ATT数值
    int8_t  dw_gain_cali;		//上行增益校准
    uint8_t reserve3[20];
    int8_t  up_tc_table[32];	//上行温补表
    int8_t  dw_tc_table[32];	//下行温补表
    uint8_t reserve4[128];
} tm_cali_rf_para;

typedef struct
{
	int8_t   min_value;
	int8_t   cali_value;
	uint8_t  valid_num;
	uint8_t  step;
	uint16_t data[94];
}flash_pow_att_t;

typedef struct
{
//    uint8_t  up1[192];
//    uint8_t  up2[192];
//    uint8_t  dw1[192];
//    uint8_t  dw2[192];
	flash_pow_att_t up1;
	flash_pow_att_t up2;
	flash_pow_att_t dw1;
	flash_pow_att_t dw2;
} tm_cali_rf_table;


typedef struct
{
    uint32_t  image_cali;		//镜像校准
    uint32_t  tx_lo_i;			//TX-I
    uint32_t  tx_lo_q;			//TX-Q
    uint32_t  rx_lo_i;
    uint32_t  rx_lo_q;
    uint32_t  normal_lo_i;
    uint32_t  normal_lo_q;
    uint32_t  block_lo_i;
    uint32_t  block_lo_q;
} tm_cali_8668;


typedef struct
{
    uint16_t start;
    uint8_t  dev_type[48];
    uint8_t  sn[48];
    uint8_t  sw_ver[48];
    uint8_t  hw_ver[48];
    uint8_t  mcu_type;
    uint8_t  modem_4g;	//4G modem 型号
    uint8_t  modem_5g;	//5G modem 型号
    uint8_t  fpga;
    uint8_t  cali_end_flag;
    uint8_t  license[64];
    uint8_t  reserve[41];
} tm_flash_factory;


typedef struct
{
    tm_cali_rf_table pow_table[4];
    tm_cali_rf_table att_table;
    tm_cali_rf_para  rf;
} tm_flash_rf;


typedef struct
{
    uint32_t     gain_table[52][4];
    uint32_t     block_gain_table[52][4];
    tm_cali_8668 cali[5];
    int8_t       gain_init;
    uint8_t      reserve[63];
} tm_flash_8668;

#pragma pack()

#endif





