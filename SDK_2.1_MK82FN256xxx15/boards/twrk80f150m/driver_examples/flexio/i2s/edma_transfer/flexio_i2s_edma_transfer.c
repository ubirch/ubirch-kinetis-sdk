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

#include "board.h"
#include "fsl_dmamux.h"
#include "fsl_flexio_i2s_edma.h"
#include "fsl_debug_console.h"

#include "fsl_sgtl5000.h"
#include "clock_config.h"
#include "pin_mux.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* SAI and I2C instance and clock */
#define DEMO_I2C I2C0
#define DEMO_FLEXIO_BASE FLEXIO0
#define DEMO_I2C_CLKSRC kCLOCK_BusClk
/* Flexio clock source frequency shall be even times of bit clock */
#define DEMO_FLEXIO_CLKSRC kCLOCK_PllFllSelClk
#define EXAMPLE_DMA DMA0
#define EXAMPLE_TX_CHANNEL 17U
#define EXAMPLE_RX_CHANNEL 16U
#define EXAMPLE_TX_DMA_SOURCE kDmaRequestMux0Group1FlexIO0Channel0
#define EXAMPLE_RX_DMA_SOURCE kDmaRequestMux0Group1FlexIO0Channel1
#define BCLK_PIN 4U
#define FRAME_SYNC_PIN 5U
#define TX_DATA_PIN 1U
#define RX_DATA_PIN 0U
#define OVER_SAMPLE_RATE (384)
#define BUFFER_SIZE (256)
#if defined(__GNUC__) /* GNU Compiler */
#ifndef __ALIGN_END
#define __ALIGN_END __attribute__((aligned(4)))
#endif
#ifndef __ALIGN_BEGIN
#define __ALIGN_BEGIN
#endif
#else
#ifndef __ALIGN_END
#define __ALIGN_END
#endif
#ifndef __ALIGN_BEGIN
#if defined(__CC_ARM) /* ARM Compiler */
#define __ALIGN_BEGIN __align(4)
#elif defined(__ICCARM__) /* IAR Compiler */
#define __ALIGN_BEGIN
#endif
#endif
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
flexio_i2s_edma_handle_t txHandle = {0};
flexio_i2s_edma_handle_t rxHandle = {0};
edma_handle_t txDmaHandle = {0};
edma_handle_t rxDmaHandle = {0};
#if defined(__ICCARM__)
#pragma data_alignment = 4
#endif
static uint8_t __ALIGN_BEGIN txBuff[BUFFER_SIZE] __ALIGN_END;
#if defined(__ICCARM__)
#pragma data_alignment = 4
#endif
static uint8_t __ALIGN_BEGIN rxBuff[BUFFER_SIZE] __ALIGN_END;
#if defined(DEMO_CODEC_WM8960)
wm8960_handle_t codecHandle = {0};
#else
sgtl_handle_t codecHandle = {0};
#endif

#if defined(FSL_FEATURE_SOC_LPI2C_COUNT) && (FSL_FEATURE_SOC_LPI2C_COUNT)
lpi2c_master_handle_t i2cHandle = {0};
#else
i2c_master_handle_t i2cHandle = {{0, 0, kI2C_Write, 0, 0, NULL, 0}, 0, 0, NULL, NULL};
#endif
FLEXIO_I2S_Type base;
static volatile bool istxFinished = false;
static volatile bool isrxFinished = false;

/*******************************************************************************
 * Code
 ******************************************************************************/
static void txCallback(FLEXIO_I2S_Type *i2sBase, flexio_i2s_edma_handle_t *handle, status_t status, void *userData)
{
    istxFinished = true;
}

static void rxCallback(FLEXIO_I2S_Type *i2sBase, flexio_i2s_edma_handle_t *handle, status_t status, void *userData)
{
    isrxFinished = true;
}

/*!
 * @brief Main function
 */
int main(void)
{
    flexio_i2s_config_t config;
    uint32_t sourceClockHz = 0U;
    flexio_i2s_format_t format;
    flexio_i2s_transfer_t xferTx, xferRx;
    edma_config_t dmaConfig = {0};
#if defined(FSL_FEATURE_SOC_LPI2C_COUNT) && (FSL_FEATURE_SOC_LPI2C_COUNT)
    lpi2c_master_config_t i2cConfig = {0};
#else
    i2c_master_config_t i2cConfig = {0};
#endif
    uint32_t i2cSourceClock;
    uint32_t i = 0;
    status_t status = kStatus_Success;

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    CLOCK_SetFlexio0Clock(1U);
    CLOCK_SetPllFllSelClock(3, 0, 0);

    PRINTF("FLEXIO I2S EDMA example started!\n\r");

    /* Set flexio i2s pin, shifter and timer */
    base.bclkPinIndex = BCLK_PIN;
    base.fsPinIndex = FRAME_SYNC_PIN;
    base.txPinIndex = TX_DATA_PIN;
    base.rxPinIndex = RX_DATA_PIN;
    base.txShifterIndex = 0;
    base.rxShifterIndex = 1;
    base.bclkTimerIndex = 0;
    base.fsTimerIndex = 1;
    base.flexioBase = DEMO_FLEXIO_BASE;

#if defined(DEMO_CODEC_WM8960)
    /* Use SAI to do master clock if using WM8960 codec */
    sai_config_t saiConfig;
    /*
     * saiConfig.masterSlave = kSAI_Master;
     * saiConfig.mclkSource = kSAI_MclkSourceSysclk;
     * saiConfig.protocol = kSAI_BusLeftJustified;
     * saiConfig.syncMode = kSAI_ModeAsync;
     * saiConfig.mclkOutputEnable = true;
     */
    SAI_TxGetDefaultConfig(&saiConfig);
    SAI_TxInit(DEMO_SAI, &saiConfig);

    /* Configure the audio format */
    sai_transfer_format_t saiFormat = {0};
    saiFormat.bitWidth = kSAI_WordWidth16bits;
    saiFormat.channel = 0U;
    saiFormat.sampleRate_Hz = kSAI_SampleRate16KHz;
#if defined(FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER) && (FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER)
    saiFormat.masterClockHz = OVER_SAMPLE_RATE * saiFormat.sampleRate_Hz;
#else
    saiFormat.masterClockHz = CLOCK_GetFreq(DEMO_SAI_CLKSRC);
#endif
    saiFormat.protocol = saiConfig.protocol;

    uint32_t masterClockSrc = CLOCK_GetFreq(DEMO_SAI_CLKSRC);
    SAI_TxSetFormat(DEMO_SAI, &saiFormat, masterClockSrc, saiFormat.masterClockHz);
    SAI_TxEnable(DEMO_SAI, true);
#endif
    /* Create EDMA handle */
    /*
     * dmaConfig.enableRoundRobinArbitration = false;
     * dmaConfig.enableHaltOnError = true;
     * dmaConfig.enableContinuousLinkMode = false;
     * dmaConfig.enableDebugMode = false;
     */
    EDMA_GetDefaultConfig(&dmaConfig);
    EDMA_Init(EXAMPLE_DMA, &dmaConfig);
    EDMA_CreateHandle(&txDmaHandle, EXAMPLE_DMA, EXAMPLE_TX_CHANNEL);
    EDMA_CreateHandle(&rxDmaHandle, EXAMPLE_DMA, EXAMPLE_RX_CHANNEL);

    DMAMUX_Init(DMAMUX0);
    DMAMUX_SetSource(DMAMUX0, EXAMPLE_TX_CHANNEL, EXAMPLE_TX_DMA_SOURCE);
    DMAMUX_EnableChannel(DMAMUX0, EXAMPLE_TX_CHANNEL);
    DMAMUX_SetSource(DMAMUX0, EXAMPLE_RX_CHANNEL, EXAMPLE_RX_DMA_SOURCE);
    DMAMUX_EnableChannel(DMAMUX0, EXAMPLE_RX_CHANNEL);

    /* Init SAI module */
    /*
     * config.enableI2S = true;
     */
    FLEXIO_I2S_GetDefaultConfig(&config);
    FLEXIO_I2S_Init(&base, &config);

    /* Configure the audio format */
    format.bitWidth = kFLEXIO_I2S_WordWidth16bits;
    format.sampleRate_Hz = kFLEXIO_I2S_SampleRate16KHz;

    /* Configure Sgtl5000 I2C */
    codecHandle.base = DEMO_I2C;
    codecHandle.i2cHandle = &i2cHandle;
    i2cSourceClock = CLOCK_GetFreq(DEMO_I2C_CLKSRC);

#if defined(FSL_FEATURE_SOC_LPI2C_COUNT) && (FSL_FEATURE_SOC_LPI2C_COUNT)
    /*
     * i2cConfig.debugEnable = false;
     * i2cConfig.ignoreAck = false;
     * i2cConfig.pinConfig = kLPI2C_2PinOpenDrain;
     * i2cConfig.baudRate_Hz = 100000U;
     * i2cConfig.busIdleTimeout_ns = 0;
     * i2cConfig.pinLowTimeout_ns = 0;
     * i2cConfig.sdaGlitchFilterWidth_ns = 0;
     * i2cConfig.sclGlitchFilterWidth_ns = 0;
     */
    LPI2C_MasterGetDefaultConfig(&i2cConfig);
    LPI2C_MasterInit(DEMO_I2C, &i2cConfig, i2cSourceClock);
    LPI2C_MasterTransferCreateHandle(DEMO_I2C, &i2cHandle, NULL, NULL);
#else
    /*
     * i2cConfig.baudRate_Bps = 100000U;
     * i2cConfig.enableHighDrive = false;
     * i2cConfig.enableStopHold = false;
     * i2cConfig.glitchFilterWidth = 0U;
     * i2cConfig.enableMaster = true;
     */
    I2C_MasterGetDefaultConfig(&i2cConfig);
    I2C_MasterInit(DEMO_I2C, &i2cConfig, i2cSourceClock);
#endif

#if defined(DEMO_CODEC_WM8960)
    WM8960_Init(&codecHandle, NULL);
    WM8960_SetProtocol(&codecHandle, kWM8960_BusI2S);
    WM8960_ConfigDataFormat(&codecHandle, OVER_SAMPLE_RATE * format.sampleRate_Hz, format.sampleRate_Hz,
                            format.bitWidth);
#else
    /* configure sgtl5000 */
    sgtl_config_t codecConfig;
    codecConfig.bus = kSGTL_BusI2S;
    codecConfig.master_slave = false;
    codecConfig.route = kSGTL_RoutePlaybackandRecord;
    SGTL_Init(&codecHandle, &codecConfig);
    SGTL_ModifyReg(&codecHandle, CHIP_I2S_CTRL, SGTL5000_I2S_SCLK_INV_CLR_MASK, SGTL5000_I2S_VAILD_FALLING_EDGE);
    /* Configure codec format */
    SGTL_ConfigDataFormat(&codecHandle, OVER_SAMPLE_RATE * format.sampleRate_Hz, format.sampleRate_Hz, format.bitWidth);
#endif

    FLEXIO_I2S_TransferTxCreateHandleEDMA(&base, &txHandle, txCallback, NULL, &txDmaHandle);
    FLEXIO_I2S_TransferRxCreateHandleEDMA(&base, &rxHandle, rxCallback, NULL, &rxDmaHandle);
    
    sourceClockHz = CLOCK_GetFreq(DEMO_FLEXIO_CLKSRC);
    FLEXIO_I2S_TransferSetFormatEDMA(&base, &txHandle, &format, sourceClockHz);
    FLEXIO_I2S_TransferSetFormatEDMA(&base, &rxHandle, &format, sourceClockHz);
    
    /*  xfer structure */
    xferTx.data = txBuff;
    xferTx.dataSize = BUFFER_SIZE;
    xferRx.data = rxBuff;
    xferRx.dataSize = BUFFER_SIZE;
    /* Add a transfer to trigger bit clock */
    FLEXIO_I2S_TransferSendEDMA(&base, &txHandle, &xferTx);
    /* First record a piece of data */
    for (i = 0; i < 5000; i++)
    {
        isrxFinished = false;
        /* Fill the queue to full */
        status = kStatus_Success;
        while(status != kStatus_FLEXIO_I2S_QueueFull)
        {
            status = FLEXIO_I2S_TransferReceiveEDMA(&base, &rxHandle, &xferRx);
        }
        /* Wait for data record finished */
        while (isrxFinished != true)
        {
        }
        /* Copy data from RX buffer to tx buffer */
        memcpy(txBuff, rxBuff, BUFFER_SIZE);
        FLEXIO_I2S_TransferSendEDMA(&base, &txHandle, &xferTx);
    }

#if defined(DEMO_CODEC_WM8960)
    WM8960_Deinit(&codecHandle);
#else
    SGTL_Deinit(&codecHandle);
#endif

#if defined(FSL_FEATURE_SOC_LPI2C_COUNT) && (FSL_FEATURE_SOC_LPI2C_COUNT)
    LPI2C_MasterDeinit(DEMO_I2C);
#else
    I2C_MasterDeinit(DEMO_I2C);
#endif
    PRINTF("\n\r FLEXIO I2S EDMA example finished!\n\r ");

    while (1)
    {
    }
}
