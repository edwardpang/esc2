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
* Creation Date: 27/01/2014
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
extern motor_phase_t	g_motor_phase_set_timer0;
extern motor_phase_t	g_motor_phase_set_timer1;
extern const motor_phase_t	abc2phase[MOTOR_PHASE_NUM];

extern uint16_t	g_u16_ls_pwm_full, g_u16_ls_pwm_empty;
extern uint16_t	g_u16_hs_pwm_full, g_u16_hs_pwm_empty;
extern throttle_direction_t	g_throttle_direction;
extern uint16_t	g_u16_throttle_pos_in_pwm_duty_current;

extern uint16_t	g_u16_speed_count_us;

extern uint16_t g_u16_speed_count_us_degree_60;
extern uint16_t g_u16_speed_count_us_degree_120;
extern uint16_t g_u16_speed_count_us_degree_180;
extern uint16_t g_u16_speed_count_us_degree_240;
extern uint16_t g_u16_speed_count_us_degree_300;
extern uint16_t g_u16_speed_count_us_degree_360;

extern uint8_t	g_u8_turbo_drive_phase_count_to_start;
extern uint8_t	g_u8_turbo_drive_phase_count;
extern bit		g_bit_turbo_drive_start;

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

	/* Update Speed */
	g_u16_speed_count_us = SPEED_1US_COUNTER_RESET_VALUE - TRJ0;
	TDR03 = SPEED_1US_TIMER_RESET_VALUE;
	R_TMR_RJ0_Stop ( );
	TRJ0 = SPEED_1US_COUNTER_RESET_VALUE;
	R_TMR_RJ0_Start ( );
	
	/* Update Phase */
	index = (PIN_HALL_SENSOR_A << 2);
	index |= (PIN_HALL_SENSOR_B << 1);
	index |= (PIN_HALL_SENSOR_C);
	g_motor_phase_current = abc2phase[index];

	/* Update Motor Control */
	if (g_app_state == APP_STATE_MOTOR_CONTROL_FWD_DRIVING) {
		switch (g_motor_phase_current) {
			case MOTOR_PHASE_DEGREE_60:
				PM1 |= (PIN_MOTOR_DRV_HS_A | PIN_MOTOR_DRV_HS_B | PIN_MOTOR_DRV_LS_A | PIN_MOTOR_DRV_LS_C);
				PM1 &= ~(PIN_MOTOR_DRV_HS_C | PIN_MOTOR_DRV_LS_B);
				g_u16_speed_count_us_degree_60 = g_u16_speed_count_us;
				break;
				
			case MOTOR_PHASE_DEGREE_120:
				PM1 |= (PIN_MOTOR_DRV_HS_B | PIN_MOTOR_DRV_HS_C | PIN_MOTOR_DRV_LS_A | PIN_MOTOR_DRV_LS_C);
				PM1 &= ~(PIN_MOTOR_DRV_HS_A | PIN_MOTOR_DRV_LS_B);
				g_u16_speed_count_us_degree_120 = g_u16_speed_count_us;
				break;
				
			case MOTOR_PHASE_DEGREE_180:
				PM1 |= (PIN_MOTOR_DRV_HS_B | PIN_MOTOR_DRV_HS_C | PIN_MOTOR_DRV_LS_A | PIN_MOTOR_DRV_LS_B);
				PM1 &= ~(PIN_MOTOR_DRV_HS_A | PIN_MOTOR_DRV_LS_C);
				g_u16_speed_count_us_degree_180 = g_u16_speed_count_us;
				break;
				
			case MOTOR_PHASE_DEGREE_240:
				PM1 |= (PIN_MOTOR_DRV_HS_A | PIN_MOTOR_DRV_HS_C | PIN_MOTOR_DRV_LS_A | PIN_MOTOR_DRV_LS_B);
				PM1 &= ~(PIN_MOTOR_DRV_HS_B | PIN_MOTOR_DRV_LS_C);
				g_u16_speed_count_us_degree_240 = g_u16_speed_count_us;
				break;
				
			case MOTOR_PHASE_DEGREE_300:
				PM1 |= (PIN_MOTOR_DRV_HS_A | PIN_MOTOR_DRV_HS_C | PIN_MOTOR_DRV_LS_B | PIN_MOTOR_DRV_LS_C);
				PM1 &= ~(PIN_MOTOR_DRV_HS_B | PIN_MOTOR_DRV_LS_A);
				g_u16_speed_count_us_degree_300 = g_u16_speed_count_us;
				break;
				
			case MOTOR_PHASE_DEGREE_360:
				PM1 |= (PIN_MOTOR_DRV_HS_A | PIN_MOTOR_DRV_HS_B | PIN_MOTOR_DRV_LS_B | PIN_MOTOR_DRV_LS_C);
				PM1 &= ~(PIN_MOTOR_DRV_HS_C | PIN_MOTOR_DRV_LS_A);
				g_u16_speed_count_us_degree_360 = g_u16_speed_count_us;
				break;

			case MOTOR_PHASE_OPEN:
			case MOTOR_PHASE_ERROR:
			default:
				break;
		}
	}
	else if (g_app_state == APP_STATE_MOTOR_CONTROL_REV_DRIVING) {
		switch (g_motor_phase_current) {
			case MOTOR_PHASE_DEGREE_60:
				PM1 |= (PIN_MOTOR_DRV_HS_B | PIN_MOTOR_DRV_HS_C | PIN_MOTOR_DRV_LS_A | PIN_MOTOR_DRV_LS_B);
				PM1 &= ~(PIN_MOTOR_DRV_HS_A | PIN_MOTOR_DRV_LS_C);
				g_u16_speed_count_us_degree_60 = g_u16_speed_count_us;
				break;
				
			case MOTOR_PHASE_DEGREE_120:
				PM1 |= (PIN_MOTOR_DRV_HS_A | PIN_MOTOR_DRV_HS_C | PIN_MOTOR_DRV_LS_A | PIN_MOTOR_DRV_LS_B);
				PM1 &= ~(PIN_MOTOR_DRV_HS_B | PIN_MOTOR_DRV_LS_C);
				g_u16_speed_count_us_degree_120 = g_u16_speed_count_us;
				break;
				
			case MOTOR_PHASE_DEGREE_180:
				PM1 |= (PIN_MOTOR_DRV_HS_A | PIN_MOTOR_DRV_HS_C | PIN_MOTOR_DRV_LS_B | PIN_MOTOR_DRV_LS_C);
				PM1 &= ~(PIN_MOTOR_DRV_HS_B | PIN_MOTOR_DRV_LS_A);
				g_u16_speed_count_us_degree_180 = g_u16_speed_count_us;
				break;
				
			case MOTOR_PHASE_DEGREE_240:
				PM1 |= (PIN_MOTOR_DRV_HS_A | PIN_MOTOR_DRV_HS_B | PIN_MOTOR_DRV_LS_B | PIN_MOTOR_DRV_LS_C);
				PM1 &= ~(PIN_MOTOR_DRV_HS_C | PIN_MOTOR_DRV_LS_A);
				g_u16_speed_count_us_degree_240 = g_u16_speed_count_us;
				break;
				
			case MOTOR_PHASE_DEGREE_300:
				PM1 |= (PIN_MOTOR_DRV_HS_A | PIN_MOTOR_DRV_HS_B | PIN_MOTOR_DRV_LS_A | PIN_MOTOR_DRV_LS_C);
				PM1 &= ~(PIN_MOTOR_DRV_HS_C | PIN_MOTOR_DRV_LS_B);
				g_u16_speed_count_us_degree_300 = g_u16_speed_count_us;
				break;
				
			case MOTOR_PHASE_DEGREE_360:
				PM1 |= (PIN_MOTOR_DRV_HS_B | PIN_MOTOR_DRV_HS_C | PIN_MOTOR_DRV_LS_A | PIN_MOTOR_DRV_LS_C);
				PM1 &= ~(PIN_MOTOR_DRV_HS_A | PIN_MOTOR_DRV_LS_B);
				g_u16_speed_count_us_degree_360 = g_u16_speed_count_us;
				break;

			case MOTOR_PHASE_OPEN:
			case MOTOR_PHASE_ERROR:
			default:
				break;
		}
	}
	else if (g_app_state == APP_STATE_MOTOR_CONTROL_PRE_TURBO_DRIVING) {
		switch (g_motor_phase_current) {
			case MOTOR_PHASE_DEGREE_60:
				PM1 |= (PIN_MOTOR_DRV_HS_A | PIN_MOTOR_DRV_HS_B | PIN_MOTOR_DRV_LS_A | PIN_MOTOR_DRV_LS_C);
				PM1 &= ~(PIN_MOTOR_DRV_HS_C | PIN_MOTOR_DRV_LS_B);
				g_u16_speed_count_us_degree_60 = g_u16_speed_count_us;
				TDR00 = g_u16_speed_count_us_degree_60 * TURBO_DRIVE_PHASE_SPEED_1US_RESET_VALUE;
				g_motor_phase_set_timer0 = MOTOR_PHASE_DEGREE_60;
				break;
				
			case MOTOR_PHASE_DEGREE_120:
				PM1 |= (PIN_MOTOR_DRV_HS_B | PIN_MOTOR_DRV_HS_C | PIN_MOTOR_DRV_LS_A | PIN_MOTOR_DRV_LS_C);
				PM1 &= ~(PIN_MOTOR_DRV_HS_A | PIN_MOTOR_DRV_LS_B);
				g_u16_speed_count_us_degree_120 = g_u16_speed_count_us;
				TDR01 = g_u16_speed_count_us_degree_120 * TURBO_DRIVE_PHASE_SPEED_1US_RESET_VALUE;
				g_motor_phase_set_timer1 = MOTOR_PHASE_DEGREE_120;
				break;
				
			case MOTOR_PHASE_DEGREE_180:
				PM1 |= (PIN_MOTOR_DRV_HS_B | PIN_MOTOR_DRV_HS_C | PIN_MOTOR_DRV_LS_A | PIN_MOTOR_DRV_LS_B);
				PM1 &= ~(PIN_MOTOR_DRV_HS_A | PIN_MOTOR_DRV_LS_C);
				g_u16_speed_count_us_degree_180 = g_u16_speed_count_us;
				TDR00 = g_u16_speed_count_us_degree_180 * TURBO_DRIVE_PHASE_SPEED_1US_RESET_VALUE;
				g_motor_phase_set_timer0 = MOTOR_PHASE_DEGREE_180;
				break;
				
			case MOTOR_PHASE_DEGREE_240:
				PM1 |= (PIN_MOTOR_DRV_HS_A | PIN_MOTOR_DRV_HS_C | PIN_MOTOR_DRV_LS_A | PIN_MOTOR_DRV_LS_B);
				PM1 &= ~(PIN_MOTOR_DRV_HS_B | PIN_MOTOR_DRV_LS_C);
				g_u16_speed_count_us_degree_240 = g_u16_speed_count_us;
				TDR01 = g_u16_speed_count_us_degree_240 * TURBO_DRIVE_PHASE_SPEED_1US_RESET_VALUE;
				g_motor_phase_set_timer1 = MOTOR_PHASE_DEGREE_240;
				break;
				
			case MOTOR_PHASE_DEGREE_300:
				PM1 |= (PIN_MOTOR_DRV_HS_A | PIN_MOTOR_DRV_HS_C | PIN_MOTOR_DRV_LS_B | PIN_MOTOR_DRV_LS_C);
				PM1 &= ~(PIN_MOTOR_DRV_HS_B | PIN_MOTOR_DRV_LS_A);
				g_u16_speed_count_us_degree_300 = g_u16_speed_count_us;
				TDR00 = g_u16_speed_count_us_degree_300 * TURBO_DRIVE_PHASE_SPEED_1US_RESET_VALUE;
				g_motor_phase_set_timer0 = MOTOR_PHASE_DEGREE_300;
				break;
				
			case MOTOR_PHASE_DEGREE_360:
				PM1 |= (PIN_MOTOR_DRV_HS_A | PIN_MOTOR_DRV_HS_B | PIN_MOTOR_DRV_LS_B | PIN_MOTOR_DRV_LS_C);
				PM1 &= ~(PIN_MOTOR_DRV_HS_C | PIN_MOTOR_DRV_LS_A);
				g_u16_speed_count_us_degree_360 = g_u16_speed_count_us;
				TDR01 = g_u16_speed_count_us_degree_360 * TURBO_DRIVE_PHASE_SPEED_1US_RESET_VALUE;
				g_motor_phase_set_timer0 = MOTOR_PHASE_DEGREE_300;
				break;

			case MOTOR_PHASE_OPEN:
			case MOTOR_PHASE_ERROR:
			default:
				break;
		}
		g_u8_turbo_drive_phase_count ++;
		if (g_u8_turbo_drive_phase_count == g_u8_turbo_drive_phase_count_to_start) {
			if (g_motor_phase_current == MOTOR_PHASE_DEGREE_120 || 
				g_motor_phase_current == MOTOR_PHASE_DEGREE_240 ||
				g_motor_phase_current == MOTOR_PHASE_DEGREE_360)
				R_TAU0_Channel0_Start ( );
			else if (g_motor_phase_current == MOTOR_PHASE_DEGREE_60 || 
				g_motor_phase_current == MOTOR_PHASE_DEGREE_180 ||
				g_motor_phase_current == MOTOR_PHASE_DEGREE_300)
				R_TAU0_Channel1_Start ( );
			g_bit_turbo_drive_start = 1;
		}
	}
	else if (g_app_state == APP_STATE_MOTOR_CONTROL_TURBO_DRIVING) {
		PIN_LED_GREEN = 1;
		switch (g_motor_phase_current) {
			case MOTOR_PHASE_DEGREE_60:
				g_u16_speed_count_us_degree_60 = g_u16_speed_count_us;
				// prepare timer 0, start timer 1
				TDR00 = g_u16_speed_count_us * TURBO_DRIVE_PHASE_SPEED_1US_RESET_VALUE;
				g_motor_phase_set_timer0 = g_motor_phase_current;
				R_TAU0_Channel1_Start ( );
				break;

			case MOTOR_PHASE_DEGREE_120:
				g_u16_speed_count_us_degree_120 = g_u16_speed_count_us;
				// prepare timer 1, start timer 0
				TDR01 = g_u16_speed_count_us * TURBO_DRIVE_PHASE_SPEED_1US_RESET_VALUE;
				g_motor_phase_set_timer1 = g_motor_phase_current;
				R_TAU0_Channel0_Start ( );
				break;

			case MOTOR_PHASE_DEGREE_180:
				g_u16_speed_count_us_degree_180 = g_u16_speed_count_us;
				// prepare timer 0, start timer 1
				TDR00 = g_u16_speed_count_us * TURBO_DRIVE_PHASE_SPEED_1US_RESET_VALUE;
				g_motor_phase_set_timer0 = g_motor_phase_current;
				R_TAU0_Channel1_Start ( );
				break;

			case MOTOR_PHASE_DEGREE_240:
				g_u16_speed_count_us_degree_240 = g_u16_speed_count_us;
				// prepare timer 1, start timer 0
				TDR01 = g_u16_speed_count_us * TURBO_DRIVE_PHASE_SPEED_1US_RESET_VALUE;
				g_motor_phase_set_timer1 = g_motor_phase_current;
				R_TAU0_Channel0_Start ( );
				break;

			case MOTOR_PHASE_DEGREE_300:
				g_u16_speed_count_us_degree_300 = g_u16_speed_count_us;
				// prepare timer 0, start timer 1
				TDR00 = g_u16_speed_count_us * TURBO_DRIVE_PHASE_SPEED_1US_RESET_VALUE;
				g_motor_phase_set_timer0 = g_motor_phase_current;
				R_TAU0_Channel1_Start ( );
				break;

			case MOTOR_PHASE_DEGREE_360:
				g_u16_speed_count_us_degree_360 = g_u16_speed_count_us;
				// prepare timer 1, start timer 0
				TDR01 = g_u16_speed_count_us * TURBO_DRIVE_PHASE_SPEED_1US_RESET_VALUE;
				g_motor_phase_set_timer1 = g_motor_phase_current;
				R_TAU0_Channel0_Start ( );
				break;

			case MOTOR_PHASE_OPEN:
			case MOTOR_PHASE_ERROR:
			default:
				break;
		}
		PIN_LED_GREEN = 0;
	}
}
/* End user code. Do not edit comment generated here */
