/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
PinsProfile:
- !!product 'Pins v1.0'
- !!processor 'MK80FN256xxx15'
- !!package 'MK80FN256VDC15'
- !!mcu_data 'ksdk2_0'
- !!processor_version '0.1.19'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"

#define PIN0_IDX                         0u   /*!< Pin number for pin 0 in a port */
#define PIN1_IDX                         1u   /*!< Pin number for pin 1 in a port */
#define PIN2_IDX                         2u   /*!< Pin number for pin 2 in a port */
#define PIN3_IDX                         3u   /*!< Pin number for pin 3 in a port */
#define PIN4_IDX                         4u   /*!< Pin number for pin 4 in a port */
#define PIN5_IDX                         5u   /*!< Pin number for pin 5 in a port */
#define PIN6_IDX                         6u   /*!< Pin number for pin 6 in a port */
#define PIN7_IDX                         7u   /*!< Pin number for pin 7 in a port */
#define PIN8_IDX                         8u   /*!< Pin number for pin 8 in a port */
#define PIN9_IDX                         9u   /*!< Pin number for pin 9 in a port */
#define PIN10_IDX                       10u   /*!< Pin number for pin 10 in a port */
#define PIN11_IDX                       11u   /*!< Pin number for pin 11 in a port */
#define SOPT5_LPUART1RXSRC_LPUART_RX  0x00u   /*!< LPUART1 receive data source select: LPUART1_RX pin */
#define SOPT5_LPUART1TXSRC_LPUART_TX  0x00u   /*!< LPUART1 transmit data source select: LPUART1_TX pin */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
BOARD_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: B8, peripheral: LPUART1, signal: RX, pin_signal: CMP1_IN1/PTC3/LLWU_P7/SPI0_PCS1/LPUART1_RX/FTM0_CH2/CLKOUT/I2S0_TX_BCLK}
  - {pin_num: A8, peripheral: LPUART1, signal: TX, pin_signal: PTC4/LLWU_P8/SPI0_PCS0/LPUART1_TX/FTM0_CH3/FB_AD11/SDRAM_A19/CMP1_OUT}
  - {pin_num: B1, peripheral: QuadSPI0, signal: 'DATA3, A', pin_signal: PTE0/SPI1_PCS1/LPUART1_TX/SDHC0_D1/QSPI0A_DATA3/I2C1_SDA/RTC_CLKOUT}
  - {pin_num: C2, peripheral: QuadSPI0, signal: 'SCLK, A', pin_signal: PTE1/LLWU_P0/SPI1_SCK/LPUART1_RX/SDHC0_D0/QSPI0A_SCLK/I2C1_SCL/SPI1_SIN}
  - {pin_num: D2, peripheral: QuadSPI0, signal: 'DATA2, A', pin_signal: PTE3/SPI1_PCS2/LPUART1_RTS_b/SDHC0_CMD/QSPI0A_DATA2/SPI1_SOUT}
  - {pin_num: D1, peripheral: QuadSPI0, signal: 'DATA1, A', pin_signal: PTE4/LLWU_P2/SPI1_SIN/LPUART3_TX/SDHC0_D3/QSPI0A_DATA1}
  - {pin_num: C1, peripheral: QuadSPI0, signal: 'DATA0, A', pin_signal: PTE2/LLWU_P1/SPI1_SOUT/LPUART1_CTS_b/SDHC0_DCLK/QSPI0A_DATA0/SPI1_SCK}
  - {pin_num: E2, peripheral: QuadSPI0, signal: 'SS0, A', pin_signal: PTE5/SPI1_PCS0/LPUART3_RX/SDHC0_D2/QSPI0A_SS0_B/FTM3_CH0/USB0_SOF_OUT}
  - {pin_num: F3, peripheral: QuadSPI0, signal: 'SCLK, B', pin_signal: PTE7/SPI2_SCK/LPUART3_RTS_b/I2S0_RXD0/QSPI0B_SCLK/FTM3_CH2/QSPI0A_SS1_B}
  - {pin_num: E1, peripheral: QuadSPI0, signal: 'DATA3, B', pin_signal: PTE6/LLWU_P16/SPI1_PCS3/LPUART3_CTS_b/I2S0_MCLK/QSPI0B_DATA3/FTM3_CH1/SDHC0_D4}
  - {pin_num: F1, peripheral: QuadSPI0, signal: 'DATA2, B', pin_signal: PTE9/LLWU_P17/I2S0_TXD1/SPI2_PCS1/I2S0_RX_BCLK/QSPI0B_DATA2/FTM3_CH4/SDHC0_D6}
  - {pin_num: G2, peripheral: QuadSPI0, signal: 'DATA1, B', pin_signal: PTE10/LLWU_P18/I2C3_SDA/SPI2_SIN/I2S0_TXD0/QSPI0B_DATA1/FTM3_CH5/SDHC0_D7}
  - {pin_num: F2, peripheral: QuadSPI0, signal: 'DATA0, B', pin_signal: PTE8/I2S0_RXD1/SPI2_SOUT/I2S0_RX_FS/QSPI0B_DATA0/FTM3_CH3/SDHC0_D5}
  - {pin_num: G1, peripheral: QuadSPI0, signal: 'SS0, B', pin_signal: PTE11/I2C3_SCL/SPI2_PCS0/I2S0_TX_FS/QSPI0B_SS0_B/FTM3_CH6/QSPI0A_DQS}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void BOARD_InitPins(void) {
  CLOCK_EnableClock(kCLOCK_PortC);                           /* Port C Clock Gate Control: Clock enabled */
  CLOCK_EnableClock(kCLOCK_PortE);                           /* Port E Clock Gate Control: Clock enabled */

  PORT_SetPinMux(PORTC, PIN3_IDX, kPORT_MuxAlt3);            /* PORTC3 (pin B8) is configured as LPUART1_RX */
  PORT_SetPinMux(PORTC, PIN4_IDX, kPORT_MuxAlt3);            /* PORTC4 (pin A8) is configured as LPUART1_TX */
  PORT_SetPinMux(PORTE, PIN0_IDX, kPORT_MuxAlt5);            /* PORTE0 (pin B1) is configured as QSPI0A_DATA3 */
  PORT_SetPinMux(PORTE, PIN1_IDX, kPORT_MuxAlt5);            /* PORTE1 (pin C2) is configured as QSPI0A_SCLK */
  PORT_SetPinMux(PORTE, PIN10_IDX, kPORT_MuxAlt5);           /* PORTE10 (pin G2) is configured as QSPI0B_DATA1 */
  PORT_SetPinMux(PORTE, PIN11_IDX, kPORT_MuxAlt5);           /* PORTE11 (pin G1) is configured as QSPI0B_SS0_B */
  PORT_SetPinMux(PORTE, PIN2_IDX, kPORT_MuxAlt5);            /* PORTE2 (pin C1) is configured as QSPI0A_DATA0 */
  PORT_SetPinMux(PORTE, PIN3_IDX, kPORT_MuxAlt5);            /* PORTE3 (pin D2) is configured as QSPI0A_DATA2 */
  PORT_SetPinMux(PORTE, PIN4_IDX, kPORT_MuxAlt5);            /* PORTE4 (pin D1) is configured as QSPI0A_DATA1 */
  PORT_SetPinMux(PORTE, PIN5_IDX, kPORT_MuxAlt5);            /* PORTE5 (pin E2) is configured as QSPI0A_SS0_B */
  PORT_SetPinMux(PORTE, PIN6_IDX, kPORT_MuxAlt5);            /* PORTE6 (pin E1) is configured as QSPI0B_DATA3 */
  PORT_SetPinMux(PORTE, PIN7_IDX, kPORT_MuxAlt5);            /* PORTE7 (pin F3) is configured as QSPI0B_SCLK */
  PORT_SetPinMux(PORTE, PIN8_IDX, kPORT_MuxAlt5);            /* PORTE8 (pin F2) is configured as QSPI0B_DATA0 */
  PORT_SetPinMux(PORTE, PIN9_IDX, kPORT_MuxAlt5);            /* PORTE9 (pin F1) is configured as QSPI0B_DATA2 */
  SIM->SOPT5 = ((SIM->SOPT5 &
    (~(SIM_SOPT5_LPUART1TXSRC_MASK | SIM_SOPT5_LPUART1RXSRC_MASK))) /* Mask bits to zero which are setting */
      | SIM_SOPT5_LPUART1TXSRC(SOPT5_LPUART1TXSRC_LPUART_TX) /* LPUART1 transmit data source select: LPUART1_TX pin */
      | SIM_SOPT5_LPUART1RXSRC(SOPT5_LPUART1RXSRC_LPUART_RX) /* LPUART1 receive data source select: LPUART1_RX pin */
    );
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
