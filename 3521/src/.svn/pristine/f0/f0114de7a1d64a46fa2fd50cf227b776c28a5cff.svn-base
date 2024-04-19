/***************************** Copyright ****************************************
*
* (C) Copyright 2023, Shaanxi Tianji Communication
*  All Rights Reserved
*
* FileName   : bsp_testmode_save.c
* Version    : none
* Author     :
* Date       : 2023-12-19
* Description: none
*******************************************************************************/
#include "main.h"
//#include "stdlib.h"
//#include "stdio.h"
//#include "string.h"
//#include "MCU_config.h"
#include "system_config.h"
#include "bsp_config.h"
#include "public_define.h"
#include "public_enum.h"
#include "public_struct.h"
#include "bsp_testmode_save.h"
//#include "bsp_testmode.h"
#include "bsp_flash_addr.h"
#include "rtt.h"
#include "bsp_dev_para.h"
#include "bsp_public_drv.h"


/*---------------------- 端口定义 --------------------------------------*/


/*---------------------- 宏定义  ---------------------------------------*/
#define START_FACTORY_BYTE	0x55AA

/*---------------------- 结构体 ----------------------------------------*/


/*---------------------- 常量 ------------------------------------------*/


/*---------------------- 全局变量 --------------------------------------*/


/*---------------------- 局部变量 --------------------------------------*/
// #if(PRODUCT_MODEL == CMDT3521C)
// __align(32) uint8_t tm_save_data[4096] __attribute__((section("CCMRAM")));
// __align(32) tm_flash_factory  t_factory_data __attribute__((section("CCMRAM")));
// #else
// uint8_t tm_save_data[4096];
// tm_flash_factory  t_factory_data;
// #endif

uint8_t tm_save_data[4096];
tm_flash_factory  t_factory_data;

tm_flash_factory * t_tm_factory = &t_factory_data;  //出厂的公共参数
tm_flash_rf   *   t_tm_rf      = (tm_flash_rf *)tm_save_data;
tm_flash_8668  *  t_tm_8668    = (tm_flash_8668 *)tm_save_data;

/*---------------------- 声明 ------------------------------------------*/

/*****************************************************************************
* @Name	  : flash_dev_para_restore_factory_setting
* @Brief  : 设备校准公共参数恢复出厂设置
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2024-01-18
* @Note   :
*****************************************************************************/
static void flash_cali_factory_restore_factory_setting(void)
{
    rtt_printf(RTT_FLASH, RTT_TEXT_CYAN"[%s restore factory L%d]: flash cali factory Para restore factory setting\n", __MODULE__, __LINE__);
    
    memset(&t_factory_data, 0x00, sizeof(t_factory_data));
    t_factory_data.start = START_FACTORY_BYTE;
    
    // 写入默认版本
    memcpy(t_factory_data.sw_ver, (uint8_t *)SW_VERSION, strlen((char *)SW_VERSION));    // 软件版本
    memcpy(t_factory_data.hw_ver, (uint8_t *)HW_VERSION, strlen((char *)HW_VERSION));    // 硬件版本
}


/*****************************************************************************
* @Name	  : flash_cali_factory_aligent
* @Brief  : 设备校准公共参数读写对齐
* @Param  : rw: [输入/出]
* @Retval :
* @Author :
* @Data   : 2024-01-19
* @Note   :
*****************************************************************************/
static void flash_cali_factory_aligent(uint8_t rw)
{
    switch(rw)
    {
        case READ:
            rtt_printf(RTT_FLASH, RTT_TEXT_GREEN"[%s read L%d]: read cali factory Para from ex_flash...\n\n", __MODULE__, __LINE__);
            
            memcpy(tDevFactoryPara.sn, t_factory_data.sn, sizeof(tDevFactoryPara.sn));              // sn
            //memcpy(tDevFactoryPara.swVer, t_factory_data.sw_ver, sizeof(tDevFactoryPara.swVer));    // 软件版本
			memset( tDevFactoryPara.swVer, 0, sizeof( tDevFactoryPara.swVer ) );
			memcpy(tDevFactoryPara.swVer, (uint8_t *)SW_VERSION, strlen((char *)SW_VERSION));    // 软件版本
            memcpy(tDevFactoryPara.hwVer, t_factory_data.hw_ver, sizeof(tDevFactoryPara.hwVer));    // 硬件版本
            memcpy(tDevFactoryPara.license, t_factory_data.license, sizeof(tDevFactoryPara.license));
            break;

        case WRITE:
            rtt_printf(RTT_FLASH, RTT_TEXT_YELLOW"[%s write L%d]: write cali factory Para\n", __MODULE__, __LINE__);

            memcpy(t_factory_data.sn, tDevFactoryPara.sn, sizeof(tDevFactoryPara.sn));              // sn
            memcpy(t_factory_data.sw_ver, tDevFactoryPara.swVer, sizeof(tDevFactoryPara.swVer));    // 软件版本
            memcpy(t_factory_data.hw_ver, tDevFactoryPara.hwVer, sizeof(tDevFactoryPara.hwVer));    // 硬件版本
            memcpy(t_factory_data.license, tDevFactoryPara.license, sizeof(tDevFactoryPara.license));
            break;

        default:
            break;
    }
}


/*****************************************************************************
* @Name	  : flash_restore_dev_factory
* @Brief  : 恢复校准数据公共参数出厂设置
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2024-01-18
* @Note   :
*****************************************************************************/
void flash_restore_cali_factory(void)
{
    uint32_t addr = get_cali_factory_data_save_addr();
    uint16_t flash_sector = spi_flash.flash_addr_to_sector(addr);

    rtt_printf(RTT_FLASH, RTT_TEXT_MAGENTA"[%s restore_cali_factory L%d]: restore cali factory of ex_flash\n", __MODULE__,  __LINE__);
    flash_cali_factory_restore_factory_setting();
    t_tm_factory = &t_factory_data;
    
    taskENTER_CRITICAL();
    spi_flash.flash_erase_sector(flash_sector);;
    spi_flash.flash_write((uint8_t *)t_tm_factory, addr, sizeof(t_factory_data)); //写默认数据
    taskEXIT_CRITICAL();
}


/*****************************************************************************
* @Name	  : tm_read_factory_data
* @Brief  : 读取出厂的公共参数
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2023-12-19
* @Note   : 开机需要读取
*****************************************************************************/
void tm_read_cali_factory_data(void)
{
    #define  factroy_data_size  256

    taskENTER_CRITICAL();
    spi_flash.flash_read((uint8_t *)t_tm_factory, CALI_DATA_PUBLIC_ADDR, factroy_data_size);   //读出数据
    taskEXIT_CRITICAL();

    if(t_tm_factory->start == START_FACTORY_BYTE)
        flash_cali_factory_aligent(READ);
    else
        flash_restore_cali_factory();
}


/*****************************************************************************
* @Name	  : tm_save_cali_factory_para
* @Brief  : 保存出厂的公共参数
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2023-12-19
* @Note   :
*****************************************************************************/
void tm_save_cali_factory_para(void)
{
    Status_t retval = STAT_ERROR;
    
    uint32_t addr = get_cali_factory_data_save_addr();
    uint16_t flash_sector = spi_flash.flash_addr_to_sector(addr);
    
    flash_cali_factory_aligent(WRITE);
    t_tm_factory = &t_factory_data;

    taskENTER_CRITICAL();
    spi_flash.flash_erase_sector(flash_sector);;
    retval = spi_flash.flash_write((uint8_t *)t_tm_factory, addr, sizeof(t_factory_data)); //写入数据
    taskEXIT_CRITICAL();
    
    if(STAT_ERROR == retval)
    {
        rtt_printf(RTT_FLASH, RTT_TEXT_RED"[%s write error L%d]: cali factory Para write of ex_flash error\n", __MODULE__, __LINE__);
        flash_restore_cali_factory();
    }
}



/*****************************************************************************
* @Name	  : tm_get_rf_cali_sector_addr
* @Brief  : 获取射频校准参数页首址
* @Param  : band: [输入/出]
* @Retval :
* @Author :
* @Data   : 2023-12-21
* @Note   :
*****************************************************************************/
uint32_t tm_get_rf_cali_sector_addr(uint8_t band)
{
    return get_cali_data_save_addr(band, CALI_TYPE_RF);
}


/*****************************************************************************
* @Name	  : tm_get_8668_cali_sector_addr
* @Brief  : 获取8668校准参数页首址
* @Param  : sys: [输入/出]
**			 band: [输入/出]
* @Retval :
* @Author :
* @Data   : 2023-12-21
* @Note   :
*****************************************************************************/
uint32_t tm_get_8668_cali_sector_addr(uint8_t band)
{
    return get_cali_data_save_addr(band, CALI_TYPE_8668);
}


/*****************************************************************************
* @Name	  : tm_read_sector_cali_para
* @Brief  : 读取指定BAND所在扇区的参数
* @Param  : band: [输入/出]
* @Retval :
* @Author :
* @Data   : 2023-12-20
* @Note   :
*****************************************************************************/
void tm_read_cali_rf_para(uint8_t band)
{
    uint32_t addr = tm_get_rf_cali_sector_addr(band);

    memset(tm_save_data, 0, sizeof(tm_save_data));
    t_tm_rf = (tm_flash_rf *)tm_save_data;
    
    rtt_printf(RTT_FLASH, RTT_TEXT_GREEN"[%s read L%d]: %s read cali rf Para from ex_flash addr = 0x%08x...\n", __MODULE__, __LINE__, rf_band_name[band], addr);
    
    taskENTER_CRITICAL();
    spi_flash.flash_read((uint8_t *)tm_save_data, addr, 1024 * 4); //读出数据
    taskEXIT_CRITICAL();
}


/*****************************************************************************
* @Name	  : tm_save_cali_rf_para
* @Brief  : 保存校准的射频参数
* @Param  : addr: [输入/出]
* @Retval :
* @Author :
* @Data   : 2024-01-10
* @Note   :
*****************************************************************************/
void tm_save_cali_rf_para(uint8_t band)
{
    Status_t retval = STAT_ERROR;
    
    uint32_t addr = tm_get_rf_cali_sector_addr(band);
    uint16_t flash_sector = spi_flash.flash_addr_to_sector(addr);        //bi_flash_addr_to_sector( addr );	//flash 扇区地址

    t_tm_rf = (tm_flash_rf *)tm_save_data;

    rtt_printf(RTT_FLASH, RTT_TEXT_YELLOW"[%s write L%d]: %s write cali rf Para\n", __MODULE__, __LINE__, rf_band_name[band]);
    
    taskENTER_CRITICAL();
    spi_flash.flash_erase_sector(flash_sector);
    retval = spi_flash.flash_write((uint8_t *)t_tm_rf, addr, 1024 * 4); //写入数据

    memset(tm_save_data, 0, sizeof(tm_save_data));

    spi_flash.flash_read((uint8_t *)tm_save_data, addr, 1024 * 4); //读出数据

    taskEXIT_CRITICAL();
    
    if(STAT_ERROR == retval)
    {
        rtt_printf(RTT_FLASH, RTT_TEXT_RED"[%s write error L%d]: %s cali rf write of ex_flash error\n", __MODULE__, __LINE__, rf_band_name[band]);
    }
}


