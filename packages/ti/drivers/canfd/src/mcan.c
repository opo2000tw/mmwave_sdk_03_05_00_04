/**
 *   @file  mcan.c
 *
 *   @brief
 *      The file implements the MCAN Driver for XWR16xx/XWR18xx/XWR68xx.
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2016 Texas Instruments, Inc.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/
#include <stdint.h>
#include <ti/common/hw_types.h>
#include <ti/common/sys_common.h>
#include <ti/drivers/canfd/include/hw_mcanss.h>
#include <ti/drivers/canfd/include/canfd_internal.h>


/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/**
 * \brief  MCAN MSG RAM BANK number for ECC AGGR.
 */
#define MCANSS_MSG_RAM_NUM                                       (1U)

/**
 * \brief  Maximum Number of Rx Buffers.
 */
#define MCANSS_RX_BUFFER_MAX                                     (64U)

/**
 * \brief  Maximum Number of Tx Buffers.
 */
#define MCANSS_TX_BUFFER_MAX                                     (32U)

/**
 * \brief  Macro for standard Message ID filter.
 */
#define MCANSS_STD_ID_FILTER_SIZE_WORDS                          (1U)

/**
 * \brief  Macro for extended Message ID filter.
 */
#define MCANSS_EXT_ID_FILTER_SIZE_WORDS                          (2U)

/**
 * \brief  Macro for Interrupt Line enable mask.
 */
#define MCANSS_INTR_LINE_EN_MASK   ((MCAN_ILE_EINT0_MASK | MCAN_ILE_EINT1_MASK))

/**
 * \brief  Mask and shift for Tx Buffers elements.
 */
#define MCANSS_TX_BUFFER_ELEM_ID_SHIFT                           (0U)
#define MCANSS_TX_BUFFER_ELEM_ID_MASK                            (0x1FFFFFFFU)
#define MCANSS_TX_BUFFER_ELEM_RTR_SHIFT                          (29U)
#define MCANSS_TX_BUFFER_ELEM_RTR_MASK                           (0x20000000U)
#define MCANSS_TX_BUFFER_ELEM_XTD_SHIFT                          (30U)
#define MCANSS_TX_BUFFER_ELEM_XTD_MASK                           (0x40000000U)
#define MCANSS_TX_BUFFER_ELEM_ESI_SHIFT                          (31U)
#define MCANSS_TX_BUFFER_ELEM_ESI_MASK                           (0x80000000U)
#define MCANSS_TX_BUFFER_ELEM_DLC_SHIFT                          (16U)
#define MCANSS_TX_BUFFER_ELEM_DLC_MASK                           (0x000F0000U)
#define MCANSS_TX_BUFFER_ELEM_BRS_SHIFT                          (20U)
#define MCANSS_TX_BUFFER_ELEM_BRS_MASK                           (0x00100000U)
#define MCANSS_TX_BUFFER_ELEM_FDF_SHIFT                          (21U)
#define MCANSS_TX_BUFFER_ELEM_FDF_MASK                           (0x00200000U)
#define MCANSS_TX_BUFFER_ELEM_EFC_SHIFT                          (23U)
#define MCANSS_TX_BUFFER_ELEM_EFC_MASK                           (0x00800000U)
#define MCANSS_TX_BUFFER_ELEM_MM_SHIFT                           (24U)
#define MCANSS_TX_BUFFER_ELEM_MM_MASK                            (0xFF000000U)

/**
 * \brief  Mask and shift for Rx Buffers elements.
 */
#define MCANSS_RX_BUFFER_ELEM_ID_SHIFT                           (0U)
#define MCANSS_RX_BUFFER_ELEM_ID_MASK                            (0x1FFFFFFFU)
#define MCANSS_RX_BUFFER_ELEM_RTR_SHIFT                          (29U)
#define MCANSS_RX_BUFFER_ELEM_RTR_MASK                           (0x20000000U)
#define MCANSS_RX_BUFFER_ELEM_XTD_SHIFT                          (30U)
#define MCANSS_RX_BUFFER_ELEM_XTD_MASK                           (0x40000000U)
#define MCANSS_RX_BUFFER_ELEM_ESI_SHIFT                          (31U)
#define MCANSS_RX_BUFFER_ELEM_ESI_MASK                           (0x80000000U)
#define MCANSS_RX_BUFFER_ELEM_RXTS_SHIFT                         (0U)
#define MCANSS_RX_BUFFER_ELEM_RXTS_MASK                          (0x0000FFFFU)
#define MCANSS_RX_BUFFER_ELEM_DLC_SHIFT                          (16U)
#define MCANSS_RX_BUFFER_ELEM_DLC_MASK                           (0x000F0000U)
#define MCANSS_RX_BUFFER_ELEM_BRS_SHIFT                          (20U)
#define MCANSS_RX_BUFFER_ELEM_BRS_MASK                           (0x00100000U)
#define MCANSS_RX_BUFFER_ELEM_FDF_SHIFT                          (21U)
#define MCANSS_RX_BUFFER_ELEM_FDF_MASK                           (0x00200000U)
#define MCANSS_RX_BUFFER_ELEM_FIDX_SHIFT                         (24U)
#define MCANSS_RX_BUFFER_ELEM_FIDX_MASK                          (0x7F000000U)
#define MCANSS_RX_BUFFER_ELEM_ANMF_SHIFT                         (31U)
#define MCANSS_RX_BUFFER_ELEM_ANMF_MASK                          (0x80000000U)

/**
 * \brief  Mask and shift for Standard Message ID Filter Elements.
 */
#define MCANSS_STD_ID_FILTER_SFID2_SHIFT                         (0U)
#define MCANSS_STD_ID_FILTER_SFID2_MASK                          (0x000003FFU)
#define MCANSS_STD_ID_FILTER_SFID1_SHIFT                         (16U)
#define MCANSS_STD_ID_FILTER_SFID1_MASK                          (0x03FF0000U)
#define MCANSS_STD_ID_FILTER_SFEC_SHIFT                          (27U)
#define MCANSS_STD_ID_FILTER_SFEC_MASK                           (0x38000000U)
#define MCANSS_STD_ID_FILTER_SFT_SHIFT                           (30U)
#define MCANSS_STD_ID_FILTER_SFT_MASK                            (0xC0000000U)

/**
 * \brief  Extended Message ID Filter Element.
 */
#define MCANSS_EXT_ID_FILTER_EFID2_SHIFT                        (0U)
#define MCANSS_EXT_ID_FILTER_EFID2_MASK                         (0x1FFFFFFFU)
#define MCANSS_EXT_ID_FILTER_EFID1_SHIFT                        (0U)
#define MCANSS_EXT_ID_FILTER_EFID1_MASK                         (0x1FFFFFFFU)
#define MCANSS_EXT_ID_FILTER_EFEC_SHIFT                         (29U)
#define MCANSS_EXT_ID_FILTER_EFEC_MASK                          (0xE0000000U)
#define MCANSS_EXT_ID_FILTER_EFT_SHIFT                          (30U)
#define MCANSS_EXT_ID_FILTER_EFT_MASK                           (0xC0000000U)

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */

/**
 * \brief   This API will unblock write access to write protected registers.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 *
 * \return  None.
 */
static void MCAN_writeProtectedRegAccessUnlock(uint32_t baseAddr);

/**
 * \brief   This API will block write access to write protected registers.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 *
 * \return  None.
 */
static void MCAN_writeProtectedRegAccessLock(uint32_t baseAddr);

/**
 * \brief   This API will load the register from ECC memory bank.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 * \param   regOffset       Offset of the register to read.
 *
 * \return  None.
 */
static void MCAN_eccLoadRegister(uint32_t baseAddr, uint32_t regOffset);

/**
 * \brief   This API will read the message object from Message RAM.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 * \param   elemAddr        Address of the message object.
 * \param   elem            Message Object.
 *                          Refer struct #MCAN_RxBufElement.
 *
 * \return  None.
 */
static void MCAN_readMsg(uint32_t           baseAddr,
                         uint32_t           elemAddr,
                         MCAN_RxBufElement *elem);

/**
 * \brief   This API will write the message object to Message RAM.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 * \param   elemAddr        Address of the message object.
 * \param   elem            Message Object.
 *                          Refer struct #MCAN_TxBufElement.
 *
 * \return  None.
 */
static void MCAN_writeMsg(uint32_t                 baseAddr,
                          uint32_t                 elemAddr,
                          const MCAN_TxBufElement *elem);

/**
 * \brief   This API will return payload depending on 'dlc'  field.
 *
 * \param   dlc             Data Length Code.
 *
 * \return  data size       Size of the payload.
 */
static uint32_t MCAN_getDataSize(uint32_t dlc);

/**
 * \brief   This API will return message object size.
 *
 * \param   elemSize        Size of the message element.
 *
 * \return  message object size
 *                          Size of the message object stored in Message RAM.
 */
static uint32_t MCAN_getMsgObjSize(uint32_t elemSize);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

uint32_t MCAN_isFDOpEnable(uint32_t baseAddr)
{
    uint32_t fdoe;
    uint32_t state;

    fdoe = HW_RD_FIELD32(MCAN_MCANSS_STAT(baseAddr),
                         MCAN_MCANSS_STAT_ENABLE_FDOE);
    if (1U == fdoe)
    {
        state = (uint32_t) 1U;
    }
    else
    {
        state = (uint32_t) 0;
    }
    return state;
}

uint32_t MCAN_isMemInitDone(uint32_t baseAddr)
{
    uint32_t memInit;
    uint32_t state;

    memInit = HW_RD_FIELD32(MCAN_MCANSS_STAT(baseAddr),
                            MCAN_MCANSS_STAT_MEM_INIT_DONE);
    if (1U == memInit)
    {
        state = (uint32_t) 1U;
    }
    else
    {
        state = (uint32_t) 0;
    }
    return state;
}

void MCAN_setOpMode(uint32_t baseAddr, uint32_t mode)
{
    HW_WR_FIELD32(MCAN_CCCR(baseAddr), MCAN_CCCR_INIT, mode);
}

uint32_t MCAN_getOpMode(uint32_t baseAddr)
{
    return (HW_RD_FIELD32(MCAN_CCCR(baseAddr), MCAN_CCCR_INIT));
}

int32_t MCAN_init(uint32_t baseAddr, const CANFD_MCANInitParams *initParams)
{
    int32_t  status;
    uint32_t regVal;

    /* Configure MCAN wakeup and clock stop controls */
    regVal = HW_RD_REG32(MCAN_MCANSS_CTRL(baseAddr));
    HW_SET_FIELD32(regVal,
                   MCAN_MCANSS_CTRL_WAKEUPREQEN,
                   initParams->wkupReqEnable);
    HW_SET_FIELD32(regVal,
                   MCAN_MCANSS_CTRL_AUTOWAKEUP,
                   initParams->autoWkupEnable);
#ifndef SOC_XWR68XX
    HW_SET_FIELD32(regVal,
                   MCAN_MCANSS_CTRL_EMUEN,
                   initParams->emulationEnable);
    HW_SET_FIELD32(regVal,
                   MCAN_MCANSS_CTRL_EMUFACK,
                   initParams->emulationFAck);
    HW_SET_FIELD32(regVal,
                   MCAN_MCANSS_CTRL_CLKFACK,
                   initParams->clkStopFAck);
#endif
    HW_WR_REG32(MCAN_MCANSS_CTRL(baseAddr), regVal);

    MCAN_writeProtectedRegAccessUnlock(baseAddr);

    /* Configure MCAN mode(FD vs Classic CAN operation) and controls */
    regVal = HW_RD_REG32(MCAN_CCCR(baseAddr));
    HW_SET_FIELD32(regVal,
                   MCAN_CCCR_FDOE,
                   initParams->fdMode);
    HW_SET_FIELD32(regVal,
                   MCAN_CCCR_BRSE,
                   initParams->brsEnable);
    HW_SET_FIELD32(regVal,
                   MCAN_CCCR_TXP,
                   initParams->txpEnable);
    HW_SET_FIELD32(regVal,
                   MCAN_CCCR_EFBI,
                   initParams->efbi);
    HW_SET_FIELD32(regVal,
                   MCAN_CCCR_PXHD,
                   initParams->pxhddisable);
    HW_SET_FIELD32(regVal,
                   MCAN_CCCR_DAR,
                   initParams->darEnable);
    HW_WR_REG32(MCAN_CCCR(baseAddr), regVal);

    if ((MCAN_TDCR_TDCF_MAX >= initParams->tdcConfig.tdcf) &&
        (MCAN_TDCR_TDCO_MAX >= initParams->tdcConfig.tdco) &&
        (MCAN_RWD_WDC_MAX >= initParams->wdcPreload))
    {
        /* Configure Transceiver Delay Compensation */
        HW_WR_FIELD32(MCAN_TDCR(baseAddr),
                      MCAN_TDCR_TDCF,
                      initParams->tdcConfig.tdcf);
        HW_WR_FIELD32(MCAN_TDCR(baseAddr),
                      MCAN_TDCR_TDCO,
                      initParams->tdcConfig.tdco);
        /* Configure MSG RAM watchdog counter preload value */
        HW_WR_FIELD32(MCAN_RWD(baseAddr),
                      MCAN_RWD_WDC,
                      initParams->wdcPreload);
        /* Enable/Disable Transceiver Delay Compensation */
        HW_WR_FIELD32(MCAN_DBTP(baseAddr),
                      MCAN_DBTP_TDC,
                      initParams->tdcEnable);
        status = CANFD_EOK;
    }
    else
    {
        status = CANFD_EINVAL;
    }

    MCAN_writeProtectedRegAccessLock(baseAddr);

    return status;
}

int32_t MCAN_config(uint32_t baseAddr, const CANFD_MCANInitParams *configParams)
{
    int32_t status;

    MCAN_writeProtectedRegAccessUnlock(baseAddr);

    /* Configure MCAN control registers */
    HW_WR_FIELD32(MCAN_CCCR(baseAddr),
                  MCAN_CCCR_MON,
                  configParams->monEnable);
    HW_WR_FIELD32(MCAN_CCCR(baseAddr),
                  MCAN_CCCR_ASM,
                  configParams->asmEnable);
    /* Configure Global Filter */
    HW_WR_FIELD32(MCAN_GFC(baseAddr),
                  MCAN_GFC_RRFE,
                  configParams->filterConfig.rrfe);
    HW_WR_FIELD32(MCAN_GFC(baseAddr),
                  MCAN_GFC_RRFS,
                  configParams->filterConfig.rrfs);
    HW_WR_FIELD32(MCAN_GFC(baseAddr),
                  MCAN_GFC_ANFE,
                  configParams->filterConfig.anfe);
    HW_WR_FIELD32(MCAN_GFC(baseAddr),
                  MCAN_GFC_ANFS,
                  configParams->filterConfig.anfs);

    if ((MCAN_TSCC_TCP_MAX >= configParams->tsPrescalar) &&
        (MCAN_TOCC_TOP_MAX >= configParams->timeoutPreload))
    {
        /* Configure Time-stamp counter */
        HW_WR_FIELD32(MCAN_TSCC(baseAddr),
                      MCAN_TSCC_TSS,
                      configParams->tsSelect);
        HW_WR_FIELD32(MCAN_TSCC(baseAddr),
                      MCAN_TSCC_TCP,
                      (configParams->tsPrescalar - 1U));
        /* Configure Time-out counter */
        HW_WR_FIELD32(MCAN_TOCC(baseAddr),
                      MCAN_TOCC_TOS,
                      configParams->timeoutSelect);
        HW_WR_FIELD32(MCAN_TOCC(baseAddr),
                      MCAN_TOCC_TOP,
                      configParams->timeoutPreload);
        /* Enable Time-out counter */
        HW_WR_FIELD32(MCAN_TOCC(baseAddr),
                      MCAN_TOCC_ETOC,
                      configParams->timeoutCntEnable);
        status = CANFD_EOK;
    }
    else
    {
        status = CANFD_EINVAL;
    }

    MCAN_writeProtectedRegAccessLock(baseAddr);

    return status;
}
void MCAN_eccConfig(uint32_t                            baseAddr,
                    const CANFD_MCANECCConfigParams     *configParams)
{
    MCAN_eccLoadRegister(baseAddr, MCAN_ECC_AGGR_CONTROL(baseAddr));
    HW_WR_FIELD32(MCAN_ECC_AGGR_CONTROL(baseAddr),
                  MCAN_ECC_AGGR_CONTROL_ECC_CHECK,
                  configParams->enableChk);
    HW_WR_FIELD32(MCAN_ECC_AGGR_CONTROL(baseAddr),
                  MCAN_ECC_AGGR_CONTROL_ECC_ENABLE,
                  configParams->enable);
    HW_WR_FIELD32(MCAN_ECC_AGGR_CONTROL(baseAddr),
                  MCAN_ECC_AGGR_CONTROL_ENABLE_RMW,
                  configParams->enableRdModWr);
}

int32_t MCAN_setBitTime(uint32_t                    baseAddr,
                        const MCAN_BitTimingParams *configParams)
{
    int32_t status;

    MCAN_writeProtectedRegAccessUnlock(baseAddr);

    if ((MCAN_NBTP_NSJW_MAX >= configParams->nomSynchJumpWidth) &&
        (MCAN_NBTP_NTSEG2_MAX >= configParams->nomTimeSeg2) &&
        (MCAN_NBTP_NTSEG1_MAX >= configParams->nomTimeSeg1) &&
        (MCAN_NBTP_NBRP_MAX >= configParams->nomRatePrescalar))
    {
        HW_WR_FIELD32(MCAN_NBTP(baseAddr),
                      MCAN_NBTP_NSJW,
                      configParams->nomSynchJumpWidth);
        HW_WR_FIELD32(MCAN_NBTP(baseAddr),
                      MCAN_NBTP_NTSEG2,
                      configParams->nomTimeSeg2);
        HW_WR_FIELD32(MCAN_NBTP(baseAddr),
                      MCAN_NBTP_NTSEG1,
                      configParams->nomTimeSeg1);
        HW_WR_FIELD32(MCAN_NBTP(baseAddr),
                      MCAN_NBTP_NBRP,
                      configParams->nomRatePrescalar);
        status = CANFD_EOK;
    }
    else
    {
        status = CANFD_EINVAL;
    }
    if (CANFD_EOK == status)
    {
        if ((MCAN_DBTP_DSJW_MAX >= configParams->dataSynchJumpWidth) &&
            (MCAN_DBTP_DTSEG2_MAX >= configParams->dataTimeSeg2) &&
            (MCAN_DBTP_DTSEG1_MAX >= configParams->dataTimeSeg1) &&
            (MCAN_DBTP_DBRP_MAX >= configParams->dataRatePrescalar))
        {
            HW_WR_FIELD32(MCAN_DBTP(baseAddr),
                          MCAN_DBTP_DSJW,
                          configParams->dataSynchJumpWidth);
            HW_WR_FIELD32(MCAN_DBTP(baseAddr),
                          MCAN_DBTP_DTSEG2,
                          configParams->dataTimeSeg2);
            HW_WR_FIELD32(MCAN_DBTP(baseAddr),
                          MCAN_DBTP_DTSEG1,
                          configParams->dataTimeSeg1);
            HW_WR_FIELD32(MCAN_DBTP(baseAddr),
                          MCAN_DBTP_DBRP,
                          configParams->dataRatePrescalar);
            status = CANFD_EOK;
        }
        else
        {
            status = CANFD_EINVAL;
        }
    }

    MCAN_writeProtectedRegAccessLock(baseAddr);
    return status;
}

int32_t MCAN_msgRAMConfig(uint32_t                      baseAddr,
                          const MCAN_MsgRAMConfigParams *msgRAMConfigParams)
{
    int32_t  status;
    uint32_t elemNum = 0U;

    MCAN_writeProtectedRegAccessUnlock(baseAddr);

    /* Configure Message Filters section */
    if (0U != msgRAMConfigParams->lss)
    {
        HW_WR_FIELD32(MCAN_SIDFC(baseAddr),
                      MCAN_SIDFC_FLSSA,
                      (msgRAMConfigParams->flssa >> 2U));
        HW_WR_FIELD32(MCAN_SIDFC(baseAddr),
                      MCAN_SIDFC_LSS,
                      msgRAMConfigParams->lss);
    }
    if (0U != msgRAMConfigParams->lse)
    {
        HW_WR_FIELD32(MCAN_XIDFC(baseAddr),
                      MCAN_XIDFC_FLESA,
                      (msgRAMConfigParams->flesa >> 2U));
        HW_WR_FIELD32(MCAN_XIDFC(baseAddr),
                      MCAN_XIDFC_LSE,
                      msgRAMConfigParams->lse);
    }
    /* Configure Rx FIFO 0 section */
    if (0U != msgRAMConfigParams->rxFIFO0size)
    {
        HW_WR_FIELD32(MCAN_RXF0C(baseAddr),
                      MCAN_RXF0C_F0SA,
                      (msgRAMConfigParams->rxFIFO0startAddr >> 2U));
        HW_WR_FIELD32(MCAN_RXF0C(baseAddr),
                      MCAN_RXF0C_F0S,
                      msgRAMConfigParams->rxFIFO0size);
        HW_WR_FIELD32(MCAN_RXF0C(baseAddr),
                      MCAN_RXF0C_F0WM,
                      msgRAMConfigParams->rxFIFO0waterMark);
        HW_WR_FIELD32(MCAN_RXF0C(baseAddr),
                      MCAN_RXF0C_F0OM,
                      msgRAMConfigParams->rxFIFO0OpMode);
        /* Configure Rx FIFO0 elements size */
        HW_WR_FIELD32(MCAN_RXESC(baseAddr),
                      MCAN_RXESC_F0DS,
                      msgRAMConfigParams->rxFIFO0ElemSize);
    }
    /* Configure Rx FIFO 1 section */
    if (0U != msgRAMConfigParams->rxFIFO1size)
    {
        HW_WR_FIELD32(MCAN_RXF1C(baseAddr),
                      MCAN_RXF1C_F1SA,
                      (msgRAMConfigParams->rxFIFO1startAddr >> 2U));
        HW_WR_FIELD32(MCAN_RXF1C(baseAddr),
                      MCAN_RXF1C_F1S,
                      msgRAMConfigParams->rxFIFO1size);
        HW_WR_FIELD32(MCAN_RXF1C(baseAddr),
                      MCAN_RXF1C_F1WM,
                      msgRAMConfigParams->rxFIFO1waterMark);
        HW_WR_FIELD32(MCAN_RXF1C(baseAddr),
                      MCAN_RXF1C_F1OM,
                      msgRAMConfigParams->rxFIFO1OpMode);
        /* Configure Rx FIFO1 elements size */
        HW_WR_FIELD32(MCAN_RXESC(baseAddr),
                      MCAN_RXESC_F1DS,
                      msgRAMConfigParams->rxFIFO1ElemSize);
    }
    /* Configure Rx Buffer Start Address */
    HW_WR_FIELD32(MCAN_RXBC(baseAddr),
                  MCAN_RXBC_RBSA,
                  (msgRAMConfigParams->rxBufStartAddr >> 2U));
    /* Configure Rx Buffer elements size */
    HW_WR_FIELD32(MCAN_RXESC(baseAddr),
                  MCAN_RXESC_RBDS,
                  msgRAMConfigParams->rxBufElemSize);
    /* Configure Tx Event FIFO section */
    if (0U != msgRAMConfigParams->txEventFIFOSize)
    {
        HW_WR_FIELD32(MCAN_TXEFC(baseAddr),
                      MCAN_TXEFC_EFSA,
                      (msgRAMConfigParams->txEventFIFOStartAddr >> 2U));
        HW_WR_FIELD32(MCAN_TXEFC(baseAddr),
                      MCAN_TXEFC_EFS,
                      msgRAMConfigParams->txEventFIFOSize);
        HW_WR_FIELD32(MCAN_TXEFC(baseAddr),
                      MCAN_TXEFC_EFWM,
                      msgRAMConfigParams->txEventFIFOWaterMark);
    }
    /* Configure Tx Buffer and FIFO/Q section */
    elemNum = msgRAMConfigParams->txBufNum + msgRAMConfigParams->txFIFOSize;
    if ((MCANSS_TX_BUFFER_MAX >= elemNum) &&
        ((0U != msgRAMConfigParams->txBufNum) ||
         (0U != msgRAMConfigParams->txFIFOSize)))
    {
        HW_WR_FIELD32(MCAN_TXBC(baseAddr),
                      MCAN_TXBC_TBSA,
                      (msgRAMConfigParams->txStartAddr >> 2U));
        HW_WR_FIELD32(MCAN_TXBC(baseAddr),
                      MCAN_TXBC_NDTB,
                      msgRAMConfigParams->txBufNum);
        HW_WR_FIELD32(MCAN_TXBC(baseAddr),
                      MCAN_TXBC_TFQS,
                      msgRAMConfigParams->txFIFOSize);
        HW_WR_FIELD32(MCAN_TXBC(baseAddr),
                      MCAN_TXBC_TFQM,
                      msgRAMConfigParams->txBufMode);
        /* Configure Tx Buffer/FIFO0/FIFO1 elements size */
        HW_WR_FIELD32(MCAN_TXESC(baseAddr),
                      MCAN_TXESC_TBDS,
                      msgRAMConfigParams->txBufElemSize);
        status = CANFD_EOK;
    }
    else
    {
        status = CANFD_EINVAL;
    }

    MCAN_writeProtectedRegAccessLock(baseAddr);

    return status;
}

int32_t MCAN_setExtIDAndMask(uint32_t baseAddr, uint32_t idMask)
{
    int32_t status;

    if (MCAN_XIDAM_EIDM_MAX >= idMask)
    {
        MCAN_writeProtectedRegAccessUnlock(baseAddr);

        HW_WR_FIELD32(MCAN_XIDAM(baseAddr),
                      MCAN_XIDAM_EIDM,
                      idMask);

        MCAN_writeProtectedRegAccessLock(baseAddr);
        status = CANFD_EOK;
    }
    else
    {
        status = CANFD_EINVAL;
    }
    return status;
}

void MCAN_writeMsgRam(uint32_t                 baseAddr,
                      uint32_t                 memType,
                      uint32_t                 bufNum,
                      const MCAN_TxBufElement *elem)
{
    uint32_t startAddr = 0U, elemSize = 0U, elemAddr = 0U;
    uint32_t idx       = 0U, enableMod = 0U;

    if (MCAN_MemType_BUF == memType)
    {
        idx       = bufNum;
        enableMod = 1U;
    }
    if (MCAN_MemType_FIFO == memType)
    {
        idx       = HW_RD_FIELD32(MCAN_TXFQS(baseAddr), MCAN_TXFQS_TFQPI);
        enableMod = 1U;
    }
    if (1U == enableMod)
    {
        startAddr = HW_RD_FIELD32(MCAN_TXBC(baseAddr),
                                  MCAN_TXBC_TBSA);
        elemSize = HW_RD_FIELD32(MCAN_TXESC(baseAddr),
                                 MCAN_TXESC_TBDS);
        startAddr = (uint32_t) (startAddr << 2U);
        elemSize  = MCAN_getMsgObjSize(elemSize);
        elemSize *= 4U;
        elemAddr  = startAddr + (elemSize * idx);
        elemAddr += MCAN_MCAN_MSG_MEM;
        MCAN_writeMsg(baseAddr, elemAddr, elem);
    }
}

int32_t MCAN_txBufAddReq(uint32_t baseAddr, uint32_t bufNum)
{
    int32_t  status;
    uint32_t regVal;

    if (MCANSS_TX_BUFFER_MAX > bufNum)
    {
        regVal  = HW_RD_REG32(MCAN_TXBAR(baseAddr));
        regVal |= ((uint32_t) 1U << bufNum);
        HW_WR_REG32(MCAN_TXBAR(baseAddr), regVal);
        status = CANFD_EOK;
    }
    else
    {
        status = CANFD_EINVAL;
    }
    return status;
}

void  MCAN_getNewDataStatus(uint32_t              baseAddr,
                            MCAN_RxNewDataStatus *newDataStatus)
{
    newDataStatus->statusLow  = HW_RD_REG32(MCAN_NDAT1(baseAddr));
    newDataStatus->statusHigh = HW_RD_REG32(MCAN_NDAT2(baseAddr));
}

void  MCAN_clearNewDataStatus(uint32_t                    baseAddr,
                              const MCAN_RxNewDataStatus *newDataStatus)
{
    HW_WR_REG32(MCAN_NDAT1(baseAddr), newDataStatus->statusLow);
    HW_WR_REG32(MCAN_NDAT2(baseAddr), newDataStatus->statusHigh);
}

void MCAN_readMsgRam(uint32_t           baseAddr,
                     uint32_t           memType,
                     uint32_t           bufNum,
                     uint32_t           fifoNum,
                     MCAN_RxBufElement *elem)
{
    uint32_t startAddr = 0U, elemSize = 0U, elemAddr = 0U;
    uint32_t enableMod = 0U, idx = 0U;

    if (MCAN_MemType_BUF == memType)
    {
        startAddr = HW_RD_FIELD32(MCAN_RXBC(baseAddr),
                                  MCAN_RXBC_RBSA);
        elemSize = HW_RD_FIELD32(MCAN_RXESC(baseAddr),
                                 MCAN_RXESC_RBDS);
        idx       = bufNum;
        enableMod = 1U;
    }
    if (MCAN_MemType_FIFO == memType)
    {
        switch (fifoNum)
        {
            case MCAN_RxFIFONum_0:
                startAddr = HW_RD_FIELD32(MCAN_RXF0C(baseAddr),
                                          MCAN_RXF0C_F0SA);
                elemSize = HW_RD_FIELD32(MCAN_RXESC(baseAddr),
                                         MCAN_RXESC_F0DS);
                idx = HW_RD_FIELD32(MCAN_RXF0S(baseAddr),
                                    MCAN_RXF0S_F0GI);
                enableMod = 1U;
                break;
            case MCAN_RxFIFONum_1:
                startAddr = HW_RD_FIELD32(MCAN_RXF1C(baseAddr),
                                          MCAN_RXF1C_F1SA);
                elemSize = HW_RD_FIELD32(MCAN_RXESC(baseAddr),
                                         MCAN_RXESC_F1DS);
                idx = HW_RD_FIELD32(MCAN_RXF1S(baseAddr),
                                    MCAN_RXF1S_F1GI);
                enableMod = 1U;
                break;
            default:
                /* Invalid option */
                break;
        }
    }
    if (1U == enableMod)
    {
        startAddr = (uint32_t) (startAddr << 2U);
        elemSize  = MCAN_getMsgObjSize(elemSize);
        elemSize *= 4U;
        elemAddr  = startAddr + (elemSize * idx);
        elemAddr += MCAN_MCAN_MSG_MEM;
        MCAN_readMsg(baseAddr, elemAddr, elem);
    }
}

void MCAN_addStdMsgIDFilter(uint32_t                          baseAddr,
                            uint32_t                          filtNum,
                            const MCAN_StdMsgIDFilterElement *elem)
{
    uint32_t startAddr, elemAddr, regVal;

    startAddr = HW_RD_FIELD32(MCAN_SIDFC(baseAddr),
                              MCAN_SIDFC_FLSSA);
    startAddr = (uint32_t) (startAddr << 2U);
    elemAddr  = startAddr + (filtNum * MCANSS_STD_ID_FILTER_SIZE_WORDS * 4U);
    elemAddr += MCAN_MCAN_MSG_MEM;

    regVal  = 0U;
    regVal |= (uint32_t) (elem->sfid2 << MCANSS_STD_ID_FILTER_SFID2_SHIFT);
    regVal |= (uint32_t) (elem->sfid1 << MCANSS_STD_ID_FILTER_SFID1_SHIFT);
    regVal |= (uint32_t) (elem->sfec << MCANSS_STD_ID_FILTER_SFEC_SHIFT);
    regVal |= (uint32_t) (elem->sft << MCANSS_STD_ID_FILTER_SFT_SHIFT);
    HW_WR_REG32(baseAddr + elemAddr, regVal);
}

void MCAN_addExtMsgIDFilter(uint32_t                          baseAddr,
                            uint32_t                          filtNum,
                            const MCAN_ExtMsgIDFilterElement *elem)
{
    uint32_t startAddr, elemAddr, regVal;

    startAddr = HW_RD_FIELD32(MCAN_XIDFC(baseAddr),
                              MCAN_XIDFC_FLESA);
    startAddr = (uint32_t) (startAddr << 2U);
    elemAddr  = startAddr + (filtNum * MCANSS_EXT_ID_FILTER_SIZE_WORDS * 4U);
    elemAddr += MCAN_MCAN_MSG_MEM;

    regVal  = 0U;
    regVal |= (uint32_t) (elem->efid1 << MCANSS_EXT_ID_FILTER_EFID1_SHIFT);
    regVal |= (uint32_t) (elem->efec << MCANSS_EXT_ID_FILTER_EFEC_SHIFT);
    HW_WR_REG32(baseAddr + elemAddr, regVal);

    elemAddr += 4U;
    regVal    = 0U;
    regVal   |= (uint32_t) (elem->efid2 << MCANSS_EXT_ID_FILTER_EFID2_SHIFT);
    regVal   |= (uint32_t) (elem->eft << MCANSS_EXT_ID_FILTER_EFT_SHIFT);
    HW_WR_REG32(baseAddr + elemAddr, regVal);
}

void MCAN_lpbkModeEnable(uint32_t baseAddr,
                         uint32_t lpbkMode,
                         uint32_t enable)
{
    MCAN_writeProtectedRegAccessUnlock(baseAddr);

    if (1U == enable)
    {
        HW_WR_FIELD32(MCAN_CCCR(baseAddr), MCAN_CCCR_TEST, 0x1U);
        HW_WR_FIELD32(MCAN_TEST(baseAddr),
                      MCAN_TEST_LBCK,
                      enable);
        if (CANFD_MCANLoopBackMode_INTERNAL == lpbkMode)
        {
            HW_WR_FIELD32(MCAN_CCCR(baseAddr),
                          MCAN_CCCR_MON,
                          0x1U);
        }
    }
    else
    {
        HW_WR_FIELD32(MCAN_TEST(baseAddr),
                      MCAN_TEST_LBCK,
                      enable);
        HW_WR_FIELD32(MCAN_CCCR(baseAddr), MCAN_CCCR_TEST, 0x0U);
        if (CANFD_MCANLoopBackMode_INTERNAL == lpbkMode)
        {
            HW_WR_FIELD32(MCAN_CCCR(baseAddr),
                          MCAN_CCCR_MON,
                          0x0U);
        }
    }
    MCAN_writeProtectedRegAccessLock(baseAddr);
}

void  MCAN_getErrCounters(uint32_t           baseAddr,
                          CANFD_MCANErrCntStatus *errCounter)
{
    errCounter->transErrLogCnt = HW_RD_FIELD32(MCAN_ECR(baseAddr),
                                               MCAN_ECR_TEC);
    errCounter->recErrCnt = HW_RD_FIELD32(MCAN_ECR(baseAddr),
                                          MCAN_ECR_REC);
    errCounter->rpStatus = HW_RD_FIELD32(MCAN_ECR(baseAddr),
                                         MCAN_ECR_RP);
    errCounter->canErrLogCnt = HW_RD_FIELD32(MCAN_ECR(baseAddr),
                                             MCAN_ECR_CEL);
}

void  MCAN_getProtocolStatus(uint32_t                   baseAddr,
                             CANFD_MCANProtocolStatus   *protStatus)
{
    uint32_t regVal;

    regVal = HW_RD_REG32(MCAN_PSR(baseAddr));
    protStatus->lastErrCode   = HW_GET_FIELD(regVal, MCAN_PSR_LEC);
    protStatus->act           = HW_GET_FIELD(regVal, MCAN_PSR_ACT);
    protStatus->errPassive    = HW_GET_FIELD(regVal, MCAN_PSR_EP);
    protStatus->warningStatus = HW_GET_FIELD(regVal, MCAN_PSR_EW);
    protStatus->busOffStatus  = HW_GET_FIELD(regVal, MCAN_PSR_BO);
    protStatus->dlec          = HW_GET_FIELD(regVal, MCAN_PSR_DLEC);
    protStatus->resi          = HW_GET_FIELD(regVal, MCAN_PSR_RESI);
    protStatus->rbrs          = HW_GET_FIELD(regVal, MCAN_PSR_RBRS);
    protStatus->rfdf          = HW_GET_FIELD(regVal, MCAN_PSR_RFDF);
    protStatus->pxe           = HW_GET_FIELD(regVal, MCAN_PSR_PXE);
    protStatus->tdcv          = HW_GET_FIELD(regVal, MCAN_PSR_TDCV);
}

void MCAN_enableIntr(uint32_t baseAddr, uint32_t intrMask, uint32_t enable)
{
    uint32_t regVal;

    if (1U == enable)
    {
        regVal  = HW_RD_REG32(MCAN_IE(baseAddr));
        regVal |= intrMask;
        HW_WR_REG32(MCAN_IE(baseAddr), regVal);
    }
    else
    {
        regVal  = HW_RD_REG32(MCAN_IE(baseAddr));
        regVal &= ~intrMask;
        HW_WR_REG32(MCAN_IE(baseAddr), regVal);
    }
}

void MCAN_selectIntrLine(uint32_t baseAddr,
                         uint32_t intrMask,
                         uint32_t lineNum)
{
    uint32_t regVal;

    if (MCAN_IntrLineNum_0 == lineNum)
    {
        regVal  = HW_RD_REG32(MCAN_ILS(baseAddr));
        regVal &= ~intrMask;
        HW_WR_REG32(MCAN_ILS(baseAddr), regVal);
    }
    else
    {
        regVal  = HW_RD_REG32(MCAN_ILS(baseAddr));
        regVal |= intrMask;
        HW_WR_REG32(MCAN_ILS(baseAddr), regVal);
    }
}

void MCAN_enableIntrLine(uint32_t baseAddr,
                         uint32_t lineNum,
                         uint32_t enable)
{
    uint32_t regVal;

    lineNum &= MCANSS_INTR_LINE_EN_MASK;
    regVal   = HW_RD_REG32(MCAN_ILE(baseAddr));
    regVal  &= ~((uint32_t) 0x1U << lineNum);
    regVal  |= (uint32_t) (enable << lineNum);
    HW_WR_REG32(MCAN_ILE(baseAddr), regVal);
}

uint32_t MCAN_getIntrStatus(uint32_t baseAddr)
{
    return (HW_RD_REG32(MCAN_IR(baseAddr)));
}

void MCAN_clearIntrStatus(uint32_t baseAddr, uint32_t intrMask)
{
    HW_WR_REG32(MCAN_IR(baseAddr), intrMask);
}

void  MCAN_getHighPriorityMsgStatus(uint32_t                  baseAddr,
                                    MCAN_HighPriorityMsgInfo *hpm)
{
    hpm->bufIdx = HW_RD_FIELD32(MCAN_HPMS(baseAddr),
                                MCAN_HPMS_BIDX);
    hpm->msi = HW_RD_FIELD32(MCAN_HPMS(baseAddr),
                             MCAN_HPMS_MSI);
    hpm->filterIdx = HW_RD_FIELD32(MCAN_HPMS(baseAddr),
                                   MCAN_HPMS_FIDX);
    hpm->filterList = HW_RD_FIELD32(MCAN_HPMS(baseAddr),
                                    MCAN_HPMS_FLST);
}

void MCAN_getRxFIFOStatus(uint32_t           baseAddr,
                          MCAN_RxFIFOStatus *fifoStatus)
{
    uint32_t regVal;

    switch (fifoStatus->num)
    {
        case MCAN_RxFIFONum_0:
            regVal = HW_RD_REG32(MCAN_RXF0S(baseAddr));
            fifoStatus->fillLvl  = HW_GET_FIELD(regVal, MCAN_RXF0S_F0FL);
            fifoStatus->getIdx   = HW_GET_FIELD(regVal, MCAN_RXF0S_F0GI);
            fifoStatus->putIdx   = HW_GET_FIELD(regVal, MCAN_RXF0S_F0PI);
            fifoStatus->fifoFull = HW_GET_FIELD(regVal, MCAN_RXF0S_F0F);
            fifoStatus->msgLost  = HW_GET_FIELD(regVal, MCAN_RXF0S_RF0L);
            break;
        case MCAN_RxFIFONum_1:
            regVal = HW_RD_REG32(MCAN_RXF1S(baseAddr));
            fifoStatus->fillLvl  = HW_GET_FIELD(regVal, MCAN_RXF1S_F1FL);
            fifoStatus->getIdx   = HW_GET_FIELD(regVal, MCAN_RXF1S_F1GI);
            fifoStatus->putIdx   = HW_GET_FIELD(regVal, MCAN_RXF1S_F1PI);
            fifoStatus->fifoFull = HW_GET_FIELD(regVal, MCAN_RXF1S_F1F);
            fifoStatus->msgLost  = HW_GET_FIELD(regVal, MCAN_RXF1S_RF1L);
            break;
        default:
            /* Invalid option */
            break;
    }
}

int32_t MCAN_writeRxFIFOAck(uint32_t baseAddr,
                            uint32_t fifoNum,
                            uint32_t idx)
{
    int32_t  status;
    uint32_t size;

    switch (fifoNum)
    {
        case MCAN_RxFIFONum_0:
            size = HW_RD_FIELD32(MCAN_RXF0C(baseAddr),
                                 MCAN_RXF0C_F0S);
            if (size >= idx)
            {
                HW_WR_FIELD32(MCAN_RXF0A(baseAddr),
                              MCAN_RXF0A_F0AI,
                              idx);
                status = CANFD_EOK;
            }
            else
            {
                status = CANFD_EINVAL;
            }
            break;
        case MCAN_RxFIFONum_1:
            size = HW_RD_FIELD32(MCAN_RXF1C(baseAddr),
                                 MCAN_RXF1C_F1S);
            if (size >= idx)
            {
                HW_WR_FIELD32(MCAN_RXF1A(baseAddr),
                              MCAN_RXF1A_F1AI,
                              idx);
                status = CANFD_EOK;
            }
            else
            {
                status = CANFD_EINVAL;
            }
            break;
        default:
            status = CANFD_EINVAL;
            break;
    }

    return status;
}

void MCAN_getTxFIFOQueStatus(uint32_t           baseAddr,
                             MCAN_TxFIFOStatus *fifoStatus)
{
    uint32_t regVal;

    regVal = HW_RD_REG32(MCAN_TXFQS(baseAddr));
    fifoStatus->freeLvl  = HW_GET_FIELD(regVal, MCAN_TXFQS_TFFL);
    fifoStatus->getIdx   = HW_GET_FIELD(regVal, MCAN_TXFQS_TFGI);
    fifoStatus->putIdx   = HW_GET_FIELD(regVal, MCAN_TXFQS_TFQPI);
    fifoStatus->fifoFull = HW_GET_FIELD(regVal, MCAN_TXFQS_TFQF);
}

uint32_t MCAN_getTxBufReqPend(uint32_t baseAddr)
{
    return (HW_RD_REG32(MCAN_TXBRP(baseAddr)));
}

int32_t MCAN_txBufCancellationReq(uint32_t baseAddr, uint32_t buffNum)
{
    int32_t  status;
    uint32_t regVal;

    if (MCANSS_TX_BUFFER_MAX > buffNum)
    {
        regVal  = HW_RD_REG32(MCAN_TXBCR(baseAddr));
        regVal |= ((uint32_t) 1U << buffNum);
        HW_WR_REG32(MCAN_TXBCR(baseAddr), regVal);
        status = CANFD_EOK;
    }
    else
    {
        status = CANFD_EINVAL;
    }
    return status;
}

uint32_t MCAN_getTxBufTransmissionStatus(uint32_t baseAddr)
{
    return (HW_RD_REG32(MCAN_TXBTO(baseAddr)));
}

uint32_t MCAN_txBufCancellationStatus(uint32_t baseAddr)
{
    return (HW_RD_REG32(MCAN_TXBCF(baseAddr)));
}

int32_t MCAN_txBufTransIntrEnable(uint32_t baseAddr,
                                  uint32_t bufNum,
                                  uint32_t enable)
{
    int32_t  status;
    uint32_t regVal;

    if (MCANSS_TX_BUFFER_MAX > bufNum)
    {
        if (1U == enable)
        {
            regVal  = HW_RD_REG32(MCAN_TXBTIE(baseAddr));
            regVal |= ((uint32_t) 1U << bufNum);
            HW_WR_REG32(MCAN_TXBTIE(baseAddr), regVal);
        }
        else
        {
            regVal  = HW_RD_REG32(MCAN_TXBTIE(baseAddr));
            regVal &= ~((uint32_t) 0x1U << bufNum);
            HW_WR_REG32(MCAN_TXBTIE(baseAddr), regVal);
        }
        status = CANFD_EOK;
    }
    else
    {
        status = CANFD_EINVAL;
    }
    return status;
}

int32_t MCAN_getTxBufCancellationIntrEnable(uint32_t baseAddr,
                                            uint32_t bufNum,
                                            uint32_t enable)
{
    int32_t  status;
    uint32_t regVal;

    if (MCANSS_TX_BUFFER_MAX > bufNum)
    {
        if (1U == enable)
        {
            regVal  = HW_RD_REG32(MCAN_TXBCIE(baseAddr));
            regVal |= ((uint32_t) 0x1U << bufNum);
            HW_WR_REG32(MCAN_TXBCIE(baseAddr), regVal);
        }
        else
        {
            regVal  = HW_RD_REG32(MCAN_TXBCIE(baseAddr));
            regVal &= ~((uint32_t) 0x1U << bufNum);
            HW_WR_REG32(MCAN_TXBCIE(baseAddr), regVal);
        }
        status = CANFD_EOK;
    }
    else
    {
        status = CANFD_EINVAL;
    }
    return status;
}

void MCAN_getTxEventFIFOStatus(uint32_t                baseAddr,
                               MCAN_TxEventFIFOStatus *fifoStatus)
{
    uint32_t regVal;

    regVal = HW_RD_REG32(MCAN_TXEFS(baseAddr));
    fifoStatus->fillLvl  = HW_GET_FIELD(regVal, MCAN_TXEFS_EFFL);
    fifoStatus->getIdx   = HW_GET_FIELD(regVal, MCAN_TXEFS_EFGI);
    fifoStatus->putIdx   = HW_GET_FIELD(regVal, MCAN_TXEFS_EFPI);
    fifoStatus->fifoFull = HW_GET_FIELD(regVal, MCAN_TXEFS_EFF);
    fifoStatus->eleLost  = HW_GET_FIELD(regVal, MCAN_TXEFS_TEFL);
}

void MCAN_addClockStopRequest(uint32_t baseAddr, uint32_t enable)
{
    if(1U == enable)
    {
        HW_WR_FIELD32(MCAN_CCCR(baseAddr), MCAN_CCCR_CSR, 0x1U);
    }
    else
    {
        HW_WR_FIELD32(MCAN_CCCR(baseAddr), MCAN_CCCR_CSR, 0x0U);
    }
}

int32_t MCAN_writeTxEventFIFOAck(uint32_t baseAddr, uint32_t idx)
{
    int32_t  status;
    uint32_t size;

    size = HW_RD_FIELD32(MCAN_TXEFC(baseAddr),
                         MCAN_TXEFC_EFS);
    if (size >= idx)
    {
        HW_WR_FIELD32(MCAN_TXEFA(baseAddr),
                      MCAN_TXEFA_EFAI,
                      idx);
        status = CANFD_EOK;
    }
    else
    {
        status = CANFD_EINVAL;
    }

    return status;
}

void MCAN_eccGetErrorStatus(uint32_t                baseAddr,
                            CANFD_MCANECCErrStatus  *eccErr)
{
    eccErr->secErr = HW_RD_FIELD32(MCAN_ECC_AGGR_SEC_STATUS_REG0(baseAddr),
                                   MCAN_ECC_AGGR_SEC_STATUS_REG0_MSGMEM_PEND);
    eccErr->dedErr = HW_RD_FIELD32(MCAN_ECC_AGGR_DED_STATUS_REG0(baseAddr),
                                   MCAN_ECC_AGGR_DED_STATUS_REG0_MSGMEM_PEND);
}

void MCAN_eccClearErrorStatus(uint32_t baseAddr, uint32_t errType)
{
    MCAN_eccLoadRegister(baseAddr, MCAN_ECC_AGGR_ERROR_STATUS1(baseAddr));
    switch (errType)
    {
        case CANFD_MCANECCErrType_SEC:
            HW_WR_FIELD32(MCAN_ECC_AGGR_ERROR_STATUS1(baseAddr),
                          MCAN_ECC_AGGR_ERROR_STATUS1_CLR_ECC_SEC,
                          0x1U);
            break;
        case CANFD_MCANECCErrType_DED:
            HW_WR_FIELD32(MCAN_ECC_AGGR_ERROR_STATUS1(baseAddr),
                          MCAN_ECC_AGGR_ERROR_STATUS1_CLR_ECC_DED,
                          0x1U);
            break;
        default:
            /* Invalid option */
            break;
    }
}

void MCAN_eccWriteEOI(uint32_t baseAddr, uint32_t errType)
{
    switch (errType)
    {
        case CANFD_MCANECCErrType_SEC:
            HW_WR_FIELD32(MCAN_ECC_AGGR_SEC_EOI_REG(baseAddr),
                          MCAN_ECC_AGGR_SEC_EOI_REG_WR,
                          0x1U);
            break;
        case CANFD_MCANECCErrType_DED:
            HW_WR_FIELD32(MCAN_ECC_AGGR_DED_EOI_REG(baseAddr),
                          MCAN_ECC_AGGR_DED_EOI_REG_WR,
                          0x1U);
            break;
        default:
            /* Invalid option */
            break;
    }
#ifndef SOC_XWR68XX
    HW_WR_FIELD32(MCAN_ECC_EOI(baseAddr),
                  MCAN_ECC_EOI,
                  0x1U);
#endif
}

void MCAN_eccEnableIntr(uint32_t baseAddr, uint32_t errType, uint32_t enable)
{
    if (1U == enable)
    {
        switch (errType)
        {
            case CANFD_MCANECCErrType_SEC:
                HW_WR_FIELD32(MCAN_ECC_AGGR_SEC_ENABLE_SET_REG0(baseAddr),
                              MCAN_ECC_AGGR_SEC_ENABLE_SET_REG0_MSGMEM,
                              0x1U);
                break;
            case CANFD_MCANECCErrType_DED:
                HW_WR_FIELD32(MCAN_ECC_AGGR_DED_ENABLE_SET_REG0(baseAddr),
                              MCAN_ECC_AGGR_DED_ENABLE_SET_REG0_MSGMEM,
                              0x1U);
                break;
            default:
                /* Invalid option */
                break;
        }
    }
    else
    {
        switch (errType)
        {
            case CANFD_MCANECCErrType_SEC:
                HW_WR_FIELD32(MCAN_ECC_AGGR_SEC_ENABLE_CLR_REG0(baseAddr),
                              MCAN_ECC_AGGR_SEC_ENABLE_CLR_REG0_MSGMEM,
                              0x1U);
                break;
            case CANFD_MCANECCErrType_DED:
                HW_WR_FIELD32(MCAN_ECC_AGGR_DED_ENABLE_CLR_REG0(baseAddr),
                              MCAN_ECC_AGGR_DED_ENABLE_CLR_REG0_MSGMEM,
                              0x1U);
                break;
            default:
                /* Invalid option */
                break;
        }
    }
}

void MCAN_extTSCounterConfig(uint32_t baseAddr,
                             uint32_t prescalar)
{
    HW_WR_FIELD32(MCAN_MCANSS_EXT_TS_PRESCALER(baseAddr),
                  MCAN_MCANSS_EXT_TS_PRESCALER, (prescalar - 1U));
}

void MCAN_extTSCounterEnable(uint32_t baseAddr, uint32_t enable)
{
    HW_WR_FIELD32(MCAN_MCANSS_CTRL(baseAddr),
                  MCAN_MCANSS_CTRL_EXT_TS_CNTR_EN,
                  enable);
}

void MCAN_extTSEnableIntr(uint32_t baseAddr, uint32_t enable)
{
    if (1U == enable)
    {
        HW_WR_FIELD32(MCAN_MCANSS_IE(baseAddr),
                      MCAN_MCANSS_IE_EXT_TS_CNTR_OVFL,
                      1U);
    }
    else
    {
        HW_WR_FIELD32(MCAN_MCANSS_IECS(baseAddr),
                      MCAN_MCANSS_IECS_EXT_TS_CNTR_OVFL,
                      1U);
    }
}

void MCAN_extTSWriteEOI(uint32_t baseAddr)
{
    HW_WR_FIELD32(MCAN_MCANSS_EOI(baseAddr),
                  MCAN_MCANSS_EOI,
                  0x1U);
}

uint32_t MCAN_extTSGetUnservicedIntrCount(uint32_t baseAddr)
{
    return (HW_RD_FIELD32(MCAN_MCANSS_EXT_TS_UNSERVICED_INTR_CNTR(baseAddr),
                          MCAN_MCANSS_EXT_TS_UNSERVICED_INTR_CNTR));
}

/* ========================================================================== */
/*                          Advance Functions                                 */
/* ========================================================================== */

void MCAN_getRevisionId(uint32_t baseAddr, MCAN_RevisionId *revId)
{
    uint32_t regVal;

    regVal        = HW_RD_REG32(MCAN_MCANSS_PID(baseAddr));
    revId->minor  = HW_GET_FIELD(regVal, MCAN_MCANSS_PID_MINOR);
    revId->custom = HW_GET_FIELD(regVal, MCAN_MCANSS_PID_CUSTOM);
    revId->major  = HW_GET_FIELD(regVal, MCAN_MCANSS_PID_MAJOR);
    revId->rtlRev = HW_GET_FIELD(regVal, MCAN_MCANSS_PID_RTL);
    revId->modId  = HW_GET_FIELD(regVal, MCAN_MCANSS_PID_MODULE_ID);
    revId->bu     = HW_GET_FIELD(regVal, MCAN_MCANSS_PID_BU);
    revId->scheme = HW_GET_FIELD(regVal, MCAN_MCANSS_PID_SCHEME);

    regVal         = HW_RD_REG32(MCAN_CREL(baseAddr));
    revId->day     = HW_GET_FIELD(regVal, MCAN_CREL_DAY);
    revId->mon     = HW_GET_FIELD(regVal, MCAN_CREL_MON);
    revId->year    = HW_GET_FIELD(regVal, MCAN_CREL_YEAR);
    revId->subStep = HW_GET_FIELD(regVal, MCAN_CREL_SUBSTEP);
    revId->step    = HW_GET_FIELD(regVal, MCAN_CREL_STEP);
    revId->rel     = HW_GET_FIELD(regVal, MCAN_CREL_REL);
}

uint32_t MCAN_getClockStopAck(uint32_t baseAddr)
{
    return (HW_RD_FIELD32(MCAN_CCCR(baseAddr), MCAN_CCCR_CSR));
}

void MCAN_extTSSetRawStatus(uint32_t baseAddr)
{
    HW_WR_FIELD32(MCAN_MCANSS_IRS(baseAddr),
                  MCAN_MCANSS_IRS_EXT_TS_CNTR_OVFL,
                  1U);
}

void MCAN_extTSClearRawStatus(uint32_t baseAddr)
{
    HW_WR_FIELD32(MCAN_MCANSS_ICS(baseAddr),
                  MCAN_MCANSS_ICS_EXT_TS_CNTR_OVFL,
                  1U);
}

uint32_t MCAN_getRxPinState(uint32_t baseAddr)
{
    return (HW_RD_FIELD32(MCAN_TEST(baseAddr), MCAN_TEST_RX));
}

void MCAN_setTxPinState(uint32_t baseAddr, uint32_t state)
{
    MCAN_writeProtectedRegAccessUnlock(baseAddr);

    HW_WR_FIELD32(MCAN_CCCR(baseAddr), MCAN_CCCR_TEST, 0x1U);
    HW_WR_FIELD32(MCAN_TEST(baseAddr),
                  MCAN_TEST_TX,
                  state);

    MCAN_writeProtectedRegAccessLock(baseAddr);
}

uint32_t MCAN_getTxPinState(uint32_t baseAddr)
{
    return (HW_RD_FIELD32(MCAN_TEST(baseAddr), MCAN_TEST_TX));
}

uint32_t MCAN_getTSCounterVal(uint32_t baseAddr)
{
    return (HW_RD_FIELD32(MCAN_TSCV(baseAddr), MCAN_TSCV_TSC));
}

uint32_t MCAN_getClkStopAck(uint32_t baseAddr)
{
    return (HW_RD_FIELD32(MCAN_CCCR(baseAddr), MCAN_CCCR_CSA));
}

void MCAN_getBitTime(uint32_t              baseAddr,
                     MCAN_BitTimingParams *configParams)
{
    configParams->nomSynchJumpWidth = HW_RD_FIELD32(MCAN_NBTP(baseAddr),
                                                    MCAN_NBTP_NSJW);
    configParams->nomTimeSeg2 = HW_RD_FIELD32(MCAN_NBTP(baseAddr),
                                              MCAN_NBTP_NTSEG2);
    configParams->nomTimeSeg1 = HW_RD_FIELD32(MCAN_NBTP(baseAddr),
                                              MCAN_NBTP_NTSEG1);
    configParams->nomRatePrescalar = HW_RD_FIELD32(MCAN_NBTP(baseAddr),
                                                   MCAN_NBTP_NBRP);

    configParams->dataSynchJumpWidth = HW_RD_FIELD32(MCAN_DBTP(baseAddr),
                                                     MCAN_DBTP_DSJW);
    configParams->dataTimeSeg2 = HW_RD_FIELD32(MCAN_DBTP(baseAddr),
                                               MCAN_DBTP_DTSEG2);
    configParams->dataTimeSeg1 = HW_RD_FIELD32(MCAN_DBTP(baseAddr),
                                               MCAN_DBTP_DTSEG1);
    configParams->dataRatePrescalar = HW_RD_FIELD32(MCAN_DBTP(baseAddr),
                                                    MCAN_DBTP_DBRP);
}

void MCAN_resetTSCounter(uint32_t baseAddr)
{
    HW_WR_FIELD32(MCAN_TSCV(baseAddr), MCAN_TSCV_TSC, 0x0U);
}

uint32_t MCAN_getTOCounterVal(uint32_t baseAddr)
{
    return (HW_RD_FIELD32(MCAN_TOCV(baseAddr), MCAN_TOCV_TOC));
}

uint32_t MCAN_extTSIsIntrEnable(uint32_t baseAddr)
{
    uint32_t status;

    if (1U == HW_RD_FIELD32(MCAN_MCANSS_IES(baseAddr),
                            MCAN_MCANSS_IES_EXT_TS_CNTR_OVFL))
    {
        status = (uint32_t) 1U;
    }
    else
    {
        status = (uint32_t) 0;
    }

    return status;
}

uint32_t MCAN_getEndianVal(uint32_t baseAddr)
{
    return (HW_RD_FIELD32(MCAN_ENDN(baseAddr), MCAN_ENDN_ETV));
}

uint32_t MCAN_getExtIDANDMassk(uint32_t baseAddr)
{
    return (HW_RD_FIELD32(MCAN_XIDAM(baseAddr), MCAN_XIDAM_EIDM));
}

/* ========================================================================== */
/*                          Internal Functions                                */
/* ========================================================================== */

static void MCAN_writeProtectedRegAccessUnlock(uint32_t baseAddr)
{
    HW_WR_FIELD32(MCAN_CCCR(baseAddr), MCAN_CCCR_CCE, 0x1U);
}

static void MCAN_writeProtectedRegAccessLock(uint32_t baseAddr)
{
    HW_WR_FIELD32(MCAN_CCCR(baseAddr), MCAN_CCCR_CCE, 0x0U);
}

static void MCAN_eccLoadRegister(uint32_t baseAddr, uint32_t regOffset)
{
    uint32_t regVal = 0U, offset;

    offset  = regOffset & 0xFFU;
    regVal |= ((uint32_t)MCANSS_MSG_RAM_NUM << MCAN_ECC_AGGR_VECTOR_SHIFT);
    regVal |= (offset << MCAN_ECC_AGGR_VECTOR_RD_SVBUS_ADDRESS_SHIFT);
    regVal |= ((uint32_t)1U << MCAN_ECC_AGGR_VECTOR_RD_SVBUS_SHIFT);
    HW_WR_REG32(MCAN_ECC_AGGR_VECTOR(baseAddr), regVal);
    while (MCAN_ECC_AGGR_VECTOR_RD_SVBUS_DONE_MASK !=
           (HW_RD_REG32(MCAN_ECC_AGGR_VECTOR(baseAddr)) &
            MCAN_ECC_AGGR_VECTOR_RD_SVBUS_DONE_MASK))
    {}
}

static void MCAN_readMsg(uint32_t           baseAddr,
                         uint32_t           elemAddr,
                         MCAN_RxBufElement *elem)
{
    uint32_t regVal = 0U;

    regVal   = HW_RD_REG32(baseAddr + elemAddr);
    elem->id = (uint32_t) ((regVal & MCANSS_RX_BUFFER_ELEM_ID_MASK)
                           >> MCANSS_RX_BUFFER_ELEM_ID_SHIFT);
    elem->rtr = (uint32_t) ((regVal & MCANSS_RX_BUFFER_ELEM_RTR_MASK)
                            >> MCANSS_RX_BUFFER_ELEM_RTR_SHIFT);
    elem->xtd = (uint32_t) ((regVal & MCANSS_RX_BUFFER_ELEM_XTD_MASK)
                            >> MCANSS_RX_BUFFER_ELEM_XTD_SHIFT);
    elem->esi = (uint32_t) ((regVal & MCANSS_RX_BUFFER_ELEM_ESI_MASK)
                            >> MCANSS_RX_BUFFER_ELEM_ESI_SHIFT);

    elemAddr  += 4U;
    regVal     = HW_RD_REG32(baseAddr + elemAddr);
    elem->rxts = (uint32_t) ((regVal & MCANSS_RX_BUFFER_ELEM_RXTS_MASK)
                             >> MCANSS_RX_BUFFER_ELEM_RXTS_SHIFT);
    elem->dlc = (uint32_t) ((regVal & MCANSS_RX_BUFFER_ELEM_DLC_MASK)
                            >> MCANSS_RX_BUFFER_ELEM_DLC_SHIFT);
    elem->brs = (uint32_t) ((regVal & MCANSS_RX_BUFFER_ELEM_BRS_MASK)
                            >> MCANSS_RX_BUFFER_ELEM_BRS_SHIFT);
    elem->fdf = (uint32_t) ((regVal & MCANSS_RX_BUFFER_ELEM_FDF_MASK)
                            >> MCANSS_RX_BUFFER_ELEM_FDF_SHIFT);
    elem->fidx = (uint32_t) ((regVal & MCANSS_RX_BUFFER_ELEM_FIDX_MASK)
                             >> MCANSS_RX_BUFFER_ELEM_FIDX_SHIFT);
    elem->anmf = (uint32_t) ((regVal & MCANSS_RX_BUFFER_ELEM_ANMF_MASK)
                             >> MCANSS_RX_BUFFER_ELEM_ANMF_SHIFT);
    elemAddr += 4U;

    elem->dataPtr = (void *)(baseAddr + elemAddr);
}

static void MCAN_writeMsg(uint32_t                 baseAddr,
                          uint32_t                 elemAddr,
                          const MCAN_TxBufElement *elem)
{
    uint32_t regVal = 0, loopCnt = 0U;

    regVal  = 0U;
    regVal |= (((uint32_t) (elem->id << MCANSS_TX_BUFFER_ELEM_ID_SHIFT)) |
               ((uint32_t) (elem->rtr << MCANSS_TX_BUFFER_ELEM_RTR_SHIFT)) |
               ((uint32_t) (elem->xtd << MCANSS_TX_BUFFER_ELEM_XTD_SHIFT)) |
               ((uint32_t) (elem->esi << MCANSS_TX_BUFFER_ELEM_ESI_SHIFT)));
    HW_WR_REG32(baseAddr + elemAddr, regVal);
    elemAddr += 4U;

    regVal  = 0U;
    regVal |= ((uint32_t) (elem->dlc << MCANSS_TX_BUFFER_ELEM_DLC_SHIFT)) |
              ((uint32_t) (elem->brs << MCANSS_TX_BUFFER_ELEM_BRS_SHIFT)) |
              ((uint32_t) (elem->fdf << MCANSS_TX_BUFFER_ELEM_FDF_SHIFT)) |
              ((uint32_t) (elem->efc << MCANSS_TX_BUFFER_ELEM_EFC_SHIFT)) |
              ((uint32_t) (elem->mm << MCANSS_TX_BUFFER_ELEM_MM_SHIFT));
    HW_WR_REG32(baseAddr + elemAddr, regVal);
    elemAddr += 4U;

    loopCnt = 0U;
    /* Framing words out of the payload bytes and writing it to message RAM */
    while ((4U <= (MCAN_getDataSize(elem->dlc) - loopCnt)) &&
           (0U != (MCAN_getDataSize(elem->dlc) - loopCnt)))
    {
        regVal  = 0U;
        regVal |= ((uint32_t)elem->data[loopCnt] |
                   ((uint32_t)elem->data[(loopCnt + 1U)] << 8U) |
                   ((uint32_t)elem->data[(loopCnt + 2U)] << 16U) |
                   ((uint32_t)elem->data[(loopCnt + 3U)] << 24U));
        HW_WR_REG32(baseAddr + elemAddr, regVal);
        elemAddr += 4U;
        loopCnt  += 4U;
    }
    /* Framing a word out of remaining payload bytes and writing it to
     * message RAM */
    if (0U < (MCAN_getDataSize(elem->dlc) - loopCnt))
    {
        regVal  = 0U;
        regVal |= ((uint32_t)elem->data[loopCnt] |
                   ((uint32_t)elem->data[(loopCnt + 1U)] << 8U) |
                   ((uint32_t)elem->data[(loopCnt + 2U)] << 16U) |
                   ((uint32_t)elem->data[(loopCnt + 3U)] << 24U));
        HW_WR_REG32(baseAddr + elemAddr, regVal);
    }
}

static uint32_t MCAN_getDataSize(uint32_t dlc)
{
    uint32_t dataSize[16] = {0,  1,  2,  3,  4,  5,  6, 7, 8,
                             12, 16, 20, 24, 32, 48, 64};

    return (dataSize[dlc]);
}

static uint32_t MCAN_getMsgObjSize(uint32_t elemSize)
{
    uint32_t objSize[8] = {4, 5, 6, 7, 8, 10, 14, 18};

    return (objSize[elemSize]);
}
