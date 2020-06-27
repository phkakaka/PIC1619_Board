#ifndef __IIC_DRIVER_
#define __IIC_DRIVER_
#include "../COS/GlobalDef.h"

#define HIGH 1
#define LOW 0
#define ACK_ERROR       1
#define TIME_OUT_ERROR  2
#define CHECKSUM_ERROR  4
#define ACK 0
#define NO_ACK 1

extern void I2C_Init(void);
extern void I2C_StartCondition(void);
extern void I2C_StopCondition(void);
extern void I2C_MasterSendAck(void);
extern void I2C_MasterSendNack(void);
extern UINT_8 I2C_WriteByte (UINT_8 txByte);
extern UINT_8 I2C_ReadByte (UINT_8 ack);

extern void I2C_Init_IO(void);
extern void I2C_StartCondition_IO(void);
extern UINT_8 I2C_WriteByte_IO (UINT_8 txByte);


#endif

