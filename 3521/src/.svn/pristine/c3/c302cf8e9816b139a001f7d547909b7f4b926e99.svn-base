/**
  ******************************************************************************
  * @file    fpga.h
  * @author  
  * @version V1.0.0
  * @date    2023-10-14
  * @brief   This file provides code for the fpga interface.
  ******************************************************************************
*/

#ifndef FPGA_H
#define FPGA_H

#include "MCU_config.h"


/*---------------------- 宏定义  ---------------------------------------*/
#define FPGA_OP_DEVICE_REG     0
#define FPGA_OP_DEVICE_OOK     1
#define FPGA_OP_DEVICE_TRX_4G  2   
#define FPGA_OP_DEVICE_TRX_5G1 3
#define FPGA_OP_DEVICE_TRX_5G2 4

#define FPGA_OP_MODE_READ  0
#define FPGA_OP_MODE_WRITE 1

#define FPGA_OP_SUCCESS  1
#define FPGA_OP_FAIL     0xFFFFFFFF

#define FPGA_REG_BASE_PUBLIC 0x1000/*寄存器基地址*/
#define FPGA_REG_BASE_SPI    0x2000/*寄存器基地址*/
#define FPGA_REG_BASE_OOK    0x3000/*寄存器基地址*/
#define FPGA_REG_BASE_CLOCK  0x4000/*寄存器基地址*/
#define FPGA_REG_BASE_MIPI   0x5000/*寄存器基地址*/

/**/
#define FPGA_REG_STATE    FPGA_REG_BASE_PUBLIC+0x0/*寄存器 状态*/
#define FPGA_REG_VER_TIME FPGA_REG_BASE_PUBLIC+0x1/*寄存器 版本时间*/
#define FPGA_REG_VER_NUM  FPGA_REG_BASE_PUBLIC+0x2/*寄存器 版本号*/

/**/
#define FPGA_REG_OOK_DATA      FPGA_REG_BASE_SPI+0x0/*寄存器 OOK数据寄存器*/
#define FPGA_REG_TRX_4G_DATA   FPGA_REG_BASE_SPI+0x1/*寄存器 TRX 4G数据寄存器*/
#define FPGA_REG_TRX_5G1_DATA  FPGA_REG_BASE_SPI+0x2/*寄存器 TRX 5G1数据寄存器*/
#define FPGA_REG_TRX_5G2_DATA  FPGA_REG_BASE_SPI+0x3/*寄存器 TRX 5G2数据寄存器*/
#define FPGA_REG_TRX_4G_GAIN   FPGA_REG_BASE_SPI+0x5/*寄存器 TRX 4G增益寄存器*/
#define FPGA_REG_TRX_5G1_GAIN  FPGA_REG_BASE_SPI+0x6/*寄存器 TRX 5G1增益寄存器*/
#define FPGA_REG_TRX_5G2_GAIN  FPGA_REG_BASE_SPI+0x7/*寄存器 TRX 5G2增益寄存器*/
#define FPGA_REG_TRX_4G_RSSI   FPGA_REG_BASE_SPI+0x8/*寄存器 TRX 4G RSSI寄存器*/
#define FPGA_REG_TRX_5G1_RSSI  FPGA_REG_BASE_SPI+0x9/*寄存器 TRX 5G1 RSSI寄存器*/
#define FPGA_REG_TRX_5G2_RSSI  FPGA_REG_BASE_SPI+0xA/*寄存器 TRX 5G2 RSSI寄存器*/
#define FPGA_REG_4G_UL_SILENT  FPGA_REG_BASE_SPI+0x10/*寄存器 4G上行静默关PA寄存器*/

/**/
#define FPGA_REG_OOK_RESET     FPGA_REG_BASE_OOK+0x0/*寄存器 OOK复位寄存器*/
#define FPGA_REG_OOK_RECV      FPGA_REG_BASE_OOK+0x1/*寄存器 OOK接收标志寄存器*/
#define FPGA_REG_4G_BAND       FPGA_REG_BASE_OOK+0x10/*寄存器 4G频段寄存器*/
#define FPGA_REG_5G_FRAME      FPGA_REG_BASE_OOK+0x11/*寄存器 5G子帧配比寄存器*/
#define FPGA_REG_5G_FRAME2     FPGA_REG_BASE_OOK+0x12/*寄存器 5G特殊子帧配比寄存器*/
#define FPGA_REG_4G_FRAME      FPGA_REG_BASE_OOK+0x13/*寄存器 4G子帧配比寄存器*/
#define FPGA_REG_4G_FRAME2     FPGA_REG_BASE_OOK+0x14/*寄存器 4G特殊子帧配比寄存器*/
#define FPGA_REG_4G_FREQ       FPGA_REG_BASE_OOK+0x15/*寄存器 4G中心频点*/
#define FPGA_REG_4G_BW         FPGA_REG_BASE_OOK+0x16/*寄存器 4G带宽*/
#define FPGA_REG_FRAME_TYPE    FPGA_REG_BASE_OOK+0x17/*寄存器 传输帧头类型*/
#define FPGA_REG_4G_FRAME_DEV  FPGA_REG_BASE_OOK+0x18/*寄存器 4G帧头偏差*/
#define FPGA_REG_5G_BW         FPGA_REG_BASE_OOK+0x19/*寄存器 5G带宽*/
#define FPGA_REG_5G_FREQ       FPGA_REG_BASE_OOK+0x1A/*寄存器 5G中心频点*/

/**/
#define FPGA_REG_CLK_VLD       FPGA_REG_BASE_CLOCK+0x0/*寄存器 参考信号有效寄存器*/
#define FPGA_REG_MEA_VLD       FPGA_REG_BASE_CLOCK+0x1/*寄存器 时钟测量有效寄存器*/
#define FPGA_REG_MEA_DAT       FPGA_REG_BASE_CLOCK+0x2/*寄存器 时钟测量寄存器*/
#define FPGA_REG_PWM           FPGA_REG_BASE_CLOCK+0x4/*寄存器 PWM寄存器（占空比）*/

/**/
#define FPGA_REG_FX5724_0      FPGA_REG_BASE_MIPI+0x0/*寄存器 FX5724寄存器0*/
#define FPGA_REG_FX5724_1      FPGA_REG_BASE_MIPI+0x1/*寄存器 FX5724寄存器1*/
#define FPGA_REG_4G_DL_INI_1   FPGA_REG_BASE_MIPI+0x11/*寄存器 4G DL初始化寄存器1*/
#define FPGA_REG_4G_DL_INI_2   FPGA_REG_BASE_MIPI+0x12/*寄存器 4G DL初始化寄存器2*/
#define FPGA_REG_4G_DL_INI_3   FPGA_REG_BASE_MIPI+0x13/*寄存器 4G DL初始化寄存器3*/
#define FPGA_REG_4G_DL_INI_4   FPGA_REG_BASE_MIPI+0x14/*寄存器 4G DL初始化寄存器4*/
#define FPGA_REG_4G_DL_INI_5   FPGA_REG_BASE_MIPI+0x15/*寄存器 4G DL初始化寄存器5*/
#define FPGA_REG_4G_DL_INI_6   FPGA_REG_BASE_MIPI+0x16/*寄存器 4G DL初始化寄存器6*/
#define FPGA_REG_4G_DL_INI_7   FPGA_REG_BASE_MIPI+0x17/*寄存器 4G DL初始化寄存器7*/
#define FPGA_REG_4G_DL_OPEN    FPGA_REG_BASE_MIPI+0x19/*寄存器 4G DL打开寄存器*/
#define FPGA_REG_4G_DL_CLOSE   FPGA_REG_BASE_MIPI+0x1C/*寄存器 4G DL关闭寄存器*/
#define FPGA_REG_4G_UL_INI_1   FPGA_REG_BASE_MIPI+0x21/*寄存器 4G UL初始化寄存器1*/
#define FPGA_REG_4G_UL_INI_2   FPGA_REG_BASE_MIPI+0x22/*寄存器 4G UL初始化寄存器2*/
#define FPGA_REG_4G_UL_INI_3   FPGA_REG_BASE_MIPI+0x23/*寄存器 4G UL初始化寄存器3*/
#define FPGA_REG_4G_UL_INI_4   FPGA_REG_BASE_MIPI+0x24/*寄存器 4G UL初始化寄存器4*/
#define FPGA_REG_4G_UL_INI_5   FPGA_REG_BASE_MIPI+0x25/*寄存器 4G UL初始化寄存器5*/
#define FPGA_REG_4G_UL_INI_6   FPGA_REG_BASE_MIPI+0x26/*寄存器 4G UL初始化寄存器6*/
#define FPGA_REG_4G_UL_INI_7   FPGA_REG_BASE_MIPI+0x27/*寄存器 4G UL初始化寄存器7*/
#define FPGA_REG_4G_UL_OPEN    FPGA_REG_BASE_MIPI+0x29/*寄存器 4G UL打开寄存器*/
#define FPGA_REG_4G_UL_CLOSE   FPGA_REG_BASE_MIPI+0x2C/*寄存器 4G UL关闭寄存器*/

/*4G频段定义*/
#define FREQ_BAND_B3_1800M  0
#define FREQ_BAND_B8_900M   1
#define FREQ_BAND_B39_F     2
#define FREQ_BAND_B40_E     3

/*---------------------- 结构体 ------------------------------------------*/
/*FPGA驱动资源定义*/
typedef struct __FPGA_config_t
{
    PORT_T dc_ctrl_port;       /*3.3v ctrl port*/ 
    GPIO_PIN_T dc_ctrl_pin;    /*3.3v ctrl pin*/ 	
	
    PORT_T ps_nconfig_port;    /*ps nconfig port*/ 
    GPIO_PIN_T ps_nconfig_pin; /*ps nconfig pin*/ 	
	
    PORT_T ps_nstatus_port;    /*ps nstatus port*/ 
    GPIO_PIN_T ps_nstatus_pin; /*ps nstatus pin*/ 	
	
    PORT_T ps_cs_port;         /*ps cs port*/ 
    GPIO_PIN_T ps_cs_pin;      /*ps cs pin*/     	

    PORT_T ps_dclk_port;        /*ps dclk port*/ 
    GPIO_PIN_T ps_dclk_pin;     /*ps dclk pin*/  	
	
    PORT_T ps_data_port;        /*ps data port*/ 
    GPIO_PIN_T ps_data_pin;     /*ps data pin*/ 
	
    PORT_T rst_n_port;            /*rst_n port*/ 
    GPIO_PIN_T rst_n_pin;         /*rst_n pin*/ 
	
    PORT_T spi_cs_port;         /*SPI cs port*/ 
    GPIO_PIN_T spi_cs_pin;      /*SPI cs pin*/ 

    SPI_CH_T spi;               /*SPI*/
} FPGA_config_t;

/*---------------------- 接口 ------------------------------------------*/
/*
**********************************************************
*函数功能: fpga_ver_time
*参数说明: 无
*返 回 值: fpga版本时间
**********************************************************
*/
uint32_t fpga_ver_time(void);

/*
**********************************************************
*函数功能: fpga_ver_num
*参数说明: 无
*返 回 值: fpga版本号
**********************************************************
*/
uint32_t fpga_ver_num(void);

/*
**********************************************************
*函数功能: fpga_spi_write FPGA spi写入  在任务中调用
*参数说明: op_type 操作类型 0寄存器 1 ook 2~4 ECR8668
           pbuffer 发送缓冲区 当type=0时 6个字节 前两个字节是地址，后四个字节是数据
                              当type=1时 ook地址+数据
                              当type=2~4时 8668地址+数据
					 buffer_size 发送缓冲区长度 				 
*返 回 值: 1成功
**********************************************************
*/
uint32_t fpga_spi_write(uint8_t op_type, uint8_t *pbuffer, uint8_t buffer_size);

/*
**********************************************************
*函数功能: fpga_spi_read  FPGA spi读  在任务中调用
*参数说明: op_type 操作类型 0寄存器 1 ook 2~4 ECR8668
           pbuffer 发送缓冲区 当type=0时 2个字节 两个字节的地址
                              当type=1时 ook地址
                              当type=2~4时 8668地址
					 buffer_size 发送缓冲区长度 															
*返 回 值: 读到的结果
**********************************************************
*/
uint32_t fpga_spi_read(uint8_t op_type, uint8_t *pbuffer, uint8_t buffer_size);

/*
**********************************************************
*函数功能: fpga_reg_write FPGA 寄存器写入  在任务中调用
*参数说明: reg_addr 寄存器地址
           data 写入的数据
*返 回 值: 1成功
**********************************************************
*/
uint32_t fpga_reg_write(uint16_t reg_addr, uint32_t data);

/*
**********************************************************
*函数功能: fpga_reg_read  FPGA 寄存器读  在任务中调用
*参数说明: reg_addr 寄存器地址
*返 回 值: 读到的结果
**********************************************************
*/
uint32_t fpga_reg_read(uint16_t reg_addr);

/*
**********************************************************
*函数功能: fpga_ook_hop_freq ook跳频
*参数说明: 无
*返 回 值: 1成功
**********************************************************
*/
uint8_t fpga_ook_hop_freq(void);

/*
**********************************************************
*函数功能: fpga_get_state 获取fpga的状态
*参数说明: 无
*返 回 值: 1状态正常
**********************************************************
*/
uint8_t fpga_get_state(void);

/*
**********************************************************
*函数功能: fpga_set_4g_freqband 设置4G频段 并打开pa 在任务中调用
*参数说明: fb：0 1800M    1 900M    2 F频段    3 E频段
*返 回 值: 1成功
**********************************************************
*/
uint8_t fpga_set_4g_freqband(uint8_t fb);
/*
**********************************************************
*函数功能: fpga_4g_dl_pa_on 打开4G DL PA
*参数说明: 无
*返 回 值: 无
**********************************************************
*/
void fpga_4g_dl_pa_on(void);
/*
**********************************************************
*函数功能: fpga_4g_dl_pa_off 关闭4G DL PA
*参数说明: 无
*返 回 值: 无
**********************************************************
*/
void fpga_4g_dl_pa_off(void);
/*
**********************************************************
*函数功能: fpga_4g_ul_pa_on 打开4G UL PA
*参数说明: 无
*返 回 值: 无
**********************************************************
*/
void fpga_4g_ul_pa_on(void);
/*
**********************************************************
*函数功能: fpga_4g_ul_pa_off 关闭4G UL PA
*参数说明: 无
*返 回 值: 无
**********************************************************
*/
void fpga_4g_ul_pa_off(void);
/*
**********************************************************
*函数功能: fpga_get_4g_freqband 获取4G频段  在任务中调用
*参数说明: 无
*返 回 值: fb：0 1800M    1 900M    2 F频段    3 E频段    其它 无效
**********************************************************
*/
uint8_t fpga_get_4g_freqband(void);

/*
**********************************************************
*函数功能: fpga_set_5g_frame 设置5G帧格式  在任务中调用
*参数说明: pf 帧格式字符串 
							5ms单周期 如配比02:07    特殊04:04:06 表示为 "02070000040406" 
							5ms双周期 如配比1:3、2:2 特殊02:02:10 表示为 "01030202020210" 
*返 回 值: 1成功
**********************************************************
*/
uint8_t fpga_set_5g_frame(char *pf);

/*
**********************************************************
*函数功能: fpga_get_5g_frame 获取5G帧格式  在任务中调用
*参数说明: pf 帧格式输出字符串 
							5ms单周期 如配比02:07    特殊04:04:06 表示为 "02070000040406" 
							5ms双周期 如配比1:3、2:2 特殊02:02:10 表示为 "01030202020210" 
*返 回 值: 1成功
**********************************************************
*/
uint8_t fpga_get_5g_frame(char *pf);

/*
**********************************************************
*函数功能: fpga_set_4g_subframe 设置4G子帧配比  在任务中调用
*参数说明: subframe 0~6 
*返 回 值: 1成功
**********************************************************
*/
uint8_t fpga_set_4g_subframe(uint8_t subframe);

/*
**********************************************************
*函数功能: fpga_get_4g_subframe 获取4G子帧配比  在任务中调用
*参数说明: 无
*返 回 值: 0~6  其它无效
**********************************************************
*/
uint8_t fpga_get_4g_subframe(void);

/*
**********************************************************
*函数功能: fpga_set_4g_specical_subframe 设置4G特殊子帧配比  在任务中调用
*参数说明: specical_subframe 0~8 
*返 回 值: 1成功
**********************************************************
*/
uint8_t fpga_set_4g_specical_subframe(uint8_t specical_subframe);

/*
**********************************************************
*函数功能: fpga_get_4g_specical_subframe 获取4G特殊子帧配比  在任务中调用
*参数说明: 无
*返 回 值: 0~8  其它无效
**********************************************************
*/
uint8_t fpga_get_4g_specical_subframe(void);

/*
**********************************************************
*函数功能: fpga_set_4g_center_freq 设置4G中心频点  在任务中调用
*参数说明: center_freq  
*返 回 值: 1成功
**********************************************************
*/
uint8_t fpga_set_4g_center_freq(uint32_t center_freq);

/*
**********************************************************
*函数功能: fpga_get_4g_center_freq 获取4G中心频点  在任务中调用
*参数说明: 无
*返 回 值: 4G中心频点  0xFFFFFFFF无效
**********************************************************
*/
uint32_t fpga_get_4g_center_freq(void);

/*
**********************************************************
*函数功能: fpga_set_4g_bandwidth 设置4G带宽  在任务中调用
*参数说明: bandwidth  
*返 回 值: 1成功
**********************************************************
*/
uint8_t fpga_set_4g_bandwidth(uint32_t bandwidth);

/*
**********************************************************
*函数功能: fpga_get_4g_bandwidth 获取4G带宽  在任务中调用
*参数说明: 无
*返 回 值: 4G带宽  0xFFFFFFFF无效
**********************************************************
*/
uint32_t fpga_get_4g_bandwidth(void);
/*
**********************************************************
*函数功能: fpga_set_5g_center_freq 设置5G中心频点  在任务中调用
*参数说明: center_freq  
*返 回 值: 1成功
**********************************************************
*/
uint8_t fpga_set_5g_center_freq(uint32_t center_freq);

/*
**********************************************************
*函数功能: fpga_get_5g_center_freq 获取5G中心频点  在任务中调用
*参数说明: 无
*返 回 值: 5G中心频点  0xFFFFFFFF无效
**********************************************************
*/
uint32_t fpga_get_5g_center_freq(void);
/*
**********************************************************
*函数功能: fpga_set_5g_bandwidth 设置5G带宽  在任务中调用
*参数说明: bandwidth  
*返 回 值: 1成功
**********************************************************
*/
uint8_t fpga_set_5g_bandwidth(uint32_t bandwidth);

/*
**********************************************************
*函数功能: fpga_get_5g_bandwidth 获取5G带宽  在任务中调用
*参数说明: 无
*返 回 值: 4G带宽  0xFFFFFFFF无效
**********************************************************
*/
uint32_t fpga_get_5g_bandwidth(void);

/*
**********************************************************
*函数功能: fpga_get_frame_type 获取传输帧头的类型
*参数说明: 无
*返 回 值: 0:5G DL  1:4G DL  2:内部5ms
**********************************************************
*/
uint8_t fpga_get_frame_type(void);

/*
**********************************************************
*函数功能: fpga_get_4g_frame_dev 获取4G帧偏差
*参数说明: 无
*返 回 值: dev
**********************************************************
*/
uint32_t fpga_get_4g_frame_dev(void);
/*
**********************************************************
*函数功能: fpga_ook_state 获取ook的状态
*参数说明: 无
*返 回 值: 1正常
**********************************************************
*/
uint8_t fpga_ook_state(void);
/*
**********************************************************
*函数功能: fpga_ook_off ook关电
*参数说明: 无
*返 回 值: 无
**********************************************************
*/
void fpga_ook_off(void);

/*
**********************************************************
*函数功能: fpga_ook_off ook开电
*参数说明: 无
*返 回 值: 无
**********************************************************
*/
void fpga_ook_on(void);

/*
**********************************************************
*函数功能: fpga_get_trx_rssi 获取TRX RSSI  在任务中调用
*参数说明: op_type: FPGA_OP_DEVICE_TRX_4G
                    FPGA_OP_DEVICE_TRX_5G1
                    FPGA_OP_DEVICE_TRX_5G2
*返 回 值: RSSI  
**********************************************************
*/
int8_t fpga_get_trx_rssi(uint8_t op_type);

/*
**********************************************************
*函数功能: fpga_set_clock_pwm
*参数说明: pwm
*返 回 值: 无
**********************************************************
*/
void fpga_set_clock_pwm(int32_t pwm);
/*
**********************************************************
*函数功能: fpga_get_clock_pwm
*参数说明: 无
*返 回 值: pwm
**********************************************************
*/
int32_t fpga_get_clock_pwm(void);
/*
**********************************************************
*函数功能: fpga 初始化 启动FPGA任务 开始spi初始化 状态检测
*参数说明: 无
*返 回 值: 无
**********************************************************
*/
void fpga_task_init(void);

/*
**********************************************************
*函数功能: fpga ps加载
*参数说明: 无
*返 回 值: 无
**********************************************************
*/
void fpga_ps_load(void);

/*
**********************************************************
*函数功能: fpga_register 器件注册,结构体初始化
*参数说明: p_fpga_config  传入器件参数
*返 回 值: STAT_OK
**********************************************************
*/ 
Status_t fpga_register(FPGA_config_t * p_fpga_config);

uint32_t OOK_GetFrequency(void);
int8_t fpga_get_clock_meaVld(void);
int32_t fpga_get_clock_meaRes(void);
#endif /* FPGA_SPI_H */
