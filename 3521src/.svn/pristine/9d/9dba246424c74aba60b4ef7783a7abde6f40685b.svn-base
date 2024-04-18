#include "ecr8668_driver.h"
#include "hi_systick.h"
#include "hi_irq.h"
#include "FreeRTOS.h"
#include "task.h"
#include "MCU_config.h"
#include "main.h"

/*---------------------- 函数 ----------------------*/
/*
 *  866x 注册函数
 *  handle_8668：8668设备结构体
 */
Status_t trx8668_fpga_register(TRX8668_FPGA_t * handle_8668)
{
    // 参数检查
    if(handle_8668 == NULL)
        return STAT_ERROR;
    handle_8668->lock = B_FALSE;
    handle_8668->ch = 0; // 天基的8668只有ch0
    handle_8668->band = BAND_END;
    handle_8668->bin_addr = 0;
    handle_8668->bin_siz = 0;
    memset(&handle_8668->ch_status, 0, sizeof(handle_8668->ch_status));
    handle_8668->adc_overload_trigger = B_FALSE;
    handle_8668->adc_antiblock_lopwr_cnt = 0;
    handle_8668->adc_overload_flag = B_FALSE;
    handle_8668->tia_overload_trigger = B_FALSE;
    handle_8668->tia_antiblock_lopwr_cnt = 0;
    handle_8668->tia_overload_flag = B_FALSE;
    
    handle_8668->anti_block_stat = B_FALSE;
    handle_8668->anti_block_switch = OPEN;
    handle_8668->silent_mode = B_FALSE;
    handle_8668->error_flag = B_FALSE;
    handle_8668->error_cnt = 0;
    handle_8668->adc_antiblock_cnt = 0;
    handle_8668->tia_antiblock_cnt = 0;
    handle_8668->anti_block_cnt = 0;
    handle_8668->anti_block_tick = 0;
    handle_8668->cali_refersh_tick = 0;

    memset(&handle_8668->factroy_data, 0, sizeof(handle_8668->factroy_data));
    handle_8668->gain_cali_lookup_table = NULL;
    handle_8668->gain_tab = &gain_tab_b3; //初始化为默认码表,防止未初始化时使用导致指针错误

    return STAT_OK;
}

/**********************************************************
 *函数名：   BSP_866x_lock
 *函数功能： 锁定8668芯片,使其不能被其他程序使用,如果已被其他任务锁定立刻返回失败
 *返回值：   STAT_ERROR STAT_OK
 */
Status_t BSP_866x_try_lock(TRX8668_FPGA_t * h8668)
{
    if (h8668->lock == B_TRUE)
    {
        return STAT_ERROR;
    }else
    {
        h8668->lock = B_TRUE;
        return STAT_OK;
    }    
}

/**********************************************************
 *函数名：   BSP_866x_lock
 *函数功能： 锁定8668芯片,使其不能被其他程序使用,超时则返回失败
 *返回值：   STAT_ERROR STAT_OK
 */
Status_t BSP_866x_wait_lock(TRX8668_FPGA_t * h8668, uint32_t timeout_ms)
{
    uint32_t time_cnt = 0;

    while(h8668->lock != B_FALSE)
    {
        vTaskDelay(10);
        time_cnt += 10;
        if(time_cnt >= timeout_ms)
        {
            break;
        }
    }

    if (h8668->lock == B_FALSE)
    {
        h8668->lock = B_TRUE;
        return STAT_OK;
    }else
    {
        return STAT_ERROR;
    }
}
/**********************************************************
 *函数名：   BSP_866x_unlock
 *函数功能： 解锁8668芯片,使其可以被其他程序使用
 *返回值：   无
 */
void BSP_866x_unlock(TRX8668_FPGA_t * h8668)
{
    h8668->lock = B_FALSE;
}

/*
 *  866x 关闭通道TX RX
 *  pcs：片选
 */
void BSP_866xCloseTxRx(TRX8668_FPGA_t * h8668)
{
    Ecr866xSetChClose(h8668, CLOSE_TXRX_FLAG);
}

/*
 *  866x 关闭通道TX
 *  pcs：片选
 */
void BSP_866xCloseTx(TRX8668_FPGA_t * h8668)
{
    Ecr866xSetChClose(h8668, OPEN_RX_CLOSE_TX_FLAG);
}

/*
 *  866x 关闭通道RX
 *  pcs：片选
 */
void BSP_866xCloseRx(TRX8668_FPGA_t * h8668)
{
    Ecr866xSetChClose(h8668, OPEN_TX_CLOSE_RX_FLAG);
}

/*
 *  866x 打开通道RX tx
 *  pcs：片选
 */
void BSP_866xOpenTxRx(TRX8668_FPGA_t * h8668)
{
    Ecr866xSetChClose(h8668, OPEN_TXRX_FLAG);
}

/**********************************************************
 *函数名：   BSP_8668ChModeSet
 *函数功能： 配置通道模式
 *返回值：   无
 */
void BSP_8668ChModeSet(TRX8668_FPGA_t * h8668, uint32_t mode)
{
    vTaskDelay(1000);             // 配置通道前增加延时,防止通道配置失败
    Ecr866xChModeCfg(h8668, mode); // 通道模式配置

    Ecr866xBwChoose(h8668, h8668->band, h8668->ch_status.bw_choose); // 带宽配置

    Ecr866xRxFreqCfg(h8668, h8668->ch, h8668->ch_status.rx_freq); // 接收频率配置

    Ecr866xTxFreqCfg(h8668, h8668->ch, h8668->ch_status.tx_freq); // 发送频率配置

    Ecr866xRxGainCfg(h8668, h8668->ch, h8668->ch_status.rx_gain); // 接收增益配置

    Ecr866xTxGainCfg(h8668, h8668->ch, h8668->ch_status.tx_gain); // 发送增益配置

    ECR8660_write(h8668->config.spi_port, SPI_RW_EXTERNAL, ECR866x_CLOSE_RBDP_ADDR, (ECR866x_CLOSE_RBDP_DATA)); // 关闭RBDP
}

/**********************************************************
 *函数名：   BSP_8668BandWidthSet
 *函数功能： 单独配置带宽
 *返回值：   无
 */
void BSP_8668BandWidthSet(TRX8668_FPGA_t * h8668, uint32_t bw)
{
    vTaskDelay(100);                                // 配置通道前增加延时,防止通道配置失败

    Ecr866xBwDirectChoose(h8668, h8668->band, bw); // 带宽配置
}

/**
 *  NCO模式通道设置
 **/
void BSP_866xNcoChSet(TRX8668_FPGA_t * h8668, Ecr866xNco_T tNcoCfg)
{
    Ecr866xNcoCfg(h8668, tNcoCfg);
}

/**
 *  rssi检测模式通道设置
 **/
void BSP_866xRssiModeSet(TRX8668_FPGA_t * h8668, Ecr866xRssi_T tRssCfg)
{
    Ecr866xRssiModeCfg(h8668, tRssCfg);
}

/**
 *  866x 接收频率配置
 *  h8668    : 片选 0/1 0:下行使用的8668  1：上行
 *  ch      ：通道选择 0/1->CH0/CH1 每片8668都支持CH0和CH1，
 *  rx_freq ：接收频率,单位100kHz
 **/
void BSP_866xRxFreqSet(TRX8668_FPGA_t * h8668, uint32_t ch, uint32_t rx_freq)
{
    Ecr866xSetRxFreq(h8668, ch, rx_freq);
}

/**
 *  866x 发送频率设置
 *  h8668    : 片选
 *  ch      ：通道选择
 *  tx_freq ：发送频率,单位100kHz
 **/
void BSP_866xTxFreqSet(TRX8668_FPGA_t * h8668, uint32_t ch, uint32_t tx_freq)
{
    Ecr866xSetTxFreq(h8668, ch, tx_freq);
}

/**
 *  866x 接收增益配置（每次配置有100ms延时）
 *  h8668    : 片选
 *  ch      ：通道选择 CH0/CH1
 *  rx_gain ：接收增益偏移量  取值范围（-30~10）
 **/
void BSP_866xRxGainSet(TRX8668_FPGA_t * h8668, uint32_t ch, int rx_gain)
{
    Ecr866xSetRxGain(h8668, ch, rx_gain);
}

/**
 *  866x 发送增益配置
 *  h8668    : 片选 0/1
 *  ch      ：通道选择  ch0/ch1
 *  tx_gain ：发送增益偏移量   取值范围（-30~3）
 **/
void BSP_866xTxGainSet(TRX8668_FPGA_t * h8668, uint32_t ch, int tx_gain)
{
    Ecr866xSetTxGain(h8668, ch, tx_gain);
}

/**
 *  866x TX频率查询
 *  h8668     : 片选
 *  return  ：所选通道的频率值
 **/
uint32_t BSP_866xGetTxFreq(TRX8668_FPGA_t * h8668)
{
    uint32_t freq = 0;
    if(h8668->config.ch == 0)
        freq = Ecr866xGetFreq(h8668, ECR_QUERY_FREQ_CH0_TX);
    else if(h8668->config.ch == 1)
        freq = Ecr866xGetFreq(h8668, ECR_QUERY_FREQ_CH1_TX);

    return freq;
}

/**
 *  866x RX频率查询
 *  h8668     : 片选
 *  return  ：所选通道的频率值
 **/
uint32_t BSP_866xGetRxFreq(TRX8668_FPGA_t * h8668)
{
    uint32_t freq = 0;
    if(h8668->config.ch == 0)
        freq = Ecr866xGetFreq(h8668, ECR_QUERY_FREQ_CH0_RX);
    else if(h8668->config.ch == 1)
        freq = Ecr866xGetFreq(h8668, ECR_QUERY_FREQ_CH1_RX);

    return freq;
}

/**
 *  866x TX增益查询
 *  h8668    : 片选 0/1
 *  return  ：所选通道的增益
 **/
uint32_t BSP_866xGetTxGain(TRX8668_FPGA_t * h8668)
{
    uint32_t gain = 0;
    if(h8668->config.ch == 0)
        gain = Ecr866xGetGain(h8668, ECR_QUERY_GAIN_CH0_TX);
    else if(h8668->config.ch == 1)
        gain = Ecr866xGetGain(h8668, ECR_QUERY_GAIN_CH1_TX);

    return gain;
}

/**
 *  866x RX增益查询
 *  h8668    : 片选 0/1
 *  return  ：所选通道的增益
 **/
uint32_t BSP_866xGetRxGain(TRX8668_FPGA_t * h8668)
{
    uint32_t gain = 0;
    if(h8668->config.ch == 0)
        gain = Ecr866xGetGain(h8668, ECR_QUERY_GAIN_CH0_RX);
    else if(h8668->config.ch == 1)
        gain = Ecr866xGetGain(h8668, ECR_QUERY_GAIN_CH1_RX);

    return gain;
}
/**
 *  866x rssi查询
 *  h8668    : 片选
 *  ch      ：通道选择
 **/
char BSP_866xRssiRead(TRX8668_FPGA_t * h8668, uint32_t ch)
{
    return Ecr66xGetRssi(h8668, ch);
}

/*
 *  866x 页外寄存器写入函数
 *  spi_periph: SPI0~SPI2
 *  addr:   地址
 *  data：  数据
 */
void BSP_866x_Ext_Write(TRX8668_FPGA_t * h8668, uint32_t addr, uint32_t data)
{
    ECR8660_write(h8668->config.spi_port, SPI_RW_EXTERNAL, addr, (data));
    rtt_printf(RTT_8668, "[info]8668 %d Ext_Write, addr = 0x%08x data = 0x%08x \r\n", h8668->config.dev_number, addr, data);
}

/*
 *  866x 页内寄存器写入函数
 *  spi_periph: SPI0~SPI2
 *  addr:   地址
 *  data：  数据
 */
void BSP_866x_In_Write(TRX8668_FPGA_t * h8668, uint32_t addr, uint32_t data)
{
    ECR8660_write(h8668->config.spi_port, SPI_RW_INTERIOR, addr, (data));
    rtt_printf(RTT_8668, "[info]8668 %d In_Write, addr = 0x%08x data = 0x%08x \r\n", h8668->config.dev_number, addr, data);
}

/*
 *  866x 页外寄存器读取函数
 *  spi_periph: SPI0~SPI2
 *  addr:   地址
 *  data：  数据
 */
uint32_t BSP_866x_Ex_Read(TRX8668_FPGA_t * h8668, uint32_t addr)
{
    rtt_printf(RTT_8668, "[info]8668 %d Ex_Read, addr = 0x%08x \r\n", h8668->config.dev_number, addr);
    return ECR8660_Read(h8668->config.spi_port, SPI_RW_EXTERNAL, addr, 4);
}

/*
 *  866x 页外寄存器读取函数
 *  spi_periph: SPI0~SPI2
 *  addr:   地址
 *  data：  数据
 */
uint32_t BSP_866x_In_Read(TRX8668_FPGA_t * h8668, uint32_t addr)
{
    rtt_printf(RTT_8668, "[info]8668 %d In_Read, addr = 0x%08x \r\n", h8668->config.dev_number, addr);
    return ECR8660_Read(h8668->config.spi_port, SPI_RW_INTERIOR, addr, 2);
}
/**********************************************************
 *函数名：   BSP_866x_get_att_num
 *函数功能： 获取8668当前增益表可用衰减的数量
 *返回值：   可用衰减的数量
 */
uint8_t BSP_866x_get_att_num(TRX8668_FPGA_t *h8668)
{
    return h8668->gain_tab->gain_tab_number - h8668->gain_tab->rated_gain_index;
}

/*
 *  866x列表配置 可以同时配置多个寄存器
 */
void BSP_866xSetList(TRX8668_FPGA_t * h8668, Ecr8660_Reg_List_T * List)
{
    ECR8660_Set_List(h8668->config.spi_port, List);
}

/**
 *  8668 初始化（包括bin文件下载）
 *  spi_periph: SPI选择
 *  rst8668pin：8668复位对应的MCU引脚
 **/

Status_t BSP_Ecr8668Init(TRX8668_FPGA_t * h8668)
{
    Status_t ret;
    BSP_866x_MCU_reset(h8668);
    rtt_printf(RTT_8668, "8668 %d. Init start\r\n", h8668->config.dev_number);
    ret = ecr8668_init(h8668); // 初始化8668
    if(ret != STAT_OK)
    {
        return STAT_ERROR;
    }

    return STAT_OK;
}

/*
 * 866x 软复位
 */
void BSP_866x_MCU_reset(TRX8668_FPGA_t * h8668)
{
    rtt_printf(RTT_8668, "[info]8668 %d soft reset\r\n", h8668->config.dev_number);
    ecr8668_soft_reset(h8668->config.spi_port);
    vTaskDelay(100); // 延时调整5s->100ms
}

/**
 * @brief 正常/阻塞状态下的本振校准数据切换
 */
void BSP_866xLoCaliDataSwitch(void)
{
}

/**********************************************************
 *函数名：   BSP_866x_check_if_warning(TRX8668_FPGA_t * h8668)
 *函数功能： 判断当前是否存在饱和告警,部分寄存器操作在饱和告警时会导致死机
 *返回值：   无
 */
BOOL_E BSP_866x_check_if_warning(TRX8668_FPGA_t * h8668)
{
    if(h8668->adc_overload_trigger == B_TRUE || h8668->adc_antiblock_lopwr_cnt >0 || h8668->tia_overload_trigger == B_TRUE || h8668->tia_antiblock_lopwr_cnt >0)
        return B_TRUE;
    else
        return B_FALSE;
}


/*
 *  866x列表配置 可以同时配置多个寄存器
 */
void BSP_866xSetOverloadThreshold(TRX8668_FPGA_t * h8668, uint8_t threshold)
{
    uint16_t data_l = 0x1008;
    
    uint32_t value = ((threshold << 16) | data_l);
    
    BSP_866x_Ext_Write(h8668, 0x00200974, value);
}

