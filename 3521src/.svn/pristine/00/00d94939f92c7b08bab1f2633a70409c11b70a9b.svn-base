/*************************************** Copyright ******************************
*
* (C) Copyright 2023, Shaanxi Tianji Communication
* All Rights Reserved
*                         
* FileName   : bsp_rf_testmode_oid.h   
* Version    : none		
* Author     : 			
* Date       : 2023-12-15         
* Description: none   
*******************************************************************************/
//#include <stdint.h>
#ifndef __BSP_TESTMODE_OID_H_
#define __BSP_TESTMODE_OID_H_

typedef enum
{
	pub_dev		= 0x00,
	monitor		= 0x01,
	m_4g		= 0x10,
	m_5g		= 0x11,
	module_ble	= 0x12,
	module_fpga = 0x13,
	module_hub	= 0x14,
	band_3		= 0x20,
	band_8		= 0x21,
	band_39		= 0x22,
	band_40		= 0x23,
	band_41		= 0x24		
}oid_module_e;


typedef enum
{
	uint1  = 0x00,
	sint1  = 0x01,
	uint2  = 0x02,
	sint2  = 0x03,
	uint4  = 0x04,
	sint4  = 0x05,
	str20  = 0x06,
	str32  = 0x07,
	str64  = 0x08,
	str192 = 0x09,
	str256 = 0x0A,
	str832 = 0x0B
}oid_data_type_e;


#define TM_ID_PUB_SN	 					0x0001	//SN
#define TM_ID_PUB_DEV_MODEL					0x0002	//设备型号
#define TM_ID_PUB_HW_VER					0x0003	//硬件版本
#define TM_ID_PUB_SW_VER					0x0004	//软件版本
#define TM_ID_PUB_MCU_TYPE  				0x0005	//MCU型号
#define TM_ID_PUB_FLASH	    				0x0006	//FLASH型号
#define TM_ID_PUB_LICENSE   				0x0007	//license
#define TM_ID_PUB_SYS_NUM					0x0008	//查询通道数量
#define TM_ID_PUB_PARA_LIST 				0x0009	//更新参数列表
#define TM_ID_PUB_SYS_NAME  				0x000A	//查询通道名称
#define TM_ID_PUB_FLASH_INIT 				0x000B	//FLASH初始化
#define TM_ID_PUB_DEV_MODE					0x000C	//设备模式
#define TM_ID_PUB_RUN_MODE  				0x000D	//运行模式
#define TM_ID_PUB_SLAVE_DEV_NUM	 			0x000E	//从机数量
#define TM_ID_PUB_SLAVE_DEV_ONLIN_NUM 		0x000F	//在线从机数量
//#define TM_ID_PUB_PORT_NUM				0x0010	//有效端口数量
#define TM_ID_PUB_RST_BY_NET 				0x0010	//网络定时重启时间
#define TM_ID_PUB_RST_BY_TIME 				0x0011	//定时重启时间间隔
#define TM_ID_PUB_LED_PLAN 					0x0012	//指示灯方案
#define TM_ID_PUB_LED_SWITCH				0x0013	//指示灯开关
#define TM_ID_PUB_SYS_RST	 				0x0014	//系统重启
#define TM_ID_PUB_ROUTING_ADDR	 			0x0015	//设备路由登记地址
#define TM_ID_PUB_MODE_SWITCH_EN			0x0016	//模式切换使能

#define TM_ID_PUB_4G_VALID_BAND				0x00C8	//4G通道有效BAND
#define TM_ID_PUB_DEV_TYPE					0x00C9  //设备类型
#define TM_ID_PUB_BLOCK_SWITCH			    0x00CA	//抗阻塞检测开关

#define TM_ID_MONITOR_FACT_ID	 	 		0x0001	//厂商代码
#define TM_ID_MONITOR_DEV_TYPE	 	 		0x0002	//设备类别
#define TM_ID_MONITOR_STATION_ID 	 		0x0003	//站点编号
#define TM_ID_MONITOR_DEVICE_ID  	 		0x0004	//设备编号
#define TM_ID_MONITOR_IPV4_IPV6_1	 		0x0005	//IPV4/IPV6选择1
#define TM_ID_MONITOR_TCP_UDP_1 	 		0x0006	//TCP/UPD选择1
#define TM_ID_MONITOR_USE_PASSWORD_1 		0x0007	//登录权限使能1
#define TM_ID_MONITOR_IPV4_ADDR_1	 		0x0008	//IPV4地址1
#define TM_ID_MONITOR_IPV6_ADDR_1  	 		0x0009	//IPV6地址1
#define TM_ID_MONITOR_IP_PORT_1	 	 		0x000A	//IP端口号1
#define TM_ID_MONITOR_APN_1 		 		0x000B	//APN1
#define TM_ID_MONITOR_USERNAME_1 	 		0x000C	//用户名1
#define TM_ID_MONITOR_PASSWORD_1 	 		0x000D	//密码1
#define TM_ID_MONITOR_IPV4_IPV6_2	 		0x000E	//IPV4/IPV6选择2
#define TM_ID_MONITOR_TCP_UDP_2 	 		0x000F	//TCP/UPD选择2
#define TM_ID_MONITOR_USE_PASSWORD_2 		0x0010	//登录权限使能2
#define TM_ID_MONITOR_IPV4_ADDR_2	 		0x0011	//IPV4地址2
#define TM_ID_MONITOR_IPV6_ADDR_2  	 		0x0012	//IPV6地址2
#define TM_ID_MONITOR_IP_PORT_2	 	 		0x0013	//IP端口号2
#define TM_ID_MONITOR_APN_2 		 		0x0014	//APN2
#define TM_ID_MONITOR_USERNAME_2 	 		0x0015	//用户名2
#define TM_ID_MONITOR_PASSWORD_2 	 		0x0016	//密码2
#define TM_ID_MONITOR_HEART		 	 		0x0017	//心跳包间隔
#define TM_ID_MONITOR_DEV_PORT		 		0x0018	//设备端口号
#define TM_ID_MONITOR_LONGITUDE		 		0x0019	//经度
#define TM_ID_MONITOR_LATITUDE		 		0x001A	//纬度
#define TM_ID_MONITOR_POSITION		 		0x001B	//位置信息

						  
#define TM_ID_BLE_EN 						0x0001	//蓝牙组网功能使能/关闭
#define TM_ID_BLE_WORK_MODE 				0x0002	//蓝牙工作模式设置
#define TM_ID_BLE_RSSI 						0x0003	//蓝牙RSSI读取(仅从机)
#define TM_ID_BLE_RST 						0x0004	//蓝牙复位
#define TM_ID_BLE_MAC 						0x0005	//蓝牙MAC地址查询
#define TM_ID_BLE_FREQ_POINT 				0x0006	//蓝牙频点设置
#define TM_ID_BLE_OUT_POW 					0x0007	//蓝牙发射功率设置
#define TM_ID_BLE_VER 						0x0008	//蓝牙版本号查询
#define TM_ID_BLE_SLAVE_NUM 				0x0009	//蓝牙当前从机数量查询
#define TM_ID_BLE_MASTER_NUM 				0x000A	//蓝牙当前主机数量查询
#define TM_ID_BLE_APP_EN 					0x000B	//使能手机APP服务，使能后可接入APP
#define TM_ID_BLE_IN_TRANS 					0x000C	//模组进入透传模式
#define TM_ID_BLE_TX 						0x000D	//发透传数据
#define TM_ID_BLE_RX 						0x000E	//收透传数据


#define TM_ID_FPGA_STATE 					0x0001	//FPGA状态查询
#define TM_ID_FPGA_VER 						0x0002	//FPGA版本信息
#define TM_ID_FPGA_OOK_STATE 				0x0003	//OOK状态查询
#define TM_ID_FPGA_OOK_FRAME_HEAD 			0x0004	//OOK传输帧头类型
#define TM_ID_FPGA_PWM_DUTY_CYCLE 			0x0005	//PWM占空比
#define TM_ID_FPGA_OOK_FREQ_POINT_SWITCH 	0x0006	//OOK频点切换
#define TM_ID_FPGA_OOK_RSSI 				0x0007	//OOK RSSI查询
#define TM_ID_FPGA_OOK_SWEEP_FREQ 			0x0008	//OOK 扫频查询
#define TM_ID_FPGA_4G_OUT_MODE 				0x0009	//4G输出模式
#define TM_ID_FPGA_5G_OUT_MODE 				0x000A	//5G输出模式
#define TM_ID_FPGA_CLK_STATE 				0x000B	//时钟状态查询
#define TM_ID_FPGA_4G_BAND 					0x000C	//4G频段
#define TM_ID_FPGA_5G_FRAME 				0x000D	//5G帧格式
#define TM_ID_FPGA_4G_FRAME 				0x000E	//4G子帧配比
#define TM_ID_FPGA_4G_SPECIAL_FRAME 		0x000F	//4G特殊子帧配比
#define TM_ID_FPGA_4G_CENTER_FREQ 			0x0010	//4G中心频点
#define TM_ID_FPGA_4G_BW 					0x0011	//4G带宽
#define TM_ID_FPGA_5G_CENTER_FREQ 			0x0012	//5G中心频点
#define TM_ID_FPGA_5G_BW 					0x0013	//5G带宽
#define TM_ID_FPGA_UP_SILENCE 				0x0014	//上行静默
#define TM_ID_FPGA_4G_TRX_RSSI 				0x0015	//4G TRX RSSI
#define TM_ID_FPGA_5G_TRX_RSSI1 			0x0016	//5G1 TRX RSSI
#define TM_ID_FPGA_5G_TRX_RSSI2 			0x0017	//5G2 TRX RSSI
#define TM_ID_FPGA_4G_UL_MIPI_CTR 			0x0018	//4G UL MIPI控制
#define TM_ID_FPGA_4G_DL_MIPI_CTR 			0x0019	//4G DL MIPI控制


#define TM_ID_MODEM_MODEL	 				0x0001	//模组型号
#define TM_ID_MODEM_HW_VER	 	 			0x0002	//硬件版本
#define TM_ID_MODEM_SW_VER	 				0x0003	//软件版本
#define TM_ID_MODEM_AP_VER	 	 			0x0004	//AP版本
#define TM_ID_MODEM_SIM_STATE	 			0x0005	//SIM卡状态
#define TM_ID_MODEM_SIM_ID	 	 			0x0006	//SIM卡卡号
#define TM_ID_MODEM_LINK_STATE	 			0x0007	//连接服务器状态
#define TM_ID_MODEM_INIT 		 			0x0008	//初始化
#define TM_ID_MODEM_POW_SWITCH 	 			0x000C	//电源开关
#define TM_ID_MODEM_RST 		 			0x000D	//复位
#define TM_ID_MODEM_RE_SCAN					0x000E	//重新扫描
#define TM_ID_MODEM_5G_SYNC					0x0010	//5G 同步状态
#define TM_ID_MODEM_5G_FRAME				0x0011	//5G 上下行配比
#define TM_ID_MODEM_5G_SPECIAL_FRAME		0x0012	//5G 特殊子帧配置
#define TM_ID_MODEM_5G_FRAME_TIME_OFFSET	0x0013	//5G 帧头时间调整量
#define TM_ID_MODEM_5G_FIRST_TIME_OFFSET	0x0014	//5G 第一转换点的提前/滞后量
#define TM_ID_MODEM_5G_SECOND_TIME_OFFSET	0x0015	//5G 第二转换点的提前/滞后量
#define TM_ID_MODEM_5G_SSB					0x0016	//5G SSB 频点
#define TM_ID_MODEM_5G_RSRP 		 		0x0017	//5G RSRP
#define TM_ID_MODEM_5G_SINR					0x0018  //5G SINR
#define TM_ID_MODEM_5G_RSSI 		 		0x0019	//5G RSSI
#define TM_ID_MODEM_5G_PCI 		 			0x001A	//5G PCI
#define TM_ID_MODEM_5G_UR_EDGE_OFFSET		0x001B	//5G UP上升沿偏移量
#define TM_ID_MODEM_5G_UF_EDGE_OFFSET		0x001C	//5G UP下降沿偏移量
#define TM_ID_MODEM_5G_DR_EDGE_OFFSET		0x001D	//5G DW上升沿偏移量
#define TM_ID_MODEM_5G_DF_EDGE_OFFSET		0x001E	//5G DW下降沿偏移量
#define TM_ID_MODEM_4G_SYNC					0x0020	//4G 同步状态
#define TM_ID_MODEM_4G_FRAME				0x0021	//4G 上下行配比
#define TM_ID_MODEM_4G_SPECIAL_FRAME		0x0022	//4G 特殊子帧配置
#define TM_ID_MODEM_4G_FRAME_TIME_OFFSET	0x0023	//4G 帧头时间调整量
#define TM_ID_MODEM_4G_FIRST_TIME_OFFSET	0x0024	//4G 第一转换点的提前/滞后量
#define TM_ID_MODEM_4G_SECOND_TIME_OFFSET	0x0025	//4G 第二转换点的提前/滞后量
#define TM_ID_MODEM_4G_SSB					0x0026	//4G SSB 频点
#define TM_ID_MODEM_4G_RSRP 		 		0x0027	//4G RSRP
#define TM_ID_MODEM_4G_SINR					0x0028  //4G SINR
#define TM_ID_MODEM_4G_RSSI 		 		0x0029	//4G RSSI
#define TM_ID_MODEM_4G_PCI 		 			0x002A	//4G PCI
#define TM_ID_MODEM_4G_UR_EDGE_OFFSET		0x002B	//4G UP上升沿偏移量
#define TM_ID_MODEM_4G_UF_EDGE_OFFSET		0x002C	//4G UP下降沿偏移量
#define TM_ID_MODEM_4G_DR_EDGE_OFFSET		0x002D	//4G DW上升沿偏移量
#define TM_ID_MODEM_4G_DF_EDGE_OFFSET		0x002E	//4G DW下降沿偏移量


#define TM_ID_RF_NAME						0x0001	//频段名称
#define TM_ID_RF_TDD_OR_FDD					0x0002	//网络制式
#define TM_ID_RF_POWER_SWITCH	 			0x0003	//电源开关
#define TM_ID_RF_OPEN_CLOSE_SWITCH			0x0004	//射频开启 / 关闭
#define TM_ID_RF_UP_DOWN_SWITCH	 			0x0005	//上行单通 / 下行单通
#define TM_ID_RF_LTE_NR_SWITCH				0x0006	//LTE / NR 切换
#define TM_ID_RF_SYNC						0x0007  //同步状态
#define TM_ID_RF_BAND						0x0008	//频段
#define TM_ID_RF_BW 						0x0009	//带宽
#define TM_ID_RF_FRQ_POINT 					0x000A	//频点
#define TM_ID_RF_CENTER_FREQ				0x000B  //中心频率
#define TM_ID_RF_SCAN_FREQ_POINT			0x000C  //扫描频点
#define TM_ID_RF_EXIST						0x000D  //通道存在状态
#define TM_ID_RF_FPGA_SWITCH_SET			0x000F	//FPGA开关设置

#define TM_ID_RF_FRAME						0x0010	//5G上下行配比
#define TM_ID_RF_SPECIAL_FRAME				0x0011	//5G特殊子帧配置
#define TM_ID_RF_FRAME_TIME_OFFSET			0x0012	//5G帧头时间调整量
#define TM_ID_RF_FIRST_TIME_OFFSET			0x0013	//第一转换点的提前/滞后量
#define TM_ID_RF_SECOND_TIME_OFFSET			0x0014	//第二转换点的提前/滞后量
#define TM_ID_RF_SSB						0x0015	//SSB 频点
#define TM_ID_RF_LTE_FRAME					0x0016	//LTE上下行配比
#define TM_ID_RF_LTE_SPECIAL_FRAME			0x0017	//LTE特殊子帧配置
#define TM_ID_RF_UL_RISING_EDGE_OFFSET		0x0018	//UP上升沿偏移量
#define TM_ID_RF_UL_FALLING_EDGE_OFFSET		0x0019	//UP下降沿偏移量
#define TM_ID_RF_DL_RISING_EDGE_OFFSET		0x001A	//DW上升沿偏移量
#define TM_ID_RF_DL_FALLING_EDGE_OFFSET		0x001B	//DW下降沿偏移量

#define TM_ID_RF_UP_THEO_GAIN				0x0020	//上行理论增益
#define TM_ID_RF_DW_THEO_GAIN				0x0021	//下行理论增益
#define TM_ID_RF_UP_THEO_OUT_POW			0x0022	//上行理论输出功率
#define TM_ID_RF_DW_THEO_OUT_POW			0x0023	//下行理论输出功率
#define TM_ID_RF_UP_ACT_GAIN				0x0024	//上行实际增益
#define TM_ID_RF_DW_ACT_GAIN				0x0025	//下行实际增益
#define TM_ID_RF_UP_ACT_OUT_POW				0x0026	//上行实际输出功率
#define TM_ID_RF_DW_ACT_OUT_POW				0x0027	//下行实际输出功率
#define TM_ID_RF_UP_ACT_IN_POW 				0x0028	//上行实际输入功率
#define TM_ID_RF_DW_ACT_IN_POW 				0x0029	//下行实际输入功率
#define TM_ID_RF_UP_EXPECT_IN_POW			0x002A	//上行期望接收功率
#define TME_ID_RF_BW_SET_FOR_TEST			0x002C	//兼容测试模式下的带宽设定
#define TM_ID_RF_OPLC_EN					0x002E	//开环工控使能
#define TM_ID_RF_RSRP_EN 					0x002F	//RSRP计算使能

#define TM_ID_RF_ISO_EN						0x0030	//隔离度检测使能
#define TM_ID_RF_WARN_EN 					0x0031	//告警使能
#define TM_ID_RF_TC_EN						0x0032	//温度补偿使能
#define TM_ID_RF_UP_SILENCE_EN				0x0033	//上行静默使能
#define TM_ID_RF_TEMP_OFFSET				0x0034	//温度偏移量
#define TM_ID_RF_UP_GAIN_OFFSET				0x0035	//上行增益调整偏移量
#define TM_ID_RF_DW_GAIN_OFFSET				0x0036	//下行增益调整偏移量
#define TM_ID_RF_UP_OUT_POW_OFFSET			0x0037	//上行输出功率调整偏移量
#define TM_ID_RF_DW_OUT_POW_OFFSET			0x0038	//下行输出功率调整偏移量
#define TM_ID_RF_COUPLING_GAIN    			0x0039	//同步耦合增益差值
#define TM_ID_RF_UP_SILENCE_GATE			0x003A	//上行静默门限
#define TM_ID_RF_OPLC_POINT					0x003B	//开环工控启控点
#define TM_ID_RF_OPLC_GATE					0x003C	//上行开环工控门限
#define TM_ID_RF_OPLC_GAIN_LINK_VALUE		0x003D	//上行开环工控增益联动差值
#define TM_ID_RF_OPLC_POW_LIN_VALUE			0x003E	//上行开环工控功率联动差值
#define TM_ID_RF_UP_ADC1_VALUE 				0x003F	//上行ADC1读取

#define TM_ID_RF_UP_ADC2_VALUE 				0x0040	//上行ADC2读取
#define TM_ID_RF_DW_ADC1_VALUE 				0x0041	//下行ADC1读取
#define TM_ID_RF_DW_ADC2_VALUE 				0x0042	//下行ADC2读取
#define TM_ID_RF_UP_OUT_POW1 				0x0043	//上行输出功率1
#define TM_ID_RF_UP_OUT_POW2 				0x0044	//上行输出功率2
#define TM_ID_RF_DW_OUT_POW1 				0x0045	//下行输出功率1
#define TM_ID_RF_DW_OUT_POW2 				0x0046	//下行输出功率2
#define TM_ID_RF_UP_LNA_ATT_VALUE1 			0x0047	//上行LNA1 ATT值
#define TM_ID_RF_UP_LNA_ATT_VALUE2 			0x0048	//上行LNA2 ATT值
#define TM_ID_RF_DW_LNA_ATT_VALUE1 			0x0049	//下行LNA1 ATT值
#define TM_ID_RF_DW_LNA_ATT_VALUE2 			0x004A	//下行LNA2 ATT值
#define TM_ID_RF_UP_LNA_SWITCH1 			0x004B	//上行LNA1状态设置
#define TM_ID_RF_UP_LNA_SWITCH2 			0x004C	//上行LNA2状态设置
#define TM_ID_RF_DW_LNA_SWITCH1 			0x004D	//下行LNA1状态设置
#define TM_ID_RF_DW_LNA_SWITCH2 			0x004E	//下行LNA2状态设置
#define TM_ID_RF_UP_PA_SWITCH1 				0x004F	//上行功放开关设置1

#define TM_ID_RF_UP_PA_SWITCH2 				0x0050	//上行功放开关设置2
#define TM_ID_RF_DW_PA_SWITCH1 				0x0051	//下行功放开关设置1
#define TM_ID_RF_DW_PA_SWITCH2 				0x0052	//下行功放开关设置2
#define TM_ID_RF_UP_DIGIT_ATT_VALUE1 		0x0053	//上行数字衰减器1设置
#define TM_ID_RF_UP_DIGIT_ATT_VALUE2 		0x0054	//上行数字衰减器2设置
#define TM_ID_RF_DW_DIGIT_ATT_VALUE1 		0x0055	//下行数字衰减器1设置
#define TM_ID_RF_DW_DIGIT_ATT_VALUE2 		0x0056	//下行数字衰减器2设置
#define TM_ID_RF_UP_ANALOG_ATT_VALUE1 		0x0057	//上行模拟衰减1设置
#define TM_ID_RF_UP_ANALOG_ATT_VALUE2 		0x0058	//上行模拟衰减2设置
#define TM_ID_RF_DW_ANALOG_ATT_VALUE1 		0x0059	//下行模拟衰减1设置
#define TM_ID_RF_DW_ANALOG_ATT_VALUE2 		0x005A	//下行模拟衰减2设置
#define TM_ID_RF_UP_ANALOG_ATT_PWM_VALUE1 	0x005B	//上行模拟衰减1PWM设置
#define TM_ID_RF_UP_ANALOG_ATT_PWM_VALUE2 	0x005C	//上行模拟衰减2PWM设置
#define TM_ID_RF_DW_ANALOG_ATT_PWM_VALUE1 	0x005D	//下行模拟衰减1PWM设置
#define TM_ID_RF_DW_ANALOG_ATT_PWM_VALUE2 	0x005E	//下行模拟衰减2PWM设置
#define TM_ID_RF_UP_8668_ADC_VALUE 			0x005F	//上行8668后ADC采样值读取

#define TM_ID_RF_DW_8668_ADC_VALUE 			0x0060	//下行8668后ADC采样值读取
#define TM_ID_RF_UP_8668_OUT_POWER 			0x0061	//上行8668后功率值读取
#define TM_ID_RF_DW_8668_OUT_POWER 			0x0062	//下行8668后功率值读取
#define TM_ID_RF_WARN_EN_UP_INPOW_UNDER  	0x0063	//上行入过功率告警使能
#define TM_ID_RF_WARN_EN_DW_INPOW_UNDER  	0x0064	//下行入过功率告警使能
#define TM_ID_RF_WARN_EN_UP_INPOW_OVER   	0x0065	//上行入欠功率告警使能
#define TM_ID_RF_WARN_EN_DW_INPOW_OVER   	0x0066	//下行入欠功率告警使能
#define TM_ID_RF_WARN_EN_UP_OUTPOW_UNDER 	0x0067	//上行出过功率告警使能
#define TM_ID_RF_WARN_EN_DW_OUTPOW_UNDER 	0x0068	//下行出过功率告警使能
#define TM_ID_RF_WARN_EN_UP_OUTPOW_OVER  	0x0069	//上行出欠功率告警使能
#define TM_ID_RF_WARN_EN_DW_OUTPOW_OVER  	0x006A	//下行出欠功率告警使能
#define TM_ID_RF_WARN_EN_UP_ISO  		 	0x006B	//上行自激告警使能
#define TM_ID_RF_WARN_EN_DW_ISO  		 	0x006C	//下行自激告警使能
#define TM_ID_RF_WARN_EN_TMP_OVER 		 	0x006D	//过温度告警使能
#define TM_ID_RF_WARN_EN_OTHER			 	0x006E	//其他告警使能
#define TM_ID_RF_WARN_GATE_UP_INPOW_UNDER  	0x006F	//上行入过功率告警门限

#define TM_ID_RF_WARN_GATE_DW_INPOW_UNDER  	0x0070 	//下行入过功率告警门限
#define TM_ID_RF_WARN_GATE_UP_INPOW_OVER   	0x0071  //上行入欠功率告警门限
#define TM_ID_RF_WARN_GATE_DW_INPOW_OVER   	0x0072 	//下行入欠功率告警门限
#define TM_ID_RF_WARN_GATE_UP_OUTPOW_UNDER 	0x0073 	//上行出过功率告警门限
#define TM_ID_RF_WARN_GATE_DW_OUTPOW_UNDER 	0x0074  //下行出过功率告警门限
#define TM_ID_RF_WARN_GATE_UP_OUTPOW_OVER  	0x0075  //上行出欠功率告警门限
#define TM_ID_RF_WARN_GATE_DW_OUTPOW_OVER  	0x0076  //下行出欠功率告警门限
#define TM_ID_RF_WARN_GATE_UP_ISO  		   	0x0077  //上行自激告警门限
#define TM_ID_RF_WARN_GATE_DW_ISO  		   	0x0078  //下行自激告警门限
#define TM_ID_RF_WARN_GATE_TMP_OVER 	   	0x0079  //过温度告警门限
#define TM_ID_RF_WARN_GATE_OTHER		   	0x007A  //其他告警门限
#define TM_ID_RF_WARN_UP_INPOW_UNDER  		0x007B 	//上行入过功率告警
#define TM_ID_RF_WARN_DW_INPOW_UNDER  		0x007C  //下行入过功率告警
#define TM_ID_RF_WARN_UP_INPOW_OVER   		0x007D 	//上行入欠功率告警
#define TM_ID_RF_WARN_DW_INPOW_OVER   		0x007E  //下行入欠功率告警
#define TM_ID_RF_WARN_UP_OUTPOW_UNDER 		0x007F  //上行出过功率告警

#define TM_ID_RF_WARN_DW_OUTPOW_UNDER 		0x0080	//下行出过功率告警
#define TM_ID_RF_WARN_UP_OUTPOW_OVER  		0x0081	//上行出欠功率告警
#define TM_ID_RF_WARN_DW_OUTPOW_OVER  		0x0082	//下行出欠功率告警
#define TM_ID_RF_WARN_UP_ISO  		  		0x0083	//上行自激告警
#define TM_ID_RF_WARN_DW_ISO  		  		0x0084	//下行自激告警
#define TM_ID_RF_WARN_TMP_OVER 	   	  		0x0085	//过温度告警
#define TM_ID_RF_WARN_OTHER		   	  		0x0086	//其他告警

#define TM_ID_RF_UP_OUT_POW_LIST1			0x00E0	//上行输出功率表1
#define TM_ID_RF_UP_OUT_POW_LIST2			0x00E1	//上行输出功率表2
#define TM_ID_RF_UP_OUT_POW_LIST_8668		0x00E2	//上行8668输出功率表
#define TM_ID_RF_DW_OUT_POW_LIST1			0x00E3	//下行输出功率表1
#define TM_ID_RF_DW_OUT_POW_LIST2			0x00E4	//下行输出功率表2
#define TM_ID_RF_DW_OUT_POW_LIST_8668		0x00E5	//下行8668输出功率表
#define TM_ID_RF_UP_ANALOG_ATT_LIST1		0x00E6	//上行模拟衰减表1
#define TM_ID_RF_UP_ANALOG_ATT_LIST2		0x00E7	//上行模拟衰减表2
#define TM_ID_RF_DW_ANALOG_ATT_LIST1		0x00E8	//下行模拟衰减表1
#define TM_ID_RF_DW_ANALOG_ATT_LIST2		0x00E9	//下行模拟衰减表2
#define TM_ID_RF_UP_TEMP_LIST				0x00EA	//上行温补表
#define TM_ID_RF_DW_TEMP_LIST				0x00EB	//下行温补表
#define TM_ID_RF_COUPLING_GAIN_START   		0x00EC	//开始同步耦合增益测量

#define TM_ID_8668_SW_RST					0x0101	//866x软件复位
#define TM_ID_8668_INIT						0x0102	//866x初始化
#define TM_ID_8668_REG_ADDRESS				0x0103	//待操作寄存器地址
#define TM_ID_8668_ON_PAGE_REG				0x0104	//866x页内寄存器读写
#define TM_ID_8668_OFF_PAGE_REG				0x0105	//866x页外寄存器读写
#define TM_ID_8668_TX_FREQ					0x0106	//866xTX频率设置
#define TM_ID_8668_RX_FREQ					0x0107	//866xRX频率设置
#define TM_ID_8668_TX_GAIN_OFFSET			0x0108	//866xTX增益偏移量设置
#define TM_ID_8668_RX_GAIN_OFFSET			0x0109	//866xRX增益偏移量设置
#define TM_ID_8668_CH_MODE_SET				0x010A	//866x链路设置
#define TM_ID_8668_START_GAIN_VALUE			0x010B	//866x初始增益值设置
#define TM_ID_8668_BW						0x010C	//866x带宽设置
#define TM_ID_8668_LOOPBACK					0x010D	//866x回环状态设置
#define TM_ID_8668_SINGLE_PASS_SET			0x010E	//866x单通状态设置
#define TM_ID_8668_RSSI						0x010F	//866x读取RSSI
#define TM_ID_8668_MIRROR_CALI 				0x0110	//镜像校准
#define TM_ID_8668_TX_LOCAL_OSC_I_1			0x0111	//TX本振I-1
#define TM_ID_8668_TX_LOCAL_OSC_Q_1			0x0112	//TX本振Q-1
#define TM_ID_8668_TX_LOCAL_OSC_I_2 		0x0113	//TX本振I-2
#define TM_ID_8668_TX_LOCAL_OSC_Q_2			0x0114	//TX本振Q-2
#define TM_ID_8668_RX_LOCAL_OSC_I_1	 		0x0115	//RX本振I-1
#define TM_ID_8668_RX_LOCAL_OSC_Q_1	 		0x0116	//RX本振Q-1
#define TM_ID_8668_RX_LOCAL_OSC_I_2	 		0x0117	//RX本振I-2
#define TM_ID_8668_RX_LOCAL_OSC_Q_2	 		0x0118	//RX本振Q-2
#define TM_ID_8668_RX_LOCAL_OSC_I_3	 		0x0119	//RX本振I-3
#define TM_ID_8668_RX_LOCAL_OSC_Q_3	 		0x011A	//RX本振Q-3
#define TM_ID_8668_RX_LOCAL_OSC_I_4	 		0x011B	//RX本振I-4
#define TM_ID_8668_RX_LOCAL_OSC_Q_4	 		0x011C	//RX本振Q-4
#define TM_ID_8668_RX_LOCAL_OSC_I_5	 		0x011D	//RX本振I-5
#define TM_ID_8668_RX_LOCAL_OSC_Q_5	 		0x011E	//RX本振Q-5
#define TM_ID_8668_NORMAL_GAIN_LIST			0x01C8	//正常模式增益表设置
#define TM_ID_8668_BLOCK_GAIN_LIST			0x01C9	//抗阻塞模式增益表设置						  
#define TM_ID_8668_GAIN_INIT_VALUE			0x01CA	//初始增益	
#define TM_ID_8668_READ_WRITE_REG			0x01CB	//校准时读写寄存器
#define TM_ID_8668_NORMAL_RATED_INDEX   	0x01CC	//额定增益在正常码表的索引值				  
#define TM_ID_8668_BLOCK_RATED_INDEX    	0x01CD	//额定增益在抗阻塞码表的索引值

#define TM_ID_8668_TEST_TEMP_01				0x0180	//8668临时测试校准使用
#define TM_ID_8668_TEST_TEMP_02				0x0181	//8668临时测试校准使用
#define TM_ID_8668_TEST_TEMP_03				0x0182	//8668临时测试校准使用
#define TM_ID_8668_TEST_TEMP_04				0x0183	//8668临时测试校准使用
#define TM_ID_8668_TEST_TEMP_05				0x0184	//8668临时测试校准使用
#define TM_ID_8668_TEST_TEMP_06				0x0185	//8668临时测试校准使用
#define TM_ID_8668_TEST_TEMP_07				0x0186	//8668临时测试校准使用
#define TM_ID_8668_TEST_TEMP_08				0x0187	//8668临时测试校准使用
#define TM_ID_8668_TEST_TEMP_09				0x0188	//8668临时测试校准使用
#define TM_ID_8668_TEST_TEMP_10				0x0189	//8668临时测试校准使用
#define TM_ID_8668_TEST_TEMP_11				0x018A	//8668临时测试校准使用
#define TM_ID_8668_TEST_TEMP_12				0x018B	//8668临时测试校准使用
#define TM_ID_8668_TEST_TEMP_13				0x018C	//8668临时测试校准使用
#define TM_ID_8668_TEST_TEMP_14				0x018D	//8668临时测试校准使用
#define TM_ID_8668_TEST_TEMP_15				0x018E	//8668临时测试校准使用
#define TM_ID_8668_TEST_TEMP_16				0x018F	//8668临时测试校准使用
#define TM_ID_8668_TEST_TEMP_17				0x0190	//8668临时测试校准使用
#define TM_ID_8668_TEST_TEMP_18				0x0191	//8668临时测试校准使用
#define TM_ID_8668_TEST_TEMP_19				0x0192	//8668临时测试校准使用
#define TM_ID_8668_TEST_TEMP_20				0x0193	//8668临时测试校准使用
#define TM_ID_8668_TEST_TEMP_21				0x0194	//8668临时测试校准使用
#define TM_ID_8668_TEST_TEMP_22				0x0195	//8668临时测试校准使用
#define TM_ID_8668_TEST_TEMP_23				0x0196	//8668临时测试校准使用
#define TM_ID_8668_TEST_TEMP_24				0x0197	//8668临时测试校准使用
#define TM_ID_8668_TEST_TEMP_25				0x0198	//8668临时测试校准使用
#define TM_ID_8668_TEST_TEMP_26				0x0199	//8668临时测试校准使用
#define TM_ID_8668_TEST_TEMP_27				0x019A	//8668临时测试校准使用
#define TM_ID_8668_TEST_TEMP_28				0x019B	//8668临时测试校准使用
#define TM_ID_8668_TEST_TEMP_29				0x019C	//8668临时测试校准使用
#define TM_ID_8668_TEST_TEMP_30				0x019D	//8668临时测试校准使用
#define TM_ID_8668_TEST_TEMP_31				0x019E	//8668临时测试校准使用


#define TM_ID_HUB_PORT1_SWITCH 				0x0001	//端口1电源开关
#define TM_ID_HUB_PORT2_SWITCH 				0x0002	//端口2电源开关
#define TM_ID_HUB_PORT3_SWITCH 				0x0003	//端口3电源开关
#define TM_ID_HUB_PORT4_SWITCH 				0x0004	//端口4电源开关
#define TM_ID_HUB_PORT5_SWITCH 				0x0005	//端口5电源开关
#define TM_ID_HUB_PORT6_SWITCH 				0x0006	//端口6电源开关
#define TM_ID_HUB_PORT7_SWITCH 				0x0007	//端口7电源开关
#define TM_ID_HUB_PORT8_SWITCH 				0x0008	//端口8电源开关
#define TM_ID_HUB_PORT9_SWITCH 				0x0009	//端口9电源开关
#define TM_ID_HUB_PORT10_SWITCH 			0x000A	//端口10电源开关
#define TM_ID_HUB_PORT11_SWITCH 			0x000B	//端口11电源开关
#define TM_ID_HUB_PORT12_SWITCH 			0x000C	//端口12电源开关
#define TM_ID_HUB_PORT13_SWITCH 			0x000D	//端口13电源开关
#define TM_ID_HUB_PORT14_SWITCH 			0x000E	//端口14电源开关
#define TM_ID_HUB_PORT15_SWITCH 			0x000F	//端口15电源开关
#define TM_ID_HUB_PORT16_SWITCH 			0x0010	//端口16电源开关
#define TM_ID_HUB_PORT1_CURRENT 			0x0011	//端口1电流
#define TM_ID_HUB_PORT2_CURRENT 			0x0012	//端口2电流
#define TM_ID_HUB_PORT3_CURRENT 			0x0013	//端口3电流
#define TM_ID_HUB_PORT4_CURRENT 			0x0014	//端口4电流
#define TM_ID_HUB_PORT5_CURRENT 			0x0015	//端口5电流
#define TM_ID_HUB_PORT6_CURRENT 			0x0016	//端口6电流
#define TM_ID_HUB_PORT7_CURRENT 			0x0017	//端口7电流
#define TM_ID_HUB_PORT8_CURRENT 			0x0018	//端口8电流
#define TM_ID_HUB_PORT9_CUTTENT 			0x0019	//端口9电流
#define TM_ID_HUB_PORT10_CUTTENT			0x001A	//端口10电流
#define TM_ID_HUB_PORT11_CUTTENT			0x001B	//端口11电流
#define TM_ID_HUB_PORT12_CUTTENT 			0x001C	//端口12电流
#define TM_ID_HUB_PORT13_CUTTENT 			0x001D	//端口13电流
#define TM_ID_HUB_PORT14_CUTTENT 			0x001E	//端口14电流
#define TM_ID_HUB_PORT15_CUTTENT 			0x001F	//端口15电流
#define TM_ID_HUB_PORT16_CUTTENT 			0x0020	//端口16电流
#define TM_ID_HUB_PORT1_EN 					0x0021	//端口1使能
#define TM_ID_HUB_PORT2_EN 					0x0022	//端口2使能
#define TM_ID_HUB_PORT3_EN 					0x0023	//端口3使能
#define TM_ID_HUB_PORT4_EN 					0x0024	//端口4使能
#define TM_ID_HUB_PORT5_EN 					0x0025	//端口5使能
#define TM_ID_HUB_PORT6_EN 					0x0026	//端口6使能
#define TM_ID_HUB_PORT7_EN 					0x0027	//端口7使能
#define TM_ID_HUB_PORT8_EN 					0x0028	//端口8使能
#define TM_ID_HUB_PORT9_EN 					0x0029	//端口9使能
#define TM_ID_HUB_PORT10_EN 				0x002A	//端口10使能
#define TM_ID_HUB_PORT11_EN 				0x002B	//端口11使能
#define TM_ID_HUB_PORT12_EN 				0x002C	//端口12使能
#define TM_ID_HUB_PORT13_EN 				0x002D	//端口13使能
#define TM_ID_HUB_PORT14_EN 				0x002E	//端口14使能
#define TM_ID_HUB_PORT15_EN 				0x002F	//端口15使能
#define TM_ID_HUB_PORT16_EN 				0x0030	//端口16使能




#endif





