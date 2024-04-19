/*************************************** Copyright ******************************
 *
 * (C) Copyright 2023, Shaanxi Tianji Communication
 * All Rights Reserved
 *
 * FileName   : bi_PA.h
 * Version    : none
 * Author     :
 * Date       : 2023-09-21
 * Description: PA的通用接口模版,提供统一的PA接口:
 *              -BI_PA_ON(obj)
 *              -BI_PA_OFF(obj)
 *              -BI_PA_FLASH(obj, val)
 * 开发PA驱动时需要按照此模版进行
 *******************************************************************************/
//

#ifndef _BI_PA_H_
#define _BI_PA_H_

/*---------------------- PA驱动模版说明 ------------------------------------------*/
// PA驱动模板说明
// -PA驱动开发时，数据结构必须基于此接口模板，在此基础上新增变量和方法，但是不允许改动模板中已有的变量和方法。

/*
设备初始化结构体，包含MCU资源分配，比如GPIO分配，SPI端口，该设备的查找表等
__PA_xxxx_HwConfig_t其中xxxx代表设备的子类别，比如通过FPGA驱动PA，则在pa后增加FPGA后缀
typedef struct __PA_xxxx_HwConfig_t
{
    PA_Port:驱动中根据实际使用的端口类型和数量来定义
    PA_CTL_E pa_init_stat;  //PA初始化后的初始状态
    //此处定义该设备所需的硬件接口（如GPIO、SPI等），资源（内存，缓冲区），查找表（功率表，衰减表，温度表）
} PA_xxxx_HwConfig_t;

设备的数据结构，包含数据和所有对外接口
typedef struct __PA_ClassTypeDef
{
    PA_xxxx_HwConfig_t hw_config;                                                                      // 初始化参数
    PA_CTL_E pa_stat;                                                                                  // PA当前使能状态 PA_OFF (0) PA_ON (1) 
    Status_t (*ctl)(PA_xxxx_t *pPAHandle, PA_CTL_E ctl);                                               // PA使能控制
} PA_ClassTypeDef;

注册函数，用于注册一个设备并分配资源
Status_t PA_xxxx_register(PA_xxxx_t *handle_PA);
*/
/*---------------------- 端口定义 --------------------------------------*/

/*---------------------- 宏定义  ---------------------------------------*/
typedef enum
{
    PA_OFF = 0,            //关闭
    PA_ON = 1,             //打开
}PA_CTL_E;

/*---------------------- 对外接口 --------------------------------------*/

/*****************************************************************************
 * @Name	  : BI_PA_OPEN
 * @brief  : 设置PA工作模式,函数原型 Status_t (*set)(PA_xxxx_t *pPAHandle, PA_CTL_E pa_ctl);
 * @param  : pPAHandle: PA灯句柄指针
 * @param  : pa_ctl PA使能 PA_OFF (0) PA_ON (1) 
 * @Retval : Status_t
 * @Note   : 用来设置PA旁路
 */
#define BI_PA_OPEN(pPAHandle) (pPAHandle)->open((pPAHandle)) // PA开关

/*****************************************************************************
 * @Name	  : BI_PA_BYPASS
 * @brief  : 设置PA工作模式,函数原型 Status_t (*set)(PA_xxxx_t *pPAHandle, PA_CTL_E pa_ctl);
 * @param  : pPAHandle: PA灯句柄指针
 * @param  : pa_ctl PA使能 PA_OFF (0) PA_ON (1) 
 * @Retval : Status_t
 * @Note   : 用来设置PA旁路
 */
#define BI_PA_CTL(pPAHandle, pa_ctl) (pPAHandle)->ctl((pPAHandle), (pa_ctl)) // PA开关

#endif /* __BI_PA_H__ */
