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

#include "fsl_device_registers.h"
#include "fsl_port.h"
#include "pin_mux.h"

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Initialize all pins used in this example
 *
 * @param disablePortClockAfterInit disable port clock after pin
 * initialization or not.
 */
void BOARD_InitPins(void)
{
    port_pin_config_t config = {0};

    /* Ungate the port clock */
    CLOCK_EnableClock(kCLOCK_PortB);
    CLOCK_EnableClock(kCLOCK_PortC);
    CLOCK_EnableClock(kCLOCK_PortA);
    CLOCK_EnableClock(kCLOCK_PortE);
    /* Configure external OSC clock */
    PORT_SetPinMux(PORTA, 18U, kPORT_PinDisabledOrAnalog);
    PORT_SetPinMux(PORTA, 19U, kPORT_PinDisabledOrAnalog);
    /* Configure RTC clock */
    PORT_SetPinMux(PORTE, 0UL, kPORT_MuxAlt7);

    /* Initialize LPUART1 pins below */
    /* Ungate the port clock */
    /* Affects PORTC_PCR3 register */
    PORT_SetPinMux(PORTC, 3U, kPORT_MuxAlt3);
    /* Affects PORTC_PCR4 register */
    PORT_SetPinMux(PORTC, 4U, kPORT_MuxAlt3);

    /* Initialize SMARTCARD pins below */
    /* IO*/
    /* Affects PORTB_PCR4 register */
    config.pullSelect = kPORT_PullDisable;
    config.openDrainEnable = kPORT_OpenDrainEnable;
    config.mux = kPORT_MuxAlt2;
    PORT_SetPinConfig(PORTB, 4U, &config);
    /* Card clock */
    config.pullSelect = kPORT_PullUp;
    config.driveStrength = kPORT_HighDriveStrength;
    config.mux = kPORT_MuxAlt2;
    config.openDrainEnable = kPORT_OpenDrainDisable;
    PORT_SetPinConfig(PORTB, 5U, &config);
    /* Reset */
    PORT_SetPinConfig(PORTB, 8U, &config);
    /* Vsel0 and Vsel1 */
    config.mux = kPORT_MuxAsGpio;
    PORT_SetPinConfig(PORTC, 9U, &config);
    PORT_SetPinConfig(PORTC, 2U, &config);
    /* Presence Detect */
    PORT_SetPinMux(PORTB, 7U, kPORT_MuxAlt2);
    /* INT pin */
    config.mux = kPORT_MuxAsGpio;
    PORT_SetPinConfig(PORTA, 13U, &config);
    /* Initialize pin interrupt */
    PORT_SetPinInterruptConfig(PORTA, 13U, kPORT_InterruptEitherEdge);
    /* Clear interrupt status flags */
    PORT_ClearPinsInterruptFlags(PORTA, 1U << 13U);
    /* VCCEN */
    config.mux = kPORT_MuxAlt2;
    PORT_SetPinConfig(PORTB, 6U, &config);
}
