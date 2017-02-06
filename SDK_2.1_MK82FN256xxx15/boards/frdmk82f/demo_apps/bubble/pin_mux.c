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
- !!processor_version '0.1.27'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"

#define PCR_PE_DISABLED \
    0x00u /*!< Pull Enable: Internal pullup or pulldown resistor is not enabled on the corresponding pin. */
#define PCR_SRE_SLOW                                                                                                  \
    0x01u /*!< Slew Rate Enable: Slow slew rate is configured on the corresponding pin, if the pin is configured as a \
             \                                                                                                        \
             digital output. */
#define PIN9_IDX 9u   /*!< Pin number for pin 9 in a port */
#define PIN10_IDX 10u /*!< Pin number for pin 10 in a port */
#define PIN14_IDX 14u /*!< Pin number for pin 14 in a port */
#define PIN15_IDX 15u /*!< Pin number for pin 15 in a port */
#define SOPT8_FTM3OCH5SRC_FTM \
    0x00u /*!< FTM3 channel 5 output source: FTM3_CH5 pin is output of FTM3 channel 5 output */
#define SOPT8_FTM3OCH6SRC_FTM \
    0x00u /*!< FTM3 channel 6 output source: FTM3_CH6 pin is output of FTM3 channel 6 output */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
BOARD_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '86', peripheral: LPUART4, signal: RX, pin_signal: PTC14/LPUART4_RX/FB_AD25/SDRAM_D25/FXIO0_D20}
  - {pin_num: '82', peripheral: FTM3, signal: 'CH, 6', pin_signal:
PTC10/I2C1_SCL/FTM3_CH6/I2S0_RX_FS/FB_AD5/SDRAM_A13/FXIO0_D18, slew_rate: slow, pull_enable: disable}
  - {pin_num: '87', peripheral: LPUART4, signal: TX, pin_signal: PTC15/LPUART4_TX/FB_AD24/SDRAM_D24/FXIO0_D21}
  - {pin_num: '81', peripheral: FTM3, signal: 'CH, 5', pin_signal:
CMP0_IN3/PTC9/FTM3_CH5/I2S0_RX_BCLK/FB_AD6/SDRAM_A14/FTM2_FLT0/FXIO0_D17, slew_rate: slow, pull_enable: disable}
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

    PORT_SetPinMux(PORTC, PIN10_IDX, kPORT_MuxAlt3); /* PORTC10 (pin 82) is configured as FTM3_CH6 */
    PORTC->PCR[10] =
        ((PORTC->PCR[10] &
          (~(PORT_PCR_PE_MASK | PORT_PCR_SRE_MASK | PORT_PCR_ISF_MASK))) /* Mask bits to zero which are setting */
         | PORT_PCR_PE(PCR_PE_DISABLED) /* Pull Enable: Internal pullup or pulldown resistor is not enabled on the
                                           corresponding pin. */
         | PORT_PCR_SRE(PCR_SRE_SLOW) /* Slew Rate Enable: Slow slew rate is configured on the corresponding pin, if the
                                         pin is configured as a digital output. */
         );
    PORT_SetPinMux(PORTC, PIN14_IDX, kPORT_MuxAlt3); /* PORTC14 (pin 86) is configured as LPUART4_RX */
    PORT_SetPinMux(PORTC, PIN15_IDX, kPORT_MuxAlt3); /* PORTC15 (pin 87) is configured as LPUART4_TX */
    PORT_SetPinMux(PORTC, PIN9_IDX, kPORT_MuxAlt3);  /* PORTC9 (pin 81) is configured as FTM3_CH5 */
    PORTC->PCR[9] =
        ((PORTC->PCR[9] &
          (~(PORT_PCR_PE_MASK | PORT_PCR_SRE_MASK | PORT_PCR_ISF_MASK))) /* Mask bits to zero which are setting */
         | PORT_PCR_PE(PCR_PE_DISABLED) /* Pull Enable: Internal pullup or pulldown resistor is not enabled on the
                                           corresponding pin. */
         | PORT_PCR_SRE(PCR_SRE_SLOW) /* Slew Rate Enable: Slow slew rate is configured on the corresponding pin, if the
                                         pin is configured as a digital output. */
         );
    SIM->SOPT8 =
        ((SIM->SOPT8 &
          (~(SIM_SOPT8_FTM3OCH5SRC_MASK | SIM_SOPT8_FTM3OCH6SRC_MASK))) /* Mask bits to zero which are setting */
         |
         SIM_SOPT8_FTM3OCH5SRC(
             SOPT8_FTM3OCH5SRC_FTM) /* FTM3 channel 5 output source: FTM3_CH5 pin is output of FTM3 channel 5 output */
         |
         SIM_SOPT8_FTM3OCH6SRC(
             SOPT8_FTM3OCH6SRC_FTM) /* FTM3 channel 6 output source: FTM3_CH6 pin is output of FTM3 channel 6 output */
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

/*******************************************************************************
 * EOF
 ******************************************************************************/
