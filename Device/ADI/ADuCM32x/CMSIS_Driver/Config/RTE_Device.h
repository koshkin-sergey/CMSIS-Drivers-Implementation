/*
 * Copyright (C) 2017 Sergey Koshkin <sergey.koshkin@neophotonics.com>
 * All rights reserved
 *
 * File Name  : RTE_Device.h
 * Description: RTE Device Configuration for ADI ADuCM320
 */

//-------- <<< Use Configuration Wizard in Context Menu >>> --------------------

#ifndef HAL_CONFIG_H_
#define HAL_CONFIG_H_

/*******************************************************************************
 *  includes
 ******************************************************************************/

#include "ADuCM320.h"

/*******************************************************************************
 *  defines and macros
 ******************************************************************************/

/*- Interrupt Priority -------------------------------------------------------*/
#define HAL_ADC_INT_PRIO                1U
#define HAL_PWM_INT_PRIO                1U
#define HAL_SPI_INT_PRIO                1U
#define HAL_USART_INT_PRIO              1U
#define HAL_I2C_INT_PRIO                1U
#define HAL_TMR_INT_PRIO                0U

// <e> USART0 (Universal synchronous asynchronous receiver transmitter) [Driver_USART0]
// </e> USART0 (Universal synchronous asynchronous receiver transmitter) [Driver_USART0]
#define HAL_USART0                      1

// <e> I2C0 (Inter-integrated Circuit Interface 0) [Driver_I2C0]
// <i> Configuration settings for Driver_I2C0 in component ::Drivers:I2C
// </e> I2C0 (Inter-integrated Circuit Interface 0) [Driver_I2C0]
#define RTE_I2C0                        1

// I2C0_SCL Pin P0.4
#define RTE_I2C0_SCL_PORT               GPIO_PORT_0
#define RTE_I2C0_SCL_PIN                GPIO_PIN_4
#define RTE_I2C0_SCL_FUNC               GPIO_PIN_FUNC_1
// I2C0_SDA Pin P0.5
#define RTE_I2C0_SDA_PORT               GPIO_PORT_0
#define RTE_I2C0_SDA_PIN                GPIO_PIN_5
#define RTE_I2C0_SDA_FUNC               GPIO_PIN_FUNC_1

// <e> I2C1 (Inter-integrated Circuit Interface 1) [Driver_I2C1]
// <i> Configuration settings for Driver_I2C1 in component ::Drivers:I2C
// </e> I2C1 (Inter-integrated Circuit Interface 1) [Driver_I2C1]
#define RTE_I2C1                        0

// I2C1_SCL Pin P0.6
#define RTE_I2C1_SCL_PORT               GPIO_PORT_0
#define RTE_I2C1_SCL_PIN                GPIO_PIN_6
#define RTE_I2C1_SCL_FUNC               GPIO_PIN_FUNC_1
// I2C1_SDA Pin P0.7
#define RTE_I2C1_SDA_PORT               GPIO_PORT_0
#define RTE_I2C1_SDA_PIN                GPIO_PIN_7
#define RTE_I2C1_SDA_FUNC               GPIO_PIN_FUNC_1

//------------- <<< end of configuration section >>> ---------------------------

/*******************************************************************************
 *  typedefs and structures
 ******************************************************************************/

/*******************************************************************************
 *  exported variables
 ******************************************************************************/

/*******************************************************************************
 *  exported function prototypes
 ******************************************************************************/

__STATIC_INLINE
void NVIC_Enable_IRQ(IRQn_Type IRQn, uint32_t priority)
{
  NVIC_SetPriority(IRQn, priority);
  NVIC_EnableIRQ(IRQn);
}

__STATIC_INLINE
void NVIC_Disable_IRQ(IRQn_Type IRQn)
{
  NVIC_DisableIRQ(IRQn);
}

#endif /* HAL_CONFIG_H_ */

/* ----------------------------- End of file ---------------------------------*/
