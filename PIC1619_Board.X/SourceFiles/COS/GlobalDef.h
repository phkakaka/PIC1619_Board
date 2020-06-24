/** @file GlobalDef.h
 *  @ingroup group_COS_Setup
 *
 * @author Steve Deasy
 * @author Joshua Hartwig
 *
 *
 */

#ifndef GLOBALDEF_H
#define	GLOBALDEF_H

/* This module is used for global defines and should be included by every module */

#ifndef _UNIT_TESTING_
    #include <xc.h>
	#include "COS_Defs.h"
#else
    #include "Unit_Test_Helper.h"
#endif // ifndef _UNIT_TESTING_

#include "COS_UserSetup.h"
#include "COS_Main.h"

#endif	/* GLOBALDEF_H */

