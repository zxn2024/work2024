/**
 * **********************************************************************************
 * @copyright Copyright (c) 2023 Shaanxi Tianji Communication Technologies Co., Ltd.
 *                         
 * @file        switch_spdt_RFASWA630PTF06.h   
 * @author      
 * @version     None
 * @date        2023-11-20     
 * @brief       None   
 * 
 * **********************************************************************************
 */

#ifndef __SWITCH_SPDT_RFASWA630PTF06_H_
#define __SWITCH_SPDT_RFASWA630PTF06_H_

#ifdef __cplusplus
 extern "C" {
#endif


#include "MCU_config.h"
#include "bi_switch_spXt.h"
/*---------------------- 端口定义 --------------------------------------*/

/*---------------------- 宏定义  ---------------------------------------*/

/*---------------------- 结构体 ------------------------------------------*/

typedef struct __Switch_spdt_HwConfig_t
{
    PORT_T switch_ctl_port;            // switch控制引脚定义
    GPIO_PIN_T switch_ctl_pin;         // switch控制引脚定义
    SW_STATE_E switch_init_stat;          // switch初始化后的初始状态
    // 此处定义该设备所需的硬件接口（如GPIO、SPI等），资源（内存，缓冲区），查找表（功率表，衰减表，温度表）
} Switch_spdt_HwConfig_t;

typedef struct __Switch_spdt_TypeDef
{
    Switch_spdt_HwConfig_t hw_config;                                                            // 初始化参数
    SW_STATE_E switch_ctl_stat;                                                                // switch当前电源状态 PWR_OFF (0) PWR_ON (1)
    Status_t (*open)(struct __Switch_spdt_TypeDef * pSwitchHandle);                  // 设置电源控制
    Status_t (*ctl)(struct __Switch_spdt_TypeDef * pSwitchHandle, SW_STATE_E sw_ctl);                  // 设置电源控制
} Switch_spdt_t;

/*---------------------- 声明 ------------------------------------------*/

Status_t switch_spdt_register(struct __Switch_spdt_TypeDef * pSwitchHandle);

#ifdef __cplusplus
}
#endif

#endif
