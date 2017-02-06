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

#define PIN0_IDX 0u                        /*!< Pin number for pin 0 in a port */
#define PIN2_IDX 2u                        /*!< Pin number for pin 2 in a port */
#define PIN3_IDX 3u                        /*!< Pin number for pin 3 in a port */
#define PIN4_IDX 4u                        /*!< Pin number for pin 4 in a port */
#define PIN5_IDX 5u                        /*!< Pin number for pin 5 in a port */
#define PIN6_IDX 6u                        /*!< Pin number for pin 6 in a port */
#define PIN7_IDX 7u                        /*!< Pin number for pin 7 in a port */
#define PIN8_IDX 8u                        /*!< Pin number for pin 8 in a port */
#define PIN9_IDX 9u                        /*!< Pin number for pin 9 in a port */
#define PIN13_IDX 13u                      /*!< Pin number for pin 13 in a port */
#define PIN18_IDX 18u                      /*!< Pin number for pin 18 in a port */
#define PIN19_IDX 19u                      /*!< Pin number for pin 19 in a port */
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
  - {pin_num: L11, peripheral: OSC, signal: EXTAL0, pin_signal: EXTAL0/PTA18/FTM0_FLT2/FTM_CLKIN0/TPM_CLKIN0}
  - {pin_num: K11, peripheral: OSC, signal: XTAL0, pin_signal:
XTAL0/PTA19/FTM1_FLT0/FTM_CLKIN1/LPTMR0_ALT1/LPTMR1_ALT1/TPM_CLKIN1}
  - {pin_num: B1, peripheral: RTC, signal: CLKOUT, pin_signal:
PTE0/SPI1_PCS1/LPUART1_TX/SDHC0_D1/QSPI0A_DATA3/I2C1_SDA/RTC_CLKOUT}
  - {pin_num: B11, peripheral: EMVSIM1, signal: IO, pin_signal: PTB4/EMVSIM1_IO/SDRAM_CS1_b/FTM1_FLT0, slew_rate: fast,
open_drain: enable, pull_select: down, pull_enable: disable}
  - {pin_num: C11, peripheral: EMVSIM1, signal: CLK, pin_signal: PTB5/EMVSIM1_CLK/FTM2_FLT0, slew_rate: fast,
open_drain: disable, pull_select: up, pull_enable: enable}
  - {pin_num: D11, peripheral: EMVSIM1, signal: RST, pin_signal: PTB8/EMVSIM1_RST/LPUART3_RTS_b/FB_AD21/SDRAM_D21,
slew_rate: fast, open_drain: disable, pull_select: up,
    pull_enable: enable}
  - {pin_num: D6, peripheral: GPIOC, signal: 'GPIO, 9', pin_signal:
CMP0_IN3/PTC9/FTM3_CH5/I2S0_RX_BCLK/FB_AD6/SDRAM_A14/FTM2_FLT0/FXIO0_D17, slew_rate: fast, open_drain: disable,
    pull_select: up, pull_enable: enable}
  - {pin_num: C8, peripheral: GPIOC, signal: 'GPIO, 2', pin_signal:
ADC0_SE4b/CMP1_IN0/TSI0_CH15/PTC2/SPI0_PCS2/LPUART1_CTS_b/FTM0_CH1/FB_AD12/SDRAM_A20/I2S0_TX_FS,
    slew_rate: fast, open_drain: disable, pull_select: up, pull_enable: enable}
  - {pin_num: E11, peripheral: EMVSIM1, signal: PD, pin_signal: PTB7/EMVSIM1_PD/FB_AD22/SDRAM_D22}
  - {pin_num: L8, peripheral: GPIOA, signal: 'GPIO, 13', pin_signal:
PTA13/LLWU_P4/FTM1_CH1/TRACE_D3/FXIO0_D19/I2S0_TX_FS/FTM1_QD_PHB/TPM1_CH1, slew_rate: fast, open_drain: disable,
    pull_select: up, pull_enable: enable}
  - {pin_num: F11, peripheral: EMVSIM1, signal: VCCEN, pin_signal: PTB6/EMVSIM1_VCCEN/FB_AD23/SDRAM_D23, slew_rate:
fast, open_drain: disable, pull_select: up, pull_enable: enable}
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
    CLOCK_EnableClock(kCLOCK_PortB); /* Port B Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC); /* Port C Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortE); /* Port E Clock Gate Control: Clock enabled */

    const port_pin_config_t porta13_pinL8_config = {
        kPORT_PullUp,               /* Internal pull-up resistor is enabled */
        kPORT_FastSlewRate,         /* Fast slew rate is configured */
        kPORT_PassiveFilterDisable, /* Passive filter is disabled */
        kPORT_OpenDrainDisable,     /* Open drain is disabled */
        kPORT_LowDriveStrength,     /* Low drive strength is configured */
        kPORT_MuxAsGpio,            /* Pin is configured as PTA13 */
        kPORT_UnlockRegister        /* Pin Control Register fields [15:0] are not locked */
    };
    PORT_SetPinConfig(PORTA, PIN13_IDX, &porta13_pinL8_config);  /* PORTA13 (pin L8) is configured as PTA13 */
    PORT_SetPinMux(PORTA, PIN18_IDX, kPORT_PinDisabledOrAnalog); /* PORTA18 (pin L11) is configured as EXTAL0 */
    PORT_SetPinMux(PORTA, PIN19_IDX, kPORT_PinDisabledOrAnalog); /* PORTA19 (pin K11) is configured as XTAL0 */
    const port_pin_config_t portb4_pinB11_config = {
        kPORT_PullDisable,          /* Internal pull-up/down resistor is disabled */
        kPORT_FastSlewRate,         /* Fast slew rate is configured */
        kPORT_PassiveFilterDisable, /* Passive filter is disabled */
        kPORT_OpenDrainEnable,      /* Open drain is enabled */
        kPORT_LowDriveStrength,     /* Low drive strength is configured */
        kPORT_MuxAlt2,              /* Pin is configured as EMVSIM1_IO */
        kPORT_UnlockRegister        /* Pin Control Register fields [15:0] are not locked */
    };
    PORT_SetPinConfig(PORTB, PIN4_IDX, &portb4_pinB11_config); /* PORTB4 (pin B11) is configured as EMVSIM1_IO */
    const port_pin_config_t portb5_pinC11_config = {
        kPORT_PullUp,               /* Internal pull-up resistor is enabled */
        kPORT_FastSlewRate,         /* Fast slew rate is configured */
        kPORT_PassiveFilterDisable, /* Passive filter is disabled */
        kPORT_OpenDrainDisable,     /* Open drain is disabled */
        kPORT_LowDriveStrength,     /* Low drive strength is configured */
        kPORT_MuxAlt2,              /* Pin is configured as EMVSIM1_CLK */
        kPORT_UnlockRegister        /* Pin Control Register fields [15:0] are not locked */
    };
    PORT_SetPinConfig(PORTB, PIN5_IDX, &portb5_pinC11_config); /* PORTB5 (pin C11) is configured as EMVSIM1_CLK */
    const port_pin_config_t portb6_pinF11_config = {
        kPORT_PullUp,               /* Internal pull-up resistor is enabled */
        kPORT_FastSlewRate,         /* Fast slew rate is configured */
        kPORT_PassiveFilterDisable, /* Passive filter is disabled */
        kPORT_OpenDrainDisable,     /* Open drain is disabled */
        kPORT_LowDriveStrength,     /* Low drive strength is configured */
        kPORT_MuxAlt2,              /* Pin is configured as EMVSIM1_VCCEN */
        kPORT_UnlockRegister        /* Pin Control Register fields [15:0] are not locked */
    };
    PORT_SetPinConfig(PORTB, PIN6_IDX, &portb6_pinF11_config); /* PORTB6 (pin F11) is configured as EMVSIM1_VCCEN */
    PORT_SetPinMux(PORTB, PIN7_IDX, kPORT_MuxAlt2);            /* PORTB7 (pin E11) is configured as EMVSIM1_PD */
    const port_pin_config_t portb8_pinD11_config = {
        kPORT_PullUp,               /* Internal pull-up resistor is enabled */
        kPORT_FastSlewRate,         /* Fast slew rate is configured */
        kPORT_PassiveFilterDisable, /* Passive filter is disabled */
        kPORT_OpenDrainDisable,     /* Open drain is disabled */
        kPORT_LowDriveStrength,     /* Low drive strength is configured */
        kPORT_MuxAlt2,              /* Pin is configured as EMVSIM1_RST */
        kPORT_UnlockRegister        /* Pin Control Register fields [15:0] are not locked */
    };
    PORT_SetPinConfig(PORTB, PIN8_IDX, &portb8_pinD11_config); /* PORTB8 (pin D11) is configured as EMVSIM1_RST */
    const port_pin_config_t portc2_pinC8_config = {
        kPORT_PullUp,               /* Internal pull-up resistor is enabled */
        kPORT_FastSlewRate,         /* Fast slew rate is configured */
        kPORT_PassiveFilterDisable, /* Passive filter is disabled */
        kPORT_OpenDrainDisable,     /* Open drain is disabled */
        kPORT_LowDriveStrength,     /* Low drive strength is configured */
        kPORT_MuxAsGpio,            /* Pin is configured as PTC2 */
        kPORT_UnlockRegister        /* Pin Control Register fields [15:0] are not locked */
    };
    PORT_SetPinConfig(PORTC, PIN2_IDX, &portc2_pinC8_config); /* PORTC2 (pin C8) is configured as PTC2 */
    PORT_SetPinMux(PORTC, PIN3_IDX, kPORT_MuxAlt3);           /* PORTC3 (pin B8) is configured as LPUART1_RX */
    PORT_SetPinMux(PORTC, PIN4_IDX, kPORT_MuxAlt3);           /* PORTC4 (pin A8) is configured as LPUART1_TX */
    const port_pin_config_t portc9_pinD6_config = {
        kPORT_PullUp,               /* Internal pull-up resistor is enabled */
        kPORT_FastSlewRate,         /* Fast slew rate is configured */
        kPORT_PassiveFilterDisable, /* Passive filter is disabled */
        kPORT_OpenDrainDisable,     /* Open drain is disabled */
        kPORT_LowDriveStrength,     /* Low drive strength is configured */
        kPORT_MuxAsGpio,            /* Pin is configured as PTC9 */
        kPORT_UnlockRegister        /* Pin Control Register fields [15:0] are not locked */
    };
    PORT_SetPinConfig(PORTC, PIN9_IDX, &portc9_pinD6_config); /* PORTC9 (pin D6) is configured as PTC9 */
    PORT_SetPinMux(PORTE, PIN0_IDX, kPORT_MuxAlt7);           /* PORTE0 (pin B1) is configured as RTC_CLKOUT */
    SIM->SOPT5 =
        ((SIM->SOPT5 &
          (~(SIM_SOPT5_LPUART1TXSRC_MASK | SIM_SOPT5_LPUART1RXSRC_MASK))) /* Mask bits to zero which are setting */
         |
         SIM_SOPT5_LPUART1TXSRC(SOPT5_LPUART1TXSRC_LPUART_TX) /* LPUART1 transmit data source select: LPUART1_TX pin */
         | SIM_SOPT5_LPUART1RXSRC(SOPT5_LPUART1RXSRC_LPUART_RX) /* LPUART1 receive data source select: LPUART1_RX pin */
         );
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
