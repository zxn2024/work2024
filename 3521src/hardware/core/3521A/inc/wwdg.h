/**
 * **********************************************************************************
 * @copyright Copyright (c) 2023 Shaanxi Tianji Communication Technologies Co., Ltd.
 *                         
 * @file        wwdg.h   
 * @author      WangJun
 * @version     None
 * @date        2023-09-05     
 * @brief       None   
 * 
 * **********************************************************************************
 */

#ifndef __WWDG_H_
#define __WWDG_H_

#include "main.h"

#ifdef __cplusplus
 extern "C" {
#endif

void wwgd_init(void);
void wwdg_feed(void);
void wwdg_dbg_stop(void);

#ifdef __cplusplus
}
#endif

#endif
