#ifndef _BSP_MONITOR_PARA_LIST_H_
#define _BSP_MONITOR_PARA_LIST_H_

#include "main.h"

//------------------------------ 保留监控参数定义------------------------------

#define MCP_ID_QUARY_ID_LIST                     (0x0009)  //查询监控项列表
#define MCP_ID_QUARY_PRIVATE_ID_LIST             (0x0F00)  //查询自定义监控项列表 


//------------------------------ 监控参数定义------------------------------
// 1.设备信息
#define MCP_ID_MANUFACTURER_CODE                 (0x0002)  //通过设备厂制造商名称，区分不同的设备厂家。 
#define MCP_ID_DEV_TYPE                          (0x0003)  //设备类别 
#define MCP_ID_DEVICE_MODEL                      (0x0004)  //设备型号 
#define MCP_ID_SN                                (0x0005)  //设备生产序列号 
#define MCP_ID_SW_VERSION                        (0x000A)  //监控的当前版本信息 
#define MCP_ID_TXMK                              (0x000B)  //远程数据通信模块 
#define MCP_ID_DATA_MODEL_VERSION                (0x000C)  //数据模型版本 
#define MCP_ID_ROUTER_ADDRESS                    (0x0873)  //设备路由登记地址 
#define MCP_ID_DEV_RESET                         (0x000D)  //设备重启 

// 2.网管参数
#define MCP_ID_SITE_ID                           (0x0101)  //站点编号 
#define MCP_ID_DEV_ID                            (0x0102)  //设备编号 
#define MCP_ID_IP_VERSION                        (0x0129)  //IPv4 / IPv6选择 
#define MCP_ID_IPV4                              (0x0130)  //监控中心IP地址（IP v4） 
#define MCP_ID_PORT                              (0x0131)  //网管系统IP服务端口号 
#define MCP_ID_IPV6                              (0x0132)  //监控中心IP地址（IP v6） 
#define MCP_ID_APN                               (0x0133)  //PS域接入点名称（即:APN） 
#define MCP_ID_HEARTBEAT_INTERVAL                (0x0134)  //设备的心跳包间隔时间 s
#define MCP_ID_USERNAME                          (0x0136)  //PS域参数:用户标识 
#define MCP_ID_PASSWD                            (0x0137)  //PS域参数:口令 
#define MCP_ID_TCP_UDP                           (0x0138)  //无线IP通信方式传输协议 
#define MCP_ID_DEV_PORT                          (0x0139)  //设备接收端口 
#define MCP_ID_REPORT_TYPE                       (0x0141)  //上报类型 
#define MCP_ID_COMM_MODE                         (0x0142)  //通信方式 

// 3.远程升级
#define MCP_ID_MCP_MODE_A                        (0x0010)  //设备监控软件运行模式 
#define MCP_ID_APC_MAX_LENGTH                    (0x0011)  //AP:C协议最大长度 
#define MCP_ID_MCP_B_INTERACTION_MODE            (0x0012)  //MCP:B 采用的交互机制 
#define MCP_ID_NUM_OF_CONTINUOUS_PACKS           (0x0013)  //连发系数(NC) 
#define MCP_ID_DEV_RESP_TIMEOUT                  (0x0014)  //设备响应超时 s
#define MCP_ID_TX_INTERVAL                       (0x0015)  //发送间隔时间 （此数据在NC＞1时才有意义） ms
#define MCP_ID_OTA_PAUSE_TIMEOUT                 (0x0016)  //暂停传输等待时间 s
#define MCP_ID_OTA_MODE_TIMEOUT                  (0x0017)  //监控模式－升级模式转换时间 s
#define MCP_ID_OTA_RESULT                        (0x0018)  //设备执行软件升级的结果 
#define MCP_ID_OTA_MODE                          (0x0020)  //设备使用的远程升级方式 
#define MCP_ID_MCP_MODE_B                        (0x0201)  //设备监控软件运行模式 
#define MCP_ID_OTA_PACKET_NUM                    (0x0202)  //下一个文件数据块序号 
#define MCP_ID_OTA_PACKET_LEN                    (0x0203)  //数据块长度 
#define MCP_ID_OTA_FILE_CRC                      (0x0301)  //文件标识码 
#define MCP_ID_OTA_TRANS_CTRL                    (0x0302)  //文件传输控制 
#define MCP_ID_OTA_PACK_ACK                      (0x0303)  //文件数据包应答 
#define MCP_ID_OTA_PACK_NUM                      (0x0304)  //文件数据块序号 
#define MCP_ID_OTA_PACK                          (0x0305)  //文件数据块 
#define MCP_ID_MCP_B_FILE_VER                    (0x03A0)  //当前待升级软件版本

// 4.采样参数
#define MCP_ID_FDD_UL_A_GAIN                     (0x05A4)  //FDD上行实际增益 
#define MCP_ID_FDD_BANDWIDTH                     (0x05E0)  //FDD带宽 
#define MCP_ID_FDD_BAND                          (0x05A5)  //FDD Band 
#define MCP_ID_FDD_CHANNEL                       (0x05A6)  //FDD Channel 
#define MCP_ID_FDD_PCI                           (0x05A7)  //FDD PCI 
#define MCP_ID_FDD_SINR                          (0x05A8)  //FDD SINR 
#define MCP_ID_FDD_RSRP                          (0x05A9)  //FDD RSRP 
#define MCP_ID_TDD_UL_A_GAIN                     (0x05B4)  //TDD上行实际增益 
#define MCP_ID_TDD_BANDWIDTH                     (0x05E1)  //TDD带宽 
#define MCP_ID_TDD_BAND                          (0x05B5)  //TDD Band 
#define MCP_ID_TDD_CHANNEL                       (0x05B6)  //TDD Channel 
#define MCP_ID_TDD_PCI                           (0x05B7)  //TDD PCI 
#define MCP_ID_TDD_SINR                          (0x05B8)  //TDD SINR 
#define MCP_ID_TDD_RSRP                          (0x05B9)  //TDD RSRP 
#define MCP_ID_NR1_UL_A_GAIN                     (0x05C0)  //5G上行实际增益 (通道1) 
#define MCP_ID_NR2_UL_A_GAIN                     (0x05C1)  //5G上行实际增益 (通道2) 
#define MCP_ID_NR_BANDWIDTH                      (0x05C2)  //5G带宽 
#define MCP_ID_NR_BAND                           (0x05EA)  //5G Band 
#define MCP_ID_NR_CHANNEL                        (0x05C4)  //5G Channel 
#define MCP_ID_NR_PCI                            (0x05E9)  //5G PCI 
#define MCP_ID_NR_SINR                           (0x05E8)  //5G SINR 
#define MCP_ID_NR_RSRP                           (0x05E5)  //5G RSRP 
#define MCP_ID_TIMESLOT_RATIO                    (0x05E2)  //5G上下行配比 
#define MCP_ID_SPECIAL_SUBFRAME                  (0x05E3)  //5G特殊子帧配置 
#define MCP_ID_FRAMEOFFSET                       (0x05E4)  //5G帧头时间调整量 

// 5.设置参数
#define MCP_ID_FDD_WARN_TH_DL_IN_OP              (0x0454)  //FDD下行输入功率过功率门限 
#define MCP_ID_FDD_WARN_TH_DL_IN_LP              (0x0453)  //FDD下行输入功率欠功率门限 
#define MCP_ID_TDD_WARN_TH_DL_IN_OP              (0x04D0)  //TDD下行输入功率过功率门限 
#define MCP_ID_TDD_WARN_TH_DL_IN_LP              (0x04D1)  //TDD下行输入功率欠功率门限 
#define MCP_ID_NR1_WARN_TH_DL_IN_OP              (0x04D6)  //5G 通道1下行输入功率过功率门限 
#define MCP_ID_NR1_WARN_TH_DL_IN_LP              (0x04D7)  //5G 通道1下行输入功率欠功率门限 
#define MCP_ID_NR2_WARN_TH_DL_IN_OP              (0x04E3)  //5G 通道2下行输入功率过功率门限 
#define MCP_ID_NR2_WARN_TH_DL_IN_LP              (0x04E4)  //5G 通道2下行输入功率欠功率门限 

// 6.告警状态
#define MCP_ID_TDD_WARN_DL_IN_OP                 (0x03A0)  //TDD下行输入过功率告警 
#define MCP_ID_TDD_WARN_DL_IN_LP                 (0x03A1)  //TDD下行输入欠功率告警 
#define MCP_ID_FDD_WARN_DL_IN_OP                 (0x03A2)  //FDD下行输入过功率告警 
#define MCP_ID_FDD_WARN_DL_IN_LP                 (0x03A3)  //FDD下行输入欠功率告警 
#define MCP_ID_NR1_WARN_DL_IN_OP                 (0x0330)  //5G通道1下行输入过功率告警 
#define MCP_ID_NR1_WARN_DL_IN_LP                 (0x0331)  //5G通道1下行输入欠功率告警 
#define MCP_ID_NR2_WARN_DL_IN_OP                 (0x0336)  //5G通道2下行输入过功率告警 
#define MCP_ID_NR2_WARN_DL_IN_LP                 (0x0337)  //5G通道2下行输入欠功率告警 
#define MCP_ID_FDD_WARN_SELF_EXECITATION         (0x0329)  //自激告警 


int16_t vMpRptParaList(uint8_t devId, uint8_t * src, uint16_t paraId);


#endif

