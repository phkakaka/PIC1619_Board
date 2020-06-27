/** @file Led.c
 *  @ingroup SunSensor
 *
 * @author Samuel Wang
 *
 * 
 */

/*-- Includes --*/
#include "../../mcc_generated_files/mcc.h"
#include "UART_Display.h"

static uint8_t U8Message = 0x55;
static uint8_t NewMessageFlag;
static uint8_t counter;

void SendByUart(uint8_t Txt)
{
    U8Message = Txt;
    NewMessageFlag = 1;
}

void UartDisplay(void)
{
    if (NewMessageFlag == 1)
    {
        EUSART_Write(U8Message);
        NewMessageFlag = 0;
    }
    
    
    
//    counter++;
//    if(counter<100)
//    {
//        EUSART_Write(counter);
//    }
//    else
//    {
//        counter = 0;
//    }
}


