/*************************************** Copyright ******************************
 *
 * (C) Copyright 2023, Shaanxi Tianji Communication
 * All Rights Reserved
 *
 * FileName   : bi_led.h
 * Version    : none
 * Author     :
 * Date       : 2023-09-21
 * Description: led的通用接口模版,提供统一的led接口:
 *              -BI_LED_SET(obj,stat,cycle)
 *              -BI_LED_RUN(obj)
 * 开发led驱动时需要按照此模版进行
 *******************************************************************************/
//

#ifndef _BI_LED_H_
#define _BI_LED_H_

/*---------------------- LED驱动模版说明 ------------------------------------------*/
// LED驱动模板说明
// -LED驱动开发时，数据结构必须基于此接口模板，在此基础上新增变量和方法，但是不允许改动模板中已有的变量和方法。

/*
设备初始化结构体，包含MCU资源分配，比如GPIO分配，SPI端口，该设备的查找表等
typedef struct __LED_xxxx_HwConfig_t
{
    LED_Port:驱动中根据实际使用的端口类型和数量来定义
    uint8_t led_init_stat;  //led初始化后的初始状态
    //此处定义该设备所需的硬件接口（如GPIO、SPI等），资源（内存，缓冲区），查找表（功率表，衰减表，温度表）
} LED_xxxx_HwConfig_t;

设备的数据结构，包含数据和所有对外接口
typedef struct __LED_ClassTypeDef
{
    LED_xxxx_HwConfig_t hw_config;                                                // 初始化参数
    LED_STAT led_stat;                                                             // led当前状态 LED_OFF (0) LED_ON (1) LED_FLASH (2)
    uint32_t flash_cycle;                                                         // led闪灯周期(当闪灯状态为LED_FLASH时生效)
    Status_t (*set)(LED_xxxx_t *pLedHandle, LED_STAT stat, uint32_t cycle);        // 设置led工作状态,及闪灯周期
    Status_t (*run)(LED_xxxx_t *pLedHandle);                                      // led闪灯执行函数
} LED_ClassTypeDef;

注册函数，用于注册一个设备并分配资源
Status_t led_xxxx_register(LED_xxxx_t *handle_led);
*/
/*---------------------- 端口定义 --------------------------------------*/

/*---------------------- 宏定义  ---------------------------------------*/

typedef enum
{
    LED_OFF = 0,             //电源/使能关闭
    LED_ON = 1,              //电源/使能打开
    LED_FLASH = 2,
}LED_STAT;
/*---------------------- 对外接口 --------------------------------------*/


/*****************************************************************************
 * @Name	  : BI_LED_SET
 * @brief  : 设置LED工作模式,函数原型 Status_t (*set)(LED_xxxx_t *pLedHandle, LED_STAT stat, uint32_t cycle);
 * @param  : pLedHandle: led灯句柄指针
 * @param  : stat led状态 LED_OFF (0) LED_ON (1) LED_FLASH (2)
 * @param  : cycle 闪灯周期,单位是ms,比如设置为1000,则亮500ms,灭500ms
 * @Retval : Status_t
 * @Note   : 用来设置led周期闪灯
 */
#define BI_LED_SET(pLedHandle, stat, cycle) (pLedHandle)->set((pLedHandle), (stat), (cycle)) // LED设置闪灯周期

/*****************************************************************************
 * @Name	  : BI_LED_RUN
 * @brief  : 设置衰减器,函数原型 Status_t (*run)(LED_xxxx_t *pLedHandle);
 * @Retval : Status_t
 * @Note   : led闪灯执行函数
 */
#define BI_LED_RUN(pLedHandle) (pLedHandle)->run(pLedHandle) // LED设置闪灯周期

#endif /* __BI_LED_H__ */
