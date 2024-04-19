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

#ifndef _FLASHA_H
#define _FLASHA_H

#include "MCU_config.h"

/*---------------------- 宏定义  ---------------------------------------*/
#define P25Q16  0x8514

/*---------------------- 结构体 ------------------------------------------*/

typedef struct __FLASH_25xx_config_t
{
    SPI_CH_T spi_ch;            // flash SPI
    PORT_T cs_port;             // cs port
    GPIO_PIN_T cs_pin;          // cs pin
    uint32_t page_size;         // flash page 大小
    uint32_t sector_size;       // flash sector 大小, 最大支持128m bit
    uint32_t sector_num;        // flash sector 数量
    uint32_t flash_size;        // flash 大小
    volatile LOCK_STATE_E lock; // flash lock
    
    Status_t (*flash_open)(void);
    Status_t (*flash_close)(void);
    uint16_t (*flash_addr_to_sector)(uint32_t addr);
    uint32_t (*flash_get_sector_start_addr)(uint16_t sector_num);
    Status_t (*flash_erase_sector)(uint16_t sector);
    Status_t (*flash_read)(uint8_t * pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);
    Status_t (*flash_write)(uint8_t * pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
    
} FLASH_25xx_config_t;

/*---------------------- 接口 ------------------------------------------*/
/**********************************************************
 *函数名：flash_25xx_register
 *函数功能：器件注册,结构体初始化
 *函数参数：flash_config 传入器件参数
 *返回值：无
 */
Status_t flash_25xx_register(FLASH_25xx_config_t * flash_config);

///*---------------------- 测试函数 ------------------------------------------*/
//int flash_25xx_rw_test(void);
//int flash_25xx_test_loop(void);

#endif
