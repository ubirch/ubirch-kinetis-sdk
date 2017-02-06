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

#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_tpm.h"

#include "clock_config.h"
#include "pin_mux.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* TPM channel pair 0 works with TPM channels 0 and 1 */
#define BOARD_TPM_CHANNEL_PAIR kTPM_Chnl_0

/* Interrupt to enable and flag to read; depends on the TPM channel pair used */
#define TPM_CHANNEL_INTERRUPT_ENABLE kTPM_Chnl0InterruptEnable
#define TPM_CHANNEL_FLAG kTPM_Chnl0Flag

/* Interrupt number and interrupt handler for the TPM instance used */
#define TPM_INTERRUPT_NUMBER TPM2_IRQn
#define TPM_LED_HANDLER TPM2_IRQHandler
/* Get source clock for TPM driver */
#define TPM_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_PllFllSelClk)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief delay a while.
 */
void delay(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool tpmIsrFlag = false;
volatile bool brightnessUp = true; /* Indicate LED is brighter or dimmer */
volatile uint8_t updatedDutycycle = 10U;

/*******************************************************************************
 * Code
 ******************************************************************************/
void delay(void)
{
    volatile uint32_t i = 0U;
    for (i = 0U; i < 80000U; ++i)
    {
        __asm("NOP"); /* delay */
    }
}

void TPM_LED_HANDLER(void)
{
    tpmIsrFlag = true;

    if (brightnessUp)
    {
        /* Increase duty cycle until it reach limited value, don't want to go upto 100% duty cycle
         * as channel interrupt will not be set for 100%
         */
        if (++updatedDutycycle >= 99U)
        {
            updatedDutycycle = 99U;
            brightnessUp = false;
        }
    }
    else
    {
        /* Decrease duty cycle until it reach limited value */
        if (--updatedDutycycle == 1U)
        {
            brightnessUp = true;
        }
    }

    /* Clear interrupt flag.*/
    TPM_ClearStatusFlags(BOARD_TPM_BASEADDR, TPM_CHANNEL_FLAG);
}

/*!
 * @brief Main function
 */
int main(void)
{
    tpm_config_t tpmInfo;
    tpm_chnl_pwm_signal_param_t tpmParam;
    tpm_pwm_level_select_t pwmLevel = kTPM_LowTrue;
    uint8_t deadtimeValue;
    uint32_t filterVal;

    /* Configure tpm params with frequency 24kHZ */
    tpmParam.chnlNumber = BOARD_TPM_CHANNEL_PAIR;
    tpmParam.level = pwmLevel;
    tpmParam.dutyCyclePercent = updatedDutycycle;
    tpmParam.firstEdgeDelayPercent = 0U;

    /* Board pin, clock, debug console init */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* TPM known issue of KL81, enable clock of TPM0 to use other TPM module */
    CLOCK_EnableClock(kCLOCK_Tpm2);
    /* Select the clock source for the TPM counter as kCLOCK_PllFllSelClk */
    CLOCK_SetTpmClock(1U);

    /* Need a deadtime value of about 200nsec */
    deadtimeValue = (((uint64_t)TPM_SOURCE_CLOCK * 200) / 1000000000) / 4;

    /* Print a note to terminal */
    PRINTF("\r\nTPM example to output combined complementary PWM signals on two channels\r\n");
    PRINTF("\r\nYou will see a change in LED brightness if an LED is connected to the TPM pin");
    PRINTF("\r\nIf no LED is connected to the TPM pin, then probe the signal using an oscilloscope");

    TPM_GetDefaultConfig(&tpmInfo);

    /* Initialize TPM module */
    TPM_Init(BOARD_TPM_BASEADDR, &tpmInfo);

    TPM_SetupPwm(BOARD_TPM_BASEADDR, &tpmParam, 1U, kTPM_CombinedPwm, 24000U, TPM_SOURCE_CLOCK);

#if defined(FSL_FEATURE_TPM_HAS_POL) && FSL_FEATURE_TPM_HAS_POL
    /* Change the polarity on the second channel of the pair to get complementary PWM signals */
    BOARD_TPM_BASEADDR->POL |= (1U << ((BOARD_TPM_CHANNEL_PAIR * 2) + 1));
#endif

    /* Set deadtime insertion for the channel pair using channel filter register */
    filterVal = BOARD_TPM_BASEADDR->FILTER;
    /* Clear the channel pair's filter values */
    filterVal &= ~((TPM_FILTER_CH0FVAL_MASK | TPM_FILTER_CH1FVAL_MASK)
                   << (BOARD_TPM_CHANNEL_PAIR * (TPM_FILTER_CH0FVAL_SHIFT + TPM_FILTER_CH1FVAL_SHIFT)));
    /* Shift the deadtime insertion value to the right place in the register */
    filterVal |= (TPM_FILTER_CH0FVAL(deadtimeValue) | TPM_FILTER_CH1FVAL(deadtimeValue))
                 << (BOARD_TPM_CHANNEL_PAIR * (TPM_FILTER_CH0FVAL_SHIFT + TPM_FILTER_CH1FVAL_SHIFT));
    BOARD_TPM_BASEADDR->FILTER = filterVal;

    /* Enable channel interrupt flag.*/
    TPM_EnableInterrupts(BOARD_TPM_BASEADDR, TPM_CHANNEL_INTERRUPT_ENABLE);

    /* Enable at the NVIC */
    EnableIRQ(TPM_INTERRUPT_NUMBER);

    TPM_StartTimer(BOARD_TPM_BASEADDR, kTPM_SystemClock);

    while (1)
    {
        /* Use interrupt to update the PWM dutycycle */
        if (true == tpmIsrFlag)
        {
            /* Disable interrupt to retain current dutycycle for a few seconds */
            TPM_DisableInterrupts(BOARD_TPM_BASEADDR, TPM_CHANNEL_INTERRUPT_ENABLE);

            tpmIsrFlag = false;

            /* Disable output on each channel of the pair before updating the dutycycle */
            TPM_UpdateChnlEdgeLevelSelect(BOARD_TPM_BASEADDR, (tpm_chnl_t)(BOARD_TPM_CHANNEL_PAIR * 2), 0U);
            TPM_UpdateChnlEdgeLevelSelect(BOARD_TPM_BASEADDR, (tpm_chnl_t)((BOARD_TPM_CHANNEL_PAIR * 2) + 1), 0U);

            /* Update PWM duty cycle on the channel pair */
            TPM_UpdatePwmDutycycle(BOARD_TPM_BASEADDR, BOARD_TPM_CHANNEL_PAIR, kTPM_CombinedPwm, updatedDutycycle);

            /* Start output on each channel of the pair with updated dutycycle */
            TPM_UpdateChnlEdgeLevelSelect(BOARD_TPM_BASEADDR, (tpm_chnl_t)(BOARD_TPM_CHANNEL_PAIR * 2), pwmLevel);
            TPM_UpdateChnlEdgeLevelSelect(BOARD_TPM_BASEADDR, (tpm_chnl_t)((BOARD_TPM_CHANNEL_PAIR * 2) + 1), pwmLevel);

            /* Delay to view the updated PWM dutycycle */
            delay();

            /* Enable interrupt flag to update PWM dutycycle */
            TPM_EnableInterrupts(BOARD_TPM_BASEADDR, TPM_CHANNEL_INTERRUPT_ENABLE);
        }
    }
}
