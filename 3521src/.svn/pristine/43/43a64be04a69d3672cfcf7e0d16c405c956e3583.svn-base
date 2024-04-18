/***************************** Copyright ****************************************
*
* (C) Copyright 2022, Shaanxi Tianji Comunication
* All Rights Reserved
*
* FileName   : bsp_basic.c
* Version    : none
* Author     : yangyang
* Date       : 2022-08-03
* Description: 移植时修改此文件,包括数据收发处理,文件存储,完成时的回调函数
*******************************************************************************/

#include "Ymodem.h"
//#include "bsp_pc.h"
//#include "bsp_p25qxx.h"
#include "rtt.h"
#include "hi_usb.h"
#include "hi_flash.h"

/*---------------------- 全局变量 --------------------------------------*/

static uint32_t flash_addr_start;       // 接收文件目标flash地址
static uint32_t flash_wr_addr;          // 当前写flash地址

//static size_t fil_rx_sz; // 传输的文件大小
//static char *fil_tx_nm;  // 传输的文件名称
flash_type_t flash_type = FLASH_ON_CHIP;
uint8_t data_buffer[2048]={0};
/*---------------------- 宏定义 ---------------------------------------*/

/*---------------------- 结构体 ----------------------------------------*/


/*---------------------- 接收端函数接口 ----------------------------------------*/

/*****************************************************************************
* @Name	  : ymodem_set_rx_flash_addr
* @Brief  : 
* @Param  : dest_addr: 从ymodem收到的文件,要存到的flash起始地址
* @Retval : 
* @Author : 
* @Date   : 
* @Note   :
*****************************************************************************/
void ymodem_set_rx_flash_addr(flash_type_t type,uint32_t dest_addr)
{
    flash_addr_start = dest_addr;
    flash_type = type;
}

/*****************************************************************************
* @Name	  : ymodem_rx_file_init
* @Brief  : 分配数字衰减器的端口和管脚
* @Param  : dest_addr: 从ymodem收到的文件,要存到的flash起始地址
* @Retval : 
* @Author : 
* @Date   : 
* @Note   :
*****************************************************************************/
uint8_t ymodem_rx_file_init(char *fil_nm, size_t fil_sz)
{
    if(flash_type == FLASH_ON_BOARD)
    {
        flash_wr_addr = flash_addr_start;
        ;//P25QXX_Erase_Sector(flash_addr_start/4096);
    }
    else if(flash_type == FLASH_ON_CHIP)
    {
        flash_wr_addr = flash_addr_start;
        //t_flash_erase(T_FLASH_PART_USER,0,0xff); //整区擦除
    }else if(flash_type == RAM_ON_CHIP)
    {
        memset(data_buffer,0,sizeof(data_buffer));
    }
    return YMODEM_OK;
}

/*****************************************************************************
* @Name	  : ymodem_rx_exit
* @Brief  : 分配数字衰减器的端口和管脚
* @Param  : dest_addr: 从ymodem收到的文件,要存到的flash起始地址
* @Retval : 
* @Author : 
* @Date   : 
* @Note   :
*****************************************************************************/
uint8_t ymodem_rx_exit(void)
{
    flash_wr_addr = 0;
    flash_addr_start = 0;
    return YMODEM_OK;
}
// 保存收到的包
// 每写满一个sector就擦除新的sector,并继续写入
uint8_t ymodem_rx_pac_store_handler(char *buf, size_t seek, size_t size)
{
    if(flash_type == FLASH_ON_BOARD)
    {
        uint32_t write_size, remain_size;
        static uint32_t next_sector_addr; 
        next_sector_addr = (flash_wr_addr / 4096 + 1) * 4096;
        if(flash_wr_addr + size >= next_sector_addr)
        {
            write_size = next_sector_addr - flash_wr_addr;
            remain_size = size - write_size;

            //P25QXX_Write((uint8_t*)buf, flash_wr_addr, write_size);
            flash_wr_addr += write_size;
            //P25QXX_Erase_Sector(flash_wr_addr/4096);
            //P25QXX_Write((uint8_t*)buf, next_sector_addr, remain_size);
            flash_wr_addr += remain_size;
        }else
        {
            write_size = size;
            //P25QXX_Write((uint8_t*)buf, flash_wr_addr, write_size);
            flash_wr_addr += write_size;
        }
    }
    else if(flash_type == FLASH_ON_CHIP)
    {
        ;
    }else if(flash_type == RAM_ON_CHIP)
    {
        memcpy(&data_buffer[seek],(uint8_t*)buf,size);
    }

    return YMODEM_OK;
}

// 收到EOT,结束传输
// 传输结束后的事件处理
// 对文件metadata的检查
// 这里可提供一个回调函数,用于处理传输结束后的事件,或提供一个事件,用于其他任务比如升级任务处理后续工作
uint8_t rd_buf[2048] = {0};
uint8_t ymodem_rx_finish_handler(uint8_t status)
{
    //TODO:用事件组或消息队列输出结果给调用者
    //func();
    
    if(flash_type == FLASH_ON_BOARD)
    {
        ymodem_rx_exit();
    }
    else if(flash_type == FLASH_ON_CHIP)
    {
        ;
    }else if(flash_type == RAM_ON_CHIP)
    {
        rtt_printf( RTT_UPGRADE, "<y-modem>file read: \r\n%s\r\n",data_buffer);
        memset(data_buffer,0,sizeof(data_buffer));
    }
    return YMODEM_OK;
}

/*---------------------- 发送端函数接口 ----------------------------------------*/

/*********************************************************************
 * @fn      ymodem_tx_file_init : 系统调用，用来获取文件名和大小
 * @param   fil_nm : 文件名 fil_sz : 文件大小
 */
uint8_t ymodem_tx_file_init(char **fil_nm, size_t *fil_sz)
{
    uint8_t ans = YMODEM_ERR;
    // FRESULT res;
    // cstr_t *px_cstr;
    // if (NULL != fil_tx_nm) // 待传输的文件名不为空
    // {
    //     if (fdst = (FIL *)pvPortMalloc(sizeof(FIL))) // 文件指针申请成功
    //     {
    //         // 打开文件
    //         if (px_cstr = cstr_create("%s\\%s", px_path_cur->string, fil_tx_nm))
    //         {
    //             res = f_open(fdst, px_cstr->string, FA_READ | FA_OPEN_EXISTING);
    //             cstr_free(px_cstr);
    //             if (FR_OK == res)
    //             {
    //                 *fil_nm = fil_tx_nm;   // 设置文件名
    //                 *fil_sz = fdst->fsize; // 设置文件大小
    //                 ans = YMODEM_OK;       // 文件发送准备已完成
    //             }
    //         }
    //     }
    // }
    return ans;
}
/*********************************************************************
 * @fn      ymodem_tx_finish_handler : 当传输结束时，会被调用
 * @param   status : 关闭的原因 YMODEM_OK 或 YMODEM_ERR
 */
uint8_t ymodem_tx_finish_handler(uint8_t status) // 返回结束原因，成功还是出错
{
    // if (fdst)
    // {
    //     f_close(fdst);
    //     vPortFree(fdst);
    //     fdst = NULL;
    //     vPortFree(fil_tx_nm);
    //     fil_tx_nm = NULL;
    // }
    return YMODEM_OK;
}
/*********************************************************************
 * @fn      ymodem_tx_pac_store_handler : 调用此来读取文件中的相应数据
 * @param   buf : 待写入的缓冲区地址 offset : 数据的偏移 size : 数据的大小
 */
uint8_t ymodem_tx_pac_store_handler(char *buf, size_t offset, size_t size)
{
    uint8_t ans = YMODEM_ERR;
    // uint32_t wd;
    // if (fdst)
    // {
    //     if (FR_OK == f_lseek(fdst, seek))
    //     {
    //         if (fil_rx_sz < seek + size)
    //             size = fil_rx_sz - seek;
    //         if (FR_OK == f_read(fdst, buf, size, &wd))
    //         {
    //             ans = YMODEM_OK;
    //         }
    //     }
    // }
    return ans;
}

// 数据发送接口
// 根据实际使用的硬件外设来修改这两个接口
void __putchar(char ch)
{
    char data = ch;
    hi_usb_send((uint8_t*)&data,1);
    rtt_printf( RTT_UPGRADE, "<y-modem>tx: 0x%02x\r\n",ch);
}
void __putbuf(char *buf, size_t len)
{
    hi_usb_send((uint8_t*)buf,len);
}

