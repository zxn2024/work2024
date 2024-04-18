/*************************************** Copyright ******************************
 *
 * (C) Copyright 2023, Shaanxi Tianji Communication
 * All Rights Reserved
 *
 * FileName   : bi_Switch.h
 * Version    : none
 * Author     :
 * Date       : 2023-09-21
 * Description: Switch的通用接口模版,提供统一的Switch接口:
 *              -BI_Switch_OPEN(obj)
 *              -BI_Switch_CLOSE(obj)
 *              -BI_Switch_SET(obj, val)
 * 开发Switch驱动时需要按照此模版进行
 *******************************************************************************/
//

#ifndef _BI_GPIO_H_
#define _BI_GPIO_H_

typedef enum
{
    SPST = 0,             //单刀单掷
    SPDT = 1,             //单刀双掷
    SP3T = 2,             //单刀三掷
    SP4T = 3,             //单刀四掷
} SW_TYPE_E;
typedef enum
{
    SW_OFF = 0,             //不接通
    SW_CH1 = 1,             //接通道1
    SW_CH2 = 2,             //接通道2
    SW_CH3 = 3,             //接通道3
    SW_CH4 = 4,             //接通道4
} SW_STATE_E;
/*---------------------- Switch驱动模版说明 ------------------------------------------*/
// Switch驱动模板说明
// -Switch驱动开发时，数据结构必须基于此接口模板，在此基础上新增变量和方法，但是不允许改动模板中已有的变量和方法。

/*
设备初始化结构体，包含MCU资源分配，比如GPIO分配，SPI端口，该设备的查找表等
typedef struct __Switch_xxxx_HwConfig_t
{
    Switch_Port:驱动中根据实际使用的端口类型和数量来定义
    SW_TYPE_E Switch_type; // switch type (spst spdt sptt spqt)
    SW_STATE_E Switch_init_state; // 默认状态
    //此处定义该设备所需的硬件接口（如GPIO、SPI等），资源（内存，缓冲区），查找表（功率表，衰减表，温度表）
} Switch_xxxx_HwConfig_t;

设备的数据结构，包含数据和所有对外接口
typedef struct __Switch_ClassTypeDef
{
    Switch_xxxx_HwConfig_t hw_config;                   // 初始化参数
    SW_STATE_E SwitchState;                           // 记录当前设置的值
    Status_t (*open)(Switch_xxxx_t *pSwitchHandle);                // 打开,设置到默认状态
    Status_t (*ctl)(Switch_xxxx_t *pSwitchHandle, SW_STATE_E stat);  // 设置Switch通道开关状态
} Switch_ClassTypeDef;

注册函数，用于注册一个设备并分配资源
Status_t Switch_xxxx_register(Switch_xxxx_t *handle_Switch);
*/

/*---------------------- 对外接口 ------------------------------------------*/

/*****************************************************************************
* @Name	  : BI_Switch_OPEN
* @brief  : 打开衰减器设备,函数原型 Status_t (*open)(void *SwitchHandle);
* @param  : pSwitchHandle: 衰减器设备句柄指针
* @Retval : Status_t
* @Note   : 用来给衰减器使能或上电(如果有的话),通常情况下衰减器无需打开,此函数为空
*/
#define BI_Switch_OPEN(pSwitchHandle) (pSwitchHandle)->open(pSwitchHandle)

/*****************************************************************************
* @Name	  : BI_Switch_SET
* @brief  : 设置衰减器,函数原型 Status_t (*set)(void *SwitchHandle, uint8_t value);
* @param  : pSwitchHandle: 衰减器设备句柄指针
* @param  : val 设置的衰减值
* @Retval : Status_t
* @Note   : 用来设置衰减器数值
*/
#define BI_Switch_CTL(pSwitchHandle, val) (pSwitchHandle)->ctl(pSwitchHandle, (val)) // Switch写衰减值

#endif /* __IWDG_H__ */
