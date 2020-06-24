#include "I2C_Driver.h"
#include <xc.h>

#define SDA LATBbits.LATB4
#define SCL LATBbits.LATB6

void I2C_Init(void)
{
    SSP1STAT = 0x00;
    SSP1CON1 = 0x08;
    SSP1CON2 = 0x00;
    SSP1ADD  = 0x27;
    SSPEN = 1;  //Enable MSSP
    SSP1IF = 0;
    SSP1IE = 1;
}

void I2C_Init_IO(void)
{
   SDA = LOW; 
   SCL = LOW;
   
   SDA = HIGH;
   SCL = HIGH;
}

void I2C_StartCondition_IO(void)
{
   SDA = HIGH;
   SCL = HIGH;
   
   SDA = LOW;
   __delay_us(10);
   SCL = LOW;
   __delay_us(10);
}

UINT_8 I2C_WriteByte_IO (UINT_8 txByte)
{
  UINT_8 mask,error=0;
  
  for (mask=0x80; mask>0; mask>>=1)   //shift bit for masking (8 times)
  { 
    if ((mask & txByte) == 0) 
    {
        SDA=LOW;//masking txByte, write bit to SDA-Line
    }
    else 
    {
        SDA=HIGH;
    }
    
    __delay_us(1);             //data set-up time (t_SU;DAT)
    SCL=HIGH;                         //generate clock pulse on SCL
    __delay_us(10);             //SCL high time (t_HIGH)
    SCL=LOW;
    __delay_us(1);             //data hold time(t_HD;DAT)
  }
  SDA=HIGH;                           //release SDA-line
  SCL=HIGH;                           //clk #9 for ack
  __delay_us(1);               //data set-up time (t_SU;DAT)
  if(SSP1CON2&0x40 == HIGH) error=ACK_ERROR; //check ack from i2c slave
  SCL=LOW;
  __delay_us(20);              //wait time to see byte package on scope
  return error;                       //return error code
}

void I2C_StartCondition(void)
{
    SSP1IF = 0;
    SSP1CON2bits.SEN = 1;    // initiate start condition
    while(!SSP1IF);
    SSP1IF = 0;
}

void I2C_StopCondition(void)
{
    SSP1IF = 0;
    SSP1CON2bits.PEN = 1;    // initiate stop condition
    while(!SSP1IF);
    SSP1IF = 0;
}

void I2C_MasterSendAck(void)
{
    SSP1CON2bits.ACKDT = 0;
    SSP1CON2bits.ACKEN = 1;
}

void I2C_MasterSendNack(void)
{
    SSP1CON2bits.ACKDT = 1;
    SSP1CON2bits.ACKEN = 1;
}

UINT_8 I2C_WriteByte (UINT_8 txByte)
{
    UINT_8 ack;
    ack = 0;
    
    SSP1IF = 0;
    SSP1BUF = txByte;
    while(!SSP1IF);
    SSP1IF = 0;
    SSP1STAT = 0;
    ack |= SSP1CON2&0x40;
    return ack; //return ACK (0) or NACK(1)
}

UINT_8 I2C_ReadByte (UINT_8 ack)
{
    UINT_8 rxByte;
    rxByte=0;

    SSP1CON2bits.RCEN = 1;        // initiate read sequence
    while(!SSP1IF);               // wait for read complete
    SSP1IF = 0;                   // clear interrupt flag
    rxByte = SSP1BUF;             // read received byte
    if(ack == 0)SSP1CON2bits.ACKDT = 0;     // set up to ACK or NACK according to ack parameter
    else SSP1CON2bits.ACKDT = 1;     // set up to ACK or NACK according to ack parameter
    SSP1IF = 0;
    SSP1CON2bits.ACKEN = 1;           // enable ack
    while(!SSP1IF);
    SSP1IF = 0;
    return rxByte;                      //return read byte
}