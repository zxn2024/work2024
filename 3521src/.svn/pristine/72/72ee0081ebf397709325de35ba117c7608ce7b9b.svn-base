/**
  ******************************************************************************
  * @file    fpga_clock.h
  * @author  
  * @version V1.0.0
  * @date    2023-11-14
  * @brief   This file provides code for the fpga clock.
  ******************************************************************************
*/
#ifndef __FPGA_CLOCK_H
#define __FPGA_CLOCK_H

struct axi_time_sys_Msg
{
    unsigned char meaVld;/*测量有效标志*/
    unsigned int meaRes;  /*测量结果*/
    unsigned int meaRes_old;  /*测量结果*/	
};

void fpga_clock_init(void);
void fpga_clock_handle(void);
#endif
