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
* File Name    : r_cg_userdefine.h
* Version      : CodeGenerator for RL78/G14 V2.01.00.02 [09 Aug 2013]
* Device(s)    : R5F104BA
* Tool-Chain   : CA78K0R
* Description  : This file includes user definition.
* Creation Date: 27/01/2014
***********************************************************************************************************************/

#ifndef _USER_DEF_H
#define _USER_DEF_H

/***********************************************************************************************************************
User definitions
***********************************************************************************************************************/

/* Start user code for function. Do not edit comment generated here */
#include "r_cg_timer.h"

#define MOTOR_DRIVER_CONF_HS_ACTIVE_H
//#define MOTOR_DRIVER_CONF_HS_ACTIVE_L

#define MOTOR_DRIVER_CONF_LS_ACTIVE_H
//#define MOTOR_DRIVER_CONF_LS_ACTIVE_L

#define MOTOR_DRV_HS_PERIOD		TRDGRA0
#define MOTOR_DRV_HS_A			TRDGRB0
#define MOTOR_DRV_HS_B			TRDGRC0
#define MOTOR_DRV_HS_C			TRDGRD0
#define MOTOR_DRV_LS_PERIOD		TRDGRA1
#define MOTOR_DRV_LS_A			TRDGRB1
#define MOTOR_DRV_LS_B			TRDGRC1
#define MOTOR_DRV_LS_C			TRDGRD1

#define PIN_MOTOR_DRV_HS_A		(0x01U << 5)
#define PIN_MOTOR_DRV_HS_B		(0x01U << 6)
#define PIN_MOTOR_DRV_HS_C		(0x01U << 4)
#define PIN_MOTOR_DRV_LS_A		(0x01U << 2)
#define PIN_MOTOR_DRV_LS_B		(0x01U << 1)
#define PIN_MOTOR_DRV_LS_C		(0x01U << 0)

#define PIN_DEBUG					P1.3
#define PIN_HALL_SENSOR_A			P5.0
#define PIN_HALL_SENSOR_B			P5.1
#define PIN_HALL_SENSOR_C			P3.0
#define PIN_LED_RED					P6.2
#define PIN_LED_GREEN				P14.7
#define PIN_MOTOR_DRIVER_ENABLE		P7.0

#define THROTTLE_POS_SHIFT_BIT				((uint16_t) (0U))
#define THROTTLE_POS_MAX					((uint16_t) (64000U))
#define THROTTLE_POS_NEUTRAL				((uint16_t) (48000U))
#define THROTTLE_POS_MIN					((uint16_t) (32000U))
#define THROTTLE_POS_NEUTRAL_TOLERANCE_PERCENTAGE	((uint16_t) (5U))
#define THROTTLE_POS_SAMPLE_TOLERANCE_PERCENTAGE	((uint16_t)	(1U))

#define MOTOR_ENABLE			1
#define MOTOR_DISABLE			0

#define MOTOR_DRIVE_HS_PWM		1
#define MOTOR_DRIVE_LS_PWM		0

#define MOTOR_DRIVE_ACTIVE_HIGH	1
#define MOTOR_DRIVE_ACTIVE_LOW	0

#define BREAK_PERIOD			(0xF9FFU)	// 0xFA00 * 0.03125 = 1ms

typedef enum _throttle_direction_t {
	THROTTLE_DIRECTION_CW = 0,
	THROTTLE_DIRECTION_CCW,
	THROTTLE_DIRECTION_NEUTRAL,
	TRHOTTLE_DIRECTION_NUM
} throttle_direction_t;

typedef enum _motor_phase_t {
	MOTOR_PHASE_DEGREE_60 = 0,
	MOTOR_PHASE_DEGREE_120,
	MOTOR_PHASE_DEGREE_180,
	MOTOR_PHASE_DEGREE_240,
	MOTOR_PHASE_DEGREE_300,
	MOTOR_PHASE_DEGREE_360,
	MOTOR_PHASE_ERROR,
	MOTOR_PHASE_OPEN,
	MOTOR_PHASE_NUM
} motor_phase_t;

typedef enum _app_state_t {
	APP_STATE_START = 0,
	APP_STATE_FACTORY_INIT,
	APP_STATE_FACTORY_RESET,
	APP_STATE_FACTORY_MODE,
	
	APP_STATE_INIT,
	
	APP_STATE_MOTOR_CONTROL_PRE_IDLE,	
	APP_STATE_MOTOR_CONTROL_IDLE,
	APP_STATE_MOTOR_CONTROL_POST_IDLE,	

	APP_STATE_MOTOR_CONTROL_PRE_IDLE_REVERSIBLE,	
	APP_STATE_MOTOR_CONTROL_IDLE_REVERSIBLE,
	APP_STATE_MOTOR_CONTROL_POST_IDLE_REVERSIBLE,
	
	APP_STATE_MOTOR_CONTROL_PRE_BREAK,
	APP_STATE_MOTOR_CONTROL_BREAK,
	APP_STATE_MOTOR_CONTROL_POST_BREAK,

	APP_STATE_MOTOR_CONTROL_PRE_FWD_DRIVING,
	APP_STATE_MOTOR_CONTROL_FWD_DRIVING,
	APP_STATE_MOTOR_CONTROL_POST_FWD_DRIVING,

	APP_STATE_MOTOR_CONTROL_PRE_REV_DRIVING,
	APP_STATE_MOTOR_CONTROL_REV_DRIVING,
	APP_STATE_MOTOR_CONTROL_POST_REV_DRIVING,
	
	APP_STATE_MOTOR_CONTROL_PRE_TURBO_DRIVING,
	APP_STATE_MOTOR_CONTROL_TURBO_DRIVING,
	APP_STATE_MOTOR_CONTROL_POST_TURBO_DRIVING,

	APP_STATE_NUM
} app_state_t;

#define COM_PREAMBLE	0xA5
#define COM_TERMINATOR	0x5A

typedef enum _com_command_t {
	COM_COMMAND_00_GET_STATUS = 0x00,
	COM_COMMAND_NUM
};

typedef enum _com_state_t {
	COM_STATE_START = 0,
	COM_STATE_INIT,
	COM_STATE_IDLE,
	COM_STATE_WAIT_PREAMBLE,
	COM_STATE_RX_PREAMBLE,
	COM_STATE_WAIT_LENGTH,
	COM_STATE_RX_LENGTH,
	COM_STATE_WAIT_COMMAND,
	COM_STATE_RX_COMMAND,
	COM_STATE_WAIT_PARAMETER,
	COM_STATE_RX_PARAMETER,
	COM_STATE_WAIT_TERMINATOR,
	COM_STATE_RX_TERMINATOR,
	COM_STATE_NUM
} com_state_t;

#define SPEED_1US_TIMER_RESET_VALUE		_001F_TAU_TDR03_VALUE
#define SPEED_1US_COUNTER_RESET_VALUE	_270F_TMRJ_TRJ0_VALUE

#define TURBO_DRIVE_PHASE_SPEED_IN_US_MIDDLE				(2000U)
#define TURBO_DRIVE_PHASE_SPEED_IN_US_TOLERENCE_PERCENTAGE	(5U)
#define TURBO_DRIVE_PHASE_SPEED_1US_RESET_VALUE				(0x001FU)
#define TURBO_DRIVE_PHASE_COUNT_TO_START					(2U)
#define TURBO_DRIVE_TEST_DEGREE								(50U)
#define TURBO_DRIVE_PHASE_DEGREE							(60U)
/* End user code. Do not edit comment generated here */
#endif
