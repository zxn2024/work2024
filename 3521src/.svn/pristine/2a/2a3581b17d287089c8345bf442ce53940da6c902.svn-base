/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2023-10-09 10:51:54
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2023-10-16 17:10:58
 * @FilePath: \MDK-ARMd:\01_WORK\2023\08_CM_5G_DT_3521\03_SourceCode\3521_MU\app\bsp_rf\bsp_dev_para.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
/*************************************** Copyright ******************************
*
* (C) Copyright 2022, Shaanxi Tianji Communication
* All Rights Reserved
*
* FileName   : bsp_dev_para.h
* Version    : none
* Author     : ZhangWei
* Date       : 2023-10-16
* Description: 设备参数定义
*******************************************************************************/
//#include <stdint.h>
#ifndef __BSP_DEV_PARA_H_
#define __BSP_DEV_PARA_H_

#include <stdint.h>
#include "system_config.h"

#pragma pack(1)
typedef struct
{
    int8_t  theoryGain;         //理论增益
    int8_t  theoryPow;          //理论输出功率
//    int8_t  tagGainMinValue;    //可调整增益的最小值
//    int8_t  tagPowMinValue;     //可调整功率的最小值
//    uint8_t powOverCloseAttNum;	//过功率关断时的衰减器数量
//    uint8_t isoCloseGate;		//隔离度关断门限
    int8_t ecr8668AlcPoint;	//8668功率启控点
} RfChStaticPara_T;

typedef struct
{
    uint8_t          sysMode;            //系统制式, TDD / FDD
    RfChStaticPara_T chPara[RF_CH_NUM];  //通道参数
} RfStaticPara_T;

#pragma pack()



extern Device_T        tDevFactoryPara;                 // 设备出厂参数信息
extern omc_t           tOmcPara;                        // 网管参数信息
extern RfStaticPara_T  tRfSysStaticPara[RF_SYS_NUM];    // 设备参数信息

void vBasicDev3BandDefaultSet( void );		//3频设备默认配置, 屏蔽MIMO 和 BAND 39 和 BAND 41
void vBasicDeviceParaInit(void);			

#endif





