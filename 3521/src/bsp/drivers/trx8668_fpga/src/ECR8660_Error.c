/********************************************************************************
 * @ Copyright    : Transa Semi.
 * @
 * @ File Name    : ECR8660_Error.c
 * @ File Mark    :
 * @ Author       : GuoHao
 * @ E-Mail       : 844396800@qq.com
 * @ Website      : www.GuoHowe.com
 * @ Version      : v1.0
 * @ Date         : 2021-04-19 09:36:23
 * @ Description  :
 * @ History:
 * @       Version      : v1.0
 * @       LastEditors  : GuoHao
 * @       LastEditTime : 2021-08-28 12:40:55
 * @       Modification :
********************************************************************************/

/********************************************************************************
*                  Include file  eg:#include "header_file.h"
********************************************************************************/
#include    "ECR8660.h"

/********************************************************************************
*             enum/struct/union/ Definition  eg:typedef struct{...}
********************************************************************************/

/********************************************************************************
*                Macro Definition  eg:#define Macro Definition
********************************************************************************/

/********************************************************************************
*        Global Function Prototypes  eg:void Function_Definitions(void);
********************************************************************************/

/********************************************************************************
*             Global variable  eg:unsigned int Global_variable;
********************************************************************************/

/********************************************************************************
 * @ Description : ECR8660_Check_MCU_Work_Log
 * @ Parameter   : pSpiInter 用于传递 Check 之前配置的 API 不同的 API 对应的 LOG 不同
 * @ Return      : [Status_t]
 *                      STAT_OK  : LOG 正确
 *                      STAT_ERROR    : LOG 错误
 * @ Note        : 用于 MCU 工作后的 LOG 状态检查
********************************************************************************/
Status_t ECR8660_Check_MCU_Work_Log(Spi_Interrupt_U *pSpiInter)
{
//    INT32U  Data;
//#if PRINT_LOG
//    Ecr8660_Log_Level_E Printf_Log_Temp;
//    DEBUG_LGE("%s Go...\r\n", __func__);
//    SET_PRINT_BACK(Printf_Log_Temp);
//    SET_PRINT_MIN(ERROR_LOG);
//#endif
//    Data =  ECR8660_Read(SPI_RW_EXTERNAL, MCU_LOG_ADDR_WORK, BYTE(4));
//    if(NULL == pSpiInter){
//        if(ECR8660_INIT_OVER != Data){
//#if PRINT_LOG
//            DEBUG_ERR("\r\nERR! ECR8660 Init Log Error Log(0x%08X)    [Check The (Delay / CodeDownload / Bin / Clk / Power )] ->[%s]\r\n\r\n", Data, __func__);
//            SET_PRINT_RECO(Printf_Log_Temp);
//#endif
//            return STAT_ERROR;
//        }
//    }else if(ECR8660_CMD_API == pSpiInter->ModeX_Bits.Cmd_Sel){
//        if(ECR8660_SPI_INTER_Normal != Data){
//#if PRINT_LOG
//            DEBUG_ERR("\r\nERR! ECR8660 Config Log Error Log(0x%08X)    [Check The (Delay / CodeDownload / Bin / Clk / Power )] ->[%s]\r\n\r\n", Data, __func__);
//            SET_PRINT_RECO(Printf_Log_Temp);
//#endif
//            return STAT_ERROR;
//        }
//    }else if(ECR8660_SPI_INTER_Debug != Data){
//#if PRINT_LOG
//        DEBUG_ERR("\r\nERR! ECR8660 Config Log Error Log(0x%08X)    [Check The (Delay / CodeDownload / Bin / Clk / Power )] ->[%s]\r\n\r\n", Data, __func__);
//        SET_PRINT_RECO(Printf_Log_Temp);
//#endif
//        return STAT_ERROR;
//    }
//#if PRINT_LOG
//    SET_PRINT_RECO(Printf_Log_Temp);
//    DEBUG_LGE("%s OK\r\n", __func__);
//#endif
//    return STAT_OK;
    return STAT_OK;
}

/********************************************************************************
 * @ Description : ECR8660_Check_Error_Log
 * @ Parameter   : None
 * @ Return      : [Status_t]
 *                      STAT_OK     : LOG 无错误
 *                      STAT_ERROR       : LOG 有错误
 * @ Note        : 用于在异常状况下检查 MCU 内置的错误码
********************************************************************************/
Status_t ECR8660_Check_Error_Log(void)
{
//    INT32U  Data;
//    Ecr8660_Err_E   ErrID;
//    CHAR    ECR8660_Error_log_Explain[ECR8660_ERR_END][128] = {
//        {"no"                                                   },
//        {"非法参数"                                                 },
//        {"参数为空"                                                 },
//        {"通道选择出错"                                             },
//        {"VCO的频率校准超时"                                       },
//        {"ADC的RC校准超时"                                       },
//        {"VCO中隊 REF_CLK 的分配出错"                              },
//        {"传参的PathIdx 过大"                                      },
//        {"传参的PathIdx不适用于当前函数"                          },
//        {"传参的VcoIdx过大"                                      },
//        {"传参VcoIdx不适用于当前函数"                          },
//        {"通道选择导致的 RBDP 配置错误"                             },
//        {"激活但未配置通道"                                         },
//        {"使用校准值时 RC 校准的校准值为零"                         },
//        {"在配置 FIR 时其工作始终出现错误"                          },
//        {"增益参数超限"                                             },
//        {"软件版本和 EFUSE 版本不匹配"                              },
//        {"EFUSE 错误: 于 INIT 阶段 (芯片异常)"                      },
//        {"EFUSE 错误: 于 INIT_END 阶段 (芯片异常)"                  },
//        {"EFUSE 错误: Efuse 校验失败 (芯片异常)"                    },
//        {"EFUSE 错误: im2 校准值异常 (性能不保证)"                  },
//        {"EFUSE 错误: efuse_get_band 没找到对应 Band (性能不保证)"  },
//        {"EFUSE 错误: VCO 阶段 (保留)"                              },
//        {"LO 配置超出版本权限 (配置超出芯片权限)"                   },
//        {"CH 配置超出版本权限 (配置超出芯片权限)"                   },
//        {"BW 配置超出版本权限 (配置超出芯片权限)"                   },
//        {"CA 配置超出版本权限 (配置超出芯片权限)"                   },
//        {"断错误: 在 MCU 工作期间 SPI 对内存进行了读操作"           },
//        {"LO 分频配置错误 (检查 LO 配置的频点)"                     },
//        {"Rx_DCOC 校准阶段PLL锁定判断超时 (怀疑失所，但不准确)"     },
//    };
#if 0 //PRINT_LOG
    Ecr8660_Log_Level_E Printf_Log_Temp;
    SET_PRINT_BACK(Printf_Log_Temp);
    SET_PRINT_MIN(ENGLISH_LOG);
    Data    = ECR8660_Read(SPI_RW_EXTERNAL, MCU_VER_ADDR_ERROR0, BYTE(4));
    ErrID   = (Ecr8660_Err_E)GET_VAL_BIT(Data, 0, LEN_08_BIT);
    if(ECR8660_ERR_RXDCOC_PLL_TIMEOUT < ErrID){
        DEBUG_ERR("\r\nERR! Error ID Error\r\n\r\n");
        SET_PRINT_RECO(Printf_Log_Temp);
        return STAT_ERROR;
    }

    switch(ErrID){
    case ECR8660_ERR_NONE:
        DEBUG_ERR("No [ErrID] Error\r\n");
        SET_PRINT_RECO(Printf_Log_Temp);
        return STAT_OK;
    case ECR8660_ERR_ILLEGAL_PARAMETER:
    case ECR8660_ERR_PATHIDX:
    case ECR8660_ERR_RBDP:
    case ECR8660_ERR_FIRCLK:
        DEBUG_LGE("MCU_LOG_ADDR_LIST(2) = 0x%08X\r\n", ECR8660_Read(SPI_RW_EXTERNAL, MCU_LOG_ADDR_LIST(2), BYTE(4)));
        break;
    case ECR8660_ERR_EFUSE_LOG_MISMATCH:
        DEBUG_LGE("MCU_LOG_ADDR_LIST(2) = 0x%08X\r\n", ECR8660_Read(SPI_RW_EXTERNAL, MCU_LOG_ADDR_LIST(2), BYTE(4)));
        DEBUG_LGE("MCU_LOG_ADDR_LIST(3) = 0x%08X\r\n", ECR8660_Read(SPI_RW_EXTERNAL, MCU_LOG_ADDR_LIST(3), BYTE(4)));
        break;
    case ECR8660_ERR_RXDCOC_PLL_TIMEOUT:
        DEBUG_LGE("MCU_LOG_ADDR_LIST(4) = 0x%08X\r\n", ECR8660_Read(SPI_RW_EXTERNAL, MCU_LOG_ADDR_LIST(4), BYTE(4)));
        DEBUG_LGE("MCU_LOG_ADDR_LIST(5) = 0x%08X\r\n", ECR8660_Read(SPI_RW_EXTERNAL, MCU_LOG_ADDR_LIST(5), BYTE(4)));
        DEBUG_LGE("MCU_LOG_ADDR_LIST(6) = 0x%08X\r\n", ECR8660_Read(SPI_RW_EXTERNAL, MCU_LOG_ADDR_LIST(6), BYTE(4)));
        break;
    default:
        break;
    }
    DEBUG_LGE("MCU_VER_ADDR_ERROR0  = 0x%08X\r\n", ECR8660_Read(SPI_RW_EXTERNAL, MCU_VER_ADDR_ERROR0, BYTE(4)));
    DEBUG_LGE("MCU_VER_ADDR_ERROR1  = 0x%08X\r\n", ECR8660_Read(SPI_RW_EXTERNAL, MCU_VER_ADDR_ERROR1, BYTE(4)));
    DEBUG_ERR("\r\nERR! %s\r\n\r\n", ECR8660_Error_log_Explain[ErrID]);
    SET_PRINT_RECO(Printf_Log_Temp);
#endif
    return STAT_ERROR;
}

/********************************************************************************
 * @ Description : ECR8660_Check_log_List
 * @ Parameter   : None
 * @ Return      : None
 * @ Note        : 用于读取 MCU 内的 LOG [需要优先级 DBG_LEVEL_MAX 大于 PRINTF_LOG]
********************************************************************************/
void    ECR8660_Check_log_List(void)
{
//    INT32U  Data;
//    Ecr8660_Ver_U   Code_Ver;
//    CHAR    Temp[16] = {0};

//#if PRINT_LOG
//    Ecr8660_Log_Level_E Printf_Log_Temp;
//    SET_PRINT_BACK(Printf_Log_Temp);
//    SET_PRINT_MIN(CHINESE_LOG);

//    Code_Ver.Word = ECR8660_Read(SPI_RW_EXTERNAL, MCU_VER_ADDR_VER0, BYTE(4));
//    DEBUG_LGE("Firmware_ID:\r\n    V%d.%d.%d\r\n", Code_Ver.Bits.major, Code_Ver.Bits.middl, Code_Ver.Bits.minor);

//    Data = ECR8660_Read(SPI_RW_EXTERNAL, MCU_CFG_ADDR_REF , BYTE(4));
//    DEBUG_LGE("Firmware_Ref:\r\n    %dKHz\r\n", Data);

//    Data = ECR8660_Read(SPI_RW_EXTERNAL, MCU_EFU_ADDR_ID0 , BYTE(4));
//    (*(INT32U *)&Temp[0])   = Data;
//    Data = ECR8660_Read(SPI_RW_EXTERNAL, MCU_EFU_ADDR_ID1 , BYTE(4));
//    (*(INT32U *)&Temp[4])   = Data;
//    DEBUG_LGE("Chip_ID:\r\n    %s\r\n" , Temp);

//    DEBUG_LGE("MCU_LOG_ADDR_LIST(0)\r\n    0x%08X\r\n", ECR8660_Read(SPI_RW_EXTERNAL, MCU_LOG_ADDR_LIST(0), BYTE(4)));
//    DEBUG_LGE("MCU_LOG_ADDR_LIST(1)\r\n    0x%08X\r\n", ECR8660_Read(SPI_RW_EXTERNAL, MCU_LOG_ADDR_LIST(1), BYTE(4)));
//    DEBUG_LGE("MCU_VER_ADDR_ERROR0\r\n    0x%08X\r\n" , ECR8660_Read(SPI_RW_EXTERNAL, MCU_VER_ADDR_ERROR0 , BYTE(4)));

//    SET_PRINT_RECO(Printf_Log_Temp);
//#endif
}
