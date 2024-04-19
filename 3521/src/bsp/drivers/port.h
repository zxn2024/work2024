/*************************************** Copyright ******************************
 *
 * (C) Copyright 2023, Shaanxi Tianji Communication
 * All Rights Reserved
 *
 * FileName   : port.h
 * Version    : none
 * Author     :
 * Date       : 2023-02-10
 * Description: none
 *******************************************************************************/
//

#ifndef _PORT_H
#define _PORT_H

#include "MCU_config.h"
#include "bi_port.h"
/*---------------------- 端口定义 --------------------------------------*/

/*---------------------- 宏定义  ---------------------------------------*/

/*---------------------- 结构体 ------------------------------------------*/

typedef struct __PORT_HwConfig_t
{
    PORT_T port_ctl_port;           // port电源控制引脚定义
    GPIO_PIN_T port_ctl_pin;        // port电源控制引脚定义
    PORT_CTL_E port_init_stat;      // port初始化后的初始状态
} PORT_HwConfig_t;

typedef struct __PORT_TypeDef
{
    PORT_HwConfig_t hw_config;                                              // 初始化参数
    PORT_CTL_E port_ctl_stat;                                               // port当前状态 PWR_OFF (0) PWR_ON (1)
    Status_t (*open)(struct __PORT_TypeDef * pPaHandle);                    // 设置电源控制
    Status_t (*ctl)(struct __PORT_TypeDef * pPaHandle, PORT_CTL_E pwr_ctl); // 设置控制
} PORT_t;

/*---------------------- 声明 ------------------------------------------*/

Status_t port_register(PORT_t * hPa);
#endif
