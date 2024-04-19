/***************************** Copyright ****************************************
*
* (C) Copyright 2023, Shaanxi Tianji Communication
*  All Rights Reserved
*
* FileName   : bsp_rf_switch.c
* Version    : none
* Author     :
* Date       : 2023-10-18
* Description: none
*******************************************************************************/
#include "main.h"
//#include "stdlib.h"
//#include "stdio.h"
//#include "string.h"
#include "system_config.h"
#include "bsp_public_drv.h"

#include "bsp_rf_global.h"
#include "bsp_dev_para.h"


/*---------------------- 端口定义 --------------------------------------*/


/*---------------------- 宏定义  ---------------------------------------*/


/*---------------------- 结构体 ----------------------------------------*/


/*---------------------- 常量 ------------------------------------------*/


/*---------------------- 全局变量 --------------------------------------*/


/*---------------------- 局部变量 --------------------------------------*/


/*---------------------- 声明 ------------------------------------------*/



/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ HAL ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/





/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DRV ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/





/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ APP ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
//射频通道开关设置

/*****************************************************************************
* @Name	  : rf_sys1_switch_set
* @Brief  : sys1 射频开关设置
* @Param  : state: RF_SWITCH_OPEN / RF_SWITCH_CLOSE / RF_SWITCH_UP / RF_SWITCH_DW
* @Retval :
* @Author :
* @Data   : 2023-10-18
* @Note   :
*          从机设单通, 需要给FPGA发指令
*          主机设单通, 向5G模组发送 modem_set_channel_outpu_modem()
*          8668配增益
*****************************************************************************/
static void rf_sys1_switch_set(uint8_t state)
{
    if(RF_SWITCH_OPEN == state)
    {

    }
    else if(RF_SWITCH_CLOSE == state)
    {

    }
    else if(RF_SWITCH_DW == state)
    {

    }
    else if(RF_SWITCH_UP == state)
    {

    }
    else
    {}
}


/*****************************************************************************
* @Name	  : rf_sys2_switch_set
* @Brief  : sys2 射频开关设置
* @Param  : state: RF_SWITCH_OPEN / RF_SWITCH_CLOSE / RF_SWITCH_UP / RF_SWITCH_DW
* @Retval :
* @Author :
* @Data   : 2023-10-18
* @Note   :
*****************************************************************************/
static void rf_sys2_switch_set(uint8_t state)
{
    if(RF_SWITCH_OPEN == state)
    {

    }
    else if(RF_SWITCH_CLOSE == state)
    {

    }
    else if(RF_SWITCH_DW == state)
    {

    }
    else if(RF_SWITCH_UP == state)
    {

    }
    else
    {}
}


/*****************************************************************************
* @Name	  : rf_sys3_switch_set
* @Brief  : sys3 射频开关设置
* @Param  : state: RF_SWITCH_OPEN / RF_SWITCH_CLOSE / RF_SWITCH_UP / RF_SWITCH_DW
* @Retval :
* @Author :
* @Data   : 2023-10-18
* @Note   :
*****************************************************************************/
static void rf_sys3_switch_set(uint8_t state)
{
    if(RF_SWITCH_OPEN == state)
    {

    }
    else if(RF_SWITCH_CLOSE == state)
    {

    }
    else if(RF_SWITCH_DW == state)
    {

    }
    else if(RF_SWITCH_UP == state)
    {

    }
    else
    {}
}


/*****************************************************************************
* @Name	  : rf_ctr_switch_set
* @Brief  : 设置射频开关状态
* @Param  : sys: [输入/出]
**			state: [输入/出]
* @Retval :
* @Author :
* @Data   : 2023-10-18
* @Note   :
*****************************************************************************/
void rf_ctr_switch_set(uint8_t sys, uint8_t state)
{
    if(sys >= RF_SYS_NUM)
        return;

    switch(sys)
    {
        case RF_SYS_1:
            rf_sys1_switch_set(state);
            break;

        case RF_SYS_2:
            rf_sys2_switch_set(state);
            break;

        case RF_SYS_3:
            rf_sys3_switch_set(state);
            break;

        default:
            break;
    }
}


