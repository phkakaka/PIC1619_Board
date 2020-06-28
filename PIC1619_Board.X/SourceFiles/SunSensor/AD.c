/** @file AD.c
 *  @ingroup SunSensor
 *
 * @author Samuel Wang
 *
 * 
 */

/*-- Includes --*/
#include "../../mcc_generated_files/mcc.h"
#include "AD.h"
#include "../COS/COS_Defs.h"

AD_tt AD_State = AD_MEASURE_START;
UINT_16 AD_Result;
static UINT_8 TimerCounter;
static UINT_8 IsMeasureDone; 

void AD_Task(void)
{
    UINT_8 str[8];
    
    switch(AD_State)
    {
        case AD_MEASURE_START:
            ADC_SelectChannel(4);
            ADC_StartConversion();
            AD_State = AD_MEASURE_GetResult;
            break;
        case AD_MEASURE_GetResult:
            if (ADC_IsConversionDone)
            {
                AD_Result = ADC_GetConversionResult();
                IsMeasureDone = TRUE;
//                sprintf(str, "%d" , AD_Result);
//
//                EUSART_Write('A');
//                EUSART_Write(str[0]);
//                EUSART_Write(str[1]);
//                EUSART_Write(str[2]);
//                EUSART_Write(str[3]);
//                EUSART_Write(str[4]);
                AD_State = AD_MEASURE_WAIT;
            }
            break;
        case AD_MEASURE_WAIT:
            TimerCounter++;
            if (TimerCounter > 50)
            {
                TimerCounter = 0;
                AD_State = AD_MEASURE_START;
            }
            break;
        default:
            break;
    }
}

float GetNtcTemp(void)
{
    float NtcTemp;
    
    if (AD_State > 100)
    {
        NtcTemp = 25.53;
    }
    else
    {
        NtcTemp = -20.96;
    }
    
    return NtcTemp;
}

UINT_8 IsADMeasureDone(void)
{
    return IsMeasureDone;
}
