 /********************************************//**
 *@file EEPROM_I2C.h
 *@brief This file contains the function declarations for I2C EEPROM 4k byte specially
 *to read and write data on and from the EEPROM. I2C functions could be used for
 *any other application.
 ********************************************/



 /********************************************//**
 * Usage: setup the configurations of the master node of the I2C bus
 * \n return type: void
 * \n Input parameters number: 1
 * \n Input parameters representation (1): bus speed or baud rate
@param baud -> the desired baudRate
********************************************/
void I2C_Master_Init(const unsigned long baud);



 /********************************************//**
 * Usage: Writing data on a determined address of the EEPROM
 * \n return type: void
 * \n Input parameters number: 2
 * \n Input parameters representation (1): the EEPROM address 16 bit
 * \n Input parameters representation (2): the data to be written 8 bit
 @param add -> data adress (16_bit)\n
 @param data -> the data to be written \n
********************************************/
void EEPROM_Write(unsigned int add, unsigned char data);


/********************************************//**
 * Usage: Reading data from a determined address of the EEPROM
 * \n return type: 8 bit read data from EEPROM
 * \n Input parameters number: 1
 * \n Input parameters representation (1): the EEPROM address to read from 16 bit
  @param add -> data adress (16_bit)
********************************************/
unsigned char EEPROM_Read(unsigned int add);



//---------------[ I2C ]------------------------
//--------------------------------------------------
void I2C_Master_Wait();


/*
 * Usage: Make the start condition to send an I2C frame
 */
void I2C_Master_Start();


/*
 * Usage: Make the repeated start condition to send
 *        another byte of data to a node through the same frame
 */
void I2C_Master_RepeatedStart();


/*
 * Make the stop condition of the I2C frame
 */
void I2C_Master_Stop();


/*
 * Send acknowledgement of the I2C frame
 */
void I2C_ACK();


/*
 * Send no-acknowledgement of the I2C frame
 */
void I2C_NACK();


/*
 * Initiate master write data of the I2C frame
 */
unsigned char I2C_Master_Write(unsigned char);
/*
 * return type: the status of data on I2C bus
 * Input parameters number: 1
 * Input parameters representation (1): the data to be written on I2C bus 8bit
 */



/*
 * Read data of the I2C frame 8bit
 */
unsigned char I2C_Read_Byte(void);
/*
 * return type: 8bit read data from I2C bus
 */



void EEPROM_Write_Page(unsigned int add, unsigned char* data, unsigned char len);
void EEPROM_Read_Page(unsigned int add, unsigned char* data, unsigned int len);
