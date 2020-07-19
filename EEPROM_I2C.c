/*
 ==============={EEPROM_I2C.c}==================
 This file contains the function implementation for I2C EEPROM 4k byte specially
 to read and write data on and from the EEPROM. I2C functions could be used for
 any other application.
 */

#include "water_heater.h"
#define EEPROM_Address_R 0xA1 //EEPROM read address formate
#define EEPROM_Address_W 0xA0 //EEPROM write address formate

//---------------[ I2C Routines ]-------------------
//--------------------------------------------------
void I2C_Master_Init(const unsigned long baud)
{
  SSPCON = 0b00101000;
  SSPCON2 = 0;
  SSPADD = (_XTAL_FREQ/(4*baud))-1;
  SSPSTAT = 0;
  TRISC3 = 1;
  TRISC4 = 1;
}
void I2C_Master_Wait()
{
  while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F));
}
void I2C_Master_Start()
{
  I2C_Master_Wait();
  SEN = 1;
}
void I2C_Master_RepeatedStart()
{
  I2C_Master_Wait();
  RSEN = 1;
}
void I2C_Master_Stop()
{
  I2C_Master_Wait();
  PEN = 1;
}
unsigned char I2C_Master_Write(unsigned char data)
{
  I2C_Master_Wait();
  SSPBUF = data;
  while(!SSPIF); // Wait Until Completion
  SSPIF = 0;
  return ACKSTAT;
}
unsigned char I2C_Read_Byte(void)
{
  //---[ Receive & Return A Byte ]---
  I2C_Master_Wait();
  RCEN = 1; // Enable & Start Reception
  while(!SSPIF); // Wait Until Completion
  SSPIF = 0; // Clear The Interrupt Flag Bit
  I2C_Master_Wait();
  return SSPBUF; // Return The Received Byte
}
void I2C_ACK(void)
{
  ACKDT = 0; // 0 -> ACK
  I2C_Master_Wait();
  ACKEN = 1; // Send ACK
}
void I2C_NACK(void)
{
  ACKDT = 1; // 1 -> NACK
  I2C_Master_Wait();
  ACKEN = 1; // Send NACK
}
//======================================================
//---------------[ I2C EEPROM Routines ]----------------
//------------------------------------------------------
//-----[ Write Operations ]-----

// EEPROM Write Byte
void EEPROM_Write(unsigned int add, unsigned char data)
{
  // frame write byte sequence
  I2C_Master_Start();
  // Wait Until EEPROM Is IDLE
  while(I2C_Master_Write(EEPROM_Address_W))
    I2C_Master_RepeatedStart();
  //I2C_Master_Write(add>>8);
  I2C_Master_Write((unsigned char)add);
  I2C_Master_Write(data);
  I2C_Master_Stop();
  //frame end
}

// EEPROM Write Page
void EEPROM_Write_Page(unsigned int add, unsigned char* data, unsigned char len)
{
  // write page sequence
  I2C_Master_Start();
  // Wait Until EEPROM Is IDLE
  while(I2C_Master_Write(EEPROM_Address_W))
    I2C_Master_RepeatedStart();
  I2C_Master_Write(add>>8);
  I2C_Master_Write((unsigned char)add);
  for(unsigned int i=0; i<len; i++)
    I2C_Master_Write(data[i]);
  I2C_Master_Stop();
  // frame end
}
//-----------------------------
//-----[ Read Operations ]-----

//---[ Byte Read ]---
unsigned char EEPROM_Read(unsigned int add)
{
    //read byte sequence
unsigned char Data;
I2C_Master_Start();
// Wait Until EEPROM Is IDLE
while(I2C_Master_Write(EEPROM_Address_W))
  I2C_Master_RepeatedStart();

I2C_Master_Write((unsigned int)add);

I2C_Master_Start();
while(I2C_Master_Write(EEPROM_Address_R))
  I2C_Master_RepeatedStart();

Data = I2C_Read_Byte();
I2C_NACK();
I2C_Master_Stop();
// end of frame
__delay_us(10); // wait after the next frame
return Data; // send data
}

//---[ Sequential Read ]---
void EEPROM_Read_Page(unsigned int add, unsigned char* data, unsigned int len)
{
  I2C_Master_Start();
  // Wait Until EEPROM Is IDLE
  while(I2C_Master_Write(EEPROM_Address_W))
    I2C_Master_RepeatedStart();
  I2C_Master_Write(add>>8);
  I2C_Master_Write((unsigned char)add);
  I2C_Master_RepeatedStart();
  I2C_Master_Write(EEPROM_Address_R);
  for(unsigned int i=0; i<len; i++)
  {
    data[i] = I2C_Read_Byte();
    I2C_ACK();
  }
  I2C_Master_Stop();
  //end of frame
}
