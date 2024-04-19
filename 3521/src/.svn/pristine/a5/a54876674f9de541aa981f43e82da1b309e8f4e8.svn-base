/*************************************** Copyright ******************************
*
* (C) Copyright 2023, Shaanxi Tianji Communication
* All Rights Reserved
*                         
* FileName   : hi_systick.h   
* Version    : none		
* Author     : 			
* Date       : 2023-05-26         
* Description: systick的通用操作接口，支持多型号MCU，MCU列表请看MCU_config.h
    
*******************************************************************************/
//
#ifndef __HI_SYSTICK_H__
#define __HI_SYSTICK_H__

#include "MCU_config.h"

/*---------------------- 端口定义 --------------------------------------*/
#if(MCU_SERIES == MCU_GD32F4XX)

#elif(MCU_SERIES == MCU_STM32F4XX || MCU_SERIES == MCU_STM32H5XX)

#endif
/*---------------------- 宏定义  ---------------------------------------*/


/*---------------------- 声明 ------------------------------------------*/


/*****************************************************************************
* @Name	  : hi_systick_init
* @brief  : Spi 读函数
* @param  : ch ：端口号,见上方端口定义
* @param  : pBuf ：数据缓冲区
* @param  : len ：数据长度
* @param  : timeout ：超时时间
* @Retval  : Status_t
* @author :
* @Data   :
* @Note   :
*****************************************************************************/
void hi_systick_init(void);

/*****************************************************************************
* @Name	  : hi_get_tick
* @brief  : Spi 写函数
* @param  : ch ：端口号,见上方端口定义
* @param  : pBuf ：数据缓冲区
* @param  : len ：数据长度
* @param  : timeout ：超时时间
* @Retval  : Status_t
* @author : 
* @Data   : 
* @Note   :
*****************************************************************************/
uint32_t hi_get_tick(void);

/*****************************************************************************
* @Name	  : hi_delay_ms
* @brief  : Spi 写函数
* @param  : ch ：端口号,见上方端口定义
* @param  : pBuf ：数据缓冲区
* @param  : len ：数据长度
* @param  : timeout ：超时时间
* @Retval  : Status_t
* @author : 
* @Data   : 
* @Note   :
*****************************************************************************/
void hi_delay_ms(uint32_t ms);





#endif
