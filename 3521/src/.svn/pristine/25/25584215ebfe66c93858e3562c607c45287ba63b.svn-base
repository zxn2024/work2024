/*************************************** Copyright ******************************
 *
 * (C) Copyright 2023, Shaanxi Tianji Communication
 * All Rights Reserved
 *
 * FileName   : bi_PORT.h
 * Version    : none
 * Author     :
 * Date       : 2023-09-21
 * Description: PORT的通用接口模版,提供统一的PORT接口:
 *              -BI_PORT_ON(obj)
 *              -BI_PORT_OFF(obj)
 * 开发PORT驱动时需要按照此模版进行
 *******************************************************************************/
//

#ifndef _BI_PORT_H_
#define _BI_PORT_H_

/*---------------------- PORT驱动模版说明 ------------------------------------------*/
// PORT驱动模板说明
// -PORT驱动开发时，数据结构必须基于此接口模板，在此基础上新增变量和方法，但是不允许改动模板中已有的变量和方法。

/*
设备初始化结构体，包含MCU资源分配，比如GPIO分配，SPI端口，该设备的查找表等
__PORT_xxxx_HwConfig_t其中xxxx代表设备的子类别，比如通过FPGA驱动PORT，则在port后增加FPGA后缀
typedef struct __PORT_xxxx_HwConfig_t
{
    PORT_Port:驱动中根据实际使用的端口类型和数量来定义
    PORT_CTL_E port_init_stat;  //PORT初始化后的初始状态
    //此处定义该设备所需的硬件接口（如GPIO、SPI等），资源（内存，缓冲区），查找表（功率表，衰减表，温度表）
} PORT_xxxx_HwConfig_t;

设备的数据结构，包含数据和所有对外接口
typedef struct __PORT_ClassTypeDef
{
    PORT_xxxx_HwConfig_t hw_config;                                                                      // 初始化参数
    PORT_CTL_E port_stat;                                                                                  // PORT当前使能状态 PORT_OFF (0) PORT_ON (1) 
    Status_t (*ctl)(PORT_xxxx_t *pPORTHandle, PORT_CTL_E ctl);                                               // PORT使能控制
} PORT_ClassTypeDef;

注册函数，用于注册一个设备并分配资源
Status_t PORT_xxxx_register(PORT_xxxx_t *handle_PORT);
*/
/*---------------------- 端口定义 --------------------------------------*/

/*---------------------- 宏定义  ---------------------------------------*/
typedef enum
{
    PORT_OFF = 0,            //关闭
    PORT_ON = 1,             //打开
}PORT_CTL_E;

/*---------------------- 对外接口 --------------------------------------*/

/*****************************************************************************
 * @Name	  : BI_PORT_OPEN
 * @brief  : 设置PORT工作模式,函数原型 Status_t (*set)(PORT_xxxx_t *pPORTHandle, PORT_CTL_E port_ctl);
 * @portram  : pPORTHandle: PORT灯句柄指针
 * @portram  : port_ctl PORT使能 PORT_OFF (0) PORT_ON (1) 
 * @Retval : Status_t
 * @Note   : 用来设置PORT旁路
 */
#define BI_PORT_OPEN(pPORTHandle) (pPORTHandle)->open((pPORTHandle)) // PORT开关

/*****************************************************************************
 * @Name	  : BI_PORT_BYPORTSS
 * @brief  : 设置PORT工作模式,函数原型 Status_t (*set)(PORT_xxxx_t *pPORTHandle, PORT_CTL_E port_ctl);
 * @portram  : pPORTHandle: PORT灯句柄指针
 * @portram  : port_ctl PORT使能 PORT_OFF (0) PORT_ON (1) 
 * @Retval : Status_t
 * @Note   : 用来设置PORT旁路
 */
#define BI_PORT_CTL(pPORTHandle, port_ctl) (pPORTHandle)->ctl((pPORTHandle), (port_ctl)) // PORT开关

#endif /* __BI_PORT_H__ */
