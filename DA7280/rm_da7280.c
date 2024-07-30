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

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "rm_da7280.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/* Definitions of Open flag */
#define RM_DA7280_OPEN                               (0x433E4432UL) // Open state

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Global function prototypes
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
extern fsp_err_t rm_da7280_delay_us(rm_da7280_instance_ctrl_t * const p_ctrl, uint32_t const delay_us);
rm_da7280_instance_ctrl_t g_da7280_sensor0_ctrl;
const rm_da7280_cfg_t g_da7280_sensor0_cfg =
{ .p_instance = &g_comms_i2c_device0, .p_callback = da7280_callback,
#if defined(NULL)
    .p_context           = NULL,
#else
  .p_context = &NULL,
#endif
        };

const rm_da7280_instance_t g_da7280_sensor0 =
{ .p_ctrl = &g_da7280_sensor0_ctrl, .p_cfg = &g_da7280_sensor0_cfg, .p_api = &g_da7280_on_da7280, };


/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * Global variables
 **********************************************************************************************************************/
rm_da7280_api_t const g_da7280_on_da7280 =
{
    .open                 = RM_DA7280_Open,
    .close                = RM_DA7280_Close,
    .setFreqTracking      = RM_DA7280_SetFrequencyTracking,
    .setMode              = RM_DA7280_SetMode,
    .setGPIO              = RM_DA7280_SetGPIOModePol,
    .setWaveform          = RM_DA7280_WriteWaveform,
    .playFromMemory       = RM_DA7280_PlayFromMemory,
};

/*******************************************************************************************************************//**
 * @addtogroup RM_DA7280
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief Opens and configures the DA7280 Middle module. Implements @ref rm_da7280_api_t::open.
 *
 * Example:
 * @snippet rm_da7280_example.c RM_DA7280_Open
 *
 * @retval FSP_SUCCESS              DA7280 successfully configured.
 * @retval FSP_ERR_ASSERTION        Null pointer, or one or more configuration options is invalid.
 * @retval FSP_ERR_ALREADY_OPEN     Module is already open.  This module can only be opened once.
 **********************************************************************************************************************/
fsp_err_t RM_DA7280_Open (rm_da7280_ctrl_t * const p_api_ctrl, rm_da7280_cfg_t const * const p_cfg)
{
    rm_comms_write_read_params_t readWriteID;
    uint8_t transfer_data[3];
    uint8_t read_val[3] = {0};


    fsp_err_t err = FSP_SUCCESS;
    rm_da7280_instance_ctrl_t * p_ctrl = (rm_da7280_instance_ctrl_t *) p_api_ctrl;

#if RM_DA7280_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ASSERT(NULL != p_cfg);
    FSP_ASSERT(NULL != p_cfg->p_instance);
    FSP_ERROR_RETURN(RM_DA7280_OPEN != p_ctrl->open, FSP_ERR_ALREADY_OPEN);
#endif

    p_ctrl->p_cfg                  = p_cfg;
    p_ctrl->p_comms_i2c_instance   = p_cfg->p_instance;
    p_ctrl->p_context              = p_cfg->p_context;
    p_ctrl->p_callback             = p_cfg->p_callback;

    /* Open Communications middleware */
    err = p_ctrl->p_comms_i2c_instance->p_api->open(p_ctrl->p_comms_i2c_instance->p_ctrl,
                                                    p_ctrl->p_comms_i2c_instance->p_cfg);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    /* Set open flag */
    p_ctrl->open = RM_DA7280_OPEN;

    /* Read Device ID (0xBA) to confirm I2C operations */
    transfer_data[0] = DA7280_CHIP_REV;

    readWriteID.p_src = transfer_data;
    readWriteID.src_bytes = 1;
    readWriteID.p_dest = read_val;
    readWriteID.dest_bytes = 1;

    err = p_ctrl->p_comms_i2c_instance->p_api->writeRead(p_ctrl->p_comms_i2c_instance->p_ctrl,
                                                    readWriteID);
    R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MILLISECONDS);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);


    if (0xBA != read_val[0])
    {
        return FSP_ERR_ASSERTION;
    }

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Disables specified DA7280 control block. Implements @ref rm_da7280_api_t::close.
 *
 * @retval FSP_SUCCESS              Successfully closed.
 * @retval FSP_ERR_ASSERTION        Null pointer passed as a parameter.
 * @retval FSP_ERR_NOT_OPEN         Module is not open.
 **********************************************************************************************************************/
fsp_err_t RM_DA7280_Close (rm_da7280_ctrl_t * const p_api_ctrl)
{
    rm_da7280_instance_ctrl_t * p_ctrl = (rm_da7280_instance_ctrl_t *) p_api_ctrl;

#if RM_DA7280_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ERROR_RETURN(RM_DA7280_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* Close Communications Middleware */
    p_ctrl->p_comms_i2c_instance->p_api->close(p_ctrl->p_comms_i2c_instance->p_ctrl);

    /* Clear Open flag */
    p_ctrl->open = 0;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Enables or disables DA7280 Frequency Tracking.
 *
 * @retval FSP_SUCCESS              Successfully closed.
 * @retval FSP_ERR_ASSERTION        Null pointer passed as a parameter.
 * @retval FSP_ERR_NOT_OPEN         Module is not open.
 **********************************************************************************************************************/
fsp_err_t RM_DA7280_SetFrequencyTracking (rm_da7280_ctrl_t * const p_api_ctrl, bool enable)
{

    rm_comms_write_read_params_t setFreqTrack;
    uint8_t transfer_data[3] = {0};
    uint8_t read_val[3] = {0};
    uint8_t freqTrack;

    fsp_err_t err = FSP_SUCCESS;
    rm_da7280_instance_ctrl_t * p_ctrl = (rm_da7280_instance_ctrl_t *) p_api_ctrl;

#if RM_DA7280_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ERROR_RETURN(RM_DA7280_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* Read the current value of the config register */
    transfer_data[0] = DA7280_TOP_CFG1;

    setFreqTrack.p_src = transfer_data;
    setFreqTrack.src_bytes = 1;
    setFreqTrack.p_dest = read_val;
    setFreqTrack.dest_bytes = 1;


    err = p_ctrl->p_comms_i2c_instance->p_api->writeRead(p_ctrl->p_comms_i2c_instance->p_ctrl,
                                                         setFreqTrack);
    R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MILLISECONDS);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    /* Mask the Frequency Tracking bit and then set to 1 or 0 */
    freqTrack = read_val[0];
    //printf("Value of register 0x%02x is 0x%02x\n", transfer_data[0], read_val[0]);

    transfer_data[1] = (uint8_t)(freqTrack & ~DA7280_FREQ_TRACK_EN_MASK);
    transfer_data[1] = (uint8_t)(transfer_data[1] | ((enable ? 1:0) << DA7280_FREQ_TRACK_EN_SHIFT));

    /* Write the final value */
    err = p_ctrl->p_comms_i2c_instance->p_api->write(p_ctrl->p_comms_i2c_instance->p_ctrl,
                                                         transfer_data, 2);
    R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MILLISECONDS);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    read_val[0] = 0;

    err = p_ctrl->p_comms_i2c_instance->p_api->writeRead(p_ctrl->p_comms_i2c_instance->p_ctrl,
                                                         setFreqTrack);
    R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MILLISECONDS);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    //printf("Value of register 0x%02x after write is 0x%02x\n", transfer_data[0], read_val[0]);


    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Sets the operating mode of the DA7280 (DRO, PWM, RTWM, or ETWM)
 *
 * @retval FSP_SUCCESS              Successfully closed.
 * @retval FSP_ERR_ASSERTION        Null pointer passed as a parameter.
 * @retval FSP_ERR_NOT_OPEN         Module is not open.
 **********************************************************************************************************************/
fsp_err_t RM_DA7280_SetMode (rm_da7280_ctrl_t * const p_api_ctrl, uint8_t mode)
{
    rm_comms_write_read_params_t setMode;
    uint8_t transfer_data[3] = {0};
    uint8_t read_val[3] = {0};
    uint8_t modeSet;

    fsp_err_t err = FSP_SUCCESS;
    rm_da7280_instance_ctrl_t * p_ctrl = (rm_da7280_instance_ctrl_t *) p_api_ctrl;

#if RM_DA7280_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ERROR_RETURN(RM_DA7280_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* Read the current value of the control register */
    transfer_data[0] = DA7280_TOP_CTL1;

    setMode.p_src = transfer_data;
    setMode.src_bytes = 1;
    setMode.p_dest = read_val;
    setMode.dest_bytes = 1;


    err = p_ctrl->p_comms_i2c_instance->p_api->writeRead(p_ctrl->p_comms_i2c_instance->p_ctrl,
                                                         setMode);
    R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MILLISECONDS);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    modeSet = read_val[0];
    //printf("Value of register 0x%02x is 0x%02x\n", transfer_data[0], modeSet);
    // TODO: do other modes (PWM, RTWM, DRO) require extra settings before updating mode bits?
//    switch(mode)
//    {
//        case (DA7280_ETWM_MODE):
//        {
//            transfer_data[1] = (uint8_t)(freqTrack & ~DA7280_OPERATION_MODE_MASK);
//            transfer_data[1] = (uint8_t)(transfer_data[1] | (mode << DA7280_OPERATION_MODE_SHIFT));
//        } break;
//        default: return FSP_ERR_ASSERTION;
//    }

    transfer_data[1] = (uint8_t)(modeSet & ~DA7280_OPERATION_MODE_MASK);
    transfer_data[1] = (uint8_t)(transfer_data[1] | (mode << DA7280_OPERATION_MODE_SHIFT));

    /* Write the final value */
    err = p_ctrl->p_comms_i2c_instance->p_api->write(p_ctrl->p_comms_i2c_instance->p_ctrl,
                                                         transfer_data, 2);
    R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MILLISECONDS);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    read_val[0] = 0;

    err = p_ctrl->p_comms_i2c_instance->p_api->writeRead(p_ctrl->p_comms_i2c_instance->p_ctrl,
                                                         setMode);
    R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MILLISECONDS);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
    //printf("Value of register 0x%02x after write is 0x%02x\n", transfer_data[0], read_val[0]);

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Sets DA7280 GPIO mode and polarity.
 *
 * @retval FSP_SUCCESS              Successfully closed.
 * @retval FSP_ERR_ASSERTION        Null pointer passed as a parameter.
 * @retval FSP_ERR_NOT_OPEN         Module is not open.
 **********************************************************************************************************************/
fsp_err_t RM_DA7280_SetGPIOModePol (rm_da7280_ctrl_t * const p_api_ctrl, uint8_t pin, uint8_t sequence, uint8_t mode, uint8_t polarity)
{

    rm_comms_write_read_params_t setGPIOMode;
    uint8_t transfer_data[3] = {0};
    uint8_t read_val[3] = {0};
    uint8_t setMode;

    fsp_err_t err = FSP_SUCCESS;
    rm_da7280_instance_ctrl_t * p_ctrl = (rm_da7280_instance_ctrl_t *) p_api_ctrl;

#if RM_DA7280_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ERROR_RETURN(RM_DA7280_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* Read the current value of the config register */
    transfer_data[0] = (uint8_t)(DA7280_GPI_0_CTL + pin);

    setGPIOMode.p_src = transfer_data;
    setGPIOMode.src_bytes = 1;
    setGPIOMode.p_dest = read_val;
    setGPIOMode.dest_bytes = 1;


    err = p_ctrl->p_comms_i2c_instance->p_api->writeRead(p_ctrl->p_comms_i2c_instance->p_ctrl,
                                                         setGPIOMode);
    R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MILLISECONDS);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    /* Mask the Frequency Tracking bit and then set to 1 or 0 */ // sequence ID is set by bits 3 to 6
    setMode = read_val[0];

    transfer_data[1] = (uint8_t)(setMode & ~DA7280_GPI0_POLARITY_MASK);
    transfer_data[1] = (uint8_t)(transfer_data[1] | (polarity << DA7280_GPI0_POLARITY_SHIFT));

    transfer_data[1] = (uint8_t)(transfer_data[1] & ~DA7280_GPI0_MODE_MASK);
    transfer_data[1] = (uint8_t)(transfer_data[1] | (mode << DA7280_GPI0_MODE_SHIFT));

    transfer_data[1] = (uint8_t)(transfer_data[1] & ~DA7280_GPI0_SEQUENCE_ID_MASK);
    transfer_data[1] = (uint8_t)(transfer_data[1] | sequence << DA7280_GPI0_SEQUENCE_ID_SHIFT);

    /* Write the final value */
    err = p_ctrl->p_comms_i2c_instance->p_api->write(p_ctrl->p_comms_i2c_instance->p_ctrl,
                                                         transfer_data, 2);
    R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MILLISECONDS);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Writes a new waveform memory pattern to the DA7280
 *
 * @retval FSP_SUCCESS              Successfully closed.
 * @retval FSP_ERR_ASSERTION        Null pointer passed as a parameter.
 * @retval FSP_ERR_NOT_OPEN         Module is not open.
 **********************************************************************************************************************/
fsp_err_t RM_DA7280_WriteWaveform (rm_da7280_ctrl_t * const p_api_ctrl, uint8_t* snp_mem)
{
    uint8_t transfer_data[3] = {0};

    fsp_err_t err = FSP_SUCCESS;
    rm_da7280_instance_ctrl_t * p_ctrl = (rm_da7280_instance_ctrl_t *) p_api_ctrl;

#if RM_DA7280_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ERROR_RETURN(RM_DA7280_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* Write the waveform memory in sequence to the memory register */
    for (uint8_t i = 0; i < 100; i++)
    {
        transfer_data[0] = (uint8_t)(DA7280_SNP_MEM_0 + i);
        transfer_data[1] = snp_mem[i];
        err = p_ctrl->p_comms_i2c_instance->p_api->write(p_ctrl->p_comms_i2c_instance->p_ctrl,
                                                             transfer_data, 2);
        R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MILLISECONDS);
    }
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief  Enables the DA7280 to play it's waveform
 *
 * @retval FSP_SUCCESS              Successfully closed.
 * @retval FSP_ERR_ASSERTION        Null pointer passed as a parameter.
 * @retval FSP_ERR_NOT_OPEN         Module is not open.
 **********************************************************************************************************************/
fsp_err_t RM_DA7280_PlayFromMemory (rm_da7280_ctrl_t * const p_api_ctrl)
{
    rm_comms_write_read_params_t playMemory;
    uint8_t transfer_data[3] = {0};
    uint8_t read_val[3] = {0};
    uint8_t playMem;

    fsp_err_t err = FSP_SUCCESS;
    rm_da7280_instance_ctrl_t * p_ctrl = (rm_da7280_instance_ctrl_t *) p_api_ctrl;

#if RM_DA7280_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ERROR_RETURN(RM_DA7280_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* Read the current value of the control register */
    transfer_data[0] = DA7280_TOP_CTL1;

    playMemory.p_src = transfer_data;
    playMemory.src_bytes = 1;
    playMemory.p_dest = read_val;
    playMemory.dest_bytes = 1;


    err = p_ctrl->p_comms_i2c_instance->p_api->writeRead(p_ctrl->p_comms_i2c_instance->p_ctrl,
                                                         playMemory);
    R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MILLISECONDS);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    playMem = read_val[0];

    transfer_data[1] = (uint8_t)(playMem & ~DA7280_SEQ_START_MASK);
    transfer_data[1] = (uint8_t)(transfer_data[1] | DA7280_SEQ_START_MASK);

    /* Write the final value */
    err = p_ctrl->p_comms_i2c_instance->p_api->write(p_ctrl->p_comms_i2c_instance->p_ctrl,
                                                         transfer_data, 2);
    R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MILLISECONDS);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    return FSP_SUCCESS;
}


/*******************************************************************************************************************//**
 * @brief DA7280 callback function called in the I2C Communications Middleware callback function.
 **********************************************************************************************************************/
void rm_da7280_callback (rm_comms_callback_args_t * p_args)
{
    rm_da7280_instance_ctrl_t * p_ctrl = (rm_da7280_instance_ctrl_t *) p_args->p_context;
    rm_da7280_callback_args_t   da7280_callback_args;

    /* Set context */
    da7280_callback_args.p_context = p_ctrl->p_context;

    /* Set event */
    switch (p_args->event)
    {
        case RM_COMMS_EVENT_OPERATION_COMPLETE:
        {
            da7280_callback_args.event = RM_DA7280_EVENT_SUCCESS;
            break;
        }

        case RM_COMMS_EVENT_ERROR:
        default:
        {
            da7280_callback_args.event = RM_DA7280_EVENT_ERROR;
            break;
        }
    }

    if (true == p_ctrl->programming_mode.blocking)
    {
        /* Set flag */
        p_ctrl->programming_mode.communication_finished = true;

        /* Set event */
        p_ctrl->programming_mode.event = da7280_callback_args.event;
    }
    else
    {
        if (NULL != p_ctrl->p_callback)
        {
            /* Call callback function */
            p_ctrl->p_callback(&da7280_callback_args);
        }
    }
}


/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Internal da7280 private function.
 **********************************************************************************************************************/
