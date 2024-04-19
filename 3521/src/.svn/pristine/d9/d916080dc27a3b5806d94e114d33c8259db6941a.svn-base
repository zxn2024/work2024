/***************************** Copyright ****************************************
*
* (C) Copyright 2023, Shaanxi Tianji Communication
*  All Rights Reserved
*
* FileName   : bsp_para_load.c
* Version    : none
* Author     :
* Date       : 2023-11-13
* Description: none
* 功率表表按带宽区分, 带宽内含有上下行数据
* 衰减表按频段分, 不区分带宽
* 8668校准参数按频段分, 然后每5个增益点校准一次, 建议一次性全部读出
* 射频参数按照频段分
*******************************************************************************/
#include "main.h"
#include "system_config.h"

#include "bsp_rf_pow.h"
#include "bsp_rf_cali.h"
#include "bsp_flash_addr.h"
#include "bsp_config.h"
#include "att_analog.h"
#include "bsp_para_load.h"
#include "bi_flash.h"
#include "bsp_testmode_save.h"
#include "bsp_rf_global.h"
#include "bsp_rf_866x_default_set.h"
#include "bsp_public_drv.h"

/*---------------------- 端口定义 --------------------------------------*/


/*---------------------- 宏定义  ---------------------------------------*/


/*---------------------- 结构体 ----------------------------------------*/


/*---------------------- 常量 ------------------------------------------*/


/*---------------------- 全局变量 --------------------------------------*/


/*---------------------- 局部变量 --------------------------------------*/


/*---------------------- 声明 ------------------------------------------*/
// #if(PRODUCT_MODEL == CMDT3521C)
// __align(32) tm_cali_rf_para t_rf_4g_cali_para[ BAND_END ] __attribute__((section("CCMRAM"))); //用于flash保存的4g 4选一校准参数
// #else
// tm_cali_rf_para  t_rf_4g_cali_para[ BAND_END ];	//用于flash保存的4g 4选一校准参数
// #endif
tm_cali_rf_para  t_rf_4g_cali_para[ BAND_END ];	//用于flash保存的4g 4选一校准参数
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ HAL ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/





/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DRV ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
POW_TABLE_PARA_T t_test_pow_test;
Analog_table_t   t_test_full_att_test;
Analog_table_t   t_test_half_att_test;

//void test_table_init( void )
//{
//	uint16_t pow[] = { 264,  277,  290,  306,  321,  338,  355,  373,  392,  412,  
//					 432,  453,  475,  494,  515,  536,  557,  579,  601,  623,  
//					 643,  664,  686,  709,  728,  750,  772,  794,  815,  836,  
//					 858,  879,  902,  924,  944,  967,  989,  1010,  1034, 1055, 
//					 1078, 1099, 1119, 1139, 1157, 1174, 1189, 1201, 1211, 4095 };
//	
//	uint16_t half_att[] = {999, 557, 439, 385, 347, 318, 297, 280, 266, 255, 
//						   245, 237, 230, 224, 218, 213, 208, 203, 198};
//	
//	uint16_t full_att[] = {999, 673, 569, 522, 480, 447, 415, 391, 370, 351, 
//						   335, 322, 310, 299, 290, 282, 274, 267, 261, 255, 
//						   250, 245, 241, 237, 233, 229, 224, 221, 217, 213, 
//						   209, 205, 201};
//		
//	t_test_pow_test.start_pow = -8;
//	t_test_pow_test.step = 1;
//	t_test_pow_test.valid_num = 50;
//	t_test_pow_test.cali_value = 0;
//	memset( t_test_pow_test.table, 0, sizeof( t_test_half_att_test.table ) );
//	memcpy( t_test_pow_test.table, pow, sizeof( pow ) );
//	
//	t_test_half_att_test.min_number = 0;
//	t_test_half_att_test.valid_number = 19;
//	t_test_half_att_test.cali_value = 0;
//	t_test_half_att_test.step = 1;
//	memset( t_test_half_att_test.table, 0, sizeof( t_test_half_att_test.table ) );
//	memcpy( t_test_half_att_test.table, half_att, sizeof( half_att ) );
//	
//	t_test_full_att_test.min_number = 0;
//	t_test_full_att_test.valid_number = 33;
//	t_test_full_att_test.cali_value = 0;
//	t_test_full_att_test.step = 1;
//	memset( t_test_full_att_test.table, 0, sizeof( t_test_full_att_test.table ) );
//	memcpy( t_test_full_att_test.table, full_att, sizeof( full_att ) );		
//}

//void use_test_data_init_app( void )
//{
//	test_table_init();
//	
//	for( uint8_t sys=0; sys<RF_SYS_NUM; sys++ )
//	{
//		memcpy( &t_pow_table_8668[sys], &t_test_pow_test, sizeof( POW_TABLE_PARA_T ) );
//		for( uint8_t ch=0; ch<RF_CH_NUM; ch++ )
//		{
//			memcpy( &t_pow_table[sys][ch], &t_test_pow_test, sizeof( POW_TABLE_PARA_T ) );
//		}
//	}
//	
//    #if LOCAL_MODULE_TYPE == _MU
//	memcpy( &analog_table_4g_up_1, &t_test_full_att_test, sizeof( Analog_table_t ) );
//	memcpy( &analog_table_4g_up_2, &t_test_full_att_test, sizeof( Analog_table_t ) );
//	memcpy( &analog_table_4g_dw, &t_test_full_att_test, sizeof( Analog_table_t ) );
//	
//	memcpy( &analog_table_nr_up_1, &t_test_full_att_test, sizeof( Analog_table_t ) );
//	memcpy( &analog_table_nr_up_2, &t_test_full_att_test, sizeof( Analog_table_t ) );
//	memcpy( &analog_table_nr_dw, &t_test_full_att_test, sizeof( Analog_table_t ) );
//	
//	memcpy( &analog_table_mimo_up_1, &t_test_full_att_test, sizeof( Analog_table_t ) );
//	memcpy( &analog_table_mimo_up_2, &t_test_full_att_test, sizeof( Analog_table_t ) );
//	memcpy( &analog_table_mimo_dw, &t_test_full_att_test, sizeof( Analog_table_t ) );
//	#else
//	memcpy( &analog_table_4g_up, &t_test_full_att_test, sizeof( Analog_table_t ) );
//	memcpy( &analog_table_4g_dw_1, &t_test_full_att_test, sizeof( Analog_table_t ) );
//	memcpy( &analog_table_4g_dw_2, &t_test_full_att_test, sizeof( Analog_table_t ) );
//	
//	memcpy( &analog_table_nr_up, &t_test_full_att_test, sizeof( Analog_table_t ) );
//	memcpy( &analog_table_nr_dw_1, &t_test_full_att_test, sizeof( Analog_table_t ) );
//	memcpy( &analog_table_nr_dw_2, &t_test_full_att_test, sizeof( Analog_table_t ) );
//	
//	memcpy( &analog_table_mimo_up, &t_test_full_att_test, sizeof( Analog_table_t ) );
//	memcpy( &analog_table_mimo_dw_1, &t_test_full_att_test, sizeof( Analog_table_t ) );
//	memcpy( &analog_table_mimo_dw_2, &t_test_full_att_test, sizeof( Analog_table_t ) );
//    #endif
//    
//	for( uint8_t band = 0; band < BAND_END; band++ )
//	{
//		t_rf_4g_cali_para[band].coupling_gain = 0;		//耦合增益
//		t_rf_4g_cali_para[band].up_theo_gain = 70;		//上行理论增益
//		t_rf_4g_cali_para[band].up_theo_out_pow = 20;	//上行理论输出功率
//		t_rf_4g_cali_para[band].up_lna_att1 = 20;		//上行LNA1抵消ATT数值
//		t_rf_4g_cali_para[band].up_lna_att2 = 21;		//上行LNA2抵消ATT数值
//		t_rf_4g_cali_para[band].up_gain_cali = 0;		//上行增益校准
////		t_rf_4g_cali_para[band].reserve1[18];
//		t_rf_4g_cali_para[band].dw_theo_gain = 70;		//下行理论增益
//		t_rf_4g_cali_para[band].dw_theo_out_pow = 21;	//下行理论输出功率
//		t_rf_4g_cali_para[band].dw_lna_att1 = 22;		//下行LNA1抵消ATT数值
//		t_rf_4g_cali_para[band].dw_lna_att2 = 23;		//下行LNA2抵消ATT数值
//		t_rf_4g_cali_para[band].dw_gain_cali = 0;		//上行增益校准
////		t_rf_4g_cali_para[band].reserve2[20];
////		t_rf_4g_cali_para[band].up_tc_table[32];	//上行温补表
////		t_rf_4g_cali_para[band].dw_tc_table[32];	//下行温补表
////		t_rf_4g_cali_para[band].reserve3[128];
//	}
//}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ APP ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


/*****************************************************************************
* @Name	  : load_rf_flash_data
* @Brief  : 载入数据
* @Param  : sys:  系统
**			freq: 频段
**			bandwidth: 带宽, 10M, 15M, 20M...
* @Retval :
* @Author :
* @Data   : 2023-11-15
* @Note   :
*****************************************************************************/
#if LOCAL_MODULE_TYPE == _MU
static void load_rf_flash_data(uint8_t sys, uint8_t band, uint8_t bandwidth)
{
//	uint8_t  group = 0;

    tm_cali_rf_para  * pt_cali_rf = NULL;               //射频校准参数

    POW_TABLE_PARA_T * pt_powlist_up  	    = NULL;	    //上行功率表
    POW_TABLE_PARA_T * pt_powlist_dw_alc    = NULL;	    //ALC功率表
    POW_TABLE_PARA_T * pt_powlist_dw_8668   = NULL;     //8668功率表

    Analog_table_t * pt_attlist_up1         = NULL;	    //上行衰减表1
    Analog_table_t * pt_attlist_up2         = NULL;	    //上行衰减表2
    Analog_table_t * pt_attlist_dw          = NULL;	    //下行衰减表

    if(sys >= RF_SYS_NUM)
        sys = RF_SYS_1;
	
	rtt_printf( RTT_RF, RTT_TEXT_MAGENTA"---->>> flash data loading: sys = %s, %s, %dMHz\n",  rf_sys_name[sys], rf_band_name[band], bandwidth );
	
    switch(sys)	//确定加载数据存储位置
    {
        case RF_SYS_1:
            pt_powlist_up      = &t_pow_table[sys][RF_CH_UP];	//功率表
            pt_powlist_dw_alc  = &t_pow_table[sys][RF_CH_DW];
            pt_powlist_dw_8668 = &t_pow_table_8668[sys];

            pt_attlist_up1 	   = &analog_table_4g_up_1;
            pt_attlist_up2 	   = &analog_table_4g_up_2;
            pt_attlist_dw      = &analog_table_4g_dw;

            pt_cali_rf  	   = &t_rf_4g_cali_para[ band ];	//衰减表
            break;

        case RF_SYS_2:
            pt_powlist_up      = &t_pow_table[sys][RF_CH_UP];	//功率表
            pt_powlist_dw_alc  = &t_pow_table[sys][RF_CH_DW];
            pt_powlist_dw_8668 = &t_pow_table_8668[sys];

            pt_attlist_up1 	   = &analog_table_nr_up_1;
            pt_attlist_up2 	   = &analog_table_nr_up_2;
            pt_attlist_dw      = &analog_table_nr_dw;

            pt_cali_rf  	   = &t_rf_4g_cali_para[ band ];	//衰减表
            break;

        case RF_SYS_3:
            pt_powlist_up      = &t_pow_table[sys][RF_CH_UP];	//功率表
            pt_powlist_dw_alc  = &t_pow_table[sys][RF_CH_DW];
            pt_powlist_dw_8668 = &t_pow_table_8668[sys];

            pt_attlist_up1 	   = &analog_table_mimo_up_1;
            pt_attlist_up2 	   = &analog_table_mimo_up_2;
            pt_attlist_dw      = &analog_table_mimo_dw;

            pt_cali_rf  	   = &t_rf_4g_cali_para[ band ];	//衰减表
            break;

        default:
            pt_powlist_up      = &t_pow_table[RF_SYS_2][RF_CH_UP];	//功率表
            pt_powlist_dw_alc  = &t_pow_table[RF_SYS_2][RF_CH_DW];
            pt_powlist_dw_8668 = &t_pow_table_8668[RF_SYS_2];

            pt_attlist_up1 	   = &analog_table_nr_up_1;
            pt_attlist_up2 	   = &analog_table_nr_up_2;
            pt_attlist_dw      = &analog_table_nr_dw;

            pt_cali_rf  	   = &t_rf_4g_cali_para[ band ];	//衰减表
            break;
    }

    //读取数据
    tm_read_cali_rf_para( band );

    //指针定向
//    tm_cali_rf_pt_assign();


    //载入校准数据, 需要将带宽转换为编号
	uint8_t bw_id = 0;
	get_bwid_by_bw( band, bandwidth, &bw_id);
    memcpy(pt_powlist_up, &t_tm_rf->pow_table[bw_id].up1, 192);
    memcpy(pt_powlist_dw_alc,  &t_tm_rf->pow_table[bw_id].dw1, 192);
    memcpy(pt_powlist_dw_8668, &t_tm_rf->pow_table[bw_id].dw2, 192);

    memcpy(pt_attlist_up1, &t_tm_rf->att_table.up1, 192);
    memcpy(pt_attlist_up2, &t_tm_rf->att_table.up2, 192);
    memcpy(pt_attlist_dw, &t_tm_rf->att_table.dw1, 192);

    memcpy(pt_cali_rf, &t_tm_rf->rf, 256);	//读取校准参数
}
#else
static void load_rf_flash_data(uint8_t sys, uint8_t band, uint8_t bandwidth)
{
//	uint8_t  group = 0;

    tm_cali_rf_para  * pt_cali_rf = NULL;			    //射频校准参数

    POW_TABLE_PARA_T * pt_powlist_up_8668   = NULL;     //上行8668功率表
    POW_TABLE_PARA_T * pt_powlist_up_alc    = NULL;	    //上行ALC功率表
    POW_TABLE_PARA_T * pt_powlist_dw  	    = NULL;	    //下行功率表

    Analog_table_t * pt_attlist_up          = NULL;	    //上行衰减表
    Analog_table_t * pt_attlist_dw1         = NULL;	    //下行衰减表1
    Analog_table_t * pt_attlist_dw2         = NULL;	    //下行衰减表2

    if(sys >= RF_SYS_NUM)
        sys = RF_SYS_1;

    switch(sys)	//确定加载数据存储位置
    {
        case RF_SYS_1:
            pt_powlist_up_8668 = &t_pow_table_8668[sys];        //功率表
            pt_powlist_up_alc  = &t_pow_table[sys][RF_CH_UP];
            pt_powlist_dw      = &t_pow_table[sys][RF_CH_DW];

            pt_attlist_up 	   = &analog_table_4g_up;
            pt_attlist_dw1 	   = &analog_table_4g_dw_1;
            pt_attlist_dw2     = &analog_table_4g_dw_2;

            pt_cali_rf  	   = &t_rf_4g_cali_para[ band ];	//衰减表
            break;

        case RF_SYS_2:
            pt_powlist_up_8668 = &t_pow_table_8668[sys];        //功率表
            pt_powlist_up_alc  = &t_pow_table[sys][RF_CH_UP];
            pt_powlist_dw      = &t_pow_table[sys][RF_CH_DW];

            pt_attlist_up 	   = &analog_table_nr_up;
            pt_attlist_dw1 	   = &analog_table_nr_dw_1;
            pt_attlist_dw2     = &analog_table_nr_dw_2;
        
            pt_cali_rf  	   = &t_rf_4g_cali_para[ band ];	//衰减表
            break;

        case RF_SYS_3:
            pt_powlist_up_8668 = &t_pow_table_8668[sys];        //功率表
            pt_powlist_up_alc  = &t_pow_table[sys][RF_CH_UP];
            pt_powlist_dw      = &t_pow_table[sys][RF_CH_DW];
            pt_attlist_up 	   = &analog_table_mimo_up;
            pt_attlist_dw1 	   = &analog_table_mimo_dw_1;
            pt_attlist_dw2     = &analog_table_mimo_dw_2;

            pt_cali_rf  	   = &t_rf_4g_cali_para[ band ];	//衰减表
            break;

        default:
            pt_powlist_up_8668 = &t_pow_table_8668[RF_SYS_2];   //功率表
            pt_powlist_up_alc  = &t_pow_table[RF_SYS_2][RF_CH_UP];
            pt_powlist_dw      = &t_pow_table[RF_SYS_2][RF_CH_DW];

            pt_attlist_up 	   = &analog_table_nr_up;
            pt_attlist_dw1 	   = &analog_table_nr_dw_1;
            pt_attlist_dw2     = &analog_table_nr_dw_2;

            pt_cali_rf  	   = &t_rf_4g_cali_para[ band ];	//衰减表
            break;
    }
    
    //读取数据
    tm_read_cali_rf_para( band );

    //指针定向
//    tm_cali_rf_pt_assign();

    //载入校准数据, 需要将带宽转换为编号
	uint8_t bw_id = 0;
	get_bwid_by_bw( band, bandwidth, &bw_id);
    
    memcpy(pt_powlist_up_alc,  &t_tm_rf->pow_table[bw_id].up1, 192);
    memcpy(pt_powlist_up_8668, &t_tm_rf->pow_table[bw_id].up2, 192);
    memcpy(pt_powlist_dw, &t_tm_rf->pow_table[bw_id].dw1, 192);

    memcpy(pt_attlist_up, &t_tm_rf->att_table.up1, 192);
    memcpy(pt_attlist_dw1, &t_tm_rf->att_table.dw1, 192);
    memcpy(pt_attlist_dw2, &t_tm_rf->att_table.dw2, 192);

    memcpy(pt_cali_rf, &t_tm_rf->rf, 256);
    
//    use_test_data_init_app();	//使用测试数据填充
}
#endif
    
    

/*****************************************************************************
* @Name	  : load_factory_cali_data
* @Brief  : 载入4G通道出厂校准参数
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2023-11-17
* @Note   :
*****************************************************************************/
void load_4g_factory_cali_data(void)
{
	RfSysPublic_T *pt = &t_rf_sys[RF_SYS_1].t_public;
    load_rf_flash_data(RF_SYS_1, pt->valid_band, pt->valid_bw);
}

/*****************************************************************************
* @Name	  : load_5g_factory_cali_data
* @Brief  : 载入4G通道出厂校准参数
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2023-12-01
* @Note   :
*****************************************************************************/
void load_5g_factory_cali_data(void)
{
	RfSysPublic_T *pt = &t_rf_sys[RF_SYS_2].t_public;
    load_rf_flash_data(RF_SYS_2, 4, pt->valid_bw);
	
	pt = &t_rf_sys[RF_SYS_3].t_public;
    load_rf_flash_data(RF_SYS_3, 5, pt->valid_bw);
}

/*****************************************************************************
* @Name	  : get_rf_cali_para_lna_att
* @Brief  : 获取校准数据LNA旁路抵消ATT的数值
* @Param  : band: [输入/出] 
**			 ch: [输入/出] 
**			 lna_id: [输入/出] 
* @Retval : 
* @Author : 
* @Data   : 2024-01-22
* @Note   :
*****************************************************************************/
uint8_t get_rf_cali_para_lna_att( uint8_t band, uint8_t ch, uint8_t lna_id )
{
	if( RF_CH_UP == ch )
	{
		return ( 1==lna_id ) ? t_rf_4g_cali_para[ band ].up_lna_att1 : t_rf_4g_cali_para[ band ].up_lna_att2;
	}
	else if( RF_CH_DW == ch )
	{
		return ( 1==lna_id ) ? t_rf_4g_cali_para[ band ].dw_lna_att1 : t_rf_4g_cali_para[ band ].dw_lna_att2;
	}
	else
	{}
		
	return 0;
}

