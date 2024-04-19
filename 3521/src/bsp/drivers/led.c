/****************************************************************************
 * Copyright(C)，2022，陕西天基通信科技有限责任公司
 * 文件名：t_digital_att.c
 * 内容简述：
 *      通用的数字衰减器驱动
 * 文件历史：
 * 版本号 日期            作者    说明
 * 01    2023-9-15       扬扬    创建该文件
 */

#include "led.h"
#include "hi_gpio.h"
#include "hi_systick.h"
/*---------------------- 端口定义 --------------------------------------*/

/*---------------------- 宏定义  ---------------------------------------*/

/*---------------------- 声明 ------------------------------------------*/

/****************************************************************************
 * 函数名： led_set(struct __LED_TypeDef *pLedHandle, uint8_t stat, uint32_t cycle)
 * 功  能： 设置led
 * 输  入： pLedHandle 衰减器结构体
 * 输  入： stat LED_OFF (0) LED_ON (1) LED_FLASH (2)
 * 输  入： led闪灯周期(当闪灯状态为LED_FLASH时生效)
 * 输  出： -
 */
static Status_t led_set(struct __LED_TypeDef * pLedHandle, LED_STAT stat, uint32_t cycle)
{
    // 参数检查
    if(pLedHandle == NULL)
        return STAT_ERROR;

    if(stat != LED_OFF && stat != LED_ON && stat != LED_FLASH)
        return STAT_ERROR;
    // 参数设置
    switch(stat)
    {
        case LED_OFF:
            pLedHandle->led_stat = LED_OFF;
            pLedHandle->flash_cycle = 0;
            break;

        case LED_ON:
            pLedHandle->led_stat = LED_ON;
            pLedHandle->flash_cycle = 0;
            break;

        case LED_FLASH:
            pLedHandle->led_stat = LED_FLASH;
            pLedHandle->flash_cycle = cycle;
            break;

        default:
            return STAT_ERROR;
            //break;
    }

    return STAT_OK;
}

/****************************************************************************
 * 函数名： led_run(struct __LED_TypeDef *pLedHandle)
 * 功  能： led闪灯执行函数,执行led亮灭和闪灯控制,在周期任务中调用此函数,将根据设置的周期改变闪灯状态.
 * 输  入： pLedHandle led结构体
 * 输  出： -
 */
static Status_t led_run(struct __LED_TypeDef * pLedHandle)
{
    LED_STAT led_flash_stat = LED_ON;
    // 参数检查
    if(pLedHandle == NULL)
        return STAT_ERROR;

    if(pLedHandle->led_stat == LED_ON)  // 亮灯处理
    {
        if(pLedHandle->hw_config.ctl_logic_invert == B_TRUE)
        {
            hi_gpio_write(pLedHandle->hw_config.led_port, pLedHandle->hw_config.led_pin, LEVEL_L);
        }
        else
        {
            hi_gpio_write(pLedHandle->hw_config.led_port, pLedHandle->hw_config.led_pin, LEVEL_H);
        }
    }
    else if(pLedHandle->led_stat == LED_OFF)  // 灭灯处理
    {
        if(pLedHandle->hw_config.ctl_logic_invert == B_TRUE)
        {
            hi_gpio_write(pLedHandle->hw_config.led_port, pLedHandle->hw_config.led_pin, LEVEL_H);
        }
        else
        {
            hi_gpio_write(pLedHandle->hw_config.led_port, pLedHandle->hw_config.led_pin, LEVEL_L);
        }
    }
    else if(pLedHandle->led_stat == LED_FLASH)  // 闪灯处理
    {
        if((hi_get_tick() % pLedHandle->flash_cycle) <= (pLedHandle->flash_cycle / 2))
            led_flash_stat = LED_ON;
        else
            led_flash_stat = LED_OFF;

        if(led_flash_stat == LED_OFF)
        {
            if(pLedHandle->hw_config.ctl_logic_invert == B_TRUE)
            {
                hi_gpio_write(pLedHandle->hw_config.led_port, pLedHandle->hw_config.led_pin, LEVEL_L);
            }
            else
            {
                hi_gpio_write(pLedHandle->hw_config.led_port, pLedHandle->hw_config.led_pin, LEVEL_H);
            }
        }
        else if(led_flash_stat == LED_ON)
        {
            if(pLedHandle->hw_config.ctl_logic_invert == B_TRUE)
            {
                hi_gpio_write(pLedHandle->hw_config.led_port, pLedHandle->hw_config.led_pin, LEVEL_H);
            }
            else
            {
                hi_gpio_write(pLedHandle->hw_config.led_port, pLedHandle->hw_config.led_pin, LEVEL_L);
            }
        }
    }
    else // 错误状态置为关灯
        pLedHandle->led_stat = LED_OFF;

    return STAT_OK;
}

/****************************************************************************
 * 函数名： led_fpga_register(LED_t *pLedHandle)
 * 功  能： LED初始化
 * 输  入： -
 * 输  出： -
 */
Status_t led_register(LED_t * pLedHandle)
{
    // 参数检查
    if(pLedHandle == NULL)
        return STAT_ERROR;
    // 函数指针初始化赋值
    pLedHandle->set = led_set;
    pLedHandle->run = led_run;
    pLedHandle->led_stat = pLedHandle->hw_config.led_init_stat;
    pLedHandle->flash_cycle = 0;
    return STAT_OK;
}
