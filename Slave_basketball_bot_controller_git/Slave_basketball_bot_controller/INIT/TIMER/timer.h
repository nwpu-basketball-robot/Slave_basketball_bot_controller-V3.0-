#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"

#define TIM_14_Duration    0.01f

void TIMER_Init(void); 	

void TIM2_Init(u16,u16);
void TIM14_Init(u16,u16);
void TIM8_TRG_COM_TIM14_IRQHandler(void);
#endif
