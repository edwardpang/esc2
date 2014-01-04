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
* File Name    : r_cg_timer.c
* Version      : CodeGenerator for RL78/G14 V2.01.00.02 [09 Aug 2013]
* Device(s)    : R5F104BA
* Tool-Chain   : CA78K0R
* Description  : This file implements device driver for TAU module.
* Creation Date: 08/12/2013
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
#include "r_cg_timer.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* For TAU0_ch2 pulse measurement */
extern volatile uint32_t g_tau0_ch2_width;
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_TAU0_Create
* Description  : This function initializes the TAU0 module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_TAU0_Create(void)
{
    TAU0EN = 1U;    /* supplies input clock */
    TPS0 = _0000_TAU_CKM0_FCLK_0 | _0000_TAU_CKM1_FCLK_0 | _0000_TAU_CKM2_FCLK_1 | _0000_TAU_CKM3_FCLK_8;
    /* Stop all channels */
    TT0 = _0001_TAU_CH0_STOP_TRG_ON | _0002_TAU_CH1_STOP_TRG_ON | _0004_TAU_CH2_STOP_TRG_ON |
          _0008_TAU_CH3_STOP_TRG_ON | _0200_TAU_CH1_H8_STOP_TRG_ON | _0800_TAU_CH3_H8_STOP_TRG_ON;
    /* Mask channel 0 interrupt */
    TMMK00 = 1U;    /* disable INTTM00 interrupt */
    TMIF00 = 0U;    /* clear INTTM00 interrupt flag */
    /* Mask channel 1 interrupt */
    TMMK01 = 1U;    /* disable INTTM01 interrupt */
    TMIF01 = 0U;    /* clear INTTM01 interrupt flag */
    /* Mask channel 1 higher 8 bits interrupt */
    TMMK01H = 1U;    /* disable INTTM01H interrupt */
    TMIF01H = 0U;    /* clear INTTM01H interrupt flag */
    /* Mask channel 2 interrupt */
    TMMK02 = 1U;    /* disable INTTM02 interrupt */
    TMIF02 = 0U;    /* clear INTTM02 interrupt flag */
    /* Mask channel 3 interrupt */
    TMMK03 = 1U;    /* disable INTTM03 interrupt */
    TMIF03 = 0U;    /* clear INTTM03 interrupt flag */
    /* Mask channel 3 higher 8 bits interrupt */
    TMMK03H = 1U;    /* disable INTTM03H interrupt */
    TMIF03H = 0U;    /* clear INTTM03H interrupt flag */
    /* Set INTTM02 low priority */
    TMPR102 = 1U;
    TMPR002 = 1U;
    /* Channel 2 is used to measure input pulse low-/high-width */
    TMR02 = _0000_TAU_CLOCK_SELECT_CKM0 | _0000_TAU_CLOCK_MODE_CKS | _0000_TAU_COMBINATION_SLAVE |
            _0200_TAU_TRIGGER_TIMN_BOTH | _00C0_TAU_TIMN_EDGE_BOTH_HIGH | _000C_TAU_MODE_HIGHLOW_MEASURE;
    TOM0 &= ~_0004_TAU_CH2_OUTPUT_COMBIN;
    TOL0 &= ~_0004_TAU_CH2_OUTPUT_LEVEL_L;
    TO0 &= ~_0004_TAU_CH2_OUTPUT_VALUE_1;
    TOE0 &= ~_0004_TAU_CH2_OUTPUT_ENABLE;
    NFEN1 |= _04_TAU_CH2_NOISE_ON;    /* enable using noise filter of TI02 pin input signal */
    /* Channel 3 used as interval timer */
    TMR03 = _0000_TAU_CLOCK_SELECT_CKM0 | _0000_TAU_CLOCK_MODE_CKS | _0000_TAU_16BITS_MODE |
            _0000_TAU_TRIGGER_SOFTWARE | _0000_TAU_MODE_INTERVAL_TIMER | _0000_TAU_START_INT_UNUSED;
    TDR03 = _7CFF_TAU_TDR03_VALUE;
    TOM0 &= ~_0008_TAU_CH3_OUTPUT_COMBIN;
    TOL0 &= ~_0008_TAU_CH3_OUTPUT_LEVEL_L;
    TO0 &= ~_0008_TAU_CH3_OUTPUT_VALUE_1;
    TOE0 &= ~_0008_TAU_CH3_OUTPUT_ENABLE;
    /* Set TI02 pin */
    PM1 |= 0x80U;
}

/***********************************************************************************************************************
* Function Name: R_TAU0_Channel2_Start
* Description  : This function starts TAU0 channel 2 counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_TAU0_Channel2_Start(void)
{
    TMIF02 = 0U;    /* clear INTTM02 interrupt flag */
    TMMK02 = 0U;    /* enable INTTM02 interrupt */
    TS0 |= _0004_TAU_CH2_START_TRG_ON;
}

/***********************************************************************************************************************
* Function Name: R_TAU0_Channel2_Stop
* Description  : This function stops TAU0 channel 2 counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_TAU0_Channel2_Stop(void)
{
    TT0 |= _0004_TAU_CH2_STOP_TRG_ON;
    /* Mask channel 2 interrupt */
    TMMK02 = 1U;    /* disable INTTM02 interrupt */
    TMIF02 = 0U;    /* clear INTTM02 interrupt flag */
}

/***********************************************************************************************************************
* Function Name: R_TAU0_Channel2_Get_PulseWidth
* Description  : This function measures TAU0 channel 2 input pulse width.
* Arguments    : width -
*                    the address where to write the input pulse width
* Return Value : None
***********************************************************************************************************************/
void R_TAU0_Channel2_Get_PulseWidth(uint32_t * const width)
{
    /* For channel 2 pulse measurement */
    *width = g_tau0_ch2_width;
}

/***********************************************************************************************************************
* Function Name: R_TAU0_Channel3_Start
* Description  : This function starts TAU0 channel 3 counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_TAU0_Channel3_Start(void)
{
    TS0 |= _0008_TAU_CH3_START_TRG_ON;
}

/***********************************************************************************************************************
* Function Name: R_TAU0_Channel3_Stop
* Description  : This function stops TAU0 channel 3 counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_TAU0_Channel3_Stop(void)
{
    TT0 |= _0008_TAU_CH3_STOP_TRG_ON;
}

/***********************************************************************************************************************
* Function Name: R_TMR_RJ0_Create
* Description  : This function initializes the TMRJ0 module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_TMR_RJ0_Create(void)
{
    TRJ0EN = 1U;    /* enable input clock supply */
    TRJCR0 &= (uint8_t)~_01_TMRJ_COUNT_START;    /* disable TMRJ0 operation */
    TRJMK0 = 1U;    /* disable INTTRJ0 interrupt */
    TRJIF0 = 0U;    /* clear INTTRJ0 interrupt flag */
    /* Set INTTRJ0 low priority */
    TRJPR10 = 1U;
    TRJPR00 = 1U;
    TRJMR0 = _00_TMRJ_MODE_TIMER | _50_TMRJ_COUNT_SOURCE_FELC;
    TRJIOC0 = _00_TMRJ_TRJIOC_INITIAL_VALUE;
    TRJ0 = _FDE7_TMRJ_TRJ0_VALUE;
}

/***********************************************************************************************************************
* Function Name: R_TMR_RJ0_Start
* Description  : This function starts TMRJ0 counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_TMR_RJ0_Start(void)
{
    TRJIF0 = 0U;    /* clear INTTRJ0 interrupt flag */
    TRJMK0 = 0U;    /* enable INTTRJ0 interrupt */
    TRJCR0 |= _01_TMRJ_COUNT_START;    /* enable TMRJ operation */
}

/***********************************************************************************************************************
* Function Name: R_TMR_RJ0_Stop
* Description  : This function stops TMRJ0 counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_TMR_RJ0_Stop(void)
{
    TRJCR0 &= (uint8_t)~_01_TMRJ_COUNT_START;    /* disable TMRJ operation */
    TRJMK0 = 1U;    /* disable INTTRJ0 interrupt */
    TRJIF0 = 0U;    /* clear INTTRJ0 interrupt flag */
}

/***********************************************************************************************************************
* Function Name: R_TMR_RD0_Create
* Description  : This function initializes the TMRD0 module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_TMR_RD0_Create(void)
{
    TRD0EN = 1U;    /* enable input clock supply */ 
    TRDSTR |= _04_TMRD_TRD0_COUNT_CONTINUES;
    TRDSTR &= (uint8_t)~_01_TMRD_TRD0_COUNT_START;  /* disable TMRD0 operation */
    TRDMK0 = 1U;    /* disable TMRD0 interrupt */
    TRDIF0 = 0U;    /* clear TMRD0 interrupt flag */
    TRDMR |= _00_TMRD_TRDGRC0_GENERAL | _00_TMRD_TRDGRD0_GENERAL;
    TRDPMR |= _01_TMRD_TRDIOB0_PWM_MODE | _02_TMRD_TRDIOC0_PWM_MODE | _04_TMRD_TRDIOD0_PWM_MODE;
#if defined MOTOR_DRIVER_CONF_HS_ACTIVE_H
    TRDDF0 = _02_TMRD_TRDIOD_LOW_OUTPUT | _08_TMRD_TRDIOC_LOW_OUTPUT | _20_TMRD_TRDIOB_LOW_OUTPUT;
#elif defined MOTOR_DRIVER_CONF_HS_ACTIVE_L
	TRDDF0 = _03_TMRD_TRDIOD_HIGH_OUTPUT | _0C_TMRD_TRDIOC_HIGH_OUTPUT | _30_TMRD_TRDIOB_HIGH_OUTPUT;
#else
	#ERROR	Undefined MOTOR_DRIVER_CONF_HS_ACTIVE
#endif
    TRDOER1 &= _F0_TMRD_CHANNEL0_OUTPUT_DEFAULT;
    TRDOER1 |= _01_TMRD_TRDIOA0_OUTPUT_DISABLE | _00_TMRD_TRDIOB0_OUTPUT_ENABLE | _00_TMRD_TRDIOC0_OUTPUT_ENABLE |
               _00_TMRD_TRDIOD0_OUTPUT_ENABLE;
    TRDOCR |= _00_TMRD_TRDIOB0_INITIAL_OUTPUT_L | _00_TMRD_TRDIOC0_INITIAL_OUTPUT_L | _00_TMRD_TRDIOD0_INITIAL_OUTPUT_L;
    TRDCR0 |= _00_TMRD_INETNAL_CLOCK_F1_FIH | _20_TMRD_COUNTER_CLEAR_TRDGRA;
    TRDIER0 = _00_TMRD_IMIA_DISABLE | _00_TMRD_IMIB_DISABLE | _00_TMRD_IMIC_DISABLE | _00_TMRD_IMID_DISABLE;
#if defined MOTOR_DRIVER_CONF_HS_ACTIVE_H
    TRDPOCR0 = _01_TMRD_TRDIOB_OUTPUT_ACTIVE_H | _02_TMRD_TRDIOC_OUTPUT_ACTIVE_H | _04_TMRD_TRDIOD_OUTPUT_ACTIVE_H;
#elif defined MOTOR_DRIVER_CONF_HS_ACTIVE_L
	TRDPOCR0 = _00_TMRD_TRDIOB_OUTPUT_ACTIVE_L | _00_TMRD_TRDIOC_OUTPUT_ACTIVE_L | _00_TMRD_TRDIOD_OUTPUT_ACTIVE_L;
#else
	#ERROR MOTOR_DRIVER_CONF_HS_ACTIVE
#endif
    TRDGRA0 = _3E7F_TMRD_TRDGRA0_VALUE;
    TRDGRB0 = _3E80_TMRD_TRDGRB0_VALUE;
    TRDGRC0 = _3E80_TMRD_TRDGRC0_VALUE;
    TRDGRD0 = _3E80_TMRD_TRDGRD0_VALUE;
    /* Set TRDIOB0 pin */
    P1 &= 0xDFU;
    PM1 &= 0xDFU;
    /* Set TRDIOC0 pin */
    P1 &= 0xBFU;
    PM1 &= 0xBFU;
    /* Set TRDIOD0 pin */
    P1 &= 0xEFU;
    PM1 &= 0xEFU;
}

/***********************************************************************************************************************
* Function Name: R_TMR_RD0_Start
* Description  : This function starts TMRD0 counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_TMR_RD0_Start(void)
{
    uint8_t trdsr_dummy;
    
    trdsr_dummy = TRDSR0; /* read TRDSR0 before write 0 */
    TRDSR0 = 0x00U; /* clear TRD0 each interrupt request */
    TRDSTR |= _04_TMRD_TRD0_COUNT_CONTINUES;
    TRDSTR |= _01_TMRD_TRD0_COUNT_START;    /* start TMRD0 counter */
}

/***********************************************************************************************************************
* Function Name: R_TMR_RD0_Stop
* Description  : This function stops TMRD0 counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_TMR_RD0_Stop(void)
{
    uint8_t trdsr_dummy;
    
    TRDSTR |= _04_TMRD_TRD0_COUNT_CONTINUES;
    TRDSTR &= (uint8_t)~_01_TMRD_TRD0_COUNT_START;  /* stop TMRD0 counter */
    trdsr_dummy = TRDSR0; /* read TRDSR0 before write 0 */
    TRDSR0 = 0x00U; /* clear TRD0 each interrupt request */
}

/***********************************************************************************************************************
* Function Name: R_TMR_RD1_Create
* Description  : This function initializes the TMRD1 module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_TMR_RD1_Create(void)
{
    TRD0EN = 1U;    /* enable input clock supply */ 
    TRDSTR |= _08_TMRD_TRD1_COUNT_CONTINUES;
    TRDSTR &= (uint8_t)~_02_TMRD_TRD1_COUNT_START;  /* disable TMRD1 operation */
    TRDMK1 = 1U;    /* disable TMRD1 interrupt */
    TRDIF1 = 0U;    /* clear TMRD1 interrupt flag */
    TRDMR |= _00_TMRD_TRDGRC1_GENERAL | _00_TMRD_TRDGRD1_GENERAL;
    TRDPMR |= _10_TMRD_TRDIOB1_PWM_MODE | _20_TMRD_TRDIOC1_PWM_MODE | _40_TMRD_TRDIOD1_PWM_MODE;
#if defined MOTOR_DRIVER_CONF_LS_ACTIVE_H
    TRDDF1 = _02_TMRD_TRDIOD_LOW_OUTPUT | _08_TMRD_TRDIOC_LOW_OUTPUT | _20_TMRD_TRDIOB_LOW_OUTPUT;
#elif defined MOTOR_DRIVER_CONF_LS_ACTIVE_L
	TRDDF1 = _03_TMRD_TRDIOD_HIGH_OUTPUT | _0C_TMRD_TRDIOC_HIGH_OUTPUT | _30_TMRD_TRDIOB_HIGH_OUTPUT;
#else
	#ERROR MOTOR_DRIVER_CONF_LS_ACTIVE
#endif
    TRDOER1 &= _0F_TMRD_CHANNEL1_OUTPUT_DEFAULT;
    TRDOER1 |= _10_TMRD_TRDIOA1_OUTPUT_DISABLE | _00_TMRD_TRDIOB1_OUTPUT_ENABLE | _00_TMRD_TRDIOC1_OUTPUT_ENABLE |
               _00_TMRD_TRDIOD1_OUTPUT_ENABLE;
    TRDOCR |= _00_TMRD_TRDIOB1_INITIAL_OUTPUT_L | _00_TMRD_TRDIOC1_INITIAL_OUTPUT_L | _00_TMRD_TRDIOD1_INITIAL_OUTPUT_L;
    TRDCR1 |= _00_TMRD_INETNAL_CLOCK_F1_FIH | _20_TMRD_COUNTER_CLEAR_TRDGRA;
    TRDIER1 = _00_TMRD_IMIA_DISABLE | _00_TMRD_IMIB_DISABLE | _00_TMRD_IMIC_DISABLE | _00_TMRD_IMID_DISABLE;
#if defined MOTOR_DRIVER_CONF_LS_ACTIVE_H
    TRDPOCR1 = _01_TMRD_TRDIOB_OUTPUT_ACTIVE_H | _02_TMRD_TRDIOC_OUTPUT_ACTIVE_H | _04_TMRD_TRDIOD_OUTPUT_ACTIVE_H;
#elif defined MOTOR_DRIVER_CONF_LS_ACTIVE_L
	TRDPOCR1 = _00_TMRD_TRDIOB_OUTPUT_ACTIVE_L | _00_TMRD_TRDIOC_OUTPUT_ACTIVE_L | _00_TMRD_TRDIOD_OUTPUT_ACTIVE_L;
#else
	#ERROR MOTOR_DRIVER_CONF_LS_ACTIVE
#endif
    TRDGRA1 = _3E7F_TMRD_TRDGRA1_VALUE;
    TRDGRB1 = _3E80_TMRD_TRDGRB1_VALUE;
    TRDGRC1 = _3E80_TMRD_TRDGRC1_VALUE;
    TRDGRD1 = _3E80_TMRD_TRDGRD1_VALUE;
    /* Set TRDIOB1 pin */
    P1 &= 0xFBU;
    PM1 &= 0xFBU;
    /* Set TRDIOC1 pin */
    P1 &= 0xFDU;
    PM1 &= 0xFDU;
    /* Set TRDIOD1 pin */
    P1 &= 0xFEU;
    PM1 &= 0xFEU;
}

/***********************************************************************************************************************
* Function Name: R_TMR_RD1_Start
* Description  : This function starts TMRD1 counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_TMR_RD1_Start(void)
{
    uint8_t trdsr_dummy;
    
    trdsr_dummy = TRDSR1; /* read TRDSR1 before write 0 */
    TRDSR1 = 0x00U; /* clear TRD1 each interrupt request */
    TRDSTR |= _08_TMRD_TRD1_COUNT_CONTINUES;
    TRDSTR |= _02_TMRD_TRD1_COUNT_START;    /* start TMRD1 counter */
}

/***********************************************************************************************************************
* Function Name: R_TMR_RD1_Stop
* Description  : This function stops TMRD1 counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_TMR_RD1_Stop(void)
{
    uint8_t trdsr_dummy;
    
    TRDSTR |= _08_TMRD_TRD1_COUNT_CONTINUES;
    TRDSTR &= (uint8_t)~_02_TMRD_TRD1_COUNT_START;  /* stop TMRD1 counter */
    trdsr_dummy = TRDSR1; /* read TRDSR1 before write 0 */
    TRDSR1 = 0x00U; /* clear TRD1 each interrupt request */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
