/***************************** Copyright ****************************************
*
* (C) Copyright 2024, Shaanxi Tianji Communication
*  All Rights Reserved
*
* FileName   : bsp_rf_iso.c
* Version    : none
* Author     : none
* Date       : 2024-03-23
* Description: none
*******************************************************************************/
#include "main.h"
#include "bsp_rf_iso.h"
#include "FreeRTOS.h"                   // ARM.FreeRTOS::RTOS:Core
#include "event_groups.h"               // ARM.FreeRTOS::RTOS:Event Groups
#include "bsp_rf_att.h"
#include "lna.h"
#include "bsp_rf_lna.h"
#include "bsp_rf_alc.h"
#include "bsp_rf_pow.h"
#include "bsp_wdg.h"
#include "bsp_rf_global.h"
#include "bsp_rf_att_set.h"
#include "hi_systick.h"
#include "hi_adc.h"
#include "rtt.h"
#include "ble.h"

/*---------------------- 端口定义 --------------------------------------*/

// 自激检测结构体
// 与射频通道参数相关的自激检测参数
#define NR_DL_ZIJI_HIGHPOW      ( 1000 * 60 * 5 )   // 大功率检测时间 单位ms
#define NR_DL_ZIJI_PERIOD       ( 1500)     // 5G下行自激超时 单位ms
#define NR_DL_ZIJI_RISING_TH    ( 1 )       // 5G下行自激上升沿阈值
#define NR_DL_ZIJI_FALLING_TH   ( 1 )       // 5G下行自激下降沿阈值

#define NR_DL_ZIJI_CONFIRM_TH   ( 9 )       // 5G下行自激诱导跳变阈值

#define NR_DL_ZIJI_CONFIRM_HI_ATT_OFFSET   ( 2 )      // 5G下行自激诱导确认,高功率att偏移量
#define NR_DL_ZIJI_CONFIRM_LO_ATT_OFFSET   ( 3 )       // 5G下行自激诱导确认,低功率att偏移量

#define NR_DL_ZIJI_CONFIRM_NUM       ( 3 )      // 自激检测诱导次数
#define NR_DL_ZIJI_CONFIRM_LO_TIME   ( 1000 )       // 自激检测诱导,低功率持续时间
#define NR_DL_ZIJI_CONFIRM_HI_TIME   ( 300 )       // 自激检测诱导,高功率持续时间

typedef enum
{
    ZIJI_CLEAR = 0,             // 自激清除
    ZIJI_START,                 // 自激起始
    ZIJI_RISING_DETECTED,       // 下行自激ADC检测上升沿阈值
    ZIJI_FALLING_DETECTED,      // 下行自激ADC检测下降沿阈值
    ZIJI_DETECTED,              // 下行自激ADC检测阈值
    ZIJI_CONFIRM_FINISH         // 自激确认完成
} eZJ;


/*---------------------- 宏定义  ---------------------------------------*/


#define TICK_SINCE(x) (hi_get_tick() - x)

#define RFCTR_STOP_ISO     (MIN_TO_TICKS(30))   //每隔30min进行5G自激检测
#define SYSTEM_ZIJI_DETECT_TIME     ( MIN_TO_TICKS(3) )       // 自激检测时间3min
#define SYNC_WAIT_TIME     ( S_TO_TICKS(5) )       // 同步后等待时间20s

#define ZIJI_RESET_NUM   (3)     //自激设置的增益回退值复位检测计数

#define RF_WAIT_ISO_CAL_TIME (5 * 1000)     // 等待隔离度检测所需时间5S


EventGroupHandle_t xIsoEgp = NULL;


/*---------------------- 结构体 ----------------------------------------*/
typedef struct
{
    uint32_t UpperCnt;          // 高计数
    uint32_t LowerCnt;          // 低计数
    uint32_t StartTick;         // 开始时间
    uint32_t PreTick;           // 执行时间
    uint32_t ZijiStatMachine;   // 自激状态机值
    uint32_t ZijiDectPeriod;    // 自激检测时间
    ATT_CH_T ZijiAttVal[NR_DL_ZIJI_CNT_NUM];     // 自激衰减器状态记录缓存
    ATT_CH_T LowerAttVal[NR_DL_ZIJI_CNT_NUM];    // 低功率衰减器状态记录缓存
    ATT_CH_T ZijiAttValMax;      // 自激最大衰减
    ATT_CH_T ZijiAttValMin;      // 自激最小衰减
    ATT_CH_T ZijiStartAttVal;    // 自激开始时衰减值
    /*用于微室分多机隔离度检测的参数*/
    uint32_t MutiConfirmStartTick;                  // 进入确认状态的时间
    uint32_t MutiSelfConfirmDelay;                      // 等待本机开始隔离度确认的时间
    uint32_t MutiAllConfirmDelay;                      // 等待全部设备完成隔离度确认的时间
    uint8_t ucAlc_cur_value;    // 当前ALC值
    uint8_t ucAlc_change_cnt;   // ALC变更次数
} Iso_Ch_Para_T;
Iso_Ch_Para_T iso_ch_para[RF_SYS_NUM][RF_CH_NUM] = {0};

/*---------------------- 常量 ------------------------------------------*/


/*---------------------- 全局变量 --------------------------------------*/


/*---------------------- 局部变量 --------------------------------------*/
uint16_t zj_run_time = 0;
uint16_t sync_time[RF_SYS_NUM] = {0};


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
        rtt_printf(RTT_OS, RTT_CTRL_TEXT_BRIGHT_RED"ERROR: xIsoEgp CREATE FAIL...\r\n");
    }
    else
    {
        xEventGroupClearBits(xIsoEgp, BIT_ISO_ALL );    // 清零
    }
}




/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ APP ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

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
    // static uint16_t usRfB3IsoTime = 0;
    // static uint16_t usRfN41IsoTime = 0;
    // static uint8_t ziji_reset_B3_num = 0;     // 4G自激一天恢复次数
    // static uint8_t ziji_reset_N41_num = 0;     // 5G自激一天恢复次数
    
    // EventBits_t bitsIso = xEventGroupGetBitsFromISR( xIsoEgp );
    // if((BIT_ISO_B3_RESET_EN == (bitsIso & BIT_ISO_B3_RESET_EN)) && (ziji_reset_B3_num < ZIJI_RESET_NUM) && tRfSysPublic[RF_SYS_B3].ucSysState == OPEN)
    // {
    //     usRfB3IsoTime++;
    //     if(usRfB3IsoTime >= RFCTR_STOP_ISO)
    //     {
    //         usRfB3IsoTime = 0;
    //         zj_run_time = 0;

    //         tRfChPara[RF_SYS_B3][RF_CH_UP].tAtt.ucIso = 0;
    //         tRfChPara[RF_SYS_B3][RF_CH_DW].tAtt.ucIso = 0;
    //         rtt_printf(RTT_4G_ISO, RTT_CTRL_TEXT_BRIGHT_WHITE"\r\n<<<<<<B3 ISO REDETECT>>>>>>\r\n");

    //         ziji_reset_B3_num++;      //释放衰减，重新进行自激检测处理，一天最多3次
    //         rtt_printf(RTT_4G_ISO, RTT_CTRL_TEXT_BRIGHT_WHITE"\r\n<<<<<<B3 ISO REDETECT CNT: %d>>>>>>\r\n", ziji_reset_B3_num);
    //         xEventGroupClearBitsFromISR(xIsoEgp, BIT_ISO_B3_RESET_EN);
    //     }
    // }
    // if((BIT_ISO_N41_RESET_EN == (bitsIso & BIT_ISO_N41_RESET_EN)) && (ziji_reset_N41_num < ZIJI_RESET_NUM) && tRfSysPublic[RF_SYS_N41].ucSysState == OPEN) {
    //     usRfN41IsoTime++;
    //     if(usRfN41IsoTime >= RFCTR_STOP_ISO)
    //     {
    //         usRfN41IsoTime = 0;
    //         zj_run_time = 0;

    //         tRfChPara[RF_SYS_N41][RF_CH_UP].tAtt.ucIso = 0;
    //         tRfChPara[RF_SYS_N41][RF_CH_DW].tAtt.ucIso = 0;
    //         rtt_printf(RTT_4G_ISO, RTT_CTRL_TEXT_BRIGHT_WHITE"\r\n<<<<<<NR ISO REDETECT>>>>>>\r\n");

    //         ziji_reset_N41_num++;      //释放衰减，重新进行自激检测处理，一天最多3次

    //         rtt_printf(RTT_4G_ISO, RTT_CTRL_TEXT_BRIGHT_WHITE"\r\n<<<<<<NR ISO REDETECT CNT: %d>>>>>>\r\n", ziji_reset_N41_num);
    //         xEventGroupClearBitsFromISR(xIsoEgp, BIT_ISO_N41_RESET_EN);
    //     }
    // }

    // if (zj_run_time <= SYSTEM_ZIJI_DETECT_TIME) {
    //     zj_run_time++;
    // }

    for (uint8_t sys = 0; sys < RF_SYS_NUM; sys++) {
        if (t_rf_sys[sys].t_ch[RF_CH_DW].tIso.weak_iso_start == ENABLE && sync_time[sys] <= SYSTEM_ZIJI_DETECT_TIME) {
            sync_time[sys]++;
        }
    }
}

static void vIsoSetAtt(RF_SYS_E sys, RF_CH_E ch, ATT_CH_T * pt_hal)
{

    if(ENABLE == pt_hal->digit_att1.en)
    {
        att_digit_set( sys, ch, pt_hal->digit_att1.new_value );
    }

    if(ENABLE == pt_hal->lna1.en)
    {
        lna_set(sys, ch, 1, pt_hal->lna1.new_value);
    }

    if(ENABLE == pt_hal->lna2.en)
    {
        lna_set(sys, ch, 2, pt_hal->lna2.new_value);
    }
}

/*****************************************************************************
* @Name   : iso_att_calc
* @Brief  : 自激衰减器计算
* @Param  : pt_hal: 衰减器指针
* @Retval :
* @Author :
* @Data   : 2024-03-24
* @Note   :
*****************************************************************************/
static uint8_t iso_att_calc(ATT_CH_T *pt_hal)
{
    uint8_t att_num = 0;
    att_num = pt_hal->digit_att1.cur_value + pt_hal->analog_att1.cur_value;
    if (pt_hal->lna1.cur_value == 0 && pt_hal->lna1.en == ENABLE) {
        att_num += pt_hal->lna1.max_num;
    }
    if (pt_hal->lna2.cur_value == 0 && pt_hal->lna2.en == ENABLE) {
        att_num += pt_hal->lna2.max_num;
    }
    return att_num;
}

/*****************************************************************************
* @Name   : iso_confirm_att_calc
* @Brief  : 自激确认衰减器计算
* @Param  : pt_hal: 衰减器指针
* @Retval :
* @Author :
* @Data   : 2024-03-24
* @Note   :
*****************************************************************************/
static void iso_confirm_att_calc(ATT_CH_T *ph_att, ATT_CH_T *pl_att, uint8_t ph_offset, uint8_t pl_offset)
{
    if (ph_att->digit_att1.cur_value >= ph_offset) {
        ph_att->digit_att1.new_value = ph_att->digit_att1.cur_value - ph_offset;
    } else {
        if (ph_att->lna2.cur_value == LNA_BYS && ph_att->lna2.en == ENABLE) {
            ph_att->lna2.new_value = LNA_RF;
            ph_att->digit_att1.new_value = ph_att->digit_att1.cur_value + ph_att->lna2.max_num - ph_offset;
        } else if (ph_att->lna1.cur_value == LNA_BYS && ph_att->lna1.en == ENABLE) {
            ph_att->lna1.new_value = LNA_RF;
            ph_att->digit_att1.new_value = ph_att->digit_att1.cur_value + ph_att->lna1.max_num - ph_offset;
        } else {
            ph_att->digit_att1.new_value = 0;
        }

        if (ph_att->digit_att1.new_value == 0) {
            pl_att->digit_att1.new_value = ph_att->digit_att1.cur_value + ph_offset + pl_offset;
            if (pl_att->lna1.en == ENABLE) {
                pl_att->lna1.new_value = LNA_RF;
            }
            if (pl_att->lna2.en == ENABLE) {
                pl_att->lna2.new_value = LNA_RF;
            }
            return;
        }
    }

    if (pl_att->digit_att1.cur_value + pl_offset > pl_att->digit_att1.max_num) {
        if (pl_att->lna1.cur_value == LNA_RF && pl_att->lna1.en == ENABLE) {
            pl_att->lna1.new_value = LNA_BYS;
            pl_att->digit_att1.new_value = pl_att->digit_att1.cur_value - pl_att->lna1.max_num + pl_offset;
        } else if (pl_att->lna2.cur_value == LNA_RF && pl_att->lna2.en == ENABLE) {
            pl_att->lna2.new_value = LNA_BYS;
            pl_att->digit_att1.new_value = pl_att->digit_att1.cur_value - pl_att->lna2.max_num + pl_offset;
        } else {
            pl_att->digit_att1.new_value = pl_att->digit_att1.max_num;
        }

        if (pl_att->digit_att1.new_value == pl_att->digit_att1.max_num) {
            ph_att->digit_att1.new_value = ph_att->digit_att1.cur_value - ph_offset - pl_offset;
            if (ph_att->lna1.en == ENABLE) {
                ph_att->lna1.new_value = LNA_BYS;
            }
            if (ph_att->lna2.en == ENABLE) {
                ph_att->lna2.new_value = LNA_BYS;
            }
        }
    } else {
        pl_att->digit_att1.new_value = pl_att->digit_att1.cur_value + pl_offset;
    }
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

static int scIsoChConfirm(RF_SYS_E eSys, RF_CH_E eCh, RfChPara_T *pt_rf, Iso_Ch_Para_T *pt_iso)
{
    uint8_t comfirmCnt = 0, confirmStat = ZIJI_CONFIRM_PWR_HI;
    ATT_CH_T ConfirmPwrHiAttVal = {0}, ConfirmPwrLoAttVal = {0};
    int8_t pow = 0, powDiff = 0;  // 获取输出功率
    int8_t ZijiTH = pt_rf->tAlc.scAlcTheoreticalPoint + NR_DL_ZIJI_RISING_TH;   //自激检测门限
    uint8_t ZijiPwrVal[NR_DL_ZIJI_CONFIRM_NUM] = {0};
    uint32_t thetick = 0;
    
    /*1参数初始化*/
    pt_iso->PreTick = hi_get_tick();

    ConfirmPwrHiAttVal = pt_iso->ZijiAttValMin;
    ConfirmPwrLoAttVal = pt_iso->ZijiAttValMin;

    iso_confirm_att_calc(&ConfirmPwrHiAttVal, &ConfirmPwrLoAttVal, NR_DL_ZIJI_CONFIRM_HI_ATT_OFFSET, NR_DL_ZIJI_CONFIRM_LO_ATT_OFFSET);
    rtt_printf(RTT_4G_ISO, RTT_CTRL_TEXT_BRIGHT_RED"Hi Att cur: %d, new: %d\r\n", pt_iso->ZijiAttValMin.digit_att1.cur_value, ConfirmPwrHiAttVal.digit_att1.new_value);
    rtt_printf(RTT_4G_ISO, RTT_CTRL_TEXT_BRIGHT_RED"Lo Att cur: %d, new: %d\r\n", pt_iso->ZijiAttValMin.digit_att1.cur_value, ConfirmPwrLoAttVal.digit_att1.new_value);

    /*2设置衰减器*/
    vIsoSetAtt( eSys, eCh, &ConfirmPwrHiAttVal );   // 按自激工况设置数字衰减
    vTaskDelay(150);
    hi_adc_get_value();
    
    /*3自激确认*/
    while(comfirmCnt < NR_DL_ZIJI_CONFIRM_NUM)
    {
        thetick = hi_get_tick();
        hi_adc_get_value();
        pow_convert();                  //ADC转换为输出功率

        pow = t_ch_pow[eSys][eCh].pow.ave;

        if(confirmStat == ZIJI_CONFIRM_PWR_HI)//按自激工况设置数字衰减,确认是否诱导自激
        {
            if(pow <= ZijiTH)//当检测到输出信号功率低,自激检测退出
            {
                rtt_printf(RTT_4G_ISO, RTT_CTRL_TEXT_BRIGHT_GREEN"ZiJi confirm Hi failure cnt = %d, set_att = %d, outpow = %d, tick = %d\r\n", comfirmCnt, ConfirmPwrHiAttVal.digit_att1.new_value, pow, thetick);
                break;
            }
            else if(pow > ZijiTH && (TICK_SINCE(pt_iso->PreTick) > NR_DL_ZIJI_CONFIRM_HI_TIME))//输出功率持续高于阈值,进行低功率信号确认
            {
                vIsoSetAtt( eSys, eCh, &ConfirmPwrLoAttVal);    // 按非自激工况设置数字衰减
                rtt_printf(RTT_4G_ISO, RTT_CTRL_TEXT_BRIGHT_YELLOW"ZiJi confirm Hi to Lo cnt = %d, set_att = %d, outpow = %d, tick = %d\r\n", comfirmCnt, ConfirmPwrHiAttVal.digit_att1.new_value, pow, thetick);
                pt_iso->PreTick = hi_get_tick();
                confirmStat = ZIJI_CONFIRM_PWR_LO;
                vTaskDelay(150);
                continue;
            }
            else if(pow > ZijiTH) //输出大于阈值时,更新pow,获取当前周期最小pow值
            {
                rtt_printf(RTT_4G_ISO, RTT_CTRL_TEXT_BRIGHT_YELLOW"ZiJi confirm Hi cnt = %d, set_att = %d, outpow = %d, tick = %d\r\n", comfirmCnt, ConfirmPwrHiAttVal.digit_att1.new_value, pow, thetick);
                if(ZijiPwrVal[comfirmCnt] == 0)
                    ZijiPwrVal[comfirmCnt] = pow;
                else if(ZijiPwrVal[comfirmCnt] > pow)
                    ZijiPwrVal[comfirmCnt] = pow;
                vTaskDelay(150);
            }
        }
        else if(confirmStat == ZIJI_CONFIRM_PWR_LO)//压缩增益,确认是否向下跳变
        {
            powDiff = ZijiPwrVal[comfirmCnt] - pow;//计算跳变值
            if(powDiff < NR_DL_ZIJI_CONFIRM_TH)//当检测到输出信号跳变差值小于阈值,自激检测退出
            {
                rtt_printf(RTT_4G_ISO, RTT_CTRL_TEXT_BRIGHT_GREEN"ZiJi confirm Lo failure cnt = %d, set_att = %d, outpow = %d, tick = %d\r\n", comfirmCnt, ConfirmPwrLoAttVal.digit_att1.new_value, pow, thetick);
                break;
            }
            else if(powDiff >= NR_DL_ZIJI_CONFIRM_TH && (TICK_SINCE(pt_iso->PreTick) > NR_DL_ZIJI_CONFIRM_LO_TIME))//输出功率持续低于阈值,进行高功率信号确认
            {
                comfirmCnt++;
                if(comfirmCnt >= (NR_DL_ZIJI_CONFIRM_NUM))//完成确认
                {
                    rtt_printf(RTT_4G_ISO, RTT_CTRL_TEXT_BRIGHT_GREEN"ZiJi confirmd cnt = %d, set_att = %d, outpow = %d, tick = %d\r\n", comfirmCnt - 1, ConfirmPwrLoAttVal.digit_att1.new_value, pow, thetick);
                    return B_TRUE;
                }
                rtt_printf(RTT_4G_ISO, RTT_CTRL_TEXT_BRIGHT_CYAN"ZiJi confirm Lo to Hi cnt = %d, set_att = %d, outpow = %d, tick = %d\r\n", comfirmCnt, ConfirmPwrLoAttVal.digit_att1.new_value, pow, thetick);

                vIsoSetAtt( eSys, eCh, &ConfirmPwrHiAttVal); //按自激工况设置数字衰减
                pt_iso->PreTick = hi_get_tick();
                confirmStat = ZIJI_CONFIRM_PWR_HI;
                vTaskDelay(150);
            }
        }
        vTaskDelay(5);
    }
    return B_FALSE;
}


/*****************************************************************************
* @Name	  : vIsoChDetect
* @Brief  : 通道隔离度检测
* @Param  : ptPow: [输入/出] 
**          ptGain: [输入/出] 
**          ptAtt: [输入/出] 
**          ptAlc: [输入/出] 
**          ptIso: [输入/出] 
* @Retval : 
* @Author : 
* @Data   : 2022-11-21
* @Note   :
*****************************************************************************/
static void vIsoChDetect(RF_SYS_E eSys, RF_CH_E eCh)
{
    RfChPara_T *pt_rf_para = &t_rf_sys[eSys].t_ch[eCh];
    Iso_Ch_Para_T *pt_iso_para = &iso_ch_para[eSys][eCh];
    ATT_CH_T CurDLAttSetVal = *pt_ch_att[eSys][eCh]; // 获取当前衰减设置值
    ATT_CH_T powHiAttMax = {0}, powHighAttMin = {0}; //输出高功率时的att最大最小值
    ATT_CH_T powLoAttMax = {0}, powLoAttMin = {0}; //输出低功率时的att最大最小值
    uint8_t curattvalue = 0;

    int8_t pow = t_ch_pow[eSys][eCh].pow.ave;    // 获取输出功率
    int8_t ZijiTH  = pt_rf_para->tAlc.scAlcTheoreticalPoint + NR_DL_ZIJI_RISING_TH;    // 自激检测门限
    int8_t LowerTH = pt_rf_para->tAlc.scAlcTheoreticalPoint - NR_DL_ZIJI_FALLING_TH;   // 线性工作区门限
    uint8_t index = 0;
    int8_t res = B_TRUE;
    uint8_t zj_cnt_num = NR_DL_ZIJI_CNT_NUM;
    

    switch (pt_iso_para->ZijiStatMachine) {
        case ZIJI_START:
            if (sync_time[eSys] < S_TO_TICKS(5) && hBle.ble_stat.slave_connect_num > 0) {
                pt_rf_para->tIso.weak_iso_start = ENABLE;
                if (pt_rf_para->tAtt.ucPresent >= 3 && pt_rf_para->tIso.cur_present_att == 0) {
                    pt_rf_para->tIso.cur_present_att = pt_rf_para->tAtt.ucPresent;
                    pt_rf_para->tAtt.ucPresent -= 3;
                    rtt_printf(RTT_4G_ISO, RTT_CTRL_TEXT_BRIGHT_RED"SYS:%d Induce Iso Start\r\n", eSys + 1);
                }
            }
            if(pow > ZijiTH) {
                pt_iso_para->StartTick = hi_get_tick();
                pt_iso_para->PreTick = hi_get_tick();
                pt_iso_para->UpperCnt++;   // 大于功率上限计数
                pt_iso_para->ZijiAttVal[pt_iso_para->UpperCnt - 1] = CurDLAttSetVal;    // 记录当前衰减状态
                curattvalue = iso_att_calc(&CurDLAttSetVal);
                pt_iso_para->ZijiStartAttVal = CurDLAttSetVal;
                pt_iso_para->ZijiStatMachine = ZIJI_RISING_DETECTED;
                pt_iso_para->MutiSelfConfirmDelay = RF_WAIT_ISO_CAL_TIME;
                pt_iso_para->MutiConfirmStartTick = 0;
                rtt_printf(RTT_4G_ISO, RTT_CTRL_TEXT_BRIGHT_GREEN "\r\n[ziji start] RF_SYS = %d cnt = %d, set_att = %d, outpow = %d\r\n", \
                    eSys + 1, pt_iso_para->UpperCnt, curattvalue, pow);
            } else {
               if (sync_time[eSys] >= S_TO_TICKS(20)) {
                    pt_rf_para->tIso.weak_iso_start = DISABLE;
                    if (pt_rf_para->tIso.cur_present_att != 0) {
                        pt_rf_para->tAtt.ucPresent = pt_rf_para->tIso.cur_present_att;
                        pt_rf_para->tIso.cur_present_att = 0;
                        rtt_printf(RTT_4G_ISO, RTT_CTRL_TEXT_BRIGHT_RED"SYS:%d Induce Iso end\r\n", eSys + 1);
                    }
                }
            }
            break;

        case ZIJI_RISING_DETECTED: // 检测到上升沿
            //确认自激
            if(pt_iso_para->UpperCnt >= zj_cnt_num) {
                pt_iso_para->ZijiStatMachine = ZIJI_DETECTED;
                // 赋初始值
                pt_iso_para->ZijiAttValMax = CurDLAttSetVal;
                pt_iso_para->ZijiAttValMin = CurDLAttSetVal;
                break;
            }
            //自激检测判断
            if(pow > ZijiTH) {  //功率高于起控点时
                if (TICK_SINCE(pt_iso_para->PreTick) > NR_DL_ZIJI_PERIOD) {
                    pt_iso_para->ZijiStatMachine = ZIJI_CLEAR; // 清除，高功率超时
                    rtt_printf(RTT_4G_ISO, RTT_CTRL_TEXT_BRIGHT_RED"[WARN] : DL channel Power the over limit %d\r\n", TICK_SINCE(pt_iso_para->PreTick));
                    break;
                }
                //记录自激的衰减最大值
                if(iso_att_calc(&pt_iso_para->ZijiAttVal[pt_iso_para->UpperCnt - 1]) < iso_att_calc(&CurDLAttSetVal)) {
                    pt_iso_para->ZijiAttVal[pt_iso_para->UpperCnt - 1] = CurDLAttSetVal;
                }
                curattvalue = iso_att_calc(&CurDLAttSetVal);
                rtt_printf(RTT_4G_ISO, RTT_CTRL_TEXT_BRIGHT_YELLOW"[ziji rising] RF_SYS = %d cnt = %d, set_att = %d, outpow = %d, time = %d\r\n", eSys + 1, pt_iso_para->UpperCnt, curattvalue, pow, TICK_SINCE(pt_iso_para->PreTick));
                //功率低于起控点
            } else if(pow < LowerTH) {
                pt_iso_para->LowerCnt++;
                pt_iso_para->LowerAttVal[pt_iso_para->LowerCnt - 1] = CurDLAttSetVal;
                curattvalue = iso_att_calc(&CurDLAttSetVal);
                pt_iso_para->ZijiStatMachine = ZIJI_FALLING_DETECTED;
                pt_iso_para->PreTick = hi_get_tick();
                rtt_printf(RTT_4G_ISO, RTT_CTRL_TEXT_BRIGHT_CYAN"[ziji rising to falling] RF_SYS = %d cnt = %d, set_att = %d, outpow = %d, time = %d\r\n", eSys + 1, pt_iso_para->LowerCnt, curattvalue, pow, pt_iso_para->PreTick);
            } else { // pow 介于ALC上下限之间(正常工作)
                if(TICK_SINCE(pt_iso_para->PreTick) > NR_DL_ZIJI_PERIOD) {  //超时退出检测
                    pt_iso_para->ZijiStatMachine = ZIJI_CLEAR;
                    rtt_printf(RTT_4G_ISO, RTT_CTRL_TEXT_BRIGHT_GREEN"[INFO] : RF_SYS = %d DL ZIJI detection timeout = %d\r\n", eSys + 1, TICK_SINCE(pt_iso_para->PreTick));
                }
            }
            break;

        case ZIJI_FALLING_DETECTED:
            if(pow < LowerTH) {
                if (TICK_SINCE(pt_iso_para->PreTick) > NR_DL_ZIJI_PERIOD) {  //检测自激超时
                    pt_iso_para->ZijiStatMachine = ZIJI_CLEAR;
                    rtt_printf(RTT_4G_ISO, RTT_CTRL_TEXT_BRIGHT_CYAN"ZiJi falling detection timeout = %d RF_SYS = %d\r\n", TICK_SINCE(pt_iso_para->PreTick), eSys + 1);
                }
                if (iso_att_calc(&pt_iso_para->LowerAttVal[pt_iso_para->LowerCnt - 1]) > iso_att_calc(&CurDLAttSetVal)) {
                    pt_iso_para->LowerAttVal[pt_iso_para->LowerCnt - 1] = CurDLAttSetVal;
                }
                curattvalue = iso_att_calc(&CurDLAttSetVal);
                rtt_printf(RTT_4G_ISO, RTT_CTRL_TEXT_BRIGHT_CYAN"[ziji falling] cnt = %d, set_att = %d, outpow = %d, time = %d\r\n", pt_iso_para->LowerCnt, curattvalue, pow, TICK_SINCE(pt_iso_para->PreTick));
            } else if(pow > ZijiTH) {
                pt_iso_para->UpperCnt++;
                pt_iso_para->ZijiStatMachine = ZIJI_RISING_DETECTED;
                pt_iso_para->ZijiAttVal[pt_iso_para->UpperCnt - 1] = CurDLAttSetVal;
                pt_iso_para->PreTick = hi_get_tick();
                curattvalue = iso_att_calc(&CurDLAttSetVal);
                rtt_printf(RTT_4G_ISO, RTT_CTRL_TEXT_BRIGHT_YELLOW"[ziji falling to rising] RF_SYS = %d cnt = %d, set_att = %d, outpow = %d, time = %d\r\n", eSys + 1, pt_iso_para->UpperCnt, curattvalue, pow, pt_iso_para->PreTick);
            } else { // pow 介于ALC上下限之间(正常工作)
                if(TICK_SINCE(pt_iso_para->PreTick) > NR_DL_ZIJI_PERIOD) {
                    pt_iso_para->ZijiStatMachine = ZIJI_CLEAR;
                    rtt_printf(RTT_4G_ISO, RTT_CTRL_TEXT_BRIGHT_GREEN"ZiJi detection timeout = %d\r\n", TICK_SINCE(pt_iso_para->PreTick));
                }
            }
            break;

        case ZIJI_DETECTED: // 检测到自激
            pt_iso_para->ZijiStatMachine = ZIJI_CONFIRM_FINISH;
            pt_iso_para->ZijiDectPeriod = hi_get_tick() - pt_iso_para->StartTick;
            // 遍历寻找最大值
            powHighAttMin = pt_iso_para->ZijiAttVal[0];
            for(index = 0; index < (pt_iso_para->UpperCnt - 1); index++) {
                if(iso_att_calc(&powHiAttMax) < iso_att_calc(&pt_iso_para->ZijiAttVal[index])) {
                    powHiAttMax = pt_iso_para->ZijiAttVal[index];
                }
                if(iso_att_calc(&powHighAttMin) > iso_att_calc(&pt_iso_para->ZijiAttVal[index])) {
                    powHighAttMin = pt_iso_para->ZijiAttVal[index];
                }
            }

            // 遍历寻找最小值
            powLoAttMin = pt_iso_para->LowerAttVal[0];
            for(index = 0; index < (pt_iso_para->LowerCnt - 1); index++) {
                if(iso_att_calc(&powLoAttMax) < iso_att_calc(&pt_iso_para->LowerAttVal[index])) {
                    powLoAttMax = pt_iso_para->LowerAttVal[index];
                }
                if(iso_att_calc(&powLoAttMin) > iso_att_calc(&pt_iso_para->LowerAttVal[index])) {
                    powLoAttMin = pt_iso_para->LowerAttVal[index];
                }
            }
            pt_iso_para->ZijiAttValMax = powHiAttMax;
            pt_iso_para->ZijiAttValMin = powLoAttMin;
            rtt_printf(RTT_4G_ISO, RTT_CTRL_TEXT_BRIGHT_RED"[INFO] Get : RF_SYS = %d UpperCnt = %d, maxatt = %d, minatt = %d, DectPeriod = %d\r\n", \
                eSys + 1, pt_iso_para->UpperCnt, iso_att_calc(&pt_iso_para->ZijiAttValMax), iso_att_calc(&pt_iso_para->ZijiAttValMin), pt_iso_para->ZijiDectPeriod);

            pt_rf_para->tIso.lockAlc = ENABLE;
            t_rf_sys[eSys].t_ch[RF_CH_UP].tIso.lockAlc = ENABLE;
            pt_iso_para->MutiConfirmStartTick = hi_get_tick();
            vIsoSetAtt(eSys, eCh, &powHiAttMax);   // 按自激中高功率最大衰减
            rtt_printf(RTT_4G_ISO, RTT_CTRL_TEXT_BRIGHT_RED"RF_SYS = %d  ALC OFF\r\n", eSys + 1);
            break;
        case ZIJI_CONFIRM_FINISH:
            if((pt_iso_para->MutiConfirmStartTick != 0) && (TICK_SINCE(pt_iso_para->MutiConfirmStartTick) <= pt_iso_para->MutiSelfConfirmDelay)) {
                break;
            }
            pt_iso_para->ZijiStatMachine = ZIJI_CLEAR;
            res = scIsoChConfirm(eSys, eCh, pt_rf_para, pt_iso_para);
            vIsoSetAtt(eSys, eCh, &CurDLAttSetVal);
            if(res != B_TRUE) {
                break;
            }
            if (pt_rf_para->tIso.cur_present_att != 0) {
                pt_rf_para->tAtt.ucPresent = pt_rf_para->tIso.cur_present_att;
                pt_rf_para->tIso.cur_present_att = 0;
                rtt_printf(RTT_4G_ISO, RTT_CTRL_TEXT_BRIGHT_RED"SYS:%d Induce Iso end\r\n", eSys + 1);
            }
            // 自激压衰减
            pt_rf_para->tAtt.ucIso = iso_att_calc(&pt_iso_para->ZijiAttValMin) - pt_rf_para->tAtt.ucPresent + 7;     // 预压的5dB已经包含在衰减值，需去掉
            t_rf_sys[eSys].t_ch[RF_CH_UP].tAtt.ucIso = pt_rf_para->tAtt.ucIso - 10;     // 上行隔离度联动，下行预压10dB，需去掉
            pt_rf_para->tIso.iso = pt_rf_para->tGain.scTheoreticalGain + 10 - pt_rf_para->tAtt.ucIso + 10;

            rtt_printf(RTT_4G_ISO, RTT_CTRL_TEXT_BRIGHT_WHITE"[att info] RF_SYS = %d dw = %d ISO = %d\r\n", eSys + 1, pt_rf_para->tAtt.ucIso, pt_rf_para->tIso.iso);

            // 自激恢复
            // bitsEvent = xEventGroupGetBits(xIsoEgp);
            // if(eSys == RF_SYS_B3 && (BIT_ISO_B3_RESET_EN != (bitsEvent & BIT_ISO_B3_RESET_EN))) {
            //     rtt_printf(RTT_4G_ISO, RTT_CTRL_TEXT_BRIGHT_WHITE"\r\n<<<<<<ISO REDETECT EN>>>>>>\r\n");
            //     xEventGroupSetBits(xIsoEgp, BIT_ISO_B3_RESET_EN);
            // }

            // if(eSys == RF_SYS_N41 && (BIT_ISO_N41_RESET_EN != (bitsEvent & BIT_ISO_N41_RESET_EN))) {
            //     rtt_printf(RTT_4G_ISO, RTT_CTRL_TEXT_BRIGHT_WHITE"\r\n<<<<<<ISO REDETECT EN>>>>>>\r\n");
            //     xEventGroupSetBits(xIsoEgp, BIT_ISO_N41_RESET_EN);
            // }

            if(pt_rf_para->tIso.iso <= 35) {  // ISO <= 35
                // if (eSys == RF_SYS_B3) {
                //     xEventGroupSetBits(xIsoEgp, BIT_ISO_B3_CLOSE_MIMO);
                //     rtt_printf(RTT_4G_ISO, RTT_CTRL_TEXT_BRIGHT_WHITE"\r\n<<<<<<B3 ISO CLOSE>>>>>>\r\n");
                // } else if (eSys == RF_SYS_N41) {
                //     xEventGroupSetBits(xIsoEgp, BIT_ISO_N41_CLOSE_MIMO);
                //     rtt_printf(RTT_4G_ISO, RTT_CTRL_TEXT_BRIGHT_WHITE"\r\n<<<<<<N41 ISO CLOSE>>>>>>\r\n");
                // }
                rtt_printf(RTT_4G_ISO, RTT_CTRL_TEXT_BRIGHT_WHITE"\r\n<<<<<<SYS%d ISO CLOSE>>>>>>\r\n", eSys + 1);
            }
            break;

        case ZIJI_CLEAR:
            if((pt_iso_para->MutiConfirmStartTick != 0) && (TICK_SINCE(pt_iso_para->MutiConfirmStartTick) <= pt_iso_para->MutiSelfConfirmDelay)) {
                break;
            } else {
                pt_rf_para->tIso.lockAlc = DISABLE;
                t_rf_sys[eSys].t_ch[RF_CH_UP].tIso.lockAlc = DISABLE;
            }
            memset(pt_iso_para, 0, sizeof(Iso_Ch_Para_T));

            pt_iso_para->ZijiStatMachine = ZIJI_START;
            break;

        default:
            break;
    }
}


/*****************************************************************************
* @Name   : vIsoInRfProgress
* @Brief  : 隔离度检测程序
* @Param  : eSys: [输入/出] 
**          eCh: [输入/出] 
* @Retval : 
* @Author : 
* @Data   : 2022-11-21
* @Note   : 在射频处理流程中调用
*****************************************************************************/
void vIsoInRfProgress( RF_SYS_E eSys, RF_CH_E eCh )
{
    if( RF_CH_DW == eCh ) {
        vIsoChDetect( eSys, eCh );
    }
}
