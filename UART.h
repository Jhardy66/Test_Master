/*
 * UART.h
 *
 *  Created on: Feb 25, 2019
 *      Author: jorda
 */

#ifndef UART_H_
#define UART_H_
void uart_init(int br);
void uart_write_fast_string(int vals, int vale,unsigned char * outbuff);
extern unsigned char tx_data_str[53],rx_data_str[53];
#endif /* UART_H_ */
