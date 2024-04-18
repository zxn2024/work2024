/*************************************** Copyright ******************************
 *
 * (C) Copyright 2022, Shaanxi Tianji Communication
 * All Rights Reserved
 *
 * FileName   : bsp_rf_slave.h
 * Version    : none
 * Author     : none
 * Date       : 2022-09-27
 * Description: none
 *******************************************************************************/
#ifndef __MCP_CORE_H_
#define __MCP_CORE_H_

#include "main.h"
#include "bsp_config.h"


/******************************** 宏定义 ***********************************/
#define MAX_MCP_DATA_LEN (1200) // MCP协议包最大长度
#define MAX_MRC_DATA_LEN (240) // 主从通信(MRC)通信包最大长度

// MCP协议包格式定义
#define ADDR_SOP (0)             // 起始符
#define ADDR_AP_PROTO_TYPE (1)   // 接入层协议(AP)
#define ADDR_VP_PROTO_TYPE (2)   // 承载层协议
#define ADDR_VP_SITE_ID (3)      // 站点id
#define ADDR_VP_DEVICE_ID (7)    // 设备id
#define ADDR_VP_PKT_NUM (8)      // 包序号
#define ADDR_VP_SIGN (10)        // vp层交互标志,发送方为0x80 接收方正常回复为0x00, 0x01表示接收方忙
#define ADDR_MCP_PROTO_TYPE (11) // MCP协议类型 0x01 MCP:A 0x02 MCP:B 0x03 MCP:C
#define ADDR_MCP_CMD_TYPE (12)   // 数据类型 上报/查询/设置
#define ADDR_MCP_RESP_SIGN (13)  // 应答标识,发起方填充0xff,回复方成功回复填充0x00,否则填充错误码
#define ADDR_MCP_DATA_FIELD (14) // MCP数据区

#define OFFSET_MCPA_DATA_LEN (0)    // MCPA 监控数据长度位置
#define OFFSET_MCPA_DATA_CODE (1)   // MCPA 监控码位置
#define OFFSET_MCPA_DATA_CODE_H (2) // MCPA 监控码位置
#define OFFSET_MCPA_DATA (3)        // MCPA 监控数据位置

#define OFFSET_MCPB_DATA_LEN (0)  // MCPB 监控数据长度位置
#define OFFSET_MCPB_DATA_CODE (2) // MCPB 监控码位置
#define OFFSET_MCPB_DATA (4)      // MCPB 监控数据位置

#define OFFSET_MCPC_DATA_LEN (0)  // MCPC 监控数据长度位置
#define OFFSET_MCPC_DATA_CODE (1) // MCPC 监控码位置
#define OFFSET_MCPC_DATA (5)      // MCPC 监控数据位置

#define OFFSET_CRC (0) // crc校验码长度
#define OFFSET_EOP (2) // 结束符长度

// MCP协议包起始符结束符
#define MCP_SOP (0x7E) // MCP协议包起始符
#define MCP_EOP (0x7E) // MCP协议包结束符

// AP层协议类型定义
#define AP_A (0x01)
#define AP_B (0x02)
#define AP_C (0x03)

// VP层协议类型定义
#define VP_A (0x01)

// VP层交互标志定义
#define VP_SIGN_SEND (0x80)
#define VP_SIGN_REPLY (0x00)

// MCP协议类型定义
#define MCP_A (0x01)   // MCPA协议,用来传输监控数据
#define MCP_B (0x02)   // MCPB协议,用来传输监控数据
#define MCP_C (0x03)   // MCPC协议,用来传输监控数据
#define MCP_MRC (0xF1) // 天基自定义协议,用来传输主从通信数据

// MCP协议包类型定义
#define MCP_CMD_TYPE_RPT (0x01)   // 上报
#define MCP_CMD_TYPE_QUERY (0x02) // 查询
#define MCP_CMD_TYPE_SET (0x03)   // 设置
#define MCP_CMD_TYPE_SWITCH_UPGRADE_MODE (0x10)   // 转换到软件升级模式
#define MCP_CMD_TYPE_SWITCH_SOFE_VERSION (0x11)   // 切换监控软件版本

// MCP协议包应答标识定义
#define MCP_RESP_SIGN_OK (0x00)          // 命令执行成功
#define MCP_RESP_SIGN_CONDITION (0x01)   // 命令被有条件执行
#define MCP_RESP_SIGN_ID_ERROR (0x02)    // 命令编号错
#define MCP_RESP_SIGN_LEN_ERROR (0x03)   // 长度错
#define MCP_RESP_SIGN_CRC_ERROR (0x04)   // CRC校验错(仅在调试时使用，正式产品中不返回此信息)
#define MCP_RESP_SIGN_PROTO_ERROR (0x05) // 监控协议标识错
#define MCP_RESP_SIGN_OTHER_ERROR (0xFE) // 其它错误
#define MCP_RESP_SIGN_SEND (0xFF)        // 命令发送

// 监控参数 错误代码
#define MCP_ERR_OK 0x00             // 无错误
#define MCP_ERR_IDERR 0x10          // 监控数据标识无法识别
#define MCP_ERR_SET_VALUE_OVER 0x20 // 监控数据的设置值超出范围
#define MCP_ERR_WIDTH_ERR 0x30      // 监控数据标识与监控数据的值不符合要求
#define MCP_ERR_LEN_ERR 0x40        // 监控数据标识与监控数据长度不匹配
#define MCP_ERR_VALUE_L 0x50        // 监控数据的检测值低于工作范围,显示"--"
#define MCP_ERR_VALUE_H 0x60        // 监控数据的检测值高于工作范围,显示"++"
#define MCP_ERR_DATA_INVALID 0x70   // 监控数据无法检测,显示"**"
#define MCP_ERR_OTHER 0x90          // 未列出的其他错误
#define MCP_ERR_ZDY 0xA0            // A~F厂家自定义
#define MCP_ERR_NOACT 0xB0          // 设备无应答
#define MCP_ERR_LOAD_FAULT 0xC0     // 加载失败

// 上报类别
#define MCP_REP_WARN 				0x01 // 告警上报
#define MCP_REP_SITE_OPEN 			0x02 // 主机开站上报
#define MCP_REP_FIX 				0x04 // 故障修复上报
#define MCP_REP_CONFIG_CHANGE 		0x05 // 主机配置变更上报
#define MCP_REP_LOGIN_MONITOR 		0x06 // 登陆到网管系统上报
#define MCP_REP_HEARTBEAT 			0x07 // 心跳上报
#define MCP_REP_UPDATE_SW 			0x08 // 设备的监控软件更新上报
#define MCP_REP_RESET_SW 			0x0B // 软件异常复位上报

#define MCP_REP_SITE_OPEN_HUB 		0x0C // hub开站上报
#define MCP_REP_CONFIG_CHANGE_HUB 	0x0D // hub配置变更上报
#define MCP_REP_SITE_OPEN_RU1 		0x0E // 从机1开站上报
#define MCP_REP_CONFIG_CHANGE_RU1 	0x0F // 从机1配置变更上报
#define MCP_REP_SITE_OPEN_RU2 		0x10 // 从机2开站上报
#define MCP_REP_CONFIG_CHANGE_RU2 	0x11 // 从机2配置变更上报
#define MCP_REP_SITE_OPEN_RU3 		0x12 // 从机3开站上报
#define MCP_REP_CONFIG_CHANGE_RU3 	0x13 // 从机3配置变更上报
#define MCP_REP_SITE_OPEN_RU4 		0x14 // 从机4开站上报
#define MCP_REP_CONFIG_CHANGE_RU4 	0x15 // 从机4配置变更上报
#define MCP_REP_SITE_OPEN_RU5 		0x16 // 从机5开站上报
#define MCP_REP_CONFIG_CHANGE_RU5 	0x17 // 从机5配置变更上报
#define MCP_REP_SITE_OPEN_RU6 		0x18 // 从机6开站上报
#define MCP_REP_CONFIG_CHANGE_RU6 	0x19 // 从机6配置变更上报
#define MCP_REP_SITE_OPEN_RU7 		0x1A // 从机7开站上报
#define MCP_REP_CONFIG_CHANGE_RU7 	0x1B // 从机7配置变更上报
#define MCP_REP_SITE_OPEN_RU8 		0x1C // 从机8开站上报
#define MCP_REP_CONFIG_CHANGE_RU8 	0x1D // 从机8配置变更上报


#define CONST_PDU_TXFS (2) // 1:短信  2: PS域  3:CS域
#define CONST_PDU_TXMK (1) // 远程数据通信模块
#define CONST_PDU_DATA_MODEL_VERSION    "1.2" //数据模型版本

// MCP消息来源定义
#define MCP_PC      (0x01)
#define MCP_MONITOR (0x02)
#define MCP_BLE     (0x03)

/******************************** 枚举定义 *******************************/


typedef enum
{
    MCP_BOOL = 0,
    MCP_UINT1,
    MCP_UINT2,
    MCP_UINT4,
    MCP_INT1,
    MCP_INT2,
    MCP_INT4,
    MCP_STRING,
    MCP_END
} MCP_TYPE_E; // 监控读写字节选择

typedef enum
{
    MCP_R = 0,
    MCP_W,
    MCP_RW
} MCP_RW_E; // 是否使用该监控项

typedef enum
{
    MCP_OFF = 0,
    MCP_ON

} MCP_MONITOR_ON_E; // 是否使用该监控项

typedef enum
{
    SERV_TYPE_PC = 0,   // PC端通过串口或usb连接到设备
    SERV_TYPE_MONITOR,  // 主机通过以太网连接到监控平台
    SERV_TYPE_MRC,      // 设备间通信
    SERV_TYPE_APP,      // 和手机小程序通信
} MCP_SERVICE_TYPE_E; // 业务类型,PC:0,监控:1,主从通信:2 APP:3


#define CONST_PDU_APCZDCD   (512)  //AP:C协议最大长度
#define CONST_PDU_MCPBJHJZ  (0)    //MCP:B交互机制 0:每包确认
#define CONST_PDU_LFXS      (0)    //连发系数, 不支持连发
#define CONST_PDU_SBXYCS    (10)   //设备响应超时, 单位S
#define CONST_PDU_FSJGSJ    (50)   //发送间隔时间 单位ms, uint2型
#define CONST_PDU_ZTDDSJ    (10)   //暂停传输等待时间, 单位S
#define CONST_PDU_JKMSZHSJ  (0)    //监控模式/升级模式转换时间, 单位S, uint2
#define CONST_PDU_YCSJFS    (1)    //远程升级方式: 0:无法升级, 1:MCP:B 2:FTP

#define MCPB_TRANS_CTRL_START    0x01/*文件传输开始*/
#define MCPB_TRANS_CTRL_END      0x02/*文件传输结束*/
#define MCPB_TRANS_CTRL_CANCEL   0x03/*监控中心取消软件升级*/
#define MCPB_TRANS_CTRL_COMPLETE 0x04/*软件升级正常结束*/
/******************************** 结构体定义 ***********************************/

// MCP发送协议包结构体,用于主从通信发包,或主动上报发包
typedef struct
{
    uint8_t mcp_proto_type;            // 协议类型
    uint8_t pkt_buf[MAX_MRC_DATA_LEN]; // 协议包数据区
    uint16_t devid;                  // 协议包数据区长度
    uint16_t pkt_id;                  // 协议包序号
    uint16_t pkt_len;                  // 协议包数据区长度
} mcp_send_pkt_t;

// 监控项列表结构体
typedef struct
{
    MCP_RW_E rd_wr;       // 读写标志
    MCP_TYPE_E data_type; // 数据类型
    int min_val;          // 最小值
    int max_val;          // 最大值
    uint16_t data_id;        // 监控码
} monitor_tab_t;

// 监控包结构体
typedef struct
{
    uint8_t start;         // 起始字符 '~'
    uint8_t ap_proto_type; // ap层协议类型 AP:A:0X01 AP:B:0X02 AP:C:0X03
    uint8_t vp_proto_type; // 定值1 与AP:A相同
    uint8_t site_id[4];    // 站点编号-四字节倒序
    uint8_t dev_id;        // 设备编号:
                           // -和监控通信时 主机 0，从机 0x01~0xFE, 本机 0xFF, 
                           // -用于主从通信时,使用从机蓝牙连接序号ble_conn_id: 主机为0 从设备从1起
    uint16_t vp_pack_num;  // 通信包标识号-2字节倒叙
    uint8_t vp_sign;       // vp层交互标识-命令发起方0x80,命令回复方0x00
    uint8_t mcp_type;      // mcp协议类型 mcpa:0x01,mcpb:0x02
    uint8_t cmd_type;      // 命令标识 0x01:上报 0x02:查询 0x03:设置
    uint8_t mcp_resp_sign; // 应答类型 命令发起方:0xFF 命令回复方:0x00
    uint8_t data_len;      // 数据长度
    uint16_t crc_result;   // 校验结果
    uint8_t stop;          // 结束字符 '~'

    uint8_t * ptr_src_buf;      // 指向监控源数据首地址
    uint8_t * ptr_site_resp;    // 站点编号回复填充
    uint8_t * ptr_devid_resp;   // 设备编号回复填充
    uint8_t * ptr_vp_resp;      // 回复指针：vp协议位地址
    uint8_t * ptr_mcp_resp;     // 回复指针：应答位地址
    uint8_t * ptr_data_unit;    // 数据指针：指向当前数据单元的首地址-从len开始
    uint8_t * ptr_mcp_data_code; // 监控id地址
    uint8_t * ptr_crc_L_resp;   // 回复指针：crc 校验位地址--低有效位
    uint8_t * ptr_crc_H_resp;   // 回复指针：crc 校验位地址--高有效位

    MCP_RW_E mcp_data_rw;     // 当前数据单元读写标志
    MCP_TYPE_E mcp_data_type; // 当前数据单元类型
    int mcp_data_min;         // 当前数据单元最小值
    int mcp_data_max;         // 当前数据单元最大值

    uint16_t tx_buf_len; // 发送buf总长度
    uint16_t rx_buf_len; // 接收buf总长度

    uint8_t msg_src;    // 消息来源

} MCP_MSG_T;
/******************************** 声明 ***********************************/
uint16_t MsgEscape(uint8_t * pBuff, uint16_t len);
uint16_t MsgUnescape(uint8_t * pSrc, uint8_t * pDest, uint16_t len);
uint16_t MsgSplit(uint8_t * src, uint8_t * dst, uint16_t len);

// 组包函数,用于通信发起方进行数据组包
void MCP_tx_packet_header_make(mcp_send_pkt_t * pkt_buf, uint8_t mcp_proto_type, uint8_t dev_id, uint8_t cmd_type); // 添加包头

void MCP_A_MRC_tx_packet_data_add(mcp_send_pkt_t * pkt_buf, uint16_t para_len, MCP_TYPE_E para_type, uint16_t code, void * para_buf); // 添加组包数据(MCPA&MRC)

void MCP_B_tx_packet_data_add(mcp_send_pkt_t * pkt_buf, uint16_t para_len, MCP_TYPE_E para_type, uint16_t code, void * para_buf); // 添加组包数据(MCPB)

void MCP_C_tx_packet_data_add(mcp_send_pkt_t * pkt_buf, uint16_t para_len, MCP_TYPE_E para_type, uint32_t code, void * para_buf); // 添加组包数据(MCPC)

void MCP_tx_packet_tail_make(mcp_send_pkt_t * pkt_buf); // 添加包尾

// 数据接收解析入口函数,用于通信接收方进行数据解析
uint16_t MCP_rx_pkt_analysis(uint8_t * rx_data, uint16_t len, uint8_t * reply_data, uint8_t msg_src);

// 用户自定义协议解析函数,移植时根据项目需求实现协议解析函数
uint16_t mcpA_msg_handler(uint8_t * rx_data, uint16_t len, uint8_t * reply_data, uint8_t msg_src);

uint16_t mcpB_msg_handler(uint8_t * rx_data, uint16_t len, uint8_t * reply_data, uint8_t msg_src);

uint16_t mcpC_msg_handler(uint8_t * rx_data, uint16_t len, uint8_t * reply_data, uint8_t msg_src);

uint16_t mcpMRC_msg_handler(uint8_t * rx_data, uint16_t len, uint8_t * reply_data, uint8_t msg_src);

// 监控项列表参数查询函数
Status_t get_MU_MCP_data_id_config(uint16_t data_id, MCP_MSG_T * mcp_msg);

Status_t get_RU_MCP_data_id_config(uint16_t data_id, MCP_MSG_T * mcp_msg);

// mcpB升级结果
uint8_t mcpB_upgrade_result(void);

#endif // __MCP_CORE_H_
