/*************************************** Copyright ******************************
*
* (C) Copyright 2023, Shaanxi Tianji Communication
* All Rights Reserved
*                         
* FileName   : hi_system.h   
* Version    : none		
* Author     : 			
* Date       : 2023-05-26         
* Description: 时钟和系统初始化的通用接口，支持多型号MCU，MCU列表请看MCU_config.h
    
*******************************************************************************/
//
#ifndef __HI_SYSTEM_H__
#define __HI_SYSTEM_H__

#include "MCU_config.h"

/*---------------------- 端口定义 --------------------------------------*/
#if(MCU_SERIES == MCU_GD32F4XX)

#elif(MCU_SERIES == MCU_STM32F4XX || MCU_SERIES == MCU_STM32H5XX)

#endif
/*---------------------- 宏定义  ---------------------------------------*/


/*---------------------- 声明 ------------------------------------------*/


/*****************************************************************************
* @Name	  : hiSpiRead
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
void hi_enable_irq(void);

/*****************************************************************************
* @Name	  : hiSpiWrite
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
void hi_disable_irq(void);

/*****************************************************************************
* @Name	  : hiSpiRead
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
void hi_chip_init(void);

/*****************************************************************************
* @Name	  : hiSpiRead
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
void hi_vecttab_offset(void);


/*MCU重启*/
void hi_reboot_mcu(void);

#endif
