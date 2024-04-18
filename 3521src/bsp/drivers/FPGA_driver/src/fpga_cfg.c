/**
  ******************************************************************************
  * @file    fpga_cfg.c
  * @author  
  * @version V1.0.0
  * @date    2023-10-14
  * @brief   This file provides code for the fpga cfg.
  ******************************************************************************
*/
#include <stdio.h>
#include <string.h>
#include "hi_spi.h"
#include "hi_gpio.h"
#include "fpga.h"
#include "Bsp_config.h"
#include "Ecr8668_driver.h"
#include "fpga_cfg.h"

FPGA_config_t g_fpga_config;
static uint8_t g_4g_freqband = 0xFF;

#if(PRODUCT_MODEL == CMDT3521A) //A S55643-22
/*
**********************************************************
*函数功能: fpga_set_4g_freqband 设置4G频段 并打开pa 在任务中调用
*参数说明: fb：0 1800M    1 900M    2 F频段    3 E频段
*返 回 值: 1成功
**********************************************************
*/
/*
S55643-22
4G DL
1  900M   B8   FDD  LB5  初始化：Reg1C:0x38  Reg00:0x28  Reg01:0x04  Reg03:0x78  Reg04:0x00  Reg07:0x00   开：Reg00:0x2C   关：Reg00:0x28
0  1800M  B3   FDD  MB2  初始化：Reg1C:0x38  Reg00:0x3A  Reg01:0x37  Reg03:0x89  Reg04:0x00  Reg07:0x00   开：Reg00:0x3E   关：Reg00:0x3A
2  F频段  B39  TDD  MB5  初始化：Reg1C:0x38  Reg00:0x52  Reg01:0x37  Reg03:0x97  Reg04:0x00  Reg07:0x00   开：Reg00:0x56   关：Reg00:0x52
3  E频段  B40  TDD  HB4  初始化：Reg1C:0x38  Reg00:0x70  Reg01:0x8A  Reg03:0xA7  Reg04:0x00  Reg07:0x03   开：Reg00:0x74   关：Reg00:0x70

4G UL
1  900M   B8   FDD  LB5  初始化：Reg1C:0x38  Reg00:0x28  Reg01:0x04  Reg03:0x78  Reg04:0x00  Reg07:0x00   开：Reg00:0x2C   关：Reg00:0x28
0  1800M  B3   FDD  MB2  初始化：Reg1C:0x38  Reg00:0x3A  Reg01:0x37  Reg03:0x89  Reg04:0x00  Reg07:0x00   开：Reg00:0x3E   关：Reg00:0x3A
2  F频段  B39  TDD  MB5  初始化：Reg1C:0x38  Reg00:0x52  Reg01:0x37  Reg03:0x97  Reg04:0x00  Reg07:0x00   开：Reg00:0x56   关：Reg00:0x52
3  E频段  B40  TDD  HB4  初始化：Reg1C:0x38  Reg00:0x70  Reg01:0x8A  Reg03:0xA7  Reg04:0x00  Reg07:0x03   开：Reg00:0x74   关：Reg00:0x70
*/
static uint8_t g_4g_pa_on_reg[4]  = {0x3E, 0x2C, 0x56, 0x74};
static uint8_t g_4g_pa_off_reg[4] = {0x3A, 0x28, 0x52, 0x70};
uint8_t fpga_set_4g_freqband(uint8_t fb)
{
    if ((fb == FREQ_BAND_B8_900M) || (fb == FREQ_BAND_B3_1800M))
		{
        fpga_reg_write(FPGA_REG_4G_DL_OPEN,  (0x00<<8)|0x00);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_UL_OPEN,  (0x00<<8)|0x00);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/  			
		}
	
    if (fb == FREQ_BAND_B8_900M)    
		{
        /*4G频段设置*/
        fpga_reg_write(FPGA_REG_4G_BAND, FREQ_BAND_B8_900M);
        g_4g_freqband = FREQ_BAND_B8_900M;
			
        /*4G DL PA初始化寄存器*/
        fpga_reg_write(FPGA_REG_4G_DL_INI_1, (0x1C<<8)|0x38);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_DL_INI_2, (0x00<<8)|0x28);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_DL_INI_3, (0x01<<8)|0x04);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_DL_INI_4, (0x03<<8)|0x78);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_DL_INI_5, (0x04<<8)|0x00);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_DL_INI_6, (0x07<<8)|0x00);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
			
        /*4G DL PA控制 关闭/打开寄存器*/			
        fpga_reg_write(FPGA_REG_4G_DL_CLOSE, (0x00<<8)|0x28);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/	
        fpga_reg_write(FPGA_REG_4G_DL_OPEN,  (0x00<<8)|0x2C);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        	
        /*4G UL PA初始化寄存器*/
        fpga_reg_write(FPGA_REG_4G_UL_INI_1, (0x1C<<8)|0x38);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/	
        fpga_reg_write(FPGA_REG_4G_UL_INI_2, (0x00<<8)|0x28);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/			
        fpga_reg_write(FPGA_REG_4G_UL_INI_3, (0x01<<8)|0x04);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_UL_INI_4, (0x03<<8)|0x78);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_UL_INI_5, (0x04<<8)|0x00);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_UL_INI_6, (0x07<<8)|0x00);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/ 
				
        /*4G UL PA控制 关闭/打开寄存器*/	
				fpga_reg_write(FPGA_REG_4G_UL_CLOSE, (0x00<<8)|0x28);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/	
        fpga_reg_write(FPGA_REG_4G_UL_OPEN,  (0x00<<8)|0x2C);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/	
		}
		else if (fb == FREQ_BAND_B3_1800M)    
		{
        /*4G频段设置*/			
        fpga_reg_write(FPGA_REG_4G_BAND, FREQ_BAND_B3_1800M);
        g_4g_freqband = FREQ_BAND_B3_1800M;
			
        /*4G DL PA初始化寄存器*/			
        fpga_reg_write(FPGA_REG_4G_DL_INI_1, (0x1C<<8)|0x38);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_DL_INI_2, (0x00<<8)|0x3A);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_DL_INI_3, (0x01<<8)|0x37);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_DL_INI_4, (0x03<<8)|0x89);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_DL_INI_5, (0x04<<8)|0x00);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_DL_INI_6, (0x07<<8)|0x00);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
			
        /*4G DL PA控制 关闭/打开寄存器*/				
        fpga_reg_write(FPGA_REG_4G_DL_CLOSE, (0x00<<8)|0x3A);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/		
        fpga_reg_write(FPGA_REG_4G_DL_OPEN,  (0x00<<8)|0x3E);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        
        /*4G UL PA初始化寄存器*/
        fpga_reg_write(FPGA_REG_4G_UL_INI_1, (0x1C<<8)|0x38);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_UL_INI_2, (0x00<<8)|0x3A);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/		
        fpga_reg_write(FPGA_REG_4G_UL_INI_3, (0x01<<8)|0x37);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_UL_INI_4, (0x03<<8)|0x89);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_UL_INI_5, (0x04<<8)|0x00);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_UL_INI_6, (0x07<<8)|0x00);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
				
				/*4G UL PA控制 关闭/打开寄存器*/	
				fpga_reg_write(FPGA_REG_4G_UL_CLOSE, (0x00<<8)|0x3A);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_UL_OPEN,  (0x00<<8)|0x3E);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/  
		} 
		else if (fb == FREQ_BAND_B39_F)    
		{
        /*4G频段设置*/
        fpga_reg_write(FPGA_REG_4G_BAND, FREQ_BAND_B39_F);
        g_4g_freqband = FREQ_BAND_B39_F;
			
        /*4G DL PA初始化寄存器*/		
        fpga_reg_write(FPGA_REG_4G_DL_INI_1, (0x1C<<8)|0x38);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/		
        fpga_reg_write(FPGA_REG_4G_DL_INI_2, (0x00<<8)|0x52);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/			
        fpga_reg_write(FPGA_REG_4G_DL_INI_3, (0x01<<8)|0x37);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_DL_INI_4, (0x03<<8)|0x97);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_DL_INI_5, (0x04<<8)|0x00);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_DL_INI_6, (0x07<<8)|0x00);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
			
        /*4G DL PA控制 关闭/打开寄存器*/	
        fpga_reg_write(FPGA_REG_4G_DL_CLOSE, (0x00<<8)|0x52);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/	
        fpga_reg_write(FPGA_REG_4G_DL_OPEN,  (0x00<<8)|0x56);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        	
        /*4G UL PA初始化寄存器*/
        fpga_reg_write(FPGA_REG_4G_UL_INI_1, (0x1C<<8)|0x38);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_UL_INI_2, (0x00<<8)|0x52);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/			
        fpga_reg_write(FPGA_REG_4G_UL_INI_3, (0x01<<8)|0x37);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_UL_INI_4, (0x03<<8)|0x97);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_UL_INI_5, (0x04<<8)|0x00);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_UL_INI_6, (0x07<<8)|0x00);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
				
        /*4G UL PA控制 关闭/打开寄存器*/	
				fpga_reg_write(FPGA_REG_4G_UL_CLOSE, (0x00<<8)|0x52);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_UL_OPEN,  (0x00<<8)|0x56);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/  				
		}
		else if (fb == FREQ_BAND_B40_E)    
		{
        /*4G频段设置*/
        fpga_reg_write(FPGA_REG_4G_BAND, FREQ_BAND_B40_E);
        g_4g_freqband = FREQ_BAND_B40_E;
			
        /*4G DL PA初始化寄存器*/	
        fpga_reg_write(FPGA_REG_4G_DL_INI_1, (0x1C<<8)|0x38);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/	
        fpga_reg_write(FPGA_REG_4G_DL_INI_2, (0x00<<8)|0x70);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/			
        fpga_reg_write(FPGA_REG_4G_DL_INI_3, (0x01<<8)|0x8A);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_DL_INI_4, (0x03<<8)|0xA7);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_DL_INI_5, (0x04<<8)|0x00);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_DL_INI_6, (0x07<<8)|0x03);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
			
        /*4G DL PA控制 关闭/打开寄存器*/	
        fpga_reg_write(FPGA_REG_4G_DL_CLOSE, (0x00<<8)|0x70);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_DL_OPEN,  (0x00<<8)|0x74);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        
        /*4G UL PA初始化寄存器*/
        fpga_reg_write(FPGA_REG_4G_UL_INI_1, (0x1C<<8)|0x38);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/		
        fpga_reg_write(FPGA_REG_4G_UL_INI_2, (0x00<<8)|0x70);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/			
        fpga_reg_write(FPGA_REG_4G_UL_INI_3, (0x01<<8)|0x8A);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_UL_INI_4, (0x03<<8)|0xA7);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_UL_INI_5, (0x04<<8)|0x00);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_UL_INI_6, (0x07<<8)|0x03);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/ 
				
        /*4G UL PA控制 关闭/打开寄存器*/
        fpga_reg_write(FPGA_REG_4G_UL_OPEN,  (0x00<<8)|0x74);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_UL_CLOSE, (0x00<<8)|0x70);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/			
		}
		else
		{
        g_4g_freqband = 0xFF;			
				return 0;
		}
		
		return FPGA_OP_SUCCESS;
}
#elif(PRODUCT_MODEL == CMDT3521B)  //B RR88643-31M
/*
**********************************************************
*函数功能: fpga_set_4g_freqband 设置4G频段 并打开pa 在任务中调用
*参数说明: fb：0 900M    1 1800M    2 F频段    3 E频段
*返 回 值: 1成功
**********************************************************
*/
/*
RR88643-31M
4G DL
1  900M   B8   FDD  LB5  初始化：Reg1C:0x38  Reg00:0x28  Reg01:0xD8  Reg02:0x00  Reg03:0x80  Reg04:0x85  Reg07:0x00   开：Reg00:0x2C   关：Reg00:0x28
0  1800M  B3   FDD  MB2  初始化：Reg1C:0x38  Reg00:0x38  Reg01:0xB5  Reg02:0x00  Reg03:0x89  Reg04:0x95  Reg07:0x00   开：Reg00:0x3C   关：Reg00:0x38
2  F频段  B39  TDD  MB5  初始化：Reg1C:0x38  Reg00:0x50  Reg01:0x75  Reg02:0x00  Reg03:0x80  Reg04:0x95  Reg07:0x00   开：Reg00:0x54   关：Reg00:0x50
3  E频段  B40  TDD  HB4  初始化：Reg1C:0x38  Reg00:0x70  Reg01:0x93  Reg02:0x00  Reg03:0x80  Reg04:0x84  Reg07:0x00   开：Reg00:0x74   关：Reg00:0x70

4G UL
1  900M   B8   FDD  LB5  初始化：Reg1C:0x38  Reg00:0x28  Reg01:0xD8  Reg02:0x00  Reg03:0x80  Reg04:0x85  Reg07:0x00   开：Reg00:0x2C   关：Reg00:0x28
0  1800M  B3   FDD  MB2  初始化：Reg1C:0x38  Reg00:0x38  Reg01:0xB5  Reg02:0x00  Reg03:0x89  Reg04:0x95  Reg07:0x00   开：Reg00:0x3C   关：Reg00:0x38
2  F频段  B39  TDD  MB5  初始化：Reg1C:0x38  Reg00:0x50  Reg01:0x75  Reg02:0x00  Reg03:0x80  Reg04:0x95  Reg07:0x00   开：Reg00:0x54   关：Reg00:0x50
3  E频段  B40  TDD  HB4  初始化：Reg1C:0x38  Reg00:0x70  Reg01:0x93  Reg02:0x00  Reg03:0x80  Reg04:0x84  Reg07:0x00   开：Reg00:0x74   关：Reg00:0x70
*/


static uint8_t g_4g_pa_on_reg[4]  = {0x3C, 0x2C, 0x54, 0x74};
static uint8_t g_4g_pa_off_reg[4] = {0x38, 0x28, 0x50, 0x70};
uint8_t fpga_set_4g_freqband(uint8_t fb)
{
    if ((fb == FREQ_BAND_B8_900M) || (fb == FREQ_BAND_B3_1800M))
		{
        fpga_reg_write(FPGA_REG_4G_DL_OPEN,  (0x00<<8)|0x00);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_UL_OPEN,  (0x00<<8)|0x00);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/  			
		}
		
    if (fb == FREQ_BAND_B8_900M)    
		{			
        /*4G频段设置*/		
        fpga_reg_write(FPGA_REG_4G_BAND, FREQ_BAND_B8_900M);
				g_4g_freqband = FREQ_BAND_B8_900M;
				
				/*4G DL PA初始化寄存器*/	
        fpga_reg_write(FPGA_REG_4G_DL_INI_1, (0x1C<<8)|0x38);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_DL_INI_2, (0x00<<8)|0x28);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_DL_INI_3, (0x01<<8)|0xD8);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_DL_INI_4, (0x02<<8)|0x00);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_DL_INI_5, (0x03<<8)|0x80);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_DL_INI_6, (0x04<<8)|0x85);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/		
        fpga_reg_write(FPGA_REG_4G_DL_INI_7, (0x07<<8)|0x00);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/	

        /*4G DL PA控制 关闭/打开寄存器*/				
        fpga_reg_write(FPGA_REG_4G_DL_CLOSE, (0x00<<8)|0x28);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/	
        fpga_reg_write(FPGA_REG_4G_DL_OPEN,  (0x00<<8)|0x2C);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        	
        /*4G UL PA初始化寄存器*/
        fpga_reg_write(FPGA_REG_4G_UL_INI_1, (0x1C<<8)|0x38);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_UL_INI_2, (0x00<<8)|0x28);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_UL_INI_3, (0x01<<8)|0xD8);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_UL_INI_4, (0x02<<8)|0x00);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_UL_INI_5, (0x03<<8)|0x80);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_UL_INI_6, (0x04<<8)|0x85);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/		
        fpga_reg_write(FPGA_REG_4G_UL_INI_7, (0x07<<8)|0x00);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/		

        /*4G UL PA控制 关闭/打开寄存器*/
				fpga_reg_write(FPGA_REG_4G_UL_CLOSE, (0x00<<8)|0x28);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/		
        fpga_reg_write(FPGA_REG_4G_UL_OPEN,  (0x00<<8)|0x2C);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
		}
		else if (fb == FREQ_BAND_B3_1800M)    
		{
        /*4G频段设置*/				
        fpga_reg_write(FPGA_REG_4G_BAND, FREQ_BAND_B3_1800M);
				g_4g_freqband = FREQ_BAND_B3_1800M;
				
				/*4G DL PA初始化寄存器*/	
        fpga_reg_write(FPGA_REG_4G_DL_INI_1, (0x1C<<8)|0x38);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_DL_INI_2, (0x00<<8)|0x38);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_DL_INI_3, (0x01<<8)|0xB5);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_DL_INI_4, (0x02<<8)|0x00);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_DL_INI_5, (0x03<<8)|0x89);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_DL_INI_6, (0x04<<8)|0x95);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/		
        fpga_reg_write(FPGA_REG_4G_DL_INI_7, (0x07<<8)|0x00);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/	

        /*4G DL PA控制 关闭/打开寄存器*/			
        fpga_reg_write(FPGA_REG_4G_DL_CLOSE, (0x00<<8)|0x38);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/	
        fpga_reg_write(FPGA_REG_4G_DL_OPEN,  (0x00<<8)|0x3C);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/

        /*4G UL PA初始化寄存器*/			
        fpga_reg_write(FPGA_REG_4G_UL_INI_1, (0x1C<<8)|0x38);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_UL_INI_2, (0x00<<8)|0x38);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_UL_INI_3, (0x01<<8)|0xB5);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_UL_INI_4, (0x02<<8)|0x00);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_UL_INI_5, (0x03<<8)|0x89);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_UL_INI_6, (0x04<<8)|0x95);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/		
        fpga_reg_write(FPGA_REG_4G_UL_INI_7, (0x07<<8)|0x00);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/		

        /*4G UL PA控制 关闭/打开寄存器*/
				fpga_reg_write(FPGA_REG_4G_UL_CLOSE, (0x00<<8)|0x38);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/	
        fpga_reg_write(FPGA_REG_4G_UL_OPEN,  (0x00<<8)|0x3C);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        
		} 
		else if (fb == FREQ_BAND_B39_F)    
		{
        /*4G频段设置*/		
        fpga_reg_write(FPGA_REG_4G_BAND, FREQ_BAND_B39_F);
				g_4g_freqband = FREQ_BAND_B39_F;
				
				/*4G DL PA初始化寄存器*/	
        fpga_reg_write(FPGA_REG_4G_DL_INI_1, (0x1C<<8)|0x38);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_DL_INI_2, (0x00<<8)|0x50);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_DL_INI_3, (0x01<<8)|0x75);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_DL_INI_4, (0x02<<8)|0x00);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_DL_INI_5, (0x03<<8)|0x80);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_DL_INI_6, (0x04<<8)|0x95);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/		
        fpga_reg_write(FPGA_REG_4G_DL_INI_7, (0x07<<8)|0x00);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/	

 				/*4G DL PA控制 关闭/打开寄存器*/	      			
        fpga_reg_write(FPGA_REG_4G_DL_CLOSE, (0x00<<8)|0x50);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/	
        fpga_reg_write(FPGA_REG_4G_DL_OPEN,  (0x00<<8)|0x54);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/    	
			
        /*4G UL PA初始化寄存器*/				
        fpga_reg_write(FPGA_REG_4G_UL_INI_1, (0x1C<<8)|0x38);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_UL_INI_2, (0x00<<8)|0x50);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_UL_INI_3, (0x01<<8)|0x75);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_UL_INI_4, (0x02<<8)|0x00);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_UL_INI_5, (0x03<<8)|0x80);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_UL_INI_6, (0x04<<8)|0x95);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/		
        fpga_reg_write(FPGA_REG_4G_UL_INI_7, (0x07<<8)|0x00);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/	
				
        /*4G UL PA控制 关闭/打开寄存器*/				
				fpga_reg_write(FPGA_REG_4G_UL_CLOSE, (0x00<<8)|0x50);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/	
        fpga_reg_write(FPGA_REG_4G_UL_OPEN,  (0x00<<8)|0x54);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/			
		}
		else if (fb == FREQ_BAND_B40_E)    
		{			
        /*4G频段设置*/		
        fpga_reg_write(FPGA_REG_4G_BAND, FREQ_BAND_B40_E);
				g_4g_freqband = FREQ_BAND_B40_E;
				
				/*4G DL PA初始化寄存器*/	
        fpga_reg_write(FPGA_REG_4G_DL_INI_1, (0x1C<<8)|0x38);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_DL_INI_2, (0x00<<8)|0x70);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_DL_INI_3, (0x01<<8)|0x93);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_DL_INI_4, (0x02<<8)|0x00);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_DL_INI_5, (0x03<<8)|0x80);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_DL_INI_6, (0x04<<8)|0x84);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/		
        fpga_reg_write(FPGA_REG_4G_DL_INI_7, (0x07<<8)|0x00);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/	

				/*4G DL PA控制 关闭/打开寄存器*/			
        fpga_reg_write(FPGA_REG_4G_DL_CLOSE, (0x00<<8)|0x70);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/	
        fpga_reg_write(FPGA_REG_4G_DL_OPEN,  (0x00<<8)|0x74);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/   

        /*4G UL PA初始化寄存器*/				
        fpga_reg_write(FPGA_REG_4G_UL_INI_1, (0x1C<<8)|0x38);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_UL_INI_2, (0x00<<8)|0x70);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_UL_INI_3, (0x01<<8)|0x93);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_UL_INI_4, (0x02<<8)|0x00);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_UL_INI_5, (0x03<<8)|0x80);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_UL_INI_6, (0x04<<8)|0x84);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/		
        fpga_reg_write(FPGA_REG_4G_UL_INI_7, (0x07<<8)|0x00);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/

        /*4G UL PA控制 关闭/打开寄存器*/
				fpga_reg_write(FPGA_REG_4G_UL_CLOSE, (0x00<<8)|0x70);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/	
        fpga_reg_write(FPGA_REG_4G_UL_OPEN,  (0x00<<8)|0x74);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
		}
		else
		{
				return 0;
		}
		
		return FPGA_OP_SUCCESS;
}
#elif(PRODUCT_MODEL == CMDT3521C)  //C S55643-55
/*
**********************************************************
*函数功能: fpga_set_4g_freqband 设置4G频段 并打开pa 在任务中调用
*参数说明: fb：0 900M    1 1800M    2 F频段    3 E频段
*返 回 值: 1成功
**********************************************************
*/
/*
S55643-55
4G DL
1  900M   B8   FDD  LB5  初始化：Reg1C:0x38  Reg00:0x28  Reg01:0x34  Reg03:0xD4  Reg07:0x00  Reg09:0x0B  开：Reg00:0x2C   关：Reg00:0x28
0  1800M  B3   FDD  MB2  初始化：Reg1C:0x38  Reg00:0x38  Reg01:0x06  Reg03:0xC9  Reg07:0x00  Reg09:0x00  开：Reg00:0x3C   关：Reg00:0x38
2  F频段  B39  TDD  MB5  初始化：Reg1C:0x38  Reg00:0x52  Reg01:0x06  Reg03:0x26  Reg07:0x00  Reg09:0xE0  开：Reg00:0x56   关：Reg00:0x52
3  E频段  B40  TDD  HB4  初始化：Reg1C:0x38  Reg00:0x72  Reg01:0xF9  Reg03:0xA0  Reg07:0x06  Reg09:0xDD  开：Reg00:0x76   关：Reg00:0x72

4G UL
1  900M   B8   FDD  LB5  初始化：Reg1C:0x38  Reg00:0x28  Reg01:0x34  Reg03:0xD4  Reg07:0x00  Reg09:0x0B  开：Reg00:0x2C   关：Reg00:0x28
0  1800M  B3   FDD  MB2  初始化：Reg1C:0x38  Reg00:0x38  Reg01:0x06  Reg03:0xC9  Reg07:0x00  Reg09:0x00  开：Reg00:0x3C   关：Reg00:0x38
2  F频段  B39  TDD  MB5  初始化：Reg1C:0x38  Reg00:0x52  Reg01:0x06  Reg03:0x26  Reg07:0x00  Reg09:0xE0  开：Reg00:0x56   关：Reg00:0x52
3  E频段  B40  TDD  HB4  初始化：Reg1C:0x38  Reg00:0x72  Reg01:0xF9  Reg03:0xA0  Reg07:0x06  Reg09:0xDD  开：Reg00:0x76   关：Reg00:0x72
*/
static uint8_t g_4g_pa_on_reg[4]  = {0x3C, 0x2C, 0x56, 0x76};
static uint8_t g_4g_pa_off_reg[4] = {0x38, 0x28, 0x52, 0x72};
uint8_t fpga_set_4g_freqband(uint8_t fb)
{
    if ((fb == FREQ_BAND_B8_900M) || (fb == FREQ_BAND_B3_1800M))
		{
        fpga_reg_write(FPGA_REG_4G_DL_OPEN,  (0x00<<8)|0x00);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_UL_OPEN,  (0x00<<8)|0x00);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/  			
		}
		
    if (fb == FREQ_BAND_B8_900M)    
		{
        /*4G频段设置*/		
        fpga_reg_write(FPGA_REG_4G_BAND, FREQ_BAND_B8_900M);
        g_4g_freqband = FREQ_BAND_B8_900M;
				
				/*4G DL PA初始化寄存器*/	
        fpga_reg_write(FPGA_REG_4G_DL_INI_1, (0x1C<<8)|0x38);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_DL_INI_2, (0x00<<8)|0x28);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_DL_INI_3, (0x01<<8)|0x34);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_DL_INI_4, (0x03<<8)|0xD4);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_DL_INI_5, (0x07<<8)|0x00);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_DL_INI_6, (0x09<<8)|0x0B);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/	

				/*4G DL PA控制 关闭/打开寄存器*/				
        fpga_reg_write(FPGA_REG_4G_DL_CLOSE, (0x00<<8)|0x28);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/	
        fpga_reg_write(FPGA_REG_4G_DL_OPEN,  (0x00<<8)|0x2C);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/

        /*4G UL PA初始化寄存器*/			
        fpga_reg_write(FPGA_REG_4G_UL_INI_1, (0x1C<<8)|0x38);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_UL_INI_2, (0x00<<8)|0x28);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_UL_INI_3, (0x01<<8)|0x34);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_UL_INI_4, (0x03<<8)|0xD4);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_UL_INI_5, (0x07<<8)|0x00);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_UL_INI_6, (0x09<<8)|0x0B);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/

        /*4G UL PA控制 关闭/打开寄存器*/
				fpga_reg_write(FPGA_REG_4G_UL_CLOSE, (0x00<<8)|0x28);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/	
        fpga_reg_write(FPGA_REG_4G_UL_OPEN,  (0x00<<8)|0x2C);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/	 		
		}
		else if (fb == FREQ_BAND_B3_1800M)    
		{
        /*4G频段设置*/		
        fpga_reg_write(FPGA_REG_4G_BAND, FREQ_BAND_B3_1800M);
        g_4g_freqband = FREQ_BAND_B3_1800M;
				
				/*4G DL PA初始化寄存器*/	
        fpga_reg_write(FPGA_REG_4G_DL_INI_1, (0x1C<<8)|0x38);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_DL_INI_2, (0x00<<8)|0x38);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_DL_INI_3, (0x01<<8)|0x06);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_DL_INI_4, (0x03<<8)|0xC9);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_DL_INI_5, (0x07<<8)|0x00);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_DL_INI_6, (0x09<<8)|0x00);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/		

				/*4G DL PA控制 关闭/打开寄存器*/				
        fpga_reg_write(FPGA_REG_4G_DL_CLOSE, (0x00<<8)|0x38);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_DL_OPEN,  (0x00<<8)|0x3C);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        		
        /*4G UL PA初始化寄存器*/			
        fpga_reg_write(FPGA_REG_4G_UL_INI_1, (0x1C<<8)|0x38);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_UL_INI_2, (0x00<<8)|0x38);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_UL_INI_3, (0x01<<8)|0x06);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_UL_INI_4, (0x03<<8)|0xC9);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_UL_INI_5, (0x07<<8)|0x00);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_UL_INI_6, (0x09<<8)|0x00);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/		

        /*4G UL PA控制 关闭/打开寄存器*/
				fpga_reg_write(FPGA_REG_4G_UL_CLOSE, (0x00<<8)|0x38);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_UL_OPEN,  (0x00<<8)|0x3C);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
		} 
		else if (fb == FREQ_BAND_B39_F)    
		{
        /*4G频段设置*/		
        fpga_reg_write(FPGA_REG_4G_BAND, FREQ_BAND_B39_F);
        g_4g_freqband = FREQ_BAND_B39_F;
				
				/*4G DL PA初始化寄存器*/	
        fpga_reg_write(FPGA_REG_4G_DL_INI_1, (0x1C<<8)|0x38);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_DL_INI_2, (0x00<<8)|0x52);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_DL_INI_3, (0x01<<8)|0x06);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_DL_INI_4, (0x03<<8)|0x26);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_DL_INI_5, (0x07<<8)|0x00);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_DL_INI_6, (0x09<<8)|0xE0);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/

				/*4G DL PA控制 关闭/打开寄存器*/				
        fpga_reg_write(FPGA_REG_4G_DL_CLOSE, (0x00<<8)|0x52);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/	
        fpga_reg_write(FPGA_REG_4G_DL_OPEN,  (0x00<<8)|0x56);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        	
        /*4G UL PA初始化寄存器*/			
        fpga_reg_write(FPGA_REG_4G_UL_INI_1, (0x1C<<8)|0x38);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_UL_INI_2, (0x00<<8)|0x52);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_UL_INI_3, (0x01<<8)|0x06);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_UL_INI_4, (0x03<<8)|0x26);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_UL_INI_5, (0x07<<8)|0x00);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_UL_INI_6, (0x09<<8)|0xE0);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/		
				
        /*4G UL PA控制 关闭/打开寄存器*/				
				fpga_reg_write(FPGA_REG_4G_UL_CLOSE, (0x00<<8)|0x52);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_UL_OPEN,  (0x00<<8)|0x56);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/	
		}
		else if (fb == FREQ_BAND_B40_E)    
		{
        /*4G频段设置*/		
        fpga_reg_write(FPGA_REG_4G_BAND, FREQ_BAND_B40_E);
        g_4g_freqband = FREQ_BAND_B40_E;
				
				/*4G DL PA初始化寄存器*/	
        fpga_reg_write(FPGA_REG_4G_DL_INI_1, (0x1C<<8)|0x38);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_DL_INI_2, (0x00<<8)|0x72);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_DL_INI_3, (0x01<<8)|0xF9);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_DL_INI_4, (0x03<<8)|0xA0);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_DL_INI_5, (0x07<<8)|0x06);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_DL_INI_6, (0x09<<8)|0xDD);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/	

				/*4G DL PA控制 关闭/打开寄存器*/				
        fpga_reg_write(FPGA_REG_4G_DL_CLOSE, (0x00<<8)|0x72);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/	
        fpga_reg_write(FPGA_REG_4G_DL_OPEN,  (0x00<<8)|0x76);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/   

        /*4G UL PA初始化寄存器*/			
        fpga_reg_write(FPGA_REG_4G_UL_INI_1, (0x1C<<8)|0x38);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_UL_INI_2, (0x00<<8)|0x72);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_UL_INI_3, (0x01<<8)|0xF9);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_UL_INI_4, (0x03<<8)|0xA0);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_UL_INI_5, (0x07<<8)|0x06);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/
        fpga_reg_write(FPGA_REG_4G_UL_INI_6, (0x09<<8)|0xDD);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/			
				
        /*4G UL PA控制 关闭/打开寄存器*/				
        fpga_reg_write(FPGA_REG_4G_UL_CLOSE, (0x00<<8)|0x72);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/					
        fpga_reg_write(FPGA_REG_4G_UL_OPEN,  (0x00<<8)|0x76);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/		
		}
		else
		{
				return 0;
		}
		
		return FPGA_OP_SUCCESS;
}
#endif

/*
**********************************************************
*函数功能: fpga_4g_dl_pa_on 打开4G DL PA
*参数说明: 无
*返 回 值: 无
**********************************************************
*/
void fpga_4g_dl_pa_on(void)
{
    if (g_4g_freqband < 4)		
        fpga_reg_write(FPGA_REG_4G_DL_OPEN, (0x00<<8)|g_4g_pa_on_reg[g_4g_freqband]);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/	
}
/*
**********************************************************
*函数功能: fpga_4g_dl_pa_off 关闭4G DL PA
*参数说明: 无
*返 回 值: 无
**********************************************************
*/
void fpga_4g_dl_pa_off(void)
{
    if (g_4g_freqband < 4)	
        fpga_reg_write(FPGA_REG_4G_DL_OPEN, (0x00<<8)|g_4g_pa_off_reg[g_4g_freqband]);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/		
}

/*
**********************************************************
*函数功能: fpga_4g_ul_pa_on 打开4G UL PA
*参数说明: 无
*返 回 值: 无
**********************************************************
*/
void fpga_4g_ul_pa_on(void)
{
    if (g_4g_freqband < 4)
        fpga_reg_write(FPGA_REG_4G_UL_OPEN, (0x00<<8)|g_4g_pa_on_reg[g_4g_freqband]);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/		
}
/*
**********************************************************
*函数功能: fpga_4g_ul_pa_off 关闭4G UL PA
*参数说明: 无
*返 回 值: 无
**********************************************************
*/
void fpga_4g_ul_pa_off(void)
{
    if (g_4g_freqband < 4)
        fpga_reg_write(FPGA_REG_4G_UL_OPEN, (0x00<<8)|g_4g_pa_off_reg[g_4g_freqband]);/*15~8:mipi寄存器地址，7~0:mipi寄存器的值*/			
}
/*
**********************************************************
*函数功能: fpga_get_4g_freqband 获取4G频段  在任务中调用
*参数说明: 无
*返 回 值: fb：0 1800M    1 900M    2 F频段    3 E频段    其它 无效
**********************************************************
*/
uint8_t fpga_get_4g_freqband(void)
{
    uint8_t fb;

    fb = fpga_reg_read(FPGA_REG_4G_BAND);
	
    return fb;
}
/*
**********************************************************
*函数功能: fpga_set_5g_frame 设置5G帧格式  在任务中调用
*参数说明: pf 帧格式字符串 
							5ms单周期 如配比02:07    特殊04:04:06 表示为 "02070000040406" 
							5ms双周期 如配比1:3、2:2 特殊02:02:10 表示为 "01030202020210" 
*返 回 值: 1成功
**********************************************************
*/
uint8_t fpga_set_5g_frame(char *pf)
{
    uint8_t ul_slot_1_num;	
    uint8_t dl_slot_1_num;	
    uint8_t ul_slot_2_num;	
    uint8_t dl_slot_2_num;	
    uint8_t uwpts_num;	
    uint8_t gp_num;	
    uint8_t dwpts_num;	
	
    if (strlen(pf) == 14)
    {
				ul_slot_1_num = (*(pf+0)  - 0x30) * 10 + (*(pf+1)  - 0x30);
				dl_slot_1_num = (*(pf+2)  - 0x30) * 10 + (*(pf+3)  - 0x30);
				ul_slot_2_num = (*(pf+4)  - 0x30) * 10 + (*(pf+5)  - 0x30);
				dl_slot_2_num = (*(pf+6)  - 0x30) * 10 + (*(pf+7)  - 0x30);
				uwpts_num     = (*(pf+8)  - 0x30) * 10 + (*(pf+9)  - 0x30);
				gp_num        = (*(pf+10) - 0x30) * 10 + (*(pf+11) - 0x30);
				dwpts_num     = (*(pf+12) - 0x30) * 10 + (*(pf+13) - 0x30);    

        /*31~24:ul_slot_1_num，23~16:dl_slot_1_num，15~8:ul_slot_2_num，7~0:dl_slot_2_num*/
        fpga_reg_write(FPGA_REG_5G_FRAME,  ((ul_slot_1_num<<24)&0xFF000000) | ((dl_slot_1_num<<16)&0xFF0000) | ((ul_slot_2_num<<8)&0xFF00) | ((dl_slot_2_num)&0xFF));
			
        /*23~16:uwpts_num，15~8:gp_num，7~0:dwpts_num*/			
        fpga_reg_write(FPGA_REG_5G_FRAME2, ((uwpts_num<<16)&0xFF0000) | ((gp_num<<8)&0xFF00) | ((dwpts_num)&0xFF));
    }
    else
    {
        return 0;
    }
		
    return FPGA_OP_SUCCESS;
}
/*
**********************************************************
*函数功能: fpga_get_5g_frame 获取5G帧格式  在任务中调用
*参数说明: pf 帧格式输出字符串 
							5ms单周期 如配比02:07    特殊04:04:06 表示为 "02070000040406" 
							5ms双周期 如配比1:3、2:2 特殊02:02:10 表示为 "01030202020210" 
*返 回 值: 1成功
**********************************************************
*/
uint8_t fpga_get_5g_frame(char *pf)
{
    uint32_t frame;
    uint32_t frame1;	

    if (pf == NULL)
        return 0;
	
    frame = fpga_reg_read(FPGA_REG_5G_FRAME);/*31~24:ul_slot_1_num，23~16:dl_slot_1_num，15~8:ul_slot_2_num，7~0:dl_slot_2_num*/	
    frame1 = fpga_reg_read(FPGA_REG_5G_FRAME2);/*23~16:uwpts_num，15~8:gp_num，7~0:dwpts_num*/	
	
    if ((frame == FPGA_OP_FAIL) || (frame1 == FPGA_OP_FAIL))
        return 0;

    sprintf(pf,"%02d%02d%02d%02d%02d%02d%02d",((frame>> 24)&0xFF)%100,
																							((frame>> 16)&0xFF)%100,
																							((frame>>  8)&0xFF)%100,
																							((frame     )&0xFF)%100,
																							((frame1>>16)&0xFF)%100,
																							((frame1>> 8)&0xFF)%100,
																							((frame1    )&0xFF)%100);	

    return FPGA_OP_SUCCESS;		
}

/*
**********************************************************
*函数功能: fpga_set_4g_subframe 设置4G子帧配比  在任务中调用
*参数说明: subframe 0~6 
*返 回 值: 1成功
**********************************************************
*/
uint8_t fpga_set_4g_subframe(uint8_t subframe)
{
    if (subframe > 6)
        return 0;
		
    fpga_reg_write(FPGA_REG_4G_FRAME, subframe);
	
    return FPGA_OP_SUCCESS;	
}
/*
**********************************************************
*函数功能: fpga_get_4g_subframe 获取4G子帧配比  在任务中调用
*参数说明: 无
*返 回 值: 0~6  其它无效
**********************************************************
*/
uint8_t fpga_get_4g_subframe(void)
{
    uint32_t subframe;
	
    subframe = fpga_reg_read(FPGA_REG_4G_FRAME);
	
    if (subframe == FPGA_OP_FAIL)
        return 0xFF;
	
    return subframe;
}
/*
**********************************************************
*函数功能: fpga_set_4g_specical_subframe 设置4G特殊子帧配比  在任务中调用
*参数说明: specical_subframe 0~8 
*返 回 值: 1成功
**********************************************************
*/
uint8_t fpga_set_4g_specical_subframe(uint8_t specical_subframe)
{
    if (specical_subframe > 8)
        return 0;
		
    fpga_reg_write(FPGA_REG_4G_FRAME2, specical_subframe);
	
    return FPGA_OP_SUCCESS;	
}
/*
**********************************************************
*函数功能: fpga_get_4g_specical_subframe 获取4G特殊子帧配比  在任务中调用
*参数说明: 无
*返 回 值: 0~8  其它无效
**********************************************************
*/
uint8_t fpga_get_4g_specical_subframe(void)
{
    uint32_t specical_subframe;
	
    specical_subframe = fpga_reg_read(FPGA_REG_4G_FRAME2);
	
    if (specical_subframe == FPGA_OP_FAIL)
        return 0xFF;
	
    return specical_subframe;
}

/*
**********************************************************
*函数功能: fpga_set_4g_center_freq 设置4G中心频点  在任务中调用
*参数说明: center_freq  
*返 回 值: 1成功
**********************************************************
*/
uint8_t fpga_set_4g_center_freq(uint32_t center_freq)
{
    fpga_reg_write(FPGA_REG_4G_FREQ, center_freq);
	
    return FPGA_OP_SUCCESS;	
}
/*
**********************************************************
*函数功能: fpga_get_4g_center_freq 获取4G中心频点  在任务中调用
*参数说明: 无
*返 回 值: 4G中心频点  0xFFFFFFFF无效
**********************************************************
*/
uint32_t fpga_get_4g_center_freq(void)
{
    uint32_t center_freq;
	
    center_freq = fpga_reg_read(FPGA_REG_4G_FREQ);
		
    return center_freq;
}
/*
**********************************************************
*函数功能: fpga_set_4g_bandwidth 设置4G带宽  在任务中调用
*参数说明: bandwidth  
*返 回 值: 1成功
**********************************************************
*/
uint8_t fpga_set_4g_bandwidth(uint32_t bandwidth)
{
    fpga_reg_write(FPGA_REG_4G_BW, bandwidth);
	
    return FPGA_OP_SUCCESS;	
}
/*
**********************************************************
*函数功能: fpga_get_4g_bandwidth 获取4G带宽  在任务中调用
*参数说明: 无
*返 回 值: 4G带宽  0xFFFFFFFF无效
**********************************************************
*/
uint32_t fpga_get_4g_bandwidth(void)
{
    uint32_t bandwidth;
	
    bandwidth = fpga_reg_read(FPGA_REG_4G_BW);
		
    return bandwidth;
}
/*
**********************************************************
*函数功能: fpga_set_5g_center_freq 设置5G中心频点  在任务中调用
*参数说明: center_freq  
*返 回 值: 1成功
**********************************************************
*/
uint8_t fpga_set_5g_center_freq(uint32_t center_freq)
{
    fpga_reg_write(FPGA_REG_5G_FREQ, center_freq);
	
    return FPGA_OP_SUCCESS;	
}
/*
**********************************************************
*函数功能: fpga_get_5g_center_freq 获取5G中心频点  在任务中调用
*参数说明: 无
*返 回 值: 4G中心频点  0xFFFFFFFF无效
**********************************************************
*/
uint32_t fpga_get_5g_center_freq(void)
{
    uint32_t center_freq;
	
    center_freq = fpga_reg_read(FPGA_REG_5G_FREQ);
		
    return center_freq;
}
/*
**********************************************************
*函数功能: fpga_set_5g_bandwidth 设置5G带宽  在任务中调用
*参数说明: bandwidth  
*返 回 值: 1成功
**********************************************************
*/
uint8_t fpga_set_5g_bandwidth(uint32_t bandwidth)
{
    fpga_reg_write(FPGA_REG_5G_BW, bandwidth);
	
    return FPGA_OP_SUCCESS;	
}
/*
**********************************************************
*函数功能: fpga_get_5g_bandwidth 获取5G带宽  在任务中调用
*参数说明: 无
*返 回 值: 4G带宽  0xFFFFFFFF无效
**********************************************************
*/
uint32_t fpga_get_5g_bandwidth(void)
{
    uint32_t bandwidth;
	
    bandwidth = fpga_reg_read(FPGA_REG_5G_BW);
		
    return bandwidth;
}
/*
**********************************************************
*函数功能: fpga_get_frame_type 获取传输帧头的类型
*参数说明: 无
*返 回 值: 0:5G DL  1:4G DL  2:内部5ms
**********************************************************
*/
uint8_t fpga_get_frame_type(void)
{
    return fpga_reg_read(FPGA_REG_FRAME_TYPE);
}
/*
**********************************************************
*函数功能: fpga_get_4g_frame_dev 获取4G帧偏差
*参数说明: 无
*返 回 值: dev
**********************************************************
*/
uint32_t fpga_get_4g_frame_dev(void)
{
    return fpga_reg_read(FPGA_REG_4G_FRAME_DEV);
}
/*
**********************************************************
*函数功能: fpga_get_trx_rssi 获取TRX RSSI  在任务中调用
*参数说明: op_type: FPGA_OP_DEVICE_TRX_4G
                    FPGA_OP_DEVICE_TRX_5G1
                    FPGA_OP_DEVICE_TRX_5G2
*返 回 值: RSSI 
**********************************************************
*/
int8_t fpga_get_trx_rssi(uint8_t op_type)
{
    uint32_t  Data;
    int8_t rssi = 0;	
	
    if (op_type == FPGA_OP_DEVICE_TRX_4G)
    {
        if ((g_4g_freqband==FREQ_BAND_B3_1800M) || (g_4g_freqband==FREQ_BAND_B8_900M))
        {
            Data = BSP_866x_Ex_Read(&h8668_LTE, 0x2009B0);
            rssi = Data&0xFF;
        }	
        else if ((g_4g_freqband==FREQ_BAND_B39_F) || (g_4g_freqband==FREQ_BAND_B40_E))	
				{
            Data = BSP_866x_Ex_Read(&h8668_LTE, 0x2009B0);
            rssi = Data&0xFF;					
				}
				else
				{
				}
    }
		else if (op_type == FPGA_OP_DEVICE_TRX_5G1)
    {
        Data = BSP_866x_Ex_Read(&h8668_NR, 0x2009B0);
        rssi = Data&0xFF;				
    }
    else if (op_type == FPGA_OP_DEVICE_TRX_5G2)
    {
        Data = BSP_866x_Ex_Read(&h8668_NR_MIMO, 0x2009B0);
        rssi = Data&0xFF;	
    }
		else
		{
		}	
		
		return rssi;
}

/*
**********************************************************
*函数功能: fpga_register 器件注册,结构体初始化
*参数说明: p_fpga_config  传入器件参数
*返 回 值: STAT_OK
**********************************************************
*/ 
Status_t fpga_register(FPGA_config_t * p_fpga_config)
{
    if(p_fpga_config == NULL)
        return STAT_ERROR;
		
		memcpy((void *)&g_fpga_config, (void *)p_fpga_config, sizeof(FPGA_config_t));

    return STAT_OK;
}
