/*************************************** Copyright ******************************
 *
 * (C) Copyright 2023, Shaanxi Tianji Communication
 * All Rights Reserved
 *
 * FileName   : bi_lna.h
 * Version    : none
 * Author     :
 * Date       : 2023-09-21
 * Description: lna的通用接口模版,提供统一的lna接口:
 *              -BI_LNA_ON(obj)
 *              -BI_LNA_OFF(obj)
 *              -BI_LNA_FLASH(obj, val)
 * 开发lna驱动时需要按照此模版进行
 *******************************************************************************/
//

#ifndef _BI_LNA_H_
#define _BI_LNA_H_

/*---------------------- LNA驱动模版说明 ------------------------------------------*/
// LNA驱动模板说明
// -LNA驱动开发时，数据结构必须基于此接口模板，在此基础上新增变量和方法，但是不允许改动模板中已有的变量和方法。

/*
设备初始化结构体，包含MCU资源分配，比如GPIO分配，SPI端口，该设备的查找表等
typedef struct __LNA_xxxx_HwConfig_t
{
    LNA_Port:驱动中根据实际使用的端口类型和数量来定义
    LNA_BYPASS_E bypass_init_stat;  //lna初始化后的初始状态
    LNA_PWR_E pwr_ctl_init_stat;  //lna初始化后的初始状态
    //此处定义该设备所需的硬件接口（如GPIO、SPI等），资源（内存，缓冲区），查找表（功率表，衰减表，温度表）
} LNA_xxxx_HwConfig_t;

设备的数据结构，包含数据和所有对外接口
typedef struct __LNA_ClassTypeDef
{
    LNA_xxxx_HwConfig_t hw_config;                                                                   // 初始化参数
    LNA_BYPASS_E byp_stat;                                                                           // lna当前旁路状态 LNA_BYS (0) LNA_RF (1)
    LNA_PWR_E pwr_stat;                                                                              // lna当前电源状态 LNA_BYS (0) LNA_RF (1)
    Status_t (*set_bypass)(LNA_xxxx_t *pLnaHandle, LNA_BYPASS_E stat);                               // 设置lna工作状态,及闪灯周期
    Status_t (*set_pwr)(LNA_xxxx_t *pLnaHandle, LNA_PWR_E ctl);                                      // lna闪灯执行函数
} LNA_ClassTypeDef;

注册函数，用于注册一个设备并分配资源
Status_t lna_xxxx_register(LNA_xxxx_t *handle_lna);
*/
/*---------------------- 端口定义 --------------------------------------*/

/*---------------------- 宏定义  ---------------------------------------*/
typedef enum
{
    LNA_BYS = 0,        //旁路
    LNA_RF = 1,         //放大
} LNA_BYPASS_E;

typedef enum
{
    PWR_OFF = 0,             //电源/使能关闭
    PWR_ON = 1,              //电源/使能打开
} LNA_PWR_E;
/*---------------------- 对外接口 --------------------------------------*/


/*****************************************************************************
 * @Name	  : BI_LNA_OPEN
 * @brief  : LNA打开设备,并按照初始化参数进行设置,函数原型 Status_t (*set)(LNA_xxxx_t *pLnaHandle, LNA_BYPASS_E stat);
 * @param  : pLnaHandle: lna句柄指针
 * @Retval : Status_t
 * @Note   : LNA打开设备
 */
#define BI_LNA_OPEN(pLnaHandle) (pLnaHandle)->open((pLnaHandle)) // LNA打开

/*****************************************************************************
 * @Name	  : BI_LNA_BYPASS
 * @brief  : 设置LNA工作模式,函数原型 Status_t (*set)(LNA_xxxx_t *pLnaHandle, LNA_BYPASS_E stat);
 * @param  : pLnaHandle: lna灯句柄指针
 * @param  : stat lna状态 LNA_BYS (0) LNA_RF (1)
 * @Retval : Status_t
 * @Note   : 用来设置lna旁路
 */
#define BI_LNA_BYPASS(pLnaHandle, bypass) (pLnaHandle)->set_bypass((pLnaHandle), (bypass)) // LNA设置旁路

/*****************************************************************************
 * @Name	  : BI_LNA_BYPASS
 * @brief  : 设置LNA工作模式,函数原型 Status_t (*set)(LNA_xxxx_t *pLnaHandle, LNA_PWR_E pwr_ctl);
 * @param  : pLnaHandle: lna灯句柄指针
 * @param  : pwr_ctl lna电源状态 LNA_BYS (0) LNA_RF (1)
 * @Retval : Status_t
 * @Note   : 用来设置lna旁路
 */
#define BI_LNA_SET_PWR(pLnaHandle, pwr_ctl) (pLnaHandle)->set_pwr((pLnaHandle), (pwr_ctl)) // LNA开关

#endif /* __BI_LNA_H__ */
