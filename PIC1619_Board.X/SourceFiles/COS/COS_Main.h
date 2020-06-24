/** @file COS_Main.h
 *  @ingroup group_COS_Setup
 *
 * @author Steve Deasy
 * @author Joshua Hartwig
 *
 *
 */

#ifndef COS_MAIN_H
#define	COS_MAIN_H

#include "COS_UserTskList.h"

/******************************************************************************/
/****************************** DECLARATIONS **********************************/
/******************************************************************************/

/* COS Declarations */
#ifdef _UNIT_TESTING_
#undef _COS_LIN_ENABLED
#endif // _UNIT_TESTING_

#ifdef _COS_LIN_ENABLED
#define TSK_COUNT               (USER_TSK_COUNT+2)   /* Number of tasks called by the task manager */
#else
#define TSK_COUNT               USER_TSK_COUNT   /* Number of tasks called by the task manager */
#endif // _COS_LIN_ENABLED

#define NUM_TSKS        TSK_COUNT   /* number of routines called by the task manager */
#define COS_TSK_SLT     SLOT_TIME   /* basic task slot time in milliseconds */

#define TIME_TSK_EVERYSLOT      SLOT_TIME       /* EVERYSLOT task time in msec */
#define TIME_TSK_HALFSPEED      (SLOT_TIME*2)   /* HALFSPEED task time in msec */
#define TIME_TSK_QUARTERSPEED   (SLOT_TIME*4)   /* QUARTERSPEED task time in msec */
#define TIME_TSK_EIGHTHSPEED    (SLOT_TIME*8)   /* EIGHTHSPEED task time in msec */

/* COS Task Slot Deeclarations */
#define TSK_EVERYSLOT           0xff    /* Runs every SLOT_TIME */
#define TSK_HALFSPEED_S1        0xaa    /* Runs every odd SLOT_TIME */
#define TSK_HALFSPEED_S2        0x55    /* Runs every even SLOT_TIME */
#define TSK_QUARTERSPEED_S1     0x88    /* Runs in Slot 1 and Slot 5 */
#define TSK_QUARTERSPEED_S2     0x44    /* Runs in Slot 2 and Slot 6 */
#define TSK_QUARTERSPEED_S3     0x22    /* Runs in Slot 3 and Slot 7 */
#define TSK_QUARTERSPEED_S4     0x11    /* Runs in Slot 4 and Slot 8 */
#define TSK_EIGHTHSPEED_S1      0x80    /* Runs in Slot 1 */
#define TSK_EIGHTHSPEED_S2      0x40    /* Runs in Slot 2 */
#define TSK_EIGHTHSPEED_S3      0x20    /* Runs in Slot 3 */
#define TSK_EIGHTHSPEED_S4      0x10    /* Runs in Slot 4 */
#define TSK_EIGHTHSPEED_S5      0x08    /* Runs in Slot 5 */
#define TSK_EIGHTHSPEED_S6      0x04    /* Runs in Slot 6*/
#define TSK_EIGHTHSPEED_S7      0x02    /* Runs in Slot 7 */
#define TSK_EIGHTHSPEED_S8      0x01    /* Runs in Slot 8 */

#define COS_SLOT_1		TSK_EIGHTHSPEED_S1
#define COS_SLOT_2		TSK_EIGHTHSPEED_S2
#define COS_SLOT_3		TSK_EIGHTHSPEED_S3
#define COS_SLOT_4		TSK_EIGHTHSPEED_S4
#define COS_SLOT_5		TSK_EIGHTHSPEED_S5
#define COS_SLOT_6		TSK_EIGHTHSPEED_S6
#define COS_SLOT_7		TSK_EIGHTHSPEED_S7
#define COS_SLOT_8		TSK_EIGHTHSPEED_S8

/* COS Vars */
extern UINT_8 COS_tsk_msk;          /* Mask for tasks to run this time slice */
extern UINT_8 COS_tsk_ind;          /* Index into COS tasks array */
extern UINT_8 COS_tsk_ena;          /* COS task enable flag */
extern UINT_16 COS_tsk_tmr;		/* task slot timer */
extern UINT_16 COS_tcnt_val;	/* value of Timer count register */
extern UINT_16 COS_tcnt_lst;	/* Timer count at last ms interval */

extern UINT_16 COS_ms_tmr;      /* Task manager ms timer */
extern volatile UINT_16 COS_sec_tmr; /* second timer */

/* COS Prototypes */
void COS_Init(void); /* Call this before main loop to configure the task manager */
void COS_TimerSetup(void);  /* Prototype for setting up base tick */
void COS_TmrHandler(void); /* Call as below in main loop */
void COS_TskMan(void); /* Call as below in main loop */
#ifdef DEBUG_COS_TSK_TIMES
void DebugTaskTimerInit(void);
	#ifndef _UNIT_TESTING_
		#warning Warning! COS Debug Enabled! Do not commit! Do not release! Warning!
	#endif
#endif

#ifdef _UNIT_TESTING_
	#include "UnitHelper_COS_main.h"
#endif

#endif	/* COS_MAIN_H */

