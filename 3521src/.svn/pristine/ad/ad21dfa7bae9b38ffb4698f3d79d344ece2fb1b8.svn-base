/*************************************** Copyright ******************************
 *
 * (C) Copyright 2023, Shaanxi Tianji Communication
 * All Rights Reserved
 *
 * FileName   : bsp_public_macro.h
 * Version    : none
 * Author     :
 * Date       : 2023-02-10
 * Description: none
 *******************************************************************************/
//

#ifndef _ANALOG_ATT_H
#define _ANALOG_ATT_H

#include "MCU_config.h"
#include "hi_tim.h"
#include "bi_att.h"
/*---------------------- 端口定义 --------------------------------------*/


/*---------------------- 宏定义  ---------------------------------------*/


/*---------------------- 结构体 ----------------------------------------*/
typedef struct __ATT_analog_data_t
{
    int8_t   min_number;        // 最小衰减值
	int8_t   cali_value;		// 校准值
    uint8_t  valid_number;      // 衰减有效数量
	uint8_t  step;				// 步进, 0:1dB, 1:0.5dB
    uint16_t table[94];
} Analog_table_t;


typedef struct __ATT_analog_config_t
{
    TIM_T tim_ch;           // PWM通道
    PWM_CH_T pwm_ch;        // PWM通道
    uint8_t att_min_number; // 最小衰减值，单位db
    uint8_t att_max_number; // 最大衰减值，单位db
    uint16_t (*att_table);  // 模拟衰减器衰减表
} ATT_analog_hw_config_t;

typedef struct __ATT_analog_t
{
    ATT_analog_hw_config_t hw_config;               // 配置初始化参数
    uint8_t attSetValue;                           // 记录当前设置的值
    Status_t (*open)(struct __ATT_analog_t * handle_att);               // 打开设备，初始化设备
    Status_t (*close)(struct __ATT_analog_t * handle_att);              // 关闭设备，注销资源
    Status_t (*set)(struct __ATT_analog_t * handle_att, uint8_t value); // 设置att值
    Status_t (*float_set)(struct __ATT_analog_t * handle_att, float value); // 设置att值,浮点数
    Status_t (*pwm_set)(struct __ATT_analog_t * handle_att, uint16_t pwm_val); // 设置pwm值
    uint16_t (*pwm_get)(struct __ATT_analog_t * handle_att); // 获取pwm值
} ATT_analog_t;
/*---------------------- 声明 ------------------------------------------*/


Status_t att_analog_register(ATT_analog_t * handle_att);
#endif
