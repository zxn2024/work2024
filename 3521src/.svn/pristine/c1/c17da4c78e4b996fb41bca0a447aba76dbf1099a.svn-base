/*************************************** Copyright ******************************
 *
 * (C) Copyright 2023, Shaanxi Tianji Communication
 * All Rights Reserved
 *
 * FileName   : 
 * Version    : none
 * Author     : 
 * Date       : 2023-02-10
 * Description: none
 *******************************************************************************/
//

#ifndef _TJC_BSP_CONFIG_H_
#define _TJC_BSP_CONFIG_H_

#include "MCU_config.h"
#include "hi_init.h"

#include "led.h"
#include "lna.h"
#include "pa.h"
#include "att_analog.h"
#include "port.h"
#include "hi_spi.h"
#include "switch_sp4t_MXD8641H.h"
#include "switch_spdt_RFASWA630PTF06.h"
#include "flash_25xx.h"
#include "fpga.h"
#include "ecr8668_driver.h"
#include "eswin_sync_modem.h"
#include "ble.h"


/****************************** 根据产品型号选择板级器件 ******************************/

#if (PRODUCT_MODEL == CMDT3521A)
    #include "cat1_EC800G_driver.h"
#elif (PRODUCT_MODEL == CMDT3521B)
    #include "cat1_N706_driver.h"
#elif (PRODUCT_MODEL == CMDT3521C)
    #include "cat1_MC661_driver.h"
#endif

#if(PRODUCT_MODEL == CMDT3521A)
    // analog att table     /*此结构体需要待频段选择完成后加载数据*/
    extern Analog_table_t analog_table_4g_up_1, analog_table_4g_up_2, analog_table_4g_dw;  
    extern Analog_table_t analog_table_nr_up_1, analog_table_nr_up_2, analog_table_nr_dw;
    extern Analog_table_t analog_table_mimo_up_1, analog_table_mimo_up_2, analog_table_mimo_dw;

    // led
    extern LED_t led_pg8, led_pc9, led_c11, led_pd0, led_pd1, led_pd2;

    // power crtl
    extern PORT_t rf_b3_dc_ctrl;                                       
    extern PORT_t rf_b8_dc_ctrl;
    extern PORT_t rf_b39_dc_ctrl;
    extern PORT_t rf_b40_dc_ctrl;
    extern PORT_t rf_4g_dc_en;
    extern PORT_t rf_nr_dc_en;
    extern PORT_t rf_mimo_dc_en;

    // 5g up silent
    extern PORT_t rf_nr_up_silent_ctrl;                                
    extern PORT_t rf_mimo_up_silent_ctrl;

    // 8668 rst
    extern PORT_t ecr8668_4g_dw_rst;
    extern PORT_t ecr8668_nr_rst;           
    extern PORT_t ecr8668_mimo_rst; 

    // lna
    extern LNA_t lna_b3_up,    lna_b3_dw_1,    lna_b3_dw_2;            // b3 lna
    extern LNA_t lna_b8_up,    lna_b8_dw_1;                            // b8 lna
    extern LNA_t lna_b39_up,   lna_b39_dw_1,   lna_b39_dw_2;           // b39 lna
    extern LNA_t lna_b40_up,   lna_b40_dw_1,   lna_b40_dw_2;           // b40 lna
    extern LNA_t lna_nr_up,    lna_nr_dw_1,    lna_nr_dw_2;            // nr lna
    extern LNA_t lna_mimo_up,  lna_mimo_dw_1,  lna_mimo_dw_2;          // mimo lna

    // spdt switch
    extern Switch_spdt_t spdt_b3_b39_up_ctrl, spdt_b3_b39_dw_ctrl;

    // sp4t switch
    extern Switch_sp4t_t sp4t_4g_dw_rx;
    extern Switch_sp4t_t sp4t_4g_dw_tx;
    extern Switch_sp4t_t sp4t_4g_ant;
    extern Switch_sp4t_t sp4t_4g_bts;

    // analog att
    extern ATT_analog_t att_analog_4g_dw;                              // 4g dw analog att
    extern ATT_analog_t att_analog_nr_dw;                              // nr dw analog att
    extern ATT_analog_t att_analog_mimo_dw;                            // mimo dw analog att
 
    extern ATT_analog_t att_analog_b3_up_1,    att_analog_b3_up_2;     // b3 up analog att
    extern ATT_analog_t att_analog_b8_up_1,    att_analog_b8_up_2;     // b8 up analog att
    extern ATT_analog_t att_analog_b39_up_1,   att_analog_b39_up_2;    // b39 up analog att
    extern ATT_analog_t att_analog_b40_up_1,   att_analog_b40_up_2;    // b40 up analog att
    extern ATT_analog_t att_analog_nr_up_1,    att_analog_nr_up_2;     // nr up analog att
    extern ATT_analog_t att_analog_mimo_up_1,  att_analog_mimo_up_2;   // mimo up analog att


    // 5g modem dc ctrl
    extern PORT_t modem_5g_dc_ctrl, modem_5g_syn;

    // spi flash
    extern FLASH_25xx_config_t spi_flash;
    
    // spi fpga
    extern spi_port_t spi_fpga;

    // fpga dc ctrl
    extern PORT_t fpga_dc_ctrl;                                           

    // fpga status check
    extern PORT_t fpga_status;

    // fpga jtag enable
    extern PORT_t fpga_jtag_en; 

    // fpga nStatus
    extern PORT_t fpga_n_status;

    // fpga nConfig
    extern PORT_t fpga_n_config;

    // fpga ps
    extern PORT_t fpga_ps_cs, fpga_ps_clk, fpga_ps_data;

    // fpga
    extern FPGA_config_t fpga;

    // trx8668
    extern TRX8668_FPGA_t h8668_LTE;
    extern TRX8668_FPGA_t h8668_NR;
    extern TRX8668_FPGA_t h8668_NR_MIMO;

    // uart
    extern uart_func_t uart_modem_fun;
    extern uart_func_t uart_cat1_fun;
    extern uart_func_t uart_ble_fun;

    
#elif(PRODUCT_MODEL == CMDT3521B)
    // analog att table     /*此结构体需要待频段选择完成后加载数据*/
    extern Analog_table_t analog_table_4g_up_1, analog_table_4g_up_2, analog_table_4g_dw;  
    extern Analog_table_t analog_table_nr_up_1, analog_table_nr_up_2, analog_table_nr_dw;
    extern Analog_table_t analog_table_mimo_up_1, analog_table_mimo_up_2, analog_table_mimo_dw;

    // led
    extern LED_t led_pg8, led_pc9, led_c11, led_pd0, led_pd1, led_pd2;

    // power crtl
    extern PORT_t rf_b3_dc_ctrl;                                       
    extern PORT_t rf_b8_dc_ctrl;
    extern PORT_t rf_b39_dc_ctrl;
    extern PORT_t rf_b40_dc_ctrl;
    extern PORT_t rf_4g_dc_en;
    extern PORT_t rf_nr_dc_en;
    extern PORT_t rf_mimo_dc_en;

    // 5g up silent
    extern PORT_t rf_nr_up_silent_ctrl;                                
    extern PORT_t rf_mimo_up_silent_ctrl;

    // 8668 rst
    extern PORT_t ecr8668_4g_dw_rst;
    extern PORT_t ecr8668_nr_rst;           
    extern PORT_t ecr8668_mimo_rst; 

    // lna
    extern LNA_t lna_b3_up,    lna_b3_dw_1,    lna_b3_dw_2;            // b3 lna
    extern LNA_t lna_b8_up,    lna_b8_dw_1;                            // b8 lna
    extern LNA_t lna_b39_up,   lna_b39_dw_1,   lna_b39_dw_2;           // b39 lna
    extern LNA_t lna_b40_up,   lna_b40_dw_1,   lna_b40_dw_2;           // b40 lna
    extern LNA_t lna_nr_up,    lna_nr_dw_1,    lna_nr_dw_2;            // nr lna
    extern LNA_t lna_mimo_up,  lna_mimo_dw_1,  lna_mimo_dw_2;          // mimo lna

    // spdt switch
    extern Switch_spdt_t spdt_b3_b39_up_ctrl, spdt_b3_b39_dw_ctrl;

    // sp4t switch
    extern Switch_sp4t_t sp4t_4g_dw_rx;
    extern Switch_sp4t_t sp4t_4g_dw_tx;
    extern Switch_sp4t_t sp4t_4g_ant;
    extern Switch_sp4t_t sp4t_4g_bts;

    // analog att
    extern ATT_analog_t att_analog_4g_dw;                              // 4g dw analog att
    extern ATT_analog_t att_analog_nr_dw;                              // nr dw analog att
    extern ATT_analog_t att_analog_mimo_dw;                            // mimo dw analog att
 
    extern ATT_analog_t att_analog_b3_up_1,    att_analog_b3_up_2;     // b3 up analog att
    extern ATT_analog_t att_analog_b8_up_1,    att_analog_b8_up_2;     // b8 up analog att
    extern ATT_analog_t att_analog_b39_up_1,   att_analog_b39_up_2;    // b39 up analog att
    extern ATT_analog_t att_analog_b40_up_1,   att_analog_b40_up_2;    // b40 up analog att
    extern ATT_analog_t att_analog_nr_up_1,    att_analog_nr_up_2;     // nr up analog att
    extern ATT_analog_t att_analog_mimo_up_1,  att_analog_mimo_up_2;   // mimo up analog att


    // 5g modem dc ctrl
    extern PORT_t modem_5g_dc_ctrl, modem_5g_syn;

    // spi flash
    extern FLASH_25xx_config_t spi_flash;
    
    // spi fpga
    extern spi_port_t spi_fpga;

    // fpga dc ctrl
    extern PORT_t fpga_dc_ctrl;                                           

    // fpga status check
    extern PORT_t fpga_status;

    // fpga jtag enable
    extern PORT_t fpga_jtag_en; 

    // fpga nStatus
    extern PORT_t fpga_n_status;

    // fpga nConfig
    extern PORT_t fpga_n_config;

    // fpga ps
    extern PORT_t fpga_ps_cs, fpga_ps_clk, fpga_ps_data;

    // fpga
    extern FPGA_config_t fpga;

    // trx8668
    extern TRX8668_FPGA_t h8668_LTE;
    extern TRX8668_FPGA_t h8668_NR;
    extern TRX8668_FPGA_t h8668_NR_MIMO;

    // uart
    extern uart_func_t uart_modem_fun;
    extern uart_func_t uart_cat1_fun;
    extern uart_func_t uart_ble_fun;

#elif(PRODUCT_MODEL == CMDT3521C)
    // analog att table     /*此结构体需要待频段选择完成后加载数据*/
    extern Analog_table_t analog_table_4g_up_1, analog_table_4g_up_2, analog_table_4g_dw;  
    extern Analog_table_t analog_table_nr_up_1, analog_table_nr_up_2, analog_table_nr_dw;
    extern Analog_table_t analog_table_mimo_up_1, analog_table_mimo_up_2, analog_table_mimo_dw;

    // led
    extern LED_t led_pg8, led_pc9, led_c11, led_pd0, led_pd1, led_pd2;

    // power crtl
    extern PORT_t rf_b3_dc_ctrl;                                       
    extern PORT_t rf_b8_dc_ctrl;
    extern PORT_t rf_b39_dc_ctrl;
    extern PORT_t rf_b40_dc_ctrl;
    extern PORT_t rf_4g_dc_en;
    extern PORT_t rf_nr_dc_en;
    extern PORT_t rf_mimo_dc_en;

    // 5g up silent
    extern PORT_t rf_nr_up_silent_ctrl;                                
    extern PORT_t rf_mimo_up_silent_ctrl;

    // 8668 rst
    extern PORT_t ecr8668_4g_dw_rst;
    extern PORT_t ecr8668_nr_rst;           
    extern PORT_t ecr8668_mimo_rst; 

    // lna
    extern LNA_t lna_b3_up,    lna_b3_dw_1,    lna_b3_dw_2;            // b3 lna
    extern LNA_t lna_b8_up,    lna_b8_dw_1;                            // b8 lna
    extern LNA_t lna_b39_up,   lna_b39_dw_1,   lna_b39_dw_2;           // b39 lna
    extern LNA_t lna_b40_up,   lna_b40_dw_1,   lna_b40_dw_2;           // b40 lna
    extern LNA_t lna_nr_up,    lna_nr_dw_1,    lna_nr_dw_2;            // nr lna
    extern LNA_t lna_mimo_up,  lna_mimo_dw_1,  lna_mimo_dw_2;          // mimo lna

    // spdt switch
    extern Switch_spdt_t spdt_b3_b39_up_ctrl, spdt_b3_b39_dw_ctrl;

    // sp4t switch
    extern Switch_sp4t_t sp4t_4g_dw_rx;
    extern Switch_sp4t_t sp4t_4g_dw_tx;
    extern Switch_sp4t_t sp4t_4g_ant;
    extern Switch_sp4t_t sp4t_4g_bts;

    // analog att
    extern ATT_analog_t att_analog_4g_dw;                              // 4g dw analog att
    extern ATT_analog_t att_analog_nr_dw;                              // nr dw analog att
    extern ATT_analog_t att_analog_mimo_dw;                            // mimo dw analog att
 
    extern ATT_analog_t att_analog_b3_up_1,    att_analog_b3_up_2;     // b3 up analog att
    extern ATT_analog_t att_analog_b8_up_1,    att_analog_b8_up_2;     // b8 up analog att
    extern ATT_analog_t att_analog_b39_up_1,   att_analog_b39_up_2;    // b39 up analog att
    extern ATT_analog_t att_analog_b40_up_1,   att_analog_b40_up_2;    // b40 up analog att
    extern ATT_analog_t att_analog_nr_up_1,    att_analog_nr_up_2;     // nr up analog att
    extern ATT_analog_t att_analog_mimo_up_1,  att_analog_mimo_up_2;   // mimo up analog att


    // 5g modem dc ctrl
    extern PORT_t modem_5g_dc_ctrl, modem_5g_syn;

    // spi flash
    extern FLASH_25xx_config_t spi_flash;
    
    // spi fpga
    extern spi_port_t spi_fpga;

    // fpga dc ctrl
    extern PORT_t fpga_dc_ctrl;                                           

    // fpga status check
    extern PORT_t fpga_status;

    // fpga jtag enable
    extern PORT_t fpga_jtag_en; 

    // fpga nStatus
    extern PORT_t fpga_n_status;

    // fpga nConfig
    extern PORT_t fpga_n_config;

    // fpga ps
    extern PORT_t fpga_ps_cs, fpga_ps_clk, fpga_ps_data;

    // fpga
    extern FPGA_config_t fpga;

    // trx8668
    extern TRX8668_FPGA_t h8668_LTE;
    extern TRX8668_FPGA_t h8668_NR;
    extern TRX8668_FPGA_t h8668_NR_MIMO;

    // uart
    extern uart_func_t uart_modem_fun;
    extern uart_func_t uart_cat1_fun;
    extern uart_func_t uart_ble_fun;
    
#elif(PRODUCT_MODEL == CMDT3521D)

#elif(PRODUCT_MODEL == CMDT3521E)

#elif(PRODUCT_MODEL == CMDT3521F)

#endif

/****************************** 声明 ******************************/


/*****************************************************************************
 * @Name	  : bsp_register
 * @brief  :  板上器件注册,资源分配
 */
void bsp_register(void);

/*****************************************************************************
 * @Name	  : bsp_device_init
 * @brief  : 根据注册的参数对板上器件初始化,复杂器件或有特殊要求的可单独在其他任务中初始化
 */
void bsp_device_init(void);

#endif
