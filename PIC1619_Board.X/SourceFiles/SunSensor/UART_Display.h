/** @file Led.c
 *  @ingroup SunSensor
 *
 * @author Samuel Wang
 *
 * 
 */

#ifndef _UART_DISPLAY_
#define _UART_DISPLAY_

/*-- Includes --*/
#include "../../mcc_generated_files/mcc.h"

typedef enum
{
    UART_TEMP,
    UATR_HUMI,
    UART_NTC,
    UART_Refresh_Interval,      
}UART_tt;

extern void SendByUart(uint8_t Txt);
extern void UartDisplay(void);

#endif
