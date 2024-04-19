/*************************************** Copyright ******************************
*
* (C) Copyright 2022, Shaanxi Tianji Communication
* All Rights Reserved
*                         
* FileName   : app_upgrade.h   
* Version    : none		
* Author     : none			
* Date       : 2024-02-26        
* Description: upgrade   
*******************************************************************************/
#ifndef __APP_UPGRADE_H_
#define __APP_UPGRADE_H_

void upgrade_run(void);

#if (LOCAL_MODULE_TYPE == _MU)

void Upgrade_RuStart(void);

void Upgrade_set_5g_modem(uint8_t isUpgrade, uint8_t isDelay);
	
void Upgrade_recv_trans_ctrl(uint8_t slave_index, uint8_t ctrl);
void Upgrade_recv_pack_num(uint8_t slave_index, uint32_t seq);
#endif

#endif

