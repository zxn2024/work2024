/**
 * **********************************************************************************
 * @copyright Copyright (c) 2023 Shaanxi Tianji Communication Technologies Co., Ltd.
 *                         
 * @file        hi_usb.h   
 * @author      WangJun
 * @version     None
 * @date        2023-09-11    
 * @brief       None   
 * 
 * **********************************************************************************
 */

#ifndef __HI_USB_H_
#define __HI_USB_H_

#include "MCU_config.h"
#if(STM32 == MCU_MANUFACTURER)
#include "usb_device.h"
#include "usbd_cdc_if.h"

extern uint8_t g_usb_new_msg;

#elif(GD32 == MCU_MANUFACTURER)
#include "drv_usb_hw.h"
#include "cdc_acm_core.h"

extern usb_core_driver cdc_acm;

#endif


#ifdef __cplusplus
 extern "C" {
#endif

#define PC_MSG_LEN  1500    // 接收缓存区大小

extern uint8_t pc_rx_array[PC_MSG_LEN];
extern uint16_t g_usb_rx_len;

void hi_usb_init(void);
Status_t hi_usb_send(uint8_t *pbuf, uint16_t len);
Status_t hi_usb_receive(uint8_t *pbuf, uint16_t * len);
void hi_usb_clear_fifo(void); // 在一个周期小于50ms的任务里调用,定期清fifo

void hi_usb_pkt_cplt_chk(void); // usb收包完成判断

USB_CONNECT_STATE_E hi_usb_get_connect_state( void );	//获取USB连接状态

#ifdef __cplusplus
}
#endif

#endif
