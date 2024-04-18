#include "ecr8668.h"
#include "hi_delay.h"
#include "hi_gpio.h"

/****************************************866x SPI 配置修改区********************************************/


uint32_t SwitchU32H_L(uint32_t data)
{
    uint32_t data_L = (data & 0xFFFF0000) >> 16;
    uint32_t data_H = (data & 0x0000FFFF) << 16;
    return (data_L + data_H);
}



//RSSI对照表,查寄存器获取的值在表中找到对应的RSSI功率值
typedef struct
{
    uint8_t rd_data;
    int8_t rssi_val;
} rssi_tab_t;

#define RSSI_TAB_LEN 60
rssi_tab_t RSSI_LIST[RSSI_TAB_LEN] =
{
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
};

/**
 * 读取RSSi值
 * trx8668_port_t spi_periph
 * ch: 0：通道0 1：通道1
 * return :rssi
**/
int8_t Ecr8668RssiRead(trx8668_port_t spi_periph, uint8_t ch)
{
    uint32_t rssr_read = 0, i = 0;
    int8_t rssi_val = 0;
    uint8_t c_data = 0;
    if(ch == 0)
    {
        rssr_read = ECR8660_Read(spi_periph, SPI_RW_EXTERNAL, 0x002009B0, 4); //默认读取4字节数据
    }
    else if(ch == 1)
    {
        rssr_read = ECR8660_Read(spi_periph, SPI_RW_EXTERNAL, 0x00200BB0, 4);
    }
    else
    {}
    c_data = rssr_read & 0x000000FF;

    for(i = 0; i < RSSI_TAB_LEN; i++)
    {
        if(c_data == RSSI_LIST[i].rd_data)//查表获取rssi值
        {
            rssi_val = RSSI_LIST[i].rssi_val;
            break;
        }
    }

    return rssi_val;
}

/**
 *  将16进制字符串转换为一个大端存储的16进制
**/
uint32_t HexCharToHex_MSB(char * databuf)
{
    long data, data1;
    char * endl;
    uint8_t i = 0, j = 0;
    char data_1[8] = {0};
    char data_2[8] = {0};
    uint8_t t = 0;
    for(i = 0; i < 4; i++) //配置数据位顺序发送
    {
        if(j > 3)
        {
            data_2[t] = databuf[8 - 2 * i];
            t = t + 1;
            data_2[t] = databuf[9 - 2 * i];
            t = t + 1;
            continue;
        }
        data_1[j] = databuf[8 - 2 * i];
        j = j + 1;
        data_1[j] = databuf[9 - 2 * i];
        j = j + 1;

    }
    data = strtol(data_1, &endl, 16);
    data = data << 16;
    data1 = strtol(data_2, &endl, 16);
    data = data | data1;
    return data;
}


/**
 *  将16进制字符串转换为一个小端存储的16进制
**/
uint32_t HexCharToHex_LSB(char * addrbuf)
{

    long data, data1;
    char * endl;
    uint8_t i = 0;
    char data_1[8] = {0};
    char data_2[8] = {0};
    for(i = 0; i < 4; i++)
    {
        data_1[i] = addrbuf[i + 2];
        data_2[i] = addrbuf[i + 6];
    }
    data = strtol(data_1, &endl, 16);
    data = data << 16;
    data1 = strtol(data_2, &endl, 16);
    data = data | data1;
    return data;
}




/**
 *  ch_mode(通道配置中模式选择)
 *  pcs    : 片选
 *  ch_mode：模式选择
**/
void Ecr866xChModeCfg(TRX8668_FPGA_t * h8668, uint32_t ch_mode)
{
    trx8668_port_t spi_perph = h8668->config.spi_port;

    switch(ch_mode)  //通道模式配置
    {
        case CH0_1T1R_PLC_TRX_NO_LO:
            ECR8660_write(spi_perph, SPI_RW_EXTERNAL, ECR866x_CH_MODE_ADDR, (ECR866x_CH_MODE_0));
            break;

        case CH0_1T1R_PLC_TRX_LO:
            ECR8660_write(spi_perph, SPI_RW_EXTERNAL, ECR866x_CH_MODE_ADDR, (ECR866x_CH_MODE_1));
            break;

        case CH0_1T_PLC:
            ECR8660_write(spi_perph, SPI_RW_EXTERNAL, ECR866x_CH_MODE_ADDR, (ECR866x_CH_MODE_2));
            break;

        case CH0_1R_PLC:
            ECR8660_write(spi_perph, SPI_RW_EXTERNAL, ECR866x_CH_MODE_ADDR, (ECR866x_CH_MODE_3));
            break;

        case CH0_1T1R_OWN_TRX_NO_LO:
            ECR8660_write(spi_perph, SPI_RW_EXTERNAL, ECR866x_CH_MODE_ADDR, (ECR866x_CH_MODE_4));
            break;

        case CH0_1T1R_OWN_TRX_LO:
            ECR8660_write(spi_perph, SPI_RW_EXTERNAL, ECR866x_CH_MODE_ADDR, (ECR866x_CH_MODE_5));
            break;

        case CH0_1T_OWN:
            ECR8660_write(spi_perph, SPI_RW_EXTERNAL, ECR866x_CH_MODE_ADDR, (ECR866x_CH_MODE_6));
            break;

        case CH0_1R_OWN:
            ECR8660_write(spi_perph, SPI_RW_EXTERNAL, ECR866x_CH_MODE_ADDR, (ECR866x_CH_MODE_7));
            break;

        case CH1_1T1R_PLC_TRX_NO_LO:
            ECR8660_write(spi_perph, SPI_RW_EXTERNAL, ECR866x_CH_MODE_ADDR, (ECR866x_CH_MODE_10));
            break;

        case CH1_1T1R_PLC_TRX_LO:
            ECR8660_write(spi_perph, SPI_RW_EXTERNAL, ECR866x_CH_MODE_ADDR, (ECR866x_CH_MODE_11));
            break;

        case CH1_1T_PLC:
            ECR8660_write(spi_perph, SPI_RW_EXTERNAL, ECR866x_CH_MODE_ADDR, (ECR866x_CH_MODE_12));
            break;

        case CH1_1R_PLC:
            ECR8660_write(spi_perph, SPI_RW_EXTERNAL, ECR866x_CH_MODE_ADDR, (ECR866x_CH_MODE_13));
            break;

        case CH1_1T1R_OWN_TRX_NO_LO:
            ECR8660_write(spi_perph, SPI_RW_EXTERNAL, ECR866x_CH_MODE_ADDR, (ECR866x_CH_MODE_14));
            break;

        case CH1_1T1R_OWN_TRX_LO:
            ECR8660_write(spi_perph, SPI_RW_EXTERNAL, ECR866x_CH_MODE_ADDR, (ECR866x_CH_MODE_15));
            break;

        case CH1_1T_OWN:
            ECR8660_write(spi_perph, SPI_RW_EXTERNAL, ECR866x_CH_MODE_ADDR, (ECR866x_CH_MODE_16));
            break;

        case CH1_1R_OWN:
            ECR8660_write(spi_perph, SPI_RW_EXTERNAL, ECR866x_CH_MODE_ADDR, (ECR866x_CH_MODE_17));
            break;

        default :
            rtt_printf(RTT_8668, RTT_TEXT_RED"error : 8668 %d ch mode=%d!\r\n", h8668->config.dev_number, ch_mode);
            return;
    }
    rtt_printf(RTT_8668, "Set 8668 ChMode %d. addr = 0x%08x ch_mode = %d\r\n", h8668->config.dev_number, ECR866x_CH_MODE_ADDR, ch_mode);

    h8668->ch_status.ch_mode = ch_mode;
}


/**
 *  (通道配置中带宽选择)
 *  pcs      : 片选
 *  bw_choose：带宽选择
 *函数参数: band_width 带宽
        #define PLC_LTE_10MHZ (10)         // LTE 10 MHz
        #define PLC_LTE_15MHZ (15)         // LTE 15 MHz
        #define PLC_LTE_20MHZ (20)         // LTE 20 MHz
        #define PLC_LTE_25MHZ (25)         // LTE 15 MHz
        #define PLC_LTE_30MHZ (30)         // LTE 20 MHz
        #define PLC_LTE_50MHZ (50)         // LTE 20 MHz
        #define PLC_NR_20MHZ (20)          // NR 20 MHz
        #define PLC_NR_60MHZ (60)          // NR 60 MHz
        #define PLC_NR_80MHZ (80)          // NR 80 MHz
        #define PLC_NR_100MHZ (100)        // NR 100 MHz
**/
void Ecr866xBwChoose(TRX8668_FPGA_t * h8668, RF_BAND_E band, uint32_t bw_choose)
{
    uint32_t bw_reg_val = 0;
    trx8668_port_t spi_perph = h8668->config.spi_port;

    switch(band)
    {
        case BAND_3:
            switch(bw_choose)        //带宽配置
            {
                case PLC_LTE_20MHZ:
                    bw_reg_val = USER_LTE_B3_20MHZ;
                    break;

                case PLC_LTE_25MHZ:
                    bw_reg_val = USER_LTE_B3_25MHZ;
                    break;

                default:
                    rtt_printf(RTT_8668, RTT_TEXT_RED"error : 8668 %d bw = %d invalid!\r\n", h8668->config.dev_number, bw_choose);
                    return;
            }
            break;

        case BAND_8:
            switch(bw_choose)        //带宽配置
            {
                case PLC_LTE_10MHZ:
                    bw_reg_val = USER_LTE_B8_10MHZ;
                    break;

                case PLC_LTE_15MHZ:
                    bw_reg_val = USER_LTE_B8_15MHZ;
                    break;

                default:
                    rtt_printf(RTT_8668, RTT_TEXT_RED"error : 8668 %d bw = %d invalid!\r\n", h8668->config.dev_number, bw_choose);
                    return;
            }
            break;

        case BAND_39:
            switch(bw_choose)        //带宽配置
            {
                case PLC_LTE_10MHZ:
                    bw_reg_val = USER_LTE_B39_10MHZ;
                    break;

                case PLC_LTE_20MHZ:
                    bw_reg_val = USER_LTE_B39_20MHZ;
                    break;

                case PLC_LTE_30MHZ:
                    bw_reg_val = USER_LTE_B39_30MHZ;
                    break;

                default:
                    rtt_printf(RTT_8668, RTT_TEXT_RED"error : 8668 %d bw = %d invalid!\r\n", h8668->config.dev_number, bw_choose);
                    return;
            }
            break;

        case BAND_40:
            switch(bw_choose)        //带宽配置
            {
                case PLC_LTE_20MHZ:
                    bw_reg_val = USER_LTE_B40_20MHZ;
                    break;

                case PLC_LTE_50MHZ:
                    bw_reg_val = USER_LTE_B40_50MHZ;
                    break;

                default:
                    rtt_printf(RTT_8668, RTT_TEXT_RED"error : 8668 %d bw = %d invalid!\r\n", h8668->config.dev_number, bw_choose);
                    return;
            }
            break;

        case BAND_41:
        case BAND_41M:
            switch(bw_choose)        //带宽配置
            {
                case PLC_NR_20MHZ:
                    bw_reg_val = USER_NR_N41_20MHZ;
                    break;

                case PLC_NR_60MHZ:
                    bw_reg_val = USER_NR_N41_60MHZ;
                    break;

                case PLC_NR_80MHZ:
                    bw_reg_val = USER_NR_N41_80MHZ;
                    break;

                case PLC_NR_100MHZ:
                    bw_reg_val = USER_NR_N41_100MHZ;
                    break;

                default:
                    rtt_printf(RTT_8668, RTT_TEXT_RED"error : 8668 %d bw = %d invalid!\r\n", h8668->config.dev_number, bw_choose);
                    return;
            }
            break;

        default:
            rtt_printf(RTT_8668, RTT_TEXT_RED"error : 8668 %d bw=%d invalid!\r\n", h8668->config.dev_number, bw_choose);
            return;
    }

    ECR8660_write(spi_perph, SPI_RW_EXTERNAL, ECR866x_BW_addr, bw_reg_val);
    rtt_printf(RTT_8668, "Set 8668 BwChoose %d. addr = 0x%08x BwChoose = %dMHz\r\n", h8668->config.dev_number, ECR866x_BW_addr, bw_choose);

    h8668->ch_status.bw_choose = bw_choose;
}

/**
 *  (通道配置中带宽选择)
 *  pcs      : 片选
 *  bw_choose：带宽选择
 *  函数参数: band_width 带宽
        #define PLC_LTE_10MHZ (10)         // LTE 10 MHz
        #define PLC_LTE_15MHZ (15)         // LTE 15 MHz
        #define PLC_LTE_20MHZ (20)         // LTE 20 MHz
        #define PLC_LTE_25MHZ (25)         // LTE 25 MHz
        #define PLC_LTE_30MHZ (30)         // LTE 30 MHz
        #define PLC_LTE_50MHZ (50)         // LTE 50 MHz
        #define PLC_NR_20MHZ (20)          // NR 20 MHz
        #define PLC_NR_60MHZ (60)          // NR 60 MHz
        #define PLC_NR_80MHZ (80)          // NR 80 MHz
        #define PLC_NR_100MHZ (100)        // NR 100 MHz
**/
void Ecr866xBwDirectChoose(TRX8668_FPGA_t * h8668, RF_BAND_E band, uint32_t bw_choose)
{
   
    vTaskDelay(100);                                // 配置通道前增加延时,防止通道配置失败

    // Ecr866xChModeCfg(h8668, CH0_1T1R_OWN_TRX_LO); // 通道模式配置

    Ecr866xBwChoose(h8668, h8668->band, bw_choose); // 带宽配置

    // Ecr866xRxFreqCfg(h8668, h8668->ch, h8668->ch_status.rx_freq); // 接收频率配置

    // Ecr866xRxGainCfg(h8668, h8668->ch, h8668->ch_status.rx_gain); // RX增益偏移配置

    // Ecr866xTxGainCfg(h8668, h8668->ch, h8668->ch_status.tx_gain); // TX增益偏移配置

    // vTaskDelay(1000); //

    Ecr866xPortCfg(h8668, h8668->config.port); // 端口配置

    // Ecr866xLoopCfg(h8668, LOOP_CH0_OPEN); // 回环配置

    // ECR8660_write(h8668->config.spi_port, SPI_RW_EXTERNAL, ECR866x_CLOSE_RBDP_ADDR, (ECR866x_CLOSE_RBDP_DATA)); // 关闭RBDP

    h8668->ch_status.bw_choose = bw_choose;

}

/**
 *  (通道配置中发送频率配置)
 *  pcs      : 片选
 *  ch       : CH选择 0->CH0-  1->CH1
 *  tx_freq  ：发送频率
**/
void Ecr866xTxFreqCfg(TRX8668_FPGA_t * h8668, uint32_t ch, uint32_t tx_freq)
{
    trx8668_port_t spi_perph = h8668->config.spi_port;
    uint32_t addr;

    if(ch == 0)
    {
        addr = ECR866x_CH0_TXFREQ_ADDR;
    }
    else if(ch == 1)
    {
        addr = ECR866x_CH1_TXFREQ_ADDR;
    }
    tx_freq = tx_freq * 100;
    ECR8660_write(spi_perph, SPI_RW_EXTERNAL, addr, (tx_freq));

    rtt_printf(RTT_8668, "Set 8668 TxFreq %d. addr = 0x%08x tx_freq = %d\r\n", h8668->config.dev_number, addr, tx_freq);

    h8668->ch_status.tx_freq = tx_freq;

    vTaskDelay(100);
}

/**
 *  (通道配置中接收频率配置)
 *  pcs      : 片选
 *  ch       : CH选择 0->CH0  1->CH1
 *  rx_freq  ：接收频率
**/
void Ecr866xRxFreqCfg(TRX8668_FPGA_t * h8668, uint32_t ch, uint32_t rx_freq)
{
    trx8668_port_t spi_perph = h8668->config.spi_port;
    uint32_t addr;

    if(ch == 0)
    {
        addr = ECR866x_CH0_RXFREQ_ADDR;
    }
    else if(ch == 1)
    {
        addr = ECR866x_CH1_RXFREQ_ADDR;
    }
    rx_freq = rx_freq * 100;
    ECR8660_write(spi_perph, SPI_RW_EXTERNAL, addr, (rx_freq));

    rtt_printf(RTT_8668, "Set 8668 RxFreq %d. addr = 0x%08x rx_freq = %d\r\n", h8668->config.dev_number, addr, rx_freq);
    h8668->ch_status.rx_freq = rx_freq;

    vTaskDelay(100);
}


/**
 *  (通道配置中发送初始增益配置)
 *  pcs      : 片选
 *  ch       : CH选择 0->CH0  1->CH1
 *  tx_gain  ：增益偏移量
**/
void Ecr866xTxGainCfg(TRX8668_FPGA_t * h8668, uint32_t ch, int tx_gain)
{
    trx8668_port_t spi_perph = h8668->config.spi_port;
    uint32_t addr;
    uint32_t txgain = 0;

    if(ch == 0)
    {
        addr = ECR866x_CH0_TXGAIN_ADDR;
        txgain = ECR866x_CH0_TXGAIN_DEFAULT_DATA;
    }
    else if(ch == 1)
    {
        addr = ECR866x_CH1_TXGAIN_ADDR;
        txgain = ECR866x_CH1_TXGAIN_DEFAULT_DATA;
    }
    txgain = txgain + tx_gain;

    ECR8660_write(spi_perph, SPI_RW_EXTERNAL, addr, (txgain));

    rtt_printf(RTT_8668, "Set 8668 TxGain %d. addr = 0x%08x tx_gain = %d\r\n", h8668->config.dev_number, addr, tx_gain);
    h8668->ch_status.tx_gain = tx_gain;
}




/**
 *  (通道配置中接收增益配置)
 *  pcs      : 片选
 *  ch       : CH选择 0->CH0  1->CH1
 *  rx_gain  ：增益偏移量
**/
void Ecr866xRxGainCfg(TRX8668_FPGA_t * h8668, uint32_t ch, int rx_gain)
{
    trx8668_port_t spi_perph = h8668->config.spi_port;
    uint32_t addr;
    uint32_t rxgain = 0;

    if(ch == 0)
    {
        addr = ECR866x_CH0_RXGAIN_ADDR;
        rxgain = ECR866x_CH0_RXGAIN_DEFAULT_DATA;
    }
    else if(ch == 1)
    {
        addr = ECR866x_CH1_RXGAIN_ADDR;
        rxgain = ECR866x_CH1_RXGAIN_DEFAULT_DATA;
    }
    rxgain = rxgain + rx_gain;

    ECR8660_write(spi_perph, SPI_RW_EXTERNAL, addr, (rxgain));

    rtt_printf(RTT_8668, "Set 8668 RxGain %d. addr = 0x%08x rx_gain = %d\r\n", h8668->config.dev_number, addr, rx_gain);
    h8668->ch_status.rx_gain = rx_gain;
}


/**
 *  (通道配置中端口配置)
 *  pcs      : 片选
 *  ch       : CH选择 0->CH0  1->CH1
 *  port     ：端口配置编号
**/
void Ecr866xPortCfg(TRX8668_FPGA_t * h8668, uint32_t port)
{
    trx8668_port_t spi_perph = h8668->config.spi_port;
    uint32_t addr = 0;

    switch(port)
    {
        case CH0_RXA_TXA:
            ECR8660_write(spi_perph, SPI_RW_EXTERNAL, ECR866x_CH0_PORT_ADDR, (ECR866x_CH0_PORT_011));
            addr = ECR866x_CH0_PORT_ADDR;
            // vTaskDelay(100);
            break;

        case CH0_RXB_TXA:
            ECR8660_write(spi_perph, SPI_RW_EXTERNAL, ECR866x_CH0_PORT_ADDR, (ECR866x_CH0_PORT_021));
            addr = ECR866x_CH0_PORT_ADDR;
            // vTaskDelay(100);
            break;

        case CH0_RXC_TXA:
            ECR8660_write(spi_perph, SPI_RW_EXTERNAL, ECR866x_CH0_PORT_ADDR, (ECR866x_CH0_PORT_031));
            addr = ECR866x_CH0_PORT_ADDR;
            // vTaskDelay(100);
            break;

        case CH0_RXA_TXB:
            ECR8660_write(spi_perph, SPI_RW_EXTERNAL, ECR866x_CH0_PORT_ADDR, (ECR866x_CH0_PORT_012));
            addr = ECR866x_CH0_PORT_ADDR;
            // vTaskDelay(100);
            break;

        case CH0_RXB_TXB:
            ECR8660_write(spi_perph, SPI_RW_EXTERNAL, ECR866x_CH0_PORT_ADDR, (ECR866x_CH0_PORT_022));
            addr = ECR866x_CH0_PORT_ADDR;
            // vTaskDelay(100);
            break;

        case CH0_RXC_TXB:
            ECR8660_write(spi_perph, SPI_RW_EXTERNAL, ECR866x_CH0_PORT_ADDR, (ECR866x_CH0_PORT_032));
            addr = ECR866x_CH0_PORT_ADDR;
            // vTaskDelay(100);
            break;

        case CH0_TXA:
            ECR8660_write(spi_perph, SPI_RW_EXTERNAL, ECR866x_CH0_PORT_ADDR, (ECR866x_CH0_PORT_001));
            addr = ECR866x_CH0_PORT_ADDR;
            // vTaskDelay(100);
            break;

        case CH0_TXB:
            ECR8660_write(spi_perph, SPI_RW_EXTERNAL, ECR866x_CH0_PORT_ADDR, (ECR866x_CH0_PORT_002));
            addr = ECR866x_CH0_PORT_ADDR;
            // vTaskDelay(100);
            break;

        case CH0_RXA:
            ECR8660_write(spi_perph, SPI_RW_EXTERNAL, ECR866x_CH0_PORT_ADDR, (ECR866x_CH0_PORT_010));
            addr = ECR866x_CH0_PORT_ADDR;
            // vTaskDelay(100);
            break;

        case CH0_RXB:
            ECR8660_write(spi_perph, SPI_RW_EXTERNAL, ECR866x_CH0_PORT_ADDR, (ECR866x_CH0_PORT_020));
            addr = ECR866x_CH0_PORT_ADDR;
            // vTaskDelay(100);
            break;

        case CH0_RXC:
            ECR8660_write(spi_perph, SPI_RW_EXTERNAL, ECR866x_CH0_PORT_ADDR, (ECR866x_CH0_PORT_030));
            addr = ECR866x_CH0_PORT_ADDR;
            // vTaskDelay(100);
            break;

        case CH1_RXA_TXA:
            ECR8660_write(spi_perph, SPI_RW_EXTERNAL, ECR866x_CH1_PORT_ADDR, (ECR866x_CH1_PORT_111));
            addr = ECR866x_CH1_PORT_ADDR;
            // vTaskDelay(100);
            break;

        case CH1_RXB_TXA:
            ECR8660_write(spi_perph, SPI_RW_EXTERNAL, ECR866x_CH1_PORT_ADDR, (ECR866x_CH1_PORT_121));
            addr = ECR866x_CH1_PORT_ADDR;
            // vTaskDelay(100);
            break;

        case CH1_RXC_TXA:
            ECR8660_write(spi_perph, SPI_RW_EXTERNAL, ECR866x_CH1_PORT_ADDR, (ECR866x_CH1_PORT_131));
            addr = ECR866x_CH1_PORT_ADDR;
            // vTaskDelay(100);
            break;

        case CH1_RXA_TXB:
            ECR8660_write(spi_perph, SPI_RW_EXTERNAL, ECR866x_CH1_PORT_ADDR, (ECR866x_CH1_PORT_112));
            addr = ECR866x_CH1_PORT_ADDR;
            // vTaskDelay(100);
            break;

        case CH1_RXB_TXB:
            ECR8660_write(spi_perph, SPI_RW_EXTERNAL, ECR866x_CH1_PORT_ADDR, (ECR866x_CH1_PORT_122));
            addr = ECR866x_CH1_PORT_ADDR;
            // vTaskDelay(100);
            break;

        case CH1_RXC_TXB:
            ECR8660_write(spi_perph, SPI_RW_EXTERNAL, ECR866x_CH1_PORT_ADDR, (ECR866x_CH1_PORT_132));
            addr = ECR866x_CH1_PORT_ADDR;
            // vTaskDelay(100);
            break;

        case CH1_TXA:
            ECR8660_write(spi_perph, SPI_RW_EXTERNAL, ECR866x_CH1_PORT_ADDR, (ECR866x_CH1_PORT_101));
            addr = ECR866x_CH1_PORT_ADDR;
            // vTaskDelay(100);
            break;

        case CH1_TXB:
            ECR8660_write(spi_perph, SPI_RW_EXTERNAL, ECR866x_CH1_PORT_ADDR, (ECR866x_CH1_PORT_102));
            addr = ECR866x_CH1_PORT_ADDR;
            // vTaskDelay(100);
            break;

        case CH1_RXA:
            ECR8660_write(spi_perph, SPI_RW_EXTERNAL, ECR866x_CH1_PORT_ADDR, (ECR866x_CH1_PORT_110));
            addr = ECR866x_CH1_PORT_ADDR;
            // vTaskDelay(100);
            break;

        case CH1_RXB:
            ECR8660_write(spi_perph, SPI_RW_EXTERNAL, ECR866x_CH1_PORT_ADDR, (ECR866x_CH1_PORT_120));
            addr = ECR866x_CH1_PORT_ADDR;
            // vTaskDelay(100);
            break;

        case CH1_RXC:
            ECR8660_write(spi_perph, SPI_RW_EXTERNAL, ECR866x_CH1_PORT_ADDR, (ECR866x_CH1_PORT_130));
            addr = ECR866x_CH1_PORT_ADDR;
            // vTaskDelay(100);
            break;

        default:
            rtt_printf(RTT_8668, RTT_TEXT_RED"error : 8668 %d port = %d!\r\n", h8668->config.dev_number, port);
            return;
    }
    rtt_printf(RTT_8668, "Set 8668 Port %d. addr = 0x%08x port = %d\r\n", h8668->config.dev_number, addr, port);
    h8668->ch_status.port = port;
    vTaskDelay(100);
}



/**
 *  (通道配置中回环配置)
 *  pcs      : 片选
 *  ch       : CH选择 0->CH0  1->CH1
 *  loop     ：端口配置编号
**/
void Ecr866xLoopCfg(TRX8668_FPGA_t * h8668, uint32_t loop)
{
    trx8668_port_t spi_perph = h8668->config.spi_port;

    switch(loop)         //回环配置
    {
        case LOOP_ALL_CLOSE:
            ECR8660_write(spi_perph, SPI_RW_EXTERNAL, ECR866x_LOOP_ADDR, (ECR866x_LOOP_0));
            break;

        case LOOP_ALL_OPEN:
            ECR8660_write(spi_perph, SPI_RW_EXTERNAL, ECR866x_LOOP_ADDR, (ECR866x_LOOP_D));
            break;

        case LOOP_CH0_OPEN:
            ECR8660_write(spi_perph, SPI_RW_EXTERNAL, ECR866x_LOOP_ADDR, (ECR866x_LOOP_E));
            break;

        case LOOP_CH1_OPEN:
            ECR8660_write(spi_perph, SPI_RW_EXTERNAL, ECR866x_LOOP_ADDR, (ECR866x_LOOP_F));
            break;

        case CH1_NCO_EN:
            ECR8660_write(spi_perph, SPI_RW_EXTERNAL, ECR866x_LOOP_ADDR, (ECR866x_LOOP_3));
            break;

        case CH0_NCO_EN:
            ECR8660_write(spi_perph, SPI_RW_EXTERNAL, ECR866x_LOOP_ADDR, (ECR866x_LOOP_4));
            break;

        default:
            rtt_printf(RTT_8668, RTT_TEXT_RED"error : 8668 %d loop = %d!\r\n", h8668->config.dev_number, loop);
            return;
    }

    rtt_printf(RTT_8668, "Set 8668 Loop %d. addr = 0x%08x loop = %d\r\n", h8668->config.dev_number, ECR866x_LOOP_ADDR, loop);
    h8668->ch_status.loop = loop;
}


/**
 *  (通道配置中RSSI检测模式使能)
 *  pcs      : 片选
 *  ch       : CH选择 0->CH0  1->CH1
**/
void Ecr866xRssiEn(TRX8668_FPGA_t * h8668, uint32_t ch)
{
    trx8668_port_t spi_perph = h8668->config.spi_port;

    if(ch == 0)
    {
        ECR8660_write(spi_perph, SPI_RW_EXTERNAL, ECR866x_CH0_RSSI_EN_ADDR0, (ECR866x_CH0_RSSI_EN_DATA0));
        rtt_printf(RTT_8668, "Set 8668 %d ch0 RssiEn. addr = 0x%08x data = 0x%08x\r\n", h8668->config.dev_number, ECR866x_CH0_RSSI_EN_ADDR0, ECR866x_CH0_RSSI_EN_DATA0);
        ECR8660_write(spi_perph, SPI_RW_EXTERNAL, ECR866x_CH0_RSSI_EN_ADDR1, (ECR866x_CH0_RSSI_EN_DATA1));
        rtt_printf(RTT_8668, "Set 8668 %d ch0 RssiEn. addr = 0x%08x data = 0x%08x\r\n", h8668->config.dev_number, ECR866x_CH0_RSSI_EN_ADDR1, ECR866x_CH0_RSSI_EN_DATA1);
    }
    else if(ch == 1)
    {
        ECR8660_write(spi_perph, SPI_RW_EXTERNAL, ECR866x_CH1_RSSI_EN_ADDR0, (ECR866x_CH1_RSSI_EN_DATA0));
        rtt_printf(RTT_8668, "Set 8668 %d ch1 RssiEn. addr = 0x%08x data = 0x%08x\r\n", h8668->config.dev_number, ECR866x_CH1_RSSI_EN_ADDR0, ECR866x_CH1_RSSI_EN_DATA0);
        ECR8660_write(spi_perph, SPI_RW_EXTERNAL, ECR866x_CH1_RSSI_EN_ADDR1, (ECR866x_CH1_RSSI_EN_DATA1));
        rtt_printf(RTT_8668, "Set 8668 %d ch1 RssiEn. addr = 0x%08x data = 0x%08x\r\n", h8668->config.dev_number, ECR866x_CH1_RSSI_EN_ADDR1, ECR866x_CH1_RSSI_EN_DATA1);
    }
    else
    {
        rtt_printf(RTT_8668, RTT_TEXT_RED"error : 8668 %d Ecr866xRssiEn ch = %d!\r\n", h8668->config.dev_number, ch);
        return;
    }
}


/**
 *  NCO模式通道配置
**/
void Ecr866xNcoCfg(TRX8668_FPGA_t * h8668, Ecr866xNco_T tNcoCfg)
{
    uint8_t ch = 0xFF;
    if(tNcoCfg.ch_mode < CH1_1T1R_PLC_TRX_NO_LO)
    {
        ch = 0;//CH0
    }
    else
    {
        ch = 1;//CH1
    }
    if(ch == 0xFF)return;//???这是什么

    Ecr866xChModeCfg(h8668, tNcoCfg.ch_mode);

    Ecr866xBwChoose(h8668, h8668->band, tNcoCfg.bw_choose);

    Ecr866xTxFreqCfg(h8668, ch, tNcoCfg.tx_freq); //发送频率配置

    Ecr866xTxGainCfg(h8668, ch, tNcoCfg.tx_gain); //发送增益配置

    Ecr866xPortCfg(h8668, tNcoCfg.port);        //端口配置

    Ecr866xLoopCfg(h8668, tNcoCfg.loop);        //回环配置

    rtt_printf(RTT_8668, "8668 %d Ecr866xNcoCfg done\r\n", h8668->config.dev_number);
}


/**
 *  rssi检测模式通道配置
**/
void Ecr866xRssiModeCfg(TRX8668_FPGA_t * h8668, Ecr866xRssi_T tRssCfg)
{
    uint8_t ch = 0xFF;
    if(tRssCfg.ch_mode < CH1_1T1R_PLC_TRX_NO_LO)
    {
        ch = 0;//CH0
    }
    else
    {
        ch = 1;//CH1
    }
    if(ch == 0xFF)return;

    Ecr866xChModeCfg(h8668, tRssCfg.ch_mode);   //通道模式配置

    Ecr866xBwChoose(h8668, h8668->band, tRssCfg.bw_choose); //带宽配置

    Ecr866xRxFreqCfg(h8668, ch, tRssCfg.rx_freq); //接收频率配置

    Ecr866xRxGainCfg(h8668, ch, tRssCfg.rx_gain); //接收增益配置

    Ecr866xPortCfg(h8668, tRssCfg.port);        //端口配置

    Ecr866xRssiEn(h8668, ch);

    rtt_printf(RTT_8668, "8668 %d Ecr866xRssiModeCfg done\r\n", h8668->config.dev_number);
}




/**
 *  866x 接收频率配置（在通道配置外单独生效）
 *  h8668    : 片选
 *  ch      ：通道选择
 *  rx_freq ：接收频率,单位100kHz
**/
void Ecr866xSetRxFreq(TRX8668_FPGA_t * h8668, uint32_t ch, uint32_t rx_freq)
{
    trx8668_port_t spi_perph = h8668->config.spi_port;

//    Ecr866xRxFreqCfg(pcs,ch,rx_freq);
    rx_freq = rx_freq * 100;
    ECR8660_write(spi_perph, SPI_RW_EXTERNAL, ECR866x_CH0_RXFREQ_ADDR, (rx_freq));
    rtt_printf(RTT_8668, "8668 %d Ecr866xSetRxFreq. addr = 0x%08x rx_freq = %d\r\n", h8668->config.dev_number, ECR866x_CH0_RXFREQ_ADDR, rx_freq);

    if(ch == 0)
    {
        ECR8660_write(spi_perph, SPI_RW_EXTERNAL, ECR866x_CH0_RXFREQ_SAVE_ADDR, (ECR866x_CH0_RXFREQ_SAVE_DATA));
        vTaskDelay(100);
        rtt_printf(RTT_8668, "[info]8668 %d ch0. addr = 0x%08x data = 0x%08x\r\n", h8668->config.dev_number, ECR866x_CH0_RXFREQ_SAVE_ADDR, ECR866x_CH0_RXFREQ_SAVE_DATA);
    }
    else if(ch == 1)
    {
        ECR8660_write(spi_perph, SPI_RW_EXTERNAL, ECR866x_CH1_RXFREQ_SAVE_ADDR, (ECR866x_CH1_RXFREQ_SAVE_DATA));
        vTaskDelay(100);
        rtt_printf(RTT_8668, "[info]8668 %d ch1. addr = 0x%08x data = 0x%08x\r\n", h8668->config.dev_number, ECR866x_CH1_RXFREQ_SAVE_ADDR, ECR866x_CH1_RXFREQ_SAVE_DATA);
    }
    else
    {
        rtt_printf(RTT_8668, RTT_TEXT_RED"error : 8668 %d Ecr866xSetRxFreq ch = %d!\r\n", h8668->config.dev_number, ch);
        return;
    }
    h8668->ch_status.rx_freq = rx_freq;
}

/**
 *  866x 发送频率配置（在通道配置外单独生效）
 *  h8668    : 片选
 *  ch      ：通道选择
 *  tx_freq ：发送频率
**/
void Ecr866xSetTxFreq(TRX8668_FPGA_t * h8668, uint32_t ch, uint32_t tx_freq)
{
    trx8668_port_t spi_perph = h8668->config.spi_port;

//    Ecr866xTxFreqCfg(pcs,ch,tx_freq);
    tx_freq = tx_freq * 100;
    ECR8660_write(spi_perph, SPI_RW_EXTERNAL, ECR866x_CH0_RXFREQ_ADDR, (tx_freq));
    rtt_printf(RTT_8668, "8668 %d Ecr866xSetTxFreq. addr = 0x%08x tx_freq = %d\r\n", h8668->config.dev_number, ECR866x_CH0_RXFREQ_ADDR, tx_freq);

    if(ch == 0)
    {
        ECR8660_write(spi_perph, SPI_RW_EXTERNAL, ECR866x_CH0_TXFREQ_SAVE_ADDR, (ECR866x_CH0_TXFREQ_SAVE_DATA));
        rtt_printf(RTT_8668, "[info]8668 %d ch0, addr = 0x%08x data = 0x%08x\r\n", h8668->config.dev_number, ECR866x_CH0_TXFREQ_SAVE_ADDR, ECR866x_CH0_TXFREQ_SAVE_DATA);
        vTaskDelay(100);
    }
    else if(ch == 1)
    {
        ECR8660_write(spi_perph, SPI_RW_EXTERNAL, ECR866x_CH1_TXFREQ_SAVE_ADDR, (ECR866x_CH1_TXFREQ_SAVE_DATA));
        vTaskDelay(100);
        rtt_printf(RTT_8668, "[info]8668 %d ch1, addr = 0x%08x data = 0x%08x\r\n", h8668->config.dev_number, ECR866x_CH1_TXFREQ_SAVE_ADDR, ECR866x_CH1_TXFREQ_SAVE_DATA);
    }
    else
    {
        rtt_printf(RTT_8668, RTT_TEXT_RED"error : 8668 %d Ecr866xSetTxFreq ch = %d!\r\n", h8668->config.dev_number, ch);
        return;
    }
    h8668->ch_status.tx_freq = tx_freq;
}

/**
 *  866x 接收增益配置（在通道配置外单独生效）
 *  h8668    : 片选
 *  ch      ：通道选择
 *  rx_gain ：发送频率
**/
void Ecr866xSetRxGain(TRX8668_FPGA_t * h8668, uint32_t ch, int rx_gain)
{
    trx8668_port_t spi_perph = h8668->config.spi_port;

    Ecr866xRxGainCfg(h8668, ch, rx_gain);

    if(ch == 0)
    {
        ECR8660_write(spi_perph, SPI_RW_EXTERNAL, ECR866x_CH0_RXGAIN_SAVE_ADDR, (ECR866x_CH0_RXGAIN_SAVE_DATA));
        vTaskDelay(100);
        rtt_printf(RTT_8668, "[info]8668 %d ch0, addr = 0x%08x data = 0x%08x\r\n", h8668->config.dev_number, ECR866x_CH0_TXFREQ_SAVE_ADDR, ECR866x_CH0_TXFREQ_SAVE_DATA);
    }
    else if(ch == 1)
    {
        ECR8660_write(spi_perph, SPI_RW_EXTERNAL, ECR866x_CH1_RXGAIN_SAVE_ADDR, (ECR866x_CH1_RXGAIN_SAVE_DATA));
        vTaskDelay(100);
        rtt_printf(RTT_8668, "[info]8668 %d ch1, addr = 0x%08x data = 0x%08x\r\n", h8668->config.dev_number, ECR866x_CH1_RXGAIN_SAVE_ADDR, ECR866x_CH1_RXGAIN_SAVE_DATA);
    }
    else
    {
        rtt_printf(RTT_8668, RTT_TEXT_RED"error : 8668 %d Ecr866xSetRxGain ch = %d!\r\n", h8668->config.dev_number, ch);
        return;
    }
    h8668->ch_status.rx_gain = rx_gain;

}

/**
 *  866x 发送增益配置（在通道配置外单独生效）
 *  h8668    : 片选
 *  ch      ：通道选择
 *  tx_gain ：发送频率
**/
void Ecr866xSetTxGain(TRX8668_FPGA_t * h8668, uint32_t ch, int tx_gain)
{
    trx8668_port_t spi_perph = h8668->config.spi_port;

    Ecr866xTxGainCfg(h8668, ch, tx_gain);

    if(ch == 0)
    {
        ECR8660_write(spi_perph, SPI_RW_EXTERNAL, ECR866x_CH0_TXGAIN_SAVE_ADDR, (ECR866x_CH0_TXGAIN_SAVE_DATA));
        vTaskDelay(100);
        rtt_printf(RTT_8668, "[info]8668 %d ch0, addr = 0x%08x data = 0x%08x\r\n", h8668->config.dev_number, ECR866x_CH0_TXGAIN_SAVE_ADDR, ECR866x_CH0_TXGAIN_SAVE_DATA);
    }
    else if(ch == 1)
    {
        ECR8660_write(spi_perph, SPI_RW_EXTERNAL, ECR866x_CH1_TXGAIN_SAVE_ADDR, (ECR866x_CH1_TXGAIN_SAVE_DATA));
        vTaskDelay(100);
    }
    else
    {
        rtt_printf(RTT_8668, RTT_TEXT_RED"error : 8668 %d Ecr866xSetTxGain ch = %d!\r\n", h8668->config.dev_number, ch);
        return;
    }
    h8668->ch_status.tx_gain = tx_gain;
}

/**
 *  866x 接收增益配置（动态增益配置，快速生效）
 *  h8668    : 片选
 *  ch      ：通道选择
 *  ch_gain 通道增益
**/
void Ecr866xSetGain(TRX8668_FPGA_t * h8668, uint32_t ch, int ch_gain)
{
//     trx8668_port_t spi_perph = h8668->config.spi_port;
//     uint32_t RxSzGainAddr;
//     uint32_t RxMnGainAddr;
//     uint32_t TxSzGainAddr;
//     int set_val = 0;

//     if(ch == 0)
//     {
//         RxSzGainAddr = ECR_CH0_RX_SZ_GAIN_ADDR;
//         RxMnGainAddr = ECR_CH0_RX_MN_GAIN_ADDR;
//         TxSzGainAddr = ECR_CH0_TX_SZ_GAIN_ADDR;
// //        TxMnGainAddr = ECR_CH0_TX_MN_GAIN_ADDR;
//     }
//     else if(ch == 1)
//     {
//         RxSzGainAddr = ECR_CH1_RX_SZ_GAIN_ADDR;
//         RxMnGainAddr = ECR_CH1_RX_MN_GAIN_ADDR;
//         TxSzGainAddr = ECR_CH1_TX_SZ_GAIN_ADDR;
// //        TxMnGainAddr = ECR_CH1_TX_MN_GAIN_ADDR;
//     }
//     else
//     {
//         rtt_printf(RTT_8668, RTT_TEXT_RED"error : 8668 %d Ecr866xSetGain ch = %d!\r\n", h8668->config.dev_number, ch);
//         return;
//     }

//     if(ch_gain < 0)
//     {
//         set_val = 0;
//     }
//     else if(ch_gain >= (MAX_ALC_GAIN_NUM - 1))
//     {
//         set_val = MAX_ALC_GAIN_NUM - 1;
//     }
//     else
//     {
//         ;
//     }

//     ECR8660_write(spi_perph, SPI_RW_EXTERNAL, RxSzGainAddr, (h8668->gain_tab->gain_tab[set_val][0])); //设置RX数字增益
//     ECR8660_write(spi_perph, SPI_RW_EXTERNAL, RxMnGainAddr, (h8668->gain_tab->gain_tab[set_val][1])); //设置RX模拟增益
//     ECR8660_write(spi_perph, SPI_RW_EXTERNAL, TxSzGainAddr, (h8668->gain_tab->gain_tab[set_val][2])); //设置TX数字增益
// //        ECR8660_write(spi_perph,SPI_RW_EXTERNAL,TxMnGainAddr,(SwitchU32H_L(default_gain_tab[h8668->switching_Gain_list_flag].gain_tab[data][3])));//设置TX模拟增益

//     h8668->ch_status.ch_gain = ch_gain;
//     rtt_printf(RTT_8668, "8668 %d Ecr866xSetGain %d\r\n", h8668->config.dev_number, ch_gain);

}

/**
 *  866x 频率查询
 *  h8668    : 片选
 *  ch      ：通道选择
**/
uint32_t Ecr866xGetFreq(TRX8668_FPGA_t * h8668, uint32_t ch)
{
    trx8668_port_t spi_perph = h8668->config.spi_port;
    uint32_t read = 0;

    switch(ch)
    {
        case ECR_QUERY_FREQ_CH0_RX:
            read = ECR8660_Read(spi_perph, SPI_RW_EXTERNAL, ECR_QUERY_FREQ_CH0_RX_ADDR, 4);
            break;

        case ECR_QUERY_FREQ_CH1_RX:
            read = ECR8660_Read(spi_perph, SPI_RW_EXTERNAL, ECR_QUERY_FREQ_CH1_RX_ADDR, 4);
            break;

        case ECR_QUERY_FREQ_CH0_TX:
            read = ECR8660_Read(spi_perph, SPI_RW_EXTERNAL, ECR_QUERY_FREQ_CH0_TX_ADDR, 4);
            break;

        case ECR_QUERY_FREQ_CH1_TX:
            read = ECR8660_Read(spi_perph, SPI_RW_EXTERNAL, ECR_QUERY_FREQ_CH1_TX_ADDR, 4);
            break;

        default:
            rtt_printf(RTT_8668, RTT_TEXT_RED"error : 8668 %d Ecr866xGetFreq ch = %d!\r\n", h8668->config.dev_number, ch);
            return 0;
    }
    rtt_printf(RTT_8668, "8668 %d Ecr866xGetFreq %d\r\n", h8668->config.dev_number, read);
    return read;
}

/**
 *  866x rssi查询
 *  h8668    : 片选
 *  ch      ：通道选择
**/
uint8_t Ecr66xGetRssi(TRX8668_FPGA_t * h8668, uint32_t ch)
{
    uint32_t read_data;
    trx8668_port_t spi_perph = h8668->config.spi_port;
    uint32_t addr;
    uint8_t ret = 0xff;

    if(ch == 0)
    {
        addr = ECR866x_CH0_RSSI_READ_ADDR;
    }
    else if(ch == 1)
    {
        addr = ECR866x_CH1_RSSI_READ_ADDR;
    }
    else
    {
        rtt_printf(RTT_8668, RTT_TEXT_RED"error : 8668 %d Ecr66xGetRssi ch = %d!\r\n", h8668->config.dev_number, ch);
        return ret;
    }
    Ecr866xRssiEn(h8668,ch);
    read_data = ECR8660_Read(spi_perph, SPI_RW_EXTERNAL, addr, 4);
    ret = read_data & 0x000000FF;

    rtt_printf(RTT_8668, "8668 %d Ecr66xGetRssi %d\r\n", h8668->config.dev_number, ret);
    return ret;
}


/**
 *  866x 增益查询
 *  h8668    : 片选
 *  ch      ：通道选择
**/
uint32_t Ecr866xGetGain(TRX8668_FPGA_t * h8668, uint32_t ch)
{
    trx8668_port_t spi_perph = h8668->config.spi_port;
    uint32_t read;

    switch(ch)
    {
        case ECR_QUERY_GAIN_CH0_RX:
            read = ECR8660_Read(spi_perph, SPI_RW_EXTERNAL, ECR_QUERY_GAIN_CH0_RX_ADDR, 4);
            rtt_printf(RTT_8668, "8668 %d Ecr866xGet ch0 RX Gain %d\r\n", h8668->config.dev_number, read);
            break;

        case ECR_QUERY_GAIN_CH1_RX:
            read = ECR8660_Read(spi_perph, SPI_RW_EXTERNAL, ECR_QUERY_GAIN_CH1_RX_ADDR, 4);
            rtt_printf(RTT_8668, "8668 %d Ecr866xGet ch1 RX Gain %d\r\n", h8668->config.dev_number, read);
            break;

        case ECR_QUERY_GAIN_CH0_TX:
            read = ECR8660_Read(spi_perph, SPI_RW_EXTERNAL, ECR_QUERY_GAIN_CH0_TX_ADDR, 4);
            rtt_printf(RTT_8668, "8668 %d Ecr866xGet ch0 TX Gain %d\r\n", h8668->config.dev_number, read);
            break;

        case ECR_QUERY_GAIN_CH1_TX:
            read = ECR8660_Read(spi_perph, SPI_RW_EXTERNAL, ECR_QUERY_GAIN_CH1_TX_ADDR, 4);
            rtt_printf(RTT_8668, "8668 %d Ecr866xGet ch1 TX Gain %d\r\n", h8668->config.dev_number, read);
            break;

        default:
            rtt_printf(RTT_8668, RTT_TEXT_RED"error : 8668 %d Ecr866xGetGain ch = %d!\r\n", h8668->config.dev_number, ch);
            return 0;
    }
    return read;
}

/**
 *  866x 本振校准
 *  h8668    : 片选
 *  ch      ：通道选择 0/1 0->CH0-TX0  1->CH1-Tx1
 *  I       ：本振校准参数I
 *  Q       ：本振校准参数Q
**/
void BSP_866xCailLoSet(TRX8668_FPGA_t * h8668, uint32_t ch, uint32_t I, uint32_t Q)
{
    trx8668_port_t spi_perph = h8668->config.spi_port;
    uint32_t addr_I;
    uint32_t addr_Q;

    if(ch == 0)
    {
        addr_I = ECR_CH0_CAIL_LO_I_ADDR;
        addr_Q = ECR_CH0_CAIL_LO_Q_ADDR;
    }
    else if(ch == 1)
    {
        addr_I = ECR_CH1_CAIL_LO_I_ADDR;
        addr_Q = ECR_CH1_CAIL_LO_Q_ADDR;
    }
    else
    {
        rtt_printf(RTT_8668, RTT_TEXT_RED"error : 8668 %d BSP_866xCailLoSet ch = %d!\r\n", h8668->config.dev_number, ch);
        return;
    }
//    I = I | 0x017F0000;
//    Q = Q | 0x017F0000;
    if(I)
    {
        rtt_printf(RTT_8668, "8668 %d BSP_866xCailLoSet LO I %x\r\n", h8668->config.dev_number, I);
        ECR8660_write(spi_perph, SPI_RW_EXTERNAL, addr_I, (I));
    }
    if(Q)
    {
        rtt_printf(RTT_8668, "8668 %d BSP_866xCailLoSet LO Q %x\r\n", h8668->config.dev_number, Q);
        ECR8660_write(spi_perph, SPI_RW_EXTERNAL, addr_Q, (Q));
    }

}

/**
 *  866x 镜像校准
 *  h8668    : 片选
 *  ch      ：通道选择 0/1 0->CH0-TX0  1->CH1-Tx1
 *  data    ：校准值
**/
void BSP_866xCailMirrorSet(TRX8668_FPGA_t * h8668, uint32_t ch, uint32_t data)
{
    trx8668_port_t spi_perph = h8668->config.spi_port;
    uint32_t addr;

    if(ch == 0)
    {
        addr = ECR_CH0_CAIL_MIROR_ADDR;
    }
    else if(ch == 1)
    {
        addr = ECR_CH1_CAIL_MIROR_ADDR;
    }
    else
    {
        rtt_printf(RTT_8668, RTT_TEXT_RED"error : 8668 %d BSP_866xCailLoSet ch = %d!\r\n", h8668->config.dev_number, ch);
        return;
    }
    ECR8660_write(spi_perph, SPI_RW_EXTERNAL, addr, (data));
    rtt_printf(RTT_8668, "8668 %d BSP_866xCailMirrorSet %x\r\n", h8668->config.dev_number, data);
}

/**
 *  866x 通道关闭/打开
 *  h8668    : 片选
 *  choose  ：选择需要关闭的通道
**/
void Ecr866xSetChClose(TRX8668_FPGA_t * h8668, uint32_t operate)
{
    trx8668_port_t spi_perph = h8668->config.spi_port;

    switch(operate)
    {
        case OPEN_RX_CLOSE_TX_FLAG:    //打开RX关闭TX
            ECR8660_write(spi_perph, SPI_RW_EXTERNAL, ECR_CH_STATE_ADDR, (ECR_OPEN_RX_CLOSE_TX));
            rtt_printf(RTT_8668, "8668 %d OPEN_RX_CLOSE_TX\r\n", h8668->config.dev_number);
            break;

        case OPEN_TX_CLOSE_RX_FLAG:    //打开TX关闭RX
            ECR8660_write(spi_perph, SPI_RW_EXTERNAL, ECR_CH_STATE_ADDR, (ECR_OPEN_TX_CLOSE_RX));
            rtt_printf(RTT_8668, "8668 %d OPEN_TX_CLOSE_RX\r\n", h8668->config.dev_number);
            break;

        case CLOSE_TXRX_FLAG:          //tx rx全部关闭
            ECR8660_write(spi_perph, SPI_RW_EXTERNAL, ECR_CH_STATE_ADDR, (ECR_CLOSE_TXRX));
            rtt_printf(RTT_8668, "8668 %d CLOSE_TXRX\r\n", h8668->config.dev_number);
            break;

        case OPEN_TXRX_FLAG:            //tx rx全部打开
            ECR8660_write(spi_perph, SPI_RW_EXTERNAL, ECR_CH_STATE_ADDR, (ECR_OPEN_TXRX));
            rtt_printf(RTT_8668, "8668 %d OPEN_TXRX\r\n", h8668->config.dev_number);
            break;

        default:
            rtt_printf(RTT_8668, RTT_TEXT_RED"error : 8668 %d Ecr866xSetChClose operate = %d!\r\n", h8668->config.dev_number, operate);
            return;
    }
}

/**
 *  8668硬复位
*  rst8668pin：复位引脚
**/
void HardReset_8668(PORT_T reset_port, GPIO_PIN_T reset_pin)
{
    //8668 0和1 硬复位
    hi_gpio_write(reset_port, reset_pin, LEVEL_L);
    vTaskDelay(10);
    hi_gpio_write(reset_port, reset_pin, LEVEL_H);
    vTaskDelay(100);
}

/**
*  函数名：ecr8668_init
 * 参数：spi_periph:SPI选择   rst8668pin：复位引脚
 * 功能：实现8668 bin文件下载与配置
**/
Status_t ecr8668_init(TRX8668_FPGA_t * h8668)
{
    int state;

    HardReset_8668(h8668->config.rst_port, h8668->config.rst_pin);
    //8668  初始化，bin文件下载
    state = ECR8660_INIT(h8668, h8668->bin_addr, h8668->bin_siz);
    if(state != STAT_OK)
    {
        return STAT_ERROR;
    }
    
    return STAT_OK;
}

/**
 * @brief 设置8668 ATT衰减值
 * @param upOrdn 上、下行
 * @param value 衰减值
 * @return int
 */
Status_t Ecr8668SetAtt(TRX8668_FPGA_t * h8668, int8_t value)
{
    int8_t  gain_tab_index = 0;
    uint32_t RxSzGainAddr;
    uint32_t RxMnGainAddr;
    uint32_t TxSzGainAddr;
    uint32_t TxMnGainAddr;
    
    UNUSED_PARA(RxSzGainAddr);
    UNUSED_PARA(TxMnGainAddr);

    if(h8668->gain_tab == NULL)
    {
        rtt_printf(RTT_8668, RTT_TEXT_RED"error : 8668 %d Ecr866xSetatt gain_tab is NULL!\r\n", h8668->config.dev_number);
        return STAT_ERROR;
    }

    trx8668_port_t spi_perph = h8668->config.spi_port;

    gain_tab_index = value + h8668->gain_tab->rated_gain_index;

    if(gain_tab_index >= h8668->gain_tab->gain_tab_number || gain_tab_index < 0)
    {
        rtt_printf(RTT_8668, RTT_TEXT_RED"error : 8668 %d Ecr866xSetatt value = %d, rated_gain_index = %d, gain_tab_number = %d!\r\n", 
                h8668->config.dev_number, value,h8668->gain_tab->rated_gain_index,h8668->gain_tab->gain_tab_number);
        return STAT_ERROR;
    }

    if(h8668->config.ch == 0)
    {
        RxSzGainAddr = ECR_CH0_RX_SZ_GAIN_ADDR;
        RxMnGainAddr = ECR_CH0_RX_MN_GAIN_ADDR;
        TxSzGainAddr = ECR_CH0_TX_SZ_GAIN_ADDR;
        TxMnGainAddr = ECR_CH0_TX_MN_GAIN_ADDR;
    }
    else if(h8668->config.ch == 1)
    {
        RxSzGainAddr = ECR_CH1_RX_SZ_GAIN_ADDR;
        RxMnGainAddr = ECR_CH1_RX_MN_GAIN_ADDR;
        TxSzGainAddr = ECR_CH1_TX_SZ_GAIN_ADDR;
        TxMnGainAddr = ECR_CH1_TX_MN_GAIN_ADDR;
    }
    else
    {
        rtt_printf(RTT_8668, RTT_TEXT_RED"error : 8668 %d Ecr866xSetGain ch = %d!\r\n", h8668->config.dev_number, h8668->config.ch);
        return STAT_ERROR;
    }

    ECR8660_write(spi_perph, SPI_RW_EXTERNAL, RxMnGainAddr, (h8668->gain_tab->gain_tab[gain_tab_index][1])); //设置RX模拟增益
    ECR8660_write(spi_perph, SPI_RW_EXTERNAL, TxSzGainAddr, (h8668->gain_tab->gain_tab[gain_tab_index][2])); //设置TX数字增益

    //ECR8660_write(spi_perph, SPI_RW_EXTERNAL, RxSzGainAddr, (h8668->gain_tab->gain_tab[gain_tab_index][0])); //设置RX数字增益
    //ECR8660_write(spi_perph, SPI_RW_EXTERNAL, TxMnGainAddr, (h8668->gain_tab->gain_tab[gain_tab_index][3])); //设置TX模拟增益

    h8668->ch_status.gain_tab_index = gain_tab_index; //更新当前所用的增益码表索引值

    rtt_printf(RTT_8668, RTT_TEXT_CYAN"8668 %d Ecr866xSet att = %d, index = %d, rated index = %d!\r\n", h8668->config.dev_number, value, gain_tab_index, h8668->gain_tab->rated_gain_index);
    return STAT_OK;
}




