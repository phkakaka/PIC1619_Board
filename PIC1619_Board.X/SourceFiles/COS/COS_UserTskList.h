/** @file COS_UserTskList.c
 *  @ingroup group_COS_Setup
 *
 * @author Steve Deasy
 * @author Joshua Hartwig
 *
 *
 */

#ifndef COS_USERTSKLIST_H
#define	COS_USERTSKLIST_H

/* Include GlobalDef.h in all C files */
#include "GlobalDef.h"

////////////////////////////////////////////////////////////////////////////////
/////////////////////////// Start User Setup ///////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/*********** User Defined Includes for Task Prototypes ***********/

/*********** Task Setup ***********/
/* MUST be equal to number of user-defined tasks in COS_tsk[] */
#define USER_TSK_COUNT          3   /* Number of user-defined tasks called by the task manager */
									/* DO NOT COUNT LIN RELATED TASKS */

#define SLOT_TIME               10  /* Basic task slot time in milliseconds (8 and 10 work well) */

////////////////////////////////////////////////////////////////////////////////
//////////////////////////// End User Setup ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


/* COS Structs */
typedef struct
{
    void (*tptr) (void);
    UINT_8 sched;
} task;

extern const task COS_tsk[]; /* Array of COS tasks */

#endif	/* COS_USERTSKLIST_H */

