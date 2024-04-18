/***************************** Copyright ****************************************
*
* (C) Copyright 2024, Shaanxi Tianji Communication
*  All Rights Reserved
*
* FileName   : bsp_rf_866x.c
* Version    : none
* Author     :
* Date       : 2024-01-01
* Description: none
*******************************************************************************/
#include "main.h"
//#include "stdlib.h"
//#include "stdio.h"
//#include "string.h"
//#include "MCU_config.h"
#include "bsp_config.h"
#include "public_define.h"
#include "public_enum.h"
#include "public_struct.h"
#include "bsp_rf_866x.h"
#include "ecr8668.h"
#include "ecr8668_driver.h"


/*---------------------- 端口定义 --------------------------------------*/


/*---------------------- 宏定义  ---------------------------------------*/
typedef enum
{
	REG_8668_OFF_PAGE = 0,
	REG_8668_ON_PAGE  = 1
}REG_8668_ON_OFF_PAGE;


/*---------------------- 结构体 ----------------------------------------*/
typedef struct
{
    uint8_t page_type;	//0:页内 1:页外
    uint8_t addr_flag;	//B_FALSE:无效  B_TRUE:有效
    uint8_t dat_flag;	//B_FALSE:无效  1B_TRUE:有效
    uint8_t sys; 		//
    uint8_t cmd;		//0:READ	 1:WRITE
    uint32_t addr;		//地址
    uint32_t dat;		//数据
} ecr866x_reg_rw_t;



/*---------------------- 常量 ------------------------------------------*/


/*---------------------- 全局变量 --------------------------------------*/
ecr866x_reg_rw_t t_866x_reg_rw;		//单个寄存器读写

/*---------------------- 局部变量 --------------------------------------*/
//#if(PRODUCT_MODEL == CMDT3521C)
//__align(32)  TRX8668_FPGA_t * pt_866x_sys[RF_SYS_NUM] __attribute__((section("CCMRAM"))) =
//{
//    &h8668_LTE,
//    &h8668_NR,
//    &h8668_NR_MIMO
//};
//#else
//#endif
TRX8668_FPGA_t * pt_866x_sys[RF_SYS_NUM] =
{
    &h8668_LTE,
    &h8668_NR,
    &h8668_NR_MIMO
};

#if LOCAL_MODULE_TYPE == _MU
//B3:  1817.5,  25M
//B8:  941.5,   15M
//B39: 1900,    20M
//B40: 2345,    20M
//B41: 2565,    100M
const uint32_t rf_866x_default_freq_point[ BAND_END ] = { 18175, 9415, 19000, 23450, 25650, 25650};	//10倍关系
const uint32_t rf_866x_default_band_width[ BAND_END ] = { 20, 15, 20, 20, 100, 100};
#else
//B3:  1722.5,  25M
//B8:  896.5,   15M
//B39: 1900,    20M
//B40: 2345,    20M
//B41: 2565,    100M
const uint32_t rf_866x_default_freq_point[ BAND_END ] = { 17225, 8965, 19000, 23450, 25650, 25650};	//10倍关系
const uint32_t rf_866x_default_band_width[ BAND_END ] = { 25, 15, 20, 20, 100, 100};
#endif

rf_866x_para_t t_rf_866x_para[RF_SYS_NUM];

/*---------------------- 声明 ------------------------------------------*/


/*****************************************************************************
* @Name	  : rf_866x_read_ch_mode_state
* @Brief  : 读取通道模式
* @Param  : tpt: [输入/出]
* @Retval :
* @Author :
* @Data   : 2024-01-01
* @Note   :
*****************************************************************************/
uint32_t rf_866x_read_ch_mode_state(TRX8668_FPGA_t * tpt)
{
    return tpt->ch_status.ch_mode;
}

/*****************************************************************************
* @Name	  : rf_866x_read_bw
* @Brief  : 读取带宽参数
* @Param  : tpt: [输入/出]
* @Retval :
* @Author :
* @Data   : 2024-01-01
* @Note   :
*****************************************************************************/
uint32_t rf_866x_read_bw(TRX8668_FPGA_t * tpt)
{
    return tpt->ch_status.bw_choose;
}

/*****************************************************************************
* @Name	  : rf_866x_read_loop_state
* @Brief  : 读取回环状态
* @Param  : tpt: [输入/出]
* @Retval :
* @Author :
* @Data   : 2024-01-01
* @Note   :
*****************************************************************************/
uint32_t rf_866x_read_loop_state(TRX8668_FPGA_t * tpt)
{
    return tpt->ch_status.loop;
}


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ APP ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*****************************************************************************
* @Name	  : rf_866x_get_default_freq_point
* @Brief  : 获取默认的中心频点
* @Param  : band: [输入/出]
* @Retval :
* @Author :
* @Data   : 2024-01-01
* @Note   :
*****************************************************************************/
uint32_t rf_866x_get_default_freq_point(uint8_t band)
{
    if(band >= BAND_END)
        return 0;

    return rf_866x_default_freq_point[ band ];
}

/*****************************************************************************
* @Name	  : rf_866x_get_default_band_width
* @Brief  : 获取默认的带宽
* @Param  : band: [输入/出]
* @Retval :
* @Author :
* @Data   : 2024-01-05
* @Note   :
*****************************************************************************/
uint32_t rf_866x_get_default_band_width(uint8_t band)
{
    if(band >= BAND_END)
        return 0;

    return rf_866x_default_band_width[ band ];
}

//Status_t BSP_866x_set_band(TRX8668_FPGA_t *h8668, RF_BAND_E band, uint32_t band_width, uint32_t freq);

/*****************************************************************************
* @Name	  : rf_866x_reload_bin
* @Brief  : 过程中重新加载8668 bin文件
* @Param  : sys: [输入/出]
**			 band: [输入/出]
**			 band_width: [输入/出]
* @Retval :
* @Author :
* @Data   : 2024-01-01
* @Note   :
*****************************************************************************/
Status_t rf_866x_reload_bin(uint8_t sys, RF_BAND_E band, uint32_t band_width, uint32_t freq)
{
    TRX8668_FPGA_t * pt_866x = pt_866x_sys[ sys ];
    
    Status_t res = STAT_OK;
    uint8_t  retry = 0;

    while(retry <= 3)
    {
        retry++;
        res = BSP_866x_set_band(pt_866x, band, band_width, freq);

        if(STAT_OK == res)
            break;
    }

    return res;
}


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 866x reg read and write ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*****************************************************************************
* @Name	  : reg_866x_rw_struct_init
* @Brief  :
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2024-01-02
* @Note   :
*****************************************************************************/
void reg_866x_rw_struct_init(void)
{
    t_866x_reg_rw.page_type = 0;		//0:页内 1:页外
    t_866x_reg_rw.addr_flag = B_FALSE;	//B_FALSE:无效  B_TRUE:有效
    t_866x_reg_rw.dat_flag = B_FALSE;	//B_FALSE:无效  1B_TRUE:有效
    t_866x_reg_rw.sys = 0; 				//BAND_3 ~ BAND_41M
    t_866x_reg_rw.cmd = READ;			//0:READ	 1:WRITE
    t_866x_reg_rw.addr = 0;				//地址
    t_866x_reg_rw.dat = 0;				//数据
}

/*****************************************************************************
* @Name	  : reg_866x_rw
* @Brief  : 866x指定寄存器读写
* @Param  : tag: 读取数据的返回值
* @Retval :
* @Author :
* @Data   : 2024-01-02
* @Note   :
*****************************************************************************/
Status_t reg_866x_rw(uint32_t * tag)
{
    TRX8668_FPGA_t * h8668;
    ecr866x_reg_rw_t * pt = &t_866x_reg_rw;

    if((B_TRUE == pt->addr_flag) && (B_TRUE == pt->dat_flag))
    {
        if(pt->sys >= RF_SYS_NUM)
            return STAT_ERROR;

        h8668 = pt_866x_sys[ pt->sys ];

        if(WRITE == pt->cmd)
        {
            if(REG_8668_ON_PAGE == pt->page_type)	//0: off_page 1: on_page
                BSP_866x_In_Write(h8668, pt->addr, pt->dat);
            else
                BSP_866x_Ext_Write(h8668, pt->addr, pt->dat);
        }
        else
        {
            if(REG_8668_ON_PAGE == pt->page_type)	//0: off_page 1: on_page
                *tag = BSP_866x_In_Read(h8668, pt->addr);
            else
                *tag = BSP_866x_Ex_Read(h8668, pt->addr);
        }
    }

    reg_866x_rw_struct_init();

    return STAT_OK;
}


/*****************************************************************************
* @Name	  : reg_866x_set_addr
* @Brief  : 设置地址标志
* @Param  : addr: [输入/出]
* @Retval :
* @Author :
* @Data   : 2024-01-02
* @Note   :
*****************************************************************************/
void reg_866x_set_addr(uint32_t addr)
{
    t_866x_reg_rw.addr = addr;
    t_866x_reg_rw.addr_flag = B_TRUE;
    rtt_printf(RTT_8668, "[info]set 8668 addr 0x%08x\r\n", addr);
}

/*****************************************************************************
* @Name	  : reg_866x_set_on_page_dat
* @Brief  : 设置片内寄存器数据
* @Param  : sys: [输入/出]
**			 cmd: [输入/出]
* @Retval :
* @Author :
* @Data   : 2024-01-02
* @Note   :
*****************************************************************************/
void reg_866x_set_on_page_dat(uint8_t sys, uint8_t cmd, uint32_t dat)
{
    t_866x_reg_rw.page_type = REG_8668_ON_PAGE; //页内
    t_866x_reg_rw.sys = sys;
    t_866x_reg_rw.cmd = cmd;
    t_866x_reg_rw.dat = dat;
    t_866x_reg_rw.dat_flag = B_TRUE;
    rtt_printf(RTT_8668, "[info]866x_set_on_page_dat: sys = %d, cmd = %d, dat = 08x\r\n", sys, cmd, dat);
}


/*****************************************************************************
* @Name	  : reg_866x_set_off_page_dat_flag
* @Brief  : 设置片外寄存器数据
* @Param  : sys: [输入/出]
**			 cmd: [输入/出]
**			 dat: [输入/出]
* @Retval :
* @Author :
* @Data   : 2024-01-02
* @Note   :
*****************************************************************************/
void reg_866x_set_off_page_dat(uint8_t sys, uint8_t cmd, uint32_t dat)
{
    t_866x_reg_rw.page_type = REG_8668_OFF_PAGE;	//页外
    t_866x_reg_rw.sys = sys;
    t_866x_reg_rw.cmd = cmd;
    t_866x_reg_rw.dat = dat;
    t_866x_reg_rw.dat_flag = B_TRUE;
    rtt_printf(RTT_8668, "[info]866x_set_off_page_dat: sys = %d, cmd = %d, dat = 08x\r\n", sys, cmd, dat);
}



/*****************************************************************************
* @Name	  : reg_8668_get_addr_dat_in_tm
* @Brief  : 
* @Param  : pt: [输入/出] 
**			 t_tag: [输入/出] 
* @Retval : 
* @Author : 
* @Data   : 2024-01-07
* @Note   :
*****************************************************************************/
Status_t reg_8668_get_addr_dat_in_tm( uint8_t cmd, uint8_t sys, uint8_t *pt, uint32_t *temp_dat )
{
	ecr866x_reg_rw_t *t_tag = &t_866x_reg_rw;
	
	t_tag->sys = sys;
	t_tag->cmd = cmd;
	t_tag->page_type = *pt;
	t_tag->addr = *(uint32_t *)(pt+1);
	t_tag->dat  = *(uint32_t *)(pt+5);
	
	if( (t_tag->addr != 0) && (t_tag->page_type <= 1) )
	{
		t_tag->addr_flag = B_TRUE;
		t_tag->dat_flag  = B_TRUE;
		
		reg_866x_rw( temp_dat );
		
		return STAT_OK;
	}
	else
	{
		t_tag->addr_flag = B_FALSE;
		t_tag->dat_flag  = B_FALSE;
		return STAT_ERROR;
	}
}
