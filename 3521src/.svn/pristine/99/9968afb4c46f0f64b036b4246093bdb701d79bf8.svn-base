/********************************************************************************
 * @ Copyright    : Transa Semi.
 * @
 * @ File Name    : ECR8660_SPI.c
 * @ File Mark    :
 * @ Author       : GuoHao
 * @ E-Mail       : 844396800@qq.com
 * @ Website      : www.GuoHowe.com
 * @ Version      : v1.0
 * @ Date         : 2021-04-14 18:46:39
 * @ Description  :
 * @ History:
 * @       Version      : v1.0
 * @       LastEditors  : GuoHao
 * @       LastEditTime : 2021-08-27 11:00:03
 * @       Modification :
********************************************************************************/

/********************************************************************************
*                  Include file  eg:#include "header_file.h"
********************************************************************************/
#include    "ECR8660.h"
#include    "ecr8668.h"
#include    "bi_fpga.h"
#include    "hi_gpio.h"
#include    "hi_delay.h"
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
// 定义基地址的
INT32U  Base_Addr = 0xFFFFFFFF;
// 用于拼凑时序的空间
Ecr8660_Fm_Spi_Time_Series_U    ECR8660_spi_time_series[3];
// 基地址的映射表
Ecr8660_Fm_Base_Addr_Dict_T     ECR8660_BaseAddr_Dict[] =
{
    {0x00000,   0x00},
    {0x00001,   0x01},
    {0x00002,   0x02},
    {0x00003,   0x03},
    {0x00004,   0x04},
    {0x00005,   0x05},
    {0x00006,   0x06},
    {0x00007,   0x07},
    {0x00008,   0x08},
    {0x00009,   0x09},
    {0x0000A,   0x0A},
    {0x0000B,   0x0B},
    {0x0000C,   0x0C},
    {0x0000D,   0x0D},
    {0x0000E,   0x0E},
    {0x0000F,   0x0F},
    {0x20000,   0x10},
    {0x20001,   0x11},
    {0x20002,   0x12},
    {0x20003,   0x13},
    {0x20004,   0x14},
    {0x20005,   0x15},
    {0x20006,   0x16},
    {0x20007,   0x17},
    {0x00200,   0x18},
    {0x00201,   0x19},
    {0xFFFFF,   0xFF},
};



/*
 *  SPI配置
 * 极性 0  相位 1
 * 数据宽度 8
 * MSB
*/



/********************************************************************************
 * @ Description : User_Printf
 * @ Parameter   : type  : [Ecr8660_Log_Level_E] 打印等级
 *                         ERROR_LOG    // 报错
 *                         ENGLISH_LOG  // 阶段性的提示(英)
 *                         CHINESE_LOG  // 阶段性的提示(中)
 *                         PRINTF_LOG   // 回显
 *                         LOG_MAX_LEVEL
 *                 p_fmt : 字符串
 *                 ...   : 可变长参数
 * @ Return      : None
 * @ Note        : 用户用于 Log 输出的函数，需要将 buff 内整理好的字符串输入到用户的
 *                 输出函数内
 *                 [函数实现需客户自己编写，下面的实现为本人环境中的实现方式]
 *                 VA_LIST 是在C语言中解决变参问题的一组宏，所在头文件：#include <stdarg.h>
 *                 用于获取不确定个数的参数
********************************************************************************/
void    User_Printf(Ecr8660_Log_Level_E type, CHAR * p_fmt, ...)
{
    va_list vArgs;
    CHAR    buff[USER_LOG_BUFF_SIZE];
    // 二级限制设置，禁止 type 大于 USER_DBG_LEVEL 的 Log 输出
    if(USER_DBG_LEVEL < type)
    {
        return;
    }
    va_start(vArgs, p_fmt);
    vsprintf((CHAR *)buff,
        (CHAR const *)p_fmt,
        vArgs);
    va_end(vArgs);

    rtt_printf(RTT_8668, "%s\n", buff);
    vTaskDelay(1);
}


void spi_cs_on(spi_port_t ch) //使能片选
{
    hi_gpio_write(ch.cs_port, ch.cs_pin, LEVEL_L);
}
void spi_cs_off(spi_port_t ch) //取消片选
{
    hi_gpio_write(ch.cs_port, ch.cs_pin, LEVEL_H);
}
//发送数据  pGlobal_Spi_Trans_U为保存SPI数据的结构体
/********************************************************************************
 * @ Description : User_SPI_Write_Read
 * @ Parameter   : pdata : 包含数据和发送长度的结构体
 * @ Return      : Data  : 读操作时读到的数据使用 Return 带出
 * @ Note        : SPI 的底层逻辑，根据结构体 pGlobal_Spi_Trans_U 内的信息，将
 *                 pdata->Bits.spidata1 与 pdata->Bits.spidata2 发出
 *                 [函数实现需客户自己编写，下面的实现为本人环境中的实现方式]
********************************************************************************/
//INT64U  User_SPI_Write_Read(pGlobal_Spi_Trans_U pdata)
//{
//    // 基于用户底层实现
//    // 可以直接将这个 变量以纯 Data 的形式发送 (就是不去管它的时序)
//    // 这个时候需要注意的就是
//    // 1、MSB、LSB 的区别的 (先发低 Bit)
//    // 2、默认状态下 Clk 的状态 (上升沿发送下降沿采样)
//    // 3、默认状态下 Cs  的状态 (默认高电平，低电平有效)

//    // SPI_CLK 的总长度为 data.Bits.writecmdlen + data.Bits.readdatalen
//    // 配置 发送 的数据的长度   data.Bits.writecmdlen   (MOSI 需要 发送 的数据长度，写模式的时候 data.Bits.writecmdlen == SPI_CLK)
//    // 配置 接收 的数据的长度   data.Bits.readdatalen   (MISO 需要 接受 的数据长度，写模式的时候 data.Bits.readdatalen ==    0   )

//    // 需要 发送 的数据为数据   data.Bits.spidata1 + data.Bits.spidata2
//    // 第一个需要发出去的是     data.Bits.spidata1 的 最高位
//    // Read 的数据直接 Return 即可
INT64U  User_SPI_Write_Read(trx8668_port_t spi_periph, Spi_Read_Write_E ReadWrite, pGlobal_Spi_Trans_U pdata)
{

    uint8_t i = 0;
    uint8_t data_1[4];
    uint8_t data_2[4];
    uint8_t data_tx[6] = {0, 0, 0, 0, 0, 0};

    uint32_t rx_data_32 = 0;
    uint64_t rx_data_64 = 0;
    uint8_t  data_rx[6];

    for(i = 0; i < 4; i++) //将一个32位数据拆分为4个字节并保存在数组中，方便SPI发送数据
    {
        data_1[i] = (pdata->Bits.spidata1 >> (32 - 8 * (i + 1))) & 0xFF;
        data_2[i] = (pdata->Bits.spidata2 >> (32 - 8 * (i + 1))) & 0xFF;
    }

    //组织要发送的数据
    if(pdata->Bits.writecmdlen == 4) //写模式 数据2B
    {
        memcpy(data_tx, data_1, sizeof(data_1));
    }
    else if(pdata->Bits.writecmdlen == 6) //写模式 数据4B
    {
        data_tx[0] = data_1[0];
        data_tx[1] = data_1[1];
        data_tx[2] = data_1[2];
        data_tx[3] = data_1[3];
        data_tx[4] = data_2[0];
        data_tx[5] = data_2[1];
    }
    else if(pdata->Bits.writecmdlen == 2)  //读模式
    {
        memcpy(data_tx, data_1, 2);
    }

    //根据端口类型收发数据 SPI/FPGA
    if(spi_periph.port_type == TYPE_SPI)//端口为SPI直接操作时
    {
        spi_cs_on(spi_periph.spi);
        for(i = 0; i < pdata->Bits.writecmdlen; i++) //先发数据
        {
            SPI_ReadWriteByte(spi_periph.spi.spi_ch, data_tx[i]);
        }
        for(i = 0; i < pdata->Bits.readdatalen; i++) //再收数据
        {
            data_rx[i] = SPI_ReadWriteByte(spi_periph.spi.spi_ch, 0xff);
        }
        spi_cs_off(spi_periph.spi);

    }
    else if(spi_periph.port_type == TYPE_FPGA) //端口为FPGA间接操作时
    {
        if(ReadWrite == ECR8660_SPI_READ)
        {
            rx_data_32 = fpga_spi_read(spi_periph.fpga_spi_addr, data_tx, pdata->Bits.writecmdlen);
        }
        else if(ReadWrite == ECR8660_SPI_WRIT)
        {
            fpga_spi_write(spi_periph.fpga_spi_addr, data_tx, pdata->Bits.writecmdlen);
        }
        data_rx[3] = rx_data_32 & 0xFF;
        data_rx[2] = (rx_data_32 >> 8) & 0xFF;
        data_rx[1] = (rx_data_32 >> 16) & 0xFF;
        data_rx[0] = (rx_data_32 >> 24) & 0xFF;
    }

    //组装接收的数据
    if(pdata->Bits.readdatalen == 2)
    {
        if(ReadWrite == ECR8660_SPI_READ)
        {
            rx_data_64 = (data_rx[2] << 8) | (data_rx[3]);
        }
        else if(ReadWrite == ECR8660_SPI_WRIT)
        {
            rx_data_64 = (data_rx[0] << 8) | (data_rx[1]);
        }
        rtt_printf(RTT_8668, "8668_read 2B->: %x\r\n", rx_data_64);
    }
    else if(pdata->Bits.readdatalen == 4)//组装接收的数据
    {
        rx_data_64 = (data_rx[2] << 8) | (data_rx[3]);
        rx_data_64 = rx_data_64 | (data_rx[1] << 16);
        rx_data_64 = rx_data_64 | (data_rx[0] << 24);
        rtt_printf(RTT_8668, "8668_read 4B->: %x\r\n", rx_data_64);
    }
    else
        rx_data_64 = 0;

    return rx_data_64;
}



/********************************************************************************
 * @ Description : ECR8660_Base_Address_Dict_Find
 * @ Parameter   : Addr : 需要查询的地址
 * @ Return      : None
 * @ Note        : 在基地址字典(ECR8660_BaseAddr_Dict)内查询地址Addr的基地址标号
********************************************************************************/
INT8U   ECR8660_Base_Address_Dict_Find(INT32U Addr)
{
    INT16U i;
    for(i = 0; ((BASE_ADDR_ERR != ECR8660_BaseAddr_Dict[i].Base_Addr)) && ((ECR8660_BaseAddr_Dict[i].Base_Addr != Addr)) ; i++);
    return ECR8660_BaseAddr_Dict[i].Base_Log;
}

/********************************************************************************
 * @ Description : ECR8660_SPI_WR
 * @ Parameter   : ReadWrite   : [Spi_Read_Write_E]
 *                      ECR8660_SPI_READ           0x0 // SPI读模式
 *                      ECR8660_SPI_WRIT           0x1 // SPI写模式
 *                 Trans_size  : [Spi_Trans_Size_E]
 *                      TRANS_ONEBYTE_DATA         0x0 // 传输1个byte数据
 *                      TRANS_TWOBYTE_DATA         0x1 // 传输2个byte数据
 *                      TRANS_FOURBYTE_DATA        0x3 // 传输4个byte数据
 *                      TRANS_INTERNAL_REGISTER    0x6 // 作为SPI内部寄存器读写状态指示
 *                 Addr        : 需要操作的地址
 *                 Data        : 需要写入的值
 * @ Return      : None
 * @ Note        : 用于拼凑时序，将时序拼凑为 Data
********************************************************************************/
static  INT8U   ECR8660_SPI_WR(Spi_Read_Write_E Read_write, Spi_Trans_Size_E Trans_size, INT16U Addr, INT64U Data)
{
    Ecr8660_Fm_Spi_Cmd_U            ECR8660_spi_cmd;
    Ecr8660_Fm_Spi_Data_U           ECR8660_spi_data;

    ECR8660_spi_cmd.Bits.Addr       = ECR8660_SPI_ADDR(Addr);
    ECR8660_spi_cmd.Bits.Trans_size = Trans_size;
    ECR8660_spi_cmd.Bits.Read_Write = Read_write;

    ECR8660_spi_time_series[0].Bits.Hight_Half_Word =   ECR8660_spi_cmd.Word;
    ECR8660_spi_data.Clear = 0;
    switch(Trans_size)
    {
        case TRANS_ONEBYTE_DATA:
            ECR8660_spi_data.OneByte.Data                   = (INT8U)Data;
            ECR8660_spi_time_series[0].Bits.Low_Half_Word   =   ECR8660_spi_data.Bits.High_16;
            return (INT8U)USE_ONE_DATAREG;
        case TRANS_TWOBYTE_DATA:
        case TRANS_INTERNAL_REGISTER:
            ECR8660_spi_data.TwoByte.Data                   = (INT16U)Data;
            ECR8660_spi_time_series[0].Bits.Low_Half_Word   =   ECR8660_spi_data.Bits.High_16;
            return (INT8U)USE_ONE_DATAREG;
        case TRANS_FOURBYTE_DATA:
            ECR8660_spi_data.FourByte.Data                  = (INT32U)Data;
            ECR8660_spi_time_series[0].Bits.Low_Half_Word   =   ECR8660_spi_data.Bits.High_16;
            ECR8660_spi_time_series[1].Word                 =   ECR8660_spi_data.Bits.Middle_32;
            return (INT8U)USE_TWO_DATAREG;
        case TRANS_EIGHTBYTE_DATA:
            ECR8660_spi_data.EightByte.Data                 =   Data;
            ECR8660_spi_time_series[0].Bits.Low_Half_Word   =   ECR8660_spi_data.Bits.High_16;
            ECR8660_spi_time_series[1].Word                 =   ECR8660_spi_data.Bits.Middle_32;
            ECR8660_spi_time_series[2].Bits.Hight_Half_Word =   ECR8660_spi_data.Bits.Low_16;
            return (INT8U)USE_THR_DATAREG;
        default:
            return (INT8U)1;
    }
}

/********************************************************************************
 * @ Description : ECR8660_read_write_Basic
 * @ Parameter   : ReadWrite   : [Spi_Read_Write_E]
 *                      ECR8660_SPI_READ           0x0 // SPI读模式
 *                      ECR8660_SPI_WRIT           0x1 // SPI写模式
 *                 Trans_size  : [Spi_Trans_Size_E]
 *                      TRANS_ONEBYTE_DATA         0x0 // 传输1个byte数据
 *                      TRANS_TWOBYTE_DATA         0x1 // 传输2个byte数据
 *                      TRANS_FOURBYTE_DATA        0x3 // 传输4个byte数据
 *                      TRANS_INTERNAL_REGISTER    0x6 // 作为SPI内部寄存器读写状态指示
 *                 Addr        : 需要操作的地址
 *                 Data        : 需要写入的值
 * @ Return      : 读取/写入 的值
 * @ Note        : SPI 读写的公用函数
********************************************************************************/
static  INT64U  ECR8660_read_write_Basic(trx8668_port_t spi_periph, Spi_Read_Write_E ReadWrite, Spi_Trans_Size_E Trans_size, INT32U Addr, INT32U SPI_Data)
{

    Global_Spi_Trans_U      data = {0};

    // 该部分为对 SPI master 的控制，底层需根据实际情况修改
    // 配置 发送 的数据的长度 : MOSI 上将会发送的真实 Bit 长度(不考虑占位)      // 内部写模式：2Byte_CMD + 2ByteWriteData  外部写模式：2Byte_CMD + 写数据的长度  读模式：2Byte_CMD
    data.Bits.writecmdlen   = (ECR8660_SPI_WRIT == ReadWrite) ? ((TRANS_INTERNAL_REGISTER == Trans_size) ? 4 : Trans_size + 3) : 2;
    // 配置 接收 的数据的长度 : MISO 上将会接收的真实 Bit 长度(不考虑移位)      // 内部读模式：2ByteReadData               外部读模式：读数据的长度              写模式：0
    data.Bits.readdatalen   = (ECR8660_SPI_READ == ReadWrite) ? ((TRANS_INTERNAL_REGISTER == Trans_size) ? 2 : Trans_size + 1) : 0;
    // Clk 的总时钟数为 data.Bits.cmdlen + data.Bits.datalen

    // 拼凑时序，将 ECR8660 所需要的 SPI 时序拼凑为 Data 数据
    ECR8660_SPI_WR(ReadWrite, Trans_size, Addr, SPI_Data);
    data.Bits.spidata1      = ECR8660_spi_time_series[0].Word;
    data.Bits.spidata2      = ECR8660_spi_time_series[1].Word;

    return User_SPI_Write_Read(spi_periph, ReadWrite, &data);
}

/********************************************************************************
 * @ Description : ECR8660_Base_Address
 * @ Parameter   : Addr 地址
 * @ Return      : None
 * @ Note        : 对页外寄存器访问时，进行页切换的函数
********************************************************************************/
static  void    ECR8660_Base_Address(trx8668_port_t spi_periph, INT32U Addr)
{
    ECR8660_read_write_Basic(spi_periph, ECR8660_SPI_WRIT, TRANS_INTERNAL_REGISTER, SPI_REG_BASE_ADDR, ECR8660_Base_Address_Dict_Find(ECR8660_SPI_BASE_ADDR(Addr)));
//    rt_thread_mdelay(10);
}

/********************************************************************************
 * @ Description : ECR8660_read_write
 * @ Parameter   : ReadWrite   : [Spi_Read_Write_E]
 *                      ECR8660_SPI_READ           0x0 // SPI读模式
 *                      ECR8660_SPI_WRIT           0x1 // SPI写模式
 *                 Trans_size  : [Spi_Trans_Size_E]
 *                      TRANS_ONEBYTE_DATA         0x0 // 传输1个byte数据
 *                      TRANS_TWOBYTE_DATA         0x1 // 传输2个byte数据
 *                      TRANS_FOURBYTE_DATA        0x3 // 传输4个byte数据
 *                      TRANS_INTERNAL_REGISTER    0x6 // 作为SPI内部寄存器读写状态指示
 *                 Addr        : 需要操作的地址
 *                 Data        : 需要写入的值
 * @ Return      : 读取/写入 的值
 * @ Note        : SPI 读写的公用函数
********************************************************************************/
INT64U  ECR8660_read_write(trx8668_port_t spi_periph, Spi_Read_Write_E ReadWrite, Spi_Trans_Size_E Trans_size, INT32U Addr, INT32U SPI_Data)
{
    //if((TRANS_INTERNAL_REGISTER != Trans_size) && (ECR8660_SPI_BASE_ADDR(Addr) != Base_Addr))
    if(TRANS_INTERNAL_REGISTER != Trans_size)
    {
        //Base_Addr = ECR8660_SPI_BASE_ADDR(Addr);
        ECR8660_Base_Address(spi_periph, Addr);
    }
    return  ECR8660_read_write_Basic(spi_periph, ReadWrite, Trans_size, Addr, SPI_Data);
}

/********************************************************************************
 * @ Description : ECR8660_Read
 * @ Parameter   : ECR8660_mode : [Spi_Reg_Ext_Int_E]
 *                      SPI_RW_INTERIOR           0x1 // 指示页内寄存器
 *                      SPI_RW_EXTERNAL           0x0 // 页外寄存器
 *                 ECR8660_Addr : 需要操作的地址
 *                 ECR8660_Len  : 需要读取的数据长度(Byte)
 * @ Return      : ECR8660_Data : 读取的值
 * @ Note        : 对 ECR8660 进行寄存器 "读" 操作
********************************************************************************/
INT64U  ECR8660_Read(trx8668_port_t spi_periph, Spi_Reg_Ext_Int_E ECR8660_mode, INT32U ECR8660_Addr, INT32U ECR8660_Len)
{

    Spi_Trans_Size_E    Spi_Datalen;
    INT64U  Data;
//    INT64U ECR8660_Data = 0;
    INT64U SPI_Data = 0x0;
    INT32U  i;
#if PRINT_LOG
    CHAR    buffer[USER_LOG_BUFF_SIZE];
#endif
    Spi_Datalen = (SPI_RW_INTERIOR == ECR8660_mode) ? TRANS_INTERNAL_DATA : USER_ECR8660_SPI_SIZE;
#if PRINT_LOG
    sprintf(buffer, "rd-> %%d 0x%%08X 0x%%0%dX\r\n", ECR8660_SPI_BYTE(Spi_Datalen) << 1);
#endif
    for(i = 0; i < ECR8660_Len; i += ECR8660_SPI_BYTE(Spi_Datalen))
    {
        Data = ECR8660_read_write(spi_periph, ECR8660_SPI_READ, (SPI_RW_INTERIOR == ECR8660_mode) ? TRANS_INTERNAL_REGISTER : Spi_Datalen, ECR8660_Addr + i, SPI_Data);
#if PRINT_LOG
        DEBUG_OUT(buffer, ECR8660_SPI_BYTE(Spi_Datalen), (ECR8660_Addr + i), Data);
#endif
//        ECR8660_Data <<= BYTE_2_BIT(ECR8660_SPI_BYTE(Spi_Datalen));
//        ECR8660_Data  |= Data;
    }
    return Data;
}

/********************************************************************************
 * @ Description : ECR8660_write
 * @ Parameter   : ECR8660_mode : [Spi_Reg_Ext_Int_E]
 *                      SPI_RW_INTERIOR           0x1 // 指示页内寄存器
 *                      SPI_RW_EXTERNAL           0x0 // 页外寄存器
 *                 ECR8660_Addr : 需要操作的地址
 *                 ECR8660_Data : 需要操作的值
 * @ Return      : None
 * @ Note        : 对 ECR8660 进行寄存器 "写" 操作 [无论使用哪种模式，均归一化为写
 *                 32Bit 的形式]
********************************************************************************/
void ECR8660_write(trx8668_port_t spi_periph, Spi_Reg_Ext_Int_E ECR8660_mode, INT32U ECR8660_Addr, INT32U ECR8660_Data)
{
    Spi_Trans_Size_E    Spi_Datalen;
    INT32U  i;
#if PRINT_LOG
    CHAR    buffer[USER_LOG_BUFF_SIZE];
#endif
    Spi_Datalen = (SPI_RW_INTERIOR == ECR8660_mode) ? TRANS_INTERNAL_DATA : USER_ECR8660_SPI_SIZE;
#if PRINT_LOG
    sprintf(buffer, "wr-> %%d 0x%%08X 0x%%0%dX\r\n", ECR8660_SPI_BYTE(Spi_Datalen) << 1);
#endif
    for(i = 0;
        i < ((SPI_RW_INTERIOR == ECR8660_mode) ? ECR8660_SPI_BYTE(TRANS_INTERNAL_DATA) : sizeof(ECR8660_Data));
        ECR8660_Data >>= BYTE_2_BIT(ECR8660_SPI_BYTE(Spi_Datalen)), i += ECR8660_SPI_BYTE(Spi_Datalen))
    {
#if PRINT_LOG
        DEBUG_OUT(buffer, ECR8660_SPI_BYTE(Spi_Datalen), ECR8660_Addr + i, ECR8660_Data & LEN_XX_BIT(ECR8660_SPI_BIT(Spi_Datalen)));
#endif
        // rtt_printf(RTT_8668,"wr-> 4 0x%08X 0x%08X\r\n", (ECR8660_Addr + i), ECR8660_Data);
        ECR8660_read_write(spi_periph, ECR8660_SPI_WRIT, (SPI_RW_INTERIOR == ECR8660_mode) ? TRANS_INTERNAL_REGISTER : Spi_Datalen, ECR8660_Addr + i, ECR8660_Data);
    }
}


void write_code_bin(INT32U ECR8660_Addr, INT32U ECR8660_Data)
{

}

/********************************************************************************
 * @ Description : ECR8660_Set_List_Basic
 * @ Parameter   : List : 需要写入的寄存器序列
 * @ Return      : None
 * @ Note        : None
********************************************************************************/
static Status_t ECR8660_Set_List_Basic(trx8668_port_t spi_periph, Ecr8660_Reg_List_T List)
{
#if PRINT_LOG
    if(List.log) DEBUG_LGC("%s\r\n", List.log);
#endif
    switch(List.readwrite)
    {
        case ECR8660_SPI_WRIT:

            ECR8660_write(spi_periph, List.mode, List.addr, (List.data));
            break;
        case ECR8660_SPI_READ:
            ECR8660_Read(spi_periph, List.mode, List.addr, List.data);
            break;
        case ECR8660_SPI_SLEEP:
#if PRINT_LOG
            DEBUG_OUT("User_Delay_ms(%d)\r\n", List.data);
#endif
            vTaskDelay(List.data);
            break;

        default:
#if PRINT_LOG
            DEBUG_ERR("ERR! List Moder Error    [Check The (List.readwrite)] ->[%s]\r\n\r\n", __func__);
#endif
            return STAT_ERROR;
    }
    return STAT_OK;
}

/********************************************************************************
 * @ Description : ECR8660_Set_List
 * @ Parameter   : List : 需要写入的寄存器序列
 * @ Return      : None
 * @ Note        : None
********************************************************************************/
void    ECR8660_Set_List(trx8668_port_t spi_periph, Ecr8660_Reg_List_T * List)
{
    INT32U i = 0;
#if PRINT_LOG
    DEBUG_LGE("%s Go...\r\n", __func__);
#endif
    for(i = 0; List[i].addr != ECR8660_LIST_END_LOG; i++)
    {
        if(List[i].addr == DFE_CONFIG_DELAY_FLAG)
        {
            vTaskDelay(100);
        }
        if((List[i].addr == 0x00201080) || (List[i].addr == 0x00201180))
        {
            vTaskDelay(100);
        }
        if(STAT_ERROR == ECR8660_Set_List_Basic(spi_periph, List[i])) return;
        if(List[i].addr == DFE_CONFIG_DELAY_FLAG)
        {
            vTaskDelay(100);
        }
        if((List[i].addr == 0x00201080) || (List[i].addr == 0x00201180))
        {
            vTaskDelay(100);
        }
        vTaskDelay(20);
    }

#if PRINT_LOG
    DEBUG_LGE("%s OK\r\n", __func__);
#endif
}

/********************************************************************************
 * @ Description : ECR8660_Buff_Write
 * @ Parameter   : Addr : 需要写入的 ECR8660 地址
 *                 Buff : 需要写入的数据数组首地址
 *                 Size : 需要写入的长度
 *                 Trans_size  : [Spi_Trans_Size_E]
 *                      TRANS_ONEBYTE_DATA         0x0 // 传输1个byte数据
 *                      TRANS_TWOBYTE_DATA         0x1 // 传输2个byte数据
 *                      TRANS_FOURBYTE_DATA        0x3 // 传输4个byte数据
 *                      TRANS_INTERNAL_REGISTER    0x6 // 作为SPI内部寄存器读写状态指示
 * @ Return      : Addr : 末尾地址
 * @ Note        : 将 [Buff] 内长度为 [Size]Byte 的数据，通过 SPI 的 Trans_size 模式写入 ECR8660 的 [Addr] 地址
********************************************************************************/
INT32U  ECR8660_Buff_Write(Spi_Trans_Size_E Trans_size, INT32U Addr, void * Buff, INT32U Size)
{
//    INT32U  i;
//    for(i = 0; i < Size; Addr += ECR8660_SPI_BYTE(USER_ECR8660_SPI_SIZE), i += ECR8660_SPI_BYTE(USER_ECR8660_SPI_SIZE) ){
//        if(ECR8660_SPI_BASE_ADDR(Addr) != Base_Addr){
//            Base_Addr = ECR8660_SPI_BASE_ADDR(Addr);
//            ECR8660_Base_Address(spi_periph,Addr);
//        }
//        ECR8660_read_write_Basic(spi_periph,ECR8660_SPI_WRIT, USER_ECR8660_SPI_SIZE, Addr, *((INT64U *)((INT32U)Buff + i)));
//    }
    return Addr;
}
