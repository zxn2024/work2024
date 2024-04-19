/**
  ******************************************************************************
  * @file    fpga_spi.c
  * @author  
  * @version V1.0.0
  * @date    2023-10-14
  * @brief   This file provides code for the fpga spi.
  ******************************************************************************
*/
#include <string.h>
//#include "gd32f4xx.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "hi_spi.h"
#include "hi_gpio.h"
#include "fpga.h"
#include "fpga_spi.h"

extern FPGA_config_t g_fpga_config;
//static SemaphoreHandle_t g_fpga_xSemaphore = NULL;

#define SPI_CS_L		hi_gpio_write(g_fpga_config.spi_cs_port, g_fpga_config.spi_cs_pin, LEVEL_L)
#define SPI_CS_H		hi_gpio_write(g_fpga_config.spi_cs_port, g_fpga_config.spi_cs_pin, LEVEL_H)

#if 0		
/*
**********************************************************
*函数功能: fpga spi初始化
*参数说明: 无
*返 回 值: 无
**********************************************************
*/
void fpga_spi_init(void)
{
    if (g_fpga_xSemaphore == NULL)
    {
         vSemaphoreCreateBinary(g_fpga_xSemaphore);
    }		
		
		if (g_fpga_xSemaphore == NULL)
		{
        /*信号量创建失败*/
		}
}
#endif

#if 0
/*
**********************************************************
*函数功能: fpga_spi_send_byte
*参数说明: byte
*返 回 值: 读出字节
**********************************************************
*/

static uint8_t fpga_spi_send_byte(uint8_t byte)
{
    while(RESET == spi_i2s_flag_get(SPI3, SPI_FLAG_TBE));/* loop while data register in not empty */

    spi_i2s_data_transmit(SPI3, byte);/* send byte through the SPI1 peripheral */

    while(RESET == spi_i2s_flag_get(SPI3, SPI_FLAG_RBNE));/* wait to receive a byte */

    return(spi_i2s_data_receive(SPI3)); /* return the byte read from the SPI bus */
}
#endif

#if 0
/*
**********************************************************
*函数功能: fpga_spi_write FPGA spi写入  在任务中调用
*参数说明: op_type 操作类型 0寄存器 1 ook 2~4 ECR8668
           pbuffer 发送缓冲区 当type=0时 6个字节 前两个字节是地址，后四个字节是数据
                              当type=1时 ook地址+数据
                              当type=2~4时 8668地址+数据
					 buffer_size 发送缓冲区长度 				 
*返 回 值: 1成功
**********************************************************
*/
uint32_t fpga_spi_write(uint8_t op_type, uint8_t *pbuffer, uint8_t buffer_size)
{
    uint8_t send_buffer[7];
    uint8_t send_num, index;	
	
    if (op_type > FPGA_OP_DEVICE_TRX_5G2)	
        return FPGA_OP_FAIL;/*类型错误*/

    if (pbuffer == NULL)
        return FPGA_OP_FAIL;/*指针空*/
		
    if (buffer_size > 6)
        return FPGA_OP_FAIL;/*长度错误*/
	
		send_buffer[0] = ((FPGA_OP_MODE_WRITE<<7)&0x80) | ((op_type<<4)&0x70) | (buffer_size&0x0F);
		memcpy(send_buffer+1, pbuffer, buffer_size);
		send_num = 1 + buffer_size;
		
		if (g_fpga_xSemaphore == NULL)	
		{
				return FPGA_OP_FAIL;/*信号量创建失败*/
		}	
		else
		{	
        /*获取信号量*/
        if(pdFALSE == xSemaphoreTake(g_fpga_xSemaphore, (TickType_t)200000000))/*等待1s*/
        {
						return FPGA_OP_FAIL;/*获取信号量失败*/
				}
				else
				{
        		SPI_CS_L;/* chip select valid */	
        		for (index=0; index<send_num; index++)
        		{
        				SPI_ReadWriteByte(g_fpga_config.spi, send_buffer[index]);/*fpga_spi_send_byte(send_buffer[index]);*/	
        		}		
        		SPI_CS_H;/* chip select invalid */		

            /*释放信号量*/
            if( xSemaphoreGive( g_fpga_xSemaphore ) != pdTRUE )
            {
                /**/
            }
        }	
		}	

		return FPGA_OP_SUCCESS;
}

/*
**********************************************************
*函数功能: fpga_spi_read  FPGA spi读  在任务中调用
*参数说明: op_type 操作类型 0寄存器 1 ook 2~4 ECR8668
           pbuffer 发送缓冲区 当type=0时 2个字节 两个字节的地址
                              当type=1时 ook地址
                              当type=2~4时 8668地址
					 buffer_size 发送缓冲区长度 															
*返 回 值: 读到的结果
**********************************************************
*/
uint32_t fpga_spi_read(uint8_t op_type, uint8_t *pbuffer, uint8_t buffer_size)
{
    uint8_t send_buffer[7];
    uint8_t recv_buffer[7];
    uint8_t send_num, index;	
    uint32_t ret;
	
    if (op_type > FPGA_OP_DEVICE_TRX_5G2)	
        return FPGA_OP_FAIL;/*类型错误*/

    if (pbuffer == NULL)
        return FPGA_OP_FAIL;/*指针空*/
		
    if (buffer_size > 6)
        return FPGA_OP_FAIL;/*长度错误*/
	
		if (op_type != FPGA_OP_DEVICE_REG)
		{
				send_buffer[0] = ((FPGA_OP_MODE_WRITE<<7)&0x80) | ((op_type<<4)&0x70) | (buffer_size&0x0F);
				memcpy(send_buffer+1, pbuffer, buffer_size);
				send_num = 1 + buffer_size;			
			
				if (g_fpga_xSemaphore == NULL)	
				{
						return FPGA_OP_FAIL;/*信号量创建失败*/
				}	
				else
				{	
        		/*获取信号量*/
        		if(pdFALSE == xSemaphoreTake(g_fpga_xSemaphore, (TickType_t)200000000))/*等待1s*/
        		{
								return FPGA_OP_FAIL;/*获取信号量失败*/
						}
						else
						{
    						SPI_CS_L;/* chip select valid */		
    						for (index=0; index<send_num; index++)
    						{
        						SPI_ReadWriteByte(g_fpga_config.spi, send_buffer[index]);/*fpga_spi_send_byte(send_buffer[index]);*/
    						}						
    						SPI_CS_H;/* chip select invalid */		

            		/*释放信号量*/
            		if( xSemaphoreGive( g_fpga_xSemaphore ) != pdTRUE )
            		{
                		/**/
            		}
        		}	
				}			
				
				/*任务延时*/
				//if ((op_type==FPGA_OP_DEVICE_TRX_4G)||(op_type==FPGA_OP_DEVICE_TRX_5G1)||(op_type==FPGA_OP_DEVICE_TRX_5G2))
				//{
        //		vTaskDelay(500);
				//}
		}
			
		memset(send_buffer, 0, 7);
		send_buffer[0] = ((FPGA_OP_MODE_READ<<7)&0x80) | ((FPGA_OP_DEVICE_REG<<4)&0x70) | (6&0x0F);
		send_buffer[1] = 0x20;
		if (op_type == FPGA_OP_DEVICE_OOK)
				send_buffer[2] = 0x00;
		else if (op_type == FPGA_OP_DEVICE_TRX_4G)
				send_buffer[2] = 0x01;
		else if (op_type == FPGA_OP_DEVICE_TRX_5G1)
				send_buffer[2] = 0x02;
		else if (op_type == FPGA_OP_DEVICE_TRX_5G2)
				send_buffer[2] = 0x03;
		else
				memcpy(send_buffer+1, pbuffer, buffer_size);
		send_num = 1 + 6;		

		/*获取信号量*/
		if (g_fpga_xSemaphore == NULL)	
		{
				return FPGA_OP_FAIL;/*信号量创建失败*/
		}	
		else
		{	
        /*获取信号量*/
        if(pdFALSE == xSemaphoreTake(g_fpga_xSemaphore, (TickType_t)200000000))/*等待1s*/
        {
						return FPGA_OP_FAIL;/*获取信号量失败*/
				}
				else
				{
						SPI_CS_L;/* chip select valid */		
    				for (index=0; index<send_num; index++)
    				{
        				recv_buffer[index] = SPI_ReadWriteByte(g_fpga_config.spi, send_buffer[index]);/*fpga_spi_send_byte(send_buffer[index]);*/		
    				}				
    				SPI_CS_H;/* chip select invalid */		

            /*释放信号量*/
            if( xSemaphoreGive( g_fpga_xSemaphore ) != pdTRUE )
            {
                /**/
            }
        }	
		}	
		
		ret = recv_buffer[3];
		ret = (ret<<8) + recv_buffer[4];
		ret = (ret<<8) + recv_buffer[5];
		ret = (ret<<8) + recv_buffer[6];
		
		if ((ret == FPGA_OP_FAIL) && ((op_type==FPGA_OP_DEVICE_TRX_4G)||(op_type==FPGA_OP_DEVICE_TRX_5G1)||(op_type==FPGA_OP_DEVICE_TRX_5G2)))
		{
				/*8668读失败，重新读一次*/
				vTaskDelay(500);
				
				memset(send_buffer, 0, 7);
				send_buffer[0] = ((FPGA_OP_MODE_READ<<7)&0x80) | ((FPGA_OP_DEVICE_REG<<4)&0x70) | (6&0x0F);
				send_buffer[1] = 0x20;
				if (op_type == FPGA_OP_DEVICE_OOK)
						send_buffer[2] = 0x00;
				else if (op_type == FPGA_OP_DEVICE_TRX_4G)
						send_buffer[2] = 0x01;
				else if (op_type == FPGA_OP_DEVICE_TRX_5G1)
						send_buffer[2] = 0x02;
				else if (op_type == FPGA_OP_DEVICE_TRX_5G2)
						send_buffer[2] = 0x03;
				else
						memcpy(send_buffer+1, pbuffer, buffer_size);
				send_num = 1 + 6;		
				
				/*获取信号量*/
				if (g_fpga_xSemaphore == NULL)	
				{
						return FPGA_OP_FAIL;/*信号量创建失败*/
				}	
				else
				{	
    		    /*获取信号量*/
    		    if(pdFALSE == xSemaphoreTake(g_fpga_xSemaphore, (TickType_t)200000000))/*等待1s*/
    		    {
								return FPGA_OP_FAIL;/*获取信号量失败*/
						}
						else
						{
								SPI_CS_L;/* chip select valid */		
    						for (index=0; index<send_num; index++)
    						{
    		    				recv_buffer[index] = SPI_ReadWriteByte(g_fpga_config.spi, send_buffer[index]);/*fpga_spi_send_byte(send_buffer[index]);*/		
    						}				
    						SPI_CS_H;/* chip select invalid */		
				
    		        /*释放信号量*/
    		        if( xSemaphoreGive( g_fpga_xSemaphore ) != pdTRUE )
    		        {
    		            /**/
    		        }
    		    }	
				}	
				
				ret = recv_buffer[3];
				ret = (ret<<8) + recv_buffer[4];
				ret = (ret<<8) + recv_buffer[5];
				ret = (ret<<8) + recv_buffer[6];				
		}			
		
		return ret;
}
#endif

/*
**********************************************************
*函数功能: fpga_spi_write FPGA spi写入  在任务中调用
*参数说明: op_type 操作类型 0寄存器 1 ook 2~4 ECR8668
           pbuffer 发送缓冲区 当type=0时 6个字节 前两个字节是地址，后四个字节是数据
                              当type=1时 ook地址+数据
                              当type=2~4时 8668地址+数据
					 buffer_size 发送缓冲区长度 				 
*返 回 值: 1成功
**********************************************************
*/
static uint8_t g_fpga_Semaphore = 0;
uint8_t fpga_xSemaphoreTake(void)
{
		uint8_t index = 0;
		
		for (index=0; index<20; index++)
		{	
				if (g_fpga_Semaphore == 0)
				{
						g_fpga_Semaphore = 1;
						return 1;
				}
				
				vTaskDelay(50);
		}

		return 0;
}


void fpga_xSemaphoreGive(void)
{
		g_fpga_Semaphore=0;
}

uint32_t fpga_spi_write(uint8_t op_type, uint8_t *pbuffer, uint8_t buffer_size)
{
    uint8_t send_buffer[7];
    uint8_t send_num, index;	
	
    if (op_type > FPGA_OP_DEVICE_TRX_5G2)	
        return FPGA_OP_FAIL;/*类型错误*/

    if (pbuffer == NULL)
        return FPGA_OP_FAIL;/*指针空*/
		
    if (buffer_size > 6)
        return FPGA_OP_FAIL;/*长度错误*/
	
		send_buffer[0] = ((FPGA_OP_MODE_WRITE<<7)&0x80) | ((op_type<<4)&0x70) | (buffer_size&0x0F);
		memcpy(send_buffer+1, pbuffer, buffer_size);
		send_num = 1 + buffer_size;
		

		/*获取信号量*/
		if (0==fpga_xSemaphoreTake())
		{
				return FPGA_OP_FAIL;/*获取信号量失败*/
		}
		else
		{
				SPI_CS_L;/* chip select valid */	
				for (index=0; index<send_num; index++)
				{
						SPI_ReadWriteByte(g_fpga_config.spi, send_buffer[index]);
				}		
				SPI_CS_H;/* chip select invalid */		
		
		    /*释放信号量*/
		    fpga_xSemaphoreGive();
		}	

		return FPGA_OP_SUCCESS;
}

/*
**********************************************************
*函数功能: fpga_spi_read  FPGA spi读  在任务中调用
*参数说明: op_type 操作类型 0寄存器 1 ook 2~4 ECR8668
           pbuffer 发送缓冲区 当type=0时 2个字节 两个字节的地址
                              当type=1时 ook地址
                              当type=2~4时 8668地址
					 buffer_size 发送缓冲区长度 															
*返 回 值: 读到的结果
**********************************************************
*/
uint32_t fpga_spi_read(uint8_t op_type, uint8_t *pbuffer, uint8_t buffer_size)
{
    uint8_t send_buffer[7];
    uint8_t recv_buffer[7];
    uint8_t send_num, index;	
    uint32_t ret;
	
    if (op_type > FPGA_OP_DEVICE_TRX_5G2)	
        return FPGA_OP_FAIL;/*类型错误*/

    if (pbuffer == NULL)
        return FPGA_OP_FAIL;/*指针空*/
		
    if (buffer_size > 6)
        return FPGA_OP_FAIL;/*长度错误*/
	
		if (op_type != FPGA_OP_DEVICE_REG)
		{
				send_buffer[0] = ((FPGA_OP_MODE_WRITE<<7)&0x80) | ((op_type<<4)&0x70) | (buffer_size&0x0F);
				memcpy(send_buffer+1, pbuffer, buffer_size);
				send_num = 1 + buffer_size;			
			
        /*获取信号量*/
        if (0==fpga_xSemaphoreTake())
        {
						return FPGA_OP_FAIL;/*获取信号量失败*/
				}
				else
				{
    				SPI_CS_L;/* chip select valid */		
    				for (index=0; index<send_num; index++)
    				{
        				SPI_ReadWriteByte(g_fpga_config.spi, send_buffer[index]);
    				}						
    				SPI_CS_H;/* chip select invalid */		

        		/*释放信号量*/
        		fpga_xSemaphoreGive();//if( xSemaphoreGive( g_fpga_xSemaphore ) != pdTRUE )
        }	
		}
			
		memset(send_buffer, 0, 7);
		send_buffer[0] = ((FPGA_OP_MODE_READ<<7)&0x80) | ((FPGA_OP_DEVICE_REG<<4)&0x70) | (6&0x0F);
		send_buffer[1] = 0x20;
		if (op_type == FPGA_OP_DEVICE_OOK)
				send_buffer[2] = 0x00;
		else if (op_type == FPGA_OP_DEVICE_TRX_4G)
				send_buffer[2] = 0x01;
		else if (op_type == FPGA_OP_DEVICE_TRX_5G1)
				send_buffer[2] = 0x02;
		else if (op_type == FPGA_OP_DEVICE_TRX_5G2)
				send_buffer[2] = 0x03;
		else
				memcpy(send_buffer+1, pbuffer, buffer_size);
		send_num = 1 + 6;		


    /*获取信号量*/
    if (0==fpga_xSemaphoreTake())
    {
				return FPGA_OP_FAIL;/*获取信号量失败*/
		}
		else
		{
				SPI_CS_L;/* chip select valid */		
    		for (index=0; index<send_num; index++)
    		{
    				recv_buffer[index] = SPI_ReadWriteByte(g_fpga_config.spi, send_buffer[index]);
    		}				
    		SPI_CS_H;/* chip select invalid */		

        /*释放信号量*/
        fpga_xSemaphoreGive();
    }	
		
		ret = recv_buffer[3];
		ret = (ret<<8) + recv_buffer[4];
		ret = (ret<<8) + recv_buffer[5];
		ret = (ret<<8) + recv_buffer[6];
		
		if ((ret == FPGA_OP_FAIL) && ((op_type==FPGA_OP_DEVICE_TRX_4G)||(op_type==FPGA_OP_DEVICE_TRX_5G1)||(op_type==FPGA_OP_DEVICE_TRX_5G2)))
		{
				/*8668读失败，重新读一次*/
				vTaskDelay(500);
				
				memset(send_buffer, 0, 7);
				send_buffer[0] = ((FPGA_OP_MODE_READ<<7)&0x80) | ((FPGA_OP_DEVICE_REG<<4)&0x70) | (6&0x0F);
				send_buffer[1] = 0x20;
				if (op_type == FPGA_OP_DEVICE_OOK)
						send_buffer[2] = 0x00;
				else if (op_type == FPGA_OP_DEVICE_TRX_4G)
						send_buffer[2] = 0x01;
				else if (op_type == FPGA_OP_DEVICE_TRX_5G1)
						send_buffer[2] = 0x02;
				else if (op_type == FPGA_OP_DEVICE_TRX_5G2)
						send_buffer[2] = 0x03;
				else
						memcpy(send_buffer+1, pbuffer, buffer_size);
				send_num = 1 + 6;		
				
    		/*获取信号量*/
    		if (0==fpga_xSemaphoreTake())
    		{
						return FPGA_OP_FAIL;/*获取信号量失败*/
				}
				else
				{
						SPI_CS_L;/* chip select valid */		
    				for (index=0; index<send_num; index++)
    				{
    						recv_buffer[index] = SPI_ReadWriteByte(g_fpga_config.spi, send_buffer[index]);	
    				}				
    				SPI_CS_H;/* chip select invalid */		
				
    		    /*释放信号量*/
    		    fpga_xSemaphoreGive();
    		}	
				
				ret = recv_buffer[3];
				ret = (ret<<8) + recv_buffer[4];
				ret = (ret<<8) + recv_buffer[5];
				ret = (ret<<8) + recv_buffer[6];				
		}			
		
		return ret;
}

/*
**********************************************************
*函数功能: fpga_reg_write FPGA 寄存器写入  在任务中调用
*参数说明: reg_addr 寄存器地址
           data 写入的数据
*返 回 值: 1成功
**********************************************************
*/
uint32_t fpga_reg_write(uint16_t reg_addr, uint32_t data)
{
    uint8_t buffer[6];

    buffer[0] = (reg_addr>>8) & 0xFF;
    buffer[1] = (reg_addr   ) & 0xFF;
    buffer[2] = (data>>24) & 0xFF;
    buffer[3] = (data>>16) & 0xFF;
    buffer[4] = (data>> 8) & 0xFF;
    buffer[5] = data       & 0xFF;
			
    return fpga_spi_write(FPGA_OP_DEVICE_REG, buffer, 6);	
}
/*
**********************************************************
*函数功能: fpga_reg_read  FPGA 寄存器读  在任务中调用
*参数说明: reg_addr 寄存器地址
*返 回 值: 读到的结果
**********************************************************
*/
uint32_t fpga_reg_read(uint16_t reg_addr)
{
    uint8_t buffer[2];

    buffer[0] = (reg_addr>>8) & 0xFF;
    buffer[1] = (reg_addr   ) & 0xFF;

    return fpga_spi_read(FPGA_OP_DEVICE_REG, buffer, 2);
}
