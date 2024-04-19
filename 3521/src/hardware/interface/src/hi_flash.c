/***************************** Copyright ****************************************
*
* (C) Copyright 2023, Shaanxi Tianji Communication
*  All Rights Reserved
*                         
* FileName   : bsp_gpio_drv.c   
* Version    : none		
* Author     : 			
* Date       :        
* Description: none   
*******************************************************************************/


#include "hi_flash.h"

/*---------------------- 端口定义 --------------------------------------*/


/*---------------------- 宏定义  ---------------------------------------*/


/*---------------------- 结构体 ----------------------------------------*/


/*---------------------- 常量 ------------------------------------------*/


/*---------------------- 全局变量 --------------------------------------*/


/*---------------------- 局部变量 --------------------------------------*/


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
Status_t hi_flash_sector_erase(uint32_t sector)
{
#if(MCU_SERIES == MCU_GD32F4XX)
    /* unlock the flash program erase controller */
    fmc_unlock();
    /* clear pending flags */
    fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_OPERR | FMC_FLAG_WPERR | FMC_FLAG_PGMERR | FMC_FLAG_PGSERR);
    /* wait the erase operation complete*/
    if(FMC_READY != fmc_sector_erase(sector))
    {
        fmc_lock();
        return STAT_ERROR;
    }
    /* lock the flash program erase controller */
    fmc_lock();
    return STAT_OK;
#elif(MCU_SERIES == MCU_STM32F4XX)
    uint8_t i;
    uint8_t ucErrCnt = 0;
    uint32_t SectorError = 0;
    FLASH_EraseInitTypeDef FlashEraseInit;

    FlashEraseInit.TypeErase    = FLASH_TYPEERASE_SECTORS; //擦除类型：扇区擦除
    FlashEraseInit.Sector       = sector;                //扇区号
    FlashEraseInit.NbSectors    = 1;                       //一次只擦除一个扇区
    FlashEraseInit.VoltageRange = FLASH_VOLTAGE_RANGE_3;   //电压范围：2.7~3.6V

    HAL_FLASH_Unlock();

    while(HAL_FLASHEx_Erase(&FlashEraseInit, &SectorError) != HAL_OK)
    {
        ucErrCnt++;
        if(ucErrCnt >= 5)
        {
            HAL_FLASH_Lock();
            return STAT_ERROR;
        }
        for(i = 0; i < 254; i++) //delay 1 ms
        {
            ;
        }
    }

    HAL_FLASH_Lock();
    return STAT_OK;
#elif(MCU_SERIES == MCU_STM32H5XX)
#endif
}

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
Status_t hi_flash_page_erase(uint32_t start_address)
{
#if(MCU_SERIES == MCU_GD32F4XX)
    /* unlock the flash program erase controller */
    fmc_unlock();
    /* clear pending flags */
    fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_OPERR | FMC_FLAG_WPERR | FMC_FLAG_PGMERR | FMC_FLAG_PGSERR);
    /* wait the erase operation complete*/
    if(FMC_READY != fmc_page_erase(start_address))
    {
        //while(1);
        return STAT_ERROR;
    }
    /* lock the flash program erase controller */
    fmc_lock();
    return STAT_OK;
#elif(MCU_SERIES == MCU_STM32F4XX)
    return STAT_ERROR;
#elif(MCU_SERIES == MCU_STM32H5XX)
    return STAT_ERROR;
#endif
}

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
Status_t hi_flash_write(uint32_t addr, uint8_t * pbuf, uint32_t len)
{
#if(MCU_SERIES == MCU_GD32F4XX)
    uint16_t i;//StartSector, EndSector, i;
    /* unlock the flash program erase controller */
    fmc_unlock();
    /* clear pending flags */
    //fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_OPERR | FMC_FLAG_WPERR | FMC_FLAG_PGMERR | FMC_FLAG_PGSERR);
    /* get the number of the start and end sectors */
    //StartSector = flash_sector_get(addr);
    //EndSector = flash_sector_get(addr + len);
    /* each time the sector number increased by 8, refer to the sector definition */
    //for(i = StartSector; i <= EndSector; i += 8)
    //{
    //    if(FMC_READY != fmc_sector_erase(i))
    //    {
    //        //while(1);
    //        return STAT_ERROR;
    //    }
    //}
    /* write data_8 to the corresponding address */
    for(i = 0; i < len; i++)
    {
        if(FMC_READY == fmc_byte_program(addr, pbuf[i]))
        {
            addr = addr + 1;
        }
        else
        {
            fmc_lock();
            return STAT_ERROR;
        }
    }
    /* lock the flash program erase controller */
    fmc_lock();
    return STAT_OK;
#elif(MCU_SERIES == MCU_STM32F4XX)
    uint16_t i;
    Status_t res = STAT_OK;
    HAL_StatusTypeDef FlashStatus = HAL_OK;

    if(NULL == pbuf)
        return STAT_ERROR;

    if(!IS_FLASH_ADDRESS(addr))
        return STAT_ERROR;

    HAL_FLASH_Unlock();

    for(i = 0; i < len; i++)
    {
        FlashStatus = HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, addr + i, *(pbuf + i));
        if(FlashStatus != HAL_OK)
        {
            res = STAT_ERROR;
            break;
        }
    }

    HAL_FLASH_Lock();
    return res;
#elif(MCU_SERIES == MCU_STM32H5XX)
#endif
}

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
Status_t hi_flash_read(uint32_t addr, uint8_t * pbuf, uint32_t len)
{
#if(MCU_SERIES == MCU_GD32F4XX)
    uint32_t i;

    if(NULL == pbuf)
        return STAT_ERROR;

    if(addr > ADDR_FMC_END)
        return STAT_ERROR;

    for(i = 0; i < len; i++)
    {
        pbuf[i] = *(__IO int8_t *)addr;
        addr = addr + 1;
    }
    return STAT_OK;
#elif(MCU_SERIES == MCU_STM32F4XX)
    uint16_t i;
    uint8_t temp;

    if(NULL == pbuf)
        return STAT_ERROR;

    if(!IS_FLASH_ADDRESS(addr))
        return STAT_ERROR;

    for(i = 0; i < len; i++)
    {
        temp = *(uint8_t *)(addr + i);
        *(pbuf + i) = temp;
    }
    return STAT_OK;
#elif(MCU_SERIES == MCU_STM32H5XX)
#endif
}


