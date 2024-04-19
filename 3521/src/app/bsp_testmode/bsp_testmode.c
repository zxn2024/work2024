/***************************** Copyright ****************************************
*
* (C) Copyright 2023, Shaanxi Tianji Communication
*  All Rights Reserved
*
* FileName   : bsp_testmode.c
* Version    : none
* Author     :
* Date       : 2023-12-11
* Description: none
*******************************************************************************/
#include "main.h"
#include "public_define.h"
#include "public_enum.h"
#include "public_struct.h"
#include "bsp_testmode.h"
#include "bsp_testmode_define.h"
#include "bsp_rf_global.h"
#include "system_config.h"
#include "rtt.h"


/*---------------------- 端口定义 --------------------------------------*/


/*---------------------- 事件组  ---------------------------------------*/

EventGroupHandle_t egp_tm_public  = NULL;
EventGroupHandle_t egp_tm_dev 	  = NULL; //蓝牙
EventGroupHandle_t egp_tm_fpga 	  = NULL; //FPGA
EventGroupHandle_t egp_tm_ecr8668 = NULL; //ECR8668
EventGroupHandle_t egp_tm_rf 	  = NULL; //射频
EventGroupHandle_t egp_tm_func 	  = NULL; //功能
EventGroupHandle_t egp_tm_ble 	  = NULL; //蓝牙



/*---------------------- 宏定义  ---------------------------------------*/


/*---------------------- 结构体 ----------------------------------------*/






/*---------------------- 常量 ------------------------------------------*/


/*---------------------- 全局变量 --------------------------------------*/


/*---------------------- 局部变量 --------------------------------------*/
SlaveDevNum_T  *  pt_tm_slave_num;	//从机数量参数
uint8_t 		 tm_msg_source;		//消息来源 PC / BLE
uint8_t          reload_866x_bin_in_tm;	//重新加载8668Bin文件


#define TM_PDU_LEN	( 1200 )
// #if(PRODUCT_MODEL == CMDT3521C)
// __align(32) uint8_t tm_pdu_buf[ TM_PDU_LEN ] __attribute__((section("CCMRAM")));
// #else
// uint8_t tm_pdu_buf[ TM_PDU_LEN ];
// #endif
uint8_t tm_pdu_buf[ TM_PDU_LEN ];

/*---------------------- 声明 ------------------------------------------*/



/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ APP ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void tm_egp_creat(void)
{
    egp_tm_public = xEventGroupCreate();

    if(egp_tm_public == NULL) /* 任务创建失败 */
    {
        rtt_printf(RTT_OS, RTT_TEXT_RED"[ERROR] epg_tm_public Create FAIL...\r\n");
        return;
    }

    xEventGroupClearBits(egp_tm_public, 0x00FFFFFF);
}


/*****************************************************************************
* @Name	  : tm_egp_set_public_bit
* @Brief  :
* @Param  : egp_bits: [输入/出]
* @Retval :
* @Author :
* @Data   : 2023-12-19
* @Note   :
*****************************************************************************/
void tm_egp_set_public_bit(EventBits_t egp_bits)
{
    xEventGroupSetBits(egp_tm_public, egp_bits);
}


/*****************************************************************************
* @Name	  : tm_egp_clr_public_bit
* @Brief  :
* @Param  : egp_bits: [输入/出]
* @Retval :
* @Author :
* @Data   : 2023-12-19
* @Note   :
*****************************************************************************/
void tm_egp_clr_public_bit(EventBits_t egp_bits)
{

}


/*****************************************************************************
* @Name	  : tm_egp_clr_all_bit
* @Brief  :
* @Param  : egp: [输入/出]
* @Retval :
* @Author :
* @Data   : 2023-12-19
* @Note   :
*****************************************************************************/
void tm_egp_clr_all_bit(EventGroupHandle_t egp)
{

}


/*****************************************************************************
* @Name	  : tm_para_init
* @Brief  :
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2023-12-11
* @Note   :
*****************************************************************************/
void tm_para_init(void)
{
    memset(tm_pdu_buf, 0x00, TM_PDU_LEN);
}



/*****************************************************************************
* @Name	  : tm_dev_init
* @Brief  : tm设备初始化
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2023-12-21
* @Note   :
*****************************************************************************/
void tm_dev_init(void)
{
    EventBits_t xEgpBit = xEventGroupGetBits(egp_tm_public);

    //加载BIN文件
    //指针重定向
    //打开电源
    //切换开关
    //
}

/*****************************************************************************
* @Name	  : tm_get_para
* @Brief  : 获取设备参数
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2023-12-18
* @Note   :
*****************************************************************************/
void tm_get_para(void)
{
    pt_tm_slave_num = &tDevPublicPara.tSlaveDevNum;
}

/*****************************************************************************
* @Name	  : tm_mode_set
* @Brief  : 设置工作模式
* @Param  : mode: [输入/出]
* @Retval :
* @Author :
* @Data   : 2023-12-11
* @Note   :
*****************************************************************************/
void tm_mode_set(uint8_t mode)
{

}


/*****************************************************************************
* @Name	  : tm_set_msg_source
* @Brief  : 设置测试模式消息来源
* @Param  : source: [输入/出]
* @Retval :
* @Author :
* @Data   : 2023-12-18
* @Note   : 该指示同时作为新消息提示使用
*****************************************************************************/
void tm_set_msg_source(uint8_t source)
{
    if(PDU_SRC_BLE == source || PDU_SRC_PC == source)
    {
        tm_msg_source = source;
    }
}

/*****************************************************************************
* @Name	  : tm_clr_msg_source
* @Brief  : 清除测试模式消息来源
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2023-12-18
* @Note   :
*****************************************************************************/
void tm_clr_msg_source(void)
{
    tm_msg_source = 0;
}



