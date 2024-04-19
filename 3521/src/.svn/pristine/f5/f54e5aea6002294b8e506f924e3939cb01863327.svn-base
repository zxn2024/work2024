/*************************************** Copyright ******************************
*
* (C) Copyright 2023, Shaanxi Tianji Communication
* All Rights Reserved
*
* FileName   : bsp_testmode_data_rw.h
* Version    : none
* Author     :
* Date       : 2023-12-13
* Description: none
*******************************************************************************/

#ifndef __BSP_TESTMODE_DATA_RW_H_
#define __BSP_TESTMODE_DATA_RW_H_
#include <stdint.h>

#include "bsp_testmode_define.h"

/*****************************************************************************
* @Name	  : tm_pdu_data_rw
* @Brief  : 测试模式下软件读写
* @Param  :  rws: 读写属性
**			 cmd: 读写选择
**			 pt_src: 源数据
**			 tag: 指向设备参数
**			 type: 数据类型
**			 len: 数据长度
**			 min: 最小值
**			 max: 最大值
**			 save: 是否保持
* @Retval : TM_CMD_ACT_OK( 正常执行 ) / TM_CMD_ACT_CONDITION ( 有条件执行 )
*****************************************************************************/
uint8_t tm_pdu_data_rw(uint8_t rws, uint8_t cmd, tm_oid_data_t * pt_src, void * tag, uint8_t type, uint16_t len, int32_t min, int32_t max, uint8_t * save);

#endif
