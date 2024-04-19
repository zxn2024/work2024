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

#ifndef _LNA_H
#define _LNA_H

#include "MCU_config.h"
#include "bi_lna.h"
/*---------------------- 端口定义 --------------------------------------*/

/*---------------------- 宏定义  ---------------------------------------*/

/*---------------------- 结构体 ------------------------------------------*/

typedef struct __LNA_HwConfig_t
{
    PORT_T lna_byp_port;            // lna旁路引脚定义
    GPIO_PIN_T lna_byp_pin;         // lna旁路引脚定义
    PORT_T lna_pwr_port;            // lna电源控制引脚定义
    GPIO_PIN_T lna_pwr_pin;         // lna电源控制引脚定义
    LNA_BYPASS_E lna_init_byp_stat; // lna初始化后的bypass初始状态
    LNA_PWR_E lna_init_pwr_stat;    // lna初始化后的power初始状态
    uint8_t byp_logic_invert;       // 控制逻辑翻转
    // 当为B_TRUE时,高电平使能bypass,低电平关闭bypass
    // 当为B_FALSE时,高电平关闭bypass,低电平使能bypass
    uint8_t pwr_logic_invert;       // 控制逻辑翻转
    // 当为B_TRUE时,高电平使能,低电平关电
    // 当为B_FALSE时,高电平关电,低电平使能
    uint8_t bypass_att;             // 旁路模式的等效衰减值
    // 此处定义该设备所需的硬件接口（如GPIO、SPI等），资源（内存，缓冲区），查找表（功率表，衰减表，温度表）
} LNA_HwConfig_t;

typedef struct __LNA_TypeDef
{
    LNA_HwConfig_t hw_config;                                                   // 初始化参数
    LNA_BYPASS_E lna_byp_stat;                                                  // lna当前旁路状态 BYP_OFF (0) BYP_ON (1)
    LNA_PWR_E lna_pwr_stat;                                                     // lna当前电源状态 PWR_OFF (0) PWR_ON (1)
    Status_t (*open)(struct __LNA_TypeDef * pLnaHandle);                        // 打开lna设备
    Status_t (*set_bypass)(struct __LNA_TypeDef * pLnaHandle, LNA_BYPASS_E byp); // 设置旁路
    Status_t (*set_pwr)(struct __LNA_TypeDef * pLnaHandle, LNA_PWR_E pwr_ctl);  // 设置电源控制
} LNA_t;

/*---------------------- 声明 ------------------------------------------*/

Status_t lna_register(LNA_t * hLna);
#endif
