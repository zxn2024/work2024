/*************************************** Copyright ******************************
*
* (C) Copyright 2022, Shaanxi Tianji Communication
* All Rights Reserved
*                         
* FileName   : bsp_version.h   
* Version    : none		
* Author     : none			
* Date       : 2022-08-03         
* Description: none   
*******************************************************************************/

#ifndef __BSP_VERSION_H_
#define __BSP_VERSION_H_

#include <stdint.h>

typedef struct
{
    uint8_t ucaSoftVer[30];     //软件版本
    uint8_t ucaDateTime[30];    //编译日期时间
    uint8_t ucaMcuModel[20];    //MCU型号
    uint32_t ulHalLibVer;       //版本库
    uint32_t ulVerUpgrade;      //软件升级用的版本号
}stSoftVersion_T;


void bspVerInfo( void );

extern stSoftVersion_T tSoftVersionInfo;

#endif

