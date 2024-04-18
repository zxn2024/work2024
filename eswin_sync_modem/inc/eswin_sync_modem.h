/**
 ******************************************************************************
 * @file    modem.h
 * @author
 * @version V1.0.0
 * @date    2023-10-14
 * @brief   This file provides code for the modem interface.
 ******************************************************************************
 */

#ifndef __ESWIN_SYNC_MODEM_H
#define __ESWIN_SYNC_MODEM_H

#include "hi_uart.h"
#include "MCU_config.h"

/*---------------------- 宏定义  ---------------------------------------*/
#define FREQ_BUF_SIZE (64)

typedef enum
{
    DEVICE_RUN_MODE = 0,   // 设备运行模式
    DEVICE_TEST_MODE = 1,  // 设备测试模式
    DEVICE_CALI_MODE = 2,  // 设备校准模式
} DEVICE_WORKMODE;

typedef enum
{
    CONFIG_IDLE= 0,     //未配置启动模块的状态
    CONFIG_CFUN0,       //设置最小功能模式
    CONFIG_FORMAT,       //设置网络支持
    CONFIG_UNLOCKBAND,   //解锁频带或者频点
    CONFIG_LOCKBAND,     //锁定频带或者频点
    CONFIG_CFUN1,        //设置全功能模式
    CONFIG_GETNETINF,    //获取搜网结果
    CONFIG_ERRHANDLE,
    CONFIG_FINE,       //配置完成，等待应用层处理搜网结果
    CONFIG_END,
} Modem_config_stat_E;

typedef enum
{
    MINIMUM_MODE = 0, // 最小功能模式,启动时在此模式,只支持配置读写
    RUN_MODE,         // 全功能模式,能够正常工作,但是进行配置前需要切换到MINIMUM_MODE
} Modem_mode_E;

typedef enum
{
    MODEM_INIT = 0, 		//模块初始态
    MODEM_IDLE, 			//模块空闲态
    MODEM_SCAN_LTE,		 	//模块初始态
    MODEM_SCAN_NR,			//模块LTE扫描态
    MODEM_SCAN_DONE,		//模块NR扫描态
    MODEM_LTE_ATTACHED,		//模块LTE驻网态
    MODEM_NR_ATTACHED,		//模NR驻网态
    MODEM_REBOOT,			//模块reboot态
    MODEM_RESET,			//模块复位态
    MODEM_END,
} Modem_stat_E;

#define CH_NR  (0)  // NR通道
#define CH_LTE (1) // LTE通道
#define CH_NR2 (2) // NR2通道

typedef enum
{
    GPIO_ALL_HI = 0, // 时序GPIO常高,即单通
    GPIO_ALL_LO = 1, // 时序GPIO常低,即单通
    GPIO_TDD = 2,    // 时序GPIO根据TDD时序输出
} gpio_sync_mode_E;

typedef enum
{
    SCAN_IDLE = 0,      // 扫描空闲态
    SCAN_START_LTE = 1, // 开始扫描LTE
    SCAN_DONE_LTE = 2,  // 结束扫描LTE
    SCAN_START_NR = 3,  // 开始扫描NR
    SCAN_DONE_NR = 4,   // 结束扫描NR
} SCAN_STAT_E;

typedef enum
{
    SYNC_NULL = 0,   // 未驻网
    SYNC_SYNC = 1,   // 驻网成功并输出时序
    SCAN_UNSYNC = 2, // 驻网并丟网
} SYNC_STAT_E;

typedef enum
{
    NR = 0,     // 单模NR
    LTE = 1,    // 单模LTE
    LTE_NR = 2, // 双模
    MODE_NULL = 3, // 无结果
} NET_MODE;

/*标志变量*/
typedef struct
{
    uint8_t flg_ecellgpio_lte : 1; // 小区跟踪指令LTE
    uint8_t flg_ecellgpio_nr : 1;  // 小区跟踪指令NR
} modem_command_reback_info_t;

/*
 * @brief CAT1 AT指令
 */
typedef enum
{
    MODEM_CMD_SET_REBOOT,        // 重启
    MODEM_CMD_QUERY_EGMR,        // 查询TA固件版本
    MODEM_CMD_QUERY_EGAPR,       // 查询AP版本
	MODEM_CMD_QUERY_EGHWR,       // 查询硬件版本
	MODEM_CMD_QUERY_ECUSR,       // 查询软件版本
    MODEM_CMD_SET_CFUN_0,        // 设置运行模式为最小功能模式,仅支持配置功能
    MODEM_CMD_SET_CFUN_1,        // 设置运行模式为全功能模式,开始搜寻网络
    MODEM_CMD_QUERY_CFUN,        // 查询运行模式
    MODEM_CMD_QUERY_BAND,        //查询BAND
    MODEM_CMD_QUERY_ERAT,        //查询网络制式
    MODEM_CMD_SET_ERAT,          // 设置网络制式 NR LTE NR+LTE
    MODEM_CMD_SET_LOCK_EBAND,    // 锁频带或频点
    MODEM_CMD_SET_UNLOCK_EBAND,  // 锁频带或频点
    MODEM_CMD_SET_ECELLGPIO_LTE, // 设置跟踪小区和GPIO组
    MODEM_CMD_SET_ECELLGPIO_NR,  // 设置跟踪小区和GPIO组
    MODEM_CMD_SET_EL1CFG,        // 设置物理层参数,时隙配比
    MODEM_CMD_SET_ETIMEOFFSET,   // 设置NR2通道的时间偏移量,并跟踪和输出同步信号
    MODEM_CMD_SET_EGPIOSIGNAL,   // 设置GPIO信号输出(配单通或TDD)
    MODEM_CMD_QUARY_ECELL,       // 查询小区信息
    MODEM_CMD_SET_EGPIOADJ,      // 设置GPIO信号调整量
    MODEM_CMD_END
} eMODEM_CMD_TYPE;


/*
 * @brief CAT1 版本升级帧类型
 */
enum {    
    TYPE_HANDSHAKE           = 0x1,    
    TYPE_DOWNLOAD_TO_FLASH   = 0x03,    
    TYPE_ERASE_BLOCK_NOALIGN = 0x06,    
    TYPE_CHANGE_BAUDRATE     = 0x09,    
    TYPE_REBOOT              = 0x16,
};
/*
 * @brief CAT1 版本升级帧头
 */
#pragma pack(1)
typedef struct {    
    uint8_t magic;   
    uint8_t type;    
    uint16_t length;
} pkt_head_t;
#pragma pack()
/*
 * @brief CAT1 版本升级处理结果
 */
#define BURN_OK       		 0
#define BURN_ERR      		-1
#define BURN_ERR_OPEN_UART  -2
#define BURN_ERR_CONNECT    -3
#define BURN_ERR_MAGIC		-4
#define BURN_ERR_ERASE		-5
#define BURN_ERR_WRITE		-6
#define BURN_ERR_NOBUF		-7
#define BURN_ERR_TIMEOUT    -8
/*
 * @brief CAT1 版本升级文件分区定义
 */
#define PARTITION_NAME_PARTITION   "partition"
#define PARTITION_NAME_UBOOT       "uboot"
/*
 * @brief CAT1 版本升级发送和接收magic
 */
#define PKT_MAGIC_SEMD   0xA5
#define PKT_MAGIC_RECV   0x5A

#define MODEM_FLASH_SECTOR_SIZE 4096

/*---------------------- 结构体 ------------------------------------------*/
/*modem驱动资源定义*/


// 同步猫工作配置信息
typedef struct
{
    uint8_t device_mode;              //设备工作模式
    uint8_t modem_network;            //同步模块网络制式
    uint8_t band[5];                  //同步模块频带  3,8,39,40,41
    NET_MODE band_net_mode_set[5];    //用于指示band[5]数组中band对应的网络制式,单一band必须是NR或LTE,不能设置成双模
    uint8_t band_invalid;           //band无效
    char freq_3[FREQ_BUF_SIZE];    //同步模块频带3对应的频点
    char freq_8[FREQ_BUF_SIZE];    //同步模块频带8对应的频点
    char freq_39[FREQ_BUF_SIZE];   //同步模块频带39对应的频点
    char freq_40[FREQ_BUF_SIZE];   //同步模块频带40对应的频点
    char freq_41[FREQ_BUF_SIZE];   //同步模块频带41对应的频点
    uint8_t startup;                //启动同步模块配置开关
    uint8_t flg_set_timing_offset;   //上下行切换点间隔设置的标志
	
    int  lte_ul_rising;      //LTE上行信号上升沿调整量
    int  lte_ul_falling;     //LTE上行信号下升沿调整量
    int  lte_dl_rising;      //LTE下行信号上升沿调整量
    int  lte_dl_falling;     //LTE下行信号下升沿调整量
    int  nr_ul_rising;       //NR上行信号上升沿调整量
    int  nr_ul_falling;      //NR上行信号下升沿调整量
    int  nr_dl_rising;       //NR下行信号上升沿调整量
    int  nr_dl_falling;      //NR下行信号下升沿调整量
	
} modem_config_buf_t;

typedef struct __modem_config_t
{
    PORT_T dc_ctrl_port;     /*3.3v ctrl port*/
    GPIO_PIN_T dc_ctrl_pin;  /*3.3v ctrl pin*/
    PORT_T sync_port;        /*同步状态port*/
    GPIO_PIN_T sync_pin;     /*同步状态pin*/
    uart_func_t *modem_uart; /*同步猫串口*/
    uint8_t lte_gpio_group;  /*lte时序映射到的同步猫gpio组*/
    uint8_t nr_gpio_group;   /*nr时序映射到的同步猫gpio组*/
    uint8_t nr2_gpio_group;  /*nr2时序,基于NR时序,用偏移量生成*/
} modem_config_t;

/*LTE实时网络状态信息*/
typedef struct __LTE_net_info_t
{
    SYNC_STAT_E LTE_Sync; // LTE同步状态
    uint32_t sync_time;  // 同步时间
	uint8_t lte_net_invalid_cnt;  // LTE 网络信息无效计数
    uint8_t band;         // band
    uint32_t freq;        // 频点
    uint32_t pci;         // 跟踪小区
    uint32_t bw;          // 带宽
    int32_t rsrp;         // rsrp
    int32_t sinr;         // sinr
    uint8_t TDD_or_FDD;   // LTE是TDD还是FDD
    uint8_t sfa;          // LTE上下行子帧配比,取值范围0，1，2，3，4，5，6，10
    uint8_t ssfp;         // LTE特殊子帧配比。整型。取值范围：0，1，2，3，4，5，6，7，8
} LTE_net_info_t;

/*NR实时网络状态信息*/
typedef struct __NR_net_info_t
{
    SYNC_STAT_E NR_Sync; // NR同步状态
    uint32_t sync_time;  // 同步时间
	uint8_t nr_net_invalid_cnt; // NR 网络信息无效计数
    uint8_t band;        // band
    uint32_t bw;         // 带宽
    uint32_t freq;       // 频点
    uint32_t pci;        // 小区
    int32_t scs;         // rsrp
    int32_t rsrp;        // rsrp
    int32_t sinr;        // sinr
} NR_net_info_t;

/*LTE搜网结果*/
typedef struct __LTE_scan_t
{
    uint8_t band;       // band
    uint32_t freq;      // 频点
    uint32_t pci;       // 跟踪小区
    uint32_t bw;        // 带宽
    int32_t rsrp;       // rsrp
    int32_t sinr;       // sinr
    uint8_t TDD_or_FDD; // LTE是TDD还是FDD
    uint8_t sfa;        // LTE上下行子帧配比,取值范围0，1，2，3，4，5，6，10
    uint8_t ssfp;       // LTE特殊子帧配比。整型。取值范围：0，1，2，3，4，5，6，7，8
} LTE_scan_result_t;

/*NR搜网结果*/
typedef struct __NR_scan_t
{
    uint8_t band;  // band
    uint32_t bw;   // 带宽
    uint32_t freq; // 频点
    uint32_t pci;  // 小区
    int32_t scs;   // rsrp
    int32_t rsrp;  // rsrp
    int32_t sinr;  // sinr
} NR_scan_result_t;

/*搜网结果*/
#define SCAN_TAB_DATA_LEN 50
typedef struct __net_scan_t
{
    SCAN_STAT_E Scan_stat;                             // 扫描状态,启动扫描后先扫LTE,再NR
    LTE_scan_result_t LTE_scan_tab[SCAN_TAB_DATA_LEN]; // LTE实时网络状态信息
    uint8_t LTE_scan_pci_num;
    NR_scan_result_t NR_scan_tab[SCAN_TAB_DATA_LEN]; // NR实时网络状态信息
    uint8_t NR_scan_pci_num;
} net_scan_result_t;

/*TDD时序输出配比*/
// 应用时需要根据需求让射频给配置
typedef struct __timing_config_t
{
    uint8_t num_of_ul_subframe_1;  // 时隙1的上行时隙数
    uint8_t num_of_dl_subframe_1;  // 时隙1的下行时隙数
    uint8_t num_of_ul_subframe_2;  // 时隙2的上行时隙数
    uint8_t num_of_dl_subframe_2;  // 时隙2的下行时隙数
    uint8_t num_of_ul_symbles_sp1; // 特殊子帧1的上行symble数
    uint8_t num_of_gp_symbles_sp1; // 特殊子帧1gp的symble数
    uint8_t num_of_dl_symbles_sp1; // 特殊子帧1的下行symble数
    uint8_t num_of_ul_symbles_sp2; // 特殊子帧2的上行symble数
    uint8_t num_of_gp_symbles_sp2; // 特殊子帧2gp的symble数
    uint8_t num_of_dl_symbles_sp2; // 特殊子帧2的下行symble数
} timing_config_t;
#define MODEM_ATCOM_BUF_SIZE (500)
typedef struct
{
    char rx_cmd_buf[MODEM_ATCOM_BUF_SIZE];
    uint16_t rx_len;
} modem_rx_AT_cmd_buf_t;

typedef struct __sync_modem_TypeDef // modem结构体
{

    modem_config_t hw_config; // 设备硬件资源

    Modem_stat_E modem_stat; // 5G猫工作流程状态机

    Modem_mode_E work_mode; // 当前工作模式,上电后为MINIMUM_MODE,切换到RUN_MODE才能工作
    uint8_t network;        // 支持网络制式 0-NR 1-LTE 2-NR&LTE

    net_scan_result_t scan_result; // 搜网结果

    LTE_net_info_t LTE_net_info; // LTE实时网络状态信息
    NR_net_info_t NR_net_info;   // NR实时网络状态信息

    uint32_t NR2_timing_offset;        // nr2时序相对NR时序的偏移
    timing_config_t NR_timing_config;  // NR TDD时序输出配比
    timing_config_t LTE_timing_config; // LTE TDD时序输出配比
    timing_config_t NR2_timing_config; // NR2 TDD时序输出配比

    // 将收到的同步猫消息分两类: AT指令回复和推送信息

    modem_rx_AT_cmd_buf_t Modem_AT_CMD_rx_buf; // AT指令回包缓冲区
    modem_rx_AT_cmd_buf_t Modem_push_msg_buf;  // 同步猫推送信息缓冲区

    uint8_t TAFirmVer[32]; // TA固件版本
    uint8_t APVer[32];     // AP版本
    uint8_t HWVer[32];     // AP版本
	uint8_t SWVer[32];     // AP版本

    uint8_t Modem_AT_cmd_no_response_cnt; // AT指令无回复错误计数,连续10次复位设备
    uint8_t AT_cmd_response_busy_cnt; // AT指令回复忙碌错误计数,连续1次复位设备
    uint8_t Modem_AT_cmd_error_response_cnt; // AT指令连续错误计数,连续10次复位设备
    uint16_t AT_cmd_reboot_cnt;       // 连续3次reboot设备，则断电重启

    modem_command_reback_info_t modem_command_reback_info; // AT指令回包信息

    Modem_config_stat_E  modem_config_state; // 5G猫配置状态机
    uint8_t modem_reboot_or_reset;   // 控制模块重启或复位的标志,0代表不重启或复位,1代表模块重启(备用) 2代表模块重新上电
    uint8_t flg_get_ver; //控制获取模块版本的标志,上电后,升级模块程序后获取一次
    BOOL_E power_on_mready; //模块上电到mready状态的标志
    BOOL_E init_done; //模块上电到mready状态的标志
} sync_modem_t;
/*---------------------- 全局变量 ------------------------------------------*/

extern  modem_config_buf_t   pc_config_modem_buf;   // MODEM参数信息

extern  sync_modem_t hModem; // PC配置同步模块参数

/*---------------------- 接口 ------------------------------------------*/
/***********************************************************
 *函数功能: 器件注册,结构体初始化
 *参数说明: p_modem_config  传入器件参数
 *返 回 值: STAT_OK
 */
void modem_register(modem_config_t *p_modem_config);

/*****************************************************************************
 * @Name	  : modem_init
 * @Brief  : MODEM上电初始化
 * @Param  : None
 * @Retval : None
 * @Author :
 * @Date   : 2022-08-16
 * @Note   :
 */
void modem_init(void);

/***********************************************************
 *函数功能: 网络制式设置
 *参数说明: 0-NR 1-LTE 2-NR&LTE
 *返 回 值: STAT_OK /STAT_ERROR
 */
Status_t modem_set_network_format(uint8_t network_format);

/***********************************************************
 *函数功能: 获取模块的频带band信息
 *返 回 值: STAT_OK /STAT_ERROR
 */
Status_t modem_get_band(void);

/***********************************************************
 *函数功能: 锁band
 *参数说明: network_channel  CH_NR 0 CH_LTE 1;band 频带eg:band=41 则锁定N41;arfcn频点
 *返 回 值: STAT_OK /STAT_ERROR
 */
Status_t modem_lock_band(uint8_t network_channel, uint8_t band, char *arfcn); // RF_BAND_E

/***********************************************************
 *函数功能: 解锁band
 *参数说明: network_channel  CH_NR 0 CH_LTE 1;band 频带eg:band=41 则解锁N41;
 *返 回 值: STAT_OK
 */
Status_t modem_unlock_band(uint8_t network_channel, uint8_t band);

/***********************************************************
 *函数功能: 停止驻网和输出,切换到最小功能模式,配置band(或更改其他配置)前需要先调用此函数,配置完成后重新扫描驻网.
 *返 回 值: STAT_OK /STAT_ERROR
 */
Status_t modem_stop(void); // CFUN=0

/***********************************************************
 *函数功能: 搜网,返回搜网结果,过程可能需要等待一段时间
 *参数说明: ptr_scan_result 出参,指针指向扫描结果
 *返 回 值: STAT_OK /STAT_ERROR
 */
Status_t modem_start_scan(void); // CFUN=1
Status_t modem_get_scan_result(net_scan_result_t *ptr_scan_result);
/***********************************************************
 *函数功能: 驻网lte小区,驻网成功后输出时序
 *参数说明: freq 频点
 *参数说明: pci 小区号
 *返 回 值: STAT_OK /STAT_ERROR
 */
Status_t modem_tracking_lte_cell(uint32_t freq, uint32_t pci, uint8_t band);

/***********************************************************
 *函数功能: 驻网NR小区,驻网成功后输出时序
 *参数说明: freq 频点
 *参数说明: pci 小区号
 *返 回 值: STAT_OK
 */
Status_t modem_tracking_NR_cell(uint32_t freq, uint32_t pci, uint8_t band);

/***********************************************************
 *函数功能: 时序GPIO输出模式设置,输出常高/常低/TDD时序
 *参数说明: gpio_group: gpio组号, 0-2 
 *参数说明: gpio_mode  HI 0 LO 1 TDD 2
 *返 回 值: STAT_OK /STAT_ERROR
 */
Status_t modem_set_channel_output_mode(uint8_t gpio_group, gpio_sync_mode_E gpio_mode_up, gpio_sync_mode_E gpio_mode_dw);

/***********************************************************
 *函数功能: 时序GPIO输出帧配比和特殊子帧结构配置
 *参数说明: gpio_group: gpio组号, 0-2 
 *参数说明: gpio_timing  设置子帧比例,特殊子帧比例  cfg_mode: 0表示单时隙配置，1表示双时隙配置
 *返 回 值: STAT_OK
 */
Status_t modem_set_gpio_timing_config(uint8_t gpio_group, timing_config_t gpio_timing, uint8_t cfg_mode);

/***********************************************************
 *函数功能: 设置输出时序偏移
 *参数说明: network_channel  CH_NR 0 CH_LTE 1 CH_NR2 2
 *参数说明: ul_rising  上行信号上升沿调整量，有符号整型。负值向左偏移，正值向右偏移。单位us
 *参数说明: ul_falling  上行信号下降沿调整量，有符号整型。负值向左偏移，正值向右偏移。单位us
 *参数说明: dl_rising  下行信号上升沿调整量，有符号整型。负值向左偏移，正值向右偏移。单位us
 *参数说明: dl_falling  下行信号下降沿调整量，有符号整型。负值向左偏移，正值向右偏移。单位us
 *返 回 值: STAT_OK /STAT_ERROR
 */
Status_t modem_set_gpio_timing_offset(uint8_t gpio_group, int ul_rising, int ul_falling, int dl_rising, int dl_falling);

/***********************************************************
 *函数功能: NR2时序是根据NR时序加指定偏移量生成的
 *参数说明: time_offset  NR2的时序偏移量
 *返 回 值: STAT_OK /STAT_ERROR
 */
Status_t modem_set_NR2_offset(uint32_t time_offset);

/***********************************************************
 *函数功能: 刷新网络信息
 *返 回 值: STAT_OK /STAT_ERROR
 */
Status_t modem_refresh_net_info(void);

/***********************************************************
 *函数功能: 获取网络信息,注意定时调用刷新modem_refresh_net_info以保证获取更新的状态
 *返 回 值: STAT_OK /STAT_ERROR
 */
Status_t modem_get_net_info(LTE_net_info_t *lte_info, NR_net_info_t *nr_info);

/***********************************************************
*函数功能: ECELL 查询小区LTE信息是否有效 band/pci/freq不为全0
 *返 回 值: STAT_OK / STAT_ERROR
 */

Status_t  is_valid_lte_net_info(void);

 /***********************************************************
*函数功能: 模块复位或者重上电 后 相关变量重置
 *返 回 值: 无
 */
void modem_reboot_or_reset_cleardata(void);

 /***********************************************************
*函数功能: ECELL 查询小区nr信息是否有效 band/pci/freq不为全0
 *返 回 值: STAT_OK / STAT_ERROR
 */

Status_t  is_valid_nr_net_info(void);

/***********************************************************
 *函数功能: 获取模块的状态
 *返 回 值: 模块状态机状态
    模块初始态 0,
    模块空闲态 1,
    模块LTE扫描态 2,
    模块NR扫描态 3,
    模块扫描结束态 4,
    模块LTE驻网态 5,
    模块NR驻网态 6,
    模块复位态 7,
 */
Modem_stat_E modem_get_state(void);

/*****************************************************************************
 * @Name	  : modem_reboot
 * @Brief  : 软复位重启MODEM,在系统复位处调用
 * @Param  : None
 * @Retval :
 * @Note   :
 */
Status_t modem_reboot(void);

/***********************************************************
 *函数功能: 获取模块的网络制式
 *返 回 值: STAT_OK /STAT_ERROR
 */
Status_t modem_get_erat(void);

/***********************************************************
 *函数功能: 获取模块的运行模式
 *返 回 值: STAT_OK /STAT_ERROR
 */
Status_t modem_get_fun(void);
/*****************************************************************************
* @Name	  : modem_uart_rx_data_handle
* @Brief  : 所有modem串口收包在这里统一分类,将收到的消息分为AT指令回包和推送消息并分别存储
            ,建议放在一个调度周期不高于50ms的任务中,不要和modem run在同一个任务
* @Retval : None
* @Author :
* @Date   :
* @Note   :
*/
void modem_uart_rx_data_handle(void);
/*****************************************************************************
* @Name	  : modem_in_timer_isr
* @Brief  : 创建定时器，在定时器任务中调用
* @Param  : None
* @Retval : None
* @Note   :
*/
void modem_in_timer_isr(void);
/*****************************************************************************
 * @Name	 : vModemEgpCreat
 * @Brief  : 创建Cat1初始化事件组
 * @Param  : None
 * @Retval : None
 * @Note   :
 */
void vModemEgpCreat(void);
/*****************************************************************************
 * @Name	 : ModemConfig
 * @Brief  : 模块配置流程
 * @Param  : None
 * @Retval : None
 * @Note   :
 */
void ModemConfig(void);
/*---------------------- 3521业务接口 ------------------------------------------*/

/***********************************************************
 *函数功能: 设备网络状态监控,错误处理,更新网络参数和同步状态,处理模组推送的状态信息
 *返 回 值: STAT_OK /STAT_ERROR
 */
void modem_run(void);

/***********************************************************
*函数功能: 同步模块配置状态机的状态控制，由上层应用通过pc_config_modem_buf.startup触发
********************************************************/
void modemconfig_state_control(void);

/***********************************************************
*函数功能: 获取并判断模块当前状态是否为扫描结束状态
*返 回 值: B_TRUE /B_FALSE
********************************************************/
BOOL_E rf_5g_modem_scan_done(void);


/*模块版本升级接收ACK*/
int modem_upgrade_recv_ack(uint8_t pkt_type, uint8_t *ack);
/*模块版本升级发送REQ*/
void modem_upgrade_send_req(uint8_t pkt_type, uint8_t *buf_head, uint16_t buf_head_len, uint8_t *buf, uint16_t buf_len);
/*模块版本升级开始*/
void modem_upgrade_start(void);
/*模块版本升级结束*/
void modem_upgrade_end(void);

void vModemPwrOn(void);
/***********************************************************
 *函数功能:  获取模组上电信息
 *返 回 值: B_TRUE 已上电 B_FALSE 未上电
 */
BOOL_E modem_get_pwron_stat(void);
#endif /* modem_SPI_H */
