/**
 * **********************************************************************************
 * @copyright Copyright (c) 2023 Shaanxi Tianji Communication Technologies Co., Ltd.
 *                         
 * @file        flash.c   
 * @author      WangJun
 * @version     None
 * @date        2023-09-05     
 * @brief       None   
 * 
 * **********************************************************************************
 */

#include "flash.h"


/*---------------------- 端口定义 ----------------------*/


/*---------------------- 宏 定 义 ----------------------*/
/*需要跟public_types.h文件中BOOL_E保持一致*/
#define FLASH_OK    0   // 正常
#define FLASH_ERR   1   // 异常/错误

/*---------------------- 结 构 体 ----------------------*/


/*---------------------- 常    量 ----------------------*/


/*---------------------- 全局变量 ----------------------*/


/*---------------------- 局部变量 ----------------------*/


/*---------------------- 函    数 ----------------------*/
/*****************************************************************************
* @brief  : 获取扇区号
* @param  : address: 地址(0x08000000~0x0807FFFF)
* @retval : 扇区号
* @date   : 2023-09-05
* @note   : 
*****************************************************************************/
uint32_t flash_sector_get(uint32_t address)
{
    uint32_t sector = 0;
    if((address < ADDR_FMC_SECTOR_1) && (address >= ADDR_FMC_SECTOR_0))
    {
        sector = CTL_SECTOR_NUMBER_0;
    }
    else if((address < ADDR_FMC_SECTOR_2) && (address >= ADDR_FMC_SECTOR_1))
    {
        sector = CTL_SECTOR_NUMBER_1;
    }
    else if((address < ADDR_FMC_SECTOR_3) && (address >= ADDR_FMC_SECTOR_2))
    {
        sector = CTL_SECTOR_NUMBER_2;
    }
    else if((address < ADDR_FMC_SECTOR_4) && (address >= ADDR_FMC_SECTOR_3))
    {
        sector = CTL_SECTOR_NUMBER_3;
    }
    else if((address < ADDR_FMC_SECTOR_5) && (address >= ADDR_FMC_SECTOR_4))
    {
        sector = CTL_SECTOR_NUMBER_4;
    }
    else if((address < ADDR_FMC_SECTOR_6) && (address >= ADDR_FMC_SECTOR_5))
    {
        sector = CTL_SECTOR_NUMBER_5;
    }
    else if((address < ADDR_FMC_SECTOR_7) && (address >= ADDR_FMC_SECTOR_6))
    {
        sector = CTL_SECTOR_NUMBER_6;
    }
    else
    {
        sector = CTL_SECTOR_NUMBER_7;
    }
    return sector;
}


/*****************************************************************************
* @brief  : 读1个字节
* @param  : address: 地址(0x08000000~0x0807FFFF)
*		    length: 长度
*		    data_8: 数据指针
* @retval : None
* @date   : 2023-09-05
* @note   : 
*****************************************************************************/
void flash_read_8bit(uint32_t address, uint16_t length, uint8_t * data_8)
{
    uint16_t i;
    for(i = 0; i < length; i++)
    {
        data_8[i] = *(__IO int8_t *)address;
        address = address + 1;
    }
}


/*****************************************************************************
* @brief  : 读2个字节
* @param  : address: 地址(0x08000000~0x0807FFFF)
*		    length: 长度
*		    data_16: 数据指针
* @retval : None
* @date   : 2023-09-05
* @note   : 
*****************************************************************************/
void flash_read_16bit(uint32_t address, uint16_t length, uint16_t * data_16)
{
    uint16_t i;
    for(i = 0; i < length; i++)
    {
        data_16[i] = *(__IO int16_t *)address;
        address = address + 2;
    }
}


/*****************************************************************************
* @brief  : 读4个字节
* @param  : address: 地址(0x08000000~0x0807FFFF)
*		    length: 长度
*		    data_32: 数据指针
* @retval : None
* @date   : 2023-09-05
* @note   : 
*****************************************************************************/
void flash_read_32bit(uint32_t address, uint16_t length, uint32_t * data_32)
{
    uint16_t i;
    for(i = 0; i < length; i++)
    {
        data_32[i] = *(__IO int32_t *)address;
        address = address + 4;
    }
}


/*****************************************************************************
* @brief  : 写1个字节
* @param  : address: 地址(0x08000000~0x0807FFFF)
*		    length: 长度
*		    data_8: 数据指针
* @retval : FLASH_OK / FLASH_ERR
* @date   : 2023-09-05
* @note   : 
*****************************************************************************/
uint8_t flash_wirte_8bit(uint32_t address, uint16_t length, uint8_t * data_8)
{
    uint16_t StartSector, EndSector, i;
    /* unlock the flash program erase controller */
    fmc_unlock();
    /* clear pending flags */
    fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_OPERR | FMC_FLAG_WPERR | FMC_FLAG_PGMERR | FMC_FLAG_PGSERR);
    /* get the number of the start and end sectors */
    StartSector = flash_sector_get(address);
    EndSector = flash_sector_get(address + length);
    /* each time the sector number increased by 8, refer to the sector definition */
    for(i = StartSector; i <= EndSector; i += 8)
    {
        if(FMC_READY != fmc_sector_erase(i))
        {
            //while(1);
            return FLASH_ERR;
        }
    }
    /* write data_8 to the corresponding address */
    for(i = 0; i < length; i++)
    {
        if(FMC_READY == fmc_byte_program(address, data_8[i]))
        {
            address = address + 1;
        }
        else
        {
            //while(1);
            return FLASH_ERR;
        }
    }
    /* lock the flash program erase controller */
    fmc_lock();
    return FLASH_OK;
}


/*****************************************************************************
* @brief  : 写2个字节
* @param  : address: 地址(0x08000000~0x0807FFFF)
*		    length: 长度
*		    data_16: 数据指针
* @retval : FLASH_OK / FLASH_ERR
* @date   : 2023-09-05
* @note   : 
*****************************************************************************/
uint8_t flash_wirte_16bit(uint32_t address, uint16_t length, uint16_t * data_16)
{
    uint16_t StartSector, EndSector, i;
    /* unlock the flash program erase controller */
    fmc_unlock();
    /* clear pending flags */
    fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_OPERR | FMC_FLAG_WPERR | FMC_FLAG_PGMERR | FMC_FLAG_PGSERR);
    /* get the number of the start and end sectors */
    StartSector = flash_sector_get(address);
    EndSector = flash_sector_get(address + length);
    /* each time the sector number increased by 8, refer to the sector definition */
    for(i = StartSector; i <= EndSector; i += 8)
    {
        if(FMC_READY != fmc_sector_erase(i))
        {
            //while(1);
            return FLASH_ERR;
        }
    }
    /* write data_8 to the corresponding address */
    for(i = 0; i < length; i++)
    {
        if(FMC_READY == fmc_halfword_program(address, data_16[i]))
        {
            address = address + 2;
        }
        else
        {
            //while(1);
            return FLASH_ERR;
        }
    }
    /* lock the flash program erase controller */
    fmc_lock();
    return FLASH_OK;
}


/*****************************************************************************
* @brief  : 写4个字节
* @param  : address: 地址(0x08000000~0x0807FFFF)
*		    length: 长度
*		    data_32: 数据指针
*  @retval : FLASH_OK / FLASH_ERR
* @date   : 2023-09-05
* @note   : 
*****************************************************************************/
uint8_t flash_wirte_32bit(uint32_t address, uint16_t length, uint32_t * data_32)
{
    uint16_t StartSector, EndSector, i;
    /* unlock the flash program erase controller */
    fmc_unlock();
    /* clear pending flags */
    fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_OPERR | FMC_FLAG_WPERR | FMC_FLAG_PGMERR | FMC_FLAG_PGSERR);
    /* get the number of the start and end sectors */
    StartSector = flash_sector_get(address);
    EndSector = flash_sector_get(address + 4 * length);
    /* each time the sector number increased by 8, refer to the sector definition */
    for(i = StartSector; i <= EndSector; i += 8)
    {
        if(FMC_READY != fmc_sector_erase(i))
        {
            //while(1);
            return FLASH_ERR;
        }
    }
    /* write data_32 to the corresponding address */
    for(i = 0; i < length; i++)
    {
        if(FMC_READY == fmc_word_program(address, data_32[i]))
        {
            address = address + 4;
        }
        else
        {
            //while(1);
            return FLASH_ERR;
        }
    }
    /* lock the flash program erase controller */
    fmc_lock();
    return FLASH_OK;
}


/*****************************************************************************
* @brief  : 扇区擦除
* @param  : sector: 扇区号
* @retval : FLASH_OK / FLASH_ERR
* @date   : 2023-09-05
* @note   : 
*****************************************************************************/
uint8_t flash_sector_erase(uint32_t sector)
{
    /* unlock the flash program erase controller */
    fmc_unlock();
    /* clear pending flags */
    fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_OPERR | FMC_FLAG_WPERR | FMC_FLAG_PGMERR | FMC_FLAG_PGSERR);
    /* wait the erase operation complete*/
    if(FMC_READY != fmc_sector_erase(sector))
    {
        //while(1);
        return FLASH_ERR;
    }
    /* lock the flash program erase controller */
    fmc_lock();
    return FLASH_OK;
}


/*****************************************************************************
* @brief  : 页擦除
* @param  : address: 页地址
* @retval : FLASH_OK / FLASH_ERR
* @date   : 2023-09-05
* @note   : 
*****************************************************************************/
uint8_t flash_page_erase(uint32_t address)
{
    /* unlock the flash program erase controller */
    fmc_unlock();
    /* clear pending flags */
    fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_OPERR | FMC_FLAG_WPERR | FMC_FLAG_PGMERR | FMC_FLAG_PGSERR);
    /* wait the erase operation complete*/
    if(FMC_READY != fmc_page_erase(address))
    {
        //while(1);
        return FLASH_ERR;
    }
    /* lock the flash program erase controller */
    fmc_lock();
    return FLASH_OK;
}
