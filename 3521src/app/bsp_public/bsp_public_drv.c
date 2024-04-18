/***************************** Copyright ****************************************
*
* (C) Copyright 2022, Shaanxi Tianji Comunication
* All Rights Reserved
*
* FileName   : bsp_basic.c
* Version    : none
* Author     : none
* Date       : 2022-08-03
* Description: none
*******************************************************************************/
//#include "bsp_mcu_config.h"

#include "system_config.h"
#include "bsp_public_drv.h"
#include "system_config.h"

#if _USE_RTOS_
    #include "freertos.h"
    #include "task.h"
#endif

#include "rtt.h"

/*---------------------- 端口定义 --------------------------------------*/


/*---------------------- 宏定义 ---------------------------------------*/


/*---------------------- 结构体 ----------------------------------------*/


/*---------------------- 常量 ------------------------------------------*/


/*---------------------- 局部变量 --------------------------------------*/


/*---------------------- 全局变量 --------------------------------------*/
static uint32_t run_time = 0;

const uint8_t rf_sys_name[RF_SYS_NUM][10] =
{
    "4G",
    "NR",
    "MIMO"
};

const uint8_t rf_band_name[BAND_END][10] = {
	"B3",
	"B8",
	"B39",
	"B40",
	"B41",
	"B41M"
};

const uint8_t rf_ch_name[2][4] =
{
    { "UP" },
    { "DW" }
};

/*---------------------- 函数声明 --------------------------------------*/



/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


/*****************************************************************************
* @Name	  : run_time_add
* @Brief  : 设备上电运行时间统计, 在1ms时基定时器中调用
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2022-10-10
* @Note   :
*****************************************************************************/
void run_time_add(void)
{
    run_time++;
}

/*****************************************************************************
* @Name	  : get_run_time
* @Brief  : 获取设备运行时间, 单位ms
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2022-10-10
* @Note   :
*****************************************************************************/
uint32_t get_run_time(void)
{
    return run_time;
}

/*****************************************************************************
* @Name	  : printf_run_time
* @Brief  : 打印系统运行时间
* @Param  : windown_id: RTT窗口编号
* @Retval :
* @Author :
* @Data   : 2023-03-23
* @Note   :
*****************************************************************************/
void printf_run_time(uint8_t windown_id)
{
    uint16_t hour;
    uint8_t  min;
    uint8_t  sec;
    uint16_t  mSec;
    uint32_t time_cnt = run_time;

    if(windown_id >= 16)
        return;

    mSec = time_cnt % 1000;
    sec  = (time_cnt / 1000) % 60;
    min  = ((time_cnt / 1000) / 60) % 60;
    hour = ((time_cnt / 1000) / 60) / 60;

    rtt_printf(windown_id, RTT_TEXT_CYAN"\n>>>>> os run_time: %04d:%02d:%02d:%03d\n", hour, min, sec, mSec);
}

/*****************************************************************************
* @Name	  : name_4g_replace
* @Brief  : 根据有效频段更换RTT打印时4G显示的有效频段名称
* @Param  : band: [输入/出]
* @Retval :
* @Author :
* @Data   : 2023-12-01
* @Note   :
*****************************************************************************/
void name_4g_replace(uint8_t band)
{
    if(band >= 5)
        band = 0;

    uint8_t name[5][10] = { {"4G"}, {"B3"}, {"B8"}, {"B39"}, {"B40"} };

    memset(&rf_sys_name[RF_SYS_1], 0, sizeof(rf_sys_name[RF_SYS_1]));
    memcpy(&rf_sys_name[RF_SYS_1], &name[band], sizeof("B3"));
}
