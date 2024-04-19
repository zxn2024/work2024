/*************************************** Copyright ******************************
*
* (C) Copyright 2023, Shaanxi Tianji Communication
* All Rights Reserved
*                         
* FileName   : hi_flash.h   
* Version    : none		
* Author     : 			
* Date       : 2023-05-26         
* Description: flash的通用操作接口，支持多型号MCU，MCU列表请看MCU_config.h
    
*******************************************************************************/
//
#ifndef __HI_FLASH_H__
#define __HI_FLASH_H__

#include "MCU_config.h"

/*---------------------- flash地址 --------------------------------------*/
#if(MCU_MODEL == MCU_GD32F427ZET6 || MCU_MODEL == MCU_GD32F427VET6)
#include "flash.h"
    #define ADDR_FLASH_SECTOR_0   		   ADDR_FMC_SECTOR_0   
    #define ADDR_FLASH_SECTOR_1   		   ADDR_FMC_SECTOR_1   
    #define ADDR_FLASH_SECTOR_2   		   ADDR_FMC_SECTOR_2   
    #define ADDR_FLASH_SECTOR_3    		   ADDR_FMC_SECTOR_3   
    #define ADDR_FLASH_SECTOR_4    		   ADDR_FMC_SECTOR_4   
    #define ADDR_FLASH_SECTOR_5    		   ADDR_FMC_SECTOR_5   
    #define ADDR_FLASH_SECTOR_6    		   ADDR_FMC_SECTOR_6   
    #define ADDR_FLASH_SECTOR_7    		   ADDR_FMC_SECTOR_7   
    #define FLASH_ADDR_END                 ADDR_FMC_END        

    #define FLASH_SECTOR_NUM_0     		    CTL_SECTOR_NUMBER_0   // 16K
    #define FLASH_SECTOR_NUM_1  		    CTL_SECTOR_NUMBER_1   // 16K   
    #define FLASH_SECTOR_NUM_2 		        CTL_SECTOR_NUMBER_2   // 16K   
    #define FLASH_SECTOR_NUM_3     		    CTL_SECTOR_NUMBER_3   // 16K   
    #define FLASH_SECTOR_NUM_4  		    CTL_SECTOR_NUMBER_4   // 64K  
    #define FLASH_SECTOR_NUM_5 		        CTL_SECTOR_NUMBER_5   // 128K  
    #define FLASH_SECTOR_NUM_6 		        CTL_SECTOR_NUMBER_6   // 128K  
    #define FLASH_SECTOR_NUM_7 		        CTL_SECTOR_NUMBER_7   // 128K  
	
#elif( MCU_MODEL == MCU_GD32F470ZET6 || MCU_MODEL == MCU_GD32F470VET6)

#elif( MCU_MODEL == MCU_STM32F407VET6 || (MCU_MODEL == MCU_STM32F407VET6))

    #define ADDR_FLASH_SECTOR_0   		   ((uint32_t)0x08000000)   // 16K
    #define ADDR_FLASH_SECTOR_1   		   ((uint32_t)0x08004000)   // 16K
    #define ADDR_FLASH_SECTOR_2   		   ((uint32_t)0x08008000)   // 16K
    #define ADDR_FLASH_SECTOR_3    		   ((uint32_t)0x0800C000)   // 16K
    #define ADDR_FLASH_SECTOR_4    		   ((uint32_t)0x08010000)   // 64K
    #define ADDR_FLASH_SECTOR_5    		   ((uint32_t)0x08020000)   // 128K
    #define ADDR_FLASH_SECTOR_6    		   ((uint32_t)0x08040000)   // 128K
    #define ADDR_FLASH_SECTOR_7    		   ((uint32_t)0x08060000)   // 128K
    #define FLASH_ADDR_END                 ((uint32_t)0x08080000)   // 128K

    #define FLASH_SECTOR_NUM_0     		    FLASH_SECTOR_0
    #define FLASH_SECTOR_NUM_1  		    FLASH_SECTOR_1
    #define FLASH_SECTOR_NUM_2 		        FLASH_SECTOR_2
    #define FLASH_SECTOR_NUM_3     		    FLASH_SECTOR_3
    #define FLASH_SECTOR_NUM_4  		    FLASH_SECTOR_4
    #define FLASH_SECTOR_NUM_5 		        FLASH_SECTOR_5
    #define FLASH_SECTOR_NUM_6 		        FLASH_SECTOR_6
    #define FLASH_SECTOR_NUM_7 		        FLASH_SECTOR_7


#elif( MCU_MODEL == MCU_STM32H562ZIT6 || MCU_MODEL == MCU_STM32H562VIT6)


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
Status_t hi_flash_sector_erase(uint32_t sector);

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
Status_t hi_flash_page_erase(uint32_t start_address);

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
Status_t hi_flash_write(uint32_t addr, uint8_t *pbuf, uint32_t len);

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
Status_t hi_flash_read(uint32_t addr, uint8_t *pbuf, uint32_t len);




#endif
