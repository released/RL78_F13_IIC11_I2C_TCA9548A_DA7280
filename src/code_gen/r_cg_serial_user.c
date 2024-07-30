/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2012, 2021 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_serial_user.c
* Version      : CodeGenerator for RL78/F13 V2.03.07.02 [08 Nov 2021]
* Device(s)    : R5F10BMG
* Tool-Chain   : CCRL
* Description  : This file implements device driver for Serial module.
* Creation Date: 2024/7/30
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_serial.h"
/* Start user code for include. Do not edit comment generated here */
#include "custom_func.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
#pragma interrupt r_uart0_interrupt_send(vect=INTST0)
#pragma interrupt r_uart0_interrupt_receive(vect=INTSR0)
#pragma interrupt r_iic11_interrupt(vect=INTIIC11)
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
extern volatile uint8_t * gp_uart0_tx_address;         /* uart0 send buffer address */
extern volatile uint16_t  g_uart0_tx_count;            /* uart0 send data number */
extern volatile uint8_t * gp_uart0_rx_address;         /* uart0 receive buffer address */
extern volatile uint16_t  g_uart0_rx_count;            /* uart0 receive data number */
extern volatile uint16_t  g_uart0_rx_length;           /* uart0 receive data length */
extern volatile uint8_t   g_iic11_master_status_flag;  /* iic11 start flag for send address check by master mode */
extern volatile uint8_t * gp_iic11_tx_address;         /* iic11 send data pointer by master mode */
extern volatile uint16_t  g_iic11_tx_count;            /* iic11 send data size by master mode */
extern volatile uint8_t * gp_iic11_rx_address;         /* iic11 receive data pointer by master mode */
extern volatile uint16_t  g_iic11_rx_count;            /* iic11 receive data size by master mode */
extern volatile uint16_t  g_iic11_rx_length;           /* iic11 receive data length by master mode */
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_uart0_interrupt_receive
* Description  : This function is INTSR0 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void __near r_uart0_interrupt_receive(void)
{
    volatile uint8_t rx_data;
    volatile uint8_t err_type;
    
    err_type = (uint8_t)(SSR01 & 0x0007U);
    SIR01 = (uint16_t)err_type;
    
    if (err_type != 0U)
    {
        r_uart0_callback_error(err_type);
    }
    
    rx_data = SDR01L;

    if (g_uart0_rx_length > g_uart0_rx_count)
    {
        *gp_uart0_rx_address = rx_data;
        gp_uart0_rx_address++;
        g_uart0_rx_count++;	

        if (g_uart0_rx_length == g_uart0_rx_count)
        {
            r_uart0_callback_receiveend();
        }
    }
    else
    {
        r_uart0_callback_softwareoverrun(rx_data);
    }
}

/***********************************************************************************************************************
* Function Name: r_uart0_interrupt_send
* Description  : This function is INTST0 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void __near r_uart0_interrupt_send(void)
{
    if (g_uart0_tx_count > 0U)
    {
        SDR00L = *gp_uart0_tx_address;
        gp_uart0_tx_address++;
        g_uart0_tx_count--;
    }
    else
    {
        r_uart0_callback_sendend();
    }
}

/***********************************************************************************************************************
* Function Name: r_uart0_callback_receiveend
* Description  : This function is a callback function when UART0 finishes reception.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_uart0_callback_receiveend(void)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_uart0_callback_softwareoverrun
* Description  : This function is a callback function when UART0 receives an overflow data.
* Arguments    : rx_data -
*                    receive data
* Return Value : None
***********************************************************************************************************************/
static void r_uart0_callback_softwareoverrun(uint16_t rx_data)
{
    /* Start user code. Do not edit comment generated here */
    UARTx_Process(rx_data);
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_uart0_callback_sendend
* Description  : This function is a callback function when UART0 finishes transmission.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_uart0_callback_sendend(void)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_uart0_callback_error
* Description  : This function is a callback function when UART0 reception error occurs.
* Arguments    : err_type -
*                    error type value
* Return Value : None
***********************************************************************************************************************/
static void r_uart0_callback_error(uint8_t err_type)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_iic11_interrupt
* Description  : This function is INTIIC11 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void __near r_iic11_interrupt(void)
{
    volatile uint16_t w_count;

    /* Set delay to start next transmission. The delay time depend on slave device.
       Here set 20us as default base on current clock */
    for (w_count = 0U; w_count <= IIC11_WAITTIME_2; w_count++)
    {
        NOP();
    }

    if (((SSR11 & _0002_SAU_PARITY_ERROR) == 0x0002U) && (g_iic11_tx_count != 0U))
    {
        SIR11 |= _0002_SAU_SIRMN_PECTMN; //clear ACK error detection flag
        R_IIC11_StopCondition();
        r_iic11_callback_master_error(MD_NACK);
    }
    else if(((SSR11 & _0001_SAU_OVERRUN_ERROR) == 0x0001U) && (g_iic11_tx_count != 0U))
    {
        SIR11 |= _0001_SAU_SIRMN_OVCTMN; //clear overrun error detection flag
        R_IIC11_StopCondition();
        r_iic11_callback_master_error(MD_OVERRUN);
    }
    else
    {
        /* Control for master send */
        if ((g_iic11_master_status_flag & _01_SAU_IIC_SEND_FLAG) == 1U)
        {
            if (g_iic11_tx_count > 0U)
            {
                SDR11L = *gp_iic11_tx_address;
                gp_iic11_tx_address++;
                g_iic11_tx_count--;
            }
            else
            {
                /* IIC master transmission finishes and a callback function can be called here. */
                r_iic11_callback_master_sendend();
            }
        }
        /* Control for master receive */
        else 
        {
            if ((g_iic11_master_status_flag & _04_SAU_IIC_SENDED_ADDRESS_FLAG) == 0U)
            {
                ST1 |= _0002_SAU_CH1_STOP_TRG_ON;
                SCR11 &= ~_C000_SAU_RECEPTION_TRANSMISSION;
                SCR11 |= _4000_SAU_RECEPTION;
                SS1 |= _0002_SAU_CH1_START_TRG_ON;
                g_iic11_master_status_flag |= _04_SAU_IIC_SENDED_ADDRESS_FLAG;

                if (g_iic11_rx_length == 1U)
                {
                    SOE1 &= ~_0002_SAU_CH1_OUTPUT_ENABLE;    /* disable IIC11 out */
                }

                SDR11L = 0xFFU;
            }
            else
            {
                if (g_iic11_rx_count < g_iic11_rx_length)
                {
                    *gp_iic11_rx_address = SDR11L;
                    gp_iic11_rx_address++;
                    g_iic11_rx_count++;
                    
                    if (g_iic11_rx_count == (g_iic11_rx_length - 1U))
                    {
                        SOE1 &= ~_0002_SAU_CH1_OUTPUT_ENABLE;    /* disable IIC11 out */
                        SDR11L = 0xFFU;
                    }
                    else if (g_iic11_rx_count == g_iic11_rx_length)
                    {
                        /* IIC master reception finishes and a callback function can be called here. */
                        r_iic11_callback_master_receiveend();
                    }
                    else
                    {
                        SDR11L = 0xFFU;
                    }
                }
            }
        }
    }
}

/***********************************************************************************************************************
* Function Name: r_iic11_callback_master_error
* Description  : This function is a callback function when IIC11 master error occurs.
* Arguments    : flag -
*                    status flag
* Return Value : None
***********************************************************************************************************************/
static void r_iic11_callback_master_error(MD_STATUS flag)
{
    /* Start user code. Do not edit comment generated here */
    drv_IIC11_callback_error(flag);
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_iic11_callback_master_receiveend
* Description  : This function is a callback function when IIC11 finishes master reception.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_iic11_callback_master_receiveend(void)
{
    /* Start user code. Do not edit comment generated here */
    R_IIC11_StopCondition();
    drv_set_IIC11_receive_flag(0);
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_iic11_callback_master_sendend
* Description  : This function is a callback function when IIC11 finishes master transmission.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_iic11_callback_master_sendend(void)
{
    /* Start user code. Do not edit comment generated here */
    R_IIC11_StopCondition();
    drv_set_IIC11_send_flag(0);
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
