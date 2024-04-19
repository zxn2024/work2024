/***************************** Copyright ****************************************
*
* (C) Copyright 2022, Shaanxi Tianji Comunication
*  All Rights Reserved
*
* FileName   : bsp_version.c
* Version    : none
* Author     : none
* Date       : 2022-08-03
* Description: none
*******************************************************************************/
#include "main.h"
#include "stdint.h"
#include "stdio.h"
#include "string.h"
#include "bsp_rtt.h"
#include "bsp_version.h"
#include "bsp_dev_config.h"


/*---------------------- 端口定义 --------------------------------------*/


/*---------------------- 宏定义  ---------------------------------------*/
#if ( LOCAL_DEVICE_TYPE == MODULE_TYPE_DT )
#define VER_DEV_NAME     "DT"
#elif ( LOCAL_DEVICE_TYPE == MODULE_TYPE_WSF )
#define VER_DEV_NAME     "WSF"
#else
#define VER_DEV_NAME     "SHIP"
#endif

#if ( LOCAL_MODULE_TYPE == MODULE_TYPE_MU )
#define VER_MODULE_NAME     "MU"
#else
#define VER_MODULE_NAME     "RU"
#endif


#if ( LOCAL_DEVICE_TYPE == DEVICE_TYPE_MGB )
	#define SW_VERSION ( VER_PRE_FACT"_"VER_DEV_NAME"_V" )  //软件完整版本号
	#define HW_VERSION ( VER_PRE_FACT"_"VER_DEV_NAME"_V"VER_HARDWARE )  //硬件完整版本号
#else
	#define SW_VERSION ( VER_PRE_FACT"_"VER_DEV_NAME"_"VER_MODULE_NAME"_V" )   //硬件完整版本号
	#define HW_VERSION ( VER_PRE_FACT"_"VER_DEV_NAME"_"VER_MODULE_NAME"_V"VER_HARDWARE )   //硬件完整版本号
#endif


/*---------------------- 结构体 ----------------------------------------*/


/*---------------------- 常量 ------------------------------------------*/


/*---------------------- 全局变量 --------------------------------------*/
stSoftVersion_T tSoftVersionInfo;


/*---------------------- 局部变量 --------------------------------------*/



/*---------------------- 函数声明 --------------------------------------*/



/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


/****************************************************************************
* 函数名： bspVerInfo()
* 功  能： 在RTT上打印软件版本信息
* 输  入： 无
* 输  出： 无
*/
void bspVerInfo( void )
{
    sprintf( (char *)tSoftVersionInfo.ucaSoftVer, "%s%d_%d", SW_VERSION, VER_SOFTWARE, LED_DISPLAY_PLAN );
    sprintf( (char *)tSoftVersionInfo.ucaDateTime, "%s, %s", __DATE__, __TIME__ );
    sprintf( (char *)tSoftVersionInfo.ucaMcuModel, "%s", MCU_MODEL );
	tSoftVersionInfo.ulVerUpgrade = VER_SOFTWARE;
//    tSoftVersionInfo.ulHalLibVer = STD_LIB;

    bsp_printf( RTT_OS, "\r\n*\r\n*\r\n*\r\n" );
    bsp_printf( RTT_OS, "MCU    : %s\r\n", MCU_MODEL );         /* MCU型号  */
    bsp_printf( RTT_OS, "SW_VER : %s\r\n", tSoftVersionInfo.ucaSoftVer );        /* 软件版本 */
//    bsp_printf( RTT_OS, "STD_LIB: %s\r\n", STD_LIB );           /* 版本库   */
    bsp_printf( RTT_OS, "SW_DATA: %s\r\n", tSoftVersionInfo.ucaDateTime);   /* 日期时间 */
    bsp_printf( RTT_OS, "*\r\n*\r\n*\r\n\r\n" );
//    bsp_printf( RTT_OS, "%s", __TIME__ );
}

