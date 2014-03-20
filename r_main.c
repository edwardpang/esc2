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
* Creation Date: 27/01/2014
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
#include "r_cg_it.h"
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
motor_phase_t	g_motor_phase_set_timer0;
motor_phase_t	g_motor_phase_set_timer1;
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

com_state_t	g_com_state;
uint8_t		g_u8_length, g_u8_command;

#define RX_BUF_SIZE		64
#define TX_BUF_SIZE		64
bit g_bit_rx_busy, g_bit_tx_busy;
uint8_t	g_u8_tx_buf[TX_BUF_SIZE];
uint8_t	g_u8_rx_buf[RX_BUF_SIZE];

uint16_t	g_u16_speed_count_us;
uint16_t	g_u16_speed_count_us_degree_60;
uint16_t	g_u16_speed_count_us_degree_120;
uint16_t	g_u16_speed_count_us_degree_180;
uint16_t	g_u16_speed_count_us_degree_240;
uint16_t	g_u16_speed_count_us_degree_300;
uint16_t	g_u16_speed_count_us_degree_360;

uint16_t	g_u16_turbo_drive_phase_speed_in_us_middle;
uint16_t	g_u16_turbo_drive_phase_speed_in_us_leave;
uint16_t	g_u16_turbo_drive_phase_speed_in_us_enter;
uint16_t	g_u16_turbo_drive_phase_speed_in_us_tolerence_percentage;
uint16_t	g_u16_turbo_drive_phase_speed_in_us_tolerence;
uint8_t		g_u8_turbo_drive_phase_count_to_start;
uint8_t		g_u8_turbo_drive_phase_count;
bit			g_bit_turbo_drive_start;

uint32_t	g_u32_tick;
bit			g_bit_tick_overflow;

void motor_driver_enable (void);
void motor_driver_disable (void);
void app_init (void);
void app_config (void);
void app_handler (void);
void com_init (void);
void com_handler (void);
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
		com_handler ( );
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
	com_init ( );
    EI();
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/***********************************************************************************************************************/
void motor_phase_reset (void) {
	uint16_t	index;
	
	index = (PIN_HALL_SENSOR_A << 2);
	index |= (PIN_HALL_SENSOR_B << 1);
	index |= (PIN_HALL_SENSOR_C);
	g_motor_phase_current = abc2phase[index];
	g_motor_phase_set_timer0 = g_motor_phase_current;
	g_motor_phase_set_timer1 = g_motor_phase_current;
}

/***********************************************************************************************************************/
void motor_driver_disable (void) {
	PIN_MOTOR_DRIVER_ENABLE = MOTOR_DISABLE;
	PM1 |= (PIN_MOTOR_DRV_HS_A |PIN_MOTOR_DRV_HS_B | PIN_MOTOR_DRV_HS_C | 
			PIN_MOTOR_DRV_LS_A | PIN_MOTOR_DRV_LS_B | PIN_MOTOR_DRV_LS_C);
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
void motor_driver_farward_enable (void) {
	
	motor_phase_reset ( );
	
	MOTOR_DRV_LS_PERIOD = g_u16_ls_pwm_full;

	MOTOR_DRV_HS_A = g_u16_throttle_pos_in_pwm_duty_current;
	MOTOR_DRV_HS_B = g_u16_throttle_pos_in_pwm_duty_current;
	MOTOR_DRV_HS_C = g_u16_throttle_pos_in_pwm_duty_current;
	MOTOR_DRV_LS_A = g_u16_ls_pwm_full;
	MOTOR_DRV_LS_B = g_u16_ls_pwm_full;
	MOTOR_DRV_LS_C = g_u16_ls_pwm_full;
	switch (g_motor_phase_current) {
		case MOTOR_PHASE_DEGREE_60:
			PM1 |= (PIN_MOTOR_DRV_HS_A | PIN_MOTOR_DRV_HS_B | PIN_MOTOR_DRV_LS_A | PIN_MOTOR_DRV_LS_C);
			PM1 &= ~(PIN_MOTOR_DRV_HS_C | PIN_MOTOR_DRV_LS_B);
			break;
			
		case MOTOR_PHASE_DEGREE_120:
			PM1 |= (PIN_MOTOR_DRV_HS_B | PIN_MOTOR_DRV_HS_C | PIN_MOTOR_DRV_LS_A | PIN_MOTOR_DRV_LS_C);
			PM1 &= ~(PIN_MOTOR_DRV_HS_A | PIN_MOTOR_DRV_LS_B);
			break;
			
		case MOTOR_PHASE_DEGREE_180:
			PM1 |= (PIN_MOTOR_DRV_HS_B | PIN_MOTOR_DRV_HS_C | PIN_MOTOR_DRV_LS_A | PIN_MOTOR_DRV_LS_B);
			PM1 &= ~(PIN_MOTOR_DRV_HS_A | PIN_MOTOR_DRV_LS_C);
			break;
			
		case MOTOR_PHASE_DEGREE_240:
			PM1 |= (PIN_MOTOR_DRV_HS_A | PIN_MOTOR_DRV_HS_C | PIN_MOTOR_DRV_LS_A | PIN_MOTOR_DRV_LS_B);
			PM1 &= ~(PIN_MOTOR_DRV_HS_B | PIN_MOTOR_DRV_LS_C);
			break;
			
		case MOTOR_PHASE_DEGREE_300:
			PM1 |= (PIN_MOTOR_DRV_HS_A | PIN_MOTOR_DRV_HS_C | PIN_MOTOR_DRV_LS_B | PIN_MOTOR_DRV_LS_C);
			PM1 &= ~(PIN_MOTOR_DRV_HS_B | PIN_MOTOR_DRV_LS_A);
			break;
			
		case MOTOR_PHASE_DEGREE_360:
			PM1 |= (PIN_MOTOR_DRV_HS_A | PIN_MOTOR_DRV_HS_B | PIN_MOTOR_DRV_LS_B | PIN_MOTOR_DRV_LS_C);
			PM1 &= ~(PIN_MOTOR_DRV_HS_C | PIN_MOTOR_DRV_LS_A);
			break;

		case MOTOR_PHASE_OPEN:
		case MOTOR_PHASE_ERROR:
		default:
			break;
	}
	
	PIN_MOTOR_DRIVER_ENABLE = MOTOR_ENABLE;
	R_TMR_RD0_Start ( );
	R_TMR_RD1_Start ( );
}

/***********************************************************************************************************************/
void motor_driver_reverse_enable (void) {

	motor_phase_reset ( );
	
	MOTOR_DRV_LS_PERIOD = g_u16_ls_pwm_full;

	MOTOR_DRV_HS_A = g_u16_throttle_pos_in_pwm_duty_current;
	MOTOR_DRV_HS_B = g_u16_throttle_pos_in_pwm_duty_current;
	MOTOR_DRV_HS_C = g_u16_throttle_pos_in_pwm_duty_current;
	MOTOR_DRV_LS_A = g_u16_ls_pwm_full;
	MOTOR_DRV_LS_B = g_u16_ls_pwm_full;
	MOTOR_DRV_LS_C = g_u16_ls_pwm_full;
	switch (g_motor_phase_current) {
		case MOTOR_PHASE_DEGREE_60:
			PM1 |= (PIN_MOTOR_DRV_HS_B | PIN_MOTOR_DRV_HS_C | PIN_MOTOR_DRV_LS_A | PIN_MOTOR_DRV_LS_B);
			PM1 &= ~(PIN_MOTOR_DRV_HS_A | PIN_MOTOR_DRV_LS_C);
			break;
			
		case MOTOR_PHASE_DEGREE_120:
			PM1 |= (PIN_MOTOR_DRV_HS_A | PIN_MOTOR_DRV_HS_C | PIN_MOTOR_DRV_LS_A | PIN_MOTOR_DRV_LS_B);
			PM1 &= ~(PIN_MOTOR_DRV_HS_B | PIN_MOTOR_DRV_LS_C);
			break;
			
		case MOTOR_PHASE_DEGREE_180:
			PM1 |= (PIN_MOTOR_DRV_HS_A | PIN_MOTOR_DRV_HS_C | PIN_MOTOR_DRV_LS_B | PIN_MOTOR_DRV_LS_C);
			PM1 &= ~(PIN_MOTOR_DRV_HS_B | PIN_MOTOR_DRV_LS_A);
			break;
			
		case MOTOR_PHASE_DEGREE_240:
			PM1 |= (PIN_MOTOR_DRV_HS_A | PIN_MOTOR_DRV_HS_B | PIN_MOTOR_DRV_LS_B | PIN_MOTOR_DRV_LS_C);
			PM1 &= ~(PIN_MOTOR_DRV_HS_C | PIN_MOTOR_DRV_LS_A);
			break;
			
		case MOTOR_PHASE_DEGREE_300:
			PM1 |= (PIN_MOTOR_DRV_HS_A | PIN_MOTOR_DRV_HS_B | PIN_MOTOR_DRV_LS_A | PIN_MOTOR_DRV_LS_C);
			PM1 &= ~(PIN_MOTOR_DRV_HS_C | PIN_MOTOR_DRV_LS_B);
			break;
			
		case MOTOR_PHASE_DEGREE_360:
			PM1 |= (PIN_MOTOR_DRV_HS_B | PIN_MOTOR_DRV_HS_C | PIN_MOTOR_DRV_LS_A | PIN_MOTOR_DRV_LS_C);
			PM1 &= ~(PIN_MOTOR_DRV_HS_A | PIN_MOTOR_DRV_LS_B);
			break;

		case MOTOR_PHASE_OPEN:
		case MOTOR_PHASE_ERROR:
		default:
			break;
	}
	
	PIN_MOTOR_DRIVER_ENABLE = MOTOR_ENABLE;
	R_TMR_RD0_Start ( );
	R_TMR_RD1_Start ( );
}

/***********************************************************************************************************************/
void motor_driver_break_enable (void) {
   	MOTOR_DRV_HS_A = g_u16_hs_pwm_empty;
	MOTOR_DRV_HS_B = g_u16_hs_pwm_empty;
	MOTOR_DRV_HS_C = g_u16_hs_pwm_empty;
	MOTOR_DRV_LS_PERIOD = BREAK_PERIOD;
    MOTOR_DRV_LS_A = (g_u16_throttle_pos_in_pwm_duty_current << 2);
    MOTOR_DRV_LS_B = (g_u16_throttle_pos_in_pwm_duty_current << 2);
    MOTOR_DRV_LS_C = (g_u16_throttle_pos_in_pwm_duty_current << 2);
	PM1 &= ~(PIN_MOTOR_DRV_HS_A |PIN_MOTOR_DRV_HS_B | PIN_MOTOR_DRV_HS_C | 
			PIN_MOTOR_DRV_LS_A | PIN_MOTOR_DRV_LS_B | PIN_MOTOR_DRV_LS_C);
	PIN_MOTOR_DRIVER_ENABLE = MOTOR_ENABLE;
	R_TMR_RD0_Start ( );
	R_TMR_RD1_Start ( );
}

/***********************************************************************************************************************/
void hall_sensor_disable (void) {
	R_INTC1_Stop ( );
	R_INTC2_Stop ( );
	R_INTC3_Stop ( );
}

/***********************************************************************************************************************/
void hall_sensor_enable (void) {
	R_INTC1_Start ( );
	R_INTC2_Start ( );
	R_INTC3_Start ( );
}

/***********************************************************************************************************************/
void throttle_enable (void) {
	R_TAU0_Channel2_Start ( );
}

/***********************************************************************************************************************/
void throttle_disable (void) {
	R_TAU0_Channel2_Stop ( );	
}

/***********************************************************************************************************************/
void speed_measurement_enable ( ) {
	TRJ0 = SPEED_1US_COUNTER_RESET_VALUE;
	TDR03 = SPEED_1US_TIMER_RESET_VALUE;
	g_u16_speed_count_us = 0;
	R_TMR_RJ0_Start ( );
	R_TAU0_Channel3_Start ( );
}

void speed_measurement_disable ( ) {
	R_TAU0_Channel3_Stop ( );
	R_TMR_RJ0_Stop ( );
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
	
	g_u16_hs_pwm_full = MOTOR_DRV_HS_PERIOD;
	g_u16_hs_pwm_empty = MOTOR_DRV_HS_PERIOD + 1U;
	g_u16_ls_pwm_full = MOTOR_DRV_LS_PERIOD;
	g_u16_ls_pwm_empty = MOTOR_DRV_LS_PERIOD + 1U;
	g_u16_throttle_pos_in_pwm_duty_current = g_u16_hs_pwm_empty;
	g_u16_throttle_pos_in_pwm_duty_last = g_u16_hs_pwm_empty;

	g_u16_turbo_drive_phase_speed_in_us_middle = TURBO_DRIVE_PHASE_SPEED_IN_US_MIDDLE;
	g_u16_turbo_drive_phase_speed_in_us_tolerence_percentage = TURBO_DRIVE_PHASE_SPEED_IN_US_TOLERENCE_PERCENTAGE;
	g_u16_turbo_drive_phase_speed_in_us_tolerence = g_u16_turbo_drive_phase_speed_in_us_middle * g_u16_turbo_drive_phase_speed_in_us_tolerence_percentage / 100U;
	g_u16_turbo_drive_phase_speed_in_us_leave = g_u16_turbo_drive_phase_speed_in_us_middle + g_u16_turbo_drive_phase_speed_in_us_tolerence;
	g_u16_turbo_drive_phase_speed_in_us_enter = g_u16_turbo_drive_phase_speed_in_us_middle - g_u16_turbo_drive_phase_speed_in_us_tolerence;
}

/***********************************************************************************************************************/
uint8_t com_process_command (void) {
	uint16_t	u16_dummy;
	uint8_t		u8_dummy;
	uint8_t		retval = 0;
	
	switch (g_u8_command) {
		case COM_COMMAND_00_GET_STATUS:
			g_u8_tx_buf[0] = COM_PREAMBLE;
			g_u8_tx_buf[1] = 18;
			g_u8_tx_buf[2] = COM_COMMAND_00_GET_STATUS;
			g_u8_tx_buf[3] = g_app_state;
			g_u8_tx_buf[4] = g_throttle_direction;
			
			u16_dummy = g_u16_throttle_pos_in_pwm_duty_current;
			u8_dummy = (uint8_t) ((u16_dummy & 0xFF00) >> 8);
			g_u8_tx_buf[5] = u8_dummy;
			u8_dummy = (uint8_t) (u16_dummy & 0x00FF);
			g_u8_tx_buf[6] = u8_dummy;

			u16_dummy = g_u16_speed_count_us_degree_60;
			u8_dummy = (uint8_t) ((u16_dummy & 0xFF00) >> 8);
			g_u8_tx_buf[7] = u8_dummy;
			u8_dummy = (uint8_t) (u16_dummy & 0x00FF);
			g_u8_tx_buf[8] = u8_dummy;

			u16_dummy = g_u16_speed_count_us_degree_120;
			u8_dummy = (uint8_t) ((u16_dummy & 0xFF00) >> 8);
			g_u8_tx_buf[9] = u8_dummy;
			u8_dummy = (uint8_t) (u16_dummy & 0x00FF);
			g_u8_tx_buf[10] = u8_dummy;

			u16_dummy = g_u16_speed_count_us_degree_180;
			u8_dummy = (uint8_t) ((u16_dummy & 0xFF00) >> 8);
			g_u8_tx_buf[11] = u8_dummy;
			u8_dummy = (uint8_t) (u16_dummy & 0x00FF);
			g_u8_tx_buf[12] = u8_dummy;

			u16_dummy = g_u16_speed_count_us_degree_240;
			u8_dummy = (uint8_t) ((u16_dummy & 0xFF00) >> 8);
			g_u8_tx_buf[13] = u8_dummy;
			u8_dummy = (uint8_t) (u16_dummy & 0x00FF);
			g_u8_tx_buf[14] = u8_dummy;

			u16_dummy = g_u16_speed_count_us_degree_300;
			u8_dummy = (uint8_t) ((u16_dummy & 0xFF00) >> 8);
			g_u8_tx_buf[15] = u8_dummy;
			u8_dummy = (uint8_t) (u16_dummy & 0x00FF);
			g_u8_tx_buf[16] = u8_dummy;

			u16_dummy = g_u16_speed_count_us_degree_360;
			u8_dummy = (uint8_t) ((u16_dummy & 0xFF00) >> 8);
			g_u8_tx_buf[17] = u8_dummy;
			u8_dummy = (uint8_t) (u16_dummy & 0x00FF);
			g_u8_tx_buf[18] = u8_dummy;

			g_u8_tx_buf[19] = COM_TERMINATOR;
			if (!g_bit_tx_busy) {
				g_bit_tx_busy = 1;
				R_UART1_Send (g_u8_tx_buf, 20);
				retval = 1;
			}			
			break;
	}
	return retval;
}

void com_init (void) {
	g_com_state = COM_STATE_INIT;
}

void com_handler (void) {
	uint8_t	i;

	switch (g_com_state) {
		case COM_STATE_START:
			break;
			
		case COM_STATE_INIT:
			g_u8_length = 0;
			g_u8_command = 0;
			
			g_bit_rx_busy = 0;
			g_bit_tx_busy = 0;
			for (i=0; i<TX_BUF_SIZE; i++)
				g_u8_tx_buf[i] = 0;
			for (i=0; i<RX_BUF_SIZE; i++)
				g_u8_rx_buf[i] = 0;
			R_UART1_Start ( );
			g_com_state = COM_STATE_IDLE;
			break;
			
		case COM_STATE_IDLE:
			if (!g_bit_rx_busy) {
				g_bit_rx_busy = 1;
				R_UART1_Receive (g_u8_rx_buf, 1);
				g_com_state = COM_STATE_WAIT_PREAMBLE;
			}
			break;
		
		case COM_STATE_WAIT_PREAMBLE:
			if (!g_bit_rx_busy) {
				if (g_u8_rx_buf[0] == COM_PREAMBLE)
					g_com_state = COM_STATE_RX_PREAMBLE;		// preamble received
				else
					g_com_state = COM_STATE_IDLE;				// discard unexpected byte
			}
			break;
			
		case COM_STATE_RX_PREAMBLE:
			if (!g_bit_rx_busy) {
				g_bit_rx_busy = 1;
				R_UART1_Receive (g_u8_rx_buf, 1);
				g_com_state = COM_STATE_WAIT_LENGTH;
			}
			break;

		case COM_STATE_WAIT_LENGTH:
			if (!g_bit_rx_busy) {
				g_u8_length = g_u8_rx_buf[0];
				g_com_state = COM_STATE_RX_LENGTH;
			}
			break;
		
		case COM_STATE_RX_LENGTH:
			if (!g_bit_rx_busy) {
				g_bit_rx_busy = 1;
				R_UART1_Receive (g_u8_rx_buf, 1);
				g_com_state = COM_STATE_WAIT_COMMAND;
			}
			break;

		case COM_STATE_WAIT_COMMAND:
			if (!g_bit_rx_busy) {
				g_u8_length --;
				g_u8_command = g_u8_rx_buf[0];
				g_com_state = COM_STATE_RX_COMMAND;		// preamble received
			}
			break;

		case COM_STATE_RX_COMMAND:
			if (!g_bit_rx_busy) {
				g_bit_rx_busy = 1;
				if (g_u8_length > 1) {
					R_UART1_Receive (g_u8_rx_buf, g_u8_length-1);
					g_com_state = COM_STATE_WAIT_PARAMETER;
				}
				else {
					R_UART1_Receive (g_u8_rx_buf, 1);
					g_com_state = COM_STATE_WAIT_TERMINATOR;
				}
			}
			break;

		case COM_STATE_WAIT_TERMINATOR:
			if (!g_bit_rx_busy) {
				if (g_u8_rx_buf[0] == COM_TERMINATOR)
					g_com_state = COM_STATE_RX_TERMINATOR;		// terminator received
				else
					g_com_state = COM_STATE_IDLE;				// discard unexpected byte
			}
			break;

		case COM_STATE_RX_TERMINATOR:
			// Only process the command and parameter in this state
			if (com_process_command ( ))
				g_com_state = COM_STATE_IDLE;
			break;
			
		default:
			break;
	}
}

/***********************************************************************************************************************/
void tick_enable (void) {
	g_u32_tick = 0;
	g_bit_tick_overflow = 0;
	R_IT_Start ( );
}

/***********************************************************************************************************************/
void app_handler (void) {
	uint16_t	average_speed;
	uint16_t	dummy;
	
	switch (g_app_state) {
		case APP_STATE_INIT:
			app_config ( );
			throttle_enable ( );
			tick_enable ( );
			g_app_state = APP_STATE_MOTOR_CONTROL_PRE_IDLE;
			break;

		/* IDLE */
		case APP_STATE_MOTOR_CONTROL_PRE_IDLE:
			speed_measurement_disable ( );
			hall_sensor_disable ( );
			motor_driver_disable ( );
			g_app_state = APP_STATE_MOTOR_CONTROL_IDLE;
			break;
			
		case APP_STATE_MOTOR_CONTROL_IDLE:
			if (g_u16_throttle_pos_in_pwm_duty_current != g_u16_hs_pwm_empty) {
				if (g_throttle_direction == THROTTLE_DIRECTION_CCW)
					g_app_state = APP_STATE_MOTOR_CONTROL_PRE_FWD_DRIVING;
				else if (g_throttle_direction == THROTTLE_DIRECTION_CW)
					g_app_state = APP_STATE_MOTOR_CONTROL_PRE_BREAK;
			}
			break;

		/* IDLE REVERSIBLE */
		case APP_STATE_MOTOR_CONTROL_PRE_IDLE_REVERSIBLE:
			speed_measurement_disable ( );
			hall_sensor_disable ( );
			motor_driver_disable ( );
			g_app_state = APP_STATE_MOTOR_CONTROL_IDLE_REVERSIBLE;
			break;
			
		case APP_STATE_MOTOR_CONTROL_IDLE_REVERSIBLE:
			if (g_u16_throttle_pos_in_pwm_duty_current != g_u16_hs_pwm_empty) {
				if (g_throttle_direction == THROTTLE_DIRECTION_CCW)
					g_app_state = APP_STATE_MOTOR_CONTROL_PRE_FWD_DRIVING;
				else if (g_throttle_direction == THROTTLE_DIRECTION_CW)
					g_app_state = APP_STATE_MOTOR_CONTROL_PRE_REV_DRIVING;
			}
			break;

		/* BREAK */
		case APP_STATE_MOTOR_CONTROL_PRE_BREAK:
			speed_measurement_enable ( );
			motor_driver_break_enable ( );
			hall_sensor_enable ( );
			g_app_state = APP_STATE_MOTOR_CONTROL_BREAK;
			break;
			
		case APP_STATE_MOTOR_CONTROL_BREAK:
			if (g_u16_throttle_pos_in_pwm_duty_current == g_u16_hs_pwm_empty)
				g_app_state = APP_STATE_MOTOR_CONTROL_PRE_IDLE_REVERSIBLE;
			else if (g_throttle_direction == THROTTLE_DIRECTION_CCW)
				g_app_state = APP_STATE_MOTOR_CONTROL_PRE_FWD_DRIVING;
			break;
		
		/* FWD */
		case APP_STATE_MOTOR_CONTROL_PRE_FWD_DRIVING:
			speed_measurement_enable ( );
			motor_driver_farward_enable ( );
			hall_sensor_enable ( );
			g_app_state = APP_STATE_MOTOR_CONTROL_FWD_DRIVING;
			break;
			
		case APP_STATE_MOTOR_CONTROL_FWD_DRIVING:
			average_speed = g_u16_speed_count_us_degree_60;
			average_speed += g_u16_speed_count_us_degree_120;
			average_speed += g_u16_speed_count_us_degree_180;
			average_speed += g_u16_speed_count_us_degree_240;
			average_speed += g_u16_speed_count_us_degree_300;
			average_speed += g_u16_speed_count_us_degree_360;
			average_speed /= 6;
			if (g_u16_throttle_pos_in_pwm_duty_current == g_u16_hs_pwm_empty)
				g_app_state = APP_STATE_MOTOR_CONTROL_PRE_IDLE;
			else if (g_throttle_direction == THROTTLE_DIRECTION_CW)
				g_app_state = APP_STATE_MOTOR_CONTROL_PRE_BREAK;
			else if ((average_speed > 0 )&& (average_speed < g_u16_turbo_drive_phase_speed_in_us_enter)) {
				g_u8_turbo_drive_phase_count_to_start = TURBO_DRIVE_PHASE_COUNT_TO_START;
				g_u8_turbo_drive_phase_count = 0;
				g_bit_turbo_drive_start = 0;
				g_app_state = APP_STATE_MOTOR_CONTROL_PRE_TURBO_DRIVING;
			}
			break;

		/* REV */
		case APP_STATE_MOTOR_CONTROL_PRE_REV_DRIVING:
			speed_measurement_enable ( );
			motor_driver_reverse_enable ( );
			hall_sensor_enable ( );
			g_app_state = APP_STATE_MOTOR_CONTROL_REV_DRIVING;
			break;
			
		case APP_STATE_MOTOR_CONTROL_REV_DRIVING:
			if (g_u16_throttle_pos_in_pwm_duty_current == g_u16_hs_pwm_empty)
				g_app_state = APP_STATE_MOTOR_CONTROL_PRE_IDLE;
			else if (g_throttle_direction == THROTTLE_DIRECTION_CCW)
				g_app_state = APP_STATE_MOTOR_CONTROL_PRE_FWD_DRIVING;
			break;
			
		/* Turbo Driving */
		case APP_STATE_MOTOR_CONTROL_PRE_TURBO_DRIVING:
			if (g_bit_turbo_drive_start) {
				g_app_state = APP_STATE_MOTOR_CONTROL_TURBO_DRIVING;
			}
			break;

		case APP_STATE_MOTOR_CONTROL_TURBO_DRIVING:
			average_speed = g_u16_speed_count_us_degree_60;
			average_speed += g_u16_speed_count_us_degree_120;
			average_speed += g_u16_speed_count_us_degree_180;
			average_speed += g_u16_speed_count_us_degree_240;
			average_speed += g_u16_speed_count_us_degree_300;
			average_speed += g_u16_speed_count_us_degree_360;
			average_speed /= 6;
			if (g_u16_throttle_pos_in_pwm_duty_current == g_u16_hs_pwm_empty)
				g_app_state = APP_STATE_MOTOR_CONTROL_PRE_IDLE;
			else if (g_throttle_direction == THROTTLE_DIRECTION_CW)
				g_app_state = APP_STATE_MOTOR_CONTROL_PRE_BREAK;
			else if (average_speed > g_u16_turbo_drive_phase_speed_in_us_leave)
				g_app_state = APP_STATE_MOTOR_CONTROL_PRE_FWD_DRIVING;
			break;

		default:
			break;
	}
}

/***********************************************************************************************************************/

/* End user code. Do not edit comment generated here */
