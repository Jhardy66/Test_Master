#include <msp430.h>
#define SDA BIT7 //CHANGE TO 4
#define SCL BIT6 //CHANGE TO 3
#define I2C_M_OUT P2OUT //CHANGE TO 3
#define I2C_M_IN P2IN
#define I2C_DIR P2DIR //CHANGE TO 3
#define delay() __delay_cycles(100)
#define I2C_DELAY() __delay_cycles(100)
#define ACK  0x00
#define NACK 0x01

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

void I2C_WriteData(unsigned char * Data,int n);
void I2C_ReadData(unsigned char *Buff, unsigned char Register, unsigned char nLength);

unsigned char SL_ADD = 0x30;
/**
 * main.c
 */

unsigned char Read_SCL(void)
{
    I2C_DIR  &= ~SCL;
    return((I2C_M_IN & SCL) != 0);

}

unsigned char Read_SDA(void)
{
    I2C_DIR  &= ~SDA;
    return((I2C_M_IN & SDA) != 0);
}

void Clear_SCL(void)
{
    I2C_DIR  |= SCL;
}

void Clear_SDA(void)
{
    I2C_DIR  |= SDA;
}

void I2C_Init(void)
{

    //Set pins, change to P3
    P2SEL   &= ~(SCL + SDA);
    P2SEL2  &= ~(SCL + SDA);
    // Set SCL and SDA HI
    I2C_DIR  &= ~(SCL + SDA);
    I2C_M_OUT  &= ~(SCL + SDA);
}

void I2C_Start(void)
{
    Read_SDA();             //set SDA to 1
    I2C_DELAY();
    Clear_SDA();            //set SDA to 0, currently SCL is 1
    I2C_DELAY();
    Clear_SCL();            //set SCL to 0
}
void I2C_Stop(void)
{
    Clear_SDA();            //set SDA to 0
    I2C_DELAY();
    Read_SCL();             //set SCL to 1
    I2C_DELAY();
    Read_SDA();             //set SDA to 1
}

void I2C_Writebit(unsigned char bit)
{
    if(bit)
        Read_SDA();
    else
        Clear_SDA();
    I2C_DELAY();
    Read_SCL();
    I2C_DELAY();
    Clear_SCL();
}

unsigned char I2C_Readbit(void)
{
    unsigned char bit;
    //Let the slave driver data
    Read_SDA();
    I2C_DELAY();
    Read_SCL();
    bit = Read_SDA();
    I2C_DELAY();
    Clear_SCL();
    return bit;
}

void I2C_WriteByte(unsigned char Data)
{
    unsigned char nBit;

    for(nBit = 0; nBit < 8; nBit++)
    {
        I2C_Writebit((Data & 0x80) != 0);
        Data <<= 1;
    }
    I2C_Readbit();
}

unsigned char I2C_ReadByte(void)
{
    unsigned char Buff = 0;
    unsigned char nBit;

    for(nBit = 0; nBit < 8; nBit++)
    {
        Buff = (Buff << 1) | I2C_Readbit();
    }
    return Buff;
}

void I2C_WriteData(unsigned char * Data,int n)
{
    unsigned char nIndex;
    I2C_Start();
    I2C_WriteByte(SL_ADD << 1);
    for(nIndex = 0; nIndex < n; nIndex++)
    {
        I2C_WriteByte(*(Data + nIndex));
    }
    I2C_Readbit();
    I2C_Stop();
}

void I2C_ReadData(unsigned char *Buff, unsigned char Register, unsigned char nLength)
{
    unsigned char nIndex;
    unsigned char nuller[1] = {0};
    I2C_Start();
    I2C_WriteByte(SL_ADD << 1);
    I2C_WriteByte(Register);
    I2C_Stop();
    _NOP();                                 // Short delay
    I2C_Start();
    _NOP();                                 // Short delay
    I2C_WriteByte((SL_ADD << 1) | 1);
    for(nIndex = 0; nIndex < nLength; nIndex++)
    {
        *(Buff + nIndex) = I2C_ReadByte();
        if(nIndex < nLength - 1) I2C_Writebit(ACK);
    }
    I2C_Writebit(NACK);
    I2C_Stop();

}
