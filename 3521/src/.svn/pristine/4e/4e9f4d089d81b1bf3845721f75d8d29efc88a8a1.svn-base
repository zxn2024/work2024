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


#include "hi_system.h"

/*---------------------- 端口定义 --------------------------------------*/


/*---------------------- 宏定义  ---------------------------------------*/


/*---------------------- 结构体 ----------------------------------------*/


/*---------------------- 常量 ------------------------------------------*/


/*---------------------- 全局变量 --------------------------------------*/


/*---------------------- 局部变量 --------------------------------------*/


/*---------------------- 声明 ------------------------------------------*/
void SystemClock_Config(void);
/*****************************************************************************
* @Name	  : hiSpiRead
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
void hi_enable_irq(void)
{
#if(MCU_SERIES == MCU_GD32F4XX)
    __enable_irq();
#elif(MCU_SERIES == MCU_STM32F4XX || MCU_SERIES == MCU_STM32H5XX)
    __enable_irq();
#endif

}

/*****************************************************************************
* @Name	  : hiSpiWrite
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
void hi_disable_irq(void)
{
#if(MCU_SERIES == MCU_GD32F4XX)
    __disable_irq();
#elif(MCU_SERIES == MCU_STM32F4XX || MCU_SERIES == MCU_STM32H5XX)
    __disable_irq();
#endif

}

/*****************************************************************************
* @Name	  : hiSpiRead
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
void hi_chip_init(void)
{
#if(MCU_SERIES == MCU_GD32F4XX)
    /* configure 4 bits pre-emption priority */
    nvic_priority_group_set(NVIC_PRIGROUP_PRE4_SUB0);

    systick_config();
#elif(MCU_SERIES == MCU_STM32F4XX || MCU_SERIES == MCU_STM32H5XX)
    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();
    /* Configure the system clock */
    SystemClock_Config();
#endif
}

/*****************************************************************************
* @Name	  : hiSpiRead
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
void hi_vecttab_offset(void)
{
#if(_USE_UPGRADE_)
    //中断向量偏移
#if(MCU_SERIES == MCU_GD32F4XX)
    SCB->VTOR = FLASH_BASE | 0xC200;  // 中断向量表偏移
    __enable_irq();
#elif(MCU_SERIES == MCU_STM32F4XX || MCU_SERIES == MCU_STM32H5XX)
    SCB->VTOR = FLASH_BASE | 0xC200;  // 中断向量表偏移
    __enable_irq();
#endif

#endif
    return;
}




/*****************************************************************************
* @brief  : 重启函数
* @param  : None
* @retval :
* @date   : 2024-1-12
* @note   :
*****************************************************************************/
void hi_reboot_mcu(void)
{
    NVIC_SystemReset();
}
