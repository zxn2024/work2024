/*************************************** Copyright ******************************
*
* (C) Copyright 2023, Shaanxi Tianji Communication
* All Rights Reserved
*
* FileName   : bsp_testmode_analysis.h
* Version    : none
* Author     :
* Date       : 2023-12-14
* Description: none
*******************************************************************************/
//
#ifndef __BSP_TESTMODE_ANALYSIS_H_
#define __BSP_TESTMODE_ANALYSIS_H_
#include <stdint.h>

void tm_get_para(void);
uint16_t tm_msg_parse_in_task(uint8_t * src, uint8_t * tag);

#endif





