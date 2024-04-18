/*********************************************************************************
 * @ Copyright    : Transa Semi.
 * @
 * @ File Name    : ECR8660_Config.h
 * @ File Mark    : 
 * @ Author       : GuoHao
 * @ E-Mail       : 844396800@qq.com
 * @ Website      : www.GuoHowe.com
 * @ Version      : v1.0
 * @ Date         : 2021-04-19 09:18:09
 * @ Description  : 
 * @ History: 
 * @       Version      : v1.0
 * @       LastEditors  : GuoHao
 * @       LastEditTime : 2021-08-28 12:35:54
 * @       Modification :  
********************************************************************************/

#ifndef __ECR8660_CONFIG_H
#define __ECR8660_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif
/********************************************************************************
*                  Include file  eg:#include "header_file.h"
********************************************************************************/
#include    "Bit_macro.h"
#include    "MCU_config.h"
//#include    "gd32f4xx.h"

/********************************************************************************
*             enum/struct/union/ Definition  eg:typedef struct{...}
********************************************************************************/
typedef enum _ecr8660_path_idx_e_{
    RXCH0,
    RXCH1,
    TXCH0,
    TXCH1,
}Ecr8660_Path_Idx_E;

typedef enum _ecr8660_ch_sel_e_{
    NOT_CHOOSE,
    CHOOSE_CH0,
    CHOOSE_CH1,
    CHOOSE_CH01,
}Ecr8660_Ch_Sel_E;

typedef enum _ecr8660_mode_e_{
    LTE_FDD,
    LTE_TDD,
}Ecr8660_Mode_E;

typedef enum _ecr8660_port_idx_e_{
    PORT_AUTO,
    PORT_A,
    PORT_B,
    PORT_C,
}Ecr8660_Port_Idx_E;

typedef enum _ecr8660_int_ext_bandwidth_idx_e_{
    LTE1P4,
    LTE3,
    LTE5,     
    LTE10,
    LTE15,
    LTE20,
    LTE40,
    LTE60,
    LTE80,
    LTE100,
    EXT_BW0,
    EXT_BW1,
    EXT_BW2,
    EXT_BW3,
    EXT_BW4,
    EXT_BW5,
    EXT_BW6,
    EXT_BW7,
    EXT_BW8,
    EXT_BW9,
}Ecr8660_Int_Ext_BW_Idx_E;

typedef enum _ecr8660_bandwidth_idx_e_{
    BW_ID_0,
    BW_ID_1,
    BW_ID_2,
    BW_ID_3,
    BW_ID_4,
    BW_ID_5,
    BW_ID_6,
    BW_ID_7,
    BW_ID_8,
    BW_ID_9,
}Ecr8660_BW_Idx_E;

typedef enum _ecr8660_cmd_e_{
    ECR8660_CMD_ERROR           ,
    ECR8660_CMD_API             ,   // 配置指定通道整个通道的属性 [LO/BW/GAIN]
    ECR8660_CMD_IQMIS           ,   // 配置 Rx 的 IQMis
    ECR8660_CMD_RX_RCCAL        ,   // 触发 Rx 的 RCCal
    ECR8660_CMD_BBPLL           ,   // 单独配置 BBPLL
    ECR8660_CMD_AUX_DAC         ,   // 配置 Aux_DAC 的开关
    ECR8660_CMD_LVDS            ,   // 配置 LVDS 模式的开关
    ECR8660_CMD_DCOC            ,   // 触发 DCOC 校准
    ECR8660_CMD_PLL             ,   // 单独配置 RF_PLL
    ECR8660_CMD_GAIN            ,   // 单独配置增益
    ECR8660_CMD_AGC             ,   // AGC 模式控制
    ECR8660_CMD_BW              ,   // 单独切换带宽
    ECR8660_CMD_REG_TEST        ,   // 寄存器读写测试
    ECR8660_CMO_FREQUE          ,   // FREQUE 的测试模块
    ECR8660_CMO_SWITCH          ,   // 部分功能模块的开关 BValue[1]:0-IQ的开关，BValue[1]:1-Rx_Fir的开关
    ECR8660_CMO_APC_LNA_SW      ,   // Rx_LNA / Tx_APC 的开关
    ECR8660_CMO_TDD_SWITCH_TRX  ,   // 进行Tx / Rx的开关切换
    ECR8660_CMD_SWITCH_PLL      ,   // 切换通道1的 PLL，为通道0/通道1
    ECR8660_CMD_CHIP_TYPE       ,   // 运行 process sensor
    ECR8660_CMD_READ_GAIN_GEARS ,   // 读取指定通道的增益档位
}Ecr8660_Cmd_E;

typedef union _ecr8660_ver_u_{
    INT32U Word;
    struct{
        INT32U              minor               : 16;   // [15:00]
        INT32U              middl               :  8;   // [23:16]
        INT32U              major               :  8;   // [31:24]
    }Bits;
}Ecr8660_Ver_U;

/****************************************************************************
 *  API 的结构体说明
****************************************************************************/
typedef union _mode_status_ctorl_u_{
    INT32U Word;
    struct{
        ON_OFF              RxStatus            :  2;   // [01:00]  Rx 通道状态                         [0:关闭 RxAntIndicate 所选的 Rx 通道    1:打开 RxAntIndicate 所选的 Rx 通道]
        Ecr8660_Ch_Sel_E    RxStatus_Ch_Sel     :  3;   // [04:02]  Rx 通道选择                         [0:不选择   1:选择 RxCH0    2:选择 RxCH1    3:同时选择 RxCH0/RxCH1]
        reserved_e          Reserved_0          :  3;   // [07:05]
        ON_OFF              TxStatus            :  2;   // [09:08]  Tx 通道状态                         [0:关闭 TxAntIndicate 所选的 Tx 通道    1:打开 TxAntIndicate 所选的 Tx 通道]
        Ecr8660_Ch_Sel_E    TxStatus_Ch_Sel     :  3;   // [12:10]  Tx 通道选择                         [0:不选择   1:选择 TxCH0    2:选择 TxCH1    3:同时选择 TxCH0/TxCH1]
        ON_OFF              Reserved_1          :  1;   // [13:13]
        ON_OFF              RxPLL_to_Tx_En      :  2;   // [15:14]  是否开启 Tx 使用 Rx 的 PLL {同频}   [0:TxCHX 使用 TxCHX 的 PLL[FDD]  1:TxCHX 使用 RxCHX 的 PLL[TDD]] (TxCH0 -> RxCH0, TxCH1 -> RxCH1)
        ON_OFF              Ext_BW_Mode_En      :  1;   // [16:16]  是否使用 外部带宽配置               [0:内部预置的 BandWidth   1:外部自定义的 Bandwidth]
        ON_OFF              Freque_Mode_En      :  1;   // [17:17]  是否使用 FREQUE 模式                [0:整/小数分频模式        1:FREQUE模式]
        reserved_e          Reserved_2          :  8;   // [25:18]
        reserved_e          Reserved_3          :  6;   // [31:26]
    }Bits;
}Mode_Status_U;

typedef union _band_bw_ant_u_{
    INT32U Word;
    struct{
        Ecr8660_BW_Idx_E    CH0_BW_Sel          :  4;   // [03:00]  用于指定 通道0 的带宽，当 Ext_BW_Mode_En == 0 时详情见枚举 Ecr8660_BW_Idx_E
        reserved_e          Reserved_0          :  4;   // [15:04]
        reserved_e          CH0_Band_Sel        :  8;   // [15:04]  在 Freque_Mode_En == 0 时使用，用于指示 通道0 的 LO 所在 Band
        Ecr8660_BW_Idx_E    CH1_BW_Sel          :  4;   // [19:16]  用于指定 通道1 的带宽，当 Ext_BW_Mode_En == 0 时详情见枚举 Ecr8660_BW_Idx_E
        reserved_e          Reserved_1          :  4;   // [15:04]
        reserved_e          CH1_Band_Sel        :  8;   // [31:20]  在 Freque_Mode_En == 0 时使用，用于指示 通道1 的 LO 所在 Band
    }Bits;
}Band_Bw_Ant_U;

// 设置分频系数或者 FREQUE
typedef union _rx_tx_freq_u_{
    INT64U FREQUE;          // 在 Freque_Mode_En == 1 时使用，单位为 KHz [eg:1950MHz 输入 1950000]
    struct{
        INT32U              Freq_Int;
        INT32U              Freq_Frac;
    }DIVs;
}Rx_Tx_Freq_U;

typedef union _rx_tx_gain_u_{
    INT32U Word;
    struct{
        INT32U              Gain_Value          :  8;   // [07:00] 增益等级(Rx 有 56 级 0 为最小/最大依次增大 1dB 一个 stap，Tx 有 35 级 0 为最小/最大依次增大 1dB 一个 stap)
        reserved_e          Reserved_0          :  7;   // [14:08]
        ON_OFF              Gain_EN             :  1;   // [15:15] 使能位，该为为一时增益等级起效
        ON_OFF              Gain_DIR            :  1;   // [16:16] 开启 Gain_Value 的表达方向转变   [0:Gain_Value = 0 表示最小增益  1:Gain_Value = 0 表示最大增益]
        reserved_e          Reserved_1          :  7;   // [23:17]
        reserved_e          Reserved_2          :  8;   // [31:14]
    }Bits;
}Rx_Tx_Gain_U;

typedef union _rx_tx_port_u_{
    INT32U Word;
    struct{
        reserved_e          Reserved_0          :  8;   // [07:00]
        reserved_e          Reserved_1          :  8;   // [15:08]
        Ecr8660_Port_Idx_E  TxCH1_Port          :  4;   // [19:16]
        Ecr8660_Port_Idx_E  TxCH0_Port          :  4;   // [23:20]
        Ecr8660_Port_Idx_E  RxCH1_Port          :  4;   // [27:24]
        Ecr8660_Port_Idx_E  RxCH0_Port          :  4;   // [31:28]
    }Bits;
}Rx_Tx_Port_U;

typedef struct _api_para_t{
    Mode_Status_U           Mode_Status_Ctrl;           // 32Bit
    Band_Bw_Ant_U           Band_Bw;                    // 32Bit
    Rx_Tx_Freq_U            Freq_RxCH0;                 // 64Bit
    Rx_Tx_Freq_U            Freq_RxCH1;                 // 64Bit
    Rx_Tx_Freq_U            Freq_TxCH0;                 // 64Bit
    Rx_Tx_Freq_U            Freq_TxCH1;                 // 64Bit
    Rx_Tx_Gain_U            Gain_RxCH0;                 // 32Bit
    Rx_Tx_Gain_U            Gain_RxCH1;                 // 32Bit
    Rx_Tx_Gain_U            Gain_TxCH0;                 // 32Bit
    Rx_Tx_Gain_U            Gain_TxCH1;                 // 32Bit
    Rx_Tx_Port_U            Port;                       // 32Bit
}Api_Para_T;

/****************************************************************************
 *  SPI 中断寄存器结构体
****************************************************************************/
typedef union _spi_interrupt_u_{
    INT32U Word;
    struct{
        Ecr8660_Cmd_E       Cmd_Sel             :  8;   // [07:00]
        INT32U              Reserved_0          :  8;   // [15:08]
        Ecr8660_Port_Idx_E  Parameter_3         :  4;   // [19:16]
        Ecr8660_Port_Idx_E  Parameter_2         :  4;   // [23:20]
        Ecr8660_Port_Idx_E  Parameter_1         :  4;   // [27:24]
        Ecr8660_Port_Idx_E  Parameter_0         :  4;   // [31:28]
    }Mode1_Bits;
    struct{
        Ecr8660_Cmd_E       Cmd_Sel             :  8;   // [07:00]
        INT32U              Reserved_0          :  8;   // [15:08]
        INT32U              Parameter_3         :  4;   // [19:16]
        INT32U              Parameter_2         :  4;   // [23:20]
        INT32U              Parameter_1         :  4;   // [27:24]
        INT32U              Parameter_0         :  4;   // [31:28]
    }ModeX_Bits;
}Spi_Interrupt_U;

/********************************************************************************
*                Macro Definition  eg:#define Macro Definition
********************************************************************************/
#define     ECR8660_API_MODE_STATUS_CTORL       (MCU_API_ADDR_HAND)
#define     ECR8660_API_BAND_BW                 (MCU_API_ADDR_HAND + 0x04)
#define     ECR8660_API_FREQ_RXCH0_INTER        (MCU_API_ADDR_HAND + 0x08)
#define     ECR8660_API_FREQ_RXCH0_FRAC         (MCU_API_ADDR_HAND + 0x0C)
#define     ECR8660_API_FREQ_RXCH1_INTER        (MCU_API_ADDR_HAND + 0x10)
#define     ECR8660_API_FREQ_RXCH1_FRAC         (MCU_API_ADDR_HAND + 0x14)
#define     ECR8660_API_FREQ_TXCH0_INTER        (MCU_API_ADDR_HAND + 0x18)
#define     ECR8660_API_FREQ_TXCH0_FRAC         (MCU_API_ADDR_HAND + 0x1C)
#define     ECR8660_API_FREQ_TXCH1_INTER        (MCU_API_ADDR_HAND + 0x20)
#define     ECR8660_API_FREQ_TXCH1_FRAC         (MCU_API_ADDR_HAND + 0x24)
#define     ECR8660_API_RXCH0_GAIN              (MCU_API_ADDR_HAND + 0x28)
#define     ECR8660_API_RXCH1_GAIN              (MCU_API_ADDR_HAND + 0x2C)
#define     ECR8660_API_TXCH0_GAIN              (MCU_API_ADDR_HAND + 0x30)
#define     ECR8660_API_TXCH1_GAIN              (MCU_API_ADDR_HAND + 0x34)

#define     ECR8660_SPI_INTERRUPT               (0x00201180)

#define     RX_MAX_GAIN_LEVEL                   (56)
#define     TX_MAX_GAIN_LEVEL                   (35)
#define     RX_MIN_GAIN_LEVEL                   (-4)
#define     TX_MIN_GAIN_LEVEL                   ( 1)

#define     MAX_GAIN_LEVEL(x)                   (  (   TXCH0 >  (x) )   ? RX_MAX_GAIN_LEVEL     \
                                                                        : TX_MAX_GAIN_LEVEL     )

#define     MIN_GAIN_LEVEL(x)                   (  (   TXCH0 >  (x) )   ? RX_MIN_GAIN_LEVEL     \
                                                                        : TX_MIN_GAIN_LEVEL     )

#define     PATH_IDX_2_LOG(x)                   (  ( ( RXCH0 == (x) )   ? "RXCH0"               \
                                                :  ( ( RXCH1 == (x) )   ? "RXCH1"               \
                                                :  ( ( TXCH0 == (x) )   ? "TXCH0"               \
                                                                        : "TXCH1"               ) ) ) )
/********************************************************************************
*           Function Definitions  eg:void Function_Definitions(void);
********************************************************************************/
Status_t ECR8660_Check_MCU_Work(Spi_Interrupt_U *pSpiInter);

Status_t ECR8660_Config_RxCH(Api_Para_T *pApi_Data, Ecr8660_Ch_Sel_E   CH_Sel, INT32U Freq_KHz,   Ecr8660_Int_Ext_BW_Idx_E BandWidth);
Status_t ECR8660_Config_TxCH(Api_Para_T *pApi_Data, Ecr8660_Ch_Sel_E   CH_Sel, INT32U Freq_KHz,   Ecr8660_Int_Ext_BW_Idx_E BandWidth);
Status_t ECR8660_Config_Gain(Api_Para_T *pApi_Data, Ecr8660_Path_Idx_E CH_Sel, INT8S  Gain_Level);
Status_t ECR8660_Config_Port(Api_Para_T *pApi_Data, Ecr8660_Path_Idx_E CH_Sel, Ecr8660_Port_Idx_E Port_Sel);
Status_t ECR8660_Config(Api_Para_T * pApi_Data);

Status_t ECR8660_Get_Versions(Ecr8660_Ver_U *Code_Ver);
Status_t ECR8660_Get_Gain(Ecr8660_Path_Idx_E CH_Sel, INT8S *pGain_Level);

/********************************************************************************
*             Global variable  eg:extern unsigned int Global_variable;
********************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* __ECR8660_CONFIG_H */
