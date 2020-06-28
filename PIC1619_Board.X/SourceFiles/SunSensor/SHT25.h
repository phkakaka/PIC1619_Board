/** @file Led.c
 *  @ingroup SunSensor
 *
 * @author Samuel Wang
 *
 * 
 */

#ifndef _SHT25_H_
#define _SHT25_H_

/*-- Includes --*/
#include "../COS/COS_Defs.h"

typedef enum
{
    SHT25_MEASURE_INIT,  //Init Hardware
    
    SHT25_MEASURE_TEMP_0, // trigger humidity sensor temperature measurement
    SHT25_MEASURE_TEMP_1, // poll for measurement complete and read temperature
    
    SHT25_MEASURE_TEMP_WAIT,
            
    SHT25_MEASURE_RH_0, // trigger relative humidity measurement
    SHT25_MEASURE_RH_1, // poll for measurement complete and read relative humidity

    SHT25_CALC_TEMP_0, // calculate temperature in degrees C bin 8
    SHT25_CALC_RH_0, // calculate temperature in relative humidity nim 8
} SHT25_State_tt;

#define I2C_MEASUREMENT_TIME_OUT    200
#define I2C_TASK_TIME			TIME_TSK_EVERYSLOT

// sensor command
#define TRIG_T_MEASUREMENT_HM       0xE3 // command trig. temp meas. hold master
#define TRIG_RH_MEASUREMENT_HM      0xE5 // command trig. humidity meas. hold master
#define TRIG_T_MEASUREMENT_POLL     0xF3 // command trig. temp meas. no hold master
#define TRIG_RH_MEASUREMENT_POLL    0xF5 // command trig. humidity meas. no hold master
#define USER_REG_W                  0xE6 // command writing user register
#define USER_REG_R                  0xE7 // command reading user register
#define SOFT_RESET                  0xFE  // command soft reset

#define SHT2x_RES_12_14BIT          0x00 // RH=12bit, T=14bit
#define SHT2x_RES_8_12BIT           0x01 // RH= 8bit, T=12bit
#define SHT2x_RES_10_13BIT          0x80 // RH=10bit, T=13bit
#define SHT2x_RES_11_11BIT          0x81 // RH=11bit, T=11bit
#define SHT2x_RES_MASK              0x81  // Mask for res. bits (7,0) in user reg.

#define SHT2x_EOB_ON                0x40 // end of battery
#define SHT2x_EOB_MASK              0x40 // Mask for EOB bit(6) in user reg.

#define SHT2x_HEATER_ON             0x04 // heater on
#define SHT2x_HEATER_OFF            0x00 // heater off
#define SHT2x_HEATER_MASK           0x04 // Mask for Heater bit(2) in user reg.

// measurement signal selection
#define  HUMIDITY   0
#define  TEMP       1

#define SHT25_ADDR_W   128   // sensor I2C address + write bit
#define SHT25_ADDR_R   129    // sensor I2C address + read bit

extern void SHT25_StateMachine(void);
extern void SHT25_FaultHand(void);
extern UINT_16 SHT2x_CalcTemperatureC(void);
extern UINT_16 SHT2x_CalcRelativeHumidity(void);
extern UINT_16 SHT3x_CalcTemperatureC(void);
extern UINT_16 SHT3x_CalcRelativeHumidity(void);
extern float SHT3x_CalcTemperatureC_F(void);
extern float SHT3x_CalcRelativeHumidity_F(void);
extern float GetTemperatureC_F(void);
extern float GetRelativeHumidity_F(void);
extern UINT_8 IsSHT31MeasureDone(void);

#endif
