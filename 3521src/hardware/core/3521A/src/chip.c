/**
 * **********************************************************************************
 * @copyright Copyright (c) 2023 Shaanxi Tianji Communication Technologies Co., Ltd.
 *                         
 * @file        chip.c   
 * @author      WangJun
 * @version     None
 * @date        2023-09-07     
 * @brief       chip init   
 * 
 * **********************************************************************************
 */

#include "chip.h"
#include "system_config.h"

/*---------------------- 端口定义 ----------------------*/


/*---------------------- 宏 定 义 ----------------------*/


/*---------------------- 结 构 体 ----------------------*/


/*---------------------- 常    量 ----------------------*/


/*---------------------- 全局变量 ----------------------*/


/*---------------------- 局部变量 ----------------------*/


/*---------------------- 函    数 ----------------------*/



/*****************************************************************************
* @brief  : chip init
* @param  : None
* @retval : 
* @date   : 2023-09-07
* @note   : 
*****************************************************************************/
void chip_init(void)
{
    /* configure 4 bits pre-emption priority */
    nvic_priority_group_set(NVIC_PRIGROUP_PRE4_SUB0);

    if(1 == _USE_UPGRADE_)
    {
        //SCB->VTOR = FLASH_BASE | FLASH_ADDR_APP;  // 中断向量表偏移
        __enable_irq();
    }
    
    systick_config();
}
