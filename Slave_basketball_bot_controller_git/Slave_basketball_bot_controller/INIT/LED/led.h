#ifndef __LED_H
#define __LED_H
#include "sys.h"



//LED�˿ڶ���
#define LED0 PBout(0)	// DS0
#define LED1 PDout(8)	// DS1	 
#define LED2 PAout(15)	// DS1	 

void LED_Init(void);//��ʼ��		 				    
#endif
