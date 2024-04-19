/*************************************** Copyright ******************************
*
* (C) Copyright 2023, Shaanxi Tianji Communication
* All Rights Reserved
*                         
* FileName   : bsp_public_macro.h   
* Version    : none		
* Author     : ZhangWei			
* Date       : 2023-02-10         
* Description: none   
*******************************************************************************/
//
#ifndef __BSP_PUBLIC_MACRO_H_
#define __BSP_PUBLIC_MACRO_H_

#include <stdint.h>


//定时器默认的周期节拍数
#define TIMER_PERIOD_TICKS  (100)


//毫秒、秒、分、小时转换为定时节拍
#define MS_TO_TICKS(x)      ((uint32_t)((x) / TIMER_PERIOD_TICKS))
#define S_TO_TICKS(x)       ((uint32_t)(((x) * 1000) / TIMER_PERIOD_TICKS))
#define MIN_TO_TICKS(x)     ((uint32_t)(((x) * 60000) / TIMER_PERIOD_TICKS))
#define HOUR_TO_TICKS(x)    ((uint32_t)(((x) * 3600000) / TIMER_PERIOD_TICKS))



/****************************** 数据拆分和并 ******************************/
#define UINT16_L(X)     ((uint8_t)(X & 0x00FF))         // 获取uint16_t的低8位
#define UINT16_H(X)     ((uint8_t)((X >> 8) & 0x00FF))  // 获取uint16_t的高8位
#define H_AND_L(H, L)   ((uint16_t)((H << 8) + L))      // 将A和B合并到一起

#define STRUCT_LEN(x)   sizeof(x),((sizeof(x) % 4) ? (4-(sizeof(x) % 4)) : 0)     //判断结构体长度是否是4的整数倍
#define STRUCT_PACK4(x)  ((sizeof(x) % 4) ? (4-(sizeof(x) % 4)) : 0)
#define STRUCT_PACK(x)  ( STRUCT_LEN(x), STRUCT_PACK4(x) )

#define LIMIT_MIN( X, Y )	( X = ( X < Y ) ? Y : X )	// 数据最小值边界判断
#define LIMIT_MAX( X, Y )   ( X = ( X > Y ) ? Y : X )	// 数据最大值边界判断

#ifndef BOOL
#define BOOL uint8_t
#endif

#endif





