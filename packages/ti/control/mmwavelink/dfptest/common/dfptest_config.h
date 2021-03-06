/*
 *   @file  dfptest_config.h
 *
 *   @brief
 *      Header file for DFP test configuration
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2019 Texas Instruments, Inc.
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

#ifndef DFPTEST_CONFIG_H_
#define DFPTEST_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <ti/sysbios/knl/Semaphore.h>
/* mmwave SDK include files */
#include <ti/control/mmwavelink/mmwavelink.h>
#include <ti/control/mmwavelink/dfptest/common/dfptest_config.h>
#include <ti/control/mmwavelink/dfptest/common/dfptest_monitoring.h>

#define DFPTEST_MAX_NUM_PROFCFG  4
#define DFPTEST_MAX_NUM_CHIRPCFG 20
#define DFPTEST_MAX_NUM_BPMCHIRPCFG 20
#define DFPTEST_MAX_NUM_PHASESHCFG 2
#define DFPTEST_MAX_NUM_DYNPHASESHCFG 10

/* Config settings */
#define CQ_NUM_SLICES    13U

/* Tx Channel Configuration */
#define TX_CHANNEL_1_ENABLE                 (1U << 0U)
#define TX_CHANNEL_2_ENABLE                 (1U << 1U)
#define TX_CHANNEL_3_ENABLE                 (1U << 2U)
#define TX_CHANNEL_1_2_ENABLE               (TX_CHANNEL_1_ENABLE | TX_CHANNEL_2_ENABLE)
#define TX_CHANNEL_2_3_ENABLE               (TX_CHANNEL_2_ENABLE | TX_CHANNEL_3_ENABLE)
#define TX_CHANNEL_1_3_ENABLE               (TX_CHANNEL_1_ENABLE | TX_CHANNEL_3_ENABLE)
#define TX_CHANNEL_1_2_3_ENABLE             (TX_CHANNEL_1_ENABLE | TX_CHANNEL_2_ENABLE | TX_CHANNEL_3_ENABLE)

/* Rx Channel Configuration */
#define RX_CHANNEL_1_ENABLE                 (1U << 0U)
#define RX_CHANNEL_2_ENABLE                 (1U << 1U)
#define RX_CHANNEL_3_ENABLE                 (1U << 2U)
#define RX_CHANNEL_4_ENABLE                 (1U << 3U)
#define RX_CHANNEL_1_2_ENABLE               (RX_CHANNEL_1_ENABLE | RX_CHANNEL_2_ENABLE)
#define RX_CHANNEL_1_3_ENABLE               (RX_CHANNEL_1_ENABLE | RX_CHANNEL_3_ENABLE)
#define RX_CHANNEL_1_4_ENABLE               (RX_CHANNEL_1_ENABLE | RX_CHANNEL_4_ENABLE)
#define RX_CHANNEL_2_3_ENABLE               (RX_CHANNEL_2_ENABLE | RX_CHANNEL_3_ENABLE)
#define RX_CHANNEL_2_4_ENABLE               (RX_CHANNEL_2_ENABLE | RX_CHANNEL_4_ENABLE)
#define RX_CHANNEL_3_4_ENABLE               (RX_CHANNEL_3_ENABLE | RX_CHANNEL_4_ENABLE)
#define RX_CHANNEL_1_2_3_ENABLE             (RX_CHANNEL_1_ENABLE | RX_CHANNEL_2_ENABLE | RX_CHANNEL_3_ENABLE)
#define RX_CHANNEL_2_3_4_ENABLE             (RX_CHANNEL_2_ENABLE | RX_CHANNEL_3_ENABLE | RX_CHANNEL_4_ENABLE)
#define RX_CHANNEL_1_3_4_ENABLE             (RX_CHANNEL_1_ENABLE | RX_CHANNEL_3_ENABLE | RX_CHANNEL_4_ENABLE)
#define RX_CHANNEL_1_2_3_4_ENABLE           (RX_CHANNEL_1_ENABLE | RX_CHANNEL_2_ENABLE | RX_CHANNEL_3_ENABLE | RX_CHANNEL_4_ENABLE)

/* ADC Config Settings */
#define ADC_BITS_12                         (0U)
#define ADC_BITS_14                         (1U)
#define ADC_BITS_16                         (2U)

#define ADC_FORMAT_REAL                     (0U)
#define ADC_FORMAT_COMPLEX                  (1U)
#define ADC_FORMAT_CPMLEX_WITH_IMG_BAND     (2U)

#define ADC_I_FIRST                         (0U)
#define ADC_Q_FIRST                         (1U)

#define ADC_INTERLEAVED_MODE                (0U)
#define ADC_NON_INTERLEAVED_MODE            (1U)

/* Data Path Configuration */
#define DATA_PATH_CSI2                      (0U)
#define DATA_PATH_LVDS                      (1U)


#define DATA_PATH_FMT1_SUPRESS              (0U)
#define DATA_PATH_FMT1_CP_CQ                (1U)
#define DATA_PATH_FMT1_CQ_CP                (2U)

#define DATA_PATH_FMT0_ADC_DATA_ONLY        (0U)
#define DATA_PATH_FMT0_CP_ADC_DATA          (1U)
#define DATA_PATH_FMT0_ADC_CP_DATA          (2U)
#define DATA_PATH_FMT0_CP_ADC_CQ_DATA       (3U)

#define DATA_PATH_CQ_FMT_BITS_12            (0U)
#define DATA_PATH_CQ_FMT_BITS_14            (1U)
#define DATA_PATH_CQ_FMT_BITS_16            (2U)

/* LVDS Clock Configuration */
#define LVDS_LANE_CLOCK_SDR                 (0U)
#define LVDS_LANE_CLOCK_DDR                 (1U)

#define LVDS_ALL_LANE_EN                    (0xFU)

#define LVDS_DATA_RATE_900                  (0U)
#define LVDS_DATA_RATE_600                  (1U)
#define LVDS_DATA_RATE_450                  (2U)
#define LVDS_DATA_RATE_400                  (3U)
#define LVDS_DATA_RATE_300                  (4U)
#define LVDS_DATA_RATE_225                  (5U)
#define LVDS_DATA_RATE_150                  (6U)

/* LCDS Lane Configuration */
#define LVDS_LANE1_DISABLE                  (0U)
#define LVDS_LANE1_FORMAT_0                 (1U)
#define LVDS_LANE1_FORMAT_1                 (2U)

#define LVDS_LANE2_DISABLE                  (0U)
#define LVDS_LANE2_FORMAT_0                 (1U)
#define LVDS_LANE2_FORMAT_1                 (2U)

#define LVDS_LANE3_DISABLE                  (0U)
#define LVDS_LANE3_FORMAT_0                 (1U)
#define LVDS_LANE3_FORMAT_1                 (2U)

#define LVDS_LANE4_DISABLE                  (0U)
#define LVDS_LANE4_FORMAT_0                 (1U)
#define LVDS_LANE4_FORMAT_1                 (2U)

#define LVDS_LANE_MSB_FIRST_ENABLE          (1U)
#define LVDS_LANE_MSB_FIRST_DISABLE         (0U)
#define LVDS_LANE_PACKET_END_PULSE_ENABLE   (1U)
#define LVDS_LANE_PACKET_END_PULSE_DISABLE  (0U)
#define LVDS_LANE_CRC_ENABLE                (1U)
#define LVDS_LANE_CRC_DISABLE               (0U)
#define LVDS_LANE_TI_MODE_ENABLE            (1U)
#define LVDS_LANE_TI_MODE_DISABLE           (0U)

/* Advanced configuration */
#define ANA_CHANNEL_COMPLEX_CHAIN           (0U)
#define ANA_CHANNEL_REAL_CHAIN              (1U)

#define LP_ADC_MODE_REGULAR                 (0U)
#define LP_ADC_MODE_LOW_POWER               (1U)

#define NOISE_FIGURE_LOW                    (0U)
#define NOISE_FIGURE_HIGH                   (1U)


/* CHIRP tab settings */
/* CHIRP Sub-tab */
#define CHIRP_START_FREQ_VAL            (0U)
#define CHIRP_FREQ_SLOPE_VAL            (0U)
#define CHIRP_IDLE_TIME_VAL             (0U)
#define CHIRP_ADC_START_TIME_VAL        (0U)
#define CHIRP_TX_CHANNEL                CHIRP_TX_1_2_3_CHANNEL_ENABLE

/* PROFILE Sub-tab */
#define PROFILE_HPFCORNER_FREQ1_VAL     CHIRP_HPF1_CORNER_FREQ_175K
#define PROFILE_HPFCORNER_FREQ2_VAL     CHIRP_HPF2_CORNER_FREQ_350K
#define PROFILE_RX_GAIN_VAL             (30U)
#define PROFILE_DIGOUT_SAMPLERATE_VAL1  6000U
#define PROFILE_DIGOUT_SAMPLERATE_VAL2  12500U
#define PROFILE_ADC_SAMPLE_VAL          (256U)
#define PROFILE_IDLE_TIME_VAL           1000U /* 10 usec */
#define PROFILE_IDLE_TIME_VAL_16_98us   1698U
#define PROFILE_IDLE_TIME_VAL_89_41us   8941U
#define PROFILE_RAMP_END_TIME_VAL       6500U /* 65 usec */
#define PROFILE_RAMP_END_TIME_VAL_67_04us 6704U
#define PROFILE_START_FREQ_VAL_60GHz    (1497521874U)
#define PROFILE_START_FREQ_VAL          (1435384036U)
#define PROFILE_START_FREQ_VAL_76_024GHz (1417190077U)
#define PROFILE_START_FREQ_VAL_76_5GHz   (1426063360U)
#define PROFILE_TXOUT_POWER_BACKOFF     (0U)
#define PROFILE_TXPHASESHIFTER_VAL      (0U)
#define PROFILE_FREQ_SLOPE_VAL_60GHz    828U
#define PROFILE_FREQ_SLOPE_VAL          621U
#define PROFILE_FREQ_SLOPE_VAL_14001k   290U
#define PROFILE_TX_START_TIME_VAL       0
#define PROFILE_TX_START_TIME_VAL_1us   100U
#define PROFILE_ADC_START_TIME_VAL      600U

/* FRAME Sub-tab */
#define FRAME_LOOP_COUNT                (1)
#define FRAME_PRIODICITY_VAL            (40000000) /* 200 mSec */
#define FRAME_TRIGGER_DELAY_VAL         (0U)
#define FRAME_ADC_SAMPLE_NUM            (512U)


/* CHIRP Tx Channel Settings */
#define CHIRP_TX_1_CHANNEL_ENABLE           (1U << 0U)
#define CHIRP_TX_2_CHANNEL_ENABLE           (1U << 1U)
#define CHIRP_TX_3_CHANNEL_ENABLE           (1U << 2U)
#define CHIRP_TX_1_2_CHANNEL_ENABLE         (CHIRP_TX_1_CHANNEL_ENABLE | CHIRP_TX_2_CHANNEL_ENABLE)
#define CHIRP_TX_1_3_CHANNEL_ENABLE         (CHIRP_TX_2_CHANNEL_ENABLE | CHIRP_TX_3_CHANNEL_ENABLE)
#define CHIRP_TX_2_3_CHANNEL_ENABLE         (CHIRP_TX_2_CHANNEL_ENABLE | CHIRP_TX_3_CHANNEL_ENABLE)
#define CHIRP_TX_1_2_3_CHANNEL_ENABLE       (CHIRP_TX_1_CHANNEL_ENABLE | CHIRP_TX_2_CHANNEL_ENABLE | CHIRP_TX_3_CHANNEL_ENABLE)

/* CHIRP Profile Settings */
#define CHIRP_HPF1_CORNER_FREQ_175K         (0U)
#define CHIRP_HPF1_CORNER_FREQ_235K         (1U)
#define CHIRP_HPF1_CORNER_FREQ_350K         (2U)
#define CHIRP_HPF1_CORNER_FREQ_700K         (3u)

#define CHIRP_HPF2_CORNER_FREQ_350K         (0U)
#define CHIRP_HPF2_CORNER_FREQ_700K         (1U)
#define CHIRP_HPF2_CORNER_FREQ_1_4M         (2U)
#define CHIRP_HPF2_CORNER_FREQ_2_8M         (3U)
#define CHIRP_HPF2_CORNER_FREQ_5M           (4U)
#define CHIRP_HPF2_CORNER_FREQ_7_5M         (5U)
#define CHIRP_HPF2_CORNER_FREQ_10M          (6U)
#define CHIRP_HPF2_CORNER_FREQ_15M          (7U)

/* Monitoring Settings */
#define MON_REPORT_MODE_PERIODIC_WITHOUT_THRESHOLD_CHECK    (0U)
#define MON_REPORT_MODE_AT_FAILURE_ONLY                     (1U)
#define MON_REPORT_MODE_PERIODIC_WITH_THRESHOLD_CHECK       (2U)

#define LOWEST_RF_FRQ_IN_PROFILES_SWEEP_BW                    (0U)
#define CENTER_RF_FRQ_IN_PROFILES_SWEEP_BW                    (1U)
#define HIGHEST_RF_FRQ_IN_PROFILES_SWEEP_BW                    (2U)
#define HIGHEST_CENTER_LOWEST_RF_FRQ_IN_PROFILES_SWEEP_BW    (7U)

/* Dynamic Chirp Settings */
#define NUM_OF_DYNAMIC_CHIRP_CONFIG_SEG        (4U)

/* Bit manipulations */
#define GET_BIT_VALUE(data, noOfBits, location)    ((((rlUInt32_t)(data)) >> (location)) &\
                                               (((rlUInt32_t)((rlUInt32_t)1U << (noOfBits))) - (rlUInt32_t)1U))


/* Maximum number of start stop with reconfiguration in between*/
#define MMWAVELINK_TEST_MAX_NUM_STARTSTOP_RECONFIG 50

/*Start/stop/reconfigure configuration*/
typedef struct dfpTestStartStopCfg
{
    /* Number of startStop to be run without reconfiguration in between*/
    uint32_t numStartStopNoReconfig;

    /* Number of startStop to be run with reconfiguration in between*/
    uint32_t numStartStopReconfig;

    /* If numStartStopReconfig > 0, this array stores which test number to switch to after
       each stop*/
    uint32_t testNumberSwitchAfterStop[MMWAVELINK_TEST_MAX_NUM_STARTSTOP_RECONFIG];

}dfpTestStartStopCfg_t;

/*Test miscellaneous configuration*/
typedef struct dfpTestMiscCfg
{
    /*Set to "true" if the device has the ???Second digital temp sensor??? (tmpDig1Sens in rlRfTempData_t).*/
    bool dig1TempSensor;

    /*Set to "true" if this is a test with advanced frame.
      Set to "false" if this is a test with legacy frame.*/
    bool testAdvancedFrame;

    /*Number of latent fault API calls before frame start*/
    uint32_t numLatentFaultBeforeFrameStart;

    /*Number of latent fault API calls after frame stop*/
    uint32_t numLatentFaultAfterFrameStop;

    /* Indicates how many chirps configurations are used in the test*/
    uint16_t numChirpConfigs;

    /* Indicates how many BPM chirp configs are used in the test.
       Set it to zero if no BPM chirp configuration are used.*/
    uint16_t numBpmChirpConfigs;

    /* Indicates how many profiles are configured for the test*/
    uint16_t numProfiles;

    /* Bitmask indicating which monitoring features are expected to
       generate a failure report and fail status flag*/
    uint32_t failFeatureMask;

    /* Holds the expected status flag for the respective fail monitoring feature.
       The value of the failed status flag depends on the induced failure.
       For instance, suppose that for the temperature monitoring the anaTempThreshMax
       was set such that it will fail, then the corresponding bit STATUS_ANA_TEMP_MAX
       in the status flag for the temperature monitor report should be zero and this is
       reflected in this array. The position for the temperature monitoring status flag
       is failStatusFlag[MMWAVELINK_TEST_MON_TEMP]*/
    uint32_t failStatusFlag[MMWAVELINK_TEST_MAX_NUM_MON];
}dfpTestMiscCfg_t;

extern rlChanCfg_t chCfg;
extern rlAdcOutCfg_t adcOutCfgArgs;
extern rlLowPowerModeCfg_t lowPowerModeCfg;
extern rlDevHsiClk_t deviceHsiClk;
extern rlProfileCfg_t  profileCfg[DFPTEST_MAX_NUM_PROFCFG];
extern rlChirpCfg_t  chirpCfg[DFPTEST_MAX_NUM_CHIRPCFG];
extern rlDynChirpCfg_t dynChirpCfg[10];
extern rlRxSatMonConf_t rxSatMonCfg;
extern rlSigImgMonConf_t sigImgMonCfg;
extern rlFrameCfg_t  frameCfg;
extern rlRfCalMonTimeUntConf_t calMonTimeUnitConf;
extern rlRfCalMonFreqLimitConf_t freqLimit;
extern rlRfInitCalConf_t rfInitCalib;
extern rlRunTimeCalibConf_t runTimeCalib;
extern rlAdvFrameCfg_t advFrameCfg;
extern rlBpmCommonCfg_t bpmCommonCfg;
extern rlBpmChirpCfg_t bpmChirpCfg[DFPTEST_MAX_NUM_BPMCHIRPCFG];
extern rlContModeCfg_t contModeCfg;
extern rlContModeEn_t contModeEn;
extern rlDynPwrSave_t dynPwrSave;
extern rlGpAdcCfg_t gpAdcCfg;
extern rlRfLdoBypassCfg_t rfLdoBypassCfg;
extern rlMonAnaEnables_t monAnaEnables;
extern rlMonDigEnables_t monDigEnables;
extern rlDigMonPeriodicConf_t monDigPeriodicCfg;
extern rlTempMonConf_t tempMonCfg;
extern rlRxGainPhaseMonConf_t rxGainPhaseMonCfg;
extern rlRxNoiseMonConf_t rxNoiseMonCfg;
extern rlRxIfStageMonConf_t monRxIfStageCfg;
extern rlTxPowMonConf_t tx0PowMonCfg;
extern rlTxPowMonConf_t tx1PowMonCfg;
extern rlTxPowMonConf_t tx2PowMonCfg;
extern rlAllTxPowMonConf_t allTxPowMonCfg;
extern rlTxBallbreakMonConf_t tx0BallBreakMonCfg;
extern rlTxBallbreakMonConf_t tx1BallBreakMonCfg;
extern rlTxBallbreakMonConf_t tx2BallBreakMonCfg;
extern rlAllTxBallBreakMonCfg_t allTxBallbreakMonCfg;
extern rlTxGainPhaseMismatchMonConf_t txGainPhMisMatchMonCfg;
extern rlTxBpmMonConf_t tx0BpmMonCfg;
extern rlTxBpmMonConf_t tx1BpmMonCfg;
extern rlTxBpmMonConf_t tx2BpmMonCfg;
extern rlAllTxBpmMonConf_t allTxBpmMonCfg;
extern rlSynthFreqMonConf_t synthFreqMonCfg;
extern rlSynthFreqMonConf_t synthFreqNonLiveMonCfg[2U];
extern rlExtAnaSignalsMonConf_t extAnaSigMonCfg;
extern rlTxIntAnaSignalsMonConf_t tx0IntAnaSigMonCfg;
extern rlTxIntAnaSignalsMonConf_t tx1IntAnaSigMonCfg;
extern rlTxIntAnaSignalsMonConf_t tx2IntAnaSigMonCfg;
extern rlAllTxIntAnaSignalsMonConf_t allTxanaSigMonCfg;
extern rlRxIntAnaSignalsMonConf_t rxIntAnaSigMonCfg;
extern rlPmClkLoIntAnaSignalsMonConf_t pmClkLoIntAnaSigMonCfg;
extern rlGpadcIntAnaSignalsMonConf_t gpadcIntAnaSigMonCfg;
extern rlPllContrVoltMonConf_t pllConVoltMonCfg;
extern rlDualClkCompMonConf_t dualClkCompMonCfg;
extern rlRxMixInPwrMonConf_t rxMixInpwrMonCfg;
extern rlRfProgFiltConf_t progFiltCfg;
extern rlRfProgFiltCoeff_t progFiltCoef;
extern rlSubFrameStartCfg_t subFrameTrigCfg;
extern rlRfMiscConf_t miscCfg;
extern rlRfPhaseShiftCfg_t phaseShiftCfg[DFPTEST_MAX_NUM_PHASESHCFG];
extern rlDynPerChirpPhShftCfg_t dynPhaseShiftCfg[DFPTEST_MAX_NUM_DYNPHASESHCFG];
extern rlInterRxGainPhConf_t interRxGainPhCfg;
extern rlTestSource_t testSourceCfg;
extern rlTestSourceEnable_t testSourceEn;
extern rlRxGainTempLutReadReq_t rxGainTempLutReadCfg;
extern rlTxGainTempLutReadReq_t txGainTempLutReadCfg;

extern dfpTestStartStopCfg_t gTestStartStopCfg;
extern dfpTestMiscCfg_t gTestMiscCfg;

extern uint32_t gTestNumber;
extern uint16_t gNumberOfFrames;
extern bool gIwrDeviceFlag;

void dfpTestLoadCfg_001(void);
void dfpTestLoadCfg_002(void);
void dfpTestLoadCfg_003(void);
void dfpTestLoadCfg_004(void);
void dfpTestLoadCfg_005(void);
void dfpTestLoadCfg_006(void);
void dfpTestLoadCfg_007(void);
void dfpTestLoadCfg_008(void);
void dfpTestLoadCfg_009(void);
void dfpTestLoadCfg_010(void);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef DFPTEST_CONFIG_H_ */
