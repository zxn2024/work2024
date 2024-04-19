/**
 * **********************************************************************************
 * @copyright Copyright (c) 2023 Shaanxi Tianji Communication Technologies Co., Ltd.
 *                         
 * @file        main.c
 * @author      WangJun
 * @version     None
 * @date        2023-09-04
 * @brief       3521B_MU
 * 
 * **********************************************************************************
 */

#include "chip.h"
#include "hi_init.h"
#include "os_init.h"
#include "rtt.h"
#include "bsp_config.h"
#include "bsp_rf_power_on.h"
#include "app_version.h"

int main(void)
{
	hi_vecttab_offset();	
    hi_chip_init();             /*芯片初始化：中断优先级分组、中断向量表、系统时钟配置*/
    rtt_init();                 /*rtt 初始化*/
    rtos_init();                /*rtos配置*/
    hi_mcu_peripheral_init();   /*外设配置*/
    bsp_register();             /*板级器件注册*/
    bsp_device_init();          /*板级器件初始化*/
 
	Ver_Init();                 /*flash中版本信息初始化*/
	fpga_ps_load();             /*fpga ps加载*/
	
	load_default_para_at_pow_on();	/* 根据默认配置完成数据初始化 */
	
    /* start scheduler */
    vTaskStartScheduler();
    
    while(1)
    {

    }
}  
