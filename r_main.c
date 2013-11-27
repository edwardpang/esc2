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
* File Name    : r_main.c
* Version      : CodeGenerator for RL78/G14 V2.01.00.02 [09 Aug 2013]
* Device(s)    : R5F104BA
* Tool-Chain   : CA78K0R
* Description  : This file implements main function.
* Creation Date: 27/11/2013
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_intc.h"
#include "r_cg_serial.h"
#include "r_cg_timer.h"
#include "r_cg_elc.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
app_state_t 	g_app_state;

motor_phase_t	g_motor_phase_current;
motor_phase_t	g_motor_phase_last;
const motor_phase_t	abc2phase[MOTOR_PHASE_NUM] = {
	MOTOR_PHASE_ERROR,
	MOTOR_PHASE_DEGREE_360,
	MOTOR_PHASE_DEGREE_240,
	MOTOR_PHASE_DEGREE_300,
	MOTOR_PHASE_DEGREE_120,
	MOTOR_PHASE_DEGREE_60,
	MOTOR_PHASE_DEGREE_180,
	MOTOR_PHASE_OPEN
};

uint16_t	g_u16_ls_pwm_full, g_u16_ls_pwm_empty;
uint16_t	g_u16_hs_pwm_full, g_u16_hs_pwm_empty;

throttle_direction_t	g_throttle_direction;
uint16_t	g_u16_throttle_pos_max;
uint16_t	g_u16_throttle_pos_neutral;
uint16_t	g_u16_throttle_pos_min;
uint16_t	g_u16_throttle_pos_neutral_upper;
uint16_t	g_u16_throttle_pos_neutral_lower;
uint16_t	g_u16_throttle_pos_neutral_tolerance_percentage;
uint16_t	g_u16_throttle_pos_sample_tolerance;
uint16_t	g_u16_throttle_pos_in_pwm_duty_current;
uint16_t	g_u16_throttle_pos_in_pwm_duty_last;

void motor_driver_enable (void);
void motor_driver_disable (void);
void app_init (void);
void app_config (void);
void app_handler (void);
/* End user code. Do not edit comment generated here */
void R_MAIN_UserInit(void);

/***********************************************************************************************************************
* Function Name: main
* Description  : This function implements main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void main(void)
{
    R_MAIN_UserInit();
    /* Start user code. Do not edit comment generated here */
    while (1U)
    {
        app_handler ( );
    }
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: R_MAIN_UserInit
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_MAIN_UserInit(void)
{
    /* Start user code. Do not edit comment generated here */
	app_init ( );
    EI();
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/***********************************************************************************************************************/
void motor_driver_disable (void) {
	PIN_MOTOR_DRIVER_ENABLE = MOTOR_DISABLE;
	MOTOR_DRV_HS_A = g_u16_hs_pwm_empty;
	MOTOR_DRV_HS_B = g_u16_hs_pwm_empty;
	MOTOR_DRV_HS_C = g_u16_hs_pwm_empty;
	MOTOR_DRV_LS_A = g_u16_ls_pwm_empty;
	MOTOR_DRV_LS_B = g_u16_ls_pwm_empty;
	MOTOR_DRV_LS_C = g_u16_ls_pwm_empty;
	R_TMR_RD0_Stop ( );
	R_TMR_RD1_Stop ( );
}

/***********************************************************************************************************************/
void motor_driver_enable (void) {
	uint16_t	index;
	
	index = (PIN_HALL_SENSOR_A << 2);
	index |= (PIN_HALL_SENSOR_B << 1);
	index |= (PIN_HALL_SENSOR_C);
	g_motor_phase_current = abc2phase[index];
	
	if (g_throttle_direction == THROTTLE_DIRECTION_CCW) {
		switch (g_motor_phase_current) {
			case MOTOR_PHASE_DEGREE_60:
			    MOTOR_DRV_HS_A = g_u16_hs_pwm_empty;	// skip
			    MOTOR_DRV_HS_B = g_u16_hs_pwm_empty;	// skip
			    MOTOR_DRV_HS_C = g_u16_throttle_pos_in_pwm_duty_current;		// skip
			    MOTOR_DRV_LS_A = g_u16_ls_pwm_empty;
			    MOTOR_DRV_LS_B = g_u16_ls_pwm_full;
			    MOTOR_DRV_LS_C = g_u16_ls_pwm_empty;	// skip
				break;
				
			case MOTOR_PHASE_DEGREE_120:
			    MOTOR_DRV_HS_A = g_u16_throttle_pos_in_pwm_duty_current;
			    MOTOR_DRV_HS_B = g_u16_hs_pwm_empty;	// skip
			    MOTOR_DRV_HS_C = g_u16_hs_pwm_empty;
			    MOTOR_DRV_LS_A = g_u16_ls_pwm_empty;	// skip
			    MOTOR_DRV_LS_B = g_u16_ls_pwm_full;	// skip
			    MOTOR_DRV_LS_C = g_u16_ls_pwm_empty;	// skip
				break;
				
			case MOTOR_PHASE_DEGREE_180:
			    MOTOR_DRV_HS_A = g_u16_throttle_pos_in_pwm_duty_current;		// skip
			    MOTOR_DRV_HS_B = g_u16_hs_pwm_empty;	// skip
			    MOTOR_DRV_HS_C = g_u16_hs_pwm_empty;	// skip
			    MOTOR_DRV_LS_A = g_u16_ls_pwm_empty;	// skip
			    MOTOR_DRV_LS_B = g_u16_ls_pwm_empty;
			    MOTOR_DRV_LS_C = g_u16_ls_pwm_full;
				break;
				
			case MOTOR_PHASE_DEGREE_240:
			    MOTOR_DRV_HS_A = g_u16_hs_pwm_empty;
			    MOTOR_DRV_HS_B = g_u16_throttle_pos_in_pwm_duty_current;
			    MOTOR_DRV_HS_C = g_u16_hs_pwm_empty;	// skip
			    MOTOR_DRV_LS_A = g_u16_ls_pwm_empty;	// skip
			    MOTOR_DRV_LS_B = g_u16_ls_pwm_empty;	// skip
			    MOTOR_DRV_LS_C = g_u16_ls_pwm_full;	// skip
				break;
				
			case MOTOR_PHASE_DEGREE_300:
			    MOTOR_DRV_HS_A = g_u16_hs_pwm_empty;	// skip
			    MOTOR_DRV_HS_B = g_u16_throttle_pos_in_pwm_duty_current;		// skip
			    MOTOR_DRV_HS_C = g_u16_hs_pwm_empty;	// skip
			    MOTOR_DRV_LS_A = g_u16_ls_pwm_full;
			    MOTOR_DRV_LS_B = g_u16_ls_pwm_empty;	// skip
			    MOTOR_DRV_LS_C = g_u16_ls_pwm_empty;
				break;
				
			case MOTOR_PHASE_DEGREE_360:
			    MOTOR_DRV_HS_A = g_u16_hs_pwm_empty;	// skip
			    MOTOR_DRV_HS_B = g_u16_hs_pwm_empty;
			    MOTOR_DRV_HS_C = g_u16_throttle_pos_in_pwm_duty_current;
			    MOTOR_DRV_LS_A = g_u16_ls_pwm_full;	// skip
			    MOTOR_DRV_LS_B = g_u16_ls_pwm_empty;	// skip
			    MOTOR_DRV_LS_C = g_u16_ls_pwm_empty;	// skip
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
	else if (g_throttle_direction == THROTTLE_DIRECTION_CW) {
		switch (g_motor_phase_current) {
			case MOTOR_PHASE_DEGREE_60:
			    MOTOR_DRV_HS_A = g_u16_throttle_pos_in_pwm_duty_current;
			    MOTOR_DRV_HS_B = g_u16_hs_pwm_empty;
			    MOTOR_DRV_HS_C = g_u16_hs_pwm_empty;	// skip
			    MOTOR_DRV_LS_A = g_u16_ls_pwm_empty;	// skip
			    MOTOR_DRV_LS_B = g_u16_ls_pwm_empty;	// skip
			    MOTOR_DRV_LS_C = g_u16_ls_pwm_full;	// skip
				break;
				
			case MOTOR_PHASE_DEGREE_120:
			    MOTOR_DRV_HS_A = g_u16_hs_pwm_empty;	// skip
			    MOTOR_DRV_HS_B = g_u16_throttle_pos_in_pwm_duty_current;		// skip
			    MOTOR_DRV_HS_C = g_u16_hs_pwm_empty;	// skip
			    MOTOR_DRV_LS_A = g_u16_ls_pwm_empty;
			    MOTOR_DRV_LS_B = g_u16_ls_pwm_empty;	// skip
			    MOTOR_DRV_LS_C = g_u16_ls_pwm_full;
				break;
				
			case MOTOR_PHASE_DEGREE_180:
			    MOTOR_DRV_HS_A = g_u16_hs_pwm_empty;	// skip
			    MOTOR_DRV_HS_B = g_u16_throttle_pos_in_pwm_duty_current;
			    MOTOR_DRV_HS_C = g_u16_hs_pwm_empty;
			    MOTOR_DRV_LS_A = g_u16_ls_pwm_full;	// skip
			    MOTOR_DRV_LS_B = g_u16_ls_pwm_empty;	// skip
			    MOTOR_DRV_LS_C = g_u16_ls_pwm_empty;	// skip
				break;
				
			case MOTOR_PHASE_DEGREE_240:
			    MOTOR_DRV_HS_A = g_u16_hs_pwm_empty;	// skip
			    MOTOR_DRV_HS_B = g_u16_hs_pwm_empty;	// skip
			    MOTOR_DRV_HS_C = g_u16_throttle_pos_in_pwm_duty_current;		// skip
			    MOTOR_DRV_LS_A = g_u16_ls_pwm_full;
			    MOTOR_DRV_LS_B = g_u16_ls_pwm_empty;
			    MOTOR_DRV_LS_C = g_u16_ls_pwm_empty;	// skip
				break;
				
			case MOTOR_PHASE_DEGREE_300:
			    MOTOR_DRV_HS_A = g_u16_hs_pwm_empty;
			    MOTOR_DRV_HS_B = g_u16_hs_pwm_empty;	// skip
			    MOTOR_DRV_HS_C = g_u16_throttle_pos_in_pwm_duty_current;
			    MOTOR_DRV_LS_A = g_u16_ls_pwm_empty;	// skip
			    MOTOR_DRV_LS_B = g_u16_ls_pwm_full;	// skip
			    MOTOR_DRV_LS_C = g_u16_ls_pwm_empty;	// skip
				break;
				
			case MOTOR_PHASE_DEGREE_360:
			    MOTOR_DRV_HS_A = g_u16_throttle_pos_in_pwm_duty_current;		// skip
			    MOTOR_DRV_HS_B = g_u16_hs_pwm_empty;	// skip
			    MOTOR_DRV_HS_C = g_u16_hs_pwm_empty;	// skip
			    MOTOR_DRV_LS_A = g_u16_ls_pwm_empty;
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
	else if (g_throttle_direction == THROTTLE_DIRECTION_NEUTRAL) {
		MOTOR_DRV_HS_A = g_u16_hs_pwm_empty;
		MOTOR_DRV_HS_B = g_u16_hs_pwm_empty;
		MOTOR_DRV_HS_C = g_u16_hs_pwm_empty;
		MOTOR_DRV_LS_A = g_u16_ls_pwm_empty;
		MOTOR_DRV_LS_B = g_u16_ls_pwm_empty;
		MOTOR_DRV_LS_C = g_u16_ls_pwm_empty;
	}
	
	PIN_MOTOR_DRIVER_ENABLE = MOTOR_ENABLE;
	R_TMR_RD0_Start ( );
	R_TMR_RD1_Start ( );
}

void hall_sensor_disable (void) {
	R_INTC1_Stop ( );
	R_INTC2_Stop ( );
	R_INTC3_Stop ( );
}

void hall_sensor_enable (void) {
	R_INTC1_Start ( );
	R_INTC2_Start ( );
	R_INTC3_Start ( );
}

/***********************************************************************************************************************/
void app_init (void) {
	g_app_state = APP_STATE_INIT;
}

/***********************************************************************************************************************/
void app_config (void) {
	uint16_t	index;
	uint16_t	tmp;
	
	/* Throttle Input */
	g_u16_throttle_pos_max 				= THROTTLE_POS_MAX;
	g_u16_throttle_pos_neutral			= THROTTLE_POS_NEUTRAL;
	g_u16_throttle_pos_min				= THROTTLE_POS_MIN;
	g_u16_throttle_pos_neutral_tolerance_percentage = THROTTLE_POS_NEUTRAL_TOLERANCE_PERCENTAGE;
	g_throttle_direction				= THROTTLE_DIRECTION_NEUTRAL;
	tmp = (uint16_t) g_u16_throttle_pos_neutral;
	tmp -= g_u16_throttle_pos_min;
	tmp /= 100U;
	g_u16_throttle_pos_neutral_upper = g_u16_throttle_pos_neutral;
	for (index=0; index<THROTTLE_POS_NEUTRAL_TOLERANCE_PERCENTAGE; index++)
		 g_u16_throttle_pos_neutral_upper += tmp;
	g_u16_throttle_pos_neutral_lower = g_u16_throttle_pos_neutral;
	for (index=0; index<THROTTLE_POS_NEUTRAL_TOLERANCE_PERCENTAGE; index++)
		 g_u16_throttle_pos_neutral_lower -= tmp;

	g_u16_throttle_pos_sample_tolerance = tmp * THROTTLE_POS_SAMPLE_TOLERANCE_PERCENTAGE;
	
	g_u16_hs_pwm_full = TRDGRA0;
	g_u16_hs_pwm_empty = TRDGRA0 + 1U;
	g_u16_ls_pwm_full = TRDGRA1;
	g_u16_ls_pwm_empty = TRDGRA1 + 1U;
	g_u16_throttle_pos_in_pwm_duty_current = g_u16_hs_pwm_empty;
	g_u16_throttle_pos_in_pwm_duty_last = g_u16_hs_pwm_empty;
}

/***********************************************************************************************************************/
void app_handler (void) {
	switch (g_app_state) {
		case APP_STATE_INIT:
			app_config ( );
			g_app_state = APP_STATE_MOTOR_CONTROL_PRE_IDLE;
			break;
			
		case APP_STATE_MOTOR_CONTROL_PRE_IDLE:
			motor_driver_disable ( );
			hall_sensor_disable ( );
			g_app_state = APP_STATE_MOTOR_CONTROL_IDLE;
			break;
			
		case APP_STATE_MOTOR_CONTROL_IDLE:
			break;
			
		case APP_STATE_MOTOR_CONTROL_PRE_ZERO_DRIVING:
			hall_sensor_enable ( );
			motor_driver_enable ( );
			g_app_state = APP_STATE_MOTOR_CONTROL_ZERO_DRIVING;
			break;
			
		case APP_STATE_MOTOR_CONTROL_ZERO_DRIVING:
			break;

		case APP_STATE_MOTOR_CONTROL_PRE_TURBO_DRIVING:
			break;
			
		case APP_STATE_MOTOR_CONTROL_TURBO_DRIVING:
			break;
			
		default:
			break;
	}
}

/***********************************************************************************************************************/

/* End user code. Do not edit comment generated here */
