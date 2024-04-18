/**
 * **********************************************************************************
 * @copyright Copyright (c) 2023 Shaanxi Tianji Communication Technologies Co., Ltd.
 *
 * @file        gd32f4xx_it.c
 * @author      WangJun
 * @version     None
 * @date        2023-09-04
 * @brief       None
 *
 * **********************************************************************************
 */

#include "gd32f4xx_it.h"
#include "systick.h"
#include "FreeRTOS.h"
#include "task.h"

#include "bsp_config.h"
#include "bsp_public_drv.h"

// trx8668
extern void BSP_866x_adc_warning_exti_handle(TRX8668_FPGA_t *h8668);

#ifdef USE_USB_FS
#include "drv_usbd_int.h"
#include "drv_usb_hw.h"

extern usb_core_driver cdc_acm;
void usb_timer_irq(void);


/*!
    \brief      this function handles timer12 interrupt Handler
    \param[in]  none
    \param[out] none
    \retval     none
*/
void TIMER7_UP_TIMER12_IRQHandler(void)
{
    usb_timer_irq();
}


/*!
    \brief      this function handles USBFS IRQ Handler
    \param[in]  none
    \param[out] none
    \retval     none
*/
void USBFS_IRQHandler(void)
{
    usbd_isr(&cdc_acm);
}


/*!
    \brief      resume mcu clock
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void resume_mcu_clk(void)
{
    /* enable HXTAL */
    rcu_osci_on(RCU_HXTAL);

    /* wait till HXTAL is ready */
    while(RESET == rcu_flag_get(RCU_FLAG_HXTALSTB))
    {
    }

    /* enable PLL */
    rcu_osci_on(RCU_PLL_CK);

    /* wait till PLL is ready */
    while(RESET == rcu_flag_get(RCU_FLAG_PLLSTB))
    {
    }

    /* select PLL as system clock source */
    rcu_system_clock_source_config(RCU_CKSYSSRC_PLLP);

    /* wait till PLL is used as system clock source */
    while(RCU_SCSS_PLLP != rcu_system_clock_source_get())
    {
    }
}


/*!
    \brief      this function handles USBFS wakeup interrupt handler
    \param[in]  none
    \param[out] none
    \retval     none
*/
void USBFS_WKUP_IRQHandler(void)
{
    if(cdc_acm.bp.low_power)
    {
        resume_mcu_clk();

        /* enable IRC48M clock */
        rcu_osci_on(RCU_IRC48M);

        /* wait till IRC48M is ready */
        while(SUCCESS != rcu_osci_stab_wait(RCU_IRC48M))
        {
        }
        rcu_ck48m_clock_config(RCU_CK48MSRC_IRC48M);

        rcu_periph_clock_enable(RCU_USBFS);

        usb_clock_active(&cdc_acm);
    }

    exti_interrupt_flag_clear(EXTI_18);
}
#endif


/*!
    \brief    this function handles NMI exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void NMI_Handler(void)
{
}

/*!
    \brief    this function handles HardFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void HardFault_Handler(void)
{
    /* if Hard Fault exception occurs, go to infinite loop */
    while(1)
    {
        
    }
}

/*!
    \brief    this function handles MemManage exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void MemManage_Handler(void)
{
    /* if Memory Manage exception occurs, go to infinite loop */
    while(1)
    {
    }
}

/*!
    \brief    this function handles BusFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void BusFault_Handler(void)
{
    /* if Bus Fault exception occurs, go to infinite loop */
    while(1)
    {
    }
}

/*!
    \brief    this function handles UsageFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void UsageFault_Handler(void)
{
    /* if Usage Fault exception occurs, go to infinite loop */
    while(1)
    {
    }
}

///*!
//    \brief    this function handles SVC exception
//    \param[in]  none
//    \param[out] none
//    \retval     none
//*/
//void SVC_Handler(void)
//{
//}

/*!
    \brief    this function handles DebugMon exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void DebugMon_Handler(void)
{
}

///*!
//    \brief    this function handles PendSV exception
//    \param[in]  none
//    \param[out] none
//    \retval     none
//*/
//void PendSV_Handler(void)
//{
//}

/*!
    \brief    this function handles SysTick exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
extern void xPortSysTickHandler(void);

void SysTick_Handler(void)
{
    if(xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
    {
        xPortSysTickHandler();
    }
    inc_tick();
    delay_decrement();
	run_time_add();
}

/*!
    \brief      this function handles external lines 0 interrupt request
    \param[in]  none
    \param[out] none
    \retval     none
*/
uint32_t irq_cnter0 = 0;
void EXTI0_IRQHandler(void)
{
    irq_cnter0 ++;
    if(RESET != exti_interrupt_flag_get(EXTI_0))
    {
        BSP_866x_adc_warning_exti_handle(&h8668_LTE);
        exti_interrupt_flag_clear(EXTI_0);
    }
}


/*!
    \brief      this function handles external lines 1 interrupt request
    \param[in]  none
    \param[out] none
    \retval     none
*/
uint32_t irq_cnter1 = 0;
void EXTI1_IRQHandler(void)
{
    irq_cnter1 ++;
    if(RESET != exti_interrupt_flag_get(EXTI_1))
    {
        BSP_866x_tia_warning_exti_handle(&h8668_LTE);
        exti_interrupt_flag_clear(EXTI_1);
    }
}


/*!
    \brief      this function handles external lines 3 interrupt request
    \param[in]  none
    \param[out] none
    \retval     none
*/
uint32_t irq_cnter3 = 0;
void EXTI3_IRQHandler(void)
{
    irq_cnter3 ++;
    if(RESET != exti_interrupt_flag_get(EXTI_3))
    {
        exti_interrupt_flag_clear(EXTI_3);
    }
}


/*!
    \brief      this function handles external lines 4 interrupt request
    \param[in]  none
    \param[out] none
    \retval     none
*/
uint32_t irq_cnter4 = 0;
void EXTI4_IRQHandler(void)
{
    irq_cnter4 ++;
    if(RESET != exti_interrupt_flag_get(EXTI_4))
    {
        exti_interrupt_flag_clear(EXTI_4);
    }
}

/*!
    \brief      this function handles external lines 5 to 9 interrupt request
    \param[in]  none
    \param[out] none
    \retval     none
*/
uint32_t irq_cnter6, irq_cnter7, irq_cnter8;
void EXTI5_9_IRQHandler(void)
{
    if(RESET != exti_interrupt_flag_get(EXTI_6))
    {
        irq_cnter6 ++;
        BSP_866x_adc_warning_exti_handle(&h8668_NR_MIMO);
        exti_interrupt_flag_clear(EXTI_6);
    }

    if(RESET != exti_interrupt_flag_get(EXTI_7))
    {
        irq_cnter7 ++;
        BSP_866x_tia_warning_exti_handle(&h8668_NR_MIMO);
        exti_interrupt_flag_clear(EXTI_7);
    }

    if(RESET != exti_interrupt_flag_get(EXTI_8))
    {
        irq_cnter8 ++;
        exti_interrupt_flag_clear(EXTI_8);
    }
}

/*!
    \brief      this function handles external lines 10 to 15 interrupt request
    \param[in]  none
    \param[out] none
    \retval     none
*/
uint32_t irq_cnter13, irq_cnter14;
void EXTI10_15_IRQHandler(void)
{
    if(RESET != exti_interrupt_flag_get(EXTI_13))
    {
        irq_cnter13++;
        BSP_866x_adc_warning_exti_handle(&h8668_NR);
        exti_interrupt_flag_clear(EXTI_13);
        // to do
    }

    if(RESET != exti_interrupt_flag_get(EXTI_14))
    {
        irq_cnter14++;
        BSP_866x_tia_warning_exti_handle(&h8668_NR);
        exti_interrupt_flag_clear(EXTI_14);
    }
}


/*!
    \brief      this function handles ADC interrupt
    \param[in]  none
    \param[out] none
    \retval     none
*/
void ADC_IRQHandler(void)
{

}


/*!
    \brief      this function handles USART0 exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void USART0_IRQHandler(void)
{
    uart_modem_fun.callback(uart_modem_fun.tpt_data);
}


/*!
    \brief      this function handles USART1 exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void USART1_IRQHandler(void)
{
    
}


/*!
    \brief      this function handles USART2 exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void USART2_IRQHandler(void)
{
    uart_ble_fun.callback(uart_ble_fun.tpt_data);
}   


/*!
    \brief      this function handles UART3 exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void UART3_IRQHandler(void)
{

}

/*!
    \brief      this function handles UART4 exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void UART4_IRQHandler(void)
{

}


/*!
    \brief      this function handles USART5 exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void USART5_IRQHandler(void)
{
    uart_cat1_fun.callback(uart_cat1_fun.tpt_data);
}


/*!
    \brief      this function handles DMA1_Channel0_IRQHandler interrupt
    \param[in]  none
    \param[out] none
    \retval     none    // adc0
*/
void DMA1_Channel0_IRQHandler(void)
{
    // if(RESET != dma_interrupt_flag_get(DMA1, DMA_CH0, DMA_INT_FLAG_HTF))    // 半传输完成
    // {
    //     dma_interrupt_flag_clear(DMA1, DMA_CH0, DMA_INT_FLAG_HTF);
    //     //ADC_ConvHalfCpltCallback();
    // }

    // if(RESET != dma_interrupt_flag_get(DMA1, DMA_CH0, DMA_INT_FLAG_FTF))    // 全传输完成
    // {
    //     dma_interrupt_flag_clear(DMA1, DMA_CH0, DMA_INT_FLAG_FTF);
    //     //ADC_ConvFullCpltCallback();
    // }
}


/*!
    \brief      this function handles DMA1_Channel1_IRQHandler interrupt
    \param[in]  none
    \param[out] none
    \retval     none    // adc2
*/
void DMA1_Channel1_IRQHandler(void)
{
    if(RESET != dma_interrupt_flag_get(DMA1, DMA_CH1, DMA_INT_FLAG_HTF))    // 半传输完成
    {
        dma_interrupt_flag_clear(DMA1, DMA_CH1, DMA_INT_FLAG_HTF);
        ADC_ConvHalfCpltCallback();
    }

    if(RESET != dma_interrupt_flag_get(DMA1, DMA_CH1, DMA_INT_FLAG_FTF))    // 全传输完成
    {
        dma_interrupt_flag_clear(DMA1, DMA_CH1, DMA_INT_FLAG_FTF);
        ADC_ConvFullCpltCallback();
    }
}


/*!
    \brief      this function handles DMA1_Channel7_IRQHandler interrupt
    \param[in]  none
    \param[out] none
    \retval     none    // USART0 TX
*/
void DMA1_Channel7_IRQHandler(void)
{
    if(RESET != dma_interrupt_flag_get(DMA1, DMA_CH7, DMA_INT_FLAG_FTF))
    {
        dma_interrupt_flag_clear(DMA1, DMA_CH7, DMA_INT_FLAG_FTF);
    }
}

///*!
//    \brief      this function handles DMA0_Channel6_IRQHandler interrupt
//    \param[in]  none
//    \param[out] none
//    \retval     none    // USART1 TX
//*/
//void DMA0_Channel6_IRQHandler(void)
//{
//    if(RESET != dma_interrupt_flag_get(DMA0, DMA_CH6, DMA_INT_FLAG_FTF))
//    {
//        dma_interrupt_flag_clear(DMA0, DMA_CH6, DMA_INT_FLAG_FTF);
//    }
//}


/*!
    \brief      this function handles DMA1_Channel6_IRQHandler interrupt
    \param[in]  none
    \param[out] none
    \retval     none    // USART5 TX
*/
void DMA1_Channel6_IRQHandler(void)
{
    if(RESET != dma_interrupt_flag_get(DMA1, DMA_CH6, DMA_INT_FLAG_FTF))
    {
        dma_interrupt_flag_clear(DMA1, DMA_CH6, DMA_INT_FLAG_FTF);
    }
}

/*!
    \brief      this function handles DMA0_Channel3_IRQHandler interrupt
    \param[in]  none
    \param[out] none
    \retval     none    // USART2 TX
*/
void DMA0_Channel3_IRQHandler(void)
{
    if(RESET != dma_interrupt_flag_get(DMA0, DMA_CH3, DMA_INT_FLAG_FTF))
    {
        dma_interrupt_flag_clear(DMA0, DMA_CH3, DMA_INT_FLAG_FTF);
    }
}


/*!
    \brief      this function handles DMA0_Channel4_IRQHandler interrupt
    \param[in]  none
    \param[out] none
    \retval     none    // UART3 TX
*/
void DMA0_Channel4_IRQHandler(void)
{
    if(RESET != dma_interrupt_flag_get(DMA0, DMA_CH4, DMA_INT_FLAG_FTF))
    {
        dma_interrupt_flag_clear(DMA0, DMA_CH4, DMA_INT_FLAG_FTF);
    }
}


/*!
    \brief      this function handles DMA0_Channel7_IRQHandler interrupt
    \param[in]  none
    \param[out] none
    \retval     none    // UART4 TX
*/
void DMA0_Channel7_IRQHandler(void)
{
    if(RESET != dma_interrupt_flag_get(DMA0, DMA_CH7, DMA_INT_FLAG_FTF))
    {
        dma_interrupt_flag_clear(DMA0, DMA_CH7, DMA_INT_FLAG_FTF);
    }
}
