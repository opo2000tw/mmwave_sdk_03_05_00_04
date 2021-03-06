/**
 *   @file  framework.h
 *
 *   @brief
 *      Test Framework Exported Header File.
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2017 Texas Instruments, Inc.
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
/**
 * ## Test Framework #
 *
 *  The Test Framework was designed for the following:-
 *
 *  1. Modularity
 *     The framework provides a clear demarcation between the application
 *     code and the data path profile.
 *  2. Simple Intutive API
 *     The test framework exposes a set of standard well defined API
 *     which allows application developers to load, configure and execute
 *     a profile
 *
 * ## Top Level Architecture #
 *
 *  The following is the top level architecture figure which showcases the
 *  demarcation between the application, Test Framework and Profile
 *
 *  @image html framework_architecture.png
 *
 * The Profile can be defined as the data path processing block which receives
 * the ADC data and performs any processing on it. The profile should be able
 * to pass back the results of this processing in any format it deems fit.
 *
 * Example:
 * CSI Stream Profile: This is an example of a profile which captures the first
 * N sampples of the ADC Data into memory and also streams it out using the CSI
 * High speed interface.
 *
 * ## Application Developers #
 *
 * The Test framework can be used by application developers which decide to utilize
 * one of the "standard" profiles. The developers in this case will use the Test
 * Framework API exported by the module. @sa TEST_FRAMEWORK_EXTERNAL_FUNCTION
 *
 * ## Profile Developers #
 *
 * The Test Framework can be used by developers to create their own custom profile
 * in which case please refer to the profile exported API
 * @sa TEST_FRAMEWORK_PROFILE_FUNCTIONS
 *
 * ## Reporting #
 *
 * Application using the framework need to register a report function which receives
 * status reports from the framework module. This is an important feedback mechanism
 * through which the framework can keep the application informed about the status of
 * operations. For a list of all the reports @sa TestFmk_Report
 */

/** @defgroup TEST_FRAMEWORK      Test Framework API
 */
#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

/* mmWave SDK Include Files: */
#include <ti/common/mmwave_error.h>
#include <ti/drivers/soc/soc.h>
#include <ti/control/mmwave/mmwave.h>
#include <ti/drivers/osal/SemaphoreP.h>
#include <ti/drivers/adcbuf/ADCBuf.h>

/**
@defgroup TEST_FRAMEWORK_EXTERNAL_FUNCTION            Test Framework External Functions
@ingroup TEST_FRAMEWORK
@brief
*   The section has a list of all the exported API which the applications need to
*   invoke in order to use the driver
*/
/**
@defgroup TEST_FRAMEWORK_EXTERNAL_DATA_STRUCTURE       Test Framework External Data Structures
@ingroup TEST_FRAMEWORK
@brief
*   The section has a list of all the data structures which are exposed to the
*   application
*/
/**
@defgroup TEST_FRAMEWORK_EXTERNAL_DEFINITIONS          Test Framework External Defintions
@ingroup TEST_FRAMEWORK
@brief
*   The section has a list of all external definitions which are exposed by the
*   module.
*/
/**
@defgroup TEST_FRAMEWORK_ERROR_CODE                    Test Framework Error Codes
@ingroup TEST_FRAMEWORK
@brief
*   The section has a list of all the error codes which are generated by the module
*/
/**
@defgroup TEST_FRAMEWORK_INTERNAL_FUNCTION             Test Framework Internal Functions
@ingroup TEST_FRAMEWORK
@brief
*   The section has a list of all internal API which are not exposed to the external
*   applications.
*/
/**
@defgroup TEST_FRAMEWORK_INTERNAL_DATA_STRUCTURE       Test Framework Internal Data Structures
@ingroup TEST_FRAMEWORK
@brief
*   The section has a list of all internal data structures which are used internally
*   by the module.
*/
/**
@defgroup TEST_FRAMEWORK_INTERNAL_DEFINITIONS          Test Framework Internal Defintions
@ingroup TEST_FRAMEWORK
@brief
*   The section has a list of all internal definitions which are used internally
*   by the module.
*/
/**
@defgroup TEST_FRAMEWORK_PROFILE_FUNCTIONS          Test Framework Profile API
@ingroup TEST_FRAMEWORK
@brief
*   The section has a list of all function which need to be populated by a developers
*   writing a custom profile to fit with the Test Framework.
*/

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup TEST_FRAMEWORK_ERROR_CODE
 @{ */

#define MMWAVE_ERRNO_TEST_FRAMEWORK_BASE           -62000

/**
 * @brief   Error Code: Invalid argument
 */
#define TESTFMK_EINVAL                  (MMWAVE_ERRNO_TEST_FRAMEWORK_BASE-1)

/**
 * @brief   Error Code: Out of memory
 */
#define TESTFMK_ENOMEM                  (MMWAVE_ERRNO_TEST_FRAMEWORK_BASE-2)

/**
 * @brief   Error Code: Not supported
 */
#define TESTFMK_ENOTSUP                 (MMWAVE_ERRNO_TEST_FRAMEWORK_BASE-3)

/**
 * @brief   Error Code: Profile configuration error
 */
#define TESTFMK_EPROFCFG                (MMWAVE_ERRNO_TEST_FRAMEWORK_BASE-4)

/**
 * @brief   Error Code: Profile reported error. Please refer to the
 * profile for more details.
 */
#define TESTFMK_EPROFERR                (MMWAVE_ERRNO_TEST_FRAMEWORK_BASE-5)

/**
@}
*/

/** @addtogroup TEST_FRAMEWORK_EXTERNAL_DEFINITIONS
 @{ */

/**
 * @brief
 * This is the command which can be used by the application or profile developers
 * to get the ADCBUF driver handle from the core module.
 *      arg    = &ADCBuf_Handle
 *      argLen = sizeof (ADCBuf_Handle)
 */
#define TEST_FMK_CMD_GET_ADCBUF_HANDLE                    1

/**
 * @brief
 * This is the command which can be used by the application or profile developers
 * to get the EDMA driver handle from the core module.
 *      arg    = &EDMA_Handle[NUM_CC]
 *      argLen = sizeof (EDMA_Handle)
 *
 * *NOTE*: The number of EDMA instances is different between the MSS & DSS. Please
 * refer to the definition in the sys_common.h and pass the correct size.
 */
#define TEST_FMK_CMD_GET_EDMA_HANDLE                     2

/**
 * @brief
 * This is the command which can be used by the application or profile developers
 * to get the configuration.
 *      arg    = &TestFmk_Cfg
 *      argLen = sizeof (TestFmk_Cfg)
 */
#define TEST_FMK_CMD_GET_CONFIG                         3

/**
 * @brief
 * This is the command which can be used by the application to set the
 * configuration which will be used across the Test Framework Instances.
 *      arg    = &TestFmk_Cfg
 *      argLen = sizeof (TestFmk_Cfg)
 *
 * *NOTE*: This should be the last configuration command issued to
 * the profile. Any profile specific configuration should be done before
 * using this "command".
 */
#define TEST_FMK_CMD_SET_CONFIG                         4

/**
 * @brief
 * This is the command which can be used by the application to inform the
 * module that there was a CPU/ESM fault reported by the mmWave link layer.
 * This is passed to the profile to take appropriate action.
 *      arg    = NULL
 *      argLen = 0
 */
#define TEST_FMK_CMD_NOTIFY_FAULT                        5

/**
 * @brief
 * Profile specific command start index: Command Index before this are reserved
 * for use by the core Module. All profiles should use this as a starting index
 * for their own custom commmands.
 */
#define TEST_FMK_CMD_PROFILE_START_INDEX                 100

/**
@}
*/

/** @addtogroup TEST_FRAMEWORK_EXTERNAL_DATA_STRUCTURE
 @{ */

/**
 * @brief
 *  Test Framework Handle
 */
typedef void*   TestFmk_Handle;

/**
 * @brief
 *  Test Framework Profile Handle
 */
typedef void*   TestFmk_ProfileHandle;

/**
 * @brief
 *  Report Types
 *
 * @details
 *  The enumeration is used to describe the various reports which are generated
 *  by the Module. Applications are notified about the operational status
 *  of the Module through these reports via the registered Reporting Function.
 *  Applications are expected to interpret these reports and take appropriate
 *  action.
 */
typedef enum TestFmk_Report_e
{
    /**
     * @brief   This is the report type to indicate that the profile has been
     * loaded. The following is the interpretation of the optional arguments:-
     *      arg0 - Always set to 0. Not Used
     *      arg1 - Always set to 0. Not Used
     */
    TestFmk_Report_PROFILE_LOADED   = 0x1,

    /**
     * @brief   This is the report type to indicate that the configuration
     * has been received. Configuration can be set by either of the instances.
     * The following is the interpretation of the optional arguments:-
     *      arg0 - Address of the received configuration
     *      arg1 - Always set to 0. Not Used
     */
    TestFmk_Report_SET_CFG,

    /**
     * @brief   This is the report type to indicate that the Module & Profile
     * has been opened. The following is the interpretation of the optional arguments:-
     *      arg0 - Always set to 0. Not Used
     *      arg1 - Always set to 0. Not Used
     */
    TestFmk_Report_PROFILE_OPENED,

    /**
     * @brief   This is the report type to indicate that the Module & Profile
     * has been started. The following is the interpretation of the optional arguments:-
     *      arg0 - Always set to 0. Not Used
     *      arg1 - Always set to 0. Not Used
     */
    TestFmk_Report_PROFILE_STARTED,

    /**
     * @brief   This is the report type to indicate that the Module & Profile
     * has been stopped. The following is the interpretation of the optional arguments:-
     *      arg0 - Always set to 0. Not Used
     *      arg1 - Always set to 0. Not Used
     */
    TestFmk_Report_PROFILE_STOPPED,

    /**
     * @brief   This is the report type to indicate that the Module & Profile
     * is being configured via the TestFmk_ioctl interface.  The following is the
     * interpretation of the optional arguments:-
     *      arg0 - Command
     *      arg1 - Command Specific Data
     *
     * @sa TestFmk_ioctl
     */
    TestFmk_Report_IOCTL,

    /**
     * @brief   This is the report type to indicate that the Module & Profile
     * has been closed. The following is the interpretation of the optional arguments:-
     *      arg0 - Always set to 0. Not Used
     *      arg1 - Always set to 0. Not Used
     */
    TestFmk_Report_PROFILE_CLOSED,

    /**
     * @brief   This is the report type to indicate that the Profile
     * has been unloaded. The following is the interpretation of the optional arguments:-
     *      arg0 - Always set to 0. Not Used
     *      arg1 - Always set to 0. Not Used
     */
    TestFmk_Report_PROFILE_UNLOADED
}TestFmk_Report;

/**
 *  @b Description
 *  @n
 *      Applications can register a report function which is invoked by the module
 *      to report the status of the profile to the application. For example: Once the
 *      profile has been configured the profile will notify the application that the
 *      sensor can be started. Errors are reported back through the same mechanism.
 *
 *  @param[in]  reportType
 *      Report Type
 *  @param[in]  errCode
 *      Error code associated with the status event. This will be set to 0 to indicate
 *      that the report was successful.
 *  @param[in]  arg0
 *      Optional argument.
 *  @param[in]  arg1
 *      Optional argument.
 *
 *  *NOTE*: Please refer to the TestFmk_Report (@sa TestFmk_Report) for more information
 *  about the optional arguments and how to interpret them.
 *
 *  @retval
 *      Not applicable
 */
typedef void (*TestFmk_ReportFxn) (TestFmk_Report reportType, int32_t errCode, uint32_t arg0, uint32_t arg1);

/**
 * @brief
 *  Initialization configuration
 *
 * @details
 *  The structure is used to initialize the TestFmk module
 */
typedef struct TestFmk_InitCfg_t
{
    /**
     * @brief   Handle to the SOC Driver
     */
    SOC_Handle          socHandle;

    /**
     * @brief   Handle to the mmWave control module. The mmWave control
     * handle is not used as is by the framework but is passed to the
     * profile for its use.
     *
     * Please refer to the profile documentation about the handle.
     */
    MMWave_Handle       ctrlHandle;

    /**
     * @brief   Report Function: The TestFmk module will invoke the application
     * registered function to report events and status about the TestFmk profiles
     */
    TestFmk_ReportFxn   reportFxn;
}TestFmk_InitCfg;

/**
 * @brief
 *  Frame Configuration
 *
 * @details
 *  The structure is used to track the frame configuration.
 */
typedef struct TestFmk_FrameCfg_t
{
    /**
     * @brief   Number of ADC samples:
     */
    uint32_t        numADCSamples;
}TestFmk_FrameCfg;

/**
 * @brief
 *  Frame Configuration
 *
 * @details
 *  The structure is used to track the continuous configuration.
 */
typedef struct TestFmk_ContCfg_t
{
    /**
     * @brief   Sample count: This refers to the number of samples per
     * channel.
     */
    uint16_t        dataTransSize;
}TestFmk_ContCfg;

/**
 * @brief
 *  TestFmk Configuration
 *
 * @details
 *  The structure is used to configure the data path and loaded profile.
 */
typedef struct TestFmk_Cfg_t
{
    /**
     * @brief   DFE Data Output Mode:
     */
    MMWave_DFEDataOutputMode        dfeDataOutputMode;

    /**
     * @brief   ADC Buffer Output format:
     *  0 - Complex
     *  1 - Real
     */
    uint8_t                         adcFmt;

    /**
     * @brief   ADCBUF IQ swap selection:
     *  0 - I in LSB, Q in MSB,
     *  1 - Q in LSB, I in MSB
     */
    uint8_t                         iqSwapSel;

    /**
     * @brief   ADCBUF channel Interleave Configuration:
     *  0 - Interleaved (NOT supported on XWR16xx/XWR18xx/XWR68xx)
     *  1 - Non-interleaved
     */
    uint8_t                         chInterleave;

    /**
     * @brief   Chirp Threshold:
     */
    uint8_t                         chirpThreshold;

    /**
     * @brief   Receive channel mask:
     */
    uint32_t                        rxChannelEn;

    /**
     * @brief   Flag which determines if the CQ needs to be configured
     */
    uint8_t                         enableCQ;

    /**
     * @brief   CQ Configuration: This is only used if the enable chirp
     * quality is enabled; else the configuration is ignored.
     */
    ADCBuf_CQConf                   cqCfg;

    /**
     * @brief   The configuration is DFE Data Mode specific.
     */
    union
    {
        /**
         * @brief   Frame configuration which is applicable only if
         * the mmWave Link has been configured to operate in frame or
         * advanced frame mode.
         */
        TestFmk_FrameCfg        frameCfg;

        /**
         * @brief   Continuous configuration which is applicable only if
         * the mmWave Link has been configured to operate in continuous
         * mode
         */
        TestFmk_ContCfg         contCfg;
    }u;
}TestFmk_Cfg;

/**
 * @brief
 *  TestFmk Profile Results
 *
 * @details
 *  Profiles post the result of the execution by populating this. The
 *  framework does not interpret this and passes this directly from the
 *  profile to the application. Developers are required to read the profile
 *  documentation about the exact format of the result.
 */
typedef struct TestFmk_Result_t
{
    /**
     * @brief   Pointer to the result buffer: Set to NULL implies that the profile
     * does not have any result available
     */
    uint8_t*            ptrBuffer;

    /**
     * @brief   Size of the result buffer: Set to 0 implies that the profile
     * does not have any result available
     */
    uint32_t            size;
}TestFmk_Result;

/**
 *  @b Description
 *  @n
 *      This is the profile registered function which is invoked once the
 *      chirp available interrupt has been detected
 *
 *  @param[in]  handle
 *      Handle to the profile
 *
 *  \ingroup TEST_FRAMEWORK_PROFILE_FUNCTIONS
 *
 *  @retval
 *      Not applicable
 */
typedef void (*TestFmk_ProfileChirpAvailableCallbackFxn) (TestFmk_ProfileHandle handle);

/**
 *  @b Description
 *  @n
 *      This is the profile registered function which is invoked once the
 *      frame start interrupt has been detected
 *
 *  @param[in]  handle
 *      Handle to the profile
 *
 *  \ingroup TEST_FRAMEWORK_PROFILE_FUNCTIONS
 *
 *  @retval
 *      Not applicable
 */
typedef void (*TestFmk_ProfileFrameStartCallbackFxn) (TestFmk_ProfileHandle handle);

/**
 *  @b Description
 *  @n
 *      This is the profile registered function which is used to initialize and
 *      setup the profile. This is invoked during the TestFmk Initialization process.
 *      Profiles are required to initialize and setup any of the modules which
 *      are required
 *
 *  @param[in]  fmkHandle
 *      Handle to the TestFmk Module loading the profile
 *  @param[in]  ptrInitCfg
 *      Pointer to the initialization configuration
 *  @param[in]  fmkSemHandle
 *      Handle to the framework semaphore.
 *  @param[out] errCode
 *      Error code populated by the API
 *
 *  \ingroup TEST_FRAMEWORK_PROFILE_FUNCTIONS
 *
 *  @retval
 *      Success - Handle to the profile
 *  @retval
 *      Error   - <0
 */
typedef TestFmk_ProfileHandle (*TestFmk_ProfileInitFxn) (TestFmk_Handle     fmkHandle,
                                                         TestFmk_InitCfg*   ptrInitCfg,
                                                         SemaphoreP_Handle  fmkSemHandle,
                                                         int32_t*           errCode);

/**
 *  @b Description
 *  @n
 *      This is the profile registered function which is used to execute the profile
 *      The profile is woken up on the reception of a chirp interrupt
 *
 *  @param[in]  handle
 *      Profile handle
 *  @param[out]  ptrResult
 *      Pointer to the result populated by the profile
 *  @param[out] errCode
 *      Error code populated by the API
 *
 *  \ingroup TEST_FRAMEWORK_PROFILE_FUNCTIONS
 *
 *  @retval
 *      Success - 0
 *  @retval
 *      Error   - <0
 */
typedef int32_t (*TestFmk_ProfileExecuteFxn) (TestFmk_ProfileHandle     handle,
                                              TestFmk_Result*           ptrResult,
                                              int32_t*                  errCode);

/**
 *  @b Description
 *  @n
 *      This is the profile registered function which is invoked by
 *      the framework to finalize all the configuration. The profile
 *      * MAY NOT* support any configuration modifications after the
 *      invocation of this function.
 *
 *  @param[in]  handle
 *      Profile handle
 *  @param[in]  ptrTestFmkCfg
 *      Pointer to the TestFmk configuration
 *  @param[out] errCode
 *      Error code populated by the API
 *
 *  \ingroup TEST_FRAMEWORK_PROFILE_FUNCTIONS
 *
 *  @retval
 *      Success - 0
 *  @retval
 *      Error   - <0
 */
typedef int32_t (*TestFmk_ProfileFinalizeCfgFxn) (TestFmk_ProfileHandle     handle,
                                                  TestFmk_Cfg*              ptrTestFmkCfg,
                                                  int32_t*                  errCode);

/**
 *  @b Description
 *  @n
 *      This is the profile registered function which is used to control the profile.
 *      Profile developers can use this to provide the ability to provide additional
 *      control capability. The "cmd" for profiles should be offset by
 *      TEST_FMK_CMD_PROFILE_START_INDEX to prevent an overlap with the TestFmk commands.
 *
 *      In addition to the profile specific commands the following commands should also
 *      be handled:-
 *      @sa TEST_FMK_CMD_NOTIFY_FAULT
 *
 *  @param[in]  handle
 *      Profile handle
 *  @param[in]  cmd
 *      Command to be processed
 *  @param[in]  arg
 *      Command specific argument.
 *  @param[in]  argLen
 *      Command specific argument length
 *  @param[out] errCode
 *      Error code populated by the API
 *
 *  \ingroup TEST_FRAMEWORK_PROFILE_FUNCTIONS
 *
 *  @retval
 *      Success - 0
 *  @retval
 *      Error   - <0
 */
typedef int32_t (*TestFmk_ProfileControlFxn) (TestFmk_ProfileHandle handle,
                                              uint32_t              cmd,
                                              void*                 arg,
                                              uint32_t              argLen,
                                              int32_t*              errCode);

/**
 *  @b Description
 *  @n
 *      This is the profile registered function which is used to start the profile.
 *      Profiles should be started before the sensor (BSS) is started. Profiles will
 *      use this to setup and prepare itself to start receiving ADC samples
 *
 *  @param[in]  handle
 *      Profile handle
 *  @param[out] errCode
 *      Error code populated by the API
 *
 *  \ingroup TEST_FRAMEWORK_PROFILE_FUNCTIONS
 *
 *  @retval
 *      Success - 0
 *  @retval
 *      Error   - <0
 */
typedef int32_t (*TestFmk_ProfileStartFxn) (TestFmk_ProfileHandle handle, int32_t* errCode);

/**
 *  @b Description
 *  @n
 *      This is the profile registered function which is used to stop the profile.
 *
 *  @param[in]  handle
 *      Profile handle
 *  @param[out] errCode
 *      Error code populated by the API
 *
 *  \ingroup TEST_FRAMEWORK_PROFILE_FUNCTIONS
 *
 *  @retval
 *      Success - 0
 *  @retval
 *      Error   - <0
 */
typedef int32_t (*TestFmk_ProfileStopFxn) (TestFmk_ProfileHandle handle, int32_t* errCode);

/**
 *  @b Description
 *  @n
 *      This is the profile registered function which is used to open the profile.
 *
 *  @param[in]  handle
 *      Profile handle
 *  @param[in]  ptrOpenCfg
 *      Pointer to the mmWave open configuration which was used to initialize the
 *      mmWave Link Layer
 *  @param[out] errCode
 *      Error code populated by the API
 *
 *  \ingroup TEST_FRAMEWORK_PROFILE_FUNCTIONS
 *
 *  @retval
 *      Success - 0
 *  @retval
 *      Error   - <0
 */
typedef int32_t (*TestFmk_ProfileOpenFxn) (TestFmk_ProfileHandle    handle,
                                           MMWave_OpenCfg*          ptrOpenCfg,
                                           int32_t*                 errCode);

/**
 *  @b Description
 *  @n
 *      This is the profile registered function which is used to close the profile.
 *
 *  @param[in]  handle
 *      Profile handle
 *  @param[out] errCode
 *      Error code populated by the API
 *
 *  \ingroup TEST_FRAMEWORK_PROFILE_FUNCTIONS
 *
 *  @retval
 *      Success - 0
 *  @retval
 *      Error   - <0
 */
typedef int32_t (*TestFmk_ProfileCloseFxn) (TestFmk_ProfileHandle handle, int32_t* errCode);

/**
 *  @b Description
 *  @n
 *      This is the profile registered function which is used to deinitialize the profile
 *      and this is invoked during the TestFmk Deinitialization process.
 *
 *  @param[in]  handle
 *      Profile handle
 *  @param[out] errCode
 *      Error code populated by the API
 *
 *  \ingroup TEST_FRAMEWORK_PROFILE_FUNCTIONS
 *
 *  @retval
 *      Success - 0
 *  @retval
 *      Error   - <0
 */
typedef int32_t (*TestFmk_ProfileDeinitFxn) (TestFmk_ProfileHandle handle, int32_t* errCode);

/**
 * @brief
 *  Profile configuration
 *
 * @details
 *  The TestFmk module is responsible for the registeration and execution of
 *  profiles. Profiles can be created and registered by populating the
 *  following configuration:
 */
typedef struct TestFmk_ProfileCfg_t
{
    /**
     * @brief   Initialization function:
     */
    TestFmk_ProfileInitFxn                      initFxn;

    /**
     * @brief   Open function:
     */
    TestFmk_ProfileOpenFxn                      openFxn;

    /**
     * @brief   Finalize configure function:
     */
    TestFmk_ProfileFinalizeCfgFxn               finalizeCfgFxn;

    /**
     * @brief   Start function:
     */
    TestFmk_ProfileStartFxn                     startFxn;

    /**
     * @brief   Execute function:
     */
    TestFmk_ProfileExecuteFxn                   executeFxn;

    /**
     * @brief   Control function:
     */
    TestFmk_ProfileControlFxn                   controlFxn;

    /**
     * @brief   Stop function:
     */
    TestFmk_ProfileStopFxn                      stopFxn;

    /**
     * @brief   Close function:
     */
    TestFmk_ProfileCloseFxn                     closeFxn;

    /**
     * @brief   Deinitialization function:
     */
    TestFmk_ProfileDeinitFxn                    deinitFxn;

    /**
     * @brief   [Optional] Chirp Available Callback function:
     */
    TestFmk_ProfileChirpAvailableCallbackFxn    chirpAvailableFxn;

    /**
     * @brief   [Optional] Frame Start Callback function:
     */
    TestFmk_ProfileFrameStartCallbackFxn        frameStartFxn;
}TestFmk_ProfileCfg;

/**
@}
*/

/***********************************************************************************************
 * TestFmk Exported API:
 ***********************************************************************************************/
extern TestFmk_Handle TestFmk_init (TestFmk_InitCfg* ptrInitCfg, int32_t* errCode);
extern int32_t TestFmk_synch (TestFmk_Handle fmkHandle, int32_t* errCode);
extern int32_t TestFmk_loadProfile (TestFmk_Handle fmkHandle, TestFmk_ProfileCfg* ptrProfileCfg, int32_t* errCode);
extern int32_t TestFmk_open (TestFmk_Handle handle, MMWave_OpenCfg* ptrOpenCfg, int32_t* errCode);
extern int32_t TestFmk_ioctl (TestFmk_Handle handle, uint32_t cmd, void* arg, uint32_t argLen, int32_t* errCode);
extern int32_t TestFmk_start (TestFmk_Handle handle, int32_t* errCode);
extern int32_t TestFmk_stop (TestFmk_Handle handle, int32_t* errCode);
extern int32_t TestFmk_execute (TestFmk_Handle handle, TestFmk_Result* ptrResult, int32_t* errCode);
extern int32_t TestFmk_close (TestFmk_Handle fmkHandle, int32_t* errCode);
extern int32_t TestFmk_unloadProfile (TestFmk_Handle handle, int32_t* errCode);
extern int32_t TestFmk_deinit (TestFmk_Handle fmkHandle, int32_t* errCode);

#ifdef __cplusplus
}
#endif

#endif /* TEST_FRAMEWORK_H */


