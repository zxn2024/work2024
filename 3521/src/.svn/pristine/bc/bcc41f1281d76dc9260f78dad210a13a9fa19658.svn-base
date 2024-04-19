/**
  ******************************************************************************
  * @file    fpga_task.c
  * @author  
  * @version V1.0.0
  * @date    2023-10-14
  * @brief   This file provides code for the fpga task.
  ******************************************************************************
*/
//#include "gd32f4xx.h"
#include "port.h"
#include "hi_gpio.h"
#include "FreeRTOS.h"
#include "task.h"
#include "fpga.h"
#include "fpga_spi.h"
#include "fpga_ook.h"
#include "fpga_clock.h"
#include "fpga_task.h"

static uint32_t g_fpga_ver_time;/*FPGA版本时间*/
static uint32_t g_fpga_ver_num; /*FPGA版本号*/
static uint8_t  g_fpga_state;   /*FPGA状态标志*/
static uint8_t  g_fpga_ook_hop_freq_flag;/*OOK跳频标志  主机*/
static uint8_t  g_fpga_ook_cfg_result = 0;/*OOK配置结果，1配置成功*/	

extern PORT_t fpga_jtag_en; 

/*
**********************************************************
*函数功能: fpga_ook_hop_freq ook跳频
*参数说明: 无
*返 回 值: 1成功
**********************************************************
*/
uint8_t fpga_ook_hop_freq(void)
{
    g_fpga_ook_hop_freq_flag = 1;
    return FPGA_OP_SUCCESS;	 
}	
/*
**********************************************************
*函数功能: fpga_get_state 获取fpga的状态
*参数说明: 无
*返 回 值: 1状态正常
**********************************************************
*/
uint8_t fpga_get_state(void)
{
    return g_fpga_state;
}
/*
**********************************************************
*函数功能: fpga_state_check FPGA状态检测
*参数说明: 无
*返 回 值: 无
**********************************************************
*/
static void fpga_state_check(void)
{
    static uint32_t state_value_old = 0;	
    uint32_t state_value = 0;

    state_value = fpga_reg_read(FPGA_REG_STATE);
    if (state_value != state_value_old)
    {
        state_value_old = state_value;
        g_fpga_state = 1;
    }
    else
    {
        g_fpga_state = 0;
    }
}

/*
**********************************************************
*函数功能: fpga_ook_freq_control FPGA OOK频点控制
*参数说明: 无
*返 回 值: 无
**********************************************************
*/
static void fpga_ook_freq_control(void)
{
    if (g_fpga_ook_hop_freq_flag == 1)
    {
        g_fpga_ook_hop_freq_flag = 0;
        OOK_HotFrequency();
    }
}

/*
**********************************************************
*函数功能: fpga_task
*参数说明: pvParameters
*返 回 值: 无
**********************************************************
*/
static void fpga_task(void *pvParameters)
{
    uint8_t counter = 0;	
	
    //fpga_spi_init();
    
    //fpga_ps_load(); 放在外设初始化里面
	
    vTaskDelay(1000);
	
    /*读FPGA版本时间和版本号*/
    g_fpga_ver_time = fpga_reg_read(FPGA_REG_VER_TIME);
    g_fpga_ver_num  = fpga_reg_read(FPGA_REG_VER_NUM);	
    g_fpga_ver_time = fpga_reg_read(FPGA_REG_VER_TIME);
	
    /*ook初始化*/
    fpga_reg_write(FPGA_REG_OOK_RESET, 1);
    vTaskDelay(100);
    fpga_reg_write(FPGA_REG_OOK_RESET, 0);	
    if (1 == OOK_Init())
    {
        g_fpga_ook_cfg_result = 1;
    }
		else
		{
        /*ook异常处理*/			
		}
	
		/*jtag引脚复用，除设置jtag_en低电平外，还需通过jtag配置一下，并下载.xfb文件*/
		fpga_jtag_en.ctl(&fpga_jtag_en, PORT_OFF);//hi_gpio_write(fpga_jtag_en.hw_config.port_ctl_port, fpga_jtag_en.hw_config.port_ctl_pin, LEVEL_L);	
		
		/*clock初始化*/
		fpga_clock_init();
				
    while(1)
    {
        counter++;
			
        if (counter >= 10)
        {
            counter = 0;
					
            /*FPGA状态检测*/
            fpga_state_check();
			
            /*时钟锁相环控制*/
						fpga_clock_handle();

        }
			
        /*OOK频点控制 主机*/
				fpga_ook_freq_control();
								
        vTaskDelay(100);
    }	
}

/*
**********************************************************
*函数功能: fpga 任务初始化 启动FPGA任务 开始spi初始化 状态检测
*参数说明: 无
*返 回 值: 无
**********************************************************
*/
void fpga_task_init(void)
{
    g_fpga_ook_hop_freq_flag = 0;
		
    xTaskCreate(fpga_task, "fpga_task", configMINIMAL_STACK_SIZE * 2, NULL, tskIDLE_PRIORITY + 1, NULL);
}

/*
**********************************************************
*函数功能: fpga_ver_time
*参数说明: 无
*返 回 值: fpga版本时间
**********************************************************
*/
uint32_t fpga_ver_time(void)
{
    return g_fpga_ver_time;
}

/*
**********************************************************
*函数功能: fpga_ver_num
*参数说明: 无
*返 回 值: fpga版本号
**********************************************************
*/
uint32_t fpga_ver_num(void)
{
    return g_fpga_ver_num;
}

/*
**********************************************************
*函数功能: fpga_ook_state 获取ook的状态
*参数说明: 无
*返 回 值: 1正常
**********************************************************
*/
uint8_t fpga_ook_state(void)
{
    uint8_t state;

    state = CMT2300A_GetChipStatus();
    if ((g_fpga_ook_cfg_result==1) && (state==CMT2300A_STA_TX))
        return 1;
		else if (g_fpga_ook_cfg_result!=1)	
        return 2;
    else if (state!=CMT2300A_STA_TX)
        return 3;
    else 
        return 0;	
}

/*
**********************************************************
*函数功能: fpga_ook_off ook关电
*参数说明: 无
*返 回 值: 无
**********************************************************
*/
void fpga_ook_off(void)
{
	CMT2300A_GoStby();
    fpga_reg_write(FPGA_REG_OOK_RESET, 1);	
    g_fpga_ook_cfg_result = 0;
}

/*
**********************************************************
*函数功能: fpga_ook_off ook开电
*参数说明: 无
*返 回 值: 无
**********************************************************
*/
void fpga_ook_on(void)
{
    fpga_reg_write(FPGA_REG_OOK_RESET, 1);
    vTaskDelay(100);
    fpga_reg_write(FPGA_REG_OOK_RESET, 0);	
    if (1 == OOK_Init())
    {
        g_fpga_ook_cfg_result = 1;
    }
		else
		{
        /*ook异常处理*/			
		}
}
