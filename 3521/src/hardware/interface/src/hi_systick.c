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


#include "hi_systick.h"

/*---------------------- 端口定义 --------------------------------------*/


/*---------------------- 宏定义  ---------------------------------------*/


/*---------------------- 结构体 ----------------------------------------*/


/*---------------------- 常量 ------------------------------------------*/


/*---------------------- 全局变量 --------------------------------------*/


/*---------------------- 局部变量 --------------------------------------*/


/*---------------------- 声明 ------------------------------------------*/




/*****************************************************************************
* @Name	  : hi_systick_init
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
void hi_systick_init(void)
{
#if(MCU_SERIES == MCU_GD32F4XX)

#elif(MCU_SERIES == MCU_STM32F4XX || MCU_SERIES == MCU_STM32H5XX)
    //已在chipinit完成初始化
    return;
#endif
}

/*****************************************************************************
* @Name	  : hi_get_tick
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
uint32_t hi_get_tick(void)
{
#if(MCU_SERIES == MCU_GD32F4XX)
    return get_tick();
#elif(MCU_SERIES == MCU_STM32F4XX || MCU_SERIES == MCU_STM32H5XX)
    return HAL_GetTick();
#endif
}

#if(MCU_SERIES == MCU_STM32F4XX)
volatile static uint32_t delay;

/*****************************************************************************
* @brief  : delay decrement
* @param  : None
* @retval : 
* @date   : 2024-01-18
* @note   : 
*****************************************************************************/
void hal_delay_decrement(void)
{
    if(0U != delay)
    {
        delay--;
    }
}


/*****************************************************************************
* @brief  : delay a time in milliseconds
* @param  : None
* @retval : 
* @date   : 2024-01-18
* @note   : 
*****************************************************************************/
static void hal_delay_1ms(uint32_t count)
{
    delay = count;

    while(0U != delay)
    {
    }
}
#endif

/*****************************************************************************
* @Name	  : hi_delay_ms
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
void hi_delay_ms(uint32_t ms)
{
#if(MCU_SERIES == MCU_GD32F4XX)
    delay_1ms(ms);
#elif(MCU_SERIES == MCU_STM32F4XX || MCU_SERIES == MCU_STM32H5XX)
    hal_delay_1ms(ms);
#endif
}

