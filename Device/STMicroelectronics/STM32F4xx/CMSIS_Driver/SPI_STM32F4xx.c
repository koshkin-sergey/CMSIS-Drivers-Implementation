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
 * Project: SPI Driver for STMicroelectronics STM32F4xx
 */

/*******************************************************************************
 *  includes
 ******************************************************************************/
 
#include "SPI_STM32F4xx.h"

/*******************************************************************************
 *  external declarations
 ******************************************************************************/

/*******************************************************************************
 *  defines and macros (scope: module-local)
 ******************************************************************************/

#define ARM_SPI_DRV_VERSION    ARM_DRIVER_VERSION_MAJOR_MINOR(1, 0) /* driver version */

/*******************************************************************************
 *  typedefs and structures (scope: module-local)
 ******************************************************************************/

/*******************************************************************************
 *  function prototypes (scope: module-local)
 ******************************************************************************/

#ifdef SPI1_TX_DMA_Stream
static void SPI1_TX_DMA_Complete(uint32_t event);
#endif // SPI1_TX_DMA_Stream

#ifdef SPI1_RX_DMA_Stream
static void SPI1_RX_DMA_Complete(uint32_t event);
#endif // SPI1_RX_DMA_Stream

#ifdef SPI2_TX_DMA_Stream
static void SPI2_TX_DMA_Complete(uint32_t event);
#endif // SPI2_TX_DMA_Stream

#ifdef SPI2_RX_DMA_Stream
static void SPI2_RX_DMA_Complete(uint32_t event);
#endif // SPI2_RX_DMA_Stream

#ifdef SPI3_TX_DMA_Stream
static void SPI3_TX_DMA_Complete(uint32_t event);
#endif // SPI3_TX_DMA_Stream

#ifdef SPI3_RX_DMA_Stream
static void SPI3_RX_DMA_Complete(uint32_t event);
#endif // SPI3_RX_DMA_Stream

#ifdef SPI4_TX_DMA_Stream
static void SPI4_TX_DMA_Complete(uint32_t event);
#endif // SPI4_TX_DMA_Stream

#ifdef SPI4_RX_DMA_Stream
static void SPI4_RX_DMA_Complete(uint32_t event);
#endif // SPI4_RX_DMA_Stream

#ifdef SPI5_TX_DMA_Stream
static void SPI5_TX_DMA_Complete(uint32_t event);
#endif // SPI5_TX_DMA_Stream

#ifdef SPI5_RX_DMA_Stream
static void SPI5_RX_DMA_Complete(uint32_t event);
#endif // SPI5_RX_DMA_Stream

#ifdef SPI6_TX_DMA_Stream
static void SPI6_TX_DMA_Complete(uint32_t event);
#endif // SPI6_TX_DMA_Stream

#ifdef SPI6_RX_DMA_Stream
static void SPI6_RX_DMA_Complete(uint32_t event);
#endif // SPI6_RX_DMA_Stream

/*******************************************************************************
 *  global variable definitions (scope: module-local)
 ******************************************************************************/

/* Driver Version */
static const ARM_DRIVER_VERSION DriverVersion = {
  ARM_SPI_API_VERSION,
  ARM_SPI_DRV_VERSION
};

/* Driver Capabilities */
static const ARM_SPI_CAPABILITIES DriverCapabilities = {
  0,  /* Simplex Mode (Master and Slave) */
  1,  /* TI Synchronous Serial Interface */
  0,  /* Microwire Interface */
  1,  /* Signal Mode Fault event: \ref ARM_SPI_EVENT_MODE_FAULT */
};

static const GPIO_PIN_CFG_t SPI_pin_cfg_af = {
    GPIO_MODE_AF_PP, GPIO_PULL_DISABLE, GPIO_SPEED_VERY_HIGH
};

static const GPIO_PIN_CFG_t SPI_pin_cfg_analog = {
    GPIO_MODE_ANALOG, GPIO_PULL_DISABLE, GPIO_SPEED_LOW
};

static const GPIO_PIN_CFG_t SPI_pin_cfg_out_pp = {
    GPIO_MODE_OUT_PP, GPIO_PULL_DISABLE, GPIO_SPEED_MEDIUM
};

#if defined(USE_SPI1)

/* SPI1 Information (Run-Time) */
static SPI_INFO SPI1_Info;
static SPI_TRANSFER_INFO SPI1_TransferInfo;

#ifdef USE_SPI1_MISO_Pin
  static SPI_PIN SPI1_miso = {SPI1_MISO_GPIO_PORT, SPI1_MISO_GPIO_PIN, SPI1_MISO_GPIO_FUNC};
#endif

#ifdef USE_SPI1_MOSI_Pin
  static SPI_PIN SPI1_mosi = {SPI1_MOSI_GPIO_PORT, SPI1_MOSI_GPIO_PIN, SPI1_MOSI_GPIO_FUNC};
#endif

#ifdef USE_SPI1_SCK_Pin
  static SPI_PIN SPI1_sck = {SPI1_SCK_GPIO_PORT, SPI1_SCK_GPIO_PIN, SPI1_SCK_GPIO_FUNC};
#endif

#ifdef USE_SPI1_NSS_Pin
  static SPI_PIN SPI1_nss = {SPI1_NSS_GPIO_PORT, SPI1_NSS_GPIO_PIN, SPI1_NSS_GPIO_FUNC};
#endif

#ifdef SPI1_TX_DMA_Stream

static DMA_Handle_t SPI1_TX_DMA_Handle;
static DMA_Resources_t SPI1_TX_DMA = {
  &SPI1_TX_DMA_Handle,
  SPI1_TX_DMA_Stream,
  SPI1_TX_DMA_Channel,
  SPI1_TX_DMA_Priority,
  SPI1_TX_DMA_Complete,
  RTE_SPI_DMA_INT_PRIORITY,
  SPI1_TX_DMA_IRQn,
};

#endif

#ifdef SPI1_RX_DMA_Stream

static DMA_Handle_t SPI1_RX_DMA_Handle;
static DMA_Resources_t SPI1_RX_DMA = {
  &SPI1_RX_DMA_Handle,
  SPI1_RX_DMA_Stream,
  SPI1_RX_DMA_Channel,
  SPI1_RX_DMA_Priority,
  SPI1_RX_DMA_Complete,
  RTE_SPI_DMA_INT_PRIORITY,
  SPI1_RX_DMA_IRQn,
};

#endif

/* SPI1 Resources */
static SPI_RESOURCES SPI1_Resources = {
  SPI1,
  {
#ifdef USE_SPI1_MISO_Pin
      &SPI1_miso,
#else
      NULL,
#endif
#ifdef USE_SPI1_MOSI_Pin
      &SPI1_mosi,
#else
      NULL,
#endif
#ifdef USE_SPI1_SCK_Pin
      &SPI1_sck,
#else
      NULL,
#endif
#ifdef USE_SPI1_NSS_Pin
      &SPI1_nss,
#else
      NULL,
#endif
  },
  {
    &RCC->APB2ENR,
    &RCC->APB2RSTR,
    RCC_APB2ENR_SPI1EN,
    RCC_FREQ_APB2,
  },
#ifdef SPI1_TX_DMA_Stream
  &SPI1_TX_DMA,
#else
  NULL,
#endif
#ifdef SPI1_RX_DMA_Stream
  &SPI1_RX_DMA,
#else
  NULL,
#endif
  &SPI1_Info,
  &SPI1_TransferInfo,
  SPI1_IRQn
};

#endif /* USE_SPI1 */

#if defined(USE_SPI2)

/* SPI2 Information (Run-Time) */
static SPI_INFO SPI2_Info;
static SPI_TRANSFER_INFO SPI2_TransferInfo;

#ifdef USE_SPI2_MISO_Pin
  static SPI_PIN SPI2_miso = {SPI2_MISO_GPIO_PORT, SPI2_MISO_GPIO_PIN, SPI2_MISO_GPIO_FUNC};
#endif

#ifdef USE_SPI2_MOSI_Pin
  static SPI_PIN SPI2_mosi = {SPI2_MOSI_GPIO_PORT, SPI2_MOSI_GPIO_PIN, SPI2_MOSI_GPIO_FUNC};
#endif

#ifdef USE_SPI2_SCK_Pin
  static SPI_PIN SPI2_sck = {SPI2_SCK_GPIO_PORT, SPI2_SCK_GPIO_PIN, SPI2_SCK_GPIO_FUNC};
#endif

#ifdef USE_SPI2_NSS_Pin
  static SPI_PIN SPI2_nss = {SPI2_NSS_GPIO_PORT, SPI2_NSS_GPIO_PIN, SPI2_NSS_GPIO_FUNC};
#endif

#ifdef SPI2_TX_DMA_Stream

static DMA_Handle_t SPI2_TX_DMA_Handle;
static DMA_Resources_t SPI2_TX_DMA = {
  &SPI2_TX_DMA_Handle,
  SPI2_TX_DMA_Stream,
  SPI2_TX_DMA_Channel,
  SPI2_TX_DMA_Priority,
  SPI2_TX_DMA_Complete,
  RTE_SPI_DMA_INT_PRIORITY,
  SPI2_TX_DMA_IRQn,
};

#endif

#ifdef SPI2_RX_DMA_Stream

static DMA_Handle_t SPI2_RX_DMA_Handle;
static DMA_Resources_t SPI2_RX_DMA = {
  &SPI2_RX_DMA_Handle,
  SPI2_RX_DMA_Stream,
  SPI2_RX_DMA_Channel,
  SPI2_RX_DMA_Priority,
  SPI2_RX_DMA_Complete,
  RTE_SPI_DMA_INT_PRIORITY,
  SPI2_RX_DMA_IRQn,
};

#endif

/* SPI2 Resources */
static SPI_RESOURCES SPI2_Resources = {
  SPI2,
  {
#ifdef USE_SPI2_MISO_Pin
      &SPI2_miso,
#else
      NULL,
#endif
#ifdef USE_SPI2_MOSI_Pin
      &SPI2_mosi,
#else
      NULL,
#endif
#ifdef USE_SPI2_SCK_Pin
      &SPI2_sck,
#else
      NULL,
#endif
#ifdef USE_SPI2_NSS_Pin
      &SPI2_nss,
#else
      NULL,
#endif
  },
  {
    &RCC->APB1ENR,
    &RCC->APB1RSTR,
    RCC_APB1ENR_SPI2EN,
    RCC_FREQ_APB1,
  },
#ifdef SPI2_TX_DMA_Stream
  &SPI2_TX_DMA,
#else
  NULL,
#endif
#ifdef SPI2_RX_DMA_Stream
  &SPI2_RX_DMA,
#else
  NULL,
#endif
  &SPI2_Info,
  &SPI2_TransferInfo,
  SPI2_IRQn
};

#endif /* USE_SPI2 */

#if defined(USE_SPI3)

/* SPI3 Information (Run-Time) */
static SPI_INFO SPI3_Info;
static SPI_TRANSFER_INFO SPI3_TransferInfo;

#ifdef USE_SPI3_MISO_Pin
  static SPI_PIN SPI3_miso = {SPI3_MISO_GPIO_PORT, SPI3_MISO_GPIO_PIN, SPI3_MISO_GPIO_FUNC};
#endif

#ifdef USE_SPI3_MOSI_Pin
  static SPI_PIN SPI3_mosi = {SPI3_MOSI_GPIO_PORT, SPI3_MOSI_GPIO_PIN, SPI3_MOSI_GPIO_FUNC};
#endif

#ifdef USE_SPI3_SCK_Pin
  static SPI_PIN SPI3_sck = {SPI3_SCK_GPIO_PORT, SPI3_SCK_GPIO_PIN, SPI3_SCK_GPIO_FUNC};
#endif

#ifdef USE_SPI3_NSS_Pin
  static SPI_PIN SPI3_nss = {SPI3_NSS_GPIO_PORT, SPI3_NSS_GPIO_PIN, SPI3_NSS_GPIO_FUNC};
#endif

#ifdef SPI3_TX_DMA_Stream

static DMA_Handle_t SPI3_TX_DMA_Handle;
static DMA_Resources_t SPI3_TX_DMA = {
  &SPI3_TX_DMA_Handle,
  SPI3_TX_DMA_Stream,
  SPI3_TX_DMA_Channel,
  SPI3_TX_DMA_Priority,
  SPI3_TX_DMA_Complete,
  RTE_SPI_DMA_INT_PRIORITY,
  SPI3_TX_DMA_IRQn,
};

#endif

#ifdef SPI3_RX_DMA_Stream

static DMA_Handle_t SPI3_RX_DMA_Handle;
static DMA_Resources_t SPI3_RX_DMA = {
  &SPI3_RX_DMA_Handle,
  SPI3_RX_DMA_Stream,
  SPI3_RX_DMA_Channel,
  SPI3_RX_DMA_Priority,
  SPI3_RX_DMA_Complete,
  RTE_SPI_DMA_INT_PRIORITY,
  SPI3_RX_DMA_IRQn,
};

#endif

/* SPI3 Resources */
static SPI_RESOURCES SPI3_Resources = {
  SPI3,
  {
#ifdef USE_SPI3_MISO_Pin
      &SPI3_miso,
#else
      NULL,
#endif
#ifdef USE_SPI3_MOSI_Pin
      &SPI3_mosi,
#else
      NULL,
#endif
#ifdef USE_SPI3_SCK_Pin
      &SPI3_sck,
#else
      NULL,
#endif
#ifdef USE_SPI3_NSS_Pin
      &SPI3_nss,
#else
      NULL,
#endif
  },
  {
    &RCC->APB1ENR,
    &RCC->APB1RSTR,
    RCC_APB1ENR_SPI3EN,
    RCC_FREQ_APB1,
  },
#ifdef SPI3_TX_DMA_Stream
  &SPI3_TX_DMA,
#else
  NULL,
#endif
#ifdef SPI3_RX_DMA_Stream
  &SPI3_RX_DMA,
#else
  NULL,
#endif
  &SPI3_Info,
  &SPI3_TransferInfo,
  SPI3_IRQn
};

#endif /* USE_SPI3 */

#if defined(USE_SPI4)

/* SPI4 Information (Run-Time) */
static SPI_INFO SPI4_Info;
static SPI_TRANSFER_INFO SPI4_TransferInfo;

#ifdef USE_SPI4_MISO_Pin
  static SPI_PIN SPI4_miso = {SPI4_MISO_GPIO_PORT, SPI4_MISO_GPIO_PIN, SPI4_MISO_GPIO_FUNC};
#endif

#ifdef USE_SPI4_MOSI_Pin
  static SPI_PIN SPI4_mosi = {SPI4_MOSI_GPIO_PORT, SPI4_MOSI_GPIO_PIN, SPI4_MOSI_GPIO_FUNC};
#endif

#ifdef USE_SPI4_SCK_Pin
  static SPI_PIN SPI4_sck = {SPI4_SCK_GPIO_PORT, SPI4_SCK_GPIO_PIN, SPI4_SCK_GPIO_FUNC};
#endif

#ifdef USE_SPI4_NSS_Pin
  static SPI_PIN SPI4_nss = {SPI4_NSS_GPIO_PORT, SPI4_NSS_GPIO_PIN, SPI4_NSS_GPIO_FUNC};
#endif

#ifdef SPI4_TX_DMA_Stream

static DMA_Handle_t SPI4_TX_DMA_Handle;
static DMA_Resources_t SPI4_TX_DMA = {
  &SPI4_TX_DMA_Handle,
  SPI4_TX_DMA_Stream,
  SPI4_TX_DMA_Channel,
  SPI4_TX_DMA_Priority,
  SPI4_TX_DMA_Complete,
  RTE_SPI_DMA_INT_PRIORITY,
  SPI4_TX_DMA_IRQn,
};

#endif

#ifdef SPI4_RX_DMA_Stream

static DMA_Handle_t SPI4_RX_DMA_Handle;
static DMA_Resources_t SPI4_RX_DMA = {
  &SPI4_RX_DMA_Handle,
  SPI4_RX_DMA_Stream,
  SPI4_RX_DMA_Channel,
  SPI4_RX_DMA_Priority,
  SPI4_RX_DMA_Complete,
  RTE_SPI_DMA_INT_PRIORITY,
  SPI4_RX_DMA_IRQn,
};

#endif

/* SPI4 Resources */
static SPI_RESOURCES SPI4_Resources = {
  SPI4,
  {
#ifdef USE_SPI4_MISO_Pin
      &SPI4_miso,
#else
      NULL,
#endif
#ifdef USE_SPI4_MOSI_Pin
      &SPI4_mosi,
#else
      NULL,
#endif
#ifdef USE_SPI4_SCK_Pin
      &SPI4_sck,
#else
      NULL,
#endif
#ifdef USE_SPI4_NSS_Pin
      &SPI4_nss,
#else
      NULL,
#endif
  },
  {
    &RCC->APB2ENR,
    &RCC->APB2RSTR,
    RCC_APB2ENR_SPI4EN,
    RCC_FREQ_APB2,
  },
#ifdef SPI4_TX_DMA_Stream
  &SPI4_TX_DMA,
#else
  NULL,
#endif
#ifdef SPI4_RX_DMA_Stream
  &SPI4_RX_DMA,
#else
  NULL,
#endif
  &SPI4_Info,
  &SPI4_TransferInfo,
  SPI4_IRQn
};

#endif /* USE_SPI4 */

#if defined(USE_SPI5)

/* SPI5 Information (Run-Time) */
static SPI_INFO SPI5_Info;
static SPI_TRANSFER_INFO SPI5_TransferInfo;

#ifdef USE_SPI5_MISO_Pin
  static SPI_PIN SPI5_miso = {SPI5_MISO_GPIO_PORT, SPI5_MISO_GPIO_PIN, SPI5_MISO_GPIO_FUNC};
#endif

#ifdef USE_SPI5_MOSI_Pin
  static SPI_PIN SPI5_mosi = {SPI5_MOSI_GPIO_PORT, SPI5_MOSI_GPIO_PIN, SPI5_MOSI_GPIO_FUNC};
#endif

#ifdef USE_SPI5_SCK_Pin
  static SPI_PIN SPI5_sck = {SPI5_SCK_GPIO_PORT, SPI5_SCK_GPIO_PIN, SPI5_SCK_GPIO_FUNC};
#endif

#ifdef USE_SPI5_NSS_Pin
  static SPI_PIN SPI5_nss = {SPI5_NSS_GPIO_PORT, SPI5_NSS_GPIO_PIN, SPI5_NSS_GPIO_FUNC};
#endif

#ifdef SPI5_TX_DMA_Stream

static DMA_Handle_t SPI5_TX_DMA_Handle;
static DMA_Resources_t SPI5_TX_DMA = {
  &SPI5_TX_DMA_Handle,
  SPI5_TX_DMA_Stream,
  SPI5_TX_DMA_Channel,
  SPI5_TX_DMA_Priority,
  SPI5_TX_DMA_Complete,
  RTE_SPI_DMA_INT_PRIORITY,
  SPI5_TX_DMA_IRQn,
};

#endif

#ifdef SPI5_RX_DMA_Stream

static DMA_Handle_t SPI5_RX_DMA_Handle;
static DMA_Resources_t SPI5_RX_DMA = {
  &SPI5_RX_DMA_Handle,
  SPI5_RX_DMA_Stream,
  SPI5_RX_DMA_Channel,
  SPI5_RX_DMA_Priority,
  SPI5_RX_DMA_Complete,
  RTE_SPI_DMA_INT_PRIORITY,
  SPI5_RX_DMA_IRQn,
};

#endif

/* SPI5 Resources */
static SPI_RESOURCES SPI5_Resources = {
  SPI5,
  {
#ifdef USE_SPI5_MISO_Pin
      &SPI5_miso,
#else
      NULL,
#endif
#ifdef USE_SPI5_MOSI_Pin
      &SPI5_mosi,
#else
      NULL,
#endif
#ifdef USE_SPI5_SCK_Pin
      &SPI5_sck,
#else
      NULL,
#endif
#ifdef USE_SPI5_NSS_Pin
      &SPI5_nss,
#else
      NULL,
#endif
  },
  {
    &RCC->APB2ENR,
    &RCC->APB2RSTR,
    RCC_APB2ENR_SPI5EN,
    RCC_FREQ_APB2,
  },
#ifdef SPI5_TX_DMA_Stream
  &SPI5_TX_DMA,
#else
  NULL,
#endif
#ifdef SPI5_RX_DMA_Stream
  &SPI5_RX_DMA,
#else
  NULL,
#endif
  &SPI5_Info,
  &SPI5_TransferInfo,
  SPI5_IRQn
};

#endif /* USE_SPI5 */

#if defined(USE_SPI6)

/* SPI6 Information (Run-Time) */
static SPI_INFO SPI6_Info;
static SPI_TRANSFER_INFO SPI6_TransferInfo;

#ifdef USE_SPI6_MISO_Pin
  static SPI_PIN SPI6_miso = {SPI6_MISO_GPIO_PORT, SPI6_MISO_GPIO_PIN, SPI6_MISO_GPIO_FUNC};
#endif

#ifdef USE_SPI6_MOSI_Pin
  static SPI_PIN SPI6_mosi = {SPI6_MOSI_GPIO_PORT, SPI6_MOSI_GPIO_PIN, SPI6_MOSI_GPIO_FUNC};
#endif

#ifdef USE_SPI6_SCK_Pin
  static SPI_PIN SPI6_sck = {SPI6_SCK_GPIO_PORT, SPI6_SCK_GPIO_PIN, SPI6_SCK_GPIO_FUNC};
#endif

#ifdef USE_SPI6_NSS_Pin
  static SPI_PIN SPI6_nss = {SPI6_NSS_GPIO_PORT, SPI6_NSS_GPIO_PIN, SPI6_NSS_GPIO_FUNC};
#endif

#ifdef SPI6_TX_DMA_Stream

static DMA_Handle_t SPI6_TX_DMA_Handle;
static DMA_Resources_t SPI6_TX_DMA = {
  &SPI6_TX_DMA_Handle,
  SPI6_TX_DMA_Stream,
  SPI6_TX_DMA_Channel,
  SPI6_TX_DMA_Priority,
  SPI6_TX_DMA_Complete,
  RTE_SPI_DMA_INT_PRIORITY,
  SPI6_TX_DMA_IRQn,
};

#endif

#ifdef SPI6_RX_DMA_Stream

static DMA_Handle_t SPI6_RX_DMA_Handle;
static DMA_Resources_t SPI6_RX_DMA = {
  &SPI6_RX_DMA_Handle,
  SPI6_RX_DMA_Stream,
  SPI6_RX_DMA_Channel,
  SPI6_RX_DMA_Priority,
  SPI6_RX_DMA_Complete,
  RTE_SPI_DMA_INT_PRIORITY,
  SPI6_RX_DMA_IRQn,
};

#endif

/* SPI6 Resources */
static SPI_RESOURCES SPI6_Resources = {
  SPI6,
  {
#ifdef USE_SPI6_MISO_Pin
      &SPI6_miso,
#else
      NULL,
#endif
#ifdef USE_SPI6_MOSI_Pin
      &SPI6_mosi,
#else
      NULL,
#endif
#ifdef USE_SPI6_SCK_Pin
      &SPI6_sck,
#else
      NULL,
#endif
#ifdef USE_SPI6_NSS_Pin
      &SPI6_nss,
#else
      NULL,
#endif
  },
  {
    &RCC->APB2ENR,
    &RCC->APB2RSTR,
    RCC_APB2ENR_SPI6EN,
    RCC_FREQ_APB2,
  },
#ifdef SPI6_TX_DMA_Stream
  &SPI6_TX_DMA,
#else
  NULL,
#endif
#ifdef SPI6_RX_DMA_Stream
  &SPI6_RX_DMA,
#else
  NULL,
#endif
  &SPI6_Info,
  &SPI6_TransferInfo,
  SPI6_IRQn
};

#endif /* USE_SPI6 */

/*******************************************************************************
 *  function implementations (scope: module-local)
 ******************************************************************************/

/**
 * @fn          void PinConfig(const SPI_PIN *io, const GPIO_PIN_CFG_t *pin_cfg)
 * @brief       Configure Pin
 * @param[in]   io       Pointer to SPI_PIN
 * @param[in]   pin_cfg  Pointer to GPIO_PIN_CFG_t
 */
static
void PinConfig(const SPI_PIN *io, const GPIO_PIN_CFG_t *pin_cfg)
{
  if ((io == NULL) || (pin_cfg == NULL))
    return;

  if (GPIO_GetPortClockState(io->port) == false)
    GPIO_PortClock(io->port, GPIO_PORT_CLK_ENABLE);

  GPIO_PinConfig(io->port, io->pin, pin_cfg);

  if (pin_cfg->mode == GPIO_MODE_AF_PP || pin_cfg->mode == GPIO_MODE_AF_OD)
    GPIO_AFConfig(io->port, io->pin, io->func);
}

/**
 * @fn          bool isData16bit(const SPI_INFO *info)
 * @brief
 * @param[in]   info
 * @return
 */
bool isData16bit(const SPI_INFO *info)
{
  return ((info->mode & ARM_SPI_DATA_BITS_Msk) == ARM_SPI_DATA_BITS(16U));
}

/**
 * @fn          uint32_t SPI_TxData(SPI_TypeDef *reg, SPI_INFO *info, SPI_TRANSFER_INFO *xfer)
 * @brief       SPI Tx Data function
 * @param[out]  reg
 * @param[in]   info
 * @param[out]  xfer
 * @return
 */
static
uint32_t SPI_TxData(SPI_TypeDef *reg, const SPI_INFO *info, SPI_TRANSFER_INFO *xfer)
{
  uint32_t event = ARM_SPI_EVENT_DATA_LOST;
  uint16_t value;

  if (xfer->tx_cnt < xfer->num) {
    if (xfer->tx_buf != NULL) {
      value = (uint16_t)(*xfer->tx_buf++);
      if (isData16bit(info)) {
        value |= (uint16_t)(*xfer->tx_buf++ << 8U);
      }
    }
    else {
      value = xfer->def_val;
    }

    /* Write data to data register */
    reg->DR = value;

    xfer->tx_cnt++;
    event = 0U;
  }

  return event;
}

/**
 * @fn          uint32_t SPI_RxData(SPI_TypeDef *reg, const SPI_INFO *info, SPI_TRANSFER_INFO *xfer)
 * @brief       SPI Rx Data function
 * @param[out]  reg
 * @param[in]   info
 * @param[out]  xfer
 * @return
 */
static
uint32_t SPI_RxData(SPI_TypeDef *reg, const SPI_INFO *info, SPI_TRANSFER_INFO *xfer)
{
  uint32_t event = ARM_SPI_EVENT_DATA_LOST;
  uint16_t value;

  if (xfer->rx_cnt < xfer->num) {
    value = reg->DR;

    if (xfer->rx_buf != NULL) {
      *xfer->rx_buf++ = (uint8_t)value;

      if (isData16bit(info)) {
        *xfer->rx_buf++ = (uint8_t)(value >> 8U);
      }
    }

    xfer->rx_cnt++;

    if (xfer->rx_cnt == xfer->num) {
      /* Transfer completed */
      event = ARM_SPI_EVENT_TRANSFER_COMPLETE;
    }
    else {
      event = 0U;
    }
  }

  return event;
}

/**
 * @fn          ARM_DRIVER_VERSION SPIx_GetVersion(void)
 * @brief       Get driver version.
 * @return      \ref ARM_DRIVER_VERSION
 */
static
ARM_DRIVER_VERSION SPIx_GetVersion(void)
{
  return DriverVersion;
}

/**
 * @fn          ARM_SPI_CAPABILITIES SPIx_GetCapabilities(void)
 * @brief       Get driver capabilities.
 * @return      \ref ARM_SPI_CAPABILITIES
 */
static
ARM_SPI_CAPABILITIES SPIx_GetCapabilities(void)
{
  return DriverCapabilities;
}

/**
 * @fn          int32_t SPI_Initialize(ARM_SPI_SignalEvent_t cb_event, SPI_RESOURCES *spi)
 * @brief       Initialize SPI Interface.
 * @param[in]   cb_event  Pointer to \ref ARM_SPI_SignalEvent
 * @param[in]   spi       Pointer to SPI resources
 * @return      \ref execution_status
 */
static
int32_t SPI_Initialize(ARM_SPI_SignalEvent_t cb_event, SPI_RESOURCES *spi)
{
  SPI_INFO *info = spi->info;

  if (info->state & SPI_INITIALIZED)
    return ARM_DRIVER_OK;

  /* Initialize SPI Run-Time Resources */
  info->cb_event          = cb_event;
  info->status.busy       = 0U;
  info->status.data_lost  = 0U;
  info->status.mode_fault = 0U;

  /* Clear transfer information */
  memset((void *)spi->xfer, 0, sizeof(SPI_TRANSFER_INFO));

  SPI_IO *io = &spi->io;
  const GPIO_PIN_CFG_t *pin_cfg = &SPI_pin_cfg_af;

  /* Configure MISO Pin */
  PinConfig(io->miso, pin_cfg);
  /* Configure MOSI Pin */
  PinConfig(io->mosi, pin_cfg);
  /* Configure SCK Pin */
  PinConfig(io->sck, pin_cfg);
  /* Configure NSS Pin */
  PinConfig(io->nss, pin_cfg);

  info->state = SPI_INITIALIZED;

  return ARM_DRIVER_OK;
}

/**
 * @fn          int32_t SPI_Uninitialize(SPI_RESOURCES *spi)
 * @brief       De-initialize SPI Interface.
 * @param[in]   spi  Pointer to SPI resources
 * @return      \ref execution_status
 */
static
int32_t SPI_Uninitialize(SPI_RESOURCES *spi)
{
  SPI_IO *io = &spi->io;
  const GPIO_PIN_CFG_t *pin_cfg = &SPI_pin_cfg_analog;

  /* Unconfigure MISO Pin */
  PinConfig(io->miso, pin_cfg);
  /* Unconfigure MOSI Pin */
  PinConfig(io->mosi, pin_cfg);
  /* Unconfigure SCK Pin */
  PinConfig(io->sck, pin_cfg);
  /* Unconfigure NSS Pin */
  PinConfig(io->nss, pin_cfg);

  /* Clear SPI state */
  spi->info->state = 0U;

  return ARM_DRIVER_OK;
}

/**
 * @fn          int32_t SPI_PowerControl(ARM_POWER_STATE state, SPI_RESOURCES *spi)
 * @brief       Control SPI Interface Power.
 * @param[in]   state  Power state
 * @param[in]   spi    Pointer to SPI resources
 * @return      \ref  execution_status
 */
static
int32_t SPI_PowerControl(ARM_POWER_STATE state, SPI_RESOURCES *spi)
{
  SPI_INFO *info = spi->info;

  switch (state) {
    case ARM_POWER_OFF:
      /* SPI peripheral reset */
      RCC_ResetPeriph(&spi->rcc);
      /* Disable SPI IRQ */
      NVIC_DisableIRQ(spi->irq_num);

      /* Uninitialize DMA */
#ifdef SPI_DMA
      if (spi->tx_dma != NULL)
        DMA_Uninitialize(spi->tx_dma);

      if (spi->rx_dma != NULL)
        DMA_Uninitialize(spi->rx_dma);
#endif

      /* Disable peripheral clock */
      RCC_DisablePeriph(&spi->rcc);

      /* Clear status flags */
      info->status.busy       = 0U;
      info->status.data_lost  = 0U;
      info->status.mode_fault = 0U;

      /* Clear powered flag */
      info->state &= ~((uint32_t)SPI_POWERED);
      break;

    case ARM_POWER_FULL:
      if ((info->state & SPI_INITIALIZED) == 0U)
        return ARM_DRIVER_ERROR;

      if ((info->state & SPI_POWERED) != 0U)
        return ARM_DRIVER_OK;

      /* Clear status flags */
      info->status.busy       = 0U;
      info->status.data_lost  = 0U;
      info->status.mode_fault = 0U;

      spi->xfer->def_val      = 0U;

      /* Ready for operation - set powered flag */
      info->state |= SPI_POWERED;

      /* Enable the peripheral clock */
      RCC_EnablePeriph(&spi->rcc);

      /* Clear and Enable SPI IRQ */
      NVIC_ClearPendingIRQ(spi->irq_num);
      NVIC_SetPriority(spi->irq_num, RTE_SPI_INT_PRIORITY);
      NVIC_EnableIRQ(spi->irq_num);

      /* Initialize DMA */
#ifdef SPI_DMA
      if (spi->tx_dma != NULL) {
        DMA_StreamConfig_t *cfg = &spi->tx_dma->handle->config;

        cfg->Channel   = spi->tx_dma->channel;
        cfg->Direction = DMA_DIR_MEM_TO_PER;
        cfg->Mode      = DMA_MODE_NORMAL;
        cfg->Priority  = spi->tx_dma->priority;
        cfg->FIFOMode  = DMA_FIFOMODE_DISABLE;
        cfg->MemBurst  = DMA_MBURST_SINGLE;
        cfg->PerBurst  = DMA_PBURST_SINGLE;

        DMA_Initialize(spi->tx_dma);
      }

      if (spi->rx_dma != NULL) {
        DMA_StreamConfig_t *cfg = &spi->rx_dma->handle->config;

        cfg->Channel   = spi->rx_dma->channel;
        cfg->Direction = DMA_DIR_PER_TO_MEM;
        cfg->Mode      = DMA_MODE_NORMAL;
        cfg->Priority  = spi->rx_dma->priority;
        cfg->FIFOMode  = DMA_FIFOMODE_DISABLE;
        cfg->MemBurst  = DMA_MBURST_SINGLE;
        cfg->PerBurst  = DMA_PBURST_SINGLE;

        DMA_Initialize(spi->rx_dma);
      }
#endif

      /* Reset the peripheral */
      RCC_ResetPeriph(&spi->rcc);
      break;

    default:
      return ARM_DRIVER_ERROR_UNSUPPORTED;
  }

  return ARM_DRIVER_OK;
}

/**
 * @fn          int32_t SPI_Send(const void *data, uint32_t num, SPI_RESOURCES *spi)
 * @brief       Start sending data to SPI transmitter.
 * @param[in]   data  Pointer to buffer with data to send to SPI transmitter
 * @param[in]   num   Number of data items to send
 * @param[in]   spi   Pointer to SPI resources
 * @return      \ref  execution_status
 */
static
int32_t SPI_Send(const void *data, uint32_t num, SPI_RESOURCES *spi)
{
  SPI_INFO *info = spi->info;
  SPI_TRANSFER_INFO *xfer = spi->xfer;
  SPI_TypeDef *reg = spi->reg;

  if ((data == NULL) || (num == 0U))
    return ARM_DRIVER_ERROR_PARAMETER;

  if ((info->state & SPI_CONFIGURED) == 0U)
    return ARM_DRIVER_ERROR;

  if (info->status.busy)
    return ARM_DRIVER_ERROR_BUSY;

  /* Check if transmit pin available */
  if ((((spi->io.mosi != NULL) && ((info->mode & ARM_SPI_CONTROL_Msk) == ARM_SPI_MODE_MASTER)) ||
       ((spi->io.miso != NULL) && ((info->mode & ARM_SPI_CONTROL_Msk) == ARM_SPI_MODE_SLAVE ))) == 0U)
    return ARM_DRIVER_ERROR;

  /* Update SPI statuses */
  info->status.busy       = 1U;
  info->status.data_lost  = 0U;
  info->status.mode_fault = 0U;

  /* Save transfer info */
  xfer->rx_buf = NULL;
  xfer->tx_buf = (uint8_t *)((uint32_t)data);
  xfer->num    = num;
  xfer->rx_cnt = 0U;
  xfer->tx_cnt = 0U;

#ifdef SPI_DMA_RX
  /* DMA mode */
  if (spi->rx_dma != NULL) {
    /* Prepare DMA to receive dummy RX data */
    DMA_StreamConfig_t *cfg = &spi->rx_dma->handle->config;

    cfg->MemInc = DMA_MINC_DISABLE;
    cfg->PerInc = DMA_PINC_DISABLE;

    if (isData16bit(info)) {
      /* 16 - bit data frame */
      cfg->MemDataAlign = DMA_MDATAALIGN_HALFWORD;
      cfg->PerDataAlign = DMA_PDATAALIGN_HALFWORD;
    }
    else {
      /* 8 - bit data frame */
      cfg->MemDataAlign = DMA_MDATAALIGN_BYTE;
      cfg->PerDataAlign = DMA_PDATAALIGN_BYTE;
    }

    /* Initialize and start DMA Stream */
    DMA_StreamConfig(spi->rx_dma);
    DMA_StreamEnable(spi->rx_dma, (uint32_t)&reg->DR, (uint32_t)&xfer->dump_val, num);
    /* RX Buffer DMA enable */
    reg->CR2 |= SPI_CR2_RXDMAEN;
  }
#endif

#ifdef SPI_DMA_TX
  /* DMA mode */
  if (spi->tx_dma != NULL) {
    /* Prepare DMA to send TX data */
    DMA_StreamConfig_t *cfg = &spi->tx_dma->handle->config;

    cfg->MemInc = DMA_MINC_ENABLE;
    cfg->PerInc = DMA_PINC_DISABLE;

    if (isData16bit(info)) {
      /* 16 - bit data frame */
      cfg->MemDataAlign = DMA_MDATAALIGN_HALFWORD;
      cfg->PerDataAlign = DMA_PDATAALIGN_HALFWORD;
    }
    else {
      /* 8 - bit data frame */
      cfg->MemDataAlign = DMA_MDATAALIGN_BYTE;
      cfg->PerDataAlign = DMA_PDATAALIGN_BYTE;
    }

    /* Initialize and start DMA Stream */
    DMA_StreamConfig(spi->tx_dma);
    DMA_StreamEnable(spi->tx_dma, (uint32_t)&reg->DR, (uint32_t)data, num);
    /* TX Buffer DMA enable */
    reg->CR2 |= SPI_CR2_TXDMAEN;
  }
  else
#endif
  {
    // Interrupt mode
    SPI_TxData(reg, info, xfer);
  }

  return ARM_DRIVER_OK;
}

/**
 * @fn          int32_t SPI_Receive(void *data, uint32_t num, SPI_RESOURCES *spi)
 * @brief       Start receiving data from SPI receiver.
 * @param[out]  data  Pointer to buffer for data to receive from SPI receiver
 * @param[in]   num   Number of data items to receive
 * @param[in]   spi   Pointer to SPI resources
 * @return      \ref  execution_status
 */
static
int32_t SPI_Receive(void *data, uint32_t num, SPI_RESOURCES *spi)
{
  SPI_INFO *info = spi->info;
  SPI_TRANSFER_INFO *xfer = spi->xfer;
  SPI_TypeDef *reg = spi->reg;

  if ((data == NULL) || (num == 0U))
    return ARM_DRIVER_ERROR_PARAMETER;

  if ((info->state & SPI_CONFIGURED) == 0U)
    return ARM_DRIVER_ERROR;

  if (info->status.busy)
    return ARM_DRIVER_ERROR_BUSY;

  // Check if receive pin available
  if ((((spi->io.miso != NULL) && ((info->mode & ARM_SPI_CONTROL_Msk) == ARM_SPI_MODE_MASTER)) ||
       ((spi->io.mosi != NULL) && ((info->mode & ARM_SPI_CONTROL_Msk) == ARM_SPI_MODE_SLAVE))) == 0U)
  {
    return ARM_DRIVER_ERROR;
  }

  // Update SPI statuses
  info->status.busy       = 1U;
  info->status.data_lost  = 0U;
  info->status.mode_fault = 0U;

  // Save transfer info
  xfer->rx_buf = (uint8_t *)data;
  xfer->tx_buf = NULL;
  xfer->num    = num;
  xfer->rx_cnt = 0U;
  xfer->tx_cnt = 0U;

#ifdef SPI_DMA_RX
  /* DMA mode */
  if (spi->rx_dma != NULL) {
    /* Prepare DMA to receive dummy RX data */
    DMA_StreamConfig_t *cfg = &spi->rx_dma->handle->config;

    cfg->MemInc = DMA_MINC_ENABLE;
    cfg->PerInc = DMA_PINC_DISABLE;

    if (isData16bit(info)) {
      /* 16 - bit data frame */
      cfg->MemDataAlign = DMA_MDATAALIGN_HALFWORD;
      cfg->PerDataAlign = DMA_PDATAALIGN_HALFWORD;
    }
    else {
      /* 8 - bit data frame */
      cfg->MemDataAlign = DMA_MDATAALIGN_BYTE;
      cfg->PerDataAlign = DMA_PDATAALIGN_BYTE;
    }

    /* Initialize and start DMA Stream */
    DMA_StreamConfig(spi->rx_dma);
    DMA_StreamEnable(spi->rx_dma, (uint32_t)&reg->DR, (uint32_t)data, num);
    /* RX Buffer DMA enable */
    reg->CR2 |= SPI_CR2_RXDMAEN;
  }
#endif

#ifdef SPI_DMA_TX
  /* DMA mode */
  if (spi->tx_dma != NULL) {
    /* Prepare DMA to send TX data */
    DMA_StreamConfig_t *cfg = &spi->tx_dma->handle->config;

    cfg->MemInc = DMA_MINC_DISABLE;
    cfg->PerInc = DMA_PINC_DISABLE;

    if (isData16bit(info)) {
      /* 16 - bit data frame */
      cfg->MemDataAlign = DMA_MDATAALIGN_HALFWORD;
      cfg->PerDataAlign = DMA_PDATAALIGN_HALFWORD;
    }
    else {
      /* 8 - bit data frame */
      cfg->MemDataAlign = DMA_MDATAALIGN_BYTE;
      cfg->PerDataAlign = DMA_PDATAALIGN_BYTE;
    }

    /* Initialize and start DMA Stream */
    DMA_StreamConfig(spi->tx_dma);
    DMA_StreamEnable(spi->tx_dma, (uint32_t)&reg->DR, (uint32_t)&xfer->def_val, num);
    /* TX Buffer DMA enable */
    reg->CR2 |= SPI_CR2_TXDMAEN;
  }
  else
#endif
  {
    // Interrupt mode
    SPI_TxData(reg, info, xfer);
  }

  return ARM_DRIVER_OK;
}

/**
 * @fn          int32_t SPI_Transfer(const void *data_out, void *data_in, uint32_t num, SPI_RESOURCES *spi)
 * @brief       Start sending/receiving data to/from SPI transmitter/receiver.
 * @param[in]   data_out  Pointer to buffer with data to send to SPI transmitter
 * @param[out]  data_in   Pointer to buffer for data to receive from SPI receiver
 * @param[in]   num       Number of data items to transfer
 * @param[in]   spi       Pointer to SPI resources
 * @return      \ref execution_status
 */
static
int32_t SPI_Transfer(const void *data_out, void *data_in, uint32_t num, SPI_RESOURCES *spi)
{
  SPI_INFO *info = spi->info;
  SPI_TRANSFER_INFO *xfer = spi->xfer;
  SPI_TypeDef *reg = spi->reg;

  if ((data_out == NULL) || (data_in == NULL) || (num == 0U))
    return ARM_DRIVER_ERROR_PARAMETER;

  if ((info->state & SPI_CONFIGURED) == 0U)
    return ARM_DRIVER_ERROR;

  if (info->status.busy)
    return ARM_DRIVER_ERROR_BUSY;

  // Check if receive and transmit pins available
  if ((spi->io.miso == NULL) || (spi->io.mosi == NULL))
    return ARM_DRIVER_ERROR;

  // Update SPI statuses
  info->status.busy       = 1U;
  info->status.data_lost  = 0U;
  info->status.mode_fault = 0U;

  // Save transfer info
  xfer->rx_buf = (uint8_t *)((uint32_t)data_in);
  xfer->tx_buf = (uint8_t *)((uint32_t)data_out);
  xfer->num    = num;
  xfer->rx_cnt = 0U;
  xfer->tx_cnt = 0U;

#ifdef SPI_DMA
  if ((spi->rx_dma != NULL) || (spi->tx_dma != NULL)) {
    /* DMA mode */
    if (spi->rx_dma != NULL) {
      /* Prepare DMA to receive dummy RX data */
      DMA_StreamConfig_t *cfg = &spi->rx_dma->handle->config;

      cfg->MemInc = DMA_MINC_ENABLE;
      cfg->PerInc = DMA_PINC_DISABLE;

      if (isData16bit(info)) {
        /* 16 - bit data frame */
        cfg->MemDataAlign = DMA_MDATAALIGN_HALFWORD;
        cfg->PerDataAlign = DMA_PDATAALIGN_HALFWORD;
      }
      else {
        /* 8 - bit data frame */
        cfg->MemDataAlign = DMA_MDATAALIGN_BYTE;
        cfg->PerDataAlign = DMA_PDATAALIGN_BYTE;
      }

      /* Initialize and start DMA Stream */
      DMA_StreamConfig(spi->rx_dma);
      DMA_StreamEnable(spi->rx_dma, (uint32_t)&reg->DR, (uint32_t)data_in, num);
      /* RX Buffer DMA enable */
      reg->CR2 |= SPI_CR2_RXDMAEN;
    }

    if (spi->tx_dma != NULL) {
      /* Prepare DMA to send TX data */
      DMA_StreamConfig_t *cfg = &spi->tx_dma->handle->config;

      cfg->MemInc = DMA_MINC_ENABLE;
      cfg->PerInc = DMA_PINC_DISABLE;

      if (isData16bit(info)) {
        /* 16 - bit data frame */
        cfg->MemDataAlign = DMA_MDATAALIGN_HALFWORD;
        cfg->PerDataAlign = DMA_PDATAALIGN_HALFWORD;
      }
      else {
        /* 8 - bit data frame */
        cfg->MemDataAlign = DMA_MDATAALIGN_BYTE;
        cfg->PerDataAlign = DMA_PDATAALIGN_BYTE;
      }

      /* Initialize and start DMA Stream */
      DMA_StreamConfig(spi->tx_dma);
      DMA_StreamEnable(spi->tx_dma, (uint32_t)&reg->DR, (uint32_t)data_out, num);
      /* TX Buffer DMA enable */
      reg->CR2 |= SPI_CR2_TXDMAEN;
    }
  }
  else
#endif
  {
    // Interrupt mode
    SPI_TxData(reg, info, xfer);
  }

  return ARM_DRIVER_OK;
}

/**
 * @fn          uint32_t SPI_GetDataCount(SPI_RESOURCES *spi)
 * @brief       Get transferred data count.
 * @param[in]   spi  Pointer to SPI resources
 * @return      Number of data items transferred
 */
static
uint32_t SPI_GetDataCount(SPI_RESOURCES *spi)
{
  return spi->xfer->rx_cnt;
}

/**
 * @fn          int32_t SPI_Control(uint32_t control, uint32_t arg, SPI_RESOURCES *spi)
 * @brief       Control SPI Interface.
 * @param[in]   control  Operation
 * @param[in]   arg      Argument of operation (optional)
 * @param[in]   spi      Pointer to SPI resources
 * @return      \ref execution_status
 */
static
int32_t SPI_Control(uint32_t control, uint32_t arg, SPI_RESOURCES *spi)
{
  uint32_t mode, val, pclk;
  uint32_t cr1, cr2;
  SPI_INFO *info = spi->info;
  SPI_TypeDef *reg = spi->reg;

  mode  = 0U;
  val   = 0U;
  cr1   = 0U;
  cr2   = 0U;

  if ((info->state & SPI_POWERED) == 0U)
    return ARM_DRIVER_ERROR;

  if ((control & ARM_SPI_CONTROL_Msk) == ARM_SPI_ABORT_TRANSFER) {
    // Send abort
    if (spi->tx_dma != NULL) {
      // DMA mode
      // TX buffer DMA disable
      reg->CR2 &= ~SPI_CR2_TXDMAEN;

      // Abort TX DMA transfer
      DMA_StreamDisable(spi->tx_dma);
    }

    // Receive abort
    if (spi->rx_dma != NULL) {
      // DMA mode
      // RX buffer DMA disable
      reg->CR2 &= ~SPI_CR2_RXDMAEN;

      // Abort RX DMA transfer
      DMA_StreamDisable(spi->rx_dma);
    }
    else {
      // Interrupt mode
      // Disable RX buffer not empty interrupt
      reg->CR2 &= ~SPI_CR2_RXNEIE;
    }

    memset((void *)spi->xfer, 0, sizeof(SPI_TRANSFER_INFO));
    info->status.busy = 0U;

    if (spi->rx_dma == NULL) {
      //Re-enable RX buffer Not Empty interrupt
      reg->CR2 |= SPI_CR2_RXNEIE;
    }

    return ARM_DRIVER_OK;
  }

  // Check for busy flag
  if (info->status.busy)
    return ARM_DRIVER_ERROR_BUSY;

  switch (control & ARM_SPI_CONTROL_Msk) {
    case ARM_SPI_MODE_INACTIVE:
      mode |= ARM_SPI_MODE_INACTIVE;
      break;

    case ARM_SPI_MODE_MASTER:
      mode |= ARM_SPI_MODE_MASTER;

      // Master enabled
      cr1 |= SPI_CR1_MSTR;
      break;

    case ARM_SPI_MODE_SLAVE:
      mode |= ARM_SPI_MODE_SLAVE;
      break;

    case ARM_SPI_MODE_MASTER_SIMPLEX:
    case ARM_SPI_MODE_SLAVE_SIMPLEX:
      return ARM_SPI_ERROR_MODE;

    case ARM_SPI_SET_BUS_SPEED:
      // Set SPI Bus Speed
      pclk = RCC_GetFreq(spi->rcc.freq_domain);
      for (val = 0U; val < 8U; val++) {
        if (arg >= (pclk >> (val + 1U))) {
          break;
        }
      }
      if ((val == 8U) || (arg < (pclk >> (val + 1U)))) {
        // Requested Bus Speed can not be configured
        return ARM_DRIVER_ERROR;
      }
      // Disable SPI, update prescaler and enable SPI
      reg->CR1 &= ~SPI_CR1_SPE;
      reg->CR1 = (reg->CR1 & ~SPI_CR1_BR) | (val << 3U);
      reg->CR1 |= SPI_CR1_SPE;
      return ARM_DRIVER_OK;

    case ARM_SPI_GET_BUS_SPEED:
      // Return current bus speed
      return (int32_t)(RCC_GetFreq(spi->rcc.freq_domain) >> (((reg->CR1 & SPI_CR1_BR) >> 3U) + 1U));

    case ARM_SPI_SET_DEFAULT_TX_VALUE:
      spi->xfer->def_val = (uint16_t)(arg & 0xFFFFU);
      return ARM_DRIVER_OK;

    case ARM_SPI_CONTROL_SS:
      val = (info->mode & ARM_SPI_CONTROL_Msk);
      // Master modes
      if (val == ARM_SPI_MODE_MASTER) {
        val = info->mode & ARM_SPI_SS_MASTER_MODE_Msk;
        // Check if NSS pin is available and
        // software slave select master is selected
        if ((spi->io.nss != NULL) && (val == ARM_SPI_SS_MASTER_SW)) {
          SPI_PIN *io = spi->io.nss;
          // Set/Clear NSS pin
          if (arg == ARM_SPI_SS_INACTIVE)
            GPIO_PinWrite(io->port, io->pin, GPIO_PIN_OUT_HIGH);
          else
            GPIO_PinWrite(io->port, io->pin, GPIO_PIN_OUT_LOW);
        }
        else {
          return ARM_DRIVER_ERROR;
        }

        return ARM_DRIVER_OK;
      }
      // Slave modes
      else if (val == ARM_SPI_MODE_SLAVE) {
        val = info->mode & ARM_SPI_SS_SLAVE_MODE_Msk;
        // Check if slave select slave mode is selected
        if (val == ARM_SPI_SS_MASTER_SW) {
          if (arg == ARM_SPI_SS_ACTIVE) {
            reg->CR1 |= SPI_CR1_SSI;
          }
          else {
            reg->CR1 &= ~SPI_CR1_SSI;
          }
          return ARM_DRIVER_OK;
        }
        else {
          return ARM_DRIVER_ERROR;
        }
      }
      else {
        return ARM_DRIVER_ERROR;
      }

    default:
      return ARM_DRIVER_ERROR_UNSUPPORTED;
  }

  // Frame format:
  switch (control & ARM_SPI_FRAME_FORMAT_Msk) {
    case ARM_SPI_CPOL0_CPHA0:
      break;
    case ARM_SPI_CPOL0_CPHA1:
      cr1 |= SPI_CR1_CPHA;
      break;
    case ARM_SPI_CPOL1_CPHA0:
      cr1 |= SPI_CR1_CPOL;
      break;
    case ARM_SPI_CPOL1_CPHA1:
      cr1 |= SPI_CR1_CPHA | SPI_CR1_CPOL;
      break;
    case ARM_SPI_TI_SSI:
      cr1 |= SPI_CR2_FRF;
      break;
    default:
      return ARM_SPI_ERROR_FRAME_FORMAT;
  }

  // Data Bits
  switch (control & ARM_SPI_DATA_BITS_Msk) {
    case ARM_SPI_DATA_BITS(8U):
      mode |= ARM_SPI_DATA_BITS(8U);
      break;
    case ARM_SPI_DATA_BITS(16U):
      mode |= ARM_SPI_DATA_BITS(16U);
      cr1 |= SPI_CR1_DFF;
      break;
    default:
      return ARM_SPI_ERROR_DATA_BITS;
  }

  // Bit order
  if ((control & ARM_SPI_BIT_ORDER_Msk) == ARM_SPI_LSB_MSB) {
    cr1 |= SPI_CR1_LSBFIRST;
  }

  // Slave select master modes
  if ((mode & ARM_SPI_CONTROL_Msk) == ARM_SPI_MODE_MASTER) {
    switch (control & ARM_SPI_SS_MASTER_MODE_Msk) {
      case ARM_SPI_SS_MASTER_UNUSED:
        if (spi->io.nss != NULL) {
          // Unconfigure NSS pin
          PinConfig(spi->io.nss, &SPI_pin_cfg_analog);
        }
        // Software slave management
        // Internal NSS always active, IO value is ignored
        cr1 |= SPI_CR1_SSM | SPI_CR1_SSI;
        mode |= ARM_SPI_SS_MASTER_UNUSED;
        break;

      case ARM_SPI_SS_MASTER_HW_INPUT:
        if (spi->io.nss != NULL) {
          // Configure NSS pin
          PinConfig(spi->io.nss, &SPI_pin_cfg_af);
        }
        else {
          // NSS pin is not available
          return ARM_SPI_ERROR_SS_MODE;
        }

        mode |= ARM_SPI_SS_MASTER_HW_INPUT;
        break;

      case ARM_SPI_SS_MASTER_SW:
        if (spi->io.nss != NULL) {
          // Configure NSS pin as GPIO output
          PinConfig(spi->io.nss, &SPI_pin_cfg_out_pp);

          // Software slave management
          cr1 |= SPI_CR1_SSM | SPI_CR1_SSI;

          mode |= ARM_SPI_SS_MASTER_SW;
        }
        else {
          // NSS pin is not available
          return ARM_SPI_ERROR_SS_MODE;
        }
        break;

      case ARM_SPI_SS_MASTER_HW_OUTPUT:
        if (spi->io.nss != NULL) {
          // Configure NSS pin - SPI NSS alternative function
          PinConfig(spi->io.nss, &SPI_pin_cfg_af);

          // Slave select output enable
          cr2 |= SPI_CR2_SSOE;

          mode |= ARM_SPI_SS_MASTER_HW_OUTPUT;
        }
        else {
          // NSS pin is not available
          return ARM_SPI_ERROR_SS_MODE;
        }
        break;
      default:
        return ARM_SPI_ERROR_SS_MODE;
    }
  }
  // Slave select slave modes
  else if ((mode & ARM_SPI_CONTROL_Msk) == ARM_SPI_MODE_SLAVE) {
    switch (control & ARM_SPI_SS_SLAVE_MODE_Msk) {
      case ARM_SPI_SS_SLAVE_HW:
        if (spi->io.nss != NULL) {
          // Configure NSS pin - SPI NSS alternative function
          PinConfig(spi->io.nss, &SPI_pin_cfg_af);

          mode |= ARM_SPI_SS_SLAVE_HW;
        }
        else {
          // NSS pin is not available
          return ARM_SPI_ERROR_SS_MODE;
        }
        break;

      case ARM_SPI_SS_SLAVE_SW:
        if (spi->io.nss != NULL) {
          // Unconfigure NSS pin
          PinConfig(spi->io.nss, &SPI_pin_cfg_analog);
        }
        // Enable software slave management
        cr1 |= SPI_CR1_SSM;
        mode |= ARM_SPI_SS_SLAVE_SW;
        break;
      default:
        return ARM_SPI_ERROR_SS_MODE;
    }
  }

  // Set SPI Bus Speed
  pclk = RCC_GetFreq(spi->rcc.freq_domain);
  for (val = 0U; val < 8U; val++) {
    if (arg >= (pclk >> (val + 1U)))
      break;
  }
  if ((val == 8U) || (arg < (pclk >> (val + 1U)))) {
    // Requested Bus Speed can not be configured
    return ARM_DRIVER_ERROR;
  }
  // Save prescaler value
  cr1 |= (val << 3U);

  info->mode = mode;

  // Configure registers
  reg->CR1 &= ~SPI_CR1_SPE;

  if ((spi->rx_dma == NULL) && (spi->tx_dma == NULL)) {
    // Enable RX buffer Not Empty interrupt
    cr2 |= SPI_CR2_RXNEIE;
  }

  reg->CR2 = cr2 | SPI_CR2_ERRIE;
  reg->CR1 = cr1;

  if ((mode & ARM_SPI_CONTROL_Msk) == ARM_SPI_MODE_INACTIVE) {
    info->state &= ~((uint32_t)SPI_CONFIGURED);
  }
  else {
    info->state |= SPI_CONFIGURED;
  }

  // Enable SPI
  reg->CR1 |= SPI_CR1_SPE;

  return ARM_DRIVER_OK;
}

/**
 * @fn          ARM_SPI_STATUS SPI_GetStatus(SPI_RESOURCES *spi)
 * @brief       Get SPI status.
 * @param[in]   spi  Pointer to SPI resources
 * @return      SPI status \ref ARM_SPI_STATUS
 */
static
ARM_SPI_STATUS SPI_GetStatus(SPI_RESOURCES *spi)
{
  ARM_SPI_STATUS status;

  status.busy       = spi->info->status.busy;
  status.data_lost  = spi->info->status.data_lost;
  status.mode_fault = spi->info->status.mode_fault;

  return status;
}

/**
 * @fn          void SPI_IRQHandler(SPI_RESOURCES *spi)
 * @brief       SPI Interrupt handler.
 * @param[in]   spi  Pointer to SPI resources
 */
void SPI_IRQHandler(SPI_RESOURCES *spi)
{
  uint16_t value;
  uint32_t event, sr;

  SPI_INFO *info = spi->info;
  SPI_TRANSFER_INFO *xfer = spi->xfer;
  SPI_TypeDef *reg = spi->reg;

  event = 0U;

  // Save status register
  sr = reg->SR;

  if ((sr & (SPI_SR_OVR | SPI_SR_UDR | SPI_SR_MODF)) != 0U) {
    if ((sr & SPI_SR_OVR) != 0U) {
      value = reg->DR;
      if ((xfer->rx_cnt < xfer->num) && (xfer->rx_buf != NULL)) {
        *xfer->rx_buf++ = (uint8_t)value;

        if (isData16bit(info))
          *xfer->rx_buf++ = (uint8_t)(value >> 8U);
      }
      xfer->rx_cnt++;
      sr = reg->SR;

      info->status.data_lost = 1U;
      event |= ARM_SPI_EVENT_DATA_LOST;
    }

    if ((sr & SPI_SR_UDR) != 0U) {
      // Underrun flag is set
      info->status.data_lost = 1U;
      event |= ARM_SPI_EVENT_DATA_LOST;
    }

    if ((sr & SPI_SR_MODF) != 0U) {
      // Mode fault flag is set
      info->status.mode_fault = 1U;

      // Write CR1 register to clear MODF flag
      reg->CR1 = (uint32_t)reg->CR1;
      event |= ARM_SPI_EVENT_MODE_FAULT;
    }
  }
  else {
    if (((sr & SPI_SR_RXNE) != 0U) && ((reg->CR2 & SPI_CR2_RXNEIE) != 0U)) {
      // RX buffer Not Empty
      event = SPI_RxData(reg, info, xfer);

      if (event == 0U) {
        event = SPI_TxData(reg, info, xfer);
      }
      else {
        if (event == ARM_SPI_EVENT_TRANSFER_COMPLETE) {
          info->status.busy = 0U;
        }
      }
    }
  }

  // Send event
  if ((event != 0U) && ((info->cb_event != NULL))) {
    info->cb_event(event);
  }
}

#ifdef SPI_DMA_TX
static
void SPI_TX_DMA_Complete(uint32_t event, SPI_RESOURCES *spi)
{
  if (event & DMA_EVENT_TRANSFER_COMPLETE) {
    spi->xfer->tx_cnt = spi->xfer->num;
  }
}
#endif  // SPI_DMA_TX

#ifdef SPI_DMA_RX
static
void SPI_RX_DMA_Complete(uint32_t event, SPI_RESOURCES *spi)
{
  if (event & DMA_EVENT_TRANSFER_COMPLETE) {
    spi->xfer->rx_cnt = spi->xfer->num;
    spi->info->status.busy = 0U;

    if (spi->info->cb_event != NULL)
      spi->info->cb_event(ARM_SPI_EVENT_TRANSFER_COMPLETE);
  }
}
#endif  // SPI_DMA_RX

#if defined(USE_SPI1)
/* SPI1 Driver wrapper functions */

static
int32_t SPI1_Initialize(ARM_SPI_SignalEvent_t cb_event)
{
  return SPI_Initialize(cb_event, &SPI1_Resources);
}

static
int32_t SPI1_Uninitialize(void)
{
  return SPI_Uninitialize(&SPI1_Resources);
}

static
int32_t SPI1_PowerControl(ARM_POWER_STATE state)
{
  return SPI_PowerControl(state, &SPI1_Resources);
}

static
int32_t SPI1_Send(const void *data, uint32_t num)
{
  return SPI_Send(data, num, &SPI1_Resources);
}

static
int32_t SPI1_Receive(void *data, uint32_t num)
{
  return SPI_Receive(data, num, &SPI1_Resources);
}

static
int32_t SPI1_Transfer(const void *data_out, void *data_in, uint32_t num)
{
  return SPI_Transfer(data_out, data_in, num, &SPI1_Resources);
}

static
uint32_t SPI1_GetDataCount(void)
{
  return SPI_GetDataCount(&SPI1_Resources);
}

static
int32_t SPI1_Control(uint32_t control, uint32_t arg)
{
  return SPI_Control(control, arg, &SPI1_Resources);
}

static
ARM_SPI_STATUS SPI1_GetStatus(void)
{
  return SPI_GetStatus(&SPI1_Resources);
}

void SPI1_IRQHandler(void)
{
  SPI_IRQHandler(&SPI1_Resources);
}

#ifdef SPI1_TX_DMA_Stream
void SPI1_TX_DMA_Handler(void)
{
  DMA_IRQ_Handle(&SPI1_TX_DMA);
}

void SPI1_TX_DMA_Complete(uint32_t event)
{
  SPI_TX_DMA_Complete(event, &SPI1_Resources);
}
#endif // SPI1_TX_DMA_Stream

#ifdef SPI1_RX_DMA_Stream
void SPI1_RX_DMA_Handler(void)
{
  DMA_IRQ_Handle(&SPI1_RX_DMA);
}

void SPI1_RX_DMA_Complete(uint32_t event)
{
  SPI_RX_DMA_Complete(event, &SPI1_Resources);
}
#endif // SPI1_RX_DMA_Stream

#endif  /* USE_SPI1 */

#if defined(USE_SPI2)
/* SPI2 Driver wrapper functions */

static
int32_t SPI2_Initialize(ARM_SPI_SignalEvent_t cb_event)
{
  return SPI_Initialize(cb_event, &SPI2_Resources);
}

static
int32_t SPI2_Uninitialize(void)
{
  return SPI_Uninitialize(&SPI2_Resources);
}

static
int32_t SPI2_PowerControl(ARM_POWER_STATE state)
{
  return SPI_PowerControl(state, &SPI2_Resources);
}

static
int32_t SPI2_Send(const void *data, uint32_t num)
{
  return SPI_Send(data, num, &SPI2_Resources);
}

static
int32_t SPI2_Receive(void *data, uint32_t num)
{
  return SPI_Receive(data, num, &SPI2_Resources);
}

static
int32_t SPI2_Transfer(const void *data_out, void *data_in, uint32_t num)
{
  return SPI_Transfer(data_out, data_in, num, &SPI2_Resources);
}

static
uint32_t SPI2_GetDataCount(void)
{
  return SPI_GetDataCount(&SPI2_Resources);
}

static
int32_t SPI2_Control(uint32_t control, uint32_t arg)
{
  return SPI_Control(control, arg, &SPI2_Resources);
}

static
ARM_SPI_STATUS SPI2_GetStatus(void)
{
  return SPI_GetStatus(&SPI2_Resources);
}

void SPI2_IRQHandler(void)
{
  SPI_IRQHandler(&SPI2_Resources);
}

#ifdef SPI2_TX_DMA_Stream
void SPI2_TX_DMA_Handler(void)
{
  DMA_IRQ_Handle(&SPI2_TX_DMA);
}

void SPI2_TX_DMA_Complete(uint32_t event)
{
  SPI_TX_DMA_Complete(event, &SPI2_Resources);
}
#endif // SPI2_TX_DMA_Stream

#ifdef SPI2_RX_DMA_Stream
void SPI2_RX_DMA_Handler(void)
{
  DMA_IRQ_Handle(&SPI2_RX_DMA);
}

void SPI2_RX_DMA_Complete(uint32_t event)
{
  SPI_RX_DMA_Complete(event, &SPI2_Resources);
}
#endif // SPI2_RX_DMA_Stream

#endif  /* USE_SPI2 */

#if defined(USE_SPI3)
/* SPI3 Driver wrapper functions */

static
int32_t SPI3_Initialize(ARM_SPI_SignalEvent_t cb_event)
{
  return SPI_Initialize(cb_event, &SPI3_Resources);
}

static
int32_t SPI3_Uninitialize(void)
{
  return SPI_Uninitialize(&SPI3_Resources);
}

static
int32_t SPI3_PowerControl(ARM_POWER_STATE state)
{
  return SPI_PowerControl(state, &SPI3_Resources);
}

static
int32_t SPI3_Send(const void *data, uint32_t num)
{
  return SPI_Send(data, num, &SPI3_Resources);
}

static
int32_t SPI3_Receive(void *data, uint32_t num)
{
  return SPI_Receive(data, num, &SPI3_Resources);
}

static
int32_t SPI3_Transfer(const void *data_out, void *data_in, uint32_t num)
{
  return SPI_Transfer(data_out, data_in, num, &SPI3_Resources);
}

static
uint32_t SPI3_GetDataCount(void)
{
  return SPI_GetDataCount(&SPI3_Resources);
}

static
int32_t SPI3_Control(uint32_t control, uint32_t arg)
{
  return SPI_Control(control, arg, &SPI3_Resources);
}

static
ARM_SPI_STATUS SPI3_GetStatus(void)
{
  return SPI_GetStatus(&SPI3_Resources);
}

void SPI3_IRQHandler(void)
{
  SPI_IRQHandler(&SPI3_Resources);
}

#ifdef SPI3_TX_DMA_Stream
void SPI3_TX_DMA_Handler(void)
{
  DMA_IRQ_Handle(&SPI3_TX_DMA);
}

void SPI3_TX_DMA_Complete(uint32_t event)
{
  SPI_TX_DMA_Complete(event, &SPI3_Resources);
}
#endif // SPI3_TX_DMA_Stream

#ifdef SPI3_RX_DMA_Stream
void SPI3_RX_DMA_Handler(void)
{
  DMA_IRQ_Handle(&SPI3_RX_DMA);
}

void SPI3_RX_DMA_Complete(uint32_t event)
{
  SPI_RX_DMA_Complete(event, &SPI3_Resources);
}
#endif // SPI3_RX_DMA_Stream

#endif  /* USE_SPI3 */

#if defined(USE_SPI4)
/* SPI4 Driver wrapper functions */

static
int32_t SPI4_Initialize(ARM_SPI_SignalEvent_t cb_event)
{
  return SPI_Initialize(cb_event, &SPI4_Resources);
}

static
int32_t SPI4_Uninitialize(void)
{
  return SPI_Uninitialize(&SPI4_Resources);
}

static
int32_t SPI4_PowerControl(ARM_POWER_STATE state)
{
  return SPI_PowerControl(state, &SPI4_Resources);
}

static
int32_t SPI4_Send(const void *data, uint32_t num)
{
  return SPI_Send(data, num, &SPI4_Resources);
}

static
int32_t SPI4_Receive(void *data, uint32_t num)
{
  return SPI_Receive(data, num, &SPI4_Resources);
}

static
int32_t SPI4_Transfer(const void *data_out, void *data_in, uint32_t num)
{
  return SPI_Transfer(data_out, data_in, num, &SPI4_Resources);
}

static
uint32_t SPI4_GetDataCount(void)
{
  return SPI_GetDataCount(&SPI4_Resources);
}

static
int32_t SPI4_Control(uint32_t control, uint32_t arg)
{
  return SPI_Control(control, arg, &SPI4_Resources);
}

static
ARM_SPI_STATUS SPI4_GetStatus(void)
{
  return SPI_GetStatus(&SPI4_Resources);
}

void SPI4_IRQHandler(void)
{
  SPI_IRQHandler(&SPI4_Resources);
}

#ifdef SPI4_TX_DMA_Stream
void SPI4_TX_DMA_Handler(void)
{
  DMA_IRQ_Handle(&SPI4_TX_DMA);
}

void SPI4_TX_DMA_Complete(uint32_t event)
{
  SPI_TX_DMA_Complete(event, &SPI4_Resources);
}
#endif // SPI4_TX_DMA_Stream

#ifdef SPI4_RX_DMA_Stream
void SPI4_RX_DMA_Handler(void)
{
  DMA_IRQ_Handle(&SPI4_RX_DMA);
}

void SPI4_RX_DMA_Complete(uint32_t event)
{
  SPI_RX_DMA_Complete(event, &SPI4_Resources);
}
#endif // SPI4_RX_DMA_Stream

#endif  /* USE_SPI4 */

#if defined(USE_SPI5)
/* SPI5 Driver wrapper functions */

static
int32_t SPI5_Initialize(ARM_SPI_SignalEvent_t cb_event)
{
  return SPI_Initialize(cb_event, &SPI5_Resources);
}

static
int32_t SPI5_Uninitialize(void)
{
  return SPI_Uninitialize(&SPI5_Resources);
}

static
int32_t SPI5_PowerControl(ARM_POWER_STATE state)
{
  return SPI_PowerControl(state, &SPI5_Resources);
}

static
int32_t SPI5_Send(const void *data, uint32_t num)
{
  return SPI_Send(data, num, &SPI5_Resources);
}

static
int32_t SPI5_Receive(void *data, uint32_t num)
{
  return SPI_Receive(data, num, &SPI5_Resources);
}

static
int32_t SPI5_Transfer(const void *data_out, void *data_in, uint32_t num)
{
  return SPI_Transfer(data_out, data_in, num, &SPI5_Resources);
}

static
uint32_t SPI5_GetDataCount(void)
{
  return SPI_GetDataCount(&SPI5_Resources);
}

static
int32_t SPI5_Control(uint32_t control, uint32_t arg)
{
  return SPI_Control(control, arg, &SPI5_Resources);
}

static
ARM_SPI_STATUS SPI5_GetStatus(void)
{
  return SPI_GetStatus(&SPI5_Resources);
}

void SPI5_IRQHandler(void)
{
  SPI_IRQHandler(&SPI5_Resources);
}

#ifdef SPI5_TX_DMA_Stream
void SPI5_TX_DMA_Handler(void)
{
  DMA_IRQ_Handle(&SPI5_TX_DMA);
}

void SPI5_TX_DMA_Complete(uint32_t event)
{
  SPI_TX_DMA_Complete(event, &SPI5_Resources);
}
#endif // SPI5_TX_DMA_Stream

#ifdef SPI5_RX_DMA_Stream
void SPI5_RX_DMA_Handler(void)
{
  DMA_IRQ_Handle(&SPI5_RX_DMA);
}

void SPI5_RX_DMA_Complete(uint32_t event)
{
  SPI_RX_DMA_Complete(event, &SPI5_Resources);
}
#endif // SPI5_RX_DMA_Stream

#endif  /* USE_SPI5 */

#if defined(USE_SPI6)
/* SPI6 Driver wrapper functions */

static
int32_t SPI6_Initialize(ARM_SPI_SignalEvent_t cb_event)
{
  return SPI_Initialize(cb_event, &SPI6_Resources);
}

static
int32_t SPI6_Uninitialize(void)
{
  return SPI_Uninitialize(&SPI6_Resources);
}

static
int32_t SPI6_PowerControl(ARM_POWER_STATE state)
{
  return SPI_PowerControl(state, &SPI6_Resources);
}

static
int32_t SPI6_Send(const void *data, uint32_t num)
{
  return SPI_Send(data, num, &SPI6_Resources);
}

static
int32_t SPI6_Receive(void *data, uint32_t num)
{
  return SPI_Receive(data, num, &SPI6_Resources);
}

static
int32_t SPI6_Transfer(const void *data_out, void *data_in, uint32_t num)
{
  return SPI_Transfer(data_out, data_in, num, &SPI6_Resources);
}

static
uint32_t SPI6_GetDataCount(void)
{
  return SPI_GetDataCount(&SPI6_Resources);
}

static
int32_t SPI6_Control(uint32_t control, uint32_t arg)
{
  return SPI_Control(control, arg, &SPI6_Resources);
}

static
ARM_SPI_STATUS SPI6_GetStatus(void)
{
  return SPI_GetStatus(&SPI6_Resources);
}

void SPI6_IRQHandler(void)
{
  SPI_IRQHandler(&SPI6_Resources);
}

#ifdef SPI6_TX_DMA_Stream
void SPI6_TX_DMA_Handler(void)
{
  DMA_IRQ_Handle(&SPI6_TX_DMA);
}

void SPI6_TX_DMA_Complete(uint32_t event)
{
  SPI_TX_DMA_Complete(event, &SPI6_Resources);
}
#endif // SPI6_TX_DMA_Stream

#ifdef SPI6_RX_DMA_Stream
void SPI6_RX_DMA_Handler(void)
{
  DMA_IRQ_Handle(&SPI6_RX_DMA);
}

void SPI6_RX_DMA_Complete(uint32_t event)
{
  SPI_RX_DMA_Complete(event, &SPI6_Resources);
}
#endif // SPI6_RX_DMA_Stream

#endif  /* USE_SPI6 */

/*******************************************************************************
 *  global variable definitions  (scope: module-exported)
 ******************************************************************************/

#if defined(USE_SPI1)

ARM_DRIVER_SPI Driver_SPI1 = {
  SPIx_GetVersion,
  SPIx_GetCapabilities,
  SPI1_Initialize,
  SPI1_Uninitialize,
  SPI1_PowerControl,
  SPI1_Send,
  SPI1_Receive,
  SPI1_Transfer,
  SPI1_GetDataCount,
  SPI1_Control,
  SPI1_GetStatus
};

#endif  /* USE_SPI1 */

#if defined(USE_SPI2)

ARM_DRIVER_SPI Driver_SPI2 = {
  SPIx_GetVersion,
  SPIx_GetCapabilities,
  SPI2_Initialize,
  SPI2_Uninitialize,
  SPI2_PowerControl,
  SPI2_Send,
  SPI2_Receive,
  SPI2_Transfer,
  SPI2_GetDataCount,
  SPI2_Control,
  SPI2_GetStatus
};

#endif  /* USE_SPI2 */

#if defined(USE_SPI3)

ARM_DRIVER_SPI Driver_SPI3 = {
  SPIx_GetVersion,
  SPIx_GetCapabilities,
  SPI3_Initialize,
  SPI3_Uninitialize,
  SPI3_PowerControl,
  SPI3_Send,
  SPI3_Receive,
  SPI3_Transfer,
  SPI3_GetDataCount,
  SPI3_Control,
  SPI3_GetStatus
};

#endif  /* USE_SPI3 */

#if defined(USE_SPI4)

ARM_DRIVER_SPI Driver_SPI4 = {
  SPIx_GetVersion,
  SPIx_GetCapabilities,
  SPI4_Initialize,
  SPI4_Uninitialize,
  SPI4_PowerControl,
  SPI4_Send,
  SPI4_Receive,
  SPI4_Transfer,
  SPI4_GetDataCount,
  SPI4_Control,
  SPI4_GetStatus
};

#endif  /* USE_SPI4 */

#if defined(USE_SPI5)

ARM_DRIVER_SPI Driver_SPI5 = {
  SPIx_GetVersion,
  SPIx_GetCapabilities,
  SPI5_Initialize,
  SPI5_Uninitialize,
  SPI5_PowerControl,
  SPI5_Send,
  SPI5_Receive,
  SPI5_Transfer,
  SPI5_GetDataCount,
  SPI5_Control,
  SPI5_GetStatus
};

#endif  /* USE_SPI5 */

#if defined(USE_SPI6)

ARM_DRIVER_SPI Driver_SPI6 = {
  SPIx_GetVersion,
  SPIx_GetCapabilities,
  SPI6_Initialize,
  SPI6_Uninitialize,
  SPI6_PowerControl,
  SPI6_Send,
  SPI6_Receive,
  SPI6_Transfer,
  SPI6_GetDataCount,
  SPI6_Control,
  SPI6_GetStatus
};

#endif  /* USE_SPI6 */

/* ----------------------------- End of file ---------------------------------*/