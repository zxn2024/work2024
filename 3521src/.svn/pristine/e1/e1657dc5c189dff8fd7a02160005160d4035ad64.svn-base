/****************************************************************************
 * Copyright(C)，2022，陕西天基通信科技有限责任公司
 * 文件名：t_digital_att.c
 * 内容简述：
 *      通用的数字衰减器驱动
 * 文件历史：
 * 版本号 日期            作者    说明
 * 01    2023-9-15       扬扬    创建该文件
 */

#include "att_digital.h"
#include "hi_gpio.h"
#include "hi_delay.h"
/*---------------------- 端口定义 --------------------------------------*/


/*---------------------- 宏定义  ---------------------------------------*/
#define DIGIT_ATT_SCK_L hi_gpio_write(handle_att_digital->hw_config.sck_port, handle_att_digital->hw_config.sck_pin,LEVEL_L);
#define DIGIT_ATT_SCK_H hi_gpio_write(handle_att_digital->hw_config.sck_port, handle_att_digital->hw_config.sck_pin,LEVEL_H);
#define DIGIT_ATT_SDA_L hi_gpio_write(handle_att_digital->hw_config.sda_port, handle_att_digital->hw_config.sda_pin,LEVEL_L);
#define DIGIT_ATT_SDA_H hi_gpio_write(handle_att_digital->hw_config.sda_port, handle_att_digital->hw_config.sda_pin,LEVEL_H);
#define DIGIT_ATT_LE_L hi_gpio_write(handle_att_digital->hw_config.le_port, handle_att_digital->hw_config.le_pin,LEVEL_L);
#define DIGIT_ATT_LE_H hi_gpio_write(handle_att_digital->hw_config.le_port, handle_att_digital->hw_config.le_pin,LEVEL_H);


/*---------------------- 声明 ------------------------------------------*/


/****************************************************************************
 * 函数名： digital_att_set(ATT_digital_t* handle_att_digital, uint8_t value)
 * 功  能： 设置衰减器衰减值
 * 输  入： ATT_digital_t 衰减器结构体   value 衰减值
 * 输  出： -
 */
static Status_t digital_att_set(ATT_digital_t * handle_att_digital, uint8_t value)
{
    //参数检查
    if(handle_att_digital == NULL)
        return STAT_ERROR;

    if(handle_att_digital->lock == UNLOCK)
        handle_att_digital->lock = LOCK;
    else
        return STAT_BUSY;

    uint8_t bit = 0x00;
    uint8_t ucValue = (uint8_t)value;

    value <<= 2;
    value &= 0x7E;

    DIGIT_ATT_SCK_L;
    DIGIT_ATT_SDA_L;
    DIGIT_ATT_LE_L;
    hi_delay_us(1);

    for(uint8_t i = 0; i <= 7; i++)
    {
        bit = 0x01 << i;
        if(ucValue & bit)
        {
            DIGIT_ATT_SDA_H;
        }
        else
        {
            DIGIT_ATT_SDA_L;
        }
        hi_delay_us(1);
        DIGIT_ATT_SCK_H;
        hi_delay_us(1);
        DIGIT_ATT_SCK_L;
    }
    hi_delay_us(1);
    DIGIT_ATT_LE_H;
    hi_delay_us(2);
    DIGIT_ATT_LE_L;
    hi_delay_us(1);
    DIGIT_ATT_SCK_L;
    DIGIT_ATT_SDA_L;

    handle_att_digital->attSetValue = value;
    handle_att_digital->lock = UNLOCK;
    return STAT_OK;
}

/****************************************************************************
 * 函数名： digital_att_open
 * 功  能： 衰减器打开，默认初始为最大衰减值??TODO :确认
 * 输  入： -
 * 输  出： -
 */
static Status_t digital_att_open(ATT_digital_t * handle_att_digital)
{
    //参数检查
    if(handle_att_digital == NULL)
        return STAT_ERROR;
    digital_att_set(handle_att_digital, handle_att_digital->hw_config.att_max_number);

    return STAT_OK;
}


/****************************************************************************
 * 函数名： void digital_att_close(void)
 * 功  能： 衰减器关闭
 * 输  入： -
 * 输  出： -
 */
static Status_t digital_att_close(ATT_digital_t * handle_att_digital)
{
    //参数检查
    if(handle_att_digital == NULL)
        return STAT_ERROR;
    return STAT_OK;
}

/****************************************************************************
 * 函数名： void att_digital_register(void)
 * 功  能： 模拟衰减器初始化
 * 输  入： -
 * 输  出： -
 */
Status_t att_digital_register(ATT_digital_t * handle_att_digital)
{
    //参数检查
    if(handle_att_digital == NULL)
        return STAT_ERROR;
    //函数指针初始化赋值
    handle_att_digital->open = digital_att_open;
    handle_att_digital->close = digital_att_close;
    handle_att_digital->set = digital_att_set;
    handle_att_digital->attSetValue = 0 ;
    handle_att_digital->lock = UNLOCK;
    return STAT_OK;
}
