/*
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
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

#include <string.h>
#include "emvl1_core.h"
#include "emvl1_adapter_sdk.h"

/*******************************************************************************
* Variables
******************************************************************************/

/*******************************************************************************
* Prototypes
******************************************************************************/
static smartcard_core_error_t ttl_send_cmd(smartcard_core_params_t *coreParams,
                                           smartcard_ttl_cmd_t *ttlCmd,
                                           smartcard_ttl_resp_t *ttlRespPtr);
static smartcard_core_error_t ttl_send_cmd_t0(smartcard_core_params_t *coreParams,
                                              smartcard_ttl_cmd_t *ttlCmd,
                                              smartcard_ttl_resp_t *ttlRespPtr);
static smartcard_core_error_t ttl_send_cmd_t1(smartcard_core_params_t *coreParams,
                                              smartcard_ttl_cmd_t *ttlCmd,
                                              smartcard_ttl_resp_t *ttlRespPtr);
static void send_data(smartcard_core_params_t *coreParams, uint8_t *txBuff, uint32_t txSize);
static void receive_data(smartcard_core_params_t *coreParams, uint8_t *rxBuff, uint32_t rxSize, uint32_t *receivedSize);
static inline void reset_smartcard(smartcard_core_params_t *coreParams, smartcard_reset_type_t resetType);
static inline void wait_for_card_reset(smartcard_core_params_t *coreParams);
/*******************************************************************************
* Code
******************************************************************************/

/*!
 * @brief This function calls SMARTCARD driver transfer function.
 */
static void send_data(smartcard_core_params_t *coreParams, uint8_t *txBuff, uint32_t txSize)
{
    coreParams->xfer.size = txSize;
    coreParams->xfer.buff = txBuff;
    coreParams->xfer.direction = kSMARTCARD_Transmit;
    /* Receive ATR bytes using non-blocking API */
    SMARTCARD_TRANSFER(coreParams->x_context, &coreParams->xfer);
#if (!defined(FSL_RTOS_FREE_RTOS) && !defined(FSL_RTOS_UCOSII) && !defined(FSL_RTOS_UCOSIII))
    /* Wait for transfer to finish */
    while (0u != SMARTCARD_GET_TRANSFER_REMAINING_BYTES((coreParams->x_context)))
    {
    }
#endif
}

/*!
 * @brief This function calls SMARTCARD driver transfer function.
 */
static void receive_data(smartcard_core_params_t *coreParams, uint8_t *rxBuff, uint32_t rxSize, uint32_t *receivedSize)
{
    /* Set the correct state */
    coreParams->xfer.size = rxSize;
    coreParams->xfer.buff = rxBuff;
    coreParams->xfer.direction = kSMARTCARD_Receive;
    /* Receive ATR bytes using non-blocking API */
    SMARTCARD_TRANSFER(coreParams->x_context, &coreParams->xfer);
#if (!defined(FSL_RTOS_FREE_RTOS) && !defined(FSL_RTOS_UCOSII) && !defined(FSL_RTOS_UCOSIII))
    /* Wait for transfer to finish */
    while (0u != SMARTCARD_GET_TRANSFER_REMAINING_BYTES(coreParams->x_context))
    {
    }
#endif
#if defined(FSL_RTOS_FREE_RTOS) || defined(FSL_RTOS_UCOSII) || defined(FSL_RTOS_UCOSIII)
    *receivedSize = rxSize - ((rtos_smartcard_context_t *)coreParams->x_context)->x_context.xSize;
#else
    *receivedSize = rxSize - ((smartcard_context_t *)coreParams->x_context)->xSize;
#endif
}

/*!
 * @brief This function will waits until initial character is received.
 */
static inline void wait_for_card_reset(smartcard_core_params_t *coreParams)
{
#if defined(FSL_RTOS_FREE_RTOS) || defined(FSL_RTOS_UCOSII) || defined(FSL_RTOS_UCOSIII)
    /* Wait until transfer complete event will occur */
    SMARTCARD_RTOS_WaitForXevent(coreParams->x_context);
#else
    while ((((smartcard_context_t *)coreParams->x_context)->transferState == kSMARTCARD_WaitingForTSState) &&
           (((smartcard_context_t *)coreParams->x_context)->xIsBusy))
    {
    }
#endif
}

/*!
 * @brief This function calls SMARTCARD driver functions to reset smartcard IC.
 */
static void reset_smartcard(smartcard_core_params_t *coreParams, smartcard_reset_type_t resetType)
{
    assert((NULL != coreParams));

    /* Enable Initial character detection first */
    SMARTCARD_ENABLE_INIT_DETECT(coreParams->x_context);
    /* Invoke SMARTCARD IP specific function to activate the card */
    SMARTCARD_PHY_ACTIVATE(coreParams->x_context, resetType);
    /* Invoke SMARTCARD IP specific function to enable reception */
    SMARTCARD_ENABLE_RECEIVER(coreParams->x_context);
}

/*!
 * @brief This function sends the command to T=0 or T=1 respective APIs
 */
static smartcard_core_error_t ttl_send_cmd(smartcard_core_params_t *coreParams,
                                           smartcard_ttl_cmd_t *ttlCmd,
                                           smartcard_ttl_resp_t *ttlRespPtr)
{
    assert((NULL != coreParams));
    assert((NULL != ttlCmd));
    assert((NULL != ttlRespPtr));

#if defined(FSL_RTOS_FREE_RTOS) || defined(FSL_RTOS_UCOSII) || defined(FSL_RTOS_UCOSIII)
    smartcard_context_t *smartcardContext = &((rtos_smartcard_context_t *)coreParams->x_context)->x_context;
#else
    smartcard_context_t *smartcardContext = (smartcard_context_t *)coreParams->x_context;
#endif

    coreParams->errCode = kSCCoreSuccess;

    smartcardContext->statusBytes[0u] = 0u;
    smartcardContext->statusBytes[1u] = 0u;

    if (smartcardContext->tType == kSMARTCARD_T0Transport)
    {
        ttl_send_cmd_t0(coreParams, ttlCmd, ttlRespPtr);
    }
    else
    {
        ttl_send_cmd_t1(coreParams, ttlCmd, ttlRespPtr);
    }

    return coreParams->errCode;
}

/*! @brief
* In T=0 transport mode, this function sends the command using
* driver apis and then handles the card response. Finally it sends the response
* back to the upper layer.
* Note:
* 1. GET RESPONSE command for retrieval of data from the ICC when case 2 or 4
* commands are used.
* 2. Procedure bytes '61xx' and '6Cxx' are returned by the ICC to control exchanges
* between the TTL and the ICC, and should never be returned to the TAL.
*/
static smartcard_core_error_t ttl_send_cmd_t0(smartcard_core_params_t *coreParams,
                                              smartcard_ttl_cmd_t *ttlCmd,
                                              smartcard_ttl_resp_t *ttlRespPtr)
{
    assert((NULL != coreParams));
    assert((NULL != ttlCmd));
    assert((NULL != ttlRespPtr));

    uint16_t dataUnitSize = 0u;
    uint8_t secondProcByte;
    uint8_t getResponseCmd[5u];
    uint32_t rcvLength = 0u;
    uint8_t *header = ttlCmd->inf;
    uint8_t remainingBytes = header[4u];
    uint8_t lc = 0u;
    uint8_t tc = 0u;
    uint8_t le = 0u;
    uint8_t re = 0u;
    uint8_t getResponseRe = 0u;
    bool sendCommand = true;
    uint16_t expectedRxLength = 2u;
    uint32_t temp32 = 0u;
#if defined(FSL_RTOS_FREE_RTOS) || defined(FSL_RTOS_UCOSII) || defined(FSL_RTOS_UCOSIII)
    smartcard_context_t *smartcardContext = &((rtos_smartcard_context_t *)coreParams->x_context)->x_context;
#else
    smartcard_context_t *smartcardContext = (smartcard_context_t *)coreParams->x_context;
#endif

    /* Start WWT timer */
    smartcardContext->timersState.wwtExpired = false;
    smartcardContext->timersState.cwtExpired = false;
    smartcardContext->timersState.bwtExpired = false;

    SMARTCARD_RESET_WWT(coreParams->x_context);

    /* Enable WWT timer before starting T=0 transport */
    SMARTCARD_ENABLE_WWT(coreParams->x_context);

    if (coreParams->Case == kSCCommandCase1)
    {
        /* The command APDU was mapped onto the command TPDU with P3 = '00'. */

        /* send command header */
        send_data(coreParams, header, 5u);

        /* The response TPDU is mapped onto the response APDU without any change. */
        receive_data(coreParams, ttlRespPtr->resp, expectedRxLength, &rcvLength);
        rcvLength = expectedRxLength - smartcardContext->xSize;

        while ((rcvLength == 2u) && (ttlRespPtr->resp[0u] == 0x60u))
        {
            ttlRespPtr->resp[0u] = ttlRespPtr->resp[1u];
            rcvLength--;

            if ((ttlRespPtr->resp[0u] == header[1u]) || ((ttlRespPtr->resp[0u] ^ (header[1u])) == 0xFFu))
            {
                break;
            }

            temp32 = 0u;
            receive_data(coreParams, (ttlRespPtr->resp + 1u), 1u, &temp32);
            rcvLength += temp32;
        }
        if (rcvLength == 2u)
        {
            if (smartcard_check_status(coreParams, ttlRespPtr->resp) == kSCCoreFail)
            {
                return kSCCoreFail;
            }
        }
        else
        {
            coreParams->errCode = kSCCoreTransportRxFail;
            /* Disable WWT timer before exiting T=0 transport */
            SMARTCARD_DISABLE_WWT(coreParams->x_context);
            return kSCCoreFail;
        }
    }
    else if (coreParams->Case == kSCCommandCase2)
    {
        do
        {
            if (sendCommand)
            {
                le = header[4u];
                /* send command header */
                send_data(coreParams, header, 5u);
                sendCommand = false;
            }

            receive_data(coreParams, (ttlRespPtr->resp + re), expectedRxLength, &rcvLength);

            while ((rcvLength == 2u) && (ttlRespPtr->resp[re] == 0x60u))
            {
                ttlRespPtr->resp[re] = ttlRespPtr->resp[re + 1u];
                rcvLength--;

                if ((ttlRespPtr->resp[re] == header[1]) || ((ttlRespPtr->resp[re] ^ (header[1u])) == 0xFFu))
                {
                    break;
                }

                temp32 = 0u;
                receive_data(coreParams, (ttlRespPtr->resp + re + 1u), 1u, &temp32);
                if (temp32 == 0u)
                { /* Incorrect GR receive length */
                    coreParams->errCode = kSCCoreTransportRxFail;
                    /* Disable WWT timer before exiting T=0 transport */
                    SMARTCARD_DISABLE_WWT(coreParams->x_context);
                    return kSCCoreFail;
                }
                rcvLength += temp32;
            }

            if (rcvLength == 0u)
            {
                /*If nothing is received ;Card has to be deactivated*/
                coreParams->errCode = kSCCoreIOError;
                break;
            }
            else if (rcvLength == 1)
            {
                /* Check if only 1st procedure byte is received */
                if (ttlRespPtr->resp[re] == header[1]) /* == INS */
                {
                    dataUnitSize = le - re;
                }
                else if ((ttlRespPtr->resp[re] ^ (header[1u])) == 0xFFu) /* == ~INS */
                {
                    dataUnitSize = 1u;
                }
                else
                { /* Incorrect GR receive length */
                    coreParams->errCode = kSCCoreUnknownProcByte;
                    /* Disable WWT timer before exiting T=0 transport */
                    SMARTCARD_DISABLE_WWT(coreParams->x_context);
                    return kSCCoreFail;
                }

                /* First clear the WWT flag that has been set while receiving */
                smartcardContext->timersState.wwtExpired = false;
                SMARTCARD_RESET_WWT(coreParams->x_context);

                /* Enable WWT timer before starting T=0 transport */
                SMARTCARD_ENABLE_WWT(coreParams->x_context);
            }
            else if (rcvLength == 2u)
            {
                /*********************** Check if 2 procedure bytes are received ****************/
                /* Process aborted; Ne not accepted, Na indicated */
                if (ttlRespPtr->resp[re] == 0x6Cu)
                {
                    secondProcByte = ttlRespPtr->resp[re + 1u];
                    /* resend the previous command header to the ICC using a length = secondProcByte */
                    header[4u] = secondProcByte;
                    /*Get ready to receive new proc byte+Licc+status/new proc bytes*/
                    le = secondProcByte;
                    sendCommand = true;
                    continue;
                }
                else if (ttlRespPtr->resp[re] == 0x61u)
                {
                    secondProcByte = ttlRespPtr->resp[re + 1u];

                    do
                    {
                        /* then send a GET RESPONSE command header */
                        getResponseCmd[0u] = 0x00u;
                        getResponseCmd[1u] = 0xC0u; /* GET RESPONSE command */
                        getResponseCmd[2u] = 0u;
                        getResponseCmd[3u] = 0u;

                        if (le > 0u)
                        {
                            /* came passed 6C Licc processing */
                            le = le - re;
                            getResponseCmd[4u] = (secondProcByte < le) ? secondProcByte : le; /* length */
                        }
                        else
                        {
                            /* ISO7816 card sends 61 xx directly w/o 6C Licc */
                            getResponseCmd[4u] = secondProcByte; /* length */
                        }
                        /*Forcibly making else statement true for the time being*/
                        getResponseCmd[4u] = secondProcByte; /* length */

                        /* send command header */
                        send_data(coreParams, getResponseCmd, 5u);

                        /* get the 1st procedure byte */
                        /* TBD: to check if it is == 0xC0 or not */
                        do
                        {
                            temp32 = 0u;
                            receive_data(coreParams, (ttlRespPtr->resp + re), 1u, &temp32);
                            if (temp32 != 1u)
                            {
                                coreParams->errCode = kSCCoreTransportRxFail;
                                /* Disable WWT timer before exiting T=0 transport */
                                SMARTCARD_DISABLE_WWT(coreParams->x_context);
                                return kSCCoreFail;
                            }
                        } while (ttlRespPtr->resp[re] == 0x60u);

                        getResponseRe = 0u;

                        while (ttlRespPtr->resp[re + getResponseRe] == 0x3Fu) /* == ~INS */
                        {
                            receive_data(coreParams, (ttlRespPtr->resp + re + getResponseRe), 1u, &temp32);
                            if (temp32 != 1u)
                            {
                                coreParams->errCode = kSCCoreTransportRxFail;
                                /* Disable WWT timer before exiting T=0 transport */
                                SMARTCARD_DISABLE_WWT(coreParams->x_context);
                                return kSCCoreFail;
                            }
                            getResponseRe++;
                            receive_data(coreParams, (ttlRespPtr->resp + re + getResponseRe), 1u, &temp32);
                            if (temp32 != 1u)
                            {
                                coreParams->errCode = kSCCoreTransportRxFail;
                                /* Disable WWT timer before exiting T=0 transport */
                                SMARTCARD_DISABLE_WWT(coreParams->x_context);
                                return kSCCoreFail;
                            }
                        }

                        if (ttlRespPtr->resp[re + getResponseRe] == 0xC0u) /* == INS */
                        {
                            temp32 = 0u;
                            receive_data(coreParams, (ttlRespPtr->resp + re + getResponseRe),
                                         (getResponseCmd[4u] - getResponseRe + 2u), &temp32);
                            if (temp32 != (getResponseCmd[4u] - getResponseRe + 2u))
                            {
                                coreParams->errCode = kSCCoreTransportRxFail;
                                /* Disable WWT timer before exiting T=0 transport */
                                SMARTCARD_DISABLE_WWT(coreParams->x_context);
                                return kSCCoreFail;
                            }
                            rcvLength = temp32 + getResponseRe;
                        }
                        else
                        {
                            temp32 = 0u;
                            receive_data(coreParams, (ttlRespPtr->resp + re + getResponseRe + 1u), 1u, &temp32);
                            if (temp32 != 1u)
                            {
                                coreParams->errCode = kSCCoreTransportRxFail;
                                /* Disable WWT timer before exiting T=0 transport */
                                SMARTCARD_DISABLE_WWT(coreParams->x_context);
                                return kSCCoreFail;
                            }
                            rcvLength = temp32 + (getResponseRe + 1);
                        }

                        if (rcvLength == 2u)
                        {
                            /*
                            * Refer EMV4.3 Book 1, Table 33: GET RESPONSE Error Conditions
                            */
                            if ((ttlRespPtr->resp[re + 0u] == 0x62u) && (ttlRespPtr->resp[re + 1u] == 0x81u))
                            {
                                /* Part of returned data may be corrupted */
                                coreParams->errCode = kSCCoreGRPartOfReturnedDataCorrupted;
                            }
                            else if ((ttlRespPtr->resp[re + 0u] == 0x67u) && (ttlRespPtr->resp[re + 1u] == 0x00u))
                            {
                                /* Length field incorrect */
                                coreParams->errCode = kSCCoreGRLengthFieldIncorrect;
                            }
                            else if ((ttlRespPtr->resp[re + 0u] == 0x6Au) && (ttlRespPtr->resp[re + 1u] == 0x86u))
                            {
                                /* P1 P2 #'00' */
                                coreParams->errCode = kSCCoreGRP1P2NonZero;
                            }
                            else if ((ttlRespPtr->resp[re + 0u] == 0x6Fu) && (ttlRespPtr->resp[re + 1u] == 0x00u))
                            {
                                /* No precise diagnosis */
                                coreParams->errCode = kSCCoreGRNoPreciseDiagnosis;
                            }

                            smartcardContext->statusBytes[0u] = ttlRespPtr->resp[re + 0u];
                            smartcardContext->statusBytes[1u] = ttlRespPtr->resp[re + 1u];

                            ttlRespPtr->length = re;
                            break;
                        }
                        else if (rcvLength == (getResponseCmd[4u] + 2u))
                        {
                            if (ttlRespPtr->resp[re + getResponseCmd[4u]] == 0x61u)
                            {
                                secondProcByte = ttlRespPtr->resp[re + getResponseCmd[4u] + 1u];
                                re += getResponseCmd[4u];
                                continue;
                            }
                            else if (((ttlRespPtr->resp[re + getResponseCmd[4u]] == 0x90u) &&
                                      (ttlRespPtr->resp[re + getResponseCmd[4u] + 1u] == 0x00u)) ||
                                     ((ttlRespPtr->resp[re + getResponseCmd[4u]] >= 0x64u) &&
                                      (ttlRespPtr->resp[re + getResponseCmd[4u]] <= 0x6Bu)) ||
                                     ((ttlRespPtr->resp[re + getResponseCmd[4u]] >= 0x6Du) &&
                                      (ttlRespPtr->resp[re + getResponseCmd[4u]] <= 0x6Fu)))
                            {
                                smartcardContext->statusBytes[0u] = ttlRespPtr->resp[re + getResponseCmd[4u]];
                                smartcardContext->statusBytes[1u] = ttlRespPtr->resp[re + getResponseCmd[4u] + 1u];
                                ttlRespPtr->length = re + getResponseCmd[4u];
                                coreParams->errCode = kSCCoreSuccess;
                                /* break the GET-RESPONSE loop */
                                break;
                            }
                            else
                            {
                                coreParams->errCode = kSCCoreIncorrectStatusBytes;
                                /* Disable WWT timer before exting T=0 transport */
                                SMARTCARD_DISABLE_WWT(coreParams->x_context);
                                return kSCCoreFail;
                            }
                        }
                        else
                        {
                            /* Incorrect GR receive length */
                            coreParams->errCode = kSCCoreGRIncorrectReceiveLength;
                            /* Disable WWT timer before exiting T=0 transport */
                            SMARTCARD_DISABLE_WWT(coreParams->x_context);
                            return kSCCoreFail;
                        }
                    } while (1);
                    /* break the kSCCommandCase4 loop */
                    break;
                }
                else if (ttlRespPtr->resp[re] == header[1u]) /* == INS */
                {
                    /* Save the data byte and remove INS byte */
                    ttlRespPtr->resp[re] = ttlRespPtr->resp[re + 1u];
                    /* One byte has already been received */
                    re++;
                    dataUnitSize = le - re;
                }
                else if ((ttlRespPtr->resp[re] ^ (header[1u])) == 0xffu) /* == ~INS */
                {
                    /* Save the data byte and remove INS byte */
                    ttlRespPtr->resp[re] = ttlRespPtr->resp[re + 1u];
                    /* given byte has already been received */
                    re++;
                    continue;
                }
                else
                {
                    /******************* Check if SW1, SW2 are received ***********************/
                    if (smartcard_check_status(coreParams, (ttlRespPtr->resp + re)) == kSCCoreFail)
                    {
                        ttlRespPtr->length = re;
                        return kSCCoreFail;
                    }
                    else
                    {
                        ttlRespPtr->length = re;
                        break;
                    }
                }
            }
            if (dataUnitSize > 1u)
            {
                /* receive all the remaining bytes */
                receive_data(coreParams, (ttlRespPtr->resp + re), dataUnitSize, &temp32);
                if (temp32 != dataUnitSize)
                {
                    coreParams->errCode = kSCCoreTransportRxFail;
                    break;
                }
                sendCommand = false;
                re += dataUnitSize;
            }
            else if (dataUnitSize == 1u)
            {
                temp32 = 0;
                /* send 1 byte at a time */
                receive_data(coreParams, (ttlRespPtr->resp + re), 1u, &temp32);
                if (temp32 != 1)
                {
                    coreParams->errCode = kSCCoreTransportRxFail;
                    break;
                }
                re++;
            }
        } while (1u);
    }
    else if (coreParams->Case == kSCCommandCase3)
    {
        /* send command header */
        send_data(coreParams, header, 5u);
        tc = 0u;
        do
        {
            /* Get procedure/status byte(s) */
            receive_data(coreParams, ttlRespPtr->resp, 1u, &rcvLength);

            if (0u == rcvLength)
            { /* If nothing is received. Card has to be deactivated */
                coreParams->errCode = kSCCoreIOError;
                break;
            }
            while (0x60u == ttlRespPtr->resp[0u])
            {
                rcvLength = 0u;
                receive_data(coreParams, ttlRespPtr->resp, 1u, &rcvLength);
                if (0u == rcvLength)
                {
                    coreParams->errCode = kSCCoreUnknownProcByte;
                    /* Disable WWT timer before exiting T=0 transport */
                    SMARTCARD_DISABLE_WWT(coreParams->x_context);
                    return kSCCoreFail;
                }
            }
            if (1u == rcvLength)
            {                                           /* Check if only 1st procedure byte is received */
                if (ttlRespPtr->resp[0u] == header[1u]) /* == INS */
                {
                    dataUnitSize = remainingBytes;
                }
                else if ((ttlRespPtr->resp[0u] ^ header[1u]) == 0xFFu) /* == ~INS */
                {
                    dataUnitSize = 1u;
                }
                else
                {
                    rcvLength = 0u;
                    receive_data(coreParams, (ttlRespPtr->resp + 1u), 1u, &rcvLength);
                    if (0u == rcvLength)
                    {
                        coreParams->errCode = kSCCoreIOError;
                        /* Disable WWT timer before exiting T=0 transport */
                        SMARTCARD_DISABLE_WWT(coreParams->x_context);
                        return kSCCoreFail;
                    }
                    rcvLength++;
                    /* Now clear the WWT flag that has been set while receiving */
                    smartcardContext->timersState.wwtExpired = false;
                    SMARTCARD_RESET_WWT(coreParams->x_context);
                    /* Enable WWT timer before starting T=0 transport */
                    SMARTCARD_ENABLE_WWT(coreParams->x_context);
                }
            }
            if (rcvLength == 2u)
            {
                if (smartcard_check_status(coreParams, ttlRespPtr->resp) == kSCCoreFail)
                {
                    return kSCCoreFail;
                }
                else
                {
                    break;
                }
            }

            if (dataUnitSize > 1u)
            {
                /* send all the remaining bytes */
                send_data(coreParams, (header + 5u + tc), remainingBytes);
                remainingBytes = 0u;
            }
            else if (dataUnitSize == 1u)
            {
                /* send 1 byte at a time */
                send_data(coreParams, (header + 5u + tc), 1u);
                tc++;
                remainingBytes--;
            }
        } while (1u);
    }
    else if (coreParams->Case == kSCCommandCase4)
    {
        do
        {
            if (sendCommand)
            {
                lc = header[4u];
                /* send command header */
                send_data(coreParams, header, 5u);
                sendCommand = false;
            }
            /* Get procedure/status byte(s) */
            receive_data(coreParams, (ttlRespPtr->resp + re), 1u, &rcvLength);

            if (0u == rcvLength)
            { /* If nothing is received, Card has to be deactivated */
                coreParams->errCode = kSCCoreIOError;
                break;
            }
            while (0x60u == ttlRespPtr->resp[0u])
            {
                rcvLength = 0u;
                receive_data(coreParams, ttlRespPtr->resp, 1u, &rcvLength);
                if (0u == rcvLength)
                {
                    coreParams->errCode = kSCCoreUnknownProcByte;
                    /* Disable WWT timer before exiting T=0 transport */
                    SMARTCARD_DISABLE_WWT(coreParams->x_context);
                    return kSCCoreFail;
                }
            }
            /* Check if only 1st procedure byte is received */
            if (ttlRespPtr->resp[re] == header[1u]) /* == INS */
            {
                if (tc < lc)
                { /* to send more command data */
                    dataUnitSize = lc - tc;
                }
                else
                { /* to receive more response data */
                    dataUnitSize = le - re;
                }
            }
            else if ((ttlRespPtr->resp[re] ^ header[1u]) == 0xFFu) /* == ~INS */
            {
                dataUnitSize = 1u;
            }
            else
            {
                rcvLength = 0u;
                receive_data(coreParams, (ttlRespPtr->resp + 1u), 1u, &rcvLength);
                if (0u == rcvLength)
                {
                    coreParams->errCode = kSCCoreIOError;
                    /* Disable WWT timer before exiting T=0 transport */
                    SMARTCARD_DISABLE_WWT(coreParams->x_context);
                    return kSCCoreFail;
                }
                rcvLength++;
                /* Now clear the WWT flag that has been set while receiving */
                smartcardContext->timersState.wwtExpired = false;
                SMARTCARD_RESET_WWT(coreParams->x_context);
                /* Enable WWT timer before starting T=0 transport */
                SMARTCARD_ENABLE_WWT(coreParams->x_context);
            }
            if (rcvLength == 2u)
            {
                smartcardContext->statusBytes[0u] = ttlRespPtr->resp[re];
                smartcardContext->statusBytes[1u] = ttlRespPtr->resp[re + 1u];

                /******************* Check if SW1, SW2 are received ***********************/
                if (ttlRespPtr->resp[re] == 0x90u) /* SW1 */
                {
                    coreParams->errCode = kSCCoreSuccess;
                    ttlRespPtr->length = re;
                    /* TBD: check secondProcOrStatusByte and set appropriate error states */
                    break;
                }

                /*********************** Check if 2 procedure bytes are received ****************/
                /* Process aborted; Ne not accepted, Na indicated */
                if (ttlRespPtr->resp[re] == 0x6Cu)
                {
                    secondProcByte = ttlRespPtr->resp[re + 1u];
                    /* resend the previous command header to the ICC using a length = secondProcByte */
                    *(header + 4) = secondProcByte;
                    le = secondProcByte;
                    sendCommand = true;
                    continue;
                }
                else if ((ttlRespPtr->resp[re] == 0x61u) ||
                         (((ttlRespPtr->resp[re] == 0x62u) || (ttlRespPtr->resp[re] == 0x63u) ||
                           (((ttlRespPtr->resp[re] & 0xF0u) == 0x90u) && ((ttlRespPtr->resp[re] & 0x0Fu) != 0x00u))) &&
                          (tc == lc)))
                {
                    secondProcByte = ttlRespPtr->resp[re + 1u];

                    do
                    {
                        /* then send a GET RESPONSE command header */
                        getResponseCmd[0u] = 0x00u;
                        getResponseCmd[1u] = 0xC0u; /* GET RESPONSE command */
                        getResponseCmd[2u] = 0u;
                        getResponseCmd[3u] = 0u;

                        /* ISO7816 card sends 61 xx directly w/o 6C Licc */
                        if ((ttlRespPtr->resp[re] == 0x61u) || (ttlRespPtr->resp[re] == 0x6Cu))
                        {
                            getResponseCmd[4u] = secondProcByte; /* length */
                        }
                        else
                        {
                            getResponseCmd[4u] = 0u;
                        }

                        /* send command header */
                        send_data(coreParams, getResponseCmd, 5u);

                        /* get the 1st procedure byte */
                        /* TBD: to check if it is == 0xC0 or not */
                        do
                        {
                            temp32 = 0u;
                            receive_data(coreParams, (ttlRespPtr->resp + re), 1u, &temp32);
                            if (temp32 != 1u)
                            {
                                coreParams->errCode = kSCCoreTransportRxFail;
                                /* Disable WWT timer before exiting T=0 transport */
                                SMARTCARD_DISABLE_WWT(coreParams->x_context);
                                return kSCCoreFail;
                            }
                        } while (ttlRespPtr->resp[re] == 0x60u);

                        getResponseRe = 0u;

                        while (ttlRespPtr->resp[re + getResponseRe] == 0x3Fu) /* == ~INS */
                        {
                            receive_data(coreParams, (ttlRespPtr->resp + re + getResponseRe), 1u, &temp32);
                            if (temp32 != 1u)
                            {
                                coreParams->errCode = kSCCoreTransportRxFail;
                                /* Disable WWT timer before exiting T=0 transport */
                                SMARTCARD_DISABLE_WWT(coreParams->x_context);
                                return kSCCoreFail;
                            }
                            getResponseRe++;
                            receive_data(coreParams, (ttlRespPtr->resp + re + getResponseRe), 1u, &temp32);
                            if (temp32 != 1u)
                            {
                                coreParams->errCode = kSCCoreTransportRxFail;
                                /* Disable WWT timer before exiting T=0 transport */
                                SMARTCARD_DISABLE_WWT(coreParams->x_context);
                                return kSCCoreFail;
                            }
                        }

                        if (ttlRespPtr->resp[re + getResponseRe] == 0xC0u) /* == INS */
                        {
                            temp32 = 0u;
                            receive_data(coreParams, (ttlRespPtr->resp + re + getResponseRe),
                                         (getResponseCmd[4u] - getResponseRe + 2u), &temp32);
                            if (temp32 != (getResponseCmd[4u] - getResponseRe + 2u))
                            {
                                coreParams->errCode = kSCCoreTransportRxFail;
                                /* Disable WWT timer before exiting T=0 transport */
                                SMARTCARD_DISABLE_WWT(coreParams->x_context);
                                return kSCCoreFail;
                            }
                            rcvLength = temp32 + getResponseRe;
                        }
                        else
                        {
                            temp32 = 0u;
                            receive_data(coreParams, (ttlRespPtr->resp + re + getResponseRe + 1u), 1u, &temp32);
                            if (temp32 != 1u)
                            {
                                coreParams->errCode = kSCCoreTransportRxFail;
                                /* Disable WWT timer before exiting T=0 transport */
                                SMARTCARD_DISABLE_WWT(coreParams->x_context);
                                return kSCCoreFail;
                            }
                            rcvLength = temp32 + (getResponseRe + 1u);
                        }

                        if (rcvLength == 2u)
                        {
                            if (ttlRespPtr->resp[re + 0u] == 0x6Cu)
                            {
                                secondProcByte = ttlRespPtr->resp[re + 1u];
                                continue;
                            }

                            /*
                            * Refer EMV4.3 Book 1, Table 33: GET RESPONSE Error Conditions
                            */
                            if ((ttlRespPtr->resp[re + 0u] == 0x62u) && (ttlRespPtr->resp[re + 1u] == 0x81u))
                            {
                                /* Part of returned data may be corrupted */
                                coreParams->errCode = kSCCoreGRPartOfReturnedDataCorrupted;
                            }
                            else if ((ttlRespPtr->resp[re + 0u] == 0x67u) && (ttlRespPtr->resp[re + 1u] == 0x00u))
                            {
                                /* Length field incorrect */
                                coreParams->errCode = kSCCoreGRLengthFieldIncorrect;
                            }
                            else if ((ttlRespPtr->resp[re + 0u] == 0x6Au) && (ttlRespPtr->resp[re + 1u] == 0x86u))
                            {
                                /* P1 P2 #'00' */
                                coreParams->errCode = kSCCoreGRP1P2NonZero;
                            }
                            else if ((ttlRespPtr->resp[re + 0u] == 0x6Fu) && (ttlRespPtr->resp[re + 1u] == 0x00u))
                            {
                                /* No precise diagnosis */
                                coreParams->errCode = kSCCoreGRNoPreciseDiagnosis;
                            }

                            smartcardContext->statusBytes[0u] = ttlRespPtr->resp[re + 0u];
                            smartcardContext->statusBytes[1u] = ttlRespPtr->resp[re + 1u];

                            ttlRespPtr->length = re;
                            break;
                        }
                        else if (rcvLength == (getResponseCmd[4u] + 2u))
                        {
                            if (ttlRespPtr->resp[re + getResponseCmd[4u]] == 0x61u)
                            {
                                secondProcByte = ttlRespPtr->resp[re + getResponseCmd[4u] + 1u];
                                re += getResponseCmd[4];
                                continue;
                            }
                            else if (((ttlRespPtr->resp[re + getResponseCmd[4u]] == 0x90u) &&
                                      (ttlRespPtr->resp[re + getResponseCmd[4u] + 1u] == 0x00u)) ||
                                     (ttlRespPtr->resp[re + getResponseCmd[4u]] == 0x62u) ||
                                     (ttlRespPtr->resp[re + getResponseCmd[4u]] == 0x63u) ||
                                     ((ttlRespPtr->resp[re + getResponseCmd[4u]] >= 0x64u) &&
                                      (ttlRespPtr->resp[re + getResponseCmd[4u]] <= 0x6Bu)) ||
                                     ((ttlRespPtr->resp[re + getResponseCmd[4u]] >= 0x6Du) &&
                                      (ttlRespPtr->resp[re + getResponseCmd[4u]] <= 0x6Fu)))
                            {
                                smartcardContext->statusBytes[0u] = ttlRespPtr->resp[re + getResponseCmd[4u]];
                                smartcardContext->statusBytes[1u] = ttlRespPtr->resp[re + getResponseCmd[4u] + 1u];
                                coreParams->errCode = kSCCoreSuccess;
                                ttlRespPtr->length = re + getResponseCmd[4u];
                                /* break the GET-RESPONSE loop */
                                break;
                            }
                            else
                            {
                                coreParams->errCode = kSCCoreIncorrectStatusBytes;
                                /* Disable WWT timer before exiting T=0 transport */
                                SMARTCARD_DISABLE_WWT(coreParams->x_context);
                                return kSCCoreFail;
                            }
                        }
                        else
                        {
                            /* Incorrect GR receive length */
                            coreParams->errCode = kSCCoreGRIncorrectReceiveLength;
                            /* Disable WWT timer before exiting T=0 transport */
                            SMARTCARD_DISABLE_WWT(coreParams->x_context);
                            return kSCCoreFail;
                        }
                    } while (1);
                    /* break the kCase4 loop */
                    break;
                }
                /* Below ones not required for EMV cards, but are applicable for ISO7816-4 cards */
                else if (ttlRespPtr->resp[re] == 0x6Eu)
                {
                    /* CLA not supported */
                    coreParams->errCode = kSCCoreCLANotSupported;

                    smartcardContext->statusBytes[0u] = ttlRespPtr->resp[0u];
                    smartcardContext->statusBytes[1u] = ttlRespPtr->resp[1u];
                    /* Disable WWT timer before exiting T=0 transport */
                    SMARTCARD_DISABLE_WWT(coreParams->x_context);

                    ttlRespPtr->length = re;
                    return kSCCoreFail;
                }
                else if (ttlRespPtr->resp[re] == 0x6Du)
                {
                    /* CLA supported, but INS not programmed or invalid  */
                    coreParams->errCode = kSCCoreINSInvalid;

                    smartcardContext->statusBytes[0u] = ttlRespPtr->resp[0u];
                    smartcardContext->statusBytes[1u] = ttlRespPtr->resp[1u];
                    /* Disable WWT timer before exiting T=0 transport */
                    SMARTCARD_DISABLE_WWT(coreParams->x_context);

                    ttlRespPtr->length = re;
                    return kSCCoreFail;
                }
                else if (ttlRespPtr->resp[re] == 0x6Bu)
                {
                    /* CLA INS supported, but P1 P2 incorrect  */
                    coreParams->errCode = kSCCoreP1P2Invalid;

                    smartcardContext->statusBytes[0u] = ttlRespPtr->resp[0u];
                    smartcardContext->statusBytes[1u] = ttlRespPtr->resp[1u];
                    /* Disable WWT timer before exiting T=0 transport */
                    SMARTCARD_DISABLE_WWT(coreParams->x_context);

                    ttlRespPtr->length = re;
                    return kSCCoreFail;
                }
                /* Process aborted; Ne definitely not accepted */
                else if (ttlRespPtr->resp[re] == 0x67u)
                {
                    /* CLA INS P1 P2 supported, but P3 incorrect   */
                    coreParams->errCode = kSCCoreP3Invalid;

                    smartcardContext->statusBytes[0u] = ttlRespPtr->resp[0u];
                    smartcardContext->statusBytes[1u] = ttlRespPtr->resp[1u];
                    /* Disable WWT timer before exiting T=0 transport */
                    SMARTCARD_DISABLE_WWT(coreParams->x_context);

                    ttlRespPtr->length = re;
                    return kSCCoreFail;
                }
                else if (ttlRespPtr->resp[re] == 0x6Fu)
                {
                    /* command not supported and no precise diagnosis given */
                    coreParams->errCode = kSCCoreCommandNotSupported;

                    smartcardContext->statusBytes[0u] = ttlRespPtr->resp[0u];
                    smartcardContext->statusBytes[1u] = ttlRespPtr->resp[1u];
                    /* Disable WWT timer before exiting T=0 transport */
                    SMARTCARD_DISABLE_WWT(coreParams->x_context);

                    ttlRespPtr->length = re;
                    return kSCCoreFail;
                }
                else if (ttlRespPtr->resp[re] == header[1u]) /* == INS */
                {
                    /*Save the data byte and remove INS byte*/
                    ttlRespPtr->resp[re] = ttlRespPtr->resp[re + 1u];
                    /*One byte has already been received*/
                    re++;
                    dataUnitSize = le - re;
                }
                else if ((ttlRespPtr->resp[re] ^ (header[1u])) == 0xffu) /* == ~INS */
                {
                    /*Save the data byte and remove INS byte*/
                    ttlRespPtr->resp[re] = ttlRespPtr->resp[re + 1u];
                    /*given byte has already been received*/
                    re++;
                    continue;
                }
                else if ((ttlRespPtr->resp[re] == 0x62u) || (ttlRespPtr->resp[re] == 0x63u) ||
                         (ttlRespPtr->resp[re] == 0x64u) || (ttlRespPtr->resp[re] == 0x65u) ||
                         (ttlRespPtr->resp[re] == 0x66u) || (ttlRespPtr->resp[re] == 0x68u) ||
                         (ttlRespPtr->resp[re] == 0x69u) || (ttlRespPtr->resp[re] == 0x6Au) ||
                         (((ttlRespPtr->resp[re] & 0xF0u) == 0x90u) && ((ttlRespPtr->resp[re] & 0x0Fu) != 0x00u)))
                {
                    coreParams->errCode = kSCCoreOther;

                    smartcardContext->statusBytes[0u] = ttlRespPtr->resp[0u];
                    smartcardContext->statusBytes[1u] = ttlRespPtr->resp[1u];
                    /* Disable WWT timer before exiting T=0 transport */
                    SMARTCARD_DISABLE_WWT(coreParams->x_context);

                    ttlRespPtr->length = re;
                    return kSCCoreFail;
                }
                else
                {
                    coreParams->errCode = kSCCoreCommandNotSupported;

                    smartcardContext->statusBytes[0u] = ttlRespPtr->resp[0u];
                    smartcardContext->statusBytes[1u] = ttlRespPtr->resp[1u];
                    /* Disable WWT timer before exiting T=0 transport */
                    SMARTCARD_DISABLE_WWT(coreParams->x_context);
                    return kSCCoreFail;
                }
            }

            if (dataUnitSize > 1u)
            {
                /* to send/receive more data */
                if (tc < lc)
                {
                    /* send all the remaining bytes */
                    send_data(coreParams, (header + 5u + tc), dataUnitSize);
                    tc += dataUnitSize;
                }
                else
                {
                    /* receive all the remaining bytes */
                    receive_data(coreParams, (ttlRespPtr->resp + re), dataUnitSize, &temp32);
                    if (temp32 != dataUnitSize)
                    {
                        coreParams->errCode = kSCCoreTransportRxFail;
                        /* Disable WWT timer before exiting T=0 transport */
                        SMARTCARD_DISABLE_WWT(coreParams->x_context);
                        return kSCCoreFail;
                    }
                    re += dataUnitSize;
                }
            }
            else if (dataUnitSize == 1)
            {
                /* to send/receive 1 more data */
                if (tc < lc)
                {
                    /* send 1 byte */
                    send_data(coreParams, (header + 5u + tc), 1u);
                    tc++;
                }
                else
                {
                    /* receive 1 byte */
                    receive_data(coreParams, (ttlRespPtr->resp + re), 1u, &temp32);
                    if (temp32 != 1u)
                    {
                        coreParams->errCode = kSCCoreTransportRxFail;
                        /* Disable WWT timer before exiting T=0 transport */
                        SMARTCARD_DISABLE_WWT(coreParams->x_context);
                        return kSCCoreFail;
                    }
                    re++;
                }
            }
        } while (1u);
    }

    /* Disable WWT timer before exiting T=0 transport */
    SMARTCARD_DISABLE_WWT(coreParams->x_context);
    return kSCCoreSuccess;
}

/*!
 * @brief In T=1 transport mode, this function sends the command using
 * driver apis and then handles the card response. Finally it sends the response
 * back to the upper layer.
 */
static smartcard_core_error_t ttl_send_cmd_t1(smartcard_core_params_t *coreParams,
                                              smartcard_ttl_cmd_t *ttlCmd,
                                              smartcard_ttl_resp_t *ttlRespPtr)
{
    assert((NULL != coreParams));
    assert((NULL != ttlCmd));
    assert((NULL != ttlRespPtr));

    int16_t j;
    uint32_t rcvLength = 0u;
    uint16_t ttlCurrentInfLengh = 0u;
    uint16_t ttlRemainingInfBytes = 0u;
    uint16_t gTxBufInfOffset = 0u;
    uint16_t expectedRxLength;
    uint16_t ttlRxLength = 0u;
    uint8_t mWTX = 0u;
    uint8_t txRetryCount = 0u;
    uint8_t rxErrorCount = 0u;
    uint8_t edc = 0u;
    uint8_t blkTypeSent = 0u;
    uint8_t localBuf[5u];
#if defined(FSL_RTOS_FREE_RTOS)
    smartcard_context_t *smartcardContext = &((rtos_smartcard_context_t *)coreParams->x_context)->x_context;
#else
    smartcard_context_t *smartcardContext = (smartcard_context_t *)coreParams->x_context;
#endif

    /* Initialize the local buffer */
    for (j = 0u; j < EMVL1_MAX_TRANSFER_SIZE; j++)
    {
        coreParams->localTxBuf[j] = 0x00u;
    }

    /* Length of the INF part of the block [0-254] */
    ttlRemainingInfBytes = ttlCmd->length;

    /* reset and enable T=1 mode timers */
    smartcardContext->timersState.wwtExpired = false;
    smartcardContext->timersState.cwtExpired = false;
    smartcardContext->timersState.bwtExpired = false;
    SMARTCARD_DISABLE_WWT(coreParams->x_context);
    SMARTCARD_ENABLE_BWT(coreParams->x_context);

    do
    {
        /* Check if INF size is more than IFSC or not */
        if (ttlRemainingInfBytes > smartcardContext->cardParams.IFSC)
        {
            /* Yes. Then break the INF to pieces with more bit set */
            ttlCurrentInfLengh = smartcardContext->cardParams.IFSC;
        }
        else
        {
            /* No. Continue w/o block chaining. */
            ttlCurrentInfLengh = ttlRemainingInfBytes;
        }

        /* Take a local copy */
        for (j = 0u; j < 254u; j++)
        {
            coreParams->localTxBuf[3u + j] = *(ttlCmd->inf + gTxBufInfOffset + j);
        }

        /* Prepare prologue */
        coreParams->localTxBuf[0u] = 0x00u;

        /*
        * PCB: I block
        * b8 = 0,
        * b7 = Sequence number
        * b6 = Chaining (more data)
        * b5-b1 = 0(RFU)
        */

        /* Currently the terminal supports max IFSD = 0xFE so no need to maintain more bit during I-block transmission
         */
        coreParams->localTxBuf[1u] = (coreParams->ttlParamsT1.txISSN << 6u);
        if (ttlCurrentInfLengh < ttlRemainingInfBytes)
        {
            /* Set the more bit */
            coreParams->ttlParamsT1.moreTx = 1u;
            coreParams->localTxBuf[1u] |= (uint8_t)(coreParams->ttlParamsT1.moreTx << 5u);
        }
        else
        {
            /* Set the more bit */
            coreParams->ttlParamsT1.moreTx = 0u;
        }

        coreParams->localTxBuf[2u] = ttlCurrentInfLengh;

        /*
        * The LRC is one byte in length and is calculated as the exclusive-OR
        * of all the bytes starting with the NAD and including the last byte
        * of INF, if present.
        */
        coreParams->localTxBuf[3u + ttlCurrentInfLengh] = 0x00u;
        for (j = 0u; j < (3u + ttlCurrentInfLengh); j++)
        {
            coreParams->localTxBuf[3u + ttlCurrentInfLengh] ^= coreParams->localTxBuf[j];
        }

        do /* to send an I-block */
        {
            /* Send an I-block */
            txRetryCount = 0u;
            send_data(coreParams, coreParams->localTxBuf, (3u + ttlCurrentInfLengh + 1u));
            txRetryCount++;
            blkTypeSent = kSCIBlock;

            /* toggle SSN */
            coreParams->ttlParamsT1.txISSN = (coreParams->ttlParamsT1.txISSN == 0u) ? 1u : 0u;

            if (coreParams->ttlParamsT1.moreTx)
            {
                /* Expecting an R-block/S-Block */
                expectedRxLength = EMVL1_LEN_UNKNOWN_T1;
            }
            else
            {
                if ((coreParams->localTxBuf[3 + ttlCurrentInfLengh - 1] == 0u))
                {
                    expectedRxLength = EMVL1_LEN_UNKNOWN_T1;
                }
                else
                {
                    /* Last in block-chain, so expecting an I-block, 3 + Extract Le + 2 + LRC*/
                    expectedRxLength = 3 + coreParams->localTxBuf[3 + ttlCurrentInfLengh - 1] + 2u + 1u;
                }
            }
            do
            {
                receive_data(coreParams, ttlRespPtr->resp + ttlRxLength, expectedRxLength, &rcvLength);
                SMARTCARD_RESET_BWT(coreParams->x_context);

                if (((rcvLength == 0u) || ((rcvLength < 0x04u) && (smartcardContext->timersState.cwtExpired == 1u)) ||
                     ((rcvLength >= 0x04u) && (ttlRespPtr->resp[ttlRxLength + 2u] != (rcvLength - 3u - 1u)) &&
                      (smartcardContext->timersState.cwtExpired == 1u))) &&
                    (smartcardContext->parityError == 0u))
                {
                    /* Nothing was received;break as of now TBD set relevant error flags */
                    rxErrorCount = 0u;
                    coreParams->errCode = kSCCoreIOError;
                    return kSCCoreFail;
                }

                if (ttlRespPtr->resp[ttlRxLength + 2u] == 0xFFu)
                {
                    rxErrorCount = 0;
                    coreParams->errCode = kSCCoreT1INFLengthInvalid;
                    return kSCCoreFail;
                }
                /*Calculate and check EDC*/
                edc = 0u;
                for (j = 0u; j < rcvLength; j++)
                {
                    edc ^= ttlRespPtr->resp[ttlRxLength + j];
                }
                if (edc || (rcvLength < 0x04u) || (ttlRespPtr->resp[ttlRxLength] != 0x00u) ||
                    ((rcvLength >= 0x04u) && (ttlRespPtr->resp[ttlRxLength + 2u] != (rcvLength - 3 - 1u))) ||
                    (smartcardContext->parityError == 1u))
                {
                    if (++rxErrorCount > EMVL1_T1_BLOCK_RECEIVE_ERROR_MAX)
                    {
                        /*Maximum no. of continuous erroneous block reception done;now deactivate the card*/
                        rxErrorCount = 0u;
                        coreParams->errCode = kSCCoreT1RxErrorExceeded;
                        return kSCCoreFail;
                    }
                    /* Request Retransmission */
                    if ((blkTypeSent == kSCIBlock) || (blkTypeSent == kSCSBlockResponse))
                    {
                        /* transmit an R-Block */
                        /* Transmit an R-Block and get ready to receive */
                        localBuf[0u] = 0x00u;
                        if (((ttlRespPtr->resp[ttlRxLength] != 0x00u) ||
                             (((ttlRespPtr->resp[ttlRxLength + 1u] & 0xE0u) == 0xC0u) &&
                              (ttlRespPtr->resp[ttlRxLength + 2u] > 1u))) &&
                            (smartcardContext->parityError == 0u))
                        {
                            localBuf[1u] = (0x82u | (uint8_t)(coreParams->ttlParamsT1.txRSSN << 4u));
                        }
                        else
                        {
                            localBuf[1u] = (0x81u | (uint8_t)(coreParams->ttlParamsT1.txRSSN << 4u));
                        }

                        localBuf[2u] = 0x00u;
                        localBuf[3u] = localBuf[1u];

                        /* previous block sent was I Block hence clear the counter */
                        txRetryCount = 0u;
                        send_data(coreParams, localBuf, 4u);
                        txRetryCount++;
                        blkTypeSent = kSCRBlock;
                        /* Read back the response */
                        continue;
                    }
                    else if (blkTypeSent == kSCRBlock)
                    {
                        /* Buff still must contain the old values */
                        if (txRetryCount++ < EMVL1_T1_BLOCK_RETRANSMIT_MAX)
                        {
                            send_data(coreParams, localBuf, 4u);
                            blkTypeSent = kSCRBlock;
                            /* Read back the response */
                            continue;
                        }
                        else
                        {
                            /* Limit crossed */
                            txRetryCount = 0u;
                            coreParams->errCode = kSCCoreT1TxRetryExceeded;
                            return kSCCoreFail;
                        }
                    }
                }

                if ((rcvLength == 4u) && ((ttlRespPtr->resp[ttlRxLength + 1u] & 0xE0u) == 0x80u))
                {
                    /* Reset Error Rx counter */
                    rxErrorCount = 0u;

                    /* PCB byte indicates R-block */
                    /* get next sequence number */
                    coreParams->ttlParamsT1.rxRSSN = (uint8_t)(ttlRespPtr->resp[ttlRxLength + 1u] & 0x10u) >> 4u;

                    if ((blkTypeSent == kSCIBlock) || (blkTypeSent == kSCSBlockResponse))
                    {
                        /* Resend previous I-Block if rxRSSN != txISSN */
                        if ((coreParams->ttlParamsT1.rxRSSN != coreParams->ttlParamsT1.txISSN) &&
                            (!(coreParams->ttlParamsT1.moreRx)))
                        {
                            if (txRetryCount++ < EMVL1_T1_BLOCK_RETRANSMIT_MAX)
                            {
                                send_data(coreParams, coreParams->localTxBuf, (3u + ttlCurrentInfLengh + 1u));
                                blkTypeSent = kSCIBlock;
                                continue;
                            }
                            else
                            {
                                /* Limit Crossed */
                                txRetryCount = 0;
                                coreParams->errCode = kSCCoreT1TxRetryExceeded;
                                return kSCCoreFail;
                            }
                        }
                        else if ((coreParams->ttlParamsT1.rxRSSN == coreParams->ttlParamsT1.txISSN) &&
                                 (coreParams->ttlParamsT1.moreTx) &&
                                 ((ttlRespPtr->resp[ttlRxLength + 1u] & 0x0Fu) == 0x00u))
                        {
                            /* Good condition */
                            ttlRemainingInfBytes -= ttlCurrentInfLengh;
                            gTxBufInfOffset += ttlCurrentInfLengh;
                            break;
                        }
                        else
                        {
                            /* Transmit an R-Block and get ready to receive */
                            localBuf[0u] = 0x00u;
                            localBuf[1u] = (0x82u | (uint8_t)(coreParams->ttlParamsT1.txRSSN << 4u));
                            localBuf[2u] = 0x0u;
                            localBuf[3u] = localBuf[1u];

                            txRetryCount = 0u;
                            send_data(coreParams, localBuf, 4u);
                            txRetryCount++;
                            blkTypeSent = kSCRBlock;
                            /* Read back the response */
                            continue;
                        }
                    }
                    else if (blkTypeSent == kSCRBlock)
                    {
                        /* Resend previous Block */
                        if ((coreParams->ttlParamsT1.rxRSSN != coreParams->ttlParamsT1.txISSN) &&
                            (!(coreParams->ttlParamsT1.moreRx)))
                        {
                            /* Resend previous I-Block */
                            txRetryCount = 0u;
                            send_data(coreParams, coreParams->localTxBuf, (3u + ttlCurrentInfLengh + 1u));
                            blkTypeSent = kSCIBlock;
                            txRetryCount++;
                            continue;
                        }
                        else if ((coreParams->ttlParamsT1.rxRSSN == coreParams->ttlParamsT1.txISSN) &&
                                 (coreParams->ttlParamsT1.moreTx) &&
                                 ((ttlRespPtr->resp[ttlRxLength + 1u] & 0x0Fu) == 0x00u))
                        {
                            /* Good condition */
                            ttlRemainingInfBytes -= ttlCurrentInfLengh;
                            gTxBufInfOffset += ttlCurrentInfLengh;
                            break;
                        }
                        else
                        {
                            if (txRetryCount++ < EMVL1_T1_BLOCK_RETRANSMIT_MAX)
                            {
                                /* Local Buffer must contain previous resend value */
                                send_data(coreParams, localBuf, 4);
                                blkTypeSent = kSCRBlock;
                                /* Read back the response */
                                continue;
                            }
                            else
                            {
                                /* Limit Crossed */
                                txRetryCount = 0u;
                                coreParams->errCode = kSCCoreT1TxRetryExceeded;
                                return kSCCoreFail;
                            }
                        }
                    }
                }
                else if ((rcvLength == 4u) && ((ttlRespPtr->resp[ttlRxLength + 1u] & 0xE0u) == 0xC0u) &&
                         (((ttlRespPtr->resp[ttlRxLength + 1u] & 0x1Fu) == 0x00u) ||
                          ((ttlRespPtr->resp[ttlRxLength + 1u] & 0x1Fu) == 0x02u)))
                {
                    /* PCB byte indicates S-block */
                    if ((ttlRespPtr->resp[ttlRxLength + 1u] & 0x1Fu) == 0x00u)
                    {
                        /* Resynchronisation request:TBD */
                    }
                    else if ((ttlRespPtr->resp[ttlRxLength + 1u] & 0x1Fu) == 0x02u)
                    {
                        /* Abort request */
                        rxErrorCount = 0u;
                        coreParams->errCode = kSCCoreT1AbortRequestReceived;
                        return kSCCoreFail;
                    }
                }
                else if ((rcvLength == 5u) && ((ttlRespPtr->resp[ttlRxLength + 1u] & 0xE0u) == 0xC0u) &&
                         ((ttlRespPtr->resp[ttlRxLength + 1u] & 0x1Fu) == 0x01u) &&
                         (ttlRespPtr->resp[ttlRxLength + 3u] >= 0x10u) && (ttlRespPtr->resp[ttlRxLength + 3u] <= 0xFEu))
                {
                    /* Reset Error Rx counter */
                    rxErrorCount = 0u;

                    /* PCB byte indicates S-block */
                    /* IFSC change request */
                    smartcardContext->cardParams.IFSC = ttlRespPtr->resp[ttlRxLength + 3];

                    /* Send response */
                    ttlRespPtr->resp[ttlRxLength + 1u] = 0xE1u;
                    ttlRespPtr->resp[ttlRxLength + 4u] = 0u;
                    for (j = 0u; j < 4u; j++)
                    {
                        ttlRespPtr->resp[ttlRxLength + 4u] ^= ttlRespPtr->resp[ttlRxLength + j];
                    }
                    /* previous block sent was I Block hence clear the counter */
                    txRetryCount = 0u;
                    send_data(coreParams, ttlRespPtr->resp + ttlRxLength, 5u);
                    txRetryCount++;
                    blkTypeSent = kSCSBlockResponse;
                }
                else if ((rcvLength == 5u) && ((ttlRespPtr->resp[ttlRxLength + 1u] & 0xE0u) == 0xC0u) &&
                         ((ttlRespPtr->resp[ttlRxLength + 1u] & 0x1Fu) == 0x03u))
                {
                    /* Reset Error Rx counter */
                    rxErrorCount = 0u;

                    /* PCB byte indicates S-block */
                    /* WTX request */
                    mWTX = ttlRespPtr->resp[ttlRxLength + 3u];
                    /*Send response*/
                    localBuf[0u] = 0x00u;
                    localBuf[1u] = 0xE3u;
                    localBuf[2u] = 0x01u;
                    localBuf[3u] = mWTX;
                    localBuf[4u] = 0x00u;
                    for (j = 0u; j < 4u; j++)
                    {
                        localBuf[4u] ^= localBuf[j];
                    }
                    /*Set the BWT timer for this waiting time window only*/
                    if (mWTX > 1u)
                    {
                        SMARTCARD_SET_WTX(coreParams->x_context, mWTX);
                    }
                    /*Correct data received once;reset the continuous transmission retry counter*/
                    txRetryCount = 0u;
                    send_data(coreParams, localBuf, 5u);
                    txRetryCount++;
                    blkTypeSent = kSCSBlockResponse;
                }
                else if ((coreParams->localTxBuf[3 + ttlCurrentInfLengh - 1u] == 0u) &&
                         (expectedRxLength == EMVL1_LEN_UNKNOWN_T1) &&
                         ((ttlRespPtr->resp[ttlRxLength + 1u] & 0x80u) == 0x00u) &&
                         (((uint8_t)(ttlRespPtr->resp[ttlRxLength + 1u] & 0x40u) >> 6u) ==
                          coreParams->ttlParamsT1.txRSSN))
                {
                    /*Reset Error Rx counter*/
                    rxErrorCount = 0u;

                    /* Received an I-block */
                    coreParams->ttlParamsT1.rxISSN = (uint8_t)(ttlRespPtr->resp[ttlRxLength + 1u] & 0x40u) >> 6u;
                    coreParams->ttlParamsT1.txRSSN = (coreParams->ttlParamsT1.txRSSN == 0u) ? 1u : 0u;

                    if ((ttlRespPtr->resp[ttlRxLength + 1u] & 0x20u) == 0x00u)
                    {
                        /* It is a good condition */
                        ttlRemainingInfBytes -= ttlCurrentInfLengh;
                        coreParams->errCode = kSCCoreSuccess;
                        smartcardContext->statusBytes[0u] = ttlRespPtr->resp[ttlRxLength + expectedRxLength - 3u];
                        smartcardContext->statusBytes[1u] = ttlRespPtr->resp[ttlRxLength + expectedRxLength - 2u];

                        for (j = 0u; j < (rcvLength - 3u - 1u); j++)
                        {
                            ttlRespPtr->resp[ttlRxLength + j] = ttlRespPtr->resp[ttlRxLength + j + 3u];
                        }

                        ttlRxLength += (rcvLength - 3u - 1u);
                        ttlRespPtr->length = ttlRxLength;

                        coreParams->ttlParamsT1.moreRx = 0u;
                        return kSCCoreSuccess;
                    }
                    else
                    {
                        /* A chained I-block */
                        /* Transmit an R-Block and get ready to receive */
                        localBuf[0u] = 0x00u;
                        localBuf[1u] = (0x80u | (uint8_t)(coreParams->ttlParamsT1.txRSSN << 4u));
                        localBuf[2u] = 0x00u;
                        localBuf[3u] = localBuf[1u];

                        send_data(coreParams, localBuf, 4u);
                        blkTypeSent = kSCRBlock;
                        coreParams->ttlParamsT1.moreRx = 1u;

                        for (j = 0u; j < (rcvLength - 3u - 1u); j++)
                        {
                            ttlRespPtr->resp[ttlRxLength + j] = ttlRespPtr->resp[ttlRxLength + j + 3u];
                        }

                        ttlRxLength += (rcvLength - 3u - 1u);
                    }
                }
                else if ((rcvLength == expectedRxLength) && ((ttlRespPtr->resp[ttlRxLength + 1] & 0x80u) == 0x00u) &&
                         (((uint8_t)(ttlRespPtr->resp[ttlRxLength + 1u] & 0x40u) >> 6u) ==
                          coreParams->ttlParamsT1.txRSSN))
                {
                    /* Received an I-block */
                    /* Reset Error Rx counter */
                    rxErrorCount = 0u;

                    coreParams->ttlParamsT1.rxISSN = (uint8_t)(ttlRespPtr->resp[ttlRxLength + 1u] & 0x40u) >> 6u;
                    coreParams->ttlParamsT1.txRSSN = (coreParams->ttlParamsT1.txRSSN == 0u) ? 1u : 0u;

                    ttlRemainingInfBytes -= ttlCurrentInfLengh;
                    coreParams->errCode = kSCCoreSuccess;
                    smartcardContext->statusBytes[0u] = ttlRespPtr->resp[ttlRxLength + expectedRxLength - 3u];
                    smartcardContext->statusBytes[1u] = ttlRespPtr->resp[ttlRxLength + expectedRxLength - 2u];

                    for (j = 0u; j < (rcvLength - 3u - 1u); j++)
                    {
                        ttlRespPtr->resp[ttlRxLength + j] = ttlRespPtr->resp[ttlRxLength + j + 3u];
                    }

                    ttlRxLength += (rcvLength - 3u - 1u);
                    ttlRespPtr->length = ttlRxLength;

                    coreParams->ttlParamsT1.moreRx = 0u;
                    break;
                }
                else
                {
                    /* Semantic Error */
                    /* Erroneous Block,Request retransmission */
                    if (++rxErrorCount > EMVL1_T1_BLOCK_RECEIVE_ERROR_MAX)
                    {
                        /* Maximum no. of continuous erroneous block reception done;now deactivate the card */
                        rxErrorCount = 0u;
                        coreParams->errCode = kSCCoreT1RxErrorExceeded;
                        return kSCCoreFail;
                    }

                    if ((blkTypeSent == kSCIBlock) || (blkTypeSent == kSCSBlockResponse))
                    {
                        /* transmit an R-Block */
                        /* Transmit an R-Block and get ready to receive */
                        localBuf[0u] = 0x00u;
                        localBuf[1u] = (0x82u | (uint8_t)(coreParams->ttlParamsT1.txRSSN << 4u));
                        localBuf[2u] = 0x00u;
                        localBuf[3u] = localBuf[1u];

                        txRetryCount = 0u;
                        send_data(coreParams, localBuf, 4u);
                        txRetryCount++;
                        blkTypeSent = kSCRBlock;
                        /* Read back the response */
                        continue;
                    }
                    else if (blkTypeSent == kSCRBlock)
                    {
                        /* Buff still must contain the old values */
                        if (txRetryCount++ < EMVL1_T1_BLOCK_RETRANSMIT_MAX)
                        {
                            send_data(coreParams, localBuf, 4u);
                            blkTypeSent = kSCRBlock;
                            /* Read back the response */
                            continue;
                        }
                        else
                        {
                            /* Limit crossed */
                            txRetryCount = 0u;
                            coreParams->errCode = kSCCoreT1TxRetryExceeded;
                            return kSCCoreFail;
                        }
                    }
                }
            } while (1u);

            if ((rcvLength == expectedRxLength) || (coreParams->ttlParamsT1.moreTx))
            {
                /* Break */
                break;
            }

        } while (1u);

        if (rcvLength == 0x00u)
        {
            /* Nothing was received;break as of now TBD set relevant error flags */
            break;
        }
    } while (ttlRemainingInfBytes > 0u);

    SMARTCARD_DISABLE_CWT(coreParams->x_context);
    SMARTCARD_DISABLE_BWT(coreParams->x_context);

    ttlRespPtr->length = ttlRxLength;

    return kSCCoreSuccess;
}

smartcard_core_error_t smartcard_tal_send_cmd(smartcard_core_params_t *coreParams,
                                              smartcard_tal_cmd_t *talCmd,
                                              smartcard_tal_resp_t *talResp)
{
    if ((NULL == coreParams) || (NULL == talCmd) || (NULL == talResp) || (NULL == talCmd->apdu) ||
        (NULL == talResp->resp))
    {
        return kSCCoreInvalidInputParameter;
    }

    smartcard_ttl_cmd_t ttlCmd;
    smartcard_ttl_resp_t ttlResp;
    smartcard_core_error_t errCode;
#if defined(FSL_RTOS_FREE_RTOS) || defined(FSL_RTOS_UCOSII) || defined(FSL_RTOS_UCOSIII)
    smartcard_context_t *smartcardContext = &((rtos_smartcard_context_t *)coreParams->x_context)->x_context;
#else
    smartcard_context_t *smartcardContext = (smartcard_context_t *)coreParams->x_context;
#endif

    if (talCmd->length == 4u)
    {
        coreParams->Case = kSCCommandCase1;
    }
    else if (talCmd->length == (4u + 1u))
    {
        coreParams->Case = kSCCommandCase2;
    }
    else if (talCmd->length == (4u + 1u + talCmd->apdu[4u]))
    {
        coreParams->Case = kSCCommandCase3;
    }
    else if (talCmd->length == (4u + 1u + talCmd->apdu[4u] + 1u))
    {
        coreParams->Case = kSCCommandCase4;
    }
    else
    {
        return kSCCoreFail;
    }

    ttlResp.resp = talResp->resp;
    ttlResp.length = 0u;

#if !defined(ISO7816_CARD_SUPPORT)
    /* EMV specification does not support ICCs having both T=0 and T=1 protocols
    present at the same time. */
    if (smartcardContext->cardParams.t0Indicated)
    {
        smartcard_setup_t0(coreParams);
    }
    else if (smartcardContext->cardParams.t1Indicated)
    {
        smartcard_setup_t1(coreParams);
    }
    else
    {
        return kSCCoreFail;
    }
#endif
    if (smartcardContext->tType == kSMARTCARD_T0Transport)
    {
        /* Do C-APDU to TAL mapping */
        if (coreParams->Case == kSCCommandCase1)
        {
            talCmd->header = talCmd->apdu;
            *(talCmd->header + 4u) = 0u;
        }
        else if ((coreParams->Case == kSCCommandCase2) || (coreParams->Case == kSCCommandCase3) ||
                 (coreParams->Case == kSCCommandCase4))
        {
            talCmd->header = talCmd->apdu;
        }

        ttlCmd.inf = talCmd->apdu;
        errCode = ttl_send_cmd(coreParams, &ttlCmd, &ttlResp);

        talResp->length = ttlResp.length;
    }
    else
    {
        /* kSMARTCARD_T1Transport */

        /* Do C-APDU to TAL mapping */
        ttlCmd.inf = talCmd->apdu;
        ttlCmd.length = talCmd->length;

        errCode = ttl_send_cmd(coreParams, &ttlCmd, &ttlResp);

        if (ttlResp.length >= 2u)
        {
            talResp->length = ttlResp.length - 2;
        }
        else
        {
            talResp->length = 0u;
        }
    }

    return errCode;
}

smartcard_core_error_t smartcard_reset_handle_atr(
    smartcard_core_params_t *coreParams, uint8_t *buff, uint8_t coldReset, uint8_t warmReset, uint32_t *atrLength)
{
    if ((NULL == coreParams) || (NULL == buff) || (NULL == atrLength))
    {
        return kSCCoreInvalidInputParameter;
    }

    uint16_t length = EMVL1_MAX_TRANSFER_SIZE;
    bool deactivateCard = 0u;
#if defined(FSL_RTOS_FREE_RTOS) || defined(FSL_RTOS_UCOSII) || defined(FSL_RTOS_UCOSIII)
    smartcard_context_t *smartcardContext = &((rtos_smartcard_context_t *)coreParams->x_context)->x_context;
#else
    smartcard_context_t *smartcardContext = (smartcard_context_t *)coreParams->x_context;
#endif
    smartcard_card_params_t *cardParams = &smartcardContext->cardParams;

    coreParams->ttlParamsT1.rxISSN = 0u;
    coreParams->ttlParamsT1.rxRSSN = 0u;
    coreParams->ttlParamsT1.txISSN = 0u;
    coreParams->ttlParamsT1.txRSSN = 0u;
    coreParams->ttlParamsT1.moreTx = 0u;
    coreParams->ttlParamsT1.moreRx = 0u;

    coreParams->doWarmReset = false;

    if ((coldReset != kSMARTCARD_ColdReset) && (warmReset != kSMARTCARD_WarmReset))
    {
        return kSCCoreFail;
    }

    while (coldReset == kSMARTCARD_ColdReset)
    {
        SMARTCARD_SETUP_ATR(coreParams->x_context);
        /*Start proceedings for cold ATR*/
        reset_smartcard(coreParams, kSMARTCARD_ColdReset);

        smartcardContext->timersState.adtExpired = false;
        smartcardContext->timersState.wwtExpired = false;
        SMARTCARD_RESET_WWT(coreParams->x_context);
        SMARTCARD_ENABLE_ADT(coreParams->x_context);
        SMARTCARD_DISABLE_WWT(coreParams->x_context);

        smartcardContext->transferState = kSMARTCARD_WaitingForTSState;
        smartcardContext->xIsBusy = true;
        /*Wait for Init char.In case wrong Init char comes, reject the card
        In case, Init char comes after 42000 clock cycles of RST , reject the card*/
        /* Wait until all the data is received or for timeout.*/
        wait_for_card_reset(coreParams);

        if (smartcardContext->timersState.initCharTimerExpired ||
            smartcardContext->transferState == kSMARTCARD_InvalidTSDetecetedState)
        {
            /* reject the card*/
            deactivateCard = true;
            break;
        }
        else
        {
            /* Store initial TS character into buffer and increase buff.
             * In case of invalid ATR or ATR is not complete, if warm reset supported, it's required
             * to decreased buff to avoid multiple TS character insertion. */
            if (cardParams->convention)
            { /* Inverse convention detected, TS = 0x3F */
                *(buff++) = 0x3Fu;
            }
            else
            { /* Direct convention detected, TS = 0x3B */
                *(buff++) = 0x3Bu;
            }
            /* Start WWT,CWT and ADT  timer */
            smartcardContext->timersState.wwtExpired = false;
            smartcardContext->timersState.cwtExpired = false;

            /* Enable WWT and ADT timer before starting T=0 transport */
            SMARTCARD_ENABLE_WWT(coreParams->x_context);

            /*Receiving cold ATR*/
            smartcardContext->transferState = kSMARTCARD_ReceivingState;
            receive_data(coreParams, buff, length, atrLength);

            SMARTCARD_DISABLE_ADT(coreParams->x_context);
            SMARTCARD_DISABLE_WWT(coreParams->x_context);

            /*In case there is a parity error in ATR,reject the card*/
            if (smartcardContext->rxtCrossed)
            {
                smartcardContext->rxtCrossed = false;
                deactivateCard = true;
                break;
            }
            else
            {
                /*Parsing and checking cold ATR*/
                if (smartcard_parse_atr(coreParams, buff, *atrLength) == kSCCoreFail)
                {
                    if (smartcardContext->timersState.wwtExpired)
                    {
                        smartcardContext->timersState.wwtExpired = false;
                        if (!cardParams->atrComplete)
                        {
                            /*Card has to be rejected as WT expired while full ATR was not received*/
                            smartcardContext->timersState.wwtExpired = false;
                            deactivateCard = true;
                            break;
                        }
                        else if (!cardParams->atrValid)
                        {
                            /*Complete ATR was received but was invalid;hence perform warm reset*/
                            coreParams->doWarmReset = true;
                            break;
                        }
                    }
                    else if (smartcardContext->timersState.adtExpired)
                    {
                        smartcardContext->timersState.adtExpired = false;
                        if (!cardParams->atrComplete)
                        {
                            /*Card has to be rejected as ADT expired while full ATR was not received*/
                            smartcardContext->timersState.wwtExpired = false;
                            deactivateCard = true;
                            break;
                        }
                        else if (!cardParams->atrValid)
                        {
                            /*Complete ATR was received but was invalid;hence perform warm reset*/
                            coreParams->doWarmReset = true;
                            break;
                        }
                    }
                    else
                    {
                        /*ATR has to be rejected only and start warm reset sequence if requested */
                        coreParams->doWarmReset = true;
                        break;
                    }
                }
                else
                {
                    if (!cardParams->modeNegotiable)
                    {
                        cardParams->currentD = cardParams->Di;
                        SMARTCARD_CONFIG_BAUDRATE(coreParams->x_context);
                    }
                    /* Increase received ATR atrLength of added initial TS character */
                    (*atrLength)++;
                    /*ATR has been successfully received; return the length of ATR received*/
                    return kSCCoreSuccess;
                }
            }
        }
    } /* doColdReset */

    while ((!deactivateCard) && (warmReset == kSMARTCARD_WarmReset))
    {
        *atrLength = 0;

        if (coreParams->doWarmReset)
        { /* shift pointer to buffer back of added TS character
           * during cold reset sequence */
            buff--;
        }
        /* Start proceedings for warm ATR */
        reset_smartcard(coreParams, kSMARTCARD_WarmReset);

        smartcardContext->timersState.adtExpired = false;
        smartcardContext->timersState.wwtExpired = false;
        SMARTCARD_RESET_WWT(coreParams->x_context);
        SMARTCARD_ENABLE_ADT(coreParams->x_context);
        SMARTCARD_DISABLE_WWT(coreParams->x_context);

        smartcardContext->transferState = kSMARTCARD_WaitingForTSState;
        smartcardContext->xIsBusy = true;
        /*Wait for Init char.In case wrong Init char comes, reject the card
        In case, Init char comes after 42000 clock cycles of RST , reject the card*/
        /* Wait until all the data is received or for timeout.*/
        wait_for_card_reset(coreParams);

        if (smartcardContext->timersState.initCharTimerExpired ||
            smartcardContext->transferState == kSMARTCARD_InvalidTSDetecetedState)
        {
            /*reject the card*/
            deactivateCard = true;
            break;
        }
        else
        {
            /* Store initial TS character into buffer and increase buff */
            if (cardParams->convention)
            { /* Inverse convention detected, TS = 0x3F */
                *(buff++) = 0x3Fu;
            }
            else
            { /* Direct convention detected, TS = 0x3B */
                *(buff++) = 0x3Bu;
            }
            /* Start WWT,CWT and ADT timer */
            smartcardContext->timersState.wwtExpired = false;
            smartcardContext->timersState.cwtExpired = false;

            /* Enable WWT and ADT timer before starting T=0 transport */
            SMARTCARD_ENABLE_WWT(coreParams->x_context);

            /*Receiving warm ATR*/
            smartcardContext->transferState = kSMARTCARD_ReceivingState;
            receive_data(coreParams, buff, length, atrLength);

            SMARTCARD_DISABLE_ADT(coreParams->x_context);
            SMARTCARD_DISABLE_WWT(coreParams->x_context);

            /*In case there is a parity error in ATR,reject the card*/
            if (smartcardContext->rxtCrossed)
            {
                smartcardContext->rxtCrossed = false;
                deactivateCard = true;
                break;
            }
            else
            {
                /*Parsing and checking warm ATR*/
                if (smartcard_parse_atr(coreParams, buff, *atrLength) == kSCCoreFail)
                {
                    /*ATR has to be rejected ; hence begin deactivation sequence */
                    deactivateCard = true;
                    break;
                }
                else
                {
                    if (!cardParams->modeNegotiable)
                    {
                        cardParams->currentD = cardParams->Di;
                        SMARTCARD_CONFIG_BAUDRATE(coreParams->x_context);
                    }
                    /*WARM ATR has been successfully received; return the length of ATR received*/
                    return kSCCoreSuccess;
                }
            }
        }
    } /* doWarmReset */
    if (deactivateCard)
    {
        /*Start deactivation sequence;send the command to HAL*/
        SMARTCARD_PHY_DEACTIVATE(coreParams->x_context);
        return kSCCoreFail;
    }
    else
    {
        if (!cardParams->modeNegotiable)
        {
            cardParams->currentD = cardParams->Di;
            SMARTCARD_CONFIG_BAUDRATE(coreParams->x_context);
        }
        return kSCCoreSuccess;
    }
}

smartcard_core_error_t smartcard_parse_atr(smartcard_core_params_t *coreParams, uint8_t *buff, uint16_t length)
{
    assert((NULL != coreParams) && (NULL != buff));

    uint16_t Fi;
    uint8_t ch;
    uint16_t i;
    uint8_t TCK = 0u;
    uint8_t Yi = 0u;
    uint8_t Y1;
    uint8_t Y2;
    uint8_t historicalBytes = 0u;
    uint8_t searchTA3 = 1u;
    uint8_t searchTB3 = 1u;
    uint8_t searchTC3 = 1u;

#if defined(FSL_RTOS_FREE_RTOS) || defined(FSL_RTOS_UCOSII) || defined(FSL_RTOS_UCOSIII)
    smartcard_context_t *smartcardContext = &((rtos_smartcard_context_t *)coreParams->x_context)->x_context;
#else
    smartcard_context_t *smartcardContext = (smartcard_context_t *)coreParams->x_context;
#endif
    smartcard_card_params_t *cardParams = &smartcardContext->cardParams;

    cardParams->modeNegotiable = true;
    cardParams->atrComplete = true;
    cardParams->atrValid = true;

    for (i = 0u; i < length; i++)
    {
        TCK ^= buff[i];
    }

    if (length == 0u)
    {
        cardParams->atrComplete = false;
        return kSCCoreFail;
    }

    /* parsing data */
    i = 0u;

    /*STORING y1 TO CHECK WHICH COMPONENTS ARE AVAILABLE*/
    ch = buff[i];
    Y1 = ch >> 4u;
    historicalBytes = ch & 0x0fu;
    length -= historicalBytes;
    i++;

    if (Y1 & 0x1u)
    {
        /*TA1 present*/
        if (i >= length)
        {
            /*TA1 not found,hence generate error*/
            cardParams->atrComplete = false;
            return kSCCoreFail;
        }
        ch = buff[i];
        Fi = ch >> 4u;

        /*switch to the real Fi value, according to the table of Spec-3, chapter 8.3 */
        switch (Fi)
        {
/*Fi = 1 is only allowed value*/
#if defined(ISO7816_CARD_SUPPORT)
            case 0u:
                cardParams->Fi = 372u;
                cardParams->fMax = 4u;
                break;
#endif
            case 1u:
                cardParams->Fi = 372u;
                cardParams->fMax = 5u;
                break;
#if defined(ISO7816_CARD_SUPPORT)
            case 2u:
                cardParams->Fi = 558u;
                cardParams->fMax = 6u;
                break;
            case 3u:
                cardParams->Fi = 744u;
                cardParams->fMax = 8u;
                break;
            case 4u:
                cardParams->Fi = 1116u;
                cardParams->fMax = 12u;
                break;
            case 5u:
                cardParams->Fi = 1488u;
                cardParams->fMax = 16u;
                break;
            case 6u:
                cardParams->Fi = 1860u;
                cardParams->fMax = 20u;
                break;
            case 9u:
                cardParams->Fi = 512u;
                cardParams->fMax = 5u;
                break;
            case 10u:
                cardParams->Fi = 768u;
                cardParams->fMax = 7u;
                break;
            case 11u:
                cardParams->Fi = 1024u;
                cardParams->fMax = 10u;
                break;
            case 12u:
                cardParams->Fi = 1536u;
                cardParams->fMax = 15u;
                break;
            case 13u:
                cardParams->Fi = 2048u;
                cardParams->fMax = 20u;
                break;
#endif
            default:
                cardParams->Fi = 372u;
                cardParams->fMax = 5u;
                cardParams->atrValid = false;
                return kSCCoreFail;
        }

        cardParams->Di = ch & 0xfu;
        switch (cardParams->Di)
        {
            /*Di = 4 is only allowed value*/
            case 1u:
                cardParams->Di = 1u;
                break;
            case 2u:
                cardParams->Di = 2u;
                break;
            case 3u:
                cardParams->Di = 4u;
                break;
#if defined(ISO7816_CARD_SUPPORT)
            case 4u:
                cardParams->Di = 8u;
                break;
            case 5u:
                cardParams->Di = 16u;
                break;
            case 6u:
                cardParams->Di = 32u;
                break;
            case 7u:
                cardParams->Di = 64u;
                break;
            case 8u:
                cardParams->Di = 12u;
                break;
            case 9u:
                cardParams->Di = 20u;
                break;
#endif
            default:
                cardParams->Di = 1;
                cardParams->atrValid = false;
                return kSCCoreFail;
        }
        i++;
    }
    else
    {
        cardParams->Fi = 372u;
        cardParams->fMax = 5u;
        cardParams->Di = 1u;
    }

    if (Y1 & 0x2u)
    {
        /*TB1 present*/
        if (i >= length)
        {
            /*TB1 not found,hence generate error*/
            cardParams->atrComplete = false;
            return kSCCoreFail;
        }
        ch = buff[i];
        if ((smartcardContext->resetType == kSMARTCARD_ColdReset) && (ch > 0x00u))
        {
            cardParams->atrValid = false;
            return kSCCoreFail;
        }
        i++;
    }
#if !defined(ISO7816_CARD_SUPPORT)
    else
    {
        if (smartcardContext->resetType == kSMARTCARD_ColdReset)
        {
            /*TB1 must be explicitly null in a COLD ATR*/
            cardParams->atrValid = false;
            return kSCCoreFail;
        }
    }
#endif
    if (Y1 & 0x4u)
    { /* TC1 present */
        if (i >= length)
        { /* TC1 not found,hence generate error */
            cardParams->atrComplete = false;
            return kSCCoreFail;
        }
        ch = buff[i];
        if (ch == 0xffu)
        {
            cardParams->GTN = 0xFFu;
        }
        else
        {
            cardParams->GTN = ch;
        }

        i++;
    }
    else
    {
        cardParams->GTN = 0x00u;
    }

    if (Y1 & 0x8u)
    {
        /*TD1 present*/
        if (i >= length)
        { /* TD1 not found,hence generate error */
            cardParams->atrComplete = false;
            return kSCCoreFail;
        }
        ch = buff[i];
        if ((buff[i] & 0xfu) == 0u)
        { /* T=0 is first offered protocol and hence must be used until TD2 is present
           * indicating presence of T=1 and TA2 is absent indicating negotiable mode */
            cardParams->t0Indicated = true;
            cardParams->t1Indicated = false;
        }
        else if ((buff[i] & 0xfu) == 1u)
        { /* T=1 is first offered protocol and hence must be used
           * TA2 is absent indicating negotiable mode */
            cardParams->t1Indicated = true;
            cardParams->t0Indicated = false;
        }
        else
        {
            cardParams->atrValid = false;
            return kSCCoreFail;
        }
        Y2 = ch >> 4u;
        i++;
    }
    else
    { /* TD1 absent,meaning only T= 0 has to be used */
        cardParams->t0Indicated = true;
        cardParams->t1Indicated = false;
        Y2 = 0u;
    }
    if (Y2 & 0x1u)
    { /* TA2 present */
        if (i >= length)
        { /* TA2 not found,hence generate error */
            cardParams->atrComplete = false;
            return kSCCoreFail;
        }
        cardParams->modeNegotiable = false;
        ch = buff[i];
        if ((ch & 0x0fu) == 0u)
        { /* T = 0 specific mode */
            cardParams->t0Indicated = true;
            cardParams->t1Indicated = false;
        }
        else if ((ch & 0x0fu) == 1u)
        { /* T = 1 specific mode */
            if (!cardParams->t1Indicated)
            {
                cardParams->atrValid = false;
                return kSCCoreFail;
            }
            else
            {
                cardParams->t0Indicated = false;
            }
        }
        else
        { /* Unsupported Protocol type */
            cardParams->atrValid = false;
            return kSCCoreFail;
        }
        if (ch & 0x10u)
        {
            cardParams->atrValid = false;
            /* b5 should be zero;hence generate error */
            return kSCCoreFail;
        }
        i++;
    }
    else
    { /* TA2 absent */
        cardParams->modeNegotiable = true;
    }

    if (Y2 & 0x2u)
    { /* TB2 present */
        if (i >= length)
        { /* TB2 not found,hence generate error */
            cardParams->atrComplete = false;
            return kSCCoreFail;
        }
        i++;
        /* TB2 is not supported by EMV ;hence generate error */
        cardParams->atrValid = false;
        return kSCCoreFail;
    }

    if (Y2 & 0x4u)
    { /* TC2 present */
        if (i >= length)
        { /* TC2 not found,hence generate error */
            cardParams->atrComplete = false;
            return kSCCoreFail;
        }
        /* ch contains temp value of WI sent by ICC */
        ch = buff[i];

        if (ch == 0u)
        { /* WI can't be zero */
            cardParams->atrValid = false;
            return kSCCoreFail;
        }
        else
        { /* reject an ATR containing TC2 having any other value (0x0A) unless it is able to support it */
            cardParams->WI = ch;
        }
        i++;
    }
    else
    { /* TC2 absent;hence WI = 0x0A ; default value */
        cardParams->WI = 0x0Au;
    }
    if (Y2 & 0x8u)
    {
        /* TD2 present */
        if (i >= length)
        {
            /* TD2 not found,hence generate error */
            cardParams->atrComplete = false;
            return kSCCoreFail;
        }
        if ((buff[i] & 0xfu) == 0u)
        {
            /* lsb can't be zero */
            cardParams->atrValid = false;
            return kSCCoreFail;
        }
        else if ((buff[i] & 0xfu) == 1u)
        {
            if (cardParams->modeNegotiable)
            {
                cardParams->t1Indicated = true;
            }
        }
        else if ((buff[i] & 0xfu) == 0x0Eu)
        {
            if (!cardParams->t1Indicated)
            {
                /*Do nothing*/
            }
            else
            {
                cardParams->atrValid = false;
                return kSCCoreFail;
            }
        }
        else
        { /* Unsupported values */
            cardParams->atrValid = false;
            return kSCCoreFail;
        }
        Yi = buff[i] >> 4;
        i++;
    }
    else
    {
        Yi = 0u;
    }

    do
    {
        if (Yi & 0x1u)
        { /* TAi present */
            if (i >= length)
            { /* TAi not found;hence generate error */
                cardParams->atrComplete = false;
                return kSCCoreFail;
            }
            if (searchTA3)
            {
                if ((buff[i] >= 0x10u) && (buff[i] <= 0xFEu))
                {
                    cardParams->IFSC = buff[i];
                }
                else
                { /* Unsupported value of TA3 */
                    cardParams->atrValid = false;
                    return kSCCoreFail;
                }
                searchTA3 = 0u;
            }
            i++;
        }
        else
        { /* TAi absent */
            if (searchTA3)
            { /* Default value of TA3 */
                cardParams->IFSC = 0x20u;
                searchTA3 = 0u;
            }
        }
        if (Yi & 0x2u)
        { /* TBi present */
            if (i >= length)
            { /* TBi not found;hence generate error */
                cardParams->atrComplete = false;
                return kSCCoreFail;
            }
            if (searchTB3)
            {
#if !defined(ISO7816_CARD_SUPPORT)
                if (((buff[i] >> 4u) <= 4u) && ((buff[i] & 0x0Fu) <= 5u))
                {
                    cardParams->BWI = buff[i] >> 4u;
                    cardParams->CWI = buff[i] & 0x0Fu;
                }
                else
                {
                    cardParams->atrValid = false;
                    return kSCCoreFail;
                }
#else
                cardParams->BWI = buff[i] >> 4u;
                cardParams->CWI = buff[i] & 0x0Fu;
#endif
                searchTB3 = 0u;
            }
            i++;
        }
        else /* TBi absent */
        {
            if (searchTB3)
            {
                if (cardParams->t1Indicated)
                { /* The terminal shall reject an ATR not containing TB3 */
                    cardParams->atrValid = false;
                    return kSCCoreFail;
                }
            }
        }
        if (Yi & 0x4u)
        { /* TCi present */
            if (i >= length)
            { /* TCi not found;hence generate error */
                cardParams->atrComplete = false;
                return kSCCoreFail;
            }
            if (searchTC3)
            {
                searchTC3 = 0x00u;
                if (buff[i] != 0x00u)
                {
                    cardParams->atrValid = false;
                    return kSCCoreFail;
                }
            }
            i++;
        }
        else
        { /* TCi absent */
            searchTC3 = 0u;
        }
        if (Yi & 0x8u)
        { /* TDi present */
            if (i >= length)
            { /* TDi not found;hence generate error */
                cardParams->atrComplete = false;
                return kSCCoreFail;
            }
            Yi = buff[i] >> 4u;
            i++;
        }
        else
        { /* TDi absent */
            Yi = 0u;
        }

    } while (Yi);

    if (i < length - 1u)
    { /* more than one extra char means that at least one unexpected char is there
       * Wrong or unexpected character */
        cardParams->atrValid = false;
        return kSCCoreFail;
    }
    else if (i == length - 1u)
    { /* only one extra char is there which may be TCK or unexpected byte */
        if (TCK)
        { /* Wrong or unexpected character */
            if (cardParams->t0Indicated)
            {
                cardParams->atrValid = false;
            }
            else
            {
                cardParams->atrComplete = false;
            }
            return kSCCoreFail;
        }
    }
    else
    { /* no extra char */
        if (cardParams->t1Indicated)
        { /* no TCK byte */
            cardParams->atrComplete = false;
            return kSCCoreFail;
        }
    }
    /* CWT should be greater than CGT */
    if (cardParams->t1Indicated)
    {
        if ((cardParams->GTN) != 0xFFu)
        {
            if (((1 << (cardParams->CWI)) <= (cardParams->GTN + 1)))
            {
                cardParams->atrValid = false;
                return kSCCoreFail;
            }
        }
    }

    return kSCCoreSuccess;
}

smartcard_core_error_t smartcard_setup_t0(smartcard_core_params_t *coreParams)
{
    if ((NULL == coreParams))
    {
        return kSCCoreInvalidInputParameter;
    }
    SMARTCARD_SETUP_T0(coreParams->x_context);

    return kSCCoreSuccess;
}

smartcard_core_error_t smartcard_setup_t1(smartcard_core_params_t *coreParams)
{
    if ((NULL == coreParams))
    {
        return kSCCoreInvalidInputParameter;
    }

    SMARTCARD_SETUP_T1(coreParams->x_context);

    return kSCCoreSuccess;
}

smartcard_core_error_t smartcard_send_pps(smartcard_core_params_t *coreParams,
                                          uint8_t *ppsRequest,
                                          uint8_t *ppsResponse,
                                          uint16_t length)
{
    if ((NULL == coreParams) || (NULL == ppsRequest) || (NULL == ppsResponse))
    {
        return kSCCoreInvalidInputParameter;
    }

    uint32_t rcvLength;
#if defined(FSL_RTOS_FREE_RTOS) || defined(FSL_RTOS_UCOSII) || defined(FSL_RTOS_UCOSIII)
    smartcard_context_t *smartcardContext = &((rtos_smartcard_context_t *)coreParams->x_context)->x_context;
#else
    smartcard_context_t *smartcardContext = (smartcard_context_t *)coreParams->x_context;
#endif

    smartcardContext->timersState.wwtExpired = false;
    smartcardContext->timersState.cwtExpired = false;
    smartcardContext->timersState.bwtExpired = false;

    /* Reset WWT timer */
    SMARTCARD_RESET_WWT(coreParams->x_context);

    /* Enable WWT timer before starting T=0 transport */
    SMARTCARD_ENABLE_WWT(coreParams->x_context);
    send_data(coreParams, ppsRequest, length);

    receive_data(coreParams, ppsResponse, length, &rcvLength);

    SMARTCARD_DISABLE_WWT(coreParams->x_context);

    coreParams->ppsDone = (rcvLength != length) ? false : true;

    return kSCCoreSuccess;
}

smartcard_core_error_t smartcard_send_ifs(smartcard_core_params_t *coreParams, uint32_t *respLength)
{
    if ((NULL == coreParams) || (NULL == respLength))
    {
        return kSCCoreInvalidInputParameter;
    }

    uint8_t localCmdBuf[5] = {0};
    uint8_t localRespBuf[20] = {0};
    uint8_t j;
#if defined(FSL_RTOS_FREE_RTOS) || defined(FSL_RTOS_UCOSII) || defined(FSL_RTOS_UCOSIII)
    smartcard_context_t *smartcardContext = &((rtos_smartcard_context_t *)coreParams->x_context)->x_context;
#else
    smartcard_context_t *smartcardContext = (smartcard_context_t *)coreParams->x_context;
#endif

    SMARTCARD_SETUP_T1(coreParams->x_context);

    smartcardContext->timersState.wwtExpired = false;
    smartcardContext->timersState.cwtExpired = false;
    smartcardContext->timersState.bwtExpired = false;
    smartcardContext->parityError = 0;

    SMARTCARD_DISABLE_WWT(coreParams->x_context);
    SMARTCARD_RESET_BWT(coreParams->x_context);
    SMARTCARD_ENABLE_BWT(coreParams->x_context);

    localCmdBuf[0u] = 0x00u;
    localCmdBuf[1u] = 0xC1u;
    localCmdBuf[2u] = 0x01u;
    localCmdBuf[3u] = 0xFEu;
    localCmdBuf[4u] = 0x3Eu;

    for (j = 0u; j < EMVL1_IFS_ATTEMPTS; j++)
    {
        send_data(coreParams, localCmdBuf, 5u);

        receive_data(coreParams, localRespBuf, sizeof(localRespBuf), respLength);

        if ((*respLength < 5u) && (smartcardContext->parityError == 0u) && ((localRespBuf[1] & 0x80u) == 0x00u))
        {
            break;
        }

        if (*respLength != 5u)
        {
            continue;
        }

        if ((localRespBuf[0u] == 0x00u) && (localRespBuf[1] == 0xE1u) && (localRespBuf[2u] == 0x01u) &&
            (localRespBuf[3u] == 0xFEu) && (localRespBuf[4] == 0x1Eu))
        {
            SMARTCARD_DISABLE_CWT(coreParams->x_context);
            SMARTCARD_DISABLE_BWT(coreParams->x_context);

            return kSCCoreSuccess;
        }
    }

    SMARTCARD_DISABLE_CWT(coreParams->x_context);
    SMARTCARD_DISABLE_BWT(coreParams->x_context);

    return kSCCoreFail;
}

smartcard_core_error_t smartcard_check_status(smartcard_core_params_t *coreParams, uint8_t *statusBytes)
{
    if ((NULL == coreParams) || (NULL == statusBytes))
    {
        return kSCCoreInvalidInputParameter;
    }

    smartcard_core_error_t errCode = kSCCoreSuccess;
#if defined(FSL_RTOS_FREE_RTOS) || defined(FSL_RTOS_UCOSII) || defined(FSL_RTOS_UCOSIII)
    smartcard_context_t *smartcardContext = &((rtos_smartcard_context_t *)coreParams->x_context)->x_context;
#else
    smartcard_context_t *smartcardContext = (smartcard_context_t *)coreParams->x_context;
#endif

    if (statusBytes[0u] == 0x90u) /* SW1 */
    {
        coreParams->errCode = kSCCoreSuccess;
        smartcardContext->statusBytes[0] = statusBytes[0];
        smartcardContext->statusBytes[1] = statusBytes[1];

        /* TBD: check secondProcOrStatusByte and set appropriate error states */
    }
    /* SW1 SW2 = '9XYZ', except for '9000' */
    else if ((statusBytes[0u] & 0xF0u) == 0x90u)
    {
        coreParams->errCode = kSCCoreSuccess;
        smartcardContext->statusBytes[0u] = statusBytes[0u];
        smartcardContext->statusBytes[1u] = statusBytes[1u];
    }
    /* Below ones not required for EMV cards, but are applicable for ISO7816-4 cards */
    else if (statusBytes[0u] == 0x6Eu)
    {
        /* CLA not supported */
        coreParams->errCode = kSCCoreCLANotSupported;

        smartcardContext->statusBytes[0u] = statusBytes[0u];
        smartcardContext->statusBytes[1u] = statusBytes[1u];
        /* Disable WWT timer before exiting T=0 transport */
        SMARTCARD_DISABLE_WWT(coreParams->x_context);
        errCode = kSCCoreFail;
    }
    else if (statusBytes[0u] == 0x6Du)
    {
        /* CLA supported, but INS not programmed or invalid  */
        coreParams->errCode = kSCCoreINSInvalid;

        smartcardContext->statusBytes[0u] = statusBytes[0u];
        smartcardContext->statusBytes[1u] = statusBytes[1u];

        /* Disable WWT timer before exiting T=0 transport */
        SMARTCARD_DISABLE_WWT(coreParams->x_context);
        errCode = kSCCoreFail;
    }
    else if (statusBytes[0u] == 0x6Bu)
    {
        /* CLA INS supported, but P1 P2 incorrect  */
        coreParams->errCode = kSCCoreP1P2Invalid;

        smartcardContext->statusBytes[0u] = statusBytes[0u];
        smartcardContext->statusBytes[1u] = statusBytes[1u];

        /* Disable WWT timer before exiting T=0 transport */
        SMARTCARD_DISABLE_WWT(coreParams->x_context);
        errCode = kSCCoreFail;
    }
    /* Process aborted; Ne definitely not accepted */
    else if (statusBytes[0u] == 0x67u)
    {
        /* CLA INS P1 P2 supported, but P3 incorrect   */
        coreParams->errCode = kSCCoreP3Invalid;

        smartcardContext->statusBytes[0u] = statusBytes[0u];
        smartcardContext->statusBytes[1u] = statusBytes[1u];

        /* Disable WWT timer before exiting T=0 transport */
        SMARTCARD_DISABLE_WWT(coreParams->x_context);
        errCode = kSCCoreFail;
    }
    else if (statusBytes[0u] == 0x6Fu)
    {
        /* command not supported and no precise diagnosis given */
        coreParams->errCode = kSCCoreCommandNotSupported;

        smartcardContext->statusBytes[0u] = statusBytes[0u];
        smartcardContext->statusBytes[1u] = statusBytes[1u];

        /* Disable WWT timer before exiting T=0 transport */
        SMARTCARD_DISABLE_WWT(coreParams->x_context);
        errCode = kSCCoreFail;
    }
    else if ((statusBytes[0u] == 0x62u) || (statusBytes[0u] == 0x63u) || (statusBytes[0u] == 0x64) ||
             (statusBytes[0u] == 0x65u) || (statusBytes[0u] == 0x66u) || (statusBytes[0u] == 0x68) ||
             (statusBytes[0u] == 0x69u) || (statusBytes[0u] == 0x6Au))
    {
        coreParams->errCode = kSCCoreOther;

        smartcardContext->statusBytes[0u] = statusBytes[0u];
        smartcardContext->statusBytes[1u] = statusBytes[1u];

        /* Disable WWT timer before exiting T=0 transport */
        SMARTCARD_DISABLE_WWT(coreParams->x_context);
        errCode = kSCCoreFail;
    }
    else
    {
        coreParams->errCode = kSCCoreIncorrectStatusBytes;

        smartcardContext->statusBytes[0u] = statusBytes[0u];
        smartcardContext->statusBytes[1u] = statusBytes[1u];

        /* Disable WWT timer before exiting T=0 transport */
        SMARTCARD_DISABLE_WWT(coreParams->x_context);
        errCode = kSCCoreFail;
    }

    return errCode;
}