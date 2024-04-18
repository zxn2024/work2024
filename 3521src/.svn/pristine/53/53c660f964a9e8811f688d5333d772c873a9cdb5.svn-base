/***************************** Copyright ****************************************
*
* (C) Copyright 2022, Shaanxi Tianji Communication
* All Rights Reserved
*
* FileName   : bsp_reboot.c
* Version    : None
* Author     :
* Date       : 2023-10-08
* Description: None
*******************************************************************************/

#include "FreeRTOS.h"
#include "event_groups.h"
#include "rtt.h"
#include "hi_systick.h"
#include "bsp_reboot.h"

#include "mu_ru_communicate.h"
#include "hi_system.h"
#include "ble.h"

#if( LOCAL_MODULE_TYPE == _MU )
#include "bsp_rf_5g_modem.h"
#if (PRODUCT_MODEL == CMDT3521A)
    #include "cat1_EC800G_driver.h"
#elif (PRODUCT_MODEL == CMDT3521B)
    #include "cat1_N706_driver.h"
#elif (PRODUCT_MODEL == CMDT3521C)
    #include "cat1_MC661_driver.h"
#endif
#endif

/*---------------------- 端口定义 --------------------------------------*/


/*---------------------- 宏定义  ---------------------------------------*/
#define BIT_RESET_GET_PARA    (1U << 0)   // 获取参数, 5s获取一次参数
#define BIT_RESET_SYSTEM      (1U << 1)   // 系统重启
#define BIT_RESET_MASTER	  (1U << 2)	  // 重启主机
#define BIT_RESET_SLAVE		  (1U << 3)	  // 重启指定编号从机
#define BIT_RESET_5GMODEM_S   (1U << 4)   // 5G模组软件重启
#define BIT_RESET_5GMODEM_H   (1U << 5)   // 5G模组硬件重启
#define BIT_RESET_MONITOR     (1U << 6)   // CAT1重启
#define BIT_RESET_BLEM        (1U << 7)   // BLEM重启
#define BIT_RESET_BLES        (1U << 8)   // BLES重启
#define BIT_RESET_FPGA        (1U << 9)   // FPGA重启
#define BIT_RESET_BRD         (1U << 22)  // 向远端广播重启信息

#define BIT_RESET_ALL           ( 0x00FFFFFF )

#define __DEBUG_RST_PRINTF__    (0) //输出复位参数

/*---------------------- 结构体 ----------------------------------------*/
typedef struct
{
    uint8_t  theo_ucRstByRunTime;  //根据运行时长设置重启时长
    uint8_t  theo_ucRstByNetTime;  //根据网络时间设置重启时间
    uint32_t run_time;             //当前运行时间
    Time_T  * tRtcTime;            //网络时间
} ReBootPara_T;

/*---------------------- 常量 ------------------------------------------*/


/*---------------------- 全局变量 --------------------------------------*/
/**
  * @brief 重启事件组
  *         bit0  获取参数, 5s获取一次参数
  *         bit1  系统重启
  *			bit2  重启主机
  *			bit3  重启指定编号从机
  *         bit4  HanRu模组软件重启
  *         bit5  HanRu模组硬件重启
  *         bit6  CAT1重启
  *         bit7  BLEM重启
  *         bit8  BLES重启
  *         bit9  FPGA重启
  *			bit22 向远端广播重启信息
  */
EventGroupHandle_t reboot_egp = NULL;


/*---------------------- 局部变量 --------------------------------------*/
ReBootPara_T t_reboot_para;

static uint8_t reboot_dev_id = 0;	//待重启的从机编号缓存
/*---------------------- 声明 ------------------------------------------*/



/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ HAL ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/



/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DRV ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*****************************************************************************
* @Name	  : save_reboot_dev_id
* @Brief  : 保存待重启的设备ID
* @Param  : id: [输入/出]
* @Retval :
* @Author :
* @Data   : 2023-10-08
* @Note   :
*****************************************************************************/
void save_reboot_dev_id(uint8_t id)
{
    reboot_dev_id = id;
}

/*****************************************************************************
* @Name	  : clr_reboot_dev_id
* @Brief  : 清除待重启的设备ID
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2023-10-08
* @Note   :
*****************************************************************************/
void clr_reboot_dev_id(void)
{
    reboot_dev_id = 0;
}

/*****************************************************************************
* @Name	  : reboot_by_rtc
* @Brief  : 通过网络时间重启设备
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2023-10-18
* @Note   :
*****************************************************************************/
static void reboot_by_rtc(void)
{
#define CONST_PRE_CNT	S_TO_TICKS( 10 )    //发送重启广播时间提前量, 此时间不得低于5S

    uint8_t theo_first_hour;
    static uint8_t last_hour = 0xFF;

    ReBootPara_T * pt = &t_reboot_para;

    if((pt->theo_ucRstByNetTime > 23) || (pt->tRtcTime->year < 22) || (pt->tRtcTime->year <= 22))	//时间或者年份不正确, 直接退出
        return;

    if(last_hour >= 24)     //第一次获取到正确时间, 初始化last_hour
        last_hour = pt->tRtcTime->hour;

    theo_first_hour  = (pt->theo_ucRstByNetTime == 0) ? 23 : (pt->theo_ucRstByNetTime - 1);

    if((theo_first_hour == last_hour) && (pt->tRtcTime->hour == pt->theo_ucRstByNetTime))
    {
        pt->run_time = (pt->theo_ucRstByRunTime * 60 * 60 * 1000 / TIMER_PERIOD_TICKS - CONST_PRE_CNT);
    }

    last_hour = pt->tRtcTime->hour;

#if __DEBUG_RST_PRINTF__
    rtt_printf(RTT_DEBUG, "[ sys rtc time ]: rst_hour = %d, rtc_time = %d, last_hour = %d, run_time = %d \n", pt->theo_ucRstByNetTime, pt->tRtcTime->hour, last_hour, pt->run_time);
#endif
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ OS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*****************************************************************************
* @Name	  : reboot_egp_creat
* @Brief  : 创建重启时间事件组
* @Param  : None
* @Retval : None
* @Author :
* @Data   : 2023-10-08
* @Note   :
*****************************************************************************/
void reboot_egp_creat(void)
{
    reboot_egp = xEventGroupCreate();

    if(reboot_egp == NULL)
    {
        /* 任务创建失败 */
        rtt_printf(RTT_OS, RTT_TEXT_RED"ERROR: reboot_egp Create Fail...\r\n");
        return;
    }

    xEventGroupClearBits(reboot_egp, BIT_RESET_ALL);    // 清零
}


/*****************************************************************************
* @Name	  : reboot_in_time_isr
* @Brief  : 计算运行时间， 根据运行时间设置系统重启事件组。
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2022-10-17
* @Note   : 重启前15S钟位置发送重启广播, 之后到了预定时间重启
*****************************************************************************/
void reboot_in_time_isr(void)
{
#define CONST_BRD_DELAY_CNT   S_TO_TICKS( 15 )
#define CONST_GET_PARA_CNT    S_TO_TICKS( 10 )

    BaseType_t xResult = pdFALSE;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    static uint32_t ulGetParaCnt = 0;

    //获取参数时间间隔计时
    if(++ulGetParaCnt >= CONST_GET_PARA_CNT)
    {
        ulGetParaCnt = 0;
        xResult = xEventGroupSetBitsFromISR(reboot_egp, BIT_RESET_GET_PARA, &xHigherPriorityTaskWoken);
    }

    //系统运行时间计时器
    if(t_reboot_para.theo_ucRstByRunTime < 25)
        t_reboot_para.theo_ucRstByRunTime = 25;

    uint32_t rst_cnt = (uint32_t)(t_reboot_para.theo_ucRstByRunTime * 60 * 60 * 1000 / TIMER_PERIOD_TICKS);

    t_reboot_para.run_time++;

    #if __DEBUG_RST_PRINTF__
    rtt_printf(RTT_DEBUG, "Time = %d\r\n", t_reboot_para.run_time);
    #endif
    if(t_reboot_para.run_time == rst_cnt - CONST_BRD_DELAY_CNT)
    {
        #if __DEBUG_RST_PRINTF__
        rtt_printf(12, LOG_TEXT_RED">>>[ Warn-1 ]: Time = %d, SysRst Brd Slave Rst Msg!\r\n", t_reboot_para.run_time);
        #endif

        xResult = xEventGroupSetBitsFromISR(reboot_egp, BIT_RESET_BRD, &xHigherPriorityTaskWoken);
    }

    if(t_reboot_para.run_time == rst_cnt)
    {
        #if __DEBUG_RST_PRINTF__
        rtt_printf(12, LOG_TEXT_RED">>>[ Warn-2 ]: Time = %d, SysRst....!\r\n", t_reboot_para.run_time);
        #endif

        xResult = xEventGroupSetBitsFromISR(reboot_egp, BIT_RESET_SYSTEM, &xHigherPriorityTaskWoken);
    }

    if(xResult != pdFALSE)
    {
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ APP ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*****************************************************************************
* @Name	  : reboot_get_sys_para
* @Brief  : 获取系统设置的重启时间间隔和有SIM卡时的重启时间
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2023-10-08
* @Note   :
*****************************************************************************/
static void reboot_get_sys_para(void)
{
//    t_reboot_para.theo_ucRstByRunTime = tDevPublicPara.ucRstByRunTime;
//    t_reboot_para.theo_ucRstByNetTime = tDevPublicPara.ucRstByNetTime;
//	  t_reboot_para.tRtcTime = &t_gps_time;
}


/*****************************************************************************
* @Name	  : reboot_brd
* @Brief  : 向从机广播重启信息, 15s后重启
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2022-10-17
* @Note   :
*****************************************************************************/
static void reboot_brd(void)
{
//    if((LOCAL_MODULE_TYPE == MODULE_TYPE_MU) && (LOCAL_DEVICE_TYPE == DEVICE_TYPE_DT)) //主机 且 非单机设备, 则需要广播远端重启
//    {
//        //vBleSoftwareRstAll();   //向蓝牙发送广播
//    }
}


/*****************************************************************************
* @Name	  : reboot_set_egp_bit
* @Brief  : 重启事件组设置
* @Param  : type: [输入/出]
* @Retval : None
* @Author :
* @Data   : 2022-08-23
* @Note   :
*****************************************************************************/
void reboot_set_egp_bit(SystemRst_E type)
{
    switch(type)
    {
        case RESET_SYSTEM:// 系统重启
            xEventGroupSetBits(reboot_egp, BIT_RESET_SYSTEM);
            break;

        case RESET_MASTER:	//重启主机
            xEventGroupSetBits(reboot_egp, BIT_RESET_MASTER);
            break;

        case RESET_SLAVE:	//重启从机
            xEventGroupSetBits(reboot_egp, BIT_RESET_SLAVE);
            break;

        case RESET_5GMODEM_SW:// 5G模组软件重启
            xEventGroupSetBits(reboot_egp, BIT_RESET_5GMODEM_S);
            break;

        case RESET_5GMODEM_HW:// 5G模组硬件重启
            xEventGroupSetBits(reboot_egp, BIT_RESET_5GMODEM_H);
            break;

        case RESET_CAT1:// CAT1重启
            xEventGroupSetBits(reboot_egp, BIT_RESET_MONITOR);
            break;

        case RESET_BLEM:// BLEM重启
            xEventGroupSetBits(reboot_egp, BIT_RESET_BLEM);
            break;

        case RESET_BLES:// BLES重启[指定从机]
            xEventGroupSetBits(reboot_egp, BIT_RESET_BLES);
            break;

        case RESET_FPGA:// FPGA重启
            xEventGroupSetBits(reboot_egp, BIT_RESET_FPGA);
            break;

        case RESET_BRD: //广播重启指令[所有从机]
            xEventGroupSetBits(reboot_egp, BIT_RESET_BRD);
            break;

        default:
            break;
    }
}

/*****************************************************************************
* @Name	  : func_get_para
* @Brief  : 获取参数
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2023-03-14
* @Note   :
*****************************************************************************/
static void func_get_para(void)
{
    EventBits_t bitsEvent = xEventGroupGetBits(reboot_egp);

    if(BIT_RESET_GET_PARA == (bitsEvent & BIT_RESET_GET_PARA)) //获取系统参数
    {
        reboot_get_sys_para();
        reboot_by_rtc();
        xEventGroupClearBits(reboot_egp, BIT_RESET_GET_PARA);
    }
}

///*****************************************************************************
//* @Name	  : func_sys_rst
//* @Brief  : 系统重启函数
//* @Param  : bitsClearEvent: [输入/出]
//* @Retval :
//* @Author :
//* @Data   : 2023-03-14
//* @Note   :
//*****************************************************************************/
//static void func_sys_rst( void )
//{
//	EventBits_t bitsEvent = xEventGroupGetBits(reboot_egp);
//
//	if(BIT_RESET_SYSTEM == (bitsEvent & BIT_RESET_SYSTEM))
//    {
//		rtt_printf( RTT_OS, RTT_TEXT_RED">>>[ RESET ]: system reset!\n" );
//		reboot_brd();
//		vTaskDelay(1000);
//		drv_system_reboot(); //MCU重启 监控也会重启
//		xEventGroupClearBits(reboot_egp, BIT_RESET_SYSTEM );
//	}
//}


//static void func_master_rst( void )
//{
//	EventBits_t bitsEvent = xEventGroupGetBits(reboot_egp);
//
//	if(BIT_RESET_MASTER == (bitsEvent & BIT_RESET_MASTER))
//    {
//		rtt_printf( RTT_OS, RTT_TEXT_RED">>>[ RESET ]: master dev reset!\n" );
//		//***** 此处添加重启函数
//		xEventGroupClearBits(reboot_egp, BIT_RESET_MASTER );
//	}
//}


//static void func_slave_rst( void )
//{
//	EventBits_t bitsEvent = xEventGroupGetBits(reboot_egp);
//
//	if( BIT_RESET_SLAVE == (bitsEvent & BIT_RESET_SLAVE))
//    {
//		rtt_printf( RTT_OS, RTT_TEXT_RED">>>[ RESET ]: slave dev %d reset!\n", reboot_dev_id );
//		//此处添加发送远程重启指令函数
//		xEventGroupClearBits(reboot_egp, BIT_RESET_SLAVE );
//	}
//}


///*****************************************************************************
//* @Name	  : func_5g_modem_rst
//* @Brief  : 5G 模组重启函数
//* @Param  : None
//* @Retval :
//* @Author :
//* @Data   : 2023-03-14
//* @Note   :
//*****************************************************************************/
//static void func_5g_modem_rst( void )
//{
//	EventBits_t bitsEvent = xEventGroupGetBits(reboot_egp);
//
//	if(BIT_RESET_5GMODEM_H == (bitsEvent & BIT_RESET_5GMODEM_H))
//    {
//		//5G 模组硬件重启函数
//	}
//	else if(BIT_RESET_5GMODEM_S == (bitsEvent & BIT_RESET_5GMODEM_S))
//    {
//		//5G 模组软件重启函数
//	}
//
//	xEventGroupClearBits(reboot_egp, BIT_RESET_5GMODEM_H | BIT_RESET_5GMODEM_S );
//}

///*****************************************************************************
//* @Name	  : func_monitor_rst
//* @Brief  : 监控模组重启函数
//* @Param  : None
//* @Retval :
//* @Author :
//* @Data   : 2023-03-14
//* @Note   :
//*****************************************************************************/
//static void func_monitor_rst( void )
//{
//	EventBits_t bitsEvent = xEventGroupGetBits(reboot_egp);
//
//	if(BIT_RESET_SYSTEM == (bitsEvent & BIT_RESET_MONITOR))
//    {
//		//vModemReset();   //CAT1重启函数
//		xEventGroupClearBits(reboot_egp, BIT_RESET_MONITOR );
//	}
//}

///*****************************************************************************
//* @Name	  : func_ble_rst
//* @Brief  : 蓝牙重启函数
//* @Param  : None
//* @Retval :
//* @Author :
//* @Data   : 2023-03-14
//* @Note   :
//*****************************************************************************/
//static void func_ble_rst( void )
//{
//	EventBits_t bitsEvent = xEventGroupGetBits(reboot_egp);
//
//	if(BIT_RESET_SYSTEM == (bitsEvent & BIT_RESET_BLEM))
//    {
//		//BLEM重启函数
//		xEventGroupClearBits(reboot_egp, BIT_RESET_BLEM );
//	}
//
//	if(BIT_RESET_SYSTEM == (bitsEvent & BIT_RESET_BLES))
//    {
//		//BLES重启函数
//		xEventGroupClearBits(reboot_egp, BIT_RESET_BLES );
//	}
//}


///*****************************************************************************
//* @Name	  : func_fpga_rst
//* @Brief  : FPGA重启函数
//* @Param  : None
//* @Retval :
//* @Author :
//* @Data   : 2023-03-14
//* @Note   :
//*****************************************************************************/
//static void func_fpga_rst( void )
//{
//	EventBits_t bitsEvent = xEventGroupGetBits(reboot_egp);
//
//	if(BIT_RESET_SYSTEM == (bitsEvent & BIT_RESET_FPGA))
//    {
//		//BLEM重启函数
//		xEventGroupClearBits(reboot_egp, BIT_RESET_FPGA );
//	}
//}

///*****************************************************************************
//* @Name	  : func_brd_rst
//* @Brief  : 广播重启函数
//* @Param  : None
//* @Retval :
//* @Author :
//* @Data   : 2023-03-14
//* @Note   :
//*****************************************************************************/
//static void func_brd_rst( void )
//{
//	EventBits_t bitsEvent = xEventGroupGetBits(reboot_egp);
//
//	if( BIT_RESET_BRD == (bitsEvent & BIT_RESET_BRD) )
//    {
//		reboot_brd();	//广播重启指令
//		xEventGroupClearBits(reboot_egp, BIT_RESET_BRD );
//	}
//}


/*****************************************************************************
* @Name	  : reboot_action
* @Brief  : 执行指定的重启操作
* @Param  : None
* @Retval :
* @Author :
* @Data   : 2023-10-08
* @Note   :
*****************************************************************************/
static void reboot_action(void)
{
    EventBits_t bitsEvent = xEventGroupGetBits(reboot_egp);

    if(BIT_RESET_5GMODEM_H == (bitsEvent & BIT_RESET_5GMODEM_H))
    {
        //5G 模组硬件重启函数
        
        xEventGroupClearBits(reboot_egp, BIT_RESET_5GMODEM_H);
    }
    
    if(BIT_RESET_5GMODEM_S == (bitsEvent & BIT_RESET_5GMODEM_S))
    {
        //5G 模组软件重启函数
        
        xEventGroupClearBits(reboot_egp, BIT_RESET_5GMODEM_S);
    }
    
    if(BIT_RESET_MONITOR == (bitsEvent & BIT_RESET_MONITOR))
    {
        //CAT1重启函数
        #if( LOCAL_MODULE_TYPE == _MU )
        cat1_reset();
        xEventGroupClearBits(reboot_egp, BIT_RESET_MONITOR);
        #endif
    }
    
    if(BIT_RESET_BLEM == (bitsEvent & BIT_RESET_BLEM))
    {
        //BLEM重启函数
        #if( LOCAL_MODULE_TYPE == _MU )
        ble_rst();
        
        xEventGroupClearBits(reboot_egp, BIT_RESET_BLEM);
        #endif
    }

    if(BIT_RESET_BLES == (bitsEvent & BIT_RESET_BLES))
    {
        //BLES重启函数
        #if( LOCAL_MODULE_TYPE == _MU ) //主机复位从机发送AT指令
        //复位指定从机?还是所有从机?
        #else
        ble_rst();
        #endif
        xEventGroupClearBits(reboot_egp, BIT_RESET_BLES);
    }

    if(BIT_RESET_FPGA == (bitsEvent & BIT_RESET_FPGA))
    {
        //FPAG重启函数
        //添加重启函数
        xEventGroupClearBits(reboot_egp, BIT_RESET_FPGA);
    }
    
    if(BIT_RESET_BRD == (bitsEvent & BIT_RESET_BRD))
    {
        #if( LOCAL_MODULE_TYPE == _MU ) //主机复位从机发送AT指令
        reboot_brd();	//广播重启指令[所有从机]
        xEventGroupClearBits(reboot_egp, BIT_RESET_BRD);
        #endif
    }

    if(BIT_RESET_SLAVE == (bitsEvent & BIT_RESET_SLAVE))
    {
        #if( LOCAL_MODULE_TYPE == _MU ) //主机复位从机  发送AT指令
        rtt_printf(RTT_OS, RTT_TEXT_RED">>>[ RESET ]: slave dev %d reset!\n", reboot_dev_id);
        //此处添加发送远程重启指令函数
        //-重启指令已透传处理
        xEventGroupClearBits(reboot_egp, BIT_RESET_SLAVE);
        #endif
    }

    if(BIT_RESET_MASTER == (bitsEvent & BIT_RESET_MASTER))
    {
        rtt_printf(RTT_OS, RTT_TEXT_RED">>>[ RESET ]: master dev reset!\n");
        //主机重启和系统重启区别:判断是否需要重启从机?
        xEventGroupClearBits(reboot_egp, BIT_RESET_MASTER);
        NVIC_SystemReset();
    }
    
    if(BIT_RESET_SYSTEM == (bitsEvent & BIT_RESET_SYSTEM))
    {
        #if( LOCAL_MODULE_TYPE == _MU ) //主机系统重启
        mrc_reset_all_ru();
        rtt_printf(RTT_OS, RTT_TEXT_RED">>>[ RESET ]: system reset!\n");
        //复位fpga
        //复位cat.1
        //复位blem
        //复位bles
        //复位mcus
        //复位mcum
		
        vTaskDelay(5000);
        hi_reboot_mcu(); //MCU重启 监控也会重启
		
        #else   // 从机系统重启
        //复位fpga
        //复位bles
        //复位mcu
		
        vTaskDelay(5000);
        hi_reboot_mcu(); //MCU重启 监控也会重启
		
        #endif
    }
}


/*****************************************************************************
* @Name	  : reboot_in_task
* @Brief  : 处理系统重启
* @Param  : None
* @Retval : None
* @Author :
* @Data   : 2022-08-19
* @Note   :
*****************************************************************************/
void reboot_in_task(void)
{
    func_get_para();
    reboot_action();
//	func_sys_rst();
//	func_5g_modem_rst();
//	func_monitor_rst();
//	func_ble_rst();
//	func_fpga_rst();
//	func_brd_rst();
}
