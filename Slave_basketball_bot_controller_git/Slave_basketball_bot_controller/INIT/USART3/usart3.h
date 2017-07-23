#ifndef __UART4_H
#define __UART4_H
#include "stdio.h"	
#include "sys.h" 



void USART3_Init(u32);
void USART3_IRQHandler(void);
void USART3_SendChar(unsigned char b);
void get_imudata(void);
void clean_imudate(void);

#endif
