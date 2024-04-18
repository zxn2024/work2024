/***************************** Copyright ****************************************
*
* (C) Copyright 2023, Shaanxi Tianji Communication
*  All Rights Reserved
*
* FileName   : bsp_testmode_analog.c
* Version    : none
* Author     :
* Date       : 2023-12-11
* Description: none
*******************************************************************************/
#include "main.h"

#include "public_define.h"
#include "public_enum.h"
#include "public_struct.h"
#include "bsp_testmode_analysis.h"
#include "bsp_testmode_ergodic.h"
#include "bsp_testmode_para_list.h"
#include "crc.h"
#include "bsp_testmode.h"
//#include "bsp_testmode_oid.h"
//#include "bsp_dev_para.h"
//#include "bsp_testmode_data_rw.h"
#include "bsp_rf_global.h"
#include "system_config.h"

#include "bsp_rf_dc_switch.h"
#include "bsp_rf_866x.h"

#include "bsp_dev_flash.h"
#include "bsp_testmode_save.h"
/*---------------------- 端口定义 --------------------------------------*/



/*---------------------- 宏定义  ---------------------------------------*/




/*
 * @brief 保存标记位定义
 */
#define PDU_MACH        	(0)     // 设备信息保存位定义标识
#define PDU_FACTORY     	(1)     // 设备参数保存位定义标识
#define PDU_WARN        	(2)     // 告警信息保存位定义标识
#define PDU_SN_TABLE    	(3)     // SN列表信息保存位定义标识

#define CONST_PDU_TXFS      (2)    //1:短信  2: PS域  3:CS域
#define CONST_PDU_TXMK      (1)    //远程数据通信模块
#define CONST_PDU_APCZDCD   (512)  //AP:C协议最大长度
#define CONST_PDU_MCPBJHJZ  (0)    //MCP:B交互机制 0:每包确认
#define CONST_PDU_LFXS      (1)    //连发系数, 不支持连发
#define CONST_PDU_SBXYCS    (3)    //设备响应超时, 单位S
#define CONST_PDU_FSJGSJ    (50)   //发送间隔时间 单位ms, uint2型
#define CONST_PDU_ZTDDSJ    (3)    //暂停传输等待时间, 单位S
#define CONST_PDU_JKMSZHSJ  (3)    //监控模式/升级模式转换时间, 单位S, uint2
#define CONST_PDU_YCSJFS    (1)    //远程升级方式: 0:无法升级, 1:MCP:B 2:FTP
#define CONST_PDU_PSYXY     (2)    //PS域传输协议: 1:UDP, 2:TCP


/*---------------------- 结构体 ----------------------------------------*/

//static WarnEnGate_T tPduWarn[WARN_TYPE_POW_END];   // 告警使能和门限缓存
/*---------------------- 常量 ------------------------------------------*/


/*---------------------- 全局变量 --------------------------------------*/


/*---------------------- 局部变量 --------------------------------------*/


/*---------------------- 声明 ------------------------------------------*/


/*****************************************************************************
* @Name	  : tm_pud_data_sys_and_model_check
* @Brief  : 数据系统号和模块号校验
* @Param  : pt_src: [输入/出]
* @Retval : B_TRUE / B_FALSE;
* @Author :
* @Data   : 2023-12-18
* @Note   :
*****************************************************************************/
uint8_t tm_pud_data_sys_and_model_check(tm_oid_data_t * pt_src)
{
    uint8_t res = B_TRUE;
	uint8_t sys_use = pt_src->sys;
	if( sys_use )
		sys_use -= 1;
    if( sys_use >= RF_SYS_NUM )
    {
        pt_src->oid |= TM_ERR_IDERR;
        res = B_FALSE;
    }

    return res;
}


/*****************************************************************************
* @Name	  : tm_pdu_analysis
* @Brief  : 测试模式下报文解析
* @Param  : src: 原始数据
* @Retval :
* @Author :
* @Data   : 2023-12-13
* @Note   :
*****************************************************************************/
uint8_t tm_pdu_analysis(uint8_t * src)
{
    uint8_t pkg_err_flag 	  = 0;  		// 错误标志
    uint8_t pdu_id_err 		  = 0;    		// 命令ID中执行错误码
    tm_save_flag_t t_save_flag;	            //设备参数保持标志

    tm_msg_t   *   pt_msg  = (tm_msg_t *)src;
    
    uint8_t    *   pt_byte = (uint8_t *)&pt_msg->tpt;
    tm_oid_data_t * pt_data = &pt_msg->tpt;		//第一个数据首地址
    
    uint16_t       len      = 0;
    const uint16_t max_len = pt_msg->len - 14;

    uint8_t dev_id = pt_msg->dev_id;
    uint8_t cmd    = pt_msg->cmd;

    t_save_flag.dev       = B_FALSE;
    t_save_flag.factory   = B_FALSE;
    t_save_flag.cali_rf   = B_FALSE;
    t_save_flag.cali_8668 = B_FALSE;

    uint8_t res = B_TRUE;
	
    if((pt_msg->dev_id == 0xFF) || (pt_msg->dev_id == 0x00))    //单机设备
        dev_id = 0;
    else if((dev_id <= pt_tm_slave_num->ucTheoNum) || (dev_id >= 200))
    {}
    else
    {
        pt_msg->direction = 0x00;
        pt_msg->ack       = TM_CMD_ACT_DEV_NON_EXIST;	//返回设备不存在
        return B_TRUE;
    }
	
	reg_866x_rw_struct_init();	
	
    while(len < max_len)
    {
        res = tm_pud_data_sys_and_model_check(pt_data);	//校验通道号 和 模块号是否正确
        if(B_TRUE == res)
        {
            switch(pt_data->model)
            {
                case TM_MODEL_PUBLIC:	//0x00, 公共参数
                    pdu_id_err = tm_pdu_public_data_ergodic(dev_id, cmd, pt_msg, pt_data, &t_save_flag);
                    break;

                case TM_MODEL_MONITOR:	//0x01, 网络与监控参数
                    pdu_id_err = tm_pdu_monitor_data_ergodic(dev_id, cmd, pt_data, &t_save_flag);
                    break;

                case TM_MODEL_4G_MODEM:	//0x10, 4G模组
					pdu_id_err = tm_pdu_cat1_data_ergodic(dev_id, cmd, pt_data, &t_save_flag);
					break;
				
                case TM_MODEL_5G_MODEM:	//0x11, 5G模组
                    pdu_id_err = tm_pdu_modem_data_ergodic(dev_id, cmd, pt_data, &t_save_flag);
                    break;

                case TM_MODEL_BLE:		//0x12, BLE模组
                    pdu_id_err = tm_pdu_ble_data_ergodic(dev_id, cmd, pt_data, &t_save_flag);
                    break;

                case TM_MODEL_FPGA:		//0x13, FPGA
                    pdu_id_err = tm_pdu_fpga_data_ergodic(dev_id, cmd, pt_data, &t_save_flag);
                    break;

                case TM_MODEL_HUB:		//0x14, HUB
                    pdu_id_err = tm_pdu_hub_data_ergodic(dev_id, cmd, pt_data, &t_save_flag);
                    break;

                case TM_MODEL_BAND3:	//0x20, BAND_3
                case TM_MODEL_BAND8:	//0x21, BAND_8
                case TM_MODEL_BAND39:	//0x22,	BAND_39
                case TM_MODEL_BAND40:	//0x23, BAND_40
                case TM_MODEL_BAND41:	//0x24, BAND_41
                    pdu_id_err = tm_pdu_rf_data_ergodic(dev_id, cmd, pt_data, &t_save_flag);
                    break;

                default:
                    break;
            }

            if(pdu_id_err)
            {
                pkg_err_flag = pdu_id_err;
            }
        }
        else
        {
            pkg_err_flag = TM_CMD_ACT_CONDITION;	//命令被有条件执行
        }

        len      += pt_data->len;
        pt_byte  += pt_data->len;
        pt_data   = (tm_oid_data_t *)pt_byte;
		
		if( pt_data->len == 0x00 )
			break;
    }

    //此处需添加保存数据函数
    if(t_save_flag.dev)     //设备公共参数
    {
        flash_dev_para_rw(WRITE);   //出厂参数
    }
    
    if(t_save_flag.factory) //校准公共参数
    {
        tm_save_cali_factory_para();
        tm_read_cali_factory_data();
    } 
    
    pt_msg->direction = 0x00;
    pt_msg->ack       = pkg_err_flag;  //设置应答标志

//    clr_pdu_msg_source();

    return B_TRUE;
}


/*****************************************************************************
* @Name	  : tm_msg_parse_in_task
* @Brief  : 测试整包数据解析
* @Param  : src: 原始数据指针
**			tag: 目标返回数据指针
* @Retval : 若返回数据长度为0, 则不需要发送, 否则从USB发送数据
* @Author :
* @Data   : 2023-12-13
* @Note   :
*****************************************************************************/
uint16_t tm_msg_parse_in_task(uint8_t * src, uint8_t * tag)
{
    uint16_t res_len = 0;
	
    tm_msg_t * pt_msg = (tm_msg_t *)src;
    
    if((pt_msg->sop == TMDP_SEOP_BYTE) && (*(src + pt_msg->len - 1) == TMDP_SEOP_BYTE))       //开始/结束字符正确
    {
        uint16_t crc_msg = (*(src + pt_msg->len - 2) << 8) | (*(src + pt_msg->len - 3));
        uint16_t crc_cal = crc16((src + 1), pt_msg->len - 4);

        if((crc_msg != crc_cal)  //CRC校验错误
            || (pt_msg->direction != TMDP_DIRECTION_TX)
            || (pt_msg->ack != TM_ACK_TX))
        {
            res_len = 0;
        }
        else
        {
            tm_pdu_analysis(src);
            
			crc_cal = crc16((src + 1), pt_msg->len - 4 );
			*(src + pt_msg->len - 3) = (uint8_t)crc_cal;
			*(src + pt_msg->len - 2) = (uint8_t)(crc_cal >> 8);
			
			memcpy(tag, src, pt_msg->len);
			res_len =  pt_msg->len;
        }
    }
    return res_len;
}


