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
    /* Ungate the port clock */
    CLOCK_EnableClock(kCLOCK_PortC);
    /* CLKOUT*/
    PORT_SetPinMux(PORTC, 3UL, kPORT_MuxAlt5);
    /* Ungate the port clock */
    CLOCK_EnableClock(kCLOCK_PortB);
    /* CAS/RAS*/
    PORT_SetPinMux(PORTB, 0UL, kPORT_MuxAlt5);
    PORT_SetPinMux(PORTB, 1UL, kPORT_MuxAlt5);
    /* WE */
    PORT_SetPinMux(PORTB, 2UL, kPORT_MuxAlt5);
    /* CS */
    PORT_SetPinMux(PORTB, 3UL, kPORT_MuxAlt5);
    /* SDRAM Data*/
    PORT_SetPinMux(PORTB, 6UL, kPORT_MuxAlt5);  /* SDRAM_D23*/
    PORT_SetPinMux(PORTB, 7UL, kPORT_MuxAlt5);  /* SDRAM_D22*/
    PORT_SetPinMux(PORTB, 8UL, kPORT_MuxAlt5);  /* SDRAM_D21*/
    PORT_SetPinMux(PORTB, 9UL, kPORT_MuxAlt5);  /* SDRAM_D20*/
    PORT_SetPinMux(PORTB, 10UL, kPORT_MuxAlt5); /* SDRAM_D19*/
    PORT_SetPinMux(PORTB, 11UL, kPORT_MuxAlt5); /* SDRAM_D18*/
    PORT_SetPinMux(PORTB, 16UL, kPORT_MuxAlt5); /* SDRAM_D17*/
    PORT_SetPinMux(PORTB, 17UL, kPORT_MuxAlt5); /* SDRAM_D16*/
    PORT_SetPinMux(PORTB, 20UL, kPORT_MuxAlt5); /* SDRAM_D31*/
    PORT_SetPinMux(PORTB, 21UL, kPORT_MuxAlt5); /* SDRAM_D30*/
    PORT_SetPinMux(PORTB, 22UL, kPORT_MuxAlt5); /* SDRAM_D29*/
    PORT_SetPinMux(PORTB, 23UL, kPORT_MuxAlt5); /* SDRAM_D28*/
    PORT_SetPinMux(PORTC, 12UL, kPORT_MuxAlt5); /* SDRAM_D27*/
    PORT_SetPinMux(PORTC, 13UL, kPORT_MuxAlt5); /* SDRAM_D26*/
    PORT_SetPinMux(PORTC, 14UL, kPORT_MuxAlt5); /* SDRAM_D25*/
    PORT_SetPinMux(PORTC, 15UL, kPORT_MuxAlt5); /* SDRAM_D24*/
    /* SDRAM Address*/
    PORT_SetPinMux(PORTC, 0UL, kPORT_MuxAlt5);  /* SDRAM_A22*/
    PORT_SetPinMux(PORTC, 1UL, kPORT_MuxAlt5);  /* SDRAM_A21*/
    PORT_SetPinMux(PORTC, 2UL, kPORT_MuxAlt5);  /* SDRAM_A20*/
    PORT_SetPinMux(PORTC, 4UL, kPORT_MuxAlt5);  /* SDRAM_A19*/
    PORT_SetPinMux(PORTC, 5UL, kPORT_MuxAlt5);  /* SDRAM_A18*/
    PORT_SetPinMux(PORTC, 6UL, kPORT_MuxAlt5);  /* SDRAM_A17*/
    PORT_SetPinMux(PORTC, 7UL, kPORT_MuxAlt5);  /* SDRAM_A16*/
    PORT_SetPinMux(PORTC, 8UL, kPORT_MuxAlt5);  /* SDRAM_A15*/
    PORT_SetPinMux(PORTC, 9UL, kPORT_MuxAlt5);  /* SDRAM_A14*/
    PORT_SetPinMux(PORTC, 10UL, kPORT_MuxAlt5); /* SDRAM_A13*/
    /* Ungate the port clock */
    CLOCK_EnableClock(kCLOCK_PortD);
    PORT_SetPinMux(PORTD, 2UL, kPORT_MuxAlt5); /* SDRAM_A12*/
    PORT_SetPinMux(PORTD, 3UL, kPORT_MuxAlt5); /* SDRAM_A11*/
    PORT_SetPinMux(PORTD, 4UL, kPORT_MuxAlt5); /* SDRAM_A10*/
    PORT_SetPinMux(PORTD, 5UL, kPORT_MuxAlt5); /* SDRAM_A9*/
    /* SDRAM_CKE*/
    PORT_SetPinMux(PORTD, 7UL, kPORT_MuxAlt5);
    /* SDRAM_DQM */
    PORT_SetPinMux(PORTC, 16UL, kPORT_MuxAlt5); /* SDRAM_DQM2*/
    PORT_SetPinMux(PORTC, 17UL, kPORT_MuxAlt5); /* SDRAM_DQM3*/

    BOARD_InitPinsForUart();
}

void BOARD_InitPinsForUart(void)
{
    /* Initialize UART0 pins below */
    /* Affects PORTC_PCR3 register */
    PORT_SetPinMux(PORTC, 3u, kPORT_MuxAlt3);
    /* Affects PORTC_PCR4 register */
    PORT_SetPinMux(PORTC, 4u, kPORT_MuxAlt3);
}

void BOARD_InitPinsForSdram(void)
{
    /* Affects PORTC_PCR3 register */
    PORT_SetPinMux(PORTC, 3u, kPORT_MuxAlt5);
    /* Affects PORTC_PCR4 register */
    PORT_SetPinMux(PORTC, 4u, kPORT_MuxAlt5);
}
