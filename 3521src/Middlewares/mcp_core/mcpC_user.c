/***************************** Copyright ****************************************
 *
 * (C) Copyright 2023, Shaanxi Tianji Communication
 *  All Rights Reserved
 *
 * FileName   : mcpA_user.c
 * Version    : none
 * Author     : none
 * Date       : 2024-2-20
 * Description: MCP:A协议解析及业务实现
 *******************************************************************************/
#include "bsp_config.h"
#include "mcp_core.h"
#include "ascii_hex.h"
#include "bsp_reboot.h"
/*---------------------- 宏定义  ---------------------------------------*/
/*---------------------- 结构体 ----------------------------------------*/

/*---------------------- 常量 ------------------------------------------*/

/*---------------------- 全局变量 --------------------------------------*/

/*---------------------- 声明 ------------------------------------------*/

uint16_t mcpC_msg_handler(uint8_t * rx_data, uint16_t len, uint8_t * reply_data, uint8_t msg_src)
{
    return 0;
}
