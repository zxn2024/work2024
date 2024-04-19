/********************************************************************************
 * @ Copyright    : Transa Semi.
 * @
 * @ File Name    : ECR8660.c
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
 * @       LastEditTime : 2021-08-27 11:00:16
 * @       Modification :
********************************************************************************/

/********************************************************************************
*                  Include file  eg:#include "header_file.h"
********************************************************************************/
#include    "ECR8660.h"
#include    "main.h"
#include    "ecr8668.h"
#include    "hi_delay.h"
#include    "hi_irq.h"
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

//ECR8668二进制代码表

// 用于临时修改 LOG 的打印等级
#if PRINT_LOG
Ecr8660_Log_Level_E User_Printf_LOG     = LOG_MAX_LEVEL;
#endif
// ECR8660 的上电序列

Ecr8660_Reg_List_T  PwrUp_ECR8660_EN[] = {
    { ECR8660_SPI_WRIT  ,   SPI_RW_INTERIOR ,  SPI_REG_DCXO_EN_L    ,  0x83      ,  "SET DCXO"          },  // 设置 DCXO 参数
    { ECR8660_SPI_WRIT  ,   SPI_RW_INTERIOR ,  SPI_REG_MCU_RESET    ,  0x0       ,  "GLOBAL RESET"      },  // 拉起全局复位
    { ECR8660_SPI_WRIT  ,   SPI_RW_INTERIOR ,  SPI_REG_LDO0_EN_L    ,  0x1       ,  NULL                },  // 释放全局复位
    { ECR8660_SPI_WRIT  ,   SPI_RW_INTERIOR ,  SPI_REG_LDO0_EN_H    ,  0xFCD0    ,  "SET SPI DRIVER"    },  // 设置 SPI 驱动
    { ECR8660_SPI_WRIT  ,   SPI_RW_INTERIOR ,  SPI_REG_DCXO_EN_H    ,  0x4741    ,  "SET LDO"           },  // 设置 LDO
    { ECR8660_SPI_WRIT  ,   SPI_RW_INTERIOR ,  SPI_REG_SOC_ISO_E    ,  0x03      ,  NULL                },  // 设置 LDO
    { ECR8660_SPI_WRIT  ,   SPI_RW_INTERIOR ,  SPI_REG_SOC_ISO_E    ,  0x01      ,  "SET DCXO "         },  // 设置 DCXO 参数
    { ECR8660_SPI_WRIT  ,   SPI_RW_INTERIOR ,  SPI_REG_DCXO_EN_L    ,  0xC3      ,  "SET ISO"           },  // 隔离拉起
    { ECR8660_SPI_WRIT  ,   SPI_RW_INTERIOR ,  SPI_REG_SOC_RESET    ,  0x0       ,  NULL                },  // 释放隔离
    { ECR8660_SPI_WRIT  ,   SPI_RW_INTERIOR ,  SPI_REG_SOC_RESET    ,  0x01      ,  "GLOBAL RESET"      },  // 拉起全局复位
    { ECR8660_SPI_WRIT  ,   SPI_RW_EXTERNAL ,  ECR8660_LIST_END_LOG ,  ECR8660_LIST_END_LOG , "END FLAG"},  //结束标志
};

//Ecr8660_Reg_List_T  PwrUp_ECR8660_EN[] = {
//    { ECR8660_SPI_WRIT  ,   SPI_RW_INTERIOR ,  SPI_REG_DCXO_EN_L    ,  0x83                 ,  "SET DCXO" },  // 设置 DCXO 参数
//    { ECR8660_SPI_WRIT  ,   SPI_RW_INTERIOR ,  SPI_REG_SOC_RESET    ,  0x0                  ,  "GLOBAL RESET"       },  // 拉起全局复位
//    { ECR8660_SPI_WRIT  ,   SPI_RW_INTERIOR ,  SPI_REG_SOC_RESET    ,  0x1                  ,  NULL             },  // 释放全局复位
//    { ECR8660_SPI_WRIT  ,   SPI_RW_INTERIOR ,  SPI_REG_SPI_PADPC    ,  0x0                  ,  "SET SPI DRIVER"  },  // 设置 SPI 驱动
//    { ECR8660_SPI_WRIT  ,   SPI_RW_INTERIOR ,  SPI_REG_LDO0_EN_L    ,  0x1                  ,  "SET LDO"       },  // 设置 LDO
//    { ECR8660_SPI_WRIT  ,   SPI_RW_INTERIOR ,  SPI_REG_LDO0_EN_H    ,  0xFCD0               ,  NULL             },  // 设置 LDO
//    { ECR8660_SPI_WRIT  ,   SPI_RW_INTERIOR ,  SPI_REG_DCXO_EN_H    ,  0x4741               ,  "SET DCXO " },  // 设置 DCXO 参数
//    { ECR8660_SPI_WRIT  ,   SPI_RW_INTERIOR ,  SPI_REG_SOC_ISO_E    ,  0x03                 ,  "SET ISO"       },  // 隔离拉起
//    { ECR8660_SPI_WRIT  ,   SPI_RW_INTERIOR ,  SPI_REG_SOC_ISO_E    ,  0x01                 ,  NULL             },  // 释放隔离
//    { ECR8660_SPI_WRIT  ,   SPI_RW_INTERIOR ,  SPI_REG_SOC_RESET    ,  0x0                  ,  "GLOBAL RESET"       },  // 拉起全局复位
//    { ECR8660_SPI_WRIT  ,   SPI_RW_INTERIOR ,  SPI_REG_SOC_RESET    ,  0x1                  ,  NULL             },  // 释放全局复位
//    { ECR8660_SPI_WRIT  ,   SPI_RW_INTERIOR ,  SPI_REG_MCU_RESET    ,  0x00                 ,  "MCU RESET"       },  // 拉起 MCU 复位
//    { ECR8660_SPI_SLEEP ,   SPI_RW_EXTERNAL ,  0                    ,  10                   ,  NULL             },
//    { ECR8660_SPI_WRIT  ,   SPI_RW_EXTERNAL ,  0x002002B0           ,  0x0                  ,  "INIT CLEAR ZERO"      },
//    { ECR8660_SPI_WRIT  ,   SPI_RW_EXTERNAL ,  0x00200494           ,  0x0                  ,  NULL             },
//    { ECR8660_SPI_WRIT  ,   SPI_RW_EXTERNAL ,  0x002004D4           ,  0x0                  ,  NULL             },
//    { ECR8660_SPI_WRIT  ,   SPI_RW_INTERIOR ,  SPI_REG_DCXO_EN_L    ,  0xC3                 ,  "SET DCXO" },  // 设置 DCXO 参数
//    { ECR8660_SPI_WRIT  ,   SPI_RW_EXTERNAL ,  ECR8660_LIST_END_LOG ,  ECR8660_LIST_END_LOG ,  "END FLAG"       },
//};

/********************************************************************************
 * @ Description : ECR8660_Device_Prepare
 * @ Parameter   : Mode : [ON_OFF]
 *                      on                         !0  // 读写后恢复   MCU_LOG_ADDR_HAND 的值
 *                      off                        0   // 读写后不恢复 MCU_LOG_ADDR_HAND 的值，
 * @ Return      : [Status_t]
 *                      STAT_OK  : 检测成功
 *                      STAT_ERROR    : 检测失败，并打印 Log
 * @ Note        : 判断 ECR8660 的 Memory 是否正常
********************************************************************************/
INT64U DataRd = 0;
Status_t ECR8660_Device_Prepare(trx8668_port_t spi_periph,ON_OFF Mode)
{
    INT64U Data;

    if(Mode)
    {
        Data = ECR8660_Read(spi_periph,SPI_RW_EXTERNAL, MCU_LOG_ADDR_HAND, ECR8660_SPI_BYTE(USER_ECR8660_SPI_SIZE));
    }
	
    ECR8660_write(spi_periph,SPI_RW_EXTERNAL, MCU_LOG_ADDR_HAND, ECR8660_SPI_REG_LOG);
    
	DataRd = ECR8660_Read(spi_periph,SPI_RW_EXTERNAL, MCU_LOG_ADDR_HAND, ECR8660_SPI_BYTE(USER_ECR8660_SPI_SIZE));
    if(ECR8660_SPI_REG_LOG != DataRd)
    {
        return STAT_ERROR;
    }
	
    if(Mode)
    {
        ECR8660_write(spi_periph,SPI_RW_EXTERNAL, MCU_LOG_ADDR_HAND, Data);
    }

    return STAT_OK;
}

/********************************************************************************
 * @ Description : ECR8660_CodeDownload
 * @ Parameter   : Flash_Addr_Head : 存放代码的 Flash 首地址
 * @ Return      : [Status_t]
 *                      STAT_OK  : 下载成功
 *                      STAT_ERROR    : 下载失败，并打印 Log
 * @ Note        : 对 ECR8660 进行代码下载操作
********************************************************************************/

Status_t ECR8660_CodeDownload(trx8668_port_t spi_periph,INT32U Flash_Addr_Head,INT32U len)
{
    //INT32U  binWriteData = 0;
    //uint8_t ucflashreadbuf[4];//读取FLASH中ECR8668 代码
    //uint32_t i;
    //uint32_t binTxAddr;
    // uint32_t flash_addr;
    //binTxAddr = ECR_8668_START_ADDR;
//    //发送bin文件前先复位
    //ECR8660_write(spi_periph,SPI_RW_INTERIOR,SPI_REG_MCU_RESET,RESET);
    //vTaskDelay(100);//复位后延时30ms再发送bin文件
    //bin文件发送 采用16+32发送 16是控制端的读写和地址信息，32位是bin文件里的4字节数据
    // for(i = 0; i < ecr8668_bin_len; i = i+4)
    // {
    //     // 从内部flash加载
    //     // flash_addr = ECR_8668_FLASH_ADDR + i;
    //     // ucDrvFlashReadWithByte(ucflashreadbuf,flash_addr,4);//从FLASH扇区6 0x08040000读取bin文件 4字节 ，读取4个发送4个，读取太多内存不够，无法运行

    //     // 从片外flash加载
    //     //P25QXX_Read(ucflashreadbuf, i, 4);//从FLASH扇区6 0x08040000读取bin文件 4字节 ，读取4个发送4个，读取太多内存不够，无法运行


    //     // 从ram加载
    //     // memcpy(ucflashreadbuf, &bin_buf[i], 4);

    //     memcpy(ucflashreadbuf, &ecr8668_bin[i], 4);

    //     binWriteData = 0;
    //     binWriteData |= (((uint32_t)ucflashreadbuf[0]) & 0x000000FF);
    //     binWriteData |= (((uint32_t)ucflashreadbuf[1] << 8 ) & 0x0000FF00);
    //     binWriteData |= (((uint32_t)ucflashreadbuf[2] << 16) & 0x00FF0000);
    //     binWriteData |= (((uint32_t)ucflashreadbuf[3] << 24) & 0xFF000000);

    //     ECR8660_write(spi_periph,SPI_RW_EXTERNAL,binTxAddr,binWriteData);
    //     binTxAddr = binTxAddr + 4;
    // }
    rtt_printf(RTT_8668,"8668_BIN download done\r\n");
    return STAT_OK;
}


/********************************************************************************
 * @ Description : ECR8660_CodeDownload_internal_flash
 * @ Parameter   : h8668
 * @ Note        : 测试临时使用,将所有bin文件存放在一个全局常量里,并从这里加载
********************************************************************************/
Status_t ECR8660_CodeDownload_internal_flash(TRX8668_FPGA_t *h8668)
{
    INT32U  binWriteData = 0;
    uint8_t ucflashreadbuf[4];//读取FLASH中ECR8668 代码
    const uint8_t *ptr_bin;
    uint32_t i,len;
    uint32_t binTxAddr;
    // uint32_t flash_addr;
    binTxAddr = ECR_8668_START_ADDR;

    if(h8668->band == BAND_3)
    {
        ptr_bin = ecr8668_bin_b3;
        len = ecr8668_bin_len_b3;
    }else if(h8668->band == BAND_8)
    {
        ptr_bin = ecr8668_bin_b8;
        len = ecr8668_bin_len_b8;
    }else if(h8668->band == BAND_39)
    {
        ptr_bin = ecr8668_bin_b39;
        len = ecr8668_bin_len_b39;
    }else if(h8668->band == BAND_40)
    {
        ptr_bin = ecr8668_bin_b40;
        len = ecr8668_bin_len_b40;
    }else if(h8668->band == BAND_41)
    {
        ptr_bin = ecr8668_bin_n41;
        len = ecr8668_bin_len_n41;
    }else if(h8668->band == BAND_41M)
    {
        ptr_bin = ecr8668_bin_n41;
        len = ecr8668_bin_len_n41;
    }
    else
        return STAT_ERROR;
        

//    //发送bin文件前先复位
    ECR8660_write(h8668->config.spi_port,SPI_RW_INTERIOR,SPI_REG_MCU_RESET,RESET);
    vTaskDelay(100);//复位后延时30ms再发送bin文件
    for(i = 0; i < len; i = i+4)
    {
        // 从ram加载
        memcpy(ucflashreadbuf, &ptr_bin[i], 4);

        binWriteData = 0;
        binWriteData |= (((uint32_t)ucflashreadbuf[0]) & 0x000000FF);
        binWriteData |= (((uint32_t)ucflashreadbuf[1] << 8 ) & 0x0000FF00);
        binWriteData |= (((uint32_t)ucflashreadbuf[2] << 16) & 0x00FF0000);
        binWriteData |= (((uint32_t)ucflashreadbuf[3] << 24) & 0xFF000000);

        ECR8660_write(h8668->config.spi_port,SPI_RW_EXTERNAL,binTxAddr,binWriteData);
        binTxAddr = binTxAddr + 4;
    }
    rtt_printf(RTT_8668,"8668 %d. BIN download done\r\n",h8668->config.dev_number);
    return STAT_OK;
}

/********************************************************************************
 * @ Description : ECR8660_INIT
 * @ Parameter   : None
 * @ Return      : [Status_t]
 *                      STAT_OK  : 初始化成功
 *                      STAT_ERROR    : 初始化失败，并打印 Log
 * @ Note        : 对 ECR8660 进行初始化的操作序列
********************************************************************************/
Status_t ECR8660_INIT(TRX8668_FPGA_t *h8668, uint32_t flash_addr, uint32_t bin_size)
{
    
#if PRINT_LOG
    DEBUG_LGE("%s Go...\r\n", __func__);
#endif
     uint64_t text1 = 0;
     uint8_t rd_cnt = 0;
    ECR8660_Set_List(h8668->config.spi_port,PwrUp_ECR8660_EN);//上电初始化
    ECR8660_Device_Prepare(h8668->config.spi_port,on);    //检查上电初始化完成情况
    //ECR8660_CodeDownload(h8668->config.spi_port,flash_addr,bin_size);//从FLASH下载8668 bin 文件并发送给8668
    ECR8660_CodeDownload_internal_flash(h8668);//直接将数组定义的8668 bin 文件并发送给8668
	ECR8660_write(h8668->config.spi_port,SPI_RW_INTERIOR,SPI_REG_MCU_RESET,RESET);
    ECR8660_write(h8668->config.spi_port,SPI_RW_INTERIOR,SPI_REG_MCU_RESET,SET); //8668释放复位

    vTaskDelay(1000); // 2s->3s 设置为1s有概率初始化失败

    do
    {
        vTaskDelay(1000);
        text1 = ECR8660_Read(h8668->config.spi_port,SPI_RW_EXTERNAL,MCU_ECR8668_INIT_SURE,2);//回读0x20000004寄存器，值为0x123代表初始化完成
        rd_cnt++;
        if(rd_cnt >= 3) //最多重复读取三次
            break;
    }while(text1 != 0x123);

    if(text1 == 0x123)
    {
        rtt_printf(RTT_8668,RTT_TEXT_GREEN"ECR8668 %d. INIT FINISH!\r\n",h8668->config.dev_number);
        return STAT_OK;
    }
    else{
        rtt_printf(RTT_8668,RTT_TEXT_RED"ECR8668 %d. INIT failed!\r\n",h8668->config.dev_number);
        return STAT_ERROR;
    }

    // return STAT_ERROR;
}


/********************************************************************************
 * @ Description : ECR8660_CodeUpData
 * @ Parameter   : Code_Addr : 上载的代码段的首地址
 *                 Code_Size : 上载的代码长度
 *                      [Code_Addr 与 Code_Size 均为 0 时，上载全部代码]
 * @ Return      : None
 * @ Note        : 对 ECR8660 进行代码上载操作
********************************************************************************/
void    ECR8660_CodeUpData(INT32U Code_Addr, INT32U Code_Size)
{
//#if PRINT_LOG
//    INT32U  i, Data;
//    Ecr8660_Log_Level_E Printf_Log_Temp;
//    SET_PRINT_BACK(Printf_Log_Temp);
//    if(0 == (Code_Addr | Code_Size)){
//        Code_Size = REG(MCU_CODE_SIZE_ADDR);
//    }
//    for(i = 0; i < Code_Size; i += 4){
//        SET_PRINT_MIN(LOG_MIN_LEVEL);
//        Data = ECR8660_Read(SPI_RW_EXTERNAL, Code_Addr + i, BYTE(4));
//        SET_PRINT_RECO(Printf_Log_Temp);
//        if(0 == (i & LEN_04_BIT)){
//            DEBUG_OUT( "\r\n%08X:", Code_Addr + i);
//        }
//        DEBUG_OUT(" %02X %02X %02X %02X",   GET_VAL_BIT(Data,  0, LEN_08_BIT),
//                                            GET_VAL_BIT(Data,  8, LEN_08_BIT),
//                                            GET_VAL_BIT(Data, 16, LEN_08_BIT),
//                                            GET_VAL_BIT(Data, 24, LEN_08_BIT));
//    }
//    DEBUG_OUT("\r\nOK\r\n\r\n");
//#endif
}


//ECR8668 软复位
void ecr8668_soft_reset(trx8668_port_t spi_periph)
{
    ECR8660_Set_List(spi_periph,PwrUp_ECR8660_EN);//软复位
    vTaskDelay(100);
    ECR8660_write(spi_periph,SPI_RW_INTERIOR,SPI_REG_MCU_RESET,SET); //8668释放复位
}

