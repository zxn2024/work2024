/**
 * **********************************************************************************
 * @copyright Copyright (c) 2023 Shaanxi Tianji Communication Technologies Co., Ltd.
 *                         
 * @file        os_semaphore.h   
 * @author      WangJun
 * @version     None
 * @date        2023-06-16     
 * @brief       None   
 * 
 * **********************************************************************************
 */

#ifndef __OS_SEMAPHORE_H_
#define __OS_SEMAPHORE_H_
#include "FreeRTOS.h"
#include "semphr.h"

void os_semaphore_create(void);

#endif

