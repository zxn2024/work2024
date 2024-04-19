/**
  ******************************************************************************
  * @file    bsp_shell.c
  * @author  
  * @version V1.0.0
  * @date    2020-09-14
  * @brief   This file provides code for the debug uart.
  ******************************************************************************
*/
#include <stdio.h>
#include <string.h>
//#include "gd32f4xx.h"
//#include "systick.h"
#include "hi_usb.h"
#include "Bsp_config.h"
#include "Ecr8668_driver.h"
#include "usart.h"
#include "fpga.h"
#include "fpga_ook.h"

#define PROMPT ""//"3521 #" 

#define INPUT_MAX_RECV_LEN 100 

#define INPUT_CMD_HISTORY_MAX_NUM 11

#define SYS_TABLE_MAX_LEN 80

#define CMD_PARA_NUM 6	

#define INPUT_CMD_MAX_LEN 100 

#define sh_printf(pFormat, arg...) do{ \
	g_sh_buff_len=sprintf(g_sh_buffer, pFormat, ##arg); \
	hi_usb_send((uint8_t *)g_sh_buffer, g_sh_buff_len); \
	}while(0)

typedef int32_t (*EXEC_FUNC)(int32_t, int32_t, int32_t, int32_t, int32_t, int32_t);

typedef struct
{
	char *FuncName;		/*function name*/
	EXEC_FUNC Func;		/*function address*/
	char acPara[CMD_PARA_NUM];		/*function para type, u int, c string, 0 null*/
	char *pcParaEx;
}SYM_TABLE;

typedef struct
{
  char *pcName;
  int32_t *pFunc;
  char *pcPara;
}TShell_Cmd;

/*调试函数声明*/
static void Shell_Help(void);
static void set_ook_power(unsigned int power);
static void show_fpga_state(void);
static void show_fpga_version(void);
static void set_4g_freqband(unsigned int fb);
static void on_4g_dl(void);
static void off_4g_dl(void);
static void on_4g_ul(void);
static void off_4g_ul(void);
static void show_4g_freqband(void);
static void set_5g_frame(char *pf);
static void show_5g_frame(void);
static void set_4g_subframe(unsigned int sf);
static void show_4g_subframe(void);
static void set_4g_specical_subframe(unsigned int ssf);
static void show_4g_specical_subframe(void);
static void set_4g_center_freq(unsigned int cf);
static void show_4g_center_freq(void);
static void set_4g_bandwidth(unsigned int bw);
static void show_4g_bandwidth(void);
static void show_frame_type(void);
static void show_4g_frame_dev(void);
static void ook_hop_freq(void);
static void show_ook_state(void);
static void ECR8668_init(unsigned int type, unsigned int band);
static void ECR8668_Ext_Write(unsigned int type, unsigned int addr, unsigned int value);
static void ECR8668_In_Write(unsigned int type, unsigned int addr, unsigned int value);
static void ECR8668_Ext_Read(unsigned int type, unsigned int addr);
static void ECR8668_In_Read(unsigned int type, unsigned int addr);
static void set_modem_cmd(char *pCmd);
static void set_gpio(char *pc);
static void get_gpio(char *pc);
static void set_att(uint32_t type, uint32_t pwm_val);
static void set_clock_pwm(unsigned int pwm);
static void show_clock_pwm(void);

static SYM_TABLE g_sys_table[]={  /*Sys table*/
	  {"help",            (EXEC_FUNC)Shell_Help,      {0,  0,0,0,0,0}, ""},
		{"set_ook_power", (EXEC_FUNC)set_ook_power, {'u',0,0,0,0,0}, "0 or 1"},
		{"show_fpga_version", (EXEC_FUNC)show_fpga_version, {0,0,0,0,0,0}, ""},
		{"show_fpga_state", (EXEC_FUNC)show_fpga_state, {0,0,0,0,0,0}, ""},
		{"set_4g_freqband",  (EXEC_FUNC)set_4g_freqband,  {'u',0,0,0,0,0}, "0:B3 1:B8 2:F 3:E"},
		{"on_4g_dl",  (EXEC_FUNC)on_4g_dl,  {0,0,0,0,0,0}, ""},
		{"off_4g_dl", (EXEC_FUNC)off_4g_dl, {0,0,0,0,0,0}, ""},
		{"on_4g_ul",  (EXEC_FUNC)on_4g_ul,  {0,0,0,0,0,0}, ""},
		{"off_4g_ul", (EXEC_FUNC)off_4g_ul, {0,0,0,0,0,0}, ""},	
		{"show_4g_freqband", (EXEC_FUNC)show_4g_freqband, {0,0,0,0,0,0}, ""},
		{"set_5g_frame", (EXEC_FUNC)set_5g_frame, {'c',0,0,0,0,0}, "\"02070000040406\""},
		{"show_5g_frame", (EXEC_FUNC)show_5g_frame, {0,0,0,0,0,0}, ""},
		{"set_4g_subframe", (EXEC_FUNC)set_4g_subframe, {'u',0,0,0,0,0}, "0-6"},
		{"show_4g_subframe", (EXEC_FUNC)show_4g_subframe, {0,0,0,0,0,0}, ""},
		{"set_4g_specical_subframe", (EXEC_FUNC)set_4g_specical_subframe, {'u',0,0,0,0,0}, "0-8"},
		{"show_4g_specical_subframe", (EXEC_FUNC)show_4g_specical_subframe, {0,0,0,0,0,0}, ""},		
		{"set_4g_center_freq", (EXEC_FUNC)set_4g_center_freq, {'u',0,0,0,0,0}, "center freq"},
		{"show_4g_center_freq", (EXEC_FUNC)show_4g_center_freq, {0,0,0,0,0,0}, ""},	
		{"set_4g_bandwidth", (EXEC_FUNC)set_4g_bandwidth, {'u',0,0,0,0,0}, "bandwidth"},
		{"show_4g_bandwidth", (EXEC_FUNC)show_4g_bandwidth, {0,0,0,0,0,0}, ""},
		{"show_frame_type", (EXEC_FUNC)show_frame_type, {0,0,0,0,0,0}, ""},
		{"show_4g_frame_dev", (EXEC_FUNC)show_4g_frame_dev, {0,0,0,0,0,0}, ""},
		{"ook_hop_freq", (EXEC_FUNC)ook_hop_freq, {0,0,0,0,0,0}, ""},
		{"show_ook_state", (EXEC_FUNC)show_ook_state, {0,0,0,0,0,0}, ""},
		{"ECR8668_init", (EXEC_FUNC)ECR8668_init, {'u','u',0,0,0,0}, "0:LTE 1:NR 2:NR MIMO, 0:B3 1:B8 2:F 3:E"},
		{"ECR8668_Ext_Write", (EXEC_FUNC)ECR8668_Ext_Write, {'u','u','u',0,0,0}, "0:LTE 1:NR 2:NR MIMO, addr, data"},
		{"ECR8668_In_Write", (EXEC_FUNC)ECR8668_In_Write, {'u','u','u',0,0,0}, "0:LTE 1:NR 2:NR MIMO, addr, data"},
		{"ECR8668_Ext_Read", (EXEC_FUNC)ECR8668_Ext_Read, {'u','u',0,0,0,0}, "0:LTE 1:NR 2:NR MIMO, addr"},
		{"ECR8668_In_Read", (EXEC_FUNC)ECR8668_In_Read, {'u','u',0,0,0,0}, "0:LTE 1:NR 2:NR MIMO, addr"},
		{"set_modem_cmd", (EXEC_FUNC)set_modem_cmd, {'c',0,0,0,0,0}, "at cmd"},
		{"set_gpio", (EXEC_FUNC)set_gpio, {'c',0,0,0,0,0}, "\"A,14,1\""},		
		{"get_gpio", (EXEC_FUNC)get_gpio, {'c',0,0,0,0,0}, "\"A,14\""},
		{"set_att", (EXEC_FUNC)set_att, {'u','u',0,0,0,0}, "0:4g_dw 1:nr_dw 2:mimo_dw 3:b3_up_1 4:b3_up_2 5:b8_up_1 6:b8_up_2 7:b39_up_1 8:b39_up_2 9:b40_up_1 10:b40_up_2 11:nr_up_1 12:nr_up_2 13:mimo_up_1 14:mimo_up_2, att"},	
		{"set_clock_pwm", (EXEC_FUNC)set_clock_pwm, {'u',0,0,0,0,0}, "0 - 50000"},
		{"show_clock_pwm", (EXEC_FUNC)show_clock_pwm, {0,0,0,0,0,0}, ""}
};
static int32_t g_sys_table_len = sizeof(g_sys_table)/sizeof(SYM_TABLE);

void Shell_Exec_SymFunc(char *ac_input_cmd);

static char g_sh_buffer[200];
static uint16_t g_sh_buff_len;


/*调试函数实现*/






/*
0 att_analog_4g_dw
1 att_analog_nr_dw
2 att_analog_mimo_dw
3 att_analog_b3_up_1
4 att_analog_b3_up_2
5 att_analog_b8_up_1
6 att_analog_b8_up_2
7 att_analog_b39_up_1
8 att_analog_b39_up_2
9 att_analog_b40_up_1
10 att_analog_b40_up_2
11 att_analog_nr_up_1
12 att_analog_nr_up_2
13 att_analog_mimo_up_1
14 att_analog_mimo_up_2
*/
static void set_att(uint32_t type, uint32_t pwm_val)
{
    if (type == 0)
        hi_tim_pwm_set(att_analog_4g_dw.hw_config.tim_ch, att_analog_4g_dw.hw_config.pwm_ch, pwm_val);
    else if (type == 1)
		    hi_tim_pwm_set(att_analog_nr_dw.hw_config.tim_ch, att_analog_nr_dw.hw_config.pwm_ch, pwm_val);
    else if (type == 2)
		    hi_tim_pwm_set(att_analog_mimo_dw.hw_config.tim_ch, att_analog_mimo_dw.hw_config.pwm_ch, pwm_val);
    else if (type == 3)
		    hi_tim_pwm_set(att_analog_b3_up_1.hw_config.tim_ch, att_analog_b3_up_1.hw_config.pwm_ch, pwm_val);
    else if (type == 4)
		    hi_tim_pwm_set(att_analog_b3_up_2.hw_config.tim_ch, att_analog_b3_up_2.hw_config.pwm_ch, pwm_val);
    else if (type == 5)
		    hi_tim_pwm_set(att_analog_b8_up_1.hw_config.tim_ch, att_analog_b8_up_1.hw_config.pwm_ch, pwm_val);
    else if (type == 6)
		    hi_tim_pwm_set(att_analog_b8_up_2.hw_config.tim_ch, att_analog_b8_up_2.hw_config.pwm_ch, pwm_val);
    else if (type == 7)
		    hi_tim_pwm_set(att_analog_b39_up_1.hw_config.tim_ch, att_analog_b39_up_1.hw_config.pwm_ch, pwm_val);
    else if (type == 8)
		    hi_tim_pwm_set(att_analog_b39_up_2.hw_config.tim_ch, att_analog_b39_up_2.hw_config.pwm_ch, pwm_val);
    else if (type == 9)
		    hi_tim_pwm_set(att_analog_b40_up_1.hw_config.tim_ch, att_analog_b40_up_1.hw_config.pwm_ch, pwm_val);
    else if (type == 10)
		    hi_tim_pwm_set(att_analog_b40_up_2.hw_config.tim_ch, att_analog_b40_up_2.hw_config.pwm_ch, pwm_val);
    else if (type == 11)
		    hi_tim_pwm_set(att_analog_nr_up_1.hw_config.tim_ch, att_analog_nr_up_1.hw_config.pwm_ch, pwm_val);
    else if (type == 12)
		    hi_tim_pwm_set(att_analog_nr_up_2.hw_config.tim_ch, att_analog_nr_up_2.hw_config.pwm_ch, pwm_val);
    else if (type == 13)
		    hi_tim_pwm_set(att_analog_mimo_up_1.hw_config.tim_ch, att_analog_mimo_up_1.hw_config.pwm_ch, pwm_val);
    else if (type == 14)
		    hi_tim_pwm_set(att_analog_mimo_up_2.hw_config.tim_ch, att_analog_mimo_up_2.hw_config.pwm_ch, pwm_val);
}

static void set_ook_power(unsigned int power)
{
    if (power == 1)
        fpga_reg_write(FPGA_REG_OOK_RESET, 1);
		else
        fpga_reg_write(FPGA_REG_OOK_RESET, 0);
}

static void set_4g_freqband(unsigned int fb)
{
    fpga_set_4g_freqband(fb);
}

static void on_4g_dl(void)
{
    fpga_4g_dl_pa_on();
}

static void off_4g_dl(void)
{
    fpga_4g_dl_pa_off();
}

static void on_4g_ul(void)
{
    fpga_4g_ul_pa_on();
}

static void off_4g_ul(void)
{
    fpga_4g_ul_pa_off();
}

static void show_4g_freqband(void)
{
    sh_printf("4g freqband: %d (0:B8 1:B3 2:F 3:E)\r\n",fpga_get_4g_freqband());
}

static void set_5g_frame(char *pf)
{
    fpga_set_5g_frame(pf);
}

static void show_5g_frame(void)
{
    char acBuf[30];
    memset(acBuf, 0, 30);
    fpga_get_5g_frame(acBuf);
    sh_printf("5g frame: %s\r\n",acBuf);
}

static void set_4g_subframe(unsigned int sf)
{
    fpga_set_4g_subframe(sf);
}

static void show_4g_subframe(void)
{
    sh_printf("4g subframe: %d\r\n",fpga_get_4g_subframe());
}

static void set_4g_specical_subframe(unsigned int ssf)
{
    fpga_set_4g_specical_subframe(ssf);
}

static void show_4g_specical_subframe(void)
{
    sh_printf("4g specical subframe: %d\r\n",fpga_get_4g_specical_subframe());
}

static void set_4g_center_freq(unsigned int cf)
{
    fpga_set_4g_center_freq(cf);
}

static void show_4g_center_freq(void)
{
    sh_printf("4g center freq: %d\r\n",fpga_get_4g_center_freq());
}

static void set_4g_bandwidth(unsigned int bw)
{
    fpga_set_4g_bandwidth(bw);
}

static void show_4g_bandwidth(void)
{
    sh_printf("4g bandwidth: %d\r\n",fpga_get_4g_bandwidth());
}

static void show_fpga_state(void)
{
    sh_printf("fpga state: %d\r\n",fpga_get_state());
}

static void show_fpga_version(void)
{
    sh_printf("fpga version: %d  %x\r\n",fpga_ver_num(),fpga_ver_time());
}

static void show_frame_type(void)
{
    sh_printf("frame type: %d (0:5G DL 1:4G DL 2:5ms)\r\n",fpga_get_frame_type());
}

static void show_4g_frame_dev(void)
{
    sh_printf("4g frame dev: %d\r\n",fpga_get_4g_frame_dev());
}

static void ook_hop_freq(void)
{
    fpga_ook_hop_freq();
}

static void show_ook_state(void)
{
    sh_printf("ook state: %d\r\n",fpga_ook_state());
    sh_printf("ook freq : %d\r\n",OOK_GetFrequency());
}

static void ECR8668_init(unsigned int type, unsigned int band)
{
    if (type == 0)
    {
        if (band < 4)
        {
            h8668_LTE.band = (RF_BAND_E)band;
            if (BSP_Ecr8668Init(&h8668_LTE) == STAT_ERROR)
                sh_printf("LTE Ecr8668 init fail!\r\n");
            else
                sh_printf("LTE Ecr8668 init success!\r\n");	
        }
		else
		{
            sh_printf("LTE band error!\r\n");	
		}
    }
    else if (type == 1)
    {
        h8668_NR.band = (RF_BAND_E)4;
        if (BSP_Ecr8668Init(&h8668_NR) == STAT_ERROR)
            sh_printf("NR Ecr8668 init fail!\r\n");
        else
            sh_printf("NR Ecr8668 init success!\r\n");
    }	
    else if (type == 2)
    {
        h8668_NR_MIMO.band = (RF_BAND_E)5;			
        if (BSP_Ecr8668Init(&h8668_NR_MIMO) == STAT_ERROR)
            sh_printf("NR MIMO Ecr8668 init fail!\r\n");
        else
            sh_printf("NR MIMO Ecr8668 init success!\r\n");
    }		
    else
    {
    
    }	
}

static void ECR8668_Ext_Write(unsigned int type, unsigned int addr, unsigned int value)
{
    if (type == 0)
    {
        BSP_866x_Ext_Write(&h8668_LTE, addr, value);
        sh_printf("LTE Ecr8668 Ext Reg Addr 0x%08x    Write Data 0x%08x\r\n", addr, value);	
    }
    else if (type == 1)
    {
        BSP_866x_Ext_Write(&h8668_NR, addr, value);
        sh_printf("NR Ecr8668 Ext Reg Addr 0x%08x    Write Data 0x%08x\r\n", addr, value);	
    }	
    else if (type == 2)
    {
        BSP_866x_Ext_Write(&h8668_NR_MIMO, addr, value);
        sh_printf("NR MIMO Ecr8668 Ext Reg Addr 0x%08x    Write Data 0x%08x\r\n", addr, value);	
    }		
    else
    {
    }		
}

static void ECR8668_In_Write(unsigned int type, unsigned int addr, unsigned int value)
{
    if (type == 0)
    {
        BSP_866x_In_Write(&h8668_LTE, addr, value);
        sh_printf("LTE Ecr8668 In Reg Addr 0x%08x    Write Data 0x%08x\r\n", addr, value);	
    }
    else if (type == 1)
    {
        BSP_866x_In_Write(&h8668_NR, addr, value);
        sh_printf("NR Ecr8668 In Reg Addr 0x%08x    Write Data 0x%08x\r\n", addr, value);	
    }	
    else if (type == 2)
    {
        BSP_866x_In_Write(&h8668_NR_MIMO, addr, value);
        sh_printf("NR MIMO Ecr8668 In Reg Addr 0x%08x    Write Data 0x%08x\r\n", addr, value);	
    }		
    else
    {
    }	
}

static void ECR8668_Ext_Read(unsigned int type, unsigned int addr)
{
	  uint32_t  Data;	
	
    if (type == 0)
    {
        Data = BSP_866x_Ex_Read(&h8668_LTE, addr);
        sh_printf("LTE Ecr8668 Ext Reg Addr 0x%08x    Read Data 0x%08x\r\n", addr, Data);	
    }
    else if (type == 1)
    {
        Data = BSP_866x_Ex_Read(&h8668_NR, addr);
        sh_printf("NR Ecr8668 Ext Reg Addr 0x%08x    Read Data 0x%08x\r\n", addr, Data);				
    }	
    else if (type == 2)
    {
        Data = BSP_866x_Ex_Read(&h8668_NR_MIMO, addr);
        sh_printf("NR MIMO Ecr8668 Ext Reg Addr 0x%08x    Read Data 0x%08x\r\n", addr, Data);	
    }		
    else
    {
    }			
}

static void ECR8668_In_Read(unsigned int type, unsigned int addr)
{
	  uint32_t  Data;	
	
    if (type == 0)
    {
        Data = BSP_866x_In_Read(&h8668_LTE, addr);
        sh_printf("LTE Ecr8668 In Reg Addr 0x%08x    Read Data 0x%08x\r\n", addr, Data);	
    }
    else if (type == 1)
    {
        Data = BSP_866x_In_Read(&h8668_NR, addr);
        sh_printf("NR Ecr8668 In Reg Addr 0x%08x    Read Data 0x%08x\r\n", addr, Data);				
    }	
    else if (type == 2)
    {
        Data = BSP_866x_In_Read(&h8668_NR_MIMO, addr);
        sh_printf("NR MIMO Ecr8668 In Reg Addr 0x%08x    Read Data 0x%08x\r\n", addr, Data);	
    }		
    else
    {
    }	
}

static void set_modem_cmd(char *pCmd)
{
    char acBuf[50];
    int len;

    memset(acBuf, 0, 50);
    len = sprintf(acBuf, "%s\r\n", pCmd);
	
#if(PRODUCT_MODEL == CMDT3521A)	
    usart0_dma_send((uint8_t *)acBuf, len);
#elif(PRODUCT_MODEL == CMDT3521B)
    usart0_dma_send((uint8_t *)acBuf, len);	
#elif(PRODUCT_MODEL == CMDT3521C)
    HAL_UART_Transmit_DMA(&huart1, (uint8_t *)acBuf, len);
#endif	
}

static void set_gpio(char *pc)
{
        char group;
        uint32_t number = 0, stat = 0;
        uint32_t io_group, io_num;
        
        sscanf((char *)pc, "%c,%d,%d", &group, &number, &stat);

        switch (group)
        {
        case 'A':
        case 'a':
            io_group = (uint32_t)GPIOA;
            break;
        case 'B':
        case 'b':
            io_group = (uint32_t)GPIOB;
            break;
        case 'C':
        case 'c':
            io_group = (uint32_t)GPIOC;
            break;
        case 'D':
        case 'd':
            io_group = (uint32_t)GPIOD;
            break;
        case 'E':
        case 'e':
            io_group = (uint32_t)GPIOE;
            break;
        case 'F':
        case 'f':
            io_group = (uint32_t)GPIOF;
            break;
        case 'G':
        case 'g':
            io_group = (uint32_t)GPIOG;
            break;
        case 'H':
        case 'h':
            io_group = (uint32_t)GPIOH;
            break;
        case 'I':
        case 'i':
            io_group = (uint32_t)GPIOI;
            break;
        
        default:
            sh_printf("Param error\r\n");	
            return;
        }

        switch (number)
        {
        case 0:
            io_num = GPIO_PIN_0;
            break;

        case 1:
            io_num = GPIO_PIN_1;
            break;

        case 2:
            io_num = GPIO_PIN_2;
            break;

        case 3:
            io_num = GPIO_PIN_3;
            break;

        case 4:
            io_num = GPIO_PIN_4;
            break;

        case 5:
            io_num = GPIO_PIN_5;
            break;

        case 6:
            io_num = GPIO_PIN_6;
            break;

        case 7:
            io_num = GPIO_PIN_7;
            break;

        case 8:
            io_num = GPIO_PIN_8;
            break;

        case 9:
            io_num = GPIO_PIN_9;
            break;

        case 10:
            io_num = GPIO_PIN_10;
            break;

        case 11:
            io_num = GPIO_PIN_11;
            break;

        case 12:
            io_num = GPIO_PIN_12;
            break;

        case 13:
            io_num = GPIO_PIN_13;
            break;

        case 14:
            io_num = GPIO_PIN_14;
            break;

        case 15:
            io_num = GPIO_PIN_15;
            break;
        
        default:
            sh_printf("Param error\r\n");
            return;
        }

        if(stat > 1)
        {
            sh_printf("Param error\r\n");
					  return;
        }

        hi_gpio_write((PORT_T)io_group, io_num, (ELEC_LEVEL_E)stat);

        sh_printf("OK\r\n");
}


static void get_gpio(char *pc)
{
        char group;
        uint32_t number = 0, stat = 0;
        uint32_t io_group, io_num;
        
        sscanf((char *)pc, "%c,%d", &group, &number);

        switch (group)
        {
        case 'A':
        case 'a':
            io_group = (uint32_t)GPIOA;
            break;
        case 'B':
        case 'b':
            io_group = (uint32_t)GPIOB;
            break;
        case 'C':
        case 'c':
            io_group = (uint32_t)GPIOC;
            break;
        case 'D':
        case 'd':
            io_group = (uint32_t)GPIOD;
            break;
        case 'E':
        case 'e':
            io_group = (uint32_t)GPIOE;
            break;
        case 'F':
        case 'f':
            io_group = (uint32_t)GPIOF;
            break;
        case 'G':
        case 'g':
            io_group = (uint32_t)GPIOG;
            break;
        case 'H':
        case 'h':
            io_group = (uint32_t)GPIOH;
            break;
        case 'I':
        case 'i':
            io_group = (uint32_t)GPIOI;
            break;
        
        default:
            sh_printf("Param error\r\n");
            return;
        }

        switch (number)
        {
        case 0:
            io_num = GPIO_PIN_0;
            break;

        case 1:
            io_num = GPIO_PIN_1;
            break;

        case 2:
            io_num = GPIO_PIN_2;
            break;

        case 3:
            io_num = GPIO_PIN_3;
            break;

        case 4:
            io_num = GPIO_PIN_4;
            break;

        case 5:
            io_num = GPIO_PIN_5;
            break;

        case 6:
            io_num = GPIO_PIN_6;
            break;

        case 7:
            io_num = GPIO_PIN_7;
            break;

        case 8:
            io_num = GPIO_PIN_8;
            break;

        case 9:
            io_num = GPIO_PIN_9;
            break;

        case 10:
            io_num = GPIO_PIN_10;
            break;

        case 11:
            io_num = GPIO_PIN_11;
            break;

        case 12:
            io_num = GPIO_PIN_12;
            break;

        case 13:
            io_num = GPIO_PIN_13;
            break;

        case 14:
            io_num = GPIO_PIN_14;
            break;

        case 15:
            io_num = GPIO_PIN_15;
            break;
        
        default:
            sh_printf("Param error\r\n");
            return;
        }
				
        stat = hi_gpio_read((PORT_T)io_group, io_num);

        sh_printf("%d\r\n",stat);
        sh_printf("OK\r\n");
}



static void set_clock_pwm(unsigned int pwm)
{
    fpga_set_clock_pwm(pwm);
    sh_printf("pwm=%d\r\n",fpga_reg_read(FPGA_REG_PWM));
}

static void show_clock_pwm(void)
{
    sh_printf("pwm=%d\r\n",fpga_get_clock_pwm());
}






/*
**********************************************************
*函数功能: shell 16进制字符串转10进制数字
*参数说明: pc piValue
*返 回 值: int32_t
**********************************************************
*/
static int32_t Shell_SHtoI(char *pc, int32_t *piValue) 
{
	int32_t index;
	int32_t ivalue = 0;
	uint8_t ch;
	int32_t ilen = strlen(pc);
	
	if ((pc == 0) || (piValue == 0))
	{
		return -1;
	}

	if ((pc[0] == 0x30) && ((pc[1] == 0x78) ||(pc[1] == 0x58)))/*0x*/
	{
		for (index = 2; pc[index] != 0 && index < ilen; index ++)
		{
			ch = pc[index];
			if (ch == 0x30) 		ivalue = ivalue * 16 + 0;/*0*/
			else if(ch == 0x31)	ivalue = ivalue * 16 + 1;/*1*/
			else if(ch == 0x32)	ivalue = ivalue * 16 + 2;/*2*/
			else if(ch == 0x33)	ivalue = ivalue * 16 + 3;/*3*/
			else if(ch == 0x34)	ivalue = ivalue * 16 + 4;/*4*/
			else if(ch == 0x35)	ivalue = ivalue * 16 + 5;/*5*/
			else if(ch == 0x36)	ivalue = ivalue * 16 + 6;/*6*/
			else if(ch == 0x37)	ivalue = ivalue * 16 + 7;/*7*/
			else if(ch == 0x38)	ivalue = ivalue * 16 + 8;/*8*/
			else if(ch == 0x39)	ivalue = ivalue * 16 + 9;/*9*/
			else if((ch == 0x61) || (ch == 0x41))	ivalue = ivalue * 16 + 10;/*a A*/
			else if((ch == 0x62) || (ch == 0x42))	ivalue = ivalue * 16 + 11;/*b B*/
			else if((ch == 0x63) || (ch == 0x43))	ivalue = ivalue * 16 + 12;/*c C*/
			else if((ch == 0x64) || (ch == 0x44))	ivalue = ivalue * 16 + 13;/*d D*/
			else if((ch == 0x65) || (ch == 0x45))	ivalue = ivalue * 16 + 14;/*e E*/
			else if((ch == 0x66) || (ch == 0x46))	ivalue = ivalue * 16 + 15;/*f F*/
			else return -1;
		}
	}
	else
	{
		return -1;
	}

	*piValue = ivalue;
	return 0;
}

/*
**********************************************************
*函数功能: shell 10进制字符串转10进制数字
*参数说明: pc piValue
*返 回 值: int32_t
**********************************************************
*/
static int32_t Shell_StoI(char *pc, int32_t *piValue) 
{
	int32_t index = 0;
	int32_t ivalue = 0;
	int32_t iflag = 0;
	uint8_t ch;
	int32_t ilen = strlen(pc);
	
	if ((pc == 0) || (piValue == 0))
	{
		return -1;
	}

	if ((pc[0] == 0x2B) || (pc[0] == 0x2D) || ((pc[0] >= 0x30) && (pc[0] <= 0x39)))/*= - 0 9*/
	{
		if (pc[0] == '+') {index = 1;}
		if (pc[0] == '-') {index = 1; iflag = 1;}
		
		for (; pc[index] != 0 && index < ilen; index ++)
		{
			ch = pc[index];
			if (ch == 0x30) 		ivalue = ivalue * 10 + 0;/*0*/
			else if(ch == 0x31)	ivalue = ivalue * 10 + 1;/*1*/
			else if(ch == 0x32)	ivalue = ivalue * 10 + 2;/*2*/
			else if(ch == 0x33)	ivalue = ivalue * 10 + 3;/*3*/
			else if(ch == 0x34)	ivalue = ivalue * 10 + 4;/*4*/
			else if(ch == 0x35)	ivalue = ivalue * 10 + 5;/*5*/
			else if(ch == 0x36)	ivalue = ivalue * 10 + 6;/*6*/
			else if(ch == 0x37)	ivalue = ivalue * 10 + 7;/*7*/
			else if(ch == 0x38)	ivalue = ivalue * 10 + 8;/*8*/
			else if(ch == 0x39)	ivalue = ivalue * 10 + 9;/*9*/
			else return -1;
		}
	}
	else
	{
		return -1;
	}

	if (iflag == 0)  	*piValue = ivalue;
	else if (iflag == 1)	*piValue = 0 - ivalue;
	else return -1;
	
	return 0;
}

/*
**********************************************************
*函数功能: shell 命令查找
*参数说明: pFunName
*返 回 值: SYM_TABLE
**********************************************************
*/
static SYM_TABLE* Shell_Find_SymFunc(char *pFunName)
{
	int32_t index;
	
	for (index=0; index<g_sys_table_len; index++)
	{
		if (0 == strcmp(g_sys_table[index].FuncName, pFunName))
		{
			return &g_sys_table[index];
		}
	}
	return NULL;
}

/*
**********************************************************
*函数功能: shell 命令注册
*参数说明: pcName  pFunc  pcPara  pcParaEx
*返 回 值: 0成功
**********************************************************
*/
#if 0
int32_t Shell_Cmd_Register(char *pcName, void *pFunc, char *pcPara, char *pcParaEx)
{
	int32_t index;
	int32_t ch;
	int32_t paraNum = strlen(pcPara);
	
	if ((pcName == NULL) || (pFunc == NULL) || (pcPara == NULL) || (paraNum > CMD_PARA_NUM))
	{
		return -1;
	}

	if (Shell_Find_SymFunc(pcName) != NULL)
	{
		return -1;
	}
	
	if (g_sys_table_len < SYS_TABLE_MAX_LEN)
	{
		g_sys_table[g_sys_table_len].FuncName = pcName;
		g_sys_table[g_sys_table_len].Func = (EXEC_FUNC)pFunc;
		g_sys_table[g_sys_table_len].pcParaEx = pcParaEx;
		for (index = 0; index < paraNum; index ++)
		{
			ch = pcPara[index];
			if ((ch == 0x75) || (ch == 0x55))/*u U*/
			{
				g_sys_table[g_sys_table_len].acPara[index] = 0x75;
			}
			else if ((ch == 0x63) ||  (ch == 0x43))/*c C*/
			{
				g_sys_table[g_sys_table_len].acPara[index] = 0x63;
			}
			else
			{
				return -1;
			}
		}

		for (; index < CMD_PARA_NUM; index ++)
		{
			g_sys_table[g_sys_table_len].acPara[index] = 0;
		}
		
		g_sys_table_len++;
	}
	
	return 0;
}
#endif
/*
**********************************************************
*函数功能: shell 命令执行
*参数说明: 无
*返 回 值: 索引
**********************************************************
*/
static int32_t g_iParaFlag[CMD_PARA_NUM];
static int32_t g_iParaLong[CMD_PARA_NUM];
static uint8_t g_acParaStr[CMD_PARA_NUM][INPUT_CMD_MAX_LEN];
static char g_acCmd[INPUT_CMD_MAX_LEN+1];
static char g_actemp[INPUT_CMD_MAX_LEN+1];
static SYM_TABLE *pSYM = NULL;
void Shell_Exec_SymFunc(char *ac_input_cmd)
{
	int32_t ilen;
	int32_t index, iloop, iParaLoop;
	//int32_t iRetValue;
	
	ilen = strlen(ac_input_cmd);
	sh_printf("\r\n");

	for (iParaLoop = 0; iParaLoop < CMD_PARA_NUM; iParaLoop++)
	{
		g_iParaFlag[iParaLoop] = 0;
		g_iParaLong[iParaLoop] = 0;
	}
		
	for (index = 0; index < ilen && ac_input_cmd[index] == 0x20; index ++){}/*去掉多余的空格*/
		
	iloop = 0;
	for (; index < ilen; index ++, iloop ++)
	{
		if ((ac_input_cmd[index] == 0x20) || (ac_input_cmd[index] == 0) || (ac_input_cmd[index] == 0x0A))
		{
			break;
		}
		g_acCmd[iloop] = ac_input_cmd[index];
	}
	g_acCmd[iloop] = 0;
	
	pSYM = Shell_Find_SymFunc(g_acCmd);
	if (pSYM == NULL)
	{
		sh_printf("    not find %s!", g_acCmd);
		sh_printf("\r\n%s", PROMPT);
		return;
	}

	for (; index < ilen && ac_input_cmd[index] == 0x20; index ++){}   
	if ((ac_input_cmd[index] == 0) || (ac_input_cmd[index] == 0x0A))    
	{
		iParaLoop = 0;
		if (pSYM->acPara[0] != 0)
		{
			sh_printf("    input parameters error!");
			sh_printf("\r\n%s", PROMPT);
			return;
		}
	}
	else
	{
		iParaLoop = 0;
		iloop = 0;
		for (; index <= ilen; index ++)
		{
			if ((ac_input_cmd[index] != 0x2C) && (ac_input_cmd[index] != 0))
			{
				if(ac_input_cmd[index] != 0x20)/*不等于空格*/
				{
					g_actemp[iloop] = ac_input_cmd[index];
					iloop ++;
				}
			}
			else if ((g_actemp[0] == 0x22) && (g_actemp[iloop-1] != 0x22) && (ac_input_cmd[index] != 0))
			{
				g_actemp[iloop] = ac_input_cmd[index];
				iloop ++;
			}
			else
			{
				g_actemp[iloop] = 0;
				/**/
				if ((g_actemp[0] == 0x30) && ((g_actemp[1] == 0x78) ||(g_actemp[1] == 0x58)))  /*0x*/
				{
					if ((pSYM->acPara[iParaLoop] != 0x75) ||  (Shell_SHtoI(g_actemp, &g_iParaLong[iParaLoop]) != 0))
					{
						sh_printf("    input parameters error[0]!");
						sh_printf("\r\n%s", PROMPT);
						return;
					}
				}
				else if ((g_actemp[0] == 0x2B) || (g_actemp[0] == 0x2D) || ((g_actemp[0] >=0x30) && (g_actemp[0] <= 0x39)))/*10*/
				{
					if ((pSYM->acPara[iParaLoop] != 0x75) ||  (Shell_StoI(g_actemp, &g_iParaLong[iParaLoop]) != 0))
					{
						sh_printf("    input parameters error[1]!");
						sh_printf("\r\n%s", PROMPT);
						return;
					}
				}
				else if((g_actemp[0] == 0x22)  && (g_actemp[iloop-1] == 0x22) && (iloop != 1))	/*string*/
				{
					if (pSYM->acPara[iParaLoop] != 0x63)
					{
						sh_printf("    input parameters error[2]!");
						sh_printf("\r\n%s", PROMPT);
						return;
					}

					memcpy(g_acParaStr[iParaLoop], (uint8_t *)&g_actemp[1], iloop-2);
					g_acParaStr[iParaLoop][iloop-2] = 0;
					g_iParaFlag[iParaLoop] = 1;
				}
				else
				{
					sh_printf("    input parameters error[3]!");
					sh_printf("\r\n%s", PROMPT);
					return;
				}

				iParaLoop ++;
				iloop = 0;
			}
		}
	}
	
	for (iloop = 0; iloop < CMD_PARA_NUM; iloop++)
	{
		if ((pSYM->acPara[iloop] != 0x63) && (pSYM->acPara[iloop] != 0x75))
		{
			break;
    }
	}
	
	if (iloop != iParaLoop)
	{
		sh_printf("    input parameters error!");
		sh_printf("\r\n%s", PROMPT);
		return;
	}
	
	pSYM->Func(((g_iParaFlag[0]== 0) ? g_iParaLong[0] : (int32_t)g_acParaStr[0]),
                            		    ((g_iParaFlag[1]== 0) ? g_iParaLong[1] : (int32_t)g_acParaStr[1]),
                             		    ((g_iParaFlag[2]== 0) ? g_iParaLong[2] : (int32_t)g_acParaStr[2]),
                             		    ((g_iParaFlag[3]== 0) ? g_iParaLong[3] : (int32_t)g_acParaStr[3]),
                            		    ((g_iParaFlag[4]== 0) ? g_iParaLong[4] : (int32_t)g_acParaStr[4]),
                             		    ((g_iParaFlag[5]== 0) ? g_iParaLong[5] : (int32_t)g_acParaStr[5]) 
                            		     );
	//sh_printf("return value = %d(0x%x)", iRetValue, iRetValue);
	sh_printf("\r\n%s", PROMPT);
}

/*
**********************************************************
*函数功能: shell 帮助打印
*参数说明: 无
*返 回 值: 无
**********************************************************
*/
static void Shell_Help(void)
{
	int index;
	for (index=0; index<g_sys_table_len; index++)
	{
		sh_printf("    %-25s:%s\r\n",g_sys_table[index].FuncName,g_sys_table[index].pcParaEx);
	}
}



/************************ (C)  *****END OF FILE****/
