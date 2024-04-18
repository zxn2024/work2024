/*************************************** Copyright ******************************
 *
 * (C) Copyright 2022, Shaanxi Tianji Communication
 * All Rights Reserved
 *
 * FileName   : bsp_cat1.h
 * Version    : None
 * Author     : WangJun
 * Date       : 2022-08-16
 * Description: TODO IPV6还不支持
 *******************************************************************************/

#ifndef __BSP_CAT1_H_
#define __BSP_CAT1_H_

#include "main.h"
#include "MCU_config.h"
#include "FreeRTOS.h"
#include "event_groups.h"
#include "task.h"
#include "queue.h"
#include "os_event.h"
#include "hi_uart.h"

/*---------------------- 宏定义  ---------------------------------------*/

/*
 * @brief 启用硬件测试(连接测试IP,启用自动发包)
 */
#define CAT1_HW_TEST (0)
/*
 * @brief 连接类型定义
 */
#define UDP (1)
#define TCP (2)

/*
 * @brief IP类型定义
 */
#define IPV4 (1)
#define IPV6 (2)
#define IPV4V6 (3)

/*
 * @brief 休眠状态
 */
#define CAT1_QUIT_SLEEP (0)  // 退出休眠
#define CAT1_ENTER_SLEEP (1) // 进入休眠

/*
 * @brief 接收发送缓存长度
 */
#define CAT1_RX_DATA_LEN 1200
#define CAT1_TX_DATA_LEN 1200


/**
 *  @brief 链接类型有效性判断
 */
#define IS_SOCKET_TYPE(TYPE) ((TCP == (TYPE)) || (UDP == (TYPE)))

/**
 *  @brief 链接号有效性判断
 */
#define IS_SOCKETID(ID) (11 >= (ID))

/**
 *  @brief IP类型有效性判断
 */
#define IS_IP_TYPE(TYPE) ((IPV4 == (TYPE)) || (IPV6 == (TYPE)) || (IPV4V6 == (TYPE)))

/**
 *  @brief 链接号有效性判断
 */
typedef enum
{
    LINK0 = 0, //连接主监控平台(全功能)
    LINK1 = 1, //连接备监控平台
} LINK_ID;

#define MAX_SOCKET_LINK 2
/*
 * @brief CAT1 查询指令
 */
typedef enum
{
    CAT1_GET_SIM_ID,           // 查询SIM的ID
    CAT1_GET_SW_VER,           // 查询软件版本
    CAT1_GET_HW_VER,           // 查询硬件版本
    CAT1_GET_LINK0_SEVER_STATE, // 查询连接服务器0状态
    CAT1_GET_LINK1_SEVER_STATE, // 查询连接服务器1状态

    CAT1_SET_APN, // 设置cat1 APN

    CAT1_SET_LINK0_IP_TPYE, // 设置link0 ip类型 IPV4/IPV6
    CAT1_SET_LINK0_SOCKET_TPYE, // 设置link0 Socket类型 TCP/UDP
    CAT1_SET_LINK0_USE_PASSWORD, // 设置link0 是否使用账户密码
    CAT1_SET_LINK0_USERNAME, // 设置link0 账户
    CAT1_SET_LINK0_PASSWORD, // 设置link0 密码
    CAT1_SET_LINK0_IPV4_ADDR, // 设置link0 服务器ipv4 地址
    CAT1_SET_LINK0_IPV6_ADDR, // 设置link0 服务器ipv6 地址
    CAT1_SET_LINK0_PORT, // 设置link0 服务器PORT

    CAT1_SET_LINK1_IP_TPYE, // 设置link1 ip类型 IPV4/IPV6
    CAT1_SET_LINK1_SOCKET_TPYE, // 设置link1 Socket类型 TCP/UDP
    CAT1_SET_LINK1_USE_PASSWORD, // 设置link1 是否使用账户密码
    CAT1_SET_LINK1_USERNAME, // 设置link1 账户
    CAT1_SET_LINK1_PASSWORD, // 设置link1 密码
    CAT1_SET_LINK1_IPV4_ADDR, // 设置link1 服务器ipv4 地址
    CAT1_SET_LINK1_IPV6_ADDR, // 设置link1 服务器ipv6 地址
    CAT1_SET_LINK1_PORT, // 设置link1 服务器PORT

} eCAT1_API_TYPE;
/*
 * @brief CAT1 AT指令
 */
typedef enum
{
    CAT1_CMD_CHECK_AT,     // 检测AT指令
    CAT1_CMD_CLOSE_ECHO,   // 关闭回显
    CAT1_CMD_QUERY_CPIN,   // 查询SIM卡状态
    CAT1_CMD_SET_CREG,     // 设置CS业务是否已注册
    CAT1_CMD_QUERY_CREG,   // 查询CS业务是否已注册
    CAT1_CMD_QUERY_CGREG,  // 查询PS业务是否已注册
    CAT1_CMD_SET_APN,   // 配置PDP场景
    CAT1_CMD_SET_PDPACT,    // 激活PDP场景
    CAT1_CMD_QUARY_PDPACT,    // 激活PDP场景
    CAT1_CMD_SET_PDPDEACT,  // 反激活PDP场景
    CAT1_CMD_SOCKET_OPEN0,       // 打开链接0
    CAT1_CMD_SOCKET_CLOSE0,      // 关闭链接0
    CAT1_CMD_SOCKET_OPEN1,       // 打开链接1
    CAT1_CMD_SOCKET_CLOSE1,      // 关闭链接1
    CAT1_CMD_SOCKET_STAT0,      // 查询链接0状态
    CAT1_CMD_SOCKET_STAT1,      // 查询链接1状态
    CAT1_CMD_QUERY_SIM_ID, // 查询SIM的ID
    CAT1_CMD_QUERY_RTC,    // 查询RTC时间
    CAT1_CMD_QUERY_SW_VER, // 查询软件版本
    CAT1_CMD_QUERY_HW_VER, // 查询硬件版本
    CAT1_CMD_QUERY_QNWINFO,    // 查询网络信息
    MONITOR_CMD_QUERY_QENG,     //查询小区信息

    CAT1_CMD_END
} eCAT1_CMD_TYPE;

/*
 * @brief CAT1 AT指令返回值
 */
typedef enum
{
    RET_OK,
    RET_ERROR,
    RET_NO_RESPONSE,
    RET_CMD_ERROR,
} eRETURN;
/*
 * @brief CAT1 器件状态机
 */
typedef enum
{
    CAT1_INIT,         //Cat1上电复位
    CAT1_WAIT_STATUS_ON,     //等待Cat1 Status状态拉高
    CAT1_CONFIG,        //Cat1基础配置
    CAT1_PDP_CONFIG,    //Cat1PDP配置,PDP激活设置
    CAT1_PDP_ACTIVED,   //Cat1PDP已激活
    CAT1_ERROR_HANDLE,  //Cat1错误处理
    CAT1_ERROR_HANDLE_DELAY,   //错误处理后等待一段时间再复位
    CAT1_STAT_END,   
} eCat1_stat_machine;

/*
 * @brief CAT1 连接状态机
 */
typedef enum
{
    SOCKET_START,   // Socket配置
    SOCKET_CONNECT, // Socket进行连接
    SCOKET_CONNECTED,   //Socket已连接
    SCOKET_DISCONNECT,  //Socket进行断连
    SCOKET_DISCONNECTED,//Socket已断连
    SCOKET_STAT_END
} eSocket_stat_machine;

/*---------------------- 结构体定义  ---------------------------------------*/
typedef struct
{
    PORT_T cat1_dc_ctrl_port;    /*3.3v ctrl port*/
    GPIO_PIN_T cat1_dc_ctrl_pin; /*3.3v ctrl pin*/
    PORT_T cat1_stat_port;       /*3.3v ctrl port*/
    GPIO_PIN_T cat1_stat_pin;    /*3.3v ctrl pin*/
    PORT_T cat1_open_port;       /*3.3v ctrl port*/
    GPIO_PIN_T cat1_open_pin;    /*3.3v ctrl pin*/
    PORT_T cat1_reset_port;      /*3.3v ctrl port*/
    GPIO_PIN_T cat1_reset_pin;   /*3.3v ctrl pin*/
    uart_func_t *uart_func;      /*串口*/
} cat1_n706_hw_config_t;

/**
 * @brief 网管参数, FlashSave
 */
typedef struct
{
    eSocket_stat_machine socket_stat; //连接状态机

    uint8_t socketID;    // SocketID：0-1

    uint8_t ucLinkSeverState;    // 链接服务器状态
    
    uint8_t SocketSendErrCnt; // 参数配置延迟生效计数

} SocketStat_t;

//typedef struct
//{
//    char rx_data_buf[CAT1_RX_DATA_LEN];
//    uint16_t rx_len;
//    LINK_ID link_id;
//} cat1_rx_data_buf_t;

typedef struct
{
    char rx_data_buf_socket0[CAT1_RX_DATA_LEN];
    uint16_t rx_len_socket0;
	
	  char rx_data_buf_socket1[CAT1_RX_DATA_LEN];
    uint16_t rx_len_socket1;
} cat1_rx_data_buf_t;

typedef struct
{
    char buf[500];
    uint16_t rx_len;
} cat1_rx_AT_cmd_buf_t;

typedef struct
{
    int mcc;
    int mnc;
    int cellid;
    int pci;
    int earfcn;
    int freq_band_ind;
    int up_bandwitdh;
    int dw_bandwitdh;
    int tac;
    int rsrp;
    int rsrq;
    int rssi;
    int sinr;
    int srxlex;
} cat1_cell_info_t; //lte小区信息

typedef struct
{    
    cat1_n706_hw_config_t cat1_hw_config; // 硬件配置,串口,GPIO

    volatile BOOL_E lock;             // 互斥锁
    BOOL_E cat1_en;             // cat1使能开关 0:关闭 1:开启

    eCat1_stat_machine cat1_stat; //Cat1状态机
    cat1_cell_info_t cell_info;//lte小区信息

    ip_para_t socketConfig[MAX_SOCKET_LINK];       // socketConfig用来加载socket的配置,和socketStat相对应
    SocketStat_t socketStat[MAX_SOCKET_LINK];       // socketStat用来维护一个网络连接, 最大支持两个

    SIM_STATE_E ucSimState;  // SIM卡状态
    uint8_t ucaSimNum[32];   // SIM卡串号
    uint8_t ucaCatSwVer[32]; // 模组软件版本
    uint8_t ucaCatHwVer[32]; // 模组硬件版本

    uint8_t apn[20];      // APN
    uint16_t devicePort; // 设备自身端口号
    
    Time_T tRtcTime;     // 网络时间

    cat1_rx_data_buf_t rx_buf; //用来缓存收到的数据
    cat1_rx_AT_cmd_buf_t rx_cmd_buf; //用来缓存收到的AT指令回包

    uint16_t AT_cmd_no_response_cnt; // AT指令无回复错误计数,连续10次复位设备
} cat1_n706_T;


/*---------------------- 全局变量  ---------------------------------------*/
extern cat1_n706_T hCat1;   // CAT1结构体


/*---------------------- 函数接口  ---------------------------------------*/

/*****************************************************************************
* @Name	  : cat1_n706_register
* @Brief  : 注册cat1设备
* @Param  : hw_config cat1设备硬件资源分配,包括GPIO Uart
* @Retval : None
* @Note   :
*/
void cat1_n706_register(cat1_n706_hw_config_t *hw_config);
/*****************************************************************************
* @Name	  : vCat1InitEgpCreat
* @Brief  : 创建Cat1初始化事件组
* @Param  : None
* @Retval : None
* @Note   :
*/
void cat1_egp_creat(void);     
/*****************************************************************************
* @Name	  : cat1_in_timer_isr
* @Brief  : 创建定时器，在定时器任务中调用
* @Param  : None
* @Retval : None
* @Note   :
*/
void cat1_in_timer_isr(void);   

/*****************************************************************************
* @Name	  : cat1_pwron
* @Brief  : CAT1上电
* @Param  : None
* @Retval : None
* @Note   :
*/
void cat1_pwron(void);

/*****************************************************************************
 * @Name	  : cat1_on_off
 * @Brief  : cat1开关控制,用于测试模式,关闭后彻底停止cat1工作
 * @Param  : None
 * @Retval : None
 * @Author :
 * @Date   : 2022-08-16
 */
void cat1_on_off(uint8_t on_off);

/*********************************************************************
* 函 数 名：void cat1_run(void)
* 功能描述：cat1业务处理函数,用于初始化,创建和保持sock连接,cat1状态监控,错误处理
* 参数说明：无
* 函数输出：无
*/
void cat1_run(void);

/*****************************************************************************
* @Name	  : cat1_uart_rx_data_handle
* @Brief  : 所有cat1串口收包在这里统一分类,将收到的消息分为AT指令回包和推送消息并分别存储
            ,建议放在一个调度周期不高于50ms的任务中,不要和cat1 run在同一个任务
* @Retval : None
* @Author :
* @Date   :
* @Note   :
*/
void cat1_uart_rx_data_handle(void);

/**************************************************************************
* 函 数 名：uint8_t cat1_send_msg(uint8_t ucSocketId, uint8_t* send_buf, uint16_t usMsgLen)
* 功能描述：客户端消息发送,写入16进制数据
* @Param  : link_id :指定要发送的socket连接,LINK0 LINK1
* 参数说明：send_buf：待发送16进制数据
* 参数说明：usMsgLen: 待发送数据长度
* 函数输出：成功/失败
*/
eRETURN cat1_send_msg(LINK_ID link_id, uint8_t* send_buf, uint16_t usMsgLen);
/*****************************************************************************
* @Name	  : cat1_recv_msg
* @Brief  : 从Cat1读取收到的数据,格式为字符串
* @Param  : link_id :指定要读取的socket连接,LINK0 LINK1
* @Param  : rx_buf: 出参,用于返回接收的数据
* @Retval : 返回数据长度,0表示无数据
* @Note   :
*/
uint16_t cat1_recv_msg(LINK_ID link_id, uint8_t* rx_buf);
/*****************************************************************************
* @Name	  : cat1_reset
* @Brief  : 重启CAT1,在系统复位处调用
* @Param  : None
* @Retval : 
* @Note   : 
*/
void cat1_reset(void);     // 复位cat1
/*****************************************************************************
* @Name	  : cat1_read_params
* @Brief  : 读取监控所需的cat1信息
* @Param  : eType: 参考eCAT1_CMD_TYPE
* @Param  : rd_buf: 出参
* @Retval : 出参长度
* @Note   :
*/
uint8_t cat1_read_params(eCAT1_API_TYPE eType, uint8_t *rd_buf); 
/*****************************************************************************
* @Name	  : cat1_set_omc_params
* @Brief  : 设置cat1的omc参数
* @Param  : eType: 参考eCAT1_CMD_TYPE
* @Param  : buf: 写入的数据
* @Param  : len: 写入的长度
* @Retval : 设置结果
* @Note   :
*/
eRETURN cat1_set_omc_params(eCAT1_API_TYPE eType, uint8_t *buf, uint8_t len); 
/*****************************************************************************
* @Name	  : cat1_get_rtc_time
* @Brief  : 获取当前RTC时间
* @Retval : rtc时间
* @Note   :
*/
Time_T cat1_get_rtc_time(void);      
/*****************************************************************************
* @Name	  : vOmcParaInit
* @Brief  : OMC相关参数初始化
* @Param  : None
* @Retval : None
* @Author : 
* @Date   : 2022-08-16
* @Note   : 
*/
void cat1_omc_para_set(omc_t *omc_param); // flash参数读取放到flash配置读取功能里调用  
/*****************************************************************************
 * @Name	  : socket_disconnect
 * @Brief  : Socket断开连接
 * @Param  : id 0/1
 * @Retval : 连接状态
 * @Note   :  返回RET_ERROR/RET_OK
 */
eRETURN socket_disconnect(LINK_ID id); 
#endif
