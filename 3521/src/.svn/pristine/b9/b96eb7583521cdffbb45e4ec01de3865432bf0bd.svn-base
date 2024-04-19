/**
 * **********************************************************************************
 * @copyright Copyright (c) 2023 Shaanxi Tianji Communication Technologies Co., Ltd.
 *                         
 * @file        public_enum.h   
 * @author      
 * @version     None
 * @date        2023-12-04     
 * @brief       None   
 * 
 * **********************************************************************************
 */

#ifndef __PUBLIC_ENUM_H_
#define __PUBLIC_ENUM_H_

#include <stdint.h>

#ifdef __cplusplus
 extern "C" {
#endif

/****************************** 数据类型定义 ******************************/

typedef uint8_t Bool_t;
typedef int32_t Handle_t;


/*---------------------- 通    用 ----------------------*/
typedef enum
{
	E_DISABLE = 0,
	E_ENABLE  = !E_DISABLE
}ENABLE_DISABLE_E;


typedef enum
{
    CLOSE = 0,              /* 关闭 */
    OPEN = !CLOSE,          /* 打开 */
}OPEN_CLOSE_E;


typedef enum
{
    LEVEL_L = 0,            /* 低电平 */
    LEVEL_H = !LEVEL_L      /* 高电平 */
}ELEC_LEVEL_E;


typedef enum
{
    B_FALSE = 0,            /* 真 / 正确 */
    B_TRUE = !B_FALSE,      /* 假 / 错误 */
}BOOL_E;


typedef enum 
{
    STAT_ERROR    = 0x00U,
    STAT_OK       = 0x01U,
    STAT_BUSY     = 0x02U,
    STAT_tIMEOUT  = 0x03U
} Status_t;


/*---------------------- Flash相关 ----------------------*/
typedef enum
{
    READ = 0,               /* 读 */
    WRITE,                  /* 写 */
}READ_WRITE_E;

typedef enum
{
    FLASH_RESTORY_FAC = 1,  /* 恢复出厂设置 */
    FLASH_ERASE_ALL,        /* 擦除所有 */
}FLASH_OPERATE_E;


/*---------------------- Modem相关 ----------------------*/
typedef enum
{
    UNLOCK = 0,             /* 未锁定 */
    LOCK = !UNLOCK,         /* 锁定 */
}LOCK_STATE_E;

typedef enum
{
    UNSYNC = 0,             /* 未同步 */
    SYNC = !UNSYNC,         /* 同步 */
}SYNC_STATE_E;


/*---------------------- RF相关 ----------------------*/
typedef enum
{
	RF_MODE_START = 0,      /* 开机（搜网）模式 */
	RF_MODE_NORMAL,         /* 正常模式 */
    RF_MODE_SINGLE,         /* 单通模式 */
	RF_MODE_COLLECTION,     /* 采集模式 */
}RF_MODE_E;

typedef enum
{
    RF_CH_UP = 0,           /* 上行 */
    RF_CH_DW,               /* 下行 */
}RF_CH_E;

//系统编号
typedef enum
{
    RF_SYS_1 = 0,           /* SYS1系统 */
    RF_SYS_2,               /* SYS2系统 */
    RF_SYS_3,               /* SYS3系统 */
    RF_SYS_4,               /* SYS4系统 */
    RF_SYS_5,               /* SYS5系统 */
    RF_SYS_6,               /* SYS6系统 */
    RF_SYS_7,               /* SYS7系统 */
    RF_SYS_8,               /* SYS8系统 */
}RF_SYS_E;


typedef enum
{
    RF_SWITCH_CLOSE = 0,    /* 关闭 */
    RF_SWITCH_OPEN,         /* 开启 */
    RF_SWITCH_UP,           /* 上行单通 */
    RF_SWITCH_DW,           /* 下行单通 */
}RF_SWITCH_E;


/*---------------------- 告警相关 ----------------------*/
typedef enum
{
    NORMAL = 0,             /* 正常 */
    WARN = !NORMAL          /* 告警 */
}WARN_STATE_E;

typedef enum
{
    RPT_NULL = 0,           // 无任何事件,初始态或上报成功清除状态
    RPT_WARN = 1,           // 有告警事件
    RPT_QUIT = 2,           // 有告警退出事件
    RPT_WARN_SENT = 3,      // 已上报告警事件,等待回包
    RPT_QUIT_SENT = 4,      // 已上报退出事件,等待回包
}REPORT_STATE_E;

/*---------------------- 监控相关 ----------------------*/
typedef enum
{
    SIM_OUT = 0,            /* SIM卡未插入 */
    SIM_IN = !SIM_OUT       /* SIM卡插入 */
}SIM_STATE_E;

typedef enum
{
    PDU_WARN = 1,           /* 告警 */
    PDU_QUERY = 2,          /* 查询 */
    PDU_SET = 3             /* 设置 */
}PDU_CMD_E;

typedef enum
{
    PDU_SRC_OMC = 1,        /* 来自于OMC */
    PDU_SRC_PC,             /* 来自于PC */
    PDU_SRC_BLE,            /* 来自于BLE */
}PDU_SRC_E;


typedef enum
{
    LINK_UDP = 1,           /* UDP */
    LINK_TCP,               /* TCP */
}IP_TYPE_E;

typedef enum
{
    LINK_IPV4 = 1,          /* IPv4 */
    LINK_IPV6,              /* IPv6 */
    LINK_IPV4V6,            /* IPv4v6 */
}LINK_TYPE_E;

typedef enum
{
    CAT1_EC800G = 0,
    CAT1_N706,
    CAT1_MC661,
}CAT1_TYPE_E;

typedef enum
{
	BAND_3  = 0,            /* 1800M */
	BAND_8,			        /* 900M */
	BAND_39,		        /* F */
	BAND_40,		        /* E */
	BAND_41,		        /* 5G NR */
	BAND_41M,	            /* 5G MIMO */
	BAND_END
}RF_BAND_E;


typedef enum
{
	B3_20M   = 0,
	B3_25M   = 1,
	B8_10M   = 0,
	B8_15M   = 1,
	B39_10M  = 0,
	B39_20M  = 1,
	B39_30M  = 2,
	B40_20M	 = 0,
	B40_50M  = 1,
	B41_20M  = 0,
	B41_60M  = 1,
	B41_80M  = 2,
	B41_100M = 3
}BAND_BW_E;


typedef enum
{
	IS_4G = 0,              /* 4G */
	IS_5G,                  /* 5G */
}IS_4G_5G_E;


typedef enum
{
	IS_FDD = 0,             /* 频分系统 */
	IS_TDD,                 /* 时分系统 */
	IS_NONE
}IS_TDD_E;

typedef enum
{
	RUN_STATE_NORMAL = 0,	/* 正常运行 */
	RUN_STATE_SILENCE,		/* 静默模式 */
	RUN_STATE_CLOSE,		/* 关断模式 */
}RUN_STATE_E;

typedef enum				
{
	WORK_NORMAL = 0,		/* 正常模式 */
	WORK_CALIB,				/* 校准模式 */
	WORK_PL,				/* 产线模式 */
}WORK_MODE_E;


typedef enum
{
	USB_UNCONNECT = 0,			/* USB未连接 */
	USB_CONNECT,				/* USB已连接 */
}USB_CONNECT_STATE_E;

typedef enum
{
	CLOSE_POW_OVER = 1,		/* 过功率关断 */
	CLOSE_ISO,				/* 隔离度关断 */
}SYS_CLOSE_REASON_E;

#ifdef __cplusplus
}
#endif

#endif
