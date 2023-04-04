/* 
 * File:   UART.h
 * Author: GEII Robot
 *
 * Created on 4 avril 2023, 09:18
 */

#ifndef UART_H
#define UART_H

void InitUART(void);
void SendMessageDirect(unsigned char* message, int length);

#endif /* UART_H */
//24rx-36tx