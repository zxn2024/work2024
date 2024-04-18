/*************************************** Copyright ******************************
*
* (C) Copyright 2023, Shaanxi Tianji Communication
* All Rights Reserved
*
* FileName   : bsp_testmode_save.h
* Version    : none
* Author     :
* Date       : 2023-12-19
* Description: none
*******************************************************************************/
//
#ifndef __BSP_TESTMODE_SAVE_H_
#define __BSP_TESTMODE_SAVE_H_
#include <stdint.h>
#include "bsp_flash_struct.h"


extern tm_flash_factory * t_tm_factory;
extern tm_flash_rf   *   t_tm_rf;
extern tm_flash_8668  *  t_tm_8668;




/*****************************************************************************
* @Name	  : tm_erase_cali_factory_data
* @Brief  : 恢复校准数据公共参数出厂设置
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2024-01-18
* @Note   : 
*****************************************************************************/
void flash_restore_cali_factory(void);


/*****************************************************************************
* @Name	  : tm_read_factory_data
* @Brief  : 读取出厂的公共参数
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2023-12-19
* @Note   : 开机需要读取
*****************************************************************************/
void tm_read_cali_factory_data(void);


/*****************************************************************************
* @Name	  : tm_save_cali_factory_para
* @Brief  : 保存出厂的公共参数
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2023-12-19
* @Note   :
*****************************************************************************/
void tm_save_cali_factory_para(void);

/*****************************************************************************
* @Name	  : tm_get_rf_cali_sector_addr
* @Brief  : 获取射频校准参数页首址
* @Param  : band: [输入/出]
* @Retval :
* @Author :
* @Data   : 2023-12-21
* @Note   :
*****************************************************************************/
uint32_t tm_get_rf_cali_sector_addr(uint8_t band);


/*****************************************************************************
* @Name	  : tm_get_8668_cali_sector_addr
* @Brief  : 获取8668校准参数页首址
* @Param  : sys: [输入/出]
**			 band: [输入/出]
* @Retval :
* @Author :
* @Data   : 2023-12-21
* @Note   :
*****************************************************************************/
uint32_t tm_get_8668_cali_sector_addr(uint8_t band);


/*****************************************************************************
* @Name	  : tm_read_sector_cali_para
* @Brief  : 读取指定BAND所在扇区的参数
* @Param  : band: [输入/出]
* @Retval :
* @Author :
* @Data   : 2023-12-20
* @Note   :
*****************************************************************************/
void tm_read_cali_rf_para(uint8_t band);


/*****************************************************************************
* @Name	  : tm_read_factory_data
* @Brief  : 读取测试模式
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2023-12-19
* @Note   : 开机需要读取
*****************************************************************************/
void tm_read_cali_factory_data(void);


/*****************************************************************************
* @Name	  : tm_save_cali_factory_para
* @Brief  : 保存出厂的公共参数
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2023-12-19
* @Note   :
*****************************************************************************/
void tm_save_cali_factory_para( void );


/*****************************************************************************
* @Name	  : tm_save_cali_rf_para
* @Brief  : 保存校准的射频参数
* @Param  : addr: [输入/出]
* @Retval :
* @Author :
* @Data   : 2024-01-10
* @Note   :
*****************************************************************************/
void tm_save_cali_rf_para(uint8_t band);


#endif





