/****************************************************************************
 * Copyright(C)，2022，陕西天基通信科技有限责任公司
 * 文件名：t_digital_att.c
 * 内容简述：
 *      通用的数字衰减器驱动
 * 文件历史：
 * 版本号 日期            作者    说明
 * 01    2023-9-15       扬扬    创建该文件
 */

#include "pa.h"
#include "hi_gpio.h"
/*---------------------- 端口定义 --------------------------------------*/

/*---------------------- 宏定义  ---------------------------------------*/

/*---------------------- 声明 ------------------------------------------*/

/****************************************************************************
 * 函数名： set_pwr(struct __PA_TypeDef *pPaHandle, PA_CTL_E pwr_ctl)
 * 功  能： 设置pa
 * 输  入： pPaHandle pa结构体
 * 输  入： stat PA_OFF (0) PA_ON (1)
 * 输  出： -
 */
static Status_t pa_ctl(struct __PA_TypeDef * pPaHandle, PA_CTL_E pwr_ctl)
{
    // 参数检查
    if(pPaHandle == NULL)
        return STAT_ERROR;

    // 参数设置
    if(pwr_ctl == PA_OFF)
    {
        pPaHandle->pa_ctl_stat = pwr_ctl;
        if(pPaHandle->hw_config.ctl_logic_invert == B_TRUE)
        {
            hi_gpio_write(pPaHandle->hw_config.pa_ctl_port, pPaHandle->hw_config.pa_ctl_pin, LEVEL_L);
        }
        else
        {
            hi_gpio_write(pPaHandle->hw_config.pa_ctl_port, pPaHandle->hw_config.pa_ctl_pin, LEVEL_H);
        }
    }
    else if(pwr_ctl == PA_ON)
    {
        pPaHandle->pa_ctl_stat = pwr_ctl;
        if(pPaHandle->hw_config.ctl_logic_invert == B_TRUE)
        {
            hi_gpio_write(pPaHandle->hw_config.pa_ctl_port, pPaHandle->hw_config.pa_ctl_pin, LEVEL_H);
        }
        else
        {
            hi_gpio_write(pPaHandle->hw_config.pa_ctl_port, pPaHandle->hw_config.pa_ctl_pin, LEVEL_L);
        }
    }
    else
        return STAT_ERROR;

    return STAT_OK;
}
/****************************************************************************
 * 函数名： set_pwr(struct __PA_TypeDef *pPaHandle, PA_CTL_E pwr_ctl)
 * 功  能： 设置pa
 * 输  入： pPaHandle pa结构体
 * 输  入： stat PA_OFF (0) PA_ON (1)
 * 输  出： -
 */
static Status_t pa_open(struct __PA_TypeDef * pPaHandle)
{
    // 参数检查
    if(pPaHandle == NULL)
        return STAT_ERROR;
    // 按照初始化参数打开设备
    return pa_ctl(pPaHandle, pPaHandle->hw_config.pa_init_stat);
}
/****************************************************************************
 * 函数名： void att_digital_register(void)
 * 功  能： 模拟衰减器初始化
 * 输  入： -
 * 输  出： -
 */
Status_t pa_register(PA_t * pPaHandle)
{
    // 参数检查
    if(pPaHandle == NULL)
        return STAT_ERROR;
    // 函数指针初始化赋值
    pPaHandle->pa_ctl_stat = pPaHandle->hw_config.pa_init_stat;
    pPaHandle->open = pa_open;
    pPaHandle->ctl = pa_ctl;
    
    return STAT_OK;
}
