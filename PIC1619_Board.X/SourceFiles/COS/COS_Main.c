/** @file COS_Main.c
 *  @ingroup group_COS_NutsBolts
 *
 * @author Steve Deasy
 * @author Joshua Hartwig
 *
 *
 */

#include "GlobalDef.h"

/* Variable definitions for the task manager. */
UINT_8 COS_tsk_ind;				/**< Task index for current task */
UINT_8 COS_tsk_msk;				/**< Task mask for current time slot */
UINT_8 COS_tsk_ena;				/**< Task enable flag */

/* Variable definitions for the timer handler. */
UINT_16 COS_ms_tmr;				/**< Task manager ms timer */
volatile UINT_16 COS_sec_tmr;	/**< second timer */
UINT_16 COS_tsk_tmr;			/**< task slot timer */
UINT_16 COS_tcnt_val;			/**< value of GPT TCNT register */
UINT_16 COS_tcnt_lst;			/**< GPT TCNT at last ms interval */

/**
 * @brief Function called to initialize the COS scheduler
 *
 * This function initializes the COS scheduler and should be called near the end
 * of microprocessor initialization before entering the main while loop.
 *
 * @ingroup group_COS_Setup
 */
void COS_Init(void)
{
	UINT_8 tempbyte1, tempbyte2;

	COS_TimerSetup();
#ifdef DEBUG_COS_TSK_TIMES
	DebugTaskTimerInit();
#endif

	COS_tsk_ind = 0;
	COS_tsk_msk = COS_SLOT_1;
	COS_tsk_ena = TRUE;
	COS_ms_tmr = 0;
	COS_sec_tmr = 0;
	COS_tsk_tmr = 0;

	tempbyte2 = TMR1H;
	tempbyte1 = TMR1L;
	if (tempbyte2 != TMR1H)
	{
		tempbyte2 = TMR1H;
		tempbyte1 = TMR1L;
	}
	COS_tcnt_val = ((UINT_16)tempbyte2 << 8)&0xff00;
	COS_tcnt_val = COS_tcnt_val | ((UINT_16)tempbyte1 & 0x00ff);
	COS_tcnt_lst = COS_tcnt_val;
}

/**
 * @brief This function initializes Timer 1 for use by the COS Scheduler.
 *
 * This function is automatically called from COS_Init().  It initializes
 * Timer 1 at 0 ticks and applies the COS_TMR1_CON configuration.
 *
 * @ingroup group_COS_NutsBolts
 */
void COS_TimerSetup(void)
{
	/* Timer 1 used for Base System Tick Time */
	/* Timer 1 Register pair must be cleared before enabling interrupts */
	TMR1H = 0;
	TMR1L = 0;

	/* Timer 1 Clock Source Fosc/4 = 4MHz */
	/* Set Timer 1 Prescaler to 1:4 --> Timer1Tick = 1usec */
	/* Switch On Timer 1 */
	T1CON = COS_TMR1_CON;
}

/**
 * @brief This function polls Timer 1 and flags when tasks slots should run.
 *
 * This function must be called in the main function.  It works on a polling basis
 * monitoring Timer 1 for a 1ms tick.  This allows for jitter and does not force
 * the microprocessor into an ISR continuously.  After 1ms passes, this function checks
 * if a slot needs to run and sets a flag accordingly.  If LIN is enabled, diagnostics
 * are called from here to ensure they are updated every 1ms.
 *
 * @ingroup group_COS_Setup
 */
void COS_TmrHandler(void)
{
	UINT_8 tempbyte1, tempbyte2;
	tempbyte2 = TMR1H;
	tempbyte1 = TMR1L;
	if (tempbyte2 != TMR1H)
	{
		tempbyte2 = TMR1H;
		tempbyte1 = TMR1L;
	}
	COS_tcnt_val = (UINT_16) (tempbyte2 << 8)&0xff00;
	COS_tcnt_val = COS_tcnt_val | (UINT_16) (tempbyte1 & 0x00ff);

	if (((UINT_16) ((UINT_16) COS_tcnt_val - (UINT_16) COS_tcnt_lst) >= (UINT_16) TIMER1_COUNTS_PER_MS))
	{
		COS_tcnt_lst = COS_tcnt_lst + TIMER1_COUNTS_PER_MS;
		COS_ms_tmr++;
        
		if (COS_ms_tmr >= MS_PER_SEC)
		{
			COS_ms_tmr = 0;
			COS_sec_tmr++;
		}
		COS_tsk_tmr++;
		if ((COS_tsk_tmr >= COS_TSK_SLT) && (!COS_tsk_ena))
		{
			COS_tsk_tmr = 0;
			COS_tsk_ena = TRUE;
		}
	}
}

/**
 * @brief This function calls tasks from the task list as needed.
 *
 * The Task Manager will call the appropriate tasks for the current slot.
 * This function should be called from within the main while loop only if
 * COS_tsk_ena is true.
 *
 * @ingroup group_COS_Setup
 */
void COS_TskMan(void)
{
	if ((COS_tsk[COS_tsk_ind].sched & COS_tsk_msk) != 0)
	{
		#ifdef DEBUG_COS_TSK_TIMES
			DEBUG_TASKTIMER_REGISTER = 0;
			DEBUG_TASKTIMER_IF_FLAG = 0;
			DEBUG_TASKTIMER_TMR_ON = 1;
		#endif

		(*COS_tsk[COS_tsk_ind].tptr)();

		#ifdef DEBUG_COS_TSK_TIMES
			DEBUG_TASKTIMER_TMR_ON = 0;
			DEBUG_TASKTIMER_REGISTER = 0;
		#endif
	}
	COS_tsk_ind++;
	if (COS_tsk_ind >= NUM_TSKS)
	{
		COS_tsk_ena = FALSE;
		COS_tsk_ind = 0;

		/* Check for valid task mask and reset accordingly if not valid */
		if (!((COS_tsk_msk == COS_SLOT_1) ||
			  (COS_tsk_msk == COS_SLOT_2) ||
			  (COS_tsk_msk == COS_SLOT_3) ||
			  (COS_tsk_msk == COS_SLOT_4) ||
			  (COS_tsk_msk == COS_SLOT_5) ||
			  (COS_tsk_msk == COS_SLOT_6) ||
			  (COS_tsk_msk == COS_SLOT_7) ||
			  (COS_tsk_msk == COS_SLOT_8)))
		{
			#ifdef _COS_TSKMSK_SOFT_RST
				COS_tsk_msk = COS_SLOT_8; /* Shift will set to first slot */
			#else /* _COS_TSKMSK_HARD_RST */
				COS_FORCE_WDT_RESET();
			#endif
		}

		/* Move on to next task slot */
		COS_tsk_msk = COS_tsk_msk >> 1;
		if (COS_tsk_msk == 0x00)
		{
			COS_tsk_msk = COS_SLOT_1;
		}
	}
}

#ifdef DEBUG_COS_TSK_TIMES
/**
 * @brief This function initializes Timer X for debug use by the COS Scheduler.
 *
 * This function is automatically called from COS_Init().  It initializes
 * Timer X at 0 ticks and sets it up for 1ms overflow to track whether tasks are
 * exceeding their 1ms task slot.  The user can then enable interrupts to trap
 * the uC if a task exceeds 1ms.
 *
 * @ingroup group_COS_NutsBolts
 */
void DebugTaskTimerInit(void)
{
	DEBUG_TASKTIMER_CONFIG_REG = DEBUG_TASKTIMER_CONFIG;
	DEBUG_TASKTIMER_TMR_ON = 0;

	DEBUG_TASKTIMER_REGISTER = 0;
	DEBUG_TASKTIMER_PR_REGISTER = DEBUG_TASKTIMER_PR_VALUE;
	DEBUG_TASKTIMER_IF_FLAG = 0;

#ifdef DEBUG_TASKTIMER_INT_ENABLE
	DEBUG_TASKTIMER_IE_FLAG = 1;
#endif
}
#endif
