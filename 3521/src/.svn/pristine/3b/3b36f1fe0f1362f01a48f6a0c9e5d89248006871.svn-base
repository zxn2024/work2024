/**
 * **********************************************************************************
 * @copyright Copyright (c) 2023 Shaanxi Tianji Communication Technologies Co., Ltd.
 *
 * @file        hi_adc.c
 * @author      WangJun
 * @version     None
 * @date        2023-11-6
 * @brief       None
 *
 * **********************************************************************************
 */

#include "hi_adc.h"
#include <string.h>
#include "os_event.h"

/*---------------------- 端口定义 --------------------------------------*/


/*---------------------- 宏定义  ---------------------------------------*/


/*---------------------- 结构体 ----------------------------------------*/
//ADC数据采集结构体
typedef struct
{
    uint16_t * pAdcDmadata1;
    uint16_t * pAdcDmadata2;
    uint16_t * pAdcDmaConv;
    uint16_t * pAdcDmaProc;
} AdcHandle_T;

/*---------------------- 常量 ------------------------------------------*/


/*---------------------- 全局变量 --------------------------------------*/
#if(PRODUCT_MODEL == CMDT3521C)
__align(32) uint16_t adc0_converted_value[ADC0_CONVERTEN_TOTAL] __attribute__((section("ORIRAM")));    //ADC0 DMA存储BUF
__align(32) uint16_t adc2_converted_value[ADC2_CONVERTEN_TOTAL] __attribute__((section("ORIRAM")));    //ADC2 DMA存储BUF

__align(32) uint16_t adc_ch_data[ADC_CH_TOTAL_NUM][ADC2_CONVERTED_NUM] __attribute__((section("CCMRAM")));  // adc通道数据
#else
uint16_t adc0_converted_value[ADC0_CONVERTEN_TOTAL];    //ADC0 DMA存储BUF
uint16_t adc2_converted_value[ADC2_CONVERTEN_TOTAL];    //ADC2 DMA存储BUF

uint16_t adc_ch_data[ADC_CH_TOTAL_NUM][ADC2_CONVERTED_NUM];    // adc通道数据
#endif

/*---------------------- 局部变量 --------------------------------------*/
/**
  * @brief 数据处理指针结构体
  */
// AdcHandle_T tAdc0Data;
static AdcHandle_T tAdc2Data;

/**
  * MU adc 数据结构
  * @ tChAdcValue[0]:4g trx pwr
  * @ tChAdcValue[1]:nr trx pwr
  * @ tChAdcValue[2]:mimo trx pwr
  * @ tChAdcValue[3]:4g up
  * @ tChAdcValue[4]:4g dw
  * @ tChAdcValue[5]:nr up
  * @ tChAdcValue[6]:nr dw
  * @ tChAdcValue[7]:mimo up
  * @ tChAdcValue[8]:mimo dw
  */
AdcArray_T tChAdcValue[ADC_CH_TOTAL_NUM];   // 每个通道10组数值
AdcArray_T tAdcTempValue;                   // 温度数据
/*---------------------- 声明 ------------------------------------------*/


/*---------------------- 声明 ------------------------------------------*/

/*****************************************************************************
* @brief  : 拆分通道数据
* @param  : None
* @retval : None
* @date   : 2023-09-14
* @note   :
*****************************************************************************/
static void hi_adc_split_data(void)
{
    uint16_t i = 0, j = 0;

    for(i = 0, j = 0; i < ADC2_CONVERTED_NUM; i++, j++)
    {
        adc_ch_data[0][j] = adc2_converted_value[i * ADC2_CH_NUM];
        adc_ch_data[1][j] = adc2_converted_value[i * ADC2_CH_NUM + 1];
        adc_ch_data[2][j] = adc2_converted_value[i * ADC2_CH_NUM + 2];
        adc_ch_data[3][j] = adc2_converted_value[i * ADC2_CH_NUM + 3];
        adc_ch_data[4][j] = adc2_converted_value[i * ADC2_CH_NUM + 4];
        adc_ch_data[5][j] = adc2_converted_value[i * ADC2_CH_NUM + 5];
        adc_ch_data[6][j] = adc2_converted_value[i * ADC2_CH_NUM + 6];
        adc_ch_data[7][j] = adc2_converted_value[i * ADC2_CH_NUM + 7];
        adc_ch_data[8][j] = adc2_converted_value[i * ADC2_CH_NUM + 8];
    }
}


/*****************************************************************************
* @brief  : 查找最大最小值
* @param  : pmax: 最大值
*           pmin: 最小值
*           pbuf: 指针
*           len: 长度
* @retval : None
* @date   : 2023-09-11
* @note   :
*****************************************************************************/
static void hi_adc_find_maxminval(uint16_t * pmax, uint16_t * pmin, uint16_t * pbuf, uint16_t len)
{
    uint16_t index = 0;
    uint16_t maxrefval = *pbuf; // 最大值
    uint16_t minrefval = *pbuf; // 最小值

    for(index = 1; index < len; index++)
    {
        if(minrefval > *(pbuf + index))        // 查找最小值
        {
            minrefval = *(pbuf + index);
        }

        if(maxrefval < * (pbuf + index))      // 查找最大值
        {
            maxrefval = *(pbuf + index);
        }
    }
    *pmax = maxrefval;   // 最大值
    *pmin = minrefval;   // 最小值
}


/*****************************************************************************
* @brief  : 计算均值
* @param  : maxvalue:最大值
*			pbuf: 指针
*			len: 长度
* @retval :
* @date   : 2023-09-11
* @note   :
*****************************************************************************/
static uint16_t hi_adc_cal_avevalue(uint16_t maxvalue, uint16_t * pbuf, uint16_t len)
{
    uint16_t index = 0;
    uint16_t refval = 0;    // 参考值

    uint16_t effrctive_cnt = 0;         // 有效值的计数
    uint32_t sum = 0;                   // 有效值的和

    refval = (uint16_t)(maxvalue * 0.8);    // 参考值（最大值的80%）

    // 求和
    for(index = 0; index < len; index++)
    {
        if(refval < * (pbuf + index))
        {
            sum += *(pbuf + index);     // 有效值求和
            effrctive_cnt++;            // 有效值计数
        }
    }

    if(effrctive_cnt)
        return (uint16_t)(sum / effrctive_cnt);   // 有效值求平均
    else
        return 0;
}


/*****************************************************************************
* @brief  : 计算每组数据
* @param  : [输入/出]
* @retval :
* @date   : 2023-09-14
* @note   :
*****************************************************************************/
static void hi_adc_cal_data(uint16_t * src, uint16_t len, AdcArray_T * tpst)
{
    uint8_t i = 0;
    uint16_t min = 0, max = 0;
    
    #if(1 == ENABLE_AVE_GROUPS)
    uint16_t ave_cnt = 0;
    uint32_t ave_sum = 0;
    #elif(2 == ENABLE_AVE_GROUPS)
    uint32_t ave_sum = 0;
    #endif
    
    if(tpst->cnt >= ADC_CH_ARRAY_NUM)
    {
        tpst->cnt = 0;
    }
        
    hi_adc_find_maxminval(&max, &min, src, len);    // 取最大最小值
    tpst->AdcArray[tpst->cnt] = hi_adc_cal_avevalue(max, src, len); // 取均值
    
    tpst->cnt++;    // 计算++
        
#if(0 == ENABLE_AVE_UPDATA)
    if(ADC_CH_ARRAY_NUM == tpst->cnt)   // 10组数据都更新后再计数平均
    {
#endif
        #if(0 == ENABLE_AVE_GROUPS)
        /* 10组均值取最大 */
        tpst->result.usAdcMax = tpst->AdcArray[0];
        tpst->result.usAdcMin = tpst->AdcArray[0];
        tpst->result.usAdcAve = tpst->AdcArray[0];
        
        for(i = 1; i < ADC_CH_ARRAY_NUM; i++)
        {
            if(tpst->AdcArray[i] >= tpst->result.usAdcMax)    // max
                tpst->result.usAdcMax = tpst->AdcArray[i];
            if(tpst->AdcArray[i] <= tpst->result.usAdcMin)    // min
                tpst->result.usAdcMin = tpst->AdcArray[i];
            if(tpst->AdcArray[i] >= tpst->result.usAdcAve)    // ave
                tpst->result.usAdcAve = tpst->AdcArray[i];
        }
        
        #elif(1 == ENABLE_AVE_GROUPS)
        /* 10组均值取大于最大的0.8倍再求平均 */
        tpst->result.usAdcMax = tpst->AdcArray[0];
        tpst->result.usAdcMin = tpst->AdcArray[0];
        
        for(i = 1; i < ADC_CH_ARRAY_NUM; i++)
        {
            if(tpst->AdcArray[i] >= tpst->result.usAdcMax)    // max
                tpst->result.usAdcMax = tpst->AdcArray[i];
            if(tpst->AdcArray[i] <= tpst->result.usAdcMin)    // min
                tpst->result.usAdcMin = tpst->AdcArray[i];
        }
        
        for(i = 0; i < ADC_CH_ARRAY_NUM; i++)
        {
            if(tpst->AdcArray[i] >= (tpst->result.usAdcMax * 0.8))
            {
                ave_sum += tpst->AdcArray[i];
                ave_cnt++;
            }
        }
        tpst->result.usAdcAve = (uint16_t)(ave_sum / ave_cnt);   // ave
        #elif(2 == ENABLE_AVE_GROUPS)
        tpst->result.usAdcMax = tpst->AdcArray[0];
        tpst->result.usAdcMin = tpst->AdcArray[0];
        
        for(i = 1; i < ADC_CH_ARRAY_NUM; i++)
        {
            if(tpst->AdcArray[i] >= tpst->result.usAdcMax)    // max
                tpst->result.usAdcMax = tpst->AdcArray[i];
            if(tpst->AdcArray[i] <= tpst->result.usAdcMin)    // min
                tpst->result.usAdcMin = tpst->AdcArray[i];
        }
        
        /* 10组均值求平均 */
        for(i = 0; i < ADC_CH_ARRAY_NUM; i++)
        {
            ave_sum += tpst->AdcArray[i];
        }
        tpst->result.usAdcAve = (uint16_t)(ave_sum / ADC_CH_ARRAY_NUM);  // ave
        #endif
        xEventGroupSetBits(xAdcEgp, BIT_ADC2_DATA_VALID);   // adc2数据有效
#if(0 == ENABLE_AVE_UPDATA)
    }
#endif
    
}


/*****************************************************************************
* @Name	  : hiAdcInit
* @Brief  : adc初始化函数
* @Retval :
* @Author :
* @Data   :
* @Note   :
*****************************************************************************/
void hi_adc_init(void)
{
#if(PRODUCT_MODEL == CMDT3521A)
    adc0_init(adc0_converted_value, (ADC0_CONVERTED_NUM * ADC0_CH_NUM));
    adc2_init(adc2_converted_value, (ADC2_CONVERTED_NUM * ADC2_CH_NUM));
#elif(PRODUCT_MODEL == CMDT3521B)
    adc0_init(adc0_converted_value, (ADC0_CONVERTED_NUM * ADC0_CH_NUM));
    adc2_init(adc2_converted_value, (ADC2_CONVERTED_NUM * ADC2_CH_NUM));
#elif(PRODUCT_MODEL == CMDT3521C)
    MX_ADC1_Init();
    MX_ADC3_Init();
#elif(PRODUCT_MODEL == CMDT3521D)

#elif(PRODUCT_MODEL == CMDT3521E)

#elif(PRODUCT_MODEL == CMDT3521F)

#endif

    uint8_t i = 0;
    for(i = 0; i < (ADC_CH_TOTAL_NUM); i++)
    {
        memset(&tChAdcValue[i], 0, sizeof(AdcArray_T));
    }

    // tAdc0Data.pAdcDmadata1 = adc0_converted_value;   // ADC buf 前半段
    // tAdc0Data.pAdcDmadata2 = (uint16_t *)adc0_converted_value + (ADC0_CONVERTEN_TOTAL / 2);  // ADC buf 后半段
    // tAdc0Data.pAdcDmaConv = tAdc0Data.pAdcDmadata1;   //adc转换缓冲区指针,指向下一个ADC DMA转换
    // tAdc0Data.pAdcDmaProc = NULL;    //adc数据处理缓冲区指针,指向已填满待处理的ADC数据

    tAdc2Data.pAdcDmadata1 = adc2_converted_value;   // ADC buf 前半段
    tAdc2Data.pAdcDmadata2 = (uint16_t *)adc2_converted_value + (ADC2_CONVERTEN_TOTAL / 2);  // ADC buf 后半段
    tAdc2Data.pAdcDmaConv = tAdc2Data.pAdcDmadata1;   //adc转换缓冲区指针,指向下一个ADC DMA转换
    tAdc2Data.pAdcDmaProc = NULL;    //adc数据处理缓冲区指针,指向已填满待处理的ADC数据
}


/*****************************************************************************
* @Name	  : hiAdcGetDMAData
* @brief  : adc DMA读取数据
* @param  : ch ：端口号,见上方端口定义
* @param  : pBuf ：取数据缓冲区
* @param  : len ：取数据长度
* @Retval  : Status_t
* @author :
* @Data   :
* @Note   : 默认使用循环DMA，双缓冲模式
*****************************************************************************/
Status_t hi_adc_get_DMA_data(ADC_CH_T ch, uint16_t * ptr_buf, uint16_t len)
{
    return STAT_OK;
}


/*****************************************************************************
* @brief  : adc读取温度
* @param  : data: adc温度数据
* @retval : 当前温度值（整数）
* @date   : 2024-01-10
* @note   : 
*****************************************************************************/
int8_t adc_read_temperature(uint16_t data)
{
    #if(MCU_SERIES == MCU_GD32F4XX)
    // 温度 (°C) = {(V25 – Vtemperature) / Avg_Slope} + 25
    return (int8_t)((1.4f - data * 3.3f / 4096) * 1000 / 4.4f + 25);
    #elif(MCU_SERIES == MCU_STM32F4XX)
    // 温度 (°C) = {(VSENSE — V25) / Avg_Slope} + 25
    return (int8_t)((data * 3.3f / 4096 - 0.76f) * 1000 / 2.5f + 25);
    #endif
}

/*****************************************************************************
* @brief  : 获取ADC数据
* @param  : None
* @retval : None
* @date   : 2023-09-18
* @note   :
*****************************************************************************/
void hi_adc_get_value(void)
{
    if(BIT_ADC2_DMA_COMPLATE == ((xEventGroupGetBits(xAdcEgp) & BIT_ADC2_DMA_COMPLATE)))
    {
        xEventGroupClearBits(xAdcEgp, BIT_ADC2_DMA_COMPLATE);
        
        uint8_t i = 0;
        uint32_t temp_sum = 0;
        
        hi_adc_split_data();    /* 拆分各通道数据 */

        for(i = 0; i < (ADC_CH_TOTAL_NUM); i++)
        {
            hi_adc_cal_data(&adc_ch_data[i][0], ADC2_CONVERTED_NUM, &tChAdcValue[i]);
        }
        
        for(i = 0; i < (ADC0_CONVERTED_NUM); i++)
        {
            temp_sum += adc0_converted_value[i];
        }
        tAdcTempValue.result.usAdcAve = (uint16_t)(temp_sum / ADC0_CONVERTED_NUM);
    }
}

#if(MCU_SERIES == MCU_GD32F4XX)
/**
  * @brief  Conversion complete callback in non-blocking mode.
  * @param hadc ADC handle
  * @retval None
  */
void ADC_ConvFullCpltCallback(void)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    tAdc2Data.pAdcDmaConv = tAdc2Data.pAdcDmadata1;
    tAdc2Data.pAdcDmaProc = tAdc2Data.pAdcDmadata2;

    xEventGroupSetBitsFromISR(xAdcEgp, BIT_ADC2_DMA_COMPLATE, &xHigherPriorityTaskWoken);
}


/**
  * @brief  Conversion half callback in non-blocking mode.
  * @param hadc ADC handle
  * @retval None
  */
void ADC_ConvHalfCpltCallback(void)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    tAdc2Data.pAdcDmaConv = tAdc2Data.pAdcDmadata2;
    tAdc2Data.pAdcDmaProc = tAdc2Data.pAdcDmadata1;

    xEventGroupSetBitsFromISR(xAdcEgp, BIT_ADC2_DMA_COMPLATE, &xHigherPriorityTaskWoken);
}

#elif(MCU_SERIES == MCU_STM32F4XX || MCU_SERIES == MCU_STM32H5XX)

/**
  * @brief  Conversion complete callback in non-blocking mode.
  * @param  hadc ADC handle
  * @retval None
  */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    if(hadc == &hadc3)
    {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        
        tAdc2Data.pAdcDmaConv = tAdc2Data.pAdcDmadata1;
        tAdc2Data.pAdcDmaProc = tAdc2Data.pAdcDmadata2;

        xEventGroupSetBitsFromISR(xAdcEgp, BIT_ADC2_DMA_COMPLATE, &xHigherPriorityTaskWoken);
    }
}

/**
  * @brief  Conversion half callback in non-blocking mode.
  * @param  hadc ADC handle
  * @retval None
  */
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc)
{
    if(hadc == &hadc3)
    {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;

        tAdc2Data.pAdcDmaConv = tAdc2Data.pAdcDmadata2;
        tAdc2Data.pAdcDmaProc = tAdc2Data.pAdcDmadata1;

        xEventGroupSetBitsFromISR(xAdcEgp, BIT_ADC2_DMA_COMPLATE, &xHigherPriorityTaskWoken);
    }
}

/*****************************************************************************
* @brief  : 开启DMA传输
* @param  : 
* @retval : 
* @date   : 2024-01-18
* @note   : 
*****************************************************************************/
void adc_start_dma(void)
{
    HAL_NVIC_DisableIRQ(DMA2_Stream4_IRQn);//关闭温度采集adcdma中断

    HAL_ADC_Start(&hadc1);
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc0_converted_value, ADC0_CONVERTEN_TOTAL);
    HAL_ADC_Start(&hadc3);
    HAL_ADC_Start_DMA(&hadc3, (uint32_t *)adc2_converted_value, ADC2_CONVERTEN_TOTAL);
}

#endif
