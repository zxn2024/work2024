//<<< Use Configuration Wizard in Context Menu >>>
/**
 * **********************************************************************************
 * @copyright Copyright (c) 2023 Shaanxi Tianji Communication Technologies Co., Ltd.
 *                         
 * @file        system_config.h   
 * @author      
 * @version     None
 * @date        2023-12-04     
 * @brief       None   
 * 
 * **********************************************************************************
 */

#ifndef __SYSTEM_CONFIG_H_
#define __SYSTEM_CONFIG_H_

#include "public_struct.h"
#include "public_define.h"
#include "public_enum.h"

#ifdef __cplusplus
 extern "C" {
#endif


/****************************** 硬件测试 ******************************/
#define EN_HW_TEST      0           /* 硬件测试开关 1:开底层硬件调试; 0:关底层硬件调试 */

/****************************** 产品代码 ******************************/
#define CMDT3521A       (0x01)
#define CMDT3521B       (0x02)
#define CMDT3521C       (0x03)
#define CMDT3521D       (0x04)
#define CMDT3521E       (0x05)
#define CMDT3521F       (0x06)

#ifndef PRODUCT_MODEL
#define PRODUCT_MODEL   (CMDT3521A)
#endif

/****************************** 产品类别 ******************************/
/* 设备类型 */
#define _MGB                    1			            /* 满格宝 */
#define _DTX                    2       	            /* 电梯型 */
#define _WSF                    3       	            /* 微室分 */

/* 主从机类型 */
#define _MU                     1       	            /* 主机 */
#define _RU                     2       	            /* 从机 */
#define _HUB                    3       	            /* HUB  */

/****************************** 监控网管 ******************************/
#define _TJ                     "_01"                   /* 自研网管 */
#define _SW                     "_02"                   /* 三维网管 */
#define _JX                     "_03"                   /* 京信网管 */
#define _ZXK                    "_04"                   /* 中信科网管 */
#define _WLW                    "_05"                   /* 物联网网管 */


/*============================以下参数需要根据设备进行修改============================*/

/****************************** 功能开关 ******************************/
#define _USE_RTOS_                          1           /* 是否使用系统 */    /* 0:裸机 1:系统 */
#define _USE_UPGRADE_                       1           /* 是否支持升级 */    /* 0:不支持 1:支持 */

/****************************** 设备类型 ****************************/
//<o> LOCAL_DEVICE_TYPE
//<i> 1: _MGB
//<i> 2: _DTX
//<i> 3: _WSF
//<2=> 5F - DTX   <3=> 3F - WSF
#define LOCAL_DEVICE_TYPE                   ( 3 )            /* 当前设备类型 */
#define LOCAL_MODULE_TYPE                   _MU             /* 当前模块类型 */

#if( _MU == LOCAL_MODULE_TYPE)
    #if( LOCAL_DEVICE_TYPE == _DTX )
        #define SLAVE_DEV_PERMIT_MAX_NUM    ( 2 )           /* 设计允许的最大从机数量 */
    #elif( _WSF == LOCAL_DEVICE_TYPE )
        #define SLAVE_DEV_PERMIT_MAX_NUM    ( 8 )
    #else
        #define SLAVE_DEV_PERMIT_MAX_NUM    ( 0 )
    #endif
#else
    #define SLAVE_DEV_PERMIT_MAX_NUM        ( 0 )           /* 设计允许的最大从机数量 */
#endif

#if( LOCAL_DEVICE_TYPE == _DTX )
    #if( LOCAL_MODULE_TYPE == _MU )
        #define LOCAL_DEVICE_ID             ( 0 )	        /* 电梯主机 */
        #define SLAVE_DEV_MAX_NUM           ( 8 )
        #define HUB_DEV_MAX_NUM             ( 0 )
    #else
        #define LOCAL_DEVICE_ID             ( 0 )	        /* 电梯从机 */
        #define SLAVE_DEV_MAX_NUM           ( 0 )
        #define HUB_DEV_MAX_NUM             ( 0 )
    #endif
#elif( LOCAL_DEVICE_TYPE == _WSF )
    #if( LOCAL_MODULE_TYPE == _MU )
        #define LOCAL_DEVICE_ID             ( 0 )	        /* 微室分主机 */
        #define SLAVE_DEV_MAX_NUM           ( 8 )
        #define HUB_DEV_MAX_NUM             ( 1 )
    #else
        #define LOCAL_DEVICE_ID             ( 0 )	        /* 微室分从机 */
        #define SLAVE_DEV_MAX_NUM           ( 0 )
        #define HUB_DEV_MAX_NUM             ( 0 )
    #endif
#elif( LOCAL_DEVICE_TYPE == _MGB )
    #define LOCAL_DEVICE_ID                 ( 0xFF )        /* 满格宝 */
    #define SLAVE_DEV_MAX_NUM               ( 0 )
    #define HUB_DEV_MAX_NUM                 ( 0 )
#else
    #define LOCAL_DEVICE_ID                 ( 0xFF )
    #define SLAVE_DEV_MAX_NUM               ( 0 )
    #define HUB_DEV_MAX_NUM                 ( 0 )
#endif

#define BLE_SLAVE_DEV_MAX_NUM       ( SLAVE_DEV_MAX_NUM + HUB_DEV_MAX_NUM )

/************************ 设备默认参数 *************************/
#if(PRODUCT_MODEL == CMDT3521A)
#define DEFAULT_FACTORY                     ( "CMDI" )           /* 厂商代码 */
#define DEFAULT_DEVICE_TYPE                 ( 0 )           /* 设备类别 */
#define DEFAULT_DEVICE_MODEL                ( "0" )         /* 设备型号 */
#define DEFAULT_OPLC_EN                     ( 1 )      	    /* 开环工控使能	0: 禁止, 1:允许 */
#define DEFAULT_LED_SILENCE                 ( 0 )     	    /* 指示灯静默, 0: 禁止, 1:允许 */
#define DEFAULT_IP_TYPE                     ( LINK_TCP )    /* IP类型 */
#define DEFAULT_LINK_TYPE                   ( LINK_IPV4 )   /* 连接类型 */
#define DEFAULT_MODEM                       ( CAT1_EC800G ) /* 模组型号 */
#define DEFAULT_CAT1_NAME                   "EC800G"        /* cat.1模组型号 */
#define DEFAULT_MCU_NAME                    "GD32F427ZET6"  /* MCU名 */
#elif(PRODUCT_MODEL == CMDT3521B)
#define DEFAULT_FACTORY                     ( "sunwave" )           /* 厂商代码 */
#define DEFAULT_DEVICE_TYPE                 ( 0 )           /* 设备类别 */
#define DEFAULT_DEVICE_MODEL                ( "0" )         /* 设备型号 */
#define DEFAULT_OPLC_EN                     ( 1 )      	    /* 开环工控使能	0: 禁止, 1:允许 */
#define DEFAULT_LED_SILENCE                 ( 0 )     	    /* 指示灯静默, 0: 禁止, 1:允许 */
#define DEFAULT_IP_TYPE                     ( LINK_TCP )    /* IP类型 */
#define DEFAULT_LINK_TYPE                   ( LINK_IPV4 )   /* 连接类型 */
#define DEFAULT_MODEM                       ( CAT1_N706 )   /* 模组型号 */
#define DEFAULT_CAT1_NAME                   "N706"          /* cat.1模组型号 */
#define DEFAULT_MCU_NAME                    "GD32F427ZET6"  /* MCU名 */
#elif(PRODUCT_MODEL == CMDT3521C)
#define DEFAULT_FACTORY                     ( "" )           /* 厂商代码 */
#define DEFAULT_DEVICE_TYPE                 ( 0 )           /* 设备类别 */
#define DEFAULT_DEVICE_MODEL                ( "0" )         /* 设备型号 */
#define DEFAULT_OPLC_EN                     ( 1 )      	    /* 开环工控使能	0: 禁止, 1:允许 */
#define DEFAULT_LED_SILENCE                 ( 0 )     	    /* 指示灯静默, 0: 禁止, 1:允许 */
#define DEFAULT_IP_TYPE                     ( LINK_TCP )    /* IP类型 */
#define DEFAULT_LINK_TYPE                   ( LINK_IPV4 )   /* 连接类型 */
#define DEFAULT_MODEM                       ( CAT1_MC661 )  /* 模组型号 */
#define DEFAULT_CAT1_NAME                   "MC661"         /* cat.1模组型号 */
#define DEFAULT_MCU_NAME                    "STM32F407ZET6" /* MCU名 */
#endif


/************************ 设备射频参数 *************************/
#if(PRODUCT_MODEL == CMDT3521A)
#define RF_SYS_NUM		                    ( 3 )           /* 射频系统个数 */
#define RF_CH_NUM		                    ( 2 )           /* 通道数 */
#elif(PRODUCT_MODEL == CMDT3521B)
#define RF_SYS_NUM		                    ( 3 )           /* 射频系统个数 */
#define RF_CH_NUM		                    ( 2 )           /* 通道数 */
#elif(PRODUCT_MODEL == CMDT3521C)
#define RF_SYS_NUM		                    ( 3 )           /* 射频系统个数 */
#define RF_CH_NUM		                    ( 2 )           /* 通道数 */
#endif


/************************ 外部FLASH型号、信息 *************************/
#if(PRODUCT_MODEL == CMDT3521A)
#if( LOCAL_MODULE_TYPE == _MU )
#define FLASH_NAME                          "PY25Q128HA"
#define FLASH_PAGE_SIZE                     256
#define FLASH_SECTOR_SIZE                   4 * 1024
#define FLASH_SECTOR_NUM                    4096
#define FLASH_SIZE                          FLASH_SECTOR_SIZE * FLASH_SECTOR_NUM
#else
#define FLASH_NAME                          "PY25Q64HA"
#define FLASH_PAGE_SIZE                     256
#define FLASH_SECTOR_SIZE                   4 * 1024
#define FLASH_SECTOR_NUM                    2048
#define FLASH_SIZE                          FLASH_SECTOR_SIZE * FLASH_SECTOR_NUM
#endif
#elif(PRODUCT_MODEL == CMDT3521B)
#if( LOCAL_MODULE_TYPE == _MU )
#define FLASH_NAME                          "XM25QH128DHIQT"
#define FLASH_PAGE_SIZE                     256
#define FLASH_SECTOR_SIZE                   4 * 1024
#define FLASH_SECTOR_NUM                    4096
#define FLASH_SIZE                          FLASH_SECTOR_SIZE * FLASH_SECTOR_NUM
#else
#define FLASH_NAME                          "XM25QH64DHIQT"
#define FLASH_PAGE_SIZE                     256
#define FLASH_SECTOR_SIZE                   4 * 1024
#define FLASH_SECTOR_NUM                    2048
#define FLASH_SIZE                          FLASH_SECTOR_SIZE * FLASH_SECTOR_NUM
#endif
#elif(PRODUCT_MODEL == CMDT3521C)
#if( LOCAL_MODULE_TYPE == _MU )
#define FLASH_NAME                          "GD25Q128ESIGR"
#define FLASH_PAGE_SIZE                     256
#define FLASH_SECTOR_SIZE                   4 * 1024
#define FLASH_SECTOR_NUM                    4096
#define FLASH_SIZE                          FLASH_SECTOR_SIZE * FLASH_SECTOR_NUM
#else
#define FLASH_NAME                          "GD25Q64ESIGR"
#define FLASH_PAGE_SIZE                     256
#define FLASH_SECTOR_SIZE                   4 * 1024
#define FLASH_SECTOR_NUM                    2048
#define FLASH_SIZE                          FLASH_SECTOR_SIZE * FLASH_SECTOR_NUM
#endif
#endif

/****************************** 版本信息 ****************************/
#if(PRODUCT_MODEL == CMDT3521A)
    #define PRJ_NAME                            "CM_5G_DT_"     /* 项目名称 */
	//<s> 3521A SW Version
	//<i> year(2Byte) - month(2Byte) - day(2Byte) - version(1Byte)
	//<i> eg. 2404112
    #define SW_VERNUM                           "2404181"       /* 软件版本 2位年份+2位月份+3位更新序列码*/
    #define HW_VERNUM                           "V1.0"          /* 硬件版本 */
    #define MONITOR_TYPE                        _TJ             /* 监控平台 */

    #if(LOCAL_DEVICE_TYPE == _DTX)
        #define SW_VER_PREFIX1                  "CMDI-DTWF-"           /* 软件版本前缀1 */
    #elif(LOCAL_DEVICE_TYPE == _WSF)
        #define SW_VER_PREFIX1                  "CMDI-KZWF-"           /* 软件版本前缀1 */
    #endif

    #if(LOCAL_MODULE_TYPE == _MU)
        #define SW_VER_PREFIX2                  "MU-"           /* 软件版本前缀2 */
    #elif(LOCAL_MODULE_TYPE == _RU)
        #define SW_VER_PREFIX2                  "RU-"           /* 软件版本前缀2 */
    #elif(LOCAL_MODULE_TYPE == _HUB)
        #define SW_VER_PREFIX2                  "HU-"           /* 软件版本前缀2 */
    #endif

#elif(PRODUCT_MODEL == CMDT3521B)
    #define PRJ_NAME                            "CM_5G_DT_"     /* 项目名称 */
	//<s> 3521B SW Version
	//<i> year(2Byte) - month(2Byte) - day(2Byte) - version(1Byte)
	//<i> eg. 2404112
    #define SW_VERNUM                           "2404181"       /* 软件版本 2位年份+2位周份+3位更新序列码*/
    #define HW_VERNUM                           "V1.0"          /* 硬件版本 */
    #define MONITOR_TYPE                        _TJ             /* 监控平台 */

    #define SW_VER_PREFIX0                      "sw_"             /* 软件版本前缀0 */

    #if(LOCAL_MODULE_TYPE == _MU)
        #define SW_VER_PREFIX1                  "B_"           /* 软件版本前缀1 */
    #elif(LOCAL_MODULE_TYPE == _RU)
        #define SW_VER_PREFIX1                  "C_"           /* 软件版本前缀1 */
    #elif(LOCAL_MODULE_TYPE == _HUB)
        #define SW_VER_PREFIX1                  "H_"           /* 软件版本前缀1 */
    #endif

    #if(LOCAL_DEVICE_TYPE == _DTX)
        #define SW_VER_PREFIX2                  "D_"           /* 软件版本前缀2 */
    #elif(LOCAL_DEVICE_TYPE == _WSF)
        #define SW_VER_PREFIX2                  "W_"           /* 软件版本前缀2 */
    #endif

#elif(PRODUCT_MODEL == CMDT3521C)
    #define PRJ_NAME                            "CM_5G_DT_"     /* 项目名称 */
	//<s> 3521C SW Version
	//<i> year(4Byte) - month(2Byte) - day(2Byte) - version(1Byte)
	//<i> eg. 202404112
    #define SW_VERNUM                           "202404181"       /* 软件版本 2024：年份，03：月份，001：版本号*/
    #define HW_VERNUM                           "V1.0"          /* 硬件版本 */
    #define MONITOR_TYPE                        _TJ             /* 监控平台 */

    #define SW_VER_PREFIX0                      "GZF_"             /* 软件版本前缀0 */

    #if(LOCAL_DEVICE_TYPE == _DTX)
        #define SW_VER_PREFIX1                  "BN-"           /* 软件版本前缀1 */
    #elif(LOCAL_DEVICE_TYPE == _WSF)
        #define SW_VER_PREFIX1                  "CN-"           /* 软件版本前缀1 */
    #endif

    #if(LOCAL_MODULE_TYPE == _MU)
        #define SW_VER_PREFIX2                  "MU_"           /* 软件版本前缀2 */
    #elif(LOCAL_MODULE_TYPE == _RU)
        #define SW_VER_PREFIX2                  "RU_"           /* 软件版本前缀2 */
    #elif(LOCAL_MODULE_TYPE == _HUB)
        #define SW_VER_PREFIX2                  "HUB_"           /* 软件版本前缀2 */
    #endif

#endif

/****************************** 设备型号 ****************************/
#if(PRODUCT_MODEL == CMDT3521A)
    #if(LOCAL_DEVICE_TYPE == _DTX)
        #if(LOCAL_MODULE_TYPE == _MU)
            #define DEVICE_MODEL                  "CMDI-DTWF-03-ZK"             /* 设备型号 */
        #elif(LOCAL_MODULE_TYPE == _RU)
            #define DEVICE_MODEL                  "CMDI-DTWF-03-JX"             /* 设备型号 */
        #endif
    #elif(LOCAL_DEVICE_TYPE == _WSF)
        #if(LOCAL_MODULE_TYPE == _MU)
            #define DEVICE_MODEL                  "CMDI-KZWF-01-JR"             /* 设备型号 */
        #elif(LOCAL_MODULE_TYPE == _RU)
            #define DEVICE_MODEL                  "CMDI-KZWF-03-YD"             /* 设备型号 */
        #elif(LOCAL_MODULE_TYPE == _HUB)
            #define DEVICE_MODEL                  "CMDI-KZWF-03-KZ"             /* 设备型号 */
        #endif
    #endif

#elif(PRODUCT_MODEL == CMDT3521B)
    #if(LOCAL_DEVICE_TYPE == _DTX)
        #if(LOCAL_MODULE_TYPE == _MU)
            #define DEVICE_MODEL                  "NMR-E"             /* 设备型号 */
        #elif(LOCAL_MODULE_TYPE == _RU)
            #define DEVICE_MODEL                  "NMR-E"             /* 设备型号 */
        #endif
    #elif(LOCAL_DEVICE_TYPE == _WSF)
        #if(LOCAL_MODULE_TYPE == _MU)
            #define DEVICE_MODEL                  "NMR-B"             /* 设备型号 */
        #elif(LOCAL_MODULE_TYPE == _RU)
            #define DEVICE_MODEL                  "NMR-B"             /* 设备型号 */
        #elif(LOCAL_MODULE_TYPE == _HUB)
            #define DEVICE_MODEL                  "NMR-B"             /* 设备型号 */
        #endif
    #endif

#elif(PRODUCT_MODEL == CMDT3521C)
    #if(LOCAL_DEVICE_TYPE == _DTX)
        #if(LOCAL_MODULE_TYPE == _MU)
            #define DEVICE_MODEL                  ""             /* 设备型号 */
        #elif(LOCAL_MODULE_TYPE == _RU)
            #define DEVICE_MODEL                  ""             /* 设备型号 */
        #endif
    #elif(LOCAL_DEVICE_TYPE == _WSF)
        #if(LOCAL_MODULE_TYPE == _MU)
            #define DEVICE_MODEL                  ""             /* 设备型号 */
        #elif(LOCAL_MODULE_TYPE == _RU)
            #define DEVICE_MODEL                  ""             /* 设备型号 */
        #elif(LOCAL_MODULE_TYPE == _HUB)
            #define DEVICE_MODEL                  ""             /* 设备型号 */
        #endif
    #endif

#endif
/*=====================================修改结束=====================================*/


/****************************** 设备版本 ****************************/

#if(PRODUCT_MODEL == CMDT3521A)
#define SW_VERSION          SW_VER_PREFIX1""SW_VER_PREFIX2""SW_VERNUM             /* 软件版本号 */
#define HW_VERSION          SW_VER_PREFIX1""SW_VER_PREFIX2""HW_VERNUM             /* 硬件版本号 */
#define SW_MON_VERSION      SW_VER_PREFIX1""SW_VER_PREFIX2""SW_VERNUM            /* 监控看到的软件版本号 */

#elif(PRODUCT_MODEL == CMDT3521B)
#define SW_VERSION          SW_VER_PREFIX0""SW_VER_PREFIX1""SW_VER_PREFIX2""SW_VERNUM             /* 软件版本号 */
#define HW_VERSION          SW_VER_PREFIX0""SW_VER_PREFIX1""SW_VER_PREFIX2""HW_VERNUM             /* 硬件版本号 */
#define SW_MON_VERSION      SW_VER_PREFIX0""SW_VER_PREFIX1""SW_VER_PREFIX2""SW_VERNUM            /* 监控看到的软件版本号 */

#elif(PRODUCT_MODEL == CMDT3521C)
#define SW_VERSION          SW_VER_PREFIX0""SW_VER_PREFIX1""SW_VER_PREFIX2""SW_VERNUM             /* 软件版本号 */
#define HW_VERSION          SW_VER_PREFIX0""SW_VER_PREFIX1""SW_VER_PREFIX2""HW_VERNUM             /* 硬件版本号 */
#define SW_MON_VERSION      SW_VER_PREFIX0""SW_VER_PREFIX1""SW_VER_PREFIX2""SW_VERNUM            /* 监控看到的软件版本号 */

#endif
#define SW_UPGRADE          atoi(SW_VERNUM)                 /* 升级版本号 */

/*********************** 设备频段数量选择 **********************/
#define DEV_MIMO_EXIST	( 0 )	//MIMO通道是否存在, 0: 不存在,  1: 存在


#ifdef __cplusplus
}
#endif

#endif
// <<< end of configuration section >>>
