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

#ifndef _PA_H
#define _PA_H

#include "MCU_config.h"
#include "bi_pa.h"
/*---------------------- 端口定义 --------------------------------------*/

/*---------------------- 宏定义  ---------------------------------------*/

/*---------------------- 结构体 ------------------------------------------*/

typedef struct __PA_HwConfig_t
{
    PORT_T pa_ctl_port;            // pa电源控制引脚定义
    GPIO_PIN_T pa_ctl_pin;         // pa电源控制引脚定义
    PA_CTL_E pa_init_stat;          // pa初始化后的初始状态
    uint8_t ctl_logic_invert;       // 控制逻辑翻转
    // 当为B_TRUE时,高电平使能,低电平关闭
    // 当为B_FALSE时,高电平关闭,低电平使能
    // 此处定义该设备所需的硬件接口（如GPIO、SPI等），资源（内存，缓冲区），查找表（功率表，衰减表，温度表）
} PA_HwConfig_t;

typedef struct __PA_TypeDef
{
    PA_HwConfig_t hw_config;                                                            // 初始化参数
    PA_CTL_E pa_ctl_stat;                                                                // pa当前电源状态 PWR_OFF (0) PWR_ON (1)
    Status_t (*open)(struct __PA_TypeDef * pPaHandle);                  // 设置电源控制
    Status_t (*ctl)(struct __PA_TypeDef * pPaHandle, PA_CTL_E pwr_ctl);                  // 设置电源控制
} PA_t;

/*---------------------- 声明 ------------------------------------------*/

Status_t pa_register(PA_t * hPa);
#endif
