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

#define PCR_ODE_ENABLED               0x01u   /*!< Open Drain Enable: Open drain output is enabled on the corresponding pin, if the pin is configured as a digital output. */
#define PCR_PE_ENABLED                0x01u   /*!< Pull Enable: Internal pullup or pulldown resistor is enabled on the corresponding pin, if the pin is configured as a digital input. */
#define PCR_PS_UP                     0x01u   /*!< Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the corresponding PE field is set. */
#define PCR_DSE_HIGH                  0x01u   /*!< Drive Strength Enable: High drive strength is configured on the corresponding pin, if pin is configured as a digital output. */
#define PIN0_IDX                         0u   /*!< Pin number for pin 0 in a port */
#define PIN2_IDX                         2u   /*!< Pin number for pin 2 in a port */
#define PIN3_IDX                         3u   /*!< Pin number for pin 3 in a port */
#define PIN4_IDX                         4u   /*!< Pin number for pin 4 in a port */
#define PIN5_IDX                         5u   /*!< Pin number for pin 5 in a port */
#define PIN6_IDX                         6u   /*!< Pin number for pin 6 in a port */
#define PIN7_IDX                         7u   /*!< Pin number for pin 7 in a port */
#define PIN8_IDX                         8u   /*!< Pin number for pin 8 in a port */
#define PIN9_IDX                         9u   /*!< Pin number for pin 9 in a port */
#define PIN13_IDX                       13u   /*!< Pin number for pin 13 in a port */
#define PIN18_IDX                       18u   /*!< Pin number for pin 18 in a port */
#define PIN19_IDX                       19u   /*!< Pin number for pin 19 in a port */
#define SOPT5_LPUART1RXSRC_LPUART_RX  0x00u   /*!< LPUART1 receive data source select: LPUART1_RX pin */
#define SOPT5_LPUART1TXSRC_LPUART_TX  0x00u   /*!< LPUART1 transmit data source select: LPUART1_TX pin */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
BOARD_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: B8, peripheral: LPUART1, signal: RX, pin_signal: CMP1_IN1/PTC3/LLWU_P7/SPI0_PCS1/LPUART1_RX/FTM0_CH2/CLKOUT/I2S0_TX_BCLK}
  - {pin_num: A8, peripheral: LPUART1, signal: TX, pin_signal: PTC4/LLWU_P8/SPI0_PCS0/LPUART1_TX/FTM0_CH3/FB_AD11/SDRAM_A19/CMP1_OUT}
  - {pin_num: L11, peripheral: OSC, signal: EXTAL0, pin_signal: EXTAL0/PTA18/FTM0_FLT2/FTM_CLKIN0/TPM_CLKIN0}
  - {pin_num: K11, peripheral: OSC, signal: XTAL0, pin_signal: XTAL0/PTA19/FTM1_FLT0/FTM_CLKIN1/LPTMR0_ALT1/LPTMR1_ALT1/TPM_CLKIN1}
  - {pin_num: B1, peripheral: RTC, signal: CLKOUT, pin_signal: PTE0/SPI1_PCS1/LPUART1_TX/SDHC0_D1/QSPI0A_DATA3/I2C1_SDA/RTC_CLKOUT}
  - {pin_num: B11, peripheral: EMVSIM1, signal: IO, pin_signal: PTB4/EMVSIM1_IO/SDRAM_CS1_b/FTM1_FLT0, open_drain: enable}
  - {pin_num: C11, peripheral: EMVSIM1, signal: CLK, pin_signal: PTB5/EMVSIM1_CLK/FTM2_FLT0, pull_select: up, pull_enable: enable}
  - {pin_num: D11, peripheral: EMVSIM1, signal: RST, pin_signal: PTB8/EMVSIM1_RST/LPUART3_RTS_b/FB_AD21/SDRAM_D21, pull_select: up, pull_enable: enable}
  - {pin_num: C8, peripheral: GPIOC, signal: 'GPIO, 2', pin_signal: ADC0_SE4b/CMP1_IN0/TSI0_CH15/PTC2/SPI0_PCS2/LPUART1_CTS_b/FTM0_CH1/FB_AD12/SDRAM_A20/I2S0_TX_FS,
    pull_select: up, pull_enable: enable}
  - {pin_num: D6, peripheral: GPIOC, signal: 'GPIO, 9', pin_signal: CMP0_IN3/PTC9/FTM3_CH5/I2S0_RX_BCLK/FB_AD6/SDRAM_A14/FTM2_FLT0/FXIO0_D17, pull_select: up, pull_enable: enable}
  - {pin_num: E11, peripheral: EMVSIM1, signal: PD, pin_signal: PTB7/EMVSIM1_PD/FB_AD22/SDRAM_D22}
  - {pin_num: L8, peripheral: GPIOA, signal: 'GPIO, 13', pin_signal: PTA13/LLWU_P4/FTM1_CH1/TRACE_D3/FXIO0_D19/I2S0_TX_FS/FTM1_QD_PHB/TPM1_CH1, pull_select: up, pull_enable: enable}
  - {pin_num: F11, peripheral: EMVSIM1, signal: VCCEN, pin_signal: PTB6/EMVSIM1_VCCEN/FB_AD23/SDRAM_D23, pull_select: up, pull_enable: enable}
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
  CLOCK_EnableClock(kCLOCK_PortE);                           /* Port E Clock Gate Control: Clock enabled */

  PORT_SetPinMux(PORTA, PIN13_IDX, kPORT_MuxAsGpio);         /* PORTA13 (pin L8) is configured as PTA13 */
  PORTA->PCR[13] = ((PORTA->PCR[13] &
    (~(PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_ISF_MASK))) /* Mask bits to zero which are setting */
      | PORT_PCR_PS(PCR_PS_UP)                               /* Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the corresponding PE field is set. */
      | PORT_PCR_PE(PCR_PE_ENABLED)                          /* Pull Enable: Internal pullup or pulldown resistor is enabled on the corresponding pin, if the pin is configured as a digital input. */
      | PORT_PCR_DSE(PCR_DSE_HIGH)                           /* Drive Strength Enable: High drive strength is configured on the corresponding pin, if pin is configured as a digital output. */
    );
  /* Initialize pin interrupt */
  PORT_SetPinInterruptConfig(PORTA, 13u, kPORT_InterruptEitherEdge);
  /* Clear interrupt status flags */
  PORT_ClearPinsInterruptFlags(PORTA, 1u << 13u);
  PORT_SetPinMux(PORTA, PIN18_IDX, kPORT_PinDisabledOrAnalog); /* PORTA18 (pin L11) is configured as EXTAL0 */
  PORT_SetPinMux(PORTA, PIN19_IDX, kPORT_PinDisabledOrAnalog); /* PORTA19 (pin K11) is configured as XTAL0 */
  PORT_SetPinMux(PORTB, PIN4_IDX, kPORT_MuxAlt2);            /* PORTB4 (pin B11) is configured as EMVSIM1_IO */
  PORTB->PCR[4] = ((PORTB->PCR[4] &
    (~(PORT_PCR_ODE_MASK | PORT_PCR_ISF_MASK)))              /* Mask bits to zero which are setting */
      | PORT_PCR_ODE(PCR_ODE_ENABLED)                        /* Open Drain Enable: Open drain output is enabled on the corresponding pin, if the pin is configured as a digital output. */
    );
  PORT_SetPinMux(PORTB, PIN5_IDX, kPORT_MuxAlt2);            /* PORTB5 (pin C11) is configured as EMVSIM1_CLK */
  PORTB->PCR[5] = ((PORTB->PCR[5] &
    (~(PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_ISF_MASK))) /* Mask bits to zero which are setting */
      | PORT_PCR_PS(PCR_PS_UP)                               /* Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the corresponding PE field is set. */
      | PORT_PCR_PE(PCR_PE_ENABLED)                          /* Pull Enable: Internal pullup or pulldown resistor is enabled on the corresponding pin, if the pin is configured as a digital input. */
      | PORT_PCR_DSE(PCR_DSE_HIGH)                           /* Drive Strength Enable: High drive strength is configured on the corresponding pin, if pin is configured as a digital output. */
      );
  PORT_SetPinMux(PORTB, PIN6_IDX, kPORT_MuxAlt2);            /* PORTB6 (pin F11) is configured as EMVSIM1_VCCEN */
  PORTB->PCR[6] = ((PORTB->PCR[6] &
    (~(PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_ISF_MASK))) /* Mask bits to zero which are setting */
      | PORT_PCR_PS(PCR_PS_UP)                               /* Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the corresponding PE field is set. */
      | PORT_PCR_DSE(PCR_DSE_HIGH)                           /* Drive Strength Enable: High drive strength is configured on the corresponding pin, if pin is configured as a digital output. */
      | PORT_PCR_PE(PCR_PE_ENABLED)                          /* Pull Enable: Internal pullup or pulldown resistor is enabled on the corresponding pin, if the pin is configured as a digital input. */
    );
  PORT_SetPinMux(PORTB, PIN7_IDX, kPORT_MuxAlt2);            /* PORTB7 (pin E11) is configured as EMVSIM1_PD */
  PORT_SetPinMux(PORTB, PIN8_IDX, kPORT_MuxAlt2);            /* PORTB8 (pin D11) is configured as EMVSIM1_RST */
  PORTB->PCR[8] = ((PORTB->PCR[8] &
    (~(PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_ISF_MASK))) /* Mask bits to zero which are setting */
      | PORT_PCR_PS(PCR_PS_UP)                               /* Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the corresponding PE field is set. */
      | PORT_PCR_PE(PCR_PE_ENABLED)                          /* Pull Enable: Internal pullup or pulldown resistor is enabled on the corresponding pin, if the pin is configured as a digital input. */
      | PORT_PCR_DSE(PCR_DSE_HIGH)                           /* Drive Strength Enable: High drive strength is configured on the corresponding pin, if pin is configured as a digital output. */
      );
  PORT_SetPinMux(PORTC, PIN2_IDX, kPORT_MuxAsGpio);          /* PORTC2 (pin C8) is configured as PTC2 */
  PORTC->PCR[2] = ((PORTC->PCR[2] &
    (~(PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_ISF_MASK))) /* Mask bits to zero which are setting */
      | PORT_PCR_PS(PCR_PS_UP)                               /* Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the corresponding PE field is set. */
      | PORT_PCR_DSE(PCR_DSE_HIGH)                           /* Drive Strength Enable: High drive strength is configured on the corresponding pin, if pin is configured as a digital output. */
      | PORT_PCR_PE(PCR_PE_ENABLED)                          /* Pull Enable: Internal pullup or pulldown resistor is enabled on the corresponding pin, if the pin is configured as a digital input. */
    );
  PORT_SetPinMux(PORTC, PIN3_IDX, kPORT_MuxAlt3);            /* PORTC3 (pin B8) is configured as LPUART1_RX */
  PORT_SetPinMux(PORTC, PIN4_IDX, kPORT_MuxAlt3);            /* PORTC4 (pin A8) is configured as LPUART1_TX */
  PORT_SetPinMux(PORTC, PIN9_IDX, kPORT_MuxAsGpio);          /* PORTC9 (pin D6) is configured as PTC9 */
  PORTC->PCR[9] = ((PORTC->PCR[9] &
    (~(PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_ISF_MASK))) /* Mask bits to zero which are setting */
      | PORT_PCR_PS(PCR_PS_UP)                               /* Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the corresponding PE field is set. */
      | PORT_PCR_DSE(PCR_DSE_HIGH)                           /* Drive Strength Enable: High drive strength is configured on the corresponding pin, if pin is configured as a digital output. */
      | PORT_PCR_PE(PCR_PE_ENABLED)                          /* Pull Enable: Internal pullup or pulldown resistor is enabled on the corresponding pin, if the pin is configured as a digital input. */
    );
  PORT_SetPinMux(PORTE, PIN0_IDX, kPORT_MuxAlt7);            /* PORTE0 (pin B1) is configured as RTC_CLKOUT */
  SIM->SOPT5 = ((SIM->SOPT5 &
    (~(SIM_SOPT5_LPUART1TXSRC_MASK | SIM_SOPT5_LPUART1RXSRC_MASK))) /* Mask bits to zero which are setting */
      | SIM_SOPT5_LPUART1TXSRC(SOPT5_LPUART1TXSRC_LPUART_TX) /* LPUART1 transmit data source select: LPUART1_TX pin */
      | SIM_SOPT5_LPUART1RXSRC(SOPT5_LPUART1RXSRC_LPUART_RX) /* LPUART1 receive data source select: LPUART1_RX pin */
    );
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
