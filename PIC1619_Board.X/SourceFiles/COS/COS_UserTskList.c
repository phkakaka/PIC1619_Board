/** @file COS_UserTskList.c
 *  @ingroup group_COS_Setup
 *
 * @author Steve Deasy
 * @author Joshua Hartwig
 *
 * 
 */

/*-- Includes --*/
#include "GlobalDef.h"
#include "COS_UserTskList.h"
#include "../SunSensor/Led.h"
#include "../SunSensor/UART_Display.h"
#include "../SunSensor/SHT25.h"

/** @var const task COS_tsk[NUM_TSKS]
 * 
 * @brief COS Task List
 *
 * This array of tasks and speed/slot definitions defines exactly what the COS
 * will run and how often.
 *
 * Example Task Definition:
 * @code
const task COS_tsk[NUM_TSKS] = /* Task array for task manager 
{
    { WDT_Trigger, TSK_EVERYSLOT },
    { LIN_StatusHand, TSK_HALFSPEED_S2 }
};
 * @endcode
 *
 * @ingroup group_COS_Setup
 */
const task COS_tsk[NUM_TSKS] = /* Task array for task manager */
{
////////////////////////////////////////////////////////////////////////////////
/////////////////////////// Start User Setup ///////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

	//{UartDisplay, TSK_EIGHTHSPEED_S1 },
    {SHT25_StateMachine, TSK_EIGHTHSPEED_S2}


////////////////////////////////////////////////////////////////////////////////
//////////////////////////// End User Setup ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/* Example using prototypes in COS_UserSetup.h
 *
 *	{ WDT_Trigger, TSK_EVERYSLOT },        	// watchdog service handler
 *	{ ACQ_StateMachine, TSK_EVERYSLOT }, 	// Sunload Sensor ACQuistion State Machine
 *	{ CALC_StateMachine, TSK_EVERYSLOT }, 	// Sunload Sensor CALCulation State Machine
 *	{ LIN_SignalHandler, TSK_EVERYSLOT },	// LIN signal handler
 *	{ LIN_StatusHand, TSK_EVERYSLOT }		// LIN status handler
 *
 * These tasks must be defined in their own C files which #include this H file
 * Every task must take void arguments and return void
 */
};

