///***************************** Copyright ****************************************
//*
//* (C) Copyright 2022, Shaanxi Tianji Communication
//*  All Rights Reserved
//*
//* FileName   : bsp_rf_testmode.c
//* Version    : none
//* Author     : ZhangWei
//* Date       : 2022-09-13
//* Description: none
//*    增益校准: 有效值范围 -3 ~ +3, 设置值>0, 增益减小,
//*                                  设置值<0, 增益增大
//*    功率校准: 有效值范围 -3 ~ +3, 设置值>0, 功率表数值比实际功率小
//                                   设置值<0, 功率表数值比实际功率大
//*
//*******************************************************************************/
//#include "main.h"
//#include "system_config.h"
//#include "bsp_public_drv.h"

//#include "bsp_rf_testmode.h"
//#include "FreeRTOS.h"
//#include "event_groups.h"
//#include "rtt.h"
//#include "bsp_rf_ctr.h"
//#include "bsp_adc.h"
//#include "bsp_rf_lna.h"
//#include "bsp_rf_pa.h"

///*---------------------- 端口定义 --------------------------------------*/


///*---------------------- 宏定义  ---------------------------------------*/

////进入测试模式后的功能选择
////              01:采集CH1上行第一级衰减
////              02:采集CH1下行第一级衰减
////              03:采集CH1上行第二级衰减
////              04:采集CH1下行第二级衰减
////              11:采集CH2上行第一级衰减
////              12:采集CH2下行第一级衰减
////              13:采集CH2上行第二级衰减
////              14:采集CH2下行第二级衰减
////              21:采集AD1,CH1上行
////              22:采集AD2,CH1下行
////              31:采集AD3,CH2上行
////              32:采集AD4,CH2下行
////进入校准模式, 81:校准CH1上行增益,
////              82:校准CH2上行增益
////              88:校准CH1上行输出功率表,
////              89:校准CH2上行输出功率表
////              91:校准CH1下行增益
////              92:校准CH2下行增益
////              98:校准CH1下行输出功率表,
////              99:校准CH2下行输出功率表
////              F0: 测试指示灯, FA:保存数据, FB:退出测试模式
//#define TEST_TYPE_COLLECT_CH1_UP_ATT1_TABLE   (0x01)
//#define TEST_TYPE_CLOOECT_CH1_DW_ATT1_TABLE   (0x02)
//#define TEST_TYPE_COLLECT_CH1_UP_ATT2_TABLE   (0x03)
//#define TEST_TYPE_CLOOECT_CH1_DW_ATT2_TABLE   (0x04)

//#define TEST_TYPE_CLOOECT_CH2_UP_ATT1_TABLE   (0x11)
//#define TEST_TYPE_CLOOECT_CH2_DW_ATT1_TABLE   (0x12)
//#define TEST_TYPE_CLOOECT_CH2_UP_ATT2_TABLE   (0x13)
//#define TEST_TYPE_CLOOECT_CH2_DW_ATT2_TABLE   (0x14)

//#define TEST_TYPE_CLOOECT_CH1_UP_POW_TABLE   (0x21)
//#define TEST_TYPE_CLOOECT_CH1_DW_POW_TABLE   (0x22)

//#define TEST_TYPE_CLOOECT_CH2_UP_POW_TABLE   (0x31)
//#define TEST_TYPE_CLOOECT_CH2_DW_POW_TABLE   (0x32)

//#define TEST_TYPE_RF_CH1_UP_OPEN			 (0x40)	//CH1 上行常开, 没有ALC, 可设置增益 / LNA / PA
//#define TEST_TYPE_RF_CH1_DW_OPEN			 (0x41)	//CH1 上行常开, 没有ALC, 可设置增益 / LNA / PA
//#define TEST_TYPE_RF_CH2_UP_OPEN			 (0x42)	//CH1 上行常开, 没有ALC, 可设置增益 / LNA / PA
//#define TEST_TYPE_RF_CH2_DW_OPEN			 (0x43)	//CH1 上行常开, 没有ALC, 可设置增益 / LNA / PA

//#define TEST_TYPE_CALIBRATION_CH1_UP_GAIN    (0x81)
//#define TEST_TYPE_CALIBRATION_CH2_UP_GAIN    (0x82)
//#define TEST_TYPE_CALIBRATION_CH1_UP_POW     (0x88)
//#define TEST_TYPE_CALIBRATION_CH2_UP_POW     (0x89)
//#define TEST_TYPE_CALIBRATION_CH1_DW_GAIN    (0x91)
//#define TEST_TYPE_CALIBRATION_CH2_DW_GAIN    (0x92)
//#define TEST_TYPE_CALIBRATION_CH1_DW_POW     (0x98)
//#define TEST_TYPE_CALIBRATION_CH2_DW_POW     (0x99)

//#define TEST_TYPE_LED                        (0xF0)

////监控UID映射
//#define TM_UID_IN_TEST_MODE         (0xFF01)    //0xFF01 进入测试模式

//#define CAB_UID_CH1_UPOW_OFFSET     (0xFF20)    //设置CH1上行输出功率表偏移量
//#define CAB_UID_CH1_DPOW_OFFSET     (0xFF21)	//设置CH1下行输出功率表偏移量
//#define CAB_UID_CH2_UPOW_OFFSET     (0xFF22)	//设置CH2上行输出功率表偏移量
//#define CAB_UID_CH2_DPOW_OFFSET     (0xFF23)	//设置CH2下行输出功率表偏移量
//#define CAB_UID_CH1_UGAIN_OFFSET    (0xFF28)    //设置CH1上行增益偏移量
//#define CAB_UID_CH1_DGAIN_OFFSET    (0xFF29)	//设置CH1下行增益偏移量
//#define CAB_UID_CH2_UGAIN_OFFSET    (0xFF2A)	//设置CH2上行增益偏移量
//#define CAB_UID_CH2_DGAIN_OFFSET    (0xFF2B)	//设置CH2下行增益偏移量

//#define TM_UID_SET_ATT_VALUE        (0x0F58)    //设置模拟衰减量
//#define TM_UID_SET_LNA_STATE		(0x0F59)	//设置LNA状态
//#define TM_UID_SET_PA_STATE			(0x0F5A)    //设置PA状态

////事件组标识
//#define BIT_CALIBRATION_UP_GAIN     (1U << 0)  // 校准上行增益
//#define BIT_CALIBRATION_DW_GAIN     (1U << 1)  // 校准下行增益
//#define BIT_CALIBRATION_UP_POWER    (1U << 2)  // 校准上行功率
//#define BIT_CALIBRATION_DW_POWER    (1U << 3)  // 校准下行功率
//#define BIT_COLLECT_UP_POWER        (1U << 8)  // 采集上行输出功率表
//#define BIT_COLLECT_DW_POWER        (1U << 9)  // 采集下行输出功率表
//#define BIT_COLLECT_UP_ATT1         (1U << 10) // 采集上行第一级衰减表
//#define BIT_COLLECT_UP_ATT2         (1U << 11) // 采集下行第一级衰减表
//#define BIT_COLLECT_DW_ATT1         (1U << 12) // 采集上行第二级衰减表
//#define BIT_COLLECT_DW_ATT2         (1U << 13) // 采集下行第二级衰减表
//#define BIT_TEST_RF_SWITCH_UP       (1U << 18) // 上行单通
//#define BIT_TEST_RF_SWITCH_DW       (1U << 19) // 下行单通
//#define BIT_TESTMODE_ALL            ( 0x00FFFFFF )
//#define BIT_TESTMODE_ALL_UP         (BIT_CALIBRATION_UP_GAIN | BIT_CALIBRATION_UP_POWER | BIT_COLLECT_UP_POWER | BIT_COLLECT_UP_ATT1 | BIT_COLLECT_UP_ATT2 | BIT_TEST_RF_SWITCH_UP)
//#define BIT_TESTMODE_ALL_DW         (BIT_CALIBRATION_DW_GAIN | BIT_CALIBRATION_DW_POWER | BIT_COLLECT_DW_POWER | BIT_COLLECT_DW_ATT1 | BIT_COLLECT_DW_ATT2 | BIT_TEST_RF_SWITCH_DW)


//#define HAVE_NEW_EGP_MSG() ( B_TRUE == vTestModeGetNewEgpMsg() )  //判断是否进行功能切换

///*---------------------- 结构体 & 枚举变量 -----------------------------*/


///*---------------------- 常量 ------------------------------------------*/


///*---------------------- 全局变量 --------------------------------------*/


///*---------------------- 局部变量 --------------------------------------*/
//static RunMode_E eRunMode;  //运行模式
//static RF_SYS_E  eRunSys;   //系统选择
//static uint16_t attValueInTestMode;    //测试衰减时保存的衰减器数值
//static uint8_t  lna_state = LNA_TO_RF;	   //LNA状态
//static uint8_t  pa_state = ENABLE; 			   //PA状态

///*事件组 */
//EventGroupHandle_t egpTestModeSys[RF_SYS_NUM] = { NULL };


///*---------------------- 声明 ------------------------------------------*/



///*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ HAL ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/



///*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DRV ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/



///*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ OS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

///****************************************************************************
//* 函数名：vTestModeEgpInit(void)
//* 功  能：初始化单通模式事件组
//* 输  入：无
//* 输  出：无
//*/
//static void vTestModeEgpInit( EventGroupHandle_t xEgp)
//{
//    xEventGroupClearBits(xEgp, BIT_TESTMODE_ALL);    // 清零
//}


///****************************************************************************
//* 函数名：vTestModeEgpCerate(void)
//* 功  能：创建单通状态通信事件组
//* 输  入：无
//* 输  出：无
//*/
//void vTestModeEgpCerate(void)
//{
//
//    for( uint8_t esys=0; esys<RF_SYS_NUM; esys++ )
//    {
//        egpTestModeSys[esys]  = xEventGroupCreate();
//        if(NULL == egpTestModeSys[esys])
//        {
//            bsp_printf(RTT_TESTMODE, LOG_TEXT_RED"Create %s egpTestModeSys Fail!!!\r\n", rf_sys_name[esys] );
//        }
//        else
//        {
//            vTestModeEgpInit(egpTestModeSys[esys]);
//        }
//    }
//}


///*****************************************************************************
//* @Name	  : vTestModemClearAllEgp
//* @Brief  : 清除所有的事件组标志
//* @Param  : None
//* @Retval :
//* @Author :
//* @Data   : 2022-09-14
//* @Note   :
//*****************************************************************************/
//static void vTestModeClearAllEgp( void )
//{
//    for( uint8_t esys=0; esys<RF_SYS_NUM; esys++ )
//    {
//        vTestModeEgpInit( egpTestModeSys[ esys ] );
//    }
//}


///*****************************************************************************
//* @Name	  : vTestModeNewEgpMsg
//* @Brief  : 判断是否有新的事件产生
//* @Param  : None
//* @Retval : B_TRUE / B_FALSE
//* @Author :
//* @Data   : 2022-09-14
//* @Note   :
//*****************************************************************************/
//static uint8_t vTestModeGetNewEgpMsg( void )
//{
//    EventBits_t bitEgp[ RF_SYS_NUM ];
//    EventBits_t bitEgp_all = 0;

//    for( uint8_t esys=0; esys<RF_SYS_NUM; esys++ )
//    {
//        bitEgp[ esys ] = xEventGroupGetBits( egpTestModeSys[ esys ] );
//        bitEgp_all |= bitEgp[ esys ];
//    }
//
//    if( bitEgp_all & 0x00FFFFFF )
//        return B_TRUE;
//    else
//        return B_FALSE;
//
//}


///*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ APP ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

///*****************************************************************************
//* @Name	  : vTestModeGetRunMode
//* @Brief  : 获取当前设备的运行模式
//* @Param  : None
//* @Retval :
//* @Author :
//* @Data   : 2022-09-14
//* @Note   :
//*****************************************************************************/
//static void vTestModeGetRunMode( void )
//{
//    eRunMode = getRfRunMode();
//}


///*****************************************************************************
//* @Name	  : vTestModemIn
//* @Brief  : 进入测试模式时设置的状态
//* @Param  : None
//* @Retval :
//* @Author :
//* @Data   : 2022-09-14
//* @Note   :
//*****************************************************************************/
//void vTestModeIn( void )
//{
//    vTestModeClearAllEgp();
//}


///*****************************************************************************
//* @Name	  : vTestModeSave
//* @Brief  : 保存测试数据
//* @Param  : None
//* @Retval :
//* @Author :
//* @Data   : 2022-09-14
//* @Note   :
//*****************************************************************************/
//void vTestModeSave( void )
//{
//
//}


///*****************************************************************************
//* @Name	  : vTestModeSys1Sel
//* @Brief  :
//* @Param  : eType: [输入/出]
//* @Retval :
//* @Author :
//* @Data   : 2022-09-14
//* @Note   :
//*****************************************************************************/
//void vTestModeSysSel( RF_SYS_E esys, EventBits_t bitType )
//{
//    xEventGroupSetBits( egpTestModeSys[esys], bitType );
//}


///*****************************************************************************
//* @Name	  : vTestModemProcess_sys1
//* @Brief  : sys1测试模式处理流程
//* @Param  : xEgp: [输入/出]
//* @Retval :
//* @Author :
//* @Data   : 2022-09-14
//* @Note   :
//*****************************************************************************/
//void vTestModemProcess( RF_SYS_E sysSel, EventGroupHandle_t xEgp )
//{
//
//    EventBits_t bitEgp = xEventGroupGetBits( xEgp);
//    xEventGroupClearBits( xEgp, BIT_TESTMODE_ALL );
//    static uint16_t value_save = 0;
//
//    if( bitEgp & BIT_TESTMODE_ALL_UP )  //上行参数
//    {
//        vRfStateSet( sysSel, RF_CH_DW, RF_SWITCH_CLOSE );    //下行关闭
//        vRfStateSet( sysSel, RF_CH_UP, RF_SWITCH_UP );       //上行单通
//
//        if( bitEgp & BIT_CALIBRATION_UP_GAIN )  //校准上行增益
//        {
//            bsp_printf( RTT_TESTMODE, "=============== Calibration %s Up Gain ===============\r\n", rf_sys_name[ sysSel ] );
//            while( 1 )
//            {
//                vTaskDelay( 100 );
//                if( HAVE_NEW_EGP_MSG() )
//                    break;
//            }
//        }
//        else if( bitEgp & BIT_CALIBRATION_UP_POWER )    //校准上行输出功率
//        {
//            bsp_printf( RTT_TESTMODE, "=============== Calibration %s Up Power ===============\r\n", rf_sys_name[ sysSel ] );
//            while( 1 )
//            {
//                //AD采集
//                //功率转换
//                //输出功率
//                vTaskDelay( 100 );
//                if( HAVE_NEW_EGP_MSG() )
//                    break;
//            }
//        }
//        else if( bitEgp & BIT_COLLECT_UP_POWER )    //采集上行输出功率表
//        {
//            bsp_printf( RTT_TESTMODE, "=============== Collect %s Up Power Table ===============\r\n", rf_sys_name[ sysSel ] );
//            while( 1 )
//            {
//                bsp_printf( RTT_TESTMODE, ">>> Collect %s Up Adc: %d\r\n", rf_sys_name[ sysSel ], tRfChPara[sysSel][RF_CH_UP].tAdc.usAdcAve );
//                vTaskDelay( 100 );
//                if( HAVE_NEW_EGP_MSG() )
//                    break;
//            }
//        }
//        else if( bitEgp & BIT_COLLECT_UP_ATT1 )     //采集上行衰减表1
//        {
//            value_save = 1000;
//			attValueInTestMode = 999;
//            bsp_printf( RTT_TESTMODE, LOG_TEXT_YELLOW"=============== Collect %s Up Att 1 Table ===============\r\n", rf_sys_name[ sysSel ] );
//            while( 1 )
//            {
//                if( attValueInTestMode >= 999 )
//                    attValueInTestMode = 999;
//
//                if( attValueInTestMode != value_save )
//                {
//                    value_save = attValueInTestMode;
//                    vSetAnalogAttValueInTestMode( sysSel, RF_CH_UP, 1, attValueInTestMode );
//                    bsp_printf( RTT_TESTMODE, ">>> Collect %s Up ATT_1: %d\r\n", rf_sys_name[ sysSel ], attValueInTestMode);
//                }
//                vTaskDelay( 150 );
//                if( HAVE_NEW_EGP_MSG() )
//                    break;
//            }
//        }
//        else if( bitEgp & BIT_COLLECT_UP_ATT2 )     //采集上行衰减表2
//        {
//			value_save = 1000;
//			attValueInTestMode = 999;
//            bsp_printf( RTT_TESTMODE, LOG_TEXT_YELLOW"=============== Collect %s Up Att 2 Table ===============\r\n", rf_sys_name[ sysSel ] );
//            while( 1 )
//            {
//                if( attValueInTestMode >= 999 )
//                    attValueInTestMode = 999;
//
//                if( attValueInTestMode != value_save )
//                {
//                    value_save = attValueInTestMode;
//                    vSetAnalogAttValueInTestMode( sysSel, RF_CH_UP, 2, attValueInTestMode );
//                    bsp_printf( RTT_TESTMODE, ">>> Collect %s Up ATT_2: %d\r\n", rf_sys_name[ sysSel ], attValueInTestMode);
//                }
//                vTaskDelay( 150 );
//                if( HAVE_NEW_EGP_MSG() )
//                    break;
//            }
//        }
//        else if( bitEgp & BIT_TEST_RF_SWITCH_UP )   //上行单通
//        {
//            bsp_printf( RTT_TESTMODE, "=============== Test %s Rf Switch To Up  ===============\r\n", rf_sys_name[ sysSel ] );
//			attValueInTestMode = 999;    //测试衰减时保存的衰减器数值
//			lna_state = LNA_TO_RF;	     //LNA状态
//			pa_state = ENABLE; 			   //PA状态
//            while( 1 )
//            {
//				//vSetAnalogAttValueInTestMode( sysSel, RF_CH_UP, 1, attValueInTestMode );
//				vSetAnalogAttWithOutLnaInTestMode( sysSel, RF_CH_UP, attValueInTestMode );
//				vChLnaSet( sysSel, RF_CH_UP, lna_state );
//				vChPaSet( sysSel, RF_CH_UP, pa_state );
//                vTaskDelay( 150 );
//                if( HAVE_NEW_EGP_MSG() )
//                    break;
//            }
//        }
//        else
//        {}
//    }
//    else if( bitEgp & BIT_TESTMODE_ALL_DW ) //下行参数
//    {
//        /* 测试下行的前置条件 */
//        vRfStateSet( sysSel, RF_CH_UP, RF_SWITCH_CLOSE );    //上行关闭
//        vRfStateSet( sysSel, RF_CH_DW, RF_SWITCH_DW );       //下行单通
//
//        if( bitEgp & BIT_CALIBRATION_DW_GAIN ) //校准下行增益
//        {
//            bsp_printf( RTT_TESTMODE, "=============== Calibration %s Dw Gain ===============\r\n", rf_sys_name[ sysSel ] );
//            while( 1 )
//            {
//                //响应衰减设置
//                vTaskDelay( 100 );
//                if( HAVE_NEW_EGP_MSG() )
//                    break;
//            }
//        }
//        else if( bitEgp & BIT_CALIBRATION_DW_POWER )    //校准下行输出功率
//        {
//            bsp_printf( RTT_TESTMODE, "=============== Calibration %s Dw Power ===============\r\n", rf_sys_name[ sysSel ] );
//            while( 1 )
//            {
//                //AD采集
//                //功率转换
//                //输出功率
//                vTaskDelay( 100 );
//                if( HAVE_NEW_EGP_MSG() )
//                    break;
//            }
//        }
//        else if( bitEgp & BIT_COLLECT_DW_POWER )    //采集下行输出功率表
//        {
//            bsp_printf( RTT_TESTMODE, "=============== Collect %s Dw Power Table ===============\r\n", rf_sys_name[ sysSel ] );
//            while( 1 )
//            {
//                bsp_printf( RTT_TESTMODE, ">>> Collect %s Dw Adc: %d\r\n", rf_sys_name[ sysSel ], tRfChPara[sysSel][RF_CH_DW].tAdc.usAdcAve );
//                vTaskDelay( 100 );
//                if( HAVE_NEW_EGP_MSG() )
//                    break;
//            }
//        }
//        else if( bitEgp & BIT_COLLECT_DW_ATT1 )     //采集下行衰减表1
//        {
//            value_save = 1000;
//			attValueInTestMode = 999;
//            bsp_printf( RTT_TESTMODE, LOG_TEXT_YELLOW"=============== Collect %s Dw Att 1 Table ===============\r\n", rf_sys_name[ sysSel ] );
//            while( 1 )
//            {
//                if( attValueInTestMode >= 999 )
//                    attValueInTestMode = 999;
//
//                if( attValueInTestMode != value_save )
//                {
//                    value_save = attValueInTestMode;
//                    vSetAnalogAttValueInTestMode( sysSel, RF_CH_DW, 1, attValueInTestMode );
//                    bsp_printf( RTT_TESTMODE, ">>> Collect %s DW ATT_1: %d\r\n", rf_sys_name[ sysSel ], attValueInTestMode);
//                }
//                vTaskDelay( 150 );
//                if( HAVE_NEW_EGP_MSG() )
//                    break;
//            }
//        }
//        else if( bitEgp & BIT_COLLECT_DW_ATT2 )     //采集下行衰减表2
//        {
//			value_save = 1000;
//			attValueInTestMode = 999;
//            bsp_printf( RTT_TESTMODE, LOG_TEXT_YELLOW"=============== Collect %s Dw Att 2 Table ===============\r\n", rf_sys_name[ sysSel ] );
//            while( 1 )
//            {
//                if( attValueInTestMode >= 999 )
//                    attValueInTestMode = 999;
//
//                if( attValueInTestMode != value_save )
//                {
//                    value_save = attValueInTestMode;
//                    vSetAnalogAttValueInTestMode( sysSel, RF_CH_DW, 2, attValueInTestMode );
//                    bsp_printf( RTT_TESTMODE, ">>> Collect %s DW ATT_2: %d\r\n", rf_sys_name[ sysSel ], attValueInTestMode);
//                }
//                vTaskDelay( 150 );
//                if( HAVE_NEW_EGP_MSG() )
//                    break;
//            }
//        }
//        else if( bitEgp & BIT_TEST_RF_SWITCH_DW )   //下行单通
//        {
//            bsp_printf( RTT_TESTMODE, "=============== Test %s Rf Switch To Dw  ===============\r\n", rf_sys_name[ sysSel ] );
//            attValueInTestMode = 999;    //测试衰减时保存的衰减器数值
//			lna_state = LNA_TO_RF;	     //LNA状态
//			pa_state = ENABLE; 			   //PA状态
//			while( 1 )
//            {
//				//vSetAnalogAttValueInTestMode( sysSel, RF_CH_DW, 1, attValueInTestMode );
//				vSetAnalogAttWithOutLnaInTestMode( sysSel, RF_CH_DW, attValueInTestMode );
//				vChLnaSet( sysSel, RF_CH_DW, lna_state );
//				vChPaSet( sysSel, RF_CH_DW, pa_state );
//                vTaskDelay( 150 );
//                if( HAVE_NEW_EGP_MSG() )
//                    break;
//            }
//        }
//        else
//        {}
//    }
//    else
//    {}
//}


///*****************************************************************************
//* @Name	  : vTestModePorcess
//* @Brief  : 测试模式处理流程
//* @Param  : None
//* @Retval :
//* @Author :
//* @Data   : 2023-05-24
//* @Note   :
//*****************************************************************************/
//void vTestModePorcess( void )
//{
//    vTestModeGetRunMode();
//
//    if( ( RUN_MODE_TEST != eRunMode ) || ( eRunSys >= RF_SYS_NUM ) )  //未进入测试模式, 退出
//        return;

//    vTestModemProcess( eRunSys, egpTestModeSys[ eRunSys] );
//}


///*****************************************************************************
//* @Name	  : vPduDataRw_int8
//* @Brief  : pdu int8 数据出来
//* @Param  : rw: READ / WRITE
//**			 src: 协议数据地址
//**			 tag: 设备数据地址
//**			 min: 有效值最小值
//**			 max: 有效值最大值
//* @Retval :
//* @Author :
//* @Data   : 2022-11-04
//* @Note   :
//*****************************************************************************/
//static uint8_t vPduDataRw_int8( uint8_t rw, uint8_t *src, int8_t *tag, int8_t min, int8_t max )
//{
//    uint8_t res_save = 0;
//
//    if( rw == READ )
//    {
//        *src = (uint8_t)(*tag);
//    }
//    else if( rw == WRITE )
//    {
//        if( ( (int8_t)(*src) >= TM_CALI_GAIN_OFFSET_MIN) && ((int8_t)(*src) <= TM_CALI_GAIN_OFFSET_MAX) )
//        {
//            res_save = 1;
//            *tag = (int8_t)(*src);
//        }
//    }
//    else
//    {}
//    return res_save;
//}


///*****************************************************************************
//* @Name	  : vTestModePduData
//* @Brief  : 响应PDU指令
//* @Param  : rw: READ/WRITE
//*			 uid: 监控参数标识
//*			 src: 数据存储位置
//* @Retval : 1:需要保持, 0:不需要保持
//* @Author :
//* @Data   : 2022-09-15
//* @Note   :
//*****************************************************************************/
//uint8_t vTestModePduData( uint8_t rw, uint16_t uid, uint8_t *src )
//{
//    uint8_t res_save = 0;
//    uint8_t test_type = 0xFF;
//
//	EventBits_t bitEgp;
//
//    switch( uid )
//    {
//        case TM_UID_IN_TEST_MODE:           //进入测试模式
//                if( WRITE == rw )
//                {
//                    SetRfRunMode( RUN_MODE_TEST_INIT ); //进入测试模式
//                    eRunMode = RUN_MODE_TEST_INIT;
//                    vTestModeIn();  //清空所有的事件
//                    test_type = *src;
//                    switch( test_type )
//                    {
//                        case TEST_TYPE_COLLECT_CH1_UP_ATT1_TABLE:
//                                eRunSys = RF_SYS_1;
//								bitEgp = BIT_COLLECT_UP_ATT1;
//                                break;

//                        case TEST_TYPE_CLOOECT_CH1_DW_ATT1_TABLE:
//                                eRunSys = RF_SYS_1;
//								bitEgp = BIT_COLLECT_DW_ATT1;
//                                break;
//
//						case TEST_TYPE_COLLECT_CH1_UP_ATT2_TABLE:
//                                eRunSys = RF_SYS_1;
//								bitEgp = BIT_COLLECT_UP_ATT2;
//                                break;

//                        case TEST_TYPE_CLOOECT_CH1_DW_ATT2_TABLE:
//                                eRunSys = RF_SYS_1;
//								bitEgp = BIT_COLLECT_DW_ATT2;
//                                break;
//
//						case TEST_TYPE_CLOOECT_CH1_UP_POW_TABLE:
//                                eRunSys = RF_SYS_1;
//								bitEgp = BIT_COLLECT_UP_POWER;
//                                break;

//                        case TEST_TYPE_CLOOECT_CH1_DW_POW_TABLE:
//                                eRunSys = RF_SYS_1;
//								bitEgp = BIT_COLLECT_DW_POWER;
//                                break;
//
//						case TEST_TYPE_RF_CH1_UP_OPEN:	//CH1 上行常开, 没有ALC, 可设置增益 / LNA / PA
//								eRunSys = RF_SYS_1;
//								bitEgp = BIT_TEST_RF_SWITCH_UP;
//								break;

//						case TEST_TYPE_RF_CH1_DW_OPEN:	//CH1 上行常开, 没有ALC, 可设置增益 / LNA / PA
//								eRunSys = RF_SYS_1;
//								bitEgp = BIT_TEST_RF_SWITCH_DW;
//								break;
//
//                        case TEST_TYPE_CALIBRATION_CH1_UP_GAIN:
//                                eRunSys = RF_SYS_1;
//								bitEgp = BIT_CALIBRATION_UP_GAIN;
//                                break;
//
//						case TEST_TYPE_CALIBRATION_CH1_UP_POW:
//                                eRunSys = RF_SYS_1;
//								bitEgp = BIT_CALIBRATION_UP_POWER;
//                                break;
//
//						case TEST_TYPE_CALIBRATION_CH1_DW_POW:
//                                eRunSys = RF_SYS_1;
//								bitEgp = BIT_CALIBRATION_DW_POWER;
//                                break;
//
//						case TEST_TYPE_CALIBRATION_CH1_DW_GAIN:
//                                eRunSys = RF_SYS_1;
//								bitEgp = BIT_CALIBRATION_DW_GAIN;
//                                break;
//
//						#if RF_SYS_NUM >= 2
//                        case TEST_TYPE_CLOOECT_CH2_UP_ATT1_TABLE:
//                                eRunSys = RF_SYS_2;
//								bitEgp = BIT_COLLECT_UP_ATT1;
//                                break;

//                        case TEST_TYPE_CLOOECT_CH2_DW_ATT1_TABLE:
//                                eRunSys = RF_SYS_2;
//								bitEgp = BIT_COLLECT_DW_ATT1;
//                                break;
//
//						case TEST_TYPE_CLOOECT_CH2_UP_ATT2_TABLE:
//                                eRunSys = RF_SYS_2;
//								bitEgp = BIT_COLLECT_UP_ATT2;
//                                break;

//                        case TEST_TYPE_CLOOECT_CH2_DW_ATT2_TABLE:
//                                eRunSys = RF_SYS_2;
//								bitEgp = BIT_COLLECT_DW_ATT2;
//                                break;
//
//                        case TEST_TYPE_CLOOECT_CH2_UP_POW_TABLE:
//                                eRunSys = RF_SYS_2;
//								bitEgp = BIT_COLLECT_UP_POWER;
//                                break;
//
//                        case TEST_TYPE_CLOOECT_CH2_DW_POW_TABLE:
//                                eRunSys = RF_SYS_2;
//								bitEgp = BIT_COLLECT_DW_POWER;
//                                break;

//						case TEST_TYPE_RF_CH2_UP_OPEN:	//CH1 上行常开, 没有ALC, 可设置增益 / LNA / PA
//								eRunSys = RF_SYS_2;
//								bitEgp = BIT_TEST_RF_SWITCH_UP;
//								break;

//						case TEST_TYPE_RF_CH2_DW_OPEN:	//CH1 上行常开, 没有ALC, 可设置增益 / LNA / PA
//								eRunSys = RF_SYS_2;
//								bitEgp = BIT_TEST_RF_SWITCH_DW;
//								break;

//                        case TEST_TYPE_CALIBRATION_CH2_UP_GAIN:
//                                eRunSys = RF_SYS_2;
//								bitEgp = BIT_CALIBRATION_UP_GAIN;
//                                break;

//                        case TEST_TYPE_CALIBRATION_CH2_UP_POW:
//                                eRunSys = RF_SYS_2;
//								bitEgp = BIT_CALIBRATION_UP_POWER;
//                                break;

//                        case TEST_TYPE_CALIBRATION_CH2_DW_GAIN:
//                                eRunSys = RF_SYS_2;
//								bitEgp = BIT_CALIBRATION_DW_GAIN;
//                                break;

//                        case TEST_TYPE_CALIBRATION_CH2_DW_POW:
//                                eRunSys = RF_SYS_2;
//								bitEgp = BIT_CALIBRATION_DW_POWER;
//                                break;
//
//						#endif
//
//                        case TEST_TYPE_LED:
//                            break;
//
//                        default:
//                            break;
//                    }
//
//					if( bitEgp )
//						vTestModeSysSel( eRunSys, bitEgp);
//                }
//                break;
//
//        case CAB_UID_CH1_UPOW_OFFSET:		//设置CH1上行输出功率表偏移量
//                res_save = vPduDataRw_int8( rw, src, &tRfChPara[RF_SYS_1][RF_CH_UP].tPowList.scCaliOffset, TM_CALI_POWLIST_OFFSET_MIN, TM_CALI_POWLIST_OFFSET_MAX );
//                break;

//        case CAB_UID_CH1_DPOW_OFFSET:		//设置CH1下行输出功率表偏移量
//                res_save = vPduDataRw_int8( rw, src, &tRfChPara[RF_SYS_1][RF_CH_DW].tPowList.scCaliOffset, TM_CALI_POWLIST_OFFSET_MIN, TM_CALI_POWLIST_OFFSET_MAX );
//                break;

//        case CAB_UID_CH1_UGAIN_OFFSET:		//设置CH1上行增益偏移量
//                res_save = vPduDataRw_int8( rw, src, &tRfChPara[RF_SYS_1][RF_CH_UP].tAtt.scCaliGainOffset, TM_CALI_GAIN_OFFSET_MIN, TM_CALI_GAIN_OFFSET_MAX );
//                break;
//
//        case CAB_UID_CH1_DGAIN_OFFSET:		//设置CH1下行增益偏移量
//                res_save = vPduDataRw_int8( rw, src, &tRfChPara[RF_SYS_1][RF_CH_DW].tAtt.scCaliGainOffset, TM_CALI_GAIN_OFFSET_MIN, TM_CALI_GAIN_OFFSET_MAX );
//                break;
//
//		#if RF_SYS_NUM >= 2
//        case CAB_UID_CH2_UPOW_OFFSET:		//设置CH2上行输出功率表偏移量
//                res_save = vPduDataRw_int8( rw, src, &tRfChPara[RF_SYS_2][RF_CH_UP].tPowList.scCaliOffset, TM_CALI_POWLIST_OFFSET_MIN, TM_CALI_POWLIST_OFFSET_MAX );
//                break;

//        case CAB_UID_CH2_DPOW_OFFSET:		//设置CH2下行输出功率表偏移量
//                res_save = vPduDataRw_int8( rw, src, &tRfChPara[RF_SYS_2][RF_CH_DW].tPowList.scCaliOffset, TM_CALI_POWLIST_OFFSET_MIN, TM_CALI_POWLIST_OFFSET_MAX );
//                break;

//        case CAB_UID_CH2_UGAIN_OFFSET:		//设置CH2上行增益偏移量
//                res_save = vPduDataRw_int8( rw, src, &tRfChPara[RF_SYS_2][RF_CH_UP].tAtt.scCaliGainOffset, TM_CALI_GAIN_OFFSET_MIN, TM_CALI_GAIN_OFFSET_MAX );
//                break;
//
//        case CAB_UID_CH2_DGAIN_OFFSET:		//设置CH2下行增益偏移量
//                res_save = vPduDataRw_int8( rw, src, &tRfChPara[RF_SYS_2][RF_CH_DW].tAtt.scCaliGainOffset, TM_CALI_GAIN_OFFSET_MIN, TM_CALI_GAIN_OFFSET_MAX );
//                break;
//
//		#endif
//
//        case TM_UID_SET_ATT_VALUE:  //设置模拟衰减量
//                if( rw == READ )
//                {
//                    *src = (uint8_t)(attValueInTestMode&0x00FF);
//                    *(src+1) = (uint8_t)(attValueInTestMode>>8);
//                }
//                else if( rw == WRITE )
//                {
//                    attValueInTestMode = (*src + ((*(src+1))<<8));
//                }
//                break;
//
//		case TM_UID_SET_LNA_STATE:
//				if( rw == READ )
//                {
//                    *src = (lna_state == LNA_TO_RF) ? 1 : 0;
//                }
//                else if( rw == WRITE )
//                {
//                    lna_state = *src ? LNA_TO_RF : LNA_TO_BYPASS;
//                }
//				break;
//
//		case TM_UID_SET_PA_STATE:
//				if( rw == READ )
//                {
//                    *src = (pa_state == ENABLE) ? 1 : 0;
//                }
//                else if( rw == WRITE )
//                {
//                    pa_state = *src ? ENABLE : DISABLE;
//                }
//				break;
//
//		 default:
//                res_save = 0;
//                //err = 1;
//                break;
//    }
//
////    if((rw==WRITE) && ( err==0 ))
////    {
////        if( *tag != *src )
////        {
////            *tag = *src;
////            *fresh = 1;  //通道增益、功率补偿修改
////            save_sign = 1;
////        }
////    }
//
//    return res_save;
//}
