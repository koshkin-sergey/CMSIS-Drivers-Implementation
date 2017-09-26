/*
 * Copyright (C) 2017 Sergey Koshkin <sergey.koshkin@neophotonics.com>
 * All rights reserved
 *
 * File Name  : CLK_ADuCM320.h
 * Description: Clock Driver Definitions for ADI ADuCM320
 */

#ifndef CLK_ADUCM320_H_
#define CLK_ADUCM320_H_

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
  CLK_PERIPH_SPI0 = (1U << 0),
  CLK_PERIPH_SPI1 = (1U << 1),
  CLK_PERIPH_I2C0 = (1U << 3),
  CLK_PERIPH_I2C1 = (1U << 4),
  CLK_PERIPH_UART = (1U << 5),
  CLK_PERIPH_D2D  = (1U << 6),
} CLK_PERIPH;

typedef enum {
  CLOCK_OFF,
  CLOCK_ON,
} CLK_MODE;

/*******************************************************************************
 *  exported variables
 ******************************************************************************/

/*******************************************************************************
 *  exported function prototypes
 ******************************************************************************/

extern
void CLK_SourceHFXTAL(void);
extern
uint32_t CLK_GetFreqHCLK(void);
extern
uint32_t CLK_GetFreqPCLK(void);
extern
uint32_t CLK_GetFreqD2DCLK(void);
extern
void CLK_PeriphGateControl(CLK_PERIPH clk, CLK_MODE mode);

#endif /* CLK_ADUCM320_H_ */

/* ----------------------------- End of file ---------------------------------*/
