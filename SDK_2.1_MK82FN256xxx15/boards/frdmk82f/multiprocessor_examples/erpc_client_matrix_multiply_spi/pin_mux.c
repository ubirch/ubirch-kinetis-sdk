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

#define PCR_PE_ENABLED 0x01u /*!< Pull Enable: Internal pullup or pulldown resistor is enabled on the corresponding pin, if the pin is configured as a digital input. */
#define PCR_PS_UP 0x01u /*!< Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the corresponding PE field is set. */
#define PIN4_IDX 4u   /*!< Pin number for pin 4 in a port */
#define PIN5_IDX 5u   /*!< Pin number for pin 5 in a port */
#define PIN6_IDX 6u   /*!< Pin number for pin 6 in a port */
#define PIN7_IDX 7u   /*!< Pin number for pin 7 in a port */
#define PIN12_IDX 12u /*!< Pin number for pin 12 in a port */
#define PIN14_IDX 14u /*!< Pin number for pin 14 in a port */
#define PIN15_IDX 15u /*!< Pin number for pin 15 in a port */
#define PIN16_IDX 16u /*!< Pin number for pin 16 in a port */
#define PIN17_IDX 17u /*!< Pin number for pin 17 in a port */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
BOARD_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '86', peripheral: LPUART4, signal: RX, pin_signal: PTC14/LPUART4_RX/FB_AD25/SDRAM_D25/FXIO0_D20}
  - {pin_num: '87', peripheral: LPUART4, signal: TX, pin_signal: PTC15/LPUART4_TX/FB_AD24/SDRAM_D24/FXIO0_D21}
  - {pin_num: '44', peripheral: SPI0, signal: PCS0_SS, pin_signal:
PTA14/SPI0_PCS0/LPUART0_TX/TRACE_D2/FXIO0_D20/I2S0_RX_BCLK/I2S0_TXD1}
  - {pin_num: '45', peripheral: SPI0, signal: SCK, pin_signal: PTA15/SPI0_SCK/LPUART0_RX/TRACE_D1/FXIO0_D21/I2S0_RXD0}
  - {pin_num: '46', peripheral: SPI0, signal: SOUT, pin_signal:
PTA16/SPI0_SOUT/LPUART0_CTS_b/TRACE_D0/FXIO0_D22/I2S0_RX_FS/I2S0_RXD1}
  - {pin_num: '47', peripheral: SPI0, signal: SIN, pin_signal: PTA17/SPI0_SIN/LPUART0_RTS_b/FXIO0_D23/I2S0_MCLK}
  - {pin_num: '40', peripheral: GPIOA, signal: 'GPIO, 4', pin_signal:
TSI0_CH5/PTA4/LLWU_P3/FTM0_CH1/FXIO0_D14/EMVSIM0_VCCEN/NMI_b, pull_select: up, pull_enable: enable}
  - {pin_num: '84', peripheral: GPIOC, signal: 'GPIO, 12', pin_signal:
PTC12/LPUART4_RTS_b/FTM_CLKIN0/FB_AD27/SDRAM_D27/FTM3_FLT0/TPM_CLKIN0}
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

    PORT_SetPinMux(PORTA, PIN4_IDX, kPORT_MuxAsGpio); /* PORTA4 (pin 40) is configured as PTA4 */
    PORTA->PCR[4] =
        ((PORTA->PCR[4] &
          (~(PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_ISF_MASK))) /* Mask bits to zero which are setting */
         | PORT_PCR_PS(PCR_PS_UP) /* Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the
                                     corresponding PE field is set. */
         | PORT_PCR_PE(PCR_PE_ENABLED) /* Pull Enable: Internal pullup or pulldown resistor is enabled on the
                                          corresponding pin, if the pin is configured as a digital input. */
         );
    PORT_SetPinMux(PORTA, PIN14_IDX, kPORT_MuxAlt2);   /* PORTA14 (pin 44) is configured as SPI0_PCS0 */
    PORT_SetPinMux(PORTA, PIN15_IDX, kPORT_MuxAlt2);   /* PORTA15 (pin 45) is configured as SPI0_SCK */
    PORT_SetPinMux(PORTA, PIN16_IDX, kPORT_MuxAlt2);   /* PORTA16 (pin 46) is configured as SPI0_SOUT */
    PORT_SetPinMux(PORTA, PIN17_IDX, kPORT_MuxAlt2);   /* PORTA17 (pin 47) is configured as SPI0_SIN */
    PORT_SetPinMux(PORTC, PIN12_IDX, kPORT_MuxAsGpio); /* PORTC12 (pin 84) is configured as PTC12 */
    PORT_SetPinMux(PORTC, PIN14_IDX, kPORT_MuxAlt3);   /* PORTC14 (pin 86) is configured as LPUART4_RX */
    PORT_SetPinMux(PORTC, PIN15_IDX, kPORT_MuxAlt3);   /* PORTC15 (pin 87) is configured as LPUART4_TX */
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
