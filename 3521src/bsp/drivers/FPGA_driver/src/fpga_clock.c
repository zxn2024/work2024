/**
  ******************************************************************************
  * @file    fpga_clock.c
  * @author  
  * @version V1.0.0
  * @date    2023-11-14
  * @brief   This file provides code for the fpga clock.
  ******************************************************************************
*/
#include <string.h>
//#include "gd32f4xx.h"
#include "fpga.h"
#include "fpga_clock.h"
/*
#include "hi_usb.h"
uint16_t g_fpga_buff_len;
static char g_fpga_buffer[200];
#define sh_printf(pFormat, arg...) do{ \
	g_fpga_buff_len=sprintf(g_fpga_buffer, pFormat, ##arg); \
	hi_usb_send((uint8_t *)g_fpga_buffer, g_fpga_buff_len); \
	}while(0)
*/

static int CurJZDA;	       /*当前晶振DA*/

struct axi_time_sys_Msg g_timeSysMsg;

/*
**********************************************************
*函数功能: ProWritePWM
*参数说明: dat
*返 回 值: dat
**********************************************************
*/
int ProWritePWM(int dat)
{
	fpga_reg_write(FPGA_REG_PWM, dat);
	return dat;
}

/*
**********************************************************
*函数功能: fpga_clock_init
*参数说明: 无
*返 回 值: 无
**********************************************************
*/
void fpga_clock_init(void)
{	
    memset(&g_timeSysMsg, 0, sizeof(struct axi_time_sys_Msg));
		g_timeSysMsg.meaRes_old = 103999999;
	
    CurJZDA = 22000;/*初始值*/

    ProWritePWM(CurJZDA);
}
/*
**********************************************************
*函数功能: fpga_clock_handle
*参数说明: 无
*返 回 值: 无
**********************************************************
*/
void fpga_clock_handle(void)
{
    g_timeSysMsg.meaVld = fpga_reg_read(FPGA_REG_MEA_VLD);
    g_timeSysMsg.meaRes = fpga_reg_read(FPGA_REG_MEA_DAT);
						
    /*sh_printf("meaVld=%d  meaRes=%d  CurJZDA=%d\r\n", g_timeSysMsg.meaVld, g_timeSysMsg.meaRes, CurJZDA);*/

    if (g_timeSysMsg.meaVld==1)
		{
				if ((g_timeSysMsg.meaRes > 104000010) && (g_timeSysMsg.meaRes_old > 104000010))/*当前频率较大*/	
						CurJZDA = CurJZDA - 10;
				else if ((g_timeSysMsg.meaRes < 103999990) && (g_timeSysMsg.meaRes_old < 103999990))/*当前频率较小*/	
						CurJZDA = CurJZDA + 10;	
				else if (g_timeSysMsg.meaRes > 104000000)/*微调*/
						CurJZDA = CurJZDA - 2;
				else if (g_timeSysMsg.meaRes < 103999999)/*微调*/
						CurJZDA = CurJZDA + 2;			
				
				g_timeSysMsg.meaRes_old = g_timeSysMsg.meaRes;
				
			
				/*压控反转*/
				if(CurJZDA >= 50000)
						CurJZDA = 50000;
				else if(CurJZDA < 0)
						CurJZDA = 0;			
			
				ProWritePWM(CurJZDA);
		}
		else
		{
				g_timeSysMsg.meaRes_old = 103999999;
		}
}

/*
**********************************************************
*函数功能: fpga_get_clock_pwm
*参数说明: 无
*返 回 值: pwm
**********************************************************
*/
int32_t fpga_get_clock_pwm(void)
{
    return CurJZDA;
}

/*
**********************************************************
*函数功能: fpga_set_clock_pwm
*参数说明: pwm
*返 回 值: 无
**********************************************************
*/
void fpga_set_clock_pwm(int32_t pwm)
{
    if ((pwm>0)	&& (pwm<50000))
    {
        CurJZDA = pwm;
        ProWritePWM(CurJZDA);
    }
}

/*
**********************************************************
*函数功能: fpga_get_clock_meaVld
*参数说明: 无
*返 回 值: meaVld
**********************************************************
*/
int8_t fpga_get_clock_meaVld(void)
{
    return g_timeSysMsg.meaVld;
}

/*
**********************************************************
*函数功能: fpga_get_clock_meaRes
*参数说明: 无
*返 回 值: meaRes
**********************************************************
*/
int32_t fpga_get_clock_meaRes(void)
{
    return g_timeSysMsg.meaRes;
}

