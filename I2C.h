/*
 * I2C.h
 *
 *  Created on: Feb 25, 2019
 *      Author: jorda
 */

#ifndef I2C_H_
#define I2C_H_


unsigned char Read_SCL(void);
unsigned char Read_SDA(void);
void Clear_SCL(void);
void Clear_SDA(void);

unsigned char I2C_ADD_SCAN(void);

void I2C_Init(void);
void I2C_Start(void);
void I2C_Stop(void);

void I2C_Writebit(unsigned char bit);
unsigned char I2C_ReadByte(void);

void I2C_WriteByte(unsigned char Data);
unsigned char I2C_ReadByte(void);

void I2C_WriteData(unsigned char Data[],int n);
void I2C_ReadData(unsigned char *Buff, unsigned char Register, unsigned char nLength);

#endif /* I2C_H_ */
