#include <msp430.h> 
#include "I2C.h"
#include "UART.h"

/**
 * main.c
 */

unsigned char mainbuff[54];
void rx_parser(unsigned char *buff);
long Twobytes2int(char MSB, char LSB);
int main(void){
    int baud = 8;
    unsigned char adc[12] = 0;
    unsigned char qei[4] = 0;
    unsigned int sIndex,eIndex;


    I2C_Init();
    uart_init(8);
    WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
    unsigned char arr[] = "HEllo";
    while(1){
    //    I2C_WriteData(arr,5);
      //  I2C_ReadData(adc,0x0B,12);
      //  I2C_ReadData(qei,0x09,4);
        __bis_SR_register(GIE);
    }
}

void rx_parser(unsigned char *buff){
    unsigned char temp;
    volatile unsigned int i,j,k;
    unsigned char qbuff[5];
    unsigned char readbuff[12];
    unsigned char readregbuff[2];
    unsigned char adcbuf[12];
    int sindex,eindex;

    I2C_ReadData(mainbuff,0,54);
    switch(*buff){


    case 'c':

        I2C_WriteData((buff + 2),5);

        break;

    case 'K':

        uart_write_fast_string(11, 12,mainbuff);
        break;

    case 'Q':

        uart_write_fast_string(0,4,mainbuff);
        break;


    case 'J':
        sindex = rx_data_str[1];
        eindex = 2;


        uart_write_fast_string(sindex,sindex + 2,mainbuff);


        break;
    }

}
long Twobytes2int(char MSB, char LSB){
    int output = 0;

    output = LSB | (MSB << 8);

    return output;

}
