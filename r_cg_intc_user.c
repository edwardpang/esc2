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
* File Name    : r_cg_intc_user.c
* Version      : CodeGenerator for RL78/G14 V2.01.00.02 [09 Aug 2013]
* Device(s)    : R5F104BA
* Tool-Chain   : CA78K0R
* Description  : This file implements device driver for INTC module.
* Creation Date: 28/11/2013
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
#pragma interrupt INTP0 r_intc0_interrupt
#if 0
#pragma interrupt INTP1 r_intc1_interrupt
#pragma interrupt INTP2 r_intc2_interrupt
#pragma interrupt INTP3 r_intc3_interrupt
#endif
/* Start user code for pragma. Do not edit comment generated here */
#pragma interrupt INTP1 r_hall_sensor_common_interrupt
#pragma interrupt INTP2 r_hall_sensor_common_interrupt
#pragma interrupt INTP3 r_hall_sensor_common_interrupt
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_intc.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
extern app_state_t 	g_app_state;

extern motor_phase_t	g_motor_phase_current;
extern motor_phase_t	g_motor_phase_last;
extern const motor_phase_t	abc2phase[MOTOR_PHASE_NUM];

extern uint16_t	g_u16_ls_pwm_full, g_u16_ls_pwm_empty;
extern uint16_t	g_u16_hs_pwm_full, g_u16_hs_pwm_empty;
extern throttle_direction_t	g_throttle_direction;
extern uint16_t	g_u16_throttle_pos_in_pwm_duty_current;
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_intc0_interrupt
* Description  : This function is INTP0 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
__interrupt static void r_intc0_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_intc1_interrupt
* Description  : This function is INTP1 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
__interrupt static void r_intc1_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_intc2_interrupt
* Description  : This function is INTP2 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
__interrupt static void r_intc2_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_intc3_interrupt
* Description  : This function is INTP3 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
__interrupt static void r_intc3_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
__interrupt static void r_hall_sensor_common_interrupt (void) {
	uint16_t	index;

	/* Update Phase */
	index = (PIN_HALL_SENSOR_A << 2);
	index |= (PIN_HALL_SENSOR_B << 1);
	index |= (PIN_HALL_SENSOR_C);
	g_motor_phase_last = g_motor_phase_current;
	g_motor_phase_current = abc2phase[index];

	/* Update Motor Control */
	if (g_throttle_direction == THROTTLE_DIRECTION_CCW) {
		if (g_app_state == APP_STATE_MOTOR_CONTROL_ZERO_DRIVING) {
			switch (g_motor_phase_current) {
				case MOTOR_PHASE_DEGREE_60:
				    //MOTOR_DRV_HS_A = g_u16_hs_pwm_empty;	// skip
				    //MOTOR_DRV_HS_B = g_u16_hs_pwm_empty;	// skip
				    //MOTOR_DRV_HS_C = g_u16_throttle_pos_in_pwm_duty_current;		// skip
				    MOTOR_DRV_LS_A = g_u16_ls_pwm_empty;
				    MOTOR_DRV_LS_B = g_u16_ls_pwm_full;
				    //MOTOR_DRV_LS_C = g_u16_ls_pwm_empty;	// skip
					break;
					
				case MOTOR_PHASE_DEGREE_120:
				    MOTOR_DRV_HS_A = g_u16_throttle_pos_in_pwm_duty_current;
				    //MOTOR_DRV_HS_B = g_u16_hs_pwm_empty;	// skip
				    MOTOR_DRV_HS_C = g_u16_hs_pwm_empty;
				    //MOTOR_DRV_LS_A = g_u16_ls_pwm_empty;	// skip
				    //MOTOR_DRV_LS_B = g_u16_ls_pwm_full;	// skip
				    //MOTOR_DRV_LS_C = g_u16_ls_pwm_empty;	// skip
					break;
					
				case MOTOR_PHASE_DEGREE_180:
				    //MOTOR_DRV_HS_A = g_u16_throttle_pos_in_pwm_duty_current;		// skip
				    //MOTOR_DRV_HS_B = g_u16_hs_pwm_empty;	// skip
				    //MOTOR_DRV_HS_C = g_u16_hs_pwm_empty;	// skip
				    //MOTOR_DRV_LS_A = g_u16_ls_pwm_empty;	// skip
				    MOTOR_DRV_LS_B = g_u16_ls_pwm_empty;
				    MOTOR_DRV_LS_C = g_u16_ls_pwm_full;
					break;
					
				case MOTOR_PHASE_DEGREE_240:
				    MOTOR_DRV_HS_A = g_u16_hs_pwm_empty;
				    MOTOR_DRV_HS_B = g_u16_throttle_pos_in_pwm_duty_current;
				    //MOTOR_DRV_HS_C = g_u16_hs_pwm_empty;	// skip
				    //MOTOR_DRV_LS_A = g_u16_ls_pwm_empty;	// skip
				    //MOTOR_DRV_LS_B = g_u16_ls_pwm_empty;	// skip
				    //MOTOR_DRV_LS_C = g_u16_ls_pwm_full;	// skip
					break;
					
				case MOTOR_PHASE_DEGREE_300:
				    //MOTOR_DRV_HS_A = g_u16_hs_pwm_empty;	// skip
				    //MOTOR_DRV_HS_B = g_u16_throttle_pos_in_pwm_duty_current;		// skip
				    //MOTOR_DRV_HS_C = g_u16_hs_pwm_empty;	// skip
				    MOTOR_DRV_LS_A = g_u16_ls_pwm_full;
				    //MOTOR_DRV_LS_B = g_u16_ls_pwm_empty;	// skip
				    MOTOR_DRV_LS_C = g_u16_ls_pwm_empty;
					break;
					
				case MOTOR_PHASE_DEGREE_360:
				    //MOTOR_DRV_HS_A = g_u16_hs_pwm_empty;	// skip
				    MOTOR_DRV_HS_B = g_u16_hs_pwm_empty;
				    MOTOR_DRV_HS_C = g_u16_throttle_pos_in_pwm_duty_current;
				    //MOTOR_DRV_LS_A = g_u16_ls_pwm_full;	// skip
				    //MOTOR_DRV_LS_B = g_u16_ls_pwm_empty;	// skip
				    //MOTOR_DRV_LS_C = g_u16_ls_pwm_empty;	// skip
					break;

				case MOTOR_PHASE_OPEN:
				case MOTOR_PHASE_ERROR:
				default:
				    MOTOR_DRV_HS_A = g_u16_hs_pwm_empty;
				    MOTOR_DRV_HS_B = g_u16_hs_pwm_empty;
				    MOTOR_DRV_HS_C = g_u16_hs_pwm_empty;
				    MOTOR_DRV_LS_A = g_u16_ls_pwm_empty;
				    MOTOR_DRV_LS_B = g_u16_ls_pwm_empty;
				    MOTOR_DRV_LS_C = g_u16_ls_pwm_empty;
					break;
			}
		}
		else {
			// Delay Driving
		}
	}
	else if (g_throttle_direction == THROTTLE_DIRECTION_CW) {
		// CW must be zero timing driving
		switch (g_motor_phase_current) {
			case MOTOR_PHASE_DEGREE_60:
			    MOTOR_DRV_HS_A = g_u16_throttle_pos_in_pwm_duty_current;
			    MOTOR_DRV_HS_B = g_u16_hs_pwm_empty;
			    //MOTOR_DRV_HS_C = g_u16_hs_pwm_empty;	// skip
			    //MOTOR_DRV_LS_A = g_u16_ls_pwm_empty;	// skip
			    //MOTOR_DRV_LS_B = g_u16_ls_pwm_empty;	// skip
			    //MOTOR_DRV_LS_C = g_u16_ls_pwm_full;	// skip
				break;
				
			case MOTOR_PHASE_DEGREE_120:
			    //MOTOR_DRV_HS_A = g_u16_hs_pwm_empty;	// skip
			    //MOTOR_DRV_HS_B = g_u16_throttle_pos_in_pwm_duty_current;		// skip
			    //MOTOR_DRV_HS_C = g_u16_hs_pwm_empty;	// skip
			    MOTOR_DRV_LS_A = g_u16_ls_pwm_empty;
			    //MOTOR_DRV_LS_B = g_u16_ls_pwm_empty;	// skip
			    MOTOR_DRV_LS_C = g_u16_ls_pwm_full;
				break;
				
			case MOTOR_PHASE_DEGREE_180:
			    //MOTOR_DRV_HS_A = g_u16_hs_pwm_empty;	// skip
			    MOTOR_DRV_HS_B = g_u16_throttle_pos_in_pwm_duty_current;
			    MOTOR_DRV_HS_C = g_u16_hs_pwm_empty;
			    //MOTOR_DRV_LS_A = g_u16_ls_pwm_full;	// skip
			    //MOTOR_DRV_LS_B = g_u16_ls_pwm_empty;	// skip
			    //MOTOR_DRV_LS_C = g_u16_ls_pwm_empty;	// skip
				break;
				
			case MOTOR_PHASE_DEGREE_240:
			    //MOTOR_DRV_HS_A = g_u16_hs_pwm_empty;	// skip
			    //MOTOR_DRV_HS_B = g_u16_hs_pwm_empty;	// skip
			    //MOTOR_DRV_HS_C = g_u16_throttle_pos_in_pwm_duty_current;		// skip
			    MOTOR_DRV_LS_A = g_u16_ls_pwm_full;
			    MOTOR_DRV_LS_B = g_u16_ls_pwm_empty;
			    //MOTOR_DRV_LS_C = g_u16_ls_pwm_empty;	// skip
				break;
				
			case MOTOR_PHASE_DEGREE_300:
			    MOTOR_DRV_HS_A = g_u16_hs_pwm_empty;
			    //MOTOR_DRV_HS_B = g_u16_hs_pwm_empty;	// skip
			    MOTOR_DRV_HS_C = g_u16_throttle_pos_in_pwm_duty_current;
			    //MOTOR_DRV_LS_A = g_u16_ls_pwm_empty;	// skip
			    //MOTOR_DRV_LS_B = g_u16_ls_pwm_full;	// skip
			    //MOTOR_DRV_LS_C = g_u16_ls_pwm_empty;	// skip
				break;
				
			case MOTOR_PHASE_DEGREE_360:
			    //MOTOR_DRV_HS_A = g_u16_throttle_pos_in_pwm_duty_current;		// skip
			    //MOTOR_DRV_HS_B = g_u16_hs_pwm_empty;	// skip
			    //MOTOR_DRV_HS_C = g_u16_hs_pwm_empty;	// skip
			    //MOTOR_DRV_LS_A = g_u16_ls_pwm_empty;
			    MOTOR_DRV_LS_B = g_u16_ls_pwm_full;
			    MOTOR_DRV_LS_C = g_u16_ls_pwm_empty;
				break;

			case MOTOR_PHASE_OPEN:
			case MOTOR_PHASE_ERROR:
			default:
			    MOTOR_DRV_HS_A = g_u16_hs_pwm_empty;
			    MOTOR_DRV_HS_B = g_u16_hs_pwm_empty;
			    MOTOR_DRV_HS_C = g_u16_hs_pwm_empty;
			    MOTOR_DRV_LS_A = g_u16_ls_pwm_empty;
			    MOTOR_DRV_LS_B = g_u16_ls_pwm_empty;
			    MOTOR_DRV_LS_C = g_u16_ls_pwm_empty;
				break;
		}
	}
	else {
		// no direction => stop or brake
	}
}
/* End user code. Do not edit comment generated here */
