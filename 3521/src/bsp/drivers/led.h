/*************************************** Copyright ******************************
 *
 * (C) Copyright 2023, Shaanxi Tianji Communication
 * All Rights Reserved
 *
 * FileName   : 
 * Version    : none
 * Author     :
 * Date       : 2023-02-10
 * Description: none
 *******************************************************************************/
//

#ifndef _LED_H
#define _LED_H

#include "MCU_config.h"
#include "bi_led.h"
/*---------------------- 端口定义 --------------------------------------*/


/*---------------------- 宏定义  ---------------------------------------*/


/*---------------------- 结构体 ------------------------------------------*/

typedef struct __LED_HwConfig_t
{
    PORT_T led_port;    // led控制引脚定义
    GPIO_PIN_T led_pin; // led控制引脚定义
    LED_STAT led_init_stat;  //led初始化后的初始状态
    uint8_t ctl_logic_invert;  //控制逻辑翻转,当为B_TRUE时,高电平亮灯,低电平灭灯,为B_FALSE时,高电平灭灯,低电平亮灯
    //此处定义该设备所需的硬件接口（如GPIO、SPI等），资源（内存，缓冲区），查找表（功率表，衰减表，温度表）
} LED_HwConfig_t;

typedef struct __LED_TypeDef
{
    LED_HwConfig_t hw_config;                                                       // 初始化参数
    LED_STAT led_stat;                                                              // led当前状态 LED_OFF (0) LED_ON (1) LED_FLASH (2)
    uint32_t flash_cycle;                                                           // led闪灯周期(当闪灯状态为LED_FLASH时生效)
    Status_t (*set)(struct __LED_TypeDef * pLedHandle, LED_STAT stat, uint32_t cycle);      // 设置led工作状态,及闪灯周期
    Status_t (*run)(struct __LED_TypeDef * pLedHandle);                                     // led闪灯执行函数
} LED_t;


/*---------------------- 声明 ------------------------------------------*/

Status_t led_register(LED_t * pLedHandle);
#endif
