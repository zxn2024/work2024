/**
 * **********************************************************************************
 * @copyright Copyright (c) 2023 Shaanxi Tianji Communication Technologies Co., Ltd.
 *
 * @file        spi.c
 * @author      WangJun
 * @version     None
 * @date        2023-09-06
 * @brief       None
 *
 * **********************************************************************************
 */

#include "spi.h"


/*---------------------- 端口定义 ----------------------*/


/*---------------------- 宏 定 义 ----------------------*/


/*---------------------- 结 构 体 ----------------------*/


/*---------------------- 常    量 ----------------------*/


/*---------------------- 全局变量 ----------------------*/


/*---------------------- 局部变量 ----------------------*/


/*---------------------- 函    数 ----------------------*/


/*****************************************************************************
* @brief  : configure spi0
* @param  : None
* @retval : None
* @date   : 2023-09-06
* @note   : 
*****************************************************************************/
void spi0_init(void)
{
    /* enable GPIOB clock */
    // rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    /* enable SPI0 clock */
    rcu_periph_clock_enable(RCU_SPI0);
    /**SPI0 GPIO Configuration
    PA15    ------> SPI0_NSS
    PB3     ------> SPI0_SCK
    PB4     ------> SPI0_MISO
    PB5     ------> SPI0_MOSI
    */

    /* configure SPI0 GPIO */
    // gpio_mode_set(FLASH_CS_GPIO_Port, GPIO_MODE_AF, GPIO_PUPD_NONE, FLASH_CS_Pin);
    // gpio_mode_set(FLASH_CS_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, FLASH_CS_Pin);
    // gpio_output_options_set(FLASH_CS_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, FLASH_CS_Pin);    
    
    gpio_mode_set(FLASH_CLK_GPIO_Port, GPIO_MODE_AF, GPIO_PUPD_NONE, FLASH_CLK_Pin);
    gpio_output_options_set(FLASH_CLK_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, FLASH_CLK_Pin);
    
    gpio_mode_set(FLASH_MISO_GPIO_Port, GPIO_MODE_AF, GPIO_PUPD_NONE, FLASH_MISO_Pin);
    gpio_output_options_set(FLASH_MISO_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, FLASH_MISO_Pin);
    
    gpio_mode_set(FLASH_MOSI_GPIO_Port, GPIO_MODE_AF, GPIO_PUPD_NONE, FLASH_MOSI_Pin);
    gpio_output_options_set(FLASH_MOSI_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, FLASH_MOSI_Pin);

    /* set SPI0 GPIO ports  alternate*/
    /* connect PA15 to SPI0_NSS */
    //gpio_af_set(FLASH_CS_GPIO_Port, GPIO_AF_5, FLASH_CS_Pin);
    /* connect PB3 to SPI0_SCK */
    gpio_af_set(FLASH_CLK_GPIO_Port, GPIO_AF_5, FLASH_CLK_Pin);
    /* connect PB4 to SPI0_MISO */
    gpio_af_set(FLASH_MISO_GPIO_Port, GPIO_AF_5, FLASH_MISO_Pin);
    /* connect PB5 to SPI0_MOSI */
    gpio_af_set(FLASH_MOSI_GPIO_Port, GPIO_AF_5, FLASH_MOSI_Pin);
    
    spi_parameter_struct spi_init_struct;

    /* configure SPI0 parameter */
    spi_init_struct.trans_mode           = SPI_TRANSMODE_FULLDUPLEX;
    spi_init_struct.device_mode          = SPI_MASTER;
    spi_init_struct.frame_size           = SPI_FRAMESIZE_8BIT;
    spi_init_struct.clock_polarity_phase = SPI_CK_PL_LOW_PH_1EDGE;
    spi_init_struct.nss                  = SPI_NSS_SOFT;
    spi_init_struct.prescale             = SPI_PSC_4;
    spi_init_struct.endian               = SPI_ENDIAN_MSB;
    spi_init(SPI0, &spi_init_struct);

    /* wait nss high */
    // GPIO_BOP(FLASH_CS_GPIO_Port) = FLASH_CS_Pin;   // 置高电平

    /* enable SPI0 */
    spi_enable(SPI0);

    /* wait nss low */
    // GPIO_BC(FLASH_CS_GPIO_Port) = FLASH_CS_Pin;   // 置低电平
}


/*****************************************************************************
* @brief  : configure spi1
* @param  : None
* @retval : None
* @date   : 2023-09-06
* @note   : 
*****************************************************************************/
void spi1_init(void)
{
    /* enable GPIOB clock */
    rcu_periph_clock_enable(RCU_GPIOB);
    /* enable SPI1 clock */
    rcu_periph_clock_enable(RCU_SPI1);
    /**SPI1 GPIO Configuration
    PB12    ------> SPI1_NSS
    PB13    ------> SPI1_SCK
    PB14    ------> SPI1_MISO
    PB15    ------> SPI1_MOSI
    */

    /* configure SPI1 GPIO */
    //gpio_mode_set(FLASH_MCU_CS_GPIO_Port, GPIO_MODE_AF, GPIO_PUPD_NONE, FLASH_MCU_CS_Pin);
    // gpio_mode_set(FPGA_MCU_CS_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, FPGA_MCU_CS_Pin);
    // gpio_output_options_set(FPGA_MCU_CS_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, FPGA_MCU_CS_Pin);
    
    gpio_mode_set(FPGA_MCU_CLK_GPIO_Port, GPIO_MODE_AF, GPIO_PUPD_NONE, FPGA_MCU_CLK_Pin);
    gpio_output_options_set(FPGA_MCU_CLK_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, FPGA_MCU_CLK_Pin);
    
    gpio_mode_set(FPGA_MCU_MISO_GPIO_Port, GPIO_MODE_AF, GPIO_PUPD_NONE, FPGA_MCU_MISO_Pin);
    gpio_output_options_set(FPGA_MCU_MISO_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, FPGA_MCU_MISO_Pin);
    
    gpio_mode_set(FPGA_MCU_MOSI_GPIO_Port, GPIO_MODE_AF, GPIO_PUPD_NONE, FPGA_MCU_MOSI_Pin);
    gpio_output_options_set(FPGA_MCU_MOSI_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, FPGA_MCU_MOSI_Pin);

    /* set SPI1 GPIO ports  alternate*/
    /* connect PB12 to SPI1_NSS */
    //gpio_af_set(FPGA_MCU_CS_GPIO_Port, GPIO_AF_5, FPGA_MCU_CS_Pin);
    /* connect PB13 to SPI1_SCK */
    gpio_af_set(FPGA_MCU_CLK_GPIO_Port, GPIO_AF_5, FPGA_MCU_CLK_Pin);
    /* connect PB14 to SPI1_MISO */
    gpio_af_set(FPGA_MCU_MISO_GPIO_Port, GPIO_AF_5, FPGA_MCU_MISO_Pin);
    /* connect PB15 to SPI1_MOSI */
    gpio_af_set(FPGA_MCU_MOSI_GPIO_Port, GPIO_AF_5, FPGA_MCU_MOSI_Pin);
    
    spi_parameter_struct spi_init_struct;

    /* configure SPI1 parameter */
    spi_init_struct.trans_mode           = SPI_TRANSMODE_FULLDUPLEX;
    spi_init_struct.device_mode          = SPI_MASTER;
    spi_init_struct.frame_size           = SPI_FRAMESIZE_8BIT;
    spi_init_struct.clock_polarity_phase = SPI_CK_PL_LOW_PH_1EDGE;
    spi_init_struct.nss                  = SPI_NSS_SOFT;
    spi_init_struct.prescale             = SPI_PSC_32;
    spi_init_struct.endian               = SPI_ENDIAN_MSB;
    spi_init(SPI1, &spi_init_struct);

    /* wait nss high */
    // GPIO_BOP(FPGA_MCU_CS_GPIO_Port) = FPGA_MCU_CS_Pin;   // 置高电平

    /* enable SPI1 */
    spi_enable(SPI1);

    /* wait nss low */
    // GPIO_BC(FPGA_MCU_CS_GPIO_Port) = FPGA_MCU_CS_Pin;   // 置低电平
}


///*****************************************************************************
//* @brief  : configure spi2
//* @param  : None
//* @retval : None
//* @date   : 2023-09-06
//* @note   : 
//*****************************************************************************/
//void spi2_init(void)
//{
//    /* enable GPIOB clock */
//    rcu_periph_clock_enable(RCU_GPIOA);
//    rcu_periph_clock_enable(RCU_GPIOC);
//    /* enable SPI2 clock */
//    rcu_periph_clock_enable(RCU_SPI2);
//    /**SPI2 GPIO Configuration
//    //PA4   ------> SPI2_NSS
//    PC10  ------> SPI2_SCK
//    PC12  ------> SPI2_MOSI
//    */

//    /* configure SPI2 GPIO */
//    //gpio_mode_set(FPGA_PS_CS_GPIO_Port, GPIO_MODE_AF, GPIO_PUPD_NONE, FPGA_PS_CS_Pin);
//    gpio_mode_set(FPGA_PS_CS_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, FPGA_PS_CS_Pin);
//    gpio_output_options_set(FPGA_PS_CS_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, FPGA_PS_CS_Pin);
//    
//    gpio_mode_set(FPGA_PS_DCLK_GPIO_Port, GPIO_MODE_AF, GPIO_PUPD_NONE, FPGA_PS_DCLK_Pin);
//    gpio_output_options_set(FPGA_PS_DCLK_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, FPGA_PS_DCLK_Pin);

//    gpio_mode_set(FPGA_PS_DATA0_GPIO_Port, GPIO_MODE_AF, GPIO_PUPD_NONE, FPGA_PS_DATA0_Pin);
//    gpio_output_options_set(FPGA_PS_DATA0_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, FPGA_PS_DATA0_Pin);

//    /* set SPI2 GPIO ports  alternate*/
//    /* connect PA4 to SPI2_NSS */
//    //gpio_af_set(FPGA_PS_CS_GPIO_Port, GPIO_AF_5, FPGA_PS_CS_Pin);
//    /* connect PC10 to SPI2_SCK */
//    gpio_af_set(FPGA_PS_DCLK_GPIO_Port, GPIO_AF_5, FPGA_PS_DCLK_Pin);
//    /* connect PC12 to SPI2_MOSI */
//    gpio_af_set(FPGA_PS_DATA0_GPIO_Port, GPIO_AF_5, FPGA_PS_DATA0_Pin);
//    
//    spi_parameter_struct spi_init_struct;

//    /* configure SPI2 parameter */
//    spi_init_struct.trans_mode           = SPI_TRANSMODE_FULLDUPLEX;
//    spi_init_struct.device_mode          = SPI_MASTER;
//    spi_init_struct.frame_size           = SPI_FRAMESIZE_8BIT;
//    spi_init_struct.clock_polarity_phase = SPI_CK_PL_LOW_PH_2EDGE;
//    spi_init_struct.nss                  = SPI_NSS_SOFT;
//    spi_init_struct.prescale             =  SPI_PSC_2;   // 25Mhz
//    spi_init_struct.endian               = SPI_ENDIAN_MSB;
//    spi_init(SPI2, &spi_init_struct);

//    /* wait nss high */
//    // GPIO_BOP(FPGA_PS_CS_GPIO_Port) = FPGA_PS_CS_Pin;   // 置高电平

//    /* enable SPI2 */
//    spi_enable(SPI2);

//    /* wait nss low */
//    // GPIO_BC(FPGA_PS_CS_GPIO_Port) = FPGA_PS_CS_Pin;   // 置低电平
//}
