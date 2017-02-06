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
- !!processor 'MK82FN256xxx15'
- !!package 'MK82FN256VLL15'
- !!mcu_data 'ksdk2_0'
- !!processor_version '0.1.19'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"

#define PIN2_IDX                         2u   /*!< Pin number for pin 2 in a port */
#define PIN3_IDX                         3u   /*!< Pin number for pin 3 in a port */
#define PIN10_IDX                       10u   /*!< Pin number for pin 10 in a port */
#define PIN11_IDX                       11u   /*!< Pin number for pin 11 in a port */
#define PIN14_IDX                       14u   /*!< Pin number for pin 14 in a port */
#define PIN15_IDX                       15u   /*!< Pin number for pin 15 in a port */
#define PIN16_IDX                       16u   /*!< Pin number for pin 16 in a port */
#define PIN17_IDX                       17u   /*!< Pin number for pin 17 in a port */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
BOARD_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '86', peripheral: LPUART4, signal: RX, pin_signal: PTC14/LPUART4_RX/FB_AD25/SDRAM_D25/FXIO0_D20}
  - {pin_num: '87', peripheral: LPUART4, signal: TX, pin_signal: PTC15/LPUART4_TX/FB_AD24/SDRAM_D24/FXIO0_D21}
  - {pin_num: '55', peripheral: FLEXIO0, signal: 'D, 2', pin_signal: ADC0_SE12/TSI0_CH7/PTB2/I2C0_SCL/LPUART0_RTS_b/SDRAM_WE/FTM0_FLT3/FXIO0_D2}
  - {pin_num: '56', peripheral: FLEXIO0, signal: 'D, 3', pin_signal: ADC0_SE13/TSI0_CH8/PTB3/I2C0_SDA/LPUART0_CTS_b/SDRAM_CS0_b/FTM0_FLT0/FXIO0_D3}
  - {pin_num: '58', peripheral: FLEXIO0, signal: 'D, 4', pin_signal: PTB10/SPI1_PCS0/LPUART3_RX/I2C2_SCL/FB_AD19/SDRAM_D19/FTM0_FLT1/FXIO0_D4}
  - {pin_num: '59', peripheral: FLEXIO0, signal: 'D, 5', pin_signal: PTB11/SPI1_SCK/LPUART3_TX/I2C2_SDA/FB_AD18/SDRAM_D18/FTM0_FLT2/FXIO0_D5}
  - {pin_num: '44', peripheral: SPI0, signal: PCS0_SS, pin_signal: PTA14/SPI0_PCS0/LPUART0_TX/TRACE_D2/FXIO0_D20/I2S0_RX_BCLK/I2S0_TXD1}
  - {pin_num: '45', peripheral: SPI0, signal: SCK, pin_signal: PTA15/SPI0_SCK/LPUART0_RX/TRACE_D1/FXIO0_D21/I2S0_RXD0}
  - {pin_num: '46', peripheral: SPI0, signal: SOUT, pin_signal: PTA16/SPI0_SOUT/LPUART0_CTS_b/TRACE_D0/FXIO0_D22/I2S0_RX_FS/I2S0_RXD1}
  - {pin_num: '47', peripheral: SPI0, signal: SIN, pin_signal: PTA17/SPI0_SIN/LPUART0_RTS_b/FXIO0_D23/I2S0_MCLK}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void BOARD_InitPins(void) {
  CLOCK_EnableClock(kCLOCK_PortA);                           /* Port A Clock Gate Control: Clock enabled */
  CLOCK_EnableClock(kCLOCK_PortB);                           /* Port B Clock Gate Control: Clock enabled */
  CLOCK_EnableClock(kCLOCK_PortC);                           /* Port C Clock Gate Control: Clock enabled */

  PORT_SetPinMux(PORTA, PIN14_IDX, kPORT_MuxAlt2);           /* PORTA14 (pin 44) is configured as SPI0_PCS0 */
  PORT_SetPinMux(PORTA, PIN15_IDX, kPORT_MuxAlt2);           /* PORTA15 (pin 45) is configured as SPI0_SCK */
  PORT_SetPinMux(PORTA, PIN16_IDX, kPORT_MuxAlt2);           /* PORTA16 (pin 46) is configured as SPI0_SOUT */
  PORT_SetPinMux(PORTA, PIN17_IDX, kPORT_MuxAlt2);           /* PORTA17 (pin 47) is configured as SPI0_SIN */
  PORT_SetPinMux(PORTB, PIN10_IDX, kPORT_MuxAlt7);           /* PORTB10 (pin 58) is configured as FXIO0_D4 */
  PORT_SetPinMux(PORTB, PIN11_IDX, kPORT_MuxAlt7);           /* PORTB11 (pin 59) is configured as FXIO0_D5 */
  PORT_SetPinMux(PORTB, PIN2_IDX, kPORT_MuxAlt7);            /* PORTB2 (pin 55) is configured as FXIO0_D2 */
  PORT_SetPinMux(PORTB, PIN3_IDX, kPORT_MuxAlt7);            /* PORTB3 (pin 56) is configured as FXIO0_D3 */
  PORT_SetPinMux(PORTC, PIN14_IDX, kPORT_MuxAlt3);           /* PORTC14 (pin 86) is configured as LPUART4_RX */
  PORT_SetPinMux(PORTC, PIN15_IDX, kPORT_MuxAlt3);           /* PORTC15 (pin 87) is configured as LPUART4_TX */
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
