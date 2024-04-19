/**
 * **********************************************************************************
 * @copyright Copyright (c) 2023 Shaanxi Tianji Communication Technologies Co., Ltd.
 *
 * @file        os_task.c
 * @author      WangJun
 * @version     None
 * @date        2023-06-16
 * @brief       None
 *
 * **********************************************************************************
 */

#include "os_task.h"
#include "os_event.h"
#include "rtt.h"

#include "hi_init.h"

#include "bsp_config.h"

#include "fpga.h"
#include "bsp_pc.h"

#include "bsp_testmode_para_list.h"
#include "app_monitor.h"
#include "bsp_warn_user.h"

#include "bsp_rf_ctr.h"
#include "ble.h"
#include "bsp_reboot.h"
#include "bsp_rf_global.h"
#include "led_display.h"
#include "bsp_rf_scan_net.h"
#include "app_monitor_report.h"
#include "app_upgrade.h"
#include "bsp_rf_5g_modem.h"
#include "mu_ru_communicate.h"
/*---------------------- 端口定义 ----------------------*/


/*---------------------- 宏 定 义 ----------------------*/


/*---------------------- 结 构 体 ----------------------*/


/*---------------------- 常    量 ----------------------*/


/*---------------------- 全局变量 ----------------------*/
void vTaskPrintfWaterMark( void );

/*---------------------- 局部变量 ----------------------*/
#if(1 == EN_HW_TEST)

static TaskHandle_t xHandleTaskHwTest = NULL;
static void vTaskHwTest(void * pvParameters);

#if(PRODUCT_MODEL == CMDT3521A) || (PRODUCT_MODEL == CMDT3521B)

// 7E01020304050607080910111213141516171819202122232425262728293031323334353637383940414243444546474849505152535455565758596061627E
uint8_t buf[64];

uint8_t send_buf[64] = {0x7E, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x60, 0x61, 0x62, 0x7E};
    
uint16_t len;

uint8_t uart0_buf[10] = "uart0 text";
uint8_t uart1_buf[10] = "uart1 text";
uint8_t uart2_buf[10] = "uart2 text";
uint8_t uart5_buf[10] = "uart5 text";
uint16_t modem_rx_len, ble_rx_len, cat1_rx_len;

int8_t temperature = 0;

uint8_t write_enable[1] = {0x06};
uint8_t write_disable[1] = {0x04};
uint8_t read[4] = {0x03, 0x00, 0x00, 0x00};
uint8_t fast_read[4] = {0x0B, 0x00, 0x00, 0x00};
uint8_t sector_4k_erase[4] = {0x20, 0x00, 0x00, 0x00};
uint8_t block_32k_erase[4] = {0x52, 0x00, 0x00, 0x00};
uint8_t block_64k_erase[4] = {0xD8, 0x00, 0x00, 0x00};
uint8_t chip_erase[1] = {0x60};

uint8_t spi1_buf[10] = {0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA};

uint32_t cnt = 0, pritf_cnt = 0;
#elif(PRODUCT_MODEL == CMDT3521C)


#endif

#else

static TaskHandle_t xHandleDataProc     = NULL;
static TaskHandle_t xHandleTaskRf       = NULL;

#if( LOCAL_MODULE_TYPE == _MU )
static TaskHandle_t xHandleTaskMonitor  = NULL;
static TaskHandle_t xHandleTask5gModem  = NULL;
static TaskHandle_t xHandleTaskCat1     = NULL;
#endif

static TaskHandle_t xHandleTaskUpGrade  = NULL;
//static TaskHandle_t xHandleTaskFpga     = NULL;
static TaskHandle_t xHandleTaskPc       = NULL;
static TaskHandle_t xHandleTaskBle      = NULL;
static TaskHandle_t xHandleTaskUser     = NULL;


static void vTaskDataProc(void * pvParameters);
static void vTaskRf(void * pvParameters);
#if( LOCAL_MODULE_TYPE == _MU )
static void vTaskMonitor(void * pvParameters);
static void vTask5gModem(void * pvParameters);
static void vTaskCat1(void * pvParameters);
#endif
static void vTaskUpGrade(void * pvParameters);
//static void vTaskFpga(void * pvParameters);
static void vTaskPc(void * pvParameters);
static void vTaskBle(void * pvParameters);
static void vTaskUser(void * pvParameters);
#endif

/*---------------------- 函    数 ----------------------*/
#if(1 == EN_HW_TEST)
extern void Shell_Exec_SymFunc(char *ac_input_cmd);
/****************************************************************************
* 函 数 名：vTaskHwTest(void *pvParameters)
* 功能描述：硬件测试
* 参数说明：
* 函数输出：
* 备    注：LED闪烁时间周期与对应任务执行周期有关
****************************************************************************/
static void vTaskHwTest(void * pvParameters)
{
    uint16_t index;
    hi_usb_init();

    while(1)
    {
        //rtt_printf(RTT_OS, "%s running\r\n", __func__);

        /*led test*/

        /*lna test*/

        /*pa test*/

        /*spdt test*/

        /*adc test*/
        
        /*usart2 test*/

        /*digital att test*/

        /*analog att test*/

        /*nor flash spi test*/
        
        /*fpag spi test*/

        /*eswin8668 spi test*/

        /*ble rst test*/
        
        /*adc test*/
        hi_adc_get_value();
        
        temperature = adc_read_temperature(tAdcTempValue.result.usAdcAve);
        /*cat1 rst test*/

        /*cat1 open test*/

        /*cat1 state test*/

        /*delay us test*/

        /* 5g modem pwr ctrl*/

        /* fpga ctrl*/

        /*usb cdc test*/
        hi_usb_receive(buf, &len);
        if(0 < len)
        {
            hi_usb_send(buf, len);
            buf[len] = 0;
            for (index=len-1; index>2; index--)
                if ((buf[index] == '\r') || (buf[index] == '\n'))	buf[index] = 0;
            if (strlen((char *)buf) > 2)
                Shell_Exec_SymFunc((char *)buf);	
            len = 0;
        }

//        // usb speedtest
//        hi_usb_receive(buf, &len);
//        if(0 < len)
//        {
//            for(uint16_t i = 0; i < 1024; i++)
//            {
//                hi_usb_send(send_buf, 64);
//            }
//            len = 0;
//        }
        
        vTaskDelay(50);
    }
}
#else
/****************************************************************************
* 函 数 名：vTaskDataProc(void *pvParameters)
* 功能描述：处理需要实时性和快速响应的数据, 进行简单的数据预处理\分拣等工作, 不要把高耗时或会阻塞的流程放在此任务
* 参数说明：
* 函数输出：
****************************************************************************/
static void vTaskDataProc(void * pvParameters)
{
    static uint16_t print_cnt = 0;
    RF_SYS_E sys = RF_SYS_1;
    TRX8668_FPGA_t *ptTRX8668[RF_SYS_NUM] = {&h8668_LTE, &h8668_NR, &h8668_NR_MIMO};
    vTaskDelay(1000);
    while(1)
    {
        //rtt_printf(RTT_OS, "%s running\r\n", __func__);
        // adc数据处理
        hi_adc_get_value();        
        
//        if(BIT_ADC2_DATA_VALID == ((xEventGroupGetBits(xAdcEgp) & BIT_ADC2_DATA_VALID)))
//        {
//            xEventGroupClearBits(xAdcEgp, BIT_ADC2_DATA_VALID);
//            
//            rtt_printf(RTT_OS, RTT_CTRL_TEXT_BRIGHT_GREEN"\r\n[Temp     adc]: %4d     [Temperature]: %4d \r\n", tAdcTempValue.result.usAdcAve, adc_read_temperature(tAdcTempValue.result.usAdcAve));
//            rtt_printf(RTT_OS, RTT_CTRL_TEXT_BRIGHT_YELLOW"[4G  TRX  ADC]: %4d     [4G       UP]: %4d        [4G     DW]: %4d \r\n", tChAdcValue[0].result.usAdcAve, tChAdcValue[3].result.usAdcAve, tChAdcValue[4].result.usAdcAve);
//            rtt_printf(RTT_OS, RTT_CTRL_TEXT_BRIGHT_MAGENTA"[NR  TRX  ADC]: %4d     [N41      UP]: %4d        [N41    DW]: %4d \r\n", tChAdcValue[1].result.usAdcAve, tChAdcValue[5].result.usAdcAve, tChAdcValue[6].result.usAdcAve);
//            rtt_printf(RTT_OS, "[NR_M TRX ADC]: %4d     [N41_M    UP]: %4d        [N41_M  DW]: %4d \r\n", tChAdcValue[2].result.usAdcAve, tChAdcValue[7].result.usAdcAve, tChAdcValue[8].result.usAdcAve);
//        
//        }
        if(++print_cnt > 200)
        {
            print_cnt = 0;
            rtt_printf(RTT_ADC, RTT_CTRL_TEXT_BRIGHT_GREEN"\r\n[Temp     adc]: %4d     [Temperature]: %4d \r\n", tAdcTempValue.result.usAdcAve, adc_read_temperature(tAdcTempValue.result.usAdcAve));
            rtt_printf(RTT_ADC, RTT_CTRL_TEXT_BRIGHT_YELLOW"[4G  TRX  ADC]: %4d     [4G       UP]: %4d        [4G     DW]: %4d \r\n", tChAdcValue[0].result.usAdcAve, tChAdcValue[3].result.usAdcAve, tChAdcValue[4].result.usAdcAve);
            rtt_printf(RTT_ADC, RTT_CTRL_TEXT_BRIGHT_MAGENTA"[NR  TRX  ADC]: %4d     [N41      UP]: %4d        [N41    DW]: %4d \r\n", tChAdcValue[1].result.usAdcAve, tChAdcValue[5].result.usAdcAve, tChAdcValue[6].result.usAdcAve);
            rtt_printf(RTT_ADC, "[NR_M TRX ADC]: %4d     [N41_M    UP]: %4d        [N41_M  DW]: %4d \r\n", tChAdcValue[2].result.usAdcAve, tChAdcValue[7].result.usAdcAve, tChAdcValue[8].result.usAdcAve);
        }
        // 8668 adc中断计数
        // 未测, 待测试
        // BSP_866x_Proc_warning_exti_periodic(&h8668_LTE);
        // BSP_866x_Proc_warning_exti_periodic(&h8668_NR);
        // BSP_866x_Proc_warning_exti_periodic(&h8668_NR_MIMO);
        for(sys = RF_SYS_1; sys < RF_SYS_NUM; sys++)
        {
            BSP_866x_Proc_warning_exti_periodic(sys, ptTRX8668[sys]);
        }

        // 串口数据分拣
        #if( LOCAL_MODULE_TYPE == _MU )
        cat1_uart_rx_data_handle();
        modem_uart_rx_data_handle();
        #endif
        ble_uart_rx_data_handle();

        // usb数据分包
        hi_usb_pkt_cplt_chk();
        
        vTaskDelay(5);
    }
}


/****************************************************************************
* 函 数 名：vTaskRf(void *pvParameters)
* 功能描述：rf
* 参数说明：
* 函数输出：
****************************************************************************/
static void vTaskRf(void * pvParameters)
{
    //等待fpga任务完成初始化才能启动射频控制
    while(fpga_get_state() != 1)
    {
        vTaskDelay(5);
    }
    // adc启动/dma启动传输
    #if(MCU_SERIES == MCU_STM32F4XX)
    adc_start_dma();
    #endif
	
//	rf_set_define_band_in_product_mode( BAND_3 );
	jump_in_test_mode();	//判断是否需要进入测试模式
	rf_5g_modem_scan_init();
    while(1)
    {
		rf_5g_modem_in_task();
		rf_ctr_in_task();

//		BSP_866xAttSetTest();
		
		if( tDevPublicPara.ucWorkMode == WORK_NORMAL )
		{
			BSP_866x_run(&h8668_LTE);
			BSP_866x_run(&h8668_NR);
			BSP_866x_run(&h8668_NR_MIMO);
		}

        //rtt_printf(RTT_OS, "%s running\r\n", __func__);
        vTaskDelay(5);
    }
}

#if( LOCAL_MODULE_TYPE == _MU )
/****************************************************************************
* 函 数 名：vTaskMonitor(void *pvParameters)
* 功能描述：monitor
* 参数说明：
* 函数输出：
****************************************************************************/
static void vTaskMonitor(void * pvParameters)
{
    dev_warn_init( );	//告警初始化
    while(1)
    {
        //rtt_printf(RTT_OS, "%s running\r\n", __func__);
        mrc_recv_msg_in_task();
        monitor_msg_in_task();
        warn_flow_in_task();
        vTaskDelay(50);
    }
}

/****************************************************************************
* 函 数 名：vTask5gModem(void *pvParameters)
* 功能描述：5g modem
* 参数说明：
* 函数输出：
****************************************************************************/
static void vTask5gModem(void * pvParameters)
{
    modem_init();
    while(1)
    {
       // rtt_printf(RTT_OS, "%s running\r\n", __func__);
        modem_run();
        vTaskDelay(100);
    }
}

/****************************************************************************
* 函 数 名：vTaskCat1(void *pvParameters)
* 功能描述：cat1
* 参数说明：
* 函数输出：
****************************************************************************/
static void vTaskCat1(void * pvParameters)
{
    while(1)
    {
        //rtt_printf(RTT_OS, "%s running\r\n", __func__);
        cat1_run();
        monitor_queue_recv();
        vTaskDelay(100);
    }
}
#endif

/****************************************************************************
* 函 数 名：vTaskUpGrade(void *pvParameters)
* 功能描述：升级
* 参数说明：
* 函数输出：
****************************************************************************/
static void vTaskUpGrade(void * pvParameters)
{
    while(1)
    {
        //rtt_printf(RTT_OS, "%s running\r\n", __func__);
		upgrade_run();
        vTaskDelay(100);
    }
}

///****************************************************************************
//* 函 数 名：vTaskFpga(void *pvParameters)
//* 功能描述：fpga
//* 参数说明：
//* 函数输出：
//****************************************************************************/
//static void vTaskFpga(void * pvParameters)
//{
//    while(1)
//    {
//        rtt_printf(RTT_OS, "%s running\r\n", __func__);
//        vTaskDelay(50);
//    }
//}

/****************************************************************************
* 函 数 名：vTaskPc(void *pvParameters)
* 功能描述：pc
* 参数说明：
* 函数输出：
****************************************************************************/
static void vTaskPc(void * pvParameters)
{
    hi_usb_init();
    para_list_init();
    while(1)
    {
        //rtt_printf(RTT_OS, "%s running\r\n", __func__);

        pc_msg_in_task();
        hi_usb_clear_fifo();
        vTaskDelay(50);
    }
}

/****************************************************************************
* 函 数 名：vTaskBle(void *pvParameters)
* 功能描述：ble
* 参数说明：
* 函数输出：
****************************************************************************/
// T_BleData ble_test_rx_buf;
// uint16_t msg_len = 0;
static void vTaskBle(void * pvParameters)
{
    ble_init();
    mrc_init();
    while(1)
    {
        //rtt_printf(RTT_OS, "%s running\r\n", __func__);
        // msg_len = ble_msg_receive(&ble_test_rx_buf);
        // if(msg_len > 0)
        // {
        //     ble_app_msg_send((uint8_t*)"1234567890", 10);
        // }
        mrc_process();
        vTaskDelay(100);
    }
}

/****************************************************************************
* 函 数 名：vTaskUser(void *pvParameters)
* 功能描述：uset
* 参数说明：
* 函数输出：
****************************************************************************/
static void vTaskUser(void * pvParameters)
{
//    led_pd0.set(&led_pd0, LED_FLASH, 500);
//    led_pg8.set(&led_pg8, LED_FLASH, 1000);
//    led_pc9.set(&led_pc9, LED_FLASH, 1000);
//    led_c11.set(&led_c11, LED_FLASH, 1000);
//    led_pd1.set(&led_pd1, LED_FLASH, 1000);
//    led_pd2.set(&led_pd2, LED_FLASH, 1000);
	led_init();
    while(1)
    {
        //rtt_printf(RTT_OS, "%s running\r\n", __func__);

		led_in_task();
//        BI_LED_RUN(&led_pd0);
//        BI_LED_RUN(&led_pg8);
//        BI_LED_RUN(&led_pc9);
//        BI_LED_RUN(&led_c11);
//        BI_LED_RUN(&led_pd1);
//        BI_LED_RUN(&led_pd2);
        vTaskPrintfWaterMark();
        reboot_in_task();
        vTaskDelay(50);
    }
}

#endif


/*****************************************************************************
* @brief  : 创建线程
* @param  : None
* @retval : None
* @date   : 2023-06-20
* @note   :
*****************************************************************************/
void os_task_create(void)
{
#if(1 == EN_HW_TEST)
    xTaskCreate(vTaskHwTest,
        "vTaskHwTest",
        512,
        NULL,
        3,
        &xHandleTaskHwTest);

    if(xHandleTaskHwTest == NULL)
    {
        rtt_printf(RTT_OS, RTT_CTRL_TEXT_BRIGHT_RED"task HwTest create fail\r\n");
    }
    
    fpga_task_init();
#else
    rtt_printf(RTT_OS, "%s ...\r\n", __func__);

    fpga_task_init();

    xTaskCreate(vTaskDataProc,
        "vTaskDataProc",
        1024,
        NULL,
        11,
        &xHandleDataProc);
    
    xTaskCreate(vTaskRf,
        "vTaskRf",
        512,
        NULL,
        10,
        &xHandleTaskRf);
    
#if( LOCAL_MODULE_TYPE == _MU )
    xTaskCreate(vTaskMonitor,
        "vTaskMonitor",
        1024,
        NULL,
        9,
        &xHandleTaskMonitor);
    
    xTaskCreate(vTask5gModem,
        "vTask5gModem",
        256,
        NULL,
        7,
        &xHandleTask5gModem);
        
    xTaskCreate(vTaskCat1,
        "vTaskCat1",
        256,
        NULL,
        3,
        &xHandleTaskCat1);
#endif
        
    xTaskCreate(vTaskUpGrade,
        "vTaskUpGrade",
        1024,
        NULL,
        8,
        &xHandleTaskUpGrade);
        
//    xTaskCreate(vTaskFpga,
//        "vTaskFpga",
//        256,
//        NULL,
//        6,
//        &xHandleTaskFpga);
        
    xTaskCreate(vTaskPc,
        "vTaskPc",
        1024,
        NULL,
        5,
        &xHandleTaskPc);
        
    xTaskCreate(vTaskBle,
        "vTaskBle",
        512,
        NULL,
        4,
        &xHandleTaskBle);
        
    xTaskCreate(vTaskUser,
        "vTaskUser",
        256,
        NULL,
        3,
        &xHandleTaskUser);

    if(xHandleDataProc == NULL)
    {
        rtt_printf(RTT_OS, RTT_CTRL_TEXT_BRIGHT_RED"task DataProc create fail\r\n");
    }
    
    if(xHandleTaskRf == NULL)
    {
        rtt_printf(RTT_OS, RTT_CTRL_TEXT_BRIGHT_RED"task rf create fail\r\n");
    }
    
    #if( LOCAL_MODULE_TYPE == _MU )
    if(xHandleTaskMonitor == NULL)
    {
        rtt_printf(RTT_OS, RTT_CTRL_TEXT_BRIGHT_RED"task monitor create fail\r\n");
    }
    
    if(xHandleTask5gModem == NULL)
    {
        rtt_printf(RTT_OS, RTT_CTRL_TEXT_BRIGHT_RED"task 5g modem create fail\r\n");
    }
    
    if(xHandleTaskCat1 == NULL)
    {
        rtt_printf(RTT_OS, RTT_CTRL_TEXT_BRIGHT_RED"task cat1 create fail\r\n");
    }
    #endif
    
    if(xHandleTaskUpGrade == NULL)
    {
        rtt_printf(RTT_OS, RTT_CTRL_TEXT_BRIGHT_RED"task upgrade create fail\r\n");
    }
    
    if(xHandleTaskPc == NULL)
    {
        rtt_printf(RTT_OS, RTT_CTRL_TEXT_BRIGHT_RED"task pc create fail\r\n");
    }
    
//    if(xHandleTaskFpga == NULL)
//    {
//        rtt_printf(RTT_OS, RTT_CTRL_TEXT_BRIGHT_RED"task fpga create fail\r\n");
//    }

    if(xHandleTaskBle == NULL)
    {
        rtt_printf(RTT_OS, RTT_CTRL_TEXT_BRIGHT_RED"task ble create fail\r\n");
    }
    
    if(xHandleTaskUser == NULL)
    {
        rtt_printf(RTT_OS, RTT_CTRL_TEXT_BRIGHT_RED"task user create fail\r\n");
    }
#endif
}


/*****************************************************************************
* @Name	  : vTaskPrintfWaterMark
* @Brief  : 打印任务最小剩余堆栈数值
* @Param  : None
* @Retval : 
* @Author : 
* @Data   : 2022-11-02
* @Note   :
*****************************************************************************/
void vTaskPrintfWaterMark( void )
{
#if 1
    static uint16_t timeCnt = 0;

    timeCnt++;
    if( timeCnt == 100 )	//time = 100, 大约5s打印一次
    {
        timeCnt = 0;
//		vPrintfRunTime( RTT_OS );
        rtt_printf( RTT_OS, "************* Water Mark *****************\n" );
        rtt_printf( RTT_OS, "vTaskDataProc  : %d\n", uxTaskGetStackHighWaterMark( xHandleDataProc  ) );
        rtt_printf( RTT_OS, "vTaskRf      	: %d\n", uxTaskGetStackHighWaterMark( xHandleTaskRf      ) );
        rtt_printf( RTT_OS, "vTaskMonitor   : %d\n", uxTaskGetStackHighWaterMark( xHandleTaskMonitor  ) );
        rtt_printf( RTT_OS, "vTask5gModem   : %d\n", uxTaskGetStackHighWaterMark( xHandleTask5gModem  ) );
        rtt_printf( RTT_OS, "vTaskCat1      : %d\n", uxTaskGetStackHighWaterMark( xHandleTaskCat1       ) );
        rtt_printf( RTT_OS, "vTaskUpGrade	: %d\n", uxTaskGetStackHighWaterMark( xHandleTaskUpGrade) );
        rtt_printf( RTT_OS, "vTaskPc      	: %d\n", uxTaskGetStackHighWaterMark( xHandleTaskPc      ) );
        rtt_printf( RTT_OS, "vTaskBle       : %d\n", uxTaskGetStackHighWaterMark( xHandleTaskBle       ) );
        rtt_printf( RTT_OS, "vTaskUser     	: %d\n", uxTaskGetStackHighWaterMark( xHandleTaskUser     ) );
        rtt_printf( RTT_OS, "******************************************\n\n" );
    }
#endif
}

