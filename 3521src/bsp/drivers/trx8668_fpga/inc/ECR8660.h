/********************************************************************************
 * @ Copyright    : Transa Semi.
 * @
 * @ File Name    : ECR8660.h
 * @ File Mark    :
 * @ Author       : GuoHao
 * @ E-Mail       : 844396800@qq.com
 * @ Website      : www.GuoHowe.com
 * @ Version      : v1.0
 * @ Date         : 2021-04-14 09:41:27
 * @ Description  :
 * @ History:
 * @       Version      : v1.0
 * @       LastEditors  : GuoHao
 * @       LastEditTime : 2021-08-27 10:31:20
 * @       Modification :
 ********************************************************************************/

#ifndef __ECR8660_H
#define __ECR8660_H

#ifdef __cplusplus
extern "C"
{
#endif

/********************************************************************************
 *                 Include file  eg:#include "header_file.h"
 ********************************************************************************/
#include "hi_systick.h"
// #include "gd32f4xx_libopt.h"
#include "Bit_macro.h"
#include <stdio.h>
#include "ECR8660_SPI.h"
#include "MCU_config.h"
    /********************************************************************************
     *             enum/struct/union/ Definition  eg:typedef struct{...}
     ********************************************************************************/
    typedef enum _ecr8660_log_level_e_
    {
        LOG_MIN_LEVEL,
        ERROR_LOG,   // 报错
        ENGLISH_LOG, // 阶段性的提示(英)
        CHINESE_LOG, // 阶段性的提示(中)
        PRINTF_LOG,  // 回显
        LOG_MAX_LEVEL,
    } Ecr8660_Log_Level_E;

// 增益码表
#define MAX_ALC_GAIN_NUM (52)      // 增益码表内最大可支持的增益档位数量
#define DEFAULT_ALC_GAIN_NUM (46)  // 内置默认增益码表档位数量
#define DEFAULT_GAIN_INDEX (6)     // 默认的增益档位索引值
#define ALC_GAIN_MAX_NORMAL (40)   // 增益码表内增益数量
#define ALC_GAIN_MAX_BLOCKING (40) // 增益码表内增益数量

#define EXIT_ANTI_BLOCK_LIMIT (100)     // 退出抗阻塞模式的次数限制
#define EXIT_ANTI_BLOCK_TIME (10 * 60 * 1000) // 退出抗阻塞模式的最小间隔时间单位ms
    typedef struct
    {
        uint32_t gain_tab[MAX_ALC_GAIN_NUM][4]; // 增益码表,预留64个码表档位
        uint8_t gain_tab_number;                // 码表档位数量
        uint8_t rated_gain_index;               // 额定增益在码表中的索引值(即0衰减在码表的索引值)
    } GAIN_TABLE_T;

    typedef struct __gain_cali_lookup_t
    {
        uint8_t gain_tab_index; // 增益码表索引值
        uint8_t rx_cali_index;  // rx本振校准值索引
        uint8_t tx_cali_index;  // rx本振校准值索引
    } gain_cali_lookup_tab_T;

    typedef struct __rated_gain_index_lookup_tab_t
    {
        RF_BAND_E band; // band
        uint32_t bw;    // 带宽
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
        uint8_t index_of_rated_gain_index;  // 额定增益索引值的索引值,用于选择额定增益索引值
    } rated_gain_index_lookup_tab_t;

    typedef struct __FLASH_xxxx_HwConfig_t
    {
        trx8668_port_t spi_port;
        PORT_T rst_port;    // 8668 reset
        GPIO_PIN_T rst_pin; // 8668 reset
        uint32_t ch;        // 使用的通道 0或1
        uint32_t port;      // RTX端口配置
        uint8_t dev_number; // 8668设备号,用于区别板上多片8668/3521项目上数值等同于rf_sys
        IRQ_T adc_irq;      // 8668 adc告警中断
        IRQ_T tia_irq;      // 8668 tia告警中断
        // 此处定义该设备所需的硬件接口（如GPIO、SPI等），资源（内存，缓冲区），查找表（功率表，衰减表，温度表）
    } Trx8668_hw_config_t;

    typedef struct
    {
        uint32_t ch_mode;   // 通道模式
        uint32_t bw_choose; // 带宽选择
        uint32_t tx_freq;   // TX频率设置
        int tx_gain;        // TX增益设置
        uint32_t port;      // RTX端口配置
        uint32_t loop;      // 回环配置
    } Ecr866xNco_T;         // 866x NCO配置

    typedef struct
    {
        uint32_t ch_mode;   // 通道模式
        uint32_t bw_choose; // 带宽选择
        uint32_t rx_freq;   // RX频率设置
        int rx_gain;        // RX增益偏移量
        uint32_t port;      // RTX端口配置
    } Ecr866xRssi_T;

    typedef struct
    {
        uint32_t ch_mode;          // 通道模式
        uint32_t bw_choose;        // 带宽选择
        uint32_t rx_freq;          // RX频率设置
        uint32_t tx_freq;          // TX频率设置
        int rx_gain;               // RX增益偏移量
        int tx_gain;               // TX增益偏移量
        int ch_gain;               // 当前通道增益
        int ch_att;                // 当前通道衰减值 = 额定增益 - 当前通道增益
        int gain_tab_index;        // 当前通道所用增益码表索引值
        uint32_t port;             // RTX端口配置
        uint32_t loop;             // 回环配置
        uint32_t channel_stat;     // 8668通道开关状态# OPEN_RX_CLOSE_TX_FLAG 0  #OPEN_TX_CLOSE_RX_FLAG 1 #CLOSE_TXRX_FLAG 2  #OPEN_TXRX_FLAG 3
        uint32_t cur_cali_tx_lo_I; // 当前tx本振校准I
        uint32_t cur_cali_tx_lo_Q; // 当前tx本振校准Q
        uint32_t cur_cali_rx_lo_I; // 当前rx本振校准I
        uint32_t cur_cali_rx_lo_Q; // 当前rx本振校准Q
        uint32_t cur_cali_mirror;  // 当前镜像校准
    } Ecr866xChannelStatus_T;      // 866x射频通道当前工作参数

    typedef struct
    {
        uint32_t cali_tx_lo_I[10];              // 校准参数组1 tx_lo_I
        uint32_t cali_tx_lo_Q[10];              // 校准参数组2 tx_lo_Q
        uint32_t cali_rx_lo_I[10];              // 校准参数组3 rx_lo_I
        uint32_t cali_rx_lo_Q[10];              // 校准参数组4 rx_lo_Q
        uint32_t cali_mirror[5];                // 校准参数组5 mirror
        GAIN_TABLE_T normal_gain_tab;           // 普通模式码表
        GAIN_TABLE_T blocking_gain_tab;         // 阻塞模式码表
        int8_t default_gain;                    // 8668初始增益
        uint8_t rated_gain_index_normal[4];     // 校准参数组6 8668额定增益码表索引值
        uint8_t rated_gain_index_anti_block[4]; // 校准参数组7 8668抗阻塞模式额定增益码表索引值
    } factroy_data_T;                           // 866x射频通道参数flash保存数据

    typedef struct __TRX8668_FPGA_TypeDef // 866x结构体
    {
        /* 866x射频通道参数和硬件配置 */
        Trx8668_hw_config_t config;                     // 设备硬件资源
        volatile BOOL_E lock;                           // 互斥锁
        RF_BAND_E band;                                 // 当前使用的band
        uint32_t ch;                                    // 当前使用的通道
        Ecr866xChannelStatus_T ch_status;               // 当前通道参数
        GAIN_TABLE_T *gain_tab;                         // 指向当前使用的增益码表
        gain_cali_lookup_tab_T *gain_cali_lookup_table; // 指向当前使用的增益档位和校准值的查找表
        uint32_t bin_addr;                              // 当前使用的bin文件地址
        uint32_t bin_siz;                               // 当前使用的bin文件大小

        /*抗阻塞模式和饱和告警相关*/
        uint32_t exti_trigger_tick;  // 用于周期性打开exti中断的周期计数
        uint8_t adc_antiblock_lopwr_cnt;    // 用于周期性打开exti中断并记录中断是否连续触发的计数,当饱和且功率低于起控点时,快速计数
        uint8_t tia_antiblock_lopwr_cnt;    // 用于周期性打开exti中断并记录中断是否连续触发的计数,当饱和且功率低于起控点时,快速计数
        uint8_t adc_antiblock_cnt;    // 周期性打开exti中断并记录中断是否连续触发且需要切换到抗阻塞模式的计数,当饱和且高于启动点的计数,周期更长
        uint8_t tia_antiblock_cnt;    // 周期性打开exti中断并记录中断是否连续触发且需要切换到抗阻塞模式的计数,当饱和且高于启动点的计数,周期更长
        BOOL_E adc_overload_trigger; // 用于周期性打开exti中断并记录中断是否连续触发的计数
        BOOL_E adc_overload_flag;    // adc饱和标志,表示8668处在饱和状态,饱和时不可读寄存器
        BOOL_E tia_overload_trigger; // 用于周期性打开exti中断并记录中断是否连续触发的计数
        BOOL_E tia_overload_flag;    // adc饱和标志,表示8668处在饱和状态,饱和时不可读寄存器
        BOOL_E anti_block_stat;      // B_TRUE 阻塞模式 B_FALSE正常模式
        uint32_t anti_block_tick;    // 用于记录进入抗阻塞模式的时间
        uint32_t anti_block_cnt;     // 用于记录触发抗阻塞模式的次数
        uint8_t anti_block_switch;   // 抗阻塞是能开关

        /*flash保存的8668参数*/
        factroy_data_T factroy_data; // 校准参数,包括增益表,校准值,初始增益等
        
        /*其他参数*/
        BOOL_E silent_mode; // B_TRUE 处于静默模式 B_FALSE正常模式
        uint32_t cali_refersh_tick; // 上次更新校准值的时间,用于校准值周期刷新,校准值会在调用衰减设置时刷新,当一段时间未设置衰减,但是其他参数比如bw等可能发生变化,需要进行自动刷新

        BOOL_E error_flag; // 错误状态位
        uint8_t error_cnt; // error计数,3次重启

    } TRX8668_FPGA_t;
/********************************************************************************
 *               Macro Definition  eg:#define Macro Definition
 ********************************************************************************/
//          Promt_Sel
#define PRINT_LOG (0)

#if PRINT_LOG
#define SET_PRINT_MIN(x) (User_Printf_LOG = ((User_Printf_LOG > (x) ? (x) : User_Printf_LOG)))
#define SET_PRINT_BACK(x) ((x) = User_Printf_LOG)
#define SET_PRINT_RECO(x) (User_Printf_LOG = (x))

#define DBG_LEVEL_MAX (LOG_MAX_LEVEL) // 一级限制设置，禁止大于改值的 Log 输出
#define DEBUG_PRINTF(x, ...)                                \
    if (((x) <= User_Printf_LOG) && ((x) <= DBG_LEVEL_MAX)) \
    User_Printf(x, ##__VA_ARGS__)
#define DEBUG_ERR(...) DEBUG_PRINTF(ERROR_LOG, ##__VA_ARGS__)   // 报错
#define DEBUG_LGE(...) DEBUG_PRINTF(ENGLISH_LOG, ##__VA_ARGS__) // 阶段性的提示(英)
#define DEBUG_LGC(...) DEBUG_PRINTF(CHINESE_LOG, ##__VA_ARGS__) // 阶段性的提示(中)
#define DEBUG_OUT(...) DEBUG_PRINTF(PRINTF_LOG, ##__VA_ARGS__)  // 回显
#endif

    //          ECR8660_MEM_ADDR

#define ECR_8668_FLASH_ADDR (0x08040000) // GD32中的ERR8668代码的FLASH地址
#define ECR_8668_START_ADDR (0x00000000) // ECR8668存放bin文件的地址

#define MCU_LOG_ADDR_HAND (0x20000000)
#define MCU_VCO_ADDR_HAND (0x20001000)
#define MCU_VER_ADDR_HAND (0x20002000)
#define MCU_API_ADDR_HAND (0x20004000)
#define MCU_REG_ADDR_HAND (0x20005000)
#define MCU_EFU_ADDR_HAND (0x20006000)
#define MCU_CFG_ADDR_HAND (0x20006800)

#define MCU_ECR8668_INIT_SURE (0x20000004) // ECR8668初始化状态判断，读此寄存器，值为0x123代表初始化成功
#define MCU_ECR8668_INIT_FLAG (0x123)
#define MCU_CFG_ADDR_REF MCU_CFG_ADDR_HAND

#define MCU_LOG_ADDR_LIST(x) (MCU_LOG_ADDR_HAND + ((x) << 2))
#define MCU_LOG_ADDR_WORK (MCU_LOG_ADDR_HAND + 0x4)

#define MCU_VER_ADDR_VER0 (MCU_VER_ADDR_HAND)
#define MCU_VER_ADDR_VER1 (MCU_VER_ADDR_HAND + 0x04)
#define MCU_VER_ADDR_ERROR0 (MCU_VER_ADDR_HAND + 0x08)
#define MCU_VER_ADDR_ERROR1 (MCU_VER_ADDR_HAND + 0x0C)

#define MCU_EFU_ADDR_ID0 (MCU_EFU_ADDR_HAND)
#define MCU_EFU_ADDR_ID1 (MCU_EFU_ADDR_HAND + 0x4)

#define ECR8660_CODE_ADDR_HEAD (0x00000000) // ECR8660 的代码段起始地址
#define ECR8660_CODE_MAX_SIZE BUFF_64K      // ECR8660 的代码最大Size

//          MCU_Code_Size_Addr
#define MCU_CODE_SIZE_ADDR (0x00000010) // ECR8660 的复位寄存器

//          ECR8660_REG_ADDR
#define SPI_REG_SOC_RESET (0xFFC) // ECR8660 的复位寄存器
#define SPI_REG_MCU_RESET (0x7FC) // ECR8660 内 MCU 的复位寄存器
#define SPI_REG_BASE_ADDR (0xFF0) // 基地址寄存器
#define SPI_REG_DCXO_EN_L (0xF08) // DCXO 的配置寄存器
#define SPI_REG_DCXO_EN_H (0xF04) // DCXO 的配置寄存器
#define SPI_REG_SPI_PADPC (0xF00) // SPI  驱动能力设置
#define SPI_REG_LDO0_EN_L (0xF1C) // LDO  的配置寄存器
#define SPI_REG_LDO0_EN_H (0xF20) // LDO  的配置寄存器
#define SPI_REG_SOC_ISO_E (0xF40) // 配置电源隔离的寄存器

    /********************************************************************************
     *           Function Definitions  eg:void Function_Definitions(void);
     ********************************************************************************/
    Status_t ECR8660_INIT(TRX8668_FPGA_t *h8668, uint32_t flash_addr, uint32_t bin_size);
    Status_t ECR8660_CodeDownload(trx8668_port_t spi_periph, INT32U Flash_Addr_Head, INT32U len);
    void ECR8660_CodeUpData(INT32U Code_Addr, INT32U Code_Size);

/********************************************************************************
 *            Global variable  eg:extern unsigned int Global_variable;
 ********************************************************************************/
#if PRINT_LOG
    extern Ecr8660_Log_Level_E User_Printf_LOG;
#endif

#include "ECR8660_SPI.h"
#include "ECR8660_Config.h"
#include "ECR8660_Error.h"
    void ecr8668_soft_reset(trx8668_port_t spi_periph);

#ifdef __cplusplus
}
#endif

#endif /* __ECR8660_H */
