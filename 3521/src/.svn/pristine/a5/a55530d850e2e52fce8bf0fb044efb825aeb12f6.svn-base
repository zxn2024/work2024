/**
 * **********************************************************************************
 * @copyright Copyright (c) 2023 Shaanxi Tianji Communication Technologies Co., Ltd.
 *                         
 * @file        MCU_config.h   
 * @author      
 * @version     None
 * @date        2023-12-04     
 * @brief       None   
 * 
 * **********************************************************************************
 */

#ifndef __MCU_CONFIG_H_
#define __MCU_CONFIG_H_

#include "system_config.h"
#include "main.h"

#ifdef __cplusplus
 extern "C" {
#endif

/****************************** 根据产品型号切换MCU库文件引用 ******************************/

#define MCU_GD32F4XX        (0x01)
#define MCU_STM32F4XX       (0x02)
#define MCU_STM32H5XX       (0x03)

#define MCU_GD32F427ZET6    (0x01)
#define MCU_GD32F427VET6    (0x02)
#define MCU_GD32F470ZET6    (0x03)
#define MCU_GD32F470VET6    (0x04)
#define MCU_STM32F407ZET6   (0x05)
#define MCU_STM32F407VET6   (0x06)
#define MCU_STM32H562ZIT6   (0x07)
#define MCU_STM32H562VIT6   (0x08)


#define GD32    (0x01)
#define STM32   (0x02)

#if(PRODUCT_MODEL == CMDT3521A)
    #include "gd32f4xx.h"
    #include "gd32f4xx_libopt.h"
    #include "gd32f4xx_it.h"
    #define MCU_SERIES  MCU_GD32F4XX
    #define MCU_MODEL   MCU_GD32F427ZET6
#elif(PRODUCT_MODEL == CMDT3521B)
    #include "gd32f4xx.h"
    #include "gd32f4xx_libopt.h"
    #include "gd32f4xx_it.h"
    #define MCU_SERIES  MCU_GD32F4XX
    #define MCU_MODEL   MCU_GD32F427ZET6
#elif(PRODUCT_MODEL == CMDT3521C)
    #include "stm32f4xx_hal.h"
    #include "stm32f4xx_it.h"
    #define MCU_SERIES  MCU_STM32F4XX
    #define MCU_MODEL   MCU_STM32F407VET6
#elif(PRODUCT_MODEL == CMDT3521D)

#elif(PRODUCT_MODEL == CMDT3521E)

#elif(PRODUCT_MODEL == CMDT3521F)

#endif

/****************************** 根据MCU型号切换外设接口结构体定义 ******************************/
#if(MCU_SERIES == MCU_GD32F4XX)
    #define MCU_MANUFACTURER    GD32
    #define PORT_T              uint32_t
    #define GPIO_PIN_T          uint32_t
    #define SPI_CH_T            uint32_t
    #define PWM_CH_T            uint32_t
    #define TIM_T               uint32_t
    #define ADC_CH_T            uint32_t
    #define UART_CH_T           uint32_t
    #define IRQ_T               IRQn_Type
#elif(MCU_SERIES == MCU_STM32F4XX || MCU_SERIES == MCU_STM32H5XX)
    #define MCU_MANUFACTURER    STM32
    #define PORT_T              GPIO_TypeDef*
    #define GPIO_PIN_T          uint32_t
    #define SPI_CH_T            SPI_HandleTypeDef*
    #define PWM_CH_T            uint32_t
    #define TIM_T               TIM_HandleTypeDef*
    #define ADC_CH_T            uint32_t
    #define UART_CH_T           UART_HandleTypeDef*
    #define IRQ_T               IRQn_Type
    
#endif


#ifdef __cplusplus
}
#endif

#endif
