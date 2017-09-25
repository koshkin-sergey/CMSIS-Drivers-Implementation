/*
 * Copyright (C) 2017 Sergey Koshkin <sergey.koshkin@neophotonics.com>
 * All rights reserved
 *
 * File Name  : GPIO_ADuCM320.c
 * Description: GPIO Driver for ADI ADuCM320
 */

/*******************************************************************************
 *  includes
 ******************************************************************************/

#include "stddef.h"
#include "ADuCM320.h"
#include "GPIO_ADuCM320.h"

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
ADI_GPIO_TypeDef* GetPort(GPIO_PORT_t port_num)
{
  ADI_GPIO_TypeDef *port = NULL;

  switch (port_num) {
    case GPIO_PORT_0:
      port = pADI_GP0;
      break;
    case GPIO_PORT_1:
      port = pADI_GP1;
      break;
    case GPIO_PORT_2:
      port = pADI_GP2;
      break;
    case GPIO_PORT_3:
      port = pADI_GP3;
      break;
    case GPIO_PORT_4:
      port = pADI_GP4;
      break;
    case GPIO_PORT_5:
      port = pADI_GP5;
      break;
  }

  return port;
}

/*******************************************************************************
 *  function implementations (scope: module-exported)
 ******************************************************************************/

/**
 * @brief       Setup the pin selection function
 * @param[in]   port_num  GPIO number (0..5)
 * @param[in]   pin_num   Port pin number
 * @param[in]   func_num  Function number, should be one of the following:
 *                        - GPIO_PIN_FUNC_0 : default function
 *                        - GPIO_PIN_FUNC_1 : first alternate function
 *                        - GPIO_PIN_FUNC_2 : second alternate function
 *                        - GPIO_PIN_FUNC_3 : third alternate function
 */
void GPIO_PinSetFunc(GPIO_PORT_t port_num, GPIO_PIN_t pin_num, GPIO_PIN_FUNC_t func_num)
{
  ADI_GPIO_TypeDef *port = GetPort(port_num);
  uint16_t con = port->GPCON;

  con &= ~(3U << (pin_num << 1));
  con |= func_num << (pin_num << 1);
  port->GPCON = con;
}

/**
 * @brief       Setup resistor mode for each pin
 * @param[in]   port_num  GPIO number (0..5)
 * @param[in]   pin_num   Port pin number
 * @param[in]   mode      Mode number, should be one of the following:
 *                        - GPIO_PULL_UP : Internal pull-up resistor
 *                        - GPIO_PULL_DISABLE : Tri-state
 *                        - GPIO_PULL_DOWN : Internal pull-down resistor
 */
void GPIO_SetPullMode(GPIO_PORT_t port_num, GPIO_PIN_t pin_num, GPIO_PULL_t mode)
{
  ADI_GPIO_TypeDef *port = GetPort(port_num);

  switch (mode) {
    case GPIO_PULL_UP:
    case GPIO_PULL_DOWN:
      port->GPPUL |= (1U << pin_num);
      break;

    case GPIO_PULL_DISABLE:
      port->GPPUL &= ~(1U << pin_num);
      break;
  }
}

/**
 * @brief       Setup Open drain mode for each pin
 * @param[in]   port_num  GPIO number (0..5)
 * @param[in]   pin_num   Port pin number
 * @param[in]   mode      Open drain mode number, should be one of the following:
 *                        - GPIO_OD_DISABLE : Pin is in the normal (not open drain) mode
 *                        - GPIO_OD_ENABLE : Pin is in the open drain mode
 */
void GPIO_SetOpenDrainMode(GPIO_PORT_t port_num, GPIO_PIN_t pin_num, GPIO_OD_t mode)
{
  ADI_GPIO_TypeDef *port = GetPort(port_num);

  if (mode == GPIO_OD_ENABLE)
    port->GPODE |= (1U << pin_num);
  else
    port->GPODE &= ~(1U << pin_num);
}

/**
 * @brief       Configure GPIO pin direction
 * @param[in]   port_num   GPIO number (0..5)
 * @param[in]   pin_num    Port pin number
 * @param[in]   dir        GPIO_DIR_INPUT, GPIO_DIR_OUTPUT
 */
void GPIO_PinSetDir(GPIO_PORT_t port_num, GPIO_PIN_t pin_num, GPIO_DIR_t dir)
{
  ADI_GPIO_TypeDef *port = GetPort(port_num);

  switch (dir) {
    case GPIO_DIR_INPUT:
      port->GPIE |= (1U << pin_num);
      port->GPOE &= ~(1U << pin_num);
      break;

    case GPIO_DIR_OUTPUT:
      port->GPIE &= ~(1U << pin_num);
      port->GPOE |= (1U << pin_num);
      break;
  }
}

/**
 * @brief       Write port pin
 * @param[in]   port_num  GPIO number (0..5)
 * @param[in]   pin_num   Port pin number
 * @param[in]   val       Port pin value, should be one of the following:
 *                        - GPIO_PIN_OUT_LOW
 *                        - GPIO_PIN_OUT_HIGH
 */
void GPIO_PinWrite(GPIO_PORT_t port_num, GPIO_PIN_t pin_num, GPIO_PIN_OUT_t val)
{
  ADI_GPIO_TypeDef *port = GetPort(port_num);

  if (val == GPIO_PIN_OUT_HIGH)
    port->GPSET = (uint8_t)(1U << pin_num);
  else
    port->GPCLR = (uint8_t)(1U << pin_num);
}

/**
 * @brief       Toggle the output of the port pin
 * @param[in]   port_num   GPIO number (0..5)
 * @param[in]   pin_num    Port pin number
 */
void GPIO_PinToggle(GPIO_PORT_t port_num, GPIO_PIN_t pin_num)
{
  ADI_GPIO_TypeDef *port = GetPort(port_num);

  port->GPTGL = (uint8_t)(1U << pin_num);
}

/**
 * @brief       Read port pin
 * @param[in]   port_num   GPIO number (0..5)
 * @param[in]   pin_num    Port pin number
 * @return      pin value (0 or 1)
 */
uint8_t GPIO_PinRead(GPIO_PORT_t port_num, GPIO_PIN_t pin_num)
{
  ADI_GPIO_TypeDef *port = GetPort(port_num);

  return ((port->GPIN >> pin_num) & 1U);
}

/**
 * @brief       Write port pins
 * @param[in]   port_num   GPIO number (0..5)
 * @param[in]   val        Pin values
 */
void GPIO_PortWrite(GPIO_PORT_t port_num, uint8_t val)
{
  ADI_GPIO_TypeDef *port = GetPort(port_num);

  port->GPOUT = val;
}

/**
 * @brief       Read port pins
 * @param[in]   port_num   GPIO number (0..5)
 * @return      port pin inputs
 */
uint8_t GPIO_PortRead(GPIO_PORT_t port_num)
{
  ADI_GPIO_TypeDef *port = GetPort(port_num);

  return port->GPIN;
}

/**
 * @brief       Configure Pin corresponding to specified parameters
 * @param[in]   cfg Pointer to a GPIO_PIN_CFG_t structure that contains the
 *                  configuration information for the specified pin.
 */
void GPIO_PinConfig(const GPIO_PIN_CFG_t *cfg)
{
  ADI_GPIO_TypeDef *port;
  uint8_t pin;

  if (cfg == NULL)
    return;

  port = GetPort(cfg->port_num);
  pin = (uint8_t)(1U << cfg->pin_num);

  if (cfg->mode == GPIO_MODE_IN_AN)
    port->GPIE &= ~pin;
  else
    port->GPIE |= pin;

  switch (cfg->mode) {
    case GPIO_MODE_IN_FL:
    case GPIO_MODE_IN_AN:
      port->GPOE &= ~pin;
      port->GPPUL &= ~pin;
      break;

    case GPIO_MODE_IN_PU:
    case GPIO_MODE_IN_PD:
      port->GPOE &= ~pin;
      port->GPPUL |= pin;
      break;

    case GPIO_MODE_OUT_PP:
      port->GPOE |= pin;
      port->GPODE &= ~pin;
      break;

    case GPIO_MODE_OUT_OD:
      port->GPOE |= pin;
      port->GPODE |= pin;
      break;

    case GPIO_MODE_AF_OD:
      port->GPPUL &= ~pin;
      /* no break */
    case GPIO_MODE_AF_PP:
    {
      uint16_t con = port->GPCON;
      con &= ~(3U << (cfg->pin_num << 1));
      con |= cfg->func_num << (cfg->pin_num << 1);
      port->GPCON = con;
    }
      break;
  }
}

/* ----------------------------- End of file ---------------------------------*/
