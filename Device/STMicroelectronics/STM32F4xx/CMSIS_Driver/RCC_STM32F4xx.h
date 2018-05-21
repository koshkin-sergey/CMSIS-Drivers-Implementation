/*
 * Copyright (C) 2018 Sergey Koshkin <koshkin.sergey@gmail.com>
 * All rights reserved
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Project: RCC Driver Definitions for STMicroelectronics STM32F4xx
 */

#ifndef RCC_STM32F4XX_H_
#define RCC_STM32F4XX_H_

/*******************************************************************************
 *  includes
 ******************************************************************************/

#include <stdint.h>

#include "stm32f4xx.h"

/*******************************************************************************
 *  defines and macros
 ******************************************************************************/

#define RCC_OSC_TYPE_NONE               (0U)
#define RCC_OSC_TYPE_HSE                (1U << 0)
#define RCC_OSC_TYPE_HSI                (1U << 1)
#define RCC_OSC_TYPE_LSE                (1U << 2)
#define RCC_OSC_TYPE_LSI                (1U << 3)

#define RCC_HSE_OFF                     (0U)
#define RCC_HSE_ON                      (1U)
#define RCC_HSE_BYPASS                  (2U)

#define RCC_PLL_NONE                    (0U)
#define RCC_PLL_ON                      (1U)
#define RCC_PLL_OFF                     (2U)

#define RCC_PLL_SRC_HSE                 RCC_PLLCFGR_PLLSRC_HSE
#define RCC_PLL_SRC_HSI                 RCC_PLLCFGR_PLLSRC_HSI

#define RCC_CLK_TYPE_SYSCLK             (1U << 0)
#define RCC_CLK_TYPE_HCLK               (1U << 1)
#define RCC_CLK_TYPE_PCLK1              (1U << 2)
#define RCC_CLK_TYPE_PCLK2              (1U << 3)

#define RCC_SYSCLK_SRC_HSI              RCC_CFGR_SW_HSI
#define RCC_SYSCLK_SRC_HSE              RCC_CFGR_SW_HSE
#define RCC_SYSCLK_SRC_PLLCLK           RCC_CFGR_SW_PLL

#define RCC_SYSCLK_DIV1                 RCC_CFGR_HPRE_DIV1
#define RCC_SYSCLK_DIV2                 RCC_CFGR_HPRE_DIV2
#define RCC_SYSCLK_DIV4                 RCC_CFGR_HPRE_DIV4
#define RCC_SYSCLK_DIV8                 RCC_CFGR_HPRE_DIV8
#define RCC_SYSCLK_DIV16                RCC_CFGR_HPRE_DIV16
#define RCC_SYSCLK_DIV64                RCC_CFGR_HPRE_DIV64
#define RCC_SYSCLK_DIV128               RCC_CFGR_HPRE_DIV128
#define RCC_SYSCLK_DIV256               RCC_CFGR_HPRE_DIV256
#define RCC_SYSCLK_DIV512               RCC_CFGR_HPRE_DIV512

#define RCC_HCLK_DIV1                   RCC_CFGR_PPRE1_DIV1
#define RCC_HCLK_DIV2                   RCC_CFGR_PPRE1_DIV2
#define RCC_HCLK_DIV4                   RCC_CFGR_PPRE1_DIV4
#define RCC_HCLK_DIV8                   RCC_CFGR_PPRE1_DIV8
#define RCC_HCLK_DIV16                  RCC_CFGR_PPRE1_DIV16

#define FLASH_LATENCY_0                 FLASH_ACR_LATENCY_0WS
#define FLASH_LATENCY_1                 FLASH_ACR_LATENCY_1WS
#define FLASH_LATENCY_2                 FLASH_ACR_LATENCY_2WS
#define FLASH_LATENCY_3                 FLASH_ACR_LATENCY_3WS
#define FLASH_LATENCY_4                 FLASH_ACR_LATENCY_4WS
#define FLASH_LATENCY_5                 FLASH_ACR_LATENCY_5WS
#define FLASH_LATENCY_6                 FLASH_ACR_LATENCY_6WS
#define FLASH_LATENCY_7                 FLASH_ACR_LATENCY_7WS
#define FLASH_LATENCY_8                 FLASH_ACR_LATENCY_8WS
#define FLASH_LATENCY_9                 FLASH_ACR_LATENCY_9WS
#define FLASH_LATENCY_10                FLASH_ACR_LATENCY_10WS
#define FLASH_LATENCY_11                FLASH_ACR_LATENCY_11WS
#define FLASH_LATENCY_12                FLASH_ACR_LATENCY_12WS
#define FLASH_LATENCY_13                FLASH_ACR_LATENCY_13WS
#define FLASH_LATENCY_14                FLASH_ACR_LATENCY_14WS
#define FLASH_LATENCY_15                FLASH_ACR_LATENCY_15WS

/*******************************************************************************
 *  typedefs and structures
 ******************************************************************************/

typedef enum {
  RCC_FREQ_HSI,
  RCC_FREQ_HSE,
  RCC_FREQ_SYSCLK,
  RCC_FREQ_HCLK,
  RCC_FREQ_APB1,
  RCC_FREQ_APB2,
  RCC_FREQ_Reserved = 0x7FFFFFFF
} RCC_FREQ_t;

typedef struct RCC_Periph_s {
  volatile uint32_t *enable_reg;
  volatile uint32_t *reset_reg;
  uint32_t           mask;
  RCC_FREQ_t         freq_domain;
} RCC_Periph_t;

typedef struct RCC_PllInit_s {
  uint32_t state;
  uint32_t source;
  uint32_t m;
  uint32_t n;
  uint32_t p;
  uint32_t q;
} RCC_PllInit_t;

typedef struct RCC_OscInit_s {
  uint32_t OSC_Type;
  uint32_t HSE_State;
  uint32_t LSE_State;
  uint32_t HSI_State;
  uint32_t LSI_State;
  RCC_PllInit_t PLL;
} RCC_OscInit_t;

typedef struct RCC_ClkInit_s {
  uint32_t type;
  uint32_t sys_clk_src;
  uint32_t AHB_clk_div;
  uint32_t APB1_clk_div;
  uint32_t APB2_clk_div;
} RCC_ClkInit_t;

/*******************************************************************************
 *  exported variables
 ******************************************************************************/

/*******************************************************************************
 *  exported function prototypes
 ******************************************************************************/

/**
 * @fn      void RCC_ClkReset(void)
 * @brief   Setup the microcontroller system. Initialize the default HSI clock
 *          source and the PLL configuration is reset.
 */
void RCC_ClkReset(void);

void RCC_OscInit(RCC_OscInit_t *init);
void RCC_ClkInit(RCC_ClkInit_t *init, uint32_t flash_latency);

/**
 * @fn        uint32_t RCC_GetFreq(RCC_FREQ_t type)
 * @brief     Get Clock Frequency
 * @param[in] type  @ref RCC_FREQ_t
 * @return    Returns clock frequency in Hz
 */
uint32_t RCC_GetFreq(RCC_FREQ_t type);

/**
 * @fn          uint32_t RCC_I2SPLL_Config(uint32_t plln, uint32_t pllr)
 * @param[in]   plln
 * @param[in]   pllr
 * @return      Returns I2SPLL clock frequency in Hz
 */
uint32_t RCC_I2SPLL_Config(uint32_t plln, uint32_t pllr);

/**
 * @fn          void RCC_EnablePeriph(RCC_Periph_t *periph)
 * @param[out]  periph
 */
void RCC_EnablePeriph(const RCC_Periph_t *periph);

/**
 * @fn          void RCC_DisablePeriph(RCC_Periph_t *periph)
 * @param[out]  periph
 */
void RCC_DisablePeriph(const RCC_Periph_t *periph);

/**
 * @fn          void RCC_ResetPeriph(RCC_Periph_t *periph)
 * @param[out]  periph
 */
void RCC_ResetPeriph(const RCC_Periph_t *periph);

#endif /* RCC_STM32F4XX_H_ */

/* ----------------------------- End of file ---------------------------------*/
