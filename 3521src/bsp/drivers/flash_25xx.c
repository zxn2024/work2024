/****************************************************************************
 * Copyright(C)，2022，陕西天基通信科技有限责任公司
 * 文件名：FPGA_3521A.c
 * 内容简述：
 *
 * 文件历史：
 * 版本号 日期            作者    说明
 * 01    2023-9-15       扬扬    创建该文件
 */


#include "flash_25xx.h"
#include "hi_spi.h"
#include "rtt.h"
#include "hi_gpio.h"
/*---------------------NOR_FLASH- 寄存器定义 --------------------------------------*/
#define REG_WriteEnable         0x06
#define REG_WriteDisable        0x04
#define REG_ReadStatusReg1      0x05
#define REG_ReadStatusReg2      0x35
#define REG_ReadConfigReg       0x15
#define REG_WriteStatusReg1     0x01
#define REG_WriteStatusReg2     0x31
#define REG_WriteCinfigReg      0x11
#define REG_ReadData            0x03
#define REG_FastReadData        0x0B
#define REG_FastReadDual        0x3B
#define REG_PageProgram         0x02
#define REG_BlockErase_32K      0x52
#define REG_BlockErase_64K      0xD8
#define REG_SectorErase         0x20
#define REG_ChipErase           0xC7
#define REG_PowerDown           0xB9
#define REG_ReleasePowerDown    0xAB
#define REG_ManufactDeviceID    0x90
#define REG_JedecDeviceID       0x9F

/*---------------------- 全局变量  ---------------------------------------*/

static FLASH_25xx_config_t hFlash;

/*---------------------- 函数 ------------------------------------------*/

/*****************************************************************************
* @brief  : 使能片选
* @param  : None
* @retval : None
* @date   : 2023-11-20
* @note   :
*****************************************************************************/
static void flash_cs_enable(void)
{
    hi_gpio_write(hFlash.cs_port, hFlash.cs_pin, LEVEL_L);
}

/*****************************************************************************
* @brief  : 禁止片选
* @param  : None
* @retval : None
* @date   : 2023-11-20
* @note   :
*****************************************************************************/
static void flash_cs_disable(void)
{
    hi_gpio_write(hFlash.cs_port, hFlash.cs_pin, LEVEL_H);
}


/*****************************************************************************
* @brief  : 锁定
* @param  : None
* @retval : None
* @date   : 2023-11-20
* @note   :
*****************************************************************************/
static Status_t flash_lock(void)
{
    if(hFlash.lock == UNLOCK)
    {
        hFlash.lock = LOCK;
        return STAT_OK;
    }
    else
        return STAT_BUSY;
}


/*****************************************************************************
* @brief  : 解锁
* @param  : None
* @retval : None
* @date   : 2023-11-20
* @note   :
*****************************************************************************/
static void flash_unlock(void)
{
    hFlash.lock = UNLOCK;
}

/**********************************************************
 *函数名：flash_readID
 *函数功能：读取设备ID
 *函数参数：无
 *返回值：设备ID
 */

uint32_t flash_readID(void)
{
    uint8_t gDataBuffer[3];
    uint32_t ret_val;

    flash_cs_enable();
    SPI_ReadWriteByte(hFlash.spi_ch, REG_ManufactDeviceID); // 发送读取ID命令
    SPI_ReadWriteByte(hFlash.spi_ch, 0x00);
    SPI_ReadWriteByte(hFlash.spi_ch, 0x00);

    gDataBuffer[2] = SPI_ReadWriteByte(hFlash.spi_ch, 0xFF);
    gDataBuffer[1] = SPI_ReadWriteByte(hFlash.spi_ch, 0xFF);
    gDataBuffer[0] = SPI_ReadWriteByte(hFlash.spi_ch, 0xFF);
    flash_cs_disable();
    ret_val = (uint32_t)(gDataBuffer[0] << 16 | gDataBuffer[1] << 8 | gDataBuffer[2]);

    return ret_val;
}

/**********************************************************
 *函数名：flash_25xx_open
 *函数功能：器件初始化
 *函数参数：无
 *返回值：无
 */
static Status_t flash_25xx_open(void)
{
    flash_cs_disable();

    return STAT_OK;
}

/**********************************************************
 *函数名：flash_close
 *函数功能：器件初始化
 *函数参数：无
 *返回值：无
 */
static Status_t flash_25xx_close(void)
{
    flash_cs_disable();

    return STAT_OK;
}
/**********************************************************
 *函数名：flash_readSR
 *函数功能：读取设备状态寄存器
 *函数参数：regno 寄存器编号
 *返回值：寄存器值
 */
static uint8_t flash_readSR(uint8_t regno)
{
    uint8_t byte = 0, command = 0;
    switch(regno)
    {
        case 1:
            command = REG_ReadStatusReg1; // 读状态寄存器1指令
            break;
        case 2:
            command = REG_ReadStatusReg2; // 读状态寄存器2指令
            break;
        default:
            command = REG_ReadStatusReg1;
            break;
    }
    flash_cs_enable();         // 使能器件
    SPI_ReadWriteByte(hFlash.spi_ch, command);    // 发送读取状态寄存器命令
    byte = SPI_ReadWriteByte(hFlash.spi_ch, 0xFF); // 读取一个字节
    flash_cs_disable();        // 取消片选
    return byte;
}

/**********************************************************
 *函数名：flash_Write_SR
 *函数功能：写设备状态寄存器
 *函数参数：sr 寄存器值
 *返回值：无
 */
void flash_Write_SR(uint8_t sr)
{
    flash_cs_enable();                 // 使能器件
    SPI_ReadWriteByte(hFlash.spi_ch, REG_WriteStatusReg1); // 发送写状态寄存器命令
    SPI_ReadWriteByte(hFlash.spi_ch, sr);                 // 写入一个字节
    flash_cs_disable();
}

/**********************************************************
 *函数名：flash_readCR
 *函数功能：读取设备配置寄存器
 *函数参数：无
 *返回值：寄存器值
 */
uint8_t flash_readCR(void)
{
    uint8_t byte = 0;
    flash_cs_enable();                // 使能器件
    SPI_ReadWriteByte(hFlash.spi_ch, REG_ReadConfigReg); // 发送读取状态寄存器命令
    byte = SPI_ReadWriteByte(hFlash.spi_ch, 0xFF);       // 读取一个字节
    flash_cs_disable();               // 取消片选
    return byte;
}

/**********************************************************
 *函数名：flash_Write_CR
 *函数功能：写设备配置寄存器
 *函数参数：sr 寄存器值
 *返回值：无
 */
void flash_Write_CR(uint8_t sr)
{
    flash_cs_enable();                 // 使能器件
    SPI_ReadWriteByte(hFlash.spi_ch, REG_WriteStatusReg1); // 发送写配置寄存器命令
    SPI_ReadWriteByte(hFlash.spi_ch, sr);                 // 写入一个字节
    flash_cs_disable();
}

/**********************************************************
 *函数名：flash_Write_Enable
 *函数功能：写使能
 *函数参数：无
 *返回值：无
 */
static void flash_Write_Enable(void)
{
    flash_cs_enable();              // 使能器件
    SPI_ReadWriteByte(hFlash.spi_ch, REG_WriteEnable); // 发送写使能
    flash_cs_disable();             // 取消片选
}

/**********************************************************
 *函数名：flash_Write_Disable
 *函数功能：写保护
 *函数参数：无
 *返回值：无
 */
void flash_Write_Disable(void)
{
    flash_cs_enable();               // 使能器件
    SPI_ReadWriteByte(hFlash.spi_ch, REG_WriteDisable); // 发送写禁止指令
    flash_cs_disable();              // 取消片选
}

/**********************************************************
 *函数名：flash_Wait_Busy
 *函数功能：等待空闲
 *函数参数：无
 *返回值：无
 */
static void flash_Wait_Busy(void)
{
    while((flash_readSR(1) & 0x01) == 0x01)
        ; // 等待BUSY位清空
}
/**********************************************************
 *函数名：flash_read
 *函数功能：读数据
 *函数参数：pBuffer 数据存储区
           ReadAddr 地址
           NumByteToRead 数据长度
 *返回值：Status_t
*/
static Status_t flash_25xx_read(uint8_t * pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
    uint16_t i = 0;

    if(pBuffer == NULL || (ReadAddr + NumByteToRead) >= hFlash.flash_size)
    {
        //rtt_printf(RTT_DRIVERS, "Warning: flash read param error!\r\n");
        return STAT_ERROR;
    }

    if(flash_lock() != STAT_OK)
    {
        //rtt_printf(RTT_DRIVERS, "Warning: Flash locked!\r\n");
        return STAT_BUSY;
    }
    flash_cs_enable();                                              // 使能器件
    SPI_ReadWriteByte(hFlash.spi_ch, REG_ReadData);                 // 发送读取命令
    SPI_ReadWriteByte(hFlash.spi_ch, (uint8_t)((ReadAddr) >> 16));  // 发送24bit地址
    SPI_ReadWriteByte(hFlash.spi_ch, (uint8_t)((ReadAddr) >> 8));
    SPI_ReadWriteByte(hFlash.spi_ch, (uint8_t)ReadAddr);
    
    for(i = 0; i < NumByteToRead; i++)
    {
        pBuffer[i] = SPI_ReadWriteByte(hFlash.spi_ch, 0xFF); // 循环读数
    }
    flash_cs_disable();
    flash_unlock();
    return STAT_OK;
}

/**********************************************************
 *函数名：flash_write_page
 *函数功能：写页数据最大256字节
 *函数参数：pBuffer 数据存储区
           WriteAddr 地址
           NumByteToRead 数据长度
 *返回值：Status_t
 */
static Status_t flash_write_page(uint8_t * pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
    uint16_t i = 0;
    if(pBuffer == NULL)
        return STAT_ERROR;

    if(flash_lock() != STAT_OK)
    {
        //rtt_printf(RTT_DRIVERS, "Warning: Flash locked!\r\n");
        return STAT_BUSY;
    }

    flash_Write_Enable();                       // SET WEL
    flash_cs_enable();                          // 使能器件
    SPI_ReadWriteByte(hFlash.spi_ch, REG_PageProgram);            // 发送写页命令
    SPI_ReadWriteByte(hFlash.spi_ch, (uint8_t)((WriteAddr) >> 16)); // 发送24bit地址
    SPI_ReadWriteByte(hFlash.spi_ch, (uint8_t)((WriteAddr) >> 8));
    SPI_ReadWriteByte(hFlash.spi_ch, (uint8_t)WriteAddr);
    for(i = 0; i < NumByteToWrite; i++)
        SPI_ReadWriteByte(hFlash.spi_ch, pBuffer[i]); // 循环写数
    flash_cs_disable();           // 取消片选
    flash_Wait_Busy(); // 等待写入结束

    flash_unlock();
    return STAT_OK;
}


/**********************************************************
 *函数名：flash_Write
 *函数功能：写数据, 带回读检测的写
 *函数参数：pBuffer 数据存储区
           WriteAddr 地址
           NumByteToRead 数据长度
 *返回值：Status_t
 *说明: 写数据前需要擦除对应区域,否则写失败
 */
static Status_t flash_25xx_write(uint8_t * pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
    uint16_t pageremain, i, errcnt = 0;
    BOOL_E flag1 = B_TRUE;
    uint8_t readbuf[512] = {0};

    if(pBuffer == NULL || (WriteAddr + NumByteToWrite) >= hFlash.flash_size)
    {
        //rtt_printf(RTT_DRIVERS, "Flash wr param error!\r\n");
        return STAT_ERROR;
    }
    pageremain = hFlash.page_size - WriteAddr % hFlash.page_size;
    
    if(NumByteToWrite <= pageremain)
        pageremain = NumByteToWrite;
    
    while(1)
    {
        do
        {
            flag1 = B_TRUE;
            if(flash_write_page(pBuffer, WriteAddr, pageremain) == STAT_OK)
            {
                if(flash_25xx_read(readbuf, WriteAddr, pageremain) == STAT_OK)
                {
                    for(i = 0; i < pageremain; i++)
                    {
                        if(readbuf[i] != pBuffer[i])
                        {
                            flag1 = B_FALSE;
                            break;
                        }
                    }
                }
                else
                    flag1 = B_FALSE;
            }
            else
                flag1 = B_FALSE;

            errcnt++;
            if(errcnt >= 3)
                break;

        } while(flag1 == B_FALSE);

        if(NumByteToWrite == pageremain)
            break;
        else
        {
            pBuffer += pageremain;
            WriteAddr += pageremain;

            NumByteToWrite -= pageremain;

            if(NumByteToWrite > hFlash.page_size)
                pageremain = hFlash.page_size;
            else
                pageremain = NumByteToWrite;
        }
    }
    if(flag1 == B_TRUE)
        return STAT_OK;
    else
    {
        //rtt_printf(RTT_DRIVERS, "Flash err\r\n");
        return STAT_ERROR;
    }
}
/**********************************************************
 *函数名：flash_25xx_addr_to_sector
 *函数功能：计算给定地址所属的扇区号
 *函数参数：addr 地址
 *返回值：扇区号
 */
//static uint16_t flash_25xx_addr_to_sector(uint32_t addr)
uint16_t flash_25xx_addr_to_sector(uint32_t addr)
{
    if(addr >= hFlash.flash_size)
    {
        rtt_printf(RTT_DRIVERS, "Input flash addr %d error!\r\n", addr);
    }
    return(addr / hFlash.sector_size);
}
/**********************************************************
 *函数名：flash_25xx_get_sector_start_addr
 *函数功能：计算给定扇区的起始地址
 *函数参数：sector_num 扇区号
 *返回值：地址
 */
static uint32_t flash_25xx_get_sector_start_addr(uint16_t sector_num)
{
    if(sector_num >= hFlash.sector_num)
    {
        rtt_printf(RTT_DRIVERS, "Input flash sector num %d error!\r\n", sector_num);
    }
    return(sector_num * hFlash.sector_size);
}
/**********************************************************
 *函数名：flash_Erase_Sector
 *函数功能：擦除扇区
 *函数参数：Dst_Addr 扇区地址
 *返回值：无
 */
static Status_t flash_25xx_erase_sector(uint16_t sector)
{
    if(sector >= hFlash.sector_num)
    {
        //rtt_printf(RTT_DRIVERS, "Flash sector_num error!\r\n");
    }

    if(flash_lock() != STAT_OK)
    {
        //rtt_printf(RTT_DRIVERS, "Warning: Flash locked!\r\n");
        return STAT_BUSY;
    }
    uint32_t sector_addr = 0;
    sector_addr = sector * hFlash.sector_size;

    flash_Write_Enable(); // SET WEL
    flash_Wait_Busy();
    flash_cs_enable();              // 使能器件
    SPI_ReadWriteByte(hFlash.spi_ch, REG_SectorErase); // 发送扇区擦除指令
    SPI_ReadWriteByte(hFlash.spi_ch, (uint8_t)((sector_addr) >> 16)); // 发送24bit地址
    SPI_ReadWriteByte(hFlash.spi_ch, (uint8_t)((sector_addr) >> 8));
    SPI_ReadWriteByte(hFlash.spi_ch, (uint8_t)sector_addr);

    flash_cs_disable(); // 取消片选
    flash_Wait_Busy();  // 等待擦除完成
    flash_unlock();
    return STAT_OK;
}

/**********************************************************
 *函数名：flash_Erase_Sector
 *函数功能：擦除块
 *函数参数：Dst_Addr 块地址
 *返回值：无
 */
Status_t flash_Erase_Block(uint32_t Dst_Addr)
{
    if(flash_lock() != STAT_OK)
    {
        //rtt_printf(RTT_DRIVERS, "Warning: Flash locked!\r\n");
        return STAT_BUSY;
    }
    Dst_Addr *= (1024 * 64);
    flash_Write_Enable(); // SET WEL
    flash_Wait_Busy();
    flash_cs_enable();                 // 使能器件
    SPI_ReadWriteByte(hFlash.spi_ch, REG_BlockErase_64K); // 发送扇区擦除指令

    SPI_ReadWriteByte(hFlash.spi_ch, (uint8_t)((Dst_Addr) >> 16)); // 发送24bit地址
    SPI_ReadWriteByte(hFlash.spi_ch, (uint8_t)((Dst_Addr) >> 8));
    SPI_ReadWriteByte(hFlash.spi_ch, (uint8_t)Dst_Addr);
    flash_cs_disable(); // 取消片选
    flash_Wait_Busy();  // 等待擦除完成
    flash_unlock();
    return STAT_OK;
}

/**********************************************************
 *函数名：flash_Erase_Chip
 *函数功能：擦除芯片
 *函数参数：无
 *返回值：无
 */
Status_t flash_Erase_Chip(void)
{
    if(flash_lock() != STAT_OK)
    {
        //rtt_printf(RTT_DRIVERS, "Warning: Flash locked!\r\n");
        return STAT_BUSY;
    }
    flash_Write_Enable(); // SET WEL
    flash_Wait_Busy();
    flash_cs_enable();            // 使能器件
    SPI_ReadWriteByte(hFlash.spi_ch, REG_ChipErase); // 发送片擦除命令
    flash_cs_disable();           // 取消片选
    flash_Wait_Busy();            // 等待芯片擦除结束

    flash_unlock();
    return STAT_OK;
}

/**********************************************************
 *函数名：flash_25xx_register
 *函数功能：器件注册,结构体初始化
 *函数参数：flash_config 传入器件参数
 *返回值：无
 */
Status_t flash_25xx_register(FLASH_25xx_config_t * flash_config)
{
    if(flash_config == NULL)
        return STAT_ERROR;

    hFlash.spi_ch = flash_config->spi_ch;
    hFlash.cs_port = flash_config->cs_port;
    hFlash.cs_pin = flash_config->cs_pin;
    hFlash.page_size = flash_config->page_size;
    hFlash.sector_size = flash_config->sector_size;
    hFlash.sector_num = flash_config->sector_num;
    hFlash.flash_size = flash_config->flash_size;
    hFlash.lock = UNLOCK;

    flash_config->flash_open = flash_25xx_open;
    flash_config->flash_close = flash_25xx_close;
    flash_config->flash_addr_to_sector = flash_25xx_addr_to_sector;
    flash_config->flash_get_sector_start_addr = flash_25xx_get_sector_start_addr;
    flash_config->flash_erase_sector = flash_25xx_erase_sector;
    flash_config->flash_read = flash_25xx_read;
    flash_config->flash_write = flash_25xx_write;

    return STAT_OK;
}

#if 1
/*---------------------- 测试函数 ------------------------------------------*/
/**********************************************************
 *函数名：flash_rw_test
 *函数功能：读ID测试,读写测试
 *函数参数：无
 *返回值：无
 */
int flash_rw_test(void)
{
    //rtt_printf(RTT_DRIVERS, "Flash test start!\r\n");
    uint32_t i, addr = 0, j = 0;
    BOOL_E testflag = B_TRUE;
    uint32_t sector = 0;

    uint8_t buf_wr[256] = {0};
    uint8_t buf_rd[256] = {0};

    for(i = 0; i < sizeof(buf_wr); i++)
    {
        buf_wr[i] = 4;
    }
//    hFlash.read(hFlash, buf_rd, addr, hFlash.page_size);
    flash_25xx_read(buf_rd, addr, hFlash.page_size);
    for(j = 0; j < 16; j++)
    {
        //rtt_printf(RTT_DRIVERS, "%x%x%x%x%x%x%x%x%x%x%x%x%x%x%x%x\r\n",
        //buf_rd[0+j*16],buf_rd[1+j*16],buf_rd[2+j*16],buf_rd[3+j*16],buf_rd[4+j*16],buf_rd[5+j*16],buf_rd[6+j*16],buf_rd[7+j*16]
        //,buf_rd[8+j*16],buf_rd[9+j*16],buf_rd[10+j*16],buf_rd[11+j*16],buf_rd[12+j*16],buf_rd[13+j*16],buf_rd[14+j*16],buf_rd[15+j*16]);
    }

    memset(buf_rd, 0, hFlash.page_size);

    sector = addr / hFlash.sector_size;
    flash_25xx_erase_sector(sector);
    flash_25xx_write(buf_wr, addr, hFlash.page_size);
    flash_25xx_read(buf_rd, addr, hFlash.page_size);


    for(i = 0; i < sizeof(buf_wr); i++)
    {
        if(buf_wr[i] != buf_rd[i])
        {
            testflag = B_FALSE;
            break;
        }
    }

    for(j = 0; j < 16; j++)
    {
        //rtt_printf(RTT_DRIVERS, "%x%x%x%x%x%x%x%x%x%x%x%x%x%x%x%x\r\n",
        //buf_rd[0+j*16],buf_rd[1+j*16],buf_rd[2+j*16],buf_rd[3+j*16],buf_rd[4+j*16],buf_rd[5+j*16],buf_rd[6+j*16],buf_rd[7+j*16]
        //,buf_rd[8+j*16],buf_rd[9+j*16],buf_rd[10+j*16],buf_rd[11+j*16],buf_rd[12+j*16],buf_rd[13+j*16],buf_rd[14+j*16],buf_rd[15+j*16]);
    }

    if(i != sizeof(buf_wr))
    {
        //rtt_printf(RTT_DRIVERS, "Flash test error: addr = %d\r\n",addr);
    }

    //rtt_printf(RTT_DRIVERS, "Flash test loop done!\r\n");

    if(testflag == B_FALSE)
        return B_FALSE;
    return B_TRUE;
}

/**********************************************************
 *函数名：flash_test_loop
 *函数功能：循环读写测试,对整片flash全部扇区进行读写
 *函数参数：无
 *返回值：无
 */

#define TEST_SIZ 256
uint8_t buf_wr[TEST_SIZ] = {0};
uint8_t buf_rd[TEST_SIZ] = {0};

int flash_test_loop(void)
{
    //rtt_printf(RTT_DRIVERS, "Flash test start!\r\n");
    uint32_t i, addr = 0, sector = 0;
    BOOL_E testflag = B_TRUE;
    uint32_t cnt = 0;


    for(i = 0; i < sizeof(buf_wr); i++)
    {
        buf_wr[i] = 2;
    }
    flash_25xx_read(buf_rd, addr, TEST_SIZ);
    for(addr = 0; addr < hFlash.flash_size - TEST_SIZ; addr += TEST_SIZ)
    {
        memset(buf_rd, 0, TEST_SIZ);

        sector = flash_25xx_addr_to_sector(addr);
        if(addr % hFlash.sector_size == 0)
            flash_25xx_erase_sector(sector);

        flash_25xx_write(buf_wr, addr, TEST_SIZ);
        flash_25xx_read(buf_rd, addr, TEST_SIZ);

        for(i = 0; i < sizeof(buf_wr); i++)
        {
            if(buf_wr[i] != buf_rd[i])
            {
                testflag = B_FALSE;
                break;
            }
        }
        if(i != sizeof(buf_wr))
        {
            rtt_printf(RTT_DRIVERS, "\r\nFlash test error: addr = %d\r\n",addr);
        }
        else
            rtt_printf(RTT_DRIVERS, "%d ",addr);

        cnt++;
        if(cnt % 20 == 0)
            rtt_printf(RTT_DRIVERS, "\r\n");
    }
    rtt_printf(RTT_DRIVERS, "Flash test loop done!\r\n");

    if(testflag == B_FALSE)
    {
        rtt_printf(RTT_DRIVERS, "\r\nFlash test error!\r\n");
        return B_FALSE;
    }

    return B_TRUE;
}
#endif

