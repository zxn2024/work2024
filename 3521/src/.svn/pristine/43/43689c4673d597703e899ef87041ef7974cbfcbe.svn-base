/*************************************** Copyright ******************************
*
* (C) Copyright 2023, Shaanxi Tianji Communication
* All Rights Reserved
*
* FileName   : bsp_flash_addr.h
* Version    : none
* Author     :
* Date       : 2023-11-25
* Description: none
*******************************************************************************/
//
#ifndef __BSP_FLASH_ADDR_H_
#define __BSP_FLASH_ADDR_H_
#include <stdint.h>
#include "system_config.h"

#define SAVE_ADDR_ERROR		    0xFFFFFFFF  //错误地址

#define CALI_DATA_PUBLIC_ADDR	0x00000000	//校准参数 - 公共参数首地址
#define CALI_DATA_RF_ADDR		0x00001000	//校准参数 - 射频通道参数首地址
#define CALI_DATA_RF_SIZE		0x00001000	//单一射频通道参数占用的大小, 4k

#define DEV_RUN_PARA_ADDR		0x0000D000	//设备运行参数首地址
#define DEV_RUN_PARA_SIZE		0x00001000	//最大占用空间地址

#define DEV_WARN_STATE_ADDR		0x0000F000	//设备告警状态首地址
#define DEV_WARN_STATE_SIZE		0x00001000	//最大占用空间地址


typedef enum
{
    CALI_TYPE_RF = 0,
    CALI_TYPE_8668,
    CALI_TYPE_END
} CALI_DATA_TYPE_E;

extern const uint16_t cali_type_addr_para[ BAND_END ][CALI_TYPE_END];

uint32_t get_cali_factory_data_save_addr( void );	//获取校准参数中厂商参数地址

/*****************************************************************************
* @Name	  : get_cali_data_save_addr
* @Brief  : 获取校准参数保存数据首地址
* @Param  : band: 频段号, 1
**			type: 校准数据类型, 0 ~ 1
* @Retval : 若参数均正确, 则返回相应的存储地址, 若参数错误, 则返回SAVE_ADDR_ERROR ( 0xFFFFFFFF )
* @Author :
* @Data   : 2023-11-25
* @Note   :
*****************************************************************************/
uint32_t get_cali_data_save_addr(uint8_t band, uint8_t type );


/*****************************************************************************
* @Name	  : get_cali_data_type_size
* @Brief  : 获取不同各类型校准数据在flash中的存储大小
* @Param  : type: [输入/出]
*****************************************************************************/
uint16_t get_cali_data_type_size(uint8_t type);

/*****************************************************************************
* @Name	  : get_dev_run_para_save_addr
* @Brief  : 获取设备运行参数存储首地址
* @Param  : None
* @Retval :
*****************************************************************************/
uint32_t get_dev_run_para_save_addr(void);
uint16_t get_dev_run_para_save_size(void);

//获取告警状态存储首地址
uint32_t get_dev_warn_state_save_addr(void);
uint16_t get_dev_warn_state_save_size(void);

#endif





