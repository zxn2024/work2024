/***************************** Copyright ****************************************
*
* (C) Copyright 2023, Shaanxi Tianji Communication
*  All Rights Reserved
*
* FileName   : bsp_testmode_ergodic.c
* Version    : none
* Author     :
* Date       : 2023-12-11
* Description: 解析各个参数
*******************************************************************************/
#include "main.h"

#include "system_config.h"
#include "bsp_testmode_ergodic.h"
#include "bsp_testmode.h"
#include "bsp_testmode_oid.h"
#include "bsp_dev_para.h"
#include "bsp_testmode_data_rw.h"
#include "bsp_rf_global.h"
#include "bsp_testmode_para_list.h"
#include "bsp_rf_switch.h"
#include "bsp_rf_dc_switch.h"
#include "bsp_up_silence.h"
#include "bsp_rf_oplc.h"
#include "bsp_rf_pow.h"
#include "bsp_rf_lna.h"
#include "bsp_rf_att.h"
#include "bsp_rf_att_set.h"
#include "bsp_flash_addr.h"
#include "bsp_testmode_save.h"
#include "bsp_testmode_hal_set.h"
#include "bsp_testmode_para_list.h"

#include "bsp_rf_dc_switch.h"
#include "bsp_rf_4g_band_sel.h"
#include "fpga.h"
#include "ecr8668_driver.h"
#include "ecr8668.h"
#include "bsp_rf_866x.h"
#include "bsp_public_drv.h"
#include "bsp_rf_866x_default_set.h"

#include "ble.h"
#include "bsp_dev_flash.h"
#include "bsp_reboot.h"
#include "bsp_rf_ctr.h"
#include "bsp_rf_scan_net.h"
#include "bsp_rf_5g_modem.h"
#include "eswin_sync_modem.h"
#include "public_enum.h"
#include "bsp_rf_fpga.h"
#include "bsp_rf_scan_net.h"
#include "mu_ru_communicate.h"

#if(PRODUCT_MODEL == CMDT3521B || PRODUCT_MODEL == CMDT3521C)
	#include "app_monitor.h"
#endif
/*---------------------- 端口定义 --------------------------------------*/


/*---------------------- 宏定义  ---------------------------------------*/
#define DEV_M_ID	0x00

/*---------------------- 结构体 ----------------------------------------*/


/*---------------------- 常量 ------------------------------------------*/


/*---------------------- 全局变量 --------------------------------------*/

union union_type
{
    uint8_t * pt_uc;
    int8_t * pt_sc;
};
/*---------------------- 局部变量 --------------------------------------*/
static uint8_t bw_use = 0;	//数据分组, 仅用于测试模式保存数据指定位置使用

#if(PRODUCT_MODEL == CMDT3521C)
    __align(32) static uint8_t tx_temp_buf[1024] __attribute__((section("CCMRAM")));//用来缓存读取校准表数据
#else
    static uint8_t tx_temp_buf[1024];//用来缓存读取校准表数据
#endif
/*---------------------- 声明 ------------------------------------------*/
extern void att_set_force_fresh( uint8_t sys );



/*****************************************************************************
* @Name	  : tm_copy_cali_rf_para
* @Brief  :
* @Param  :  cmd: TM_CMD_SET / TM_CMD_SET
**			 sys: RF_SYS_1 / RF_SYS_2 / RF_SYS_3
**			 band: BAND_3 / BAND_8 / BAND_39 / BAND_40 / BAND_41 / BAND_41M
**           bw: 带宽, 10M / 15M /25M /30M /.....
**			 pt_src_data: 报文数据发送的首地址
**			 tag: 目标存贮地址
**			 len: [输入/出]
* @Retval :
* @Author :
* @Data   : 2024-01-04
* @Note   :
*****************************************************************************/
void tm_copy_cali_rf_para(uint8_t cmd, uint8_t band, tm_oid_data_t * pt_src_data, void * tag, uint16_t len)
{
    uint32_t addr = tm_get_rf_cali_sector_addr(band);

    uint8_t * pst_src = &pt_src_data->dat;	//指向源数据

    rtt_printf(RTT_RF, "in testmodm read secotr data, addr = 0x%08x\n", addr);

    tm_read_cali_rf_para(band);	//读取4K扇区数据

    uint8_t * tag1 = (uint8_t *)tag;

    if(TM_CMD_SET == cmd)
    {
        memcpy(tag1, pst_src, len);
        tm_save_cali_rf_para(band);
    }
    else
    {
        memcpy(pst_src, tag1, len);
    }
}


/*****************************************************************************
* @Name	  : tm_power_on_load_bin
* @Brief  : 电源开关上电加载bin文件
* @Param  : sys: [输入/出]
**			band: [输入/出]
* @Retval :
* @Author :
* @Data   : 2024-01-12
* @Note   :
*****************************************************************************/
uint8_t tm_power_on_load_bin(uint8_t sys, uint8_t band)
{
    uint8_t res = TM_ERR_OK;
	rf_5g_modem_switch( (RF_BAND_E)band,    RF_SWITCH_CLOSE );//关闭输出

    rtt_printf(RTT_RF, RTT_TEXT_YELLOW"\r\n************************************************************\n");
    rtt_printf(RTT_RF, RTT_TEXT_YELLOW"In Tm, %s reload para....\n", rf_band_name[band]);
    tm_sys_open(band);		      //通道上电, 4g 需要重定向指针
    uint32_t freq_8668 = rf_866x_get_default_freq_point(band);
    uint32_t bw_8668 = rf_866x_get_default_band_width(band);
    rf_5g_modem_switch( (RF_BAND_E)band, RF_SWITCH_CLOSE );
    Status_t load_res = rf_866x_reload_bin(sys, (RF_BAND_E)band, bw_8668, freq_8668);	//8668BIN 文件加载

    if(STAT_OK != load_res)
    {
        rtt_printf(RTT_RF, RTT_TEXT_RED"In Tm, %s 866x Bin load fault!\n", rf_band_name[band]);
        res = TM_ERR_LOAD_FAULT;	//加载失败
    }
    else
    {
        rtt_printf(RTT_RF, RTT_TEXT_CYAN"In Tm, %s 866x Bin load OK, start config default para..\r\n", rf_band_name[band]);
        rf_866x_default_set(band);

        rtt_printf(RTT_RF, RTT_TEXT_GREEN"In Tm, %s 866x config OK\r\n", rf_band_name[band]);
    }
	
	if( WORK_CALIB == tDevPublicPara.ucWorkMode )
		
		rf_ch_lna_att_default_set(band, RF_CH_DW);	//默认加载下行数据
	else 
		rf_run_start_init();

    return res;
}

/*****************************************************************************
* @Name	  : tm_model_to_band
* @Brief  : 测试模块号转换为实际的band
* @Param  : sys: [输入/出]
**			 model: [输入/出]
* @Retval :
* @Author :
* @Data   : 2024-01-04
* @Note   :
*****************************************************************************/
static RF_BAND_E tm_model_to_band(uint8_t sys, uint8_t model)
{
    RF_BAND_E res = BAND_3;

    if(sys >= RF_SYS_NUM)
        return BAND_3;

    switch(model)
    {
        case TM_MODEL_BAND3:
            res = BAND_3;
            break;

        case TM_MODEL_BAND8:
            res = BAND_8;
            break;

        case TM_MODEL_BAND39:
            res = BAND_39;
            break;

        case TM_MODEL_BAND40:
            res = BAND_40;
            break;

        case TM_MODEL_BAND41:
            res = (RF_SYS_2 == sys) ?  BAND_41 : BAND_41M;
            break;

        default:
            res = BAND_3;
    }

    return res;
}
/*****************************************************************************
* @Name	  : tm_pdu_public_data_ergodic
* @Brief  : 公共参数解析
* @Param  :  dev_id: 设备编号
**			 cmd: 查询 / 设置
**			 pt_msg: 整包报文首地址
**			 pt: 数据存储结构体
**			 t_save: 保存标识
* @Retval :
* @Author :
* @Data   : 2023-12-18
* @Note   :
*****************************************************************************/
uint8_t tm_pdu_public_data_ergodic(uint8_t dev_id, uint8_t cmd, tm_msg_t * pt_msg, tm_oid_data_t * pt, tm_save_flag_t * t_save)
{
    uint8_t res = TM_ERR_OK;
    uint8_t temp_uint8 = 0;
    RF_SYS_E sys = RF_SYS_1;

    switch(pt->oid)
    {
        case TM_ID_PUB_SN:	 					// 0x0001	SN
            res = tm_pdu_data_rw(TM_RW, cmd, pt, &tDevFactoryPara.sn, TM_STR, 32, 0, 0, &t_save->factory);
            break;

        case TM_ID_PUB_DEV_MODEL:				// 0x0002	设备型号
            res = tm_pdu_data_rw(TM_RW, cmd, pt, &tDevFactoryPara.devMode, TM_STR, 32, 0, 0, &t_save->dev);
            break;

        case TM_ID_PUB_HW_VER:					// 0x0003	硬件版本
            res = tm_pdu_data_rw(TM_RW, cmd, pt, &tDevFactoryPara.hwVer, TM_STR, 32, 0, 0, &t_save->factory);
            break;

        case TM_ID_PUB_SW_VER:					// 0x0004	软件版本
            res = tm_pdu_data_rw(TM_RW, cmd, pt, &tDevFactoryPara.swVer, TM_STR, 32, 0, 0, &t_save->factory);
            break;

        case TM_ID_PUB_MCU_TYPE:  				// 0x0005	MCU型号
            res = tm_pdu_data_rw(TM_R, cmd, pt, &tDevFactoryPara.mcuType, TM_STR, 32, 0, 0, &t_save->unsave);
            break;

        case TM_ID_PUB_FLASH:	    			// 0x0006	FLASH型号
            res = tm_pdu_data_rw(TM_R, cmd, pt, &tDevFactoryPara.flashType, TM_STR, 32, 0, 0, &t_save->unsave);
            break;

        case TM_ID_PUB_LICENSE:  				// 0x0007	license
            res = tm_pdu_data_rw(TM_RW, cmd, pt, &tDevFactoryPara.license, TM_STR, 64, 0, 0, &t_save->factory);
            break;

        case TM_ID_PUB_SYS_NUM:					// 0x0008	查询通道数量
            temp_uint8 = RF_SYS_NUM;
            res = tm_pdu_data_rw(TM_R, cmd, pt, &temp_uint8, TM_UINT1, 1, 1, 16, &t_save->unsave);
            break;

        case TM_ID_PUB_PARA_LIST: 				// 0x0009	更新参数列表
            res = tm_para_list_check(dev_id, pt);
            if(TM_CMD_ACT_OK == res)
                pt_msg->len = tm_rpt_para_list(dev_id, pt);
            break;

        case TM_ID_PUB_SYS_NAME:  				// 0x000A	查询通道名称, 暂时不用
            break;

        case TM_ID_PUB_FLASH_INIT: 				// 0x000B	FLASH初始化
            res = tm_pdu_data_rw(TM_W, cmd, pt, &temp_uint8, TM_UINT1, 1, 1, 2, &t_save->unsave);

            if(FLASH_RESTORY_FAC == temp_uint8)   // 恢复出厂设置
            {
                flash_restore_dev_factory();
                flash_dev_para_rw(READ);
            }
            else if(FLASH_ERASE_ALL == temp_uint8)// 格式化
            {
                flash_restore_dev_factory();
                flash_dev_para_rw(READ);
                flash_restore_cali_factory();
                tm_read_cali_factory_data();
            }
            break;

        case TM_ID_PUB_DEV_MODE:				// 0x000C	设备模式, 单机 / 主从 / 单主机
            res = tm_pdu_data_rw(TM_RW, cmd, pt, &tDevPublicPara.ucDevMode, TM_UINT1, 1, 0, 1, &t_save->dev);
            break;

        case TM_ID_PUB_RUN_MODE:  				// 0x000D	运行模式
            res = tm_pdu_data_rw(TM_RW, cmd, pt, &tDevPublicPara.ucWorkMode, TM_UINT1, 1, 0, 2, &t_save->unsave);
			if( WORK_PL == tDevPublicPara.ucWorkMode )
				rtt_printf( RTT_OS, RTT_TEXT_YELLOW" ***** dev work in TEST mode *****\n" );
			else if( WORK_CALIB == tDevPublicPara.ucWorkMode )
				rtt_printf( RTT_OS, RTT_TEXT_YELLOW" ***** dev work in CALIB mode *****\n" );
			else
			{}
            break;

        case TM_ID_PUB_SLAVE_DEV_NUM:	 		// 0x000E	从机数量
            res = tm_pdu_data_rw(TM_RW, cmd, pt, &tDevPublicPara.tSlaveDevNum.ucTheoNum, TM_UINT1, 1, 0, 1, &t_save->dev);
            break;

        case TM_ID_PUB_SLAVE_DEV_ONLIN_NUM: 	// 0x000F	在线从机数量
            res = tm_pdu_data_rw(TM_R, cmd, pt, &tDevPublicPara.tSlaveDevNum.ucOnlineNum, TM_UINT1, 1, 0, 1, &t_save->unsave);
            break;

        case TM_ID_PUB_RST_BY_NET: 				// 0x0010	网络定时重启时间
            res = tm_pdu_data_rw(TM_RW, cmd, pt, &tDevPublicPara.ucRstByNetTime, TM_UINT1, 1, 0, 23, &t_save->dev);
            break;

        case TM_ID_PUB_RST_BY_TIME: 			// 0x0011	定时重启时间间隔
            res = tm_pdu_data_rw(TM_RW, cmd, pt, &tDevPublicPara.ucRstByNetTime, TM_UINT1, 1, 25, 72, &t_save->dev);
            break;

        case TM_ID_PUB_LED_PLAN: 				// 0x0012	指示灯方案
            res = tm_pdu_data_rw(TM_RW, cmd, pt, &tDevPublicPara.ucLedPlan, TM_UINT1, 1, 0, 1, &t_save->dev);
            break;

        case TM_ID_PUB_LED_SWITCH:				// 0x0013	指示灯开关
            res = tm_pdu_data_rw(TM_RW, cmd, pt, &tDevPublicPara.ucLedSwitch, TM_UINT1, 1, 0, 1, &t_save->dev);
            break;

        case TM_ID_PUB_SYS_RST:	 				// 0x0014	系统重启
            res = tm_pdu_data_rw(TM_W, cmd, pt, &temp_uint8, TM_UINT1, 1, 0, 1, &t_save->unsave);
            if(1 == temp_uint8)
                reboot_set_egp_bit(RESET_SYSTEM);
            break;

        case TM_ID_PUB_ROUTING_ADDR:	 		// 0x0015	设备路由登记地址
            res = tm_pdu_data_rw(TM_R, cmd, pt, &tDevPublicPara.devRoutingAddr, TM_UINT4, 1, 0, 0, &t_save->unsave);
            break;

		case TM_ID_PUB_MODE_SWITCH_EN:			// 0x0016	//模式切换使能
			res = tm_pdu_data_rw(TM_R, cmd, pt, &tDevPublicPara.ucModeSwitchEn, TM_UINT1, 1, 0, 1, &t_save->unsave);
			break;
		
        case TM_ID_PUB_4G_VALID_BAND:			// 0x00C8	4G有效band
            res = tm_pdu_data_rw(TM_R, cmd, pt, &t_rf_sys[ RF_SYS_1 ].t_public.valid_band, TM_UINT1, 1, 0, 3, &t_save->unsave);
            break;
		
		case TM_ID_PUB_DEV_TYPE:				// 0x00C9   设备类型
			temp_uint8 = LOCAL_MODULE_TYPE;
			res = tm_pdu_data_rw(TM_R, cmd, pt, &temp_uint8, TM_UINT1, 1, 1, 4, &t_save->unsave);
			break;

        case TM_ID_PUB_BLOCK_SWITCH:
            if(TM_CMD_SET == cmd)
            {
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint8, TM_UINT1, 1, 0, 1, &t_save->unsave);

                if(CLOSE == temp_uint8)
                {
                    h8668_LTE.anti_block_switch = CLOSE;
                    h8668_NR.anti_block_switch = CLOSE;
                    h8668_NR_MIMO.anti_block_switch = CLOSE;
                    for(sys = RF_SYS_1; sys < RF_SYS_NUM; sys++)
                        BSP_866x_tia_adc_switch(sys, 0);
                }
                else if(OPEN == temp_uint8)
                {
                    h8668_LTE.anti_block_switch = OPEN;
                    h8668_NR.anti_block_switch = OPEN;
                    h8668_NR_MIMO.anti_block_switch = OPEN;
                    for(sys = RF_SYS_1; sys < RF_SYS_NUM; sys++)
                        BSP_866x_tia_adc_switch(sys, 1);
                }
            }
            else if(TM_CMD_QUERY == cmd)
            {
                temp_uint8 = h8668_LTE.anti_block_switch;
                
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint8, TM_UINT1, 1, 0, 1, &t_save->unsave);
            }

            break;

        default:
            res = TM_ERR_IDERR;
            break;
    }

    return res;
}


/*****************************************************************************
* @Name	  : tm_pdu_monitor_data_ergodic
* @Brief  :
* @Param  : dev_id: [输入/出]
**			 cmd: [输入/出]
**			 pt: [输入/出]
**			 t_save:
* @Retval :
* @Author :
* @Data   : 2023-12-18
* @Note   :
*****************************************************************************/
uint8_t tm_pdu_monitor_data_ergodic(uint8_t dev_id, uint8_t cmd, tm_oid_data_t * pt, tm_save_flag_t * t_save)
{
    uint8_t res = TM_ERR_OK;

    switch(pt->oid)
    {
        case TM_ID_MONITOR_FACT_ID:	 	 		// 0x0001	厂商代码
            res = tm_pdu_data_rw(TM_RW, cmd, pt, &tDevFactoryPara.factoryId, TM_UINT1, 1, 0, 255, &t_save->dev);
            break;

        case TM_ID_MONITOR_DEV_TYPE:	 	 	// 0x0002	设备类别
            res = tm_pdu_data_rw(TM_RW, cmd, pt, &tDevFactoryPara.devType, TM_UINT1, 1, 1, 3, &t_save->dev);
            break;

        case TM_ID_MONITOR_STATION_ID: 	 		// 0x0003	站点编号
            res = tm_pdu_data_rw(TM_RW, cmd, pt, &tOmcPara.bs_id, TM_UINT4, 4, 0, 0, &t_save->dev);
            break;

        case TM_ID_MONITOR_DEVICE_ID:  	 		// 0x0004	设备编号
            res = tm_pdu_data_rw(TM_RW, cmd, pt, &tOmcPara.dev_id, TM_UINT1, 1, 0, 255, &t_save->dev);
            break;

        case TM_ID_MONITOR_IPV4_IPV6_1:	 		// 0x0005	IPV4/IPV6选择1
            res = tm_pdu_data_rw(TM_RW, cmd, pt, &tOmcPara.ip[0].ip_type, TM_UINT1, 1, 1, 2, &t_save->dev);
            break;

        case TM_ID_MONITOR_TCP_UDP_1: 	 		// 0x0006	TCP/UPD选择1
            res = tm_pdu_data_rw(TM_RW, cmd, pt, &tOmcPara.ip[0].tcp_or_udp, TM_UINT1, 1, 1, 2, &t_save->dev);
            break;

        case TM_ID_MONITOR_USE_PASSWORD_1: 		// 0x0007	登录权限使能1
            res = tm_pdu_data_rw(TM_RW, cmd, pt, &tOmcPara.ip[0].use_password_en, TM_UINT1, 1, 0, 1, &t_save->dev);
            break;

        case TM_ID_MONITOR_IPV4_ADDR_1:	 		// 0x0008	IPV4地址1
            res = tm_pdu_data_rw(TM_RW, cmd, pt, &tOmcPara.ip[0].ipv4_addr, TM_UINT4, 4, 0, 0, &t_save->dev);
            break;

        case TM_ID_MONITOR_IPV6_ADDR_1:  	 	// 0x0009	IPV6地址1
            res = tm_pdu_data_rw(TM_RW, cmd, pt, &tOmcPara.ip[0].ipv6_addr, TM_STR, 48, 0, 0, &t_save->dev);
            break;

        case TM_ID_MONITOR_IP_PORT_1:	 	 	// 0x000A	IP端口号1
            res = tm_pdu_data_rw(TM_RW, cmd, pt, &tOmcPara.ip[0].port, TM_UINT2, 2, 0, 65535, &t_save->dev);
            break;

        case TM_ID_MONITOR_APN_1: 		 		// 0x000B	APN1
            res = tm_pdu_data_rw(TM_RW, cmd, pt, &tOmcPara.apn, TM_STR, 20, 0, 0, &t_save->dev);
            break;

        case TM_ID_MONITOR_USERNAME_1: 	 		// 0x000C	用户名1
            res = tm_pdu_data_rw(TM_RW, cmd, pt, &tOmcPara.ip[0].username, TM_STR, 20, 0, 0, &t_save->dev);
            break;

        case TM_ID_MONITOR_PASSWORD_1: 	 		// 0x000D	密码1
            res = tm_pdu_data_rw(TM_RW, cmd, pt, &tOmcPara.ip[0].password, TM_STR, 20, 0, 0, &t_save->dev);
            break;

        case TM_ID_MONITOR_IPV4_IPV6_2:	 		// 0x000E	IPV4/IPV6选择2
            res = tm_pdu_data_rw(TM_RW, cmd, pt, &tOmcPara.ip[1].ip_type, TM_UINT1, 1, 1, 2, &t_save->dev);
            break;

        case TM_ID_MONITOR_TCP_UDP_2: 	 		// 0x000F	TCP/UPD选择2
            res = tm_pdu_data_rw(TM_RW, cmd, pt, &tOmcPara.ip[1].tcp_or_udp, TM_UINT1, 1, 1, 2, &t_save->dev);
            break;

        case TM_ID_MONITOR_USE_PASSWORD_2: 		// 0x0010	登录权限使能2
            res = tm_pdu_data_rw(TM_RW, cmd, pt, &tOmcPara.ip[1].use_password_en, TM_UINT1, 1, 0, 1, &t_save->dev);
            break;

        case TM_ID_MONITOR_IPV4_ADDR_2:	 		// 0x0011	IPV4地址2
            res = tm_pdu_data_rw(TM_RW, cmd, pt, &tOmcPara.ip[1].ipv4_addr, TM_UINT4, 4, 0, 0, &t_save->dev);
            break;

        case TM_ID_MONITOR_IPV6_ADDR_2:  	 	// 0x0012	IPV6地址2
            res = tm_pdu_data_rw(TM_RW, cmd, pt, &tOmcPara.ip[1].ipv6_addr, TM_STR, 48, 0, 0, &t_save->dev);
            break;

        case TM_ID_MONITOR_IP_PORT_2:	 	 	// 0x0013	IP端口号2
            res = tm_pdu_data_rw(TM_RW, cmd, pt, &tOmcPara.ip[1].port, TM_UINT2, 2, 0, 65535, &t_save->dev);
            break;

        case TM_ID_MONITOR_APN_2: 		 		// 0x0014	APN2
            res = tm_pdu_data_rw(TM_RW, cmd, pt, &tOmcPara.apn, TM_STR, 20, 0, 0, &t_save->dev);
            break;

        case TM_ID_MONITOR_USERNAME_2: 	 		// 0x0015	用户名2
            res = tm_pdu_data_rw(TM_RW, cmd, pt, &tOmcPara.ip[1].username, TM_STR, 20, 0, 0, &t_save->dev);
            break;

        case TM_ID_MONITOR_PASSWORD_2: 	 		// 0x0016	密码2
            res = tm_pdu_data_rw(TM_RW, cmd, pt, &tOmcPara.ip[1].password, TM_STR, 20, 0, 0, &t_save->dev);
            break;

        case TM_ID_MONITOR_HEART:		 	 	// 0x0017	心跳包间隔
            res = tm_pdu_data_rw(TM_RW, cmd, pt, &tOmcPara.heart, TM_UINT2, 2, 1, 300, &t_save->dev);
            break;

        case TM_ID_MONITOR_DEV_PORT:		 	// 0x0018	设备端口号
            res = tm_pdu_data_rw(TM_RW, cmd, pt, &tOmcPara.dev_port, TM_UINT2, 2, 0, 65535, &t_save->dev);
            break;

        case TM_ID_MONITOR_LONGITUDE:		 	// 0x0019	经度
            res = tm_pdu_data_rw(TM_RW, cmd, pt, &tOmcPara.longitude, TM_STR, 20, 0, 0, &t_save->dev);
            break;

        case TM_ID_MONITOR_LATITUDE:		 	// 0x001A	纬度
            res = tm_pdu_data_rw(TM_RW, cmd, pt, &tOmcPara.latitude, TM_STR, 20, 0, 0, &t_save->dev);
            break;

        case TM_ID_MONITOR_POSITION:			// 0x001B	位置信息
            res = tm_pdu_data_rw(TM_RW, cmd, pt, &tOmcPara.address, TM_STR, 64, 0, 0, &t_save->dev);
            break;

        default:
            res = TM_ERR_IDERR;
            break;
    }

    return res;
}

/*****************************************************************************
* @Name	  : tm_pdu_cat1_data_ergodic
* @Brief  : 
* @Param  : dev_id: [输入/出]
**			 cmd: [输入/出]
**			 pt: [输入/出]
**			 t_save:
* @Retval :
* @Author :
* @Data   : 2023-12-18
* @Note   :
*****************************************************************************/
uint8_t tm_pdu_cat1_data_ergodic(uint8_t dev_id, uint8_t cmd, tm_oid_data_t * pt, tm_save_flag_t * t_save)
{
    uint8_t  res = TM_ERR_OK;
    uint8_t  temp_uint8 = 0;
	uint8_t cat1_name[32];
	
		
    switch(pt->oid)
    {
        case TM_ID_MODEM_MODEL:	 				// 0x0001	模组型号
			memset( cat1_name, 0, 32 );
			memcpy( cat1_name, DEFAULT_CAT1_NAME, strlen(DEFAULT_CAT1_NAME));
			res = tm_pdu_data_rw(TM_R, cmd, pt, cat1_name, TM_STR, 32, 0, 0, &t_save->dev);
            break;

        case TM_ID_MODEM_HW_VER:	 	 		// 0x0002	硬件版本
			res = tm_pdu_data_rw(TM_R, cmd, pt, &hCat1.ucaCatHwVer, TM_STR, 32, 0, 0, &t_save->dev);
            break;

        case TM_ID_MODEM_SW_VER:	 			// 0x0003	软件版本
			res = tm_pdu_data_rw(TM_R, cmd, pt, &hCat1.ucaCatSwVer, TM_STR, 32, 0, 0, &t_save->dev);
            break;

        case TM_ID_MODEM_SIM_STATE:	 			// 0x0005	SIM卡状态
			res = tm_pdu_data_rw(TM_R, cmd, pt, &hCat1.ucSimState, TM_UINT1, 1, 0, 1, &t_save->dev);
            break;

        case TM_ID_MODEM_SIM_ID:	 	 		// 0x0006	SIM卡卡号
			res = tm_pdu_data_rw(TM_R, cmd, pt, &hCat1.ucaSimNum, TM_STR, 32, 0, 0, &t_save->dev);
            break;

        case TM_ID_MODEM_LINK_STATE:	 		// 0x0007	连接服务器状态
			if( hCat1.socketStat[SOCKET_LINK_USER].socket_stat == SCOKET_CONNECTED )
				temp_uint8 = 1;
			else
				temp_uint8 = 0;
			res = tm_pdu_data_rw(TM_R, cmd, pt, &temp_uint8, TM_UINT1, 1, 0, 1, &t_save->dev);
            break;

        case TM_ID_MODEM_INIT:		 			// 0x0008	初始化

            break;

        case TM_ID_MODEM_POW_SWITCH: 	 		// 0x000C	电源开关
            temp_uint8 = hCat1.cat1_en;
            res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint8, TM_UINT1, 1, 0, 3, &t_save->unsave);
            if(TM_CMD_ACT_OK == res)
                cat1_on_off(temp_uint8);
            break;

        case TM_ID_MODEM_RST: 		 			// 0x000D	复位

            break;

        default:
            res = TM_ERR_IDERR;
            break;
    }

    return res;
}


/*****************************************************************************
* @Name	  : tm_pdu_modem_data_ergodic
* @Brief  :
* @Param  : dev_id: [输入/出]
**			 cmd: [输入/出]
**			 pt: [输入/出]
**			 t_save:
* @Retval :
* @Author :
* @Data   : 2023-12-18
* @Note   :
*****************************************************************************/
uint8_t tm_pdu_modem_data_ergodic(uint8_t dev_id, uint8_t cmd, tm_oid_data_t * pt, tm_save_flag_t * t_save)
{
    uint8_t  res = TM_ERR_OK;
    uint8_t  temp_uint8 = 0;
	int16_t  temp_int16 = 0;
		
	static uint8_t frame_cfg_flag   = B_FALSE;;
	static uint8_t spframe_cfg_flag = B_FALSE;
			
    switch(pt->oid)
    {
        case TM_ID_MODEM_MODEL:	 				// 0x0001	模组型号
			
            break;

        case TM_ID_MODEM_HW_VER:	 	 		// 0x0002	硬件版本
			res = tm_pdu_data_rw(TM_R, cmd, pt, &hModem.HWVer, TM_STR, 32, 0, 0, &t_save->dev);
            break;

        case TM_ID_MODEM_SW_VER:	 			// 0x0003	软件版本
			//res = tm_pdu_data_rw(TM_R, cmd, pt, &hModem.TAFirmVer, TM_STR, 32, 0, 0, &t_save->dev);
		res = tm_pdu_data_rw(TM_R, cmd, pt, &hModem.SWVer, TM_STR, 32, 0, 0, &t_save->dev);
            break;

        case TM_ID_MODEM_AP_VER:	 	 		// 0x0004	AP版本
			res = tm_pdu_data_rw(TM_R, cmd, pt, &hModem.APVer, TM_STR, 32, 0, 0, &t_save->dev);
            break;

        case TM_ID_MODEM_SIM_STATE:	 			// 0x0005	SIM卡状态

            break;

        case TM_ID_MODEM_SIM_ID:	 	 		// 0x0006	SIM卡卡号

            break;

        case TM_ID_MODEM_LINK_STATE:	 		// 0x0007	连接服务器状态

            break;

        case TM_ID_MODEM_INIT:		 			// 0x0008	初始化

            break;

        case TM_ID_MODEM_POW_SWITCH: 	 		// 0x000C	电源开关
            temp_uint8 = hCat1.cat1_en;
            res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint8, TM_UINT1, 1, 0, 3, &t_save->unsave);
            if(TM_CMD_ACT_OK == res)
                cat1_on_off(temp_uint8);
            break;

        case TM_ID_MODEM_RST: 		 			// 0x000D	复位

            break;

		case TM_ID_MODEM_RE_SCAN:				// 0x000E 重新扫描
            set_band_scan_restart();
			break;
		
		case TM_ID_MODEM_5G_SYNC:					//0x0010	//5G 同步状态
			temp_uint8 = ( SYNC_SYNC == hModem.NR_net_info.NR_Sync ) ? SYNC : UNSYNC;
			res = tm_pdu_data_rw(TM_R, cmd, pt, &temp_uint8, TM_UINT1, 1, 0, 1, &t_save->unsave);
			break;
		
        case TM_ID_MODEM_5G_FRAME:					// 0x0011	5G上下行配比
			res = tm_pdu_data_rw(TM_RW, cmd, pt, &t_5g_modem_config.frame, TM_UINT1, 1, 1, 6, &t_save->dev);
			if( (TM_CMD_SET == cmd) &&( t_5g_modem_config.frame>0 ) && ( t_5g_modem_config.frame<=6 ) )
			{
				frame_cfg_flag = B_TRUE;
				if((B_TRUE==frame_cfg_flag) && (B_TRUE==spframe_cfg_flag))
				{
					rf_5g_modem_switch( (RF_BAND_E)t_rf_sys[ RF_SYS_2 ].t_public.valid_band, RF_SWITCH_OPEN );
					frame_cfg_flag = B_FALSE;
					spframe_cfg_flag = B_FALSE;
					flag_fpga_cmd_set_5g_frame();
				}
			}
            break;

        case TM_ID_MODEM_5G_SPECIAL_FRAME:			// 0x0012	5G特殊子帧配置
			res = tm_pdu_data_rw(TM_RW, cmd, pt, &t_5g_modem_config.specialTime, TM_UINT1, 1, 1, 3, &t_save->dev);
			if( (TM_CMD_SET == cmd) &&( t_5g_modem_config.specialTime>0 ) && ( t_5g_modem_config.specialTime<=3 ) )
			{
				spframe_cfg_flag = B_TRUE;
				if((B_TRUE==frame_cfg_flag) && (B_TRUE==spframe_cfg_flag))
				{
					rf_5g_modem_switch( (RF_BAND_E)t_rf_sys[ RF_SYS_2 ].t_public.valid_band, RF_SWITCH_OPEN );
					frame_cfg_flag = B_FALSE;
					spframe_cfg_flag = B_FALSE;
					flag_fpga_cmd_set_5g_frame();
				}
			}
            break;

        case TM_ID_MODEM_5G_FRAME_TIME_OFFSET:		// 0x0013	5G帧头时间调整量

            break;

        case TM_ID_MODEM_5G_FIRST_TIME_OFFSET:		// 0x0014	第一转换点的提前/滞后量

            break;

        case TM_ID_MODEM_5G_SECOND_TIME_OFFSET:		// 0x0015	第二转换点的提前/滞后量

            break;
		
		case TM_ID_MODEM_5G_SSB:					// 0x0016  SSB频点
			res = tm_pdu_data_rw(TM_RW, cmd, pt, &t_5g_modem_config.ssb, TM_UINT4, 4, 1, 0xFFFFFFFF, &t_save->dev);
			if( TM_CMD_SET == cmd )
				modem_tracking_NR_cell( hModem.NR_net_info.freq, hModem.NR_net_info.pci, 41 );
            break;
		
		case TM_ID_MODEM_5G_RSRP: 		 		//0x0017	5G RSRP
			temp_int16 = read_rsrp_in_calib_mode( NR );
			res = tm_pdu_data_rw(TM_R, cmd, pt, &temp_int16, TM_SINT2, 2, -256, 256, &t_save->unsave);
			break;

		case TM_ID_MODEM_5G_SINR:				//0x0018  	5G SINR
			temp_int16 = hModem.NR_net_info.sinr;
			res = tm_pdu_data_rw(TM_R, cmd, pt, &temp_int16, TM_SINT2, 2, -256, 256, &t_save->unsave);
			break;

		case TM_ID_MODEM_5G_RSSI: 		 		//0x0019	5G RSSI
			
			break;

		case TM_ID_MODEM_5G_PCI: 		 		//0x001A	5G PCI
			temp_int16 = hModem.NR_net_info.pci;
			res = tm_pdu_data_rw(TM_R, cmd, pt, &temp_int16, TM_SINT2, 2, -256, 256, &t_save->unsave);
			break;
		
		case TM_ID_MODEM_5G_UR_EDGE_OFFSET:		//0x001B	5G UP上升沿偏移量
			res = tm_pdu_data_rw(TM_R, cmd, pt, &t_edge.nr_ul_rising, TM_SINT4, 4, -3000, 3000, &t_save->unsave);
			rf_5g_modem_edge_cfg( NR );
			break;
		
		case TM_ID_MODEM_5G_UF_EDGE_OFFSET:		//0x001C	5G UP下降沿偏移量
			res = tm_pdu_data_rw(TM_R, cmd, pt, &t_edge.nr_ul_falling, TM_SINT4, 4, -3000, 3000, &t_save->unsave);
			rf_5g_modem_edge_cfg( NR );
			break;

		case TM_ID_MODEM_5G_DR_EDGE_OFFSET:		//0x001D	5G DW上升沿偏移量
			res = tm_pdu_data_rw(TM_R, cmd, pt, &t_edge.nr_dl_rising, TM_SINT4, 4, -3000, 3000, &t_save->unsave);
			rf_5g_modem_edge_cfg( NR );
			break;

		case TM_ID_MODEM_5G_DF_EDGE_OFFSET:		//0x001E	5G DW下降沿偏移量
			res = tm_pdu_data_rw(TM_R, cmd, pt, &t_edge.nr_dl_falling, TM_SINT4, 4, -3000, 3000, &t_save->unsave);
			rf_5g_modem_edge_cfg( NR );
			break;

		case TM_ID_MODEM_4G_SYNC:				//0x0020	4G 同步状态
			temp_uint8 = ( SYNC_SYNC == hModem.LTE_net_info.LTE_Sync ) ? SYNC : UNSYNC;
			res = tm_pdu_data_rw(TM_R, cmd, pt, &temp_uint8, TM_UINT1, 1, 0, 1, &t_save->unsave);
			break;
		
		case TM_ID_MODEM_4G_FRAME:				//0x0021	4G 上下行配比
			temp_uint8 = hModem.LTE_net_info.sfa;
			res = tm_pdu_data_rw(TM_R, cmd, pt, &temp_uint8, TM_UINT1, 1, 0, 10, &t_save->unsave);	
			break;

		case TM_ID_MODEM_4G_SPECIAL_FRAME:		//0x0022	4G 特殊子帧配置
			temp_uint8 = hModem.LTE_net_info.ssfp;
			res = tm_pdu_data_rw(TM_R, cmd, pt, &temp_uint8, TM_UINT1, 1, 0, 10, &t_save->unsave);		
			break;

		case TM_ID_MODEM_4G_FRAME_TIME_OFFSET:	//0x0023	4G 帧头时间调整量
				
			break;

		case TM_ID_MODEM_4G_FIRST_TIME_OFFSET:	//0x0024	4G 第一转换点的提前/滞后量
				
			break;

		case TM_ID_MODEM_4G_SECOND_TIME_OFFSET:	//0x0025	4G 第二转换点的提前/滞后量
			
			break;

		case TM_ID_MODEM_4G_SSB:				//0x0026	4G SSB 频点
				
			break;

		case TM_ID_MODEM_4G_RSRP: 		 		//0x0027	4G RSRP
			//temp_int16 = hModem.LTE_net_info.rsrp;
			temp_int16 = read_rsrp_in_calib_mode( LTE );
			res = tm_pdu_data_rw(TM_R, cmd, pt, &temp_int16, TM_SINT2, 2, -256, 256, &t_save->unsave);
			break;

		case TM_ID_MODEM_4G_SINR:				//0x0028 	4G SINR
			temp_int16 = hModem.LTE_net_info.sinr;
			res = tm_pdu_data_rw(TM_R, cmd, pt, &temp_int16, TM_SINT2, 2, -256, 256, &t_save->unsave);
			break;	

		case TM_ID_MODEM_4G_RSSI: 		 		//0x0029	4G RSSI
			break;

		case TM_ID_MODEM_4G_PCI: 		 		//0x002A	4G PCI
			temp_int16 = hModem.LTE_net_info.pci;
			res = tm_pdu_data_rw(TM_R, cmd, pt, &temp_int16, TM_SINT2, 2, -256, 256, &t_save->unsave);
			break;

		case TM_ID_MODEM_4G_UR_EDGE_OFFSET:		//0x002B	4G UP上升沿偏移量
			res = tm_pdu_data_rw(TM_R, cmd, pt, &t_edge.lte_ul_rising, TM_SINT4, 4, -3000, 3000, &t_save->unsave);
			rf_5g_modem_edge_cfg( LTE );
			break;

		case TM_ID_MODEM_4G_UF_EDGE_OFFSET:		//0x002C	4G UP下降沿偏移量
			res = tm_pdu_data_rw(TM_R, cmd, pt, &t_edge.lte_ul_falling, TM_SINT4, 4, -3000, 3000, &t_save->unsave);	
			rf_5g_modem_edge_cfg( LTE );
			break;

		case TM_ID_MODEM_4G_DR_EDGE_OFFSET:		//0x002D	4G DW上升沿偏移量
			res = tm_pdu_data_rw(TM_R, cmd, pt, &t_edge.lte_dl_rising, TM_SINT4, 4, -3000, 3000, &t_save->unsave);	
			rf_5g_modem_edge_cfg( LTE );
			break;

		case TM_ID_MODEM_4G_DF_EDGE_OFFSET:		//0x002E	4G DW下降沿偏移量
			res = tm_pdu_data_rw(TM_R, cmd, pt, &t_edge.lte_dl_falling, TM_SINT4, 4, -3000, 3000, &t_save->unsave);	
			rf_5g_modem_edge_cfg( LTE );
			break;
		
        default:
            res = TM_ERR_IDERR;
            break;
    }

    return res;
}


/*****************************************************************************
* @Name	  : tm_pdu_fpga_data_ergodic
* @Brief  :
* @Param  : dev_id: [输入/出]
**			 cmd: [输入/出]
**			 pt: [输入/出]
** 			 t_save:
* @Retval :
* @Author :
* @Data   : 2023-12-18
* @Note   :
*****************************************************************************/
uint8_t tm_pdu_fpga_data_ergodic(uint8_t dev_id, uint8_t cmd, tm_oid_data_t * pt, tm_save_flag_t * t_save)
{
    uint8_t res = TM_ERR_OK;

    uint8_t  temp_uint8 = 0;
    uint32_t temp_uint32 = 0;

    uint8_t temp_str[100] = {0};

    uint8_t  temp_int8 = 0;
    static uint8_t sys_use = 0xFF;//pt->sys - 1;

    switch(pt->oid)
    {
        case TM_ID_FPGA_STATE: 					// 0x0001	FPGA状态查询
            temp_uint8 = fpga_get_state();
            res = tm_pdu_data_rw(TM_R, cmd, pt, &temp_uint8, TM_UINT1, 1, 0, 1, &t_save->unsave);
            break;

        case TM_ID_FPGA_VER: 					// 0x0002	FPGA版本信息
            sprintf((char *)temp_str, "%x  %x", fpga_ver_num(), fpga_ver_time());
            res = tm_pdu_data_rw(TM_R, cmd, pt, temp_str, TM_STR, 50, 0, 0, &t_save->unsave);
            break;

        case TM_ID_FPGA_OOK_STATE: 				// 0x0003	OOK状态查询
            temp_uint8 = fpga_ook_state();
            res = tm_pdu_data_rw(TM_R, cmd, pt, &temp_uint8, TM_UINT1, 1, 0, 4, &t_save->unsave);
            break;

        case TM_ID_FPGA_OOK_FRAME_HEAD: 		// 0x0004	OOK传输帧头类型
            temp_uint8 = fpga_get_frame_type();
            res = tm_pdu_data_rw(TM_R, cmd, pt, &temp_uint8, TM_UINT1, 1, 0, 2, &t_save->unsave);
            break;

        case TM_ID_FPGA_PWM_DUTY_CYCLE: 		// 0x0005	PWM占空比
            if(TM_CMD_SET == cmd)
            {
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint32, TM_UINT4, 4, 0, 50000, &t_save->unsave);
                fpga_set_clock_pwm(temp_uint32);
            }
            else
            {
                temp_uint32 = fpga_get_clock_pwm();
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint32, TM_UINT4, 4, 0, 50000, &t_save->unsave);
            }
            break;

        case TM_ID_FPGA_OOK_FREQ_POINT_SWITCH: 	// 0x0006	OOK频点切换
            if(TM_CMD_SET == cmd)
            {
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint8, TM_UINT1, 1, 0, 10, &t_save->unsave);
                if(temp_uint8 == 1)
                    fpga_ook_hop_freq();
                else if(temp_uint8 == 2)
                    fpga_ook_off();
                else if(temp_uint8 == 3)
                    fpga_ook_on();
            }
            else
            {
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint8, TM_UINT1, 1, 0, 10, &t_save->unsave);
            }
            break;

        case TM_ID_FPGA_OOK_RSSI: 				// 0x0007	OOK RSSI查询
			#if LOCAL_MODULE_TYPE == _MU

			#else
            temp_int8 = fpga_get_ook_rssi();
            res = tm_pdu_data_rw(TM_R, cmd, pt, &temp_int8, TM_SINT1, 1, -128, 127, &t_save->unsave);
			#endif
            break;

        case TM_ID_FPGA_OOK_SWEEP_FREQ: 		// 0x0008	OOK 扫频查询
			#if LOCAL_MODULE_TYPE == _MU
            sprintf((char *)temp_str, "%d", OOK_GetFrequency());
            res = tm_pdu_data_rw(TM_R, cmd, pt, temp_str, TM_STR, 50, 0, 0, &t_save->unsave);
			#else
            fpga_ook_scan_freq((char *)temp_str);
            res = tm_pdu_data_rw(TM_R, cmd, pt, temp_str, TM_STR, 50, 0, 0, &t_save->unsave);
			#endif
            break;

        case TM_ID_FPGA_4G_OUT_MODE: 			// 0x0009	4G输出模式
            if(TM_CMD_SET == cmd)
            {
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint8, TM_UINT1, 1, 0, 4, &t_save->unsave);
				#if LOCAL_MODULE_TYPE == _MU

				#else
                if((FREQ_BAND_B3_1800M == sys_use) || (FREQ_BAND_B8_900M == sys_use))
                {
                    fpga_set_4g_tdd_mode(temp_uint8);
                }
                else if((FREQ_BAND_B39_F == sys_use) || (FREQ_BAND_B40_E == sys_use))
                {
                    if(temp_uint8 < 4)
                        fpga_set_4g_tdd_mode(temp_uint8);   //0时分1常发2常收
                }
                else
                {}
				#endif
            }
            else
            {
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint8, TM_UINT1, 1, 0, 4, &t_save->unsave);
            }
            break;

        case TM_ID_FPGA_5G_OUT_MODE: 			// 0x000A	5G输出模式
            if(TM_CMD_SET == cmd)
            {
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint8, TM_UINT1, 1, 0, 3, &t_save->unsave);
				#if LOCAL_MODULE_TYPE == _MU

				#else
                fpga_set_5g_tdd_mode(temp_uint8);   //0时分1常发2常收
				#endif
            }
            else
            {
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint8, TM_UINT1, 1, 0, 3, &t_save->unsave);
            }
            break;

        case TM_ID_FPGA_CLK_STATE: 				// 0x000B	时钟状态查询
            sprintf((char *)temp_str, "mea=%d(%d) pwm=%d dev=%d\r\n", fpga_get_clock_meaRes(), fpga_get_clock_meaVld(), fpga_get_clock_pwm(), fpga_get_4g_frame_dev());
            res = tm_pdu_data_rw(TM_R, cmd, pt, temp_str, TM_STR, 100, 0, 0, &t_save->unsave);
            break;

        case TM_ID_FPGA_4G_BAND:				// 0x000C	4G频段
            if(TM_CMD_SET == cmd)
            {
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint8, TM_UINT1, 1, 0, 3, &t_save->unsave);
                tm_sys_open(temp_uint8);
                hi_delay_ms(50);
                sys_use = temp_uint8;
                fpga_set_4g_freqband(temp_uint8);
            }
            else
            {
                temp_uint8 = fpga_get_4g_freqband();
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint8, TM_UINT1, 1, 0, 3, &t_save->unsave);
            }
            break;

        case TM_ID_FPGA_5G_FRAME: 				// 0x000D	5G帧格式
            if(TM_CMD_SET == cmd)
            {
                res = tm_pdu_data_rw(TM_RW, cmd, pt, temp_str, TM_STR, 20, 0, 0, &t_save->unsave);
                fpga_set_5g_frame((char *)temp_str);
				flag_fpga_cmd_set_5g_frame();
            }
            else
            {
                fpga_get_5g_frame((char *)temp_str);
                res = tm_pdu_data_rw(TM_RW, cmd, pt, temp_str, TM_STR, 20, 0, 0, &t_save->unsave);
            }
            break;

        case TM_ID_FPGA_4G_FRAME: 				// 0x000E	4G子帧配比
            if(TM_CMD_SET == cmd)
            {
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint8, TM_UINT1, 1, 0, 6, &t_save->unsave);
                fpga_set_4g_subframe(temp_uint8);
            }
            else
            {
                temp_uint8 = fpga_get_4g_subframe();
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint8, TM_UINT1, 1, 0, 6, &t_save->unsave);
            }
            break;

        case TM_ID_FPGA_4G_SPECIAL_FRAME: 		// 0x000F	4G特殊子帧配比
            if(TM_CMD_SET == cmd)
            {
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint8, TM_UINT1, 1, 0, 8, &t_save->unsave);
                fpga_set_4g_specical_subframe(temp_uint8);
            }
            else
            {
                temp_uint8 = fpga_get_4g_specical_subframe();
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint8, TM_UINT1, 1, 0, 8, &t_save->unsave);
            }
            break;

        case TM_ID_FPGA_4G_CENTER_FREQ: 		// 0x0010	4G中心频点
            if(TM_CMD_SET == cmd)
            {
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint32, TM_UINT4, 4, 0, 0, &t_save->unsave);
                fpga_set_4g_center_freq(temp_uint32);
            }
            else
            {
                temp_uint32 = fpga_get_4g_center_freq();
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint32, TM_UINT4, 4, 0, 0, &t_save->unsave);
            }
            break;

        case TM_ID_FPGA_4G_BW: 					// 0x0011	4G带宽
            if(TM_CMD_SET == cmd)
            {
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint32, TM_UINT4, 4, 0, 0, &t_save->unsave);
                fpga_set_4g_bandwidth(temp_uint32);
            }
            else
            {
                temp_uint32 = fpga_get_4g_bandwidth();
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint32, TM_UINT4, 4, 0, 0, &t_save->unsave);
            }
            break;

        case TM_ID_FPGA_UP_SILENCE: 			// 0x0012	上行静默
            if(TM_CMD_SET == cmd)
            {
                res = tm_pdu_data_rw(TM_W, cmd, pt, &temp_uint8, TM_UINT1, 1, 0, 6, &t_save->unsave);

#if LOCAL_MODULE_TYPE == _MU
                if(0 == temp_uint8)  //0: 4G 8668初始化
                {
                    if(sys_use == FREQ_BAND_B3_1800M)
                    {
                        h8668_LTE.band = (RF_BAND_E)sys_use;
                        if(BSP_Ecr8668Init(&h8668_LTE) == STAT_ERROR)
                        {
                            rtt_printf(RTT_RF, RTT_TEXT_CYAN"LTE B3 Ecr8668 init fail!\r\n");
                        }
                        else
                        {
                            rtt_printf(RTT_RF, RTT_TEXT_CYAN"LTE B3 Ecr8668 init success!\r\n"); //rtt_printf( RTT_RF, RTT_TEXT_CYAN"LTE B3 Ecr8668 init success! %d\r\n", BSP_866x_Ex_Read(&h8668_LTE, 0x20000004) );
                            hi_delay_ms(1000);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x20004000, 0x00034505);
                            hi_delay_ms(200);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x20004004, 0x00010001);
                            hi_delay_ms(200);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x20004008, 0x001BBB9C);
                            hi_delay_ms(200);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x20004028, 0x00008025);
                            hi_delay_ms(200);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x20004030, 0x00008020);
                            hi_delay_ms(200);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x00201180, 0x30200001);
                            hi_delay_ms(200);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x0020108C, 0x080E0000);
                            hi_delay_ms(200);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x00201080, 0x00000000);
                            hi_delay_ms(200);

                            BSP_866x_Ext_Write(&h8668_LTE, 0x00200880, 0x27C);
                            hi_delay_ms(200);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x00200938, 0x1082);
                            hi_delay_ms(200);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x00200960, 0xB260);
                            hi_delay_ms(200);
                        }
                    }
                    else if(sys_use == FREQ_BAND_B8_900M)
                    {
                        h8668_LTE.band = (RF_BAND_E)sys_use;
                        if(BSP_Ecr8668Init(&h8668_LTE) == STAT_ERROR)
                        {
                            rtt_printf(RTT_RF, RTT_TEXT_CYAN"LTE B8 Ecr8668 init fail!\r\n");
                        }
                        else
                        {
                            rtt_printf(RTT_RF, RTT_TEXT_CYAN"LTE B8 Ecr8668 init success!\r\n"); //rtt_printf( RTT_RF, RTT_TEXT_CYAN"LTE B8 Ecr8668 init success! %d\r\n", BSP_866x_Ex_Read(&h8668_LTE, 0x20000004) );
                            hi_delay_ms(1000);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x20004000, 0x00034505);
                            hi_delay_ms(200);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x20004004, 0x00000000);
                            hi_delay_ms(200);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x20004008, 0x000E5DBC);
                            hi_delay_ms(200);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x20004028, 0x00008025);
                            hi_delay_ms(200);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x20004030, 0x00008020);
                            hi_delay_ms(200);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x00201180, 0x30200001);
                            hi_delay_ms(200);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x0020108C, 0x080E0000);
                            hi_delay_ms(200);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x00201080, 0x00000000);
                            hi_delay_ms(200);

                            BSP_866x_Ext_Write(&h8668_LTE, 0x00200880, 0x27C);
                            hi_delay_ms(200);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x00200938, 0x1082);
                            hi_delay_ms(200);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x00200960, 0xB260);
                            hi_delay_ms(200);
                        }
                    }
                    else if(sys_use == FREQ_BAND_B39_F)
                    {
                        h8668_LTE.band = (RF_BAND_E)sys_use;
                        if(BSP_Ecr8668Init(&h8668_LTE) == STAT_ERROR)
                        {
                            rtt_printf(RTT_RF, RTT_TEXT_CYAN"LTE B39 Ecr8668 init fail!\r\n");
                        }
                        else
                        {
                            rtt_printf(RTT_RF, RTT_TEXT_CYAN"LTE B39 Ecr8668 init success!\r\n"); //rtt_printf( RTT_RF, RTT_TEXT_CYAN"LTE B39 Ecr8668 init success! %d\r\n", BSP_866x_Ex_Read(&h8668_LTE, 0x20000004) );
                            hi_delay_ms(1000);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x20004000, 0x00034505);
                            hi_delay_ms(200);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x20004004, 0x00010001);
                            hi_delay_ms(200);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x20004008, 0x001CFDE0);
                            hi_delay_ms(200);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x20004028, 0x00008025);
                            hi_delay_ms(200);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x20004030, 0x00008020);
                            hi_delay_ms(200);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x00201180, 0x30200001);
                            hi_delay_ms(200);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x0020108C, 0x080E0000);
                            hi_delay_ms(200);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x00201080, 0x00000000);
                            hi_delay_ms(200);

                            BSP_866x_Ext_Write(&h8668_LTE, 0x00200880, 0x27C);
                            hi_delay_ms(200);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x00200938, 0x1082);
                            hi_delay_ms(200);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x00200960, 0xB260);
                            hi_delay_ms(200);
                        }
                    }
                    else if(sys_use == FREQ_BAND_B40_E)
                    {
                        h8668_LTE.band = (RF_BAND_E)sys_use;
                        if(BSP_Ecr8668Init(&h8668_LTE) == STAT_ERROR)
                        {
                            rtt_printf(RTT_RF, RTT_TEXT_CYAN"LTE B40 Ecr8668 init fail!\r\n");
                        }
                        else
                        {
                            rtt_printf(RTT_RF, RTT_TEXT_CYAN"LTE B40 Ecr8668 init success!\r\n"); //rtt_printf( RTT_RF, RTT_TEXT_CYAN"LTE B40 Ecr8668 init success! %d\r\n", BSP_866x_Ex_Read(&h8668_LTE, 0x20000004) );
                            hi_delay_ms(1000);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x20004000, 0x00034505);
                            hi_delay_ms(200);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x20004004, 0x00010001);
                            hi_delay_ms(200);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x20004008, 0x0023DBB0);
                            hi_delay_ms(200);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x20004028, 0x00008025);
                            hi_delay_ms(200);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x20004030, 0x00008020);
                            hi_delay_ms(200);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x00201180, 0x30200001);
                            hi_delay_ms(200);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x0020108C, 0x080E0000);
                            hi_delay_ms(200);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x00201080, 0x00000000);
                            hi_delay_ms(200);

                            BSP_866x_Ext_Write(&h8668_LTE, 0x00200880, 0x27C);
                            hi_delay_ms(200);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x00200938, 0x1082);
                            hi_delay_ms(200);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x00200960, 0xB260);
                            hi_delay_ms(200);
                        }
                    }
                    else
                    {
                        rtt_printf(RTT_RF, RTT_TEXT_CYAN"LTE band error!\r\n");
                    }
                }
                else if(1 == temp_uint8)  //1: 5G 8668初始化
                {
                    tm_sys_open(BAND_41);
                    tm_sys_open(BAND_41M);
                    hi_delay_ms(50);

                    h8668_NR.band = (RF_BAND_E)4;
                    if(BSP_Ecr8668Init(&h8668_NR) == STAT_ERROR)
                    {
                        rtt_printf(RTT_RF, RTT_TEXT_CYAN"NR Ecr8668 init fail!\r\n");
                    }
                    else
                    {
                        rtt_printf(RTT_RF, RTT_TEXT_CYAN"NR Ecr8668 init success!\r\n"); //rtt_printf( RTT_RF, RTT_TEXT_CYAN"NR Ecr8668 init success! %d\r\n", BSP_866x_Ex_Read(&h8668_NR, 0x20000004) );
                        hi_delay_ms(1000);
                        BSP_866x_Ext_Write(&h8668_NR, 0x20004000, 0x00034505);
                        hi_delay_ms(200);
                        BSP_866x_Ext_Write(&h8668_NR, 0x20004004, 0x00030003);
                        hi_delay_ms(200);
                        BSP_866x_Ext_Write(&h8668_NR, 0x20004008, 0x00272388);
                        hi_delay_ms(200);
                        BSP_866x_Ext_Write(&h8668_NR, 0x20004028, 0x00008025);
                        hi_delay_ms(200);
                        BSP_866x_Ext_Write(&h8668_NR, 0x20004030, 0x00008020);
                        hi_delay_ms(200);
                        BSP_866x_Ext_Write(&h8668_NR, 0x00201180, 0x30200001);
                        hi_delay_ms(200);
                        BSP_866x_Ext_Write(&h8668_NR, 0x0020108C, 0x080E0000);
                        hi_delay_ms(200);
                        BSP_866x_Ext_Write(&h8668_NR, 0x00201080, 0x00000000);
                        hi_delay_ms(200);

                        BSP_866x_Ext_Write(&h8668_NR, 0x00200880, 0x27C);
                        hi_delay_ms(200);
                        BSP_866x_Ext_Write(&h8668_NR, 0x00200938, 0x1082);
                        hi_delay_ms(200);
                        BSP_866x_Ext_Write(&h8668_NR, 0x00200960, 0xB260);
                        hi_delay_ms(200);
                    }

                    h8668_NR_MIMO.band = (RF_BAND_E)5;
                    if(BSP_Ecr8668Init(&h8668_NR_MIMO) == STAT_ERROR)
                    {
                        rtt_printf(RTT_RF, RTT_TEXT_CYAN"NR MIMO Ecr8668 init fail!\r\n");
                    }
                    else
                    {
                        rtt_printf(RTT_RF, RTT_TEXT_CYAN"NR MIMO Ecr8668 init success!\r\n"); //rtt_printf( RTT_RF, RTT_TEXT_CYAN"NR MIMO Ecr8668 init success! %d\r\n", BSP_866x_Ex_Read(&h8668_NR_MIMO, 0x20000004) );
                        hi_delay_ms(1000);
                        BSP_866x_Ext_Write(&h8668_NR_MIMO, 0x20004000, 0x00034505);
                        hi_delay_ms(200);
                        BSP_866x_Ext_Write(&h8668_NR_MIMO, 0x20004004, 0x00030003);
                        hi_delay_ms(200);
                        BSP_866x_Ext_Write(&h8668_NR_MIMO, 0x20004008, 0x00272388);
                        hi_delay_ms(200);
                        BSP_866x_Ext_Write(&h8668_NR_MIMO, 0x20004028, 0x00008025);
                        hi_delay_ms(200);
                        BSP_866x_Ext_Write(&h8668_NR_MIMO, 0x20004030, 0x00008020);
                        hi_delay_ms(200);
                        BSP_866x_Ext_Write(&h8668_NR_MIMO, 0x00201180, 0x30200001);
                        hi_delay_ms(200);
                        BSP_866x_Ext_Write(&h8668_NR_MIMO, 0x0020108C, 0x080E0000);
                        hi_delay_ms(200);
                        BSP_866x_Ext_Write(&h8668_NR_MIMO, 0x00201080, 0x00000000);
                        hi_delay_ms(200);

                        BSP_866x_Ext_Write(&h8668_NR_MIMO, 0x00200880, 0x27C);
                        hi_delay_ms(200);
                        BSP_866x_Ext_Write(&h8668_NR_MIMO, 0x00200938, 0x1082);
                        hi_delay_ms(200);
                        BSP_866x_Ext_Write(&h8668_NR_MIMO, 0x00200960, 0xB260);
                        hi_delay_ms(200);
                    }
                }
                else
                {
                }
#else
                if(0 == temp_uint8)  //0: 4G 8668初始化
                {
                    if(sys_use == FREQ_BAND_B3_1800M)
                    {
                        h8668_LTE.band = (RF_BAND_E)sys_use;
                        if(BSP_Ecr8668Init(&h8668_LTE) == STAT_ERROR)
                        {
                            rtt_printf(RTT_RF, RTT_TEXT_CYAN"LTE B3 Ecr8668 init fail!\r\n");
                        }
                        else
                        {
                            rtt_printf(RTT_RF, RTT_TEXT_CYAN"LTE B3 Ecr8668 init success!\r\n"); //rtt_printf( RTT_RF, RTT_TEXT_CYAN"LTE B3 Ecr8668 init success! %d\r\n", BSP_866x_Ex_Read(&h8668_LTE, 0x20000004) );
                            hi_delay_ms(1000);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x20004000, 0x00034505);
                            hi_delay_ms(200);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x20004004, 0x00010001);
                            hi_delay_ms(200);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x20004008, 0x001A4884);
                            hi_delay_ms(200);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x20004028, 0x00008025);
                            hi_delay_ms(200);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x20004030, 0x00008020);
                            hi_delay_ms(200);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x00201180, 0x30200001);
                            hi_delay_ms(200);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x0020108C, 0x080E0000);
                            hi_delay_ms(200);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x00201080, 0x00000000);
                            hi_delay_ms(200);

                            BSP_866x_Ext_Write(&h8668_LTE, 0x00200880, 0x27C);
                            hi_delay_ms(200);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x00200938, 0x1082);
                            hi_delay_ms(200);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x00200960, 0xB260);
                            hi_delay_ms(200);
                        }
                    }
                    else if(sys_use == FREQ_BAND_B8_900M)
                    {
                        h8668_LTE.band = (RF_BAND_E)sys_use;
                        if(BSP_Ecr8668Init(&h8668_LTE) == STAT_ERROR)
                        {
                            rtt_printf(RTT_RF, RTT_TEXT_CYAN"LTE B8 Ecr8668 init fail!\r\n");
                        }
                        else
                        {
                            rtt_printf(RTT_RF, RTT_TEXT_CYAN"LTE B8 Ecr8668 init success!\r\n"); //rtt_printf( RTT_RF, RTT_TEXT_CYAN"LTE B8 Ecr8668 init success! %d\r\n", BSP_866x_Ex_Read(&h8668_LTE, 0x20000004) );
                            hi_delay_ms(1000);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x20004000, 0x00034505);
                            hi_delay_ms(200);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x20004004, 0x00000000);
                            hi_delay_ms(200);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x20004008, 0x000DADF4);
                            hi_delay_ms(200);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x20004028, 0x00008025);
                            hi_delay_ms(200);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x20004030, 0x00008020);
                            hi_delay_ms(200);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x00201180, 0x30200001);
                            hi_delay_ms(200);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x0020108C, 0x080E0000);
                            hi_delay_ms(200);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x00201080, 0x00000000);
                            hi_delay_ms(200);

                            BSP_866x_Ext_Write(&h8668_LTE, 0x00200880, 0x27C);
                            hi_delay_ms(200);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x00200938, 0x1082);
                            hi_delay_ms(200);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x00200960, 0xB260);
                            hi_delay_ms(200);
                        }
                    }
                    else if(sys_use == FREQ_BAND_B39_F)
                    {
                        h8668_LTE.band = (RF_BAND_E)sys_use;
                        if(BSP_Ecr8668Init(&h8668_LTE) == STAT_ERROR)
                        {
                            rtt_printf(RTT_RF, RTT_TEXT_CYAN"LTE B39 Ecr8668 init fail!\r\n");
                        }
                        else
                        {
                            rtt_printf(RTT_RF, RTT_TEXT_CYAN"LTE B39 Ecr8668 init success!\r\n"); //rtt_printf( RTT_RF, RTT_TEXT_CYAN"LTE B39 Ecr8668 init success! %d\r\n", BSP_866x_Ex_Read(&h8668_LTE, 0x20000004) );
                            hi_delay_ms(1000);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x20004000, 0x00034505);
                            hi_delay_ms(200);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x20004004, 0x00010001);
                            hi_delay_ms(200);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x20004008, 0x001CFDE0);
                            hi_delay_ms(200);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x20004028, 0x00008025);
                            hi_delay_ms(200);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x20004030, 0x00008020);
                            hi_delay_ms(200);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x00201180, 0x30200001);
                            hi_delay_ms(200);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x0020108C, 0x080E0000);
                            hi_delay_ms(200);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x00201080, 0x00000000);
                            hi_delay_ms(200);

                            BSP_866x_Ext_Write(&h8668_LTE, 0x00200880, 0x27C);
                            hi_delay_ms(200);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x00200938, 0x1082);
                            hi_delay_ms(200);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x00200960, 0xB260);
                            hi_delay_ms(200);
                        }
                    }
                    else if(sys_use == FREQ_BAND_B40_E)
                    {
                        h8668_LTE.band = (RF_BAND_E)sys_use;
                        if(BSP_Ecr8668Init(&h8668_LTE) == STAT_ERROR)
                        {
                            rtt_printf(RTT_RF, RTT_TEXT_CYAN"LTE B40 Ecr8668 init fail!\r\n");
                        }
                        else
                        {
                            rtt_printf(RTT_RF, RTT_TEXT_CYAN"LTE B40 Ecr8668 init success!\r\n"); //rtt_printf( RTT_RF, RTT_TEXT_CYAN"LTE B40 Ecr8668 init success! %d\r\n", BSP_866x_Ex_Read(&h8668_LTE, 0x20000004) );
                            hi_delay_ms(1000);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x20004000, 0x00034505);
                            hi_delay_ms(200);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x20004004, 0x00010001);
                            hi_delay_ms(200);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x20004008, 0x0023DBB0);
                            hi_delay_ms(200);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x20004028, 0x00008025);
                            hi_delay_ms(200);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x20004030, 0x00008020);
                            hi_delay_ms(200);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x00201180, 0x30200001);
                            hi_delay_ms(200);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x0020108C, 0x080E0000);
                            hi_delay_ms(200);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x00201080, 0x00000000);
                            hi_delay_ms(200);

                            BSP_866x_Ext_Write(&h8668_LTE, 0x00200880, 0x27C);
                            hi_delay_ms(200);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x00200938, 0x1082);
                            hi_delay_ms(200);
                            BSP_866x_Ext_Write(&h8668_LTE, 0x00200960, 0xB260);
                            hi_delay_ms(200);
                        }
                    }
                    else
                    {
                        rtt_printf(RTT_RF, RTT_TEXT_CYAN"LTE band error!\r\n");
                    }
                }
                else if(1 == temp_uint8)  //1: 5G 8668初始化
                {
                    tm_sys_open(BAND_41);
                    tm_sys_open(BAND_41M);
                    hi_delay_ms(50);

                    h8668_NR.band = (RF_BAND_E)4;
                    if(BSP_Ecr8668Init(&h8668_NR) == STAT_ERROR)
                    {
                        rtt_printf(RTT_RF, RTT_TEXT_CYAN"NR Ecr8668 init fail!\r\n");
                    }
                    else
                    {
                        rtt_printf(RTT_RF, RTT_TEXT_CYAN"NR Ecr8668 init success!\r\n"); //rtt_printf( RTT_RF, RTT_TEXT_CYAN"NR Ecr8668 init success! %d\r\n", BSP_866x_Ex_Read(&h8668_NR, 0x20000004) );
                        hi_delay_ms(1000);
                        BSP_866x_Ext_Write(&h8668_NR, 0x20004000, 0x00034505);
                        hi_delay_ms(200);
                        BSP_866x_Ext_Write(&h8668_NR, 0x20004004, 0x00030003);
                        hi_delay_ms(200);
                        BSP_866x_Ext_Write(&h8668_NR, 0x20004008, 0x00272388);
                        hi_delay_ms(200);
                        BSP_866x_Ext_Write(&h8668_NR, 0x20004028, 0x00008025);
                        hi_delay_ms(200);
                        BSP_866x_Ext_Write(&h8668_NR, 0x20004030, 0x00008020);
                        hi_delay_ms(200);
                        BSP_866x_Ext_Write(&h8668_NR, 0x00201180, 0x30200001);
                        hi_delay_ms(200);
                        BSP_866x_Ext_Write(&h8668_NR, 0x0020108C, 0x080E0000);
                        hi_delay_ms(200);
                        BSP_866x_Ext_Write(&h8668_NR, 0x00201080, 0x00000000);
                        hi_delay_ms(200);

                        BSP_866x_Ext_Write(&h8668_NR, 0x00200880, 0x27C);
                        hi_delay_ms(200);
                        BSP_866x_Ext_Write(&h8668_NR, 0x00200938, 0x1082);
                        hi_delay_ms(200);
                        BSP_866x_Ext_Write(&h8668_NR, 0x00200960, 0xB260);
                        hi_delay_ms(200);
                    }

                    h8668_NR_MIMO.band = (RF_BAND_E)5;
                    if(BSP_Ecr8668Init(&h8668_NR_MIMO) == STAT_ERROR)
                    {
                        rtt_printf(RTT_RF, RTT_TEXT_CYAN"NR MIMO Ecr8668 init fail!\r\n");
                    }
                    else
                    {
                        rtt_printf(RTT_RF, RTT_TEXT_CYAN"NR MIMO Ecr8668 init success!\r\n"); //rtt_printf( RTT_RF, RTT_TEXT_CYAN"NR MIMO Ecr8668 init success! %d\r\n", BSP_866x_Ex_Read(&h8668_NR_MIMO, 0x20000004) );
                        hi_delay_ms(1000);
                        BSP_866x_Ext_Write(&h8668_NR_MIMO, 0x20004000, 0x00034505);
                        hi_delay_ms(200);
                        BSP_866x_Ext_Write(&h8668_NR_MIMO, 0x20004004, 0x00030003);
                        hi_delay_ms(200);
                        BSP_866x_Ext_Write(&h8668_NR_MIMO, 0x20004008, 0x00272388);
                        hi_delay_ms(200);
                        BSP_866x_Ext_Write(&h8668_NR_MIMO, 0x20004028, 0x00008025);
                        hi_delay_ms(200);
                        BSP_866x_Ext_Write(&h8668_NR_MIMO, 0x20004030, 0x00008020);
                        hi_delay_ms(200);
                        BSP_866x_Ext_Write(&h8668_NR_MIMO, 0x00201180, 0x30200001);
                        hi_delay_ms(200);
                        BSP_866x_Ext_Write(&h8668_NR_MIMO, 0x0020108C, 0x080E0000);
                        hi_delay_ms(200);
                        BSP_866x_Ext_Write(&h8668_NR_MIMO, 0x00201080, 0x00000000);
                        hi_delay_ms(200);

                        BSP_866x_Ext_Write(&h8668_NR_MIMO, 0x00200880, 0x27C);
                        hi_delay_ms(200);
                        BSP_866x_Ext_Write(&h8668_NR_MIMO, 0x00200938, 0x1082);
                        hi_delay_ms(200);
                        BSP_866x_Ext_Write(&h8668_NR_MIMO, 0x00200960, 0xB260);
                        hi_delay_ms(200);
                    }
                }
                else if(2 == temp_uint8)  //2: 4G静默
                {
                    rf_4g_up_silent_ctrl.ctl(&rf_4g_up_silent_ctrl, PORT_OFF);
                    BSP_866x_Ext_Write(&h8668_LTE, 0x00200880, 0x25C);
                    hi_delay_ms(500);
                    BSP_866x_Ext_Write(&h8668_LTE, 0x00200974, 0x301008);
                    hi_delay_ms(500);
                    fpga_set_4g_ul_silent();
                    fpga_set_trx_gain(FPGA_OP_DEVICE_TRX_4G, 0x27C);
                }
                else if(3 == temp_uint8)  //3: 5G静默
                {
                    rf_5g_up_silent_ctrl.ctl(&rf_5g_up_silent_ctrl, PORT_OFF);
                    BSP_866x_Ext_Write(&h8668_NR, 0x00200880, 0x25C);
                    hi_delay_ms(500);
                    BSP_866x_Ext_Write(&h8668_NR_MIMO, 0x00200880, 0x25C);
                    hi_delay_ms(500);
                    BSP_866x_Ext_Write(&h8668_NR, 0x00200974, 0x101008);
                    hi_delay_ms(500);
                    BSP_866x_Ext_Write(&h8668_NR_MIMO, 0x00200974, 0x101008);
                    hi_delay_ms(500);
                    fpga_set_trx_gain(FPGA_OP_DEVICE_TRX_5G1, 0x27C);
                    fpga_set_trx_gain(FPGA_OP_DEVICE_TRX_5G2, 0x27C);
                }
                else if(4 == temp_uint8)  //4: 4G退出静默
                {
                    rf_4g_up_silent_ctrl.ctl(&rf_4g_up_silent_ctrl, PORT_ON);
                }
                else if(5 == temp_uint8)  //5: 5G退出静默
                {
                    rf_5g_up_silent_ctrl.ctl(&rf_5g_up_silent_ctrl, PORT_ON);
                }
                else
                {
                }
#endif
            }
            break;

        case TM_ID_FPGA_4G_TRX_RSSI: 			// 0x0013	4G TRX RSSI
            temp_int8 = fpga_get_trx_rssi(FPGA_OP_DEVICE_TRX_4G);
            res = tm_pdu_data_rw(TM_R, cmd, pt, &temp_int8, TM_SINT1, 1, 0, 0, &t_save->unsave);
            break;

        case TM_ID_FPGA_5G_TRX_RSSI1: 			// 0x0014	5G1 TRX RSSI
            temp_int8 = fpga_get_trx_rssi(FPGA_OP_DEVICE_TRX_5G1);
            res = tm_pdu_data_rw(TM_R, cmd, pt, &temp_int8, TM_SINT1, 1, 0, 0, &t_save->unsave);
            break;

        case TM_ID_FPGA_5G_TRX_RSSI2: 			// 0x0015	5G2 TRX RSSI
            temp_int8 = fpga_get_trx_rssi(FPGA_OP_DEVICE_TRX_5G2);
            res = tm_pdu_data_rw(TM_R, cmd, pt, &temp_int8, TM_SINT1, 1, 0, 0, &t_save->unsave);
            break;

        case TM_ID_FPGA_4G_UL_MIPI_CTR: 		// 0x0016	4G UL MIPI控制
            res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint8, TM_UINT1, 1, 0, 2, &t_save->unsave);

            //0: 初始化    1: 打开	   2: 关闭
            if(0 == temp_uint8)
            {
            }
            else if(1 == temp_uint8)
            {
                fpga_4g_ul_pa_on();
            }
            else if(2 == temp_uint8)
            {
                fpga_4g_ul_pa_off();
            }
            else
            {}
            break;

        case TM_ID_FPGA_4G_DL_MIPI_CTR: 		// 0x0017	4G DL MIPI控制
            res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint8, TM_UINT1, 1, 0, 2, &t_save->unsave);

            //0: 初始化    1: 打开	   2: 关闭
            if(0 == temp_uint8)
            {
            }
            else if(1 == temp_uint8)
            {
                fpga_4g_dl_pa_on();
            }
            else if(2 == temp_uint8)
            {
                fpga_4g_dl_pa_off();
            }
            else
            {}
            break;

        default:
            res = TM_ERR_IDERR;
            break;
    }

    return res;
}


/*****************************************************************************
* @Name	  : tm_pdu_rf_data_ergodic
* @Brief  :
* @Param  : dev_id: [输入/出]
**			 cmd: [输入/出]
**			 pt: [输入/出]
**			 t_save:
* @Retval :
* @Author :
* @Data   : 2023-12-18
* @Note   :
*****************************************************************************/
uint8_t tm_pdu_rf_data_ergodic(uint8_t dev_id, uint8_t cmd, tm_oid_data_t * pt, tm_save_flag_t * t_save)
{
    uint8_t  res = TM_ERR_OK;
    int8_t   temp_int8 = 0;
    uint8_t  temp_uint8 = 0;
    uint16_t temp_uint16 = 0;
    uint32_t temp_uint32 = 0;
    int i = 0;

    static uint16_t pwm_value = 0;
    Status_t res_8668 = STAT_ERROR;

    uint8_t   * pst_dat = &pt->dat;
    uint8_t   sys_use = pt->sys - 1;
    RF_BAND_E band_use = tm_model_to_band(sys_use, pt->model);	//模块编码 -> BAND编码, 例: 0x20(B3) -> BAND_3(0)
    uint8_t   group = 0;		//写校准表使用
    Status_t  stat_res = STAT_OK;
    uint8_t   run_mode = tDevPublicPara.ucWorkMode;
	
	static uint8_t frame_cfg_flag = B_FALSE;
	static uint8_t spframe_cfg_flag = B_FALSE;
	
	uint8_t set_8668_flag = B_FALSE;
	uint8_t reload_para = B_FALSE;
//	uint8_t rescan_flag = B_FALSE;
				
    switch(pt->oid)
    {
        case TM_ID_RF_NAME:						// 0x0001	频段名称
            res = tm_pdu_data_rw(TM_R, cmd, pt, (void *)rf_band_name[band_use], TM_STR, 10, 0, 0, &t_save->unsave);
            break;

        case TM_ID_RF_TDD_OR_FDD:				// 0x0002	网络制式
            if(RF_SYS_1 == sys_use)
            {
                if((band_use == BAND_3) || (band_use == BAND_8))
                {
                    *pst_dat = IS_FDD;
                }
                else
                {
                    *pst_dat = IS_TDD;
                }
            }
            else  if((RF_SYS_2 == sys_use) || (RF_SYS_3 == sys_use))
            {
                *pst_dat = IS_TDD;
            }
            break;

        case TM_ID_RF_POWER_SWITCH:	 			//0x0003	电源开关
//			
//			if( WORK_NORMAL == run_mode )
//				break;
			
            res = tm_pdu_data_rw(TM_RW, cmd, pt, &t_rf_sys[sys_use].t_public.dc_switch, TM_UINT1, 1, 0, 1, &t_save->dev);
            if(1 == t_rf_sys[sys_use].t_public.dc_switch)
            {
                if(TM_CMD_SET == cmd)
                {
					if( RF_SYS_1 == sys_use )
					{
						t_rf_sys[sys_use].t_public.valid_band = band_use;
						t_rf_sys[sys_use].t_public.valid_bw   = t_rf_4g_save_para[band_use].band_width;
						t_rf_sys[sys_use].t_public.valid_freq = t_rf_4g_save_para[band_use].freq;
					}
					temp_uint8 = tm_power_on_load_bin(sys_use, band_use);
					if( RF_SYS_1 == sys_use )	//在进入pl模式时, t_rf_sys[RF_SYS_1].t_public.exist = 0, 禁止加载4G的bin文件
												//4G的bin文件需要发指令加载
						t_rf_sys[RF_SYS_1].t_public.exist = 1;
                    res = temp_uint8;
					
//					for(i = 0; i < BLE_CONNECT_MAX_NUM; i++)
//					{
//						if(ru_device_list[i].dev_type == DEV_TYPE_RU)
//							mrc_set_ru_param(ru_device_list[i].ble_conn_id, MRC_RU_CH_OPEN, &sys_use, 0, 0);
//					}
                }

            }
            else
            {
                rf_dc_set(sys_use, (PORT_CTL_E)t_rf_sys[sys_use].t_public.dc_switch);
//                for(i = 0; i < BLE_CONNECT_MAX_NUM; i++)
//                {
//                    if(ru_device_list[i].dev_type == DEV_TYPE_RU)
//                        mrc_set_ru_param(ru_device_list[i].ble_conn_id, MRC_RU_CH_CLOSE, &band_use, 0, 0);
//                }
            }
            break;

        case TM_ID_RF_OPEN_CLOSE_SWITCH:		//0x0004	射频开启 / 关闭
			res = tm_pdu_data_rw(TM_RW, cmd, pt, &t_rf_sys[sys_use].t_public.tRfSwitch.ucNormalState, TM_UINT1, 1, 0, 1, &t_save->unsave);
		    temp_uint8 = t_rf_sys[sys_use].t_public.tRfSwitch.ucNormalState;
            if((temp_uint8 <= 1) && (TM_CMD_SET == cmd))
            {
                // 5G
                if((BAND_41 == band_use) || (BAND_41M == band_use))
                {
					#if LOCAL_MODULE_TYPE == _MU
                    rf_set_modem_cmd(sys_use, temp_uint8, IS_TDD);
					#else
                    tm_sys_fpga_switch(IS_5G, IS_TDD, (RF_SWITCH_E)temp_uint8);  // 5G时序
					#endif
                }
                else if((BAND_39 == band_use) || (BAND_40 == band_use)) // 4G TDD
                {
					#if LOCAL_MODULE_TYPE == _MU
                    // 需要添加主机4G时序设置命令
//                    rf_set_modem_cmd(sys_use, 0, IS_FDD);
//                    hi_delay_ms(200);
                    rf_set_modem_cmd(sys_use, temp_uint8, IS_TDD);
					#else
                    tm_sys_fpga_switch(IS_4G, IS_TDD, (RF_SWITCH_E)temp_uint8);  // 4G时序
					#endif
                    tm_sys_fpga_4g_pa((RF_SWITCH_E)temp_uint8);                  // 4G PA
                }
                else    // 4G FDD
                {
					#if LOCAL_MODULE_TYPE == _MU
					rf_set_modem_cmd(sys_use, temp_uint8, IS_FDD);
					#else
					tm_sys_fpga_switch(IS_4G, IS_FDD, (RF_SWITCH_E)temp_uint8);  // 4G时序
					#endif
					tm_sys_fpga_4g_pa((RF_SWITCH_E)temp_uint8);                  // 4G PA
                }
				
				for(i = 0; i < BLE_CONNECT_MAX_NUM; i++)
                {
                    if(ru_device_list[i].dev_type == DEV_TYPE_RU)
					{
						if( temp_uint8 == 0x01 )
							mrc_set_ru_param(ru_device_list[i].ble_conn_id, MRC_RU_CH_OPEN, &sys_use, 0, 0);
						else
							mrc_set_ru_param(ru_device_list[i].ble_conn_id, MRC_RU_CH_CLOSE, &sys_use, 0, 0);
					}
                }
				
				if( RF_SWITCH_CLOSE == temp_uint8 )
				{
					t_rf_sys[sys_use].t_public.dc_switch = CLOSE;
					rf_dc_set(sys_use, (PORT_CTL_E)t_rf_sys[sys_use].t_public.dc_switch);
				}
            }
            break;
/*			
            if(TM_CMD_SET == cmd)
            {
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &t_rf_sys[sys_use].t_public.tRfSwitch.ucNormalState, TM_UINT1, 1, 0, 1, &t_save->cali_rf);
                if((BAND_41 == band_use) || (BAND_41M == band_use)) // 5G TDD
                {
					#if LOCAL_MODULE_TYPE == _MU
                    // 主机添加时序控制命令
					#else
                    tm_sys_fpga_switch(IS_5G, IS_TDD, (RF_SWITCH_E)t_rf_sys[sys_use].t_public.tRfSwitch.ucNormalState);  // 从机5G时序
					#endif
                }
                else if((BAND_39 == band_use) || (BAND_40 == band_use)) // 4G TDD
                {
					#if LOCAL_MODULE_TYPE == _MU
                    // 主机添加时序控制命令
					#else
                    tm_sys_fpga_switch(IS_4G, IS_TDD, (RF_SWITCH_E)t_rf_sys[sys_use].t_public.tRfSwitch.ucNormalState);  // 从机4G时序
					#endif

                    tm_sys_fpga_4g_pa((RF_SWITCH_E)t_rf_sys[sys_use].t_public.tRfSwitch.ucNormalState);                  // 4G PA
                }
                else    // 4G FDD
                {
					#if LOCAL_MODULE_TYPE == _MU
                    // 主机添加时序控制命令
					#else
                    tm_sys_fpga_switch(IS_4G, IS_FDD, (RF_SWITCH_E)t_rf_sys[sys_use].t_public.tRfSwitch.ucNormalState);  // 从机4G时序
					#endif

                    tm_sys_fpga_4g_pa((RF_SWITCH_E)t_rf_sys[sys_use].t_public.tRfSwitch.ucNormalState);                  // 4G PA
                }
            }
            else
            {
                temp_uint8 = t_rf_sys[sys_use].t_public.tRfSwitch.ucNormalState;
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint8, TM_UINT1, 1, 0, 1, &t_save->cali_rf);
            }
            break;
*/

        case TM_ID_RF_UP_DOWN_SWITCH:	 		//0x0005	上行单通 / 下行单通
            if(TM_CMD_SET == cmd)
            {
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &t_rf_sys[sys_use].t_public.tRfSwitch.ucTestState, TM_UINT1, 1, 0, 1, &t_save->unsave);
                rf_ch_lna_att_default_set(band_use, t_rf_sys[sys_use].t_public.tRfSwitch.ucTestState);

                // 5G
                if((BAND_41 == band_use) || (BAND_41M == band_use))
                {
                    if(0 == t_rf_sys[sys_use].t_public.tRfSwitch.ucTestState)         //上行单通
                    {
						#if LOCAL_MODULE_TYPE == _MU
                        // 需要添加主机5G时序设置命令
						#else
                        tm_sys_fpga_switch(IS_5G, IS_TDD, RF_SWITCH_UP);  // 从机5G时序
						#endif
                    }
                    else if(1 == t_rf_sys[sys_use].t_public.tRfSwitch.ucTestState)    //下行单通
                    {
						#if LOCAL_MODULE_TYPE == _MU
                        // 需要添加主机5G时序设置命令
						#else
                        tm_sys_fpga_switch(IS_5G, IS_TDD, RF_SWITCH_DW);  // 从机5G时序
						#endif
                    }
                }
                else if((BAND_39 == band_use) || (BAND_40 == band_use)) // 4G TDD
                {
                    if(0 == t_rf_sys[sys_use].t_public.tRfSwitch.ucTestState)         //上行单通
                    {
						#if LOCAL_MODULE_TYPE == _MU
                        // 需要添加主机4G时序设置命令
						#else
                        tm_sys_fpga_switch(IS_4G, IS_TDD, RF_SWITCH_UP);  // 从机4G时序
						#endif
                        tm_sys_fpga_4g_pa(RF_SWITCH_UP);                  // 4G PA
                    }
                    else if(1 == t_rf_sys[sys_use].t_public.tRfSwitch.ucTestState)    //下行单通
                    {
						#if LOCAL_MODULE_TYPE == _MU
                        // 需要添加主机4G时序设置命令
						#else
                        tm_sys_fpga_switch(IS_4G, IS_TDD, RF_SWITCH_DW);  // 从机4G时序
						#endif
                        tm_sys_fpga_4g_pa(RF_SWITCH_DW);                  // 4G PA
                    }
                }
            }
            else
            {
                temp_uint8 = t_rf_sys[sys_use].t_public.tRfSwitch.ucTestState;
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint8, TM_UINT1, 1, 0, 1, &t_save->unsave);
            }
            break;


        case TM_ID_RF_LTE_NR_SWITCH:					//0x0006	LTE/NR选择, 用于测试时的数据保存分组
			   temp_uint8 = get_band_net_mode( (RF_BAND_E)band_use );
			   res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint8, TM_UINT1, 1, 0, 1, &t_save->unsave);
			   if(TM_CMD_SET == cmd)
			   {
					if( BAND_8 == band_use )
						set_band_net_mode((RF_BAND_E)band_use, LTE);
					else
						set_band_net_mode((RF_BAND_E)band_use,(NET_MODE)temp_uint8);
			   }
			  rtt_printf(RTT_MODEM, "band_net_mode[%d] =%d \r\n",band_use,band_net_mode[band_use]);
			  break;

        case TM_ID_RF_SYNC:						//0x0007    同步状态
			if( band_use == t_rf_sys[sys_use].t_public.valid_band )
				res = tm_pdu_data_rw(TM_R, cmd, pt, &t_rf_sys[sys_use].t_public.sync, TM_UINT1, 1, 0, 1, &t_save->unsave);
			else
			{
				temp_uint8 = UNSYNC;
				res = tm_pdu_data_rw(TM_R, cmd, pt, &temp_uint8, TM_UINT1, 1, 0, 1, &t_save->unsave);
			}
				
            break;
		
        case TM_ID_RF_BAND:						// 0x0008	频段
            if(WORK_NORMAL == tDevPublicPara.ucWorkMode)	//正常模式
            {
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &t_rf_sys[sys_use].t_public.valid_band, TM_UINT1, 1, 3, 41, &t_save->unsave);
            }
            else
            {
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &band_use, TM_UINT1, 1, 3, 41, &t_save->unsave);
            }
            break;
		
		case TME_ID_RF_BW_SET_FOR_TEST:			// 0x002C   兼容测试模式下的带宽写入
        case TM_ID_RF_BW:						// 0x0009	带宽
			if( TM_CMD_QUERY == cmd )
			{
				if( band_use == t_rf_sys[sys_use].t_public.valid_band )	//是当前有效频段, 正常返回
				{
					 res = tm_pdu_data_rw(TM_RW, cmd, pt, &t_rf_sys[sys_use].t_public.valid_bw, TM_UINT1, 1, 1, 255, &t_save->unsave);
				}
				else	//非有效频段, 返回指令无效
				{
				
				}
			}
			else	//设置
			{
				res = tm_pdu_data_rw(TM_RW, cmd, pt, &bw_use, TM_UINT1, 1, 1, 255, &t_save->unsave);
				bandwidth_net[ band_use ] = bw_use;	//lte 同步 nr波形 测试使用
				if( band_use == t_rf_sys[sys_use].t_public.valid_band )	//是当前有效频段, 正常设置, 如果为测试模式, 则需要重新加载数据
				{
					if( WORK_NORMAL == run_mode )	//正常模式, 如果设置5G BAND需要重载数据, 保存参数, 
					{
						if( sys_use == RF_SYS_1 )
						{
							res = tm_pdu_data_rw( TM_RW, cmd, pt, &t_rf_4g_save_para[band_use].band_width, TM_UINT1, 1, 1, 255, &t_save->dev );
							if( band_use == t_rf_sys[sys_use].t_public.valid_band )
							{
								t_rf_sys[sys_use].t_public.valid_bw = t_rf_4g_save_para[band_use].band_width;
								set_8668_flag = B_TRUE;
								reload_para   = B_TRUE;	//加载数据
								flag_fpga_cmd_set_4g_bandwidth();
							}
						}
						else
						{
							res = tm_pdu_data_rw( TM_RW, cmd, pt, &t_rf_sys[sys_use].t_public.valid_bw, TM_UINT1, 1, 1, 255, &t_save->dev );
							//t_rf_sys[sys_use].t_public.valid_bw = bw_use;
							set_8668_flag = B_TRUE;
							reload_para = B_TRUE;	//加载数据
							//rescan_flag = B_TRUE;	//重新扫描数据
							flag_fpga_cmd_set_5g_bw();
						}
					}
					else if( WORK_PL == run_mode )	//测试模式, 需要重载数据, 不保存
					{
						t_rf_sys[sys_use].t_public.valid_bw = bw_use;
						set_8668_flag = B_TRUE;
						reload_para = B_TRUE;	//加载数据
						if( sys_use == RF_SYS_1 )
						{
							flag_fpga_cmd_set_4g_bandwidth();
						}
						else
						{
							flag_fpga_cmd_set_5g_bw();
						}
					}
					else	//校准模式, 不需要重载数据, 不保存
					{
						t_rf_sys[sys_use].t_public.valid_bw = bw_use;
						bw_use = t_rf_sys[sys_use].t_public.valid_bw;
						set_8668_flag = B_TRUE;
						if( sys_use == RF_SYS_1 )
							flag_fpga_cmd_set_4g_bandwidth();
						else
							flag_fpga_cmd_set_5g_bw();
					}
					
					
					if( B_TRUE == reload_para )	
					{
						rf_run_start_init();	//此时更新, bw数据尚未保存到FLASH, 需要重新重新写入
						t_rf_sys[sys_use].t_public.valid_bw = bw_use;
					}
					
					if( B_TRUE == set_8668_flag )
					{
					    BSP_8668BandWidthSet(pt_866x_sys[ sys_use ], (uint32_t)t_rf_sys[sys_use].t_public.valid_bw);
						att_set_force_fresh( sys_use );
						rtt_printf(RTT_RF, "oid = %d, 8668_BW_SET = %d MHz\n", TM_ID_RF_BW, t_rf_sys[sys_use].t_public.valid_bw);
					}
				}
				else	//非有效频段, 返回指令无效
				{
//					res = tm_pdu_data_rw(TM_RW, cmd, pt, &bw_use, TM_UINT1, 1, 1, 255, &t_save->unsave);
				}
			}
            break;

        case TM_ID_RF_FRQ_POINT: 				// 0x000A	频点
			
			break;
		

		case TM_ID_RF_CENTER_FREQ:				// 0x000B  中心频率
			if( TM_CMD_QUERY == cmd )
			{
				if( band_use == t_rf_sys[sys_use].t_public.valid_band )	//是当前有效频段, 正常返回
				{
					 res = tm_pdu_data_rw(TM_RW, cmd, pt, &t_rf_sys[sys_use].t_public.valid_freq, TM_UINT2, 2, 1, 100000, &t_save->unsave);
				}
				else	//非有效频段, 返回指令无效
				{
					
				}
			}
			else	//设置
			{
				res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint32, TM_UINT2, 2, 1, 100000, &t_save->unsave);
				
				if( band_use == t_rf_sys[sys_use].t_public.valid_band )	//是当前有效频段, 正常设置, 如果为测试模式, 则需要重新加载数据
				{
					if( WORK_NORMAL == run_mode )	//正常模式, 如果设置5G BAND需要重载数据, 保存参数, 
					{
						if( sys_use == RF_SYS_1 )
						{
							res = tm_pdu_data_rw( TM_RW, cmd, pt, &t_rf_4g_save_para[band_use].freq, TM_UINT2, 2, 1, 100000, &t_save->dev );
							t_rf_sys[sys_use].t_public.valid_freq = t_rf_4g_save_para[band_use].freq;
							flag_fpga_cmd_set_4g_center_freq();
						}
						else
						{
							res = tm_pdu_data_rw( TM_RW, cmd, pt, &t_rf_sys[RF_SYS_2].t_public.valid_freq, TM_UINT2, 2, 1, 100000, &t_save->dev );
							t_rf_sys[RF_SYS_3].t_public.valid_freq = t_rf_sys[RF_SYS_2].t_public.valid_freq;
							flag_fpga_cmd_set_5g_center_freq();
						}
						BSP_866xRxFreqSet(pt_866x_sys[ sys_use ], pt_866x_sys[ sys_use ]->ch, temp_uint32);
						
					}
					else	//测试模式, 需要重载数据, 不保存
					{
						t_rf_sys[sys_use].t_public.valid_freq = temp_uint32;
					}
				}
				else	//非有效频段, 返回指令无效
				{
				}
			}
			break;	

			
		case TM_ID_RF_SCAN_FREQ_POINT:			// 0x000C  扫描频点
			if( RF_SYS_1 == sys_use )
			{
				res = tm_pdu_data_rw(TM_RW, cmd, pt, (void *)t_rf_4g_save_para[band_use].scan_freq, TM_STR, 64, 0, 0, &t_save->dev);
				if( band_use == t_rf_sys[sys_use].t_public.valid_band )
				{
					memcpy( t_rf_sys[sys_use].t_public.scan_freq, t_rf_4g_save_para[band_use].scan_freq, 64 );
				}
			}
			else
			{
				res = tm_pdu_data_rw(TM_RW, cmd, pt, (void *)t_rf_sys[sys_use].t_public.scan_freq, TM_STR, 64, 0, 0, &t_save->dev);
			}
			break;
		
		case TM_ID_RF_EXIST:					//0x000D   通道存在状态
			if( sys_use == RF_SYS_1 )
			{
				res = tm_pdu_data_rw(TM_RW, cmd, pt, &t_rf_4g_save_para[band_use].exist, TM_UINT1, 1, 0, 1, &t_save->dev);	
				if( band_use == t_rf_sys[sys_use].t_public.valid_band )
					t_rf_sys[sys_use].t_public.exist = t_rf_4g_save_para[band_use].exist;
			}
			else
			{
				res = tm_pdu_data_rw(TM_RW, cmd, pt, &t_rf_sys[sys_use].t_public.exist, TM_UINT1, 1, 0, 1, &t_save->dev);	
			}
			break;
		
        case TM_ID_RF_FPGA_SWITCH_SET:			//0x000F    通过FPGA设置MODEM开关状态
            res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint8, TM_UINT1, 1, 0, 3, &t_save->unsave);
            if((temp_uint8 <= 3) && (TM_CMD_SET == cmd))
            {
                // 5G
                if((BAND_41 == band_use) || (BAND_41M == band_use))
                {
					#if LOCAL_MODULE_TYPE == _MU
                    // 需要添加主机5G时序设置命令
                    rf_set_modem_cmd(sys_use, temp_uint8, IS_TDD);
					#else
                    tm_sys_fpga_switch(IS_5G, IS_TDD, (RF_SWITCH_E)temp_uint8);  // 5G时序
					#endif

                    if(2 == temp_uint8)         //上行单通
                    {
                        rf_ch_lna_att_default_set(band_use, RF_CH_UP);
                    }
                    else if(3 == temp_uint8)    //下行单通
                    {
                        rf_ch_lna_att_default_set(band_use, RF_CH_DW);
                    }
                }
                else if((BAND_39 == band_use) || (BAND_40 == band_use)) // 4G TDD
                {
					#if LOCAL_MODULE_TYPE == _MU
                    // 需要添加主机4G时序设置命令
//                    rf_set_modem_cmd(sys_use, 0, IS_FDD);
//                    hi_delay_ms(200);
                    rf_set_modem_cmd(sys_use, temp_uint8, IS_TDD);
					#else
                    tm_sys_fpga_switch(IS_4G, IS_TDD, (RF_SWITCH_E)temp_uint8);  // 4G时序
					#endif
                    tm_sys_fpga_4g_pa((RF_SWITCH_E)temp_uint8);                  // 4G PA

                    if(2 == temp_uint8)         //上行单通
                    {
                        rf_ch_lna_att_default_set(band_use, RF_CH_UP);
                    }
                    else if(3 == temp_uint8)    //下行单通
                    {
                        rf_ch_lna_att_default_set(band_use, RF_CH_DW);
                    }
                }
                else    // 4G FDD
                {
                    if(2 > temp_uint8)  // 0关1开
                    {
						#if LOCAL_MODULE_TYPE == _MU
                        rf_set_modem_cmd(sys_use, temp_uint8, IS_FDD);
						#else
                        tm_sys_fpga_switch(IS_4G, IS_FDD, (RF_SWITCH_E)temp_uint8);  // 4G时序
						#endif
                        tm_sys_fpga_4g_pa((RF_SWITCH_E)temp_uint8);                  // 4G PA
                    }
                }
            }
            break;


        case TM_ID_RF_FRAME:					// 0x0010	5G上下行配比
			res = tm_pdu_data_rw(TM_RW, cmd, pt, &frame_net[band_use], TM_UINT1, 1, 0, 3, &t_save->dev);
		
		
			res = tm_pdu_data_rw(TM_RW, cmd, pt, &t_5g_modem_config.frame, TM_UINT1, 1, 1, 6, &t_save->dev);
			frame_net[ band_use ] = t_5g_modem_config.frame;
			if( (TM_CMD_SET == cmd) &&( t_5g_modem_config.frame>0 ) && ( t_5g_modem_config.frame<=6 ) )
			{
				frame_cfg_flag = B_TRUE;
				if((B_TRUE==frame_cfg_flag) && (B_TRUE==spframe_cfg_flag))
				{
					rf_5g_modem_switch( (RF_BAND_E)t_rf_sys[ RF_SYS_2 ].t_public.valid_band, RF_SWITCH_OPEN );
					frame_cfg_flag = B_FALSE;
					spframe_cfg_flag = B_FALSE;
				}
			}
            break;

        case TM_ID_RF_SPECIAL_FRAME:			// 0x0011	5G特殊子帧配置
			res = tm_pdu_data_rw(TM_RW, cmd, pt, &t_5g_modem_config.specialTime, TM_UINT1, 1, 1, 3, &t_save->dev);
			if( (TM_CMD_SET == cmd) &&( t_5g_modem_config.specialTime>0 ) && ( t_5g_modem_config.specialTime<=3 ) )
			{
				spframe_cfg_flag = B_TRUE;
				if((B_TRUE==frame_cfg_flag) && (B_TRUE==spframe_cfg_flag))
				{
					rf_5g_modem_switch( (RF_BAND_E)t_rf_sys[ RF_SYS_2 ].t_public.valid_band, RF_SWITCH_OPEN );
					frame_cfg_flag = B_FALSE;
					spframe_cfg_flag = B_FALSE;
				}
			}
            break;

        case TM_ID_RF_FRAME_TIME_OFFSET:		// 0x0012	5G帧头时间调整量

            break;

        case TM_ID_RF_FIRST_TIME_OFFSET:		// 0x0013	第一转换点的提前/滞后量

            break;

        case TM_ID_RF_SECOND_TIME_OFFSET:		// 0x0014	第二转换点的提前/滞后量

            break;
		
		case TM_ID_RF_SSB:						// 0x0015   SSB频点
			
			break;
		
		case TM_ID_RF_LTE_FRAME:				//0x0016  LTE 上下行配比
			res = tm_pdu_data_rw(TM_RW, cmd, pt, &frame_net[band_use], TM_UINT1, 1, 0, 2, &t_save->unsave);
			break;

		case TM_ID_RF_LTE_SPECIAL_FRAME:		//0x0017  LTE 特殊子帧配置
			res = tm_pdu_data_rw(TM_RW, cmd, pt, &sp_frame_net[band_use], TM_UINT1, 1, 0, 8, &t_save->unsave);
			break;

		case TM_ID_RF_UL_RISING_EDGE_OFFSET:	//0x0018   UP上升沿偏移量
			
			break;

		case TM_ID_RF_UL_FALLING_EDGE_OFFSET:	//0x0019   UP下降沿偏移量
			
			break;

		case TM_ID_RF_DL_RISING_EDGE_OFFSET:	//0x001A   DW上升沿偏移量
			
			break;

		case TM_ID_RF_DL_FALLING_EDGE_OFFSET:	//0x001B   DW下降沿偏移量
			
			break;
		
		
        case TM_ID_RF_UP_THEO_GAIN:				// 0x0020	上行理论增益
            if(tDevPublicPara.ucWorkMode == WORK_CALIB)
            {
                tm_copy_cali_rf_para(cmd, band_use, pt, &t_tm_rf->rf.up_theo_gain, 1);
            }
            else
            {
                temp_int8 = (DEV_M_ID == dev_id) ? t_rf_sys[ sys_use ].t_ch[RF_CH_UP].tGain.scTheoreticalGain : 0;
                *pst_dat = temp_int8;
            }
            break;

        case TM_ID_RF_DW_THEO_GAIN:				// 0x0021	下行理论增益
            if(tDevPublicPara.ucWorkMode == WORK_CALIB)
            {
                tm_copy_cali_rf_para(cmd, band_use, pt, &t_tm_rf->rf.dw_theo_gain, 1);
            }
            else
            {
                temp_int8 = (DEV_M_ID == dev_id) ? t_rf_sys[ sys_use ].t_ch[RF_CH_DW].tGain.scTheoreticalGain : 0;
                *pst_dat = temp_int8;
            }
            break;

        case TM_ID_RF_UP_THEO_OUT_POW:			// 0x0022	上行理论输出功率
            if(tDevPublicPara.ucWorkMode == WORK_CALIB)
            {
                tm_copy_cali_rf_para(cmd, band_use, pt, &t_tm_rf->rf.up_theo_out_pow, 1);
            }
            else
            {
                temp_int8 = (DEV_M_ID == dev_id) ? t_rf_sys[ sys_use ].t_ch[RF_CH_UP].tPow.scTheoOutPow : 0;
                *pst_dat = temp_int8;
            }
            break;

        case TM_ID_RF_DW_THEO_OUT_POW:			// 0x0023	下行理论输出功率
            if(tDevPublicPara.ucWorkMode == WORK_CALIB)
            {
                tm_copy_cali_rf_para(cmd, band_use, pt, &t_tm_rf->rf.dw_theo_out_pow, 1);
            }
            else
            {
                temp_int8 = (DEV_M_ID == dev_id) ? t_rf_sys[ sys_use ].t_ch[RF_CH_DW].tPow.scTheoOutPow : 0;
                *pst_dat = temp_int8;
            }
            break;

        case TM_ID_RF_UP_ACT_GAIN:				// 0x0024	上行实际增益
            temp_int8 = (DEV_M_ID == dev_id) ? t_rf_sys[ sys_use ].t_ch[RF_CH_UP].tGain.scRealGain : 0;
            *pst_dat = temp_int8;
            break;

        case TM_ID_RF_DW_ACT_GAIN:				// 0x0025	下行实际增益
            temp_int8 = (DEV_M_ID == dev_id) ? t_rf_sys[ sys_use ].t_ch[RF_CH_DW].tGain.scRealGain : 0;
            *pst_dat = temp_int8;
            break;

        case TM_ID_RF_UP_ACT_OUT_POW:			// 0x0026	上行实际输出功率
            temp_int8 = (DEV_M_ID == dev_id) ? t_rf_sys[ sys_use ].t_ch[RF_CH_UP].tPow.scOutPow : 0;
            *pst_dat = temp_int8;
            break;

        case TM_ID_RF_DW_ACT_OUT_POW:			// 0x0027	下行实际输出功率
            temp_int8 = (DEV_M_ID == dev_id) ? t_rf_sys[ sys_use ].t_ch[RF_CH_DW].tPow.scOutPow : 0;
            *pst_dat = temp_int8;
            break;

        case TM_ID_RF_UP_ACT_IN_POW: 			// 0x0028	上行实际输入功率
            temp_int8 = (DEV_M_ID == dev_id) ? t_rf_sys[ sys_use ].t_ch[RF_CH_UP].tPow.scInPow : 0;
            *pst_dat = temp_int8;
            break;

        case TM_ID_RF_DW_ACT_IN_POW: 			// 0x0029	下行实际输入功率
            temp_int8 = (DEV_M_ID == dev_id) ? t_rf_sys[ sys_use ].t_ch[RF_CH_DW].tPow.scInPow : 0;
            *pst_dat = temp_int8;
            break;

        case TM_ID_RF_UP_EXPECT_IN_POW:			// 0x002A	上行期望接收功率
            *pst_dat = t_rf_sys[ sys_use ].t_public.upExpectRxPow;
            break;

        case TM_ID_RF_OPLC_EN:					// 0x002E	开环工控使能
            res = tm_pdu_data_rw(TM_RW, cmd, pt, &t_rf_sys[sys_use].t_public.opleEn, TM_UINT1, 1, 0, 1, &t_save->dev);
            break;

        case TM_ID_RF_RSRP_EN: 					// 0x002F	RSRP计算使能
            res = tm_pdu_data_rw(TM_RW, cmd, pt, &t_rf_sys[sys_use].t_public.rsrpEn, TM_UINT1, 1, 0, 1, &t_save->dev);
            break;

        case TM_ID_RF_ISO_EN:					// 0x0030	隔离度检测使能
			res = tm_pdu_data_rw(TM_RW, cmd, pt, &t_rf_sys[sys_use].t_public.isoEn, TM_UINT1, 1, 0, 1, &t_save->dev);
            break;

        case TM_ID_RF_WARN_EN: 					// 0x0031	告警使能
            res = tm_pdu_data_rw(TM_RW, cmd, pt, &t_rf_sys[sys_use].t_public.warnEn, TM_UINT1, 1, 0, 1, &t_save->dev);
            break;

        case TM_ID_RF_TC_EN:					// 0x0032	温度补偿使能
            res = tm_pdu_data_rw(TM_RW, cmd, pt, &t_rf_sys[sys_use].t_public.tcEn, TM_UINT1, 1, 0, 1, &t_save->dev);
            break;

        case TM_ID_RF_UP_SILENCE_EN:			// 0x0033	上行静默使能
            res = tm_pdu_data_rw(TM_RW, cmd, pt, &t_rf_sys[sys_use].t_public.silenceEn, TM_UINT1, 1, 0, 1, &t_save->dev);
            break;

        case TM_ID_RF_TEMP_OFFSET:				// 0x0034	温度偏移量
            res = tm_pdu_data_rw(TM_RW, cmd, pt, &t_rf_sys[sys_use].t_public.tmpOffset, TM_SINT1, 1, -30, 30, &t_save->dev);
            break;

        case TM_ID_RF_UP_GAIN_OFFSET:			// 0x0035	上行增益调整偏移量
			if( sys_use == RF_SYS_1 )
			{
				res = tm_pdu_data_rw(TM_RW, cmd, pt, &t_rf_4g_save_para[ band_use ].up_gain_offset, TM_SINT1, 1, -3, 3, &t_save->dev);
				if( band_use == t_rf_sys[RF_SYS_1].t_public.valid_band )
					t_rf_sys[RF_SYS_1].t_ch[RF_CH_UP].tAtt.ucManuCal = t_rf_4g_save_para[ band_use ].up_gain_offset;
			}
			else
				res = tm_pdu_data_rw(TM_RW, cmd, pt, &t_rf_sys[sys_use].t_ch[RF_CH_UP].tAtt.ucManuCal, TM_SINT1, 1, -3, 3, &t_save->dev);
            break;

        case TM_ID_RF_DW_GAIN_OFFSET:			// 0x0036	下行增益调整偏移量
            if( sys_use == RF_SYS_1 )
			{
				res = tm_pdu_data_rw(TM_RW, cmd, pt, &t_rf_4g_save_para[ band_use ].dw_gain_offset, TM_SINT1, 1, -3, 3, &t_save->dev);
				if( band_use == t_rf_sys[RF_SYS_1].t_public.valid_band )
					t_rf_sys[RF_SYS_1].t_ch[RF_CH_DW].tAtt.ucManuCal = t_rf_4g_save_para[ band_use ].dw_gain_offset;
			}
			else
				res = tm_pdu_data_rw(TM_RW, cmd, pt, &t_rf_sys[sys_use].t_ch[RF_CH_DW].tAtt.ucManuCal, TM_SINT1, 1, -3, 3, &t_save->dev);
            break;

        case TM_ID_RF_UP_OUT_POW_OFFSET:		// 0x0037	上行输出功率调整偏移量
			if( sys_use == RF_SYS_1 )
			{
				res = tm_pdu_data_rw(TM_RW, cmd, pt, &t_rf_4g_save_para[ band_use ].up_pow_offset, TM_SINT1, 1, -30, 30, &t_save->dev);
				if( band_use == t_rf_sys[RF_SYS_1].t_public.valid_band )
					t_rf_sys[RF_SYS_1].t_ch[RF_CH_UP].tAlc.scAlcPointOffset = t_rf_4g_save_para[ band_use ].up_pow_offset;
			}
			else
				res = tm_pdu_data_rw(TM_RW, cmd, pt, &t_rf_sys[sys_use].t_ch[RF_CH_UP].tAlc.scAlcPointOffset, TM_SINT1, 1, -30, 30, &t_save->dev);
            break;

        case TM_ID_RF_DW_OUT_POW_OFFSET:		// 0x0038	下行输出功率调整偏移量
			if( sys_use == RF_SYS_1 )
			{
				res = tm_pdu_data_rw(TM_RW, cmd, pt, &t_rf_4g_save_para[ band_use ].dw_pow_offset, TM_SINT1, 1, -30, 30, &t_save->dev);
				if( band_use == t_rf_sys[RF_SYS_1].t_public.valid_band )
					t_rf_sys[RF_SYS_1].t_ch[RF_CH_DW].tAlc.scAlcPointOffset = t_rf_4g_save_para[ band_use ].dw_pow_offset;
			}
			else
				res = tm_pdu_data_rw(TM_RW, cmd, pt, &t_rf_sys[sys_use].t_ch[RF_CH_DW].tAlc.scAlcPointOffset, TM_SINT1, 1, -30, 30, &t_save->dev);
            break;

        case TM_ID_RF_COUPLING_GAIN:    		// 0x0039	同步耦合增益差值
            tm_copy_cali_rf_para(cmd, band_use, pt, &t_tm_rf->rf.coupling_gain, 1);
            break;

        case TM_ID_RF_UP_SILENCE_GATE:			// 0x003A	上行静默门限
            res = tm_pdu_data_rw(TM_RW, cmd, pt, &t_silence_para[sys_use].gate, TM_SINT1, 1, -128, 127, &t_save->cali_rf);
            break;

        case TM_ID_RF_OPLC_POINT:				// 0x003B	开环工控启控点
            res = tm_pdu_data_rw(TM_R, cmd, pt, tOplc[sys_use].pt_pow_point, TM_SINT1, 1, -128, 127, &t_save->unsave);
            break;

        case TM_ID_RF_OPLC_GATE:				// 0x003C	上行开环工控门限
            res = tm_pdu_data_rw(TM_RW, cmd, pt, &tOplc[sys_use].link_gate, TM_SINT1, 1, -128, 127, &t_save->cali_rf);
            break;

        case TM_ID_RF_OPLC_GAIN_LINK_VALUE:		// 0x003D	上行开环工控增益联动差值
            res = tm_pdu_data_rw(TM_RW, cmd, pt, &tOplc[sys_use].gain_offset, TM_SINT1, 1, -128, 127, &t_save->cali_rf);
            break;

        case TM_ID_RF_OPLC_POW_LIN_VALUE:		// 0x003E	上行开环工控功率联动差值
            res = tm_pdu_data_rw(TM_RW, cmd, pt, &tOplc[sys_use].pow_offset, TM_SINT1, 1, -128, 127, &t_save->cali_rf);
            break;

        case TM_ID_RF_UP_ADC1_VALUE: 			// 0x003F	上行ADC1读取
            temp_uint16 = get_ch_adc_value(sys_use, RF_CH_UP, ADC_VALUE_TYPE_ALC);
            res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint16, TM_UINT2, 2, 0, 4095, &t_save->unsave);
            break;

        case TM_ID_RF_UP_ADC2_VALUE:			// 0x0040	上行ADC2读取
            temp_uint16 = 0x00;
            res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint16, TM_UINT2, 2, 0, 4095, &t_save->unsave);
            break;

        case TM_ID_RF_DW_ADC1_VALUE: 			// 0x0041	下行ADC1读取
            temp_uint16 = get_ch_adc_value(sys_use, RF_CH_DW, ADC_VALUE_TYPE_ALC);
            res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint16, TM_UINT2, 2, 0, 4095, &t_save->unsave);
            break;

        case TM_ID_RF_DW_ADC2_VALUE: 			// 0x0042	下行ADC2读取
            temp_uint16 = 0x00;
            res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint16, TM_UINT2, 2, 0, 4095, &t_save->unsave);
            break;

        case TM_ID_RF_UP_OUT_POW1: 				// 0x0043	上行输出功率1
            temp_int8 = get_ch_pow_for_alc(sys_use, RF_CH_UP);
            res = tm_pdu_data_rw(TM_R, cmd, pt, &temp_int8, TM_SINT1, 1, -128, 127, &t_save->unsave);
            break;

        case TM_ID_RF_UP_OUT_POW2: 				// 0x0044	上行输出功率2

            break;

        case TM_ID_RF_DW_OUT_POW1: 				// 0x0045	下行输出功率1
            temp_int8 = get_ch_pow_for_alc(sys_use, RF_CH_DW);
            res = tm_pdu_data_rw(TM_R, cmd, pt, &temp_int8, TM_SINT1, 1, -128, 127, &t_save->unsave);
            break;

        case TM_ID_RF_DW_OUT_POW2: 				// 0x0046	下行输出功率2
            temp_int8 = get_ch_pow_for_8668(sys_use, RF_CH_DW);
            res = tm_pdu_data_rw(TM_R, cmd, pt, &temp_int8, TM_SINT1, 1, -128, 127, &t_save->unsave);
            break;

        case TM_ID_RF_UP_LNA_ATT_VALUE1: 		// 0x0047	上行LNA1 ATT值
            //res = tm_pdu_data_rw(TM_RW, cmd, pt, &pt_ch_att[sys_use][RF_CH_UP]->lna1.max_num, TM_UINT1, 1, 0, 50, &t_save->dev);
            tm_copy_cali_rf_para(cmd, band_use, pt, &t_tm_rf->rf.up_lna_att1, 1);
            break;

        case TM_ID_RF_UP_LNA_ATT_VALUE2: 		// 0x0048	上行LNA2 ATT值
            //res = tm_pdu_data_rw(TM_RW, cmd, pt, &pt_ch_att[sys_use][RF_CH_UP]->lna2.max_num, TM_UINT1, 1, 0, 50, &t_save->dev);
            tm_copy_cali_rf_para(cmd, band_use, pt, &t_tm_rf->rf.up_lna_att2, 1);
            break;

        case TM_ID_RF_DW_LNA_ATT_VALUE1: 		// 0x0049	下行LNA1 ATT值
            //res = tm_pdu_data_rw(TM_RW, cmd, pt, &pt_ch_att[sys_use][RF_CH_DW]->lna1.max_num, TM_UINT1, 1, 0, 50, &t_save->dev);
            tm_copy_cali_rf_para(cmd, band_use, pt, &t_tm_rf->rf.dw_lna_att1, 1);
            break;

        case TM_ID_RF_DW_LNA_ATT_VALUE2: 		// 0x004A	下行LNA2 ATT值
            //res = tm_pdu_data_rw(TM_RW, cmd, pt, &pt_ch_att[sys_use][RF_CH_DW]->lna2.max_num, TM_UINT1, 1, 0, 50, &t_save->dev);
            tm_copy_cali_rf_para(cmd, band_use, pt, &t_tm_rf->rf.dw_lna_att2, 1);
            break;

        case TM_ID_RF_UP_LNA_SWITCH1: 			// 0x004B	上行LNA1状态设置
            res = tm_pdu_data_rw(TM_RW, cmd, pt, &pt_ch_att[sys_use][RF_CH_UP]->lna1.new_value, TM_UINT1, 1, 0, 1, &t_save->unsave);
            if(TM_CMD_SET == cmd)
                lna_set(sys_use, RF_CH_UP, 1, *pst_dat);
            break;

        case TM_ID_RF_UP_LNA_SWITCH2: 			// 0x004C	上行LNA2状态设置
            res = tm_pdu_data_rw(TM_RW, cmd, pt, &pt_ch_att[sys_use][RF_CH_UP]->lna2.new_value, TM_UINT1, 1, 0, 1, &t_save->unsave);
            if(TM_CMD_SET == cmd)
                lna_set(sys_use, RF_CH_UP, 2, *pst_dat);
            break;

        case TM_ID_RF_DW_LNA_SWITCH1: 			// 0x004D	下行LNA1状态设置
            res = tm_pdu_data_rw(TM_RW, cmd, pt, &pt_ch_att[sys_use][RF_CH_DW]->lna1.new_value, TM_UINT1, 1, 0, 1, &t_save->unsave);
            if(TM_CMD_SET == cmd)
                lna_set(sys_use, RF_CH_DW, 1, *pst_dat);
            break;

        case TM_ID_RF_DW_LNA_SWITCH2: 			// 0x004E	下行LNA2状态设置
            res = tm_pdu_data_rw(TM_RW, cmd, pt, &pt_ch_att[sys_use][RF_CH_DW]->lna2.new_value, TM_UINT1, 1, 0, 1, &t_save->unsave);
            if(TM_CMD_SET == cmd)
                lna_set(sys_use, RF_CH_DW, 2, *pst_dat);
            break;

        case TM_ID_RF_UP_PA_SWITCH1: 			// 0x004F	上行功放开关设置1
            res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint8, TM_UINT1, 1, 0, 1, &t_save->unsave);
            if((RF_SYS_1 == sys_use) && (TM_CMD_SET == cmd))
            {
                if(1 == temp_uint8)
                    fpga_4g_ul_pa_on();
                else
                    fpga_4g_ul_pa_off();
            }
            break;

        case TM_ID_RF_UP_PA_SWITCH2: 			// 0x0050	上行功放开关设置2

            break;

        case TM_ID_RF_DW_PA_SWITCH1: 			// 0x0051	下行功放开关设置1
            res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint8, TM_UINT1, 1, 0, 1, &t_save->unsave);
            if((RF_SYS_1 == sys_use) && (TM_CMD_SET == cmd))
            {
                if(1 == temp_uint8)
                    fpga_4g_dl_pa_on();
                else
                    fpga_4g_dl_pa_off();
            }
            break;

        case TM_ID_RF_DW_PA_SWITCH2: 			// 0x0052	下行功放开关设置2

            break;

        case TM_ID_RF_UP_ANALOG_ATT_VALUE1: 	// 0x0053	上行模拟衰减1设置
            res = tm_pdu_data_rw(TM_RW, cmd, pt, &pt_ch_att[sys_use][RF_CH_UP]->analog_att1.new_value, TM_UINT1, 1, 0, 80, &t_save->unsave);
            break;

        case TM_ID_RF_UP_ANALOG_ATT_VALUE2: 	// 0x0054	上行模拟衰减2设置
            res = tm_pdu_data_rw(TM_RW, cmd, pt, &pt_ch_att[sys_use][RF_CH_UP]->analog_att2.new_value, TM_UINT1, 1, 0, 80, &t_save->unsave);
            break;

        case TM_ID_RF_DW_ANALOG_ATT_VALUE1: 	// 0x0055	下行模拟衰减1设置
            res = tm_pdu_data_rw(TM_RW, cmd, pt, &pt_ch_att[sys_use][RF_CH_DW]->analog_att1.new_value, TM_UINT1, 1, 0, 80, &t_save->unsave);
            break;

        case TM_ID_RF_DW_ANALOG_ATT_VALUE2: 	// 0x0056	下行模拟衰减2设置
            res = tm_pdu_data_rw(TM_RW, cmd, pt, &pt_ch_att[sys_use][RF_CH_DW]->analog_att2.new_value, TM_UINT1, 1, 0, 80, &t_save->unsave);
            break;

        case TM_ID_RF_UP_DIGIT_ATT_VALUE1: 		// 0x0057	上行数字衰减器1设置

            break;

        case TM_ID_RF_UP_DIGIT_ATT_VALUE2: 		// 0x0058	上行数字衰减器2设置

            break;

        case TM_ID_RF_DW_DIGIT_ATT_VALUE1: 		// 0x0059	下行数字衰减器1设置

            break;

        case TM_ID_RF_DW_DIGIT_ATT_VALUE2: 		// 0x005A	下行数字衰减器2设置

            break;

        case TM_ID_RF_UP_ANALOG_ATT_PWM_VALUE1: // 0x005B	上行模拟衰减1采集
            res = tm_pdu_data_rw(TM_RW, cmd, pt, &pwm_value, TM_UINT2, 2, 0, 999, &t_save->unsave);
            if(TM_CMD_SET == cmd)
                att_analog_pwm_set(sys_use, RF_CH_UP, 1, pwm_value);
            break;

        case TM_ID_RF_UP_ANALOG_ATT_PWM_VALUE2: // 0x005C	上行模拟衰减2采集
            res = tm_pdu_data_rw(TM_RW, cmd, pt, &pwm_value, TM_UINT2, 2, 0, 999, &t_save->unsave);
            if(TM_CMD_SET == cmd)
                att_analog_pwm_set(sys_use, RF_CH_UP, 2, pwm_value);
            break;

        case TM_ID_RF_DW_ANALOG_ATT_PWM_VALUE1: // 0x005D	下行模拟衰减1采集
            res = tm_pdu_data_rw(TM_RW, cmd, pt, &pwm_value, TM_UINT2, 2, 0, 999, &t_save->unsave);
            if(TM_CMD_SET == cmd)
                att_analog_pwm_set(sys_use, RF_CH_DW, 1, pwm_value);
            break;

        case TM_ID_RF_DW_ANALOG_ATT_PWM_VALUE2: // 0x005E	下行模拟衰减2采集
            res = tm_pdu_data_rw(TM_RW, cmd, pt, &pwm_value, TM_UINT2, 2, 0, 999, &t_save->unsave);
            if(TM_CMD_SET == cmd)
                att_analog_pwm_set(sys_use, RF_CH_DW, 2, pwm_value);
            break;

        case TM_ID_RF_UP_8668_ADC_VALUE: 		//0x005F  8668上行ADC采集值
            temp_uint16 = get_ch_adc_value(sys_use, RF_CH_UP, ADC_VALUE_TYPE_8668);
            res = tm_pdu_data_rw(TM_R, cmd, pt, &temp_uint16, TM_UINT2, 2, 0, 4095, &t_save->unsave);
            break;

        case TM_ID_RF_DW_8668_ADC_VALUE: 		//0x0060  8668下行ADC读取
            temp_uint16 = get_ch_adc_value(sys_use, RF_CH_DW, ADC_VALUE_TYPE_8668);
            res = tm_pdu_data_rw(TM_R, cmd, pt, &temp_uint16, TM_UINT2, 2, 0, 4095, &t_save->unsave);
            break;

        case TM_ID_RF_UP_8668_OUT_POWER: 		//0x0061  8668上行输出功率
            temp_int8 = get_ch_pow_for_8668(sys_use, RF_CH_UP);
            res = tm_pdu_data_rw(TM_R, cmd, pt, &temp_int8, TM_SINT1, 1, -128, 127, &t_save->unsave);
            break;

        case TM_ID_RF_DW_8668_OUT_POWER:	 	//0x0062  8668下行输出功率
            temp_int8 = get_ch_pow_for_8668(sys_use, RF_CH_UP);
            res = tm_pdu_data_rw(TM_R, cmd, pt, &temp_int8, TM_SINT1, 1, -128, 127, &t_save->unsave);
            break;

        case TM_ID_RF_WARN_EN_UP_INPOW_UNDER:  	// 0x0063	上行入过功率告警使能

            break;

        case TM_ID_RF_WARN_EN_DW_INPOW_UNDER:  	// 0x0064	下行入过功率告警使能

            break;

        case TM_ID_RF_WARN_EN_UP_INPOW_OVER:   	// 0x0065	上行入欠功率告警使能

            break;

        case TM_ID_RF_WARN_EN_DW_INPOW_OVER:   	// 0x0066	下行入欠功率告警使能

            break;

        case TM_ID_RF_WARN_EN_UP_OUTPOW_UNDER: 	// 0x0067	上行出过功率告警使能

            break;

        case TM_ID_RF_WARN_EN_DW_OUTPOW_UNDER: 	// 0x0068	下行出过功率告警使能

            break;

        case TM_ID_RF_WARN_EN_UP_OUTPOW_OVER:  	// 0x0069	上行出欠功率告警使能

            break;

        case TM_ID_RF_WARN_EN_DW_OUTPOW_OVER:  	// 0x006A	下行出欠功率告警使能

            break;

        case TM_ID_RF_WARN_EN_UP_ISO:  		 	// 0x006B	上行自激告警使能

            break;

        case TM_ID_RF_WARN_EN_DW_ISO:  		 	// 0x006C	下行自激告警使能

            break;

        case TM_ID_RF_WARN_EN_TMP_OVER: 		// 0x006D	过温度告警使能

            break;

        case TM_ID_RF_WARN_EN_OTHER:		 	// 0x006E	其他告警使能

            break;

        case TM_ID_RF_WARN_GATE_UP_INPOW_UNDER: // 0x006F 	上行入过功率告警门限

            break;

        case TM_ID_RF_WARN_GATE_DW_INPOW_UNDER: // 0x0070 	下行入过功率告警门限

            break;

        case TM_ID_RF_WARN_GATE_UP_INPOW_OVER:  // 0x0071 	上行入欠功率告警门限

            break;

        case TM_ID_RF_WARN_GATE_DW_INPOW_OVER:  // 0x0072 	下行入欠功率告警门限

            break;

        case TM_ID_RF_WARN_GATE_UP_OUTPOW_UNDER:// 0x0073 	上行出过功率告警门限

            break;

        case TM_ID_RF_WARN_GATE_DW_OUTPOW_UNDER:// 0x0074 	下行出过功率告警门限

            break;

        case TM_ID_RF_WARN_GATE_UP_OUTPOW_OVER: // 0x0075 	上行出欠功率告警门限

            break;

        case TM_ID_RF_WARN_GATE_DW_OUTPOW_OVER: // 0x0076 	下行出欠功率告警门限

            break;

        case TM_ID_RF_WARN_GATE_UP_ISO:  		// 0x0077 	上行自激告警门限

            break;

        case TM_ID_RF_WARN_GATE_DW_ISO:  		// 0x0078 	下行自激告警门限

            break;

        case TM_ID_RF_WARN_GATE_TMP_OVER: 	   	// 0x0079 	过温度告警门限

            break;

        case TM_ID_RF_WARN_GATE_OTHER:		   	// 0x007A 	其他告警门限

            break;

        case TM_ID_RF_WARN_UP_INPOW_UNDER:  	// 0x007B	上行入过功率告警

            break;

        case TM_ID_RF_WARN_DW_INPOW_UNDER:  	// 0x007C	下行入过功率告警

            break;

        case TM_ID_RF_WARN_UP_INPOW_OVER:   	// 0x007D	上行入欠功率告警

            break;

        case TM_ID_RF_WARN_DW_INPOW_OVER:   	// 0x007E	下行入欠功率告警

            break;

        case TM_ID_RF_WARN_UP_OUTPOW_UNDER: 	// 0x007F	上行出过功率告警

            break;

        case TM_ID_RF_WARN_DW_OUTPOW_UNDER: 	// 0x0080	下行出过功率告警

            break;

        case TM_ID_RF_WARN_UP_OUTPOW_OVER:  	// 0x0081	上行出欠功率告警

            break;

        case TM_ID_RF_WARN_DW_OUTPOW_OVER:  	// 0x0082	下行出欠功率告警

            break;

        case TM_ID_RF_WARN_UP_ISO:  		  	// 0x0083	上行自激告警

            break;

        case TM_ID_RF_WARN_DW_ISO:  		  	// 0x0084	下行自激告警

            break;

        case TM_ID_RF_WARN_TMP_OVER: 	   	  	// 0x0085	过温度告警

            break;

        case TM_ID_RF_WARN_OTHER:		   	  	// 0x0086	其他告警

            break;

        case TM_ID_RF_UP_OUT_POW_LIST1:			// 0x00C8	上行输出功率表1
            stat_res = get_bwid_by_bw(band_use, bw_use, &group);
            if((STAT_OK == stat_res) && (group < 4))
                tm_copy_cali_rf_para(cmd, band_use, pt, &t_tm_rf->pow_table[group].up1, 192);
            else
                res = TM_ERR_OTHER;
            break;


        case TM_ID_RF_UP_OUT_POW_LIST2:			// 0x00C9	上行输出功率表2
            break;

        case TM_ID_RF_UP_OUT_POW_LIST_8668:		//0x00E2	上行8668输出功率表:
            stat_res = get_bwid_by_bw(band_use, bw_use, &group);
            if((STAT_OK == stat_res) && (group < 4))
                tm_copy_cali_rf_para(cmd, band_use, pt, &t_tm_rf->pow_table[group].up2, 192);
            else
                res = TM_ERR_OTHER;
            break;

        case TM_ID_RF_DW_OUT_POW_LIST1:			// 0x00CA	下行输出功率表1
            stat_res = get_bwid_by_bw(band_use, bw_use, &group);
            if((STAT_OK == stat_res) && (group < 4))
                tm_copy_cali_rf_para(cmd, band_use, pt, &t_tm_rf->pow_table[group].dw1, 192);
            else
                res = TM_ERR_OTHER;
            break;

        case TM_ID_RF_DW_OUT_POW_LIST2:			// 0x00CB	下行输出功率表2
            break;

        case TM_ID_RF_DW_OUT_POW_LIST_8668:		//0x00E5	下行8668输出功率表
            stat_res = get_bwid_by_bw(band_use, bw_use, &group);
            if((STAT_OK == stat_res) && (group < 4))
                tm_copy_cali_rf_para(cmd, band_use, pt, &t_tm_rf->pow_table[group].dw2, 192);
            else
                res = TM_ERR_OTHER;
            break;

        case TM_ID_RF_UP_ANALOG_ATT_LIST1:		// 0x00CC	上行模拟衰减表1
            tm_copy_cali_rf_para(cmd, band_use, pt, &t_tm_rf->att_table.up1, 192);
            break;

        case TM_ID_RF_UP_ANALOG_ATT_LIST2:		// 0x00CD	上行模拟衰减表2
            tm_copy_cali_rf_para(cmd, band_use, pt, &t_tm_rf->att_table.up2, 192);
            break;

        case TM_ID_RF_DW_ANALOG_ATT_LIST1:		// 0x00CE	下行模拟衰减表1
            tm_copy_cali_rf_para(cmd, band_use, pt, &t_tm_rf->att_table.dw1, 192);
            break;

        case TM_ID_RF_DW_ANALOG_ATT_LIST2:		// 0x00CF	下行模拟衰减表2
            tm_copy_cali_rf_para(cmd, band_use, pt, &t_tm_rf->att_table.dw2, 192);
            break;

        case TM_ID_RF_UP_TEMP_LIST:				// 0x00D0	上行温补表
            tm_copy_cali_rf_para(cmd, band_use, pt, (uint8_t *)t_tm_rf->rf.up_tc_table, 32);
            break;

        case TM_ID_RF_DW_TEMP_LIST:				// 0x00D1	下行温补表
            tm_copy_cali_rf_para(cmd, band_use, pt, (uint8_t *)t_tm_rf->rf.dw_tc_table, 32);
            break;

        case TM_ID_8668_SW_RST:					// 0x0101	866x软件复位
            res = tm_pdu_data_rw(TM_W, cmd, pt, &temp_uint8, TM_UINT1, 1, 0, 1, &t_save->unsave);
            if((1 == temp_uint8) && (TM_CMD_SET == cmd))
            {
                BSP_866x_MCU_reset(pt_866x_sys[ sys_use ]);
            }
            break;
		
		case TM_ID_RF_COUPLING_GAIN_START:		//0x00EC 开始同步耦合增益测量	
			 res = tm_pdu_data_rw(TM_W, cmd, pt, &temp_uint8, TM_UINT1, 1, 0, 1, &t_save->unsave);
			 if((1 == temp_uint8) && (TM_CMD_SET == cmd))
             {
                Status_t sta = measure_coupling_gain_in_calib_mode( band_use );
     			res = ( STAT_OK == sta ) ? TM_CMD_ACT_OK : TM_ERR_OTHER;
             }
			break;
		
        case TM_ID_8668_INIT:					// 0x0102	866x初始化
            res = tm_pdu_data_rw(TM_W, cmd, pt, &temp_uint8, TM_UINT1, 1, 0, 1, &t_save->unsave);
            if((1 == temp_uint8) && (TM_CMD_SET == cmd) && (RF_SYS_1 == sys_use) )
            {
                measure_coupling_gain_in_calib_mode( band_use );
            }
            break;

        case TM_ID_8668_REG_ADDRESS:			// 0x0103   待操作寄存器地址
            res = tm_pdu_data_rw(TM_RW, TM_CMD_SET, pt, &temp_uint32, TM_UINT4, 4, 0, 0, &t_save->unsave);
            if(temp_uint32 != 0)
            {
                reg_866x_set_addr(temp_uint32);
            }
            break;

        case TM_ID_8668_ON_PAGE_REG:			// 0x0104	866x页内寄存器读写
            if(TM_CMD_SET == cmd)
            {
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint32, TM_UINT4, 4, 0, 0, &t_save->unsave);
                reg_866x_set_on_page_dat(sys_use, WRITE, temp_uint32);
                reg_866x_rw(&temp_uint32);
            }
            else
            {
                reg_866x_set_on_page_dat(sys_use, READ, 0);
                reg_866x_rw(&temp_uint32);
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint32, TM_UINT4, 4, 0, 0, &t_save->unsave);
            }
            break;

        case TM_ID_8668_OFF_PAGE_REG:			// 0x0105	866x页外寄存器读写
            if(TM_CMD_SET == cmd)
            {
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint32, TM_UINT4, 4, 0, 0, &t_save->unsave);
                reg_866x_set_off_page_dat(sys_use, WRITE, temp_uint32);
                reg_866x_rw(&temp_uint32);
            }
            else
            {
                reg_866x_set_off_page_dat(sys_use, READ, 0);
                reg_866x_rw(&temp_uint32);
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint32, TM_UINT4, 4, 0, 0, &t_save->unsave);
            }
            break;

        case TM_ID_8668_TX_FREQ:				// 0x0106	866xTX频率设置
            if(TM_CMD_SET == cmd)
            {
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint32, TM_UINT4, 4, 0, 0, &t_save->unsave);
                BSP_866xTxFreqSet(pt_866x_sys[ sys_use ], pt_866x_sys[ sys_use ]->ch, temp_uint32);
            }
            else
            {
                temp_uint32 = BSP_866xGetTxFreq(pt_866x_sys[ sys_use ]);
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint32, TM_UINT4, 4, 0, 0, &t_save->unsave);
            }
            break;

        case TM_ID_8668_RX_FREQ:				// 0x0107	866xRX频率设置
            if(TM_CMD_SET == cmd)
            {
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint32, TM_UINT4, 4, 0, 0, &t_save->unsave);
                BSP_866xRxFreqSet(pt_866x_sys[ sys_use ], pt_866x_sys[ sys_use ]->ch, temp_uint32);
            }
            else
            {
                temp_uint32 = BSP_866xGetRxFreq(pt_866x_sys[ sys_use ]);
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint32, TM_UINT4, 4, 0, 0, &t_save->unsave);
            }
            break;

        case TM_ID_8668_TX_GAIN_OFFSET:			// 0x0108	866xTX增益偏移量设置
            if(TM_CMD_SET == cmd)
            {
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint32, TM_SINT1, 1, -30, 3, &t_save->unsave);
                BSP_866xTxGainSet(pt_866x_sys[ sys_use ], pt_866x_sys[ sys_use ]->ch, temp_uint32);
            }
            else
            {
                temp_uint32 = BSP_866xGetTxGain(pt_866x_sys[ sys_use ]);
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint32, TM_SINT1, 1, -30, 3, &t_save->unsave);
            }
            break;

        case TM_ID_8668_RX_GAIN_OFFSET:			// 0x0109	866xRX增益偏移量设置
            if(TM_CMD_SET == cmd)
            {
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint32, TM_SINT1, 1, -30, 10, &t_save->unsave);
                BSP_866xRxGainSet(pt_866x_sys[ sys_use ], pt_866x_sys[ sys_use ]->ch, temp_uint32);
            }
            else
            {
                temp_uint32 = BSP_866xGetRxGain(pt_866x_sys[ sys_use ]);
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint32, TM_SINT1, 1, -30, 10, &t_save->unsave);
            }
            break;

        case TM_ID_8668_CH_MODE_SET:			// 0x010A	866x链路设置
            if(TM_CMD_SET == cmd)
            {
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint32, TM_UINT1, 1, 0, 7, &t_save->unsave);
                BSP_8668ChModeSet(pt_866x_sys[ sys_use ], temp_uint32);
            }
            else
            {
                temp_uint32 = rf_866x_read_ch_mode_state(pt_866x_sys[ sys_use ]);
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint32, TM_UINT1, 1, 0, 7, &t_save->unsave);
            }
            break;

        case TM_ID_8668_START_GAIN_VALUE:		// 0x010B	866x初始增益值设置

            break;

        case TM_ID_8668_BW:						// 0x010C	866x带宽设置
            if(TM_CMD_SET == cmd)
            {
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint8, TM_UINT1, 1, 10, 100, &t_save->unsave);
                BSP_8668BandWidthSet(pt_866x_sys[ sys_use ], (uint32_t)temp_uint8);
                rtt_printf(RTT_RF, "8668_BW_SET = %d\n", temp_uint8);
            }
            else
            {
                temp_uint32 = rf_866x_read_bw(pt_866x_sys[ sys_use ]);
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint8, TM_UINT1, 1, 10, 100, &t_save->unsave);
            }
            break;

        case TM_ID_8668_LOOPBACK:				// 0x010D	866x回环状态设置
            if(TM_CMD_SET == cmd)
            {
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint32, TM_UINT1, 1, 0, 5, &t_save->unsave);
                Ecr866xLoopCfg(pt_866x_sys[ sys_use ], temp_uint32);
            }
            else
            {
                temp_uint32 = rf_866x_read_loop_state(pt_866x_sys[ sys_use ]);
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint32, TM_UINT1, 1, 0, 5, &t_save->unsave);
            }
            break;

        case TM_ID_8668_SINGLE_PASS_SET:		// 0x010E	866x单通状态设置
            if(TM_CMD_SET == cmd)
            {
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint32, TM_UINT1, 1, 0, 3, &t_save->unsave);
                if(temp_uint32 <= 3)
                {
                    t_rf_866x_para[sys_use].ch_switch_state = temp_uint32;
                    Ecr866xSetChClose(pt_866x_sys[ sys_use ], temp_uint32);
                }
            }
            else
            {
                temp_uint32 = t_rf_866x_para[sys_use].ch_switch_state;	//当前无法读取8668状态
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint32, TM_UINT1, 1, 0, 3, &t_save->unsave);
            }
            break;

        case TM_ID_8668_RSSI:					// 0x010F	866x读取RSSI
            if(TM_CMD_QUERY == cmd)
            {
                temp_int8 = BSP_866xRssiRead(pt_866x_sys[ sys_use ], pt_866x_sys[ sys_use ]->ch);
                res = tm_pdu_data_rw(TM_R, cmd, pt, &temp_int8, TM_SINT1, 1, 0, 0, &t_save->unsave);
            }
            break;

        case TM_ID_8668_MIRROR_CALI: 			// 0x0110	镜像校准
            if(TM_CMD_SET == cmd)
            {
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint32, TM_UINT4, 4, 0, 0, &t_save->unsave);
                BSP_866x_set_cali_data((RF_BAND_E)band_use, CALI_8668_MIRROR, 0, temp_uint32);
            }
            else
            {
                temp_uint32 = BSP_866x_get_cali_data((RF_BAND_E)band_use, CALI_8668_MIRROR, 0);
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint32, TM_UINT4, 4, 0, 0, &t_save->unsave);
            }
            break;

        case TM_ID_8668_TX_LOCAL_OSC_I_1: 		// 0x0111	TX本振I
            if(TM_CMD_SET == cmd)
            {
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint32, TM_UINT4, 4, 0, 0, &t_save->unsave);
                BSP_866x_set_cali_data((RF_BAND_E)band_use, CALI_8668_TX_LO_I, 0, temp_uint32);
            }
            else
            {
                temp_uint32 = BSP_866x_get_cali_data((RF_BAND_E)band_use, CALI_8668_TX_LO_I, 0);
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint32, TM_UINT4, 4, 0, 0, &t_save->unsave);
            }
            break;

        case TM_ID_8668_TX_LOCAL_OSC_Q_1:			// 0x0112	TX本振Q
            if(TM_CMD_SET == cmd)
            {
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint32, TM_UINT4, 4, 0, 0, &t_save->unsave);
                BSP_866x_set_cali_data((RF_BAND_E)band_use, CALI_8668_TX_LO_Q, 0, temp_uint32);
            }
            else
            {
                temp_uint32 = BSP_866x_get_cali_data((RF_BAND_E)band_use, CALI_8668_TX_LO_Q, 0);
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint32, TM_UINT4, 4, 0, 0, &t_save->unsave);
            }
            break;

        case TM_ID_8668_TX_LOCAL_OSC_I_2:	 		// 0x0113	TX本振I
            if(TM_CMD_SET == cmd)
            {
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint32, TM_UINT4, 4, 0, 0, &t_save->unsave);
                BSP_866x_set_cali_data((RF_BAND_E)band_use, CALI_8668_TX_LO_I, 1, temp_uint32);
            }
            else
            {
                temp_uint32 = BSP_866x_get_cali_data((RF_BAND_E)band_use, CALI_8668_TX_LO_I, 1);
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint32, TM_UINT4, 4, 0, 0, &t_save->unsave);
            }
            break;

        case TM_ID_8668_TX_LOCAL_OSC_Q_2:	 		// 0x0114	TX本振Q
            if(TM_CMD_SET == cmd)
            {
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint32, TM_UINT4, 4, 0, 0, &t_save->unsave);
                BSP_866x_set_cali_data((RF_BAND_E)band_use, CALI_8668_TX_LO_Q, 1, temp_uint32);
            }
            else
            {
                temp_uint32 = BSP_866x_get_cali_data((RF_BAND_E)band_use, CALI_8668_TX_LO_Q, 1);
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint32, TM_UINT4, 4, 0, 0, &t_save->unsave);
            }
            break;

        case TM_ID_8668_RX_LOCAL_OSC_I_1:	 		// 0x0115	RX本振I
            if(TM_CMD_SET == cmd)
            {
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint32, TM_UINT4, 4, 0, 0, &t_save->unsave);
                BSP_866x_set_cali_data((RF_BAND_E)band_use, CALI_8668_RX_LO_I, 0, temp_uint32);
            }
            else
            {
                temp_uint32 = BSP_866x_get_cali_data((RF_BAND_E)band_use, CALI_8668_RX_LO_I, 0);
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint32, TM_UINT4, 4, 0, 0, &t_save->unsave);
            }
            break;

        case TM_ID_8668_RX_LOCAL_OSC_Q_1:		// 0x0116	RX本振Q
            if(TM_CMD_SET == cmd)
            {
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint32, TM_UINT4, 4, 0, 0, &t_save->unsave);
                BSP_866x_set_cali_data((RF_BAND_E)band_use, CALI_8668_RX_LO_Q, 0, temp_uint32);
            }
            else
            {
                temp_uint32 = BSP_866x_get_cali_data((RF_BAND_E)band_use, CALI_8668_RX_LO_Q, 0);
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint32, TM_UINT4, 4, 0, 0, &t_save->unsave);
            }
            break;

        case TM_ID_8668_RX_LOCAL_OSC_I_2: 	// 0x0117	RX本振I
            if(TM_CMD_SET == cmd)
            {
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint32, TM_UINT4, 4, 0, 0, &t_save->unsave);
                BSP_866x_set_cali_data((RF_BAND_E)band_use, CALI_8668_RX_LO_I, 1, temp_uint32);
            }
            else
            {
                temp_uint32 = BSP_866x_get_cali_data((RF_BAND_E)band_use, CALI_8668_RX_LO_I, 1);
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint32, TM_UINT4, 4, 0, 0, &t_save->unsave);
            }
            break;

        case TM_ID_8668_RX_LOCAL_OSC_Q_2:		// 0x0118	RX本振Q
            if(TM_CMD_SET == cmd)
            {
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint32, TM_UINT4, 4, 0, 0, &t_save->unsave);
                BSP_866x_set_cali_data((RF_BAND_E)band_use, CALI_8668_RX_LO_Q, 1, temp_uint32);
            }
            else
            {
                temp_uint32 = BSP_866x_get_cali_data((RF_BAND_E)band_use, CALI_8668_RX_LO_Q, 1);
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint32, TM_UINT4, 4, 0, 0, &t_save->unsave);
            }
            break;

        case TM_ID_8668_RX_LOCAL_OSC_I_3:	 		// 0x0119	RX本振I
            if(TM_CMD_SET == cmd)
            {
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint32, TM_UINT4, 4, 0, 0, &t_save->unsave);
                BSP_866x_set_cali_data((RF_BAND_E)band_use, CALI_8668_RX_LO_I, 2, temp_uint32);
            }
            else
            {
                temp_uint32 = BSP_866x_get_cali_data((RF_BAND_E)band_use, CALI_8668_RX_LO_I, 2);
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint32, TM_UINT4, 4, 0, 0, &t_save->unsave);
            }
            break;

        case TM_ID_8668_RX_LOCAL_OSC_Q_3:		// 0x011A	RX本振Q
            if(TM_CMD_SET == cmd)
            {
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint32, TM_UINT4, 4, 0, 0, &t_save->unsave);
                BSP_866x_set_cali_data((RF_BAND_E)band_use, CALI_8668_RX_LO_Q, 2, temp_uint32);
            }
            else
            {
                temp_uint32 = BSP_866x_get_cali_data((RF_BAND_E)band_use, CALI_8668_RX_LO_Q, 2);
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint32, TM_UINT4, 4, 0, 0, &t_save->unsave);
            }
            break;

        case TM_ID_8668_RX_LOCAL_OSC_I_4: 	// 0x011B	RX本振I
            if(TM_CMD_SET == cmd)
            {
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint32, TM_UINT4, 4, 0, 0, &t_save->unsave);
                BSP_866x_set_cali_data((RF_BAND_E)band_use, CALI_8668_RX_LO_I, 3, temp_uint32);
            }
            else
            {
                temp_uint32 = BSP_866x_get_cali_data((RF_BAND_E)band_use, CALI_8668_RX_LO_I, 3);
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint32, TM_UINT4, 4, 0, 0, &t_save->unsave);
            }
            break;

        case TM_ID_8668_RX_LOCAL_OSC_Q_4:		// 0x011C	RX本振Q
            if(TM_CMD_SET == cmd)
            {
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint32, TM_UINT4, 4, 0, 0, &t_save->unsave);
                BSP_866x_set_cali_data((RF_BAND_E)band_use, CALI_8668_RX_LO_Q, 3, temp_uint32);
            }
            else
            {
                temp_uint32 = BSP_866x_get_cali_data((RF_BAND_E)band_use, CALI_8668_RX_LO_Q, 3);
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint32, TM_UINT4, 4, 0, 0, &t_save->unsave);
            }
            break;

        case TM_ID_8668_RX_LOCAL_OSC_I_5: 	// 0x011D	RX本振I
            if(TM_CMD_SET == cmd)
            {
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint32, TM_UINT4, 4, 0, 0, &t_save->unsave);
                BSP_866x_set_cali_data((RF_BAND_E)band_use, CALI_8668_RX_LO_I, 4, temp_uint32);
            }
            else
            {
                temp_uint32 = BSP_866x_get_cali_data((RF_BAND_E)band_use, CALI_8668_RX_LO_I, 4);
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint32, TM_UINT4, 4, 0, 0, &t_save->unsave);
            }
            break;

        case TM_ID_8668_RX_LOCAL_OSC_Q_5:		// 0x011E	RX本振Q
            if(TM_CMD_SET == cmd)
            {
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint32, TM_UINT4, 4, 0, 0, &t_save->unsave);
                BSP_866x_set_cali_data((RF_BAND_E)band_use, CALI_8668_RX_LO_Q, 4, temp_uint32);
            }
            else
            {
                temp_uint32 = BSP_866x_get_cali_data((RF_BAND_E)band_use, CALI_8668_RX_LO_Q, 4);
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint32, TM_UINT4, 4, 0, 0, &t_save->unsave);
            }
            break;

        case TM_ID_8668_NORMAL_GAIN_LIST:				// 0x01C8	增益表设置
            if(TM_CMD_SET == cmd)
            {
                if(BSP_866x_set_gain_table((RF_BAND_E)band_use, TRX_MODE_NORMAL, &pt->dat, pt->len - 6) == STAT_OK)
                    res = TM_CMD_ACT_OK;
                else
                    res = TM_CMD_ACT_OTHER;
            }
            else
            {
                memset(tx_temp_buf, 0, sizeof(tx_temp_buf));
                if(BSP_866x_get_gain_table((RF_BAND_E)band_use, TRX_MODE_NORMAL, tx_temp_buf, pt->len - 6) == STAT_OK)
                {
                    res = tm_pdu_data_rw(TM_RW, cmd, pt, tx_temp_buf, TM_STR, pt->len - 6, 0, 0, &t_save->unsave);
                }
                else
                    res = TM_CMD_ACT_OTHER;
            }
            break;

        case TM_ID_8668_BLOCK_GAIN_LIST:		// 0x01C9	抗阻塞增益表设置
            if(TM_CMD_SET == cmd)
            {
                if(BSP_866x_set_gain_table((RF_BAND_E)band_use, TRX_MODE_ANTI_BLOCKING, &pt->dat, pt->len - 6) == STAT_OK)
                    res = TM_CMD_ACT_OK;
                else
                    res = TM_CMD_ACT_OTHER;
            }
            else
            {
                memset(tx_temp_buf, 0, sizeof(tx_temp_buf));
                if(BSP_866x_get_gain_table((RF_BAND_E)band_use, TRX_MODE_ANTI_BLOCKING, tx_temp_buf, pt->len - 6) == STAT_OK)
                {
                    res = tm_pdu_data_rw(TM_RW, cmd, pt, tx_temp_buf, TM_STR, pt->len - 6, 0, 0, &t_save->unsave);
                }
                else
                    res = TM_CMD_ACT_OTHER;
            }
            break;

        case TM_ID_8668_GAIN_INIT_VALUE:		// 0x01CA	初始增益
            if(TM_CMD_SET == cmd)
            {
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_int8, TM_SINT1, 1, 0, 0, &t_save->unsave);
                BSP_866x_set_default_gain((RF_BAND_E)band_use, temp_int8);
            }
            else
            {
                temp_int8 = BSP_866x_get_default_gain((RF_BAND_E)band_use);
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_int8, TM_SINT1, 1, 0, 0, &t_save->unsave);
            }
            break;

        case TM_ID_8668_READ_WRITE_REG:			//0x01CB  校准时读写寄存器
            if(TM_CMD_SET == cmd)
                res_8668 = reg_8668_get_addr_dat_in_tm(WRITE, sys_use, &pt->dat, &temp_uint32);
            else
                res_8668 = reg_8668_get_addr_dat_in_tm(READ, sys_use, &pt->dat, &temp_uint32);

            if(STAT_OK == res_8668)
            {
                res = TM_CMD_ACT_OK;
                if(TM_CMD_QUERY == cmd)
                {
                    *(uint32_t *)(&pt->dat + 5) = temp_uint32;
                }
            }
            else
                res = TM_CMD_ACT_OTHER;
            break;

        case TM_ID_8668_NORMAL_RATED_INDEX:				// 0x01CC	额定增益在正常码表的索引值
            if(TM_CMD_SET == cmd)
            {
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint8, TM_UINT1, 1, 0, 0, &t_save->unsave);
                BSP_866x_set_rated_gain_index((RF_BAND_E)band_use, TRX_MODE_NORMAL, get_valid_bw(sys_use), temp_uint8);
            }
            else
            {
                temp_uint8 = BSP_866x_get_rated_gain_index((RF_BAND_E)band_use, TRX_MODE_NORMAL,get_valid_bw(sys_use));
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint8, TM_UINT1, 1, 0, 0, &t_save->unsave);
            }
            break;

        case TM_ID_8668_BLOCK_RATED_INDEX:		// 0x01CD	额定增益在抗阻塞码表的索引值
            if(TM_CMD_SET == cmd)
            {
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint8, TM_UINT1, 1, 0, 0, &t_save->unsave);
                BSP_866x_set_rated_gain_index((RF_BAND_E)band_use, TRX_MODE_ANTI_BLOCKING, get_valid_bw(sys_use), temp_uint8);
            }
            else
            {
                temp_uint8 = BSP_866x_get_rated_gain_index((RF_BAND_E)band_use, TRX_MODE_ANTI_BLOCKING, get_valid_bw(sys_use));
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint8, TM_UINT1, 1, 0, 0, &t_save->unsave);
            }
            break;

        //临时测试使用
        case TM_ID_8668_TEST_TEMP_01:				//0x0180  8668临时测试校准使用
            temp_uint32 = BSP_866x_Ex_Read(pt_866x_sys[sys_use], 0x20000004);
            rtt_printf(RTT_RF, "01 - %s read 866x state = %d\n", rf_sys_name[sys_use], temp_uint32);
            res = tm_pdu_data_rw(TM_R, cmd, pt, &temp_uint32, TM_UINT4, 4, 0, 0, &t_save->unsave);
            break;

        case TM_ID_8668_TEST_TEMP_02:				//0x0181  8668临时测试校准使用
            BSP_866x_Ext_Write(pt_866x_sys[ sys_use ], 0x00200500, 0x0F85CC48);
            rtt_printf(RTT_RF, "02 - %s ext page: 0x00200500, 0x0F85CC48\n", rf_sys_name[sys_use]);
            break;

        case TM_ID_8668_TEST_TEMP_03:				//0x0182  8668临时测试校准使用
            BSP_866x_Ext_Write(pt_866x_sys[ sys_use ], 0x00200380, 0x017F8080);
            rtt_printf(RTT_RF, "03 - %s ext page: 0x00200380, 0x017F8080\n", rf_sys_name[sys_use]);
            break;

        case TM_ID_8668_TEST_TEMP_04:				//0x0183  8668临时测试校准使用
            BSP_866x_Ext_Write(pt_866x_sys[ sys_use ], 0x00200388, 0x017F8080);
            rtt_printf(RTT_RF, "04 - %s ext page: 0x00200388, 0x017F8080\n", rf_sys_name[sys_use]);
            break;

        case TM_ID_8668_TEST_TEMP_05:				//0x0184  8668临时测试校准使用
            BSP_866x_Ext_Write(pt_866x_sys[ sys_use ], 0x00200380, 0x017F80C0);
            rtt_printf(RTT_RF, "05 - %s ext page: 0x00200380, 0x017F80C0\n", rf_sys_name[sys_use]);
            break;

        case TM_ID_8668_TEST_TEMP_06:				//0x0185  8668临时测试校准使用
            BSP_866x_Ext_Write(pt_866x_sys[ sys_use ], 0x00200388, 0x017F8080);
            rtt_printf(RTT_RF, "06 - %s ext page: 0x00200388, 0x017F8080\n", rf_sys_name[sys_use]);
            break;

        case TM_ID_8668_TEST_TEMP_07:				//0x0186  8668临时测试校准使用
            BSP_866x_Ext_Write(pt_866x_sys[ sys_use ], 0x00200380, 0x017FC080);
            rtt_printf(RTT_RF, "07 - %s ext page: 0x00200380, 0x017FC080\n", rf_sys_name[sys_use]);
            break;

        case TM_ID_8668_TEST_TEMP_08:				//0x0187  8668临时测试校准使用
            BSP_866x_Ext_Write(pt_866x_sys[ sys_use ], 0x00200388, 0x017F8080);
            rtt_printf(RTT_RF, "08 - %s ext page: 0x00200388, 0x017F8080\n", rf_sys_name[sys_use]);
            break;

        case TM_ID_8668_TEST_TEMP_09:				//0x0188  8668临时测试校准使用
            BSP_866x_Ext_Write(pt_866x_sys[ sys_use ], 0x00200380, 0x017F8080);
            rtt_printf(RTT_RF, "09 - %s ext page: 0x00200380, 0x017F8080\n", rf_sys_name[sys_use]);
            break;

        case TM_ID_8668_TEST_TEMP_10:				//0x0189  8668临时测试校准使用
            BSP_866x_Ext_Write(pt_866x_sys[ sys_use ], 0x00200388, 0x017F80C0);
            rtt_printf(RTT_RF, "10 - %s ext page: 0x00200388, 0x017F80C0\n", rf_sys_name[sys_use]);
            break;

        case TM_ID_8668_TEST_TEMP_11:				//0x018A  8668临时测试校准使用
            BSP_866x_Ext_Write(pt_866x_sys[ sys_use ], 0x00200380, 0x017F8080);
            rtt_printf(RTT_RF, "11 - %s ext page: 0x00200380, 0x017F8080\n", rf_sys_name[sys_use]);
            break;

        case TM_ID_8668_TEST_TEMP_12:				//0x018B  8668临时测试校准使用
            BSP_866x_Ext_Write(pt_866x_sys[ sys_use ], 0x00200388, 0x017FC080);
            rtt_printf(RTT_RF, "12 - %s ext page: 0x00200388, 0x017FC080\n", rf_sys_name[sys_use]);
            break;

        case TM_ID_8668_TEST_TEMP_13:				//0x018C  8668临时测试校准使用
            res = tm_pdu_data_rw(TM_W, cmd, pt, &temp_uint32, TM_UINT4, 4, 0, 0, &t_save->unsave);
            BSP_866x_Ext_Write(pt_866x_sys[ sys_use ], 0x0200380, temp_uint32);
            rtt_printf(RTT_RF, "13 - %s ext page: 0x0200380, 0x%08x\n", rf_sys_name[sys_use], temp_uint32);
            break;

        case TM_ID_8668_TEST_TEMP_14:				//0x018D  8668临时测试校准使用
            res = tm_pdu_data_rw(TM_W, cmd, pt, &temp_uint32, TM_UINT4, 4, 0, 0, &t_save->unsave);
            BSP_866x_Ext_Write(pt_866x_sys[ sys_use ], 0x00200388, temp_uint32);
            rtt_printf(RTT_RF, "14 - %s ext page: 0x00200388, 0x%08x\n", rf_sys_name[sys_use], temp_uint32);
            break;

        case TM_ID_8668_TEST_TEMP_15:				//0x018E  8668临时测试校准使用
            BSP_866x_Ext_Write(pt_866x_sys[ sys_use ], 0x00200500, 0x0F85FF49);
            rtt_printf(RTT_RF, "15 - %s ext page: 0x00200500, 0x0F85FF49\n", rf_sys_name[sys_use]);
            break;

        case TM_ID_8668_TEST_TEMP_16:				//0x018F  8668临时测试校准使用
            BSP_866x_Ext_Write(pt_866x_sys[ sys_use ], 0x00200540, 0x00000000);
            rtt_printf(RTT_RF, "16 - %s ext page: 0x00200540, 0x00000000\n", rf_sys_name[sys_use]);
            break;

        case TM_ID_8668_TEST_TEMP_17:				//0x0190  8668临时测试校准使用
            BSP_866x_Ext_Write(pt_866x_sys[ sys_use ], 0x00200544, 0x00000000);
            rtt_printf(RTT_RF, "17 - %s ext page: 0x00200544, 0x00000000\n", rf_sys_name[sys_use]);
            break;

        case TM_ID_8668_TEST_TEMP_18:				//0x0191  8668临时测试校准使用
            BSP_866x_Ext_Write(pt_866x_sys[ sys_use ], 0x00200540, 0x00000040);
            rtt_printf(RTT_RF, "18 - %s ext page: 0x00200540, 0x00000040\n", rf_sys_name[sys_use]);
            break;

        case TM_ID_8668_TEST_TEMP_19:				//0x0192  8668临时测试校准使用
            BSP_866x_Ext_Write(pt_866x_sys[ sys_use ], 0x00200544, 0x00000000);
            rtt_printf(RTT_RF, "19 - %s ext page: 0x00200544, 0x00000000\n", rf_sys_name[sys_use]);
            break;

        case TM_ID_8668_TEST_TEMP_20:				//0x0193  8668临时测试校准使用
            BSP_866x_Ext_Write(pt_866x_sys[ sys_use ], 0x00200540, 0x00004000);
            rtt_printf(RTT_RF, "20 - %s ext page: 0x00200540, 0x00004000\n", rf_sys_name[sys_use]);
            break;

        case TM_ID_8668_TEST_TEMP_21:				//0x0194  8668临时测试校准使用
            BSP_866x_Ext_Write(pt_866x_sys[ sys_use ], 0x00200544, 0x00000000);
            rtt_printf(RTT_RF, "21 - %s ext page: 0x00200544, 0x00000000\n", rf_sys_name[sys_use]);
            break;

        case TM_ID_8668_TEST_TEMP_22:				//0x0195  8668临时测试校准使用
            BSP_866x_Ext_Write(pt_866x_sys[ sys_use ], 0x00200540, 0x00000000);
            rtt_printf(RTT_RF, "22 - %s ext page: 0x00200540, 0x00000000\n", rf_sys_name[sys_use]);
            break;

        case TM_ID_8668_TEST_TEMP_23:				//0x0196  8668临时测试校准使用
            BSP_866x_Ext_Write(pt_866x_sys[ sys_use ], 0x00200544, 0x00000040);
            rtt_printf(RTT_RF, "23 - %s ext page: 0x00200544, 0x00000040\n", rf_sys_name[sys_use]);
            break;

        case TM_ID_8668_TEST_TEMP_24:				//0x0197  8668临时测试校准使用
            BSP_866x_Ext_Write(pt_866x_sys[ sys_use ], 0x00200540, 0x00000000);
            rtt_printf(RTT_RF, "24 - %s ext page: 0x00200540, 0x00000000\n", rf_sys_name[sys_use]);
            break;

        case TM_ID_8668_TEST_TEMP_25:				//0x0198  8668临时测试校准使用
            BSP_866x_Ext_Write(pt_866x_sys[ sys_use ], 0x00200544, 0x00004000);
            rtt_printf(RTT_RF, "25 - %s ext page: 0x00200544, 0x00004000\n", rf_sys_name[sys_use]);
            break;

        case TM_ID_8668_TEST_TEMP_26:				//0x0199  8668临时测试校准使用
            res = tm_pdu_data_rw(TM_W, cmd, pt, &temp_uint32, TM_UINT4, 4, 0, 0, &t_save->unsave);
            temp_uint32 -= 0x00008080;  // 减0x00008080
            BSP_866x_Ext_Write(pt_866x_sys[ sys_use ], 0x00200540, temp_uint32);
            rtt_printf(RTT_RF, "26 - %s ext page: 0x00200540, 0x%08x\n", rf_sys_name[sys_use], temp_uint32);
            break;

        case TM_ID_8668_TEST_TEMP_27:				//0x019A  8668临时测试校准使用
            res = tm_pdu_data_rw(TM_W, cmd, pt, &temp_uint32, TM_UINT4, 4, 0, 0, &t_save->unsave);
            temp_uint32 -= 0x00008080;  // 减0x00008080
            BSP_866x_Ext_Write(pt_866x_sys[ sys_use ], 0x00200544, temp_uint32);
            rtt_printf(RTT_RF, "27 - %s ext page: 0x00200544, 0x%08x\n", rf_sys_name[sys_use], temp_uint32);
            break;

        case TM_ID_8668_TEST_TEMP_28:				//0x019B  8668临时测试校准使用
            BSP_866x_Ext_Write(pt_866x_sys[ sys_use ], 0x00200D08, 0x00520148);
            rtt_printf(RTT_RF, "28 - %s ext page: 0x00200D08, 0x00520148\n", rf_sys_name[sys_use]);
            break;

        case TM_ID_8668_TEST_TEMP_29:				//0x019C  8668临时测试校准使用
            BSP_866x_Ext_Write(pt_866x_sys[ sys_use ], 0x00200D08, 0xFFAE0148);
            rtt_printf(RTT_RF, "29 - %s ext page: 0x00200D08, 0xFFAE0148\n", rf_sys_name[sys_use]);
            break;

        case TM_ID_8668_TEST_TEMP_30:				//0x019D  8668临时测试校准使用
            if(RF_SYS_1 == sys_use)
            {
                BSP_866x_Ext_Write(pt_866x_sys[ sys_use ], 0x00200D08, 0xFF72FEB8);
                rtt_printf(RTT_RF, "30 - %s ext page: 0x00200D08, 0xFF72FEB8\n", rf_sys_name[sys_use]);
            }
            else
            {
                BSP_866x_Ext_Write(pt_866x_sys[ sys_use ], 0x00200D08, 0xFFAEFEB8);
                rtt_printf(RTT_RF, "30 - %s ext page: 0x00200D08, 0xFFAEFEB8\n", rf_sys_name[sys_use]);
            }
            break;

        case TM_ID_8668_TEST_TEMP_31:				//0x019E  8668临时测试校准使用
            res = tm_pdu_data_rw(TM_W, cmd, pt, &temp_uint32, TM_UINT4, 4, 0, 0, &t_save->unsave);
            BSP_866x_Ext_Write(pt_866x_sys[ sys_use ], 0x00200D08, temp_uint32);
            rtt_printf(RTT_RF, "31 - %s ext page: 0x00200D08, 0x%08x\n", rf_sys_name[sys_use], temp_uint32);
            break;

        default:
            res = TM_ERR_IDERR;
            break;
    }

    return res;
}

/*****************************************************************************
* @Name	  : tm_pdu_ble_data_ergodic
* @Brief  :
* @Param  :  dev_id:
**			 cmd: [输入/出]
**			 pt: [输入/出]
**			 t_save:
* @Retval :
* @Author :
* @Data   : 2023-12-18
* @Note   :
*****************************************************************************/
uint8_t tm_pdu_ble_data_ergodic(uint8_t dev_id, uint8_t cmd, tm_oid_data_t * pt, tm_save_flag_t * t_save)
{
    uint8_t res = TM_ERR_OK;
    int8_t  temp_int8 = 0;
    uint8_t  temp_uint8 = 0;
    uint32_t temp_uint32 = 0;

    switch(pt->oid)
    {
        case TM_ID_BLE_EN: 						// 0x0001	蓝牙组网功能使能/关闭
            if(TM_CMD_SET == cmd)
            {
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint8, TM_UINT1, 1, 0, 1, &t_save->unsave);
                hBle.module_config.autoconn_enable = temp_uint8;
                ble_set_conn_en(temp_uint8);
                hBle.module_config.appstatus = temp_uint8;
                ble_cmd(CMD_SET_BLE_APP);
            }
            else
            {
                ble_cmd(CMD_QUERY_BLE_AUTOCONN);
                temp_uint8 = hBle.module_config.autoconn_enable;
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint8, TM_UINT1, 1, 0, 1, &t_save->unsave);
            }
            break;

        case TM_ID_BLE_WORK_MODE: 				// 0x0002	蓝牙工作模式设置
            if(TM_CMD_SET == cmd)
            {
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint8, TM_UINT1, 1, 1, 3, &t_save->unsave);
                hBle.module_config.mode = temp_uint8;
                ble_cmd(CMD_SET_BLE_MODE);
            }
            else
            {
                ble_cmd(CMD_QUERY_BLE_MODE);
                temp_uint8 = hBle.module_config.mode;
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint8, TM_UINT1, 1, 1, 3, &t_save->unsave);
            }
            break;

        case TM_ID_BLE_RSSI: 					// 0x0003	蓝牙RSSI读取(仅从机)
            #if LOCAL_MODULE_TYPE == _RU
            ble_cmd(CMD_QUERY_BLE_RSSI);
            temp_int8 = hBle.ble_stat.rssi;
            res = tm_pdu_data_rw(TM_R, cmd, pt, &temp_int8, TM_SINT1, 1, -100, 100, &t_save->unsave);
            #endif
            break;

        case TM_ID_BLE_RST: 					// 0x0004	蓝牙复位
            res = tm_pdu_data_rw(TM_W, cmd, pt, &temp_uint8, TM_UINT1, 1, 0, 1, &t_save->unsave);
            if(1 == temp_uint8)
                ble_cmd(CMD_SET_BLE_RESET);
            break;

        case TM_ID_BLE_MAC: 					// 0x0005	蓝牙MAC地址查询
            //temp_uint32 = blepara.mac[0][0];
            //res = tm_pdu_data_rw(TM_R, cmd, pt, &temp_uint32, TM_UINT4, 4, 0, 0, &t_save->unsave);
            break;

        case TM_ID_BLE_FREQ_POINT: 				// 0x0006	蓝牙频点设置

            break;

        case TM_ID_BLE_OUT_POW: 				// 0x0007	蓝牙发射功率设置
            if(TM_CMD_SET == cmd)
            {
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_int8, TM_SINT1, 1, -128, 127, &t_save->unsave);
                hBle.module_config.ble_tx_pow = temp_int8;
                ble_cmd(CMD_SET_BLE_TXPWR);
            }
            else
            {
                temp_int8 = hBle.module_config.ble_tx_pow;
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_int8, TM_SINT1, 1, -128, 127, &t_save->unsave);
            }
            break;

        case TM_ID_BLE_VER: 					// 0x0008	蓝牙版本号查询
            temp_uint32 = ble_get_version();
            res = tm_pdu_data_rw(TM_R, cmd, pt, &temp_uint32, TM_UINT4, 4, 0, 0, &t_save->unsave);
            break;

        case TM_ID_BLE_SLAVE_NUM: 				// 0x0009	蓝牙当前从机数量查询
            ble_cmd(CMD_QUERY_BLE_QLINK);
            temp_uint8 = hBle.ble_stat.slave_connect_num;
            res = tm_pdu_data_rw(TM_R, cmd, pt, &temp_uint8, TM_UINT1, 1, 0, 8, &t_save->unsave);
            break;

        case TM_ID_BLE_MASTER_NUM: 				// 0x000A	蓝牙当前主机数量查询

            break;

        case TM_ID_BLE_APP_EN: 					// 0x000B	使能手机APP服务，使能后可接入APP
            if(TM_CMD_SET == cmd)
            {
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint8, TM_UINT1, 1, 0, 1, &t_save->unsave);
                hBle.module_config.appstatus = temp_uint8;
                ble_cmd(CMD_SET_BLE_APP);
            }
            else
            {
                temp_uint8 = hBle.module_config.appstatus;
                res = tm_pdu_data_rw(TM_RW, cmd, pt, &temp_uint8, TM_UINT1, 1, 0, 1, &t_save->unsave);
            }
            break;

        case TM_ID_BLE_IN_TRANS: 				// 0x000C	模组进入透传模式

            break;

        case TM_ID_BLE_TX: 						// 0x000D	发透传数据

            break;

        case TM_ID_BLE_RX: 						// 0x000E	收透传数据

            break;

        default:
            res = TM_ERR_IDERR;
            break;
    }

    return res;
}

/*****************************************************************************
* @Name	  : tm_pdu_hub_data_ergodic
* @Brief  :
* @Param  :  dev_id:
**			 cmd: [输入/出]
**			 pt: [输入/出]
**			 t_save:
* @Retval :
* @Author :
* @Data   : 2023-12-18
* @Note   :
*****************************************************************************/
uint8_t tm_pdu_hub_data_ergodic(uint8_t dev_id, uint8_t cmd, tm_oid_data_t * pt, tm_save_flag_t * t_save)
{
    uint8_t res = TM_ERR_OK;

    switch(pt->oid)
    {
        case TM_ID_HUB_PORT1_SWITCH: 			// 0x0001	端口1电源开关

            break;

        case TM_ID_HUB_PORT2_SWITCH: 			// 0x0002	端口2电源开关

            break;

        case TM_ID_HUB_PORT3_SWITCH: 			// 0x0003	端口3电源开关

            break;

        case TM_ID_HUB_PORT4_SWITCH: 			// 0x0004	端口4电源开关

            break;

        case TM_ID_HUB_PORT5_SWITCH:			// 0x0005	端口5电源开关

            break;

        case TM_ID_HUB_PORT6_SWITCH: 			// 0x0006	端口6电源开关

            break;

        case TM_ID_HUB_PORT7_SWITCH: 			// 0x0007	端口7电源开关

            break;

        case TM_ID_HUB_PORT8_SWITCH: 			// 0x0008	端口8电源开关

            break;

        case TM_ID_HUB_PORT9_SWITCH: 			// 0x0009	端口9电源开关

            break;

        case TM_ID_HUB_PORT10_SWITCH: 			// 0x000A	端口10电源开关

            break;

        case TM_ID_HUB_PORT11_SWITCH: 			// 0x000B	端口11电源开关

            break;

        case TM_ID_HUB_PORT12_SWITCH: 			// 0x000C	端口12电源开关

            break;

        case TM_ID_HUB_PORT13_SWITCH: 			// 0x000D	端口13电源开关

            break;

        case TM_ID_HUB_PORT14_SWITCH: 			// 0x000E	端口14电源开关

            break;

        case TM_ID_HUB_PORT15_SWITCH: 			// 0x000F	端口15电源开关

            break;

        case TM_ID_HUB_PORT16_SWITCH: 			// 0x0010	端口16电源开关

            break;

        case TM_ID_HUB_PORT1_CURRENT: 			// 0x0011	端口1电流

            break;

        case TM_ID_HUB_PORT2_CURRENT: 			// 0x0012	端口2电流

            break;

        case TM_ID_HUB_PORT3_CURRENT: 			// 0x0013	端口3电流

            break;

        case TM_ID_HUB_PORT4_CURRENT: 			// 0x0014	端口4电流

            break;

        case TM_ID_HUB_PORT5_CURRENT: 			// 0x0015	端口5电流

            break;

        case TM_ID_HUB_PORT6_CURRENT: 			// 0x0016	端口6电流

            break;

        case TM_ID_HUB_PORT7_CURRENT: 			// 0x0017	端口7电流

            break;

        case TM_ID_HUB_PORT8_CURRENT: 			// 0x0018	端口8电流

            break;

        case TM_ID_HUB_PORT9_CUTTENT: 			// 0x0019	端口9电流

            break;

        case TM_ID_HUB_PORT10_CUTTENT:			// 0x001A	端口10电流

            break;

        case TM_ID_HUB_PORT11_CUTTENT:			// 0x001B	端口11电流

            break;

        case TM_ID_HUB_PORT12_CUTTENT: 			// 0x001C	端口12电流

            break;

        case TM_ID_HUB_PORT13_CUTTENT: 			// 0x001D	端口13电流

            break;

        case TM_ID_HUB_PORT14_CUTTENT: 			// 0x001E	端口14电流

            break;

        case TM_ID_HUB_PORT15_CUTTENT: 			// 0x001F	端口15电流

            break;

        case TM_ID_HUB_PORT16_CUTTENT: 			// 0x0020	端口16电流

            break;

        case TM_ID_HUB_PORT1_EN: 				// 0x0021	端口1使能

            break;

        case TM_ID_HUB_PORT2_EN: 				// 0x0022	端口2使能

            break;

        case TM_ID_HUB_PORT3_EN: 				// 0x0023	端口3使能

            break;

        case TM_ID_HUB_PORT4_EN: 				// 0x0024	端口4使能

            break;

        case TM_ID_HUB_PORT5_EN: 				// 0x0025	端口5使能

            break;

        case TM_ID_HUB_PORT6_EN: 				// 0x0026	端口6使能

            break;

        case TM_ID_HUB_PORT7_EN: 				// 0x0027	端口7使能

            break;

        case TM_ID_HUB_PORT8_EN: 				// 0x0028	端口8使能

            break;

        case TM_ID_HUB_PORT9_EN: 				// 0x0029	端口9使能

            break;

        case TM_ID_HUB_PORT10_EN: 				// 0x002A	端口10使能

            break;

        case TM_ID_HUB_PORT11_EN: 				// 0x002B	端口11使能

            break;

        case TM_ID_HUB_PORT12_EN: 				// 0x002C	端口12使能

            break;

        case TM_ID_HUB_PORT13_EN: 				// 0x002D	端口13使能

            break;

        case TM_ID_HUB_PORT14_EN: 				// 0x002E	端口14使能

            break;

        case TM_ID_HUB_PORT15_EN: 				// 0x002F	端口15使能

            break;

        case TM_ID_HUB_PORT16_EN: 				// 0x0030	端口16使能

            break;

        default:
            res = TM_ERR_IDERR;
            break;
    }

    return res;
}

