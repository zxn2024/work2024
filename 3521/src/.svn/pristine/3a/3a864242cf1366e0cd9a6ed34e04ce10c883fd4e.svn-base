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

#ifndef _DIGITAL_ATT_H
#define _DIGITAL_ATT_H

#include "MCU_config.h"

/*---------------------- 端口定义 --------------------------------------*/

/*---------------------- 宏定义  ---------------------------------------*/

/*---------------------- 结构体 ------------------------------------------*/

typedef struct __ATT_digital_hw_config_t
{
    PORT_T sck_port;        // sck port
    GPIO_PIN_T sck_pin;     // sck pin
    PORT_T sda_port;        // sda port
    GPIO_PIN_T sda_pin;     // sda pin
    PORT_T le_port;         // le port
    GPIO_PIN_T le_pin;      // le pin
    uint8_t att_max_number; // 可用衰减器数量，单位db
} ATT_digital_hw_config_t;

typedef struct __ATT_digital_t
{
    ATT_digital_hw_config_t hw_config;                                  // 配置初始化参数
    uint8_t attSetValue;                                                // 记录当前设置的值
    LOCK_STATE_E lock;                                                  // 锁
    Status_t (*open)(struct __ATT_digital_t * handle_att);              // 打开设备，初始化设备
    Status_t (*close)(struct __ATT_digital_t * handle_att);             // 关闭设备，注销资源
    Status_t (*set)(struct __ATT_digital_t * handle_att, uint8_t value); // 设置att值
} ATT_digital_t;

/*---------------------- 声明 ------------------------------------------*/

Status_t att_digital_register(ATT_digital_t * handle_att);
#endif
