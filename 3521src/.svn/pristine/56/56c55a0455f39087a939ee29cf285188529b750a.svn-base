/***************************** Copyright ****************************************
*
* (C) Copyright 2023, Shaanxi Tianji Communication
*  All Rights Reserved
*
* FileName   : bsp_flash_addr.c
* Version    : none
* Author     :
* Date       : 2023-11-25
* Description: none
*******************************************************************************/
#include "main.h"
//#include "stdlib.h"
//#include "stdio.h"
//#include "string.h"
#include "system_config.h"
#include "bsp_flash_addr.h"


/*---------------------- 端口定义 --------------------------------------*/


/*---------------------- 宏定义  ---------------------------------------*/


/*---------------------- 结构体 ----------------------------------------*/


/*---------------------- 常量 ------------------------------------------*/


/*---------------------- 全局变量 --------------------------------------*/


/*---------------------- 局部变量 --------------------------------------*/


/*---------------------- 声明 ------------------------------------------*/


const uint16_t cali_type_addr_para[ BAND_END ][CALI_TYPE_END] = { { 0x1000, 0x7000 },
																  { 0x2000, 0x8000 },
																  { 0x3000, 0x9000 },
																  { 0x4000, 0xA000 },
																  { 0x5000, 0xB000 },
																  { 0x6000, 0xC000 },
};


/*****************************************************************************
* @Name	  : get_cali_factory_data_save_addr
* @Brief  : 获取校准参数中设备参数地址
* @Param  : None
* @Retval : 
* @Author : 
* @Data   : 2024-01-10
* @Note   :
*****************************************************************************/
uint32_t get_cali_factory_data_save_addr( void )
{
	return CALI_DATA_PUBLIC_ADDR;
}


/*****************************************************************************
* @Name	  : get_cali_data_save_addr
* @Brief  : 获取校准参数保存数据首地址
* @Param  : band: 频段号, 1
**			type: 校准数据类型, 0 ~5
**			group: 组别, 仅功率表 / 8668校准表使用, 功率表有效值1~4, 8668校准表有效值1~5
**			ch:   上下行选择, 0 ~ 1, 8668不使用, 用0填充
**			num:  同一通道内的校准数据分组编号( 按带宽或其他 ), 有效值 0 ~ 1, 8668不使用, 用0填充
* @Retval : 若参数均正确, 则返回相应的存储地址, 若参数错误, 则返回SAVE_ADDR_ERROR ( 0xFFFFFFFF )
* @Author :
* @Data   : 2023-11-25
* @Note   :
*      1. 查询SYS1 的B3频段的第一组的上行输出功率表1首地址
*           uint32_addr = get_cali_data_save_addr( BAND_3, CALI_TYPE_POW_LIST,  1, RF_CH_UP, 1 );
*	   2. 查询5G 8668的第二组校准表首地址
*			uint32_addr = get_cali_data_save_addr( BAND_41, CALI_TYPE_8668_CALI,  1, 0, 0 );
*****************************************************************************/
uint32_t get_cali_data_save_addr(uint8_t band, uint8_t type )
{
    uint32_t res_error = 0;

    if(band >= BAND_END || type >= CALI_TYPE_END)
        return res_error;

    return (uint32_t)cali_type_addr_para[band][type];	//获取同类数据首地址
}



/*****************************************************************************
* @Name	  : get_dev_run_para_save_addr
* @Brief  : 获取设备运行参数存储首地址
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2023-11-27
* @Note   :
*****************************************************************************/
uint32_t get_dev_run_para_save_addr(void)
{
    return DEV_RUN_PARA_ADDR;
}

/*****************************************************************************
* @Name	  : get_dev_run_para_save_addr
* @Brief  : 获取设备运行参数存储空间
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2023-11-27
* @Note   :
*****************************************************************************/
uint16_t get_dev_run_para_save_size(void)
{
    return (uint16_t)DEV_RUN_PARA_SIZE;
}

/*****************************************************************************
* @Name	  : get_dev_warn_state_save_addr
* @Brief  : 获取告警状态存储首地址
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2023-11-27
* @Note   :
*****************************************************************************/
uint32_t get_dev_warn_state_save_addr(void)
{
    return DEV_WARN_STATE_ADDR;
}

/*****************************************************************************
* @Name	  : get_dev_warn_state_save_size
* @Brief  : 获取设备告警状态存储空间
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2023-11-27
* @Note   :
*****************************************************************************/
uint16_t get_dev_warn_state_save_size(void)
{
    return (uint16_t)DEV_WARN_STATE_SIZE;
}
