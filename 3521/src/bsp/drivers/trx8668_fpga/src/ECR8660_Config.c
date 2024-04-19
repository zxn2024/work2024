/********************************************************************************
 * @ Copyright    : Transa Semi.
 * @
 * @ File Name    : ECR8660_Config.c
 * @ File Mark    : 
 * @ Author       : GuoHao
 * @ E-Mail       : 844396800@qq.com
 * @ Website      : www.GuoHowe.com
 * @ Version      : v1.0
 * @ Date         : 2021-04-19 09:18:02
 * @ Description  : 
 * @ History: 
 * @       Version      : v1.0
 * @       LastEditors  : GuoHao
 * @       LastEditTime : 2021-08-28 12:44:14
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
 * @ Description : ECR8660_Check_MCU_Work
 * @ Parameter   : pSpiInter   : 用于传递 Check 之前配置的 API 不同的 API 使用 方案1 时
 *                 Delay 的时间不同 
 * @ Return      : [Status_t]
 *                      STAT_OK      : 工作完成
 *                      STAT_ERROR        : 检测超时
 * @ Note        : 方案0：检查 ECR8660 的 MCU 是否在工作 [CTRL_OUT7 出现下降沿即为工作结束]
 *                 方案1：使用 User_Delay_ms 等待结束
********************************************************************************/
Status_t ECR8660_Check_MCU_Work(Spi_Interrupt_U *pSpiInter)
{
//#if PRINT_LOG
//    Ecr8660_Log_Level_E Printf_Log_Temp;
//    SET_PRINT_BACK(Printf_Log_Temp);
//    SET_PRINT_MIN(Printf_Log_Temp);
//    DEBUG_LGE("%s Go...\r\n", __func__);
//#endif

//#if CHECK_MCU_WORK_MODE
//    if(NULL == pSpiInter){
//        rt_thread_mdelay(200);
//    }else if(ECR8660_CMD_API == pSpiInter->ModeX_Bits.Cmd_Sel){
//        rt_thread_mdelay(50);
//    }else{
//        rt_thread_mdelay(10);
//    }
//#else
//    if(STAT_ERROR == User_Check_MCU_Work(pSpiInter)){
//#if PRINT_LOG
//        DEBUG_ERR("\r\nERR! Check_MCU_Work Overtime    [Check The (Bin / CodeDownload / Clk / Power )] ->[%s]\r\n\r\n", __func__);
//        SET_PRINT_RECO(Printf_Log_Temp);
//#endif
//        return STAT_ERROR;
//    }
//#endif
//#if PRINT_LOG
//    DEBUG_LGE("%s OK\r\n", __func__);
//    SET_PRINT_RECO(Printf_Log_Temp);
//#endif
    return STAT_OK;
}

/********************************************************************************
 * @ Description : ECR8660_Config_RxCH
 * @ Parameter   : pApi_Data    : 存放配置的结构体
 *                 CH_Sel       : 此次配置的通道选择
 *                      CHOOSE_CH0  : 指示此次配置的通道为 通道0
 *                      CHOOSE_CH1  : 指示此次配置的通道为 通道1
 *                      CHOOSE_CH01 : 指示此次配置的通道为 通道0 与 通道1
 *                 Freq_KHz     : 配置的通道 LO 频率
 *                 BandWidth    : 配置的通道带宽
 * @ Return      : [Status_t]
 *                      STAT_OK      : 配置成功
 *                      STAT_ERROR        : 配置失败
 * @ Note        : 对 ECR8660 的 Api_Data 参数进行 Rx 通道参数设置
 *                 需要使用 ECR8660_Config 函数使能配置
********************************************************************************/
Status_t ECR8660_Config_RxCH(Api_Para_T *pApi_Data, Ecr8660_Ch_Sel_E CH_Sel, INT32U Freq_KHz, Ecr8660_Int_Ext_BW_Idx_E BandWidth)
{
//    if(GHZ(6) < Freq_KHz){
//#if PRINT_LOG
//        DEBUG_ERR("\r\nERR! ECR8660 Config Tx Error    [Check The (Parameter: {INT32U} Freq_KHz] ->[%s]\r\n\r\n", __func__);
//#endif
//        return STAT_ERROR;
//    }
//    switch(CH_Sel){
//    case NOT_CHOOSE:
//#if PRINT_LOG
//        DEBUG_ERR("\r\nERR! ECR8660 Config Rx Error    [Check The (Parameter: {Ecr8660_Ch_Sel_E} CH_Sel)] ->[%s]\r\n\r\n", __func__);
//#endif
//        return STAT_ERROR;
//    case CHOOSE_CH0:
//        pApi_Data->Freq_RxCH0.FREQUE        =   Freq_KHz;
//        pApi_Data->Band_Bw.Bits.CH0_BW_Sel  =   (Ecr8660_BW_Idx_E)((LTE100 < BandWidth) ? (BandWidth - EXT_BW0) : BandWidth);
//        break;
//    case CHOOSE_CH1:
//        pApi_Data->Freq_RxCH1.FREQUE        =   Freq_KHz;
//        pApi_Data->Band_Bw.Bits.CH1_BW_Sel  =   (Ecr8660_BW_Idx_E)((LTE100 < BandWidth) ? (BandWidth - EXT_BW0) : BandWidth);
//        break;
//    case CHOOSE_CH01:
//        pApi_Data->Freq_RxCH0.FREQUE        =   Freq_KHz;
//        pApi_Data->Band_Bw.Bits.CH0_BW_Sel  =   (Ecr8660_BW_Idx_E)((LTE100 < BandWidth) ? (BandWidth - EXT_BW0) : BandWidth);
//        pApi_Data->Freq_RxCH1.FREQUE        =   Freq_KHz;
//        pApi_Data->Band_Bw.Bits.CH1_BW_Sel  =   (Ecr8660_BW_Idx_E)((LTE100 < BandWidth) ? (BandWidth - EXT_BW0) : BandWidth);
//        break;
//    }
//    if(on == pApi_Data->Mode_Status_Ctrl.Bits.TxStatus){
//        if((pApi_Data->Freq_TxCH0.FREQUE == Freq_KHz) || (pApi_Data->Freq_TxCH1.FREQUE == Freq_KHz))
//            pApi_Data->Mode_Status_Ctrl.Bits.RxPLL_to_Tx_En = on;
//    }
//    pApi_Data->Mode_Status_Ctrl.Bits.RxStatus           =   on;
//    pApi_Data->Mode_Status_Ctrl.Bits.Freque_Mode_En     =   on;
//    pApi_Data->Mode_Status_Ctrl.Bits.RxStatus_Ch_Sel    =   CH_Sel;
//    pApi_Data->Mode_Status_Ctrl.Bits.Ext_BW_Mode_En     =   (LTE100 < BandWidth) ? on : off;
    return STAT_OK;
}

/********************************************************************************
 * @ Description : ECR8660_Config_TxCH
 * @ Parameter   : pApi_Data    : 存放配置的结构体
 *                 CH_Sel       : 此次配置的通道选择
 *                      CHOOSE_CH0  : 指示此次配置的通道为 通道0
 *                      CHOOSE_CH1  : 指示此次配置的通道为 通道1
 *                      CHOOSE_CH01 : 指示此次配置的通道为 通道0 与 通道1
 *                 Freq_KHz     : 配置的通道 LO 频率( < 6000000)
 *                 BandWidth    : 配置的通道带宽
 * @ Return      : [Status_t]
 *                      STAT_OK      : 配置成功
 *                      STAT_ERROR        : 配置失败
 * @ Note        : 对 ECR8660 的 Api_Data 参数进行 Tx 通道参数设置
 *                 需要使用 ECR8660_Config 函数使能配置
********************************************************************************/
Status_t ECR8660_Config_TxCH(Api_Para_T *pApi_Data, Ecr8660_Ch_Sel_E CH_Sel, INT32U Freq_KHz, Ecr8660_Int_Ext_BW_Idx_E BandWidth)
{
//    if(GHZ(6) < Freq_KHz){
//#if PRINT_LOG
//        DEBUG_ERR("\r\nERR! ECR8660 Config Tx Error    [Check The (Parameter: {INT32U} Freq_KHz] ->[%s]\r\n\r\n", __func__);
//#endif
//        return STAT_ERROR;
//    }
//    switch(CH_Sel){
//    case NOT_CHOOSE:
//#if PRINT_LOG
//        DEBUG_ERR("\r\nERR! ECR8660 Config Tx Error    [Check The (Parameter: {Ecr8660_Ch_Sel_E} CH_Sel)] ->[%s]\r\n\r\n", __func__);
//#endif
//        return STAT_ERROR;
//    case CHOOSE_CH0:
//        pApi_Data->Freq_TxCH0.FREQUE        =   Freq_KHz;
//        pApi_Data->Band_Bw.Bits.CH0_BW_Sel  =   (Ecr8660_BW_Idx_E)((LTE100 < BandWidth) ? (BandWidth - EXT_BW0) : BandWidth);
//        break;
//    case CHOOSE_CH1:
//        pApi_Data->Freq_TxCH1.FREQUE        =   Freq_KHz;
//        pApi_Data->Band_Bw.Bits.CH1_BW_Sel  =   (Ecr8660_BW_Idx_E)((LTE100 < BandWidth) ? (BandWidth - EXT_BW0) : BandWidth);
//        break;
//    case CHOOSE_CH01:
//        pApi_Data->Freq_TxCH0.FREQUE        =   Freq_KHz;
//        pApi_Data->Band_Bw.Bits.CH0_BW_Sel  =   (Ecr8660_BW_Idx_E)((LTE100 < BandWidth) ? (BandWidth - EXT_BW0) : BandWidth);
//        pApi_Data->Freq_TxCH1.FREQUE        =   Freq_KHz;
//        pApi_Data->Band_Bw.Bits.CH1_BW_Sel  =   (Ecr8660_BW_Idx_E)((LTE100 < BandWidth) ? (BandWidth - EXT_BW0) : BandWidth);
//        break;
//    }
//    if(on == pApi_Data->Mode_Status_Ctrl.Bits.RxStatus){
//        if((pApi_Data->Freq_RxCH0.FREQUE == Freq_KHz) || (pApi_Data->Freq_RxCH1.FREQUE == Freq_KHz))
//            pApi_Data->Mode_Status_Ctrl.Bits.RxPLL_to_Tx_En = on;
//    }

//    pApi_Data->Mode_Status_Ctrl.Bits.TxStatus           =   on;
//    pApi_Data->Mode_Status_Ctrl.Bits.Freque_Mode_En     =   on;
//    pApi_Data->Mode_Status_Ctrl.Bits.TxStatus_Ch_Sel    =   CH_Sel;
//    pApi_Data->Mode_Status_Ctrl.Bits.Ext_BW_Mode_En     =   (LTE100 < BandWidth) ? on : off;
    return STAT_OK;
}

/********************************************************************************
 * @ Description : ECR8660_Config_Gain
 * @ Parameter   : pApi_Data    : 存放配置的结构体
 *                 CH_Sel       : 此次配置的通道选择
 *                      RXCH0       : 指示此次配置的通道为 RXCH0
 *                      RXCH1       : 指示此次配置的通道为 RXCH1
 *                      TXCH0       : 指示此次配置的通道为 TXCH0
 *                      TXCH1       : 指示此次配置的通道为 TXCH1
 *                 Gain_Level   : 增益等级的设置
 *                      Rx: -4 <= Gain_Level <= 56
 *                      Tx:  1 <= Gain_Level <= 35
 * @ Return      : [Status_t]
 *                      STAT_OK      : 配置成功
 *                      STAT_ERROR        : 配置失败
 * @ Note        : 对 ECR8660 的 Api_Data 参数进行增益配置
 *                 需要使用 ECR8660_Config 函数使能配置
********************************************************************************/
Status_t ECR8660_Config_Gain(Api_Para_T *pApi_Data, Ecr8660_Path_Idx_E CH_Sel, INT8S Gain_Level)
{

//    if((MAX_GAIN_LEVEL(CH_Sel) < Gain_Level) || (MIN_GAIN_LEVEL(CH_Sel) > Gain_Level)){
//#if PRINT_LOG
//        DEBUG_ERR("\r\nERR! ECR8660 Config %s Gain Error    [Check The (Parameter: {INT8U} Gain_Level [%s: %d <= Gain_Level <= %d])] ->[%s]\r\n\r\n", PATH_IDX_2_LOG(CH_Sel), PATH_IDX_2_LOG(CH_Sel), MIN_GAIN_LEVEL(CH_Sel), MAX_GAIN_LEVEL(CH_Sel), __func__);
//#endif
//        return STAT_ERROR;
//    }

//    switch(CH_Sel){
//    case RXCH0:
//        pApi_Data->Gain_RxCH0.Bits.Gain_DIR     = off;
//        pApi_Data->Gain_RxCH0.Bits.Gain_EN      = on;
//        pApi_Data->Gain_RxCH0.Bits.Gain_Value   = Gain_Level;
//        break;
//    case RXCH1:
//        pApi_Data->Gain_RxCH1.Bits.Gain_DIR     = off;
//        pApi_Data->Gain_RxCH1.Bits.Gain_EN      = on;
//        pApi_Data->Gain_RxCH1.Bits.Gain_Value   = Gain_Level;
//        break;
//    case TXCH0:
//        pApi_Data->Gain_TxCH0.Bits.Gain_DIR     = off;
//        pApi_Data->Gain_TxCH0.Bits.Gain_EN      = on;
//        pApi_Data->Gain_TxCH0.Bits.Gain_Value   = Gain_Level;
//        break;
//    case TXCH1:
//        pApi_Data->Gain_TxCH1.Bits.Gain_DIR     = off;
//        pApi_Data->Gain_TxCH1.Bits.Gain_EN      = on;
//        pApi_Data->Gain_TxCH1.Bits.Gain_Value   = Gain_Level;
//        break;
//    }
    return STAT_OK;
}

/********************************************************************************
 * @ Description : ECR8660_Config_Port
 * @ Parameter   : pApi_Data    : 存放配置的结构体
 *                 CH_Sel       : 此次配置的通道选择
 *                      RXCH0       : 指示此次配置的通道为 RXCH0
 *                      RXCH1       : 指示此次配置的通道为 RXCH1
 *                      TXCH0       : 指示此次配置的通道为 TXCH0
 *                      TXCH1       : 指示此次配置的通道为 TXCH1
 *                 Port_Sel     : 指定 CH_Sel 的 Port 口
 *                      Rx: PORT_A \ PORT_B \ PORT_C
 *                      Tx: PORT_A \ PORT_B
 * @ Return      : [Status_t]
 *                      STAT_OK      : 配置成功
 *                      STAT_ERROR        : 配置失败
 * @ Note        : 对 ECR8660 的 Api_Data 参数进行 Port 口配置
 *                 需要使用 ECR8660_Config 函数使能配置
********************************************************************************/
Status_t ECR8660_Config_Port(Api_Para_T *pApi_Data, Ecr8660_Path_Idx_E CH_Sel, Ecr8660_Port_Idx_E Port_Sel)
{
//    if((GET_VAL_BIT(CH_Sel, 1, LEN_01_BIT)) && (PORT_B < Port_Sel)){
//#if PRINT_LOG
//        DEBUG_ERR("\r\nERR! ECR8660 Config TxCHX Port Error    [Check The (Parameter: {Ecr8660_Port_Idx_E} Port_Sel [Tx: PORT_A \\ PORT_B] ->[%s]\r\n\r\n", __func__);
//#endif
//        return STAT_ERROR;
//    }
//    switch(CH_Sel){
//    case RXCH0:
//        pApi_Data->Port.Bits.RxCH0_Port     =   Port_Sel;
//        break;
//    case RXCH1:
//        pApi_Data->Port.Bits.RxCH1_Port     =   Port_Sel;
//        break;
//    case TXCH0:
//        pApi_Data->Port.Bits.TxCH0_Port     =   Port_Sel;
//        break;
//    case TXCH1:
//        pApi_Data->Port.Bits.TxCH1_Port     =   Port_Sel;
//        break;
//    }
    return STAT_OK;
}

/********************************************************************************
 * @ Description : ECR8660_Config
 * @ Parameter   : pApi_Data    : 存放配置的结构体
 * @ Return      : [Status_t]
 *                      STAT_OK      : 配置成功
 *                      STAT_ERROR        : 配置失败
 * @ Note        : 将 ECR8660 的 API 参数进行 SPI 命令下发，完成配置时会清除 pApi_Data
********************************************************************************/
//Status_t ECR8660_Config(Api_Para_T * pApi_Data)
//{
//    Spi_Interrupt_U SpiInter = {0};
//#if PRINT_LOG
//    DEBUG_LGE("%s Go...\r\n", __func__);
//#endif
//    ECR8660_write(SPI_RW_EXTERNAL,  ECR8660_API_MODE_STATUS_CTORL   , pApi_Data->Mode_Status_Ctrl.Word      );      // 0x20004000
//    ECR8660_write(SPI_RW_EXTERNAL,  ECR8660_API_BAND_BW             , (*(INT32U *)&pApi_Data->Band_Bw)      );      // 0x20004004
//    ECR8660_write(SPI_RW_EXTERNAL,  ECR8660_API_FREQ_RXCH0_INTER    , pApi_Data->Freq_RxCH0.DIVs.Freq_Int   );      // 0x20004008
//    ECR8660_write(SPI_RW_EXTERNAL,  ECR8660_API_FREQ_RXCH0_FRAC     , pApi_Data->Freq_RxCH0.DIVs.Freq_Frac  );      // 0x2000400C
//    ECR8660_write(SPI_RW_EXTERNAL,  ECR8660_API_FREQ_RXCH1_INTER    , pApi_Data->Freq_RxCH1.DIVs.Freq_Int   );      // 0x20004010
//    ECR8660_write(SPI_RW_EXTERNAL,  ECR8660_API_FREQ_RXCH1_FRAC     , pApi_Data->Freq_RxCH1.DIVs.Freq_Frac  );      // 0x20004014
//    ECR8660_write(SPI_RW_EXTERNAL,  ECR8660_API_FREQ_TXCH0_INTER    , pApi_Data->Freq_TxCH0.DIVs.Freq_Int   );      // 0x20004018
//    ECR8660_write(SPI_RW_EXTERNAL,  ECR8660_API_FREQ_TXCH0_FRAC     , pApi_Data->Freq_TxCH0.DIVs.Freq_Frac  );      // 0x2000401C
//    ECR8660_write(SPI_RW_EXTERNAL,  ECR8660_API_FREQ_TXCH1_INTER    , pApi_Data->Freq_TxCH1.DIVs.Freq_Int   );      // 0x20004020
//    ECR8660_write(SPI_RW_EXTERNAL,  ECR8660_API_FREQ_TXCH1_FRAC     , pApi_Data->Freq_TxCH1.DIVs.Freq_Frac  );      // 0x20004024
//    ECR8660_write(SPI_RW_EXTERNAL,  ECR8660_API_RXCH0_GAIN          , pApi_Data->Gain_RxCH0.Word            );      // 0x20004028
//    ECR8660_write(SPI_RW_EXTERNAL,  ECR8660_API_RXCH1_GAIN          , pApi_Data->Gain_RxCH1.Word            );      // 0x2000402C
//    ECR8660_write(SPI_RW_EXTERNAL,  ECR8660_API_TXCH0_GAIN          , pApi_Data->Gain_TxCH0.Word            );      // 0x20004030
//    ECR8660_write(SPI_RW_EXTERNAL,  ECR8660_API_TXCH1_GAIN          , pApi_Data->Gain_TxCH1.Word            );      // 0x20004034

//    SpiInter.Word                =  pApi_Data->Port.Word;
//    SpiInter.Mode1_Bits.Cmd_Sel  =  ECR8660_CMD_API;
//    ECR8660_write(SPI_RW_EXTERNAL, ECR8660_SPI_INTERRUPT            , SpiInter.Word                         );      // 0x00201180

//    if(STAT_ERROR == ECR8660_Check_MCU_Work(&SpiInter))                  return STAT_ERROR;
//    if(STAT_ERROR == ECR8660_Check_MCU_Work_Log(&SpiInter))              return STAT_ERROR;

//    memset(pApi_Data, rese_zero, sizeof(Api_Para_T));
//#if PRINT_LOG
//    DEBUG_LGE("%s OK\r\n", __func__);
//#endif
//    return STAT_OK;
//}

/********************************************************************************
 * @ Description : ECR8660_Get_Versions
 * @ Parameter   : *pCode_Ver   : 存放 MCU 版本号的结构体地址
 * @ Return      : [Status_t]
 *                      STAT_OK      : 获取成功
 *                      STAT_ERROR        : 获取失败
 * @ Note        : 获取 ECR8660 指定通道的 增益
********************************************************************************/
Status_t ECR8660_Get_Versions(Ecr8660_Ver_U *pCode_Ver)
{
//#if PRINT_LOG
//    DEBUG_LGE("%s Go...\r\n", __func__);
//#endif
//    pCode_Ver->Word = ECR8660_Read(SPI_RW_EXTERNAL, MCU_VER_ADDR_VER0, BYTE(4));
//#if PRINT_LOG
//    DEBUG_LGE("Firmware_ID:\r\n    V%d.%d.%d\r\n", pCode_Ver->Bits.major, pCode_Ver->Bits.middl, pCode_Ver->Bits.minor);
//    DEBUG_LGE("%s OK\r\n", __func__);
//#endif
    return STAT_OK;
}

/********************************************************************************
 * @ Description : ECR8660_Get_Gain
 * @ Parameter   : CH_Sel       : 此次配置的通道选择
 *                      RXCH0       : 指示此次配置的通道为 RXCH0
 *                      RXCH1       : 指示此次配置的通道为 RXCH1
 *                      TXCH0       : 指示此次配置的通道为 TXCH0
 *                      TXCH1       : 指示此次配置的通道为 TXCH1
 *                 *pGain_Level : 获取到的增益等级
 * @ Return      : [Status_t]
 *                      STAT_OK      : 获取成功
 *                      STAT_ERROR        : 获取失败
 * @ Note        : 获取 ECR8660 指定通道的 增益
********************************************************************************/
Status_t ECR8660_Get_Gain(Ecr8660_Path_Idx_E CH_Sel, INT8S *pGain_Level)
{
//    Spi_Interrupt_U SpiInter = {0};
//    INT32U          Gain_Temp;
//    Ecr8660_Ver_U   Code_Ver;
//#if PRINT_LOG
//    Ecr8660_Log_Level_E Printf_Log_Temp;
//    DEBUG_LGE("%s Go...\r\n", __func__);
//    SET_PRINT_BACK(Printf_Log_Temp);
//    SET_PRINT_MIN(ERROR_LOG);
//#endif

//    ECR8660_Get_Versions(&Code_Ver);
//    if((Code_Ver.Bits.minor < 2) || (Code_Ver.Bits.middl < 61)){
//#if PRINT_LOG
//        DEBUG_ERR("\r\nERR! %s Error:The current software version(V%d.%d.%d) does not support this function (Need to upgrade) ->[%s]\r\n\r\n", Code_Ver.Bits.major, Code_Ver.Bits.middl, Code_Ver.Bits.minor, __func__);
//#endif
//        return STAT_ERROR;
//    }
//    SpiInter.ModeX_Bits.Parameter_0   =   CH_Sel;
//    SpiInter.ModeX_Bits.Cmd_Sel       =   ECR8660_CMD_READ_GAIN_GEARS;
//    ECR8660_write(SPI_RW_EXTERNAL, ECR8660_SPI_INTERRUPT, SpiInter.Word);      // 0x00201180

//    Gain_Temp   =   ECR8660_Read(SPI_RW_EXTERNAL, MCU_LOG_ADDR_LIST(4), BYTE(4));

//    if((INT32U)-1 == Gain_Temp){
//#if PRINT_LOG
//        DEBUG_ERR("\r\nERR! ECR8660 Get Gain Error ->[%s]\r\n\r\n", __func__);
//#endif
//        return STAT_ERROR;
//    }
//    *pGain_Level = (INT8S)Gain_Temp;
//#if PRINT_LOG
//    SET_PRINT_RECO(Printf_Log_Temp);
//    DEBUG_LGE("%s OK\r\n", __func__);
//#endif
//    return STAT_OK;
    return STAT_OK;
}
