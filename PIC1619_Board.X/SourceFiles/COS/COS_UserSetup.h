/** @file COS_UserSetup.h
 *  @ingroup group_COS_Setup
 *
 * @author Steve Deasy
 * @author Joshua Hartwig
 *
 * 
 */

/* Include GlobalDef.h in all C files */

#ifndef COS_USERSETUP
#define COS_USERSETUP

#ifndef _UNIT_TESTING_
	#include "COS_Defs.h"
#else
	#include "Unit_Test_Helper.h"
#endif

////////////////////////////////////////////////////////////////////////////////
/////////////////////////// Start User Setup ///////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/*********** FOSC Definition ***********/
#define _XTAL_FREQ				16000000	/* Define FOSC in Hz */

/*********** LIN Configuration ***********/
#define _COS_LIN_ENABLED /* Set to define if LIN is used */
#ifdef _COS_LIN_ENABLED

	#define CS_PIN		LATCbits.LATC1	/* Define CS Pin for LIN transceiver */

#endif // _COS_LIN_ENABLED

/******** Timer Setup - Provide Timer1 with tick scheme to generate 1ms *******/
#define COS_TMR1_CON            0b00100001  /* Set for 1usec tick */
#define TIMER1_COUNTS_PER_MS    1000

/**************** Invalid Task Mask Behavior - ONLY SELECT ONE ****************/
#undef	_COS_TSKMSK_SOFT_RST	/* Recover by going to first task slot */
#define _COS_TSKMSK_HARD_RST	/* Recover by forcing watchdog reset */

/******* Debug Setup - Provide TxCON such that TimerX interrupts in 1ms *******/
/* Example:
 * 32MHz FOSC -> 8MHz base freq.
 * Need 1ms interrupt = 1:64 prescaler, 1:1 postscaler 125 ticks
 * Set Timer OFF (automatically turned on in the scheduler at correct time)
 */
#undef DEBUG_COS_TSK_TIMES
#ifdef DEBUG_COS_TSK_TIMES

	// Select only one timer
	#undef DEBUG_TASKTIMER_SELECT_TIMER2
	#undef DEBUG_TASKTIMER_SELECT_TIMER4
	#undef DEBUG_TASKTIMER_SELECT_TIMER6

	// Set up timer for 1ms interrupt
	// Use DEBUG_TASKTIMER_IF_FLAG for reading the interrupt flag in your ISR
	#define DEBUG_TASKTIMER_CONFIG		0bxxxxxxxx
	#define DEBUG_TASKTIMER_PR_VALUE	xxx
	#undef DEBUG_TASKTIMER_INT_ENABLE
#endif

////////////////////////////////////////////////////////////////////////////////
//////////////////////////// End User Setup ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

///* Example main function */
///*
///*  void main()
///*  {
///*      CLRWDT();
///* 
///*      MIC_Initializations();  /* All Initializations are done inside this function */
///*      COS_Init();
///* 
///*      CLRWDT();
///* 
///*      while (1)
///*      {
///*          COS_TmrHandler();
///*          l_cyclic_com_task(); /* Refresh LIN Buffers */
///*          if (COS_tsk_ena)
///*          {
///*              COS_TskMan();
///*          }
///*          l_cyclic_com_task(); /* Refresh LIN Buffers */
///*      }
///*  }                                                           */

#ifdef DEBUG_COS_TSK_TIMES
#ifdef DEBUG_TASKTIMER_SELECT_TIMER2
	#define DEBUG_TASKTIMER_CONFIG_REG T2CON
	#define DEBUG_TASKTIMER_TMR_ON T2CONbits.TMR2ON
	#define DEBUG_TASKTIMER_REGISTER TMR2
	#define DEBUG_TASKTIMER_PR_REGISTER PR2
	#define DEBUG_TASKTIMER_IF_FLAG PIR1bits.TMR2IF
	#define DEBUG_TASKTIMER_IE_FLAG PIE1bits.TMR2IE
#elif defined DEBUG_TASKTIMER_SELECT_TIMER4
	#define DEBUG_TASKTIMER_CONFIG_REG T4CON
	#define DEBUG_TASKTIMER_TMR_ON T4CONbits.TMR4ON
	#define DEBUG_TASKTIMER_REGISTER TMR4
	#define DEBUG_TASKTIMER_PR_REGISTER PR4
	#define DEBUG_TASKTIMER_IF_FLAG PIR3bits.TMR4IF
	#define DEBUG_TASKTIMER_IE_FLAG PIE3bits.TMR4IE
#elif defined DEBUG_TASKTIMER_SELECT_TIMER6
	#define DEBUG_TASKTIMER_CONFIG_REG T6CON
	#define DEBUG_TASKTIMER_TMR_ON T6CONbits.TMR6ON
	#define DEBUG_TASKTIMER_REGISTER TMR6
	#define DEBUG_TASKTIMER_PR_REGISTER PR6
	#define DEBUG_TASKTIMER_IF_FLAG PIR3bits.TMR6IF
	#define DEBUG_TASKTIMER_IE_FLAG PIE3bits.TMR6IE
#endif // DEBUG_TIMER_SELECT_TIMER6
#endif // DEBUG_COS_TSK_TIMES

#ifdef _UNIT_TESTING_
#undef _COS_LIN_ENABLED // Workaround for LIN in Unit Testing
#endif // _UNIT_TESTING_

#endif