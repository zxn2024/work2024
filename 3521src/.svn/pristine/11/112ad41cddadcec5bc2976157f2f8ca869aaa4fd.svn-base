/*************************************** Copyright ******************************
*
* (C) Copyright 2023, Shaanxi Tianji Communication
* All Rights Reserved
*                         
* FileName   : hi_gpio.h   
* Version    : none		
* Author     : 			
* Date       : 2023-05-26         
* Description: GPIO的通用操作接口，支持多型号MCU，MCU列表请看MCU_config.h
    
*******************************************************************************/
//
#ifndef __HI_GPIO_H_
#define __HI_GPIO_H_

#include "gpio.h"
#include "MCU_config.h"


/*****************************************************************************
* @Name	  : hiGpioInit
* @Brief  : GPIO初始化函数
* @Retval : 
* @Author : 
* @Data   : 
* @Note   : 
*****************************************************************************/
void hi_gpio_init(void);

/*****************************************************************************
* @Name	  : hiGpioWrite
* @brief  : 设置管脚状态
* @param  : port ：端口号
* @param  : pin ：引脚号
* @param  : gpioStat ：GPIO引脚状态
            -LEVEL_L 低电平
            -LEVEL_H 高电平
* @retval : None
* @author : 
* @Data   : 
* @Note   :
*****************************************************************************/
void hi_gpio_write(PORT_T port,GPIO_PIN_T pin, ELEC_LEVEL_E gpioStat);

/*****************************************************************************
* @Name	  : hiGpioRead
* @brief  : 读取管脚状态
* @param  : port ：端口号
* @param  : pin ：引脚号
* @Retval  : gpioStat ：GPIO引脚状态
            -LEVEL_L 低电平
            -LEVEL_H 高电平
* @author :
* @Data   :
* @Note   :
*****************************************************************************/
ELEC_LEVEL_E hi_gpio_read(PORT_T port,GPIO_PIN_T pin);

/*****************************************************************************
* @Name	  : hiGpioInvert
* @brief  : 翻转管脚状态
* @param  : port ：端口号
* @param  : pin ：引脚号
* @author :
* @Data   : 
* @Note   :
*****************************************************************************/
void hi_gpio_toggle(PORT_T port,GPIO_PIN_T pin);
#endif
