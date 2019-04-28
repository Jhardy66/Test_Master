/*
 * UART.c
 *
 *  Created on: Feb 25, 2019
 *      Author: jorda
 */
#include <msp430.h>
#include "main.h"
#define uart_max 24
#define i2c_max  12

unsigned char tx_data_str[uart_max], rx_data_str[uart_max], rx_flag=0, dec_str[6], eos_flag=0;
char dec_char[6];
int tx_ptr,e_tx_ptr;
unsigned char i2cTXData[53],i2cRXData[i2c_max];
unsigned char inbuff;
volatile int i2cTXData_ptr=0,i2cRXData_ptr=0,i2crxflag=0;
volatile int i2cmode=0;
unsigned int * ptr;

void uart_write_fast_string(int vals, int vale,unsigned char * outbuff);
void uart_init(int br){
    volatile int temp=0;
    //Set baud rate to 300, 1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600
    // use index of 0 1 2 3... corresponding to the rates above
    volatile unsigned int brvec[]={0x55, 0x15, 0x0B, 0x05, 0x83, 0x41, 0xA1, 0x16, 139, 69, 35, 17};
    volatile unsigned int hrvec[]={0xD0,0x34,0x1A,0x0D,6,3,1,1,0,0,0,0};
    BCSCTL1 = CALBC1_16MHZ;                    // Set DCO
    DCOCTL = CALDCO_16MHZ;
    P1SEL |= (BIT1+BIT2);                             // P3.4,5 = USCI_A0 TXD/RXD
    P1SEL2 |= (BIT1+BIT2);
    //  UCA0CTL1 |= UCSWRST;
    UCA0CTL1 |= UCSSEL_2;                     // SMCLK
    UCA0BR0 = brvec[br];
    UCA0BR1 = hrvec[br];
    UCA0MCTL = UCBRS0;                        // Modulation UCBRSx = 1
    UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
    IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt
    __bis_SR_register(GIE);                 // interrupts enabled
}

void uart_write_fast_string(int vals, int vale,unsigned char * outbuff){
    unsigned int i,k;                                  // writes a string from global variable tx_data_str.  vals is starting pointer and vale is the ending value


    tx_ptr=vals;                                // writes a string from global variable tx_data_str.  vals is starting pointer and vale is the ending value
    e_tx_ptr=vale;                              //  Uses interrupts to send out bytes

    for(i=vals;i<vale;i++){
        while (!(IFG2&UCA0TXIFG));
             UCA0TXBUF = *(outbuff + i);
    }

}


#pragma vector=USCIAB0TX_VECTOR
__interrupt void USCI0TX_ISR(void)
{

}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
    volatile char temp;
    if(IFG2 & UCA0RXIFG){
        // Receive data on UART
        rx_data_str[rx_flag]=UCA0RXBUF;             // data is stored in rx_data_str
        temp=rx_data_str[rx_flag];
        while (!(IFG2&UCA0TXIFG));
        //  UCA0TXBUF=temp;
        if (rx_data_str[rx_flag]=='\r'){             // new line or carriage return set eos_flag global variable
            eos_flag=1;
            rx_parser(rx_data_str);
            rx_flag = 0;
        }
        else{
            rx_flag++;
        }
        if (rx_data_str[rx_flag]=='\n')
            eos_flag=1;


        if (rx_flag>uart_max){                          // maximum of characters starts at the beginning again
            rx_flag=0;
        }

    }

}


