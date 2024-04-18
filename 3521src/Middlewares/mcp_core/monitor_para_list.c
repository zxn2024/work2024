#include "monitor_para_list.h"
#include "mcp_core.h"
#include "main.h"

/******************************** MU监控参数列表 ***********************************/
#define PARA_TAB_SIZE_MU 75
monitor_tab_t monitor_para_table_mu[PARA_TAB_SIZE_MU] = {
    {MCP_RW, MCP_STRING , 0    , 0    , MCP_ID_MANUFACTURER_CODE                }, // 通过设备厂制造商名称，区分不同的设备厂家。
    {MCP_RW, MCP_UINT1  , 0    , 0    , MCP_ID_DEV_TYPE                         }, // 设备类别
    {MCP_RW, MCP_STRING , 0    , 0    , MCP_ID_DEVICE_MODEL                     }, // 设备型号
    {MCP_RW, MCP_STRING , 0    , 0    , MCP_ID_SN                               }, // 设备生产序列号
    {MCP_R , MCP_STRING , 0    , 0    , MCP_ID_SW_VERSION                       }, // 监控的当前版本信息
    {MCP_R , MCP_UINT1  , 0    , 0    , MCP_ID_TXMK                             }, // 远程数据通信模块
    {MCP_R , MCP_STRING , 0    , 0    , MCP_ID_DATA_MODEL_VERSION               }, // 数据模型版本
    {MCP_R , MCP_UINT4  , 0    , 0    , MCP_ID_ROUTER_ADDRESS                   }, // 设备路由登记地址
    {MCP_RW, MCP_BOOL   , 0    , 1    , MCP_ID_DEV_RESET                        }, // 设备重启
    {MCP_RW, MCP_UINT4  , 0    , 0    , MCP_ID_SITE_ID                          }, // 站点编号
    {MCP_RW, MCP_UINT1  , 0    , 0    , MCP_ID_DEV_ID                           }, // 设备编号
    {MCP_RW, MCP_UINT1  , 1    , 2    , MCP_ID_IP_VERSION                       }, // IPv4 / IPv6选择
    {MCP_RW, MCP_STRING , 0    , 0    , MCP_ID_IPV4                             }, // 监控中心IP地址（IP v4）
    {MCP_RW, MCP_UINT2  , 0    , 0    , MCP_ID_PORT                             }, // 网管系统IP服务端口号
    {MCP_RW, MCP_STRING , 0    , 0    , MCP_ID_IPV6                             }, // 监控中心IP地址（IP v6）
    {MCP_RW, MCP_STRING , 0    , 0    , MCP_ID_APN                              }, // PS域接入点名称（即:APN）
    {MCP_RW, MCP_UINT2  , 10   , 180  , MCP_ID_HEARTBEAT_INTERVAL               }, // 设备的心跳包间隔时间
    {MCP_RW, MCP_STRING , 0    , 0    , MCP_ID_USERNAME                         }, // PS域参数:用户标识
    {MCP_RW, MCP_STRING , 0    , 0    , MCP_ID_PASSWD                           }, // PS域参数:口令
    {MCP_RW, MCP_UINT1  , 1    , 2    , MCP_ID_TCP_UDP                           }, // 无线IP通信方式传输协议
    {MCP_RW, MCP_UINT2  , 0    , 0    , MCP_ID_DEV_PORT                         }, // 设备接收端口
    {MCP_RW , MCP_UINT1 , 0    , 0    , MCP_ID_REPORT_TYPE                      }, // 上报类型
    {MCP_R , MCP_UINT1  , 2    , 2    , MCP_ID_COMM_MODE                        }, // 通信方式
    {MCP_R , MCP_UINT1  , 0    , 0    , MCP_ID_MCP_MODE_A                       }, // 设备监控软件运行模式
    {MCP_R , MCP_UINT2  , 0    , 0    , MCP_ID_APC_MAX_LENGTH                   }, // AP:C协议最大长度
    {MCP_R , MCP_UINT1  , 0    , 0    , MCP_ID_MCP_B_INTERACTION_MODE           }, // MCP:B 采用的交互机制
    {MCP_R , MCP_UINT1  , 0    , 0    , MCP_ID_NUM_OF_CONTINUOUS_PACKS          }, // 连发系数(NC)
    {MCP_R , MCP_UINT1  , 0    , 0    , MCP_ID_DEV_RESP_TIMEOUT                 }, // 设备响应超时
    {MCP_R , MCP_UINT2  , 0    , 0    , MCP_ID_TX_INTERVAL                      }, // 发送间隔时间 （此数据在NC＞1时才有意义）
    {MCP_R , MCP_UINT1  , 0    , 0    , MCP_ID_OTA_PAUSE_TIMEOUT                }, // 暂停传输等待时间
    {MCP_R , MCP_UINT2  , 0    , 0    , MCP_ID_OTA_MODE_TIMEOUT                 }, // 监控模式－升级模式转换时间
    {MCP_R , MCP_UINT1  , 0    , 0    , MCP_ID_OTA_RESULT                       }, // 设备执行软件升级的结果
    {MCP_R , MCP_UINT1  , 0    , 0    , MCP_ID_OTA_MODE                         }, // 设备使用的远程升级方式
    {MCP_R , MCP_INT1   , 0    , 0    , MCP_ID_FDD_UL_A_GAIN                    }, // FDD上行实际增益
    {MCP_R , MCP_UINT1  , 0    , 0    , MCP_ID_FDD_BANDWIDTH                    }, // FDD带宽
    {MCP_R , MCP_UINT1  , 0    , 0    , MCP_ID_FDD_BAND                         }, // FDD Band
    {MCP_R , MCP_UINT4  , 0    , 0    , MCP_ID_FDD_CHANNEL                      }, // FDD Channel
    {MCP_R , MCP_UINT2  , 0    , 0    , MCP_ID_FDD_PCI                          }, // FDD PCI
    {MCP_R , MCP_INT2   , 0    , 0    , MCP_ID_FDD_SINR                         }, // FDD SINR
    {MCP_R , MCP_INT2   , 0    , 0    , MCP_ID_FDD_RSRP                         }, // FDD RSRP
    {MCP_R , MCP_INT1   , 0    , 0    , MCP_ID_TDD_UL_A_GAIN                    }, // TDD上行实际增益
    {MCP_R , MCP_UINT1  , 0    , 0    , MCP_ID_TDD_BANDWIDTH                    }, // TDD带宽
    {MCP_R , MCP_UINT1  , 0    , 0    , MCP_ID_TDD_BAND                         }, // TDD Band
    {MCP_R , MCP_UINT4  , 0    , 0    , MCP_ID_TDD_CHANNEL                      }, // TDD Channel
    {MCP_R , MCP_UINT2  , 0    , 0    , MCP_ID_TDD_PCI                          }, // TDD PCI
    {MCP_R , MCP_INT2   , 0    , 0    , MCP_ID_TDD_SINR                         }, // TDD SINR
    {MCP_R , MCP_INT2   , 0    , 0    , MCP_ID_TDD_RSRP                         }, // TDD RSRP
    {MCP_R , MCP_INT1   , 0    , 0    , MCP_ID_NR1_UL_A_GAIN                    }, // 5G上行实际增益 (通道1)
    {MCP_R , MCP_INT1   , 0    , 0    , MCP_ID_NR2_UL_A_GAIN                    }, // 5G上行实际增益 (通道2)
    {MCP_R , MCP_UINT1  , 0    , 0    , MCP_ID_NR_BANDWIDTH                     }, // 5G带宽
    {MCP_R , MCP_UINT1  , 0    , 0    , MCP_ID_NR_BAND                          }, // 5G Band
    {MCP_R , MCP_UINT4  , 0    , 0    , MCP_ID_NR_CHANNEL                       }, // 5G Channel
    {MCP_R , MCP_UINT2  , 0    , 0    , MCP_ID_NR_PCI                           }, // 5G PCI
    {MCP_R , MCP_INT2   , 0    , 0    , MCP_ID_NR_SINR                          }, // 5G SINR
    {MCP_R , MCP_INT2   , 0    , 0    , MCP_ID_NR_RSRP                          }, // 5G RSRP
    {MCP_RW, MCP_UINT1  , 0    , 1    , MCP_ID_TIMESLOT_RATIO                   }, // 5G上下行配比
    {MCP_RW, MCP_UINT1  , 1    , 6    , MCP_ID_SPECIAL_SUBFRAME                 }, // 5G特殊子帧配置
    {MCP_RW, MCP_INT2   , 1    , 3    , MCP_ID_FRAMEOFFSET                      }, // 5G帧头时间调整量
    {MCP_RW, MCP_INT1   , -128 , 127  , MCP_ID_FDD_WARN_TH_DL_IN_OP             }, // FDD下行输入功率过功率门限
    {MCP_RW, MCP_INT1   , -128 , 127  , MCP_ID_FDD_WARN_TH_DL_IN_LP             }, // FDD下行输入功率欠功率门限
    {MCP_RW, MCP_INT1   , -128 , 127  , MCP_ID_TDD_WARN_TH_DL_IN_OP             }, // TDD下行输入功率过功率门限
    {MCP_RW, MCP_INT1   , -128 , 127  , MCP_ID_TDD_WARN_TH_DL_IN_LP             }, // TDD下行输入功率欠功率门限
    {MCP_RW, MCP_INT1   , -128 , 127  , MCP_ID_NR1_WARN_TH_DL_IN_OP             }, // 5G 通道1下行输入功率过功率门限
    {MCP_RW, MCP_INT1   , -128 , 127  , MCP_ID_NR1_WARN_TH_DL_IN_LP             }, // 5G 通道1下行输入功率欠功率门限
    {MCP_RW, MCP_INT1   , -128 , 127  , MCP_ID_NR2_WARN_TH_DL_IN_OP             }, // 5G 通道2下行输入功率过功率门限
    {MCP_RW, MCP_INT1   , -128 , 127  , MCP_ID_NR2_WARN_TH_DL_IN_LP             }, // 5G 通道2下行输入功率欠功率门限
    {MCP_R , MCP_BOOL   , 0    , 0    , MCP_ID_TDD_WARN_DL_IN_OP                }, // TDD下行输入过功率告警
    {MCP_R , MCP_BOOL   , 0    , 0    , MCP_ID_TDD_WARN_DL_IN_LP                }, // TDD下行输入欠功率告警
    {MCP_R , MCP_BOOL   , 0    , 0    , MCP_ID_FDD_WARN_DL_IN_OP                }, // FDD下行输入过功率告警
    {MCP_R , MCP_BOOL   , 0    , 0    , MCP_ID_FDD_WARN_DL_IN_LP                }, // FDD下行输入欠功率告警
    {MCP_R , MCP_BOOL   , 0    , 0    , MCP_ID_NR1_WARN_DL_IN_OP                }, // 5G通道1下行输入过功率告警
    {MCP_R , MCP_BOOL   , 0    , 0    , MCP_ID_NR1_WARN_DL_IN_LP                }, // 5G通道1下行输入欠功率告警
    {MCP_R , MCP_BOOL   , 0    , 0    , MCP_ID_NR2_WARN_DL_IN_OP                }, // 5G通道2下行输入过功率告警
    {MCP_R , MCP_BOOL   , 0    , 0    , MCP_ID_NR2_WARN_DL_IN_LP                }, // 5G通道2下行输入欠功率告警
    {MCP_R , MCP_BOOL   , 0    , 0    , MCP_ID_FDD_WARN_SELF_EXECITATION        }, // 自激告警
};




/******************************** RU监控参数列表 ***********************************/
#define PARA_TAB_SIZE_RU 36
monitor_tab_t monitor_para_table_ru[PARA_TAB_SIZE_RU] = {
    {MCP_RW, MCP_STRING , 0    , 0    , MCP_ID_MANUFACTURER_CODE                }, // 通过设备厂制造商名称，区分不同的设备厂家。
    {MCP_RW, MCP_UINT1  , 0    , 0    , MCP_ID_DEV_TYPE                         }, // 设备类别
    {MCP_RW, MCP_STRING , 0    , 0    , MCP_ID_DEVICE_MODEL                     }, // 设备型号
    {MCP_RW, MCP_STRING , 0    , 0    , MCP_ID_SN                               }, // 设备生产序列号
    {MCP_R , MCP_STRING , 0    , 0    , MCP_ID_SW_VERSION                       }, // 监控的当前版本信息
    {MCP_R , MCP_UINT4  , 0    , 0    , MCP_ID_ROUTER_ADDRESS                   }, // 设备路由登记地址
    {MCP_RW, MCP_BOOL   , 0    , 1    , MCP_ID_DEV_RESET                        }, // 设备重启
    {MCP_RW, MCP_UINT4  , 0    , 0    , MCP_ID_SITE_ID                          }, // 站点编号
    {MCP_RW, MCP_UINT1  , 0    , 0    , MCP_ID_DEV_ID                           }, // 设备编号
    {MCP_R , MCP_UINT1  , 0    , 0    , MCP_ID_MCP_MODE_A                       }, // 设备监控软件运行模式
    {MCP_R , MCP_UINT2  , 0    , 0    , MCP_ID_APC_MAX_LENGTH                   }, // AP:C协议最大长度
    {MCP_R , MCP_UINT1  , 0    , 0    , MCP_ID_MCP_B_INTERACTION_MODE           }, // MCP:B 采用的交互机制
    {MCP_R , MCP_UINT1  , 0    , 0    , MCP_ID_NUM_OF_CONTINUOUS_PACKS          }, // 连发系数(NC)
    {MCP_R , MCP_UINT1  , 0    , 0    , MCP_ID_DEV_RESP_TIMEOUT                 }, // 设备响应超时
    {MCP_R , MCP_UINT2  , 0    , 0    , MCP_ID_TX_INTERVAL                      }, // 发送间隔时间 （此数据在NC＞1时才有意义）
    {MCP_R , MCP_UINT1  , 0    , 0    , MCP_ID_OTA_PAUSE_TIMEOUT                }, // 暂停传输等待时间
    {MCP_R , MCP_UINT2  , 0    , 0    , MCP_ID_OTA_MODE_TIMEOUT                 }, // 监控模式－升级模式转换时间
    {MCP_R , MCP_UINT1  , 0    , 0    , MCP_ID_OTA_RESULT                       }, // 设备执行软件升级的结果
    {MCP_R , MCP_UINT1  , 0    , 0    , MCP_ID_OTA_MODE                         }, // 设备使用的远程升级方式
    {MCP_RW, MCP_INT1   , -128 , 127  , MCP_ID_FDD_WARN_TH_DL_IN_OP             }, // FDD下行输入功率过功率门限
    {MCP_RW, MCP_INT1   , -128 , 127  , MCP_ID_FDD_WARN_TH_DL_IN_LP             }, // FDD下行输入功率欠功率门限
    {MCP_RW, MCP_INT1   , -128 , 127  , MCP_ID_TDD_WARN_TH_DL_IN_OP             }, // TDD下行输入功率过功率门限
    {MCP_RW, MCP_INT1   , -128 , 127  , MCP_ID_TDD_WARN_TH_DL_IN_LP             }, // TDD下行输入功率欠功率门限
    {MCP_RW, MCP_INT1   , -128 , 127  , MCP_ID_NR1_WARN_TH_DL_IN_OP             }, // 5G 通道1下行输入功率过功率门限
    {MCP_RW, MCP_INT1   , -128 , 127  , MCP_ID_NR1_WARN_TH_DL_IN_LP             }, // 5G 通道1下行输入功率欠功率门限
    {MCP_RW, MCP_INT1   , -128 , 127  , MCP_ID_NR2_WARN_TH_DL_IN_OP             }, // 5G 通道2下行输入功率过功率门限
    {MCP_RW, MCP_INT1   , -128 , 127  , MCP_ID_NR2_WARN_TH_DL_IN_LP             }, // 5G 通道2下行输入功率欠功率门限
    {MCP_R , MCP_BOOL   , 0    , 0    , MCP_ID_TDD_WARN_DL_IN_OP                }, // TDD下行输入过功率告警
    {MCP_R , MCP_BOOL   , 0    , 0    , MCP_ID_TDD_WARN_DL_IN_LP                }, // TDD下行输入欠功率告警
    {MCP_R , MCP_BOOL   , 0    , 0    , MCP_ID_FDD_WARN_DL_IN_OP                }, // FDD下行输入过功率告警
    {MCP_R , MCP_BOOL   , 0    , 0    , MCP_ID_FDD_WARN_DL_IN_LP                }, // FDD下行输入欠功率告警
    {MCP_R , MCP_BOOL   , 0    , 0    , MCP_ID_NR1_WARN_DL_IN_OP                }, // 5G通道1下行输入过功率告警
    {MCP_R , MCP_BOOL   , 0    , 0    , MCP_ID_NR1_WARN_DL_IN_LP                }, // 5G通道1下行输入欠功率告警
    {MCP_R , MCP_BOOL   , 0    , 0    , MCP_ID_NR2_WARN_DL_IN_OP                }, // 5G通道2下行输入过功率告警
    {MCP_R , MCP_BOOL   , 0    , 0    , MCP_ID_NR2_WARN_DL_IN_LP                }, // 5G通道2下行输入欠功率告警
    {MCP_R , MCP_BOOL   , 0    , 0    , MCP_ID_FDD_WARN_SELF_EXECITATION        }, // 自激告警
};


#define PARA_TAB_SIZE_HUB 10
monitor_tab_t monitor_para_table_hub[PARA_TAB_SIZE_HUB] = {
    {MCP_RW, MCP_STRING , 0    , 0    , MCP_ID_MANUFACTURER_CODE                }, // 通过设备厂制造商名称，区分不同的设备厂家。
    {MCP_RW, MCP_UINT1  , 0    , 0    , MCP_ID_DEV_TYPE                         }, // 设备类别
    {MCP_RW, MCP_STRING , 0    , 0    , MCP_ID_DEVICE_MODEL                     }, // 设备型号
    {MCP_RW, MCP_STRING , 0    , 0    , MCP_ID_SN                               }, // 设备生产序列号
    {MCP_R , MCP_STRING , 0    , 0    , MCP_ID_SW_VERSION                       }, // 监控的当前版本信息
    {MCP_R , MCP_STRING , 0    , 0    , MCP_ID_DATA_MODEL_VERSION               }, // 数据模型版本
    {MCP_R , MCP_UINT4  , 0    , 0    , MCP_ID_ROUTER_ADDRESS                   }, // 设备路由登记地址
    {MCP_RW, MCP_BOOL   , 0    , 1    , MCP_ID_DEV_RESET                        }, // 设备重启
    {MCP_RW, MCP_UINT4  , 0    , 0    , MCP_ID_SITE_ID                          }, // 站点编号
    {MCP_RW, MCP_UINT1  , 0    , 0    , MCP_ID_DEV_ID                           }, // 设备编号
};

/*****************************************************************************
* @Name	  : vPduParaListPkg
* @Brief  : 上报参数列表数据组包
* @Param  :  src: [输入/出]
**			 list: 参数列表数组地址
**			 listNum: 参数列表长度
* @Retval :  数据包长度
* @Author :
* @Data   : 2022-11-24
* @Note   :
*****************************************************************************/
int16_t vPduParaListPkg(uint8_t * src, monitor_tab_t * list, uint16_t listNum)
{
#define PDU_PKG_ELEMENT_MAX_NUM     ( 50 )  //每包数据发送的最大的参数数量

    uint8_t * pt_len = src; //当前监控项长度
    uint8_t * pt_data_start = src + 5; //列表元素填充首地址
    uint8_t * pt_pkg_total = src + 3; //总包数
    uint8_t * pt_pkg_id = src + 4;  //当前查询包编号

    uint8_t pkg_total = 0;
    uint8_t last_pkg_len = 0;
    uint8_t now_pkg_len = 0;
    uint16_t element_id = 0;
    int16_t ssPkgLen = 0;  //数据包总长度

    if(*pt_len != 0x05)
        return -1;

    //计算总的上报包数量
    pkg_total = listNum / 50;
    last_pkg_len = listNum % 50;

    if(last_pkg_len)
        pkg_total += 1;

    if(*pt_pkg_total > pkg_total)
        return -1;

    if(*pt_pkg_id > *pt_pkg_total)
        return -1;

    //修改总包数
    *pt_pkg_total = pkg_total;

    //当前包发送的参数数量
    now_pkg_len = (*pt_pkg_id == pkg_total) ? last_pkg_len : PDU_PKG_ELEMENT_MAX_NUM;

    //参数数据长度
    *pt_len = (5 + now_pkg_len * 2);

    //填充数据
    for(uint8_t i = 0; i < now_pkg_len; i++)
    {
        element_id = list[(*pt_pkg_id - 1) * 50 + i ].data_id;
        *(pt_data_start + i * 2) = (uint8_t)(element_id & 0x00FF);
        *(pt_data_start + i * 2 + 1) = (uint8_t)(element_id >> 8);
    }

    //结束符
    *(pt_data_start + now_pkg_len * 2 + 2) = 0x7E;     //结束字符

    ssPkgLen = 5 + now_pkg_len * 2 + 3; //包含结束标识0x7E的总长度
    return(ssPkgLen);
}



/*****************************************************************************
* @Name	  : vMpParaListRptPkg
* @Brief  :
* @Param  :  devId:  [输入] 设备ID, 0:主机, 1~0xFE 从机 , 0xFF:本机
**			 src:    [输出]
**			 paraId: [输入]
* @Retval :
* @Author :
* @Data   : 2022-11-24
* @Note   :
*****************************************************************************/
int16_t vMpRptParaList(uint8_t devId, uint8_t * src, uint16_t paraId)
{

    monitor_tab_t * pt = NULL;
    uint16_t num = 0;
    int16_t ssPkgLen = 0;

    #if (LOCAL_MODULE_TYPE == _MU)

    if(0 == devId || 0xff == devId)   //主机参数列表
    {
        if(MCP_ID_QUARY_ID_LIST == paraId)
        {
            pt = monitor_para_table_mu;
            num = PARA_TAB_SIZE_MU;
        }
        else    //自定义参数项列表
        {
            pt = NULL;
            num = 0;
        }
    }
    else if((devId > 230) && (devId <= 254))
    {
        if(MCP_ID_QUARY_ID_LIST == paraId)
        {
            pt = monitor_para_table_hub;
            num = PARA_TAB_SIZE_HUB;
        }
        else    //自定义参数项列表
        {
            pt = NULL;
            num = 0;
        }
    }
    else if(devId > 230)
    {
        if(MCP_ID_QUARY_ID_LIST == paraId)
        {
            pt = monitor_para_table_hub;
            num = PARA_TAB_SIZE_HUB;
        }
        else    //自定义参数项列表
        {
            pt = NULL;
            num = 0;
        }
    }
    else
    {
        if(MCP_ID_QUARY_ID_LIST == paraId)
        {
            pt = monitor_para_table_ru;
            num = PARA_TAB_SIZE_RU;
        }
        else    //自定义参数项列表
        {
            pt = NULL;
            num = 0;
        }
    }

    #elif (LOCAL_MODULE_TYPE == _RU)
    if(MCP_ID_QUARY_ID_LIST == paraId)
    {
        pt = monitor_para_table_ru;
        num = PARA_TAB_SIZE_RU;
    }
    else    //自定义参数项列表
    {
        pt = NULL;
        num = 0;
    }
    #endif

    ssPkgLen = vPduParaListPkg(src, pt, num);
    return(ssPkgLen);
}



/*****************************************************************************
 * @Name	  : get_MU_MCP_data_id_config
 * @Brief  : 获取监控标识符对应的数据配置,包括读写权限,数据类型,最大最小值
 * @Param  : data_id: 监控标识符
 *			mcp_msg: 指向收到的消息
 * @Retval : 返回错误码
 * @Author :
 * @Data   : 2024-3-1
 * @Note   :
 *****************************************************************************/
Status_t get_MU_MCP_data_id_config(uint16_t data_id, MCP_MSG_T * mcp_msg)
{
    uint32_t i = 0;
    for(i = 0; i < PARA_TAB_SIZE_MU; i++)
    {
        if(data_id == monitor_para_table_mu[i].data_id)
        {
            mcp_msg->mcp_data_type = monitor_para_table_mu[i].data_type;
            mcp_msg->mcp_data_rw = monitor_para_table_mu[i].rd_wr;
            mcp_msg->mcp_data_min = monitor_para_table_mu[i].min_val;
            mcp_msg->mcp_data_max = monitor_para_table_mu[i].max_val;
            return STAT_OK;
        }
    }
    return STAT_ERROR;
}




/*****************************************************************************
 * @Name	  : get_RU_MCP_data_id_config
 * @Brief  : 获取监控标识符对应的数据配置,包括读写权限,数据类型,最大最小值
 * @Param  : data_id: 监控标识符
 *			mcp_msg: 指向收到的消息
 * @Retval : 返回错误码
 * @Author :
 * @Data   : 2024-3-1
 * @Note   :
 *****************************************************************************/
Status_t get_RU_MCP_data_id_config(uint16_t data_id, MCP_MSG_T * mcp_msg)
{
    uint32_t i = 0;
    for(i = 0; i < PARA_TAB_SIZE_RU; i++)
    {
        if(data_id == monitor_para_table_ru[i].data_id)
        {
            mcp_msg->mcp_data_type = monitor_para_table_ru[i].data_type;
            mcp_msg->mcp_data_rw = monitor_para_table_ru[i].rd_wr;
            mcp_msg->mcp_data_min = monitor_para_table_ru[i].min_val;
            mcp_msg->mcp_data_max = monitor_para_table_ru[i].max_val;
            return STAT_OK;
        }
    }
    return STAT_ERROR;
}






