/*************************************** Copyright ******************************
*
* (C) Copyright 2023, Shaanxi Tianji Communication
* All Rights Reserved
*
* FileName   : bsp_testmode_define.h
* Version    : none
* Author     :
* Date       : 2023-12-13
* Description: none
*******************************************************************************/
//
#ifndef __BSP_TESTMODE_DEFINE_H_
#define __BSP_TESTMODE_DEFINE_H_
#include <stdint.h>


//监控参数 错误代码
#define TM_ERR_OK					0x00	//无错误
#define TM_ERR_IDERR				0x10    //监控数据标识无法识别
#define TM_ERR_SET_VALUE_OVER       0x20	//监控数据的设置值超出范围
#define TM_ERR_WIDTH_ERR			0x30	//监控数据标识与监控数据的值不符合要求
#define TM_ERR_LEN_ERR				0x40	//监控数据标识与监控数据长度不匹配
#define TM_ERR_VALUE_L				0x50	//监控数据的检测值低于工作范围,显示"--"
#define TM_ERR_VALUE_H				0x60	//监控数据的检测值高于工作范围,显示"++"
#define TM_ERR_DATA_INVALID		    0x70	//监控数据无法检测,显示"**"
#define TM_ERR_OTHER				0x90	//未列出的其他错误
#define TM_ERR_ZDY					0xA0	//A~F厂家自定义
#define TM_ERR_NOACT                0xB0    //设备无应答
#define TM_ERR_LOAD_FAULT			0xC0	//加载失败


//监控项错误代码
#define TM_CMD_ACT_OK			    0x00	//成功
#define TM_CMD_ACT_CONDITION	    0x01	//命令被有条件执行
#define TM_CMD_ACT_ID_ERROR	        0x02	//命令编号错
#define TM_CMD_ACT_LEN_ERROR	    0x03	//长度错
#define TM_CMD_ACT_CRC_ERROR	    0x04	//CRC校验错
#define TM_CMD_ACT_DEV_NON_EXIST	0xFC	//设备不存在
#define TM_CMD_ACT_OTHER		    0xFE	//其它错误
#define TM_CMD_ACT_SEND		        0xFF	//命令发送


#define TMDP_SEOP_BYTE		'!'		//起始字节
#define TMDP_AP				0x01
#define TMDP_NC				0x01


typedef enum
{
    TMDP_DIRECTION_RX = 0x00,
    TMDP_DIRECTION_TX = 0x80
} TMDP_DIRECTION_E;


//ACK应答
typedef enum
{
    TM_ACK_RX = 0x00,
    TM_ACK_TX = 0xFF
} TMDP_ACK_E;


//指令类型
typedef enum
{
    TM_CMD_RPT   = 0x01,
    TM_CMD_QUERY = 0x02,
    TM_CMD_SET	 = 0x03
} TM_CMD_TYPE_E;


//模块类型
typedef enum
{
    TM_MODEL_PUBLIC 	= 0x00,
    TM_MODEL_MONITOR	= 0x01,
    TM_MODEL_4G_MODEM	= 0x10,
    TM_MODEL_5G_MODEM	= 0x11,
    TM_MODEL_BLE		= 0x12,
    TM_MODEL_FPGA		= 0x13,
    TM_MODEL_HUB		= 0x14,
    TM_MODEL_BAND3		= 0x20,
    TM_MODEL_BAND8		= 0x21,
    TM_MODEL_BAND39		= 0x22,
    TM_MODEL_BAND40		= 0x23,
    TM_MODEL_BAND41		= 0x24
} TM_MODEL_E;

//读写属性
typedef enum
{
    TM_R = 0,	//只读
    TM_W, 		//只写
    TM_RW		//可读可写
} TM_RW_E;

//数据类型
typedef enum
{
    TM_BOOL = 0,
    TM_UINT1,
    TM_SINT1,
    TM_UINT2,
    TM_SINT2,
    TM_UINT4,
    TM_SINT4,
    TM_STR,
    TM_DATA_TYPE_END
} TM_DATA_TYPE_E;

//校准功能ID
typedef enum
{
    TM_FUNC_ADC = 0,
    TM_FUNC_ATT,
    TM_FUNC_CALI,
    TM_FUNC_8668,
} TM_FUNC_TYPE_E;

typedef struct
{
    uint8_t * pt_mode;	//运行模式
    uint8_t sys;		//系统
    uint8_t func_id;	//功能id
    uint8_t ch;			//上下行
    uint8_t band;		//频段
    uint8_t bw;			//带宽
    uint8_t group;		//组别
    uint8_t num;		//编号
} TEST_MODE_PARA_T;


#pragma pack(1)
typedef struct
{
    uint16_t len;
    uint16_t oid;
    uint8_t  model;
    uint8_t  sys;
    uint8_t  dat;   // 取第一个有效数据
} tm_oid_data_t;

typedef struct
{
    uint8_t  		sop;		//起始, byte1, "!"
    uint16_t 		len;		//长度, 从sop到eop,, byte2~3
    uint8_t  		ap;			//ap,填充 0x01, byte4
    uint8_t  		nc1;		//填充 0x01, byte5
    uint8_t  		dev_id;		//设备编号, 单设备 0xFF, 主机0x00, 从机0x01~0xFE, byte6
    uint16_t 		pkg_id;		//包编号, byte7 ~ byte8
    uint8_t  		direction;	//传输方向, 发送: 0x80, 接收: 0x00 byte9
    uint8_t  		cmd;		//命令类型, 上报: 0x01, 查询: 0x02, 设置: 0x03, byte10
    uint8_t  		ack;		//应答: 发起方: 0xFF, 应答方: 正确0x00, 或者错误码, byte11
    tm_oid_data_t 	tpt;		//首个数据指针, 始于byte12
} tm_msg_t;
#pragma pack()

typedef struct
{
    uint8_t  unsave;            //不保存
    uint8_t  dev;               //设备公共出厂参数保存标识
    uint8_t  factory;           //校准公共出厂参数保存标识
    uint8_t	 cali_rf;           //rf参数保存标识
    uint8_t	 cali_8668;         //8668参数保存标识
} tm_save_flag_t;

#endif





