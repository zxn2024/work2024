/**
 * **********************************************************************************
 * @copyright Copyright (c) 2023 Shaanxi Tianji Communication Technologies Co., Ltd.
 *
 * @file        hi_gpio.c
 * @author      WangJun
 * @version     None
 * @date        2023-11-6
 * @brief       None
 *
 * **********************************************************************************
 */
 
#include "hi_gpio.h"

/*---------------------- 端口定义 --------------------------------------*/


/*---------------------- 宏定义  ---------------------------------------*/


/*---------------------- 结构体 ----------------------------------------*/


/*---------------------- 常量 ------------------------------------------*/


/*---------------------- 全局变量 --------------------------------------*/


/*---------------------- 局部变量 --------------------------------------*/


/*---------------------- 声明 ------------------------------------------*/




/*****************************************************************************
* @Name	  : hiGpioInit
* @Brief  :
* @Retval :
* @Author :
* @Data   :
* @Note   :
* @Note   : Core/gpio.c中实现初始化函数
*****************************************************************************/
void hi_gpio_init(void)
{
#if(MCU_SERIES == MCU_GD32F4XX)
    gpio_init();
#elif(MCU_SERIES == MCU_STM32F4XX || MCU_SERIES == MCU_STM32H5XX)
    MX_GPIO_Init();
#endif

}

/*****************************************************************************
* @Name	  : hiGpioWrite
* @brief  : 设置管脚状态
* @param  :
* @retval : None
* @author :
* @Data   :
* @Note   :
* @Note   :
*****************************************************************************/
void hi_gpio_write(PORT_T port, GPIO_PIN_T pin, ELEC_LEVEL_E gpioStat)
{
#if(MCU_SERIES == MCU_GD32F4XX)
    if(gpioStat == LEVEL_L)
        gpio_bit_reset(port, pin);
    else if(gpioStat == LEVEL_H)
        gpio_bit_set(port, pin);
    else
        return;
#elif(MCU_SERIES == MCU_STM32F4XX || MCU_SERIES == MCU_STM32H5XX)
    if(gpioStat == LEVEL_L)
        HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);
    else if(gpioStat == LEVEL_H)
        HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);
    else
        return;
#endif

}

/*****************************************************************************
* @Name	  : hiGpioRead
* @brief  : 读取管脚状态
* @param  :
* @author :
* @Data   :
* @Note   :
*****************************************************************************/
ELEC_LEVEL_E hi_gpio_read(PORT_T port, GPIO_PIN_T pin)
{
    int ret = 0;
#if(MCU_SERIES == MCU_GD32F4XX)
    ret = gpio_input_bit_get(port, pin);
    if(ret == RESET)
        return LEVEL_L;
    else
        return LEVEL_H;
#elif(MCU_SERIES == MCU_STM32F4XX || MCU_SERIES == MCU_STM32H5XX)
    ret = HAL_GPIO_ReadPin(port, pin);
    if(ret == RESET)
        return LEVEL_L;
    else
        return LEVEL_H;
#endif
}

/*****************************************************************************
* @Name	  : hiGpioInvert
* @brief  : 读取管脚状态
* @param  :
* @author :
* @Data   :
*****************************************************************************/
void hi_gpio_toggle(PORT_T port, GPIO_PIN_T pin)
{
#if(MCU_SERIES == MCU_GD32F4XX)
    gpio_bit_toggle(port, pin);
#elif(MCU_SERIES == MCU_STM32F4XX || MCU_SERIES == MCU_STM32H5XX)
    HAL_GPIO_TogglePin(port, pin);
#endif
}
