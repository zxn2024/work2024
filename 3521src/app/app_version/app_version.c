/***************************** Copyright ****************************************
*
* (C) Copyright 2022, Shaanxi Tianji Comunication
*  All Rights Reserved
*
* FileName   : bsp_version.c
* Version    : none
* Author     : none
* Date       : 2024-02-26
* Description: version
*******************************************************************************/
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "main.h"
#include "crc.h"
#include "bsp_config.h"
#include "bsp_reboot.h"
#include "app_upgrade.h"
#include "app_version.h"

/*---------------------- 端口定义 --------------------------------------*/


/*---------------------- 宏定义  ---------------------------------------*/
#define VER_ERROR_ADDR  0xFFFFFFFF/*错误地址*/

/*事件组标志位*/
#define BIT_UPGRADE_EGP_IDLE            (1U << 0) 
#define BIT_TIME_VER_TIMEOUT_EN         (1U << 1)

/*监控更新包接收超时，3min*/
#define VER_PACKET_REV_TIMEOUT_CNT      (1800)   /*3*60*10=1800*/  

/*boot信息开始字节*/
#define BOOTLOADER_START_BYTE   ( 0x55AA )

/*---------------------- 结构体 ----------------------------------------*/	


/*---------------------- 常量 ------------------------------------------*/


/*---------------------- 全局变量 --------------------------------------*/
static uint8_t ver_UpdateFunc_Reset(uint32_t verIndex);
static uint8_t ver_UpdateFunc_McuBoot(uint32_t verIndex);
static uint8_t ver_UpdateFunc_McuApp(uint32_t verIndex);

#if (LOCAL_MODULE_TYPE == _MU)
static uint8_t ver_UpdateFunc_SoftVersion(uint32_t verIndex);
static uint8_t ver_UpdateFunc_5GModem(uint32_t verIndex);
#endif

static stVerTable g_staVerTable[]={
#if LOCAL_MODULE_TYPE == _MU
	{VER_TYPE_WORK, VER_DEVICE_MU,    VER_MODULE_MCU_BOOT,  VER_FLASH_WORK_MCU_BOOT_S_SECTOR, VER_FLASH_WORK_MCU_BOOT_E_SECTOR, 0, 0, ver_UpdateFunc_McuBoot},
	{VER_TYPE_WORK, VER_DEVICE_MU,    VER_MODULE_MCU_APP,   VER_FLASH_WORK_MCU_APP_S_SECTOR,  VER_FLASH_WORK_MCU_APP_E_SECTOR,  0, 0, ver_UpdateFunc_McuApp},
	{VER_TYPE_WORK, VER_DEVICE_MU,    VER_MODULE_FPGA,      VER_FLASH_WORK_FPGA_S_SECTOR,     VER_FLASH_WORK_FPGA_E_SECTOR,     0, 0, ver_UpdateFunc_Reset},
	{VER_TYPE_WORK, VER_DEVICE_MU,    VER_MODULE_8668_BIN1, VER_FLASH_WORK_8668_B3_S_SECTOR,  VER_FLASH_WORK_8668_B3_E_SECTOR,  0, 0, ver_UpdateFunc_Reset},
	{VER_TYPE_WORK, VER_DEVICE_MU,    VER_MODULE_8668_BIN2, VER_FLASH_WORK_8668_B8_S_SECTOR,  VER_FLASH_WORK_8668_B8_E_SECTOR,  0, 0, ver_UpdateFunc_Reset},
	{VER_TYPE_WORK, VER_DEVICE_MU,    VER_MODULE_8668_BIN3, VER_FLASH_WORK_8668_B39_S_SECTOR, VER_FLASH_WORK_8668_B39_E_SECTOR, 0, 0, ver_UpdateFunc_Reset},
	{VER_TYPE_WORK, VER_DEVICE_MU,    VER_MODULE_8668_BIN4, VER_FLASH_WORK_8668_B40_S_SECTOR, VER_FLASH_WORK_8668_B40_E_SECTOR, 0, 0, ver_UpdateFunc_Reset},
	{VER_TYPE_WORK, VER_DEVICE_MU,    VER_MODULE_8668_BIN5, VER_FLASH_WORK_8668_N41_S_SECTOR, VER_FLASH_WORK_8668_N41_E_SECTOR, 0, 0, ver_UpdateFunc_Reset},	
	{VER_TYPE_WORK, VER_DEVICE_RU,    VER_MODULE_MCU_BOOT,  VER_FLASH_RU_MCU_BOOT_S_SECTOR,   VER_FLASH_RU_MCU_BOOT_E_SECTOR,   0, 0, ver_UpdateFunc_SoftVersion},
	{VER_TYPE_WORK, VER_DEVICE_RU,    VER_MODULE_MCU_APP,   VER_FLASH_RU_MCU_APP_S_SECTOR,    VER_FLASH_RU_MCU_APP_E_SECTOR,    0, 0, ver_UpdateFunc_SoftVersion},
	{VER_TYPE_WORK, VER_DEVICE_RU,    VER_MODULE_FPGA,      VER_FLASH_RU_FPGA_S_SECTOR,       VER_FLASH_RU_FPGA_E_SECTOR,       0, 0, ver_UpdateFunc_SoftVersion},
	{VER_TYPE_WORK, VER_DEVICE_RU,    VER_MODULE_8668_BIN1, VER_FLASH_RU_8668_B3_S_SECTOR,    VER_FLASH_RU_8668_B3_E_SECTOR,    0, 0, ver_UpdateFunc_SoftVersion},
	{VER_TYPE_WORK, VER_DEVICE_RU,    VER_MODULE_8668_BIN2, VER_FLASH_RU_8668_B8_S_SECTOR,    VER_FLASH_RU_8668_B8_E_SECTOR,    0, 0, ver_UpdateFunc_SoftVersion},
	{VER_TYPE_WORK, VER_DEVICE_RU,    VER_MODULE_8668_BIN3, VER_FLASH_RU_8668_B39_S_SECTOR,   VER_FLASH_RU_8668_B39_E_SECTOR,   0, 0, ver_UpdateFunc_SoftVersion},
	{VER_TYPE_WORK, VER_DEVICE_RU,    VER_MODULE_8668_BIN4, VER_FLASH_RU_8668_B40_S_SECTOR,   VER_FLASH_RU_8668_B40_E_SECTOR,   0, 0, ver_UpdateFunc_SoftVersion},
	{VER_TYPE_WORK, VER_DEVICE_RU,    VER_MODULE_8668_BIN5, VER_FLASH_RU_8668_N41_S_SECTOR,   VER_FLASH_RU_8668_N41_E_SECTOR,   0, 0, ver_UpdateFunc_SoftVersion},	
	{VER_TYPE_WORK, VER_DEVICE_OTHER, VER_MODULE_5G_MODEM,  VER_FLASH_WORK_5G_MODEM_S_SECTOR, VER_FLASH_WORK_5G_MODEM_E_SECTOR, 0, 0, ver_UpdateFunc_5GModem},
	{VER_TYPE_BAK,  VER_DEVICE_MU,    VER_MODULE_MCU_BOOT,  VER_FLASH_BAK_MCU_BOOT_S_SECTOR,  VER_FLASH_BAK_MCU_BOOT_E_SECTOR,  0, 0, NULL},
	{VER_TYPE_BAK,  VER_DEVICE_MU,    VER_MODULE_MCU_APP,   VER_FLASH_BAK_MCU_APP_S_SECTOR,   VER_FLASH_BAK_MCU_APP_E_SECTOR,   0, 0, NULL},
	{VER_TYPE_BAK,  VER_DEVICE_MU,    VER_MODULE_FPGA,      VER_FLASH_BAK_FPGA_S_SECTOR,      VER_FLASH_BAK_FPGA_E_SECTOR,      0, 0, NULL},
	{VER_TYPE_BAK,  VER_DEVICE_MU,    VER_MODULE_8668_BIN1, VER_FLASH_BAK_8668_B3_S_SECTOR,   VER_FLASH_BAK_8668_B3_E_SECTOR,   0, 0, NULL},
	{VER_TYPE_BAK,  VER_DEVICE_MU,    VER_MODULE_8668_BIN2, VER_FLASH_BAK_8668_B8_S_SECTOR,   VER_FLASH_BAK_8668_B8_E_SECTOR,   0, 0, NULL},
	{VER_TYPE_BAK,  VER_DEVICE_MU,    VER_MODULE_8668_BIN3, VER_FLASH_BAK_8668_B39_S_SECTOR,  VER_FLASH_BAK_8668_B39_E_SECTOR,  0, 0, NULL},
	{VER_TYPE_BAK,  VER_DEVICE_MU,    VER_MODULE_8668_BIN4, VER_FLASH_BAK_8668_B40_S_SECTOR,  VER_FLASH_BAK_8668_B40_E_SECTOR,  0, 0, NULL},
	{VER_TYPE_BAK,  VER_DEVICE_MU,    VER_MODULE_8668_BIN5, VER_FLASH_BAK_8668_N41_S_SECTOR,  VER_FLASH_BAK_8668_N41_E_SECTOR,  0, 0, NULL},	
	{VER_TYPE_BAK,  VER_DEVICE_OTHER, VER_MODULE_5G_MODEM,  VER_FLASH_BAK_5G_MODEM_S_SECTOR,  VER_FLASH_BAK_5G_MODEM_E_SECTOR,  0, 0, NULL}
#else
	{VER_TYPE_WORK, VER_DEVICE_RU,    VER_MODULE_MCU_BOOT,  VER_FLASH_WORK_MCU_BOOT_S_SECTOR, VER_FLASH_WORK_MCU_BOOT_E_SECTOR, 0, 0, ver_UpdateFunc_McuBoot},
	{VER_TYPE_WORK, VER_DEVICE_RU,    VER_MODULE_MCU_APP,   VER_FLASH_WORK_MCU_APP_S_SECTOR,  VER_FLASH_WORK_MCU_APP_E_SECTOR,  0, 0, ver_UpdateFunc_McuApp},
	{VER_TYPE_WORK, VER_DEVICE_RU,    VER_MODULE_FPGA,      VER_FLASH_WORK_FPGA_S_SECTOR,     VER_FLASH_WORK_FPGA_E_SECTOR,     0, 0, ver_UpdateFunc_Reset},
	{VER_TYPE_WORK, VER_DEVICE_RU,    VER_MODULE_8668_BIN1, VER_FLASH_WORK_8668_B3_S_SECTOR,  VER_FLASH_WORK_8668_B3_E_SECTOR,  0, 0, ver_UpdateFunc_Reset},
	{VER_TYPE_WORK, VER_DEVICE_RU,    VER_MODULE_8668_BIN2, VER_FLASH_WORK_8668_B8_S_SECTOR,  VER_FLASH_WORK_8668_B8_E_SECTOR,  0, 0, ver_UpdateFunc_Reset},
	{VER_TYPE_WORK, VER_DEVICE_RU,    VER_MODULE_8668_BIN3, VER_FLASH_WORK_8668_B39_S_SECTOR, VER_FLASH_WORK_8668_B39_E_SECTOR, 0, 0, ver_UpdateFunc_Reset},
	{VER_TYPE_WORK, VER_DEVICE_RU,    VER_MODULE_8668_BIN4, VER_FLASH_WORK_8668_B40_S_SECTOR, VER_FLASH_WORK_8668_B40_E_SECTOR, 0, 0, ver_UpdateFunc_Reset},
	{VER_TYPE_WORK, VER_DEVICE_RU,    VER_MODULE_8668_BIN5, VER_FLASH_WORK_8668_N41_S_SECTOR, VER_FLASH_WORK_8668_N41_E_SECTOR, 0, 0, ver_UpdateFunc_Reset},
	{VER_TYPE_BAK,  VER_DEVICE_RU,    VER_MODULE_MCU_BOOT,  VER_FLASH_BAK_MCU_BOOT_S_SECTOR,  VER_FLASH_BAK_MCU_BOOT_E_SECTOR,  0, 0, NULL},
	{VER_TYPE_BAK,  VER_DEVICE_RU,    VER_MODULE_MCU_APP,   VER_FLASH_BAK_MCU_APP_S_SECTOR,   VER_FLASH_BAK_MCU_APP_E_SECTOR,   0, 0, NULL},
	{VER_TYPE_BAK,  VER_DEVICE_RU,    VER_MODULE_FPGA,      VER_FLASH_BAK_FPGA_S_SECTOR,      VER_FLASH_BAK_FPGA_E_SECTOR,      0, 0, NULL},
	{VER_TYPE_BAK,  VER_DEVICE_RU,    VER_MODULE_8668_BIN1, VER_FLASH_BAK_8668_B3_S_SECTOR,   VER_FLASH_BAK_8668_B3_E_SECTOR,   0, 0, NULL},
	{VER_TYPE_BAK,  VER_DEVICE_RU,    VER_MODULE_8668_BIN2, VER_FLASH_BAK_8668_B8_S_SECTOR,   VER_FLASH_BAK_8668_B8_E_SECTOR,   0, 0, NULL},
	{VER_TYPE_BAK,  VER_DEVICE_RU,    VER_MODULE_8668_BIN3, VER_FLASH_BAK_8668_B39_S_SECTOR,  VER_FLASH_BAK_8668_B39_E_SECTOR,  0, 0, NULL},
	{VER_TYPE_BAK,  VER_DEVICE_RU,    VER_MODULE_8668_BIN4, VER_FLASH_BAK_8668_B40_S_SECTOR,  VER_FLASH_BAK_8668_B40_E_SECTOR,  0, 0, NULL},
	{VER_TYPE_BAK,  VER_DEVICE_RU,    VER_MODULE_8668_BIN5, VER_FLASH_BAK_8668_N41_S_SECTOR,  VER_FLASH_BAK_8668_N41_E_SECTOR,  0, 0, NULL}	
#endif	
};
static uint32_t g_ulVerTableLen = sizeof(g_staVerTable)/sizeof(stVerTable);

static stVerUpgradeFormat g_stVerUpgradeFormat = {0};

static uint16_t g_usVerTimeOut = 0; 

static EventGroupHandle_t g_xVerEgp = NULL;    /* 事件组 */

static uint8_t g_ucVerFileLoadFlag = 0;

/*---------------------- 局部变量 --------------------------------------*/


/*---------------------- 函数声明 --------------------------------------*/



/*---------------------- 函数定义 --------------------------------------*/
/*****************************************************************************
* @Name	  :  ver_FileInfo
* @Brief  :  读取文件信息
* @Param  :  addr: 文件存放地址
**           pst: 输出文件信息
* @Retval :  执行结果
* @Author :
* @Data   :  2024-02-26
* @Note   :  文件头结构
**           1	start byte	   起始字节	   2	0x00 ~ 0x01	   起始字节, 0x55AA	
**           2	设备类型	   设备类型	   1	0x02	       0x01:  主机 
**                                                             0x02:  从机 
**                                                             0x03:  三级从机 
**                                                             0x04:  HUB 
**                                                             0xF0:  无差别设备	
**           3	模块类型	   未定义	   1	0x03	       0x00:  MCU_BOOT 
**                                                             0x01:  MCU_APP 
**                                                             0x02:  BLE 
**                                                             0x03:  FPGA 
**                                                             0x04:  5G MODEM 
**                                                             0x05:  监控模组 
**                                                             0x06:  8668 bin文件1(B3) 
**                                                             0x07:  8668 bin文件2(B8) 
**                                                             0x08:  8668 bin文件3(B39) 
**                                                             0x09:  8668 bin文件4(B40) 
**                                                             0x0A:  8668 bin文件5(N41) 
**                                                             0x0B:  8668 bin文件6 
**                                                             0x0C:  8668 bin文件7 
**                                                             0x0D:  8668 bin文件8 
**                                                             0x0E:  8668 bin文件9 
**                                                             0x0F:  8668 bin文件10	
**           4	mcu type	   MCU型号	   32	0x04 ~ 0x23	   MCU型号, 字符串 
**                                                             GD32F427ZET6 
**                                                             STM32F407ZET6	
**           5	hw version	   硬件版本	   32	0x24 ~ 0x43	   硬件版本, 字符串（HW_VERNUM）: 
**                                                             V1.0	
**           6	5G模组型号		           1	0x44	       0x01: 自研cat1	
**           7	监控模组型号		       1	0x45	       0x01: EC800G 
**                                                             0x02: N706 
**                                                             0x03: MC661	
**           8	BLE type		           1	0x46	       0x01: ST-LP-355	
**           9	FPGA type		           1	0x47	       0x01: SL2E-5E	
**           10	soft version   软件版本    4	0x48 ~ 0x4B	   软件版本（SW_VERNUM） 十进制23010101	
**           11	file length	   文件长度	   4	0x4C ~ 0x4F	   原始可执行的bin文件长度,(不包含文件参数长度 )	
**           12	file valid	   文件有效性  4	0x50 ~ 0x53	   取原始BIN文件位于0x1000~0x1003字节的32位数据 Data1和位于0x1FFC~0x1FFF字节的32位数据 Data2进行异或	
**           13	file_crc32	   正确性验证  4	0x54 ~ 0x57	   原始Bin文件的CRC校验码	
**           14	reserve	       预留字节	   36	0x58 ~ 0x7B	   预留字节	
**           15	para_crc32	   参数区CRC   4	0x7C - 0x7F	   参数字节的CRC校验值, 从start_byte 到 reserve最后一个字节, 共124个字节	
*****************************************************************************/
static uint8_t ver_FileInfo(uint32_t addr, stVerFileInfo *pst)
{
	uint8_t ucaData[128];
	uint32_t ulCrc, ulCrcS;
	uint16_t usStartByte;
	
	/*读FLASH*/
	if(STAT_OK != spi_flash.flash_read(ucaData, addr, 128))
	{
		rtt_printf(RTT_UPGRADE, RTT_TEXT_RED"ver_FileInfo: flash_read 0x%x fail!\r\n", addr);
		return VER_EXE_FAIL;/*执行失败*/
	}

	/*开始字节校验*/
	usStartByte = (ucaData[1] << 8) | ucaData[0];
	if(usStartByte != VER_FILE_HEAD_START_BYTE)
	{
		rtt_printf(RTT_UPGRADE, RTT_TEXT_RED"ver_FileInfo: 0x%x usStartByte=0x%x!\r\n", addr, usStartByte);		
		return VER_EXE_FAIL;/*执行失败*/
	}

	/*文件头CRC校验*/
	ulCrcS = (ucaData[0x7C]) | (ucaData[0x7D]<<8) | (ucaData[0x7E]<<16) | (ucaData[0x7F]<<24);
	ulCrc = crc32(ucaData, 124);
	if(ulCrcS != ulCrc)
	{
		rtt_printf(RTT_UPGRADE, RTT_TEXT_RED"ver_FileInfo: 0x%x crc 0x%x != 0x%x\r\n", addr, ulCrcS, ulCrc);		
		return VER_EXE_FAIL;/*执行失败*/
	}	
	
	pst->ucDeviceType = ucaData[0x2];
	pst->ucModuleType = ucaData[0x3];
	memcpy(pst->ucaMcuType, &(ucaData[0x4]), 32);
	pst->ulSoftVersion  = (ucaData[0x48]) | (ucaData[0x49]<<8) | (ucaData[0x4A]<<16) | (ucaData[0x4B]<<24);
	pst->ulBinFileLen   = (ucaData[0x4C]) | (ucaData[0x4D]<<8) | (ucaData[0x4E]<<16) | (ucaData[0x4F]<<24);
	pst->ulBinFileValid = (ucaData[0x50]<<24) | (ucaData[0x51]<<16) | (ucaData[0x52]<<8) | (ucaData[0x53]);
	pst->ulBinFileCrc32 = (ucaData[0x54]) | (ucaData[0x55]<<8) | (ucaData[0x56]<<16) | (ucaData[0x57]<<24);
	
	/*rtt_printf(RTT_UPGRADE, RTT_TEXT_YELLOW"ver_FileInfo: 0x%x  SoftVersion  %d\r\n", addr, pst->ulSoftVersion);*/
	/*rtt_printf(RTT_UPGRADE, RTT_TEXT_YELLOW"ver_FileInfo: 0x%x  BinFileLen   %d\r\n", addr, pst->ulBinFileLen);*/
	/*rtt_printf(RTT_UPGRADE, RTT_TEXT_YELLOW"ver_FileInfo: 0x%x  BinFileValid 0x%x\r\n", addr, pst->ulBinFileValid);*/
	/*rtt_printf(RTT_UPGRADE, RTT_TEXT_YELLOW"ver_FileInfo: 0x%x  BinFileCrc32 0x%x\r\n", addr, pst->ulBinFileCrc32);*/

    return VER_EXE_SUCCESS;
}

/*****************************************************************************
* @Name	  :  ver_FileCheck
* @Brief  :  文件校验 有效性 正确性(CRC32)
* @Param  :  addr: 文件存放地址
**           pst: 文件信息
* @Retval :  执行结果
* @Author :
* @Data   :  2024-02-26
* @Note   :  
*****************************************************************************/
static uint8_t ver_FileCheck(uint32_t addr, stVerFileInfo *pst)
{
	uint32_t ulCrc = 0;
	uint32_t ulAddr = addr+VER_FILE_HEAD_LEN;
	uint32_t ulLen;
	uint32_t ulData1, ulData2;
	
	/*有效性校验  取原始BIN文件位于0x1000~0x1003字节的32位数据 Data1和位于0x1FFC~0x1FFF字节的32位数据 Data2进行异或*/
	if(STAT_OK != spi_flash.flash_read(g_stVerUpgradeFormat.ucaData, ulAddr+0x1000, 4))	/*读FLASH*/
	{
		rtt_printf(RTT_UPGRADE, RTT_TEXT_RED"ver_FileCheck: flash_read 0x%x fail!\r\n", ulAddr+0x1000);
		return VER_EXE_FAIL;/*执行失败*/
	}		
	ulData1 = (g_stVerUpgradeFormat.ucaData[0x0]<<24) | (g_stVerUpgradeFormat.ucaData[0x1]<<16) | (g_stVerUpgradeFormat.ucaData[0x2]<<8) | g_stVerUpgradeFormat.ucaData[0x3];

	if(STAT_OK != spi_flash.flash_read(g_stVerUpgradeFormat.ucaData, ulAddr+0x1FFC, 4))	/*读FLASH*/
	{
		rtt_printf(RTT_UPGRADE, RTT_TEXT_RED"ver_FileCheck: flash_read 0x%x fail!\r\n", ulAddr+0x1FFC);
		return VER_EXE_FAIL;/*执行失败*/
	}	
	ulData2 = (g_stVerUpgradeFormat.ucaData[0x0]<<24) | (g_stVerUpgradeFormat.ucaData[0x1]<<16) | (g_stVerUpgradeFormat.ucaData[0x2]<<8) | g_stVerUpgradeFormat.ucaData[0x3];

	if((ulData1^ulData2) != pst->ulBinFileValid)
	{
		rtt_printf(RTT_UPGRADE, RTT_TEXT_RED"ver_FileCheck: Valid  0x%x != 0x%x\r\n", (ulData1^ulData2), pst->ulBinFileValid);
		return VER_EXE_FAIL;/*执行失败*/
	}		

	/*正确性校验  CRC32*/
	ulLen = pst->ulBinFileLen;
	while(ulLen > VER_UPGRADE_DATA_BUF_SIZE)
	{
		if(STAT_OK != spi_flash.flash_read(g_stVerUpgradeFormat.ucaData, ulAddr, VER_UPGRADE_DATA_BUF_SIZE))	/*读FLASH*/
		{	
			rtt_printf(RTT_UPGRADE, RTT_TEXT_RED"ver_FileCheck: flash_read 0x%x fail!\r\n", ulAddr);
			return VER_EXE_FAIL;/*执行失败*/
		}	
		ulCrc = crc32_s(ulCrc, g_stVerUpgradeFormat.ucaData, VER_UPGRADE_DATA_BUF_SIZE);
		ulAddr += VER_UPGRADE_DATA_BUF_SIZE;
		ulLen -= VER_UPGRADE_DATA_BUF_SIZE;
	}
	
	if(ulLen > 0)
	{
		if(STAT_OK != spi_flash.flash_read(g_stVerUpgradeFormat.ucaData, ulAddr, ulLen))	/*读FLASH*/
		{	
			rtt_printf(RTT_UPGRADE, RTT_TEXT_RED"ver_FileCheck: flash_read 0x%x fail!\r\n", ulAddr);
			return VER_EXE_FAIL;/*执行失败*/
		}	
		ulCrc = crc32_s(ulCrc, g_stVerUpgradeFormat.ucaData, ulLen);		
	}

	if(ulCrc != pst->ulBinFileCrc32)
	{
		rtt_printf(RTT_UPGRADE, RTT_TEXT_RED"ver_FileCheck: 0x%x  Crc  0x%x != 0x%x\r\n", addr, ulCrc, pst->ulBinFileCrc32);
		return VER_EXE_FAIL;/*执行失败*/
	}		
	
    return VER_EXE_SUCCESS;
}
/*****************************************************************************
* @Name	  :  ver_McuTypeCheck
* @Brief  :  MCU类型检查
* @Param  :  pst: 文件信息
* @Retval :  执行结果
* @Author :
* @Data   :  2024-02-26
* @Note   :  
*****************************************************************************/
static uint8_t ver_McuTypeCheck(stVerFileInfo *pst)
{
#if LOCAL_MODULE_TYPE == _MU
	if(pst->ucDeviceType==VER_DEVICE_MU)/*主机*/
	{
		if((pst->ucModuleType==VER_MODULE_MCU_BOOT) || (pst->ucModuleType==VER_MODULE_MCU_APP))
		{
			if(strncmp((char *)(pst->ucaMcuType), DEFAULT_MCU_NAME, strlen(DEFAULT_MCU_NAME)) != 0)
			{
				rtt_printf(RTT_UPGRADE, RTT_TEXT_RED"ver_McuTypeCheck: %s != %s\r\n", pst->ucaMcuType, DEFAULT_MCU_NAME);
				return VER_EXE_FAIL;/*执行失败*/
			}			
		}
	}
#else
	if(pst->ucDeviceType==VER_DEVICE_RU)/*从机*/
	{
		if((pst->ucModuleType==VER_MODULE_MCU_BOOT) || (pst->ucModuleType==VER_MODULE_MCU_APP))
		{
			if(strncmp((char *)(pst->ucaMcuType), DEFAULT_MCU_NAME, strlen(DEFAULT_MCU_NAME)) != 0)
			{
				rtt_printf(RTT_UPGRADE, RTT_TEXT_RED"ver_McuTypeCheck: %s != %s\r\n", pst->ucaMcuType, DEFAULT_MCU_NAME);
				return VER_EXE_FAIL;/*执行失败*/
			}			
		}
	}	
#endif		
	
    return VER_EXE_SUCCESS;
}

/*****************************************************************************
* @Name	  :  ver_FileWorkAddr
* @Brief  :  返回有效（已擦除）的固件区地址
* @Param  :  pst: 文件信息
* @Retval :  固件区地址
* @Author :
* @Data   :  2024-02-26
* @Note   :  
*****************************************************************************/
static uint32_t ver_FileWorkAddr(stVerFileInfo *pst)
{
	uint32_t ulIndex;
	uint32_t ulLen = pst->ulBinFileLen + VER_FILE_HEAD_LEN;
	uint32_t ulNum;
	uint32_t ulSector;
	
	for(ulIndex=0; ulIndex<g_ulVerTableLen; ulIndex++)
	{
		if(g_staVerTable[ulIndex].ucVersionType == VER_TYPE_WORK)
		{
			if((g_staVerTable[ulIndex].ucDeviceType == VER_DEVICE_MU) || (g_staVerTable[ulIndex].ucDeviceType == VER_DEVICE_RU))
			{
				if((g_staVerTable[ulIndex].ucDeviceType == pst->ucDeviceType) && (g_staVerTable[ulIndex].ucModuleType == pst->ucModuleType))
				{
					break;
				}
			}
			else
			{
				if(g_staVerTable[ulIndex].ucModuleType == pst->ucModuleType)
				{
					break;
				}
			}
		}			
	}
	
	if(ulIndex<g_ulVerTableLen)
	{
		if(ulLen <= ((g_staVerTable[ulIndex].ulFlashESector-g_staVerTable[ulIndex].ulFlashSSector+1) * spi_flash.sector_size))
		{
			if((ulLen % spi_flash.sector_size) == 0)
				ulNum = ulLen / spi_flash.sector_size;
			else
				ulNum = ulLen / spi_flash.sector_size + 1;
			
			ulSector = g_staVerTable[ulIndex].ulFlashSSector;
			while(ulNum>0)
			{
				if(STAT_OK != spi_flash.flash_erase_sector(ulSector))
				{
					rtt_printf(RTT_UPGRADE, RTT_TEXT_RED"ver_FileWorkAddr: flash_erase_sector 0x%x fail!\r\n", ulSector);
					return VER_ERROR_ADDR;
				}			
				ulNum--;
				ulSector++;
			}
			
			return g_staVerTable[ulIndex].ulFlashSSector * spi_flash.sector_size;
		}		
	}
	
	rtt_printf(RTT_UPGRADE, RTT_TEXT_RED"ver_FileWorkAddr: ulIndex %d  ulLen %d\r\n", ulIndex, ulLen);
    return VER_ERROR_ADDR;
}
/*****************************************************************************
* @Name	  :  ver_FileMove
* @Brief  :  根据文件类型进行文件搬移
* @Param  :  addr: 文件存放地址
**           pst: 文件信息
* @Retval :  执行结果
* @Author :
* @Data   :  2024-02-26
* @Note   :  
*****************************************************************************/
static uint8_t ver_FileMove(uint32_t addr, stVerFileInfo *pst)
{
	uint32_t ulAddr, ulSrcAddr, ulDstAddr;
	uint32_t ulLen;
	stVerFileInfo fileInfo;
	
	ulAddr = ver_FileWorkAddr(pst);
	if(ulAddr == VER_ERROR_ADDR)/*无效地址*/
	{
		return VER_EXE_FAIL;
	}
	
	ulSrcAddr = addr;
	ulDstAddr = ulAddr;
	ulLen = pst->ulBinFileLen + VER_FILE_HEAD_LEN;
	while(ulLen > VER_UPGRADE_DATA_BUF_SIZE)
	{
		if(STAT_OK != spi_flash.flash_read(g_stVerUpgradeFormat.ucaData, ulSrcAddr, VER_UPGRADE_DATA_BUF_SIZE))	/*读FLASH*/
		{	
			rtt_printf(RTT_UPGRADE, RTT_TEXT_RED"ver_FileMove: flash_read 0x%x fail!\r\n", ulSrcAddr);
			return VER_EXE_FAIL;/*执行失败*/
		}

		if(STAT_OK != spi_flash.flash_write(g_stVerUpgradeFormat.ucaData, ulDstAddr, VER_UPGRADE_DATA_BUF_SIZE))/*写FLASH*/
		{
			rtt_printf(RTT_UPGRADE, RTT_TEXT_RED"ver_FileMove: flash_write 0x%x fail!\r\n", ulDstAddr);
			return VER_EXE_FAIL;/*执行失败*/
		}

		ulSrcAddr += VER_UPGRADE_DATA_BUF_SIZE;
		ulDstAddr += VER_UPGRADE_DATA_BUF_SIZE;
		ulLen -= VER_UPGRADE_DATA_BUF_SIZE;
	}	
	
	if(ulLen > 0)
	{
		if(STAT_OK != spi_flash.flash_read(g_stVerUpgradeFormat.ucaData, ulSrcAddr, ulLen))	/*读FLASH*/
		{	
			rtt_printf(RTT_UPGRADE, RTT_TEXT_RED"ver_FileMove: flash_read 0x%x fail!\r\n", ulSrcAddr);
			return VER_EXE_FAIL;/*执行失败*/
		}	
		
		if(STAT_OK != spi_flash.flash_write(g_stVerUpgradeFormat.ucaData, ulDstAddr, ulLen))/*写FLASH*/
		{
			rtt_printf(RTT_UPGRADE, RTT_TEXT_RED"ver_FileMove: flash_write 0x%x fail!\r\n", ulDstAddr);
			return VER_EXE_FAIL;/*执行失败*/
		}	
	}		
	
	/*搬移后校验*/
	if(VER_EXE_SUCCESS != ver_FileInfo(ulAddr, &fileInfo))
	{
		return VER_EXE_FAIL;/*执行失败*/
	}	
	
	if(VER_EXE_SUCCESS != ver_FileCheck(ulAddr, &fileInfo))
	{
		return VER_EXE_FAIL;/*执行失败*/
	}	
	
    return VER_EXE_SUCCESS;
}

/*****************************************************************************
* @Name	  :  ver_UpdateFunc_Reset
* @Brief  :  版本更新方式：重启系统
* @Param  :  verIndex: 表中的index
* @Retval :  执行结果
* @Author :
* @Data   :  2024-02-26
* @Note   :  
*****************************************************************************/
static uint8_t ver_UpdateFunc_Reset(uint32_t verIndex)
{
	reboot_set_egp_bit(RESET_SYSTEM);	
	return VER_EXE_SUCCESS;
}
/*****************************************************************************
* @Name	  :  ver_BootMove
* @Brief  :  boot文件搬移到芯片flash 0 1 2扇区
* @Param  :  addr: 源文件地址
**           pst: 文件信息
* @Retval :  执行结果
* @Author :
* @Data   :  2024-02-26
* @Note   :  
*****************************************************************************/
static uint8_t ver_BootMove(uint32_t addr, stVerFileInfo *pst)
{
	uint32_t ulSrcAddr, ulDstAddr;
	uint32_t ulLen;	
	
	uint32_t ulData1, ulData2;
	uint32_t ulCrc = 0;	
	
	ulSrcAddr = addr;
	ulDstAddr = ADDR_FLASH_SECTOR_0;
	ulLen = pst->ulBinFileLen;	
	if(ulLen>=(ADDR_FLASH_SECTOR_3-ADDR_FLASH_SECTOR_0))/*boot分配芯片flash的0 1 2扇区*/	
	{
		rtt_printf(RTT_UPGRADE, RTT_TEXT_RED"ver_BootMove: ulLen %d\r\n", ulLen);
		return VER_EXE_FAIL;/*执行失败*/		
	}
	
	if((STAT_OK!=hi_flash_sector_erase(FLASH_SECTOR_NUM_0)) 
		|| (STAT_OK!=hi_flash_sector_erase(FLASH_SECTOR_NUM_1)) 
		|| (STAT_OK!=hi_flash_sector_erase(FLASH_SECTOR_NUM_2)))
	{
		rtt_printf(RTT_UPGRADE, RTT_TEXT_RED"ver_BootMove: hi_flash_sector_erase fail!\r\n");
		return VER_EXE_FAIL;/*执行失败*/		
	}
	
	while(ulLen > VER_UPGRADE_DATA_BUF_SIZE)
	{
		if(STAT_OK != spi_flash.flash_read(g_stVerUpgradeFormat.ucaData, ulSrcAddr, VER_UPGRADE_DATA_BUF_SIZE))	/*读FLASH*/
		{	
			rtt_printf(RTT_UPGRADE, RTT_TEXT_RED"ver_BootMove: flash_read 0x%x fail!\r\n", ulSrcAddr);
			return VER_EXE_FAIL;/*执行失败*/
		}

		if(STAT_OK != hi_flash_write(ulDstAddr, g_stVerUpgradeFormat.ucaData, VER_UPGRADE_DATA_BUF_SIZE))/*写芯片FLASH*/
		{
			rtt_printf(RTT_UPGRADE, RTT_TEXT_RED"ver_BootMove: hi_flash_write 0x%x fail!\r\n", ulDstAddr);
			return VER_EXE_FAIL;/*执行失败*/
		}

		ulSrcAddr += VER_UPGRADE_DATA_BUF_SIZE;
		ulDstAddr += VER_UPGRADE_DATA_BUF_SIZE;
		ulLen -= VER_UPGRADE_DATA_BUF_SIZE;
	}	
	
	if(ulLen > 0)
	{
		if(STAT_OK != spi_flash.flash_read(g_stVerUpgradeFormat.ucaData, ulSrcAddr, ulLen))	/*读FLASH*/
		{	
			rtt_printf(RTT_UPGRADE, RTT_TEXT_RED"ver_BootMove: flash_read 0x%x fail!\r\n", ulSrcAddr);
			return VER_EXE_FAIL;/*执行失败*/
		}	
		
		if(STAT_OK != hi_flash_write(ulDstAddr, g_stVerUpgradeFormat.ucaData, ulLen))/*写芯片FLASH*/
		{
			rtt_printf(RTT_UPGRADE, RTT_TEXT_RED"ver_BootMove: hi_flash_write 0x%x fail!\r\n", ulDstAddr);
			return VER_EXE_FAIL;/*执行失败*/
		}	
	}		
	
	/*校验*/
	ulDstAddr = ADDR_FLASH_SECTOR_0;

	/*有效性校验  取原始BIN文件位于0x1000~0x1003字节的32位数据 Data1和位于0x1FFC~0x1FFF字节的32位数据 Data2进行异或*/
	if(STAT_OK != hi_flash_read(ulDstAddr+0x1000, g_stVerUpgradeFormat.ucaData, 4))	/*读芯片FLASH*/
	{
		rtt_printf(RTT_UPGRADE, RTT_TEXT_RED"ver_BootMove: hi_flash_read 0x%x fail!\r\n", ulDstAddr+0x1000);
		return VER_EXE_FAIL;/*执行失败*/
	}		
	ulData1 = (g_stVerUpgradeFormat.ucaData[0x0]<<24) | (g_stVerUpgradeFormat.ucaData[0x1]<<16) | (g_stVerUpgradeFormat.ucaData[0x2]<<8) | g_stVerUpgradeFormat.ucaData[0x3];

	if(STAT_OK != hi_flash_read(ulDstAddr+0x1FFC, g_stVerUpgradeFormat.ucaData, 4))	/*读芯片FLASH*/
	{
		rtt_printf(RTT_UPGRADE, RTT_TEXT_RED"ver_BootMove: hi_flash_read 0x%x fail!\r\n", ulDstAddr+0x1FFC);
		return VER_EXE_FAIL;/*执行失败*/
	}	
	ulData2 = (g_stVerUpgradeFormat.ucaData[0x0]<<24) | (g_stVerUpgradeFormat.ucaData[0x1]<<16) | (g_stVerUpgradeFormat.ucaData[0x2]<<8) | g_stVerUpgradeFormat.ucaData[0x3];

	if((ulData1^ulData2) != pst->ulBinFileValid)
	{
		rtt_printf(RTT_UPGRADE, RTT_TEXT_RED"ver_BootMove: BinFileValid 0x%x ! 0x%x\r\n", (ulData1^ulData2), pst->ulBinFileValid);
		return VER_EXE_FAIL;/*执行失败*/
	}			
	
	/*正确性校验  CRC32*/
	ulLen = pst->ulBinFileLen;
	while(ulLen > VER_UPGRADE_DATA_BUF_SIZE)
	{
		if(STAT_OK != hi_flash_read(ulDstAddr, g_stVerUpgradeFormat.ucaData, VER_UPGRADE_DATA_BUF_SIZE))	/*读芯片FLASH*/
		{	
			rtt_printf(RTT_UPGRADE, RTT_TEXT_RED"ver_BootMove: hi_flash_read 0x%x fail!\r\n", ulDstAddr);
			return VER_EXE_FAIL;/*执行失败*/
		}	
		ulCrc = crc32_s(ulCrc, g_stVerUpgradeFormat.ucaData, VER_UPGRADE_DATA_BUF_SIZE);
		ulDstAddr += VER_UPGRADE_DATA_BUF_SIZE;
		ulLen -= VER_UPGRADE_DATA_BUF_SIZE;
	}
	
	if(ulLen > 0)
	{
		if(STAT_OK != hi_flash_read(ulDstAddr, g_stVerUpgradeFormat.ucaData, ulLen))	/*读芯片FLASH*/
		{	
			rtt_printf(RTT_UPGRADE, RTT_TEXT_RED"ver_BootMove: hi_flash_read 0x%x fail!\r\n", ulDstAddr);
			return VER_EXE_FAIL;/*执行失败*/
		}	
		ulCrc = crc32_s(ulCrc, g_stVerUpgradeFormat.ucaData, ulLen);		
	}

	if(ulCrc != pst->ulBinFileCrc32)
	{
		rtt_printf(RTT_UPGRADE, RTT_TEXT_RED"ver_BootMove: Crc 0x%x ! 0x%x\r\n", ulCrc, pst->ulBinFileCrc32);
		return VER_EXE_FAIL;/*执行失败*/
	}		
	
    return VER_EXE_SUCCESS;		
}

/*****************************************************************************
* @Name	  :  ver_UpdateFunc_McuBoot
* @Brief  :  版本更新方式：拷贝到芯片内部flash，重启系统
* @Param  :  verIndex: 表中的index
* @Retval :  执行结果
* @Author :
* @Data   :  2024-02-26
* @Note   :  
*****************************************************************************/
static uint8_t ver_UpdateFunc_McuBoot(uint32_t verIndex)
{
	stVerFileInfo fileInfo;
	
	if(verIndex>=g_ulVerTableLen)
	{
		return VER_EXE_FAIL;/*执行失败*/
	}
	
	if(VER_EXE_SUCCESS != ver_FileInfo(g_staVerTable[verIndex].ulFlashSSector*spi_flash.sector_size, &fileInfo))
	{
		return VER_EXE_FAIL;/*执行失败*/
	}
	
	if(VER_EXE_SUCCESS != ver_BootMove(g_staVerTable[verIndex].ulFlashSSector*spi_flash.sector_size + VER_FILE_HEAD_LEN, &fileInfo))
	{
		if(VER_EXE_SUCCESS != ver_BootMove(g_staVerTable[verIndex].ulFlashSSector*spi_flash.sector_size + VER_FILE_HEAD_LEN, &fileInfo))
		{
			if(VER_EXE_SUCCESS != ver_BootMove(g_staVerTable[verIndex].ulFlashSSector*spi_flash.sector_size + VER_FILE_HEAD_LEN, &fileInfo))
			{
				return VER_EXE_FAIL;/*执行失败*/
			}		
		}		
	}
	
	reboot_set_egp_bit(RESET_SYSTEM);
	
	return VER_EXE_SUCCESS;
}

/*****************************************************************************
* @Name	  :  ver_BootInfoRead
* @Brief  :  读boot info信息
* @Param  :  pt_info: boot info信息
* @Retval :  执行结果
* @Author :
* @Data   :  2024-02-26
* @Note   :  
*****************************************************************************/
static uint8_t ver_BootInfoRead( bootloader_info_t *pt_info )
{
    uint32_t crc_cal = 0;	
	
	if(pt_info == NULL)
		return VER_EXE_FAIL;/*执行失败*/
	
	/*读FLASH*/
	if(STAT_OK != spi_flash.flash_read((uint8_t *)pt_info, VER_FLASH_UPDATE_INFO_SECTOR*spi_flash.sector_size, sizeof(bootloader_info_t)))
	{
		rtt_printf(RTT_UPGRADE, RTT_TEXT_RED"ver_BootInfoRead: flash_read 0x%x fail!\r\n", VER_FLASH_UPDATE_INFO_SECTOR*spi_flash.sector_size);
		return VER_EXE_FAIL;/*执行失败*/
	}	
	
    if( BOOTLOADER_START_BYTE != pt_info->start )
    {
		rtt_printf(RTT_UPGRADE, RTT_TEXT_RED"ver_BootInfoRead: t_info.start 0x%x\r\n", pt_info->start);
        return VER_EXE_FAIL;
    }
	
    crc_cal = crc32( (uint8_t *)pt_info, sizeof(bootloader_info_t)-4 ); /*计算校验*/
    if( pt_info->crc32 != crc_cal )    /*校验*/
    {
		rtt_printf(RTT_UPGRADE, RTT_TEXT_RED"ver_BootInfoRead: Crc 0x%x != 0x%x\r\n", pt_info->crc32, crc_cal);
        return VER_EXE_FAIL;
    }	
	
	return VER_EXE_SUCCESS;
}
	
/*****************************************************************************
* @Name	  :  ver_BootInfoWrie
* @Brief  :  保存boot info信息
* @Param  :  pt_info: boot info信息
* @Retval :  执行结果
* @Author :
* @Data   :  2024-02-26
* @Note   :  
*****************************************************************************/
static uint8_t ver_BootInfoWrie( bootloader_info_t *pt_info )
{
	pt_info->start = BOOTLOADER_START_BYTE;
	pt_info->crc32 = crc32( (uint8_t *)pt_info, sizeof(bootloader_info_t) - 4 ); /*CRC校验*/
	
	if(STAT_OK != spi_flash.flash_erase_sector(VER_FLASH_UPDATE_INFO_SECTOR))
	{
		rtt_printf(RTT_UPGRADE, RTT_TEXT_RED"ver_BootInfoWrie: flash_erase_sector %d fail!\r\n", VER_FLASH_UPDATE_INFO_SECTOR);
		return VER_EXE_FAIL;/*执行失败*/
	}
	
	/*写FLASH*/
	if(STAT_OK != spi_flash.flash_write((uint8_t *)pt_info, VER_FLASH_UPDATE_INFO_SECTOR*spi_flash.sector_size, sizeof(bootloader_info_t)))
	{
		rtt_printf(RTT_UPGRADE, RTT_TEXT_RED"ver_BootInfoWrie: flash_write 0x%x fail!\r\n", VER_FLASH_UPDATE_INFO_SECTOR*spi_flash.sector_size);
		return VER_EXE_FAIL;/*执行失败*/
	}		
	
	return VER_EXE_SUCCESS;	
}


/*****************************************************************************
* @Name	  :  ver_BootInfoHandle
* @Brief  :  处理bootinfo，APP启动后调用
* @Param  :  无
* @Retval :  执行结果
* @Author :
* @Data   :  2024-02-26
* @Note   :  
*****************************************************************************/
static uint8_t ver_BootInfoHandle( void )
{
	bootloader_info_t t_info;
	uint8_t ucRet;
	
	/*读boot info*/
	ucRet = ver_BootInfoRead(&t_info);
	
#if (LOCAL_MODULE_TYPE == _MU)
	/*设置5G MODEM升级*/
	if(VER_EXE_SUCCESS != ucRet)
		Upgrade_set_5g_modem(0, 0);
	else
		Upgrade_set_5g_modem(t_info.reserve[0], 1);
#endif		
	
	if(VER_EXE_SUCCESS != ucRet)
	{
		return VER_EXE_FAIL;/*执行失败*/
	}
	
	if((t_info.upg_flag!=0) || (t_info.complete_flg!=0))
	{
		rtt_printf(RTT_UPGRADE, RTT_TEXT_YELLOW"ver_BootInfoHandle: upg_flag %d  complete_flg %d\r\n", t_info.upg_flag, t_info.complete_flg);
		t_info.upg_flag = UPG_FLAG_JUMP_TO_APP;
		t_info.complete_flg = 0;
		/*写boot info*/
		if(VER_EXE_SUCCESS != ver_BootInfoWrie(&t_info))
		{
			return VER_EXE_FAIL;/*执行失败*/
		}	
	}
	
	return VER_EXE_SUCCESS;
}

/*****************************************************************************
* @Name	  :  ver_SetAppUpgrade
* @Brief  :  设置APP升级标志
* @Param  :  无
* @Retval :  执行结果
* @Author :
* @Data   :  2024-02-26
* @Note   :  
*****************************************************************************/
static uint8_t ver_SetAppUpgrade( void )
{
	bootloader_info_t t_info;
	
	/*读boot info*/
	if(VER_EXE_SUCCESS != ver_BootInfoRead(&t_info))
	{
		memset(&t_info, 0, sizeof(bootloader_info_t));
	}	
	
	t_info.upg_flag = UPG_FLAG_FROM_UPGRADE_TO_APP;
	/*写boot info*/
	if(VER_EXE_SUCCESS != ver_BootInfoWrie(&t_info))
	{
		return VER_EXE_FAIL;/*执行失败*/
	}	

	reboot_set_egp_bit(RESET_SYSTEM);
	
	return VER_EXE_SUCCESS;
}

/*****************************************************************************
* @Name	  :  ver_Set5gModemUpgrade
* @Brief  :  设置5G MODEM升级标志
* @Param  :  无
* @Retval :  执行结果
* @Author :
* @Data   :  2024-02-26
* @Note   :  
*****************************************************************************/
uint8_t ver_Set5gModemUpgrade(uint8_t isUpgrade)
{
	bootloader_info_t t_info;

	/*读boot info*/
	if(VER_EXE_SUCCESS != ver_BootInfoRead(&t_info))
	{
		memset(&t_info, 0, sizeof(bootloader_info_t));
	}	
	
	t_info.reserve[0] = isUpgrade;
	/*写boot info*/
	if(VER_EXE_SUCCESS != ver_BootInfoWrie(&t_info))
	{
		return VER_EXE_FAIL;/*执行失败*/
	}	

#if (LOCAL_MODULE_TYPE == _MU)
	/*设置5G MODEM升级*/	
	Upgrade_set_5g_modem(t_info.reserve[0], 0);
#endif
	
	return VER_EXE_SUCCESS;	
}
/*****************************************************************************
* @Name	  :  ver_UpdateFunc_McuApp
* @Brief  :  版本更新方式：设置升级标记，重启系统
* @Param  :  verIndex: 表中的index
* @Retval :  执行结果
* @Author :
* @Data   :  2024-02-26
* @Note   :  
*****************************************************************************/
static uint8_t ver_UpdateFunc_McuApp(uint32_t verIndex)
{
	return ver_SetAppUpgrade();
}

#if (LOCAL_MODULE_TYPE == _MU)
/*****************************************************************************
* @Name	  :  ver_UpdateFunc_SoftVersion
* @Brief  :  版本更新方式：更新软件版本
* @Param  :  verIndex: 表中的index
* @Retval :  执行结果
* @Author :
* @Data   :  2024-02-26
* @Note   :  
*****************************************************************************/
static uint8_t ver_UpdateFunc_SoftVersion(uint32_t verIndex)
{
	stVerFileInfo fileInfo;
	
	if(verIndex<g_ulVerTableLen)
	{
		if(VER_EXE_SUCCESS == ver_FileInfo(g_staVerTable[verIndex].ulFlashSSector*spi_flash.sector_size, &fileInfo))
		{
			g_staVerTable[verIndex].ulSoftVersion = fileInfo.ulSoftVersion;
			g_staVerTable[verIndex].ulBinFileLen  = fileInfo.ulBinFileLen;
			Upgrade_RuStart();
			return VER_EXE_SUCCESS;
		}
	}	
	return VER_EXE_FAIL;
}
/*****************************************************************************
* @Name	  :  ver_UpdateFunc_5GModem
* @Brief  :  版本更新方式：开启5G Modem升级
* @Param  :  verIndex: 表中的index
* @Retval :  执行结果
* @Author :
* @Data   :  2024-02-26
* @Note   :  
*****************************************************************************/
static uint8_t ver_UpdateFunc_5GModem(uint32_t verIndex)
{
	stVerFileInfo fileInfo;
	
	if(verIndex<g_ulVerTableLen)
	{
		if(VER_EXE_SUCCESS == ver_FileInfo(g_staVerTable[verIndex].ulFlashSSector*spi_flash.sector_size, &fileInfo))
		{
			g_staVerTable[verIndex].ulSoftVersion = fileInfo.ulSoftVersion;
			g_staVerTable[verIndex].ulBinFileLen  = fileInfo.ulBinFileLen;
			return ver_Set5gModemUpgrade(1);
		}
	}	
	return VER_EXE_FAIL;
}
#endif
/*****************************************************************************
* @Name	  :  ver_FileTransInit
* @Brief  :  固件升级初始化，擦除升级文件数据记录区
* @Param  :  无
* @Retval :  执行结果
* @Author :
* @Data   :  2024-02-26
* @Note   :
*****************************************************************************/
uint8_t ver_FileTransInit(void)
{
	g_stVerUpgradeFormat.ulWriteOffset = 0;
	g_stVerUpgradeFormat.ulDatalen = 0;
	
    return EN_VER_UPGRADE_SUCCESS;	
}

/*****************************************************************************
* @Name	  :  ver_FileDataSave
* @Brief  :  保存升级文件数据到flash文件接收区
* @Param  :  data: 指向升级文件数据
**			 datalen: 升级文件数据长度
* @Retval :  执行结果
* @Author :
* @Data   :  2024-02-26
* @Note   :
*****************************************************************************/
uint8_t ver_FileDataSave(uint8_t *data, uint16_t datalen)
{
    uint32_t ulFlasAddr    = 0 ;
    uint32_t ulLen     = 0 ;
	uint16_t usSector;

	/*参数检查*/
    if((NULL == data) || (datalen > VER_UPGRADE_DATA_BUF_SIZE))
    {
        return EN_VER_UPGRADE_UNDONE;
    }

	/*数据先写入缓冲区，缓冲区满再写入FLASH*/
    if((g_stVerUpgradeFormat.ulDatalen + datalen) >= VER_UPGRADE_DATA_BUF_SIZE)
    {
        ulLen = VER_UPGRADE_DATA_BUF_SIZE - g_stVerUpgradeFormat.ulDatalen;
        memcpy(g_stVerUpgradeFormat.ucaData+g_stVerUpgradeFormat.ulDatalen, data, ulLen);

        ulFlasAddr = VER_FLASH_FILE_RECV_S_SECTOR * spi_flash.sector_size + g_stVerUpgradeFormat.ulWriteOffset;
        if(g_stVerUpgradeFormat.ulWriteOffset >= ((VER_FLASH_FILE_RECV_E_SECTOR-VER_FLASH_FILE_RECV_S_SECTOR+1) * spi_flash.sector_size))
        {
			rtt_printf(RTT_UPGRADE, RTT_TEXT_RED"ver_FileDataSave: ulWriteOffset=0x%x!\r\n", g_stVerUpgradeFormat.ulWriteOffset);
            return EN_VER_UPGRADE_FILE_TOO_LAGER ;/*固件过大*/
        }
		
		/*先擦FLASH*/
		if((ulFlasAddr%spi_flash.sector_size) == 0)
		{
			usSector = spi_flash.flash_addr_to_sector(ulFlasAddr);
			/*rtt_printf(RTT_UPGRADE, RTT_TEXT_RED"ver_FileDataSave: ulFlasAddr=0x%x  usSector=%d\r\n", ulFlasAddr, usSector);*/
			if(STAT_OK != spi_flash.flash_erase_sector(usSector))
			{
				rtt_printf(RTT_UPGRADE, RTT_TEXT_RED"ver_FileDataSave: flash_erase_sector %d fail!\r\n", usSector);
				return EN_VER_UPGRADE_WE_FLASH_FAIL;/*写擦flash失败*/
			}
		}
		
		/*rtt_printf(RTT_UPGRADE, RTT_TEXT_YELLOW"ver_FileDataSave: ulFlasAddr=0x%x write %x%x\r\n", ulFlasAddr, g_stVerUpgradeFormat.ucaData[0], g_stVerUpgradeFormat.ucaData[1]);*/
		
		/*写FLASH*/
		if(STAT_OK != spi_flash.flash_write(g_stVerUpgradeFormat.ucaData, ulFlasAddr, VER_UPGRADE_DATA_BUF_SIZE))
		{
			rtt_printf(RTT_UPGRADE, RTT_TEXT_RED"ver_FileDataSave: flash_write 0x%x fail!\r\n", ulFlasAddr);
			return EN_VER_UPGRADE_WE_FLASH_FAIL;/*写擦flash失败*/
		}

		/*更新控制变量*/
        g_stVerUpgradeFormat.ulWriteOffset += VER_UPGRADE_DATA_BUF_SIZE;
        memcpy(g_stVerUpgradeFormat.ucaData, data + ulLen, datalen - ulLen);
        g_stVerUpgradeFormat.ulDatalen = datalen - ulLen;
    }
    else
    {
        memcpy(g_stVerUpgradeFormat.ucaData+g_stVerUpgradeFormat.ulDatalen, data, datalen);
        g_stVerUpgradeFormat.ulDatalen += datalen;
    }

    return EN_VER_UPGRADE_SUCCESS;
}

/*****************************************************************************
* @Name	  :  ver_FileTransEnd
* @Brief  :  保存升级文件最后的数据到flash中，进行校验和数据搬移
* @Param  :  无
* @Retval :  执行结果
* @Author :
* @Data   :  2024-02-26
* @Note   :
*****************************************************************************/
uint8_t ver_FileTransEnd(void)
{
    uint32_t ulFlasAddr    = 0 ;
	uint16_t usSector;
	stVerFileInfo fileInfo;
	
    if(g_stVerUpgradeFormat.ulDatalen)
    {
        ulFlasAddr = VER_FLASH_FILE_RECV_S_SECTOR * spi_flash.sector_size + g_stVerUpgradeFormat.ulWriteOffset;
        if(g_stVerUpgradeFormat.ulWriteOffset >= ((VER_FLASH_FILE_RECV_E_SECTOR-VER_FLASH_FILE_RECV_S_SECTOR+1) * spi_flash.sector_size))
        {
			rtt_printf(RTT_UPGRADE, RTT_TEXT_RED"ver_FileDataSave: ulWriteOffset=0x%x!\r\n", g_stVerUpgradeFormat.ulWriteOffset);
            return EN_VER_UPGRADE_FILE_TOO_LAGER ;/*固件过大*/
        }
		
		/*先擦FLASH*/
		if((ulFlasAddr%spi_flash.sector_size) == 0)
		{
			usSector = spi_flash.flash_addr_to_sector(ulFlasAddr);
			if(STAT_OK != spi_flash.flash_erase_sector(usSector))
			{
				rtt_printf(RTT_UPGRADE, RTT_TEXT_RED"ver_FileDataSave: flash_erase_sector %d fail!\r\n", usSector);
				return EN_VER_UPGRADE_WE_FLASH_FAIL;/*写擦flash失败*/
			}
		}
		
		/*写FLASH*/
		if(STAT_OK != spi_flash.flash_write(g_stVerUpgradeFormat.ucaData, ulFlasAddr, g_stVerUpgradeFormat.ulDatalen))
		{
			rtt_printf(RTT_UPGRADE, RTT_TEXT_RED"ver_FileDataSave: flash_write 0x%x fail!\r\n", ulFlasAddr);
			return EN_VER_UPGRADE_WE_FLASH_FAIL;/*写擦flash失败*/
		}
    }
	
	g_stVerUpgradeFormat.ulDatalen = 0;
		
	/*获取文件信息*/
	ulFlasAddr = VER_FLASH_FILE_RECV_S_SECTOR * spi_flash.sector_size;
	if(VER_EXE_SUCCESS != ver_FileInfo(ulFlasAddr, &fileInfo))
	{
		return EN_VER_UPGRADE_FILE_INFO_ERROR;/*固件信息错误*/
	}	
	
	/*文件校验*/
	if(VER_EXE_SUCCESS != ver_FileCheck(ulFlasAddr, &fileInfo))
	{
		return EN_VER_UPGRADE_FILE_CRC_FAIL;/*校验失败*/
	}	

	/*特殊校验：MCU类型校验*/
	if(VER_EXE_SUCCESS != ver_McuTypeCheck(&fileInfo))
	{
		return EN_VER_UPGRADE_MCU_TYPE_ERROR;/*MCU类型错误*/
	}	
	
	/*搬移到固件区*/
	//if(VER_EXE_SUCCESS != ver_FileMove(ulFlasAddr, &fileInfo))
	//{
	//	if(VER_EXE_SUCCESS != ver_FileMove(ulFlasAddr, &fileInfo))
	//	{
	//		if(VER_EXE_SUCCESS != ver_FileMove(ulFlasAddr, &fileInfo))
	//		{
	//			return EN_VER_UPGRADE_FILE_MOVE_FAIL;/*固件搬移失败*/
	//		}
	//	}
	//}	
	
    return EN_VER_UPGRADE_SUCCESS;
}

/*****************************************************************************
* @Name	  :  ver_FileLoad
* @Brief  :  版本加载
* @Param  :  无
* @Retval :  执行结果
* @Author :
* @Data   :  2024-02-26
* @Note   :  
*****************************************************************************/
uint8_t ver_FileLoad(void)
{
	g_ucVerFileLoadFlag = 1;
	return EN_VER_UPGRADE_SUCCESS;
}
static void ver_FileLoad_Func(void)
{
	stVerFileInfo fileInfo;
	uint32_t ulFlasAddr    = 0 ;
	uint32_t ulIndex;
	
	if(g_ucVerFileLoadFlag != 1)
		return;
	
	rtt_printf(RTT_UPGRADE, RTT_TEXT_YELLOW"ver_FileLoad_Func: start\r\n");
	
	g_ucVerFileLoadFlag = 0;
	
	/*获取文件信息*/
	ulFlasAddr = VER_FLASH_FILE_RECV_S_SECTOR * spi_flash.sector_size;
	if(VER_EXE_SUCCESS != ver_FileInfo(ulFlasAddr, &fileInfo))
	{
		return;
	}		
	
	/*搬移到固件区*/
	if(VER_EXE_SUCCESS != ver_FileMove(ulFlasAddr, &fileInfo))
	{
		if(VER_EXE_SUCCESS != ver_FileMove(ulFlasAddr, &fileInfo))
		{
			if(VER_EXE_SUCCESS != ver_FileMove(ulFlasAddr, &fileInfo))
			{
				return;/*固件搬移失败*/
			}
		}
	}	
	
	for(ulIndex=0; ulIndex<g_ulVerTableLen; ulIndex++)
	{
		if(g_staVerTable[ulIndex].ucVersionType == VER_TYPE_WORK)
		{
			if((g_staVerTable[ulIndex].ucDeviceType == VER_DEVICE_MU) || (g_staVerTable[ulIndex].ucDeviceType == VER_DEVICE_RU))
			{
				if((g_staVerTable[ulIndex].ucDeviceType == fileInfo.ucDeviceType) && (g_staVerTable[ulIndex].ucModuleType == fileInfo.ucModuleType))
				{
					break;
				}
			}
			else
			{
				if(g_staVerTable[ulIndex].ucModuleType == fileInfo.ucModuleType)
				{
					break;
				}
			}
		}
	}
	
	if(ulIndex<g_ulVerTableLen)
	{
		if(g_staVerTable[ulIndex].updateFunc != NULL)
			g_staVerTable[ulIndex].updateFunc(ulIndex);
	}
}

/*****************************************************************************
* @Name	  :  ver_UpgradeTimeValueClear
* @Brief  :  升级超时时间清零
* @Param  :  无
* @Retval :  执行结果
* @Author :
* @Data   :  2024-02-26
* @Note   :  
*****************************************************************************/
uint8_t ver_UpgradeTimeValueClear(void)
{
    g_usVerTimeOut = 0;
    return EN_VER_UPGRADE_SUCCESS;
}

/*****************************************************************************
* @Name	  :  ver_UpgradeTimeOutCtrl
* @Brief  :  升级超时定时器控制
* @Param  :  status 
* @Retval :  执行结果
* @Author :
* @Data   :  2024-02-26
* @Note   :  
*****************************************************************************/
uint8_t ver_UpgradeTimeOutCtrl(uint8_t status)
{
    uint8_t res = EN_VER_UPGRADE_SUCCESS;

    if(status == ENABLE)
    {
        xEventGroupSetBits(g_xVerEgp, BIT_TIME_VER_TIMEOUT_EN);/*超时使能*/
    }
    else
    {
        xEventGroupClearBits(g_xVerEgp, BIT_TIME_VER_TIMEOUT_EN);/*超时不使能*/
    }

    res = ver_UpgradeTimeValueClear();

    return res;
}

/*****************************************************************************
* @Name	  :  ver_CntInTask
* @Brief  :  超时检测
* @Param  :  无
* @Retval :  无
* @Author :
* @Data   :  2024-02-28
* @Note   :
*****************************************************************************/ 
extern uint8_t  g_ucVerUpgradeFlag;
extern uint32_t g_ulVerUpgradePackageSeq;
extern uint32_t g_ulVerUpgradePackageSeqBk;
static void ver_CntInTask(void)
{
    EventBits_t bitsUpgradeEventGroup = xEventGroupGetBitsFromISR(g_xVerEgp);

    if(bitsUpgradeEventGroup & BIT_TIME_VER_TIMEOUT_EN)
    {
        g_usVerTimeOut++;
        if(g_usVerTimeOut >= VER_PACKET_REV_TIMEOUT_CNT)
        {
            g_usVerTimeOut = 0;
			
            ver_UpgradeTimeOutCtrl(DISABLE);     /*关闭超时检测*/
            g_ucVerUpgradeFlag = 0;		
            g_ulVerUpgradePackageSeqBk = 0;
            g_ulVerUpgradePackageSeq = 0;				
            rtt_printf(RTT_UPGRADE, RTT_TEXT_RED"[ERROR]: update timeout , packet%d!\r\n", g_ulVerUpgradePackageSeq);		
        }
    }
}

/*****************************************************************************
* @Name	  :  Ver_Init
* @Brief  :  ver模块初始化
* @Param  :  无
* @Retval :  无
* @Author :
* @Data   :  2024-02-26
* @Note   :  
*****************************************************************************/
void Ver_Init(void)
{
	uint32_t ulIndex;
	stVerFileInfo fileInfo;
	
    g_xVerEgp = xEventGroupCreate();
    if(NULL == g_xVerEgp)
    {
		rtt_printf(RTT_UPGRADE, RTT_TEXT_RED"Create ver xegp Failed!\r\n"); 
    }
	
	for(ulIndex=0; ulIndex<g_ulVerTableLen; ulIndex++)
	{
		if(VER_EXE_SUCCESS == ver_FileInfo(g_staVerTable[ulIndex].ulFlashSSector*spi_flash.sector_size, &fileInfo))
		{
			g_staVerTable[ulIndex].ulSoftVersion = fileInfo.ulSoftVersion;
			g_staVerTable[ulIndex].ulBinFileLen  = fileInfo.ulBinFileLen;
		}
	}
	
	rtt_printf(RTT_UPGRADE, RTT_TEXT_YELLOW"Ver_Init: run\r\n");
	for(ulIndex=0; ulIndex<g_ulVerTableLen; ulIndex++)
	{
		rtt_printf(RTT_UPGRADE, RTT_TEXT_YELLOW"Ver_Init: type:%d %d %d version:%d len:%d\r\n", 
			g_staVerTable[ulIndex].ucVersionType, g_staVerTable[ulIndex].ucDeviceType, g_staVerTable[ulIndex].ucModuleType, g_staVerTable[ulIndex].ulSoftVersion, g_staVerTable[ulIndex].ulBinFileLen);
	}	
	
	rtt_printf(RTT_UPGRADE, RTT_TEXT_YELLOW"Ver_Init: %s\r\n", SW_VERSION);
	
	ver_BootInfoHandle();
}

/*****************************************************************************
* @Name	  :  Ver_Work
* @Brief  :  ver模块运行
* @Param  :  无
* @Retval :  无
* @Author :
* @Data   :  2024-02-26
* @Note   :  
*****************************************************************************/
void Ver_Work(void)
{
/*	
	static uint32_t run_cnt = 0;
	
	run_cnt++;
	if (run_cnt > 100)
	{
		run_cnt = 0;
		rtt_printf(RTT_UPGRADE, RTT_TEXT_YELLOW"Ver_Work: run\r\n");
	}
*/		
	ver_CntInTask();
	
	ver_FileLoad_Func();
}

/*****************************************************************************
* @Name	  :  Ver_GetBinFile
* @Brief  :  根据模块类型获取可用的原始文件的地址和长度
* @Param  :  ucType: 模块类型
                      VER_MODULE_FPGA       0x03 FPGA
                      VER_MODULE_8668_BIN1  0x06 8668 bin文件1(B3)
                      VER_MODULE_8668_BIN2  0x07 8668 bin文件2(B8)
                      VER_MODULE_8668_BIN3  0x08 8668 bin文件3(B39)
                      VER_MODULE_8668_BIN4  0x09 8668 bin文件4(B40)
                      VER_MODULE_8668_BIN5  0x0A 8668 bin文件5(N41)
**           pAddr: 文件地址
**           pLen: 文件长度
* @Retval :  执行结果
* @Author :
* @Data   :  2024-03-11
* @Note   :  
*****************************************************************************/
uint8_t Ver_GetBinFile(uint8_t ucType, uint32_t *pAddr, uint32_t *pLen)
{
	uint32_t ulIndex;
	
	if((pAddr==NULL) || (pLen==NULL))
		return VER_EXE_FAIL;
	
#if LOCAL_MODULE_TYPE == _MU
	for(ulIndex=0; ulIndex<g_ulVerTableLen; ulIndex++)
	{
		if(g_staVerTable[ulIndex].ucVersionType == VER_TYPE_WORK)/*先判断工作区版本*/
		{
			if((g_staVerTable[ulIndex].ucDeviceType == VER_DEVICE_MU) && (g_staVerTable[ulIndex].ucModuleType == ucType))
			{
				if((g_staVerTable[ulIndex].ulSoftVersion > 0) && (g_staVerTable[ulIndex].ulBinFileLen > 0))/*文件有效*/
				{
					*pAddr = g_staVerTable[ulIndex].ulFlashSSector*spi_flash.sector_size + VER_FILE_HEAD_LEN;
					*pLen  = g_staVerTable[ulIndex].ulBinFileLen;
					return VER_EXE_SUCCESS;
				}
			}
		}
	}
	
	rtt_printf(RTT_UPGRADE, RTT_TEXT_YELLOW"Ver_GetBinFile: module 0x%x not found work file\r\n", ucType);
	for(ulIndex=0; ulIndex<g_ulVerTableLen; ulIndex++)
	{
		if(g_staVerTable[ulIndex].ucVersionType == VER_TYPE_BAK)/*再判断备份区版本*/
		{
			if((g_staVerTable[ulIndex].ucDeviceType == VER_DEVICE_MU) && (g_staVerTable[ulIndex].ucModuleType == ucType))
			{
				if((g_staVerTable[ulIndex].ulSoftVersion > 0) && (g_staVerTable[ulIndex].ulBinFileLen > 0))/*文件有效*/
				{
					*pAddr = g_staVerTable[ulIndex].ulFlashSSector*spi_flash.sector_size + VER_FILE_HEAD_LEN;
					*pLen  = g_staVerTable[ulIndex].ulBinFileLen;
					return VER_EXE_SUCCESS;
				}
			}
		}
	}	
	
	rtt_printf(RTT_UPGRADE, RTT_TEXT_YELLOW"Ver_GetBinFile: module 0x%x not found bak file\r\n", ucType);
	return VER_EXE_FAIL;
#else
	for(ulIndex=0; ulIndex<g_ulVerTableLen; ulIndex++)
	{
		if(g_staVerTable[ulIndex].ucVersionType == VER_TYPE_WORK)/*先判断工作区版本*/
		{
			if((g_staVerTable[ulIndex].ucDeviceType == VER_DEVICE_RU) && (g_staVerTable[ulIndex].ucModuleType == ucType))
			{
				if((g_staVerTable[ulIndex].ulSoftVersion > 0) && (g_staVerTable[ulIndex].ulBinFileLen > 0))/*文件有效*/
				{
					*pAddr = g_staVerTable[ulIndex].ulFlashSSector*spi_flash.sector_size + VER_FILE_HEAD_LEN;
					*pLen  = g_staVerTable[ulIndex].ulBinFileLen;
					return VER_EXE_SUCCESS;
				}
			}
		}
	}
	
	rtt_printf(RTT_UPGRADE, RTT_TEXT_YELLOW"Ver_GetBinFile: module 0x%x not found work file\r\n", ucType);
	for(ulIndex=0; ulIndex<g_ulVerTableLen; ulIndex++)
	{
		if(g_staVerTable[ulIndex].ucVersionType == VER_TYPE_BAK)/*再判断备份区版本*/
		{
			if((g_staVerTable[ulIndex].ucDeviceType == VER_DEVICE_RU) && (g_staVerTable[ulIndex].ucModuleType == ucType))
			{
				if((g_staVerTable[ulIndex].ulSoftVersion > 0) && (g_staVerTable[ulIndex].ulBinFileLen > 0))/*文件有效*/
				{
					*pAddr = g_staVerTable[ulIndex].ulFlashSSector*spi_flash.sector_size + VER_FILE_HEAD_LEN;
					*pLen  = g_staVerTable[ulIndex].ulBinFileLen;
					return VER_EXE_SUCCESS;
				}
			}
		}
	}	
	
	rtt_printf(RTT_UPGRADE, RTT_TEXT_YELLOW"Ver_GetBinFile: module 0x%x not found bak file\r\n", ucType);
	return VER_EXE_FAIL;	
#endif			
}

/*****************************************************************************
* @Name	  :  Ver_GetRuFile
* @Brief  :  根据模块类型获取RU文件的地址 长度和版本号
* @Param  :  ucType: 模块类型
                      VER_MODULE_MCU_BOOT   0x00 MCU_BOOT
                      VER_MODULE_MCU_APP    0x01 MCU_APP
                      VER_MODULE_FPGA       0x03 FPGA
                      VER_MODULE_8668_BIN1  0x06 8668 bin文件1(B3)
                      VER_MODULE_8668_BIN2  0x07 8668 bin文件2(B8)
                      VER_MODULE_8668_BIN3  0x08 8668 bin文件3(B39)
                      VER_MODULE_8668_BIN4  0x09 8668 bin文件4(B40)
                      VER_MODULE_8668_BIN5  0x0A 8668 bin文件5(N41)
**           pAddr: 文件地址
**           pLen: 文件长度
**           pVer: 文件版本号
* @Retval :  执行结果
* @Author :
* @Data   :  2024-03-11
* @Note   :  
*****************************************************************************/
uint8_t Ver_GetRuFile(uint8_t ucType, uint32_t *pVer, uint32_t *pAddr, uint32_t *pLen)
{
	uint32_t ulIndex;
	
	//if((pAddr==NULL) || (pLen==NULL) || (pVer==NULL))
	//	return VER_EXE_FAIL;
	if(pVer != NULL) *pVer  = 23010101;/*默认值*/
	
	for(ulIndex=0; ulIndex<g_ulVerTableLen; ulIndex++)
	{
		if(g_staVerTable[ulIndex].ucVersionType == VER_TYPE_WORK)/*判断工作区版本*/
		{
			if((g_staVerTable[ulIndex].ucDeviceType == VER_DEVICE_RU) && (g_staVerTable[ulIndex].ucModuleType == ucType))
			{
				if((g_staVerTable[ulIndex].ulSoftVersion > 0) && (g_staVerTable[ulIndex].ulBinFileLen > 0))/*文件有效*/
				{
					if(pAddr!= NULL) *pAddr = g_staVerTable[ulIndex].ulFlashSSector*spi_flash.sector_size;
					if(pLen != NULL) *pLen  = g_staVerTable[ulIndex].ulBinFileLen + VER_FILE_HEAD_LEN;
					if(pVer != NULL) *pVer  = g_staVerTable[ulIndex].ulSoftVersion;
					return VER_EXE_SUCCESS;
				}
			}
		}
	}
	
	rtt_printf(RTT_UPGRADE, RTT_TEXT_YELLOW"Ver_GetRuFile: module 0x%x not found work file\r\n", ucType);
	return VER_EXE_FAIL;
}
/*****************************************************************************
* @Name	  :  Ver_Get5gModemFile
* @Brief  :  获取5G模组文件的地址 长度和版本号
* @Param  :  pAddr: 文件地址
**           pLen: 文件长度
**           pVer: 文件版本号
* @Retval :  执行结果
* @Author :
* @Data   :  2024-03-11
* @Note   :  
*****************************************************************************/
uint8_t Ver_Get5gModemFile(uint32_t *pVer, uint32_t *pAddr, uint32_t *pLen)
{
	uint32_t ulIndex;
	
	if((pAddr==NULL) || (pLen==NULL) || (pVer==NULL))
		return VER_EXE_FAIL;
	
	for(ulIndex=0; ulIndex<g_ulVerTableLen; ulIndex++)
	{
		if(g_staVerTable[ulIndex].ucVersionType == VER_TYPE_WORK)/*判断工作区版本*/
		{
			if(g_staVerTable[ulIndex].ucModuleType == VER_MODULE_5G_MODEM)
			{
				if((g_staVerTable[ulIndex].ulSoftVersion > 0) && (g_staVerTable[ulIndex].ulBinFileLen > 0))/*文件有效*/
				{
					*pAddr = g_staVerTable[ulIndex].ulFlashSSector*spi_flash.sector_size + VER_FILE_HEAD_LEN;
					*pLen  = g_staVerTable[ulIndex].ulBinFileLen;
					*pVer  = g_staVerTable[ulIndex].ulSoftVersion;
					return VER_EXE_SUCCESS;
				}
			}
		}
	}
	
	rtt_printf(RTT_UPGRADE, RTT_TEXT_YELLOW"Ver_Get5gModemFile: not found work file\r\n");
	return VER_EXE_FAIL;
}
