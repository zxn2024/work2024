/*************************************** Copyright ******************************
*
* (C) Copyright 2023, Shaanxi Tianji Communication
* All Rights Reserved
*
* FileName   : bsp_dev_flash.h
* Version    : none
* Author     :
* Date       : 2023-11-27
* Description: none
*******************************************************************************/
//
#ifndef __BSP_DEV_FLASH_H_
#define __BSP_DEV_FLASH_H_
#include <stdint.h>

void flash_dev_para_rw( uint8_t rw );
void flash_restore_dev_factory( void );	//设备参数恢复出厂设置
void flash_restore_warn_para(void);

void flash_rw_test( void );//读写测试

#endif





