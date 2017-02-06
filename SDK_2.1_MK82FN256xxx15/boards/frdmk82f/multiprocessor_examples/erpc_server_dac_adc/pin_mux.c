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
- !!package 'MK82FN256VLQ15'
- !!mcu_data 'ksdk2_0'
- !!processor_version '1.0.0'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"

#define PIN8_IDX 8u   /*!< Pin number for pin 8 in a port */
#define PIN9_IDX 9u   /*!< Pin number for pin 9 in a port */
#define PIN10_IDX 10u /*!< Pin number for pin 10 in a port */
#define PIN14_IDX 14u /*!< Pin number for pin 10 in a port */
#define PIN15_IDX 15u /*!< Pin number for pin 10 in a port */

#define SOPT5_LPUART0TXSRC_LPUART_TX 0x00u /*!< UART 0 transmit data source select: UART0_TX pin */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
BOARD_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '113', peripheral: GPIOC, signal: 'GPIO, 8', pin_signal:
CMP0_IN2/PTC8/FTM3_CH4/I2S0_MCLK/FB_AD7/SDRAM_A15/FXIO0_D16, direction: OUTPUT}
  - {pin_num: '114', peripheral: GPIOC, signal: 'GPIO, 9', pin_signal:
CMP0_IN3/PTC9/FTM3_CH5/I2S0_RX_BCLK/FB_AD6/SDRAM_A14/FTM2_FLT0/FXIO0_D17, direction: OUTPUT}
  - {pin_num: '115', peripheral: GPIOC, signal: 'GPIO, 10', pin_signal:
PTC10/I2C1_SCL/FTM3_CH6/I2S0_RX_FS/FB_AD5/SDRAM_A13/FXIO0_D18, direction: OUTPUT}
  - {pin_num: '51', peripheral: I2C3, signal: SDA, pin_signal:
TSI0_CH2/PTA1/LPUART0_RX/FTM0_CH6/I2C3_SDA/FXIO0_D11/EMVSIM0_IO/JTAG_TDI}
  - {pin_num: '52', peripheral: I2C3, signal: SCL, pin_signal:
TSI0_CH3/PTA2/LPUART0_TX/FTM0_CH7/I2C3_SCL/FXIO0_D12/EMVSIM0_PD/JTAG_TDO/TRACE_SWO}
  - {pin_num: '40', peripheral: DAC0, signal: OUT, pin_signal: DAC0_OUT/CMP1_IN3/ADC0_SE23}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void BOARD_InitPins(void)
{
    CLOCK_EnableClock(kCLOCK_PortA); /* Port A Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC); /* Port C Clock Gate Control: Clock enabled */

    PORT_SetPinMux(PORTC, PIN10_IDX, kPORT_MuxAsGpio); /* PORTC10 (pin 115) is configured as PTC10 */
    PORT_SetPinMux(PORTC, PIN8_IDX, kPORT_MuxAsGpio);  /* PORTC8 (pin 113) is configured as PTC8 */
    PORT_SetPinMux(PORTC, PIN9_IDX, kPORT_MuxAsGpio);  /* PORTC9 (pin 114) is configured as PTC9 */

    PORT_SetPinMux(PORTC, PIN14_IDX, kPORT_MuxAlt3); /* PORTC14 (pin 86) is configured as LPUART4_RX */
    PORT_SetPinMux(PORTC, PIN15_IDX, kPORT_MuxAlt3); /* PORTC15 (pin 87) is configured as LPUART4_TX */

    SIM->SOPT5 =
        ((SIM->SOPT5 & (~(SIM_SOPT5_LPUART0TXSRC_MASK)))        /* Mask bits to zero which are setting */
         | SIM_SOPT5_LPUART0TXSRC(SOPT5_LPUART0TXSRC_LPUART_TX) /* UART 0 transmit data source select: UART0_TX pin */
         );
}

#define PIN1_IDX 1u /*!< Pin number for pin 1 in a port */
#define PIN2_IDX 2u /*!< Pin number for pin 2 in a port */
                    /*
                     * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
                    BOARD_I2C_ConfigurePins:
                    - options: {coreID: singlecore, enableClock: 'true'}
                    - pin_list:
                      - {pin_num: '37', peripheral: I2C3, signal: SDA, pin_signal:
                    TSI0_CH2/PTA1/LPUART0_RX/FTM0_CH6/I2C3_SDA/FXIO0_D11/EMVSIM0_IO/JTAG_TDI, slew_rate: fast, open_drain: enable,
                        pull_select: up, pull_enable: enable}
                      - {pin_num: '38', peripheral: I2C3, signal: SCL, pin_signal:
                    TSI0_CH3/PTA2/LPUART0_TX/FTM0_CH7/I2C3_SCL/FXIO0_D12/EMVSIM0_PD/JTAG_TDO/TRACE_SWO, slew_rate: fast,
                        open_drain: enable, pull_select: up, pull_enable: enable}
                     * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
                     */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_I2C_ConfigurePins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void BOARD_I2C_ConfigurePins(void)
{
    CLOCK_EnableClock(kCLOCK_PortA); /* Port A Clock Gate Control: Clock enabled */

    const port_pin_config_t porta1_pin37_config = {
        kPORT_PullUp,               /* Internal pull-up resistor is enabled */
        kPORT_FastSlewRate,         /* Fast slew rate is configured */
        kPORT_PassiveFilterDisable, /* Passive filter is disabled */
        kPORT_OpenDrainEnable,      /* Open drain is enabled */
        kPORT_LowDriveStrength,     /* Low drive strength is configured */
        kPORT_MuxAlt4,              /* Pin is configured as I2C3_SDA */
        kPORT_UnlockRegister        /* Pin Control Register fields [15:0] are not locked */
    };
    PORT_SetPinConfig(PORTA, PIN1_IDX, &porta1_pin37_config); /* PORTA1 (pin 37) is configured as I2C3_SDA */
    const port_pin_config_t porta2_pin38_config = {
        kPORT_PullUp,               /* Internal pull-up resistor is enabled */
        kPORT_FastSlewRate,         /* Fast slew rate is configured */
        kPORT_PassiveFilterDisable, /* Passive filter is disabled */
        kPORT_OpenDrainEnable,      /* Open drain is enabled */
        kPORT_LowDriveStrength,     /* Low drive strength is configured */
        kPORT_MuxAlt4,              /* Pin is configured as I2C3_SCL */
        kPORT_UnlockRegister        /* Pin Control Register fields [15:0] are not locked */
    };
    PORT_SetPinConfig(PORTA, PIN2_IDX, &porta2_pin38_config); /* PORTA2 (pin 38) is configured as I2C3_SCL */
}

#define PIN6_IDX 6u                        /*!< Pin number for pin 6 in a port */
#define PIN14_IDX 14u                      /*!< Pin number for pin 14 in a port */
#define SOPT5_LPUART0RXSRC_LPUART_RX 0x00u /*!< LPUART0 receive data source select: LPUART0_RX pin */
#define SOPT5_LPUART0TXSRC_LPUART_TX 0x00u /*!< LPUART0 transmit data source select: LPUART0_TX pin */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
LPUART0_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '44', peripheral: LPUART0, signal: TX, pin_signal:
PTA14/SPI0_PCS0/LPUART0_TX/TRACE_D2/FXIO0_D20/I2S0_RX_BCLK/I2S0_TXD1}
  - {pin_num: '97', peripheral: LPUART0, signal: RX, pin_signal:
ADC0_SE7b/PTD6/LLWU_P15/SPI0_PCS3/LPUART0_RX/FTM0_CH6/FB_AD0/FTM0_FLT0/SPI1_SOUT}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : LPUART0_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void LPUART0_InitPins(void)
{
    CLOCK_EnableClock(kCLOCK_PortA); /* Port A Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortD); /* Port D Clock Gate Control: Clock enabled */

    PORT_SetPinMux(PORTA, PIN14_IDX, kPORT_MuxAlt3); /* PORTA14 (pin 44) is configured as LPUART0_TX */
    PORT_SetPinMux(PORTD, PIN6_IDX, kPORT_MuxAlt3);  /* PORTD6 (pin 97) is configured as LPUART0_RX */
    SIM->SOPT5 =
        ((SIM->SOPT5 &
          (~(SIM_SOPT5_LPUART0TXSRC_MASK | SIM_SOPT5_LPUART0RXSRC_MASK))) /* Mask bits to zero which are setting */
         |
         SIM_SOPT5_LPUART0TXSRC(SOPT5_LPUART0TXSRC_LPUART_TX) /* LPUART0 transmit data source select: LPUART0_TX pin */
         | SIM_SOPT5_LPUART0RXSRC(SOPT5_LPUART0RXSRC_LPUART_RX) /* LPUART0 receive data source select: LPUART0_RX pin */
         );
}

#define PIN6_IDX 6u   /*!< Pin number for pin 6 in a port */
#define PIN14_IDX 14u /*!< Pin number for pin 14 in a port */
                      /*
                       * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
                      LPUART0_DeinitPins:
                      - options: {coreID: singlecore, enableClock: 'false'}
                      - pin_list:
                        - {pin_num: '44', peripheral: n/a, signal: disabled, pin_signal:
                      PTA14/SPI0_PCS0/LPUART0_TX/TRACE_D2/FXIO0_D20/I2S0_RX_BCLK/I2S0_TXD1}
                        - {pin_num: '97', peripheral: ADC0, signal: 'SE, 7b', pin_signal:
                      ADC0_SE7b/PTD6/LLWU_P15/SPI0_PCS3/LPUART0_RX/FTM0_CH6/FB_AD0/FTM0_FLT0/SPI1_SOUT}
                       * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
                       */

/*FUNCTION**********************************************************************
 *
 * Function Name : LPUART0_DeinitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void LPUART0_DeinitPins(void)
{
    PORT_SetPinMux(PORTA, PIN14_IDX, kPORT_PinDisabledOrAnalog); /* PORTA14 (pin 44) is disabled */
    PORT_SetPinMux(PORTD, PIN6_IDX, kPORT_PinDisabledOrAnalog);  /* PORTD6 (pin 97) is configured as ADC0_SE7b */
}

#define PIN3_IDX 3u                        /*!< Pin number for pin 3 in a port */
#define PIN4_IDX 4u                        /*!< Pin number for pin 4 in a port */
#define SOPT5_LPUART1RXSRC_LPUART_RX 0x00u /*!< LPUART1 receive data source select: LPUART1_RX pin */
#define SOPT5_LPUART1TXSRC_LPUART_TX 0x00u /*!< LPUART1 transmit data source select: LPUART1_TX pin */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
LPUART1_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '73', peripheral: LPUART1, signal: RX, pin_signal:
CMP1_IN1/PTC3/LLWU_P7/SPI0_PCS1/LPUART1_RX/FTM0_CH2/CLKOUT/I2S0_TX_BCLK}
  - {pin_num: '76', peripheral: LPUART1, signal: TX, pin_signal:
PTC4/LLWU_P8/SPI0_PCS0/LPUART1_TX/FTM0_CH3/FB_AD11/SDRAM_A19/CMP1_OUT}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : LPUART1_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void LPUART1_InitPins(void)
{
    CLOCK_EnableClock(kCLOCK_PortC); /* Port C Clock Gate Control: Clock enabled */

    PORT_SetPinMux(PORTC, PIN3_IDX, kPORT_MuxAlt3); /* PORTC3 (pin 73) is configured as LPUART1_RX */
    PORT_SetPinMux(PORTC, PIN4_IDX, kPORT_MuxAlt3); /* PORTC4 (pin 76) is configured as LPUART1_TX */
    SIM->SOPT5 =
        ((SIM->SOPT5 &
          (~(SIM_SOPT5_LPUART1TXSRC_MASK | SIM_SOPT5_LPUART1RXSRC_MASK))) /* Mask bits to zero which are setting */
         |
         SIM_SOPT5_LPUART1TXSRC(SOPT5_LPUART1TXSRC_LPUART_TX) /* LPUART1 transmit data source select: LPUART1_TX pin */
         | SIM_SOPT5_LPUART1RXSRC(SOPT5_LPUART1RXSRC_LPUART_RX) /* LPUART1 receive data source select: LPUART1_RX pin */
         );
}

#define PIN3_IDX 3u /*!< Pin number for pin 3 in a port */
#define PIN4_IDX 4u /*!< Pin number for pin 4 in a port */
                    /*
                     * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
                    LPUART1_DeinitPins:
                    - options: {coreID: singlecore, enableClock: 'false'}
                    - pin_list:
                      - {pin_num: '73', peripheral: CMP1, signal: 'IN, 1', pin_signal:
                    CMP1_IN1/PTC3/LLWU_P7/SPI0_PCS1/LPUART1_RX/FTM0_CH2/CLKOUT/I2S0_TX_BCLK}
                      - {pin_num: '76', peripheral: n/a, signal: disabled, pin_signal:
                    PTC4/LLWU_P8/SPI0_PCS0/LPUART1_TX/FTM0_CH3/FB_AD11/SDRAM_A19/CMP1_OUT}
                     * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
                     */

/*FUNCTION**********************************************************************
 *
 * Function Name : LPUART1_DeinitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void LPUART1_DeinitPins(void)
{
    PORT_SetPinMux(PORTC, PIN3_IDX, kPORT_PinDisabledOrAnalog); /* PORTC3 (pin 73) is configured as CMP1_IN1 */
    PORT_SetPinMux(PORTC, PIN4_IDX, kPORT_PinDisabledOrAnalog); /* PORTC4 (pin 76) is disabled */
}

#define PIN2_IDX 2u /*!< Pin number for pin 2 in a port */
#define PIN3_IDX 3u /*!< Pin number for pin 3 in a port */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
LPUART2_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '93', peripheral: LPUART2, signal: RX, pin_signal:
PTD2/LLWU_P13/SPI0_SOUT/LPUART2_RX/FTM3_CH2/FB_AD4/SDRAM_A12/I2C0_SCL}
  - {pin_num: '94', peripheral: LPUART2, signal: TX, pin_signal:
PTD3/SPI0_SIN/LPUART2_TX/FTM3_CH3/FB_AD3/SDRAM_A11/I2C0_SDA}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : LPUART2_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void LPUART2_InitPins(void)
{
    CLOCK_EnableClock(kCLOCK_PortD); /* Port D Clock Gate Control: Clock enabled */

    PORT_SetPinMux(PORTD, PIN2_IDX, kPORT_MuxAlt3); /* PORTD2 (pin 93) is configured as LPUART2_RX */
    PORT_SetPinMux(PORTD, PIN3_IDX, kPORT_MuxAlt3); /* PORTD3 (pin 94) is configured as LPUART2_TX */
}

#define PIN2_IDX 2u /*!< Pin number for pin 2 in a port */
#define PIN3_IDX 3u /*!< Pin number for pin 3 in a port */
                    /*
                     * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
                    LPUART2_DeinitPins:
                    - options: {coreID: singlecore, enableClock: 'false'}
                    - pin_list:
                      - {pin_num: '93', peripheral: n/a, signal: disabled, pin_signal:
                    PTD2/LLWU_P13/SPI0_SOUT/LPUART2_RX/FTM3_CH2/FB_AD4/SDRAM_A12/I2C0_SCL}
                      - {pin_num: '94', peripheral: n/a, signal: disabled, pin_signal:
                    PTD3/SPI0_SIN/LPUART2_TX/FTM3_CH3/FB_AD3/SDRAM_A11/I2C0_SDA}
                     * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
                     */

/*FUNCTION**********************************************************************
 *
 * Function Name : LPUART2_DeinitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void LPUART2_DeinitPins(void)
{
    PORT_SetPinMux(PORTD, PIN2_IDX, kPORT_PinDisabledOrAnalog); /* PORTD2 (pin 93) is disabled */
    PORT_SetPinMux(PORTD, PIN3_IDX, kPORT_PinDisabledOrAnalog); /* PORTD3 (pin 94) is disabled */
}

#define PIN10_IDX 10u /*!< Pin number for pin 10 in a port */
#define PIN11_IDX 11u /*!< Pin number for pin 11 in a port */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
LPUART3_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '58', peripheral: LPUART3, signal: RX, pin_signal:
PTB10/SPI1_PCS0/LPUART3_RX/I2C2_SCL/FB_AD19/SDRAM_D19/FTM0_FLT1/FXIO0_D4}
  - {pin_num: '59', peripheral: LPUART3, signal: TX, pin_signal:
PTB11/SPI1_SCK/LPUART3_TX/I2C2_SDA/FB_AD18/SDRAM_D18/FTM0_FLT2/FXIO0_D5}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : LPUART3_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void LPUART3_InitPins(void)
{
    CLOCK_EnableClock(kCLOCK_PortB); /* Port B Clock Gate Control: Clock enabled */

    PORT_SetPinMux(PORTB, PIN10_IDX, kPORT_MuxAlt3); /* PORTB10 (pin 58) is configured as LPUART3_RX */
    PORT_SetPinMux(PORTB, PIN11_IDX, kPORT_MuxAlt3); /* PORTB11 (pin 59) is configured as LPUART3_TX */
}

#define PIN10_IDX 10u /*!< Pin number for pin 10 in a port */
#define PIN11_IDX 11u /*!< Pin number for pin 11 in a port */
                      /*
                       * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
                      LPUART3_DeinitPins:
                      - options: {coreID: singlecore, enableClock: 'false'}
                      - pin_list:
                        - {pin_num: '58', peripheral: n/a, signal: disabled, pin_signal:
                      PTB10/SPI1_PCS0/LPUART3_RX/I2C2_SCL/FB_AD19/SDRAM_D19/FTM0_FLT1/FXIO0_D4}
                        - {pin_num: '59', peripheral: n/a, signal: disabled, pin_signal:
                      PTB11/SPI1_SCK/LPUART3_TX/I2C2_SDA/FB_AD18/SDRAM_D18/FTM0_FLT2/FXIO0_D5}
                       * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
                       */

/*FUNCTION**********************************************************************
 *
 * Function Name : LPUART3_DeinitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void LPUART3_DeinitPins(void)
{
    PORT_SetPinMux(PORTB, PIN10_IDX, kPORT_PinDisabledOrAnalog); /* PORTB10 (pin 58) is disabled */
    PORT_SetPinMux(PORTB, PIN11_IDX, kPORT_PinDisabledOrAnalog); /* PORTB11 (pin 59) is disabled */
}

#define PIN14_IDX 14u /*!< Pin number for pin 14 in a port */
#define PIN15_IDX 15u /*!< Pin number for pin 15 in a port */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
LPUART4_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '86', peripheral: LPUART4, signal: RX, pin_signal: PTC14/LPUART4_RX/FB_AD25/SDRAM_D25/FXIO0_D20}
  - {pin_num: '87', peripheral: LPUART4, signal: TX, pin_signal: PTC15/LPUART4_TX/FB_AD24/SDRAM_D24/FXIO0_D21}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : LPUART4_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void LPUART4_InitPins(void)
{
    CLOCK_EnableClock(kCLOCK_PortC); /* Port C Clock Gate Control: Clock enabled */

    PORT_SetPinMux(PORTC, PIN14_IDX, kPORT_MuxAlt3); /* PORTC14 (pin 86) is configured as LPUART4_RX */
    PORT_SetPinMux(PORTC, PIN15_IDX, kPORT_MuxAlt3); /* PORTC15 (pin 87) is configured as LPUART4_TX */
}

#define PIN14_IDX 14u /*!< Pin number for pin 14 in a port */
#define PIN15_IDX 15u /*!< Pin number for pin 15 in a port */
                      /*
                       * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
                      LPUART4_DeinitPins:
                      - options: {coreID: singlecore, enableClock: 'false'}
                      - pin_list:
                        - {pin_num: '87', peripheral: n/a, signal: disabled, pin_signal: PTC15/LPUART4_TX/FB_AD24/SDRAM_D24/FXIO0_D21}
                        - {pin_num: '86', peripheral: n/a, signal: disabled, pin_signal: PTC14/LPUART4_RX/FB_AD25/SDRAM_D25/FXIO0_D20}
                       * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
                       */

/*FUNCTION**********************************************************************
 *
 * Function Name : LPUART4_DeinitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void LPUART4_DeinitPins(void)
{
    PORT_SetPinMux(PORTC, PIN14_IDX, kPORT_PinDisabledOrAnalog); /* PORTC14 (pin 86) is disabled */
    PORT_SetPinMux(PORTC, PIN15_IDX, kPORT_PinDisabledOrAnalog); /* PORTC15 (pin 87) is disabled */
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
