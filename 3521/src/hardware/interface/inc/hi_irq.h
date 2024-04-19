/*************************************** Copyright ******************************
*
* (C) Copyright 2023, Shaanxi Tianji Communication
* All Rights Reserved
*                         
* FileName   : hi_irq.h   
* Version    : none		
* Author     : 			
* Date       : 2023-05-26         
* Description: IRQ的通用操作接口，支持多型号MCU，MCU列表请看MCU_config.h
    
*******************************************************************************/
//
#ifndef __HI_IRQ_H_
#define __HI_IRQ_H_

#include "hi_irq.h"
#include "MCU_config.h"

/*****************************************************************************
* @Name	  : hi_irq_on
* @Brief  : 使能中断
* @param  : irq 中断
* @param  : priority 优先级,仅主优先级,次优先级默认为0
* @Author :
* @Data   :
* @Note   :

*****************************************************************************/
void hi_irq_on(IRQ_T irq, uint8_t priority);


/*****************************************************************************
* @Name	  : hiGpioWrite
* @brief  : 失能中断
* @param  :
* @retval : None
* @author :
* @Data   :
* @Note   :
* @Note   :
*****************************************************************************/
void hi_irq_off(IRQ_T irq);

#endif
