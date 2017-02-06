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
- !!processor_version '1.0.0'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"

#define PIN3_IDX 3u                        /*!< Pin number for pin 3 in a port */
#define PIN4_IDX 4u                        /*!< Pin number for pin 4 in a port */
#define PIN5_IDX 5u                        /*!< Pin number for pin 5 in a port */
#define PIN12_IDX 12u                      /*!< Pin number for pin 12 in a port */
#define PIN13_IDX 13u                      /*!< Pin number for pin 13 in a port */
#define PIN15_IDX 15u                      /*!< Pin number for pin 15 in a port */
#define PIN17_IDX 17u                      /*!< Pin number for pin 17 in a port */
#define SOPT5_LPUART1RXSRC_LPUART_RX 0x00u /*!< LPUART1 receive data source select: LPUART1_RX pin */
#define SOPT5_LPUART1TXSRC_LPUART_TX 0x00u /*!< LPUART1 transmit data source select: LPUART1_TX pin */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
BOARD_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: B8, peripheral: LPUART1, signal: RX, pin_signal:
CMP1_IN1/PTC3/LLWU_P7/SPI0_PCS1/LPUART1_RX/FTM0_CH2/CLKOUT/I2S0_TX_BCLK}
  - {pin_num: A8, peripheral: LPUART1, signal: TX, pin_signal:
PTC4/LLWU_P8/SPI0_PCS0/LPUART1_TX/FTM0_CH3/FB_AD11/SDRAM_A19/CMP1_OUT}
  - {pin_num: H10, peripheral: I2S0, signal: MCLK, pin_signal: PTA17/SPI0_SIN/LPUART0_RTS_b/FXIO0_D23/I2S0_MCLK}
  - {pin_num: K7, peripheral: I2S0, signal: TX_BCLK, pin_signal:
PTA5/USB0_CLKIN/FTM0_CH2/FXIO0_D15/I2S0_TX_BCLK/JTAG_TRST_b}
  - {pin_num: L8, peripheral: I2S0, signal: TX_FS, pin_signal:
PTA13/LLWU_P4/FTM1_CH1/TRACE_D3/FXIO0_D19/I2S0_TX_FS/FTM1_QD_PHB/TPM1_CH1}
  - {pin_num: L9, peripheral: I2S0, signal: RXD0, pin_signal: PTA15/SPI0_SCK/LPUART0_RX/TRACE_D1/FXIO0_D21/I2S0_RXD0}
  - {pin_num: K8, peripheral: I2S0, signal: TXD0, pin_signal:
PTA12/FTM1_CH0/TRACE_CLKOUT/FXIO0_D18/I2S0_TXD0/FTM1_QD_PHA/TPM1_CH0}
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

    PORT_SetPinMux(PORTA, PIN12_IDX, kPORT_MuxAlt6); /* PORTA12 (pin K8) is configured as I2S0_TXD0 */
    PORT_SetPinMux(PORTA, PIN13_IDX, kPORT_MuxAlt6); /* PORTA13 (pin L8) is configured as I2S0_TX_FS */
    PORT_SetPinMux(PORTA, PIN15_IDX, kPORT_MuxAlt6); /* PORTA15 (pin L9) is configured as I2S0_RXD0 */
    PORT_SetPinMux(PORTA, PIN17_IDX, kPORT_MuxAlt6); /* PORTA17 (pin H10) is configured as I2S0_MCLK */
    PORT_SetPinMux(PORTA, PIN5_IDX, kPORT_MuxAlt6);  /* PORTA5 (pin K7) is configured as I2S0_TX_BCLK */
    PORT_SetPinMux(PORTC, PIN3_IDX, kPORT_MuxAlt3);  /* PORTC3 (pin B8) is configured as LPUART1_RX */
    PORT_SetPinMux(PORTC, PIN4_IDX, kPORT_MuxAlt3);  /* PORTC4 (pin A8) is configured as LPUART1_TX */
    SIM->SOPT5 =
        ((SIM->SOPT5 &
          (~(SIM_SOPT5_LPUART1TXSRC_MASK | SIM_SOPT5_LPUART1RXSRC_MASK))) /* Mask bits to zero which are setting */
         |
         SIM_SOPT5_LPUART1TXSRC(SOPT5_LPUART1TXSRC_LPUART_TX) /* LPUART1 transmit data source select: LPUART1_TX pin */
         | SIM_SOPT5_LPUART1RXSRC(SOPT5_LPUART1RXSRC_LPUART_RX) /* LPUART1 receive data source select: LPUART1_RX pin */
         );
}

#define PIN8_IDX 8u /*!< Pin number for pin 8 in a port */
#define PIN9_IDX 9u /*!< Pin number for pin 9 in a port */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
BOARD_I2C_ConfigurePins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: A10, peripheral: I2C0, signal: SCL, pin_signal: PTD8/LLWU_P24/I2C0_SCL/FB_A16/FXIO0_D24, slew_rate: fast,
open_drain: enable, pull_select: up, pull_enable: enable}
  - {pin_num: A9, peripheral: I2C0, signal: SDA, pin_signal: PTD9/I2C0_SDA/FB_A17/FXIO0_D25, slew_rate: fast,
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
    CLOCK_EnableClock(kCLOCK_PortD); /* Port D Clock Gate Control: Clock enabled */

    const port_pin_config_t portd8_pinA10_config = {
        kPORT_PullUp,               /* Internal pull-up resistor is enabled */
        kPORT_FastSlewRate,         /* Fast slew rate is configured */
        kPORT_PassiveFilterDisable, /* Passive filter is disabled */
        kPORT_OpenDrainEnable,      /* Open drain is enabled */
        kPORT_LowDriveStrength,     /* Low drive strength is configured */
        kPORT_MuxAlt2,              /* Pin is configured as I2C0_SCL */
        kPORT_UnlockRegister        /* Pin Control Register fields [15:0] are not locked */
    };
    PORT_SetPinConfig(PORTD, PIN8_IDX, &portd8_pinA10_config); /* PORTD8 (pin A10) is configured as I2C0_SCL */
    const port_pin_config_t portd9_pinA9_config = {
        kPORT_PullUp,               /* Internal pull-up resistor is enabled */
        kPORT_FastSlewRate,         /* Fast slew rate is configured */
        kPORT_PassiveFilterDisable, /* Passive filter is disabled */
        kPORT_OpenDrainEnable,      /* Open drain is enabled */
        kPORT_LowDriveStrength,     /* Low drive strength is configured */
        kPORT_MuxAlt2,              /* Pin is configured as I2C0_SDA */
        kPORT_UnlockRegister        /* Pin Control Register fields [15:0] are not locked */
    };
    PORT_SetPinConfig(PORTD, PIN9_IDX, &portd9_pinA9_config); /* PORTD9 (pin A9) is configured as I2C0_SDA */
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
