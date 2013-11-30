/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only 
* intended for use with Renesas products. No other uses are authorized. This 
* software is owned by Renesas Electronics Corporation and is protected under 
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING 
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT 
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
* AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR 
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE 
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software 
* and to discontinue the availability of this software.  By using this software, 
* you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2011, 2013 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_timer_user.c
* Version      : CodeGenerator for RL78/G14 V2.01.00.02 [09 Aug 2013]
* Device(s)    : R5F104BA
* Tool-Chain   : CA78K0R
* Description  : This file implements device driver for TAU module.
* Creation Date: 28/11/2013
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
#pragma interrupt INTTM02 r_tau0_channel2_interrupt
#pragma interrupt INTTRJ0 r_tmr_rj0_interrupt
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_timer.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* For TAU0_ch2 pulse measurement */
volatile uint32_t g_tau0_ch2_width = 0U;
/* Start user code for global. Do not edit comment generated here */
extern throttle_direction_t		g_throttle_direction;
extern uint16_t		g_u16_throttle_pos_max;
extern uint16_t		g_u16_throttle_pos_neutral;
extern uint16_t		g_u16_throttle_pos_min;
extern uint16_t		g_u16_throttle_pos_neutral_upper;
extern uint16_t		g_u16_throttle_pos_neutral_lower;
extern uint16_t		g_u16_throttle_pos_tolerance_percentage;
extern uint16_t		g_u16_throttle_pos_in_pwm_duty_current;
extern uint16_t		g_u16_throttle_pos_in_pwm_duty_last;
volatile uint16_t	u16_throttle_sample;
extern uint16_t		g_u16_hs_pwm_full, g_u16_hs_pwm_empty;
extern uint16_t		g_u16_ls_pwm_full, g_u16_ls_pwm_empty;

extern const motor_phase_t	abc2phase[MOTOR_PHASE_NUM];
extern motor_phase_t	g_motor_phase_current;

extern app_state_t 	g_app_state;
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_tau0_channel2_interrupt
* Description  : This function is INTTM02 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
__interrupt static void r_tau0_channel2_interrupt(void)
{
    if ((TSR02 & _0001_TAU_OVERFLOW_OCCURS) == 1U)    /* overflow occurs */
    {            
        g_tau0_ch2_width = (uint32_t)(TDR02 + 1U) + 0x10000U;
    }
    else
    {
        g_tau0_ch2_width = (uint32_t)(TDR02 + 1U);
    }

    /* Start user code. Do not edit comment generated here */
	u16_throttle_sample = (TDR02 + 1U);
	g_u16_throttle_pos_in_pwm_duty_last = g_u16_throttle_pos_in_pwm_duty_current;
	if (u16_throttle_sample < g_u16_throttle_pos_min) {
		g_u16_throttle_pos_in_pwm_duty_current = g_u16_hs_pwm_full;
		g_throttle_direction = THROTTLE_DIRECTION_CCW;
	}
	else if (u16_throttle_sample < g_u16_throttle_pos_neutral_lower) {
		g_u16_throttle_pos_in_pwm_duty_current = 16000U - (g_u16_throttle_pos_neutral - u16_throttle_sample);
		g_u16_throttle_pos_in_pwm_duty_current = (g_u16_throttle_pos_in_pwm_duty_current >> THROTTLE_POS_SHIFT_BIT);
		g_throttle_direction = THROTTLE_DIRECTION_CCW;
	}
	else if (u16_throttle_sample < g_u16_throttle_pos_neutral_upper) {
		g_u16_throttle_pos_in_pwm_duty_current = g_u16_hs_pwm_empty;
		g_throttle_direction = THROTTLE_DIRECTION_NEUTRAL;
	}
	else if (u16_throttle_sample < g_u16_throttle_pos_max) {
		g_u16_throttle_pos_in_pwm_duty_current = 16000U - (u16_throttle_sample - g_u16_throttle_pos_neutral);
		g_u16_throttle_pos_in_pwm_duty_current = (g_u16_throttle_pos_in_pwm_duty_current >> THROTTLE_POS_SHIFT_BIT);
		g_throttle_direction = THROTTLE_DIRECTION_CW;
	}
	else {
		g_u16_throttle_pos_in_pwm_duty_current = g_u16_hs_pwm_full;
		g_throttle_direction = THROTTLE_DIRECTION_CW;
	}
	
	if (g_app_state == APP_STATE_MOTOR_CONTROL_BREAK) {
	    MOTOR_DRV_LS_A = (g_u16_throttle_pos_in_pwm_duty_current << 2);
    	MOTOR_DRV_LS_B = (g_u16_throttle_pos_in_pwm_duty_current << 2);
    	MOTOR_DRV_LS_C = (g_u16_throttle_pos_in_pwm_duty_current << 2);
	}
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_tmr_rj0_interrupt
* Description  : This function is INTTRJ0 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
__interrupt static void r_tmr_rj0_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
