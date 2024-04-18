/****************************************************************************
 * Copyright(C)，2022，陕西天基通信科技有限责任公司
 * 文件名：t_digital_att.c
 * 内容简述：
 *      通用的数字衰减器驱动
 * 文件历史：
 * 版本号 日期            作者    说明
 * 01    2023-9-15       扬扬    创建该文件
 */

#include "lna.h"
#include "hi_gpio.h"
/*---------------------- 端口定义 --------------------------------------*/

/*---------------------- 宏定义  ---------------------------------------*/

/*---------------------- 声明 ------------------------------------------*/

/****************************************************************************
 * 函数名： lna_set_bypass(struct __LNA_TypeDef *pLnaHandle, LNA_BYPASS_E byp)
 * 功  能： 设置lna bypass
 * 输  入： pLnaHandle lna结构体
 * 输  入： byp LNA_BYS (0) LNA_RF (1)
 * 输  出： -
 */
static Status_t lna_set_bypass(struct __LNA_TypeDef * pLnaHandle, LNA_BYPASS_E byp)
{
    // 参数检查
    if(pLnaHandle == NULL)
        return STAT_ERROR;

    // 参数设置
    if(byp == LNA_BYS)
    {
        pLnaHandle->lna_byp_stat = byp;
        if(pLnaHandle->hw_config.byp_logic_invert == B_TRUE)
        {
            hi_gpio_write(pLnaHandle->hw_config.lna_byp_port, pLnaHandle->hw_config.lna_byp_pin, LEVEL_L);
        }
        else
        {
            hi_gpio_write(pLnaHandle->hw_config.lna_byp_port, pLnaHandle->hw_config.lna_byp_pin, LEVEL_H);
        }
    }
    else if(byp == LNA_RF)
    {
        pLnaHandle->lna_byp_stat = byp;
        if(pLnaHandle->hw_config.byp_logic_invert == B_TRUE)
        {
            hi_gpio_write(pLnaHandle->hw_config.lna_byp_port, pLnaHandle->hw_config.lna_byp_pin, LEVEL_H);
        }
        else
        {
            hi_gpio_write(pLnaHandle->hw_config.lna_byp_port, pLnaHandle->hw_config.lna_byp_pin, LEVEL_L);
        }
    }
    else
        return STAT_ERROR;

    return STAT_OK;
}

/****************************************************************************
 * 函数名： set_pwr(struct __LNA_TypeDef *pLnaHandle, LNA_PWR_E pwr_ctl)
 * 功  能： 设置lna
 * 输  入： pLnaHandle lna结构体
 * 输  入： stat PWR_OFF (0) PWR_ON (1)
 * 输  出： -
 */
static Status_t lna_set_pwr(struct __LNA_TypeDef * pLnaHandle, LNA_PWR_E pwr_ctl)
{
    // 参数检查
    if(pLnaHandle == NULL)
        return STAT_ERROR;

    // 参数设置
    if(pwr_ctl == PWR_OFF)
    {
        pLnaHandle->lna_pwr_stat = pwr_ctl;
        if(pLnaHandle->hw_config.pwr_logic_invert == B_TRUE)
        {
            hi_gpio_write(pLnaHandle->hw_config.lna_pwr_port, pLnaHandle->hw_config.lna_pwr_pin, LEVEL_L);
        }
        else
        {
            hi_gpio_write(pLnaHandle->hw_config.lna_pwr_port, pLnaHandle->hw_config.lna_pwr_pin, LEVEL_H);
        }
    }
    else if(pwr_ctl == PWR_ON)
    {
        pLnaHandle->lna_pwr_stat = pwr_ctl;
        if(pLnaHandle->hw_config.pwr_logic_invert == B_TRUE)
        {
            hi_gpio_write(pLnaHandle->hw_config.lna_pwr_port, pLnaHandle->hw_config.lna_pwr_pin, LEVEL_H);
        }
        else
        {
            hi_gpio_write(pLnaHandle->hw_config.lna_pwr_port, pLnaHandle->hw_config.lna_pwr_pin, LEVEL_L);
        }
    }
    else
        return STAT_ERROR;

    return STAT_OK;
}
/****************************************************************************
 * 函数名： lna_set_bypass(struct __LNA_TypeDef *pLnaHandle, LNA_BYPASS_E byp)
 * 功  能： 设置lna bypass
 * 输  入： pLnaHandle lna结构体
 * 输  入： byp LNA_BYS (0) LNA_RF (1)
 * 输  出： -
 */
static Status_t lna_open(struct __LNA_TypeDef * pLnaHandle)
{
    // 参数检查
    if(pLnaHandle == NULL)
        return STAT_ERROR;

    lna_set_bypass(pLnaHandle, pLnaHandle->hw_config.lna_init_byp_stat);
    lna_set_pwr(pLnaHandle, pLnaHandle->hw_config.lna_init_pwr_stat);
    return STAT_OK;
}
/****************************************************************************
 * 函数名： void att_digital_register(void)
 * 功  能： 模拟衰减器初始化
 * 输  入： -
 * 输  出： -
 */
Status_t lna_register(LNA_t * pLnaHandle)
{
    // 参数检查
    if(pLnaHandle == NULL)
        return STAT_ERROR;
    // 函数指针初始化赋值
    pLnaHandle->open = lna_open;
    pLnaHandle->set_bypass = lna_set_bypass;
    pLnaHandle->set_pwr = lna_set_pwr;
    pLnaHandle->lna_byp_stat = pLnaHandle->hw_config.lna_init_byp_stat;
    pLnaHandle->lna_pwr_stat = pLnaHandle->hw_config.lna_init_pwr_stat;
    return STAT_OK;
}
