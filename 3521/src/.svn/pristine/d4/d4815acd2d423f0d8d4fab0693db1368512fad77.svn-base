#include "MCU_config.h"
#include "ecr8668_driver.h"
#include "main.h"
#include "bsp_config.h"
#include "uart_at_cmd.h"
#include "rtt.h"


/**********************************************************
 *函数名：   BSP_866x_uart_cmd_handle
 *函数功能： 3521调试临时用, 8668寄存器读写
             格式: AT+TRXWI=<uint8 cs>,<uint32 reg_addr>,<uint32 reg_value> //页内寄存器写
             格式: AT+TRXWE=<uint8 cs>,<uint32 reg_addr>,<uint32 reg_value> //页外寄存器写
             格式: AT+TRXRI=<uint8 cs>,<uint32 reg_addr> //页内寄存器读
             格式: AT+TRXRE=<uint8 cs>,<uint32 reg_addr> //页外寄存器读
             cs: 片选, 0 -lte; 1 -NR; 2 -NR MIMO
             
             格式: AT+WRIO=<IOGROUP>,<IOBUM>,<Lvl>      //GPIO写
             格式: AT+RDIO=<IOGROUP>,<IOBUM>            //GPIO读
             eg:AT+WRIO=A,12,1
 *返回值：  STAT_OK 处理了一条串口指令
            STAT_ERROR 非串口指令,或非法参数
 */

Status_t BSP_uart_AT_cmd_process(uint8_t *cmd_data, uint16_t len)
{
    
    uint8_t cs = 0xff;
    uint8_t rx_msg[50] = {0};
    uint32_t reg_addr = 0, reg_value = 0;
    TRX8668_FPGA_t *h8668;

    if(cmd_data == NULL || len == 0) 
        return STAT_ERROR;
    
    if(strstr((char*)cmd_data, "AT+") == NULL)
        return STAT_ERROR;

    if(strstr((char*)cmd_data, "AT+TRXWI") != NULL)
    {
        sscanf((char *)cmd_data, "AT+TRXWI=%hhu,%ul,%ul", &cs, &reg_addr, &reg_value);
        if(cs == 0)
            h8668 = &h8668_LTE;
        else if(cs == 1)
            h8668 = &h8668_NR;
        else if(cs == 2)
            h8668 = &h8668_NR_MIMO;
        else
        {
            hi_usb_send((uint8_t *)"AT cmd Param 'cs' error\r\n", strlen("AT cmd Param 'cs' error\r\n"));
            rtt_printf(RTT_PC, RTT_CTRL_TEXT_BRIGHT_YELLOW"AT cmd Param 'cs' error\r\n");
            return STAT_ERROR;
        }
        BSP_866x_In_Write(h8668, reg_addr, reg_value);
        hi_usb_send((uint8_t *)"OK\r\n", strlen("OK\r\n"));

    }else if(strstr((char*)cmd_data, "AT+TRXWE") != NULL)
    {
        sscanf((char *)cmd_data, "AT+TRXWE=%hhu,%lu,%lu", &cs, &reg_addr, &reg_value);
        if(cs == 0)
            h8668 = &h8668_LTE;
        else if(cs == 1)
            h8668 = &h8668_NR;
        else if(cs == 2)
            h8668 = &h8668_NR_MIMO;
        else
        {
            hi_usb_send((uint8_t *)"AT cmd Param 'cs' error\r\n", strlen("AT cmd Param 'cs' error\r\n"));
            return STAT_ERROR;
        }
        BSP_866x_Ext_Write(h8668, reg_addr, reg_value);
        hi_usb_send((uint8_t *)"OK\r\n", strlen("OK\r\n"));

    }else if(strstr((char *)cmd_data, "AT+TRXRI") != NULL)
    {
        sscanf((char *)cmd_data, "AT+TRXRI=%hhu,%ul", &cs, &reg_addr);
        if(cs == 0)
            h8668 = &h8668_LTE;
        else if(cs == 1)
            h8668 = &h8668_NR;
        else if(cs == 2)
            h8668 = &h8668_NR_MIMO;
        else
        {
            hi_usb_send((uint8_t *)"AT cmd Param 'cs' error\r\n", strlen("AT cmd Param 'cs' error\r\n"));
            return STAT_ERROR;
        }
        reg_value = BSP_866x_In_Read(h8668, reg_addr);
        sprintf((char *)rx_msg, "+TRXRI=%d", reg_value);

        hi_usb_send(rx_msg, strlen((char *)rx_msg));
        hi_usb_send((uint8_t *)"OK\r\n", strlen("OK\r\n"));

    }else if(strstr((char *)cmd_data, "AT+TRXRE") != NULL)
    {
        sscanf((char *)cmd_data, "AT+TRXRE=%hhu,%ul", &cs, &reg_addr);
        if(cs == 0)
            h8668 = &h8668_LTE;
        else if(cs == 1)
            h8668 = &h8668_NR;
        else if(cs == 2)
            h8668 = &h8668_NR_MIMO;
        else
        {
            hi_usb_send((uint8_t *)"AT cmd Param 'cs' error\r\n", strlen("AT cmd Param 'cs' error\r\n"));
            return STAT_ERROR;
        }
        reg_value = BSP_866x_Ex_Read(h8668, reg_addr);
        sprintf((char *)rx_msg, "+TRXRE=%d", reg_value);
        
        hi_usb_send((uint8_t *)rx_msg, strlen((char *)rx_msg));
        hi_usb_send((uint8_t *)"OK\r\n", strlen("OK\r\n"));

    }else if(strstr((char*)cmd_data, "AT+WRIO") != NULL)
    {
        char group;
        uint32_t number = 0, stat = 0;
        uint32_t io_group, io_num;
        
        sscanf((char *)cmd_data, "AT+WRIO=%c,%d,%d", &group, &number, &stat);

        switch (group)
        {
        case 'A':
        case 'a':
            io_group = (uint32_t)GPIOA;
            break;
        case 'B':
        case 'b':
            io_group = (uint32_t)GPIOB;
            break;
        case 'C':
        case 'c':
            io_group = (uint32_t)GPIOC;
            break;
        case 'D':
        case 'd':
            io_group = (uint32_t)GPIOD;
            break;
        case 'E':
        case 'e':
            io_group = (uint32_t)GPIOE;
            break;
        case 'F':
        case 'f':
            io_group = (uint32_t)GPIOF;
            break;
        case 'G':
        case 'g':
            io_group = (uint32_t)GPIOG;
            break;
        case 'H':
        case 'h':
            io_group = (uint32_t)GPIOH;
            break;
        case 'I':
        case 'i':
            io_group = (uint32_t)GPIOI;
            break;
        
        default:
            hi_usb_send((uint8_t *)"AT cmd Param 'gourp' error\r\n", strlen("AT cmd Param 'gourp' error\r\n"));
            break;
        }

        switch (number)
        {
        case 0:
            io_num = GPIO_PIN_0;
            break;

        case 1:
            io_num = GPIO_PIN_1;
            break;

        case 2:
            io_num = GPIO_PIN_2;
            break;

        case 3:
            io_num = GPIO_PIN_3;
            break;

        case 4:
            io_num = GPIO_PIN_4;
            break;

        case 5:
            io_num = GPIO_PIN_5;
            break;

        case 6:
            io_num = GPIO_PIN_6;
            break;

        case 7:
            io_num = GPIO_PIN_7;
            break;

        case 8:
            io_num = GPIO_PIN_8;
            break;

        case 9:
            io_num = GPIO_PIN_9;
            break;

        case 10:
            io_num = GPIO_PIN_10;
            break;

        case 11:
            io_num = GPIO_PIN_11;
            break;

        case 12:
            io_num = GPIO_PIN_12;
            break;

        case 13:
            io_num = GPIO_PIN_13;
            break;

        case 14:
            io_num = GPIO_PIN_14;
            break;

        case 15:
            io_num = GPIO_PIN_15;
            break;
        
        default:
            hi_usb_send((uint8_t *)"AT cmd Param 'num' error\r\n", strlen("AT cmd Param 'num' error\r\n"));
            break;
        }

        if(stat > 1)
        {
            hi_usb_send((uint8_t *)"AT cmd Param 'stat' error\r\n", strlen("AT cmd Param 'stat' error\r\n"));
            return STAT_ERROR;
        }

        hi_gpio_write((PORT_T)io_group, io_num, (ELEC_LEVEL_E)stat);

        hi_usb_send((uint8_t *)"OK\r\n", strlen("OK\r\n"));

    }else if(strstr((char *)cmd_data, "AT+RDIO") != NULL)
    {
        char group;
        uint32_t number = 0, stat = 0;
        uint32_t io_group, io_num;
        
        sscanf((char *)cmd_data, "AT+RDIO=%c,%d", &group, &number);

        switch (group)
        {
        case 'A':
        case 'a':
            io_group = (uint32_t)GPIOA;
            break;
        case 'B':
        case 'b':
            io_group = (uint32_t)GPIOB;
            break;
        case 'C':
        case 'c':
            io_group = (uint32_t)GPIOC;
            break;
        case 'D':
        case 'd':
            io_group = (uint32_t)GPIOD;
            break;
        case 'E':
        case 'e':
            io_group = (uint32_t)GPIOE;
            break;
        case 'F':
        case 'f':
            io_group = (uint32_t)GPIOF;
            break;
        case 'G':
        case 'g':
            io_group = (uint32_t)GPIOG;
            break;
        case 'H':
        case 'h':
            io_group = (uint32_t)GPIOH;
            break;
        case 'I':
        case 'i':
            io_group = (uint32_t)GPIOI;
            break;
        
        default:
            hi_usb_send((uint8_t *)"AT cmd Param 'gourp' error\r\n", strlen("AT cmd Param 'gourp' error\r\n"));
            break;
        }

        switch (number)
        {
        case 0:
            io_num = GPIO_PIN_0;
            break;

        case 1:
            io_num = GPIO_PIN_1;
            break;

        case 2:
            io_num = GPIO_PIN_2;
            break;

        case 3:
            io_num = GPIO_PIN_3;
            break;

        case 4:
            io_num = GPIO_PIN_4;
            break;

        case 5:
            io_num = GPIO_PIN_5;
            break;

        case 6:
            io_num = GPIO_PIN_6;
            break;

        case 7:
            io_num = GPIO_PIN_7;
            break;

        case 8:
            io_num = GPIO_PIN_8;
            break;

        case 9:
            io_num = GPIO_PIN_9;
            break;

        case 10:
            io_num = GPIO_PIN_10;
            break;

        case 11:
            io_num = GPIO_PIN_11;
            break;

        case 12:
            io_num = GPIO_PIN_12;
            break;

        case 13:
            io_num = GPIO_PIN_13;
            break;

        case 14:
            io_num = GPIO_PIN_14;
            break;

        case 15:
            io_num = GPIO_PIN_15;
            break;
        
        default:
            hi_usb_send((uint8_t *)"AT cmd Param 'num' error\r\n", strlen("AT cmd Param 'num' error\r\n"));
            break;
        }

        stat = hi_gpio_read((PORT_T)io_group, io_num);
        sprintf((char *)rx_msg, "+IORD=%d", stat);

        hi_usb_send(rx_msg, strlen((char *)rx_msg));
        hi_usb_send((uint8_t *)"OK\r\n", strlen("OK\r\n"));

    }else
        return STAT_ERROR;

    return STAT_OK;
}
