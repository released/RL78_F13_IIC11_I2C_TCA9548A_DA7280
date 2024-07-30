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
 * @addtogroup RM_DA7280
 * @{
 **********************************************************************************************************************/

#ifndef RM_DA7280_H
#define RM_DA7280_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
// #include "rm_da7280_api.h"
#include <stdio.h>

#define DA7280_CHIP_REV                       0x0000
#define DA7280_IRQ_EVENT1                     0x0003
#define DA7280_IRQ_EVENT_WARNING_DIAG         0x0004
#define DA7280_IRQ_EVENT_PAT_DIAG             0x0005
#define DA7280_IRQ_STATUS1                    0x0006
#define DA7280_IRQ_MASK1                      0x0007
#define DA7280_CIF_I2C1                       0x0008
#define DA7280_FRQ_LRA_PER_H                  0x000A
#define DA7280_FRQ_LRA_PER_L                  0x000B
#define DA7280_ACTUATOR1                      0x000C
#define DA7280_ACTUATOR2                      0x000D
#define DA7280_ACTUATOR3                      0x000E
#define DA7280_CALIB_V2I_H                    0x000F
#define DA7280_CALIB_V2I_L                    0x0010
#define DA7280_CALIB_IMP_H                    0x0011
#define DA7280_CALIB_IMP_L                    0x0012
#define DA7280_TOP_CFG1                       0x0013
#define DA7280_TOP_CFG2                       0x0014
#define DA7280_TOP_CFG3                       0x0015
#define DA7280_TOP_CFG4                       0x0016
#define DA7280_TOP_INT_CFG1                   0x0017
#define DA7280_TOP_INT_CFG6_H                 0x001C
#define DA7280_TOP_INT_CFG6_L                 0x001D
#define DA7280_TOP_INT_CFG7_H                 0x001E
#define DA7280_TOP_INT_CFG7_L                 0x001F
#define DA7280_TOP_INT_CFG8                   0x0020
#define DA7280_TOP_CTL1                       0x0022
#define DA7280_TOP_CTL2                       0x0023
#define DA7280_SEQ_CTL1                       0x0024
#define DA7280_SWG_C1                         0x0025
#define DA7280_SWG_C2                         0x0026
#define DA7280_SWG_C3                         0x0027
#define DA7280_SEQ_CTL2                       0x0028
#define DA7280_GPI_0_CTL                      0x0029
#define DA7280_GPI_1_CTL                      0x002A
#define DA7280_GPI_2_CTL                      0x002B
#define DA7280_MEM_CTL1                       0x002C
#define DA7280_MEM_CTL2                       0x002D
#define DA7280_ADC_DATA_H1                    0x002E
#define DA7280_ADC_DATA_L1                    0x002F
#define DA7280_POLARITY                       0x0043
#define DA7280_LRA_AVR_H                      0x0044
#define DA7280_LRA_AVR_L                      0x0045
#define DA7280_FRQ_LRA_PER_ACT_H              0x0046
#define DA7280_FRQ_LRA_PER_ACT_L              0x0047
#define DA7280_FRQ_PHASE_H                    0x0048
#define DA7280_FRQ_PHASE_L                    0x0049
#define DA7280_FRQ_CTL                        0x004C
#define DA7280_TRIM3                          0x005F
#define DA7280_TRIM4                          0x0060
#define DA7280_TRIM6                          0x0062
#define DA7280_IRQ_EVENT_ACTUATOR_FAULT       0x0081
#define DA7280_IRQ_STATUS2                    0x0082
#define DA7280_IRQ_MASK2                      0x0083
#define DA7280_SNP_MEM_0                      0x0084

/* DA7280_TOP_CFG1 (Address 0x0013) */
#define DA7280_AMP_PID_EN_SHIFT                 0
#define DA7280_AMP_PID_EN_MASK                  (1 << 0)
#define DA7280_RAPID_STOP_EN_SHIFT              1
#define DA7280_RAPID_STOP_EN_MASK               (1 << 1)
#define DA7280_ACCELERATION_EN_SHIFT            2
#define DA7280_ACCELERATION_EN_MASK             (1 << 2)
#define DA7280_FREQ_TRACK_EN_SHIFT              3
#define DA7280_FREQ_TRACK_EN_MASK               (1 << 3)
#define DA7280_BEMF_SENSE_EN_SHIFT              4
#define DA7280_BEMF_SENSE_EN_MASK               (1 << 4)
#define DA7280_ACTUATOR_TYPE_SHIFT              5
#define DA7280_ACTUATOR_TYPE_MASK               (1 << 5)
#define DA7280_EMBEDDED_MODE_SHIFT              7
#define DA7280_EMBEDDED_MODE_MASK               (1 << 7)

/* DA7280_TOP_CTL1 (Address 0x0022) */
#define DA7280_OPERATION_MODE_SHIFT             0
#define DA7280_OPERATION_MODE_MASK              (7 << 0)
#define DA7280_STANDBY_EN_SHIFT                 3
#define DA7280_STANDBY_EN_MASK                  (1 << 3)
#define DA7280_SEQ_START_SHIFT                  4
#define DA7280_SEQ_START_MASK                   (1 << 4)

/* DA7280_GPIO_0_CTL (Address 0x0029) */
#define DA7280_GPI0_POLARITY_SHIFT              0
#define DA7280_GPI0_POLARITY_MASK               (3 << 0)
#define DA7280_GPI0_MODE_SHIFT                  2
#define DA7280_GPI0_MODE_MASK                   (1 << 2)
#define DA7280_GPI0_SEQUENCE_ID_SHIFT           3
#define DA7280_GPI0_SEQUENCE_ID_MASK            (15 << 3)


/* DA7280_SEQ_CTL1 (Address 0x0024) */ 
#define DA7280_FREQ_WAVEFORM_TIMEBASE_EN_SHIFT      2
#define DA7280_FREQ_WAVEFORM_TIMEBASE_EN_MA         (1 << 2)
#define DA7280_WAVEGEN_MODE_SHIFT                   1
#define DA7280_WAVEGEN_MODE_MASK                    (1 << 1)
#define DA7280_SEQ_CONTINUE_SHIFT                   0
#define DA7280_SEQ_CONTINUE_MASK                    (1 << 0)

/* DA7280_SEQ_CTL2 (Address 0x0028) */
#define DA7280_PS_SEQ_ID_SHIFT                      0
#define DA7280_PS_SEQ_ID_MASK                       (1 << 0)

#define DA7280_PS_SEQ_LOOP_SHIFT                    4
#define DA7280_PS_SEQ_LOOP_MASK                     (1 << 4)

/* DA7280_IRQ_MASK1 (Address 0x0007) */
#define DA7280_SEQ_CONTINUE_M_SHIFT                 0
#define DA7280_SEQ_CONTINUE_M_MASK                  (1 << 0)
#define DA7280_E_UVLO_M_SHIFT                       1
#define DA7280_E_UVLO_M_MASK                        (1 << 1)
#define DA7280_SEQ_DONE_M_SHIFT                     2
#define DA7280_SEQ_DONE_M_MASK                      (1 << 2)
#define DA7280_OVERTEMP_CRIT_M_SHIFT                3
#define DA7280_OVERTEMP_CRIT_M_MASK                 (1 << 3)
#define DA7280_SEQ_FAULT_M_SHIFT                    4
#define DA7280_SEQ_FAULT_M_MASK                     (1 << 4)
#define DA7280_WARNING_M_SHIFT                      5
#define DA7280_WARNING_M_MASK                       (1 << 5)
#define DA7280_ACTUATOR_M_SHIFT                     6
#define DA7280_ACTUATOR_M_MASK                      (1 << 6)
#define DA7280_OC_M_SHIFT                           7
#define DA7280_OC_M_MASK                            (1 << 7)


/* DA7280_IRQ_EVENT1 (Address 0x0003) */
#define DA7280_E_SEQ_CONTINUE_SHIFT                 0
#define DA7280_E_SEQ_CONTINUE_MASK                  (1 << 0)
#define DA7280_E_UVLO_SHIFT                         1
#define DA7280_E_UVLO_MASK                          (1 << 1)
#define DA7280_E_SEQ_DONE_SHIFT                     2
#define DA7280_E_SEQ_DONE_MASK                      (1 << 2)
#define DA7280_E_OVERTEMP_CRIT_SHIFT                3
#define DA7280_E_OVERTEMP_CRIT_MASK                 (1 << 3)
#define DA7280_E_SEQ_FAULT_SHIFT                    4
#define DA7280_E_SEQ_FAULT_MASK                     (1 << 4)
#define DA7280_E_WARNING_SHIFT                      5
#define DA7280_E_WARNING_MASK                       (1 << 5)
#define DA7280_E_ACTUATOR_FAULT_SHIFT               6
#define DA7280_E_ACTUATOR_FAULT_MASK                (1 << 6)
#define DA7280_E_OC_FAULT_SHIFT                     7
#define DA7280_E_OC_FAULT_MASK                      (1 << 7)

#define HAPTIC_SUCCESS                              (0x00)
#define E_SEQ_CONTINUE                              (0x01)
#define E_UVLO                                      (0x02)
#define E_SEQ_DONE                                  (0x04)
#define E_OVERTEMP_CRIT                             (0x08)
#define E_SEQ_FAULT                                 (0x10)
#define E_WARNING                                   (0x20)
#define E_ACTUATOR_FAULT                            (0x40)
#define E_OC_FAULT                                  (0x80)


/* DA7280_IRQ_EVENT_PAT_DIAG (Address 0x0005) */
#define DA7280_E_PWM_FAULT_SHIFT                    5
#define DA7280_E_PWM_FAULT_MASK                     (1 << 5)
#define DA7280_E_MEM_FAULT_SHIFT                    6
#define DA7280_E_MEM_FAULT_MASK                     (1 << 6)
#define DA7280_E_SEQ_ID_FAULT_SHIFT                 7
#define DA7280_E_SEQ_ID_FAULT_MASK                  (1 << 7)

/* DA7280_IRQ_EVENT_WARNING_DIAG (Address 0x0004) */
#define DA7280_E_OVERTEMP_WARN_SHIFT                3
#define DA7280_E_OVERTEMP_WARN_MASK                 (1 << 3)
#define DA7280_E_MEM_TYPE_SHIFT                     4
#define DA7280_E_MEM_TYPE_MASK                      (1 << 4)
#define DA7280_E_LIM_DRIVE_ACC_SHIFT                6
#define DA7280_E_LIM_DRIVE_ACC_MASK                 (1 << 6)
#define DA7280_E_LIM_DRIVE_SHIFT                    7
#define DA7280_E_LIM_DRIVE_MASK                     (1 << 7)


#define SEQUENCE_IDX0                               (0x00)
#define SEQUENCE_IDX1                               (0x01)
#define SEQUENCE_IDX2                               (0x02)
#define SEQUENCE_IDX3                               (0x03)
#define SEQUENCE_IDX4                               (0x04)
#define SEQUENCE_IDX5                               (0x05)
#define SEQUENCE_IDX6                               (0x06)
#define SEQUENCE_IDX7                               (0x07)

#define SEQUENCE_IDX8                               (0x08)
#define SEQUENCE_IDX9                               (0x09)
#define SEQUENCE_IDX10                              (0x0A)
#define SEQUENCE_IDX11                              (0x0B)
#define SEQUENCE_IDX12                              (0x0C)
#define SEQUENCE_IDX13                              (0x0D)
#define SEQUENCE_IDX14                              (0x0E)
#define SEQUENCE_IDX15                              (0x0F)

enum da7280_op_mode {
    DA7280_INACTIVE_MODE = 0,
    DA7280_DRO_MODE = 1,
    DA7280_PWM_MODE = 2,
    DA7280_RTWM_MODE    = 3,
    DA7280_ETWM_MODE    = 4,
    DA7280_MODE_MAX,
};

 enum da7280_gpi_mod {
     DA7280_SINGLE_PTN   = 0,
     DA7280_MULTI_PTN    = 1,
     DA7280_GPI_MOD_MAX,
 };

 enum da7280_sequence_id {
     SEQUENCE_SHORT         = 1,
     SEQUENCE_MEDIUM        = 2,
     SEQUENCE_LONG          = 3,
     SEQUENCE_VERY_LONG     = 4,
     SEQUENCE_SHORT_LONG    = 5,
     SEQUENCE_PHONE_RING    = 6,
     SEQUENCE_QUIET         = 7,
     SEQUENCE_RAMP_UP       = 8,
     SEQUENCE_ALARM_CLOCK,
 };

 enum da7280_gpi_pol {
     DA7280_RISING_EDGE      = 0,
     DA7280_FALLING_EDGE = 1,
     DA7280_BOTH_EDGE        = 2,
     DA7280_GPI_POL_MAX,
 };


// #if defined(__CCRX__) || defined(__ICCRX__) || defined(__RX__)
//  #include "r_da7280_rx_config.h"
// #elif defined(__CCRL__) || defined(__ICCRL__) || defined(__RL78__)
//  #include "r_da7280_rl_config.h"
// #else

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
// FSP_HEADER
// #endif

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** DA7280 programming mode process block */
// typedef struct st_rm_da7280_programmnig_mode_params
// {
//     volatile bool              enter;                  ///< Enter flag.
//     volatile bool              blocking;               ///< Blocking flag.
//     volatile bool              communication_finished; ///< Communication flag for blocking.
//     volatile rm_da7280_event_t event;                  ///< Callback event
// } rm_da7280_programmnig_mode_params_t;


/** DA7280 Control Block */
// typedef struct rm_da7280_instance_ctrl
// {
//     uint32_t                            open;                 ///< Open flag
//     rm_da7280_cfg_t const             * p_cfg;                ///< Pointer to DA7280 Configuration
//     rm_comms_instance_t const         * p_comms_i2c_instance; ///< Pointer of I2C Communications Middleware instance structure
//     void const                        * p_context;            ///< Pointer to the user-provided context
//     rm_da7280_programmnig_mode_params_t programming_mode;     ///< Programming mode flag

//     /* Pointer to callback and optional working memory */
//     void (* p_callback)(rm_da7280_callback_args_t * p_args);
// } rm_da7280_instance_ctrl_t;

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/

/** @cond INC_HEADER_DEFS_SEC */
/** Filled in Interface API structure for this Instance. */
// extern rm_da7280_api_t const g_da7280_on_da7280;

/** @endcond */
// extern const rm_da7280_instance_t g_da7280_sensor0;
// extern rm_da7280_instance_ctrl_t g_da7280_sensor0_ctrl;
// extern const rm_da7280_cfg_t g_da7280_sensor0_cfg;
// #ifndef da7280_callback
// void da7280_callback(rm_da7280_callback_args_t *p_args);
// #endif

/**********************************************************************************************************************
 * Public Function Prototypes
 **********************************************************************************************************************/
// fsp_err_t RM_DA7280_Open(rm_da7280_ctrl_t * const p_api_ctrl, rm_da7280_cfg_t const * const p_cfg);
// fsp_err_t RM_DA7280_Close(rm_da7280_ctrl_t * const p_api_ctrl);
// fsp_err_t RM_DA7280_SetFrequencyTracking(rm_da7280_ctrl_t * const p_api_ctrl, bool enable);
// fsp_err_t RM_DA7280_SetMode (rm_da7280_ctrl_t * const p_api_ctrl, uint8_t mode);
// fsp_err_t RM_DA7280_SetGPIOModePol (rm_da7280_ctrl_t * const p_api_ctrl, uint8_t sequence, uint8_t pin, uint8_t mode, uint8_t polarity);
// fsp_err_t RM_DA7280_WriteWaveform (rm_da7280_ctrl_t * const p_api_ctrl, uint8_t* snp_mem);
// fsp_err_t RM_DA7280_PlayFromMemory (rm_da7280_ctrl_t * const p_api_ctrl);

#if defined(__CCRX__) || defined(__ICCRX__) || defined(__RX__)
#elif defined(__CCRL__) || defined(__ICCRL__) || defined(__RL78__)
#else

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_FOOTER
#endif

#endif                                 /* RM_DA7280_H_*/

/*******************************************************************************************************************//**
 * @} (end addtogroup RM_DA7280)
 **********************************************************************************************************************/
