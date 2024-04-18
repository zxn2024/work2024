/***************************** Copyright ****************************************
*
* (C) Copyright 2023, Shaanxi Tianji Communication
*  All Rights Reserved
*
* FileName   : bsp_rf_workmode.c
* Version    : none
* Author     :
* Date       : 2023-05-12
* Description: none
*******************************************************************************/
#include "main.h"
//#include "stdlib.h"
//#include "stdio.h"
//#include "string.h"
#include "system_config.h"
#include "bsp_public_drv.h"
#include "bsp_rf_workmode.h"
#include "bsp_dev_para.h"
#include "bsp_rf_ctr.h"
#include "bsp_rf_global.h"


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

void rf_in_task(void)
{
    if(WORK_CALIB == tDevPublicPara.ucWorkMode) 	//校准模式
    {

    }
    else
    {

    }

#if 0
    uint8_t dev_type = LOCAL_DEVICE_TYPE;		//设备类型
    uint8_t module_type = LOCAL_MODULE_TYPE;	//模块类型

    uint8_t workmode = tDevPublicPara.ucDevMode;

    RfChStaticPara_T * ch_pt = NULL;

    if((module_type == MODULE_TYPE_MU) && (dev_type == DEVICE_TYPE_DT))	//电梯主机设备
    {
        if(workmode == WORK_MODE_STANDLONE)	//单机模式
        {
            for(RF_SYS_E sys = RF_SYS_1; sys < RF_SYS_NUM; sys++)
            {
                ch_pt = &tRfChStaticPara[sys][RF_CH_UP];
                ch_pt->theoryGain  = 65;
                ch_pt->attOtherPre = 5;
                ch_pt->attPreSum = ch_pt->attCaliPre + ch_pt->attTempPre + ch_pt->attOtherPre;  //预置衰减器总和
            }
        }
        else	//正常模式
        {
            for(RF_SYS_E sys = RF_SYS_1; sys < RF_SYS_NUM; sys++)
            {
                ch_pt = &tRfChStaticPara[sys][RF_CH_UP];
                ch_pt->theoryGain  = 70;
                ch_pt->attOtherPre = 0;
                ch_pt->attPreSum = ch_pt->attCaliPre + ch_pt->attTempPre + ch_pt->attOtherPre;  //预置衰减器总和
            }
        }
    }
#endif
}

