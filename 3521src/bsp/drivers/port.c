/****************************************************************************
 * Copyright(C)，2022，陕西天基通信科技有限责任公司
 * 文件名：port.c
 * 内容简述：
 *      通用端口
 * 文件历史：
 */

#include "port.h"
#include "hi_gpio.h"
/*---------------------- 端口定义 --------------------------------------*/

/*---------------------- 宏定义  ---------------------------------------*/

/*---------------------- 声明 ------------------------------------------*/

/****************************************************************************
 * 函数名： set_pwr(struct __PORT_TypeDef *pPaHandle, PORT_CTL_E pwr_ctl)
 * 功  能： 设置port
 * 输  入： pPaHandle port结构体
 * 输  入： stat PORT_OFF (0) PORT_ON (1)
 * 输  出： -
 */
static Status_t port_ctl(struct __PORT_TypeDef * pPaHandle, PORT_CTL_E pwr_ctl)
{
    // 参数检查
    if(pPaHandle == NULL)
        return STAT_ERROR;

    // 参数设置
    if(pwr_ctl == PORT_OFF)
    {
        pPaHandle->port_ctl_stat = pwr_ctl;
        hi_gpio_write(pPaHandle->hw_config.port_ctl_port, pPaHandle->hw_config.port_ctl_pin, LEVEL_L);
    }
    else if(pwr_ctl == PORT_ON)
    {
        pPaHandle->port_ctl_stat = pwr_ctl;
        hi_gpio_write(pPaHandle->hw_config.port_ctl_port, pPaHandle->hw_config.port_ctl_pin, LEVEL_H);
    }
    else
        return STAT_ERROR;

    return STAT_OK;
}
/****************************************************************************
 * 函数名： set_pwr(struct __PORT_TypeDef *pPaHandle, PORT_CTL_E pwr_ctl)
 * 功  能： 设置port
 * 输  入： pPaHandle port结构体
 * 输  入： stat PORT_OFF (0) PORT_ON (1)
 * 输  出： -
 */
static Status_t port_open(struct __PORT_TypeDef * pPaHandle)
{
    // 参数检查
    if(pPaHandle == NULL)
        return STAT_ERROR;
    // 按照初始化参数打开设备
    return port_ctl(pPaHandle, pPaHandle->hw_config.port_init_stat);
}

/****************************************************************************
 * 函数名： void att_digital_register(void)
 * 功  能： PORT初始化
 * 输  入： -
 * 输  出： -
 */
Status_t port_register(PORT_t * pPaHandle)
{
    // 参数检查
    if(pPaHandle == NULL)
        return STAT_ERROR;
    // 函数指针初始化赋值
    pPaHandle->port_ctl_stat = pPaHandle->hw_config.port_init_stat;
    pPaHandle->open = port_open;
    pPaHandle->ctl = port_ctl;
    
    return STAT_OK;
}
