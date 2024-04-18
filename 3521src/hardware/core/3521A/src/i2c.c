/**
 * **********************************************************************************
 * @copyright Copyright (c) 2023 Shaanxi Tianji Communication Technologies Co., Ltd.
 *
 * @file        i2c.c
 * @author      WangJun
 * @version     None
 * @date        2023-09-05
 * @brief       None
 *
 * **********************************************************************************
 */

#include "i2c.h"


/*---------------------- 端口定义 ----------------------*/


/*---------------------- 宏 定 义 ----------------------*/


/*---------------------- 结 构 体 ----------------------*/


/*---------------------- 常    量 ----------------------*/


/*---------------------- 全局变量 ----------------------*/


/*---------------------- 局部变量 ----------------------*/


/*---------------------- 函    数 ----------------------*/
/*****************************************************************************
* @brief  : i2c初始化
* @param  : None
* @retval : None
* @date   : 2023-09-05
* @note   : 
*****************************************************************************/
void i2c0_init(void)
{
    /* enable GPIOB clock */
    rcu_periph_clock_enable(RCU_GPIOB);
    /* enable I2C0 clock */
    rcu_periph_clock_enable(RCU_I2C0);
    /**I2C0 GPIO Configuration
    PB6     ------> I2C0_SCL
    PB7     ------> I2C0_SDA
    */

    /* configure I2C0 GPIO */
    gpio_mode_set(I2C0_SCL_GPIO_Port, GPIO_MODE_AF, GPIO_PUPD_NONE, I2C0_SCL_Pin);
    gpio_output_options_set(I2C0_SCL_GPIO_Port, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, I2C0_SCL_Pin);
    gpio_mode_set(I2C0_SDA_GPIO_Port, GPIO_MODE_AF, GPIO_PUPD_NONE, I2C0_SDA_Pin);
    gpio_output_options_set(I2C0_SDA_GPIO_Port, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, I2C0_SDA_Pin);

    /* set I2C0 GPIO ports  alternate */
    /* connect PB6 to I2C0_SCL */
    gpio_af_set(I2C0_SCL_GPIO_Port, GPIO_AF_4, I2C0_SCL_Pin);
    /* connect PB7 to I2C0_SDA */
    gpio_af_set(I2C0_SDA_GPIO_Port, GPIO_AF_4, I2C0_SDA_Pin);

    /* configure I2C clock */
    i2c_clock_config(I2C0, 100000, I2C_DTCY_2);
    /* configure I2C address */
    i2c_mode_addr_config(I2C0, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, 0x90);  // 从机地址
    /* enable I2C0 */
    i2c_enable(I2C0);
    /* enable acknowledge */
    i2c_ack_config(I2C0, I2C_ACK_ENABLE);
}

/**
  * @brief   read temp
  * @param   None
  * @retval  retval:温度寄存器值
  */


/*****************************************************************************
* @brief  : 读取温度
* @param  : None
* @retval : 温度值
* @date   : 2023-09-05
* @note   :
*****************************************************************************/
uint16_t read_temp(void)
{
    uint8_t i = 0;
    uint8_t i2c_receiver[16];

    /* wait until I2C bus is idle */
    while(i2c_flag_get(I2C0, I2C_FLAG_I2CBSY));
    /* send a start condition to I2C bus */
    i2c_start_on_bus(I2C0);
    /* wait until SBSEND bit is set */
    while(!i2c_flag_get(I2C0, I2C_FLAG_SBSEND));
    /* send slave address to I2C bus */
    i2c_master_addressing(I2C0, 0x90, I2C_RECEIVER);
    /* wait until ADDSEND bit is set */
    while(!i2c_flag_get(I2C0, I2C_FLAG_ADDSEND));
    /* clear ADDSEND bit */
    i2c_flag_clear(I2C0, I2C_FLAG_ADDSEND);

    for(i = 0; i < 16; i++)
    {
        if(13 == i)
        {
            /* wait until the second last data byte is received into the shift register */
            while(!i2c_flag_get(I2C0, I2C_FLAG_BTC));
            /* disable acknowledge */
            i2c_ack_config(I2C0, I2C_ACK_DISABLE);
        }
        /* wait until the RBNE bit is set */
        while(!i2c_flag_get(I2C0, I2C_FLAG_RBNE));
        /* read a data from I2C_DATA */
        i2c_receiver[i] = i2c_data_receive(I2C0);
    }
    /* send a stop condition to I2C bus */
    i2c_stop_on_bus(I2C0);

    /* wait until stop condition generate */
    while(I2C_CTL0(I2C0) & I2C_CTL0_STOP);
    /* enable acknowledge */
    i2c_ack_config(I2C0, I2C_ACK_ENABLE);

    return ((i2c_receiver[0] << 8 | i2c_receiver[1]) >> 4);
}
