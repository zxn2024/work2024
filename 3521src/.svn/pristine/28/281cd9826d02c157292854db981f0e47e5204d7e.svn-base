#ifndef __MU_RU_PROTOCOL_PARA_LIST_H__
#define __MU_RU_PROTOCOL_PARA_LIST_H__

#include "main.h"
#include "mcp_core.h"
//------------------------------ 监控参数定义------------------------------
// 3521主从协议
#define MRC_TIME_SYNC                            (1)  //MU单片机对时,用于设置时间同步值给RU ms
#define MRC_DEV_TYPE                             (2)  //设备类型查询 
#define MRC_RUN_TIME                             (4)  //设备运行时间查询 ms
#define MRC_MCU_FW_VERSION                       (5)  //MCU软件版本 
#define MRC_BLE_FW_VERSION                       (6)  //BLE软件版本 
#define MRC_FPGA_FW_VERSION                      (7)  //FPGA软件版本 
#define MRC_8668_FW1_VERSION                     (8)  //8668固件1版本 
#define MRC_8668_FW2_VERSION                     (9)  //8668固件2版本 
#define MRC_8668_FW3_VERSION                     (10)  //8668固件3版本 
#define MRC_8668_FW4_VERSION                     (11)  //8668固件4版本 
#define MRC_8668_FW5_VERSION                     (12)  //8668固件5版本 
#define MRC_LTE_IN_PWR                           (13)  //LTE下行输入功率 db
#define MRC_NR_IN_PWR                            (14)  //5G通道1下行输入功率 db
#define MRC_NR2_IN_PWR                           (15)  //5G通道2下行输入功率 db
#define MRC_SELF_EXCITED                         (16)  //自激状态 
#define MRC_UL_SILENT                            (17)  //从机上报上行静默 db
#define MRC_HUB_CH1_OPEN_TIME                    (18)  //HUB通道1开启时间查询 ms
#define MRC_HUB_CH2_OPEN_TIME                    (19)  //HUB通道2开启时间查询 ms
#define MRC_HUB_CH3_OPEN_TIME                    (20)  //HUB通道3开启时间查询 ms
#define MRC_HUB_CH4_OPEN_TIME                    (21)  //HUB通道4开启时间查询 ms
#define MRC_HUB_CH5_OPEN_TIME                    (22)  //HUB通道5开启时间查询 ms
#define MRC_HUB_CH6_OPEN_TIME                    (23)  //HUB通道6开启时间查询 ms
#define MRC_HUB_CH7_OPEN_TIME                    (24)  //HUB通道7开启时间查询 ms
#define MRC_HUB_CH8_OPEN_TIME                    (25)  //HUB通道8开启时间查询 ms
#define MRC_HUB_CH1_OPEN                         (26)  //HUB通道1开关 
#define MRC_HUB_CH2_OPEN                         (27)  //HUB通道2开关 
#define MRC_HUB_CH3_OPEN                         (28)  //HUB通道3开关 
#define MRC_HUB_CH4_OPEN                         (29)  //HUB通道4开关 
#define MRC_HUB_CH5_OPEN                         (30)  //HUB通道5开关 
#define MRC_HUB_CH6_OPEN                         (31)  //HUB通道6开关 
#define MRC_HUB_CH7_OPEN                         (32)  //HUB通道7开关 
#define MRC_HUB_CH8_OPEN                         (33)  //HUB通道8开关 
#define MRC_SYSPC_SWITCH                         (34)  //系统功控开关 
#define MRC_SYSPC_LTE_UL_IN_EXPECT               (35)  //系统功控LTE上行期望接收功率 
#define MRC_SYSPC_LTE_DL_OUT                     (36)  //系统功控LTE发射功率 
#define MRC_SYSPC_NR_UL_IN_EXPECT                (37)  //系统功控NR上行期望接收功率 
#define MRC_SYSPC_NR_DL_OUT                      (38)  //系统功控NR发射功率 
#define MRC_RU_CH_CLOSE                          (39)  //从机射频通道关闭 
#define MRC_RU_CH_OPEN                           (40)  //从机射频通道打开 
#define MRC_HUB_LED1_OPEN                        (41)  //HUB通道1 led开关 
#define MRC_HUB_LED2_OPEN                        (42)  //HUB通道2 led开关 
#define MRC_HUB_LED3_OPEN                        (43)  //HUB通道3 led开关 
#define MRC_HUB_LED4_OPEN                        (44)  //HUB通道4 led开关 
#define MRC_HUB_LED5_OPEN                        (45)  //HUB通道5 led开关 
#define MRC_HUB_LED6_OPEN                        (46)  //HUB通道6 led开关 
#define MRC_HUB_LED7_OPEN                        (47)  //HUB通道7 led开关 
#define MRC_HUB_LED8_OPEN                        (48)  //HUB通道8 led开关 

Status_t get_MRC_data_id_config(uint16_t data_id, MCP_MSG_T *mcp_msg);

#endif /* __MU_RU_PROTOCOL_PARA_LIST_H__ */
