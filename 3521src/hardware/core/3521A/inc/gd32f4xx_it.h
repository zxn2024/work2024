/*!
    \file    gd32f4xx_it.h
    \brief   the header file of the ISR

    \version 2023-06-25, V3.1.0, firmware for GD32F4xx
*/

/*
    Copyright (c) 2023, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this 
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, 
       this list of conditions and the following disclaimer in the documentation 
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors 
       may be used to endorse or promote products derived from this software without 
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE.
*/

#ifndef GD32F4XX_IT_H
#define GD32F4XX_IT_H

#include "gd32f4xx.h"

/* function declarations */
/* this function handles NMI exception */
void NMI_Handler(void);
/* this function handles HardFault exception */
void HardFault_Handler(void);
/* this function handles MemManage exception */
void MemManage_Handler(void);
/* this function handles BusFault exception */
void BusFault_Handler(void);
/* this function handles UsageFault exception */
void UsageFault_Handler(void);
///* this function handles SVC exception */
//void SVC_Handler(void);
/* this function handles DebugMon exception */
void DebugMon_Handler(void);
///* this function handles PendSV exception */
//void PendSV_Handler(void);
/* this function handles SysTick exception */
void SysTick_Handler(void);


/* EXTI0 handle function */
void EXTI0_IRQHandler(void);
/* EXTI1 handle function */
void EXTI1_IRQHandler(void);
/* EXTI3 handle function */
void EXTI3_IRQHandler(void);
/* EXTI5_9 handle function */
void EXTI5_9_IRQHandler(void);
/* EXTI10_15 handle function */
void EXTI10_15_IRQHandler(void);


/**手动添加**/
#ifdef USE_USB_FS
void TIM2_IRQHandler(void);
void USBFS_IRQHandler(void);
void USBFS_WKUP_IRQHandler(void);
#endif


void ADC_IRQHandler(void);
void USART0_IRQHandler(void);
void USART1_IRQHandler(void);
void USART2_IRQHandler(void);
void UART3_IRQHandler(void);
void UART4_IRQHandler(void);
void USART5_IRQHandler(void);


void DMA1_Channel0_IRQHandler(void);
void DMA1_Channel0_IRQHandler(void);
void DMA1_Channel7_IRQHandler(void);
void DMA0_Channel6_IRQHandler(void);
void DMA1_Channel6_IRQHandler(void);
void DMA0_Channel3_IRQHandler(void);
void DMA0_Channel4_IRQHandler(void);
void DMA0_Channel7_IRQHandler(void);

#ifdef __cplusplus
}
#endif

#endif
