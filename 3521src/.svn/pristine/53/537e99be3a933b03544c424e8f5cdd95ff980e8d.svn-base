/****************************************************************************
* Copyright(C),2020-2021,陕西天基通信科技有限责任公司
* 文 件 名：bsp_config.h
* 内容简介：公共定义
*
* 文件历史：
* 版本号       日期      作者      说明
* 01a       2021-08-01   张威     创建公共定义
*/

#ifndef __BSP_DEV_CONFIG_H_
#define __BSP_DEV_CONFIG_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "system_config.h"

/*************************** 当前的设备类型 ***************************/
#define LOCAL_DEVICE_TYPE   _DTX  		//当前设备类型 							===================== 设备类型 =====================
#define LOCAL_MODULE_TYPE   _MU   		//当前模块类型							===================== 模块类型 =====================

#if( LOCAL_MODULE_TYPE == _MU )
	#if( LOCAL_DEVICE_TYPE == _DTX )
		#define SLAVE_DEV_PERMIT_MAX_NUM   ( 2 )   //设计允许的最大从机数量
	#elif( LOCAL_DEVICE_TYPE == _WSF )
		#define SLAVE_DEV_PERMIT_MAX_NUM   ( 8 )  
	#else
		#define SLAVE_DEV_PERMIT_MAX_NUM   ( 0 )   
	#endif
#else
	#define SLAVE_DEV_PERMIT_MAX_NUM   ( 0 )   //设计允许的最大从机数量
#endif

#if( LOCAL_DEVICE_TYPE == _DTX )
	#if( LOCAL_MODULE_TYPE == _MU )
		#define LOCAL_DEVICE_ID     ( 0 )	//电梯主机
		#define SLAVE_DEV_MAX_NUM   ( 2 )
	#else
		#define LOCAL_DEVICE_ID     ( 0 )	//电梯从机
		#define SLAVE_DEV_MAX_NUM   ( 0 )
	#endif
	
#elif( LOCAL_DEVICE_TYPE == _WSF )
	#if( LOCAL_MODULE_TYPE == _MU )
		#define LOCAL_DEVICE_ID     ( 0 )	//微室分主机
		#define SLAVE_DEV_MAX_NUM   ( 4 )
	#else
		#define LOCAL_DEVICE_ID     ( 0 )	//微室分从机
		#define SLAVE_DEV_MAX_NUM   ( 0 )
	#endif
#elif( LOCAL_DEVICE_TYPE == _MGB )
    #define LOCAL_DEVICE_ID     ( 0xFF )	//满格宝
    #define SLAVE_DEV_MAX_NUM   ( 0 )
#else
    #define LOCAL_DEVICE_ID     ( 0xFF )
    #define SLAVE_DEV_MAX_NUM   ( 0 )
#endif


/****************************** 设备默认参数 ******************************/
#define DEFAULT_FACTORY         ( 0 )           //厂商代码
#define DEFAULT_DEVICE_TYPE     ( 0 )           //设备类别
#define DEFAULT_DEVICE_MODEL    ( "0" )         //设备型号
#define DEFAULT_OPLC_EN         ( 1 )      		//开环工控使能	0: 禁止, 1:允许
#define DEFAULT_LED_SILENCE     ( 0 )     		//指示灯静默, 0: 禁止, 1:允许
#define DEFAULT_IP_TYPE         ( 0 )           //IP类型 : 0:IPv4 / 1:IPv6
#define DEFAULT_LINK_TYPE       ( 0 )           //连接类型 0:TCP / 1:UDP 
#define DEFAULT_MODEM           ( 0 )           //模组型号
/****************************** 软件版本 *********************************/


//硬件版本
//#define MCU_FACT		MCU_GD					//MCU厂商 0: ST,  1: GD
//#define MCU_MODEL       "GD32F407VET6"          //单片机型号
//#define MODEM_5G        "EC200U"                //5G 模组型号
//#define MODEM_CAT1      "EC200U"                //CAT1模组型号
////#define STD_LIB         "V3.0.0"              //标准库版本
//#define STD_LIB         HAL_GetHalVersion()   //标准库版本

//#define VER_PRE_FACT    "CM"                    //厂商名缩写


/****************************** 设备参数 ******************************/
//#define FACTORY_ID    ( 0 )    //厂商代码
//#define DEVICE_TYPE   ( LOCAL_MODULE_TYPE )    //设备类型 1: 主机 0: 从机
//#if( LOCAL_MODULE_TYPE == MODULE_TYPE_MU ) 
//	#define DEVICE_MODEL  "MU"     //设备型号
//#else
//	#define DEVICE_MODEL  "RU"     //设备型号
//#endif



//系统编号
typedef enum
{
    RF_SYS_1 = 0,           /* SYS1系统 */
    RF_SYS_2,               /* SYS2系统 */
    RF_SYS_3,               /* SYS3系统 */
//    RF_SYS_4,               /* SYS4系统 */
//    RF_SYS_5,               /* SYS5系统 */
//    RF_SYS_6,               /* SYS6系统 */
//    RF_SYS_7,               /* SYS7系统 */
//    RF_SYS_8,               /* SYS8系统 */
//    RF_SYS_NUM
}RF_SYS_E;

typedef enum
{
	BAND_3  = 1,	//1800M
	BAND_8,			//900M
	BAND_39,		//F
	BAND_40,		//E
	BAND_NR,		//5G
	BAND_MIMO,	//5GMIMO
	BAND_END
}E_RF_4G_BAND;

typedef enum
{
	E_EXIST = 0,
	E_NONE
}E_EXIST_STATE;


typedef enum
{
	SYS_FDD = 0,
	SYS_TDD,
	SYS_NONE
}E_SYS_MODE;

//typedef enum
//{
//    RF_CH_UP = 0,  //上行 
//    RF_CH_DW,      //下行
//    RF_CH_NUM
//}RF_CH_E;
#define RF_SYS_NUM       ( 3 )
#define RF_CH_NUM		 ( 2 )


#endif
