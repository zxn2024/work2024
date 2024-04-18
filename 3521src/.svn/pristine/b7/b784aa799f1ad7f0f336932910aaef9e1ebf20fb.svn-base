/***************************** Copyright ****************************************
*
* (C) Copyright 2022, Shaanxi Tianji Comunication
*  All Rights Reserved
*
* FileName   : bsp_upgrade.c
* Version    : none
* Author     : none
* Date       : 2024-02-28
* Description: upgrade
*******************************************************************************/
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "main.h"
#include "crc.h"
#include "monitor_para_list.h"
#include "bsp_config.h"
#include "bsp_reboot.h"
#include "app_version.h"
#include "app_upgrade.h"

#if (LOCAL_MODULE_TYPE == _MU)
#include "ru_device_list.h"
#endif
/*---------------------- 端口定义 --------------------------------------*/


/*---------------------- 宏定义  ---------------------------------------*/
#define UPGRADE_IDLE  (uint8_t)(0)  /*upgrade初始状态*/
#define UPGRADE_WORK  (uint8_t)(1)  /*upgrade工作状态*/ 

#define UPGRADE_RU_INIT        0/*初始*/
#define UPGRADE_RU_START_TRANS 1/*文件传输开始*/
#define UPGRADE_RU_FILE_DATA   2/*文件数据块*/
#define UPGRADE_RU_END_TRANS   3/*文件传输结束*/
#define UPGRADE_RU_COMPLETE    4/*软件升级正常结束*/
#define UPGRADE_RU_STATE_NUM   5/*升级RU状态数*/

#define UPGRADE_SEND_RU_MAX_NUM      3/*发送的最大次数*/
#define UPGRADE_SEND_RU_DATA_LEN     128/*发送数据长度*/

#define UPGRADE_SEND_RU_TIME_OUT     100/*10s*/
#define UPGRADE_CONN_RU_TIME_OUT     6000/*600s*/
#define UPGRADE_CHECK_RU_TIME_OUT    3000/*300s*/

#define UPGRADE_5G_MODEM_INIT        0/*初始*/
#define UPGRADE_5G_MODEM_CONNECT_MDL     1/*connect_mdl*/
#define UPGRADE_5G_MODEM_ERASE_FLASH     2/*读分区信息并擦除flash*/
#define UPGRADE_5G_MODEM_WRITE_FLASH     3/*写flash*/
#define UPGRADE_5G_MODEM_REBOOT_MDL      4/*复位*/
#define UPGRADE_5G_MODEM_STATE_NUM       5/*升级5G_MODEM状态数*/

#define UPGRADE_SEND_5G_MODEM_DATA_LEN     1024/*发送数据长度*/

#define UPGRADE_CHECK_5G_MODEM_TIME_OUT    900/*90s*/

/*---------------------- 结构体 ----------------------------------------*/
/*RU升级控制信息*/
typedef struct
{ 
	uint8_t       ucState;     /*状态 0初始 1表示文件传输开始 2文件数据块 3表示文件传输结束 4表示软件升级正常结束*/
	uint8_t       ucModuleType;/*升级模块类型*/
	uint8_t       ucSendNum;   /*发送次数*/
	uint8_t       ucRecvAck;   /*接收应答 0未收到 1文件传输开始应答 2文件数据块应答 3文件传输结束应答 4软件升级正常结束应答*/
	uint32_t      ulDataSeq;   /*数据块号*/
	uint32_t      ulFileAddr;  /*文件地址*/
	uint32_t      ulFileLen;   /*文件长度*/
	uint32_t      ulSendTimeOutCnt;   /*发送超时计数*/
	uint32_t      ulConnFailCnt;/*连接失败计数*/
	uint32_t      ulCheckVerCnt;/*版本检测计数*/
} stRuUpgradeCtrl;	

typedef void (*UPGRADE_RU_FUNC)(uint8_t slave_index);

/*5G MODEM升级控制信息*/
typedef struct
{ 
	uint8_t       ucState;     /*状态 0初始  1 AT+EFOTADL  2 DATA SEND  3 AT+EFOTAUPDATE*/
	uint8_t       ucIsUpgrade; /*1 升级*/   
	uint8_t       ucUpgradeNum;/*升级次数，升级3次失败，本次不再升级*/  	
	int16_t       usConnectMdlNum;/*connect mdl次数，3次失败，退出本次升级*/
	uint32_t      ulFileOff;   /*文件偏移*/
	uint32_t      ulFileAddr;  /*文件地址*/
	uint32_t      ulFileLen;   /*文件长度*/	
	uint8_t       ucImgSum;    /*aio文件分区总数*/  
	uint8_t       ucImgCount;  /*aio文件分区计数*/ 
	uint32_t      ulImgLen;    /*img长度*/  
	uint32_t      ulDstFlashAddr;/*img写入地址*/ 
	uint8_t       ucFlashSecFlag;/*img写入方式*/ 
	uint32_t      ulImgWriteLen;/*img写入长度*/ 
	uint8_t       ucImgWriteNum;/*img写入次数*/
	uint32_t      ulCheckVerCnt;/*版本检测计数*/	
} st5gModemUpgradeCtrl;	

/*5G MODEM版本文件分区信息*/
#pragma pack(1)
typedef struct {
	uint32_t partition_off;
	uint32_t partition_size;
	uint8_t  dev_type;
	uint8_t  flash_sec_flag;
	uint8_t  fota_flag;
	uint8_t  partition_name_len;
} partition_info_t;
#pragma pack()

typedef void (*UPGRADE_5G_MODEM_FUNC)(void);

typedef void (*UPGRADE_PROC_FUNC)(void);

/*---------------------- 常量 ------------------------------------------*/


/*---------------------- 全局变量 --------------------------------------*/


/*---------------------- 局部变量 --------------------------------------*/
static uint8_t g_u8UpgradeState = UPGRADE_IDLE;


/*---------------------- 函数声明 --------------------------------------*/


/*---------------------- 函数定义 --------------------------------------*/
static void Upgrade_Init(void);
static void Upgrade_Work(void);

static UPGRADE_PROC_FUNC g_UpgradeProcTable[] = {Upgrade_Init, Upgrade_Work};



#if (LOCAL_MODULE_TYPE == _MU)
static void UpgradeRuInit(uint8_t slave_index);
static void UpgradeRuStartTrans(uint8_t slave_index);
static void UpgradeRuFileData(uint8_t slave_index);
static void UpgradeRuEndTrans(uint8_t slave_index);
static void UpgradeRuComplete(uint8_t slave_index);


static void Upgrade5gModemInit(void);
static void Upgrade5gModemConnectMdl(void);
static void Upgrade5gModemEraseFlash(void);
static void Upgrade5gModemWriteFlash(void);
static void Upgrade5gModemRebootMdl(void);


/*RU升级状态处理表*/
static UPGRADE_RU_FUNC g_UpgradeRuFuncTable[UPGRADE_RU_STATE_NUM]={
	UpgradeRuInit      ,
	UpgradeRuStartTrans,
	UpgradeRuFileData  ,
	UpgradeRuEndTrans  ,
	UpgradeRuComplete
};
static stRuUpgradeCtrl g_staRuUpgradeCtrlTable[BLE_CONNECT_MAX_NUM];


/*5G MODEM升级状态处理表*/
static UPGRADE_5G_MODEM_FUNC g_Upgrade5gModemFuncTable[UPGRADE_5G_MODEM_STATE_NUM]={
	Upgrade5gModemInit       ,
	Upgrade5gModemConnectMdl ,
	Upgrade5gModemEraseFlash ,
	Upgrade5gModemWriteFlash ,
	Upgrade5gModemRebootMdl
};
static st5gModemUpgradeCtrl g_st5gModemUpgradeCtrl;

extern void mcpB_send_trans_ctrl(uint8_t ble_conn_id, uint8_t ctrl);
extern void mcpB_send_file_data(uint8_t ble_conn_id, uint32_t seq, uint8_t *pbuf, uint8_t len);

/*****************************************************************************
* @Name	  :  Upgrade_RuStart
* @Brief  :  开始RU版本升级
* @Param  :  无
* @Retval :  无
* @Author :
* @Data   :  2024-02-28
* @Note   :
*****************************************************************************/ 
void Upgrade_RuStart(void)
{
	uint32_t index;
	
	for(index=0; index<BLE_CONNECT_MAX_NUM; index++)
	{
		g_staRuUpgradeCtrlTable[index].ulCheckVerCnt = UPGRADE_CHECK_RU_TIME_OUT-10;/*1s后触发主机给从机升级*/
	}
}
/*****************************************************************************
* @Name	  :  UpgradeIsNeed
* @Brief  :  判断是否需要升级
* @Param  :  pCtrl
*            ucType
*            ver
*            pAddr
*            pLen
* @Retval :  1 需要升级
* @Author :
* @Data   :  2024-02-28
* @Note   :
*****************************************************************************/ 
static uint8_t UpgradeIsNeed(stRuUpgradeCtrl *pCtrl, uint8_t ucType, uint32_t ver, uint32_t *pAddr, uint32_t *pLen)
{
	uint32_t ulVer;
	
	if(ver != 0)
	{
		if(VER_EXE_SUCCESS == Ver_GetRuFile(ucType, &ulVer, pAddr, pLen))
		{
			if(ulVer != ver)/*版本号不一致，支持升级和回退*/
			{
				return 1;
			}
		}
	}	
	
	return 0;
}
/*****************************************************************************
* @Name	  :  UpgradeRuInit
* @Brief  :  Ru升级状态处理 初始化
* @Param  :  ctrlIndex
* @Retval :  无
* @Author :
* @Data   :  2024-02-28
* @Note   :
*****************************************************************************/ 
static void UpgradeRuInit(uint8_t slave_index)
{
	uint32_t ulAddr;
	uint32_t ulLen;
	uint8_t  ucFlag = 0;
	
	if(slave_index >= BLE_CONNECT_MAX_NUM)
		return;
	
	if((ru_device_list[slave_index].ru_conn_state == RU_CONN_STATE_CONFIGURED) && (ru_device_list[slave_index].dev_type == DEV_TYPE_RU))
	{
		if(g_staRuUpgradeCtrlTable[slave_index].ulCheckVerCnt > UPGRADE_CHECK_RU_TIME_OUT)
		{
			/*MCU boot*/
			
			
			/*MCU app*/
			if((0 == ucFlag) 
				&& (g_staRuUpgradeCtrlTable[slave_index].ucModuleType != VER_MODULE_MCU_APP)
				&& (1 == UpgradeIsNeed(&(g_staRuUpgradeCtrlTable[slave_index]), VER_MODULE_MCU_APP, ru_device_list[slave_index].pt_mrc_ru_data->mcu_sw_ver_u32, &ulAddr, &ulLen)))
			{
				g_staRuUpgradeCtrlTable[slave_index].ucModuleType = VER_MODULE_MCU_APP;				
				ucFlag = 1;
			}
			
			/*FPGA*/
			if((0 == ucFlag) 
				&& (g_staRuUpgradeCtrlTable[slave_index].ucModuleType != VER_MODULE_FPGA)				
				&& (1 == UpgradeIsNeed(&(g_staRuUpgradeCtrlTable[slave_index]), VER_MODULE_FPGA, ru_device_list[slave_index].pt_mrc_ru_data->fpga_sw_ver, &ulAddr, &ulLen)))
			{
				g_staRuUpgradeCtrlTable[slave_index].ucModuleType = VER_MODULE_FPGA;				
				ucFlag = 1;
			}
		
			/*8668 bin文件1(B3)*/
			if((0 == ucFlag) 
				&& (g_staRuUpgradeCtrlTable[slave_index].ucModuleType != VER_MODULE_8668_BIN1)					
				&& (1 == UpgradeIsNeed(&(g_staRuUpgradeCtrlTable[slave_index]), VER_MODULE_8668_BIN1, ru_device_list[slave_index].pt_mrc_ru_data->ecr8668_fw1_ver, &ulAddr, &ulLen)))
			{
				g_staRuUpgradeCtrlTable[slave_index].ucModuleType = VER_MODULE_8668_BIN1;				
				ucFlag = 1;
			}
		
			/*8668 bin文件2(B8)*/
			if((0 == ucFlag) 
				&& (g_staRuUpgradeCtrlTable[slave_index].ucModuleType != VER_MODULE_8668_BIN2)					
				&& (1 == UpgradeIsNeed(&(g_staRuUpgradeCtrlTable[slave_index]), VER_MODULE_8668_BIN2, ru_device_list[slave_index].pt_mrc_ru_data->ecr8668_fw2_ver, &ulAddr, &ulLen)))
			{
				g_staRuUpgradeCtrlTable[slave_index].ucModuleType = VER_MODULE_8668_BIN2;				
				ucFlag = 1;
			}	
		
			/*8668 bin文件3(B39)*/
			if((0 == ucFlag) 
				&& (g_staRuUpgradeCtrlTable[slave_index].ucModuleType != VER_MODULE_8668_BIN3)				
				&& (1 == UpgradeIsNeed(&(g_staRuUpgradeCtrlTable[slave_index]), VER_MODULE_8668_BIN3, ru_device_list[slave_index].pt_mrc_ru_data->ecr8668_fw3_ver, &ulAddr, &ulLen)))
			{
				g_staRuUpgradeCtrlTable[slave_index].ucModuleType = VER_MODULE_8668_BIN3;				
				ucFlag = 1;
			}		
		
			/*8668 bin文件4(B40)*/
			if((0 == ucFlag) 
				&& (g_staRuUpgradeCtrlTable[slave_index].ucModuleType != VER_MODULE_8668_BIN4)					
				&& (1 == UpgradeIsNeed(&(g_staRuUpgradeCtrlTable[slave_index]), VER_MODULE_8668_BIN4, ru_device_list[slave_index].pt_mrc_ru_data->ecr8668_fw4_ver, &ulAddr, &ulLen)))
			{
				g_staRuUpgradeCtrlTable[slave_index].ucModuleType = VER_MODULE_8668_BIN4;				
				ucFlag = 1;
			}	
		
			/*8668 bin文件5(N41)*/
			if((0 == ucFlag) 
				&& (g_staRuUpgradeCtrlTable[slave_index].ucModuleType != VER_MODULE_8668_BIN5)					
				&& (1 == UpgradeIsNeed(&(g_staRuUpgradeCtrlTable[slave_index]), VER_MODULE_8668_BIN5, ru_device_list[slave_index].pt_mrc_ru_data->ecr8668_fw5_ver, &ulAddr, &ulLen)))
			{
				g_staRuUpgradeCtrlTable[slave_index].ucModuleType = VER_MODULE_8668_BIN5;				
				ucFlag = 1;
			}	
		
			rtt_printf(RTT_UPGRADE, RTT_TEXT_YELLOW"UpgradeRuInit: slave_index %d  mcu_sw_ver      %d\r\n", slave_index, ru_device_list[slave_index].pt_mrc_ru_data->mcu_sw_ver_u32);
			rtt_printf(RTT_UPGRADE, RTT_TEXT_YELLOW"UpgradeRuInit: slave_index %d  fpga_sw_ver     %d\r\n", slave_index, ru_device_list[slave_index].pt_mrc_ru_data->fpga_sw_ver);
			rtt_printf(RTT_UPGRADE, RTT_TEXT_YELLOW"UpgradeRuInit: slave_index %d  ecr8668_fw1_ver %d\r\n", slave_index, ru_device_list[slave_index].pt_mrc_ru_data->ecr8668_fw1_ver);
			rtt_printf(RTT_UPGRADE, RTT_TEXT_YELLOW"UpgradeRuInit: slave_index %d  ecr8668_fw2_ver %d\r\n", slave_index, ru_device_list[slave_index].pt_mrc_ru_data->ecr8668_fw2_ver);
			rtt_printf(RTT_UPGRADE, RTT_TEXT_YELLOW"UpgradeRuInit: slave_index %d  ecr8668_fw3_ver %d\r\n", slave_index, ru_device_list[slave_index].pt_mrc_ru_data->ecr8668_fw3_ver);
			rtt_printf(RTT_UPGRADE, RTT_TEXT_YELLOW"UpgradeRuInit: slave_index %d  ecr8668_fw4_ver %d\r\n", slave_index, ru_device_list[slave_index].pt_mrc_ru_data->ecr8668_fw4_ver);
			rtt_printf(RTT_UPGRADE, RTT_TEXT_YELLOW"UpgradeRuInit: slave_index %d  ecr8668_fw5_ver %d\r\n", slave_index, ru_device_list[slave_index].pt_mrc_ru_data->ecr8668_fw5_ver);
			
			if(1 == ucFlag)
			{
				g_staRuUpgradeCtrlTable[slave_index].ucSendNum    = 0;
				g_staRuUpgradeCtrlTable[slave_index].ucRecvAck    = 0;
				g_staRuUpgradeCtrlTable[slave_index].ulDataSeq    = 0;
				g_staRuUpgradeCtrlTable[slave_index].ulFileAddr   = ulAddr;
				g_staRuUpgradeCtrlTable[slave_index].ulFileLen    = ulLen;
				g_staRuUpgradeCtrlTable[slave_index].ulSendTimeOutCnt    = UPGRADE_SEND_RU_TIME_OUT;
				g_staRuUpgradeCtrlTable[slave_index].ulConnFailCnt= 0;
				
				/*状态跳转 文件传输开始*/
				rtt_printf(RTT_UPGRADE, RTT_TEXT_YELLOW"UpgradeRuInit: slave_index %d  module_type %d upgrade\r\n", slave_index, g_staRuUpgradeCtrlTable[slave_index].ucModuleType);
				g_staRuUpgradeCtrlTable[slave_index].ucState      = UPGRADE_RU_START_TRANS;	
			}
			else
			{
				g_staRuUpgradeCtrlTable[slave_index].ucModuleType++;
				g_staRuUpgradeCtrlTable[slave_index].ulCheckVerCnt = 0;	
			}				
		}
	}
}
/*****************************************************************************
* @Name	  :  UpgradeRuStartTrans
* @Brief  :  Ru升级状态处理 开始传输
* @Param  :  ctrlIndex
* @Retval :  无
* @Author :
* @Data   :  2024-02-28
* @Note   :
*****************************************************************************/ 
static void UpgradeRuStartTrans(uint8_t slave_index)
{
	if(slave_index >= BLE_CONNECT_MAX_NUM)
		return;
	
	if(g_staRuUpgradeCtrlTable[slave_index].ucRecvAck == UPGRADE_RU_START_TRANS)
	{
		g_staRuUpgradeCtrlTable[slave_index].ucSendNum    = 0;
		g_staRuUpgradeCtrlTable[slave_index].ucRecvAck    = 0;
		g_staRuUpgradeCtrlTable[slave_index].ulDataSeq    = 0;
		g_staRuUpgradeCtrlTable[slave_index].ulSendTimeOutCnt    = UPGRADE_SEND_RU_TIME_OUT;
		g_staRuUpgradeCtrlTable[slave_index].ulConnFailCnt= 0;
		
		/*状态跳转 文件数据块*/	
		rtt_printf(RTT_UPGRADE, RTT_TEXT_YELLOW"UpgradeRuStartTrans: slave_index %d  module_type %d ok\r\n", slave_index, g_staRuUpgradeCtrlTable[slave_index].ucModuleType);
		g_staRuUpgradeCtrlTable[slave_index].ucState      = UPGRADE_RU_FILE_DATA;			
	}
	else
	{
		if(g_staRuUpgradeCtrlTable[slave_index].ulSendTimeOutCnt > UPGRADE_SEND_RU_TIME_OUT)
		{
			g_staRuUpgradeCtrlTable[slave_index].ulSendTimeOutCnt = 0;
			
			if(g_staRuUpgradeCtrlTable[slave_index].ucSendNum > UPGRADE_SEND_RU_MAX_NUM)/*连续发送失败*/
			{
				g_staRuUpgradeCtrlTable[slave_index].ulCheckVerCnt= 0;
				
				/*状态跳转 返回初始 过一会再尝试发送*/
				rtt_printf(RTT_UPGRADE, RTT_TEXT_RED"UpgradeRuStartTrans: slave_index %d  module_type %d fail!\r\n", slave_index, g_staRuUpgradeCtrlTable[slave_index].ucModuleType);
				g_staRuUpgradeCtrlTable[slave_index].ucState      = UPGRADE_RU_INIT;	
			}
			else
			{
				/*发送文件传输开始消息*/
				mcpB_send_trans_ctrl(ru_device_list[slave_index].ble_conn_id, MCPB_TRANS_CTRL_START);				
				g_staRuUpgradeCtrlTable[slave_index].ucSendNum++;
			}
		}
	}
}
/*****************************************************************************
* @Name	  :  UpgradeRuStartTrans
* @Brief  :  Ru升级状态处理 开始传输
* @Param  :  ctrlIndex
* @Retval :  无
* @Author :
* @Data   :  2024-02-28
* @Note   :
*****************************************************************************/ 
static void UpgradeRuFileData(uint8_t slave_index)
{
	uint8_t ucaData[UPGRADE_SEND_RU_DATA_LEN];
	uint32_t ulLen;
	
	if(slave_index >= BLE_CONNECT_MAX_NUM)
		return;
	
	if(g_staRuUpgradeCtrlTable[slave_index].ucRecvAck == UPGRADE_RU_FILE_DATA)
	{
		g_staRuUpgradeCtrlTable[slave_index].ulDataSeq++;
		ulLen = g_staRuUpgradeCtrlTable[slave_index].ulDataSeq*UPGRADE_SEND_RU_DATA_LEN;
		if(ulLen > g_staRuUpgradeCtrlTable[slave_index].ulFileLen)
		{
			/*发送完成*/
			g_staRuUpgradeCtrlTable[slave_index].ucSendNum    = 0;
			g_staRuUpgradeCtrlTable[slave_index].ucRecvAck    = 0;
			g_staRuUpgradeCtrlTable[slave_index].ulDataSeq    = 0;
			g_staRuUpgradeCtrlTable[slave_index].ulSendTimeOutCnt    = UPGRADE_SEND_RU_TIME_OUT;
			g_staRuUpgradeCtrlTable[slave_index].ulConnFailCnt= 0;
			
			/*状态跳转 文件传输结束*/	
			rtt_printf(RTT_UPGRADE, RTT_TEXT_YELLOW"UpgradeRuFileData: slave_index %d  module_type %d ok\r\n", slave_index, g_staRuUpgradeCtrlTable[slave_index].ucModuleType);
			g_staRuUpgradeCtrlTable[slave_index].ucState      = UPGRADE_RU_END_TRANS;					
		}
		else
		{
			/*读FLASH, 发送下一包*/
			if(STAT_OK != spi_flash.flash_read(ucaData, g_staRuUpgradeCtrlTable[slave_index].ulFileAddr+ulLen, UPGRADE_SEND_RU_DATA_LEN))
			{
				rtt_printf(RTT_UPGRADE, RTT_TEXT_RED"UpgradeRuFileData: %d flash_read 0x%x fail!\r\n", slave_index, g_staRuUpgradeCtrlTable[slave_index].ulFileAddr+ulLen);
			}			
			
			mcpB_send_file_data(ru_device_list[slave_index].ble_conn_id, g_staRuUpgradeCtrlTable[slave_index].ulDataSeq, ucaData, UPGRADE_SEND_RU_DATA_LEN);
		
			g_staRuUpgradeCtrlTable[slave_index].ucSendNum    = 0;
			g_staRuUpgradeCtrlTable[slave_index].ucRecvAck    = 0;
			g_staRuUpgradeCtrlTable[slave_index].ulSendTimeOutCnt    = 0;
			g_staRuUpgradeCtrlTable[slave_index].ulConnFailCnt= 0;				
		}	
	}
	else
	{
		if(g_staRuUpgradeCtrlTable[slave_index].ulSendTimeOutCnt > UPGRADE_SEND_RU_TIME_OUT)
		{
			g_staRuUpgradeCtrlTable[slave_index].ulSendTimeOutCnt = 0;
			
			if(g_staRuUpgradeCtrlTable[slave_index].ucSendNum > UPGRADE_SEND_RU_MAX_NUM)/*连续发送失败*/
			{
				g_staRuUpgradeCtrlTable[slave_index].ulCheckVerCnt= 0;
				
				/*状态跳转 返回初始 过一会再尝试发送*/
				rtt_printf(RTT_UPGRADE, RTT_TEXT_RED"UpgradeRuFileData: slave_index %d  module_type %d fail!\r\n", slave_index, g_staRuUpgradeCtrlTable[slave_index].ucModuleType);			
				g_staRuUpgradeCtrlTable[slave_index].ucState      = UPGRADE_RU_INIT;
			}
			else
			{
				/*发送失败，读FLASH, 继续发送当前包*/
				ulLen = g_staRuUpgradeCtrlTable[slave_index].ulDataSeq*UPGRADE_SEND_RU_DATA_LEN;
				if(STAT_OK != spi_flash.flash_read(ucaData, g_staRuUpgradeCtrlTable[slave_index].ulFileAddr+ulLen, UPGRADE_SEND_RU_DATA_LEN))
				{
					rtt_printf(RTT_UPGRADE, RTT_TEXT_RED"UpgradeRuFileData: %d flash_read 0x%x fail!\r\n", slave_index, g_staRuUpgradeCtrlTable[slave_index].ulFileAddr+ulLen);
				}			
				
				mcpB_send_file_data(ru_device_list[slave_index].ble_conn_id, g_staRuUpgradeCtrlTable[slave_index].ulDataSeq, ucaData, UPGRADE_SEND_RU_DATA_LEN);
				
				/*rtt_printf(RTT_UPGRADE, RTT_TEXT_YELLOW"UpgradeRuFileData: slave_index %d  module_type %d  send %d\r\n", 
									slave_index, g_staRuUpgradeCtrlTable[slave_index].ucModuleType, g_staRuUpgradeCtrlTable[slave_index].ulDataSeq);*/						
				
				g_staRuUpgradeCtrlTable[slave_index].ucSendNum    ++;
			}			
		}
	}	
}
/*****************************************************************************
* @Name	  :  UpgradeRuEndTrans
* @Brief  :  Ru升级状态处理 传输结束
* @Param  :  ctrlIndex
* @Retval :  无
* @Author :
* @Data   :  2024-02-28
* @Note   :
*****************************************************************************/ 
static void UpgradeRuEndTrans(uint8_t slave_index)
{
	if(slave_index >= BLE_CONNECT_MAX_NUM)
		return;
	
	if(g_staRuUpgradeCtrlTable[slave_index].ucRecvAck == UPGRADE_RU_END_TRANS)
	{
		g_staRuUpgradeCtrlTable[slave_index].ucSendNum    = 0;
		g_staRuUpgradeCtrlTable[slave_index].ucRecvAck    = 0;
		g_staRuUpgradeCtrlTable[slave_index].ulDataSeq    = 0;
		g_staRuUpgradeCtrlTable[slave_index].ulSendTimeOutCnt    = UPGRADE_SEND_RU_TIME_OUT;
		g_staRuUpgradeCtrlTable[slave_index].ulConnFailCnt= 0;
		
		/*状态跳转 升级完成*/	
		rtt_printf(RTT_UPGRADE, RTT_TEXT_YELLOW"UpgradeRuEndTrans: slave_index %d  module_type %d ok\r\n", slave_index, g_staRuUpgradeCtrlTable[slave_index].ucModuleType);			
		g_staRuUpgradeCtrlTable[slave_index].ucState      = UPGRADE_RU_COMPLETE;			
	}
	else
	{
		if(g_staRuUpgradeCtrlTable[slave_index].ulSendTimeOutCnt > UPGRADE_SEND_RU_TIME_OUT)
		{
			g_staRuUpgradeCtrlTable[slave_index].ulSendTimeOutCnt = 0;
			
			if(g_staRuUpgradeCtrlTable[slave_index].ucSendNum > UPGRADE_SEND_RU_MAX_NUM)/*连续发送失败*/
			{
				g_staRuUpgradeCtrlTable[slave_index].ulCheckVerCnt= 0;
				
				/*状态跳转 返回初始 过一会再尝试发送*/
				rtt_printf(RTT_UPGRADE, RTT_TEXT_RED"UpgradeRuEndTrans: slave_index %d  module_type %d fail!\r\n", slave_index, g_staRuUpgradeCtrlTable[slave_index].ucModuleType);				
				g_staRuUpgradeCtrlTable[slave_index].ucState      = UPGRADE_RU_INIT;	
			}
			else
			{
				/*发送文件传输结束消息*/
				mcpB_send_trans_ctrl(ru_device_list[slave_index].ble_conn_id, MCPB_TRANS_CTRL_END);				
				g_staRuUpgradeCtrlTable[slave_index].ucSendNum++;
			}
		}
	}	
}
/*****************************************************************************
* @Name	  :  UpgradeRuComplete
* @Brief  :  Ru升级状态处理 升级完成
* @Param  :  ctrlIndex
* @Retval :  无
* @Author :
* @Data   :  2024-02-28
* @Note   :
*****************************************************************************/ 
static void UpgradeRuComplete(uint8_t slave_index)
{
	if(slave_index >= BLE_CONNECT_MAX_NUM)
		return;
	
	if(g_staRuUpgradeCtrlTable[slave_index].ucRecvAck == UPGRADE_RU_COMPLETE)
	{
		g_staRuUpgradeCtrlTable[slave_index].ucSendNum    = 0;
		g_staRuUpgradeCtrlTable[slave_index].ucRecvAck    = 0;
		g_staRuUpgradeCtrlTable[slave_index].ulDataSeq    = 0;
		g_staRuUpgradeCtrlTable[slave_index].ulSendTimeOutCnt    = 0;
		g_staRuUpgradeCtrlTable[slave_index].ulConnFailCnt= 0;
		
		g_staRuUpgradeCtrlTable[slave_index].ulCheckVerCnt= 2400;/*等待从机重启，1分钟后触发下一个版本下载*/
		
		/*状态跳转 升级完成*/	
		rtt_printf(RTT_UPGRADE, RTT_TEXT_YELLOW"UpgradeRuComplete: slave_index %d  module_type %d ok\r\n", slave_index, g_staRuUpgradeCtrlTable[slave_index].ucModuleType);			
		g_staRuUpgradeCtrlTable[slave_index].ucState      = UPGRADE_RU_INIT;			
	}
	else
	{
		if(g_staRuUpgradeCtrlTable[slave_index].ulSendTimeOutCnt > UPGRADE_SEND_RU_TIME_OUT)
		{
			g_staRuUpgradeCtrlTable[slave_index].ulSendTimeOutCnt = 0;
			
			if(g_staRuUpgradeCtrlTable[slave_index].ucSendNum > UPGRADE_SEND_RU_MAX_NUM)/*连续发送失败*/
			{
				g_staRuUpgradeCtrlTable[slave_index].ulCheckVerCnt= 0;
				
				/*状态跳转 返回初始 过一会再尝试发送*/
				rtt_printf(RTT_UPGRADE, RTT_TEXT_RED"UpgradeRuComplete: slave_index %d  module_type %d fail!\r\n", slave_index, g_staRuUpgradeCtrlTable[slave_index].ucModuleType);			
				g_staRuUpgradeCtrlTable[slave_index].ucState      = UPGRADE_RU_INIT;	
			}
			else
			{
				/*发送文件传输完成消息*/
				mcpB_send_trans_ctrl(ru_device_list[slave_index].ble_conn_id, MCPB_TRANS_CTRL_COMPLETE);				
				g_staRuUpgradeCtrlTable[slave_index].ucSendNum++;
			}
		}
	}		
}

/*****************************************************************************
 * @Name	  : Upgrade_recv_trans_ctrl
 * @Brief  : MCP_B协议接收消息
 * @Param  : slave_index
 *           ctrl:MCPB_TRANS_CTRL_START    0x01 文件传输开始
                  MCPB_TRANS_CTRL_END      0x02 文件传输结束
                  MCPB_TRANS_CTRL_CANCEL   0x03 监控中心取消软件升级
                  MCPB_TRANS_CTRL_COMPLETE 0x04 软件升级正常结束
 * @Retval : 无
 * @Author :
 * @Data   : 2024-3-12
 * @Note   :
 *****************************************************************************/
void Upgrade_recv_trans_ctrl(uint8_t slave_index, uint8_t ctrl)
{
	if(slave_index >= BLE_CONNECT_MAX_NUM)
		return;

	if(ctrl == MCPB_TRANS_CTRL_START)/*表示文件传输开始*/
		g_staRuUpgradeCtrlTable[slave_index].ucRecvAck = UPGRADE_RU_START_TRANS;
	else if(ctrl == MCPB_TRANS_CTRL_END)/*表示文件传输结束*/
		g_staRuUpgradeCtrlTable[slave_index].ucRecvAck = UPGRADE_RU_END_TRANS;
	else if(ctrl == MCPB_TRANS_CTRL_COMPLETE)/*表示软件升级正常结束*/
		g_staRuUpgradeCtrlTable[slave_index].ucRecvAck = UPGRADE_RU_COMPLETE;
	else
		;
}
/*****************************************************************************
 * @Name	  : Upgrade_recv_pack_num
 * @Brief  : MCP_B协议接收消息
 * @Param  : slave_index
 *           seq:
 * @Retval : 无
 * @Author :
 * @Data   : 2024-3-12
 * @Note   :
 *****************************************************************************/
void Upgrade_recv_pack_num(uint8_t slave_index, uint32_t seq)
{
	if(slave_index >= BLE_CONNECT_MAX_NUM)
		return;

	if(g_staRuUpgradeCtrlTable[slave_index].ulDataSeq == seq)
		g_staRuUpgradeCtrlTable[slave_index].ucRecvAck = UPGRADE_RU_FILE_DATA;
}

/*****************************************************************************
 * @Name	  : Upgrade_set_5g_modem
 * @Brief  : 设置5G MODEM升级
 * @Param  : isUpgrade 是否升级
             isDelay 是否延时升级
 * @Retval : 无
 * @Author :
 * @Data   : 2024-3-12
 * @Note   :
 *****************************************************************************/
void Upgrade_set_5g_modem(uint8_t isUpgrade, uint8_t isDelay)
{
	if(isUpgrade == 1)
	{
		g_st5gModemUpgradeCtrl.ucIsUpgrade = 1;
		if(isDelay == 1)
			g_st5gModemUpgradeCtrl.ulCheckVerCnt = 0;
		else
			g_st5gModemUpgradeCtrl.ulCheckVerCnt = UPGRADE_CHECK_5G_MODEM_TIME_OUT;
		g_st5gModemUpgradeCtrl.ucUpgradeNum = 0;
	}
	else
	{
		g_st5gModemUpgradeCtrl.ucIsUpgrade = 0;
	}
}

/*****************************************************************************
 * @Name	  : Upgrade5gModemInit
 * @Brief  : 5G MODEM升级状态处理 初始态
 * @Param  : 无
 * @Retval : 无
 * @Author :
 * @Data   : 2024-3-12
 * @Note   :
 *****************************************************************************/
static uint8_t g_uca5gModemData[UPGRADE_SEND_5G_MODEM_DATA_LEN];
static void Upgrade5gModemInit(void)
{
	uint32_t ulAddr;
	uint32_t ulLen;
	uint32_t ulVer;
	uint8_t aio_header[4];	
	
	if(g_st5gModemUpgradeCtrl.ulCheckVerCnt > UPGRADE_CHECK_5G_MODEM_TIME_OUT)
	{
		if(g_st5gModemUpgradeCtrl.ucUpgradeNum < 10)/*升级次数大于10次，不再升级*/
		{
			if(g_st5gModemUpgradeCtrl.ucIsUpgrade == 1)
			{
				g_st5gModemUpgradeCtrl.ucUpgradeNum++;
				
				if(VER_EXE_SUCCESS == Ver_Get5gModemFile(&ulVer, &ulAddr, &ulLen))
				{
					g_st5gModemUpgradeCtrl.ulFileOff = 0;
					g_st5gModemUpgradeCtrl.ulFileAddr= ulAddr;
					g_st5gModemUpgradeCtrl.ulFileLen = ulLen;
					
					rtt_printf(RTT_UPGRADE, RTT_TEXT_YELLOW"Upgrade5gModemInit: ulAddr 0x%x  ulLen %d\r\n", ulAddr, ulLen);
							
					/*读FLASH，得到aio header*/
					ulAddr = g_st5gModemUpgradeCtrl.ulFileAddr+g_st5gModemUpgradeCtrl.ulFileOff;
					if(STAT_OK != spi_flash.flash_read(aio_header, ulAddr, 4))
					{
						rtt_printf(RTT_UPGRADE, RTT_TEXT_RED"Upgrade5gModemInit: flash_read 0x%x fail!\r\n", ulAddr);
					}
					else
					{
					    if ((aio_header[0] != 0x41) || (aio_header[1] != 0x49) || (aio_header[2] != 0x4F)) 
					    {
							rtt_printf(RTT_UPGRADE, RTT_TEXT_RED"Upgrade5gModemInit: aio magic check fail!\r\n");
					    }
					    else
					    {
							g_st5gModemUpgradeCtrl.usConnectMdlNum = 0;
							g_st5gModemUpgradeCtrl.ucImgCount = 0;
							g_st5gModemUpgradeCtrl.ucImgSum = aio_header[3];
							
							g_st5gModemUpgradeCtrl.ulFileOff += 4;
							
							/*状态跳转*/
							g_st5gModemUpgradeCtrl.ucState   = UPGRADE_5G_MODEM_CONNECT_MDL;
							/*rtt_printf(RTT_UPGRADE, RTT_TEXT_YELLOW"Upgrade5gModemInit: start connect mdl\r\n");*/
					    }
					}	
				}
			}			
		}

		g_st5gModemUpgradeCtrl.ulCheckVerCnt = 0;	
	}
}
/*****************************************************************************
 * @Name	  : Upgrade5gModemJumpInitState
 * @Brief  : 5G MODEM升级状态处理 跳转到初始态
 * @Param  : 无
 * @Retval : 无
 * @Author :
 * @Data   : 2024-3-12
 * @Note   :
 *****************************************************************************/
static void Upgrade5gModemJumpInitState(void)
{
	modem_upgrade_end();
	//vModemPwrOn(); //在modem_upgrade_end()中已经进行过电源复位操作
	g_st5gModemUpgradeCtrl.ulCheckVerCnt = 0;
	g_st5gModemUpgradeCtrl.ucState = UPGRADE_5G_MODEM_INIT;
	rtt_printf(RTT_UPGRADE, RTT_TEXT_RED"Upgrade5gModemJumpInitState: jump init state %d\r\n", g_st5gModemUpgradeCtrl.ucUpgradeNum);	
}
/*****************************************************************************
 * @Name	  : Upgrade5gModemConnectMdl
 * @Brief  : 5G MODEM升级状态处理 ConnectMdl
 * @Param  : 无
 * @Retval : 无
 * @Author :
 * @Data   : 2024-3-12
 * @Note   :
 *****************************************************************************/
static void Upgrade5gModemConnectMdl(void)
{
	int ret;
	uint8_t ack;
	uint32_t wait_ack_time = 20, send_cnts = 200, i;
	
	/*ConnectMdl*/
	rtt_printf(RTT_UPGRADE, RTT_TEXT_YELLOW"Upgrade5gModemConnectMdl: start %d\r\n", g_st5gModemUpgradeCtrl.usConnectMdlNum);

	g_st5gModemUpgradeCtrl.usConnectMdlNum++;
	if (g_st5gModemUpgradeCtrl.usConnectMdlNum > 3)
	{
		/*3次连接失败，退出升级，返回初始态*/
		Upgrade5gModemJumpInitState();
	}
	else
	{
		modem_upgrade_start();
		vModemPwrOn();
		vTaskDelay(wait_ack_time);
		for (i=0; i<send_cnts; i++) 
		{
			modem_upgrade_send_req(TYPE_HANDSHAKE, NULL, 0, NULL, 0);
			vTaskDelay(wait_ack_time);
			ret = modem_upgrade_recv_ack(TYPE_HANDSHAKE, &ack);		
			if ((ret == BURN_OK) && (ack == 0)) 
			{
				/*握手成功，状态跳转*/
				g_st5gModemUpgradeCtrl.ucState = UPGRADE_5G_MODEM_ERASE_FLASH;
				rtt_printf(RTT_UPGRADE, RTT_TEXT_YELLOW"Upgrade5gModemConnectMdl: jump erase flash state\r\n");
				break;
			}
		}
	}
}
/*****************************************************************************
 * @Name	  : Upgrade5gModemEraseFlash
 * @Brief  : 5G MODEM升级状态处理 EraseFlash
 * @Param  : 无
 * @Retval : 无
 * @Author :
 * @Data   : 2024-3-12
 * @Note   :
 *****************************************************************************/
static void Upgrade5gModemEraseFlash(void)
{
    partition_info_t partition_info;
	uint8_t img_name_len;
	uint32_t data_len;
	uint8_t ack;
	uint32_t buf_head[2];	
	uint16_t index;
	int ret;
	uint32_t ulAddr;
	
	g_st5gModemUpgradeCtrl.ucImgCount++;
	if (g_st5gModemUpgradeCtrl.ucImgCount > g_st5gModemUpgradeCtrl.ucImgSum)
	{
		/*升级完成进入复位状态*/
		g_st5gModemUpgradeCtrl.ucState = UPGRADE_5G_MODEM_REBOOT_MDL;
		rtt_printf(RTT_UPGRADE, RTT_TEXT_YELLOW"Upgrade5gModemEraseFlash: jump reboot mdl state\r\n");
	}
	else
	{
		/*读FLASH，得到分区信息、img name len、deta len*/
		ulAddr = g_st5gModemUpgradeCtrl.ulFileAddr+g_st5gModemUpgradeCtrl.ulFileOff;
		if(STAT_OK != spi_flash.flash_read(g_uca5gModemData, ulAddr, sizeof(partition_info_t)+1+4))
		{
			rtt_printf(RTT_UPGRADE, RTT_TEXT_RED"Upgrade5gModemEraseFlash: flash_read 0x%x fail!\r\n", ulAddr);
			Upgrade5gModemJumpInitState();				
		}
		else
		{	
			memcpy((uint8_t *)&partition_info, g_uca5gModemData, sizeof(partition_info_t));
			memcpy((uint8_t *)&img_name_len, g_uca5gModemData+sizeof(partition_info_t), 1);
			memcpy((uint8_t *)&data_len, g_uca5gModemData+sizeof(partition_info_t)+1, 4);
		
			g_st5gModemUpgradeCtrl.ulFileOff += (sizeof(partition_info_t)+1+4);
			ulAddr = g_st5gModemUpgradeCtrl.ulFileAddr+g_st5gModemUpgradeCtrl.ulFileOff;
			if(STAT_OK != spi_flash.flash_read(g_uca5gModemData, ulAddr, partition_info.partition_name_len))
			{
				rtt_printf(RTT_UPGRADE, RTT_TEXT_RED"Upgrade5gModemEraseFlash: flash_read 0x%x fail!\r\n", ulAddr);
				Upgrade5gModemJumpInitState();
			}
			else
			{
				g_uca5gModemData[partition_info.partition_name_len] = 0;
				g_st5gModemUpgradeCtrl.ulFileOff += partition_info.partition_name_len;
				g_st5gModemUpgradeCtrl.ulFileOff += img_name_len;
				
				rtt_printf(RTT_UPGRADE, RTT_TEXT_YELLOW"Upgrade5gModemEraseFlash: partition name:%s off:0x%x len:%d flag:%d\r\n",g_uca5gModemData,partition_info.partition_off,data_len,partition_info.flash_sec_flag);
				
				if ((data_len == 0) || (strcmp((char *)g_uca5gModemData, PARTITION_NAME_PARTITION) == 0) || (strcmp((char *)g_uca5gModemData, PARTITION_NAME_UBOOT) == 0)) 
				{
					/*跳过partition和uboot*/
					g_st5gModemUpgradeCtrl.ulFileOff += data_len;
				}			
				else
				{
					g_st5gModemUpgradeCtrl.ulDstFlashAddr = partition_info.partition_off;
					g_st5gModemUpgradeCtrl.ulImgLen = data_len;
					g_st5gModemUpgradeCtrl.ulImgWriteLen = 0;
					g_st5gModemUpgradeCtrl.ucFlashSecFlag = partition_info.flash_sec_flag;
					g_st5gModemUpgradeCtrl.ucImgWriteNum = 0;
										
					/*擦除modem flash*/
					if ((data_len % MODEM_FLASH_SECTOR_SIZE) != 0) 
					{
						data_len = (data_len / MODEM_FLASH_SECTOR_SIZE + 1) * MODEM_FLASH_SECTOR_SIZE;
					}
					buf_head[0] = partition_info.partition_off;/*擦除地址*/
					buf_head[1] = data_len;/*擦除长度*/
					
					/*发送擦除*/
					modem_upgrade_send_req(TYPE_ERASE_BLOCK_NOALIGN, (uint8_t *)&buf_head, 8, NULL, 0);
					for (index=0; index<750; index++)
					{
						vTaskDelay(20);
						ret = modem_upgrade_recv_ack(TYPE_ERASE_BLOCK_NOALIGN, &ack);
						if ((ret == BURN_OK) && (ack == 0))
						{
							break;
						}
					}
					
					if (index < 750)
					{
						/*状态跳转*/
						g_st5gModemUpgradeCtrl.ucState = UPGRADE_5G_MODEM_WRITE_FLASH;		
						rtt_printf(RTT_UPGRADE, RTT_TEXT_YELLOW"Upgrade5gModemEraseFlash: jump write flash state\r\n");
					}
					else
					{
						/*状态跳转*/
						//g_st5gModemUpgradeCtrl.ucImgCount = 0;							
						//g_st5gModemUpgradeCtrl.ulFileOff = 4;						
						//g_st5gModemUpgradeCtrl.ucState = UPGRADE_5G_MODEM_CONNECT_MDL;		
						//rtt_printf(RTT_UPGRADE, RTT_TEXT_RED"Upgrade5gModemEraseFlash: erase flash fail, jump connect mdl state\r\n");	

						g_st5gModemUpgradeCtrl.ucState = UPGRADE_5G_MODEM_WRITE_FLASH;
						rtt_printf(RTT_UPGRADE, RTT_TEXT_RED"Upgrade5gModemEraseFlash: erase flash timeout, jump write flash state\r\n");						
					}
				}
			}		
		}
	}
}

/*****************************************************************************
 * @Name	  : Upgrade5gModemWriteFlash
 * @Brief  : 5G MODEM升级状态处理 WriteFlash
 * @Param  : 无
 * @Retval : 无
 * @Author :
 * @Data   : 2024-3-12
 * @Note   :
 *****************************************************************************/
static void Upgrade5gModemWriteFlash(void)
{
	uint8_t ack;
	int ret;	
	uint8_t buf_head[9];
	uint32_t *ptr = (uint32_t *)buf_head;	
	uint32_t w_len;	
	uint32_t ulAddr;	
	
	ret = modem_upgrade_recv_ack(TYPE_DOWNLOAD_TO_FLASH, &ack);
	if ((ret == BURN_OK) && (ack == 0)) 
	{
		/*收到写flash ack*/
		if((g_st5gModemUpgradeCtrl.ulImgWriteLen+UPGRADE_SEND_5G_MODEM_DATA_LEN)>=g_st5gModemUpgradeCtrl.ulImgLen)
		{
			g_st5gModemUpgradeCtrl.ulFileOff += g_st5gModemUpgradeCtrl.ulImgLen;
			/*当前分区写入完成，状态跳转到erase flash*/
			g_st5gModemUpgradeCtrl.ucState = UPGRADE_5G_MODEM_ERASE_FLASH;	
			rtt_printf(RTT_UPGRADE, RTT_TEXT_YELLOW"Upgrade5gModemWriteFlash: jump erase flash state\r\n");
		}
		else
		{
			g_st5gModemUpgradeCtrl.ucImgWriteNum = 0;
			g_st5gModemUpgradeCtrl.ulImgWriteLen += UPGRADE_SEND_5G_MODEM_DATA_LEN;	
			if(g_st5gModemUpgradeCtrl.ulImgWriteLen%0x10000 == 0)
			{
				rtt_printf(RTT_UPGRADE, RTT_TEXT_YELLOW"Upgrade5gModemWriteFlash: write flash 64k\r\n");
			}
		}
	}
	else
	{
		g_st5gModemUpgradeCtrl.ucImgWriteNum++;
		if (g_st5gModemUpgradeCtrl.ucImgWriteNum>3)
		{
			/*3次写入失败，状态跳转到connect mdl*/
			g_st5gModemUpgradeCtrl.ucImgCount = 0;							
			g_st5gModemUpgradeCtrl.ulFileOff = 4;			
			g_st5gModemUpgradeCtrl.ucState = UPGRADE_5G_MODEM_CONNECT_MDL;	
			rtt_printf(RTT_UPGRADE, RTT_TEXT_RED"Upgrade5gModemWriteFlash: jump connect mdl state\r\n");			
		}		
	}	
	
	/*发送write flash req*/
	if((g_st5gModemUpgradeCtrl.ulImgWriteLen+UPGRADE_SEND_5G_MODEM_DATA_LEN)>g_st5gModemUpgradeCtrl.ulImgLen)
		w_len = g_st5gModemUpgradeCtrl.ulImgLen - g_st5gModemUpgradeCtrl.ulImgWriteLen;
	else
		w_len = UPGRADE_SEND_5G_MODEM_DATA_LEN;
	
	ulAddr = g_st5gModemUpgradeCtrl.ulFileAddr+g_st5gModemUpgradeCtrl.ulFileOff+g_st5gModemUpgradeCtrl.ulImgWriteLen;
	if(STAT_OK != spi_flash.flash_read(g_uca5gModemData, ulAddr, w_len))
	{
		rtt_printf(RTT_UPGRADE, RTT_TEXT_RED"Upgrade5gModemWriteFlash: flash_read 0x%x fail!\r\n", ulAddr);
		Upgrade5gModemJumpInitState();
	}
	else
	{		
		*ptr = g_st5gModemUpgradeCtrl.ulDstFlashAddr+g_st5gModemUpgradeCtrl.ulImgWriteLen;
		*(ptr+1) = w_len;
		buf_head[8] = g_st5gModemUpgradeCtrl.ucFlashSecFlag;	
	
		modem_upgrade_send_req(TYPE_DOWNLOAD_TO_FLASH, buf_head, 9, g_uca5gModemData, w_len);
	}	
}

/*****************************************************************************
 * @Name	  : Upgrade5gModemRebootMdl
 * @Brief  : 5G MODEM升级状态处理 RebootMdl
 * @Param  : 无
 * @Retval : 无
 * @Author :
 * @Data   : 2024-3-12
 * @Note   :
 *****************************************************************************/
static void Upgrade5gModemRebootMdl(void)
{
	modem_upgrade_send_req(TYPE_REBOOT, NULL, 0, NULL, 0);
	
	ver_Set5gModemUpgrade(0);
	
	modem_upgrade_end();
	
	rtt_printf(RTT_UPGRADE, RTT_TEXT_YELLOW"Upgrade5gModemRebootMdl: success\r\n");
	
	/*状态跳转*/
	g_st5gModemUpgradeCtrl.ucState   = UPGRADE_5G_MODEM_INIT;	
	g_st5gModemUpgradeCtrl.ulCheckVerCnt = 0;		
}
#endif

/*****************************************************************************
* @Name	  :  upgrade_run
* @Brief  :  upgrade_run
* @Param  :  无
* @Retval :  无
* @Author :
* @Data   :  2024-02-28
* @Note   :
*****************************************************************************/ 
void upgrade_run(void)
{
	g_UpgradeProcTable[g_u8UpgradeState]();
}

/*****************************************************************************
* @Name	  :  Upgrade_Init
* @Brief  :  升级任务初始化
* @Param  :  无
* @Retval :  无
* @Author :
* @Data   :  2024-02-28
* @Note   :
*****************************************************************************/ 
static void Upgrade_Init(void)
{
#if (LOCAL_MODULE_TYPE == _MU)	
	uint32_t index;
	
	for(index=0; index<BLE_CONNECT_MAX_NUM; index++)
	{
		g_staRuUpgradeCtrlTable[index].ucModuleType = 0xFF;
		g_staRuUpgradeCtrlTable[index].ucState = UPGRADE_RU_INIT;
		g_staRuUpgradeCtrlTable[index].ulCheckVerCnt = 1800;/*上电2分钟后触发升级*/
	}
	
	g_st5gModemUpgradeCtrl.ucState = UPGRADE_5G_MODEM_INIT;
	g_st5gModemUpgradeCtrl.ucUpgradeNum = 0;
#endif	
		
	/*Ver_Init();*/
	
	g_u8UpgradeState = UPGRADE_WORK;
}

/*****************************************************************************
* @Name	  :  Upgrade_Work
* @Brief  :  升级任务运行
* @Param  :  无
* @Retval :  无
* @Author :
* @Data   :  2024-02-28
* @Note   :
*****************************************************************************/ 
static void Upgrade_Work(void)
{	
#if (LOCAL_MODULE_TYPE == _MU)	
	uint32_t index;
	
	static uint32_t cnt = 0;
	if(++cnt > 300)
	{
		for(index=0; index<BLE_CONNECT_MAX_NUM; index++)
		{
			rtt_printf(RTT_UPGRADE, RTT_TEXT_GREEN"Upgrade_Work: %d  ble_conn_id %d  ru_conn_state %d  dev_type %d\r\n", 
						index, 	
						ru_device_list[index].ble_conn_id, 
						ru_device_list[index].ru_conn_state,
						ru_device_list[index].dev_type);
		}	
		cnt = 0;
	}
	
	for(index=0; index<BLE_CONNECT_MAX_NUM; index++)
	{
		if(g_staRuUpgradeCtrlTable[index].ucState < UPGRADE_RU_STATE_NUM)
			g_UpgradeRuFuncTable[g_staRuUpgradeCtrlTable[index].ucState](index);
		
		g_staRuUpgradeCtrlTable[index].ulSendTimeOutCnt++;
		g_staRuUpgradeCtrlTable[index].ulConnFailCnt++;
		g_staRuUpgradeCtrlTable[index].ulCheckVerCnt++;
	}
	
	if(g_st5gModemUpgradeCtrl.ucState < UPGRADE_5G_MODEM_STATE_NUM)
		g_Upgrade5gModemFuncTable[g_st5gModemUpgradeCtrl.ucState]();
	
	g_st5gModemUpgradeCtrl.ulCheckVerCnt++;	
#endif		

	Ver_Work();

	g_u8UpgradeState = UPGRADE_WORK;
}
