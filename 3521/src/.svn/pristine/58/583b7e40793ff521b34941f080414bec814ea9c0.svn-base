/*************************************** Copyright ******************************
 *
 * (C) Copyright 2023, Shaanxi Tianji Communication
 * All Rights Reserved
 *
 * FileName   : bsp_config.h
 * Version    : none
 * Author     :
 * Date       : 2023-09-19
 * Description:
 * 板级器件的资源分配,包括GPIO分配,总线接口分配,衰减表/功率表,设备句柄初始化.
 * 注意:此处进行资源分配,而非初始化设备,设备的初始化需要在应用中自行调用驱动open接口.
 *******************************************************************************/
//

#include "bsp_config.h"
#include "rtt.h"
#include "os_event.h"

#if (PRODUCT_MODEL == CMDT3521A)
// analog att table     /*此结构体需要待频段选择完成后加载数据*/
Analog_table_t analog_table_4g_up_1, analog_table_4g_up_2, analog_table_4g_dw;   
Analog_table_t analog_table_nr_up_1, analog_table_nr_up_2, analog_table_nr_dw;
Analog_table_t analog_table_mimo_up_1, analog_table_mimo_up_2, analog_table_mimo_dw;

// led
LED_t led_pg8, led_pc9, led_c11, led_pd0, led_pd1, led_pd2;

// power crtl
PORT_t rf_b3_dc_ctrl;                                       
PORT_t rf_b8_dc_ctrl;
PORT_t rf_b39_dc_ctrl;
PORT_t rf_b40_dc_ctrl;
PORT_t rf_4g_dc_en;
PORT_t rf_nr_dc_en;
PORT_t rf_mimo_dc_en;

// 5g up silent
PORT_t rf_nr_up_silent_ctrl;                                
PORT_t rf_mimo_up_silent_ctrl;

// 8668 rst
PORT_t ecr8668_4g_dw_rst;
PORT_t ecr8668_nr_rst;           
PORT_t ecr8668_mimo_rst; 

// lna
LNA_t lna_b3_up,    lna_b3_dw_1,    lna_b3_dw_2;            // b3 lna
LNA_t lna_b8_up,    lna_b8_dw_1;                            // b8 lna
LNA_t lna_b39_up,   lna_b39_dw_1,   lna_b39_dw_2;           // b39 lna
LNA_t lna_b40_up,   lna_b40_dw_1,   lna_b40_dw_2;           // b40 lna
LNA_t lna_nr_up,    lna_nr_dw_1,    lna_nr_dw_2;            // nr lna
LNA_t lna_mimo_up,  lna_mimo_dw_1,  lna_mimo_dw_2;          // mimo lna

// spdt switch
Switch_spdt_t spdt_b3_b39_up_ctrl, spdt_b3_b39_dw_ctrl;

// sp4t switch
Switch_sp4t_t sp4t_4g_dw_rx;
Switch_sp4t_t sp4t_4g_dw_tx;
Switch_sp4t_t sp4t_4g_ant;
Switch_sp4t_t sp4t_4g_bts;

// analog att
ATT_analog_t att_analog_4g_dw;                              // 4g dw analog att
ATT_analog_t att_analog_nr_dw;                              // nr dw analog att
ATT_analog_t att_analog_mimo_dw;                            // mimo dw analog att

ATT_analog_t att_analog_b3_up_1,    att_analog_b3_up_2;     // b3 up analog att
ATT_analog_t att_analog_b8_up_1,    att_analog_b8_up_2;     // b8 up analog att
ATT_analog_t att_analog_b39_up_1,   att_analog_b39_up_2;    // b39 up analog att
ATT_analog_t att_analog_b40_up_1,   att_analog_b40_up_2;    // b40 up analog att
ATT_analog_t att_analog_nr_up_1,    att_analog_nr_up_2;     // nr up analog att
ATT_analog_t att_analog_mimo_up_1,  att_analog_mimo_up_2;   // mimo up analog att

// ble config
ble_hw_config_t ble_config;

// 5g modem config
modem_config_t sync_modem_config;

// spi flash
FLASH_25xx_config_t spi_flash;                             

// fpga jtag enable
PORT_t fpga_jtag_en; 

// fpga nStatus
PORT_t fpga_n_status;

// fpga
FPGA_config_t fpga;

// trx8668
TRX8668_FPGA_t h8668_LTE;
TRX8668_FPGA_t h8668_NR;
TRX8668_FPGA_t h8668_NR_MIMO;

// cat1
cat1_ec800_hw_config_t cat1_ec800_hw_config;

// uart
/* 5g modem */
#define MODEM_RX_LEN  300
#define MODEM_TX_LEN  1200
uint8_t modem_name[10] = "modem";       // USART0
//uint8_t moden_ringbuf[MODEM_RX_LEN];    // 环形缓存区
uint8_t modem_rx_buf[MODEM_RX_LEN];
uint8_t modem_tx_buf[MODEM_TX_LEN];
static Cbuffer_T modem_buf;
static uart_data_t modem_data;
uart_func_t uart_modem_fun;
/* cat1 */
uint8_t cat1_name[10] = "cat1";         // USART5
uint8_t cat1_rx_buf[CAT1_RX_DATA_LEN];
uint8_t cat1_tx_buf[CAT1_TX_DATA_LEN];
static Cbuffer_T cat1_buf;
static uart_data_t cat1_data;
uart_func_t uart_cat1_fun;
/* ble */
#define BLE_RX_LEN  512
#define BLE_TX_LEN  512
uint8_t ble_name[10] = "ble1";         // USART2
uint8_t ble_rx_buf[BLE_RX_LEN];
uint8_t ble_tx_buf[BLE_TX_LEN];
static Cbuffer_T ble_buf;
static uart_data_t ble_data;
uart_func_t uart_ble_fun; 

void bsp_register(void)
{
    /*---------------------- uart ----------------------*/
    /*modem uart*/
    memcpy(modem_data.name_des,  modem_name, 10);
    modem_data.rtt_index = RTT_MODEM;
    modem_data.uart_id = USART0;
    modem_data.msg_egp = xUartNewMsgEgp;
    modem_data.msg_egp_bit = BIT_UART0_MSG;
    //modem_data.msg_queue = ;
    //modem_data.msg_queue_len = ;
    modem_data.rx_data_len = MODEM_RX_LEN;
    modem_data.tx_data_len = MODEM_TX_LEN;
    modem_data.pt_rx_data = modem_rx_buf;
    modem_data.pt_tx_data = modem_tx_buf;
    modem_data.tpt_buffer = &modem_buf;
    hi_uart_func_struct_init(&uart_modem_fun, &modem_data);
    uart_modem_fun.init(&modem_data);
    /*cat1 uart*/
    memcpy(cat1_data.name_des,  cat1_name, 10);
    cat1_data.rtt_index = RTT_CAT1;
    cat1_data.uart_id = USART5;
    cat1_data.msg_egp = xUartNewMsgEgp;
    cat1_data.msg_egp_bit = BIT_UART5_MSG;
    //cat1_data.msg_queue = ;
    //cat1_data.msg_queue_len = ;
    cat1_data.rx_data_len = CAT1_RX_DATA_LEN;
    cat1_data.tx_data_len = CAT1_TX_DATA_LEN;
    cat1_data.pt_rx_data = cat1_rx_buf;
    cat1_data.pt_tx_data = cat1_tx_buf;
    cat1_data.tpt_buffer = &cat1_buf;
    hi_uart_func_struct_init(&uart_cat1_fun, &cat1_data);
    uart_cat1_fun.init(&cat1_data);
    /*ble uart*/
    memcpy(ble_data.name_des,  ble_name, 10);
    ble_data.rtt_index = RTT_BLE;
    ble_data.uart_id = USART2;
    ble_data.msg_egp = xUartNewMsgEgp;
    ble_data.msg_egp_bit = BIT_UART2_MSG;
    //ble_data.msg_queue = ;
    //ble_data.msg_queue_len = ;
    ble_data.rx_data_len = BLE_RX_LEN;
    ble_data.tx_data_len = BLE_TX_LEN;
    ble_data.pt_rx_data = ble_rx_buf;
    ble_data.pt_tx_data = ble_tx_buf;
    ble_data.tpt_buffer = &ble_buf;
    hi_uart_func_struct_init(&uart_ble_fun, &ble_data);
    uart_ble_fun.init(&ble_data);
    /*---------------------- led ----------------------*/
    //led_pg8
    memset(&led_pg8, 0, sizeof(LED_t));
    led_pg8.hw_config.led_port = LED_PG8_GPIO_Port;
    led_pg8.hw_config.led_pin = LED_PG8_Pin;
    led_pg8.hw_config.led_init_stat = LED_OFF;
    led_pg8.hw_config.ctl_logic_invert = B_TRUE;
    led_register(&led_pg8);
    //led_pc9
    memset(&led_pc9, 0, sizeof(LED_t));
    led_pc9.hw_config.led_port = LED_PC9_GPIO_Port;
    led_pc9.hw_config.led_pin = LED_PC9_Pin;
    led_pc9.hw_config.led_init_stat = LED_OFF;
    led_pc9.hw_config.ctl_logic_invert = B_TRUE;
    led_register(&led_pc9);
    //led_c11
    memset(&led_c11, 0, sizeof(LED_t));
    led_c11.hw_config.led_port = LED_PC11_GPIO_Port;
    led_c11.hw_config.led_pin = LED_PC11_Pin;
    led_c11.hw_config.led_init_stat = LED_OFF;
    led_c11.hw_config.ctl_logic_invert = B_TRUE;
    led_register(&led_c11);
    //led_pd0
    memset(&led_pd0, 0, sizeof(LED_t));
    led_pd0.hw_config.led_port = LED_PD0_GPIO_Port;
    led_pd0.hw_config.led_pin = LED_PD0_Pin;
    led_pd0.hw_config.led_init_stat = LED_OFF;
    led_pd0.hw_config.ctl_logic_invert = B_TRUE;
    led_register(&led_pd0);
    //led_pd1
    memset(&led_pd1, 0, sizeof(LED_t));
    led_pd1.hw_config.led_port = LED_PD1_GPIO_Port;
    led_pd1.hw_config.led_pin = LED_PD1_Pin;
    led_pd1.hw_config.led_init_stat = LED_OFF;
    led_pd1.hw_config.ctl_logic_invert = B_TRUE;
    led_register(&led_pd1);
    //led_pd2
    memset(&led_pd2, 0, sizeof(LED_t));
    led_pd2.hw_config.led_port = LED_PD2_GPIO_Port;
    led_pd2.hw_config.led_pin = LED_PD2_Pin;
    led_pd2.hw_config.led_init_stat = LED_OFF;
    led_pd2.hw_config.ctl_logic_invert = B_TRUE;
    led_register(&led_pd2);

    /*---------------------- power ctrl ----------------------*/
    /*b3 dc ctrl*/
    memset(&rf_b3_dc_ctrl, 0, sizeof(PORT_t));
    rf_b3_dc_ctrl.hw_config.port_ctl_port = RF_B3_DC_CTRL_GPIO_Port;
    rf_b3_dc_ctrl.hw_config.port_ctl_pin = RF_B3_DC_CTRL_Pin;
    rf_b3_dc_ctrl.hw_config.port_init_stat = PORT_ON;
    port_register(&rf_b3_dc_ctrl);
    BI_PORT_OPEN(&rf_b3_dc_ctrl);
    /*b3 dc ctrl*/
    memset(&rf_b8_dc_ctrl, 0, sizeof(PORT_t));
    rf_b8_dc_ctrl.hw_config.port_ctl_port = RF_B8_DC_CTRL_GPIO_Port;
    rf_b8_dc_ctrl.hw_config.port_ctl_pin = RF_B8_DC_CTRL_Pin;
    rf_b8_dc_ctrl.hw_config.port_init_stat = PORT_ON;
    port_register(&rf_b8_dc_ctrl);
    BI_PORT_OPEN(&rf_b8_dc_ctrl);
    /*b39 dc ctrl*/
    memset(&rf_b39_dc_ctrl, 0, sizeof(PORT_t));
    rf_b39_dc_ctrl.hw_config.port_ctl_port = RF_B39_DC_CTRL_GPIO_Port;
    rf_b39_dc_ctrl.hw_config.port_ctl_pin = RF_B39_DC_CTRL_Pin;
    rf_b39_dc_ctrl.hw_config.port_init_stat = PORT_ON;
    port_register(&rf_b39_dc_ctrl);
    BI_PORT_OPEN(&rf_b39_dc_ctrl);
    /*b40 dc ctrl*/
    memset(&rf_b40_dc_ctrl, 0, sizeof(PORT_t));
    rf_b40_dc_ctrl.hw_config.port_ctl_port = RF_B40_DC_CTRL_GPIO_Port;
    rf_b40_dc_ctrl.hw_config.port_ctl_pin = RF_B40_DC_CTRL_Pin;
    rf_b40_dc_ctrl.hw_config.port_init_stat = PORT_ON;
    port_register(&rf_b40_dc_ctrl);
    BI_PORT_OPEN(&rf_b40_dc_ctrl);
    /*4g dc en*/
    memset(&rf_4g_dc_en, 0, sizeof(PORT_t));
    rf_4g_dc_en.hw_config.port_ctl_port = RF_4G_DC_EN_CTRL_GPIO_Port;
    rf_4g_dc_en.hw_config.port_ctl_pin = RF_4G_DC_EN_CTRL_Pin;
    rf_4g_dc_en.hw_config.port_init_stat = PORT_OFF;
    port_register(&rf_4g_dc_en);
    BI_PORT_OPEN(&rf_4g_dc_en);
    /*nr dc en*/
    memset(&rf_nr_dc_en, 0, sizeof(PORT_t));
    rf_nr_dc_en.hw_config.port_ctl_port = RF_NR_DC_EN_CTRL_GPIO_Port;
    rf_nr_dc_en.hw_config.port_ctl_pin = RF_NR_DC_EN_CTRL_Pin;
    rf_nr_dc_en.hw_config.port_init_stat = PORT_OFF;
    port_register(&rf_nr_dc_en);
    BI_PORT_OPEN(&rf_nr_dc_en);
    /*mimo dc en*/
    memset(&rf_mimo_dc_en, 0, sizeof(PORT_t));
    rf_mimo_dc_en.hw_config.port_ctl_port = RF_MIMO_DC_EN_CTRL_GPIO_Port;
    rf_mimo_dc_en.hw_config.port_ctl_pin = RF_MIMO_DC_EN_CTRL_Pin;
    rf_mimo_dc_en.hw_config.port_init_stat = PORT_OFF;
    port_register(&rf_mimo_dc_en);
    BI_PORT_OPEN(&rf_mimo_dc_en);

    /*---------------------- 5g up silent ctrl ----------------------*/
    /*nr up silent ctrl*/
    memset(&rf_nr_up_silent_ctrl, 0, sizeof(PORT_t));
    rf_nr_up_silent_ctrl.hw_config.port_ctl_port = RF_NR_UP_SILENT_CTRL_GPIO_Port;
    rf_nr_up_silent_ctrl.hw_config.port_ctl_pin = RF_NR_UP_SILENT_CTRL_Pin;
    rf_nr_up_silent_ctrl.hw_config.port_init_stat = PORT_ON;
    port_register(&rf_nr_up_silent_ctrl);
    BI_PORT_OPEN(&rf_nr_up_silent_ctrl);
    /*mimo up silent ctrl*/
    memset(&rf_mimo_up_silent_ctrl, 0, sizeof(PORT_t));
    rf_mimo_up_silent_ctrl.hw_config.port_ctl_port = RF_MIMO_UP_SILENT_CTRL_GPIO_Port;
    rf_mimo_up_silent_ctrl.hw_config.port_ctl_pin = RF_MIMO_UP_SILENT_CTRL_Pin;
    rf_mimo_up_silent_ctrl.hw_config.port_init_stat = PORT_ON;
    port_register(&rf_mimo_up_silent_ctrl);
    BI_PORT_OPEN(&rf_mimo_up_silent_ctrl);

    /*---------------------- ecr8668 ----------------------*/
    /*4g dw 8668 rst ctrl*/
    memset(&ecr8668_4g_dw_rst, 0, sizeof(PORT_t));
    ecr8668_4g_dw_rst.hw_config.port_ctl_port = ECR8668_4G_DW_RST_GPIO_Port;
    ecr8668_4g_dw_rst.hw_config.port_ctl_pin = ECR8668_4G_DW_RST_Pin;
    ecr8668_4g_dw_rst.hw_config.port_init_stat = PORT_ON;
    port_register(&ecr8668_4g_dw_rst);
    BI_PORT_OPEN(&ecr8668_4g_dw_rst);
    /*nr 8668 rst ctrl*/
    memset(&ecr8668_nr_rst, 0, sizeof(PORT_t));
    ecr8668_nr_rst.hw_config.port_ctl_port = ECR8668_NR_RST_GPIO_Port;
    ecr8668_nr_rst.hw_config.port_ctl_pin = ECR8668_NR_RST_Pin;
    ecr8668_nr_rst.hw_config.port_init_stat = PORT_ON;
    port_register(&ecr8668_nr_rst);
    BI_PORT_OPEN(&ecr8668_nr_rst);
    /*mimo 8668 rst ctrl*/
    memset(&ecr8668_mimo_rst, 0, sizeof(PORT_t));
    ecr8668_mimo_rst.hw_config.port_ctl_port = ECR8668_MIMO_RST_GPIO_Port;
    ecr8668_mimo_rst.hw_config.port_ctl_pin = ECR8668_MIMO_RST_Pin;
    ecr8668_mimo_rst.hw_config.port_init_stat = PORT_ON;
    port_register(&ecr8668_mimo_rst);
    BI_PORT_OPEN(&ecr8668_mimo_rst);
    
    /*---------------------- lna ----------------------*/
    /*b3 up lna*/
    memset(&lna_b3_up, 0, sizeof(LNA_t));
    lna_b3_up.hw_config.lna_byp_port = LNA_B3_UP_GPIO_Port;
    lna_b3_up.hw_config.lna_byp_pin = LNA_B3_UP_Pin;
    lna_b3_up.hw_config.lna_init_byp_stat = LNA_BYS;
    lna_b3_up.hw_config.byp_logic_invert = B_TRUE;
    lna_b3_up.hw_config.bypass_att = 0;
    //lna_b3_up.hw_config.lna_pwr_port = ;
    //lna_b3_up.hw_config.lna_pwr_pin = ;
    //lna_b3_up.hw_config.lna_init_pwr_stat = PWR_ON;
    //lna_b3_up.hw_config.pwr_logic_invert = B_TRUE;
    lna_register(&lna_b3_up);
    BI_LNA_OPEN(&lna_b3_up);
    /*b3 dw 1 lna*/
    memset(&lna_b3_dw_1, 0, sizeof(LNA_t));
    lna_b3_dw_1.hw_config.lna_byp_port = LNA_B3_DW_1_GPIO_Port;
    lna_b3_dw_1.hw_config.lna_byp_pin = LNA_B3_DW_1_Pin;
    lna_b3_dw_1.hw_config.lna_init_byp_stat = LNA_BYS;
    lna_b3_dw_1.hw_config.byp_logic_invert = B_TRUE;
    lna_b3_dw_1.hw_config.bypass_att = 0;
    //lna_b3_dw_1.hw_config.lna_pwr_port = ;
    //lna_b3_dw_1.hw_config.lna_pwr_pin = ;
    //lna_b3_dw_1.hw_config.lna_init_pwr_stat = PWR_ON;
    //lna_b3_dw_1.hw_config.pwr_logic_invert = B_TRUE;
    lna_register(&lna_b3_dw_1);
    BI_LNA_OPEN(&lna_b3_dw_1);
    /*b3 dw 2 lna*/
    memset(&lna_b3_dw_2, 0, sizeof(LNA_t));
    lna_b3_dw_2.hw_config.lna_byp_port = LNA_B3_DW_2_GPIO_Port;
    lna_b3_dw_2.hw_config.lna_byp_pin = LNA_B3_DW_2_Pin;
    lna_b3_dw_2.hw_config.lna_init_byp_stat = LNA_BYS;
    lna_b3_dw_2.hw_config.byp_logic_invert = B_TRUE;
    lna_b3_dw_2.hw_config.bypass_att = 0;
    //lna_b3_dw_2.hw_config.lna_pwr_port = ;
    //lna_b3_dw_2.hw_config.lna_pwr_pin = ;
    //lna_b3_dw_2.hw_config.lna_init_pwr_stat = PWR_ON;
    //lna_b3_dw_2.hw_config.pwr_logic_invert = B_TRUE;
    lna_register(&lna_b3_dw_2);
    BI_LNA_OPEN(&lna_b3_dw_2);
    /*b8 up lna*/
    memset(&lna_b8_up, 0, sizeof(LNA_t));
    lna_b8_up.hw_config.lna_byp_port = LNA_B8_UP_GPIO_Port;
    lna_b8_up.hw_config.lna_byp_pin = LNA_B8_UP_Pin;
    lna_b8_up.hw_config.lna_init_byp_stat = LNA_BYS;
    lna_b8_up.hw_config.byp_logic_invert = B_TRUE;
    lna_b8_up.hw_config.bypass_att = 0;
    //lna_b8_up.hw_config.lna_pwr_port = ;
    //lna_b8_up.hw_config.lna_pwr_pin = ;
    //lna_b8_up.hw_config.lna_init_pwr_stat = PWR_ON;
    //lna_b8_up.hw_config.pwr_logic_invert = B_TRUE;
    lna_register(&lna_b8_up);
    BI_LNA_OPEN(&lna_b8_up);
    /*b8 dw 1 lna*/
    memset(&lna_b8_dw_1, 0, sizeof(LNA_t));
    lna_b8_dw_1.hw_config.lna_byp_port = LNA_B8_DW_GPIO_Port;
    lna_b8_dw_1.hw_config.lna_byp_pin = LNA_B8_DW_Pin;
    lna_b8_dw_1.hw_config.lna_init_byp_stat = LNA_BYS;
    lna_b8_dw_1.hw_config.byp_logic_invert = B_TRUE;
    lna_b8_dw_1.hw_config.bypass_att = 0;
    //lna_b8_dw_1.hw_config.lna_pwr_port = ;
    //lna_b8_dw_1.hw_config.lna_pwr_pin = ;
    //lna_b8_dw_1.hw_config.lna_init_pwr_stat = PWR_ON;
    //lna_b8_dw_1.hw_config.pwr_logic_invert = B_TRUE;
    lna_register(&lna_b8_dw_1);
    BI_LNA_OPEN(&lna_b8_dw_1);
    /*b39 up lna*/
    memset(&lna_b39_up, 0, sizeof(LNA_t));
    lna_b39_up.hw_config.lna_byp_port = LNA_B39_UP_GPIO_Port;
    lna_b39_up.hw_config.lna_byp_pin = LNA_B39_UP_Pin;
    lna_b39_up.hw_config.lna_init_byp_stat = LNA_BYS;
    lna_b39_up.hw_config.byp_logic_invert = B_TRUE;
    lna_b39_up.hw_config.bypass_att = 0;
    //lna_b39_up.hw_config.lna_pwr_port = ;
    //lna_b39_up.hw_config.lna_pwr_pin = ;
    //lna_b39_up.hw_config.lna_init_pwr_stat = PWR_ON;
    //lna_b39_up.hw_config.pwr_logic_invert = B_TRUE;
    lna_register(&lna_b39_up);
    BI_LNA_OPEN(&lna_b39_up);
    /*b39 dw 1 lna*/
    memset(&lna_b39_dw_1, 0, sizeof(LNA_t));
    lna_b39_dw_1.hw_config.lna_byp_port = LNA_B39_DW_1_GPIO_Port;
    lna_b39_dw_1.hw_config.lna_byp_pin = LNA_B39_DW_1_Pin;
    lna_b39_dw_1.hw_config.lna_init_byp_stat = LNA_BYS;
    lna_b39_dw_1.hw_config.byp_logic_invert = B_TRUE;
    lna_b39_dw_1.hw_config.bypass_att = 0;
    //lna_b39_dw_1.hw_config.lna_pwr_port = ;
    //lna_b39_dw_1.hw_config.lna_pwr_pin = ;
    //lna_b39_dw_1.hw_config.lna_init_pwr_stat = PWR_ON;
    //lna_b39_dw_1.hw_config.pwr_logic_invert = B_TRUE;
    lna_register(&lna_b39_dw_1);
    BI_LNA_OPEN(&lna_b39_dw_1);
    /*b39 dw 2 lna*/
    memset(&lna_b39_dw_2, 0, sizeof(LNA_t));
    lna_b39_dw_2.hw_config.lna_byp_port = LNA_B39_DW_2_GPIO_Port;
    lna_b39_dw_2.hw_config.lna_byp_pin = LNA_B39_DW_2_Pin;
    lna_b39_dw_2.hw_config.lna_init_byp_stat = LNA_BYS;
    lna_b39_dw_2.hw_config.byp_logic_invert = B_TRUE;
    lna_b39_dw_2.hw_config.bypass_att = 0;
    //lna_b39_dw_2.hw_config.lna_pwr_port = ;
    //lna_b39_dw_2.hw_config.lna_pwr_pin = ;
    //lna_b39_dw_2.hw_config.lna_init_pwr_stat = PWR_ON;
    //lna_b39_dw_2.hw_config.pwr_logic_invert = B_TRUE;
    lna_register(&lna_b39_dw_2);
    BI_LNA_OPEN(&lna_b39_dw_2);
    /*b40 up lna*/
    memset(&lna_b40_up, 0, sizeof(LNA_t));
    lna_b40_up.hw_config.lna_byp_port = LNA_B40_UP_GPIO_Port;
    lna_b40_up.hw_config.lna_byp_pin = LNA_B40_UP_Pin;
    lna_b40_up.hw_config.lna_init_byp_stat = LNA_BYS;
    lna_b40_up.hw_config.byp_logic_invert = B_TRUE;
    lna_b40_up.hw_config.bypass_att = 0;
    //lna_b40_up.hw_config.lna_pwr_port = ;
    //lna_b40_up.hw_config.lna_pwr_pin = ;
    //lna_b40_up.hw_config.lna_init_pwr_stat = PWR_ON;
    //lna_b40_up.hw_config.pwr_logic_invert = B_TRUE;
    lna_register(&lna_b40_up);
    BI_LNA_OPEN(&lna_b40_up);
    /*b40 dw 1 lna*/
    memset(&lna_b40_dw_1, 0, sizeof(LNA_t));
    lna_b40_dw_1.hw_config.lna_byp_port = LNA_B40_DW_1_GPIO_Port;
    lna_b40_dw_1.hw_config.lna_byp_pin = LNA_B40_DW_1_Pin;
    lna_b40_dw_1.hw_config.lna_init_byp_stat = LNA_BYS;
    lna_b40_dw_1.hw_config.byp_logic_invert = B_TRUE;
    lna_b40_dw_1.hw_config.bypass_att = 0;
    //lna_b40_dw_1.hw_config.lna_pwr_port = ;
    //lna_b40_dw_1.hw_config.lna_pwr_pin = ;
    //lna_b40_dw_1.hw_config.lna_init_pwr_stat = PWR_ON;
    //lna_b40_dw_1.hw_config.pwr_logic_invert = B_TRUE;
    lna_register(&lna_b40_dw_1);
    BI_LNA_OPEN(&lna_b40_dw_1);
    /*b40 dw 2 lna*/
    memset(&lna_b40_dw_2, 0, sizeof(LNA_t));
    lna_b40_dw_2.hw_config.lna_byp_port = LNA_B40_DW_2_GPIO_Port;
    lna_b40_dw_2.hw_config.lna_byp_pin = LNA_B40_DW_2_Pin;
    lna_b40_dw_2.hw_config.lna_init_byp_stat = LNA_BYS;
    lna_b40_dw_2.hw_config.byp_logic_invert = B_TRUE;
    lna_b40_dw_2.hw_config.bypass_att = 0;
    //lna_b40_dw_2.hw_config.lna_pwr_port = ;
    //lna_b40_dw_2.hw_config.lna_pwr_pin = ;
    //lna_b40_dw_2.hw_config.lna_init_pwr_stat = PWR_ON;
    //lna_b40_dw_2.hw_config.pwr_logic_invert = B_TRUE;
    lna_register(&lna_b40_dw_2);
    BI_LNA_OPEN(&lna_b40_dw_2);
    /*nr up lna*/
    memset(&lna_nr_up, 0, sizeof(LNA_t));
    lna_nr_up.hw_config.lna_byp_port = LNA_NR_UP_GPIO_Port;
    lna_nr_up.hw_config.lna_byp_pin = LNA_NR_UP_Pin;
    lna_nr_up.hw_config.lna_init_byp_stat = LNA_BYS;
    lna_nr_up.hw_config.byp_logic_invert = B_TRUE;
    lna_nr_up.hw_config.bypass_att = 0;
    //lna_nr_up.hw_config.lna_pwr_port = ;
    //lna_nr_up.hw_config.lna_pwr_pin = ;
    //lna_nr_up.hw_config.lna_init_pwr_stat = PWR_ON;
    //lna_nr_up.hw_config.pwr_logic_invert = B_TRUE;
    lna_register(&lna_nr_up);
    BI_LNA_OPEN(&lna_nr_up);
    /*nr dw 1 lna*/
    memset(&lna_nr_dw_1, 0, sizeof(LNA_t));
    lna_nr_dw_1.hw_config.lna_byp_port = LNA_NR_DW_1_GPIO_Port;
    lna_nr_dw_1.hw_config.lna_byp_pin = LNA_NR_DW_1_Pin;
    lna_nr_dw_1.hw_config.lna_init_byp_stat = LNA_BYS;
    lna_nr_dw_1.hw_config.byp_logic_invert = B_TRUE;
    lna_nr_dw_1.hw_config.bypass_att = 0;
    //lna_nr_dw_1.hw_config.lna_pwr_port = ;
    //lna_nr_dw_1.hw_config.lna_pwr_pin = ;
    //lna_nr_dw_1.hw_config.lna_init_pwr_stat = PWR_ON;
    //lna_nr_dw_1.hw_config.pwr_logic_invert = B_TRUE;
    lna_register(&lna_nr_dw_1);
    BI_LNA_OPEN(&lna_nr_dw_1);
    /*nr dw 2 lna*/
    memset(&lna_nr_dw_2, 0, sizeof(LNA_t));
    lna_nr_dw_2.hw_config.lna_byp_port = LNA_NR_DW_2_GPIO_Port;
    lna_nr_dw_2.hw_config.lna_byp_pin = LNA_NR_DW_2_Pin;
    lna_nr_dw_2.hw_config.lna_init_byp_stat = LNA_BYS;
    lna_nr_dw_2.hw_config.byp_logic_invert = B_TRUE;
    lna_nr_dw_2.hw_config.bypass_att = 0;
    //lna_nr_dw_2.hw_config.lna_pwr_port = ;
    //lna_nr_dw_2.hw_config.lna_pwr_pin = ;
    //lna_nr_dw_2.hw_config.lna_init_pwr_stat = PWR_ON;
    //lna_nr_dw_2.hw_config.pwr_logic_invert = B_TRUE;
    lna_register(&lna_nr_dw_2);
    BI_LNA_OPEN(&lna_nr_dw_2);
    /*mimo up lna*/
    memset(&lna_mimo_up, 0, sizeof(LNA_t));
    lna_mimo_up.hw_config.lna_byp_port = LNA_MIMO_UP_GPIO_Port;
    lna_mimo_up.hw_config.lna_byp_pin = LNA_MIMO_UP_Pin;
    lna_mimo_up.hw_config.lna_init_byp_stat = LNA_BYS;
    lna_mimo_up.hw_config.byp_logic_invert = B_TRUE;
    lna_mimo_up.hw_config.bypass_att = 0;
    //lna_mimo_up.hw_config.lna_pwr_port = ;
    //lna_mimo_up.hw_config.lna_pwr_pin = ;
    //lna_mimo_up.hw_config.lna_init_pwr_stat = PWR_ON;
    //lna_mimo_up.hw_config.pwr_logic_invert = B_TRUE;
    lna_register(&lna_mimo_up);
    BI_LNA_OPEN(&lna_mimo_up);
    /*mimo dw 1 lna*/
    memset(&lna_mimo_dw_1, 0, sizeof(LNA_t));
    lna_mimo_dw_1.hw_config.lna_byp_port = LNA_MIMO_DW_1_GPIO_Port;
    lna_mimo_dw_1.hw_config.lna_byp_pin = LNA_MIMO_DW_1_Pin;
    lna_mimo_dw_1.hw_config.lna_init_byp_stat = LNA_BYS;
    lna_mimo_dw_1.hw_config.byp_logic_invert = B_TRUE;
    lna_mimo_dw_1.hw_config.bypass_att = 0;
    //lna_mimo_dw_1.hw_config.lna_pwr_port = ;
    //lna_mimo_dw_1.hw_config.lna_pwr_pin = ;
    //lna_mimo_dw_1.hw_config.lna_init_pwr_stat = PWR_ON;
    //lna_mimo_dw_1.hw_config.pwr_logic_invert = B_TRUE;
    lna_register(&lna_mimo_dw_1);
    BI_LNA_OPEN(&lna_mimo_dw_1);
    /*mimo dw 2 lna*/
    memset(&lna_mimo_dw_2, 0, sizeof(LNA_t));
    lna_mimo_dw_2.hw_config.lna_byp_port = LNA_MIMO_DW_2_GPIO_Port;
    lna_mimo_dw_2.hw_config.lna_byp_pin = LNA_MIMO_DW_2_Pin;
    lna_mimo_dw_2.hw_config.lna_init_byp_stat = LNA_BYS;
    lna_mimo_dw_2.hw_config.byp_logic_invert = B_TRUE;
    lna_mimo_dw_2.hw_config.bypass_att = 0;
    //lna_mimo_dw_2.hw_config.lna_pwr_port = ;
    //lna_mimo_dw_2.hw_config.lna_pwr_pin = ;
    //lna_mimo_dw_2.hw_config.lna_init_pwr_stat = PWR_ON;
    //lna_mimo_dw_2.hw_config.pwr_logic_invert = B_TRUE;
    lna_register(&lna_mimo_dw_2);
    BI_LNA_OPEN(&lna_mimo_dw_2);
    
    /*---------------------- spdt switch ----------------------*/
    /*b3_b39 up ctrl*/
    memset(&spdt_b3_b39_up_ctrl, 0, sizeof(Switch_spdt_t));
    spdt_b3_b39_up_ctrl.hw_config.switch_ctl_port = SPDT_B3_B39_UP_CTRL_GPIO_Port;
    spdt_b3_b39_up_ctrl.hw_config.switch_ctl_pin = SPDT_B3_B39_UP_CTRL_Pin;
    spdt_b3_b39_up_ctrl.hw_config.switch_init_stat = SW_CH2;
    switch_spdt_register(&spdt_b3_b39_up_ctrl);
    BI_Switch_OPEN(&spdt_b3_b39_up_ctrl);
    /*b3_b39 dw ctrl*/
    memset(&spdt_b3_b39_dw_ctrl, 0, sizeof(Switch_spdt_t));
    spdt_b3_b39_dw_ctrl.hw_config.switch_ctl_port = SPDT_B3_B39_DW_CTRL_GPIO_Port;
    spdt_b3_b39_dw_ctrl.hw_config.switch_ctl_pin = SPDT_B3_B39_DW_CTRL_Pin;
    spdt_b3_b39_dw_ctrl.hw_config.switch_init_stat = SW_CH1;
    switch_spdt_register(&spdt_b3_b39_dw_ctrl);
    BI_Switch_OPEN(&spdt_b3_b39_dw_ctrl);
    
    /*---------------------- sp4t switch ----------------------*/
    /*4g dw rx ctrl*/
    memset(&sp4t_4g_dw_rx, 0, sizeof(PORT_t));
    sp4t_4g_dw_rx.hw_config.switch_v2_port = SP4T_4G_DW_RX_V2_GPIO_Port;
    sp4t_4g_dw_rx.hw_config.switch_v2_pin = SP4T_4G_DW_RX_V2_Pin;
    sp4t_4g_dw_rx.hw_config.switch_v3_port = SP4T_4G_DW_RX_V3_GPIO_Port;
    sp4t_4g_dw_rx.hw_config.switch_v3_pin = SP4T_4G_DW_RX_V3_Pin;
    sp4t_4g_dw_rx.hw_config.switch_init_stat = SW_CH1;
    switch_sp4t_register(&sp4t_4g_dw_rx);
    BI_Switch_OPEN(&sp4t_4g_dw_rx);
    /*4g dw tx ctrl*/
    memset(&sp4t_4g_dw_tx, 0, sizeof(PORT_t));
    sp4t_4g_dw_tx.hw_config.switch_v2_port = SP4T_4G_DW_TX_V2_GPIO_Port;
    sp4t_4g_dw_tx.hw_config.switch_v2_pin = SP4T_4G_DW_TX_V2_Pin;
    sp4t_4g_dw_tx.hw_config.switch_v3_port = SP4T_4G_DW_TX_V3_GPIO_Port;
    sp4t_4g_dw_tx.hw_config.switch_v3_pin = SP4T_4G_DW_TX_V3_Pin;
    sp4t_4g_dw_tx.hw_config.switch_init_stat = SW_CH1;
    switch_sp4t_register(&sp4t_4g_dw_tx);
    BI_Switch_OPEN(&sp4t_4g_dw_tx);
    /*4g ant ctrl*/
    memset(&sp4t_4g_ant, 0, sizeof(PORT_t));
    sp4t_4g_ant.hw_config.switch_v2_port = SP4T_4G_ANT_V2_GPIO_Port;
    sp4t_4g_ant.hw_config.switch_v2_pin = SP4T_4G_ANT_V2_Pin;
    sp4t_4g_ant.hw_config.switch_v3_port = SP4T_4G_ANT_V3_GPIO_Port;
    sp4t_4g_ant.hw_config.switch_v3_pin = SP4T_4G_ANT_V3_Pin;
    sp4t_4g_ant.hw_config.switch_init_stat = SW_CH1;
    switch_sp4t_register(&sp4t_4g_ant);
    BI_Switch_OPEN(&sp4t_4g_ant);
    /*4g bts ctrl*/
    memset(&sp4t_4g_bts, 0, sizeof(PORT_t));
    sp4t_4g_bts.hw_config.switch_v2_port = SP4T_4G_BTS_V2_GPIO_Port;
    sp4t_4g_bts.hw_config.switch_v2_pin = SP4T_4G_BTS_V2_Pin;
    sp4t_4g_bts.hw_config.switch_v3_port = SP4T_4G_BTS_V3_GPIO_Port;
    sp4t_4g_bts.hw_config.switch_v3_pin = SP4T_4G_BTS_V3_Pin;
    sp4t_4g_bts.hw_config.switch_init_stat = SW_CH1;
    switch_sp4t_register(&sp4t_4g_bts);
    BI_Switch_OPEN(&sp4t_4g_bts);
    
    /*---------------------- analog att ----------------------*/
    /*4g dw analog att*/
    memset(&att_analog_4g_dw, 0, sizeof(ATT_analog_t));
    att_analog_4g_dw.hw_config.tim_ch = HTIM2;
    att_analog_4g_dw.hw_config.pwm_ch = PWMCH1;
    att_analog_4g_dw.hw_config.att_min_number = analog_table_4g_dw.min_number;
    att_analog_4g_dw.hw_config.att_max_number = analog_table_4g_dw.valid_number;
    att_analog_4g_dw.hw_config.att_table = analog_table_4g_dw.table;
    att_analog_register(&att_analog_4g_dw);
    /*nr dw tx analog att*/
    memset(&att_analog_nr_dw, 0, sizeof(ATT_analog_t));
    att_analog_nr_dw.hw_config.tim_ch = HTIM2;
    att_analog_nr_dw.hw_config.pwm_ch = PWMCH0;
    att_analog_nr_dw.hw_config.att_min_number = analog_table_nr_dw.min_number;
    att_analog_nr_dw.hw_config.att_max_number = analog_table_nr_dw.valid_number;
    att_analog_nr_dw.hw_config.att_table = analog_table_nr_dw.table;
    att_analog_register(&att_analog_nr_dw);
    /*mimo dw tx analog att*/
    memset(&att_analog_mimo_dw, 0, sizeof(ATT_analog_t));
    att_analog_mimo_dw.hw_config.tim_ch = HTIM1;
    att_analog_mimo_dw.hw_config.pwm_ch = PWMCH0;
    att_analog_mimo_dw.hw_config.att_min_number = analog_table_mimo_dw.min_number;
    att_analog_mimo_dw.hw_config.att_max_number = analog_table_mimo_dw.valid_number;
    att_analog_mimo_dw.hw_config.att_table = analog_table_mimo_dw.table;
    att_analog_register(&att_analog_mimo_dw);
    
    /*b3 up 1 analog att*/
    memset(&att_analog_b3_up_1, 0, sizeof(ATT_analog_t));
    att_analog_b3_up_1.hw_config.tim_ch = HTIM0;
    att_analog_b3_up_1.hw_config.pwm_ch = PWMCH0;
    att_analog_b3_up_1.hw_config.att_min_number = analog_table_4g_up_1.min_number;
    att_analog_b3_up_1.hw_config.att_max_number = analog_table_4g_up_1.valid_number;
    att_analog_b3_up_1.hw_config.att_table = analog_table_4g_up_1.table;
    att_analog_register(&att_analog_b3_up_1);
    /*b3 up 2 analog att*/
    memset(&att_analog_b3_up_2, 0, sizeof(ATT_analog_t));
    att_analog_b3_up_2.hw_config.tim_ch = HTIM1;
    att_analog_b3_up_2.hw_config.pwm_ch = PWMCH3;
    att_analog_b3_up_2.hw_config.att_min_number = analog_table_4g_up_2.min_number;
    att_analog_b3_up_2.hw_config.att_max_number = analog_table_4g_up_2.valid_number;
    att_analog_b3_up_2.hw_config.att_table = analog_table_4g_up_2.table;
    att_analog_register(&att_analog_b3_up_2);
    /*b8 up 1 analog att*/
    memset(&att_analog_b8_up_1, 0, sizeof(ATT_analog_t));
    att_analog_b8_up_1.hw_config.tim_ch = HTIM0;
    att_analog_b8_up_1.hw_config.pwm_ch = PWMCH3;
    att_analog_b8_up_1.hw_config.att_min_number = analog_table_4g_up_1.min_number;
    att_analog_b8_up_1.hw_config.att_max_number = analog_table_4g_up_1.valid_number;
    att_analog_b8_up_1.hw_config.att_table = analog_table_4g_up_1.table;
    att_analog_register(&att_analog_b8_up_1);
    /*b8 up 2 analog att*/
    memset(&att_analog_b8_up_2, 0, sizeof(ATT_analog_t));
    att_analog_b8_up_2.hw_config.tim_ch = HTIM1;
    att_analog_b8_up_2.hw_config.pwm_ch = PWMCH2;
    att_analog_b8_up_2.hw_config.att_min_number = analog_table_4g_up_2.min_number;
    att_analog_b8_up_2.hw_config.att_max_number = analog_table_4g_up_2.valid_number;
    att_analog_b8_up_2.hw_config.att_table = analog_table_4g_up_2.table;
    att_analog_register(&att_analog_b8_up_2);
    /*b39 up 1 analog att*/
    memset(&att_analog_b39_up_1, 0, sizeof(ATT_analog_t));
    att_analog_b39_up_1.hw_config.tim_ch = HTIM0;
    att_analog_b39_up_1.hw_config.pwm_ch = PWMCH1;
    att_analog_b39_up_1.hw_config.att_min_number = analog_table_4g_up_1.min_number;
    att_analog_b39_up_1.hw_config.att_max_number = analog_table_4g_up_1.valid_number;
    att_analog_b39_up_1.hw_config.att_table = analog_table_4g_up_1.table;
    att_analog_register(&att_analog_b39_up_1);
    /*b39 up 2 analog att*/
    memset(&att_analog_b39_up_2, 0, sizeof(ATT_analog_t));
    att_analog_b39_up_2.hw_config.tim_ch = HTIM0;
    att_analog_b39_up_2.hw_config.pwm_ch = PWMCH2;
    att_analog_b39_up_2.hw_config.att_min_number = analog_table_4g_up_2.min_number;
    att_analog_b39_up_2.hw_config.att_max_number = analog_table_4g_up_2.valid_number;
    att_analog_b39_up_2.hw_config.att_table = analog_table_4g_up_2.table;
    att_analog_register(&att_analog_b39_up_2);
    /*b40 up 1 analog att*/
    memset(&att_analog_b40_up_1, 0, sizeof(ATT_analog_t));
    att_analog_b40_up_1.hw_config.tim_ch = HTIM3;
    att_analog_b40_up_1.hw_config.pwm_ch = PWMCH2;
    att_analog_b40_up_1.hw_config.att_min_number = analog_table_4g_up_1.min_number;
    att_analog_b40_up_1.hw_config.att_max_number = analog_table_4g_up_1.valid_number;
    att_analog_b40_up_1.hw_config.att_table = analog_table_4g_up_1.table;
    att_analog_register(&att_analog_b40_up_1);
    /*b40 up 2 analog att*/
    memset(&att_analog_b40_up_2, 0, sizeof(ATT_analog_t));
    att_analog_b40_up_2.hw_config.tim_ch = HTIM1;
    att_analog_b40_up_2.hw_config.pwm_ch = PWMCH1;
    att_analog_b40_up_2.hw_config.att_min_number = analog_table_4g_up_2.min_number;
    att_analog_b40_up_2.hw_config.att_max_number = analog_table_4g_up_2.valid_number;
    att_analog_b40_up_2.hw_config.att_table = analog_table_4g_up_2.table;
    att_analog_register(&att_analog_b40_up_2);
    /*nr up 1 analog att*/
    memset(&att_analog_nr_up_1, 0, sizeof(ATT_analog_t));
    att_analog_nr_up_1.hw_config.tim_ch = HTIM4;
    att_analog_nr_up_1.hw_config.pwm_ch = PWMCH0;
    att_analog_nr_up_1.hw_config.att_min_number = analog_table_nr_up_1.min_number;
    att_analog_nr_up_1.hw_config.att_max_number = analog_table_nr_up_1.valid_number;
    att_analog_nr_up_1.hw_config.att_table = analog_table_nr_up_1.table;
    att_analog_register(&att_analog_nr_up_1);
    /*nr up 2 analog att*/
    memset(&att_analog_nr_up_2, 0, sizeof(ATT_analog_t));
    att_analog_nr_up_2.hw_config.tim_ch = HTIM4;
    att_analog_nr_up_2.hw_config.pwm_ch = PWMCH1;
    att_analog_nr_up_2.hw_config.att_min_number = analog_table_nr_up_2.min_number;
    att_analog_nr_up_2.hw_config.att_max_number = analog_table_nr_up_2.valid_number;
    att_analog_nr_up_2.hw_config.att_table = analog_table_nr_up_2.table;
    att_analog_register(&att_analog_nr_up_2);
    /*mimo up 1 analog att*/
    memset(&att_analog_mimo_up_1, 0, sizeof(ATT_analog_t));
    att_analog_mimo_up_1.hw_config.tim_ch = HTIM3;
    att_analog_mimo_up_1.hw_config.pwm_ch = PWMCH3;
    att_analog_mimo_up_1.hw_config.att_min_number = analog_table_mimo_up_1.min_number;
    att_analog_mimo_up_1.hw_config.att_max_number = analog_table_mimo_up_1.valid_number;
    att_analog_mimo_up_1.hw_config.att_table = analog_table_mimo_up_1.table;
    att_analog_register(&att_analog_mimo_up_1);
    /*mimo up 2 analog att*/
    memset(&att_analog_mimo_up_2, 0, sizeof(ATT_analog_t));
    att_analog_mimo_up_2.hw_config.tim_ch = HTIM2;
    att_analog_mimo_up_2.hw_config.pwm_ch = PWMCH2;
    att_analog_mimo_up_2.hw_config.att_min_number = analog_table_mimo_up_2.min_number;
    att_analog_mimo_up_2.hw_config.att_max_number = analog_table_mimo_up_2.valid_number;
    att_analog_mimo_up_2.hw_config.att_table = analog_table_mimo_up_2.table;
    att_analog_register(&att_analog_mimo_up_2);

    /*---------------------- ble config ----------------------*/
    memset(&ble_config, 0, sizeof(ble_hw_config_t));
    ble_config.ble_dc_ctrl.hw_config.port_ctl_port = BLE_DC_CTRL_GPIO_Port;
    ble_config.ble_dc_ctrl.hw_config.port_ctl_pin = BLE_DC_CTRL_Pin;
    ble_config.ble_dc_ctrl.hw_config.port_init_stat = PORT_ON;
    ble_config.ble_uart = &uart_ble_fun;
    port_register(&ble_config.ble_dc_ctrl);
    BI_PORT_OPEN(&ble_config.ble_dc_ctrl);
    ble_register(&ble_config);
    
    /*----------------------  sync modem  ----------------------*/
    sync_modem_config.dc_ctrl_pin = MODEM_DC_CTRL_Pin;
    sync_modem_config.dc_ctrl_port = MODEM_DC_CTRL_GPIO_Port;
    sync_modem_config.sync_pin = MODEM_SYN_Pin;
    sync_modem_config.sync_port = MODEM_SYN_GPIO_Port;
    sync_modem_config.modem_uart = &uart_modem_fun;
    sync_modem_config.lte_gpio_group = 1;//0
    sync_modem_config.nr_gpio_group = 0;  //1
    sync_modem_config.nr2_gpio_group = 2;
    modem_register(&sync_modem_config);

    /*----------------------flash spi0 ----------------------*/
    spi_flash.spi_ch = SPI_0;
    spi_flash.cs_port = FLASH_CS_GPIO_Port;
    spi_flash.cs_pin = FLASH_CS_Pin;
    spi_flash.page_size = FLASH_PAGE_SIZE;
    spi_flash.sector_size = FLASH_SECTOR_SIZE;
    spi_flash.sector_num = FLASH_SECTOR_NUM;
    spi_flash.flash_size = FLASH_SIZE;
    spi_flash.lock = UNLOCK;
    flash_25xx_register(&spi_flash);
    
    /*---------------------- fpga jtag enable ----------------------*/
    /*fpga jtag enable*/
    memset(&fpga_jtag_en, 0, sizeof(PORT_t));
    fpga_jtag_en.hw_config.port_ctl_port = FPGA_JTAG_EN_GPIO_Port;
    fpga_jtag_en.hw_config.port_ctl_pin = FPGA_JTAG_EN_Pin;
    fpga_jtag_en.hw_config.port_init_stat = PORT_ON;
    port_register(&fpga_jtag_en);
    BI_PORT_OPEN(&fpga_jtag_en);
    
    /*---------------------- fpga nStatus ----------------------*/
    /*fpga nStatus*/
    memset(&fpga_n_status, 0, sizeof(PORT_t));
    fpga_n_status.hw_config.port_ctl_port = FPGA_N_STATUS_GPIO_Port;
    fpga_n_status.hw_config.port_ctl_pin = FPGA_N_STATUS_Pin;
    fpga_n_status.hw_config.port_init_stat = PORT_ON;
    port_register(&fpga_n_status);
    BI_PORT_OPEN(&fpga_n_status);
    
    /*---------------------- fpga ----------------------*/
    fpga.dc_ctrl_port = FPGA_DC_CTRL_GPIO_Port; //
    fpga.dc_ctrl_pin = FPGA_DC_CTRL_Pin;
    fpga.ps_nconfig_port = FPGA_N_CONFIG_GPIO_Port;
    fpga.ps_nconfig_pin = FPGA_N_CONFIG_Pin;
    fpga.ps_cs_port = FPGA_PS_CS_GPIO_Port;
    fpga.ps_cs_pin = FPGA_PS_CS_Pin;
    fpga.ps_dclk_port = FPGA_PS_DCLK_GPIO_Port;
    fpga.ps_dclk_pin = FPGA_PS_DCLK_Pin;
    fpga.ps_data_port = FPGA_PS_DATA0_GPIO_Port;
    fpga.ps_data_pin = FPGA_PS_DATA0_Pin;
    fpga.spi_cs_port = FPGA_MCU_CS_GPIO_Port;
    fpga.spi_cs_pin = FPGA_MCU_CS_Pin;
	fpga.rst_n_port = FPGA_MCU_STATUS_GPIO_Port;
	fpga.rst_n_pin = FPGA_MCU_STATUS_Pin;
    fpga.spi = SPI_1;
    fpga_register(&fpga);

    /*---------------------- 8668 ----------------------*/
    memset(&h8668_LTE, 0, sizeof(h8668_LTE));
    h8668_LTE.config.spi_port.fpga_spi_addr = FPGA_OP_DEVICE_TRX_4G; //FPGA分配给8668的地址
    h8668_LTE.config.spi_port.port_type = TYPE_FPGA;//通信方式
    h8668_LTE.config.rst_pin = ECR8668_4G_DW_RST_Pin;
    h8668_LTE.config.rst_port = ECR8668_4G_DW_RST_GPIO_Port;
    h8668_LTE.config.ch = 0; // /8668固定只用通道0
    h8668_LTE.config.port = CH0_RXC_TXB; // 所用端口,待定,硬件提供
    h8668_LTE.config.dev_number = RF_SYS_1; // 8668编号,当板上多片8668时以区分设备打印信息
    h8668_LTE.config.adc_irq = ECR8668_4G_DW_ADC_OVERLOAD_EXTI_IRQn; // 8668 adc告警中断
    h8668_LTE.config.tia_irq = ECR8668_4G_DW_TIA_OVERLOAD_EXTI_IRQn; // 8668 tia告警中断

    trx8668_fpga_register(&h8668_LTE);

    memset(&h8668_NR, 0, sizeof(h8668_NR));
    h8668_NR.config.spi_port.fpga_spi_addr = FPGA_OP_DEVICE_TRX_5G1; //FPGA分配给8668的地址
    h8668_NR.config.spi_port.port_type = TYPE_FPGA;//通信方式
    h8668_NR.config.rst_pin = ECR8668_NR_RST_Pin;
    h8668_NR.config.rst_port = ECR8668_NR_RST_GPIO_Port;
    h8668_NR.config.ch = 0; // 8668固定只用通道0
    h8668_NR.config.port = CH0_RXC_TXB; // 所用端口,待定,硬件提供
    h8668_NR.config.dev_number = RF_SYS_2; // 8668编号,当板上多片8668时以区分设备打印信息
    h8668_NR.config.adc_irq = ECR8668_NR_ADC_OVERLOAD_EXTI_IRQn; // 8668 adc告警中断
    h8668_NR.config.tia_irq = ECR8668_NR_TIA_OVERLOAD_EXTI_IRQn; // 8668 tia告警中断

    trx8668_fpga_register(&h8668_NR);

    memset(&h8668_NR_MIMO, 0, sizeof(h8668_NR_MIMO));
    h8668_NR_MIMO.config.spi_port.fpga_spi_addr = FPGA_OP_DEVICE_TRX_5G2; //FPGA分配给8668的地址
    h8668_NR_MIMO.config.spi_port.port_type = TYPE_FPGA;//通信方式
    h8668_NR_MIMO.config.rst_pin = ECR8668_MIMO_RST_Pin;
    h8668_NR_MIMO.config.rst_port = ECR8668_MIMO_RST_GPIO_Port;
    h8668_NR_MIMO.config.ch = 0; // 8668固定只用通道0
    h8668_NR_MIMO.config.port = CH0_RXC_TXB; // 所用端口,待定,硬件提供
    h8668_NR_MIMO.config.dev_number = RF_SYS_3; // 8668编号,当板上多片8668时以区分设备打印信息
    h8668_NR_MIMO.config.adc_irq = ECR8668_MIMO_ADC_OVERLOAD_EXTI_IRQn; // 8668 adc告警中断
    h8668_NR_MIMO.config.tia_irq = ECR8668_MIMO_TIA_OVERLOAD_EXTI_IRQn; // 8668 tia告警中断

    trx8668_fpga_register(&h8668_NR_MIMO);

    /*---------------------- cat1 ----------------------*/
    cat1_ec800_hw_config.cat1_dc_ctrl_port = CAT1_DC_CTRL_GPIO_Port;
    cat1_ec800_hw_config.cat1_dc_ctrl_pin = CAT1_DC_CTRL_Pin;
    cat1_ec800_hw_config.cat1_stat_port = CAT1_STATE_GPIO_Port;
    cat1_ec800_hw_config.cat1_stat_pin = CAT1_STATE_Pin;
    cat1_ec800_hw_config.cat1_open_port = CAT1_OPEN_GPIO_Port;
    cat1_ec800_hw_config.cat1_open_pin = CAT1_OPEN_Pin;
    cat1_ec800_hw_config.cat1_reset_port = CAT1_RESET_GPIO_Port;
    cat1_ec800_hw_config.cat1_reset_pin = CAT1_RESET_Pin;
    cat1_ec800_hw_config.uart_func = &uart_cat1_fun;
    cat1_ec800_register(&cat1_ec800_hw_config);
}

/*****************************************************************************
* @brief  : 
* @param  : None
* @retval : 
* @date   : 2023-11-28
* @note   : 
*****************************************************************************/
void bsp_device_init(void)
{
    //fpga_ps_load();
    spi_flash.flash_open();
}

#elif (PRODUCT_MODEL == CMDT3521B)
// analog att table     /*此结构体需要待频段选择完成后加载数据*/
Analog_table_t analog_table_4g_up_1, analog_table_4g_up_2, analog_table_4g_dw;   
Analog_table_t analog_table_nr_up_1, analog_table_nr_up_2, analog_table_nr_dw;
Analog_table_t analog_table_mimo_up_1, analog_table_mimo_up_2, analog_table_mimo_dw;

// led
LED_t led_pg8, led_pc9, led_c11, led_pd0, led_pd1, led_pd2;

// power crtl
PORT_t rf_b3_dc_ctrl;                                       
PORT_t rf_b8_dc_ctrl;
PORT_t rf_b39_dc_ctrl;
PORT_t rf_b40_dc_ctrl;
PORT_t rf_4g_dc_en;
PORT_t rf_nr_dc_en;
PORT_t rf_mimo_dc_en;

// 5g up silent
PORT_t rf_nr_up_silent_ctrl;                                
PORT_t rf_mimo_up_silent_ctrl;

// 8668 rst
PORT_t ecr8668_4g_dw_rst;
PORT_t ecr8668_nr_rst;           
PORT_t ecr8668_mimo_rst; 

// lna
LNA_t lna_b3_up,    lna_b3_dw_1,    lna_b3_dw_2;            // b3 lna
LNA_t lna_b8_up,    lna_b8_dw_1;                            // b8 lna
LNA_t lna_b39_up,   lna_b39_dw_1,   lna_b39_dw_2;           // b39 lna
LNA_t lna_b40_up,   lna_b40_dw_1,   lna_b40_dw_2;           // b40 lna
LNA_t lna_nr_up,    lna_nr_dw_1,    lna_nr_dw_2;            // nr lna
LNA_t lna_mimo_up,  lna_mimo_dw_1,  lna_mimo_dw_2;          // mimo lna

// spdt switch
Switch_spdt_t spdt_b3_b39_up_ctrl, spdt_b3_b39_dw_ctrl;

// sp4t switch
Switch_sp4t_t sp4t_4g_dw_rx;
Switch_sp4t_t sp4t_4g_dw_tx;
Switch_sp4t_t sp4t_4g_ant;
Switch_sp4t_t sp4t_4g_bts;

// analog att
ATT_analog_t att_analog_4g_dw;                              // 4g dw analog att
ATT_analog_t att_analog_nr_dw;                              // nr dw analog att
ATT_analog_t att_analog_mimo_dw;                            // mimo dw analog att

ATT_analog_t att_analog_b3_up_1,    att_analog_b3_up_2;     // b3 up analog att
ATT_analog_t att_analog_b8_up_1,    att_analog_b8_up_2;     // b8 up analog att
ATT_analog_t att_analog_b39_up_1,   att_analog_b39_up_2;    // b39 up analog att
ATT_analog_t att_analog_b40_up_1,   att_analog_b40_up_2;    // b40 up analog att
ATT_analog_t att_analog_nr_up_1,    att_analog_nr_up_2;     // nr up analog att
ATT_analog_t att_analog_mimo_up_1,  att_analog_mimo_up_2;   // mimo up analog att

// ble config
ble_hw_config_t ble_config;

// 5g modem config
modem_config_t sync_modem_config;

// spi flash
FLASH_25xx_config_t spi_flash;                             

// fpga jtag enable
PORT_t fpga_jtag_en; 

// fpga nStatus
PORT_t fpga_n_status;

// fpga
FPGA_config_t fpga;

// trx8668
TRX8668_FPGA_t h8668_LTE;
TRX8668_FPGA_t h8668_NR;
TRX8668_FPGA_t h8668_NR_MIMO;

// cat1
cat1_n706_hw_config_t cat1_n706_hw_config;
// uart
/* 5g modem */
#define MODEM_RX_LEN  300
#define MODEM_TX_LEN  1200
uint8_t modem_name[10] = "modem";       // USART0
//uint8_t moden_ringbuf[MODEM_RX_LEN];    // 环形缓存区
uint8_t modem_rx_buf[MODEM_RX_LEN];
uint8_t modem_tx_buf[MODEM_TX_LEN];
static Cbuffer_T modem_buf;
static uart_data_t modem_data;
uart_func_t uart_modem_fun;
/* cat1 */
uint8_t cat1_name[10] = "cat1";         // USART5
uint8_t cat1_rx_buf[CAT1_RX_DATA_LEN];
uint8_t cat1_tx_buf[CAT1_TX_DATA_LEN];
static Cbuffer_T cat1_buf;
static uart_data_t cat1_data;
uart_func_t uart_cat1_fun;
/* ble */
#define BLE_RX_LEN  512
#define BLE_TX_LEN  512
uint8_t ble_name[10] = "ble1";         // USART2
uint8_t ble_rx_buf[BLE_RX_LEN];
uint8_t ble_tx_buf[BLE_TX_LEN];
static Cbuffer_T ble_buf;
static uart_data_t ble_data;
uart_func_t uart_ble_fun; 

void bsp_register(void)
{
    /*---------------------- uart ----------------------*/
    /*modem uart*/
    memcpy(modem_data.name_des,  modem_name, 10);
    modem_data.rtt_index = RTT_MODEM;
    modem_data.uart_id = USART0;
    modem_data.msg_egp = xUartNewMsgEgp;
    modem_data.msg_egp_bit = BIT_UART0_MSG;
    //modem_data.msg_queue = ;
    //modem_data.msg_queue_len = ;
    modem_data.rx_data_len = MODEM_RX_LEN;
    modem_data.tx_data_len = MODEM_TX_LEN;
    modem_data.pt_rx_data = modem_rx_buf;
    modem_data.pt_tx_data = modem_tx_buf;
    modem_data.tpt_buffer = &modem_buf;
    hi_uart_func_struct_init(&uart_modem_fun, &modem_data);
    uart_modem_fun.init(&modem_data);
    /*cat1 uart*/
    memcpy(cat1_data.name_des,  cat1_name, 10);
    cat1_data.rtt_index = RTT_CAT1;
    cat1_data.uart_id = USART5;
    cat1_data.msg_egp = xUartNewMsgEgp;
    cat1_data.msg_egp_bit = BIT_UART5_MSG;
    //cat1_data.msg_queue = ;
    //cat1_data.msg_queue_len = ;
    cat1_data.rx_data_len = CAT1_RX_DATA_LEN;
    cat1_data.tx_data_len = CAT1_TX_DATA_LEN;
    cat1_data.pt_rx_data = cat1_rx_buf;
    cat1_data.pt_tx_data = cat1_tx_buf;
    cat1_data.tpt_buffer = &cat1_buf;
    hi_uart_func_struct_init(&uart_cat1_fun, &cat1_data);
    uart_cat1_fun.init(&cat1_data);
    /*ble uart*/
    memcpy(ble_data.name_des,  ble_name, 10);
    ble_data.rtt_index = RTT_BLE;
    ble_data.uart_id = USART2;
    ble_data.msg_egp = xUartNewMsgEgp;
    ble_data.msg_egp_bit = BIT_UART2_MSG;
    //ble_data.msg_queue = ;
    //ble_data.msg_queue_len = ;
    ble_data.rx_data_len = BLE_RX_LEN;
    ble_data.tx_data_len = BLE_TX_LEN;
    ble_data.pt_rx_data = ble_rx_buf;
    ble_data.pt_tx_data = ble_tx_buf;
    ble_data.tpt_buffer = &ble_buf;
    hi_uart_func_struct_init(&uart_ble_fun, &ble_data);
    uart_ble_fun.init(&ble_data);
    
    /*---------------------- led ----------------------*/
    //led_pg8
    memset(&led_pg8, 0, sizeof(LED_t));
    led_pg8.hw_config.led_port = LED_PG8_GPIO_Port;
    led_pg8.hw_config.led_pin = LED_PG8_Pin;
    led_pg8.hw_config.led_init_stat = LED_OFF;
    led_pg8.hw_config.ctl_logic_invert = B_TRUE;
    led_register(&led_pg8);
    //led_pc9
    memset(&led_pc9, 0, sizeof(LED_t));
    led_pc9.hw_config.led_port = LED_PC9_GPIO_Port;
    led_pc9.hw_config.led_pin = LED_PC9_Pin;
    led_pc9.hw_config.led_init_stat = LED_OFF;
    led_pc9.hw_config.ctl_logic_invert = B_TRUE;
    led_register(&led_pc9);
    //led_c11
    memset(&led_c11, 0, sizeof(LED_t));
    led_c11.hw_config.led_port = LED_PC11_GPIO_Port;
    led_c11.hw_config.led_pin = LED_PC11_Pin;
    led_c11.hw_config.led_init_stat = LED_OFF;
    led_c11.hw_config.ctl_logic_invert = B_TRUE;
    led_register(&led_c11);
    //led_pd0
    memset(&led_pd0, 0, sizeof(LED_t));
    led_pd0.hw_config.led_port = LED_PD0_GPIO_Port;
    led_pd0.hw_config.led_pin = LED_PD0_Pin;
    led_pd0.hw_config.led_init_stat = LED_OFF;
    led_pd0.hw_config.ctl_logic_invert = B_TRUE;
    led_register(&led_pd0);
    //led_pd1
    memset(&led_pd1, 0, sizeof(LED_t));
    led_pd1.hw_config.led_port = LED_PD1_GPIO_Port;
    led_pd1.hw_config.led_pin = LED_PD1_Pin;
    led_pd1.hw_config.led_init_stat = LED_OFF;
    led_pd1.hw_config.ctl_logic_invert = B_TRUE;
    led_register(&led_pd1);
    //led_pd2
    memset(&led_pd2, 0, sizeof(LED_t));
    led_pd2.hw_config.led_port = LED_PD2_GPIO_Port;
    led_pd2.hw_config.led_pin = LED_PD2_Pin;
    led_pd2.hw_config.led_init_stat = LED_OFF;
    led_pd2.hw_config.ctl_logic_invert = B_TRUE;
    led_register(&led_pd2);

    /*---------------------- power ctrl ----------------------*/
    /*b3 dc ctrl*/
    memset(&rf_b3_dc_ctrl, 0, sizeof(PORT_t));
    rf_b3_dc_ctrl.hw_config.port_ctl_port = RF_B3_DC_CTRL_GPIO_Port;
    rf_b3_dc_ctrl.hw_config.port_ctl_pin = RF_B3_DC_CTRL_Pin;
    rf_b3_dc_ctrl.hw_config.port_init_stat = PORT_ON;
    port_register(&rf_b3_dc_ctrl);
    BI_PORT_OPEN(&rf_b3_dc_ctrl);
    /*b3 dc ctrl*/
    memset(&rf_b8_dc_ctrl, 0, sizeof(PORT_t));
    rf_b8_dc_ctrl.hw_config.port_ctl_port = RF_B8_DC_CTRL_GPIO_Port;
    rf_b8_dc_ctrl.hw_config.port_ctl_pin = RF_B8_DC_CTRL_Pin;
    rf_b8_dc_ctrl.hw_config.port_init_stat = PORT_ON;
    port_register(&rf_b8_dc_ctrl);
    BI_PORT_OPEN(&rf_b8_dc_ctrl);
    /*b39 dc ctrl*/
    memset(&rf_b39_dc_ctrl, 0, sizeof(PORT_t));
    rf_b39_dc_ctrl.hw_config.port_ctl_port = RF_B39_DC_CTRL_GPIO_Port;
    rf_b39_dc_ctrl.hw_config.port_ctl_pin = RF_B39_DC_CTRL_Pin;
    rf_b39_dc_ctrl.hw_config.port_init_stat = PORT_ON;
    port_register(&rf_b39_dc_ctrl);
    BI_PORT_OPEN(&rf_b39_dc_ctrl);
    /*b40 dc ctrl*/
    memset(&rf_b40_dc_ctrl, 0, sizeof(PORT_t));
    rf_b40_dc_ctrl.hw_config.port_ctl_port = RF_B40_DC_CTRL_GPIO_Port;
    rf_b40_dc_ctrl.hw_config.port_ctl_pin = RF_B40_DC_CTRL_Pin;
    rf_b40_dc_ctrl.hw_config.port_init_stat = PORT_ON;
    port_register(&rf_b40_dc_ctrl);
    BI_PORT_OPEN(&rf_b40_dc_ctrl);
    /*4g dc en*/
    memset(&rf_4g_dc_en, 0, sizeof(PORT_t));
    rf_4g_dc_en.hw_config.port_ctl_port = RF_4G_DC_EN_CTRL_GPIO_Port;
    rf_4g_dc_en.hw_config.port_ctl_pin = RF_4G_DC_EN_CTRL_Pin;
    rf_4g_dc_en.hw_config.port_init_stat = PORT_OFF;
    port_register(&rf_4g_dc_en);
    BI_PORT_OPEN(&rf_4g_dc_en);
    /*nr dc en*/
    memset(&rf_nr_dc_en, 0, sizeof(PORT_t));
    rf_nr_dc_en.hw_config.port_ctl_port = RF_NR_DC_EN_CTRL_GPIO_Port;
    rf_nr_dc_en.hw_config.port_ctl_pin = RF_NR_DC_EN_CTRL_Pin;
    rf_nr_dc_en.hw_config.port_init_stat = PORT_OFF;
    port_register(&rf_nr_dc_en);
    BI_PORT_OPEN(&rf_nr_dc_en);
    /*mimo dc en*/
    memset(&rf_mimo_dc_en, 0, sizeof(PORT_t));
    rf_mimo_dc_en.hw_config.port_ctl_port = RF_MIMO_DC_EN_CTRL_GPIO_Port;
    rf_mimo_dc_en.hw_config.port_ctl_pin = RF_MIMO_DC_EN_CTRL_Pin;
    rf_mimo_dc_en.hw_config.port_init_stat = PORT_OFF;
    port_register(&rf_mimo_dc_en);
    BI_PORT_OPEN(&rf_mimo_dc_en);

    /*---------------------- 5g up silent ctrl ----------------------*/
    /*nr up silent ctrl*/
    memset(&rf_nr_up_silent_ctrl, 0, sizeof(PORT_t));
    rf_nr_up_silent_ctrl.hw_config.port_ctl_port = RF_NR_UP_SILENT_CTRL_GPIO_Port;
    rf_nr_up_silent_ctrl.hw_config.port_ctl_pin = RF_NR_UP_SILENT_CTRL_Pin;
    rf_nr_up_silent_ctrl.hw_config.port_init_stat = PORT_ON;
    port_register(&rf_nr_up_silent_ctrl);
    BI_PORT_OPEN(&rf_nr_up_silent_ctrl);
    /*mimo up silent ctrl*/
    memset(&rf_mimo_up_silent_ctrl, 0, sizeof(PORT_t));
    rf_mimo_up_silent_ctrl.hw_config.port_ctl_port = RF_MIMO_UP_SILENT_CTRL_GPIO_Port;
    rf_mimo_up_silent_ctrl.hw_config.port_ctl_pin = RF_MIMO_UP_SILENT_CTRL_Pin;
    rf_mimo_up_silent_ctrl.hw_config.port_init_stat = PORT_ON;
    port_register(&rf_mimo_up_silent_ctrl);
    BI_PORT_OPEN(&rf_mimo_up_silent_ctrl);

    /*---------------------- ecr8668 ----------------------*/
    /*4g dw 8668 rst ctrl*/
    memset(&ecr8668_4g_dw_rst, 0, sizeof(PORT_t));
    ecr8668_4g_dw_rst.hw_config.port_ctl_port = ECR8668_4G_DW_RST_GPIO_Port;
    ecr8668_4g_dw_rst.hw_config.port_ctl_pin = ECR8668_4G_DW_RST_Pin;
    ecr8668_4g_dw_rst.hw_config.port_init_stat = PORT_ON;
    port_register(&ecr8668_4g_dw_rst);
    BI_PORT_OPEN(&ecr8668_4g_dw_rst);
    /*nr 8668 rst ctrl*/
    memset(&ecr8668_nr_rst, 0, sizeof(PORT_t));
    ecr8668_nr_rst.hw_config.port_ctl_port = ECR8668_NR_RST_GPIO_Port;
    ecr8668_nr_rst.hw_config.port_ctl_pin = ECR8668_NR_RST_Pin;
    ecr8668_nr_rst.hw_config.port_init_stat = PORT_ON;
    port_register(&ecr8668_nr_rst);
    BI_PORT_OPEN(&ecr8668_nr_rst);
    /*mimo 8668 rst ctrl*/
    memset(&ecr8668_mimo_rst, 0, sizeof(PORT_t));
    ecr8668_mimo_rst.hw_config.port_ctl_port = ECR8668_MIMO_RST_GPIO_Port;
    ecr8668_mimo_rst.hw_config.port_ctl_pin = ECR8668_MIMO_RST_Pin;
    ecr8668_mimo_rst.hw_config.port_init_stat = PORT_ON;
    port_register(&ecr8668_mimo_rst);
    BI_PORT_OPEN(&ecr8668_mimo_rst);
    
    /*---------------------- lna ----------------------*/
    /*b3 up lna*/
    memset(&lna_b3_up, 0, sizeof(LNA_t));
    lna_b3_up.hw_config.lna_byp_port = LNA_B3_UP_GPIO_Port;
    lna_b3_up.hw_config.lna_byp_pin = LNA_B3_UP_Pin;
    lna_b3_up.hw_config.lna_init_byp_stat = LNA_BYS;
    lna_b3_up.hw_config.byp_logic_invert = B_TRUE;
    lna_b3_up.hw_config.bypass_att = 0;
    //lna_b3_up.hw_config.lna_pwr_port = ;
    //lna_b3_up.hw_config.lna_pwr_pin = ;
    //lna_b3_up.hw_config.lna_init_pwr_stat = PWR_ON;
    //lna_b3_up.hw_config.pwr_logic_invert = B_TRUE;
    lna_register(&lna_b3_up);
    BI_LNA_OPEN(&lna_b3_up);
    /*b3 dw 1 lna*/
    memset(&lna_b3_dw_1, 0, sizeof(LNA_t));
    lna_b3_dw_1.hw_config.lna_byp_port = LNA_B3_DW_1_GPIO_Port;
    lna_b3_dw_1.hw_config.lna_byp_pin = LNA_B3_DW_1_Pin;
    lna_b3_dw_1.hw_config.lna_init_byp_stat = LNA_BYS;
    lna_b3_dw_1.hw_config.byp_logic_invert = B_TRUE;
    lna_b3_dw_1.hw_config.bypass_att = 0;
    //lna_b3_dw_1.hw_config.lna_pwr_port = ;
    //lna_b3_dw_1.hw_config.lna_pwr_pin = ;
    //lna_b3_dw_1.hw_config.lna_init_pwr_stat = PWR_ON;
    //lna_b3_dw_1.hw_config.pwr_logic_invert = B_TRUE;
    lna_register(&lna_b3_dw_1);
    BI_LNA_OPEN(&lna_b3_dw_1);
    /*b3 dw 2 lna*/
    memset(&lna_b3_dw_2, 0, sizeof(LNA_t));
    lna_b3_dw_2.hw_config.lna_byp_port = LNA_B3_DW_2_GPIO_Port;
    lna_b3_dw_2.hw_config.lna_byp_pin = LNA_B3_DW_2_Pin;
    lna_b3_dw_2.hw_config.lna_init_byp_stat = LNA_BYS;
    lna_b3_dw_2.hw_config.byp_logic_invert = B_TRUE;
    lna_b3_dw_2.hw_config.bypass_att = 0;
    //lna_b3_dw_2.hw_config.lna_pwr_port = ;
    //lna_b3_dw_2.hw_config.lna_pwr_pin = ;
    //lna_b3_dw_2.hw_config.lna_init_pwr_stat = PWR_ON;
    //lna_b3_dw_2.hw_config.pwr_logic_invert = B_TRUE;
    lna_register(&lna_b3_dw_2);
    BI_LNA_OPEN(&lna_b3_dw_2);
    /*b8 up lna*/
    memset(&lna_b8_up, 0, sizeof(LNA_t));
    lna_b8_up.hw_config.lna_byp_port = LNA_B8_UP_GPIO_Port;
    lna_b8_up.hw_config.lna_byp_pin = LNA_B8_UP_Pin;
    lna_b8_up.hw_config.lna_init_byp_stat = LNA_BYS;
    lna_b8_up.hw_config.byp_logic_invert = B_TRUE;
    lna_b8_up.hw_config.bypass_att = 0;
    //lna_b8_up.hw_config.lna_pwr_port = ;
    //lna_b8_up.hw_config.lna_pwr_pin = ;
    //lna_b8_up.hw_config.lna_init_pwr_stat = PWR_ON;
    //lna_b8_up.hw_config.pwr_logic_invert = B_TRUE;
    lna_register(&lna_b8_up);
    BI_LNA_OPEN(&lna_b8_up);
    /*b8 dw 1 lna*/
    memset(&lna_b8_dw_1, 0, sizeof(LNA_t));
    lna_b8_dw_1.hw_config.lna_byp_port = LNA_B8_DW_GPIO_Port;
    lna_b8_dw_1.hw_config.lna_byp_pin = LNA_B8_DW_Pin;
    lna_b8_dw_1.hw_config.lna_init_byp_stat = LNA_BYS;
    lna_b8_dw_1.hw_config.byp_logic_invert = B_TRUE;
    lna_b8_dw_1.hw_config.bypass_att = 0;
    //lna_b8_dw_1.hw_config.lna_pwr_port = ;
    //lna_b8_dw_1.hw_config.lna_pwr_pin = ;
    //lna_b8_dw_1.hw_config.lna_init_pwr_stat = PWR_ON;
    //lna_b8_dw_1.hw_config.pwr_logic_invert = B_TRUE;
    lna_register(&lna_b8_dw_1);
    BI_LNA_OPEN(&lna_b8_dw_1);
    /*b39 up lna*/
    memset(&lna_b39_up, 0, sizeof(LNA_t));
    lna_b39_up.hw_config.lna_byp_port = LNA_B39_UP_GPIO_Port;
    lna_b39_up.hw_config.lna_byp_pin = LNA_B39_UP_Pin;
    lna_b39_up.hw_config.lna_init_byp_stat = LNA_BYS;
    lna_b39_up.hw_config.byp_logic_invert = B_TRUE;
    lna_b39_up.hw_config.bypass_att = 0;
    //lna_b39_up.hw_config.lna_pwr_port = ;
    //lna_b39_up.hw_config.lna_pwr_pin = ;
    //lna_b39_up.hw_config.lna_init_pwr_stat = PWR_ON;
    //lna_b39_up.hw_config.pwr_logic_invert = B_TRUE;
    lna_register(&lna_b39_up);
    BI_LNA_OPEN(&lna_b39_up);
    /*b39 dw 1 lna*/
    memset(&lna_b39_dw_1, 0, sizeof(LNA_t));
    lna_b39_dw_1.hw_config.lna_byp_port = LNA_B39_DW_1_GPIO_Port;
    lna_b39_dw_1.hw_config.lna_byp_pin = LNA_B39_DW_1_Pin;
    lna_b39_dw_1.hw_config.lna_init_byp_stat = LNA_BYS;
    lna_b39_dw_1.hw_config.byp_logic_invert = B_TRUE;
    lna_b39_dw_1.hw_config.bypass_att = 0;
    //lna_b39_dw_1.hw_config.lna_pwr_port = ;
    //lna_b39_dw_1.hw_config.lna_pwr_pin = ;
    //lna_b39_dw_1.hw_config.lna_init_pwr_stat = PWR_ON;
    //lna_b39_dw_1.hw_config.pwr_logic_invert = B_TRUE;
    lna_register(&lna_b39_dw_1);
    BI_LNA_OPEN(&lna_b39_dw_1);
    /*b39 dw 2 lna*/
    memset(&lna_b39_dw_2, 0, sizeof(LNA_t));
    lna_b39_dw_2.hw_config.lna_byp_port = LNA_B39_DW_2_GPIO_Port;
    lna_b39_dw_2.hw_config.lna_byp_pin = LNA_B39_DW_2_Pin;
    lna_b39_dw_2.hw_config.lna_init_byp_stat = LNA_BYS;
    lna_b39_dw_2.hw_config.byp_logic_invert = B_TRUE;
    lna_b39_dw_2.hw_config.bypass_att = 0;
    //lna_b39_dw_2.hw_config.lna_pwr_port = ;
    //lna_b39_dw_2.hw_config.lna_pwr_pin = ;
    //lna_b39_dw_2.hw_config.lna_init_pwr_stat = PWR_ON;
    //lna_b39_dw_2.hw_config.pwr_logic_invert = B_TRUE;
    lna_register(&lna_b39_dw_2);
    BI_LNA_OPEN(&lna_b39_dw_2);
    /*b40 up lna*/
    memset(&lna_b40_up, 0, sizeof(LNA_t));
    lna_b40_up.hw_config.lna_byp_port = LNA_B40_UP_GPIO_Port;
    lna_b40_up.hw_config.lna_byp_pin = LNA_B40_UP_Pin;
    lna_b40_up.hw_config.lna_init_byp_stat = LNA_BYS;
    lna_b40_up.hw_config.byp_logic_invert = B_TRUE;
    lna_b40_up.hw_config.bypass_att = 0;
    //lna_b40_up.hw_config.lna_pwr_port = ;
    //lna_b40_up.hw_config.lna_pwr_pin = ;
    //lna_b40_up.hw_config.lna_init_pwr_stat = PWR_ON;
    //lna_b40_up.hw_config.pwr_logic_invert = B_TRUE;
    lna_register(&lna_b40_up);
    BI_LNA_OPEN(&lna_b40_up);
    /*b40 dw 1 lna*/
    memset(&lna_b40_dw_1, 0, sizeof(LNA_t));
    lna_b40_dw_1.hw_config.lna_byp_port = LNA_B40_DW_1_GPIO_Port;
    lna_b40_dw_1.hw_config.lna_byp_pin = LNA_B40_DW_1_Pin;
    lna_b40_dw_1.hw_config.lna_init_byp_stat = LNA_BYS;
    lna_b40_dw_1.hw_config.byp_logic_invert = B_TRUE;
    lna_b40_dw_1.hw_config.bypass_att = 0;
    //lna_b40_dw_1.hw_config.lna_pwr_port = ;
    //lna_b40_dw_1.hw_config.lna_pwr_pin = ;
    //lna_b40_dw_1.hw_config.lna_init_pwr_stat = PWR_ON;
    //lna_b40_dw_1.hw_config.pwr_logic_invert = B_TRUE;
    lna_register(&lna_b40_dw_1);
    BI_LNA_OPEN(&lna_b40_dw_1);
    /*b40 dw 2 lna*/
    memset(&lna_b40_dw_2, 0, sizeof(LNA_t));
    lna_b40_dw_2.hw_config.lna_byp_port = LNA_B40_DW_2_GPIO_Port;
    lna_b40_dw_2.hw_config.lna_byp_pin = LNA_B40_DW_2_Pin;
    lna_b40_dw_2.hw_config.lna_init_byp_stat = LNA_BYS;
    lna_b40_dw_2.hw_config.byp_logic_invert = B_TRUE;
    lna_b40_dw_2.hw_config.bypass_att = 0;
    //lna_b40_dw_2.hw_config.lna_pwr_port = ;
    //lna_b40_dw_2.hw_config.lna_pwr_pin = ;
    //lna_b40_dw_2.hw_config.lna_init_pwr_stat = PWR_ON;
    //lna_b40_dw_2.hw_config.pwr_logic_invert = B_TRUE;
    lna_register(&lna_b40_dw_2);
    BI_LNA_OPEN(&lna_b40_dw_2);
    /*nr up lna*/
    memset(&lna_nr_up, 0, sizeof(LNA_t));
    lna_nr_up.hw_config.lna_byp_port = LNA_NR_UP_GPIO_Port;
    lna_nr_up.hw_config.lna_byp_pin = LNA_NR_UP_Pin;
    lna_nr_up.hw_config.lna_init_byp_stat = LNA_BYS;
    lna_nr_up.hw_config.byp_logic_invert = B_TRUE;
    lna_nr_up.hw_config.bypass_att = 0;
    //lna_nr_up.hw_config.lna_pwr_port = ;
    //lna_nr_up.hw_config.lna_pwr_pin = ;
    //lna_nr_up.hw_config.lna_init_pwr_stat = PWR_ON;
    //lna_nr_up.hw_config.pwr_logic_invert = B_TRUE;
    lna_register(&lna_nr_up);
    BI_LNA_OPEN(&lna_nr_up);
    /*nr dw 1 lna*/
    memset(&lna_nr_dw_1, 0, sizeof(LNA_t));
    lna_nr_dw_1.hw_config.lna_byp_port = LNA_NR_DW_1_GPIO_Port;
    lna_nr_dw_1.hw_config.lna_byp_pin = LNA_NR_DW_1_Pin;
    lna_nr_dw_1.hw_config.lna_init_byp_stat = LNA_BYS;
    lna_nr_dw_1.hw_config.byp_logic_invert = B_TRUE;
    lna_nr_dw_1.hw_config.bypass_att = 0;
    //lna_nr_dw_1.hw_config.lna_pwr_port = ;
    //lna_nr_dw_1.hw_config.lna_pwr_pin = ;
    //lna_nr_dw_1.hw_config.lna_init_pwr_stat = PWR_ON;
    //lna_nr_dw_1.hw_config.pwr_logic_invert = B_TRUE;
    lna_register(&lna_nr_dw_1);
    BI_LNA_OPEN(&lna_nr_dw_1);
    /*nr dw 2 lna*/
    memset(&lna_nr_dw_2, 0, sizeof(LNA_t));
    lna_nr_dw_2.hw_config.lna_byp_port = LNA_NR_DW_2_GPIO_Port;
    lna_nr_dw_2.hw_config.lna_byp_pin = LNA_NR_DW_2_Pin;
    lna_nr_dw_2.hw_config.lna_init_byp_stat = LNA_BYS;
    lna_nr_dw_2.hw_config.byp_logic_invert = B_TRUE;
    lna_nr_dw_2.hw_config.bypass_att = 0;
    //lna_nr_dw_2.hw_config.lna_pwr_port = ;
    //lna_nr_dw_2.hw_config.lna_pwr_pin = ;
    //lna_nr_dw_2.hw_config.lna_init_pwr_stat = PWR_ON;
    //lna_nr_dw_2.hw_config.pwr_logic_invert = B_TRUE;
    lna_register(&lna_nr_dw_2);
    BI_LNA_OPEN(&lna_nr_dw_2);
    /*mimo up lna*/
    memset(&lna_mimo_up, 0, sizeof(LNA_t));
    lna_mimo_up.hw_config.lna_byp_port = LNA_MIMO_UP_GPIO_Port;
    lna_mimo_up.hw_config.lna_byp_pin = LNA_MIMO_UP_Pin;
    lna_mimo_up.hw_config.lna_init_byp_stat = LNA_BYS;
    lna_mimo_up.hw_config.byp_logic_invert = B_TRUE;
    lna_mimo_up.hw_config.bypass_att = 0;
    //lna_mimo_up.hw_config.lna_pwr_port = ;
    //lna_mimo_up.hw_config.lna_pwr_pin = ;
    //lna_mimo_up.hw_config.lna_init_pwr_stat = PWR_ON;
    //lna_mimo_up.hw_config.pwr_logic_invert = B_TRUE;
    lna_register(&lna_mimo_up);
    BI_LNA_OPEN(&lna_mimo_up);
    /*mimo dw 1 lna*/
    memset(&lna_mimo_dw_1, 0, sizeof(LNA_t));
    lna_mimo_dw_1.hw_config.lna_byp_port = LNA_MIMO_DW_1_GPIO_Port;
    lna_mimo_dw_1.hw_config.lna_byp_pin = LNA_MIMO_DW_1_Pin;
    lna_mimo_dw_1.hw_config.lna_init_byp_stat = LNA_BYS;
    lna_mimo_dw_1.hw_config.byp_logic_invert = B_TRUE;
    lna_mimo_dw_1.hw_config.bypass_att = 0;
    //lna_mimo_dw_1.hw_config.lna_pwr_port = ;
    //lna_mimo_dw_1.hw_config.lna_pwr_pin = ;
    //lna_mimo_dw_1.hw_config.lna_init_pwr_stat = PWR_ON;
    //lna_mimo_dw_1.hw_config.pwr_logic_invert = B_TRUE;
    lna_register(&lna_mimo_dw_1);
    BI_LNA_OPEN(&lna_mimo_dw_1);
    /*mimo dw 2 lna*/
    memset(&lna_mimo_dw_2, 0, sizeof(LNA_t));
    lna_mimo_dw_2.hw_config.lna_byp_port = LNA_MIMO_DW_2_GPIO_Port;
    lna_mimo_dw_2.hw_config.lna_byp_pin = LNA_MIMO_DW_2_Pin;
    lna_mimo_dw_2.hw_config.lna_init_byp_stat = LNA_BYS;
    lna_mimo_dw_2.hw_config.byp_logic_invert = B_TRUE;
    lna_mimo_dw_2.hw_config.bypass_att = 0;
    //lna_mimo_dw_2.hw_config.lna_pwr_port = ;
    //lna_mimo_dw_2.hw_config.lna_pwr_pin = ;
    //lna_mimo_dw_2.hw_config.lna_init_pwr_stat = PWR_ON;
    //lna_mimo_dw_2.hw_config.pwr_logic_invert = B_TRUE;
    lna_register(&lna_mimo_dw_2);
    BI_LNA_OPEN(&lna_mimo_dw_2);
    
    /*---------------------- spdt switch ----------------------*/
    /*b3_b39 up ctrl*/
    memset(&spdt_b3_b39_up_ctrl, 0, sizeof(Switch_spdt_t));
    spdt_b3_b39_up_ctrl.hw_config.switch_ctl_port = SPDT_B3_B39_UP_CTRL_GPIO_Port;
    spdt_b3_b39_up_ctrl.hw_config.switch_ctl_pin = SPDT_B3_B39_UP_CTRL_Pin;
    spdt_b3_b39_up_ctrl.hw_config.switch_init_stat = SW_CH2;
    switch_spdt_register(&spdt_b3_b39_up_ctrl);
    BI_Switch_OPEN(&spdt_b3_b39_up_ctrl);
    /*b3_b39 dw ctrl*/
    memset(&spdt_b3_b39_dw_ctrl, 0, sizeof(Switch_spdt_t));
    spdt_b3_b39_dw_ctrl.hw_config.switch_ctl_port = SPDT_B3_B39_DW_CTRL_GPIO_Port;
    spdt_b3_b39_dw_ctrl.hw_config.switch_ctl_pin = SPDT_B3_B39_DW_CTRL_Pin;
    spdt_b3_b39_dw_ctrl.hw_config.switch_init_stat = SW_CH1;
    switch_spdt_register(&spdt_b3_b39_dw_ctrl);
    BI_Switch_OPEN(&spdt_b3_b39_dw_ctrl);
    
    /*---------------------- sp4t switch ----------------------*/
    /*4g dw rx ctrl*/
    memset(&sp4t_4g_dw_rx, 0, sizeof(PORT_t));
    sp4t_4g_dw_rx.hw_config.switch_v2_port = SP4T_4G_DW_RX_V2_GPIO_Port;
    sp4t_4g_dw_rx.hw_config.switch_v2_pin = SP4T_4G_DW_RX_V2_Pin;
    sp4t_4g_dw_rx.hw_config.switch_v3_port = SP4T_4G_DW_RX_V3_GPIO_Port;
    sp4t_4g_dw_rx.hw_config.switch_v3_pin = SP4T_4G_DW_RX_V3_Pin;
    sp4t_4g_dw_rx.hw_config.switch_init_stat = SW_CH1;
    switch_sp4t_register(&sp4t_4g_dw_rx);
    BI_Switch_OPEN(&sp4t_4g_dw_rx);
    /*4g dw tx ctrl*/
    memset(&sp4t_4g_dw_tx, 0, sizeof(PORT_t));
    sp4t_4g_dw_tx.hw_config.switch_v2_port = SP4T_4G_DW_TX_V2_GPIO_Port;
    sp4t_4g_dw_tx.hw_config.switch_v2_pin = SP4T_4G_DW_TX_V2_Pin;
    sp4t_4g_dw_tx.hw_config.switch_v3_port = SP4T_4G_DW_TX_V3_GPIO_Port;
    sp4t_4g_dw_tx.hw_config.switch_v3_pin = SP4T_4G_DW_TX_V3_Pin;
    sp4t_4g_dw_tx.hw_config.switch_init_stat = SW_CH1;
    switch_sp4t_register(&sp4t_4g_dw_tx);
    BI_Switch_OPEN(&sp4t_4g_dw_tx);
    /*4g ant ctrl*/
    memset(&sp4t_4g_ant, 0, sizeof(PORT_t));
    sp4t_4g_ant.hw_config.switch_v2_port = SP4T_4G_ANT_V2_GPIO_Port;
    sp4t_4g_ant.hw_config.switch_v2_pin = SP4T_4G_ANT_V2_Pin;
    sp4t_4g_ant.hw_config.switch_v3_port = SP4T_4G_ANT_V3_GPIO_Port;
    sp4t_4g_ant.hw_config.switch_v3_pin = SP4T_4G_ANT_V3_Pin;
    sp4t_4g_ant.hw_config.switch_init_stat = SW_CH1;
    switch_sp4t_register(&sp4t_4g_ant);
    BI_Switch_OPEN(&sp4t_4g_ant);
    /*4g bts ctrl*/
    memset(&sp4t_4g_bts, 0, sizeof(PORT_t));
    sp4t_4g_bts.hw_config.switch_v2_port = SP4T_4G_BTS_V2_GPIO_Port;
    sp4t_4g_bts.hw_config.switch_v2_pin = SP4T_4G_BTS_V2_Pin;
    sp4t_4g_bts.hw_config.switch_v3_port = SP4T_4G_BTS_V3_GPIO_Port;
    sp4t_4g_bts.hw_config.switch_v3_pin = SP4T_4G_BTS_V3_Pin;
    sp4t_4g_bts.hw_config.switch_init_stat = SW_CH1;
    switch_sp4t_register(&sp4t_4g_bts);
    BI_Switch_OPEN(&sp4t_4g_bts);
    
    /*---------------------- analog att ----------------------*/
    /*4g dw analog att*/
    memset(&att_analog_4g_dw, 0, sizeof(ATT_analog_t));
    att_analog_4g_dw.hw_config.tim_ch = HTIM2;
    att_analog_4g_dw.hw_config.pwm_ch = PWMCH1;
    att_analog_4g_dw.hw_config.att_min_number = analog_table_4g_dw.min_number;
    att_analog_4g_dw.hw_config.att_max_number = analog_table_4g_dw.valid_number-1;
    att_analog_4g_dw.hw_config.att_table = analog_table_4g_dw.table;
    att_analog_register(&att_analog_4g_dw);
    /*nr dw tx analog att*/
    memset(&att_analog_nr_dw, 0, sizeof(ATT_analog_t));
    att_analog_nr_dw.hw_config.tim_ch = HTIM2;
    att_analog_nr_dw.hw_config.pwm_ch = PWMCH0;
    att_analog_nr_dw.hw_config.att_min_number = analog_table_nr_dw.min_number;
    att_analog_nr_dw.hw_config.att_max_number = analog_table_nr_dw.valid_number-1;
    att_analog_nr_dw.hw_config.att_table = analog_table_nr_dw.table;
    att_analog_register(&att_analog_nr_dw);
    /*mimo dw tx analog att*/
    memset(&att_analog_mimo_dw, 0, sizeof(ATT_analog_t));
    att_analog_mimo_dw.hw_config.tim_ch = HTIM1;
    att_analog_mimo_dw.hw_config.pwm_ch = PWMCH0;
    att_analog_mimo_dw.hw_config.att_min_number = analog_table_mimo_dw.min_number;
    att_analog_mimo_dw.hw_config.att_max_number = analog_table_mimo_dw.valid_number-1;
    att_analog_mimo_dw.hw_config.att_table = analog_table_mimo_dw.table;
    att_analog_register(&att_analog_mimo_dw);
    
    /*b3 up 1 analog att*/
    memset(&att_analog_b3_up_1, 0, sizeof(ATT_analog_t));
    att_analog_b3_up_1.hw_config.tim_ch = HTIM0;
    att_analog_b3_up_1.hw_config.pwm_ch = PWMCH0;
    att_analog_b3_up_1.hw_config.att_min_number = analog_table_4g_up_1.min_number;
    att_analog_b3_up_1.hw_config.att_max_number = analog_table_4g_up_1.valid_number-1;
    att_analog_b3_up_1.hw_config.att_table = analog_table_4g_up_1.table;
    att_analog_register(&att_analog_b3_up_1);
    /*b3 up 2 analog att*/
    memset(&att_analog_b3_up_2, 0, sizeof(ATT_analog_t));
    att_analog_b3_up_2.hw_config.tim_ch = HTIM1;
    att_analog_b3_up_2.hw_config.pwm_ch = PWMCH3;
    att_analog_b3_up_2.hw_config.att_min_number = analog_table_4g_up_2.min_number;
    att_analog_b3_up_2.hw_config.att_max_number = analog_table_4g_up_2.valid_number-1;
    att_analog_b3_up_2.hw_config.att_table = analog_table_4g_up_2.table;
    att_analog_register(&att_analog_b3_up_2);
    /*b8 up 1 analog att*/
    memset(&att_analog_b8_up_1, 0, sizeof(ATT_analog_t));
    att_analog_b8_up_1.hw_config.tim_ch = HTIM0;
    att_analog_b8_up_1.hw_config.pwm_ch = PWMCH3;
    att_analog_b8_up_1.hw_config.att_min_number = analog_table_4g_up_1.min_number;
    att_analog_b8_up_1.hw_config.att_max_number = analog_table_4g_up_1.valid_number-1;
    att_analog_b8_up_1.hw_config.att_table = analog_table_4g_up_1.table;
    att_analog_register(&att_analog_b8_up_1);
    /*b8 up 2 analog att*/
    memset(&att_analog_b8_up_2, 0, sizeof(ATT_analog_t));
    att_analog_b8_up_2.hw_config.tim_ch = HTIM1;
    att_analog_b8_up_2.hw_config.pwm_ch = PWMCH2;
    att_analog_b8_up_2.hw_config.att_min_number = analog_table_4g_up_2.min_number;
    att_analog_b8_up_2.hw_config.att_max_number = analog_table_4g_up_2.valid_number-1;
    att_analog_b8_up_2.hw_config.att_table = analog_table_4g_up_2.table;
    att_analog_register(&att_analog_b8_up_2);
    /*b39 up 1 analog att*/
    memset(&att_analog_b39_up_1, 0, sizeof(ATT_analog_t));
    att_analog_b39_up_1.hw_config.tim_ch = HTIM0;
    att_analog_b39_up_1.hw_config.pwm_ch = PWMCH1;
    att_analog_b39_up_1.hw_config.att_min_number = analog_table_4g_up_1.min_number;
    att_analog_b39_up_1.hw_config.att_max_number = analog_table_4g_up_1.valid_number-1;
    att_analog_b39_up_1.hw_config.att_table = analog_table_4g_up_1.table;
    att_analog_register(&att_analog_b39_up_1);
    /*b39 up 2 analog att*/
    memset(&att_analog_b39_up_2, 0, sizeof(ATT_analog_t));
    att_analog_b39_up_2.hw_config.tim_ch = HTIM0;
    att_analog_b39_up_2.hw_config.pwm_ch = PWMCH2;
    att_analog_b39_up_2.hw_config.att_min_number = analog_table_4g_up_2.min_number;
    att_analog_b39_up_2.hw_config.att_max_number = analog_table_4g_up_2.valid_number-1;
    att_analog_b39_up_2.hw_config.att_table = analog_table_4g_up_2.table;
    att_analog_register(&att_analog_b39_up_2);
    /*b40 up 1 analog att*/
    memset(&att_analog_b40_up_1, 0, sizeof(ATT_analog_t));
    att_analog_b40_up_1.hw_config.tim_ch = HTIM3;
    att_analog_b40_up_1.hw_config.pwm_ch = PWMCH2;
    att_analog_b40_up_1.hw_config.att_min_number = analog_table_4g_up_1.min_number;
    att_analog_b40_up_1.hw_config.att_max_number = analog_table_4g_up_1.valid_number-1;
    att_analog_b40_up_1.hw_config.att_table = analog_table_4g_up_1.table;
    att_analog_register(&att_analog_b40_up_1);
    /*b40 up 2 analog att*/
    memset(&att_analog_b40_up_2, 0, sizeof(ATT_analog_t));
    att_analog_b40_up_2.hw_config.tim_ch = HTIM1;
    att_analog_b40_up_2.hw_config.pwm_ch = PWMCH1;
    att_analog_b40_up_2.hw_config.att_min_number = analog_table_4g_up_2.min_number;
    att_analog_b40_up_2.hw_config.att_max_number = analog_table_4g_up_2.valid_number-1;
    att_analog_b40_up_2.hw_config.att_table = analog_table_4g_up_2.table;
    att_analog_register(&att_analog_b40_up_2);
    /*nr up 1 analog att*/
    memset(&att_analog_nr_up_1, 0, sizeof(ATT_analog_t));
    att_analog_nr_up_1.hw_config.tim_ch = HTIM4;
    att_analog_nr_up_1.hw_config.pwm_ch = PWMCH0;
    att_analog_nr_up_1.hw_config.att_min_number = analog_table_nr_up_1.min_number;
    att_analog_nr_up_1.hw_config.att_max_number = analog_table_nr_up_1.valid_number-1;
    att_analog_nr_up_1.hw_config.att_table = analog_table_nr_up_1.table;
    att_analog_register(&att_analog_nr_up_1);
    /*nr up 2 analog att*/
    memset(&att_analog_nr_up_2, 0, sizeof(ATT_analog_t));
    att_analog_nr_up_2.hw_config.tim_ch = HTIM4;
    att_analog_nr_up_2.hw_config.pwm_ch = PWMCH1;
    att_analog_nr_up_2.hw_config.att_min_number = analog_table_nr_up_2.min_number;
    att_analog_nr_up_2.hw_config.att_max_number = analog_table_nr_up_2.valid_number-1;
    att_analog_nr_up_2.hw_config.att_table = analog_table_nr_up_2.table;
    att_analog_register(&att_analog_nr_up_2);
    /*mimo up 1 analog att*/
    memset(&att_analog_mimo_up_1, 0, sizeof(ATT_analog_t));
    att_analog_mimo_up_1.hw_config.tim_ch = HTIM3;
    att_analog_mimo_up_1.hw_config.pwm_ch = PWMCH3;
    att_analog_mimo_up_1.hw_config.att_min_number = analog_table_mimo_up_1.min_number;
    att_analog_mimo_up_1.hw_config.att_max_number = analog_table_mimo_up_1.valid_number-1;
    att_analog_mimo_up_1.hw_config.att_table = analog_table_mimo_up_1.table;
    att_analog_register(&att_analog_mimo_up_1);
    /*mimo up 2 analog att*/
    memset(&att_analog_mimo_up_2, 0, sizeof(ATT_analog_t));
    att_analog_mimo_up_2.hw_config.tim_ch = HTIM2;
    att_analog_mimo_up_2.hw_config.pwm_ch = PWMCH2;
    att_analog_mimo_up_2.hw_config.att_min_number = analog_table_mimo_up_2.min_number;
    att_analog_mimo_up_2.hw_config.att_max_number = analog_table_mimo_up_2.valid_number-1;
    att_analog_mimo_up_2.hw_config.att_table = analog_table_mimo_up_2.table;
    att_analog_register(&att_analog_mimo_up_2);

    /*---------------------- ble config ----------------------*/
    memset(&ble_config, 0, sizeof(ble_hw_config_t));
    ble_config.ble_dc_ctrl.hw_config.port_ctl_port = BLE_DC_CTRL_GPIO_Port;
    ble_config.ble_dc_ctrl.hw_config.port_ctl_pin = BLE_DC_CTRL_Pin;
    ble_config.ble_dc_ctrl.hw_config.port_init_stat = PORT_ON;
    ble_config.ble_uart = &uart_ble_fun;
    port_register(&ble_config.ble_dc_ctrl);
    BI_PORT_OPEN(&ble_config.ble_dc_ctrl);
    ble_register(&ble_config);
    
    
    /*----------------------  sync modem  ----------------------*/
    sync_modem_config.dc_ctrl_pin = MODEM_DC_CTRL_Pin;
    sync_modem_config.dc_ctrl_port = MODEM_DC_CTRL_GPIO_Port;
    sync_modem_config.sync_pin = MODEM_SYN_Pin;
    sync_modem_config.sync_port = MODEM_SYN_GPIO_Port;
    sync_modem_config.modem_uart = &uart_modem_fun;
    sync_modem_config.lte_gpio_group = 1;//0
    sync_modem_config.nr_gpio_group = 0;  //1
    sync_modem_config.nr2_gpio_group = 2;
    modem_register(&sync_modem_config);

    /*----------------------flash spi0 ----------------------*/
    spi_flash.spi_ch = SPI_0;
    spi_flash.cs_port = FLASH_CS_GPIO_Port;
    spi_flash.cs_pin = FLASH_CS_Pin;
    spi_flash.page_size = FLASH_PAGE_SIZE;
    spi_flash.sector_size = FLASH_SECTOR_SIZE;
    spi_flash.sector_num = FLASH_SECTOR_NUM;
    spi_flash.flash_size = FLASH_SIZE;
    spi_flash.lock = UNLOCK;
    flash_25xx_register(&spi_flash);
    
    /*---------------------- fpga jtag enable ----------------------*/
    /*fpga jtag enable*/
    memset(&fpga_jtag_en, 0, sizeof(PORT_t));
    fpga_jtag_en.hw_config.port_ctl_port = FPGA_JTAG_EN_GPIO_Port;
    fpga_jtag_en.hw_config.port_ctl_pin = FPGA_JTAG_EN_Pin;
    fpga_jtag_en.hw_config.port_init_stat = PORT_ON;
    port_register(&fpga_jtag_en);
    BI_PORT_OPEN(&fpga_jtag_en);
    
    /*---------------------- fpga nStatus ----------------------*/
    /*fpga nStatus*/
    memset(&fpga_n_status, 0, sizeof(PORT_t));
    fpga_n_status.hw_config.port_ctl_port = FPGA_N_STATUS_GPIO_Port;
    fpga_n_status.hw_config.port_ctl_pin = FPGA_N_STATUS_Pin;
    fpga_n_status.hw_config.port_init_stat = PORT_ON;
    port_register(&fpga_n_status);
    BI_PORT_OPEN(&fpga_n_status);
    
    /*---------------------- fpga ----------------------*/
    fpga.dc_ctrl_port = FPGA_DC_CTRL_GPIO_Port; //
    fpga.dc_ctrl_pin = FPGA_DC_CTRL_Pin;
    fpga.ps_nconfig_port = FPGA_N_CONFIG_GPIO_Port;
    fpga.ps_nconfig_pin = FPGA_N_CONFIG_Pin;
    fpga.ps_cs_port = FPGA_PS_CS_GPIO_Port;
    fpga.ps_cs_pin = FPGA_PS_CS_Pin;
    fpga.ps_dclk_port = FPGA_PS_DCLK_GPIO_Port;
    fpga.ps_dclk_pin = FPGA_PS_DCLK_Pin;
    fpga.ps_data_port = FPGA_PS_DATA0_GPIO_Port;
    fpga.ps_data_pin = FPGA_PS_DATA0_Pin;
    fpga.spi_cs_port = FPGA_MCU_CS_GPIO_Port;
    fpga.spi_cs_pin = FPGA_MCU_CS_Pin;
	fpga.rst_n_port = FPGA_MCU_STATUS_GPIO_Port;
	fpga.rst_n_pin = FPGA_MCU_STATUS_Pin;
    fpga.spi = SPI_1;
    fpga_register(&fpga);

    /*---------------------- 8668 ----------------------*/
    memset(&h8668_LTE, 0, sizeof(h8668_LTE));
    h8668_LTE.config.spi_port.fpga_spi_addr = FPGA_OP_DEVICE_TRX_4G; //FPGA分配给8668的地址
    h8668_LTE.config.spi_port.port_type = TYPE_FPGA;//通信方式
    h8668_LTE.config.rst_pin = ECR8668_4G_DW_RST_Pin;
    h8668_LTE.config.rst_port = ECR8668_4G_DW_RST_GPIO_Port;
    h8668_LTE.config.ch = 0; // /8668固定只用通道0
    h8668_LTE.config.port = CH0_RXC_TXB; // 所用端口,待定,硬件提供
    h8668_LTE.config.dev_number = RF_SYS_1; // 8668编号,当板上多片8668时以区分设备打印信息
    h8668_LTE.config.adc_irq = ECR8668_4G_DW_ADC_OVERLOAD_EXTI_IRQn; // 8668 adc告警中断
    h8668_LTE.config.tia_irq = ECR8668_4G_DW_TIA_OVERLOAD_EXTI_IRQn; // 8668 tia告警中断

    trx8668_fpga_register(&h8668_LTE);

    memset(&h8668_NR, 0, sizeof(h8668_NR));
    h8668_NR.config.spi_port.fpga_spi_addr = FPGA_OP_DEVICE_TRX_5G1; //FPGA分配给8668的地址
    h8668_NR.config.spi_port.port_type = TYPE_FPGA;//通信方式
    h8668_NR.config.rst_pin = ECR8668_NR_RST_Pin;
    h8668_NR.config.rst_port = ECR8668_NR_RST_GPIO_Port;
    h8668_NR.config.ch = 0; // 8668固定只用通道0
    h8668_NR.config.port = CH0_RXC_TXB; // 所用端口,待定,硬件提供
    h8668_NR.config.dev_number = RF_SYS_2; // 8668编号,当板上多片8668时以区分设备打印信息
    h8668_NR.config.adc_irq = ECR8668_NR_ADC_OVERLOAD_EXTI_IRQn; // 8668 adc告警中断
    h8668_NR.config.tia_irq = ECR8668_NR_TIA_OVERLOAD_EXTI_IRQn; // 8668 tia告警中断

    trx8668_fpga_register(&h8668_NR);

    memset(&h8668_NR_MIMO, 0, sizeof(h8668_NR_MIMO));
    h8668_NR_MIMO.config.spi_port.fpga_spi_addr = FPGA_OP_DEVICE_TRX_5G2; //FPGA分配给8668的地址
    h8668_NR_MIMO.config.spi_port.port_type = TYPE_FPGA;//通信方式
    h8668_NR_MIMO.config.rst_pin = ECR8668_MIMO_RST_Pin;
    h8668_NR_MIMO.config.rst_port = ECR8668_MIMO_RST_GPIO_Port;
    h8668_NR_MIMO.config.ch = 0; // 8668固定只用通道0
    h8668_NR_MIMO.config.port = CH0_RXC_TXB; // 所用端口,待定,硬件提供
    h8668_NR_MIMO.config.dev_number = RF_SYS_3; // 8668编号,当板上多片8668时以区分设备打印信息
    h8668_NR_MIMO.config.adc_irq = ECR8668_MIMO_ADC_OVERLOAD_EXTI_IRQn; // 8668 adc告警中断
    h8668_NR_MIMO.config.tia_irq = ECR8668_MIMO_TIA_OVERLOAD_EXTI_IRQn; // 8668 tia告警中断

    trx8668_fpga_register(&h8668_NR_MIMO);

    /*---------------------- cat1 ----------------------*/
    cat1_n706_hw_config.cat1_dc_ctrl_port = CAT1_DC_CTRL_GPIO_Port;
    cat1_n706_hw_config.cat1_dc_ctrl_pin = CAT1_DC_CTRL_Pin;
    cat1_n706_hw_config.cat1_stat_port = CAT1_STATE_GPIO_Port;
    cat1_n706_hw_config.cat1_stat_pin = CAT1_STATE_Pin;
    cat1_n706_hw_config.cat1_open_port = CAT1_OPEN_GPIO_Port;
    cat1_n706_hw_config.cat1_open_pin = CAT1_OPEN_Pin;
    cat1_n706_hw_config.cat1_reset_port = CAT1_RESET_GPIO_Port;
    cat1_n706_hw_config.cat1_reset_pin = CAT1_RESET_Pin;
    cat1_n706_hw_config.uart_func = &uart_cat1_fun;
    cat1_n706_register(&cat1_n706_hw_config);
}

/*****************************************************************************
* @brief  : 
* @param  : None
* @retval : 
* @date   : 2023-11-28
* @note   : 
*****************************************************************************/
void bsp_device_init(void)
{
    //fpga_ps_load();
    spi_flash.flash_open();
}

#elif (PRODUCT_MODEL == CMDT3521C)
// trx8668
extern void BSP_866x_adc_warning_exti_handle(TRX8668_FPGA_t *h8668);

// analog att table     /*此结构体需要待频段选择完成后加载数据*/
Analog_table_t analog_table_4g_up_1, analog_table_4g_up_2, analog_table_4g_dw;   
Analog_table_t analog_table_nr_up_1, analog_table_nr_up_2, analog_table_nr_dw;
Analog_table_t analog_table_mimo_up_1, analog_table_mimo_up_2, analog_table_mimo_dw;

// led
LED_t led_pg8, led_pc9, led_c11, led_pd0, led_pd1, led_pd2;

// power crtl
PORT_t rf_b3_dc_ctrl;                                       
PORT_t rf_b8_dc_ctrl;
PORT_t rf_b39_dc_ctrl;
PORT_t rf_b40_dc_ctrl;
PORT_t rf_4g_dc_en;
PORT_t rf_nr_dc_en;
PORT_t rf_mimo_dc_en;

// 5g up silent
PORT_t rf_nr_up_silent_ctrl;                                
PORT_t rf_mimo_up_silent_ctrl;

// 8668 rst
PORT_t ecr8668_4g_dw_rst;
PORT_t ecr8668_nr_rst;           
PORT_t ecr8668_mimo_rst; 

// lna
LNA_t lna_b3_up,    lna_b3_dw_1,    lna_b3_dw_2;            // b3 lna
LNA_t lna_b8_up,    lna_b8_dw_1;                            // b8 lna
LNA_t lna_b39_up,   lna_b39_dw_1,   lna_b39_dw_2;           // b39 lna
LNA_t lna_b40_up,   lna_b40_dw_1,   lna_b40_dw_2;           // b40 lna
LNA_t lna_nr_up,    lna_nr_dw_1,    lna_nr_dw_2;            // nr lna
LNA_t lna_mimo_up,  lna_mimo_dw_1,  lna_mimo_dw_2;          // mimo lna

// spdt switch
Switch_spdt_t spdt_b3_b39_up_ctrl, spdt_b3_b39_dw_ctrl;

// sp4t switch
Switch_sp4t_t sp4t_4g_dw_rx;
Switch_sp4t_t sp4t_4g_dw_tx;
Switch_sp4t_t sp4t_4g_ant;
Switch_sp4t_t sp4t_4g_bts;

// analog att
ATT_analog_t att_analog_4g_dw;                              // 4g dw analog att
ATT_analog_t att_analog_nr_dw;                              // nr dw analog att
ATT_analog_t att_analog_mimo_dw;                            // mimo dw analog att

ATT_analog_t att_analog_b3_up_1,    att_analog_b3_up_2;     // b3 up analog att
ATT_analog_t att_analog_b8_up_1,    att_analog_b8_up_2;     // b8 up analog att
ATT_analog_t att_analog_b39_up_1,   att_analog_b39_up_2;    // b39 up analog att
ATT_analog_t att_analog_b40_up_1,   att_analog_b40_up_2;    // b40 up analog att
ATT_analog_t att_analog_nr_up_1,    att_analog_nr_up_2;     // nr up analog att
ATT_analog_t att_analog_mimo_up_1,  att_analog_mimo_up_2;   // mimo up analog att

// ble config
ble_hw_config_t ble_config;

// 5g modem config
modem_config_t sync_modem_config;

// spi flash
FLASH_25xx_config_t spi_flash;                             

// fpga jtag enable
PORT_t fpga_jtag_en; 

// fpga nStatus
PORT_t fpga_n_status;

// fpga
FPGA_config_t fpga;

// trx8668
TRX8668_FPGA_t h8668_LTE;
TRX8668_FPGA_t h8668_NR;
TRX8668_FPGA_t h8668_NR_MIMO;
// cat1
cat1_mc661_hw_config_t cat1_mc661_hw_config;
// uart
/* 5g modem */
#define MODEM_RX_LEN  300
#define MODEM_TX_LEN  1200
uint8_t modem_name[10] = "modem";       // USART1
//uint8_t moden_ringbuf[MODEM_RX_LEN];    // 环形缓存区
__align(32) uint8_t modem_rx_buf[MODEM_RX_LEN] __attribute__((section("CCMRAM")));
__align(32) uint8_t modem_tx_buf[MODEM_TX_LEN] __attribute__((section("ORIRAM")));
static Cbuffer_T modem_buf;
static uart_data_t modem_data;
uart_func_t uart_modem_fun;
/* cat1 */
uint8_t cat1_name[10] = "cat1";         // USART6
__align(32) uint8_t cat1_rx_buf[CAT1_RX_DATA_LEN] __attribute__((section("CCMRAM")));
__align(32) uint8_t cat1_tx_buf[CAT1_TX_DATA_LEN] __attribute__((section("ORIRAM")));
static Cbuffer_T cat1_buf;
static uart_data_t cat1_data;
uart_func_t uart_cat1_fun;
/* ble */
#define BLE_RX_LEN  512
#define BLE_TX_LEN  512
uint8_t ble_name[10] = "ble1";         // USART3
__align(32) uint8_t ble_rx_buf[BLE_RX_LEN] __attribute__((section("CCMRAM")));
__align(32) uint8_t ble_tx_buf[BLE_TX_LEN] __attribute__((section("ORIRAM")));
static Cbuffer_T ble_buf;
static uart_data_t ble_data;
uart_func_t uart_ble_fun; 

void bsp_register(void)
{
    /*---------------------- uart ----------------------*/
    /*modem uart*/
    memcpy(modem_data.name_des,  modem_name, 10);
    modem_data.rtt_index = RTT_MODEM;
    modem_data.uart_id = &huart1;
    modem_data.msg_egp = xUartNewMsgEgp;
    modem_data.msg_egp_bit = BIT_UART1_MSG;
    //modem_data.msg_queue = ;
    //modem_data.msg_queue_len = ;
    modem_data.rx_data_len = MODEM_RX_LEN;
    modem_data.tx_data_len = MODEM_TX_LEN;
    modem_data.pt_rx_data = modem_rx_buf;
    modem_data.pt_tx_data = modem_tx_buf;
    modem_data.tpt_buffer = &modem_buf;
    hi_uart_func_struct_init(&uart_modem_fun, &modem_data);
    uart_modem_fun.init(&modem_data);
    /*cat1 uart*/
    memcpy(cat1_data.name_des,  cat1_name, 10);
    cat1_data.rtt_index = RTT_CAT1;
    cat1_data.uart_id = &huart6;
    cat1_data.msg_egp = xUartNewMsgEgp;
    cat1_data.msg_egp_bit = BIT_UART6_MSG;
    //cat1_data.msg_queue = ;
    //cat1_data.msg_queue_len = ;
    cat1_data.rx_data_len = CAT1_RX_DATA_LEN;
    cat1_data.tx_data_len = CAT1_TX_DATA_LEN;
    cat1_data.pt_rx_data = cat1_rx_buf;
    cat1_data.pt_tx_data = cat1_tx_buf;
    cat1_data.tpt_buffer = &cat1_buf;
    hi_uart_func_struct_init(&uart_cat1_fun, &cat1_data);
    uart_cat1_fun.init(&cat1_data);
    /*ble uart*/
    memcpy(ble_data.name_des,  ble_name, 10);
    ble_data.rtt_index = RTT_BLE;
    ble_data.uart_id = &huart3;
    ble_data.msg_egp = xUartNewMsgEgp;
    ble_data.msg_egp_bit = BIT_UART3_MSG;
    //ble_data.msg_queue = ;
    //ble_data.msg_queue_len = ;
    ble_data.rx_data_len = BLE_RX_LEN;
    ble_data.tx_data_len = BLE_TX_LEN;
    ble_data.pt_rx_data = ble_rx_buf;
    ble_data.pt_tx_data = ble_tx_buf;
    ble_data.tpt_buffer = &ble_buf;
    hi_uart_func_struct_init(&uart_ble_fun, &ble_data);
    uart_ble_fun.init(&ble_data);
    
    /*---------------------- led ----------------------*/
    //led_pg8
    memset(&led_pg8, 0, sizeof(LED_t));
    led_pg8.hw_config.led_port = LED_PG8_GPIO_Port;
    led_pg8.hw_config.led_pin = LED_PG8_Pin;
    led_pg8.hw_config.led_init_stat = LED_OFF;
    led_pg8.hw_config.ctl_logic_invert = B_TRUE;
    led_register(&led_pg8);
    //led_pc9
    memset(&led_pc9, 0, sizeof(LED_t));
    led_pc9.hw_config.led_port = LED_PC9_GPIO_Port;
    led_pc9.hw_config.led_pin = LED_PC9_Pin;
    led_pc9.hw_config.led_init_stat = LED_OFF;
    led_pc9.hw_config.ctl_logic_invert = B_TRUE;
    led_register(&led_pc9);
    //led_c11
    memset(&led_c11, 0, sizeof(LED_t));
    led_c11.hw_config.led_port = LED_PC11_GPIO_Port;
    led_c11.hw_config.led_pin = LED_PC11_Pin;
    led_c11.hw_config.led_init_stat = LED_OFF;
    led_c11.hw_config.ctl_logic_invert = B_TRUE;
    led_register(&led_c11);
    //led_pd0
    memset(&led_pd0, 0, sizeof(LED_t));
    led_pd0.hw_config.led_port = LED_PD0_GPIO_Port;
    led_pd0.hw_config.led_pin = LED_PD0_Pin;
    led_pd0.hw_config.led_init_stat = LED_OFF;
    led_pd0.hw_config.ctl_logic_invert = B_TRUE;
    led_register(&led_pd0);
    //led_pd1
    memset(&led_pd1, 0, sizeof(LED_t));
    led_pd1.hw_config.led_port = LED_PD1_GPIO_Port;
    led_pd1.hw_config.led_pin = LED_PD1_Pin;
    led_pd1.hw_config.led_init_stat = LED_OFF;
    led_pd1.hw_config.ctl_logic_invert = B_TRUE;
    led_register(&led_pd1);
    //led_pd2
    memset(&led_pd2, 0, sizeof(LED_t));
    led_pd2.hw_config.led_port = LED_PD2_GPIO_Port;
    led_pd2.hw_config.led_pin = LED_PD2_Pin;
    led_pd2.hw_config.led_init_stat = LED_OFF;
    led_pd2.hw_config.ctl_logic_invert = B_TRUE;
    led_register(&led_pd2);

    /*---------------------- power ctrl ----------------------*/
    /*b3 dc ctrl*/
    memset(&rf_b3_dc_ctrl, 0, sizeof(PORT_t));
    rf_b3_dc_ctrl.hw_config.port_ctl_port = RF_B3_DC_CTRL_GPIO_Port;
    rf_b3_dc_ctrl.hw_config.port_ctl_pin = RF_B3_DC_CTRL_Pin;
    rf_b3_dc_ctrl.hw_config.port_init_stat = PORT_ON;
    port_register(&rf_b3_dc_ctrl);
    BI_PORT_OPEN(&rf_b3_dc_ctrl);
    /*b3 dc ctrl*/
    memset(&rf_b8_dc_ctrl, 0, sizeof(PORT_t));
    rf_b8_dc_ctrl.hw_config.port_ctl_port = RF_B8_DC_CTRL_GPIO_Port;
    rf_b8_dc_ctrl.hw_config.port_ctl_pin = RF_B8_DC_CTRL_Pin;
    rf_b8_dc_ctrl.hw_config.port_init_stat = PORT_ON;
    port_register(&rf_b8_dc_ctrl);
    BI_PORT_OPEN(&rf_b8_dc_ctrl);
    /*b39 dc ctrl*/
    memset(&rf_b39_dc_ctrl, 0, sizeof(PORT_t));
    rf_b39_dc_ctrl.hw_config.port_ctl_port = RF_B39_DC_CTRL_GPIO_Port;
    rf_b39_dc_ctrl.hw_config.port_ctl_pin = RF_B39_DC_CTRL_Pin;
    rf_b39_dc_ctrl.hw_config.port_init_stat = PORT_ON;
    port_register(&rf_b39_dc_ctrl);
    BI_PORT_OPEN(&rf_b39_dc_ctrl);
    /*b40 dc ctrl*/
    memset(&rf_b40_dc_ctrl, 0, sizeof(PORT_t));
    rf_b40_dc_ctrl.hw_config.port_ctl_port = RF_B40_DC_CTRL_GPIO_Port;
    rf_b40_dc_ctrl.hw_config.port_ctl_pin = RF_B40_DC_CTRL_Pin;
    rf_b40_dc_ctrl.hw_config.port_init_stat = PORT_ON;
    port_register(&rf_b40_dc_ctrl);
    BI_PORT_OPEN(&rf_b40_dc_ctrl);
    /*4g dc en*/
    memset(&rf_4g_dc_en, 0, sizeof(PORT_t));
    rf_4g_dc_en.hw_config.port_ctl_port = RF_4G_DC_EN_CTRL_GPIO_Port;
    rf_4g_dc_en.hw_config.port_ctl_pin = RF_4G_DC_EN_CTRL_Pin;
    rf_4g_dc_en.hw_config.port_init_stat = PORT_OFF;
    port_register(&rf_4g_dc_en);
    BI_PORT_OPEN(&rf_4g_dc_en);
    /*nr dc en*/
    memset(&rf_nr_dc_en, 0, sizeof(PORT_t));
    rf_nr_dc_en.hw_config.port_ctl_port = RF_NR_DC_EN_CTRL_GPIO_Port;
    rf_nr_dc_en.hw_config.port_ctl_pin = RF_NR_DC_EN_CTRL_Pin;
    rf_nr_dc_en.hw_config.port_init_stat = PORT_OFF;
    port_register(&rf_nr_dc_en);
    BI_PORT_OPEN(&rf_nr_dc_en);
    /*mimo dc en*/
    memset(&rf_mimo_dc_en, 0, sizeof(PORT_t));
    rf_mimo_dc_en.hw_config.port_ctl_port = RF_MIMO_DC_EN_CTRL_GPIO_Port;
    rf_mimo_dc_en.hw_config.port_ctl_pin = RF_MIMO_DC_EN_CTRL_Pin;
    rf_mimo_dc_en.hw_config.port_init_stat = PORT_OFF;
    port_register(&rf_mimo_dc_en);
    BI_PORT_OPEN(&rf_mimo_dc_en);

    /*---------------------- 5g up silent ctrl ----------------------*/
    /*nr up silent ctrl*/
    memset(&rf_nr_up_silent_ctrl, 0, sizeof(PORT_t));
    rf_nr_up_silent_ctrl.hw_config.port_ctl_port = RF_NR_UP_SILENT_CTRL_GPIO_Port;
    rf_nr_up_silent_ctrl.hw_config.port_ctl_pin = RF_NR_UP_SILENT_CTRL_Pin;
    rf_nr_up_silent_ctrl.hw_config.port_init_stat = PORT_ON;
    port_register(&rf_nr_up_silent_ctrl);
    BI_PORT_OPEN(&rf_nr_up_silent_ctrl);
    /*mimo up silent ctrl*/
    memset(&rf_mimo_up_silent_ctrl, 0, sizeof(PORT_t));
    rf_mimo_up_silent_ctrl.hw_config.port_ctl_port = RF_MIMO_UP_SILENT_CTRL_GPIO_Port;
    rf_mimo_up_silent_ctrl.hw_config.port_ctl_pin = RF_MIMO_UP_SILENT_CTRL_Pin;
    rf_mimo_up_silent_ctrl.hw_config.port_init_stat = PORT_ON;
    port_register(&rf_mimo_up_silent_ctrl);
    BI_PORT_OPEN(&rf_mimo_up_silent_ctrl);

    /*---------------------- ecr8668 ----------------------*/
    /*4g dw 8668 rst ctrl*/
    memset(&ecr8668_4g_dw_rst, 0, sizeof(PORT_t));
    ecr8668_4g_dw_rst.hw_config.port_ctl_port = ECR8668_4G_DW_RST_GPIO_Port;
    ecr8668_4g_dw_rst.hw_config.port_ctl_pin = ECR8668_4G_DW_RST_Pin;
    ecr8668_4g_dw_rst.hw_config.port_init_stat = PORT_ON;
    port_register(&ecr8668_4g_dw_rst);
    BI_PORT_OPEN(&ecr8668_4g_dw_rst);
    /*nr 8668 rst ctrl*/
    memset(&ecr8668_nr_rst, 0, sizeof(PORT_t));
    ecr8668_nr_rst.hw_config.port_ctl_port = ECR8668_NR_RST_GPIO_Port;
    ecr8668_nr_rst.hw_config.port_ctl_pin = ECR8668_NR_RST_Pin;
    ecr8668_nr_rst.hw_config.port_init_stat = PORT_ON;
    port_register(&ecr8668_nr_rst);
    BI_PORT_OPEN(&ecr8668_nr_rst);
    /*mimo 8668 rst ctrl*/
    memset(&ecr8668_mimo_rst, 0, sizeof(PORT_t));
    ecr8668_mimo_rst.hw_config.port_ctl_port = ECR8668_MIMO_RST_GPIO_Port;
    ecr8668_mimo_rst.hw_config.port_ctl_pin = ECR8668_MIMO_RST_Pin;
    ecr8668_mimo_rst.hw_config.port_init_stat = PORT_ON;
    port_register(&ecr8668_mimo_rst);
    BI_PORT_OPEN(&ecr8668_mimo_rst);
    
    /*---------------------- lna ----------------------*/
    /*b3 up lna*/
    memset(&lna_b3_up, 0, sizeof(LNA_t));
    lna_b3_up.hw_config.lna_byp_port = LNA_B3_UP_GPIO_Port;
    lna_b3_up.hw_config.lna_byp_pin = LNA_B3_UP_Pin;
    lna_b3_up.hw_config.lna_init_byp_stat = LNA_BYS;
    lna_b3_up.hw_config.byp_logic_invert = B_TRUE;
    lna_b3_up.hw_config.bypass_att = 0;
    //lna_b3_up.hw_config.lna_pwr_port = ;
    //lna_b3_up.hw_config.lna_pwr_pin = ;
    //lna_b3_up.hw_config.lna_init_pwr_stat = PWR_ON;
    //lna_b3_up.hw_config.pwr_logic_invert = B_TRUE;
    lna_register(&lna_b3_up);
    BI_LNA_OPEN(&lna_b3_up);
    /*b3 dw 1 lna*/
    memset(&lna_b3_dw_1, 0, sizeof(LNA_t));
    lna_b3_dw_1.hw_config.lna_byp_port = LNA_B3_DW_1_GPIO_Port;
    lna_b3_dw_1.hw_config.lna_byp_pin = LNA_B3_DW_1_Pin;
    lna_b3_dw_1.hw_config.lna_init_byp_stat = LNA_BYS;
    lna_b3_dw_1.hw_config.byp_logic_invert = B_TRUE;
    lna_b3_dw_1.hw_config.bypass_att = 0;
    //lna_b3_dw_1.hw_config.lna_pwr_port = ;
    //lna_b3_dw_1.hw_config.lna_pwr_pin = ;
    //lna_b3_dw_1.hw_config.lna_init_pwr_stat = PWR_ON;
    //lna_b3_dw_1.hw_config.pwr_logic_invert = B_TRUE;
    lna_register(&lna_b3_dw_1);
    BI_LNA_OPEN(&lna_b3_dw_1);
    /*b3 dw 2 lna*/
    memset(&lna_b3_dw_2, 0, sizeof(LNA_t));
    lna_b3_dw_2.hw_config.lna_byp_port = LNA_B3_DW_2_GPIO_Port;
    lna_b3_dw_2.hw_config.lna_byp_pin = LNA_B3_DW_2_Pin;
    lna_b3_dw_2.hw_config.lna_init_byp_stat = LNA_BYS;
    lna_b3_dw_2.hw_config.byp_logic_invert = B_TRUE;
    lna_b3_dw_2.hw_config.bypass_att = 0;
    //lna_b3_dw_2.hw_config.lna_pwr_port = ;
    //lna_b3_dw_2.hw_config.lna_pwr_pin = ;
    //lna_b3_dw_2.hw_config.lna_init_pwr_stat = PWR_ON;
    //lna_b3_dw_2.hw_config.pwr_logic_invert = B_TRUE;
    lna_register(&lna_b3_dw_2);
    BI_LNA_OPEN(&lna_b3_dw_2);
    /*b8 up lna*/
    memset(&lna_b8_up, 0, sizeof(LNA_t));
    lna_b8_up.hw_config.lna_byp_port = LNA_B8_UP_GPIO_Port;
    lna_b8_up.hw_config.lna_byp_pin = LNA_B8_UP_Pin;
    lna_b8_up.hw_config.lna_init_byp_stat = LNA_BYS;
    lna_b8_up.hw_config.byp_logic_invert = B_TRUE;
    lna_b8_up.hw_config.bypass_att = 0;
    //lna_b8_up.hw_config.lna_pwr_port = ;
    //lna_b8_up.hw_config.lna_pwr_pin = ;
    //lna_b8_up.hw_config.lna_init_pwr_stat = PWR_ON;
    //lna_b8_up.hw_config.pwr_logic_invert = B_TRUE;
    lna_register(&lna_b8_up);
    BI_LNA_OPEN(&lna_b8_up);
    /*b8 dw 1 lna*/
    memset(&lna_b8_dw_1, 0, sizeof(LNA_t));
    lna_b8_dw_1.hw_config.lna_byp_port = LNA_B8_DW_GPIO_Port;
    lna_b8_dw_1.hw_config.lna_byp_pin = LNA_B8_DW_Pin;
    lna_b8_dw_1.hw_config.lna_init_byp_stat = LNA_BYS;
    lna_b8_dw_1.hw_config.byp_logic_invert = B_TRUE;
    lna_b8_dw_1.hw_config.bypass_att = 0;
    //lna_b8_dw_1.hw_config.lna_pwr_port = ;
    //lna_b8_dw_1.hw_config.lna_pwr_pin = ;
    //lna_b8_dw_1.hw_config.lna_init_pwr_stat = PWR_ON;
    //lna_b8_dw_1.hw_config.pwr_logic_invert = B_TRUE;
    lna_register(&lna_b8_dw_1);
    BI_LNA_OPEN(&lna_b8_dw_1);
    /*b39 up lna*/
    memset(&lna_b39_up, 0, sizeof(LNA_t));
    lna_b39_up.hw_config.lna_byp_port = LNA_B39_UP_GPIO_Port;
    lna_b39_up.hw_config.lna_byp_pin = LNA_B39_UP_Pin;
    lna_b39_up.hw_config.lna_init_byp_stat = LNA_BYS;
    lna_b39_up.hw_config.byp_logic_invert = B_TRUE;
    lna_b39_up.hw_config.bypass_att = 0;
    //lna_b39_up.hw_config.lna_pwr_port = ;
    //lna_b39_up.hw_config.lna_pwr_pin = ;
    //lna_b39_up.hw_config.lna_init_pwr_stat = PWR_ON;
    //lna_b39_up.hw_config.pwr_logic_invert = B_TRUE;
    lna_register(&lna_b39_up);
    BI_LNA_OPEN(&lna_b39_up);
    /*b39 dw 1 lna*/
    memset(&lna_b39_dw_1, 0, sizeof(LNA_t));
    lna_b39_dw_1.hw_config.lna_byp_port = LNA_B39_DW_1_GPIO_Port;
    lna_b39_dw_1.hw_config.lna_byp_pin = LNA_B39_DW_1_Pin;
    lna_b39_dw_1.hw_config.lna_init_byp_stat = LNA_BYS;
    lna_b39_dw_1.hw_config.byp_logic_invert = B_TRUE;
    lna_b39_dw_1.hw_config.bypass_att = 0;
    //lna_b39_dw_1.hw_config.lna_pwr_port = ;
    //lna_b39_dw_1.hw_config.lna_pwr_pin = ;
    //lna_b39_dw_1.hw_config.lna_init_pwr_stat = PWR_ON;
    //lna_b39_dw_1.hw_config.pwr_logic_invert = B_TRUE;
    lna_register(&lna_b39_dw_1);
    BI_LNA_OPEN(&lna_b39_dw_1);
    /*b39 dw 2 lna*/
    memset(&lna_b39_dw_2, 0, sizeof(LNA_t));
    lna_b39_dw_2.hw_config.lna_byp_port = LNA_B39_DW_2_GPIO_Port;
    lna_b39_dw_2.hw_config.lna_byp_pin = LNA_B39_DW_2_Pin;
    lna_b39_dw_2.hw_config.lna_init_byp_stat = LNA_BYS;
    lna_b39_dw_2.hw_config.byp_logic_invert = B_TRUE;
    lna_b39_dw_2.hw_config.bypass_att = 0;
    //lna_b39_dw_2.hw_config.lna_pwr_port = ;
    //lna_b39_dw_2.hw_config.lna_pwr_pin = ;
    //lna_b39_dw_2.hw_config.lna_init_pwr_stat = PWR_ON;
    //lna_b39_dw_2.hw_config.pwr_logic_invert = B_TRUE;
    lna_register(&lna_b39_dw_2);
    BI_LNA_OPEN(&lna_b39_dw_2);
    /*b40 up lna*/
    memset(&lna_b40_up, 0, sizeof(LNA_t));
    lna_b40_up.hw_config.lna_byp_port = LNA_B40_UP_GPIO_Port;
    lna_b40_up.hw_config.lna_byp_pin = LNA_B40_UP_Pin;
    lna_b40_up.hw_config.lna_init_byp_stat = LNA_BYS;
    lna_b40_up.hw_config.byp_logic_invert = B_TRUE;
    lna_b40_up.hw_config.bypass_att = 0;
    //lna_b40_up.hw_config.lna_pwr_port = ;
    //lna_b40_up.hw_config.lna_pwr_pin = ;
    //lna_b40_up.hw_config.lna_init_pwr_stat = PWR_ON;
    //lna_b40_up.hw_config.pwr_logic_invert = B_TRUE;
    lna_register(&lna_b40_up);
    BI_LNA_OPEN(&lna_b40_up);
    /*b40 dw 1 lna*/
    memset(&lna_b40_dw_1, 0, sizeof(LNA_t));
    lna_b40_dw_1.hw_config.lna_byp_port = LNA_B40_DW_1_GPIO_Port;
    lna_b40_dw_1.hw_config.lna_byp_pin = LNA_B40_DW_1_Pin;
    lna_b40_dw_1.hw_config.lna_init_byp_stat = LNA_BYS;
    lna_b40_dw_1.hw_config.byp_logic_invert = B_TRUE;
    lna_b40_dw_1.hw_config.bypass_att = 0;
    //lna_b40_dw_1.hw_config.lna_pwr_port = ;
    //lna_b40_dw_1.hw_config.lna_pwr_pin = ;
    //lna_b40_dw_1.hw_config.lna_init_pwr_stat = PWR_ON;
    //lna_b40_dw_1.hw_config.pwr_logic_invert = B_TRUE;
    lna_register(&lna_b40_dw_1);
    BI_LNA_OPEN(&lna_b40_dw_1);
    /*b40 dw 2 lna*/
    memset(&lna_b40_dw_2, 0, sizeof(LNA_t));
    lna_b40_dw_2.hw_config.lna_byp_port = LNA_B40_DW_2_GPIO_Port;
    lna_b40_dw_2.hw_config.lna_byp_pin = LNA_B40_DW_2_Pin;
    lna_b40_dw_2.hw_config.lna_init_byp_stat = LNA_BYS;
    lna_b40_dw_2.hw_config.byp_logic_invert = B_TRUE;
    lna_b40_dw_2.hw_config.bypass_att = 0;
    //lna_b40_dw_2.hw_config.lna_pwr_port = ;
    //lna_b40_dw_2.hw_config.lna_pwr_pin = ;
    //lna_b40_dw_2.hw_config.lna_init_pwr_stat = PWR_ON;
    //lna_b40_dw_2.hw_config.pwr_logic_invert = B_TRUE;
    lna_register(&lna_b40_dw_2);
    BI_LNA_OPEN(&lna_b40_dw_2);
    /*nr up lna*/
    memset(&lna_nr_up, 0, sizeof(LNA_t));
    lna_nr_up.hw_config.lna_byp_port = LNA_NR_UP_GPIO_Port;
    lna_nr_up.hw_config.lna_byp_pin = LNA_NR_UP_Pin;
    lna_nr_up.hw_config.lna_init_byp_stat = LNA_BYS;
    lna_nr_up.hw_config.byp_logic_invert = B_TRUE;
    lna_nr_up.hw_config.bypass_att = 0;
    //lna_nr_up.hw_config.lna_pwr_port = ;
    //lna_nr_up.hw_config.lna_pwr_pin = ;
    //lna_nr_up.hw_config.lna_init_pwr_stat = PWR_ON;
    //lna_nr_up.hw_config.pwr_logic_invert = B_TRUE;
    lna_register(&lna_nr_up);
    BI_LNA_OPEN(&lna_nr_up);
    /*nr dw 1 lna*/
    memset(&lna_nr_dw_1, 0, sizeof(LNA_t));
    lna_nr_dw_1.hw_config.lna_byp_port = LNA_NR_DW_1_GPIO_Port;
    lna_nr_dw_1.hw_config.lna_byp_pin = LNA_NR_DW_1_Pin;
    lna_nr_dw_1.hw_config.lna_init_byp_stat = LNA_BYS;
    lna_nr_dw_1.hw_config.byp_logic_invert = B_TRUE;
    lna_nr_dw_1.hw_config.bypass_att = 0;
    //lna_nr_dw_1.hw_config.lna_pwr_port = ;
    //lna_nr_dw_1.hw_config.lna_pwr_pin = ;
    //lna_nr_dw_1.hw_config.lna_init_pwr_stat = PWR_ON;
    //lna_nr_dw_1.hw_config.pwr_logic_invert = B_TRUE;
    lna_register(&lna_nr_dw_1);
    BI_LNA_OPEN(&lna_nr_dw_1);
    /*nr dw 2 lna*/
    memset(&lna_nr_dw_2, 0, sizeof(LNA_t));
    lna_nr_dw_2.hw_config.lna_byp_port = LNA_NR_DW_2_GPIO_Port;
    lna_nr_dw_2.hw_config.lna_byp_pin = LNA_NR_DW_2_Pin;
    lna_nr_dw_2.hw_config.lna_init_byp_stat = LNA_BYS;
    lna_nr_dw_2.hw_config.byp_logic_invert = B_TRUE;
    lna_nr_dw_2.hw_config.bypass_att = 0;
    //lna_nr_dw_2.hw_config.lna_pwr_port = ;
    //lna_nr_dw_2.hw_config.lna_pwr_pin = ;
    //lna_nr_dw_2.hw_config.lna_init_pwr_stat = PWR_ON;
    //lna_nr_dw_2.hw_config.pwr_logic_invert = B_TRUE;
    lna_register(&lna_nr_dw_2);
    BI_LNA_OPEN(&lna_nr_dw_2);
    /*mimo up lna*/
    memset(&lna_mimo_up, 0, sizeof(LNA_t));
    lna_mimo_up.hw_config.lna_byp_port = LNA_MIMO_UP_GPIO_Port;
    lna_mimo_up.hw_config.lna_byp_pin = LNA_MIMO_UP_Pin;
    lna_mimo_up.hw_config.lna_init_byp_stat = LNA_BYS;
    lna_mimo_up.hw_config.byp_logic_invert = B_TRUE;
    lna_mimo_up.hw_config.bypass_att = 0;
    //lna_mimo_up.hw_config.lna_pwr_port = ;
    //lna_mimo_up.hw_config.lna_pwr_pin = ;
    //lna_mimo_up.hw_config.lna_init_pwr_stat = PWR_ON;
    //lna_mimo_up.hw_config.pwr_logic_invert = B_TRUE;
    lna_register(&lna_mimo_up);
    BI_LNA_OPEN(&lna_mimo_up);
    /*mimo dw 1 lna*/
    memset(&lna_mimo_dw_1, 0, sizeof(LNA_t));
    lna_mimo_dw_1.hw_config.lna_byp_port = LNA_MIMO_DW_1_GPIO_Port;
    lna_mimo_dw_1.hw_config.lna_byp_pin = LNA_MIMO_DW_1_Pin;
    lna_mimo_dw_1.hw_config.lna_init_byp_stat = LNA_BYS;
    lna_mimo_dw_1.hw_config.byp_logic_invert = B_TRUE;
    lna_mimo_dw_1.hw_config.bypass_att = 0;
    //lna_mimo_dw_1.hw_config.lna_pwr_port = ;
    //lna_mimo_dw_1.hw_config.lna_pwr_pin = ;
    //lna_mimo_dw_1.hw_config.lna_init_pwr_stat = PWR_ON;
    //lna_mimo_dw_1.hw_config.pwr_logic_invert = B_TRUE;
    lna_register(&lna_mimo_dw_1);
    BI_LNA_OPEN(&lna_mimo_dw_1);
    /*mimo dw 2 lna*/
    memset(&lna_mimo_dw_2, 0, sizeof(LNA_t));
    lna_mimo_dw_2.hw_config.lna_byp_port = LNA_MIMO_DW_2_GPIO_Port;
    lna_mimo_dw_2.hw_config.lna_byp_pin = LNA_MIMO_DW_2_Pin;
    lna_mimo_dw_2.hw_config.lna_init_byp_stat = LNA_BYS;
    lna_mimo_dw_2.hw_config.byp_logic_invert = B_TRUE;
    lna_mimo_dw_2.hw_config.bypass_att = 0;
    //lna_mimo_dw_2.hw_config.lna_pwr_port = ;
    //lna_mimo_dw_2.hw_config.lna_pwr_pin = ;
    //lna_mimo_dw_2.hw_config.lna_init_pwr_stat = PWR_ON;
    //lna_mimo_dw_2.hw_config.pwr_logic_invert = B_TRUE;
    lna_register(&lna_mimo_dw_2);
    BI_LNA_OPEN(&lna_mimo_dw_2);
    
    /*---------------------- spdt switch ----------------------*/
    /*b3_b39 up ctrl*/
    memset(&spdt_b3_b39_up_ctrl, 0, sizeof(Switch_spdt_t));
    spdt_b3_b39_up_ctrl.hw_config.switch_ctl_port = SPDT_B3_B39_UP_CTRL_GPIO_Port;
    spdt_b3_b39_up_ctrl.hw_config.switch_ctl_pin = SPDT_B3_B39_UP_CTRL_Pin;
    spdt_b3_b39_up_ctrl.hw_config.switch_init_stat = SW_CH2;
    switch_spdt_register(&spdt_b3_b39_up_ctrl);
    BI_Switch_OPEN(&spdt_b3_b39_up_ctrl);
    /*b3_b39 dw ctrl*/
    memset(&spdt_b3_b39_dw_ctrl, 0, sizeof(Switch_spdt_t));
    spdt_b3_b39_dw_ctrl.hw_config.switch_ctl_port = SPDT_B3_B39_DW_CTRL_GPIO_Port;
    spdt_b3_b39_dw_ctrl.hw_config.switch_ctl_pin = SPDT_B3_B39_DW_CTRL_Pin;
    spdt_b3_b39_dw_ctrl.hw_config.switch_init_stat = SW_CH1;
    switch_spdt_register(&spdt_b3_b39_dw_ctrl);
    BI_Switch_OPEN(&spdt_b3_b39_dw_ctrl);
    
    /*---------------------- sp4t switch ----------------------*/
    /*4g dw rx ctrl*/
    memset(&sp4t_4g_dw_rx, 0, sizeof(PORT_t));
    sp4t_4g_dw_rx.hw_config.switch_v2_port = SP4T_4G_DW_RX_V2_GPIO_Port;
    sp4t_4g_dw_rx.hw_config.switch_v2_pin = SP4T_4G_DW_RX_V2_Pin;
    sp4t_4g_dw_rx.hw_config.switch_v3_port = SP4T_4G_DW_RX_V3_GPIO_Port;
    sp4t_4g_dw_rx.hw_config.switch_v3_pin = SP4T_4G_DW_RX_V3_Pin;
    sp4t_4g_dw_rx.hw_config.switch_init_stat = SW_CH1;
    switch_sp4t_register(&sp4t_4g_dw_rx);
    BI_Switch_OPEN(&sp4t_4g_dw_rx);
    /*4g dw tx ctrl*/
    memset(&sp4t_4g_dw_tx, 0, sizeof(PORT_t));
    sp4t_4g_dw_tx.hw_config.switch_v2_port = SP4T_4G_DW_TX_V2_GPIO_Port;
    sp4t_4g_dw_tx.hw_config.switch_v2_pin = SP4T_4G_DW_TX_V2_Pin;
    sp4t_4g_dw_tx.hw_config.switch_v3_port = SP4T_4G_DW_TX_V3_GPIO_Port;
    sp4t_4g_dw_tx.hw_config.switch_v3_pin = SP4T_4G_DW_TX_V3_Pin;
    sp4t_4g_dw_tx.hw_config.switch_init_stat = SW_CH1;
    switch_sp4t_register(&sp4t_4g_dw_tx);
    BI_Switch_OPEN(&sp4t_4g_dw_tx);
    /*4g ant ctrl*/
    memset(&sp4t_4g_ant, 0, sizeof(PORT_t));
    sp4t_4g_ant.hw_config.switch_v2_port = SP4T_4G_ANT_V2_GPIO_Port;
    sp4t_4g_ant.hw_config.switch_v2_pin = SP4T_4G_ANT_V2_Pin;
    sp4t_4g_ant.hw_config.switch_v3_port = SP4T_4G_ANT_V3_GPIO_Port;
    sp4t_4g_ant.hw_config.switch_v3_pin = SP4T_4G_ANT_V3_Pin;
    sp4t_4g_ant.hw_config.switch_init_stat = SW_CH1;
    switch_sp4t_register(&sp4t_4g_ant);
    BI_Switch_OPEN(&sp4t_4g_ant);
    /*4g bts ctrl*/
    memset(&sp4t_4g_bts, 0, sizeof(PORT_t));
    sp4t_4g_bts.hw_config.switch_v2_port = SP4T_4G_BTS_V2_GPIO_Port;
    sp4t_4g_bts.hw_config.switch_v2_pin = SP4T_4G_BTS_V2_Pin;
    sp4t_4g_bts.hw_config.switch_v3_port = SP4T_4G_BTS_V3_GPIO_Port;
    sp4t_4g_bts.hw_config.switch_v3_pin = SP4T_4G_BTS_V3_Pin;
    sp4t_4g_bts.hw_config.switch_init_stat = SW_CH1;
    switch_sp4t_register(&sp4t_4g_bts);
    BI_Switch_OPEN(&sp4t_4g_bts);
    
    /*---------------------- analog att ----------------------*/
    /*4g dw analog att*/
    memset(&att_analog_4g_dw, 0, sizeof(ATT_analog_t));
    att_analog_4g_dw.hw_config.tim_ch = HTIM3;
    att_analog_4g_dw.hw_config.pwm_ch = PWMCH2;
    att_analog_4g_dw.hw_config.att_min_number = analog_table_4g_dw.min_number;
    att_analog_4g_dw.hw_config.att_max_number = analog_table_4g_dw.valid_number;
    att_analog_4g_dw.hw_config.att_table = analog_table_4g_dw.table;
    att_analog_register(&att_analog_4g_dw);
    /*nr dw tx analog att*/
    memset(&att_analog_nr_dw, 0, sizeof(ATT_analog_t));
    att_analog_nr_dw.hw_config.tim_ch = HTIM3;
    att_analog_nr_dw.hw_config.pwm_ch = PWMCH1;
    att_analog_nr_dw.hw_config.att_min_number = analog_table_nr_dw.min_number;
    att_analog_nr_dw.hw_config.att_max_number = analog_table_nr_dw.valid_number;
    att_analog_nr_dw.hw_config.att_table = analog_table_nr_dw.table;
    att_analog_register(&att_analog_nr_dw);
    /*mimo dw tx analog att*/
    memset(&att_analog_mimo_dw, 0, sizeof(ATT_analog_t));
    att_analog_mimo_dw.hw_config.tim_ch = HTIM2;
    att_analog_mimo_dw.hw_config.pwm_ch = PWMCH1;
    att_analog_mimo_dw.hw_config.att_min_number = analog_table_mimo_dw.min_number;
    att_analog_mimo_dw.hw_config.att_max_number = analog_table_mimo_dw.valid_number;
    att_analog_mimo_dw.hw_config.att_table = analog_table_mimo_dw.table;
    att_analog_register(&att_analog_mimo_dw);
    
    /*b3 up 1 analog att*/
    memset(&att_analog_b3_up_1, 0, sizeof(ATT_analog_t));
    att_analog_b3_up_1.hw_config.tim_ch = HTIM1;
    att_analog_b3_up_1.hw_config.pwm_ch = PWMCH1;
    att_analog_b3_up_1.hw_config.att_min_number = analog_table_4g_up_1.min_number;
    att_analog_b3_up_1.hw_config.att_max_number = analog_table_4g_up_1.valid_number;
    att_analog_b3_up_1.hw_config.att_table = analog_table_4g_up_1.table;
    att_analog_register(&att_analog_b3_up_1);
    /*b3 up 2 analog att*/
    memset(&att_analog_b3_up_2, 0, sizeof(ATT_analog_t));
    att_analog_b3_up_2.hw_config.tim_ch = HTIM5;
    att_analog_b3_up_2.hw_config.pwm_ch = PWMCH4;
    att_analog_b3_up_2.hw_config.att_min_number = analog_table_4g_up_2.min_number;
    att_analog_b3_up_2.hw_config.att_max_number = analog_table_4g_up_2.valid_number;
    att_analog_b3_up_2.hw_config.att_table = analog_table_4g_up_2.table;
    att_analog_register(&att_analog_b3_up_2);
    /*b8 up 1 analog att*/
    memset(&att_analog_b8_up_1, 0, sizeof(ATT_analog_t));
    att_analog_b8_up_1.hw_config.tim_ch = HTIM1;
    att_analog_b8_up_1.hw_config.pwm_ch = PWMCH4;
    att_analog_b8_up_1.hw_config.att_min_number = analog_table_4g_up_1.min_number;
    att_analog_b8_up_1.hw_config.att_max_number = analog_table_4g_up_1.valid_number;
    att_analog_b8_up_1.hw_config.att_table = analog_table_4g_up_1.table;
    att_analog_register(&att_analog_b8_up_1);
    /*b8 up 2 analog att*/
    memset(&att_analog_b8_up_2, 0, sizeof(ATT_analog_t));
    att_analog_b8_up_2.hw_config.tim_ch = HTIM5;
    att_analog_b8_up_2.hw_config.pwm_ch = PWMCH3;
    att_analog_b8_up_2.hw_config.att_min_number = analog_table_4g_up_2.min_number;
    att_analog_b8_up_2.hw_config.att_max_number = analog_table_4g_up_2.valid_number;
    att_analog_b8_up_2.hw_config.att_table = analog_table_4g_up_2.table;
    att_analog_register(&att_analog_b8_up_2);
    /*b39 up 1 analog att*/
    memset(&att_analog_b39_up_1, 0, sizeof(ATT_analog_t));
    att_analog_b39_up_1.hw_config.tim_ch = HTIM1;
    att_analog_b39_up_1.hw_config.pwm_ch = PWMCH2;
    att_analog_b39_up_1.hw_config.att_min_number = analog_table_4g_up_1.min_number;
    att_analog_b39_up_1.hw_config.att_max_number = analog_table_4g_up_1.valid_number;
    att_analog_b39_up_1.hw_config.att_table = analog_table_4g_up_1.table;
    att_analog_register(&att_analog_b39_up_1);
    /*b39 up 2 analog att*/
    memset(&att_analog_b39_up_2, 0, sizeof(ATT_analog_t));
    att_analog_b39_up_2.hw_config.tim_ch = HTIM1;
    att_analog_b39_up_2.hw_config.pwm_ch = PWMCH3;
    att_analog_b39_up_2.hw_config.att_min_number = analog_table_4g_up_2.min_number;
    att_analog_b39_up_2.hw_config.att_max_number = analog_table_4g_up_2.valid_number;
    att_analog_b39_up_2.hw_config.att_table = analog_table_4g_up_2.table;
    att_analog_register(&att_analog_b39_up_2);
    /*b40 up 1 analog att*/
    memset(&att_analog_b40_up_1, 0, sizeof(ATT_analog_t));
    att_analog_b40_up_1.hw_config.tim_ch = HTIM4;
    att_analog_b40_up_1.hw_config.pwm_ch = PWMCH3;
    att_analog_b40_up_1.hw_config.att_min_number = analog_table_4g_up_1.min_number;
    att_analog_b40_up_1.hw_config.att_max_number = analog_table_4g_up_1.valid_number;
    att_analog_b40_up_1.hw_config.att_table = analog_table_4g_up_1.table;
    att_analog_register(&att_analog_b40_up_1);
    /*b40 up 2 analog att*/
    memset(&att_analog_b40_up_2, 0, sizeof(ATT_analog_t));
    att_analog_b40_up_2.hw_config.tim_ch = HTIM11;
    att_analog_b40_up_2.hw_config.pwm_ch = PWMCH1;
    att_analog_b40_up_2.hw_config.att_min_number = analog_table_4g_up_2.min_number;
    att_analog_b40_up_2.hw_config.att_max_number = analog_table_4g_up_2.valid_number;
    att_analog_b40_up_2.hw_config.att_table = analog_table_4g_up_2.table;
    att_analog_register(&att_analog_b40_up_2);
    /*nr up 1 analog att*/
    memset(&att_analog_nr_up_1, 0, sizeof(ATT_analog_t));
    att_analog_nr_up_1.hw_config.tim_ch = HTIM5;
    att_analog_nr_up_1.hw_config.pwm_ch = PWMCH1;
    att_analog_nr_up_1.hw_config.att_min_number = analog_table_nr_up_1.min_number;
    att_analog_nr_up_1.hw_config.att_max_number = analog_table_nr_up_1.valid_number;
    att_analog_nr_up_1.hw_config.att_table = analog_table_nr_up_1.table;
    att_analog_register(&att_analog_nr_up_1);
    /*nr up 2 analog att*/
    memset(&att_analog_nr_up_2, 0, sizeof(ATT_analog_t));
    att_analog_nr_up_2.hw_config.tim_ch = HTIM5;
    att_analog_nr_up_2.hw_config.pwm_ch = PWMCH2;
    att_analog_nr_up_2.hw_config.att_min_number = analog_table_nr_up_2.min_number;
    att_analog_nr_up_2.hw_config.att_max_number = analog_table_nr_up_2.valid_number;
    att_analog_nr_up_2.hw_config.att_table = analog_table_nr_up_2.table;
    att_analog_register(&att_analog_nr_up_2);
    /*mimo up 1 analog att*/
    memset(&att_analog_mimo_up_1, 0, sizeof(ATT_analog_t));
    att_analog_mimo_up_1.hw_config.tim_ch = HTIM4;
    att_analog_mimo_up_1.hw_config.pwm_ch = PWMCH4;
    att_analog_mimo_up_1.hw_config.att_min_number = analog_table_mimo_up_1.min_number;
    att_analog_mimo_up_1.hw_config.att_max_number = analog_table_mimo_up_1.valid_number;
    att_analog_mimo_up_1.hw_config.att_table = analog_table_mimo_up_1.table;
    att_analog_register(&att_analog_mimo_up_1);
    /*mimo up 2 analog att*/
    memset(&att_analog_mimo_up_2, 0, sizeof(ATT_analog_t));
    att_analog_mimo_up_2.hw_config.tim_ch = HTIM3;
    att_analog_mimo_up_2.hw_config.pwm_ch = PWMCH3;
    att_analog_mimo_up_2.hw_config.att_min_number = analog_table_mimo_up_2.min_number;
    att_analog_mimo_up_2.hw_config.att_max_number = analog_table_mimo_up_2.valid_number;
    att_analog_mimo_up_2.hw_config.att_table = analog_table_mimo_up_2.table;
    att_analog_register(&att_analog_mimo_up_2);

    /*---------------------- ble config ----------------------*/
    memset(&ble_config, 0, sizeof(ble_hw_config_t));
    ble_config.ble_dc_ctrl.hw_config.port_ctl_port = BLE_DC_CTRL_GPIO_Port;
    ble_config.ble_dc_ctrl.hw_config.port_ctl_pin = BLE_DC_CTRL_Pin;
    ble_config.ble_dc_ctrl.hw_config.port_init_stat = PORT_ON;
    ble_config.ble_uart = &uart_ble_fun;
    port_register(&ble_config.ble_dc_ctrl);
    BI_PORT_OPEN(&ble_config.ble_dc_ctrl);
    ble_register(&ble_config);
    
    
    /*----------------------  sync modem  ----------------------*/
    sync_modem_config.dc_ctrl_pin = MODEM_DC_CTRL_Pin;
    sync_modem_config.dc_ctrl_port = MODEM_DC_CTRL_GPIO_Port;
    sync_modem_config.sync_pin = MODEM_SYN_Pin;
    sync_modem_config.sync_port = MODEM_SYN_GPIO_Port;
    sync_modem_config.modem_uart = &uart_modem_fun;
    sync_modem_config.lte_gpio_group = 1;//0
    sync_modem_config.nr_gpio_group = 0;  //1
    sync_modem_config.nr2_gpio_group = 2;
    modem_register(&sync_modem_config);

    /*----------------------flash spi1 ----------------------*/
    spi_flash.spi_ch = SPI_1;
    spi_flash.cs_port = FLASH_CS_GPIO_Port;
    spi_flash.cs_pin = FLASH_CS_Pin;
    spi_flash.page_size = FLASH_PAGE_SIZE;
    spi_flash.sector_size = FLASH_SECTOR_SIZE;
    spi_flash.sector_num = FLASH_SECTOR_NUM;
    spi_flash.flash_size = FLASH_SIZE;
    spi_flash.lock = UNLOCK;
    flash_25xx_register(&spi_flash);
    
    /*---------------------- fpga jtag enable ----------------------*/
    /*fpga jtag enable*/
    memset(&fpga_jtag_en, 0, sizeof(PORT_t));
    fpga_jtag_en.hw_config.port_ctl_port = FPGA_JTAG_EN_GPIO_Port;
    fpga_jtag_en.hw_config.port_ctl_pin = FPGA_JTAG_EN_Pin;
    fpga_jtag_en.hw_config.port_init_stat = PORT_ON;
    port_register(&fpga_jtag_en);
    BI_PORT_OPEN(&fpga_jtag_en);
    
    /*---------------------- fpga nStatus ----------------------*/
    /*fpga nStatus*/
    memset(&fpga_n_status, 0, sizeof(PORT_t));
    fpga_n_status.hw_config.port_ctl_port = FPGA_N_STATUS_GPIO_Port;
    fpga_n_status.hw_config.port_ctl_pin = FPGA_N_STATUS_Pin;
    fpga_n_status.hw_config.port_init_stat = PORT_ON;
    port_register(&fpga_n_status);
    BI_PORT_OPEN(&fpga_n_status);
    
    /*---------------------- fpga ----------------------*/
    fpga.dc_ctrl_port = FPGA_DC_CTRL_GPIO_Port; //
    fpga.dc_ctrl_pin = FPGA_DC_CTRL_Pin;
    fpga.ps_nconfig_port = FPGA_N_CONFIG_GPIO_Port;
    fpga.ps_nconfig_pin = FPGA_N_CONFIG_Pin;
    fpga.ps_cs_port = FPGA_PS_CS_GPIO_Port;
    fpga.ps_cs_pin = FPGA_PS_CS_Pin;
    fpga.ps_dclk_port = FPGA_PS_DCLK_GPIO_Port;
    fpga.ps_dclk_pin = FPGA_PS_DCLK_Pin;
    fpga.ps_data_port = FPGA_PS_DATA0_GPIO_Port;
    fpga.ps_data_pin = FPGA_PS_DATA0_Pin;
    fpga.spi_cs_port = FPGA_MCU_CS_GPIO_Port;
    fpga.spi_cs_pin = FPGA_MCU_CS_Pin;
    fpga.rst_n_port = FPGA_MCU_STATUS_GPIO_Port;
    fpga.rst_n_pin = FPGA_MCU_STATUS_Pin;
    fpga.spi = SPI_2;
    fpga_register(&fpga);

    /*---------------------- 8668 ----------------------*/
    memset(&h8668_LTE, 0, sizeof(h8668_LTE));
    h8668_LTE.config.spi_port.fpga_spi_addr = FPGA_OP_DEVICE_TRX_4G; //FPGA分配给8668的地址
    h8668_LTE.config.spi_port.port_type = TYPE_FPGA;//通信方式
    h8668_LTE.config.rst_pin = ECR8668_4G_DW_RST_Pin;
    h8668_LTE.config.rst_port = ECR8668_4G_DW_RST_GPIO_Port;
    h8668_LTE.config.ch = 0; // /8668固定只用通道0
    h8668_LTE.config.port = CH0_RXC_TXB; // 所用端口,待定,硬件提供
    h8668_LTE.config.dev_number = RF_SYS_1; // 8668编号,当板上多片8668时以区分设备打印信息
    h8668_LTE.config.adc_irq = ECR8668_4G_DW_ADC_OVERLOAD_EXTI_IRQn; // 8668 adc告警中断
    h8668_LTE.config.tia_irq = ECR8668_4G_DW_TIA_OVERLOAD_EXTI_IRQn; // 8668 tia告警中断

    trx8668_fpga_register(&h8668_LTE);

    memset(&h8668_NR, 0, sizeof(h8668_NR));
    h8668_NR.config.spi_port.fpga_spi_addr = FPGA_OP_DEVICE_TRX_5G1; //FPGA分配给8668的地址
    h8668_NR.config.spi_port.port_type = TYPE_FPGA;//通信方式
    h8668_NR.config.rst_pin = ECR8668_NR_RST_Pin;
    h8668_NR.config.rst_port = ECR8668_NR_RST_GPIO_Port;
    h8668_NR.config.ch = 0; // 8668固定只用通道0
    h8668_NR.config.port = CH0_RXC_TXB; // 所用端口,待定,硬件提供
    h8668_NR.config.dev_number = RF_SYS_2; // 8668编号,当板上多片8668时以区分设备打印信息
    h8668_NR.config.adc_irq = ECR8668_NR_ADC_OVERLOAD_EXTI_IRQn; // 8668 adc告警中断
    h8668_NR.config.tia_irq = ECR8668_NR_TIA_OVERLOAD_EXTI_IRQn; // 8668 tia告警中断

    trx8668_fpga_register(&h8668_NR);

    memset(&h8668_NR_MIMO, 0, sizeof(h8668_NR_MIMO));
    h8668_NR_MIMO.config.spi_port.fpga_spi_addr = FPGA_OP_DEVICE_TRX_5G2; //FPGA分配给8668的地址
    h8668_NR_MIMO.config.spi_port.port_type = TYPE_FPGA;//通信方式
    h8668_NR_MIMO.config.rst_pin = ECR8668_MIMO_RST_Pin;
    h8668_NR_MIMO.config.rst_port = ECR8668_MIMO_RST_GPIO_Port;
    h8668_NR_MIMO.config.ch = 0; // 8668固定只用通道0
    h8668_NR_MIMO.config.port = CH0_RXC_TXB; // 所用端口,待定,硬件提供
    h8668_NR_MIMO.config.dev_number = RF_SYS_3; // 8668编号,当板上多片8668时以区分设备打印信息
    h8668_NR_MIMO.config.adc_irq = ECR8668_MIMO_ADC_OVERLOAD_EXTI_IRQn; // 8668 adc告警中断
    h8668_NR_MIMO.config.tia_irq = ECR8668_MIMO_TIA_OVERLOAD_EXTI_IRQn; // 8668 tia告警中断

    trx8668_fpga_register(&h8668_NR_MIMO);

    /*---------------------- cat1 ----------------------*/
    cat1_mc661_hw_config.cat1_dc_ctrl_port = CAT1_DC_CTRL_GPIO_Port;
    cat1_mc661_hw_config.cat1_dc_ctrl_pin = CAT1_DC_CTRL_Pin;
    cat1_mc661_hw_config.cat1_stat_port = CAT1_STATE_GPIO_Port;
    cat1_mc661_hw_config.cat1_stat_pin = CAT1_STATE_Pin;
    cat1_mc661_hw_config.cat1_open_port = CAT1_OPEN_GPIO_Port;
    cat1_mc661_hw_config.cat1_open_pin = CAT1_OPEN_Pin;
    cat1_mc661_hw_config.cat1_reset_port = CAT1_RESET_GPIO_Port;
    cat1_mc661_hw_config.cat1_reset_pin = CAT1_RESET_Pin;
    cat1_mc661_hw_config.uart_func = &uart_cat1_fun;
    cat1_mc661_register(&cat1_mc661_hw_config);
}

/*****************************************************************************
* @brief  : 
* @param  : None
* @retval : 
* @date   : 2023-11-28
* @note   : 
*****************************************************************************/
void bsp_device_init(void)
{
    //fpga_ps_load();
    spi_flash.flash_open();
}

/*****************************************************************************
* @brief  : ST外部中断执行函数
* @param  : GPIO_Pin: [输入/出] 
* @retval : 
* @date   : 2024-01-16
* @note   : 
*****************************************************************************/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    switch(GPIO_Pin)
    {
        case ECR8668_4G_DW_ADC_OVERLOAD_Pin:
            BSP_866x_adc_warning_exti_handle(&h8668_LTE);
            break;
        
        case ECR8668_4G_DW_TIA_OVERLOAD_Pin:
            BSP_866x_tia_warning_exti_handle(&h8668_LTE);
            break;
        
        case ECR8668_NR_ADC_OVERLOAD_Pin:
            BSP_866x_adc_warning_exti_handle(&h8668_NR);
            break;
        
        case ECR8668_NR_TIA_OVERLOAD_Pin:
            BSP_866x_tia_warning_exti_handle(&h8668_NR);
            break;
        
        case ECR8668_MIMO_ADC_OVERLOAD_Pin:
            BSP_866x_adc_warning_exti_handle(&h8668_NR_MIMO);
            break;
        
        case ECR8668_MIMO_TIA_OVERLOAD_Pin:
            BSP_866x_tia_warning_exti_handle(&h8668_NR_MIMO);
            break;
        
        case MODEM_4G_UP_Pin:
            break;
        
        case MODEM_4G_DW_Pin:
            break;
        
        //case MODEM_5G_UP_Pin: // 和MIMO tia引脚冲突
        //    break;
        
        case MODEM_5G_DW_Pin:
            break;
        
        default:
            break;
    }
}

#elif(PRODUCT_MODEL == CMDT3521D)

#elif(PRODUCT_MODEL == CMDT3521E)

#elif(PRODUCT_MODEL == CMDT3521F)

#endif

