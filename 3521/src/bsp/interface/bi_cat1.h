/*************************************** Copyright ******************************
 *
 * (C) Copyright 2023, Shaanxi Tianji Communication
 * All Rights Reserved
 *
 * FileName   : bi_cat1.h
 * Version    : none
 * Author     :
 * Date       : 2023-09-21

 *******************************************************************************/
//

#ifndef _BI_CAT1_H_
#define _BI_CAT1_H_


/*---------------------- cat1驱动接口说明 ------------------------------------------*/

/*
-note
 -cat1驱动需包含初始化结构体,cat1_xxxx_register函数.
 -驱动接口根据具体型号和业务决定,如无必要请勿变更接口.

-设备初始化结构体
 包含MCU资源分配，比如GPIO分配，SPI端口，该设备的查找表等
typedef struct __CAT1_xxxx_HwConfig_t
{
    PORT_T cs_port;     // cs port
    GPIO_PIN_T cs_pin;  // cs pin
    ....
    //此处定义该设备所需的硬件接口（如GPIO、SPI等），资源（内存，缓冲区），查找表（功率表，衰减表，温度表）
} CAT1_xxxx_config_t;

-注册函数
 用于注册一个设备并分配资源
Status_t cat1_xxxx_register函数(CAT1_xxxx_t *handle_cat1);
*/

/*---------------------- 对外接口 ------------------------------------------*/

//TODO


#endif /* _BI_CAT1_H_ */
