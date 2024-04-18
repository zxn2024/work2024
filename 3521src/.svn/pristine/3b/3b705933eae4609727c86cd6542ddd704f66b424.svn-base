#ifndef __BSP_ECR8668_H_
#define __BSP_ECR8668_H_

#include "ecr8668.h"

/*---------------------- 结构体定义 ------------------------------------------*/
typedef enum
{
	CALI_8668_RX_LO_I = 0,
	CALI_8668_RX_LO_Q,		
	CALI_8668_TX_LO_I,
	CALI_8668_TX_LO_Q,
	CALI_8668_MIRROR,	
	CALI_8668_END
}CALI_8668_TYPE_E;

typedef enum
{
	TRX_MODE_NORMAL = 0, //普通模式
	TRX_MODE_ANTI_BLOCKING, //抗阻塞模式
}TRX_MODE_E;

/*---------------------- 声明 ------------------------------------------*/
/**********************************************************
 *函数名：   trx8668_fpga_register
 *函数功能： 8668 器件注册
 *返回值：   无
 */
Status_t trx8668_fpga_register(TRX8668_FPGA_t *handle_8668);

/**********************************************************
 *函数名：   BSP_866x_lock
 *函数功能： 锁定8668芯片,使其不能被其他程序使用,如果已被其他任务锁定立刻返回失败
 *返回值：   STAT_ERROR STAT_OK
 */
Status_t BSP_866x_try_lock(TRX8668_FPGA_t * h8668);

/**********************************************************
 *函数名：   BSP_866x_lock
 *函数功能： 锁定8668芯片,使其不能被其他程序使用,超时则返回失败
 *返回值：   STAT_ERROR STAT_OK
 */
Status_t BSP_866x_wait_lock(TRX8668_FPGA_t * h8668, uint32_t timeout_ms);
/**********************************************************
 *函数名：   BSP_866x_unlock
 *函数功能： 解锁8668芯片,使其可以被其他程序使用
 *返回值：   无
 */
void BSP_866x_unlock(TRX8668_FPGA_t * h8668);
/**********************************************************
 *函数名：   BSP_866x_Ext_Write
 *函数功能： 页外寄存器写
 *返回值：   无
 */
void BSP_866x_Ext_Write(TRX8668_FPGA_t *h8668, uint32_t addr, uint32_t data);
/**********************************************************
 *函数名：   BSP_866x_In_Write
 *函数功能： 页内寄存器写
 *返回值：   无
 */
void BSP_866x_In_Write(TRX8668_FPGA_t *h8668, uint32_t addr, uint32_t data);
/**********************************************************
 *函数名：   BSP_866x_Ex_Read
 *函数功能： 页外寄存器读
 *返回值：   无
 */
uint32_t BSP_866x_Ex_Read(TRX8668_FPGA_t *h8668, uint32_t addr);
/**********************************************************
 *函数名：   BSP_866x_In_Read
 *函数功能： 页内寄存器读
 *返回值：   无
 */
uint32_t BSP_866x_In_Read(TRX8668_FPGA_t *h8668, uint32_t addr);

/**********************************************************
 *函数名：   BSP_Ecr8668Init
 *函数功能： 8668初始化
 *返回值：   无
 */
Status_t BSP_Ecr8668Init(TRX8668_FPGA_t *h8668);
/**********************************************************
 *函数名：   BSP_866x_MCU_reset
 *函数功能： 866x 软复位
 *返回值：   无
 */
void BSP_866x_MCU_reset(TRX8668_FPGA_t *h8668);
/**********************************************************
 *函数名：   BSP_866xSetList
 *函数功能： 执行866x列表配置,依次配置列表中的多个寄存器
 *返回值：   无
 */
void BSP_866xSetList(TRX8668_FPGA_t *h8668, Ecr8660_Reg_List_T *List);
/**********************************************************
 *函数名：   BSP_8668AttSet
 *函数功能： 经过封装的衰减值设置接口
 *返回值：   无
 */
Status_t BSP_8668AttSet(TRX8668_FPGA_t * h8668, int8_t value);

/**********************************************************
 *函数名：   BSP_866xNcoChSet
 *函数功能： 单音模式通道设置
 *返回值：   无
 */
void BSP_866xNcoChSet(TRX8668_FPGA_t *h8668,Ecr866xNco_T tNcoCfg);
/**********************************************************
 *函数名：   BSP_866xRssiModeSet
 *函数功能： rssi检测模式通道设置,用于扫频
 *返回值：   无
 */
void BSP_866xRssiModeSet(TRX8668_FPGA_t *h8668,Ecr866xRssi_T tRssCfg);

/**********************************************************
 *函数名：   BSP_8668ChModeSet
 *函数功能： 设置通道模式,可以单独配置通道模式
 *返回值：   无
 */
void BSP_8668ChModeSet(TRX8668_FPGA_t *h8668,  uint32_t mode);

/**********************************************************
 *函数名：   BSP_8668BandSet
 *函数功能： 带宽设置接口,可以单独配置带宽
 *返回值：   无
 */
void BSP_8668BandWidthSet(TRX8668_FPGA_t *h8668,  uint32_t bw);
/**********************************************************
 *函数名：   BSP_866xRxFreqSet
 *函数功能： 866x 可以单独配置RX频率
 *参数:  h8668   : 片选
 *参数:  ch      ：通道选择 0/1->CH0/CH1 每片8668都支持CH0和CH1
 *参数:  rx_freq ：接收频率,单位100kHz
 */
void BSP_866xRxFreqSet(TRX8668_FPGA_t *h8668, uint32_t ch, uint32_t rx_freq);
/**********************************************************
 *函数名：   BSP_866xTxFreqSet
 *函数功能： 866x 可以单独配置RX频率
 *参数:  h8668   : 片选
 *参数:  ch      ：通道选择 0/1->CH0/CH1 每片8668都支持CH0和CH1
 *参数:  rx_freq ：接收频率,单位100kHz
 */
void BSP_866xTxFreqSet(TRX8668_FPGA_t *h8668, uint32_t ch, uint32_t tx_freq);
/**********************************************************
 *函数名：   BSP_866xRxGainSet
 *函数功能： 通道初始增益值,可以单独配置RX增益偏移量
 *返回值：   无
 */
void BSP_866xRxGainSet(TRX8668_FPGA_t *h8668, uint32_t ch, int rx_gain);
/**********************************************************
 *函数名：   BSP_866xTxGainSet
 *函数功能： 通道初始增益值,可以单独配置TX增益偏移量
 *返回值：   无
 */
void BSP_866xTxGainSet(TRX8668_FPGA_t *h8668, uint32_t ch, int tx_gain);
/**********************************************************
 *  866x TX频率查询
 *  h8668     : 片选
 *  return  ：所选通道的频率值
**/
uint32_t BSP_866xGetTxFreq(TRX8668_FPGA_t *h8668);

/**********************************************************
 *  866x RX频率查询
 *  h8668     : 片选
 *  return  ：所选通道的频率值
**/
uint32_t BSP_866xGetRxFreq(TRX8668_FPGA_t *h8668);
/**********************************************************
 *  866x TX增益查询
 *  h8668    : 片选 0/1
 *  return  ：所选通道的增益
**/
uint32_t BSP_866xGetTxGain(TRX8668_FPGA_t *h8668);

/**********************************************************
 *  866x RX增益查询
 *  h8668    : 片选 0/1
 *  return  ：所选通道的增益
**/
uint32_t BSP_866xGetRxGain(TRX8668_FPGA_t *h8668);
/**********************************************************
 *函数名：   BSP_866xGainQuery
 *函数功能： 查询增益
 *返回值：   无
 */
uint32_t BSP_866xGainQuery(TRX8668_FPGA_t *h8668, uint32_t ch);
/**********************************************************
 *函数名：   BSP_866xRssiRead
 *函数功能： 读RSSI
 *返回值：   无
 */
char BSP_866xRssiRead(TRX8668_FPGA_t *h8668, uint32_t ch);
/**********************************************************
 *函数名：   BSP_866xCloseTxRx
 *函数功能： 关闭射频收发
 *返回值：   无
 */
void BSP_866xCloseTxRx(TRX8668_FPGA_t *h8668);
/**********************************************************
 *函数名：   BSP_866xCloseTx
 *函数功能： 关闭射频发
 *返回值：   无
 */
void BSP_866xCloseTx(TRX8668_FPGA_t *h8668);
/**********************************************************
 *函数名：   BSP_866xCloseRx
 *函数功能： 关闭射频收
 *返回值：   无
 */
void BSP_866xCloseRx(TRX8668_FPGA_t *h8668);
/**********************************************************
 *函数名：   BSP_866xOpenTxRx
 *函数功能： 打开射频收发
 *返回值：   无
 */
void BSP_866xOpenTxRx(TRX8668_FPGA_t *h8668);
/**********************************************************
 *函数名：   BSP_866xLoCaliSwitch
 *函数功能： 检测阻塞模式本振切换
 *返回值：   无
 */
void BSP_866xLoCaliSwitch(void);

/************************************************************************
                       以下为3521项目的业务接口
************************************************************************/

/*---------------------- 宏定义 ------------------------------------------*/

/*---------------------- 函数声明 ----------------------------------------*/
/**********************************************************
 *函数名：   BSP_866x_set_band
 *函数功能： trx8668根据选定的band,带宽,频点进行初始化,重加载bin文件,并加载对应band的增益表和校准参数
 *函数参数: band 频段
 *函数参数: band_width 带宽
        #define PLC_LTE_10MHZ (10)         // LTE 10 MHz
        #define PLC_LTE_15MHZ (15)         // LTE 15 MHz
        #define PLC_LTE_20MHZ (20)         // LTE 20 MHz
        #define PLC_LTE_25MHZ (25)         // LTE 15 MHz
        #define PLC_LTE_30MHZ (30)         // LTE 20 MHz
        #define PLC_LTE_50MHZ (50)         // LTE 20 MHz
        #define PLC_NR_40MHZ (40)          // NR 40 MHz
        #define PLC_NR_60MHZ (60)          // NR 60 MHz
        #define PLC_NR_80MHZ (80)          // NR 80 MHz
        #define PLC_NR_100MHZ (100)        // NR 100 MHz
 *函数参数: freq 频点,单位100kHz
 *返回值：   无
 */

Status_t BSP_866x_set_band(TRX8668_FPGA_t *h8668, RF_BAND_E band, uint32_t band_width, uint32_t freq);

/**********************************************************
 *函数名：   BSP_866x_run
 *函数功能： trx8668的告警处理(adc饱和等),错误处理,器件状态监控以及其他一些8668器件本身的业务
 *返回值：   无
 */
void BSP_866x_run(TRX8668_FPGA_t * h8668);

/**********************************************************
 *函数名：   isr_866x_quit_silent_mode
 *函数功能： (从机)退出上行静默,在GPIO中断中调用
 *返回值：   无
 */
void isr_866x_quit_silent_mode(TRX8668_FPGA_t *h8668);

/**********************************************************
 *函数名：   BSP_866x_enter_silent_mode
 *函数功能： (从机)8668进入上行静默
 *返回值：   无
 */
void BSP_866x_enter_silent_mode(TRX8668_FPGA_t *h8668);

/**********************************************************
 *函数名：   BSP_866x_set_cali_data
 *函数功能： 保存校准值,写入到flash
 *参数:      band 频段
 *参数:      group 0-10 校准参数按工况分几档,具体分档方式根据调测结果决定
 *参数:      cali_data_type 校准值类型
 *参数:      cali_data 校准值
 *返回值：   Status_t
 */
Status_t BSP_866x_set_cali_data(RF_BAND_E band, CALI_8668_TYPE_E cali_data_type, uint8_t group, uint32_t cali_data);

/**********************************************************
 *函数名：   BSP_866x_set_cali_data
 *函数功能： 保存校准值,写入到flash
 *参数:      band 频段
 *参数:      group 0-10 校准参数按工况分几档,具体分档方式根据调测结果决定
 *参数:      cali_data_type 校准值类型
 *返回值：   cali_data 校准值
 */
uint32_t BSP_866x_get_cali_data(RF_BAND_E band, CALI_8668_TYPE_E cali_data_type, uint8_t group);

/**********************************************************
 *函数名：   BSP_866x_set_default_gain
 *函数功能： 保存默认增益,写入到flash
 *参数:      band 频段
 *参数:      default_gain 默认增益
 *返回值：   无
 */
Status_t BSP_866x_set_default_gain(RF_BAND_E band, char default_gain);

/**********************************************************
 *函数名：   BSP_866x_set_default_gain
 *函数功能： 保存默认增益,写入到flash
 *参数:      band 频段
 *返回值:      default_gain 默认增益
 */
char BSP_866x_get_default_gain(RF_BAND_E band);


/**********************************************************
 *函数名：   BSP_866x_set_rated_gain_index
 *函数功能： 设置额定增益在码表的索引值
 *参数:      band 频段
 *参数:      mode 0正常模式 1抗阻塞模式
 *参数:      index 索引值
 *参数:      bw 带宽
            // PLC_LTE_10MHZ (10)         // LTE 10 MHz
            // PLC_LTE_15MHZ (15)         // LTE 15 MHz
            // PLC_LTE_20MHZ (20)         // LTE 20 MHz
            // PLC_LTE_25MHZ (25)         // LTE 15 MHz
            // PLC_LTE_30MHZ (30)         // LTE 20 MHz
            // PLC_LTE_50MHZ (50)         // LTE 20 MHz
            // PLC_NR_20MHZ (20)          // NR 20 MHz
            // PLC_NR_60MHZ (60)          // NR 60 MHz
            // PLC_NR_80MHZ (80)          // NR 80 MHz
            // PLC_NR_100MHZ (100)        // NR 100 MHz
 *返回值：   无
 */
Status_t BSP_866x_set_rated_gain_index(RF_BAND_E band, TRX_MODE_E mode, uint32_t bw, uint8_t gain_index);

/**********************************************************
 *函数名：   BSP_866x_get_rated_gain_index
 *函数功能： 读取额定增益在码表的索引值
 *参数:      band 频段
 *参数:      bw 带宽
            // PLC_LTE_10MHZ (10)         // LTE 10 MHz
            // PLC_LTE_15MHZ (15)         // LTE 15 MHz
            // PLC_LTE_20MHZ (20)         // LTE 20 MHz
            // PLC_LTE_25MHZ (25)         // LTE 15 MHz
            // PLC_LTE_30MHZ (30)         // LTE 20 MHz
            // PLC_LTE_50MHZ (50)         // LTE 20 MHz
            // PLC_NR_20MHZ (20)          // NR 20 MHz
            // PLC_NR_60MHZ (60)          // NR 60 MHz
            // PLC_NR_80MHZ (80)          // NR 80 MHz
            // PLC_NR_100MHZ (100)        // NR 100 MHz
 *返回值:      mode  0正常模式 1抗阻塞模式
 */
uint8_t BSP_866x_get_rated_gain_index(RF_BAND_E band, TRX_MODE_E mode, uint32_t bw);
/**********************************************************
 *函数名：   BSP_866x_set_gain_table
 *函数功能： 保存gain table,写入到flash
 *参数:      band 频段
 *参数:      gain_tab_type 增益表类型 普通模式增益或阻塞模式增益
 *参数:      gain_tab 增益表
 *参数:      len 增益表长度
 *返回值：   无
 */
Status_t BSP_866x_set_gain_table(RF_BAND_E band, TRX_MODE_E gain_tab_type, uint8_t *gain_tab, uint32_t len);

/**********************************************************
 *函数名：   BSP_866x_get_gain_table
 *函数功能： 读出gain table
 *参数:      band 频段
 *参数:      gain_tab_type 增益表类型 普通模式增益或阻塞模式增益
 *参数:      gain_tab 增益表
 *参数:      len 增益表长度
 *返回值：   无
 */
Status_t BSP_866x_get_gain_table(RF_BAND_E band, TRX_MODE_E gain_tab_type, uint8_t *gain_tab, uint32_t len);

/**********************************************************
 *函数名：   BSP_866x_cali_data_switch
 *函数功能： 根据当前的射频参数,band,带宽,增益等,设置校准参数
 *返回值：   无
 */
void BSP_866x_cali_data_switch(TRX8668_FPGA_t *h8668);

/**********************************************************
 *函数名：   BSP_866x_adc_warning_exti_handle
 *函数功能： Adc饱和告警判断和处理
 *返回值：   无
 */
void BSP_866x_adc_warning_exti_handle(TRX8668_FPGA_t *h8668);

/**********************************************************
 *函数名：   BSP_866x_tia_warning_exti_handle
 *函数功能： Adc饱和告警中断处理函数
 *返回值：   无
 */
void BSP_866x_tia_warning_exti_handle(TRX8668_FPGA_t * h8668);

/**********************************************************
 *函数名：   BSP_866x_open_adc_warning_exti_periodic
 *函数功能： 周期性(50ms)打开adc饱和中断
 *返回值：   无
 */
void BSP_866x_open_adc_warning_exti_periodic(TRX8668_FPGA_t *h8668);

/**********************************************************
 *函数名：   BSP_866x_tia_adc_switch
 *函数功能： 饱和中断使能开关
 *返回值：   无
 */
void BSP_866x_tia_adc_switch(RF_SYS_E sys, uint8_t OnOff);

/**********************************************************
 *函数名：   BSP_866x_open_adc_warning_exti_periodic
 *函数功能： 周期性(50ms)打开adc饱和中断
 *返回值：   无
 */
void BSP_866x_Proc_warning_exti_periodic(RF_SYS_E sys, TRX8668_FPGA_t *h8668);

/**********************************************************
 *函数名：   BSP_866x_get_att_num
 *函数功能： 获取8668当前增益表可用衰减的数量
 *返回值：   可用衰减的数量
 */
uint8_t BSP_866x_get_att_num(TRX8668_FPGA_t *h8668);

/**********************************************************
 *函数名：   BSP_866x_cali_test
 *函数功能： 8668校准存取测试
 *返回值：   无
 *说  明：   校准值存取,默认增益存取,码表存取
 */
void BSP_866x_cali_test(void);

/**********************************************************
 *函数名：   BSP_866xAttSetTest
 *函数功能： 用于测试8668衰减设置
 *返回值：   无
 */
void BSP_866xAttSetTest(void);

#endif
