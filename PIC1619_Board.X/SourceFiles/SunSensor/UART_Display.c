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
#include "AD.h"
#include "SHT25.h"
#include "stdlib.h"

static uint8_t U8Message = 0x55;
static uint8_t NewMessageFlag;
static uint8_t counter;
UART_tt UART_STATE = UART_TEMP;
static uint8_t TimerCounter;

void SendByUart(uint8_t Txt)
{
    U8Message = Txt;
    NewMessageFlag = 1;
}

void UartDisplay(void)
{
    float TempF;
    float HumiF;
    float NtcTempF;
    UINT_16 Temp;
    UINT_16 Humi;
    UINT_16 NtcTemp;
    UINT_8 str[8];
    
    switch (UART_STATE)
    {
        case UART_TEMP:
            if (IsSHT31MeasureDone())
            {
                EUSART_Write('T');
                TempF = GetTemperatureC_F();
                Temp = (UINT_16)(TempF * 100);
                if (TempF < 0)
                {
                  EUSART_Write('-'); 
                  TempF = abs(TempF);
                }
                //sprintf(str, "%2.2f" , Temp);
                sprintf(str, "%4d" , Temp);
                
                EUSART_Write(str[0]);
                EUSART_Write(str[1]);
                EUSART_Write('.');
                EUSART_Write(str[2]);
                EUSART_Write(str[3]);
                EUSART_Write(' ');
            }
            UART_STATE = UATR_HUMI;        
            break;
        case UATR_HUMI:
            if (IsSHT31MeasureDone())
            {
                EUSART_Write('H');
                HumiF = GetRelativeHumidity_F();
                Humi = (UINT_16)(HumiF * 100);
                //sprintf(str, "%2.2f" , Humi);
                sprintf(str, "%4d" , Humi);
                
                EUSART_Write(str[0]);
                EUSART_Write(str[1]);
                EUSART_Write('.');
                EUSART_Write(str[2]);
                EUSART_Write(str[3]);
                EUSART_Write(' ');
            }
            UART_STATE = UART_NTC;
            break;
        case UART_NTC:
            if (IsADMeasureDone())
            {
                EUSART_Write('N');
                NtcTempF = GetNtcTemp();
                NtcTemp = (UINT_16)(NtcTempF * 100);
                if (NtcTempF < 0)
                {
                    EUSART_Write('-');
                    NtcTemp = abs(NtcTemp);
                }
                //sprintf(str, "%2.2f" , NtcTemp);
                sprintf(str, "%4d" , NtcTemp);
                
                EUSART_Write(str[0]);
                EUSART_Write(str[1]);
                EUSART_Write('.');
                EUSART_Write(str[2]);
                EUSART_Write(str[3]);
                EUSART_Write(' ');
            }
            UART_STATE = UART_Refresh_Interval;
            break;
        case UART_Refresh_Interval:
            TimerCounter++;
            if (TimerCounter > 50)
            {
                TimerCounter = 0;
                UART_STATE = UART_TEMP;
            }
            break;
        default:
            break;
    }
}
