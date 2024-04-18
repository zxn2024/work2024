/***************************** Copyright ****************************************
*
* (C) Copyright 2022, Shaanxi Tianji Communication
*  All Rights Reserved
*
* FileName   : bsp_temperature.c
* Version    : none
* Author     : ZhangWei
* Date       : 2022-11-21
* Description:
*******************************************************************************/
#include "main.h"

#include "system_config.h"
#include "bsp_public_drv.h"

#include "bsp_temperature.h"
#include "rtt.h"
#include "FreeRTOS.h"
#include "event_groups.h"
//#include "bsp_rf_ctr.h"
#include "bsp_rf_global.h"
#include "bsp_rf_4g_band_sel.h"

/*---------------------- 端口定义 --------------------------------------*/


/*---------------------- 宏定义  ---------------------------------------*/
#define TMP_COMPENSAVTION_EN    ( 1 )   //0: 禁止温补功能, 1: 允许温补功能
#define USE_EXTER_SENSOR		( 1 )	//0: 未使用, 1:使用外部传感器

#if USE_EXTER_SENSOR
    #define TMP751AID_ADDRESS 0x90      // 地址
#endif

#define TEMP_MIN    ( -20 ) //最低温度值 -20 
#define TEMP_STEP   ( 10 )  //步进为 10 

/*---------------------- 结构体 ----------------------------------------*/
typedef struct
{
    uint8_t      en;                 //使能 ENABLE / DISABLE
    stTempData   stTempDataInUse;    //正在使用的温度补偿数据
    stTempData * ptOffsetList;  		 //补偿功率表指针
    uint8_t listLen;
} stTempOffset_T;


/*---------------------- 常量 ------------------------------------------*/
//特定温度对应的 增益功率补偿值数据, 温度 / 增益 / 功率
stTempData staTempData_B3_up[] = { 	{ 10,   1, 0 },
									{ 45,   0, 0 },
									{ 65,  -2, 0 },
									{ 127, -3, 0 }
								};
#define TEMP_LEVEL_B3_UP  ( sizeof( staTempData_B3_up ) / (sizeof( stTempData )) )

stTempData staTempData_B3_dw[] = { 	{ 10,   1, 0 },
									{ 45,   0, 0 },
									{ 65,  -1, 0 },
									{ 127, -3, 0 }
								};

#define TEMP_LEVEL_B3_DW  ( sizeof( staTempData_B3_dw ) / (sizeof( stTempData )) )

stTempData staTempData_B8_up[] = { 	{ 10,   1, 0 },
									{ 45,   0, 0 },
									{ 65,  -2, 0 },
									{ 127, -3, 0 }
								};
#define TEMP_LEVEL_B8_UP  ( sizeof( staTempData_B8_up ) / (sizeof( stTempData )) )

stTempData staTempData_B8_dw[] = { 	{ 10,   1, 0 },
									{ 45,   0, 0 },
									{ 65,  -1, 0 },
									{ 127, -3, 0 }
								};

#define TEMP_LEVEL_B8_DW  ( sizeof( staTempData_B8_dw ) / (sizeof( stTempData )) )

stTempData staTempData_B39_up[] = { { 10,   1, 0 },
									{ 45,   0, 0 },
									{ 65,  -2, 0 },
									{ 127, -3, 0 }
								};
#define TEMP_LEVEL_B39_UP  ( sizeof( staTempData_B39_up ) / (sizeof( stTempData )) )

stTempData staTempData_B39_dw[] = { { 10,   1, 0 },
									{ 45,   0, 0 },
									{ 65,  -1, 0 },
									{ 127, -3, 0 }
								};

#define TEMP_LEVEL_B39_DW  ( sizeof( staTempData_B39_dw ) / (sizeof( stTempData )) )

stTempData staTempData_B40_up[] = { { 10,   1, 0 },
									{ 45,   0, 0 },
									{ 65,  -2, 0 },
									{ 127, -3, 0 }
								};
#define TEMP_LEVEL_B40_UP  ( sizeof( staTempData_B40_up ) / (sizeof( stTempData )) )

stTempData staTempData_B40_dw[] = { { 10,   1, 0 },
									{ 45,   0, 0 },
									{ 65,  -1, 0 },
									{ 127, -3, 0 }
								};

#define TEMP_LEVEL_B40_DW  ( sizeof( staTempData_B40_dw ) / (sizeof( stTempData )) )

stTempData staTempData_5G_up[] = { 	{ 10,   1, 0 },
									{ 45,   0, 0 },
									{ 65,  -2, 0 },
									{ 127, -3, 0 }
								};
#define TEMP_LEVEL_5G_UP  ( sizeof( staTempData_5G_up ) / (sizeof( stTempData )) )

stTempData staTempData_5G_dw[] = { 	{ 10,   1, 0 },
									{ 45,   0, 0 },
									{ 65,  -1, 0 },
									{ 127, -3, 0 }
								};

#define TEMP_LEVEL_5G_DW  ( sizeof( staTempData_5G_dw ) / (sizeof( stTempData )) )


stTempData staTempData_MIMO_up[] = {{ 10,   1, 0 },
									{ 45,   0, 0 },
									{ 65,  -2, 0 },
									{ 127, -3, 0 }
								};
#define TEMP_LEVEL_MIMO_UP  ( sizeof( staTempData_MIMO_up ) / (sizeof( stTempData )) )

stTempData staTempData_MIMO_dw[] = {{ 10,   1, 0 },
									{ 45,   0, 0 },
									{ 65,  -1, 0 },
									{ 127, -3, 0 }
								};
#define TEMP_LEVEL_MIMO_DW  ( sizeof( staTempData_MIMO_dw ) / (sizeof( stTempData )) )




/*---------------------- 全局变量 --------------------------------------*/


/*---------------------- 局部变量 --------------------------------------*/
int8_t  scTemperature = 0;            //温度值
uint8_t ucTmpMainSwitch = ENABLE;     //总开关, ENABLE: 温补可用, DISABLE: 温补不可用
uint8_t ucTmpValueFreshSign = B_FALSE;  //更新温度数据标识, B_TRUE: 更新 B_FALSE: 不计算

stTempOffset_T tTemp[ RF_SYS_NUM ][RF_CH_NUM ];


/*---------------------- 事件组 ------------------------------------------*/



/*---------------------- 声明 ------------------------------------------*/



/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ HAL ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/



/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DRV ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/



/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ OS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/



/*****************************************************************************
* @Name	  : tmp_cnt_in_time_isr
* @Brief  : 温度补偿时间计数器, 每间隔2s采样一次数据并计算温补
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2022-11-22
* @Note   : 软件定时器中调用
*****************************************************************************/
void tmp_cnt_in_time_isr(void)    //定时器计数
{
#define TMP_FRESH_CNT 	S_TO_TICKS( 2 )

    static uint16_t usTmpValueFreshCnt = 0;

    usTmpValueFreshCnt++;
    if(usTmpValueFreshCnt >= TMP_FRESH_CNT)
    {
        usTmpValueFreshCnt = 0;
        ucTmpValueFreshSign = B_TRUE;
    }
}


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ APP ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*****************************************************************************
* @Name	  : tmp_init
* @Brief  : 温补数据初始化
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2022-11-22
* @Note   :
*****************************************************************************/
void tmp_init(void)    //初始化数据
{
    uint8_t sys, ch;
    for(sys = 0; sys < RF_SYS_NUM; sys++)
    {
        for(ch = 0; ch < RF_CH_NUM; ch++)
        {
            memset(&tTemp[sys][ch].stTempDataInUse, 0, sizeof(stTempData));
            tTemp[sys][ch].en = ENABLE;
        }
    }
}

/*****************************************************************************
* @Name	  : tmp_tmp_data_sel
* @Brief  : 选择有效的温补数据
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2023-11-02
* @Note   :
*****************************************************************************/
void tmp_tmp_data_sel(void)
{
    uint8_t valid_band = get_valid_band(RF_SYS_1);

    if(BAND_3 == valid_band)
    {
        tTemp[RF_SYS_1][RF_CH_UP].ptOffsetList = staTempData_B3_up;
        tTemp[RF_SYS_1][RF_CH_UP].listLen      = TEMP_LEVEL_B3_UP;
        tTemp[RF_SYS_1][RF_CH_DW].ptOffsetList = staTempData_B3_dw;
        tTemp[RF_SYS_1][RF_CH_DW].listLen      = TEMP_LEVEL_B3_DW;
    }
    else if(BAND_8 == valid_band)
    {
        tTemp[RF_SYS_1][RF_CH_UP].ptOffsetList = staTempData_B8_up;
        tTemp[RF_SYS_1][RF_CH_UP].listLen      = TEMP_LEVEL_B8_UP;
        tTemp[RF_SYS_1][RF_CH_DW].ptOffsetList = staTempData_B8_dw;
        tTemp[RF_SYS_1][RF_CH_DW].listLen      = TEMP_LEVEL_B8_DW;
    }
    else if(BAND_39 == valid_band)
    {
        tTemp[RF_SYS_1][RF_CH_UP].ptOffsetList = staTempData_B39_up;
        tTemp[RF_SYS_1][RF_CH_UP].listLen      = TEMP_LEVEL_B39_UP;
        tTemp[RF_SYS_1][RF_CH_DW].ptOffsetList = staTempData_B39_dw;
        tTemp[RF_SYS_1][RF_CH_DW].listLen      = TEMP_LEVEL_B39_DW;
    }
    else
    {
        tTemp[RF_SYS_1][RF_CH_UP].ptOffsetList = staTempData_B40_up;
        tTemp[RF_SYS_1][RF_CH_UP].listLen      = TEMP_LEVEL_B40_UP;
        tTemp[RF_SYS_1][RF_CH_DW].ptOffsetList = staTempData_B40_dw;
        tTemp[RF_SYS_1][RF_CH_DW].listLen      = TEMP_LEVEL_B40_DW;
    }

    tTemp[RF_SYS_2][RF_CH_UP].ptOffsetList = staTempData_5G_up;
    tTemp[RF_SYS_2][RF_CH_UP].listLen      = TEMP_LEVEL_5G_UP;
    tTemp[RF_SYS_2][RF_CH_DW].ptOffsetList = staTempData_5G_dw;
    tTemp[RF_SYS_2][RF_CH_DW].listLen      = TEMP_LEVEL_5G_DW;

    tTemp[RF_SYS_3][RF_CH_UP].ptOffsetList = staTempData_MIMO_up;
    tTemp[RF_SYS_3][RF_CH_UP].listLen      = TEMP_LEVEL_MIMO_UP;
    tTemp[RF_SYS_3][RF_CH_DW].ptOffsetList = staTempData_MIMO_dw;
    tTemp[RF_SYS_3][RF_CH_DW].listLen      = TEMP_LEVEL_MIMO_DW;
}

/*****************************************************************************
* @Name	  : get_tmp_en
* @Brief  : 获取使能参数
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2022-11-22
* @Note   :
*****************************************************************************/
void get_tmp_para(void)
{
    scTemperature = adc_read_temperature(tAdcTempValue.result.usAdcAve);
	//scTemperature = 55;
	tDevPublicPara.scTemp = scTemperature;
    for(uint8_t sys = 0; sys < RF_SYS_NUM; sys++)
    {
        tTemp[ sys ][ RF_CH_UP ].en = (ENABLE == t_rf_sys[ sys ].t_public.tcEn) ? ENABLE : DISABLE;
        tTemp[ sys ][ RF_CH_DW ].en = (ENABLE == t_rf_sys[ sys ].t_public.tcEn) ? ENABLE : DISABLE;
    }
}

/*****************************************************************************
* @Name	  : get_temp_value
* @Brief  : 获取温度值
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2023-11-02
* @Note   :
*****************************************************************************/
int8_t get_temp_value(void)
{
    return -128;
}


/*****************************************************************************
* @Name	  : ucTempValueSmooth
* @Brief  : 平滑温度数据, 消除切换点的跳变
* @Param  : tempValue: 当前获取的温度值
* @Retval : 是否更新温补数据: B_TRUE: 更新, B_FALSE: 不更新
* @Author :
* @Data   : 2022-11-22
* @Note   :
*****************************************************************************/
uint8_t ucTempValueSmooth(int8_t tempValue)
{
#define TEMP_ARRAY_LEN (6)

    static int8_t tempInUseValue = -128;
    static int8_t tempArray[ TEMP_ARRAY_LEN ] = {0};
    static uint8_t tempSaveAddr = 0;
    static uint8_t result = B_TRUE;
    int8_t tempMax, tempMin;
    uint8_t res = B_FALSE;

    tempSaveAddr++;
    if(tempSaveAddr >= TEMP_ARRAY_LEN)
        tempSaveAddr  = 0;

    tempArray[ tempSaveAddr ] = tempValue;

    tempMax = tempArray[0];
    tempMin = tempArray[0];

    result = B_TRUE;
    for(uint8_t i = 0; i <= TEMP_ARRAY_LEN - 2; i++) //判断数组中的数字是否均相等
    {
        if(tempArray[i] > tempMax)
            tempMax = tempArray[i];
        else if(tempArray[i] < tempMin)
            tempMin = tempArray[i];
        else
        {}

        if(tempArray[i] == tempArray[i + 1])
            continue;
        else
            result = B_FALSE;
    }

    //温度一致时, 用最新温度更新
    //温度不一致时, 若最大最小温差>3, 或者当前温差与正使用的温度差>3, 则更新
    if((result == B_TRUE) || (tempMax - tempMin >= 3)
        || (tempValue - tempInUseValue > 3)
        || (tempInUseValue - tempValue > 3))      //相等, 更新
    {
        res = B_TRUE;
//        tempInUseValue = tempValue;
//        memset( tempArray, tempValue, TEMP_ARRAY_LEN );
    }

    return res;
}



/*****************************************************************************
* @Name	  : find_offset_value_in_array
* @Brief  : 查表计算温度补偿和功率补偿
* @Param  : pstTemp:   结构体指针
*			tempValue: 当前温度值
* @Retval :
* @Author :
* @Data   : 2023-11-02
* @Note   :
*****************************************************************************/
void find_offset_value_in_array(stTempOffset_T * pstTemp,  int8_t tempValue)
{
#if TMP_COMPENSAVTION_EN

    if(ENABLE == pstTemp->en)
    {
        int8_t i = 0;

        pstTemp->stTempDataInUse.scTemperature = tempValue;

        for(i = 0; i < pstTemp->listLen; i++)
        {
            if(pstTemp->stTempDataInUse.scTemperature <= pstTemp->ptOffsetList[i].scTemperature)
                break;
            else
                continue;
        }

        pstTemp->stTempDataInUse.scGain  = pstTemp->ptOffsetList[i].scGain;
        pstTemp->stTempDataInUse.scPower = pstTemp->ptOffsetList[i].scPower;
    }
    else
    {
        pstTemp->stTempDataInUse.scGain  = 0;
        pstTemp->stTempDataInUse.scPower = 0;
    }
#else
    pstTemp->stTempDataInUse.scGain  = 0;
    pstTemp->stTempDataInUse.scPower = 0;
#endif
}


/*****************************************************************************
* @Name	  : ucTempCalOffset
* @Brief  : 计算温度补偿值
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2022-11-22
* @Note   :
*****************************************************************************/
static void ucTempCalOffset(int8_t tmpValue)
{
    uint8_t sys, eCh;

    for(sys = 0; sys < RF_SYS_NUM; sys++)
    {
        for(eCh = 0; eCh < RF_CH_NUM; eCh++)
            find_offset_value_in_array(&tTemp[sys][eCh], tmpValue);
    }
}


/*****************************************************************************
* @Name	  : tmp_in_task
* @Brief  : 温补处理流程
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2022-11-22
* @Note   : 在任务中调用
*****************************************************************************/
void tmp_in_task(void)
{
	tmp_tmp_data_sel();	//选择功率表
    get_tmp_para();		//获取设备参数
    if(ucTmpValueFreshSign == B_TRUE)
    {
        ucTmpValueFreshSign = B_FALSE;

        uint8_t res = ucTempValueSmooth(scTemperature);
        if(B_TRUE == res)
            ucTempCalOffset(scTemperature);
    }
}


/*****************************************************************************
* @Name	  : get_gain_tc_value
* @Brief  : 获取增益温补值
* @Param  : sys: [输入/出]
**			 ch: [输入/出]
* @Retval :
* @Author :
* @Data   : 2023-11-18
* @Note   :
*****************************************************************************/
int8_t get_gain_tc_value(uint8_t sys, uint8_t ch)
{
    return tTemp[sys][ch].stTempDataInUse.scGain;
}


/*****************************************************************************
* @Name	  : get_pow_tc_value
* @Brief  : 获取功率温补值
* @Param  : sys: [输入/出]
**			 ch: [输入/出]
* @Retval :
* @Author :
* @Data   : 2023-11-18
* @Note   :
*****************************************************************************/
int8_t get_pow_tc_value(uint8_t sys, uint8_t ch)
{
    return tTemp[sys][ch].stTempDataInUse.scPower;
}

/*****************************************************************************
* @Name	  : vGetTmpValue
* @Brief  : 获取温度值
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2022-11-22
* @Note   :
*****************************************************************************/
int8_t vGetTmpValue(void)
{
    return scTemperature;
}
