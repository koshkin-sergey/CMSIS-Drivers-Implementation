/*
 * Copyright (C) 2017 Sergey Koshkin <koshkin.sergey@gmail.com>
 * All rights reserved
 *
 * File Name  : GPIO_ADuCM320.c
 * Description: GPIO Driver for ST STM32F0xx
 */

/*******************************************************************************
 *  includes
 ******************************************************************************/

#include <stddef.h>
#include "GPIO_STM32F0xx.h"
#include "stm32f0xx.h"

/*******************************************************************************
 *  external declarations
 ******************************************************************************/

/*******************************************************************************
 *  defines and macros (scope: module-local)
 ******************************************************************************/

/*******************************************************************************
 *  typedefs and structures (scope: module-local)
 ******************************************************************************/

/*******************************************************************************
 *  global variable definitions  (scope: module-exported)
 ******************************************************************************/

/*******************************************************************************
 *  global variable definitions (scope: module-local)
 ******************************************************************************/

/*******************************************************************************
 *  function prototypes (scope: module-local)
 ******************************************************************************/

/*******************************************************************************
 *  function implementations (scope: module-local)
 ******************************************************************************/

static
GPIO_TypeDef* GetPort(GPIO_PORT_t port)
{
  GPIO_TypeDef *gpio = NULL;

  switch (port) {
    case GPIO_PORT_A:
      gpio = GPIOA;
      break;
    case GPIO_PORT_B:
      gpio = GPIOB;
      break;
    case GPIO_PORT_C:
      gpio = GPIOC;
      break;
    case GPIO_PORT_D:
      gpio = GPIOD;
      break;
#if defined(STM32F070x6) || defined(STM32F070xB) || defined(STM32F071xB) || \
    defined(STM32F072xB) || defined(STM32F078xx) || defined(STM32F091xC) || \
    defined(STM32F098xx)
    case GPIO_PORT_E:
      gpio = GPIOE;
      break;
#endif
    case GPIO_PORT_F:
      gpio = GPIOF;
      break;
  }

  return gpio;
}

/*******************************************************************************
 *  function implementations (scope: module-exported)
 ******************************************************************************/

/**
 * @fn          void GPIO_PortClock(GPIO_PORT_t port, GPIO_PORT_CLK_t state)
 * @brief       Port Clock Control
 * @param[in]   port    GPIO port (A..F)
 * @param[in]   state   GPIO_PORT_CLK_DISABLE - to disable
 *                      GPIO_PORT_CLK_ENABLE  - to enable
 */
void GPIO_PortClock(GPIO_PORT_t port, GPIO_PORT_CLK_t state)
{
  if (state == true) {
    switch (port) {
      case GPIO_PORT_A:
        RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
        break;
      case GPIO_PORT_B:
        RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
        break;
      case GPIO_PORT_C:
        RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
        break;
      case GPIO_PORT_D:
        RCC->AHBENR |= RCC_AHBENR_GPIODEN;
        break;
#if defined(STM32F070x6) || defined(STM32F070xB) || defined(STM32F071xB) || \
    defined(STM32F072xB) || defined(STM32F078xx) || defined(STM32F091xC) || \
    defined(STM32F098xx)
      case GPIO_PORT_E:
        RCC->AHBENR |= RCC_AHBENR_GPIOEEN;
        break;
#endif
      case GPIO_PORT_F:
        RCC->AHBENR |= RCC_AHBENR_GPIOFEN;
        break;
    }
  }
  else {
    switch (port) {
      case GPIO_PORT_A:
        RCC->AHBENR &= ~RCC_AHBENR_GPIOAEN;
        break;
      case GPIO_PORT_B:
        RCC->AHBENR &= ~RCC_AHBENR_GPIOBEN;
        break;
      case GPIO_PORT_C:
        RCC->AHBENR &= ~RCC_AHBENR_GPIOCEN;
        break;
      case GPIO_PORT_D:
        RCC->AHBENR &= ~RCC_AHBENR_GPIODEN;
        break;
#if defined(STM32F070x6) || defined(STM32F070xB) || defined(STM32F071xB) || \
    defined(STM32F072xB) || defined(STM32F078xx) || defined(STM32F091xC) || \
    defined(STM32F098xx)
      case GPIO_PORT_E:
        RCC->AHBENR &= ~RCC_AHBENR_GPIOEEN;
        break;
#endif
      case GPIO_PORT_F:
        RCC->AHBENR &= ~RCC_AHBENR_GPIOFEN;
        break;
    }
  }
}

/**
 * @fn          bool GPIO_GetPortClockState(GPIO_PORT_t port)
 * @brief       Get GPIO port clock state
 * @param[in]   port    GPIO port (A..F)
 * @return      false - disabled
 *              true  - enabled
 */
bool GPIO_GetPortClockState(GPIO_PORT_t port)
{
  switch (port) {
    case GPIO_PORT_A:
      return ((RCC->AHBENR & RCC_AHBENR_GPIOAEN) != 0UL);
    case GPIO_PORT_B:
      return ((RCC->AHBENR & RCC_AHBENR_GPIOBEN) != 0UL);
    case GPIO_PORT_C:
      return ((RCC->AHBENR & RCC_AHBENR_GPIOCEN) != 0UL);
    case GPIO_PORT_D:
      return ((RCC->AHBENR & RCC_AHBENR_GPIODEN) != 0UL);
#if defined(STM32F070x6) || defined(STM32F070xB) || defined(STM32F071xB) || \
  defined(STM32F072xB) || defined(STM32F078xx) || defined(STM32F091xC) || \
  defined(STM32F098xx)
    case GPIO_PORT_E:
      return ((RCC->AHBENR & RCC_AHBENR_GPIOEEN) != 0UL);
#endif
    case GPIO_PORT_F:
      return ((RCC->AHBENR & RCC_AHBENR_GPIOFEN) != 0UL);
    default:
      return false;
  }
}

/**
 * @fn          void GPIO_PinToggle(GPIO_PORT_t port, GPIO_PIN_t pin)
 * @brief       Toggle the output of the port pin
 * @param[in]   port  GPIO port (A..F)
 * @param[in]   pin   Port pin number
 */
void GPIO_PinToggle(GPIO_PORT_t port, GPIO_PIN_t pin)
{
  GPIO_TypeDef *gpio = GetPort(port);

  gpio->ODR ^= (1UL << pin);
}

/**
 * @fn          void GPIO_PinWrite(GPIO_PORT_t port, GPIO_PIN_t pin, GPIO_PIN_OUT_t value)
 * @brief       Write port pin
 * @param[in]   port  GPIO port (A..F)
 * @param[in]   pin   Port pin number
 * @param[in]   value Port pin value (0 or 1)
 */
void GPIO_PinWrite(GPIO_PORT_t port, GPIO_PIN_t pin, GPIO_PIN_OUT_t value)
{
  GPIO_TypeDef *gpio = GetPort(port);
  uint32_t shift = 0;

  if (value == GPIO_PIN_OUT_LOW)
    shift = 16;

  gpio->BSRR = ((1UL << pin) << shift);
}

/**
 * @fn          uint32_t GPIO_PinRead(GPIO_PORT_t port, GPIO_PIN_t pin)
 * @brief       Read port pin
 * @param[in]   port  GPIO port (A..F)
 * @param[in]   pin   Port pin number
 * @return      pin value (0 or 1)
 */
uint32_t GPIO_PinRead(GPIO_PORT_t port, GPIO_PIN_t pin)
{
  GPIO_TypeDef *gpio = GetPort(port);

  return ((gpio->IDR >> pin) & 1UL);
}

/**
 * @fn          void GPIO_PortWrite(GPIO_PORT_t port, uint16_t value)
 * @brief       Write port pins
 * @param[in]   port  GPIO port (A..F)
 * @param[in]   value Pin values
 */
void GPIO_PortWrite(GPIO_PORT_t port, uint16_t value)
{
  GPIO_TypeDef *gpio = GetPort(port);

  gpio->ODR = (uint32_t)value;
}

/**
 * @fn          uint16_t GPIO_PortRead(GPIO_PORT_t port)
 * @brief       Read port pins
 * @param[in]   port  GPIO port (A..F)
 * @return      port  pin inputs
 */
uint16_t GPIO_PortRead(GPIO_PORT_t port)
{
  GPIO_TypeDef *gpio = GetPort(port);

  return (uint16_t)gpio->IDR;
}

/**
 * @fn          void GPIO_PinConfig(GPIO_PORT_t port, GPIO_PIN_t pin, const GPIO_PIN_CFG_t *cfg)
 * @brief       Configure Pin corresponding to specified parameters
 * @param[in]   port  GPIO port (A..F)
 * @param[in]   pin   Port pin number (0..15)
 * @param[in]   cfg   Pointer to a GPIO_PIN_CFG_t structure that contains the
 *                    configuration information for the specified pin.
 */
void GPIO_PinConfig(GPIO_PORT_t port, GPIO_PIN_t pin, const GPIO_PIN_CFG_t *cfg)
{
  GPIO_TypeDef *gpio;
  uint32_t shift;
  uint32_t moder, otyper, ospeedr;

  if (cfg == NULL)
    return;

  gpio = GetPort(port);
  shift = (pin << 1);

  moder = (gpio->MODER & ~(3UL << shift));
  otyper = (gpio->OTYPER & ~(1UL << pin));
  ospeedr = (gpio->OSPEEDR  & ~(3UL << shift));

  gpio->MODER = (moder | ((cfg->mode & 3UL) << shift));
  gpio->OTYPER = (otyper | (((cfg->mode >> 8) & 1UL) << pin));
  gpio->OSPEEDR = (ospeedr | ((cfg->speed & 3UL) << shift));

  if ((cfg->mode & 3UL) == 2UL) {
    uint32_t afr;
    volatile uint32_t *pafr = &gpio->AFR[pin >> 3];

    shift = (((uint32_t)(pin & 7UL)) << 2);
    afr = (*pafr & ~(0xF << shift));
    *pafr = (afr | ((uint32_t)cfg->func << shift));
  }
}

/* ----------------------------- End of file ---------------------------------*/
