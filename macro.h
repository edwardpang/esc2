#include "r_cg_macrodriver.h"

/****************************************************************************************************/
#define MACRO_TURN_ON_TIMER0	{ \
    TMIF00 = 0U; \
    TMMK00 = 0U; \
    TS0 |= _0001_TAU_CH0_START_TRG_ON; \
	g_bit_turbo_timer0_busy = 1; \
}

#define MACRO_TURN_OFF_TIMER0	{ \
    TT0 |= _0001_TAU_CH0_STOP_TRG_ON; \
    TMMK00 = 1U; \
    TMIF00 = 0U; \
	g_bit_turbo_timer0_busy = 0; \
}

#define MACRO_TURN_ON_TIMER1	{ \
    TMIF01 = 0U; \
    TMMK01 = 0U; \
    TS0 |= _0002_TAU_CH1_START_TRG_ON; \
	g_bit_turbo_timer1_busy = 1; \
}

#define MACRO_TURN_OFF_TIMER1	{ \
    TT0 |= _0002_TAU_CH1_STOP_TRG_ON; \
    TMMK01 = 1U; \
    TMIF01 = 0U; \
	g_bit_turbo_timer1_busy = 0; \
}

/****************************************************************************************************/
#define MACRO_MOTOR_DRIVE_SWITCH_PHASE_OFF { \
	PM1 |= (PIN_MOTOR_DRV_HS_A | PIN_MOTOR_DRV_HS_B | PIN_MOTOR_DRV_HS_C | \
			PIN_MOTOR_DRV_LS_A | PIN_MOTOR_DRV_LS_B | PIN_MOTOR_DRV_LS_C); }

#define MACRO_MOTOR_DRIVE_SWITCH_PHASE_UNKNOWN MACRO_MOTOR_DRIVE_SWITCH_PHASE_OFF

/****************************************************************************************************/
#define MACRO_MOTOR_DRIVE_FWD_SWITCH_PHASE_DEGREE_60 { \
	PM1 |= (PIN_MOTOR_DRV_HS_A | PIN_MOTOR_DRV_HS_B | PIN_MOTOR_DRV_LS_A | PIN_MOTOR_DRV_LS_C); \
	PM1 &= ~(PIN_MOTOR_DRV_HS_C | PIN_MOTOR_DRV_LS_B); }

#define MACRO_MOTOR_DRIVE_FWD_SWITCH_PHASE_DEGREE_120 { \
	PM1 |= (PIN_MOTOR_DRV_HS_B | PIN_MOTOR_DRV_HS_C | PIN_MOTOR_DRV_LS_A | PIN_MOTOR_DRV_LS_C); \
	PM1 &= ~(PIN_MOTOR_DRV_HS_A | PIN_MOTOR_DRV_LS_B); }

#define MACRO_MOTOR_DRIVE_FWD_SWITCH_PHASE_DEGREE_180 { \
	PM1 |= (PIN_MOTOR_DRV_HS_B | PIN_MOTOR_DRV_HS_C | PIN_MOTOR_DRV_LS_A | PIN_MOTOR_DRV_LS_B); \
	PM1 &= ~(PIN_MOTOR_DRV_HS_A | PIN_MOTOR_DRV_LS_C); }

#define MACRO_MOTOR_DRIVE_FWD_SWITCH_PHASE_DEGREE_240 { \
	PM1 |= (PIN_MOTOR_DRV_HS_A | PIN_MOTOR_DRV_HS_C | PIN_MOTOR_DRV_LS_A | PIN_MOTOR_DRV_LS_B); \
	PM1 &= ~(PIN_MOTOR_DRV_HS_B | PIN_MOTOR_DRV_LS_C); }

#define MACRO_MOTOR_DRIVE_FWD_SWITCH_PHASE_DEGREE_300 {\
	PM1 |= (PIN_MOTOR_DRV_HS_A | PIN_MOTOR_DRV_HS_C | PIN_MOTOR_DRV_LS_B | PIN_MOTOR_DRV_LS_C); \
	PM1 &= ~(PIN_MOTOR_DRV_HS_B | PIN_MOTOR_DRV_LS_A); }

#define MACRO_MOTOR_DRIVE_FWD_SWITCH_PHASE_DEGREE_360 {\
	PM1 |= (PIN_MOTOR_DRV_HS_A | PIN_MOTOR_DRV_HS_B | PIN_MOTOR_DRV_LS_B | PIN_MOTOR_DRV_LS_C); \
	PM1 &= ~(PIN_MOTOR_DRV_HS_C | PIN_MOTOR_DRV_LS_A); }

#define MACRO_MOTOR_DRIVE_SWITCH_PHASE_UNKNOWN { \
	PM1 |= (PIN_MOTOR_DRV_HS_A | PIN_MOTOR_DRV_HS_B | PIN_MOTOR_DRV_HS_C | \
			PIN_MOTOR_DRV_LS_A | PIN_MOTOR_DRV_LS_B | PIN_MOTOR_DRV_LS_C); }

/****************************************************************************************************/
#define MACRO_MOTOR_DRIVE_REV_SWITCH_PHASE_DEGREE_60 { \
	PM1 |= (PIN_MOTOR_DRV_HS_B | PIN_MOTOR_DRV_HS_C | PIN_MOTOR_DRV_LS_A | PIN_MOTOR_DRV_LS_B); \
	PM1 &= ~(PIN_MOTOR_DRV_HS_A | PIN_MOTOR_DRV_LS_C); }

#define MACRO_MOTOR_DRIVE_REV_SWITCH_PHASE_DEGREE_120 { \
	PM1 |= (PIN_MOTOR_DRV_HS_A | PIN_MOTOR_DRV_HS_C | PIN_MOTOR_DRV_LS_A | PIN_MOTOR_DRV_LS_B); \
	PM1 &= ~(PIN_MOTOR_DRV_HS_B | PIN_MOTOR_DRV_LS_C); }

#define MACRO_MOTOR_DRIVE_REV_SWITCH_PHASE_DEGREE_180 { \
	PM1 |= (PIN_MOTOR_DRV_HS_A | PIN_MOTOR_DRV_HS_C | PIN_MOTOR_DRV_LS_B | PIN_MOTOR_DRV_LS_C); \
	PM1 &= ~(PIN_MOTOR_DRV_HS_B | PIN_MOTOR_DRV_LS_A); }

#define MACRO_MOTOR_DRIVE_REV_SWITCH_PHASE_DEGREE_240 { \
	PM1 |= (PIN_MOTOR_DRV_HS_A | PIN_MOTOR_DRV_HS_B | PIN_MOTOR_DRV_LS_B | PIN_MOTOR_DRV_LS_C); \
	PM1 &= ~(PIN_MOTOR_DRV_HS_C | PIN_MOTOR_DRV_LS_A); }

#define MACRO_MOTOR_DRIVE_REV_SWITCH_PHASE_DEGREE_300 { \
	PM1 |= (PIN_MOTOR_DRV_HS_A | PIN_MOTOR_DRV_HS_B | PIN_MOTOR_DRV_LS_A | PIN_MOTOR_DRV_LS_C); \
	PM1 &= ~(PIN_MOTOR_DRV_HS_C | PIN_MOTOR_DRV_LS_B); }

#define MACRO_MOTOR_DRIVE_REV_SWITCH_PHASE_DEGREE_360 { \
	PM1 |= (PIN_MOTOR_DRV_HS_B | PIN_MOTOR_DRV_HS_C | PIN_MOTOR_DRV_LS_A | PIN_MOTOR_DRV_LS_C); \
	PM1 &= ~(PIN_MOTOR_DRV_HS_A | PIN_MOTOR_DRV_LS_B); }

#define MACRO_MOTOR_DRIVE_REV_SWITCH_PHASE_UNKNOWN { \
	PM1 |= (PIN_MOTOR_DRV_HS_A | PIN_MOTOR_DRV_HS_B | PIN_MOTOR_DRV_HS_C | \
			PIN_MOTOR_DRV_LS_A | PIN_MOTOR_DRV_LS_B | PIN_MOTOR_DRV_LS_C); }

/****************************************************************************************************/
#define MACRO_GET_AVERAGE_SPEED	 \
	((g_u16_speed_count_us_degree_60 + g_u16_speed_count_us_degree_120 + g_u16_speed_count_us_degree_180 + \
	g_u16_speed_count_us_degree_240 + g_u16_speed_count_us_degree_300 + g_u16_speed_count_us_degree_360) / 6)

/****************************************************************************************************/
