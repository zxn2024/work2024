/*************************************** Copyright ******************************
 *
 * (C) Copyright 2023, Shaanxi Tianji Communication
 * All Rights Reserved
 *
 * FileName   : bi_att.h
 * Version    : none
 * Author     :
 * Date       : 2023-09-21
 * Description: att的通用接口模版,提供统一的att接口:
 *              -BI_ATT_OPEN(obj) 
 *              -BI_ATT_CLOSE(obj)
 *              -BI_ATT_SET(obj, val)
 * 开发att驱动时需要按照此模版进行
 *******************************************************************************/
//

#ifndef _BI_ATT_H_
#define _BI_ATT_H_


/*---------------------- ATT驱动模版说明 ------------------------------------------*/
// ATT驱动模板说明
// -ATT驱动开发时，数据结构必须基于此接口模板，在此基础上新增变量和方法，但是不允许改动模板中已有的变量和方法。

/*
设备初始化结构体，包含MCU资源分配，比如GPIO分配，SPI端口，该设备的查找表等
typedef struct __ATT_xxxx_HwConfig_t
{
    ATT_Port:驱动中根据实际使用的端口类型和数量来定义
    uint8_t att_max_number; // 可用衰减器数量，单位db
    //此处定义该设备所需的硬件接口（如GPIO、SPI等），资源（内存，缓冲区），查找表（功率表，衰减表，温度表）
} ATT_xxxx_HwConfig_t;

设备的数据结构，包含数据和所有对外接口
typedef struct __ATT_ClassTypeDef
{
    ATT_xxxx_HwConfig_t hw_config;                   // 初始化参数
    uint8_t attSetValue;                           // 记录当前设置的值
    Status_t (*open)(ATT_xxxx_t *pAttHandle);                // 打开
    Status_t (*close)(ATT_xxxx_t *pAttHandle);               // 关闭
    Status_t (*set)(ATT_xxxx_t *pAttHandle, uint8_t value); // 设置att值
    Status_t (*set_pwm)(ATT_xxxx_t *pAttHandle, uint16_t value); // 设置模拟衰减器的PWM比较值
} ATT_ClassTypeDef;

注册函数，用于注册一个设备并分配资源
Status_t att_xxxx_register(ATT_xxxx_t *handle_att);
*/

/*---------------------- 对外接口 ------------------------------------------*/

/*****************************************************************************
* @Name	  : BI_ATT_OPEN
* @brief  : 打开衰减器设备,函数原型 Status_t (*open)(void *attHandle);
* @param  : pAttHandle: 衰减器设备句柄指针
* @Retval : Status_t
* @Note   : 用来给打开衰减器设备，衰减器使能或上电(如果有的话),并初始化设置为最大衰减值
*/
#define BI_ATT_OPEN(pAttHandle) (pAttHandle)->open(pAttHandle) 

/*****************************************************************************
* @Name	  : BI_ATT_CLOSE
* @brief  : 关闭衰减器设备,函数原型 Status_t (*close)(void *pAttHandle);
* @param  : pAttHandle: 衰减器设备句柄指针
* @Retval : Status_t
* @Note   : 用来给衰减器关闭或下电(如果有的话),通常情况下衰减器无需打开或关闭,此函数为空
*/
#define BI_ATT_CLOSE(pAttHandle) (pAttHandle)->close(pAttHandle)         // ATT设备关闭

/*****************************************************************************
* @Name	  : BI_ATT_SET
* @brief  : 设置衰减器,函数原型 Status_t (*set)(void *attHandle, uint8_t value);
* @param  : pAttHandle: 衰减器设备句柄指针
* @param  : val 设置的衰减值
* @Retval : Status_t
* @Note   : 用来设置衰减器数值
*/
#define BI_ATT_SET(pAttHandle, val) (pAttHandle)->set((pAttHandle), (val)) // ATT写衰减值

/*****************************************************************************
* @Name	  : BI_PWM_SET
* @brief  : 设置衰减器,函数原型 Status_t (*set)(void *attHandle, uint8_t value);
* @param  : pAttHandle: 衰减器设备句柄指针
* @param  : val 设置的衰减值
* @Retval : Status_t
* @Note   : 用来设置衰减器数值
*/
#define BI_ATT_PWM_SET(pAttHandle, pwm_val) (pAttHandle)->pwm_set((pAttHandle), (pwm_val)) // ATT写衰减值

#endif /* __IWDG_H__ */
