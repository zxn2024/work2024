/*************************************** Copyright ******************************
*
* (C) Copyright 2023, Shaanxi Tianji Communication
* All Rights Reserved
*                         
* FileName   : bsp_public_struct.h   
* Version    : none		
* Author     : ZhangWei			
* Date       : 2023-02-14         
* Description: none   
*******************************************************************************/
//
#ifndef __BSP_PUBLIC_STRUCT_H_
#define __BSP_PUBLIC_STRUCT_H_

#include <stdint.h>
//#include <bsp_public_enum.h>
#include "bsp_dev_config.h"

/******************************宏定义******************************/
#define NR_DL_ZIJI_CNT_NUM      ( 6 )      // 自激被动检测确认次数


/******************************结构体******************************/



// ADC采样结构体
// 与射频通道参数相关的ADC采集值
typedef struct
{
    uint16_t usAdcMax;          // ADC最大值
    uint16_t usAdcMin;          // ADC最小值
    uint16_t usAdcAve;          // ADC平均值
} AdcValue_T;

typedef struct 
{
    int8_t max;
    int8_t min;
    int8_t ave;
} PowValue_T;

// 不分区通道公用的ADC数据
typedef struct
{
    uint16_t usAcValue;         // 掉电检测ADC采集值
    uint16_t usTempValue;       // 温度检测ADC采集值
} AdcPublicValue_T;


// 衰减器结构体
typedef struct
{
    //常量
    uint8_t constAttNum;       	//衰减器数量，理论值

	uint8_t set_value1;			//待设置的衰减器数量1
	uint8_t set_value2;			//待设置的衰减器数量2
	
    //实时数据
    uint8_t ucIso;              //隔离度衰减
	uint8_t ucRsrpAtt;			//根据RSRP计算的衰减值
	uint8_t uc8668Att;			//8668功率检测衰减值
    uint8_t ucAlc;              //ALC衰减
    uint8_t ucManuCal;          //手动调整增益偏执( > 0 )
    uint8_t ucUpDownLink;       //上下行联动衰减
    uint8_t ucMasterSlaveLink;  //远近端联动衰减
    int8_t  scCaliGainOffset;   //较准增益偏移量  (-3 ~ 3 )
    int8_t  scTempOffset;       //温度补偿衰减偏移量 (-3 ~ 3 )
    uint8_t ucPresent;          //预置衰减器
} Att_T, *ptAtt_T;


// 功率表相关
typedef struct
{
    uint16_t *ptList;            //功率表指针
    uint8_t  ucTabNum;           //功率表元素数量
    int8_t   scMinValue;         //功率表中最小功率值
    int8_t   scCaliOffset;       //功率较准偏移量，±3有效
    int8_t   scTempOffset;       //功率温补偏移量，±3有效
} Powlist_T;

// 功率结构体
// 说明: 1. 查表计算输出功率时, 若ADC值为第n个元素区间,则功率计算如下:
// sOutpow =  n + scPowTabMinValue + scPowTabOffset + scPowTabTempOffset;
//       2. 若有功率温度补偿, 则应实时计算ALC的上下限功率值.
typedef struct
{
    int8_t  scTheoOutPow;       // 理论输出功率 = ALC启控点
    int8_t  scOutPow;           // 实际输出功率
    int8_t  scInPow;            // 实际输入功率
    int8_t  scRsrpOuptPow;      // Rsrp计算得到的输出功率
    int8_t  scRsrpInPow;        // Rsrp计算得到的输入功率
} Pow_T;


// 增益相关
typedef struct
{
    int8_t scTheoreticalGain;   // 理论增益
    int8_t scRealGain;          // 实际增益       
} Gain_T;

// ALC参数
typedef struct
{
    int8_t  scAlcTheoreticalPoint;  // 理论启控点 = 理论输出功率
    int8_t  scOplcPowPoint;         // 开环工控启控点
    int8_t  scOplcOffset;           // 开环工控偏移量
    int8_t  scAlcUpperLimit;        // 起控点上限
    int8_t  scAlcLowerLimit;        // 起控点下限
} Alc_T;

typedef struct
{
    uint32_t UpperCnt;          // 高计数
    uint32_t LowerCnt;          // 低计数
    uint32_t StartTick;         // 开始时间
    uint32_t PreTick;           // 执行时间
    uint32_t ZijiStatMachine;   // 自激状态机值
    uint32_t ZijiDectPeriod;    // 自激检测时间
    uint8_t  ZijiAttVal[NR_DL_ZIJI_CNT_NUM];     // 自激衰减记录缓存
    uint8_t  LowerAttVal[NR_DL_ZIJI_CNT_NUM];    // 低衰减记录缓存
    uint8_t  ZijiAttValMax;      // 自激最大衰减
    uint8_t  ZijiAttValMin;      // 自激最小衰减
    uint8_t  preSetAttVal;
    int8_t   attChange;
    uint8_t  iso;                //隔离度值
} Iso_T;


// 路损参数
typedef struct
{
    uint8_t ucWithRsrp;     // 通过RSRP计算的路损值
    uint8_t ucWithAdc;      // 通过输出功率计算的路损值
} PathLoss_T;

//网络配置
typedef struct
{   
    uint8_t   frame;          //配置的帧结构配比
    uint8_t   specialTime;    //配置的时隙配比
    uint16_t  timeOffset;     //配置的帧头偏移量
    uint32_t  plmn;           //配置的PLMN
    uint16_t  band;           //BAND设置值
    int8_t    firstTransitionPointOffset; //第一转换点    1
    uint8_t   firstProtectTime;           //第一转换点保护间隔    1
    int8_t    secondTransitionPointOffset;//第二转换点    1
    uint8_t   secondProtectTime;          //第二转换点保护间隔    1
    uint8_t   reserve[14];
} NetConfig_T;

//网络参数
typedef struct
{   
    uint8_t   sync;          //同步状态
    int16_t   rssi;          //rssi
    int16_t   rsrp;          //RSRP
    int16_t   sinr;          //SINR
    uint16_t  channel;       //CHANNEL
    uint16_t  cellid;
    uint32_t  frqPoint;      //频点
	uint8_t   bandWidth;	 //带宽
	uint8_t   subBw;		 //子载波带宽
    NetConfig_T tReadConfig;  
} NetPara_T;


typedef struct
{
	uint8_t  tcp_upd_sel;     	//连接类型 1:UDP, 2:TCP
	uint8_t  ipv4v6_sel;        //IP类型 1:IPv4, 2:IPv6, 3:IPv4v6
	uint8_t  ipv4_addr[4];	    //IPV4地址
	uint8_t  ipv6_addr[48];	 	//IPV6地址
	uint16_t port;			 	//端口号
	uint8_t  use_password_en;	//使用用户名密码使能
	uint8_t  username[20];   	//用户名
    uint8_t  password[20];   	//密码
}ip_para_t;	//97字节


/**
 * @brief 网管参数, FlashSave
 */
typedef struct
{
    uint32_t  bs_id;           //站点编号
    uint8_t   dev_id;          //设备编号
	ip_para_t ip[2];		   //IPV4参数	
    uint8_t   apn[20];         //APN
    uint16_t  heart;           //心跳包间隔
    uint8_t   longitude[20];   //经度
    uint8_t   latitude[20];    //纬度
    uint8_t   address[64];     //位置信息
    uint16_t  dev_port;        //设备自身端口号
    uint8_t   reserve[57];     //预留参数
} omc_t; //384

/**
 * @brief 设备参数, FlashSave
 */
typedef struct
{
    uint8_t factoryId;      //厂商代码
    uint8_t devType;        //设备类型
    uint8_t devMode[20];    //设备型号
    uint8_t swVer[20];      //软件版本
    uint8_t hwVer[20];      //硬件版本
    uint8_t mcuType[20];    //MCU型号
    uint8_t sn[30];         //sn号
    uint8_t license[30];    //授权码
    uint8_t reserve[14];    //预留参数
} Device_T;


//告警使能/门限
typedef struct
{
    uint8_t      en;         //告警使能, 1
    int8_t       gate;       //门限 
} WarnEnGate_T;

//告警参数
typedef struct
{
    uint8_t      invalid;      //有效性, TRUE: 有效, B_FALSE: 无效, 设备初始化时完成
    WarnEnGate_T *ptEnGate;
    uint16_t     time;         //持续时间, 时间单位s, 所有的功率告警均需要持续
    uint16_t     maxTime;      //最大持续时间, 单位s
    WARN_STATE_E nowState;     //当前状态
    WARN_STATE_E oldState;     //历史状态
    uint8_t      warnReport;   //告警上报标识
} Warn_T;


// 射频开关状态
typedef struct
{
    uint8_t ucNormalState;     //射频开关状态, 0:关闭, 1:开启
    uint8_t ucTestState;       //单通状态, 仅在测试模式下有效, 2: 上行单通, 3:下行单通
} RfSwitch_T;


//从机数量
typedef struct
{
    uint8_t ucMaxNum;      //支持的最大数量
    uint8_t ucTheoNum;     //设备里设置的理论从机数量数量, <=ucMaxNum
    uint8_t ucOnlineNum;   //实际的在线数量
} SlaveDevNum_T;


// 设备相关的公共参数
// 不分区通道公用的设备参数
typedef struct
{
    uint8_t       ucWorkMode;		    //工作模式: 0:正常模式, 1:单机模式, 如让主从设备的主机工作在单机状态, 此参数需结合设备类型应用
    uint8_t       devRunState;         	//运行状态, 开机 / 正常 / 测试模式
    uint8_t       ucAcVol;            	//转换后的电压值
    int8_t        scTemp;             	//转换后的温度值
    int8_t        scTempOffset;       	//温度校准值偏移量
    uint8_t       ucWarnEn;           	//告警允许使能，1：允许  0：禁止
    uint8_t       ucLedSwitch;        	//指示灯开关, OPEN    
	uint8_t       ucLedPlan;			//指示灯显示方案选择, 有效值 1 ~ 8
    uint8_t       ucRstByRunTime;     	//根据运行时长设置重启时长间隔, 0~255
    uint8_t       ucRstByNetTime;     	//根据网络时间设置重启时间, 0~24
	uint8_t  	  net_priority;			//搜网优先级
    uint8_t       ucCommFrePoint;     	//通信频点
    SlaveDevNum_T tSlaveDevNum; 		//从机数量
} DevPublicPara_T;


// 节能参数
typedef struct
{
    uint8_t  en;            //节能开关
    uint16_t startTime;     //节能起始时间
    uint16_t overTime;      //节能结束时间
    uint8_t  powerGate;     //节能功率门限
    uint8_t  durationTime;  //满足节能功率门限的持续时间
    uint8_t  state;         //当前节能状态 
    uint32_t timeLen;       //节能累计时长, 单位S    
    uint8_t  reserve[4];    //预留   
} PowSave_T;


// 时间参数
typedef struct
{
    uint8_t  year;          //年
    uint8_t  month;         //月
    uint8_t  day;           //日
    uint8_t  hour;          //时
    uint8_t  minute;        //分
    uint8_t  second;        //秒
} Time_T;

//射频相关的不区分上下行的公共参数
typedef struct 
{   
	uint8_t     exist;			//硬件是否存在, TRUE: 存在, B_FALSE: 不存在, 不存在的时候, 不需要操作任何硬件动作
	uint8_t  	SysMode;        //系统制式, SYS_TDD: TDD   SYS_FDD: FDD
	uint8_t  	enable;			//是否有效, ENABLE: 有效, DISABLE: 无效, 结合exist状态判断电源开关和是否进行操作.
	uint8_t     valid_band;		//有效的频段
	uint8_t     valid_bw;		//有效的带宽
	uint8_t  	sync;			//同步状态, 若TDD, 则需检测此状态, 若FDD, 默认设置为SYNC
    uint8_t     openState;      //开机状态, TRUE: 已完成开机, B_FALSE: 正在开机过程中
    uint8_t  	sysRunState;    //设备运行状态, 用于异常关断, OPEN: 正常, CLOSE:关闭
    uint8_t  	opleEn;         //开环功控使能开关, 1：允许  0：禁止
    uint8_t  	rsrpEn;         //RSRP计算开关使能, 1: 允许, 0: 禁止
    uint8_t  	tcEn;           //温度补偿使能
	uint8_t     warnEn;			//告警使能
	uint8_t     silenceEn;		//上行静默使能
    int8_t  	upExpectRxPow;  //上行期望接收功率, 用于系统工控
	int8_t      tmpOffset;		//温度偏移量
	RfSwitch_T  tRfSwitch;      //射频开关状态, 正常/关闭
	NetPara_T   tNet;			//网络参数
}RfSysPublic_T;

//射频相关的不区分上下行的 通道独立参数
typedef struct
{
	uint8_t    ucAntSwitch;	    // 天线开关状态
    Powlist_T  tPowList;   		// 功率表相关
    Pow_T      tPow;       		// 功率数据
    Gain_T     tGain;      		// 增益相关
    Att_T      tAtt;       		// 衰减器
    RfSwitch_T tSwitch;    		// 射频开关状态
    Alc_T      tAlc;       		// ALC相关参数
//	Alc_T      tAlcFor8668;		// 使用的ALC
    PathLoss_T tPathLoss;  		// 路损参数    
    Iso_T      tIso;       		// 隔离度参数
} RfChPara_T;


// 消息队列结构体
typedef struct
{
    uint8_t ucId;        // 设备ID号, 最大数量不超过SLAVE_MAX_NUM定义
    uint8_t ucSys;       // 系统选择, 参照RF_SYS_E, (0~END)
    uint8_t ucCh;        // 通道号, 参考RF_CH_E, (0,END)
    uint8_t ucCmdType;   // 命令类型 查询为2 设置为3, 参照CMD_TYPE_E, (2~3)
    uint8_t ucCmd;       // 命令
} McuSendQueryCmd_T;


typedef enum
{
    RUN_MODE_NORMAL = 0,   //正常运行
    RUN_MODE_START,        //初始状态
    RUN_MODE_TEST_INIT,    //进入测试模式前的初始化
    RUN_MODE_TEST,         //测试模式
    RUN_MODE_END
}RunMode_E;

#endif





