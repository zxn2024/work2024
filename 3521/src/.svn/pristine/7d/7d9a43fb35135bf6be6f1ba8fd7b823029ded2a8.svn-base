/****************************************************************************
 * Copyright(C)，2022，陕西天基通信科技有限责任公司
 * 文件名：t_analog_att.c
 * 内容简述：
 *
 * 文件历史：
 * 版本号 日期            作者    说明
 * 01    2023-9-15       扬扬    创建该文件
 */

#include "att_analog.h"
#include "rtt.h"



/****************************************************************************
 * 函数名： get_ana_att_tab_index
 * 功  能： 查表返回模拟衰减器衰减值在表中对应的元素(PWM比较值)
 * 输  入： ucTab: 指向表头的指针   ucTabLen：表元素个数，  ucValue：待查找值
 * 输  出： PWM比较值
 */
static uint16_t get_ana_att_tab_index(uint16_t (*ptr_tab), uint16_t tab_len, uint8_t att_val)
{
    uint8_t index = 0;

    if(att_val < tab_len)
    {
        index = att_val;
    }
    else
    {
        index = tab_len - 1;
    }

    return ptr_tab[index];
}

/****************************************************************************
 * 函数名： set_att_val_float
 * 功  能： 查表返回模拟衰减器衰减值在表中对应的元素(PWM比较值)
 * 输  入： ucTab: 指向表头的指针   ucTabLen：表元素个数，  ucValue：待查找值
 * 输  出： PWM比较值
 */
static Status_t set_att_val_float(ATT_analog_t * handle_att_analog, float att_val)
{
    uint8_t index = 0;
    uint16_t upper = 0, lower = 0, pwm_val = 0;
    float k = 0.0, b = 0.0;
    uint8_t tab_len = 0;
    uint16_t (*ptr_tab) = NULL;

    //参数检查
    if(handle_att_analog == NULL)
        return STAT_ERROR;

    tab_len = handle_att_analog->hw_config.att_max_number;
    ptr_tab = handle_att_analog->hw_config.att_table;

    for(index = 0; index < tab_len-1; index++)
    {
        if(ptr_tab[index] <= att_val && ptr_tab[index+1] > att_val && index < tab_len-1)
        {
            lower = ptr_tab[index];
            upper = ptr_tab[index+1];
            break;
        }else if(index == tab_len-1)
        {
            pwm_val = ptr_tab[index];
            break;
        }
    }
    if(index == tab_len-1)
    {
        hi_tim_pwm_set(handle_att_analog->hw_config.tim_ch, handle_att_analog->hw_config.pwm_ch, (uint16_t)pwm_val);
        return STAT_OK;
    }
 
    k = (float)(upper - lower) / (float)(1);
    b = (float)lower - (float)k * index;

    pwm_val = (uint16_t)(k * att_val + b);

    if(pwm_val > ptr_tab[index+1])
    {
	    rtt_printf( RTT_DRIVERS, RTT_TEXT_RED"[ WARN ]Analog att float value is out of max range\r\n" );
        pwm_val = ptr_tab[index+1];
    }else if(pwm_val < ptr_tab[index])
    {
	    rtt_printf( RTT_DRIVERS, RTT_TEXT_RED"[ WARN ]Analog att float value is out of min range\r\n" );
        pwm_val = ptr_tab[index];
    }

    hi_tim_pwm_set(handle_att_analog->hw_config.tim_ch, handle_att_analog->hw_config.pwm_ch, (uint16_t)pwm_val);

    handle_att_analog->attSetValue = (uint8_t)att_val;

    return STAT_OK;
}


/****************************************************************************
 * 函数名： vAnalogAttSet(ptRF_LENE_T pAnaAtt, uint8_t uAttValue)
 * 功  能： 设置模拟衰减器衰减值(PWM比较值)
 * 输  入： RF_LINE_T* AnaAtt 衰减器结构体   uAttValue 衰减值
 * 输  出： -
 */
static Status_t analog_att_set(ATT_analog_t * handle_att_analog, uint8_t value)
{
    uint16_t ccr = 0;
    //参数检查
    if(handle_att_analog == NULL)
        return STAT_ERROR;
    //查表
    ccr = get_ana_att_tab_index(handle_att_analog->hw_config.att_table, handle_att_analog->hw_config.att_max_number, value);
    //设置pwm
    hi_tim_pwm_set(handle_att_analog->hw_config.tim_ch, handle_att_analog->hw_config.pwm_ch, (uint16_t)ccr);

    handle_att_analog->attSetValue = value;
    return STAT_OK;
}

/****************************************************************************
 * 函数名： analog_pwm_set(ptRF_LENE_T pAnaAtt, uint8_t uAttValue)
 * 功  能： 设置模拟衰减器PWM比较值,用于测试采集衰减表
 * 输  入： RF_LINE_T* AnaAtt 衰减器结构体   pwm_val pwm比较值 0~999
 * 输  出： -
 */
static Status_t analog_pwm_set(ATT_analog_t * handle_att_analog, uint16_t pwm_val)
{
    //参数检查
    if(handle_att_analog == NULL)
        return STAT_ERROR;
    //设置pwm
    hi_tim_pwm_set(handle_att_analog->hw_config.tim_ch, handle_att_analog->hw_config.pwm_ch, pwm_val);

    return STAT_OK;
}

/****************************************************************************
 * 函数名： analog_pwm_set(ptRF_LENE_T pAnaAtt, uint8_t uAttValue)
 * 功  能： 设置模拟衰减器PWM比较值,用于测试采集衰减表
 * 输  入： RF_LINE_T* AnaAtt 衰减器结构体   pwm_val pwm比较值 0~999
 * 输  出： -
 */
static uint16_t analog_pwm_get(ATT_analog_t * handle_att_analog)
{
    //参数检查
    if(handle_att_analog == NULL)
        return 0;
    //获取pwm
    return(hi_tim_pwm_get(handle_att_analog->hw_config.tim_ch, handle_att_analog->hw_config.pwm_ch));
}
/****************************************************************************
 * 函数名： analog_att_open
 * 功  能： 模拟衰减器打开，默认初始为最大衰减值
 * 输  入： -
 * 输  出： -
 */
static Status_t analog_att_open(ATT_analog_t * handle_att_analog)
{
    //参数检查
    if(handle_att_analog == NULL)
        return STAT_ERROR;
    analog_att_set(handle_att_analog, (handle_att_analog->hw_config.att_max_number - 1));

    return STAT_OK;
}


/****************************************************************************
 * 函数名： void analog_att_close(void)
 * 功  能： 模拟衰减器初始化
 * 输  入： -
 * 输  出： -
 */
static Status_t analog_att_close(ATT_analog_t * handle_att_analog)
{
    //参数检查
    if(handle_att_analog == NULL)
        return STAT_ERROR;
    analog_att_set(handle_att_analog, handle_att_analog->hw_config.att_min_number);	

    return STAT_OK;
}

/****************************************************************************
 * 函数名： void analog_att_close(void)
 * 功  能： 模拟衰减器初始化
 * 输  入： -
 * 输  出： -
 */
Status_t att_analog_register(ATT_analog_t * handle_att_analog)
{
    //参数检查
    if(handle_att_analog == NULL || handle_att_analog->hw_config.att_table == NULL)
        return STAT_ERROR;
    //函数指针初始化赋值
    handle_att_analog->open = analog_att_open;
    handle_att_analog->close = analog_att_close;
    handle_att_analog->set = analog_att_set;
    handle_att_analog->float_set = set_att_val_float;
    handle_att_analog->pwm_set = analog_pwm_set;
    handle_att_analog->pwm_get = analog_pwm_get;
    handle_att_analog->attSetValue = 0;

    return STAT_OK;
}
