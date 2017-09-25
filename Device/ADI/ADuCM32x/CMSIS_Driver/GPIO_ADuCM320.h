/*
 * Copyright (C) 2017 Sergey Koshkin <sergey.koshkin@neophotonics.com>
 * All rights reserved
 *
 * File Name  : GPIO_ADuCM320.h
 * Description: GPIO Driver Definitions for ADI ADuCM320
 */

#ifndef GPIO_ADUCM320_H_
#define GPIO_ADUCM320_H_

/*******************************************************************************
 *  includes
 ******************************************************************************/

#include <stdint.h>

/*******************************************************************************
 *  defines and macros (scope: module-local)
 ******************************************************************************/

/*******************************************************************************
 *  typedefs and structures (scope: module-local)
 ******************************************************************************/

typedef enum {
  GPIO_PORT_0   = 0,
  GPIO_PORT_1   = 1,
  GPIO_PORT_2   = 2,
  GPIO_PORT_3   = 3,
  GPIO_PORT_4   = 4,
  GPIO_PORT_5   = 5,
} GPIO_PORT_t;

typedef enum {
  GPIO_PIN_0    = 0,
  GPIO_PIN_1    = 1,
  GPIO_PIN_2    = 2,
  GPIO_PIN_3    = 3,
  GPIO_PIN_4    = 4,
  GPIO_PIN_5    = 5,
  GPIO_PIN_6    = 6,
  GPIO_PIN_7    = 7,
} GPIO_PIN_t;

typedef enum {
  GPIO_PIN_OUT_LOW  = 0,
  GPIO_PIN_OUT_HIGH  = 1,
} GPIO_PIN_OUT_t;

typedef enum {
  GPIO_DIR_INPUT  = 0,
  GPIO_DIR_OUTPUT = 1
} GPIO_DIR_t;

typedef enum {
  GPIO_PIN_FUNC_0 = 0,
  GPIO_PIN_FUNC_1 = 1,
  GPIO_PIN_FUNC_2 = 2,
  GPIO_PIN_FUNC_3 = 3
} GPIO_PIN_FUNC_t;

typedef enum {
  GPIO_PULL_UP      = 0,
  GPIO_PULL_DISABLE = 1,
  GPIO_PULL_DOWN    = 2
} GPIO_PULL_t;

typedef enum {
  GPIO_OD_DISABLE = 0,
  GPIO_OD_ENABLE = 1
} GPIO_OD_t;

typedef enum {
  GPIO_MODE_IN_FL   = 0,
  GPIO_MODE_IN_PU   = 1,
  GPIO_MODE_IN_PD   = 2,
  GPIO_MODE_IN_AN   = 3,
  GPIO_MODE_OUT_PP  = 4,
  GPIO_MODE_OUT_OD  = 5,
  GPIO_MODE_AF_PP   = 6,
  GPIO_MODE_AF_OD   = 7
} GPIO_MODE_t;

typedef struct _GPIO_PIN_CFG {
  GPIO_PORT_t port_num;
  GPIO_PIN_t pin_num;
  GPIO_MODE_t mode;
  GPIO_PIN_FUNC_t func_num;
} GPIO_PIN_CFG_t;

/*******************************************************************************
 *  exported variables
 ******************************************************************************/

/*******************************************************************************
 *  exported function prototypes
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
extern
void GPIO_PinSetFunc(GPIO_PORT_t port_num, GPIO_PIN_t pin_num, GPIO_PIN_FUNC_t func_num);

/**
 * @brief       Setup resistor mode for each pin
 * @param[in]   port_num  GPIO number (0..5)
 * @param[in]   pin_num   Port pin number
 * @param[in]   mode      Mode number, should be one of the following:
 *                        - GPIO_PULL_UP : Internal pull-up resistor
 *                        - GPIO_PULL_DISABLE : Tri-state
 *                        - GPIO_PULL_DOWN : Internal pull-down resistor
 */
extern
void GPIO_SetPullMode(GPIO_PORT_t port_num, GPIO_PIN_t pin_num, GPIO_PULL_t mode);

/**
 * @brief       Setup Open drain mode for each pin
 * @param[in]   port_num  GPIO number (0..5)
 * @param[in]   pin_num   Port pin number
 * @param[in]   mode      Open drain mode number, should be one of the following:
 *                        - GPIO_OD_DISABLE : Pin is in the normal (not open drain) mode
 *                        - GPIO_OD_ENABLE : Pin is in the open drain mode
 */
extern
void GPIO_SetOpenDrainMode(GPIO_PORT_t port_num, GPIO_PIN_t pin_num, GPIO_OD_t mode);

/**
 * @brief       Configure GPIO pin direction
 * @param[in]   port_num   GPIO number (0..5)
 * @param[in]   pin_num    Port pin number
 * @param[in]   dir        GPIO_DIR_INPUT, GPIO_DIR_OUTPUT
 */
extern
void GPIO_PinSetDir(GPIO_PORT_t port_num, GPIO_PIN_t pin_num, GPIO_DIR_t dir);

/**
 * @brief       Write port pin
 * @param[in]   port_num   GPIO number (0..5)
 * @param[in]   pin_num    Port pin number
 * @param[in]   val        Port pin value (0 or 1)
 */
extern
void GPIO_PinWrite(GPIO_PORT_t port_num, GPIO_PIN_t pin_num, GPIO_PIN_OUT_t val);

/**
 * @brief       Toggle the output of the port pin
 * @param[in]   port_num   GPIO number (0..5)
 * @param[in]   pin_num    Port pin number
 */
extern
void GPIO_PinToggle(GPIO_PORT_t port_num, GPIO_PIN_t pin_num);

/**
 * @brief       Read port pin
 * @param[in]   port_num   GPIO number (0..5)
 * @param[in]   pin_num    Port pin number
 * @return      pin value (0 or 1)
 */
extern
uint8_t GPIO_PinRead(GPIO_PORT_t port_num, GPIO_PIN_t pin_num);

/**
 * @brief       Write port pins
 * @param[in]   port_num   GPIO number (0..5)
 * @param[in]   val        Pin values
 */
extern
void GPIO_PortWrite(GPIO_PORT_t port_num, uint8_t val);

/**
 * @brief       Read port pins
 * @param[in]   port_num   GPIO number (0..5)
 * @return      port pin inputs
 */
extern
uint8_t GPIO_PortRead(GPIO_PORT_t port_num);

/**
 * @brief       Configure Pin corresponding to specified parameters
 * @param[in]   cfg Pointer to a GPIO_PIN_CFG_t structure that contains the
 *                  configuration information for the specified pin.
 */
extern
void GPIO_PinConfig(const GPIO_PIN_CFG_t *cfg);

#endif /* GPIO_ADUCM320_H_ */

/* ----------------------------- End of file ---------------------------------*/
