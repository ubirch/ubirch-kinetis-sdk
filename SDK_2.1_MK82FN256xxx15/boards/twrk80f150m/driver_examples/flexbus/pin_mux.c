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
#define PIN12_IDX                       12u   /*!< Pin number for pin 12 in a port */
#define PIN13_IDX                       13u   /*!< Pin number for pin 13 in a port */
#define PIN14_IDX                       14u   /*!< Pin number for pin 14 in a port */
#define PIN15_IDX                       15u   /*!< Pin number for pin 15 in a port */
#define PIN16_IDX                       16u   /*!< Pin number for pin 16 in a port */
#define PIN17_IDX                       17u   /*!< Pin number for pin 17 in a port */
#define PIN18_IDX                       18u   /*!< Pin number for pin 18 in a port */
#define PIN19_IDX                       19u   /*!< Pin number for pin 19 in a port */
#define PIN20_IDX                       20u   /*!< Pin number for pin 20 in a port */
#define PIN21_IDX                       21u   /*!< Pin number for pin 21 in a port */
#define PIN22_IDX                       22u   /*!< Pin number for pin 22 in a port */
#define PIN23_IDX                       23u   /*!< Pin number for pin 23 in a port */
#define SOPT5_LPUART1RXSRC_LPUART_RX  0x00u   /*!< LPUART1 receive data source select: LPUART1_RX pin */
#define SOPT5_LPUART1TXSRC_LPUART_TX  0x00u   /*!< LPUART1 transmit data source select: LPUART1_TX pin */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
BOARD_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: B8, peripheral: LPUART1, signal: RX, pin_signal: CMP1_IN1/PTC3/LLWU_P7/SPI0_PCS1/LPUART1_RX/FTM0_CH2/CLKOUT/I2S0_TX_BCLK}
  - {pin_num: A8, peripheral: LPUART1, signal: TX, pin_signal: PTC4/LLWU_P8/SPI0_PCS0/LPUART1_TX/FTM0_CH3/FB_AD11/SDRAM_A19/CMP1_OUT}
  - {pin_num: B2, peripheral: FB, signal: 'AD, 0', pin_signal: ADC0_SE7b/PTD6/LLWU_P15/SPI0_PCS3/LPUART0_RX/FTM0_CH6/FB_AD0/FTM0_FLT0/SPI1_SOUT}
  - {pin_num: A2, peripheral: FB, signal: 'AD, 1', pin_signal: ADC0_SE6b/PTD5/SPI0_PCS2/LPUART0_CTS_b/FTM0_CH5/FB_AD1/SDRAM_A9/EWM_OUT_b/SPI1_SCK}
  - {pin_num: A3, peripheral: FB, signal: 'AD, 2', pin_signal: PTD4/LLWU_P14/SPI0_PCS1/LPUART0_RTS_b/FTM0_CH4/FB_AD2/SDRAM_A10/EWM_IN/SPI1_PCS0}
  - {pin_num: B3, peripheral: FB, signal: 'AD, 3', pin_signal: PTD3/SPI0_SIN/LPUART2_TX/FTM3_CH3/FB_AD3/SDRAM_A11/I2C0_SDA}
  - {pin_num: C3, peripheral: FB, signal: 'AD, 4', pin_signal: PTD2/LLWU_P13/SPI0_SOUT/LPUART2_RX/FTM3_CH2/FB_AD4/SDRAM_A12/I2C0_SCL}
  - {pin_num: C6, peripheral: FB, signal: 'AD, 5', pin_signal: PTC10/I2C1_SCL/FTM3_CH6/I2S0_RX_FS/FB_AD5/SDRAM_A13/FXIO0_D18}
  - {pin_num: D6, peripheral: FB, signal: 'AD, 6', pin_signal: CMP0_IN3/PTC9/FTM3_CH5/I2S0_RX_BCLK/FB_AD6/SDRAM_A14/FTM2_FLT0/FXIO0_D17}
  - {pin_num: A7, peripheral: FB, signal: 'AD, 7', pin_signal: CMP0_IN2/PTC8/FTM3_CH4/I2S0_MCLK/FB_AD7/SDRAM_A15/FXIO0_D16}
  - {pin_num: B7, peripheral: FB, signal: 'AD, 8', pin_signal: CMP0_IN1/PTC7/SPI0_SIN/USB0_SOF_OUT/I2S0_RX_FS/FB_AD8/SDRAM_A16/FXIO0_D15}
  - {pin_num: C7, peripheral: FB, signal: 'AD, 9', pin_signal: CMP0_IN0/PTC6/LLWU_P10/SPI0_SOUT/PDB0_EXTRG/I2S0_RX_BCLK/FB_AD9/SDRAM_A17/I2S0_MCLK/FXIO0_D14}
  - {pin_num: D7, peripheral: FB, signal: 'AD, 10', pin_signal: PTC5/LLWU_P9/SPI0_SCK/LPTMR0_ALT2/LPTMR1_ALT2/I2S0_RXD0/FB_AD10/SDRAM_A18/CMP0_OUT/FTM0_CH2}
  - {pin_num: B9, peripheral: FB, signal: 'AD, 14', pin_signal: ADC0_SE14/TSI0_CH13/PTC0/SPI0_PCS4/PDB0_EXTRG/USB0_SOF_OUT/FB_AD14/SDRAM_A22/I2S0_TXD1/FXIO0_D12}
  - {pin_num: D8, peripheral: FB, signal: 'AD, 13', pin_signal: ADC0_SE15/TSI0_CH14/PTC1/LLWU_P6/SPI0_PCS3/LPUART1_RTS_b/FTM0_CH0/FB_AD13/SDRAM_A21/I2S0_TXD0/FXIO0_D13}
  - {pin_num: C8, peripheral: FB, signal: 'AD, 12', pin_signal: ADC0_SE4b/CMP1_IN0/TSI0_CH15/PTC2/SPI0_PCS2/LPUART1_CTS_b/FTM0_CH1/FB_AD12/SDRAM_A20/I2S0_TX_FS}
  - {pin_num: D9, peripheral: FB, signal: 'AD, 15', pin_signal: TSI0_CH11/PTB18/FTM2_CH0/I2S0_TX_BCLK/FB_AD15/SDRAM_A23/FTM2_QD_PHA/TPM2_CH0/FXIO0_D6}
  - {pin_num: E9, peripheral: FB, signal: 'AD, 16', pin_signal: TSI0_CH10/PTB17/SPI1_SIN/LPUART0_TX/FTM_CLKIN1/FB_AD16/SDRAM_D16/EWM_OUT_b/TPM_CLKIN1}
  - {pin_num: B10, peripheral: FB, signal: 'AD, 17', pin_signal: TSI0_CH9/PTB16/SPI1_SOUT/LPUART0_RX/FTM_CLKIN0/FB_AD17/SDRAM_D17/EWM_IN/TPM_CLKIN0}
  - {pin_num: C10, peripheral: FB, signal: 'AD, 18', pin_signal: PTB11/SPI1_SCK/LPUART3_TX/I2C2_SDA/FB_AD18/SDRAM_D18/FTM0_FLT2/FXIO0_D5}
  - {pin_num: D10, peripheral: FB, signal: 'AD, 19', pin_signal: PTB10/SPI1_PCS0/LPUART3_RX/I2C2_SCL/FB_AD19/SDRAM_D19/FTM0_FLT1/FXIO0_D4}
  - {pin_num: B5, peripheral: FB, signal: 'AD, 24', pin_signal: PTC15/LPUART4_TX/FB_AD24/SDRAM_D24/FXIO0_D21}
  - {pin_num: A5, peripheral: FB, signal: 'AD, 25', pin_signal: PTC14/LPUART4_RX/FB_AD25/SDRAM_D25/FXIO0_D20}
  - {pin_num: A6, peripheral: FB, signal: 'AD, 26', pin_signal: PTC13/LPUART4_CTS_b/FTM_CLKIN1/FB_AD26/SDRAM_D26/TPM_CLKIN1}
  - {pin_num: B6, peripheral: FB, signal: 'AD, 27', pin_signal: PTC12/LPUART4_RTS_b/FTM_CLKIN0/FB_AD27/SDRAM_D27/FTM3_FLT0/TPM_CLKIN0}
  - {pin_num: E8, peripheral: FB, signal: 'AD, 28', pin_signal: PTB23/SPI2_SIN/SPI0_PCS5/FB_AD28/SDRAM_D28/FXIO0_D11}
  - {pin_num: F8, peripheral: FB, signal: 'AD, 29', pin_signal: PTB22/SPI2_SOUT/FB_AD29/SDRAM_D29/FXIO0_D10}
  - {pin_num: F9, peripheral: FB, signal: 'AD, 30', pin_signal: PTB21/SPI2_SCK/FB_AD30/SDRAM_D30/CMP1_OUT/FXIO0_D9}
  - {pin_num: F10, peripheral: FB, signal: 'AD, 31', pin_signal: PTB20/SPI2_PCS0/FB_AD31/SDRAM_D31/CMP0_OUT/FXIO0_D8}
  - {pin_num: C5, peripheral: FB, signal: RW, pin_signal: PTC11/LLWU_P11/I2C1_SDA/FTM3_CH7/I2S0_RXD1/FB_RW_b/FXIO0_D19}
  - {pin_num: C9, peripheral: FB, signal: OE, pin_signal: TSI0_CH12/PTB19/FTM2_CH1/I2S0_TX_FS/FB_OE_b/FTM2_QD_PHB/TPM2_CH1/FXIO0_D7}
  - {pin_num: D4, peripheral: FB, signal: CS1_ALE_TS, pin_signal: PTD0/LLWU_P12/SPI0_PCS0/LPUART2_RTS_b/FTM3_CH0/FB_ALE/FB_CS1_b/FB_TS_b/FXIO0_D22}
  - {pin_num: D3, peripheral: FB, signal: CS0, pin_signal: ADC0_SE5b/PTD1/SPI0_SCK/LPUART2_CTS_b/FTM3_CH1/FB_CS0_b/FXIO0_D23}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void BOARD_InitPins(void) {
  CLOCK_EnableClock(kCLOCK_PortB);                           /* Port B Clock Gate Control: Clock enabled */
  CLOCK_EnableClock(kCLOCK_PortC);                           /* Port C Clock Gate Control: Clock enabled */
  CLOCK_EnableClock(kCLOCK_PortD);                           /* Port D Clock Gate Control: Clock enabled */

  PORT_SetPinMux(PORTB, PIN10_IDX, kPORT_MuxAlt5);           /* PORTB10 (pin D10) is configured as FB_AD19 */
  PORT_SetPinMux(PORTB, PIN11_IDX, kPORT_MuxAlt5);           /* PORTB11 (pin C10) is configured as FB_AD18 */
  PORT_SetPinMux(PORTB, PIN16_IDX, kPORT_MuxAlt5);           /* PORTB16 (pin B10) is configured as FB_AD17 */
  PORT_SetPinMux(PORTB, PIN17_IDX, kPORT_MuxAlt5);           /* PORTB17 (pin E9) is configured as FB_AD16 */
  PORT_SetPinMux(PORTB, PIN18_IDX, kPORT_MuxAlt5);           /* PORTB18 (pin D9) is configured as FB_AD15 */
  PORT_SetPinMux(PORTB, PIN19_IDX, kPORT_MuxAlt5);           /* PORTB19 (pin C9) is configured as FB_OE_b */
  PORT_SetPinMux(PORTB, PIN20_IDX, kPORT_MuxAlt5);           /* PORTB20 (pin F10) is configured as FB_AD31 */
  PORT_SetPinMux(PORTB, PIN21_IDX, kPORT_MuxAlt5);           /* PORTB21 (pin F9) is configured as FB_AD30 */
  PORT_SetPinMux(PORTB, PIN22_IDX, kPORT_MuxAlt5);           /* PORTB22 (pin F8) is configured as FB_AD29 */
  PORT_SetPinMux(PORTB, PIN23_IDX, kPORT_MuxAlt5);           /* PORTB23 (pin E8) is configured as FB_AD28 */
  PORT_SetPinMux(PORTC, PIN0_IDX, kPORT_MuxAlt5);            /* PORTC0 (pin B9) is configured as FB_AD14 */
  PORT_SetPinMux(PORTC, PIN1_IDX, kPORT_MuxAlt5);            /* PORTC1 (pin D8) is configured as FB_AD13 */
  PORT_SetPinMux(PORTC, PIN10_IDX, kPORT_MuxAlt5);           /* PORTC10 (pin C6) is configured as FB_AD5 */
  PORT_SetPinMux(PORTC, PIN11_IDX, kPORT_MuxAlt5);           /* PORTC11 (pin C5) is configured as FB_RW_b */
  PORT_SetPinMux(PORTC, PIN12_IDX, kPORT_MuxAlt5);           /* PORTC12 (pin B6) is configured as FB_AD27 */
  PORT_SetPinMux(PORTC, PIN13_IDX, kPORT_MuxAlt5);           /* PORTC13 (pin A6) is configured as FB_AD26 */
  PORT_SetPinMux(PORTC, PIN14_IDX, kPORT_MuxAlt5);           /* PORTC14 (pin A5) is configured as FB_AD25 */
  PORT_SetPinMux(PORTC, PIN15_IDX, kPORT_MuxAlt5);           /* PORTC15 (pin B5) is configured as FB_AD24 */
  PORT_SetPinMux(PORTC, PIN2_IDX, kPORT_MuxAlt5);            /* PORTC2 (pin C8) is configured as FB_AD12 */
  PORT_SetPinMux(PORTC, PIN3_IDX, kPORT_MuxAlt3);            /* PORTC3 (pin B8) is configured as LPUART1_RX */
  PORT_SetPinMux(PORTC, PIN4_IDX, kPORT_MuxAlt3);            /* PORTC4 (pin A8) is configured as LPUART1_TX */
  PORT_SetPinMux(PORTC, PIN5_IDX, kPORT_MuxAlt5);            /* PORTC5 (pin D7) is configured as FB_AD10 */
  PORT_SetPinMux(PORTC, PIN6_IDX, kPORT_MuxAlt5);            /* PORTC6 (pin C7) is configured as FB_AD9 */
  PORT_SetPinMux(PORTC, PIN7_IDX, kPORT_MuxAlt5);            /* PORTC7 (pin B7) is configured as FB_AD8 */
  PORT_SetPinMux(PORTC, PIN8_IDX, kPORT_MuxAlt5);            /* PORTC8 (pin A7) is configured as FB_AD7 */
  PORT_SetPinMux(PORTC, PIN9_IDX, kPORT_MuxAlt5);            /* PORTC9 (pin D6) is configured as FB_AD6 */
  PORT_SetPinMux(PORTD, PIN0_IDX, kPORT_MuxAlt5);            /* PORTD0 (pin D4) is configured as FB_CS1_b */
  PORT_SetPinMux(PORTD, PIN1_IDX, kPORT_MuxAlt5);            /* PORTD1 (pin D3) is configured as FB_CS0_b */
  PORT_SetPinMux(PORTD, PIN2_IDX, kPORT_MuxAlt5);            /* PORTD2 (pin C3) is configured as FB_AD4 */
  PORT_SetPinMux(PORTD, PIN3_IDX, kPORT_MuxAlt5);            /* PORTD3 (pin B3) is configured as FB_AD3 */
  PORT_SetPinMux(PORTD, PIN4_IDX, kPORT_MuxAlt5);            /* PORTD4 (pin A3) is configured as FB_AD2 */
  PORT_SetPinMux(PORTD, PIN5_IDX, kPORT_MuxAlt5);            /* PORTD5 (pin A2) is configured as FB_AD1 */
  PORT_SetPinMux(PORTD, PIN6_IDX, kPORT_MuxAlt5);            /* PORTD6 (pin B2) is configured as FB_AD0 */
  SIM->SOPT5 = ((SIM->SOPT5 &
    (~(SIM_SOPT5_LPUART1TXSRC_MASK | SIM_SOPT5_LPUART1RXSRC_MASK))) /* Mask bits to zero which are setting */
      | SIM_SOPT5_LPUART1TXSRC(SOPT5_LPUART1TXSRC_LPUART_TX) /* LPUART1 transmit data source select: LPUART1_TX pin */
      | SIM_SOPT5_LPUART1RXSRC(SOPT5_LPUART1RXSRC_LPUART_RX) /* LPUART1 receive data source select: LPUART1_RX pin */
    );
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
