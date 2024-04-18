/***************************** Copyright ****************************************
*
* (C) Copyright 2022, Shaanxi Tianji Communication
*  All Rights Reserved
*
* FileName   : bsp_rf_iso.c
* Version    : none
* Author     : ZhangWei
* Date       : 2022-08-10
* Description: none
*******************************************************************************/
#include "main.h"
#include "bsp_public_macro.h"

#include "bsp_rf_iso.h"
#include "FreeRTOS.h"                   // ARM.FreeRTOS::RTOS:Core
#include "event_groups.h"               // ARM.FreeRTOS::RTOS:Event Groups
#include "bsp_rtt.h"
//#include "systick.h"
#include "bsp_rf_att.h"
#include "bsp_rf_lna.h"
#include "bsp_rf_ctr.h"
#include "bsp_rf_digit_att.h"
#include "bsp_rf_analog_att.h"
#include "bsp_rf_alc.h"
#include "bsp_rf_pow.h"
#include "bsp_adc.h"


/*---------------------- 端口定义 --------------------------------------*/

// 自激检测结构体
// 与射频通道参数相关的自激检测参数
//#define NR_DL_ZIJI_CNT_NUM      ( 6 )      // 自激被动检测确认次数
#define NR_DL_ZIJI_HIGHPOW      ( 1000 * 60 * 5 )   // 大功率检测时间 单位ms
#define NR_DL_ZIJI_PERIOD       ( 1200)     // 5G下行自激超时 单位ms
#define NR_DL_ZIJI_RISING_TH    ( 0 )       // 5G下行自激上升沿阈值
#define NR_DL_ZIJI_FALLING_TH   ( 0 )       // 5G下行自激下降沿阈值

#define NR_DL_ZIJI_CONFIRM_TH   ( 9 )       // 5G下行自激诱导跳变阈值

#define NR_DL_ZIJI_CONFIRM_HI_ATT_OFFSET   ( 2 )      // 5G下行自激诱导确认,高功率att偏移量
#define NR_DL_ZIJI_CONFIRM_LO_ATT_OFFSET   ( 3 )       // 5G下行自激诱导确认,低功率att偏移量

#define NR_DL_ZIJI_CONFIRM_NUM       ( 5 )      // 自激检测诱导次数
#define NR_DL_ZIJI_CONFIRM_LO_TIME   ( 1000 )       // 自激检测诱导,低功率持续时间
#define NR_DL_ZIJI_CONFIRM_HI_TIME   ( 300 )       // 自激检测诱导,高功率持续时间

typedef enum
{
    ZIJI_CLEAR = 0,             // 自激清除
    ZIJI_START,                 // 自激起始
    ZIJI_RISING_DETECTED,       // 5G下行自激ADC检测上升沿阈值
    ZIJI_FALLING_DETECTED,      // 5G下行自激ADC检测下降沿阈值
    ZIJI_DETECTED              // 5G下行自激ADC检测阈值
} eZJ;


/*---------------------- 宏定义  ---------------------------------------*/


#define TICK_SINCE(x) (GetTick() - x)

#define RFCTR_STOP_ISO     (MIN_TO_TICKS(30))   //每隔30min进行5G自激检测


#define ZIJI_RESET_NUM   (3)     //自激设置的增益回退值复位检测计数
#define ZIJI_CLOSE_MIMO_NUM  (30)

EventGroupHandle_t xIsoEgp = NULL;


/*---------------------- 结构体 ----------------------------------------*/

/*---------------------- 常量 ------------------------------------------*/
#define BIT_ISO_SYS_RESET_EN_(x)	( 1U << x )			//x系统编号, RF_SYS_1, RF_SYS_2, .....
#define BIT_ISO_SYS_CLOSE_MIMO_(x)	( 1U << (x+16) )	//x系统编号, RF_SYS_1, RF_SYS_2, .....
#define BIT_ISO_ALL 0x00FFFFFF

/*---------------------- 全局变量 --------------------------------------*/

__IO uint32_t uwIsoTick;   //1ms计数器


/*---------------------- 局部变量 --------------------------------------*/


/*---------------------- 声明 ------------------------------------------*/



/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ HAL ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/





/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ OS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*****************************************************************************
* @Name	  : vAdcEgpCreat
* @Brief  : 创建事件组初始化
* @Param  : None
* @Retval : None
* @Author :
* @Data   : 2022-08-23
* @Note   :
*****************************************************************************/
void vIsoEgpCreat(void)
{
    xIsoEgp  = xEventGroupCreate();

    if(xIsoEgp == NULL)
    {
        /* 任务创建失败 */
        bsp_printf(RTT_OS, RTT_CTRL_TEXT_BRIGHT_RED"ERROR: xIsoEgp CREATE FAIL...\r\n");
    }
    else
    {
        xEventGroupClearBits(xIsoEgp, BIT_ISO_ALL );    // 清零
    }
}




/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ APP ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/**
  * @brief 计数
  */
/*****************************************************************************
* @Name	  : vIncTick
* @Brief  : 1ms 计数器累加
* @Param  : None
* @Retval : 
* @Author : 
* @Data   : 2022-11-21
* @Note   : 在 void xPortSysTickHandler(void) 中断函数中调用
*****************************************************************************/
void vIsoIncTick(void)
{
    uwIsoTick += 1;
}


/*****************************************************************************
* @Name	  : GetTick
* @Brief  : 获取uwIsoTick数值
* @Param  : None
* @Retval : 
* @Author : 
* @Data   : 2022-11-21
* @Note   :
*****************************************************************************/
static uint32_t GetTick(void)
{
    return uwIsoTick;
}

/*****************************************************************************
* @Name	  : vSysIsoTimeAdd
* @Brief  : 
* @Param  : eSys: [输入/出] 
*           xegp: [输入/出]  
*		    bit_en: [输入/出] 
*			pt_iso_time: [输入/出] 
*			pt_reset_num: [输入/出] 
* @Retval : 
* @Author : 
* @Data   : 2023-03-17
* @Note   :
*****************************************************************************/
static void vSysIsoTimeAdd( RF_SYS_E eSys,  EventGroupHandle_t xegp, const EventBits_t bit_en, uint16_t *pt_iso_time, uint8_t *pt_reset_num )
{
    EventBits_t bitsIso = xEventGroupGetBitsFromISR( xegp );
    if((bit_en == (bitsIso & bit_en)) && (*pt_reset_num < ZIJI_RESET_NUM))
    {
        (*pt_iso_time)++;
        if( *pt_iso_time >= RFCTR_STOP_ISO)
        {
            *pt_iso_time = 0;

            tRfChPara[RF_SYS_1][RF_CH_UP].tAtt.ucIso = 0;
            tRfChPara[RF_SYS_1][RF_CH_DW].tAtt.ucIso = 0;
            bsp_printf(RTT_ISO, RTT_CTRL_TEXT_BRIGHT_WHITE"\r\n<<<<<<%s ISO REDETECT>>>>>>\r\n", rf_sys_name[eSys] );

            (*pt_reset_num)++;      //释放衰减，重新进行自激检测处理，一天最多3次
            
            bsp_printf(RTT_ISO, RTT_CTRL_TEXT_BRIGHT_WHITE"\r\n<<<<<<%s ISO REDETECT CNT: %d>>>>>>\r\n", rf_sys_name[eSys], *pt_reset_num);
            xEventGroupClearBitsFromISR(xegp, bit_en);
        }
    }
}


/*****************************************************************************
* @Name	  : vIsoTimeAddInTimeIsr
* @Brief  : 隔离度定时恢复
* @Param  : None
* @Retval : 
* @Author : 
* @Data   : 2022-12-12
* @Note   :
*****************************************************************************/
void vIsoTimeAddInTimeIsr( void )
{
	static uint16_t usRfSysIsoTime[ RF_SYS_NUM ] = {0};
	static uint8_t  ziji_reset_sys_num[ RF_SYS_NUM ] = {0};
	
//    static uint16_t usRfSys1IsoTime = 0;
//    static uint16_t usRfSys2IsoTime = 0;
//    static uint8_t ziji_reset_sys1_num = 0;     // 4G自激一天恢复次数
//    static uint8_t ziji_reset_sys2_num = 0;     // 5G自激一天恢复次数
    
    EventBits_t bitsIso = xEventGroupGetBitsFromISR( xIsoEgp );
	
	
	for( RF_SYS_E esys = RF_SYS_1; esys<RF_SYS_END; esys++ )
		vSysIsoTimeAdd( esys, xIsoEgp, BIT_ISO_SYS_RESET_EN_( esys ), &usRfSysIsoTime[ esys ], &ziji_reset_sys_num[ esys ] );
	
//	vSysIsoTimeAdd( RF_SYS_1, xIsoEgp, BIT_ISO_SYS1_RESET_EN, &usRfSysIsoTime[RF_SYS_1], &ziji_reset_sys_num[RF_SYS_1] );
//	
//	#if RF_SYS_NUM >= 2
//	vSysIsoTimeAdd( RF_SYS_2, xIsoEgp, BIT_ISO_SYS2_RESET_EN, &usRfSysIsoTime[RF_SYS_2], &ziji_reset_sys_num[RF_SYS_2] );
//	#endif
	
}

/*****************************************************************************
* @Name	  : vIsoGetRfPara
* @Brief  : 获取射频参数
* @Param  : None
* @Retval : 
* @Author : 
* @Data   : 2022-11-21
* @Note   :
*****************************************************************************/
static void vIsoGetRfPara( void )
{
    
}


static void UT_dataPrint(void)
{
//    for(uint8_t i = 0; i < ADC_ARRAY_NUM; i++)
//    {
//        bsp_Printf(RTT_ISO, RTT_CTRL_TEXT_BRIGHT_WHITE"%d,",tDownAdcValue.AdcArray[i]);
//    }
//        bsp_Printf(RTT_ISO, RTT_CTRL_TEXT_BRIGHT_WHITE"\r\n");
}


static void vIsoSetAtt( RF_SYS_E eSys, RF_CH_E eCh, Att_T *ptAtt, uint8_t value )
{
//    #define LNA_BYPASS_OFFSET ( 6 )
//    
//    uint8_t lna_to_rf_point = ptAtt->constBypassAttNum; 
//    uint8_t lna_to_bypass_point = ptAtt->constBypassAttNum + LNA_BYPASS_OFFSET;
    
    
//    ptAtt->ucDigitSum = value;
    
    vAttSpliteAttToAttAndLna( ptAtt );
    
//    ptAtt->ucSetValueA1 = ptAtt->ucAnalogSum; 
    
//    vChDigitAttSet( eSys, eCh, ptAtt->ucSetValueD1 );
    vChAnalogAttSet( eSys, eCh, 1, ptAtt->ucSetValueA1 );
	vChAnalogAttSet( eSys, eCh, 2, ptAtt->ucSetValueA1 );
}


/*****************************************************************************
* @Name	  : scIsoChConfirm
* @Brief  : 自激确认
* @Param  : ptPow: [输入/出] 
**          ptAtt: [输入/出] 
**          ptAlc: [输入/出] 
**          ptIso: [输入/出] 
* @Retval : 
* @Author : 
* @Data   : 2022-11-21
* @Note   :
*****************************************************************************/
#define ZIJI_CONFIRM_PWR_HI 1               // 5G下行自激确认ADC检测上升沿阈值
#define ZIJI_CONFIRM_PWR_LO 2               // 5G下行自激确认ADC检测下降沿阈值

static int scIsoChConfirm( RF_SYS_E eSys, RF_CH_E eCh, Pow_T *ptPow, Att_T *ptAtt, Alc_T *ptAlc, Iso_T *ptIso )
{
    uint8_t comfirmCnt = 0, confirmStat = ZIJI_CONFIRM_PWR_HI;
    int8_t ConfirmPwrHiAttVal = 0, ConfirmPwrLoAttVal = 0;
    int8_t pow = 0, powDiff = 0;  // 获取输出功率
    int8_t ZijiTH = ptAlc->scAlcUpperLimit + NR_DL_ZIJI_RISING_TH;   //自激检测门限
//    int8_t LowerTH = ptAlc->scAlcLowerLimit - NR_DL_ZIJI_FALLING_TH; //线性工作区门限
    uint8_t ZijiPwrVal[NR_DL_ZIJI_CONFIRM_NUM] = {0};
    uint8_t attSum = ptAtt->constDigitAttNum + ptAtt->constBypassAttNum;
    uint32_t thetick = 0;
    uint8_t  res = FALSE;
    
    /*1参数初始化*/
    ptIso->PreTick = GetTick();

    ConfirmPwrHiAttVal = ptIso->ZijiAttValMin - NR_DL_ZIJI_CONFIRM_HI_ATT_OFFSET;
    ConfirmPwrLoAttVal = ptIso->ZijiAttValMin + NR_DL_ZIJI_CONFIRM_LO_ATT_OFFSET;
    if(ConfirmPwrHiAttVal <= 0)//检查参数越界
    {
        ConfirmPwrHiAttVal = 0;
        ConfirmPwrLoAttVal = ConfirmPwrHiAttVal + NR_DL_ZIJI_CONFIRM_LO_ATT_OFFSET + NR_DL_ZIJI_CONFIRM_HI_ATT_OFFSET;
    }
    if(ConfirmPwrLoAttVal >= attSum)//检查参数越界
    {
        ConfirmPwrLoAttVal = attSum;
        ConfirmPwrHiAttVal = ConfirmPwrLoAttVal - NR_DL_ZIJI_CONFIRM_LO_ATT_OFFSET - NR_DL_ZIJI_CONFIRM_HI_ATT_OFFSET;
    }

    /*2设置衰减器*/
    vIsoSetAtt( eSys, eCh, ptAtt, ConfirmPwrHiAttVal );
    //ISO5GDwdigAttSet(ConfirmPwrHiAttVal);   // 按自激工况设置数字衰减
    
    /*3自激确认*/
    while(comfirmCnt < NR_DL_ZIJI_CONFIRM_NUM)
    {
        thetick = GetTick();
        
        res = vGetEgpCollectEndSign();
        if( TRUE == res )
        {
            vAdcEgpClear();
            
            //vBspPowCal();       //计算5G上下行通道功率
            vRfGetData();                   //ADC采集
            vPowConvert();                  //ADC转换为输出功率
            
            pow = ptPow->scOutPow;
            //bsp_Printf(RTT_ISO,RTT_CTRL_TEXT_BRIGHT_WHITE"Pow = %d, att = %d, tick = %d\r\n",pow,ptRfCh->tAtt.ucSetValue,thetick);

            if(confirmStat == ZIJI_CONFIRM_PWR_HI)//按自激工况设置数字衰减,确认是否诱导自激
            {
                if(pow <= ZijiTH)//当检测到输出信号功率低,自激检测退出
                {
                    bsp_printf(RTT_ISO, RTT_CTRL_TEXT_BRIGHT_GREEN"ZiJi confirm Hi failure cnt = %d, set_att = %d, outpow = %d, tick = %d\r\n", comfirmCnt, ConfirmPwrHiAttVal, pow, thetick);
                    break;//yy test break->UT_dataPrint
                }
                else if(pow > ZijiTH && (TICK_SINCE(ptIso->PreTick) > NR_DL_ZIJI_CONFIRM_HI_TIME))//输出功率持续高于阈值,进行低功率信号确认
                {
                    vIsoSetAtt( eSys, eCh, ptAtt, ConfirmPwrLoAttVal);    // 按非自激工况设置数字衰减
                    bsp_printf(RTT_ISO, RTT_CTRL_TEXT_BRIGHT_YELLOW"ZiJi confirm Hi to Lo cnt = %d, set_att = %d, outpow = %d, tick = %d\r\n", comfirmCnt, ConfirmPwrHiAttVal, pow, thetick);
                    ptIso->PreTick = GetTick();
                    confirmStat = ZIJI_CONFIRM_PWR_LO;
                    vTaskDelay(150);

                    continue;
                }
                else if(pow > ZijiTH) //输出大于阈值时,更新pow,获取当前周期最小pow值
                {
                    bsp_printf(RTT_ISO, RTT_CTRL_TEXT_BRIGHT_YELLOW"ZiJi confirm Hi cnt = %d, set_att = %d, outpow = %d, tick = %d\r\n", comfirmCnt, ConfirmPwrHiAttVal, pow, thetick);
                    if(ZijiPwrVal[comfirmCnt] == 0)
                        ZijiPwrVal[comfirmCnt] = pow;
                    else if(ZijiPwrVal[comfirmCnt] > pow)
                        ZijiPwrVal[comfirmCnt] = pow;
                }
            }
            else if(confirmStat == ZIJI_CONFIRM_PWR_LO)//压缩增益,确认是否向下跳变
            {
                powDiff = ZijiPwrVal[comfirmCnt] - pow;//计算跳变值
                if(powDiff < NR_DL_ZIJI_CONFIRM_TH)//当检测到输出信号跳变差值小于阈值,自激检测退出
                {
                    bsp_printf(RTT_ISO, RTT_CTRL_TEXT_BRIGHT_GREEN"ZiJi confirm Lo failure cnt = %d, set_att = %d, outpow = %d, tick = %d\r\n", comfirmCnt, ConfirmPwrLoAttVal, pow, thetick);
                    UT_dataPrint();
                    break; //yy test break->UT_dataPrint
                }
                else if(powDiff >= NR_DL_ZIJI_CONFIRM_TH && (TICK_SINCE(ptIso->PreTick) > NR_DL_ZIJI_CONFIRM_LO_TIME))//输出功率持续低于阈值,进行高功率信号确认
                {
                    comfirmCnt++;
                    if(comfirmCnt >= (NR_DL_ZIJI_CONFIRM_NUM))//完成确认
                    {
                        bsp_printf(RTT_ISO, RTT_CTRL_TEXT_BRIGHT_GREEN"ZiJi confirmd cnt = %d, set_att = %d, outpow = %d, tick = %d\r\n", comfirmCnt - 1, ConfirmPwrLoAttVal, pow, thetick);
                        return TRUE;
                    }
                    bsp_printf(RTT_ISO, RTT_CTRL_TEXT_BRIGHT_CYAN"ZiJi confirm Lo to Hi cnt = %d, set_att = %d, outpow = %d, tick = %d\r\n", comfirmCnt, ConfirmPwrLoAttVal, pow, thetick);

                    vIsoSetAtt( eSys, eCh, ptAtt, ConfirmPwrHiAttVal); //按自激工况设置数字衰减
                    ptIso->PreTick = GetTick();
                    confirmStat = ZIJI_CONFIRM_PWR_HI;
                }
            }
        }
#if __FWDT_ENABLE__
        vWdtTask(RF_TASK_PRIORITY);
#endif
        vTaskDelay(5);
        //setatt
    }
    return FALSE;
}

/*****************************************************************************
* @Name	  : vIsoSetCloseEventBit
* @Brief  : 设置隔离度关断标识
* @Param  : eSys: [输入/出] 
* @Retval : 
* @Author : 
* @Data   : 2023-03-17
* @Note   :
*****************************************************************************/
static void vIsoSetCloseEventBit( RF_SYS_E eSys )
{
	if( eSys >= RF_SYS_END )
		return;
	
	if( RF_SYS_1 == eSys )
		xEventGroupSetBits(xIsoEgp, BIT_ISO_SYS_CLOSE_MIMO_( eSys ));
	else
		xEventGroupSetBits(xIsoEgp, BIT_ISO_SYS_CLOSE_MIMO_( eSys ));
	
	bsp_printf(RTT_ISO, RTT_CTRL_TEXT_BRIGHT_WHITE"\r\n<<<<<<%s ISO CLOSE>>>>>>\r\n", rf_sys_name[eSys]);
}

/*****************************************************************************
* @Name	  : vIsoChDetect
* @Brief  : 通道隔离度检测
* @Param  : ptPow: [输入/出] 
**			 ptGain: [输入/出] 
**			 ptAtt: [输入/出] 
**			 ptAlc: [输入/出] 
**			 ptIso: [输入/出] 
* @Retval : 
* @Author : 
* @Data   : 2022-11-21
* @Note   :
*****************************************************************************/
static void vIsoChDetect( RF_SYS_E eSys, RF_CH_E eCh, Pow_T *ptPow, Gain_T *ptGain, Att_T *ptAtt, Alc_T *ptAlc, Iso_T *ptIso )
{
    uint8_t Cur5GDLAttSetVal = ptAtt->ucDigitSum; // 获取当前衰减设置值
    uint8_t powHiAttMax = 0, powHighAttMin = 0xff; //输出高功率时的att最大最小值
    uint8_t powLoAttMax = 0, powLoAttMin = 0xff; //输出低功率时的att最大最小值

    int8_t pow = ptPow->scOutPow;   //    ->tPower.scOutPow;   // 获取输出功率
    int8_t ZijiTH  = ptAlc->scAlcUpperLimit + NR_DL_ZIJI_RISING_TH;  //  ptRfCh->tRealPara.scAlcUpperLimit + NR_DL_ZIJI_RISING_TH;   //自激检测门限
    int8_t LowerTH = ptAlc->scAlcLowerLimit - NR_DL_ZIJI_FALLING_TH; //    ptRfCh->tRealPara.scAlcLowerLimit - NR_DL_ZIJI_FALLING_TH; //线性工作区门限
    EventBits_t bitsEvent = 0;
    uint8_t index = 0;
    uint8_t attSum = ptAtt->constDigitAttNum + ptAtt->constBypassAttNum;
    int8_t res;
    
    Att_T tAttForConfirm;

    EqIndependentPara_T *pt = &tRfChPara[eSys][0];
    
    switch( ptIso->ZijiStatMachine )
    {
        case ZIJI_START:
            if(pow > ZijiTH)
            {
                ptIso->StartTick = GetTick();
                ptIso->PreTick = ptIso->StartTick;
                ptIso->UpperCnt++;   // 大于功率上限计数
                ptIso->ZijiAttVal[ptIso->UpperCnt - 1] = Cur5GDLAttSetVal;    // 记录当前衰减
                ptIso->ZijiStatMachine = ZIJI_RISING_DETECTED;
                bsp_printf(RTT_ISO, RTT_CTRL_TEXT_BRIGHT_GREEN "\r\n[ %s ][ziji start] cnt = %d, set_att = %d, outpow = %d\r\n", \
                    rf_sys_name[eSys], ptIso->UpperCnt, Cur5GDLAttSetVal, pow);
            }
            break;

        case ZIJI_RISING_DETECTED: // 检测到上升沿
            //确认自激
            if(ptIso->UpperCnt >= NR_DL_ZIJI_CNT_NUM)
            {
                ptIso->ZijiStatMachine = ZIJI_DETECTED;

                // 赋初始值
                ptIso->ZijiAttValMax = Cur5GDLAttSetVal;
                ptIso->ZijiAttValMin = Cur5GDLAttSetVal;
                break;
            }

            //自激检测判断
            if(pow > ZijiTH)  //功率高于起控点时
            {
                if((Cur5GDLAttSetVal >= attSum ) && (TICK_SINCE(ptIso->PreTick) > NR_DL_ZIJI_HIGHPOW))  //持续高于起控点且衰减用完
                {
                    ptIso->ZijiStatMachine = ZIJI_CLEAR; // 清除，认为大功率
                    //过功率事件
                    //g_ptRf5gPara->ucBigPowCloseMimo = 1; // 大功率关断标识

                    bsp_printf(RTT_ISO, RTT_CTRL_TEXT_BRIGHT_RED"[ %s ][eSys][WARN]: DL channel Power the over limit %d\r\n", rf_sys_name[eSys], TICK_SINCE(ptIso->PreTick));
                    break;
                }
#if 1
                //记录自激的衰减最大值
                if(ptIso->ZijiAttVal[ptIso->UpperCnt - 1] < Cur5GDLAttSetVal)
                    ptIso->ZijiAttVal[ptIso->UpperCnt - 1] = Cur5GDLAttSetVal;
                bsp_printf(RTT_ISO, RTT_CTRL_TEXT_BRIGHT_YELLOW"[ %s ][ziji rising] cnt = %d, set_att = %d, outpow = %d\r\n", rf_sys_name[eSys], ptIso->UpperCnt, Cur5GDLAttSetVal, pow);
                //功率低于起控点
#endif
            }
            else if(pow < LowerTH)
            {
                ptIso->LowerCnt++;
                ptIso->LowerAttVal[ptIso->LowerCnt - 1] = Cur5GDLAttSetVal;
                ptIso->ZijiStatMachine = ZIJI_FALLING_DETECTED;
                ptIso->PreTick = GetTick();
                bsp_printf(RTT_ISO, RTT_CTRL_TEXT_BRIGHT_CYAN"[ %s ][ziji rising to falling] cnt = %d, set_att = %d, outpow = %d\r\n", rf_sys_name[eSys], ptIso->LowerCnt, Cur5GDLAttSetVal, pow);
            }
            else // pow 介于ALC上下限之间(正常工作)
            {
                if(TICK_SINCE(ptIso->PreTick) > NR_DL_ZIJI_PERIOD)  //超时退出检测
                {
                    ptIso->ZijiStatMachine = ZIJI_CLEAR;
                    bsp_printf(RTT_ISO, RTT_CTRL_TEXT_BRIGHT_GREEN"[ %s ][INFO]: DL ZIJI detection timeout = %d\r\n", rf_sys_name[eSys], TICK_SINCE(ptIso->PreTick));
                }
            }
            break;

        case ZIJI_FALLING_DETECTED:
            if(pow < LowerTH)
            {
                if(TICK_SINCE(ptIso->PreTick) > NR_DL_ZIJI_PERIOD)  //检测自激超时
                {
                    ptIso->ZijiStatMachine = ZIJI_CLEAR;
                    bsp_printf(RTT_ISO, RTT_CTRL_TEXT_BRIGHT_CYAN"[ %s ] ZiJi falling detection timeout = %d\r\n", rf_sys_name[eSys], TICK_SINCE(ptIso->PreTick) );
                }
                if(ptIso->LowerAttVal[ptIso->LowerCnt - 1] > Cur5GDLAttSetVal)
                    ptIso->LowerAttVal[ptIso->LowerCnt - 1] = Cur5GDLAttSetVal;
                bsp_printf(RTT_ISO, RTT_CTRL_TEXT_BRIGHT_CYAN"[ %s ][ziji falling] %s cnt = %d, set_att = %d, outpow = %d\r\n", rf_sys_name[eSys], ptIso->LowerCnt, Cur5GDLAttSetVal, pow);
            }
            else if(pow > ZijiTH)
            {
//                YY 1008 remove
//                if(ptIso->attChange >= 0)
//                {
//                    bsp_Printf(RTT_ISO, RTT_CTRL_TEXT_BRIGHT_GREEN"ZiJi rising dismatch att decrese, cnt = %d, set_att = %d, outpow = %d\r\n", ptIso->UpperCnt, Cur5GDLAttSetVal, pow);
//                    ptIso->ZijiStatMachine = ZIJI_CLEAR;
//                    break;
//                }
                ptIso->UpperCnt++;
                ptIso->ZijiStatMachine = ZIJI_RISING_DETECTED;
                ptIso->ZijiAttVal[ptIso->UpperCnt - 1] = Cur5GDLAttSetVal;
                ptIso->PreTick = GetTick();
                bsp_printf(RTT_ISO, RTT_CTRL_TEXT_BRIGHT_YELLOW"[ %s ][ziji falling to rising] RF_SYS = %d cnt = %d, set_att = %d, outpow = %d\r\n", rf_sys_name[eSys], ptIso->UpperCnt, Cur5GDLAttSetVal, pow);

            }
            else // pow 介于ALC上下限之间(正常工作)
            {
                if(TICK_SINCE(ptIso->PreTick) > NR_DL_ZIJI_PERIOD)
                {
                    ptIso->ZijiStatMachine = ZIJI_CLEAR;
                    bsp_printf(RTT_ISO, RTT_CTRL_TEXT_BRIGHT_GREEN"[ %s ] ZiJi detection timeout = %d\r\n", rf_sys_name[eSys], TICK_SINCE(ptIso->PreTick));
                }
            }
            break;

        case ZIJI_DETECTED: // 检测到自激
            // AttInc 5DB

            ptIso->ZijiDectPeriod = GetTick() - ptIso->StartTick;
            ptIso->ZijiStatMachine = ZIJI_CLEAR;

            // 遍历寻找最大值
            for(index = 0; index < (ptIso->UpperCnt - 1); index++)
            {
                if(powHiAttMax < ptIso->ZijiAttVal[index])
                    powHiAttMax = ptIso->ZijiAttVal[index];
                if(powHighAttMin > ptIso->ZijiAttVal[index])
                    powHighAttMin = ptIso->ZijiAttVal[index];
            }

            // 遍历寻找最小值
            for(index = 0; index < (ptIso->LowerCnt - 1); index++)
            {
                if(powLoAttMax < ptIso->LowerAttVal[index])
                    powLoAttMax = ptIso->LowerAttVal[index];
                if(powLoAttMin > ptIso->LowerAttVal[index])
                    powLoAttMin = ptIso->LowerAttVal[index];
            }
            //判断是否平滑 yy1008 remove
            /*
            if((powHiAttMax - powHighAttMin) > 1 || (powLoAttMax - powLoAttMin) > 1)
            {
                bsp_printf(RTT_ISO, RTT_CTRL_TEXT_BRIGHT_GREEN"ZiJi detection nosie signal : hiMax = %d, hiMin = %d, loMax = %d, loMin = %d\r\n", powHiAttMax , powHighAttMin, powLoAttMax, powLoAttMin);
                ptIso->ZijiStatMachine = ZIJI_CLEAR;
                break;
            }*/

            ptIso->ZijiAttValMax = powHiAttMax;
            ptIso->ZijiAttValMin = powLoAttMin;

            bsp_printf(RTT_ISO, RTT_CTRL_TEXT_BRIGHT_RED"[ %s ][INFO] Get : UpperCnt = %d, maxatt = %d, minatt = %d, DectPeriod = %d\r\n", \
                rf_sys_name[eSys], ptIso->UpperCnt, ptIso->ZijiAttValMax, ptIso->ZijiAttValMin, ptIso->ZijiDectPeriod);
            // 自激确认 yy add 1008
            memcpy( &tAttForConfirm, ptAtt, sizeof( Att_T ) );
            res = scIsoChConfirm( eSys, eCh, ptPow, &tAttForConfirm, ptAlc, ptIso );
            //int8_t res = scIsoChConfirm( ptPow, &tAttForConfirm, ptAlc, ptIso );
            
            if(res != TRUE)
            {
                ptIso->ZijiStatMachine = ZIJI_CLEAR;
                break;
            }
            // 自激压衰减

            ptAtt->ucIso = ptIso->ZijiAttValMin + 8;
            pt->tAtt.ucIso = ptIso->ZijiAttValMin + 8;     //上行隔离度联动
            tRfChPara[eSys][eCh].ucIso = ptGain->scTheoreticalGain - ptAtt->ucIso + 10;

            bsp_printf(RTT_ISO, RTT_CTRL_TEXT_BRIGHT_WHITE"[ %s ][att info] dw = %d ISO = %d\r\n", rf_sys_name[eSys], ptAtt->ucIso, tRfChPara[eSys][eCh].ucIso);

            // 自激恢复
            bitsEvent = xEventGroupGetBits(xIsoEgp);
            
			if( BIT_ISO_SYS_RESET_EN_( eSys ) != (bitsEvent & BIT_ISO_SYS_RESET_EN_( eSys )))
			{
				bsp_printf(RTT_ISO, RTT_CTRL_TEXT_BRIGHT_WHITE"\r\n<<<<<<%s ISO REDETECT EN>>>>>>\r\n", rf_sys_name[eSys]);
				xEventGroupSetBits(xIsoEgp, BIT_ISO_SYS_RESET_EN_( eSys ));
			}

			
//            if(eSys == RF_SYS_1 && (BIT_ISO_SYS1_RESET_EN != (bitsEvent & BIT_ISO_SYS1_RESET_EN)))
//            {
//                bsp_printf(RTT_ISO, RTT_CTRL_TEXT_BRIGHT_WHITE"\r\n<<<<<<%s ISO REDETECT EN>>>>>>\r\n", rf_sys_name[eSys]);
//                xEventGroupSetBits(xIsoEgp, BIT_ISO_SYS1_RESET_EN);
//            }
			
//            if(eSys == RF_SYS_2 && (BIT_ISO_SYS2_RESET_EN != (bitsEvent & BIT_ISO_SYS2_RESET_EN)))
//            {
//                bsp_printf(RTT_ISO, RTT_CTRL_TEXT_BRIGHT_WHITE"\r\n<<<<<<%s ISO REDETECT EN>>>>>>\r\n", rf_sys_name[eSys]);
//                xEventGroupSetBits(xIsoEgp, BIT_ISO_SYS2_RESET_EN);
//            }

            
            // if(ptAtt->ucIso >= ((ptGain->scTheoreticalGain + 5) - ZIJI_CLOSE_MIMO_NUM))  // ISO <= 35
            if(tRfChPara[eSys][eCh].ucIso <= 35)  // ISO <= 35
            {
                vIsoSetCloseEventBit( eSys );
            }
            break;

        case ZIJI_CLEAR:
            ptIso->StartTick = 0;
            ptIso->PreTick = 0;
            ptIso->UpperCnt = 0;
            ptIso->LowerCnt = 0;

            ptIso->ZijiAttValMax = 0;
            ptIso->ZijiAttValMin = 0;
            ptIso->ZijiDectPeriod = 0;

            memset(ptIso->ZijiAttVal, 0, sizeof(ptIso->ZijiAttVal));
            memset(ptIso->LowerAttVal, 0, sizeof(ptIso->LowerAttVal));

            ptIso->ZijiStatMachine = ZIJI_START;
            break;

        default:
            break;
    }
    // ptIso->attChange = Cur5GDLAttSetVal - ptIso->preSetAttVal ;
    // ptIso->preSetAttVal = Cur5GDLAttSetVal;
}


/*****************************************************************************
* @Name	  : vIsoInRfProgress
* @Brief  : 隔离度检测程序
* @Param  : eSys: [输入/出] 
**			 eCh: [输入/出] 
* @Retval : 
* @Author : 
* @Data   : 2022-11-21
* @Note   : 在射频处理流程中调用
*****************************************************************************/
void vIsoInRfProgress( RF_SYS_E eSys, RF_CH_E eCh )
{
    EqIndependentPara_T *pt = &tRfChPara[eSys][eCh];
    
    vIsoGetRfPara();
    
    if( RF_CH_DW == eCh )
        vIsoChDetect( eSys, eCh, &pt->tPow, &pt->tGain, &pt->tAtt, &pt->tAlc, &pt->tIso );
}
