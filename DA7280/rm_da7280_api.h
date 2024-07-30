/***********************************************************************************************************************
 * Copyright [2020-2021] Renesas Electronics Corporation and/or its affiliates.  All Rights Reserved.
 *
 * This software and documentation are supplied by Renesas Electronics America Inc. and may only be used with products
 * of Renesas Electronics Corp. and its affiliates ("Renesas").  No other uses are authorized.  Renesas products are
 * sold pursuant to Renesas terms and conditions of sale.  Purchasers are solely responsible for the selection and use
 * of Renesas products and Renesas assumes no liability.  No license, express or implied, to any intellectual property
 * right is granted by Renesas. This software is protected under all applicable laws, including copyright laws. Renesas
 * reserves the right to change or discontinue this software and/or this documentation. THE SOFTWARE AND DOCUMENTATION
 * IS DELIVERED TO YOU "AS IS," AND RENESAS MAKES NO REPRESENTATIONS OR WARRANTIES, AND TO THE FULLEST EXTENT
 * PERMISSIBLE UNDER APPLICABLE LAW, DISCLAIMS ALL WARRANTIES, WHETHER EXPLICITLY OR IMPLICITLY, INCLUDING WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND NONINFRINGEMENT, WITH RESPECT TO THE SOFTWARE OR
 * DOCUMENTATION.  RENESAS SHALL HAVE NO LIABILITY ARISING OUT OF ANY SECURITY VULNERABILITY OR BREACH.  TO THE MAXIMUM
 * EXTENT PERMITTED BY LAW, IN NO EVENT WILL RENESAS BE LIABLE TO YOU IN CONNECTION WITH THE SOFTWARE OR DOCUMENTATION
 * (OR ANY PERSON OR ENTITY CLAIMING RIGHTS DERIVED FROM YOU) FOR ANY LOSS, DAMAGES, OR CLAIMS WHATSOEVER, INCLUDING,
 * WITHOUT LIMITATION, ANY DIRECT, CONSEQUENTIAL, SPECIAL, INDIRECT, PUNITIVE, OR INCIDENTAL DAMAGES; ANY LOST PROFITS,
 * OTHER ECONOMIC DAMAGE, PROPERTY DAMAGE, OR PERSONAL INJURY; AND EVEN IF RENESAS HAS BEEN ADVISED OF THE POSSIBILITY
 * OF SUCH LOSS, DAMAGES, CLAIMS OR COSTS.
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @ingroup RENESAS_INTERFACES
 * @defgroup RM_DA7280_API DA7280 Middleware Interface
 * @brief Interface for DA7280 Middleware functions.
 *
 * @section RM_DA7280_API_Summary Summary
 * The DA7280 interface provides DA7280 functionality.
 *
 * The DA7280 interface can be implemented by:
 * - @ref RM_DA7280
 *
 * @{
 **********************************************************************************************************************/

#ifndef RM_DA7280_API_H_
#define RM_DA7280_API_H_

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#if defined(__CCRX__) || defined(__ICCRX__) || defined(__RX__)
 #include <string.h>
 #include "platform.h"
#elif defined(__CCRL__) || defined(__ICCRL__) || defined(__RL78__)
 #include <string.h>
 #include "r_cg_macrodriver.h"
 #include "r_fsp_error.h"
#else
 #include "bsp_api.h"
#endif

#include "rm_comms_api.h"
#include "hal_data_da7280.h"
#include "rm_da7280.h"

#if defined(__CCRX__) || defined(__ICCRX__) || defined(__RX__)
#elif defined(__CCRL__) || defined(__ICCRL__) || defined(__RL78__)
#else

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER
#endif

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** Event in the callback function */
typedef enum e_rm_da7280_event
{
    RM_DA7280_EVENT_SUCCESS = 0,
    RM_DA7280_EVENT_ERROR,
} rm_da7280_event_t;

/** Data type of DA7280 */
typedef enum e_rm_da7280_data_type
{
    RM_DA7280_HUMIDITY_DATA = 0,
    RM_DA7280_TEMPERATURE_DATA,
} rm_da7280_data_type_t;

/** Resolution type of DA7280 */
typedef enum e_rm_da7280_resolution
{
    RM_DA7280_RESOLUTION_8BIT  = 0x00,
    RM_DA7280_RESOLUTION_10BIT = 0x04,
    RM_DA7280_RESOLUTION_12BIT = 0x08,
    RM_DA7280_RESOLUTION_14BIT = 0x0C,
} rm_da7280_resolution_t;

/** DA7280 callback parameter definition */
typedef struct st_rm_da7280_callback_args
{
    void const      * p_context;
    rm_da7280_event_t event;
} rm_da7280_callback_args_t;

/** DA7280 Configuration */
typedef struct st_rm_da7280_cfg
{
    rm_comms_instance_t const * p_instance;                  ///< Pointer to Communications Middleware instance.
    void const                * p_context;                   ///< Pointer to the user-provided context.
    void const                * p_extend;                    ///< Pointer to extended configuration by instance of interface.
    void (* p_callback)(rm_da7280_callback_args_t * p_args); ///< Pointer to callback function.
} rm_da7280_cfg_t;

/** DA7280 control block.  Allocate an instance specific control block to pass into the DA7280 API calls.
 * @par Implemented as
 * - rm_da7280_instance_ctrl_t
 */
typedef void rm_da7280_ctrl_t;

/** DA7280 APIs */
typedef struct st_rm_da7280_api
{
    /** Open sensor.
     * @par Implemented as
     * - @ref RM_DA7280_Open()
     *
     * @param[in]  p_ctrl       Pointer to control structure.
     * @param[in]  p_cfg        Pointer to configuration structure.
     */
    fsp_err_t (* open)(rm_da7280_ctrl_t * const p_ctrl, rm_da7280_cfg_t const * const p_cfg);

    /** Close DA7280.
     * @par Implemented as
     * - @ref RM_DA7280_Close()
     *
     * @param[in]  p_ctrl       Pointer to control structure.
     */
    fsp_err_t (* close)(rm_da7280_ctrl_t * const p_ctrl);

    fsp_err_t (* setFreqTracking)(rm_da7280_ctrl_t * const p_ctrl, bool enable);

    fsp_err_t (* setMode)(rm_da7280_ctrl_t * const p_ctrl, uint8_t mode);

    fsp_err_t (* setGPIO)(rm_da7280_ctrl_t * const p_api_ctrl, uint8_t pin, uint8_t sequence, uint8_t mode, uint8_t polarity);

    fsp_err_t (* setWaveform)(rm_da7280_ctrl_t * const p_api_ctrl, uint8_t* snp_mem);

    fsp_err_t (* playFromMemory)(rm_da7280_ctrl_t * const p_api_ctrl);

} rm_da7280_api_t;

/** DA7280 instance */
typedef struct st_rm_da7280_instance
{
    rm_da7280_ctrl_t      * p_ctrl;    /**< Pointer to the control structure for this instance */
    rm_da7280_cfg_t const * p_cfg;     /**< Pointer to the configuration structure for this instance */
    rm_da7280_api_t const * p_api;     /**< Pointer to the API structure for this instance */
} rm_da7280_instance_t;

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * Public Function Prototypes
 **********************************************************************************************************************/

#if defined(__CCRX__) || defined(__ICCRX__) || defined(__RX__)
#elif defined(__CCRL__) || defined(__ICCRL__) || defined(__RL78__)
#else

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_FOOTER
#endif

/* This is a record of required structs, callbacks, etc. in case of hal_data deletion (by FSP gen.) */
/* These contents have been moved to rm_da7280.c and rm_da7280.h */
//rm_da7280_instance_ctrl_t g_da7280_sensor0_ctrl;
//const rm_da7280_cfg_t g_da7280_sensor0_cfg =
//{ .p_instance = &g_comms_i2c_device0, .p_callback = da7280_callback,
//#if defined(NULL)
//    .p_context           = NULL,
//#else
//  .p_context = &NULL,
//#endif
//        };
//
//const rm_da7280_instance_t g_da7280_sensor0 =
//{ .p_ctrl = &g_da7280_sensor0_ctrl, .p_cfg = &g_da7280_sensor0_cfg, .p_api = &g_da7280_on_da7280, };

//#ifndef rm_da7280_callback
//void rm_da7280_callback(rm_comms_callback_args_t *p_args);
//#endif
///* DA7280 Sensor */
//extern const rm_da7280_instance_t g_da7280_sensor0;
//extern rm_da7280_instance_ctrl_t g_da7280_sensor0_ctrl;
//extern const rm_da7280_cfg_t g_da7280_sensor0_cfg;
//#ifndef da7280_callback
//void da7280_callback(rm_da7280_callback_args_t *p_args);
//#endif


#endif                                 /* RM_DA7280_API_H_*/

/*******************************************************************************************************************//**
 * @} (end addtogroup RM_DA7280_API)
 **********************************************************************************************************************/
