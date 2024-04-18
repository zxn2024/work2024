/***************************** Copyright ****************************************
*
* (C) Copyright 2022, Shaanxi Tianji Communication
*  All Rights Reserved
*
* FileName   : bsp_pow.c
* Version    : none
* Author     : ZhangWei
* Date       : 2023-10-12
* Description: none
*******************************************************************************/
#include "main.h"
#include "system_config.h"
#include "bsp_rf_pow.h"

#include "bsp_rf_global.h"
#include "hi_adc.h"

//#include "bsp_rf_ctr.h"
//#include "bsp_adc.h"
//#include "bsp_dev_para.h"

/*---------------------- 端口定义 --------------------------------------*/


/*---------------------- 宏定义  ---------------------------------------*/


/*---------------------- 结构体 ----------------------------------------*/
//typedef struct
//{
//    AdcValue_T        * ptAdc;     //ADC采样值, 指向adc数据采样保存位置
//    POW_TABLE_PARA_T  * ptList;    //功率表
//    PowValue_T          pow;
//} AdcToPow_T;



/*---------------------- 常量 ------------------------------------------*/


/*---------------------- 全局变量 --------------------------------------*/
POW_TABLE_PARA_T t_pow_table[RF_SYS_NUM][RF_CH_NUM];    //保存从FLASH中载入的功率表数据
AdcToPow_T 		 t_ch_pow[RF_SYS_NUM][RF_CH_NUM];       //各系统通道对应的功率值
AdcValue_T 		 *t_ch_adc[RF_SYS_NUM][RF_CH_NUM];      //各系统通道对应的检波电压

POW_TABLE_PARA_T t_pow_table_8668[RF_SYS_NUM];          //用于保存8668后检波的ADC值, 主机的下行 / 从机的上行
AdcToPow_T 		 t_ch_pow_8668[RF_SYS_NUM];             // 8668功率值
AdcValue_T 		 *t_ch_adc_8668[RF_SYS_NUM];            // 8668检波值



/*---------------------- 局部变量 --------------------------------------*/


/*---------------------- 声明 ------------------------------------------*/



/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ HAL ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/





/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DRV ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void pow_table_data_init( void )
{
	POW_TABLE_PARA_T t_pow_table_init;
	
	memset( &t_pow_table_init, 0xFF, sizeof( POW_TABLE_PARA_T ) );
	
	t_pow_table_init.start_pow  = -10;
	t_pow_table_init.cali_value = 0;
	t_pow_table_init.step       = 1;
	t_pow_table_init.valid_num  = 50;
	
	t_pow_table_init.table[0] = 0;
	
	for( uint8_t i=1; i<=t_pow_table_init.valid_num-1; i++ )
	{
		t_pow_table_init.table[i] = t_pow_table_init.table[i-1] + 0x10;
	}
	
	for( uint8_t sys = 0; sys<RF_SYS_NUM; sys++ )
	{
		memcpy( &t_pow_table_8668[sys], &t_pow_table_init, sizeof( POW_TABLE_PARA_T ));
		
		for( uint8_t ch = 0; ch<RF_CH_NUM; ch++ )
			memcpy( &t_pow_table[sys][ch], &t_pow_table_init, sizeof( POW_TABLE_PARA_T ));
	}
}


/*****************************************************************************
* @Name	  : adcToPow
* @brief  : adc数据查表转化为功率数据
* @param  : pt: 功率表结构体指针
* @retval :
* @author :
* @Data   : 2023-10-12
*****************************************************************************/
static int8_t scAdcToPow(uint16_t adc_value, AdcToPow_T * pt)
{
    uint8_t i = 0;
    uint16_t mid_value = 0;

    int8_t res = -128;

    while((adc_value > pt->ptList->table[i]) && (i <= pt->ptList->valid_num))
        i++;

    if((i != 0) && (pt->ptList->table[i] != 4095))	//四舍五入取最接近的功率值
    {
        mid_value = ( pt->ptList->table[i-1] + pt->ptList->table[i] ) / 2;
        if(pt->ptAdc->usAdcAve <= mid_value)
            i -= 1;
    }

    res = i + pt->ptList->start_pow
        + pt->ptList->cali_value;

    return(res);
}
/*****************************************************************************
* @Name	  : adcToPow
* @brief  : adc数据查表转化为功率数据
* @param  : pt: 功率表结构体指针
* @retval :
* @author :
* @Data   : 2023-10-12
*****************************************************************************/
static float scAdcToPowFloat(uint16_t adc_value, AdcToPow_T * pt)
{
    int i = 0;
    uint16_t upper = 0, lower = 0;
    double a = 0.0, b = 0.0, f_pwr = 0.0;
    float res = 0.0;

    if(pt == NULL)
        return 0.0;
    
    for(i = 0; i < pt->ptList->valid_num - 1; i++)
    {
        if(i == 0 && adc_value <= pt->ptList->table[0])
        {
            i = -1;
            break;
        }
        else if(adc_value > pt->ptList->table[i+1])
            continue;
        else
            break;
    }

    if((i != -1) && (pt->ptList->table[i] != 4095) && (i <= pt->ptList->valid_num - 1))	
    {
        lower = pt->ptList->table[i];
        upper = pt->ptList->table[i + 1];
        a = (double)1 / (double)(upper - lower);
        b = (double)i - (double)a * lower;
        f_pwr = (double)(a * adc_value + b);
    }else if(i == pt->ptList->valid_num - 1) //上限
        f_pwr = (double)i;
    else if(i == -1) //下限
        f_pwr = 0;

    res = (float)(f_pwr + (double)pt->ptList->start_pow
        + (double)pt->ptList->cali_value);

    return(res);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ APP ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*****************************************************************************
* @Name	  : adc_to_pow_struct_Init
* @brief  : 结构体初始化
* @param  : pt_src:         指定通道待初始化的功率结构体
**          pt_adc:         对应通道的adc采集结构体指针
**          pt_list:        对应通道的功率表数据指针
**          tab_num:        功率表中元素的个数
**          tab_min_value:  功率表最小数据对应的功率值
* @retval :
* @author :
* @Data   : 2023-10-30
*****************************************************************************/
static void adc_to_pow_struct_Init(AdcToPow_T * pt_src, AdcValue_T * pt_adc, POW_TABLE_PARA_T * pt_table)
{
    pt_src->ptAdc                = pt_adc;
    pt_src->ptList       		 = pt_table;
//    pt_src->ptList->ucTabNum     = pt_table->valid_num;
//    pt_src->ptList->scMinValue   = pt_table->start_pow;
//    pt_src->ptList->scCaliOffset = pt_table->cali_value;
//    pt_src->ptList->scTempOffset = 0;
    pt_src->pow.ave              = -128;
    pt_src->pow.min              = -128;
    pt_src->pow.max              = -128;
}


/*****************************************************************************
* @Name   : adc_to_pow_init
* @brief  : 结构体初始化
* @param  :
* @retval :
* @author :
* @Data   : 2023-10-15
*****************************************************************************/
void pow_init(void)
{
    t_ch_adc[RF_SYS_1][RF_CH_UP] = &tChAdcValue[3].result;	//4g up
    t_ch_adc[RF_SYS_1][RF_CH_DW] = &tChAdcValue[4].result;	//4g dw
    t_ch_adc[RF_SYS_2][RF_CH_UP] = &tChAdcValue[5].result;	//nr up
    t_ch_adc[RF_SYS_2][RF_CH_DW] = &tChAdcValue[6].result;	//nr dw
    t_ch_adc[RF_SYS_3][RF_CH_UP] = &tChAdcValue[7].result;	//mino up
    t_ch_adc[RF_SYS_3][RF_CH_DW] = &tChAdcValue[8].result;	//mimo dw

    t_ch_adc_8668[RF_SYS_1]		 = &tChAdcValue[0].result;	//4g trx pwr
    t_ch_adc_8668[RF_SYS_2]		 = &tChAdcValue[1].result;	//nr trx pwr
    t_ch_adc_8668[RF_SYS_3]		 = &tChAdcValue[2].result;	//mimo trx pwr

    for(uint8_t sys = 0; sys < RF_SYS_NUM; sys++)
    {
        for(uint8_t ch = 0; ch < RF_CH_NUM; ch++)
        {
            adc_to_pow_struct_Init(&t_ch_pow[sys][ch], t_ch_adc[sys][ch], &t_pow_table[sys][ch]);
        }

        adc_to_pow_struct_Init(&t_ch_pow_8668[sys], t_ch_adc_8668[sys], &t_pow_table_8668[sys]);
    }
}


/*****************************************************************************
* @Name	  : set_pow_cali_offset
* @brief  : 设置功率校准参数
* @param  : sys:        系统编号
**          ch:         通道编号, RF_CH_UP / RF_CH_DW
**          caloffset:  校准值
* @retval :
* @author :
* @Data   : 2023-10-15
*****************************************************************************/
void set_pow_cali_offset(uint8_t sys, uint8_t ch, int8_t calOffset)
{
    if((sys >= RF_SYS_NUM) || (ch >= RF_CH_NUM) || (calOffset >= 5) || (calOffset <= -5))
        return;

    AdcToPow_T * pt = &t_ch_pow[sys][ch];

    pt->ptList->cali_value = calOffset;
}


/*****************************************************************************
* @Name	  : set_pow_temp_offset
* @param  : sys:        系统编号
**			ch:         通道编号, RF_CH_UP / RF_CH_DW
**          tempOffset:  校准值
* @retval :
* @author :
* @Data   : 2023-10-15
*****************************************************************************/
void set_pow_temp_offset(uint8_t sys, uint8_t ch, int8_t tempOffset)
{
//    if((sys >= RF_SYS_NUM) || (ch >= RF_CH_NUM) || (tempOffset >= 10) || (tempOffset <= -10))
//        return;

//    AdcToPow_T * pt = &t_ch_pow[sys][ch];

//    pt->ptList-> = tempOffset;
}

/*****************************************************************************
* @Name	  : set_pow_cali_offset_8668
* @brief  : 设置功率校准参数
* @param  : sys:        系统编号
**			ch:         通道编号, RF_CH_UP / RF_CH_DW
**          caloffset:  校准值
* @retval :
* @author :
* @Data   : 2023-10-15
*****************************************************************************/
void set_pow_cali_offset_8668(uint8_t sys, int8_t calOffset)
{
    if((sys >= RF_SYS_NUM) || (calOffset >= 5) || (calOffset <= -5))
        return;

    AdcToPow_T * pt = &t_ch_pow_8668[sys];

    pt->ptList->cali_value = calOffset;
}


/*****************************************************************************
* @Name	  : set_pow_temp_offset_8668
* @param  : sys:        系统编号
**			ch:         通道编号, RF_CH_UP / RF_CH_DW
**          tempOffset:  校准值
* @retval :
* @author :
* @Data   : 2023-10-15
*****************************************************************************/
void set_pow_temp_offset_8668(uint8_t sys, int8_t tempOffset)
{
//    if((sys >= RF_SYS_NUM) || (tempOffset >= 10) || (tempOffset <= -10))
//        return;

//    AdcToPow_T * pt = &t_ch_pow_8668[sys];

//    pt->ptList->scTempOffset = tempOffset;
}


/*****************************************************************************
* @Name	  : pow_convert
* @brief  : 功率转换
* @param  : None
* @retval :
* @author :
* @Data   : 2023-10-15
*****************************************************************************/
#define FILTER_WEIGHT (0.5f)  //权值越大滤波越平滑,但是实时性变差
#define FILTER_COMPLEMENT_WEIGHT (1-FILTER_WEIGHT)
void pow_convert(void)
{
    AdcToPow_T * pt = NULL;
//   float f_pwr_temp = 0.0;
//   float pre_val = 0.0;

    for(uint8_t sys = 0; sys < RF_SYS_NUM; sys++)
    {

        memset(&t_ch_pow[sys][RF_CH_UP].pow, -128, sizeof(PowValue_T));
        memset(&t_ch_pow[sys][RF_CH_DW].pow, -128, sizeof(PowValue_T));
        //memset(&t_ch_pow_8668[sys].pow,      -128, sizeof(PowValue_T));

        for(uint8_t ch = 0; ch < RF_CH_NUM; ch++)
        {
            pt = &t_ch_pow[sys][ch];
            pt->pow.ave = scAdcToPow(pt->ptAdc->usAdcAve, pt);
            pt->pow.max = scAdcToPow(pt->ptAdc->usAdcMax, pt);
            pt->pow.min = scAdcToPow(pt->ptAdc->usAdcMin, pt);
            pt->pow.ave_precision = scAdcToPowFloat(pt->ptAdc->usAdcAve, pt);
        }

        pt = &t_ch_pow_8668[sys];
        pt->pow.ave = scAdcToPow(pt->ptAdc->usAdcAve, pt);
        pt->pow.max = scAdcToPow(pt->ptAdc->usAdcMax, pt);
        pt->pow.min = scAdcToPow(pt->ptAdc->usAdcMin, pt);
        pt->pow.ave_precision = scAdcToPowFloat(pt->ptAdc->usAdcAve, pt);
//        f_pwr_temp = (float)scAdcToPowFloat(pt->ptAdc->usAdcAve, pt) * (float)FILTER_WEIGHT;
//        pre_val = (float)pt->pow.ave_precision * (float)FILTER_WEIGHT;
//        pt->pow.ave_precision = f_pwr_temp + pre_val;

    }
}


/*****************************************************************************
* @Name	  : get_ch_pow
* @Brief  : 获取指定通道的输出功率值
* @Param  : sys: [输入/出]
**			 ch: [输入/出]
* @Retval :
* @Author :
* @Data   : 2023-11-01
* @Note   :
*****************************************************************************/
int8_t get_ch_pow_for_alc(uint8_t sys, uint8_t ch)
{
    if((sys >= RF_SYS_NUM) || (ch >= RF_CH_NUM))
        return -128;

    return t_ch_pow[ sys ][ ch ].pow.ave;
}
float get_float_ch_pow_for_alc(uint8_t sys, uint8_t ch)
{
    if((sys >= RF_SYS_NUM) || (ch >= RF_CH_NUM))
        return -128;

    return t_ch_pow[ sys ][ ch ].pow.ave_precision;
}


/*****************************************************************************
* @Name	  : get_ch_pow_for_8668
* @Brief  : 获取指定通道的输出功率值
* @Param  : sys: [输入/出]
**			 ch: [输入/出]
* @Retval :
* @Author :
* @Data   : 2023-11-09
* @Note   : 仅用于获取8668后ADC功率值
*****************************************************************************/
int8_t get_ch_pow_for_8668(uint8_t sys, uint8_t ch)
{
#if( LOCAL_MODULE_TYPE == _MU )
    if((sys >= RF_SYS_NUM) || (ch != RF_CH_DW))
        return -128;
#else
    if((sys >= RF_SYS_NUM) || (ch != RF_CH_UP))
        return -128;
#endif

    return t_ch_pow_8668[ sys ].pow.ave;
}
float get_float_ch_pow_for_8668(uint8_t sys, uint8_t ch)
{
    #if( LOCAL_MODULE_TYPE == _MU )
    if((sys >= RF_SYS_NUM) || (ch != RF_CH_DW))
        return -128;
    #else
    if((sys >= RF_SYS_NUM) || (ch != RF_CH_UP))
        return -128;
    #endif

    return t_ch_pow_8668[ sys ].pow.ave_precision;
}


/*****************************************************************************
* @Name	  : get_ch_pow_table_for_alc_addr
* @Brief  : 获取用于ALC的功率表的结构体地址, 用于功率表数据载入
* @Param  : sys: [输入/出]
**			 ch: [输入/出]
* @Retval :
* @Author :
* @Data   : 2023-11-15
* @Note   :
*****************************************************************************/
POW_TABLE_PARA_T * get_ch_pow_table_for_alc_addr(uint8_t sys, uint8_t ch)
{
    if((sys >= RF_SYS_NUM) || (ch >= RF_CH_NUM))
        return NULL;

    return &t_pow_table[sys][ch];
}


/*****************************************************************************
* @Name	  : get_ch_pow_table_for_8668_addr
* @Brief  : 获取用于8668后的功率表的结构体地址, 用于功率表数据载入
* @Param  : sys: [输入/出]
* @Retval :
* @Author :
* @Data   : 2023-11-15
* @Note   :
*****************************************************************************/
POW_TABLE_PARA_T * get_ch_pow_table_for_8668_addr(uint8_t sys)
{
    if(sys >= RF_SYS_NUM)
        return NULL;

    return &t_pow_table_8668[sys];
}


/*****************************************************************************
* @Name	  : get_ch_adc_value
* @Brief  : 获取各通道ADC采样值
* @Param  : sys: [输入/出]
**			ch: [输入/出]
**			type: ADC_VALUE_TYPE_ALC / ADC_VALUE_TYPE_8668
* @Retval :
* @Author :
* @Data   : 2023-12-21
* @Note   :
*****************************************************************************/
uint16_t get_ch_adc_value(uint8_t sys, uint8_t ch, uint8_t type)
{
    if(ADC_VALUE_TYPE_ALC == type)
    {
        return t_ch_adc[sys][ch]->usAdcAve;
    }
    else if(ADC_VALUE_TYPE_8668 == type)
    {
        return t_ch_adc_8668[sys]->usAdcAve;
    }
    else
    {
        return 0x0000;
    }
}
