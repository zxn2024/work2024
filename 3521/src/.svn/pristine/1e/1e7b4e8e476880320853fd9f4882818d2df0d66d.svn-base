/**
 * **********************************************************************************
 * @copyright Copyright (c) 2023 Shaanxi Tianji Communication Technologies Co., Ltd.
 *
 * @file        systick.c
 * @author      WangJun
 * @version     None
 * @date        2023-09-04
 * @brief       None
 *
 * **********************************************************************************
 */

#include "systick.h"
#include "gd32f4xx.h"

volatile static uint32_t delay;

/**
  * @brief 获取Tick
  */
__IO uint32_t uwTick;

/*!
    \brief    configure systick
    \param[in]  none
    \param[out] none
    \retval     none
*/
void systick_config(void)
{
    /* setup systick timer for 1000Hz interrupts */
    if(SysTick_Config(SystemCoreClock / 1000U))
    {
        /* capture error */
        while(1)
        {
        }
    }
    /* configure the systick handler priority */
    NVIC_SetPriority(SysTick_IRQn, 0x00U);
}

/*!
    \brief    delay a time in milliseconds
    \param[in]  count: count in milliseconds
    \param[out] none
    \retval     none
*/
void delay_1ms(uint32_t count)
{
    delay = count;

    while(0U != delay)
    {
    }
}

/*!
    \brief    delay decrement
    \param[in]  none
    \param[out] none
    \retval     none
*/
void delay_decrement(void)
{
    if(0U != delay)
    {
        delay--;
    }
}


/*****************************************************************************
* @brief  : tick计数器
* @param  : None
* @retval : None
* @date   : 2023-09-11
* @note   : 
*****************************************************************************/
void inc_tick(void)
{
    uwTick += 1;
}


/*****************************************************************************
* @brief  : 获取tick
* @param  : None
* @retval : 当前tick数
* @date   : 2023-09-11
* @note   :
*****************************************************************************/
uint32_t get_tick(void)
{
    return uwTick;
}
