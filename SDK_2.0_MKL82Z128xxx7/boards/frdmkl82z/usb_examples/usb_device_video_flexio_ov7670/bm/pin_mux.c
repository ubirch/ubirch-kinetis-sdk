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
 */

#include "fsl_port.h"
#include "pin_mux.h"
#include "fsl_common.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

/* Function Name : BOARD_InitPins */
void BOARD_InitPins(void)
{
    port_pin_config_t vsyncPinConfig = {0};
    port_pin_config_t i2cPinConfig =
    {
        kPORT_PullUp,
        kPORT_FastSlewRate,
        kPORT_PassiveFilterDisable,
#if defined(FSL_FEATURE_PORT_HAS_OPEN_DRAIN) && FSL_FEATURE_PORT_HAS_OPEN_DRAIN
        kPORT_OpenDrainEnable,
#endif /* FSL_FEATURE_PORT_HAS_OPEN_DRAIN */
        kPORT_LowDriveStrength,
        kPORT_MuxAlt7,
#if defined(FSL_FEATURE_PORT_HAS_PIN_CONTROL_LOCK) && FSL_FEATURE_PORT_HAS_PIN_CONTROL_LOCK
        kPORT_UnlockRegister,
#endif /* FSL_FEATURE_PORT_HAS_PIN_CONTROL_LOCK */
    };

    /* Initialize LPUART0 pins below */
    /* Ungate the port clock */
    CLOCK_EnableClock(kCLOCK_PortB);
    /* Affects PORTB_PCR16 register */
    PORT_SetPinMux(PORTB, 16U, kPORT_MuxAlt3);
    /* Affects PORTB_PCR17 register */
    PORT_SetPinMux(PORTB, 17U, kPORT_MuxAlt3);

    CLOCK_EnableClock(kCLOCK_PortC);
    CLOCK_EnableClock(kCLOCK_PortA);

    CLOCK_EnableClock(kCLOCK_PortD);
    CLOCK_EnableClock(kCLOCK_PortE);


    /* Affects PORTC_PCR3 register */
    /* CLKOUT -> OV7670_XCLK -> 12MHz */
    PORT_SetPinMux(PORTC, 3u, kPORT_MuxAlt5);
    PORT_SetPinMux(PORTA, 18u, kPORT_PinDisabledOrAnalog);
    PORT_SetPinMux(PORTA, 19u, kPORT_PinDisabledOrAnalog);

    /* Affects PORTD_PCR2 register */
    PORT_SetPinConfig(PORTD, 2U, &i2cPinConfig); /*!< I2C0 SCL, SIO_C*/
    /* Affects PORTD_PCR3 register */
    PORT_SetPinConfig(PORTD, 3U, &i2cPinConfig); /*!< I2C0 SDA, SIO_D */

    /* Configure FlexIO pins */
    PORT_SetPinMux(PORTA, 17u, kPORT_MuxAlt5); /*!< FXIO_D23 -> OV7670_PCLK */
    PORT_SetPinMux(PORTB, 11u, kPORT_MuxAlt7); /*!< FXIO_D5 -> OV7670_HREF */

    PORT_SetPinMux(PORTC, 6u, kPORT_MuxAlt7); /*!< FXIO_D14 -> OV7670_D0 */
    PORT_SetPinMux(PORTC, 7u, kPORT_MuxAlt7); /*!< FXIO_D15 -> OV7670_D1 */
    PORT_SetPinMux(PORTC, 8u, kPORT_MuxAlt7); /*!< FXIO_D16 -> OV7670_D2 */
    PORT_SetPinMux(PORTC, 9u, kPORT_MuxAlt7); /*!< FXIO_D17 -> OV7670_D3 */
    PORT_SetPinMux(PORTC, 10u, kPORT_MuxAlt7); /*!< FXIO_D18 -> OV7670_D4 */
    PORT_SetPinMux(PORTC, 11u, kPORT_MuxAlt7); /*!< FXIO_D19 -> OV7670_D5 */
    PORT_SetPinMux(PORTA, 14u, kPORT_MuxAlt5); /*!< FXIO_D20 -> OV7670_D6 */
    PORT_SetPinMux(PORTA, 15u, kPORT_MuxAlt5); /*!< FXIO_D21 -> OV7670_D7 */

    /* Configure OV7670 ASYN */
    vsyncPinConfig.mux = kPORT_MuxAsGpio;
    vsyncPinConfig.pullSelect = kPORT_PullUp;
    vsyncPinConfig.slewRate = kPORT_FastSlewRate;
    PORT_SetPinConfig(PORTA, 16U, &vsyncPinConfig);
    PORT_SetPinInterruptConfig(PORTA, 16U, kPORT_InterruptRisingEdge);

    /* Configure OV7670 Reset */
    vsyncPinConfig.mux = kPORT_MuxAsGpio;
    vsyncPinConfig.pullSelect = kPORT_PullUp;
    vsyncPinConfig.slewRate = kPORT_SlowSlewRate;
    PORT_SetPinConfig(PORTC, 4U, &vsyncPinConfig);
}
