/**
 * **********************************************************************************
 * @copyright Copyright (c) 2023 Shaanxi Tianji Communication Technologies Co., Ltd.
 *
 * @file        hi_spi.c
 * @author      WangJun
 * @version     None
 * @date        2023-11-6
 * @brief       None
 *
 * **********************************************************************************
 */

#include "hi_spi.h"
#include "hi_gpio.h"
#include "hi_delay.h"

/*---------------------- 端口定义 --------------------------------------*/


/*---------------------- 宏定义  ---------------------------------------*/


/*---------------------- 结构体 ----------------------------------------*/


/*---------------------- 常量 ------------------------------------------*/


/*---------------------- 全局变量 --------------------------------------*/


/*---------------------- 局部变量 --------------------------------------*/


/*---------------------- 声明 ------------------------------------------*/




/*****************************************************************************
* @Name	  : hi_spi_init
* @Brief  :
* @Retval :
* @Author :
* @Data   :
* @Note   :
* @Note   : Core/spi.c中实现初始化函数
*****************************************************************************/
void hi_spi_init(void)
{
#if(PRODUCT_MODEL == CMDT3521A)
    spi0_init();
    spi1_init();
#elif(PRODUCT_MODEL == CMDT3521B)
    spi0_init();
    spi1_init();
#elif(PRODUCT_MODEL == CMDT3521C)
    MX_SPI1_Init();
    MX_SPI2_Init();
#elif(PRODUCT_MODEL == CMDT3521D)

#elif(PRODUCT_MODEL == CMDT3521E)

#elif(PRODUCT_MODEL == CMDT3521F)

#endif
}


/*****************************************************************************
* @Name	  : hi_spi_cs_on
* @Brief  : 使能片选
* @Retval :
* @Author :
* @Data   :
* @Note   :
*****************************************************************************/
void hi_spi_cs_on(spi_port_t ch)
{
    hi_gpio_write(ch.cs_port, ch.cs_pin, LEVEL_H);
}


/*****************************************************************************
* @Name	  : hi_spi_cs_off
* @Brief  : 取消片选
* @Retval :
* @Author :
* @Data   :
* @Note   :
*****************************************************************************/
void hi_spi_cs_off(spi_port_t ch)
{
    hi_gpio_write(ch.cs_port, ch.cs_pin, LEVEL_L);
}


/*****************************************************************************
* @Name	  : hi_spi_write
* @brief  : spi写
* @param  :
* @retval : None
* @author :
* @Data   :
* @Note   :
* @Note   :
*****************************************************************************/
Status_t hi_spi_write(SPI_CH_T ch, uint8_t * pBuf, uint16_t len, uint32_t timeout)
{
#if(MCU_SERIES == MCU_GD32F4XX)
    uint16_t i = 0;

    while(i < len)
    {
        while(RESET == spi_i2s_flag_get(ch, SPI_FLAG_TBE));
        spi_i2s_data_transmit(ch, pBuf[i++]);
    }
    while(RESET == spi_i2s_flag_get(ch, SPI_FLAG_TBE));
    
    return STAT_OK;
#elif(MCU_SERIES == MCU_STM32F4XX || MCU_SERIES == MCU_STM32H5XX)
    int ret = STAT_OK;
    
    ret = HAL_SPI_Transmit(ch, pBuf, len, timeout);
    if(ret != HAL_OK)
        return STAT_ERROR;
    return STAT_OK;
#endif
}

/*****************************************************************************
* @Name	  : hi_spi_read
* @brief  : spi读
* @param  :
* @author :
* @Data   :
* @Note   :
*****************************************************************************/
Status_t hi_spi_read(SPI_CH_T ch, uint8_t * pBuf, uint16_t len, uint32_t timeout)
{
#if(MCU_SERIES == MCU_GD32F4XX)
    uint16_t i= 0;
    
    while(i < len)
    {
        while(RESET == spi_i2s_flag_get(ch, SPI_FLAG_RBNE));
        pBuf[i++] = spi_i2s_data_receive(ch);
    }
    while(RESET == spi_i2s_flag_get(ch, SPI_FLAG_RBNE));
    
    return STAT_OK;
#elif(MCU_SERIES == MCU_STM32F4XX || MCU_SERIES == MCU_STM32H5XX)
    int ret = STAT_OK;
    
    ret = HAL_SPI_Receive(ch, pBuf, len, timeout);
    if(ret != HAL_OK)
        return STAT_ERROR;
    return STAT_OK;
#endif
}


/*****************************************************************************
* @Name	  : SPI_ReadWriteByte
* @brief  : spi读写
* @param  :
* @author :
* @Data   :
* @Note   :
*****************************************************************************/
uint8_t SPI_ReadWriteByte(SPI_CH_T ch, uint8_t tx_data)
{
	uint8_t rx_data;
#if(MCU_SERIES == MCU_GD32F4XX)
    while(RESET == spi_i2s_flag_get(ch, SPI_FLAG_TBE));
    spi_i2s_data_transmit(ch, tx_data);
    
    while(RESET == spi_i2s_flag_get(ch, SPI_FLAG_RBNE));
    rx_data = spi_i2s_data_receive(ch);
#elif(MCU_SERIES == MCU_STM32F4XX || MCU_SERIES == MCU_STM32H5XX)
	HAL_SPI_TransmitReceive(ch,&tx_data, &rx_data, 1, 1000);
#endif
	return rx_data;          		    //返回收到的数据		
}
