/*************************************** Copyright ******************************
*
* (C) Copyright 2023, Shaanxi Tianji Communication
* All Rights Reserved
*
* FileName   : hi_spi.h
* Version    : none
* Author     :
* Date       : 2023-05-26
* Description: spi的通用操作接口，支持多型号MCU，MCU列表请看MCU_config.h

*******************************************************************************/

#ifndef __HI_SPI_H__
#define __HI_SPI_H__

#include "spi.h"
#include "MCU_config.h"

/*---------------------- 端口定义 --------------------------------------*/
#if(MCU_SERIES == MCU_GD32F4XX)

#define SPI_0 SPI0
#define SPI_1 SPI1

#elif(MCU_SERIES == MCU_STM32F4XX || MCU_SERIES == MCU_STM32H5XX)

#define SPI_1 &hspi1
#define SPI_2 &hspi2

#endif
/*---------------------- 宏定义  ---------------------------------------*/
typedef struct SPI_Port_t
{
    SPI_CH_T    spi_ch;     // spi
    PORT_T      cs_port;    // cs port
    GPIO_PIN_T  cs_pin;     // cs pin
} spi_port_t;

/*---------------------- 声明 ------------------------------------------*/
/*****************************************************************************
* @Name	  : hi_spi_init
* @Brief  : SPI初始化函数
* @Retval :
* @Author :
* @Data   :
* @Note   :
*****************************************************************************/
void hi_spi_init(void);

/*****************************************************************************
* @Name	  : hi_spi_cs_on
* @Brief  : SPI使能片选函数
* @Retval :
* @Author :
* @Data   :
* @Note   :
*****************************************************************************/
void hi_spi_cs_on(spi_port_t ch);

/*****************************************************************************
* @Name	  : hi_spi_cs_off
* @Brief  : SPI取消片选函数
* @Retval :
* @Author :
* @Data   :
* @Note   :
*****************************************************************************/
void hi_spi_cs_off(spi_port_t ch);

/*****************************************************************************
* @Name	  : hi_spi_write
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
Status_t hi_spi_write(SPI_CH_T ch, uint8_t * pBuf, uint16_t len, uint32_t timeout);

/*****************************************************************************
* @Name	  : hi_spi_read
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
Status_t hi_spi_read(SPI_CH_T ch, uint8_t * pBuf, uint16_t len, uint32_t timeout);

/*****************************************************************************
* @Name	  : SPI_ReadWriteByte
* @brief  : Spi 单字节读写函数
* @param  : ch ：端口号,见上方端口定义
* @param  : tx_data ：数据
* @Retval  : 读到的1字节数据
* @author :
* @Data   :
* @Note   :
*****************************************************************************/
uint8_t SPI_ReadWriteByte(SPI_CH_T ch, uint8_t tx_data);


#endif
