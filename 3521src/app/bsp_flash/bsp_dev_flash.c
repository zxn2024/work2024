/***************************** Copyright ****************************************
*
* (C) Copyright 2023, Shaanxi Tianji Communication
*  All Rights Reserved
*
* FileName   : bsp_dev_flash.c
* Version    : none
* Author     :
* Date       : 2023-11-27
* Description: none
*******************************************************************************/
#include "main.h"
#include "rtt.h"
#include "system_config.h"
#include "bsp_config.h"
#include "bsp_rf_global.h"
#include "bsp_dev_flash.h"
#include "bsp_dev_para.h"
#include "bsp_rf_oplc.h"
#include "crc.h"
#include "bsp_flash_addr.h"
//#include "bi_flash.h"
#include "bsp_rf_4g_band_sel.h"
#include "FreeRTOS.h"
#include "task.h"
#include "bsp_flash_struct.h"
#include "bsp_config.h"
#include "bsp_rf_866x.h"
#include "bsp_warn_user.h"
#include "bsp_dev_para.h"

#if( _MU == LOCAL_MODULE_TYPE )
#include "bsp_rf_5g_modem.h"
#endif

//extern uint16_t flash_25xx_addr_to_sector(uint32_t addr);
/*---------------------- 端口定义 --------------------------------------*/


/*---------------------- 宏定义  ---------------------------------------*/
#define START_DEVICE_BYTE	0x55AA

/*---------------------- 结构体 ----------------------------------------*/


/*---------------------- 常量 ------------------------------------------*/


/*---------------------- 全局变量 --------------------------------------*/


/*---------------------- 局部变量 --------------------------------------*/
// #if(PRODUCT_MODEL == CMDT3521C)
// __align(32) static flash_dev_para_t  t_flash_dev_para __attribute__((section("CCMRAM")));    //接收数据缓冲区
// #else
// static flash_dev_para_t  t_flash_dev_para;
// #endif
static flash_dev_para_t  t_flash_dev_para;
/*---------------------- 常量 ------------------------------------------*/


/*---------------------- 全局变量 --------------------------------------*/


/*---------------------- 局部变量 --------------------------------------*/


/*---------------------- 声明 ------------------------------------------*/


/*****************************************************************************
* @Name	  : flash_dev_para_restore_factory_setting
* @Brief  : 设备参数恢复出厂设置
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2023-11-29
* @Note   :
*****************************************************************************/
static void flash_dev_para_restore_factory_setting(void)
{
    memset(&t_flash_dev_para, 0x00, sizeof(t_flash_dev_para));   // 清空结构体

    flash_dev_para_t * pt = &t_flash_dev_para;

    rtt_printf(RTT_FLASH, RTT_TEXT_CYAN"[%s restore factory L%d]: flash device Para restore factory setting\n", __MODULE__, __LINE__);

    pt->head					= START_DEVICE_BYTE;            //起始字节 2

    pt->t_dev.work_mode			= 1;							//工作模式
    pt->t_dev.slave_dev_num		= 8;				            //从机数量
    pt->t_dev.rst_time_by_net	= 0x03;							//根据网络定时重启时间
    pt->t_dev.rst_by_run_time	= 72;							//重启间隔
    pt->t_dev.led_plan			= 0x01;							//指示灯方案
    pt->t_dev.led_switch		= DISABLE;						//指示灯开关
    pt->t_dev.tmp_offset		= 0;							//温度偏移量
	pt->t_dev.warn_en			= ENABLE;						//告警使能
	pt->t_dev.warn_save_en      = ENABLE;						//告警状态保存使能
    memset(pt->t_dev.reserve, 0, sizeof(pt->t_dev.reserve)); 	//预留字节
    
	memset( &pt->t_omc, 0, sizeof( omc_t ) );
    memcpy(pt->t_fact.factory_id, (uint8_t *)DEFAULT_FACTORY, strlen((char *)DEFAULT_FACTORY));     // 设备厂商
    memcpy(pt->t_fact.dev_model, (uint8_t *)DEVICE_MODEL, strlen((char *)DEVICE_MODEL));     // 设备型号
    pt->t_fact.dev_type = LOCAL_DEVICE_TYPE;        //设备类别
    memset(pt->t_fact.reserve, 0xFF, sizeof(pt->t_fact.reserve));

    pt->t_omc.ip[0].tcp_or_udp   = LINK_TCP;    // TCP
    pt->t_omc.ip[0].ip_type      = LINK_IPV4;   // IPV4

    pt->t_omc.ip[1].tcp_or_udp   = LINK_TCP;    // TCP
    pt->t_omc.ip[1].ip_type      = LINK_IPV4;   // IPV4
    pt->t_omc.ip[1].ipv4_addr[0] = 39;
    pt->t_omc.ip[1].ipv4_addr[1] = 106;
    pt->t_omc.ip[1].ipv4_addr[2] = 42;
    pt->t_omc.ip[1].ipv4_addr[3] = 94;
    pt->t_omc.ip[1].port 		 = 4238;
	memcpy( pt->t_omc.apn, "CMNET", sizeof( "CMNET") ); 
	pt->t_omc.heart              = 180;        // 心跳包默认180s
	
    for(uint8_t sys = RF_SYS_1; sys < RF_SYS_NUM; sys++)
    {
        pt->t_sys_pub[ sys ].tmp_offset 	= 0;			//温度校准值
        pt->t_sys_pub[ sys ].warn_en  		= ENABLE;		//允许告警使能
        pt->t_sys_pub[ sys ].oplc_en  		= DISABLE;		//开环工控使能
        pt->t_sys_pub[ sys ].rsrp_en  		= ENABLE;		//rsrp使能
        pt->t_sys_pub[ sys ].tmp_en  		= DISABLE;		//温补使能
        pt->t_sys_pub[ sys ].silence_en  	= ENABLE;		//静默使能
        pt->t_sys_pub[ sys ].iso_en  		= ENABLE;		//隔离度检测使能
        pt->t_sys_pub[ sys ].rf_switch 		= OPEN;			//射频开关状态
        memset(pt->t_sys_pub[sys].reserve, 0xFF, sizeof(pt->t_sys_pub[sys].reserve));
    }

    for(uint8_t band = BAND_3; band <= BAND_END; band++)
    {
		memset( &pt->t_rf_ch_para[band], 0, sizeof( para_rf_ch_t ) );
    }
	
	
	pt->t_rf_ch_para[ BAND_3 ].exist               = B_TRUE;
	pt->t_rf_ch_para[ BAND_3 ].oplc_gate 		   = 0;
	pt->t_rf_ch_para[ BAND_3 ].oplc_gain_offset    = 0;
	pt->t_rf_ch_para[ BAND_3 ].oplc_pow_offset     = 0;
	pt->t_rf_ch_para[ BAND_3 ].band_width          = (uint8_t)rf_866x_get_default_band_width( BAND_3 );
	pt->t_rf_ch_para[ BAND_3 ].freq                = rf_866x_get_default_freq_point( BAND_3 );
	memcpy( pt->t_rf_ch_para[ BAND_3 ].scan_freq, "1300,1325,1350",sizeof( "1300,1325,1350" ) );
	
	pt->t_rf_ch_para[ BAND_8 ].exist               = B_TRUE;
	pt->t_rf_ch_para[ BAND_8 ].oplc_gate 		   = 0;
	pt->t_rf_ch_para[ BAND_8 ].oplc_gain_offset    = 0;
	pt->t_rf_ch_para[ BAND_8 ].oplc_pow_offset     = 0;
	pt->t_rf_ch_para[ BAND_8 ].band_width          = (uint8_t)rf_866x_get_default_band_width( BAND_8 );
	pt->t_rf_ch_para[ BAND_8 ].freq                = rf_866x_get_default_freq_point( BAND_8 );
	memcpy( pt->t_rf_ch_para[ BAND_8 ].scan_freq, "3590,3615,3640",sizeof( "3590,3615,3640" ) );
	
	pt->t_rf_ch_para[ BAND_39 ].exist 			   = ( 3 == DEV_BAND_NUM ) ? B_FALSE : B_TRUE;
	pt->t_rf_ch_para[ BAND_39 ].oplc_gate 		   = 0;
	pt->t_rf_ch_para[ BAND_39 ].oplc_gain_offset   = 0;
	pt->t_rf_ch_para[ BAND_39 ].oplc_pow_offset    = 0;
	pt->t_rf_ch_para[ BAND_39 ].band_width         = (uint8_t)rf_866x_get_default_band_width( BAND_39 );
	pt->t_rf_ch_para[ BAND_39 ].freq               = rf_866x_get_default_freq_point( BAND_39 );
	memcpy( pt->t_rf_ch_para[ BAND_39 ].scan_freq, "38400,38450,38500",sizeof( "38400,38450,38500" ) );
	
	pt->t_rf_ch_para[ BAND_40 ].exist 			   = ( 3 == DEV_BAND_NUM ) ? B_FALSE : B_TRUE;
	pt->t_rf_ch_para[ BAND_40 ].oplc_gate 		   = 0;
	pt->t_rf_ch_para[ BAND_40 ].oplc_gain_offset   = 0;
	pt->t_rf_ch_para[ BAND_40 ].oplc_pow_offset    = 0;
	pt->t_rf_ch_para[ BAND_40 ].band_width         = (uint8_t)rf_866x_get_default_band_width( BAND_40 );
	pt->t_rf_ch_para[ BAND_40 ].freq               = rf_866x_get_default_freq_point( BAND_40 );
	memcpy( pt->t_rf_ch_para[ BAND_40 ].scan_freq, "38950,39100,39250",sizeof( "38950,39100,39250" ) );

	pt->t_rf_ch_para[ BAND_41 ].exist              = B_TRUE;
	pt->t_rf_ch_para[ BAND_41 ].oplc_gate 		   = 0;
	pt->t_rf_ch_para[ BAND_41 ].oplc_gain_offset   = 0;
	pt->t_rf_ch_para[ BAND_41 ].oplc_pow_offset    = 0;
	pt->t_rf_ch_para[ BAND_41 ].band_width         = (uint8_t)rf_866x_get_default_band_width( BAND_41 );
	pt->t_rf_ch_para[ BAND_41 ].freq               = rf_866x_get_default_freq_point( BAND_41 );
	memset( pt->t_rf_ch_para[ BAND_41 ].scan_freq, 0, 64 );
	memcpy( pt->t_rf_ch_para[ BAND_41 ].scan_freq, "504990,510990,516990",sizeof( "504990,510990,516990" ) );

	pt->t_rf_ch_para[ BAND_41M ].exist             = B_TRUE;
	pt->t_rf_ch_para[ BAND_41M ].oplc_gate 		   = 0;
	pt->t_rf_ch_para[ BAND_41M ].oplc_gain_offset  = 0;
	pt->t_rf_ch_para[ BAND_41M ].oplc_pow_offset   = 0;
	pt->t_rf_ch_para[ BAND_41M ].band_width        = (uint8_t)rf_866x_get_default_band_width( BAND_41M );
	pt->t_rf_ch_para[ BAND_41M ].freq              = rf_866x_get_default_freq_point( BAND_41M );
	memset( pt->t_rf_ch_para[ BAND_41M ].scan_freq, 0, 64 );
	memcpy( pt->t_rf_ch_para[ BAND_41M ].scan_freq, "504990,510990,516990",sizeof( "504990,510990,516990" ) );
	
	memset( &pt->t_net_config, 0, sizeof( NetConfig_T ) ); //5g 模组配置参数
	pt->t_net_config.frame = 1;
	pt->t_net_config.specialTime = 1;
	pt->t_net_config.ssb = 504990;
	
	for(uint8_t sys = RF_SYS_1; sys < RF_SYS_NUM; sys++)
	{
		pt->t_warn[sys].mu.up_inpow_greater.en  = ENABLE;
		pt->t_warn[sys].mu.up_inpow_less.en   	= ENABLE;
		pt->t_warn[sys].mu.up_outpow_greater.en = ENABLE;
		pt->t_warn[sys].mu.up_outpow_less.en  	= ENABLE;
		pt->t_warn[sys].mu.up_iso.en 		  	= ENABLE;
		pt->t_warn[sys].mu.dw_inpow_greater.en  = ENABLE;
		pt->t_warn[sys].mu.dw_inpow_less.en   	= ENABLE;
		pt->t_warn[sys].mu.dw_outpow_greater.en = ENABLE;
		pt->t_warn[sys].mu.dw_outpow_less.en  	= ENABLE;
		pt->t_warn[sys].mu.dw_iso.en          	= ENABLE;
		pt->t_warn[sys].mu.tmp_greater.en       = ENABLE;
		pt->t_warn[sys].mu.other.en           	= ENABLE;
		
		pt->t_warn[sys].mu.up_inpow_less.gate     = -128;
		pt->t_warn[sys].mu.up_inpow_greater.gate  =  127;
		pt->t_warn[sys].mu.up_outpow_less.gate    = -128;
		pt->t_warn[sys].mu.up_outpow_greater.gate =  127;
		pt->t_warn[sys].mu.up_iso.gate            = -128;
		pt->t_warn[sys].mu.dw_inpow_less.gate  	  = -128;
		pt->t_warn[sys].mu.dw_inpow_greater.gate  =  127;
		pt->t_warn[sys].mu.dw_outpow_less.gate 	  = -128;
		pt->t_warn[sys].mu.dw_outpow_greater.gate =  127;
		pt->t_warn[sys].mu.dw_iso.gate            = -128;
		pt->t_warn[sys].mu.tmp_greater.gate       = -128;
		pt->t_warn[sys].mu.other.gate             = -128;
		
		pt->t_warn[sys].ru.up_inpow_less.en       = ENABLE;
		pt->t_warn[sys].ru.up_inpow_greater.en    = ENABLE;
		pt->t_warn[sys].ru.up_outpow_less.en      = ENABLE;
		pt->t_warn[sys].ru.up_outpow_greater.en   = ENABLE;
		pt->t_warn[sys].ru.up_iso.en 		      = ENABLE;
		pt->t_warn[sys].ru.dw_inpow_less.en  	  = ENABLE;
		pt->t_warn[sys].ru.dw_inpow_greater.en    = ENABLE;
		pt->t_warn[sys].ru.dw_outpow_less.en 	  = ENABLE;
		pt->t_warn[sys].ru.dw_outpow_greater.en   = ENABLE;
		pt->t_warn[sys].ru.dw_iso.en              = ENABLE;
		pt->t_warn[sys].ru.tmp_greater.en         = ENABLE;
		pt->t_warn[sys].ru.other.en               = ENABLE;
		
		pt->t_warn[sys].ru.up_inpow_less.gate     = -128;
		pt->t_warn[sys].ru.up_inpow_greater.gate  =  127;
		pt->t_warn[sys].ru.up_outpow_less.gate    = -128;
		pt->t_warn[sys].ru.up_outpow_greater.gate =  127;
		pt->t_warn[sys].ru.up_iso.gate            = -128;
		pt->t_warn[sys].ru.dw_inpow_less.gate  	  = -128;
		pt->t_warn[sys].ru.dw_inpow_greater.gate  =  127;
		pt->t_warn[sys].ru.dw_outpow_less.gate 	  = -128;
		pt->t_warn[sys].ru.dw_outpow_greater.gate =  127;
		pt->t_warn[sys].ru.dw_iso.gate            = -128;
		pt->t_warn[sys].ru.tmp_greater.gate       = -128;
		pt->t_warn[sys].ru.other.gate             = -128;
	}
    pt->crc = crc32((uint8_t *)pt, sizeof(flash_dev_para_t) - 4);
}


/*****************************************************************************
* @Name	  : flash_dev_para_aligent
* @Brief  : 设备参数读写对齐
* @Param  : rw: [输入/出]
* @Retval :
* @Author :
* @Data   : 2023-11-29
* @Note   :
*****************************************************************************/
static void flash_dev_para_aligent(uint8_t rw)
{
    flash_dev_para_t * pt = &t_flash_dev_para;
    DevPublicPara_T * pt_pub = &tDevPublicPara;
	RF_SYS_E sys = RF_SYS_1;
	
    switch(rw)
    {
        case READ:
            rtt_printf(RTT_FLASH, RTT_TEXT_GREEN"[%s read L%d]: read device Para from ex_flash...\n\n", __MODULE__, __LINE__);

            pt->head					   = START_DEVICE_BYTE;             //起始字节 2

            pt_pub->ucDevMode 			   = pt->t_dev.work_mode; 			//工作模式
            pt_pub->tSlaveDevNum.ucTheoNum = pt->t_dev.slave_dev_num;		//从机数量
            pt_pub->ucRstByNetTime 		   = pt->t_dev.rst_time_by_net;		//根据网络定时重启时间
            pt_pub->ucRstByRunTime 		   = pt->t_dev.rst_by_run_time;		//重启间隔
            pt_pub->ucLedPlan 			   = pt->t_dev.led_plan;			//指示灯方案
            pt_pub->ucLedSwitch 		   = pt->t_dev.led_switch;			//指示灯开关
            pt_pub->scTempOffset 		   = pt->t_dev.tmp_offset;			//温度校准值
			pt_pub->ucWarnEn			   = pt->t_dev.warn_en;				//告警使能
		    pt_pub->ucWarnSaveEn           = pt->t_dev.warn_save_en;		//告警状态保存使能
            tDevFactoryPara.devType = pt->t_fact.dev_type;                  //设备类型
            memcpy(tDevFactoryPara.factoryId,pt->t_fact.factory_id,sizeof(pt->t_fact.factory_id));  //厂商代码
            memcpy(tDevFactoryPara.devMode, pt->t_fact.dev_model, sizeof(pt->t_fact.dev_model));    //设备型号
//            memcpy(tDevFactoryPara.sn, pt->t_fact.sn, sizeof(tDevFactoryPara.sn));              // sn

            memcpy(&tOmcPara, &pt->t_omc, sizeof(pt->t_omc));                // 网管参数

            for( sys = RF_SYS_1; sys < RF_SYS_NUM; sys++)
            {
                t_rf_sys[ sys ].t_public.tmpOffset = pt->t_sys_pub[ sys ].tmp_offset;   //温度校准值
                t_rf_sys[ sys ].t_public.warnEn    = pt->t_sys_pub[ sys ].warn_en;      //允许告警使能
                t_rf_sys[ sys ].t_public.opleEn    = pt->t_sys_pub[ sys ].oplc_en;		//开环工控使能
                t_rf_sys[ sys ].t_public.rsrpEn    = pt->t_sys_pub[ sys ].rsrp_en;		//rsrp使能
                t_rf_sys[ sys ].t_public.tcEn 	   = pt->t_sys_pub[ sys ].tmp_en;		//温补使能
                t_rf_sys[ sys ].t_public.silenceEn = pt->t_sys_pub[ sys ].silence_en;	//静默使能
                t_rf_sys[ sys ].t_public.isoEn     = pt->t_sys_pub[ sys ].iso_en;		//隔离度检测使能
                t_rf_sys[ sys ].t_public.tRfSwitch.ucNormalState = pt->t_sys_pub[ sys ].rf_switch;	//射频开关状态

            }

            for(uint8_t band = BAND_3; band <= BAND_40; band++)
            {
				  memcpy( &t_rf_4g_save_para[ band ], &pt->t_rf_ch_para[ band ], sizeof(para_rf_ch_t) );
            }
			
			sys = RF_SYS_2;
			t_rf_sys[ sys ].t_public.exist                       = pt->t_rf_ch_para[ BAND_41 ].exist;
			t_rf_sys[ sys ].t_public.valid_band		   	         = BAND_41;
			t_rf_sys[ sys ].t_public.valid_bw  		             = pt->t_rf_ch_para[ BAND_41 ].band_width;
			t_rf_sys[ sys ].t_public.valid_freq   		         = pt->t_rf_ch_para[ BAND_41 ].freq;
            t_rf_sys[ sys ].t_ch[RF_CH_UP].tAtt.ucManuCal        = pt->t_rf_ch_para[ BAND_41 ].up_gain_offset;
            t_rf_sys[ sys ].t_ch[RF_CH_DW].tAtt.ucManuCal        = pt->t_rf_ch_para[ BAND_41 ].dw_gain_offset;
			t_rf_sys[ sys ].t_ch[RF_CH_UP].tAlc.scAlcPointOffset = pt->t_rf_ch_para[ BAND_41 ].up_pow_offset;
			t_rf_sys[ sys ].t_ch[RF_CH_DW].tAlc.scAlcPointOffset = pt->t_rf_ch_para[ BAND_41 ].dw_pow_offset;
            tOplc[sys].link_gate 						         = pt->t_rf_ch_para[ BAND_41 ].oplc_gate;
            tOplc[sys].gain_offset 					   	         = pt->t_rf_ch_para[ BAND_41 ].oplc_gain_offset;
            tOplc[sys].pow_offset  					   	         = pt->t_rf_ch_para[ BAND_41 ].oplc_pow_offset;
			memcpy( &t_rf_sys[ sys ].t_public.scan_freq, &pt->t_rf_ch_para[ BAND_41 ].scan_freq, 64 );
			
			sys = RF_SYS_3;
			t_rf_sys[ sys ].t_public.exist                       = pt->t_rf_ch_para[ BAND_41M ].exist;
			t_rf_sys[ sys ].t_public.valid_band		   	         = BAND_41M;
			t_rf_sys[ sys ].t_public.valid_bw  		   	         = pt->t_rf_ch_para[ BAND_41M ].band_width;
			t_rf_sys[ sys ].t_public.valid_freq   		         = pt->t_rf_ch_para[ BAND_41M ].freq;
            t_rf_sys[ sys ].t_ch[RF_CH_UP].tAtt.ucManuCal        = pt->t_rf_ch_para[ BAND_41M ].up_gain_offset;
            t_rf_sys[ sys ].t_ch[RF_CH_DW].tAtt.ucManuCal        = pt->t_rf_ch_para[ BAND_41M ].dw_gain_offset;
			t_rf_sys[ sys ].t_ch[RF_CH_UP].tAlc.scAlcPointOffset = pt->t_rf_ch_para[ BAND_41M ].up_pow_offset;
			t_rf_sys[ sys ].t_ch[RF_CH_DW].tAlc.scAlcPointOffset = pt->t_rf_ch_para[ BAND_41M ].dw_pow_offset;       
            tOplc[sys].link_gate 						         = pt->t_rf_ch_para[ BAND_41M ].oplc_gate ;
            tOplc[sys].gain_offset 					   	         = pt->t_rf_ch_para[ BAND_41M ].oplc_gain_offset;
            tOplc[sys].pow_offset 						         = pt->t_rf_ch_para[ BAND_41M ].oplc_pow_offset;
			memcpy( &t_rf_sys[ sys ].t_public.scan_freq, &pt->t_rf_ch_para[ BAND_41M ].scan_freq, 64 );
			
            #if LOCAL_MODULE_TYPE == _MU
            cat1_omc_para_set(&pt->t_omc);    //cat1的omc参数初始化读取 
			memcpy( &t_5g_modem_config, &pt->t_net_config, sizeof( NetConfig_T ) );	//5g 模组配置参数
            
            t_dev_warn[0].t_warn_ch1_dw_inpow_greater.scGate = pt->t_warn[RF_SYS_1].mu.dw_inpow_greater.gate;
            t_dev_warn[0].t_warn_ch1_dw_inpow_less.scGate = pt->t_warn[RF_SYS_1].mu.dw_inpow_less.gate;
            t_dev_warn[0].t_warn_ch2_dw_inpow_greater.scGate = pt->t_warn[RF_SYS_2].mu.dw_inpow_greater.gate;
            t_dev_warn[0].t_warn_ch2_dw_inpow_less.scGate = pt->t_warn[RF_SYS_2].mu.dw_inpow_less.gate;
            t_dev_warn[0].t_warn_ch3_dw_inpow_greater.scGate = pt->t_warn[RF_SYS_3].mu.dw_inpow_greater.gate;
            t_dev_warn[0].t_warn_ch3_dw_inpow_less.scGate = pt->t_warn[RF_SYS_3].mu.dw_inpow_less.gate;

            for(int i=1; i<=WARN_SLV_DEV_MAX_NUM; i++)
            {
                t_dev_warn[i].t_warn_ch1_dw_inpow_greater.scGate = pt->t_warn[RF_SYS_1].ru.dw_inpow_greater.gate;
                t_dev_warn[i].t_warn_ch1_dw_inpow_less.scGate = pt->t_warn[RF_SYS_1].ru.dw_inpow_less.gate;
                t_dev_warn[i].t_warn_ch2_dw_inpow_greater.scGate = pt->t_warn[RF_SYS_2].ru.dw_inpow_greater.gate;
                t_dev_warn[i].t_warn_ch2_dw_inpow_less.scGate = pt->t_warn[RF_SYS_2].ru.dw_inpow_less.gate;
                t_dev_warn[i].t_warn_ch3_dw_inpow_greater.scGate = pt->t_warn[RF_SYS_3].ru.dw_inpow_greater.gate;
                t_dev_warn[i].t_warn_ch3_dw_inpow_less.scGate = pt->t_warn[RF_SYS_3].ru.dw_inpow_less.gate;
            }

            #endif
            break;

        case WRITE:
            rtt_printf(RTT_FLASH, RTT_TEXT_YELLOW"[%s write L%d]: write device Para\n", __MODULE__, __LINE__);

            pt->head					= START_DEVICE_BYTE;                //起始字节 2

            pt->t_dev.work_mode			= pt_pub->ucDevMode;				//工作模式
            pt->t_dev.slave_dev_num		= pt_pub->tSlaveDevNum.ucTheoNum;	//从机数量
            pt->t_dev.rst_time_by_net	= pt_pub->ucRstByNetTime;			//根据网络定时重启时间
            pt->t_dev.rst_by_run_time	= pt_pub->ucRstByRunTime;			//重启间隔
            pt->t_dev.led_plan			= pt_pub->ucLedPlan;				//指示灯方案
            pt->t_dev.led_switch		= pt_pub->ucLedSwitch;				//指示灯开关
            pt->t_dev.tmp_offset 		= pt_pub->scTempOffset;				//温度校准值
			pt->t_dev.warn_en			= pt_pub->ucWarnEn;					//告警使能
			pt->t_dev.warn_save_en      = pt_pub->ucWarnSaveEn;				//告警状态保存使能
            memset(pt->t_dev.reserve, 0xFF, sizeof(pt->t_dev.reserve));     //保留

            pt->t_fact.dev_type	  		= tDevFactoryPara.devType;			//设备类型
            memcpy(pt->t_fact.factory_id, tDevFactoryPara.factoryId, sizeof(pt->t_fact.factory_id));    //厂商代码
            memcpy(pt->t_fact.dev_model, tDevFactoryPara.devMode, sizeof(pt->t_fact.dev_model));    //设备型号
            memcpy(pt->t_fact.sn, tDevFactoryPara.sn, sizeof(tDevFactoryPara.sn));              // sn
            memset(pt->t_fact.reserve, 0xFF, sizeof(pt->t_fact.reserve));

            memcpy(&pt->t_omc, &tOmcPara, sizeof(tOmcPara));                // 网管参数

            for( sys = RF_SYS_1; sys < RF_SYS_NUM; sys++)
            {
                pt->t_sys_pub[ sys ].tmp_offset 	= t_rf_sys[ sys ].t_public.tmpOffset;	//温度校准值
                pt->t_sys_pub[ sys ].warn_en  		= t_rf_sys[ sys ].t_public.warnEn;		//允许告警使能
                pt->t_sys_pub[ sys ].oplc_en  		= t_rf_sys[ sys ].t_public.opleEn;		//开环工控使能
                pt->t_sys_pub[ sys ].rsrp_en  		= t_rf_sys[ sys ].t_public.rsrpEn;		//rsrp使能
                pt->t_sys_pub[ sys ].tmp_en  		= t_rf_sys[ sys ].t_public.tcEn;		//温补使能
                pt->t_sys_pub[ sys ].silence_en  	= t_rf_sys[ sys ].t_public.silenceEn;	//静默使能
                pt->t_sys_pub[ sys ].iso_en  		= t_rf_sys[ sys ].t_public.isoEn;		//隔离度检测使能
                pt->t_sys_pub[ sys ].rf_switch 		= t_rf_sys[ sys ].t_public.tRfSwitch.ucNormalState;	//射频开关状态
                memset(pt->t_sys_pub[sys].reserve, 0xFF, sizeof(pt->t_sys_pub[sys].reserve));
            }

            for(uint8_t band = BAND_3; band <= BAND_40; band++)
            {
                pt->t_rf_ch_para[ band ].up_gain_offset    = t_rf_4g_save_para[ band ].up_gain_offset;
                pt->t_rf_ch_para[ band ].dw_gain_offset    = t_rf_4g_save_para[ band ].dw_gain_offset;
                pt->t_rf_ch_para[ band ].oplc_gate 		   = t_rf_4g_save_para[ band ].oplc_gate;
                pt->t_rf_ch_para[ band ].oplc_gain_offset  = t_rf_4g_save_para[ band ].oplc_gain_offset;
                pt->t_rf_ch_para[ band ].oplc_pow_offset   = t_rf_4g_save_para[ band ].oplc_pow_offset;
            }
			
			for(uint8_t band = BAND_3; band <= BAND_40; band++)
            {
				  memcpy( &pt->t_rf_ch_para[ band ], &t_rf_4g_save_para[ band ], sizeof(para_rf_ch_t) );
            }
			
			sys = RF_SYS_2;
            pt->t_rf_ch_para[ BAND_41 ].exist            = t_rf_sys[ sys ].t_public.exist;
            pt->t_rf_ch_para[ BAND_41 ].up_gain_offset 	 = t_rf_sys[ sys ].t_ch[RF_CH_UP].tAtt.ucManuCal;
            pt->t_rf_ch_para[ BAND_41 ].dw_gain_offset 	 = t_rf_sys[ sys ].t_ch[RF_CH_DW].tAtt.ucManuCal;
			pt->t_rf_ch_para[ BAND_41 ].up_pow_offset    = t_rf_sys[ sys ].t_ch[RF_CH_UP].tAlc.scAlcPointOffset;
			pt->t_rf_ch_para[ BAND_41 ].dw_pow_offset    = t_rf_sys[ sys ].t_ch[RF_CH_DW].tAlc.scAlcPointOffset; 
            pt->t_rf_ch_para[ BAND_41 ].oplc_gate 		 = tOplc[ sys ].link_gate;
            pt->t_rf_ch_para[ BAND_41 ].oplc_gain_offset = tOplc[ sys ].gain_offset;
            pt->t_rf_ch_para[ BAND_41 ].oplc_pow_offset  = tOplc[ sys ].pow_offset;
			pt->t_rf_ch_para[ BAND_41 ].freq             = t_rf_sys[ sys ].t_public.valid_freq;
			pt->t_rf_ch_para[ BAND_41 ].band_width       = t_rf_sys[ sys ].t_public.valid_bw;
            memcpy(&pt->t_rf_ch_para[ BAND_41 ].scan_freq, &t_rf_sys[ sys ].t_public.scan_freq, 64);

			sys = RF_SYS_3;
            pt->t_rf_ch_para[ BAND_41M ].exist            = t_rf_sys[ sys ].t_public.exist;
            pt->t_rf_ch_para[ BAND_41M ].up_gain_offset   = t_rf_sys[ sys ].t_ch[RF_CH_UP].tAtt.ucManuCal;
            pt->t_rf_ch_para[ BAND_41M ].dw_gain_offset   = t_rf_sys[ sys ].t_ch[RF_CH_DW].tAtt.ucManuCal;
			pt->t_rf_ch_para[ BAND_41M ].up_pow_offset    = t_rf_sys[ sys ].t_ch[RF_CH_UP].tAlc.scAlcPointOffset;
			pt->t_rf_ch_para[ BAND_41M ].dw_pow_offset    = t_rf_sys[ sys ].t_ch[RF_CH_DW].tAlc.scAlcPointOffset; 
            pt->t_rf_ch_para[ BAND_41M ].oplc_gate 		  = tOplc[sys].link_gate;
            pt->t_rf_ch_para[ BAND_41M ].oplc_gain_offset = tOplc[sys].gain_offset;
            pt->t_rf_ch_para[ BAND_41M ].oplc_pow_offset  = tOplc[sys].pow_offset;
			pt->t_rf_ch_para[ BAND_41M ].freq             = t_rf_sys[ sys ].t_public.valid_freq;
			pt->t_rf_ch_para[ BAND_41M ].band_width       = t_rf_sys[sys].t_public.valid_bw;
            memcpy( &pt->t_rf_ch_para[ BAND_41M ].scan_freq, &t_rf_sys[ sys ].t_public.scan_freq, 64 );
						
             
            #if LOCAL_MODULE_TYPE == _MU
            
			memcpy( &tOmcPara, &pt->t_omc, sizeof(tOmcPara));                // 网管参数
			
            //cat1的omc参数写入 
            memcpy(hCat1.apn, pt->t_omc.apn, sizeof(pt->t_omc.apn));
            memcpy(&hCat1.socketConfig[0], &pt->t_omc.ip[0], sizeof(pt->t_omc.ip[0]));
            memcpy(&hCat1.socketConfig[1], &pt->t_omc.ip[1], sizeof(pt->t_omc.ip[1]));
            memcpy( &pt->t_net_config, &t_5g_modem_config, sizeof( NetConfig_T ) );	//5g 模组配置参数
            #endif
            
            pt->t_warn[RF_SYS_1].mu.dw_inpow_greater.gate = t_dev_warn[0].t_warn_ch1_dw_inpow_greater.scGate;
            pt->t_warn[RF_SYS_1].mu.dw_inpow_less.gate = t_dev_warn[0].t_warn_ch1_dw_inpow_less.scGate;
            pt->t_warn[RF_SYS_2].mu.dw_inpow_greater.gate = t_dev_warn[0].t_warn_ch2_dw_inpow_greater.scGate;
            pt->t_warn[RF_SYS_2].mu.dw_inpow_less.gate = t_dev_warn[0].t_warn_ch2_dw_inpow_less.scGate;
            pt->t_warn[RF_SYS_3].mu.dw_inpow_greater.gate = t_dev_warn[0].t_warn_ch3_dw_inpow_greater.scGate;
            pt->t_warn[RF_SYS_3].mu.dw_inpow_less.gate = t_dev_warn[0].t_warn_ch3_dw_inpow_less.scGate;

            pt->t_warn[RF_SYS_1].ru.dw_inpow_greater.gate = t_dev_warn[1].t_warn_ch1_dw_inpow_greater.scGate;
            pt->t_warn[RF_SYS_1].ru.dw_inpow_less.gate = t_dev_warn[1].t_warn_ch1_dw_inpow_less.scGate;
            pt->t_warn[RF_SYS_2].ru.dw_inpow_greater.gate = t_dev_warn[1].t_warn_ch2_dw_inpow_greater.scGate;
            pt->t_warn[RF_SYS_2].ru.dw_inpow_less.gate = t_dev_warn[1].t_warn_ch2_dw_inpow_less.scGate;
            pt->t_warn[RF_SYS_3].ru.dw_inpow_greater.gate = t_dev_warn[1].t_warn_ch3_dw_inpow_greater.scGate;
            pt->t_warn[RF_SYS_3].ru.dw_inpow_less.gate = t_dev_warn[1].t_warn_ch3_dw_inpow_less.scGate;

			pt->crc = crc32((uint8_t *)pt, sizeof(flash_dev_para_t) - 4);
			 
            break;

        default:
            break;
    }
}


/*****************************************************************************
* @Name	  : flash_dev_rw_fail
* @Brief  : 连续读写失败后设置默认数据
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2023-11-29
* @Note   :
*****************************************************************************/
void flash_dev_rw_fail(void)
{
    flash_dev_para_restore_factory_setting();
    flash_dev_para_aligent(READ);
}


/*****************************************************************************
* @Name	  : flash_restore_dev_factory
* @Brief  : 恢复设备出厂设置
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2023-12-30
* @Note   :
*****************************************************************************/
void flash_restore_dev_factory(void)
{
    flash_dev_para_t * pt = &t_flash_dev_para;
    uint32_t addr = get_dev_run_para_save_addr();
    uint16_t len  = sizeof(flash_dev_para_t);
    uint16_t flash_sector = spi_flash.flash_addr_to_sector(addr);

    rtt_printf(RTT_FLASH, RTT_TEXT_MAGENTA"[%s restore_dev_factory L%d]: restore dev factory of ex_flash\n", __MODULE__, __LINE__);
    flash_dev_para_restore_factory_setting();

    taskENTER_CRITICAL();
    spi_flash.flash_erase_sector(flash_sector);
    spi_flash.flash_write((uint8_t *)pt, addr, len);  //写入数据
    taskEXIT_CRITICAL();
}

/*****************************************************************************
* @Name	  : flash_restore_warn_para
* @Brief  : 恢复告警参数
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2024-4-6
* @Note   :
*****************************************************************************/
void flash_restore_warn_para(void)
{
    dev_warn_save_t *pt = t_warn_save;

    uint32_t addr = get_dev_warn_state_save_addr();
    uint16_t len  = sizeof(t_warn_save ) * ( WARN_SLV_DEV_MAX_NUM + 1);
    uint16_t flash_sector = spi_flash.flash_addr_to_sector(addr);

    memset( t_warn_save, NORMAL, sizeof( dev_warn_save_t )*(WARN_SLV_DEV_MAX_NUM + 1) );

    taskENTER_CRITICAL();
    spi_flash.flash_erase_sector(flash_sector);
    spi_flash.flash_write((uint8_t *)pt, addr, len);  //写入数据
    taskEXIT_CRITICAL();
}



/*****************************************************************************
* @Name	  : flash_dev_para_rw
* @Brief  : 设备参数读写操作
* @Param  : rw: [输入/出]
* @Retval :
* @Author :
* @Data   : 2023-11-29
* @Note   :
*****************************************************************************/
void flash_dev_para_rw(uint8_t rw)
{
    flash_dev_para_t * pt = &t_flash_dev_para;

    uint32_t crc = 0;
    uint32_t addr = get_dev_run_para_save_addr();
    uint16_t len  = sizeof(flash_dev_para_t);

    uint16_t flash_sector = spi_flash.flash_addr_to_sector(addr);        //bi_flash_addr_to_sector( addr );	//flash 扇区地址

    uint8_t  err = 0;
    if(READ == rw)
    {
        while(err < 3)
        {
            //读数据
            taskENTER_CRITICAL();
            spi_flash.flash_read((uint8_t *)pt, addr, len);
            taskEXIT_CRITICAL();

            //校验
            if(pt->head != START_DEVICE_BYTE)	//无数据, 需要初始化
            {
                rtt_printf(RTT_FLASH, RTT_TEXT_MAGENTA"[%s info L%d]: ex_flash no device Para\n", __MODULE__, __LINE__);
                flash_restore_dev_factory();
            }

            crc = crc32((uint8_t *)pt, len - 4);

            if(crc == pt->crc)	//读写正确, 更新数据, 退出循环
            {
                flash_dev_para_aligent(READ);
                break;
            }

            err++;
            if(err == 3)	//连续读出错误
            {
                rtt_printf(RTT_FLASH, RTT_TEXT_RED"[%s read error L%d]: device Para continuous reading of ex_flash error\n", __MODULE__, __LINE__);
                taskENTER_CRITICAL();
                spi_flash.flash_erase_sector(flash_sector);
                flash_dev_rw_fail();
                spi_flash.flash_write((uint8_t *)pt, addr, len);
                taskEXIT_CRITICAL();
            }
        }
    }
    else if(WRITE == rw)
    {
        err = 0;
        while(err < 3)
        {
            flash_dev_para_aligent(WRITE);
            taskENTER_CRITICAL();
            spi_flash.flash_erase_sector(flash_sector);
            spi_flash.flash_write((uint8_t *)pt, addr, len);  //写入数据
            spi_flash.flash_read((uint8_t *)pt, addr, len);   //读出数据
            taskEXIT_CRITICAL();
            crc = crc32((uint8_t *)pt, len - 4);
            if(crc == pt->crc)	//读写正确, 更新数据, 退出循环
                break;

            err++;
            if(err == 3)
            {
                rtt_printf(RTT_FLASH, RTT_TEXT_RED"[%s write error L%d]: device Para continuous writing of ex_flash error\n", __MODULE__, __LINE__);
                taskENTER_CRITICAL();
                spi_flash.flash_erase_sector(flash_sector);
                flash_dev_rw_fail();
                spi_flash.flash_write((uint8_t *)pt, addr, len);
                taskEXIT_CRITICAL();
            }
        }
    }
    else
    {}
}

