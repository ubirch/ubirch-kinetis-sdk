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

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"

#include "fsl_dma_manager.h"
#include "fsl_ltc_edma.h"

#include "clock_config.h"
#include "pin_mux.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define OUTPUT_ARRAY_LEN 512
#define DES_BLOCK_LENGTH 8
#define DES_KEY_LENGTH 8

/*8-byte multiple*/
static const uint8_t des_test_full[] =
    "Once upon a midnight dreary, while I pondered weak and weary,"
    "Over many a quaint and curious volume of forgotten lore,"
    "While I nodded, nearly napping, suddenly there came a tapping,"
    "As of some one gently rapping, rapping at my chamber door"
    "Its some visitor, I muttered, tapping at my chamber door"
    "Only this, and nothing more.";

static uint8_t output[OUTPUT_ARRAY_LEN];
static uint8_t cipher[OUTPUT_ARRAY_LEN];

/* Variable used by LTC EDMA */
ltc_edma_handle_t g_ltcEdmaHandle;
edma_handle_t g_ltcInputFifoEdmaHandle;
edma_handle_t g_ltcOutputFifoEdmaHandle;
volatile bool g_ltcEdmaDone = false;
volatile status_t g_ltcEdmaStatus = kStatus_Success;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void ltc_print_msg(const uint8_t *data, uint32_t length);
static void ltc_example_task(void);
static void ltc_EdmaUserCallback(LTC_Type *base, ltc_edma_handle_t *handle, status_t status, void *userData);
static void ltc_EdmaWait(void);

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Function ltc_print_msg prints data into console.
 */
static void ltc_print_msg(const uint8_t *data, uint32_t length)
{
    uint32_t i;

    PRINTF("          ");
    for (i = 0; i < length; i++)
    {
        PUTCHAR(data[i]);
        if (data[i] == ',')
        {
            PRINTF("\r\n          ");
        }
    }
    PRINTF("\r\n");
}

/* @brief LTC EDMA user callback */
static void ltc_EdmaUserCallback(LTC_Type *base, ltc_edma_handle_t *handle, status_t status, void *userData)
{
    /* Not used.*/
    userData = userData;
    base = base;
    handle = handle;

    g_ltcEdmaDone = true;
    g_ltcEdmaStatus = status;
}

/* @brief Wait for LTC eDMA complete. */
static void ltc_EdmaWait(void)
{
    while (g_ltcEdmaDone == false)
    {
    }
    /* Reset flag.*/
    g_ltcEdmaDone = false;
}

/*!
 * @brief Function ltc_example_task demonstrates use of LTC driver
 * encryption/decryption functions on short sample text.
 */
static void ltc_example_task(void)
{
    unsigned int length;

    /*8 bytes key: "password"*/
    uint8_t key[DES_KEY_LENGTH] = {0x70, 0x61, 0x73, 0x73, 0x77, 0x6F, 0x72, 0x64};
    /*initialization vector: 8 bytes: "mysecret"*/
    uint8_t ive[DES_BLOCK_LENGTH] = {0x6d, 0x79, 0x73, 0x65, 0x63, 0x72, 0x65, 0x74};
    /* DES3 keys */
    /*24 bytes key: "verynicepassword12345678"*/
    uint8_t key1[DES_KEY_LENGTH] = {0x76, 0x65, 0x72, 0x79, 0x6e, 0x69, 0x63, 0x65};
    uint8_t key2[DES_KEY_LENGTH] = {0x70, 0x61, 0x73, 0x73, 0x77, 0x6f, 0x72, 0x64};
    uint8_t key3[DES_KEY_LENGTH] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38};

    /* Initialize LTC driver.
     * This enables clocking and resets the module to a known state. */
    LTC_Init(LTC0);

    /* Configure DMA. */
    DMAMGR_Init();
    /* Request dma channels from DMA manager. */
    if (DMAMGR_RequestChannel(kDmaRequestMux0Group1LTC0InputFIFO, DMAMGR_DYNAMIC_ALLOCATE, &g_ltcInputFifoEdmaHandle) !=
        kStatus_Success)
    {
        PRINTF("\r\n DMA RequestChannel Failed!\r\n");
    }
    if (DMAMGR_RequestChannel(kDmaRequestMux0Group1LTC0OutputFIFO, DMAMGR_DYNAMIC_ALLOCATE,
                              &g_ltcOutputFifoEdmaHandle) != kStatus_Success)
    {
        PRINTF("\r\n DMA RequestChannel Failed!\r\n");
    }

    /* Create LTC EDMA handle. */
    LTC_CreateHandleEDMA(LTC0, &g_ltcEdmaHandle, ltc_EdmaUserCallback, NULL, &g_ltcInputFifoEdmaHandle,
                         &g_ltcOutputFifoEdmaHandle);

    /* Print welcome string */
    PRINTF("............................. LTC (DES EDMA)  DRIVER  EXAMPLE .................\r\n\r\n");

    /******* FIRST PART USING DES-CBC method ********/
    memset(&output[0], 0, OUTPUT_ARRAY_LEN);
    memset(&cipher[0], 0, OUTPUT_ARRAY_LEN);
    length = sizeof(des_test_full) - 1u;

    PRINTF("Testing input string: \r\n");
    ltc_print_msg(&des_test_full[0], length);
    /* Format console output */
    PRINTF("\r\n");

    /******* FIRST PART USING DES-CBC method ********/
    PRINTF("----------------------------------- DES-CBC method ----------------------------\r\n");
    /*   ENCRYPTION   */
    PRINTF("DES CBC Encryption of %u bytes.\r\n", length);

    /* Call DES_cbc encryption */
    LTC_DES_EncryptCbcEDMA(LTC0, &g_ltcEdmaHandle, des_test_full, cipher, length, ive, key);
    ltc_EdmaWait();
    if (g_ltcEdmaStatus != kStatus_Success)
    {
        PRINTF("DES CBC encryption failed !\r\n");
        return;
    }

    /* Result message */
    PRINTF("DES CBC encryption finished.\r\n\r\n");

    /*   DECRYPTION   */
    PRINTF("DES CBC Decryption of %u bytes.\r\n", length);

    /* Call DES_cbc decryption */
    LTC_DES_DecryptCbcEDMA(LTC0, &g_ltcEdmaHandle, cipher, output, length, ive, key);
    ltc_EdmaWait();
    if (g_ltcEdmaStatus != kStatus_Success)
    {
        PRINTF("DES CBC decryption failed !\r\n");
        return;
    }

    /* Result message */
    PRINTF("DES CBC decryption finished.\r\n");
    /* Print decrypted string */
    PRINTF("Decrypted string :\r\n");
    ltc_print_msg(output, length);
    /* Format console output */
    PRINTF("\r\n");

    /******* SECOND PART USING DES3-CBC method ********/
    PRINTF("----------------------------------- DES3-CBC method ---------------------------\r\n");
    length = sizeof(des_test_full) - 1u;
    /*   ENCRYPTION   */
    PRINTF("DES3 CBC Encryption of %u bytes.\r\n", length);

    /* Call DES3_cbc encryption */
    LTC_DES3_EncryptCbcEDMA(LTC0, &g_ltcEdmaHandle, des_test_full, cipher, length, ive, key1, key2, key3);
    ltc_EdmaWait();
    if (g_ltcEdmaStatus != kStatus_Success)
    {
        PRINTF("DES3 CBC encryption failed !\r\n");
        return;
    }

    /* Result message */
    PRINTF("DES3 CBC encryption finished.\r\n\r\n");

    /*   DECRYPTION   */
    PRINTF("DES3 CBC decryption of %u bytes.\r\n", length);

    /* Call DES3_cbc decryption */
    LTC_DES3_DecryptCbcEDMA(LTC0, &g_ltcEdmaHandle, cipher, output, length, ive, key1, key2, key3);
    ltc_EdmaWait();
    if (g_ltcEdmaStatus != kStatus_Success)
    {
        PRINTF("DES3 CBC decryption failed !\r\n");
        return;
    }

    /* Result message */
    PRINTF("DES3 CBC decryption finished.\r\n");
    /* Print decrypted string */
    PRINTF("Decrypted string :\r\n");
    ltc_print_msg(output, length);
    PRINTF("\r\n");

    /* Goodbye message */
    PRINTF(".............. THE  END  OF  THE  LTC (DES EDMA)  DRIVER  EXAMPLE .............\r\n");
}

int main(void)
{
    /* Init hardware*/
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* Call example task */
    ltc_example_task();

    while (1)
    {
    }
}
