/** @file Led.c
 *  @ingroup SunSensor
 *
 * @author Samuel Wang
 *
 * 
 */

/*-- Includes --*/
#include "../../mcc_generated_files/mcc.h"
#include "SHT25.h"
#include "UART_Display.h"
#include "I2C_Driver.h"
#include "../../mcc_generated_files/examples/i2c_master_example.h"
#include "stdlib.h"

#define SHT31_ADDR_W   0x44   // sensor I2C address + write bit
#define SHT31_ADDR_R   0x45    // sensor I2C address + read bit
#define Numtoasc(b) ('0'+ b)

//Variables with external linkage
UINT_16 SensirionTemperature_UBP8;
UINT_16 SensirionRH_UBP8;
float SensirionTemperature_float;
float SensirionRH_float;
UINT_1 SHT25_Fault;
UINT_8 ErrorStat;
UINT_16 I2C_Timer;

//Variables used only by this module
UINT_8 checksum;
UINT_8  TemperatureRaw_H;
UINT_8  TemperatureRaw_L;
UINT_16 TemperatureRaw;
UINT_8  RelativeHumidityRaw_H;
UINT_8  RelativeHumidityRaw_L;
UINT_16 RelativeHumidityRaw;
SHT25_State_tt SHT25_State = SHT25_MEASURE_INIT;
UINT_8 Command[2] = {0x22,0x20};
UINT_8 RxBuff[8];
UINT_8 AddressW = SHT25_ADDR_W;
UINT_16 TimerCounter = 0;
static UINT_8 IsMeasureDone = FALSE;


void SHT25_StateMachine(void)
{
    switch(SHT25_State)
    {
        case SHT25_MEASURE_INIT:
            SHT25_State = SHT25_MEASURE_TEMP_0;
            break;
        case SHT25_MEASURE_TEMP_0:
            I2C_WriteNBytes(SHT31_ADDR_W,Command,2);
            SHT25_State = SHT25_MEASURE_TEMP_WAIT;
            TimerCounter = 0;
            
            
            break;
        case SHT25_MEASURE_TEMP_WAIT:
            TimerCounter++;
            if (TimerCounter > 50)
            {
                Command[0] = 0xE0;
                Command[1] = 0x00;
                I2C_WriteNBytes(SHT31_ADDR_W,Command,2);
                TimerCounter = 0;
                SHT25_State = SHT25_MEASURE_TEMP_1;
            }
            break;
        case SHT25_MEASURE_TEMP_1:
            I2C_ReadNBytes(SHT31_ADDR_W,RxBuff,6);
            
            TemperatureRaw_H = RxBuff[0];
            TemperatureRaw_L = RxBuff[1];
            TemperatureRaw = ((UINT_16)(TemperatureRaw_H) << 8) + TemperatureRaw_L ;
            
            RelativeHumidityRaw_H = RxBuff[3];
            RelativeHumidityRaw_L = RxBuff[4];
            RelativeHumidityRaw = ((UINT_16)(RelativeHumidityRaw_H) << 8) + RelativeHumidityRaw_L ;
            
            SHT25_State = SHT25_CALC_TEMP_0;
            break;
        case SHT25_CALC_TEMP_0:
            SensirionTemperature_float = SHT3x_CalcTemperatureC_F();
            SensirionRH_float = SHT3x_CalcRelativeHumidity_F();
            //SensirionTemperature_float = SHT3x_CalcTemperatureC();
            //SensirionRH_float = SHT3x_CalcRelativeHumidity();
            SHT25_State = SHT25_CALC_RH_0;
            IsMeasureDone = TRUE;
            break;
        case SHT25_CALC_RH_0:
            SHT25_State = SHT25_MEASURE_TEMP_WAIT;
            break;
        default:
            break;
    }
}


void SHT25_FaultHand(void)
{
    TemperatureRaw = 0;
    RelativeHumidityRaw = 0;
    SHT25_Fault = 1;
    SHT25_State = SHT25_CALC_TEMP_0;
}

float SHT3x_CalcTemperatureC_F(void)
{
    float TemperatureFloat;

    TemperatureFloat = (float)TemperatureRaw * 175 / 65535 - 45;

    return TemperatureFloat ;
}


float SHT3x_CalcRelativeHumidity_F(void)
{
    float RelativeHumidityFloat;

    RelativeHumidityFloat = (float)RelativeHumidityRaw * 100 / 65535;
 
    return RelativeHumidityFloat ;
}

float GetTemperatureC_F(void)
{
    return SensirionTemperature_float;
}

float GetRelativeHumidity_F(void)
{
    return SensirionRH_float;
}

UINT_8 IsSHT31MeasureDone(void)
{
    return IsMeasureDone;
}

UINT_16 SHT3x_CalcTemperatureC(void)
{
    SINT_32 TemperatureTemp;
    UINT_16 Temperature_UBP8;
    UINT_16 Temperature_U16;
    UINT_8 Temperature_L8;
    
    TemperatureTemp = (UINT_32)TemperatureRaw * 175;
    Temperature_U16 = (UINT_16)(TemperatureTemp >> 16);
    Temperature_UBP8 = (UINT_16)Temperature_U16 - 45;
    return Temperature_UBP8 ;
}

UINT_16 SHT3x_CalcRelativeHumidity(void)
{
    SINT_32 RelativeHumidityTemp;
    UINT_16 RelativeHumidity_UBP8;
    UINT_8 RelativeHumidity_U16;
    UINT_8 RelativeHumidity_L8;
    
    RelativeHumidityTemp = (UINT_32)RelativeHumidityRaw * 100;
    RelativeHumidity_U16 = (UINT_16)(RelativeHumidityTemp >> 16);
    RelativeHumidity_UBP8 = (UINT_16)RelativeHumidity_U16;
    return RelativeHumidity_UBP8 ;
}

