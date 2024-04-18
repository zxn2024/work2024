/**
 * **********************************************************************************
 * @copyright Copyright (c) 2023 Shaanxi Tianji Communication Technologies Co., Ltd.
 *                         
 * @file        flash.h   
 * @author      WangJun
 * @version     None
 * @date        2023-09-05     
 * @brief       None   
 * 
 * **********************************************************************************
 */

#ifndef __FLASH_H_
#define __FLASH_H_

#include "main.h"

#ifdef __cplusplus
 extern "C" {
#endif

/* base address of the FMC sectors */
#define ADDR_FMC_SECTOR_0     ((uint32_t)0x08000000) /*!< base address of sector 0, 16 kbytes */
#define ADDR_FMC_SECTOR_1     ((uint32_t)0x08004000) /*!< base address of sector 1, 16 kbytes */
#define ADDR_FMC_SECTOR_2     ((uint32_t)0x08008000) /*!< base address of sector 2, 16 kbytes */
#define ADDR_FMC_SECTOR_3     ((uint32_t)0x0800C000) /*!< base address of sector 3, 16 kbytes */
#define ADDR_FMC_SECTOR_4     ((uint32_t)0x08010000) /*!< base address of sector 4, 64 kbytes */
#define ADDR_FMC_SECTOR_5     ((uint32_t)0x08020000) /*!< base address of sector 5, 64 kbytes */
#define ADDR_FMC_SECTOR_6     ((uint32_t)0x08040000) /*!< base address of sector 6, 64 kbytes */
#define ADDR_FMC_SECTOR_7     ((uint32_t)0x08060000) /*!< base address of sector 7, 64 kbytes */
#define ADDR_FMC_END          ((uint32_t)0x08080000) /*!< END */


uint32_t flash_sector_get(uint32_t address);

//void flash_read_8bit(uint32_t address, uint16_t length, uint8_t * data_8);
//void flash_read_16bit(uint32_t address, uint16_t length, uint16_t * data_16);
//void flash_read_32bit(uint32_t address, uint16_t length, uint32_t * data_32);

//uint8_t flash_wirte_8bit(uint32_t address, uint16_t length, uint8_t* data_8);
//uint8_t flash_wirte_16bit(uint32_t address, uint16_t length, uint16_t* data_16);
//uint8_t flash_wirte_32bit(uint32_t address, uint16_t length, uint32_t* data_32);

//uint8_t flash_sector_erase(uint32_t sector);
//uint8_t flash_page_erase(uint32_t address);

#ifdef __cplusplus
}
#endif

#endif
