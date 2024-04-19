/*********************************************************************************
 * @ Copyright    : Transa Semi.
 * @
 * @ File Name    : ECR8660_Error.h
 * @ File Mark    :
 * @ Author       : GuoHao
 * @ E-Mail       : 844396800@qq.com
 * @ Website      : www.GuoHowe.com
 * @ Version      : v1.0
 * @ Date         : 2021-04-19 09:35:59
 * @ Description  :
 * @ History:
 * @       Version      : v1.0
 * @       LastEditors  : GuoHao
 * @       LastEditTime : 2021-08-27 09:58:57
 * @       Modification :
********************************************************************************/

#ifndef __ECR8660_ERROR_H
#define __ECR8660_ERROR_H

#ifdef __cplusplus
extern "C" {
#endif
/********************************************************************************
*                  Include file  eg:#include "header_file.h"
********************************************************************************/
#include    "Bit_macro.h"

/********************************************************************************
*             enum/struct/union/ Definition  eg:typedef struct{...}
********************************************************************************/

typedef enum _ecr8660_err_e_{
    ECR8660_ERR_NONE                        ,   // 无错误
    ECR8660_ERR_ILLEGAL_PARAMETER           ,   // 非法参数
    ECR8660_ERR_NULL_PARAMETER              ,   // 参数为空
    ECR8660_ERR_ANTINDICATE                 ,   // 通道选择出错
    ECR8660_ERR_VCO_TIMEOUT                 ,   // VCO 的频率校准超时
    ECR8660_ERR_ADCRCCAL_TIMEOUT            ,   // ADC 的 RC 校准超时
    ECR8660_ERR_VCO_REF_DIV                 ,   // VCO 中隊 REF_CLK 的分配出错
    ECR8660_ERR_PATHIDX                     ,   // 传参的 PathIdx 过大
    ECR8660_ERR_PATHIDX_1                   ,   // 传参的 PathIdx 不适用于当前函数
    ECR8660_ERR_VCOIDX                      ,   // 传参的 VcoIdx  过大
    ECR8660_ERR_VCOIDX_1                    ,   // 传参的 VcoIdx  不适用于当前函数
    ECR8660_ERR_RBDP                        ,   // 通道选择导致的 RBDP 配置错误
    ECR8660_ERR_NO_ANT_NEED                 ,   // 激活但未配置通道
    ECR8660_ERR_RCCAL_VALUE                 ,   // 使用校准值时 RC 校准的校准值为零
    ECR8660_ERR_FIRCLK                      ,   // 在配置 FIR 时其工作始终出现错误
    ECR8660_ERR_GAIN_AN_ARRAY               ,   // 增益参数超限
    ECR8660_ERR_EFUSE_NO_SFTWV              ,   // 软件版本和 EFUSE 版本不匹配
    ECR8660_ERR_EFUSE_LOG_INIT              ,   // EFUSE 错误存在于 INIT 阶段
    ECR8660_ERR_EFUSE_LOG_INIT_END          ,   // EFUSE 错误存在于 INIT_END 阶段
    ECR8660_ERR_EFUSE_LOG_ILLEGAL           ,   // EFUSE 错误存在于 ILLEGAL 阶段    (Efuse 校验失败)
    ECR8660_ERR_EFUSE_LOG_INVALID           ,   // EFUSE 错误存在于 INVALID 阶段    (im2 校准值异常)
    ECR8660_ERR_EFUSE_LOG_MISMATCH          ,   // EFUSE 错误存在于 MISMATCH 阶段   (efuse_get_band 没找到对应 Band)
    ECR8660_ERR_EFUSE_LOG_VCO               ,   // EFUSE 错误存在于 VCO 阶段
    ECR8660_ERR_BEYOND_AUTHORITY_LO         ,   // LO 配置超出版本权限
    ECR8660_ERR_BEYOND_AUTHORITY_CH         ,   // CH 配置超出版本权限
    ECR8660_ERR_BEYOND_AUTHORITY_BW         ,   // BW 配置超出版本权限
    ECR8660_ERR_BEYOND_AUTHORITY_CA         ,   // CA 配置超出版本权限
    ECR8660_ERR_HARDFAULT_HANDLER           ,   // 断错误（1、在 MCU 工作期间 SPI 对内存进行了读操作）
    ECR8660_ERR_LO_DIV_SEL                  ,   // LO 分频配置错误
    ECR8660_ERR_RXDCOC_PLL_TIMEOUT          ,   // Rx_DCOC 校准阶段PLL锁定判断超时
    ECR8660_ERR_END                         ,
}Ecr8660_Err_E;

typedef enum _ecr8660_mark_e_{
    ECR8660_MAIN                = 0x00000000,   //
    ECR8660_INIT_VCO            = 0x55AA55AA,   // 初始化执行到初始化 VCO 前的 LOG
    ECR8660_INIT_CALIBRATION    = 0x00000005,   // 初始化执行到校准前的 LOG
    ECR8660_INIT_OVER           = 0x00000123,   // 初始化完成的 LOG
    ECR8660_SPI_INTER_IN        = 0x000000CC,   // MCU 该开始进入配置工作时的 LOG
    ECR8660_SPI_INTER_log00     = 0x000000CD,   // 在 PLL_Lock_Detect 前的 LOG
    ECR8660_SPI_INTER_Normal    = 0x000000DD,   // ECR8660_CMD_API 执行完成的 LOG
    ECR8660_SPI_INTER_log01     = 0x000000CE,   // 保留
    ECR8660_SPI_INTER_Debug     = 0x000000EE,   // 非 ECR8660_CMD_API 模式执行完成的 LOG
}Ecr8660_Mark_E;
/********************************************************************************
*                Macro Definition  eg:#define Macro Definition
********************************************************************************/

/********************************************************************************
*           Function Definitions  eg:void Function_Definitions(void);
********************************************************************************/
Status_t ECR8660_Check_MCU_Work_Log(Spi_Interrupt_U *pSpiInter);
Status_t ECR8660_Check_Error_Log(void);
void        ECR8660_Check_log_List(void);

/********************************************************************************
*             Global variable  eg:extern unsigned int Global_variable;
********************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* __ECR8660_ERROR_H */
