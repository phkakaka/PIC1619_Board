/** @file ad.h
 *  @ingroup SunSensor
 *
 * @author Samuel Wang
 *
 * 
 */

#ifndef _AD_H_
#define _AD_H_

#include "../COS/COS_Defs.h"

/*-- Includes --*/
typedef enum
{
    AD_MEASURE_START,
    AD_MEASURE_GetResult,  
    AD_MEASURE_WAIT,
}AD_tt;


extern void AD_Task(void);
extern float GetNtcTemp(void);
extern UINT_8 IsADMeasureDone(void);

#endif
