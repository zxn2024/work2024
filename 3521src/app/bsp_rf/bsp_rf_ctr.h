/*************************************** Copyright ******************************
*
* (C) Copyright 2022, Shaanxi Tianji Communication
* All Rights Reserved
*                         
* FileName   : bsp_rf_ctr.h   
* Version    : none		
* Author     : ZhangWei			
* Date       : 2022-08-29         
* Description: none   
*******************************************************************************/
#ifndef __BSP_RF_CTR_H_
#define __BSP_RF_CTR_H_

#include "stdint.h"
#include "system_config.h"

void rf_set_define_band_in_product_mode( uint8_t band_4g );
void rf_run_start_init(void);	//按照频段和带宽加载参数

void rf_ctr_in_task(void);
void jump_in_test_mode( void );
void rf_set_mode_switch_en( BOOL_E en );

//typedef struct
//{
//	RfSysPublic_T 		 t_public;
//	RfChPara_T  t_ch[RF_CH_NUM];
//}RfSystem_T;

//extern DevPublicPara_T tDevPublicPara;   //与硬件相关的公共参数
//extern RfSystem_T      t_rf_sys[ RF_SYS_NUM ];

#endif
