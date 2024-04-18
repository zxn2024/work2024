#include "mu_ru_comm_para_list.h"
#include "mcp_core.h"
#include "main.h"

/******************************** MU通信参数列表 ***********************************/
#define PARA_TAB_SIZE_MU 2
monitor_tab_t mrc_para_table_mu[PARA_TAB_SIZE_MU] = {
    {MCP_R , MCP_UINT4  , 0    , 0    , MRC_RUN_TIME                            }, // 设备运行时间查询
    {MCP_RW, MCP_INT1   , 0    , 0    , MRC_UL_SILENT                           }, // 从机上报上行静默
};

/******************************** RU通信参数列表 ***********************************/
#define PARA_TAB_SIZE_RU 23
monitor_tab_t mrc_para_table_ru[PARA_TAB_SIZE_RU] = {
    {MCP_RW, MCP_UINT4  , 0    , 0    , MRC_TIME_SYNC                           }, // MU单片机对时,用于设置时间同步值给RU
    {MCP_R , MCP_UINT1  , 0    , 0    , MRC_DEV_TYPE                            }, // 设备类型查询
    {MCP_R , MCP_UINT4  , 0    , 0    , MRC_RUN_TIME                            }, // 设备运行时间查询
    {MCP_R , MCP_UINT4  , 0    , 0    , MRC_MCU_FW_VERSION                      }, // MCU软件版本
    {MCP_R , MCP_UINT4  , 0    , 0    , MRC_BLE_FW_VERSION                      }, // BLE软件版本
    {MCP_R , MCP_UINT4  , 0    , 0    , MRC_FPGA_FW_VERSION                     }, // FPGA软件版本
    {MCP_R , MCP_UINT4  , 0    , 0    , MRC_8668_FW1_VERSION                    }, // 8668固件1版本
    {MCP_R , MCP_UINT4  , 0    , 0    , MRC_8668_FW2_VERSION                    }, // 8668固件2版本
    {MCP_R , MCP_UINT4  , 0    , 0    , MRC_8668_FW3_VERSION                    }, // 8668固件3版本
    {MCP_R , MCP_UINT4  , 0    , 0    , MRC_8668_FW4_VERSION                    }, // 8668固件4版本
    {MCP_R , MCP_UINT4  , 0    , 0    , MRC_8668_FW5_VERSION                    }, // 8668固件5版本
    {MCP_R , MCP_INT1   , 0    , 0    , MRC_LTE_IN_PWR                          }, // LTE下行输入功率
    {MCP_R , MCP_INT1   , 0    , 0    , MRC_NR_IN_PWR                           }, // 5G通道1下行输入功率
    {MCP_R , MCP_INT1   , 0    , 0    , MRC_NR2_IN_PWR                          }, // 5G通道2下行输入功率
    {MCP_R , MCP_UINT1  , 0    , 0    , MRC_SELF_EXCITED                        }, // 自激状态
    {MCP_RW, MCP_INT1   , 0    , 0    , MRC_UL_SILENT                           }, // 从机上报上行静默
    {MCP_W , MCP_UINT1  , 0    , 0    , MRC_SYSPC_SWITCH                        }, // 系统功控开关
    {MCP_W , MCP_INT1   , 0    , 0    , MRC_SYSPC_LTE_UL_IN_EXPECT              }, // 系统功控LTE上行期望接收功率
    {MCP_W , MCP_INT1   , 0    , 0    , MRC_SYSPC_LTE_DL_OUT                    }, // 系统功控LTE发射功率
    {MCP_W , MCP_INT1   , 0    , 0    , MRC_SYSPC_NR_UL_IN_EXPECT               }, // 系统功控NR上行期望接收功率
    {MCP_W , MCP_INT1   , 0    , 0    , MRC_SYSPC_NR_DL_OUT                     }, // 系统功控NR发射功率
    {MCP_W , MCP_UINT1  , 0    , 0    , MRC_RU_CH_CLOSE                         }, // 从机射频通道关闭
    {MCP_W , MCP_UINT1  , 0    , 0    , MRC_RU_CH_OPEN                          }, // 从机射频通道打开
};


/******************************** HUB通信参数列表 ***********************************/
#define PARA_TAB_SIZE_HUB 27
monitor_tab_t mrc_para_table_hub[PARA_TAB_SIZE_HUB] = {
    {MCP_RW, MCP_UINT4  , 0    , 0    , MRC_TIME_SYNC                           }, // MU单片机对时,用于设置时间同步值给RU
    {MCP_R , MCP_UINT1  , 0    , 0    , MRC_DEV_TYPE                            }, // 设备类型查询
    {MCP_R , MCP_UINT4  , 0    , 0    , MRC_RUN_TIME                            }, // 设备运行时间查询
    {MCP_R , MCP_INT4   , 0    , 0    , MRC_HUB_CH1_OPEN_TIME                   }, // HUB通道1开启时间查询
    {MCP_R , MCP_INT4   , 0    , 0    , MRC_HUB_CH2_OPEN_TIME                   }, // HUB通道2开启时间查询
    {MCP_R , MCP_INT4   , 0    , 0    , MRC_HUB_CH3_OPEN_TIME                   }, // HUB通道3开启时间查询
    {MCP_R , MCP_INT4   , 0    , 0    , MRC_HUB_CH4_OPEN_TIME                   }, // HUB通道4开启时间查询
    {MCP_R , MCP_INT4   , 0    , 0    , MRC_HUB_CH5_OPEN_TIME                   }, // HUB通道5开启时间查询
    {MCP_R , MCP_INT4   , 0    , 0    , MRC_HUB_CH6_OPEN_TIME                   }, // HUB通道6开启时间查询
    {MCP_R , MCP_INT4   , 0    , 0    , MRC_HUB_CH7_OPEN_TIME                   }, // HUB通道7开启时间查询
    {MCP_R , MCP_INT4   , 0    , 0    , MRC_HUB_CH8_OPEN_TIME                   }, // HUB通道8开启时间查询
    {MCP_RW, MCP_UINT1  , 0    , 0    , MRC_HUB_CH1_OPEN                        }, // HUB通道1开关
    {MCP_RW, MCP_UINT1  , 0    , 0    , MRC_HUB_CH2_OPEN                        }, // HUB通道2开关
    {MCP_RW, MCP_UINT1  , 0    , 0    , MRC_HUB_CH3_OPEN                        }, // HUB通道3开关
    {MCP_RW, MCP_UINT1  , 0    , 0    , MRC_HUB_CH4_OPEN                        }, // HUB通道4开关
    {MCP_RW, MCP_UINT1  , 0    , 0    , MRC_HUB_CH5_OPEN                        }, // HUB通道5开关
    {MCP_RW, MCP_UINT1  , 0    , 0    , MRC_HUB_CH6_OPEN                        }, // HUB通道6开关
    {MCP_RW, MCP_UINT1  , 0    , 0    , MRC_HUB_CH7_OPEN                        }, // HUB通道7开关
    {MCP_RW, MCP_UINT1  , 0    , 0    , MRC_HUB_CH8_OPEN                        }, // HUB通道8开关
    {MCP_W , MCP_UINT1  , 0    , 0    , MRC_HUB_LED1_OPEN                       }, // HUB通道1 led开关
    {MCP_W , MCP_UINT1  , 0    , 0    , MRC_HUB_LED2_OPEN                       }, // HUB通道2 led开关
    {MCP_W , MCP_UINT1  , 0    , 0    , MRC_HUB_LED3_OPEN                       }, // HUB通道3 led开关
    {MCP_W , MCP_UINT1  , 0    , 0    , MRC_HUB_LED4_OPEN                       }, // HUB通道4 led开关
    {MCP_W , MCP_UINT1  , 0    , 0    , MRC_HUB_LED5_OPEN                       }, // HUB通道5 led开关
    {MCP_W , MCP_UINT1  , 0    , 0    , MRC_HUB_LED6_OPEN                       }, // HUB通道6 led开关
    {MCP_W , MCP_UINT1  , 0    , 0    , MRC_HUB_LED7_OPEN                       }, // HUB通道7 led开关
    {MCP_W , MCP_UINT1  , 0    , 0    , MRC_HUB_LED8_OPEN                       }, // HUB通道8 led开关
};


/*****************************************************************************
 * @Name	  : get_MRC_data_id_config
 * @Brief  : 获取监控标识符对应的数据配置,包括读写权限,数据类型,最大最小值
 * @Param  : data_id: 监控标识符
 *			mcp_msg: 指向收到的消息
 * @Retval : 返回错误码
 * @Author :
 * @Data   : 2024-3-1
 * @Note   :
 *****************************************************************************/
Status_t get_MRC_data_id_config(uint16_t data_id, MCP_MSG_T *mcp_msg)
{
    uint32_t i = 0;
    for(i = 0; i < PARA_TAB_SIZE_MU; i++)
    {
        if(data_id == mrc_para_table_mu[i].data_id)
        {
            mcp_msg->mcp_data_type = mrc_para_table_mu[i].data_type;
            mcp_msg->mcp_data_rw = mrc_para_table_mu[i].rd_wr;
            mcp_msg->mcp_data_min = mrc_para_table_mu[i].min_val;
            mcp_msg->mcp_data_max = mrc_para_table_mu[i].max_val;
            return STAT_OK;
        }
    }
    for(i = 0; i < PARA_TAB_SIZE_RU; i++)
    {
        if(data_id == mrc_para_table_ru[i].data_id)
        {
            mcp_msg->mcp_data_type = mrc_para_table_ru[i].data_type;
            mcp_msg->mcp_data_rw = mrc_para_table_ru[i].rd_wr;
            mcp_msg->mcp_data_min = mrc_para_table_ru[i].min_val;
            mcp_msg->mcp_data_max = mrc_para_table_ru[i].max_val;
            return STAT_OK;
        }
    }
    for(i = 0; i < PARA_TAB_SIZE_HUB; i++)
    {
        if(data_id == mrc_para_table_hub[i].data_id)
        {
            mcp_msg->mcp_data_type = mrc_para_table_hub[i].data_type;
            mcp_msg->mcp_data_rw = mrc_para_table_hub[i].rd_wr;
            mcp_msg->mcp_data_min = mrc_para_table_hub[i].min_val;
            mcp_msg->mcp_data_max = mrc_para_table_hub[i].max_val;
            return STAT_OK;
        }
    }
    return STAT_ERROR;
}

