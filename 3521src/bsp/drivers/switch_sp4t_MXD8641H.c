/****************************************************************************
 * Copyright(C)，2022，陕西天基通信科技有限责任公司
 * 文件名：t_digital_att.c
 * 内容简述：
 *      通用的数字衰减器驱动
 * 文件历史：
 * 版本号 日期            作者    说明
 * 01    2023-9-15       扬扬    创建该文件
 */

#include "switch_sp4t_MXD8641H.h"
#include "hi_gpio.h"
/*---------------------- 端口定义 --------------------------------------*/

/*---------------------- 宏定义  ---------------------------------------*/

/*---------------------- 声明 ------------------------------------------*/

/****************************************************************************
 * 函数名： set_pwr(struct __Switch_TypeDef *pSwitchHandle, SW_STATE_E sw_ctl)
 * 功  能： 设置switch
 * 输  入： pSwitchHandle switch结构体
 * 输  入： stat SW_OFF (0) Switch_ON (1)
 * 输  出： -
 */
static Status_t switch_ctl(struct __Switch_sp4t_TypeDef * pSwitchHandle, SW_STATE_E sw_ctl)
{
    // 参数检查
    if(pSwitchHandle == NULL)
        return STAT_ERROR;

    // 参数设置
    if(sw_ctl == SW_CH1)
    {
        pSwitchHandle->switch_ctl_stat = sw_ctl;
        hi_gpio_write(pSwitchHandle->hw_config.switch_v2_port, pSwitchHandle->hw_config.switch_v2_pin, LEVEL_L);
        hi_gpio_write(pSwitchHandle->hw_config.switch_v3_port, pSwitchHandle->hw_config.switch_v3_pin, LEVEL_L);
    }
    else if(sw_ctl == SW_CH2)
    {
        pSwitchHandle->switch_ctl_stat = sw_ctl;
        hi_gpio_write(pSwitchHandle->hw_config.switch_v2_port, pSwitchHandle->hw_config.switch_v2_pin, LEVEL_L);
        hi_gpio_write(pSwitchHandle->hw_config.switch_v3_port, pSwitchHandle->hw_config.switch_v3_pin, LEVEL_H);
    }
    else if(sw_ctl == SW_CH3)
    {
        pSwitchHandle->switch_ctl_stat = sw_ctl;
        hi_gpio_write(pSwitchHandle->hw_config.switch_v2_port, pSwitchHandle->hw_config.switch_v2_pin, LEVEL_H);
        hi_gpio_write(pSwitchHandle->hw_config.switch_v3_port, pSwitchHandle->hw_config.switch_v3_pin, LEVEL_L);
    }
    else if(sw_ctl == SW_CH4)
    {
        pSwitchHandle->switch_ctl_stat = sw_ctl;
        hi_gpio_write(pSwitchHandle->hw_config.switch_v2_port, pSwitchHandle->hw_config.switch_v2_pin, LEVEL_H);
        hi_gpio_write(pSwitchHandle->hw_config.switch_v3_port, pSwitchHandle->hw_config.switch_v3_pin, LEVEL_H);
    }
    else
        return STAT_ERROR;

    return STAT_OK;
}
/****************************************************************************
 * 函数名： set_pwr(struct __Switch_TypeDef *pSwitchHandle, SW_STATE_E sw_ctl)
 * 功  能： 设置switch
 * 输  入： pSwitchHandle switch结构体
 * 输  入： stat SW_OFF (0) Switch_ON (1)
 * 输  出： -
 */
static Status_t switch_open(struct __Switch_sp4t_TypeDef * pSwitchHandle)
{
    // 参数检查
    if(pSwitchHandle == NULL)
        return STAT_ERROR;
    // 按照初始化参数打开设备
    return switch_ctl(pSwitchHandle, pSwitchHandle->hw_config.switch_init_stat);
}
/****************************************************************************
 * 函数名： void att_digital_register(void)
 * 功  能： 模拟衰减器初始化
 * 输  入： -
 * 输  出： -
 */
Status_t switch_sp4t_register(struct __Switch_sp4t_TypeDef * pSwitchHandle)
{
    // 参数检查
    if(pSwitchHandle == NULL)
        return STAT_ERROR;
    // 函数指针初始化赋值
    pSwitchHandle->switch_ctl_stat = pSwitchHandle->hw_config.switch_init_stat;
    pSwitchHandle->open = switch_open;
    pSwitchHandle->ctl = switch_ctl;

    return STAT_OK;
}
