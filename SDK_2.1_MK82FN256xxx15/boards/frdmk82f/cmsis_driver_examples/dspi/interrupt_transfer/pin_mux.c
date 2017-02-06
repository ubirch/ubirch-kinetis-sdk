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
- !!product 'Pins v2.0'
- !!processor 'MK82FN256xxx15'
- !!package 'MK82FN256VLL15'
- !!mcu_data 'ksdk2_0'
- !!processor_version '0.1.19'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"

#define PIN14_IDX                       14u   /*!< Pin number for pin 14 in a port */
#define PIN15_IDX                       15u   /*!< Pin number for pin 15 in a port */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
BOARD_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '86', peripheral: LPUART4, signal: RX, pin_signal: PTC14/LPUART4_RX/FB_AD25/SDRAM_D25/FXIO0_D20}
  - {pin_num: '87', peripheral: LPUART4, signal: TX, pin_signal: PTC15/LPUART4_TX/FB_AD24/SDRAM_D24/FXIO0_D21}
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

  PORT_SetPinMux(PORTC, PIN14_IDX, kPORT_MuxAlt3);           /* PORTC14 (pin 86) is configured as LPUART4_RX */
  PORT_SetPinMux(PORTC, PIN15_IDX, kPORT_MuxAlt3);           /* PORTC15 (pin 87) is configured as LPUART4_TX */
}


#define PIN14_IDX                       14u   /*!< Pin number for pin 14 in a port */
#define PIN15_IDX                       15u   /*!< Pin number for pin 15 in a port */
#define PIN16_IDX                       16u   /*!< Pin number for pin 16 in a port */
#define PIN17_IDX                       17u   /*!< Pin number for pin 17 in a port */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
DSPI0_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '44', peripheral: SPI0, signal: PCS0_SS, pin_signal: PTA14/SPI0_PCS0/LPUART0_TX/TRACE_D2/FXIO0_D20/I2S0_RX_BCLK/I2S0_TXD1}
  - {pin_num: '45', peripheral: SPI0, signal: SCK, pin_signal: PTA15/SPI0_SCK/LPUART0_RX/TRACE_D1/FXIO0_D21/I2S0_RXD0}
  - {pin_num: '46', peripheral: SPI0, signal: SOUT, pin_signal: PTA16/SPI0_SOUT/LPUART0_CTS_b/TRACE_D0/FXIO0_D22/I2S0_RX_FS/I2S0_RXD1}
  - {pin_num: '47', peripheral: SPI0, signal: SIN, pin_signal: PTA17/SPI0_SIN/LPUART0_RTS_b/FXIO0_D23/I2S0_MCLK}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : DSPI0_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void DSPI0_InitPins(void) {
  CLOCK_EnableClock(kCLOCK_PortA);                           /* Port A Clock Gate Control: Clock enabled */

  PORT_SetPinMux(PORTA, PIN14_IDX, kPORT_MuxAlt2);           /* PORTA14 (pin 44) is configured as SPI0_PCS0 */
  PORT_SetPinMux(PORTA, PIN15_IDX, kPORT_MuxAlt2);           /* PORTA15 (pin 45) is configured as SPI0_SCK */
  PORT_SetPinMux(PORTA, PIN16_IDX, kPORT_MuxAlt2);           /* PORTA16 (pin 46) is configured as SPI0_SOUT */
  PORT_SetPinMux(PORTA, PIN17_IDX, kPORT_MuxAlt2);           /* PORTA17 (pin 47) is configured as SPI0_SIN */
}


#define PIN14_IDX                       14u   /*!< Pin number for pin 14 in a port */
#define PIN15_IDX                       15u   /*!< Pin number for pin 15 in a port */
#define PIN16_IDX                       16u   /*!< Pin number for pin 16 in a port */
#define PIN17_IDX                       17u   /*!< Pin number for pin 17 in a port */
/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
DSPI0_DeinitPins:
- options: {coreID: singlecore, enableClock: 'false'}
- pin_list:
  - {pin_num: '44', peripheral: n/a, signal: disabled, pin_signal: PTA14/SPI0_PCS0/LPUART0_TX/TRACE_D2/FXIO0_D20/I2S0_RX_BCLK/I2S0_TXD1}
  - {pin_num: '45', peripheral: n/a, signal: disabled, pin_signal: PTA15/SPI0_SCK/LPUART0_RX/TRACE_D1/FXIO0_D21/I2S0_RXD0}
  - {pin_num: '46', peripheral: n/a, signal: disabled, pin_signal: PTA16/SPI0_SOUT/LPUART0_CTS_b/TRACE_D0/FXIO0_D22/I2S0_RX_FS/I2S0_RXD1}
  - {pin_num: '47', peripheral: n/a, signal: disabled, pin_signal: PTA17/SPI0_SIN/LPUART0_RTS_b/FXIO0_D23/I2S0_MCLK}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : DSPI0_DeinitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void DSPI0_DeinitPins(void) {
  PORT_SetPinMux(PORTA, PIN14_IDX, kPORT_PinDisabledOrAnalog); /* PORTA14 (pin 44) is disabled */
  PORT_SetPinMux(PORTA, PIN15_IDX, kPORT_PinDisabledOrAnalog); /* PORTA15 (pin 45) is disabled */
  PORT_SetPinMux(PORTA, PIN16_IDX, kPORT_PinDisabledOrAnalog); /* PORTA16 (pin 46) is disabled */
  PORT_SetPinMux(PORTA, PIN17_IDX, kPORT_PinDisabledOrAnalog); /* PORTA17 (pin 47) is disabled */
}


#define PIN4_IDX                         4u   /*!< Pin number for pin 4 in a port */
#define PIN5_IDX                         5u   /*!< Pin number for pin 5 in a port */
#define PIN6_IDX                         6u   /*!< Pin number for pin 6 in a port */
#define PIN7_IDX                         7u   /*!< Pin number for pin 7 in a port */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
DSPI1_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '95', peripheral: SPI1, signal: PCS0_SS, pin_signal: PTD4/LLWU_P14/SPI0_PCS1/LPUART0_RTS_b/FTM0_CH4/FB_AD2/SDRAM_A10/EWM_IN/SPI1_PCS0}
  - {pin_num: '96', peripheral: SPI1, signal: SCK, pin_signal: ADC0_SE6b/PTD5/SPI0_PCS2/LPUART0_CTS_b/FTM0_CH5/FB_AD1/SDRAM_A9/EWM_OUT_b/SPI1_SCK}
  - {pin_num: '97', peripheral: SPI1, signal: SOUT, pin_signal: ADC0_SE7b/PTD6/LLWU_P15/SPI0_PCS3/LPUART0_RX/FTM0_CH6/FB_AD0/FTM0_FLT0/SPI1_SOUT}
  - {pin_num: '100', peripheral: SPI1, signal: SIN, pin_signal: PTD7/CMT_IRO/LPUART0_TX/FTM0_CH7/SDRAM_CKE/FTM0_FLT1/SPI1_SIN}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : DSPI1_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void DSPI1_InitPins(void) {
  CLOCK_EnableClock(kCLOCK_PortD);                           /* Port D Clock Gate Control: Clock enabled */

  PORT_SetPinMux(PORTD, PIN4_IDX, kPORT_MuxAlt7);            /* PORTD4 (pin 95) is configured as SPI1_PCS0 */
  PORT_SetPinMux(PORTD, PIN5_IDX, kPORT_MuxAlt7);            /* PORTD5 (pin 96) is configured as SPI1_SCK */
  PORT_SetPinMux(PORTD, PIN6_IDX, kPORT_MuxAlt7);            /* PORTD6 (pin 97) is configured as SPI1_SOUT */
  PORT_SetPinMux(PORTD, PIN7_IDX, kPORT_MuxAlt7);            /* PORTD7 (pin 100) is configured as SPI1_SIN */
}


#define PIN4_IDX                         4u   /*!< Pin number for pin 4 in a port */
#define PIN5_IDX                         5u   /*!< Pin number for pin 5 in a port */
#define PIN6_IDX                         6u   /*!< Pin number for pin 6 in a port */
#define PIN7_IDX                         7u   /*!< Pin number for pin 7 in a port */
/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
DSPI1_DeinitPins:
- options: {coreID: singlecore, enableClock: 'false'}
- pin_list:
  - {pin_num: '95', peripheral: n/a, signal: disabled, pin_signal: PTD4/LLWU_P14/SPI0_PCS1/LPUART0_RTS_b/FTM0_CH4/FB_AD2/SDRAM_A10/EWM_IN/SPI1_PCS0}
  - {pin_num: '96', peripheral: ADC0, signal: 'SE, 6b', pin_signal: ADC0_SE6b/PTD5/SPI0_PCS2/LPUART0_CTS_b/FTM0_CH5/FB_AD1/SDRAM_A9/EWM_OUT_b/SPI1_SCK}
  - {pin_num: '97', peripheral: ADC0, signal: 'SE, 7b', pin_signal: ADC0_SE7b/PTD6/LLWU_P15/SPI0_PCS3/LPUART0_RX/FTM0_CH6/FB_AD0/FTM0_FLT0/SPI1_SOUT}
  - {pin_num: '100', peripheral: n/a, signal: disabled, pin_signal: PTD7/CMT_IRO/LPUART0_TX/FTM0_CH7/SDRAM_CKE/FTM0_FLT1/SPI1_SIN}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : DSPI1_DeinitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void DSPI1_DeinitPins(void) {
  PORT_SetPinMux(PORTD, PIN4_IDX, kPORT_PinDisabledOrAnalog); /* PORTD4 (pin 95) is disabled */
  PORT_SetPinMux(PORTD, PIN5_IDX, kPORT_PinDisabledOrAnalog); /* PORTD5 (pin 96) is configured as ADC0_SE6b */
  PORT_SetPinMux(PORTD, PIN6_IDX, kPORT_PinDisabledOrAnalog); /* PORTD6 (pin 97) is configured as ADC0_SE7b */
  PORT_SetPinMux(PORTD, PIN7_IDX, kPORT_PinDisabledOrAnalog); /* PORTD7 (pin 100) is disabled */
}


#define PIN20_IDX                       20u   /*!< Pin number for pin 20 in a port */
#define PIN21_IDX                       21u   /*!< Pin number for pin 21 in a port */
#define PIN22_IDX                       22u   /*!< Pin number for pin 22 in a port */
#define PIN23_IDX                       23u   /*!< Pin number for pin 23 in a port */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
DSPI2_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '66', peripheral: SPI2, signal: PCS0_SS, pin_signal: PTB20/SPI2_PCS0/FB_AD31/SDRAM_D31/CMP0_OUT/FXIO0_D8}
  - {pin_num: '67', peripheral: SPI2, signal: SCK, pin_signal: PTB21/SPI2_SCK/FB_AD30/SDRAM_D30/CMP1_OUT/FXIO0_D9}
  - {pin_num: '69', peripheral: SPI2, signal: SIN, pin_signal: PTB23/SPI2_SIN/SPI0_PCS5/FB_AD28/SDRAM_D28/FXIO0_D11}
  - {pin_num: '68', peripheral: SPI2, signal: SOUT, pin_signal: PTB22/SPI2_SOUT/FB_AD29/SDRAM_D29/FXIO0_D10}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : DSPI2_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void DSPI2_InitPins(void) {
  CLOCK_EnableClock(kCLOCK_PortB);                           /* Port B Clock Gate Control: Clock enabled */

  PORT_SetPinMux(PORTB, PIN20_IDX, kPORT_MuxAlt2);           /* PORTB20 (pin 66) is configured as SPI2_PCS0 */
  PORT_SetPinMux(PORTB, PIN21_IDX, kPORT_MuxAlt2);           /* PORTB21 (pin 67) is configured as SPI2_SCK */
  PORT_SetPinMux(PORTB, PIN22_IDX, kPORT_MuxAlt2);           /* PORTB22 (pin 68) is configured as SPI2_SOUT */
  PORT_SetPinMux(PORTB, PIN23_IDX, kPORT_MuxAlt2);           /* PORTB23 (pin 69) is configured as SPI2_SIN */
}


#define PIN20_IDX                       20u   /*!< Pin number for pin 20 in a port */
#define PIN21_IDX                       21u   /*!< Pin number for pin 21 in a port */
#define PIN22_IDX                       22u   /*!< Pin number for pin 22 in a port */
#define PIN23_IDX                       23u   /*!< Pin number for pin 23 in a port */
/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
DSPI2_DeinitPins:
- options: {coreID: singlecore, enableClock: 'false'}
- pin_list:
  - {pin_num: '66', peripheral: n/a, signal: disabled, pin_signal: PTB20/SPI2_PCS0/FB_AD31/SDRAM_D31/CMP0_OUT/FXIO0_D8}
  - {pin_num: '67', peripheral: n/a, signal: disabled, pin_signal: PTB21/SPI2_SCK/FB_AD30/SDRAM_D30/CMP1_OUT/FXIO0_D9}
  - {pin_num: '68', peripheral: n/a, signal: disabled, pin_signal: PTB22/SPI2_SOUT/FB_AD29/SDRAM_D29/FXIO0_D10}
  - {pin_num: '69', peripheral: n/a, signal: disabled, pin_signal: PTB23/SPI2_SIN/SPI0_PCS5/FB_AD28/SDRAM_D28/FXIO0_D11}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : DSPI2_DeinitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void DSPI2_DeinitPins(void) {
  PORT_SetPinMux(PORTB, PIN20_IDX, kPORT_PinDisabledOrAnalog); /* PORTB20 (pin 66) is disabled */
  PORT_SetPinMux(PORTB, PIN21_IDX, kPORT_PinDisabledOrAnalog); /* PORTB21 (pin 67) is disabled */
  PORT_SetPinMux(PORTB, PIN22_IDX, kPORT_PinDisabledOrAnalog); /* PORTB22 (pin 68) is disabled */
  PORT_SetPinMux(PORTB, PIN23_IDX, kPORT_PinDisabledOrAnalog); /* PORTB23 (pin 69) is disabled */
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
