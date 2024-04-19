/*************************************** Copyright ******************************
 *
 * (C) Copyright 2023, Shaanxi Tianji Communication
 * All Rights Reserved
 *
 * FileName   : bi_flash.h
 * Version    : none
 * Author     :
 * Date       : 2023-09-21

 *******************************************************************************/
//

#ifndef _BI_FLASH_H_
#define _BI_FLASH_H_

#include "flash_25xx.h"
/*---------------------- FLASH驱动接口说明 ------------------------------------------*/

/*
-note
 -FLASH驱动需包含初始化结构体,flash_xxxx_register函数.
 -驱动接口根据具体型号和业务决定,如无必要请勿变更接口.

-设备初始化结构体
 包含MCU资源分配，比如GPIO分配，SPI端口，该设备的查找表等
typedef struct __FLASH_xxxx_HwConfig_t
{
    PORT_T cs_port;     // cs port
    GPIO_PIN_T cs_pin;  // cs pin
    SPI_CH_T spi_ch;    // flash SPI
    ....
    //此处定义该设备所需的硬件接口（如GPIO、SPI等），资源（内存，缓冲区），查找表（功率表，衰减表，温度表）
} FLASH_25xx_config_t;

-注册函数
 用于注册一个设备并分配资源
Status_t flash_xxxx_register(FLASH_xxxx_t *handle_flash);
*/

/*---------------------- 对外接口 ------------------------------------------*/

/*****************************************************************************
* @Name	  : bi_flash_register
* @brief  : 函数原型 flash_xxxx_register(FLASH_25xx_config_t *flash_config);
* @param  : flash_config: flash配置结构体
* @Retval : Status_t
* @Note   : 用来注册一个设备并分配资源
*/
#define bi_flash_register flash_25xx_register
/*****************************************************************************
* @Name	  : bi_flash_open
* @brief  : 函数原型 Status_t flash_xxxx_open(void);
* @param  :  
* @Retval : Status_t
* @Note   : 打开设备并初始化
*/
#define bi_flash_open flash_25xx_open
/*****************************************************************************
* @Name	  : bi_flash_addr_to_sector
* @brief  : 函数原型 uint16_t flash_25xx_addr_to_sector(uint32_t addr);
* @param  : addr: flash地址
* @Retval : Status_t
* @Note   : 将给定地址转换成sector
*/
#define bi_flash_addr_to_sector flash_25xx_addr_to_sector
/*****************************************************************************
* @Name	  : bi_flash_erase_sector
* @brief  : 函数原型 Status_t flash_25xx_erase_sector(uint16_t sector);
* @param  : sector: 扇区号
* @Retval : Status_t
* @Note   : 擦除扇区
*/
#define bi_flash_erase_sector flash_25xx_erase_sector
/*****************************************************************************
* @Name	  : bi_flash_read
* @brief  : 函数原型 flash_25xx_read(uint8_t *pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);
 *@param  : pBuffer 数据存储区
            ReadAddr 地址
            NumByteToRead 数据长度
* @Retval : Status_t
* @Note   : 读数据
*/
#define bi_flash_read flash_25xx_read
/*****************************************************************************
* @Name	  : bi_flash_write
* @brief  : 函数原型 Status_t flash_25xx_write(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
 *@param  : pBuffer 数据存储区
            ReadAddr 地址
            NumByteToRead 数据长度
* @Retval : Status_t
* @Note   : 写数据, 带回读检测的写,写数据前需要擦除对应区域
*/
#define bi_flash_write flash_25xx_write



#endif /* __IWDG_H__ */
