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
            SensirionTemperature_UBP8 = SHT3x_CalcTemperatureC();
            SensirionRH_UBP8 = SHT3x_CalcRelativeHumidity();
            SHT25_State = SHT25_CALC_RH_0;
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


UINT_16 SHT3x_CalcTemperatureC(void)
{
    SINT_32 TemperatureTemp;
    UINT_16 Temperature_UBP8;
    UINT_8 Temperature_U8;
    UINT_8 Temperature_L8;
    
    FLOAT_24 TemperatureFloat;
    UINT_16 TemperatureInt;
    UINT_8 str[8];
    
    TemperatureTemp = (UINT_32)TemperatureRaw * 175;
    Temperature_U8 = (UINT_8)(TemperatureTemp >> 16);
    Temperature_L8 = (UINT_8)(TemperatureTemp >> 8);
    Temperature_UBP8 = (UINT_16)Temperature_U8 << 8 + Temperature_L8;
    //Temperature_UBP8 = UINT_16(TemperatureTemp);
    //EUSART_Write(Temperature_U8);
    //EUSART_Write(Temperature_L8);
    
    
    TemperatureFloat = 100 *((float)TemperatureRaw * 175 / 65535 - 45);
    if (TemperatureFloat < 0)
    {
        TemperatureFloat = abs(TemperatureFloat);
        EUSART_Write('-');
    }
    TemperatureInt = (UINT_16)(TemperatureFloat);
    sprintf(str, "%d" , TemperatureInt);
    
    EUSART_Write('T');
    EUSART_Write(str[0]);
    EUSART_Write(str[1]);
    EUSART_Write('.');
    EUSART_Write(str[2]);
    EUSART_Write(str[3]);
    //EUSART_Write(str[5]);
    
    return Temperature_UBP8 ;
}

UINT_16 SHT3x_CalcRelativeHumidity(void)
{
    SINT_32 RelativeHumidityTemp;
    UINT_16 RelativeHumidity_UBP8;
    UINT_8 RelativeHumidity_U8;
    UINT_8 RelativeHumidity_L8;
    
    FLOAT_24 RelativeHumidityFloat;
    UINT_16 RelativeHumidityInt;
    UINT_8 str[8];

    RelativeHumidityFloat = 100 * ((float)RelativeHumidityRaw * 100 / 65535);
    RelativeHumidityInt = (UINT_16)(RelativeHumidityFloat);
      
    sprintf(str, "%d" , RelativeHumidityInt);
    
    EUSART_Write(' ');
    EUSART_Write('H');
    EUSART_Write(str[0]);
    EUSART_Write(str[1]);
    EUSART_Write('.');
    EUSART_Write(str[2]);
    EUSART_Write(str[3]);
    
    RelativeHumidityTemp = (UINT_32)RelativeHumidityRaw * 100;
    RelativeHumidity_U8 = (UINT_8)(RelativeHumidityTemp >> 16);
    RelativeHumidity_L8 = (UINT_8)(RelativeHumidityTemp >> 8);
    RelativeHumidity_UBP8 = (UINT_16)RelativeHumidity_U8 << 8 + RelativeHumidity_L8;
    
    
//    EUSART_Write(RelativeHumidity_U8);
//    EUSART_Write(RelativeHumidity_L8);
    
    return RelativeHumidity_UBP8 ;
}

UINT_16 SHT2x_CalcTemperatureC(void)
{
    SINT_32 TemperatureTemp;
    UINT_16 Temperature_UBP8;
    //-- calculate temperature [°C] --
    //  temperatureC= -46.85 + 175.72/65536 *(ft)u16sT; //T= -46.85 + 175.72 * ST/2^16

    TemperatureTemp = (SINT_32)TemperatureRaw * (SINT_32)(175 * BIN8CONV);
    TemperatureTemp -= (SINT_32)(45 * BIN16CONV * BIN8CONV);
    if(TemperatureTemp < 0)TemperatureTemp = 0;
    TemperatureTemp >>= 8;
    TemperatureTemp >>= 8;
    Temperature_UBP8 = (UINT_16)TemperatureTemp;
    return(Temperature_UBP8);
}

UINT_16 SHT2x_CalcRelativeHumidity(void)
{
    SINT_32 RelativeHumidityTemp;
    UINT_16 RelativeHumidity_UBP8;
    //-- calculate temperature [°C] --
    //  temperatureC= -46.85 + 175.72/65536 *(ft)u16sT; //T= -46.85 + 175.72 * ST/2^16

    RelativeHumidityTemp = (SINT_32)RelativeHumidityRaw * (SINT_32)(125 * BIN8CONV);
    RelativeHumidityTemp -= (SINT_32)(6 * BIN16CONV * BIN8CONV);
    if(RelativeHumidityTemp < 0)RelativeHumidityTemp = 0;
    RelativeHumidityTemp >>= 8;
    RelativeHumidityTemp >>= 8;
    RelativeHumidity_UBP8 = (UINT_16)RelativeHumidityTemp;
    if(RelativeHumidity_UBP8 >(UINT_16)(100.0 * BIN8CONV))
    {
        RelativeHumidity_UBP8 = (UINT_16)(100.0 * BIN8CONV);
    }
    return(RelativeHumidity_UBP8);
}


