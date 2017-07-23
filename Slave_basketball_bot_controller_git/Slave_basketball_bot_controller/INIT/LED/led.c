#include "data.h"



/********************************************

      此部分代码对3个LED进行初始化
			LED0    ----    PC(6)
			LED1    ----    PD(8)
			LED2    ----    PE(5)

********************************************/


    
void LED_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;

 // RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);             //使能GPIOA时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);             //使能GPIOA时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);             //使能GPIOA时钟
	
/********************************************************************/
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;                        //LED2对应IO口
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;                    //普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                   //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;               //100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                     //上拉
  GPIO_Init(GPIOA, &GPIO_InitStructure);                           //初始化GPIO
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;                        //LED1对应IO口
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;                    //普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                   //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;               //100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                     //上拉
  GPIO_Init(GPIOD, &GPIO_InitStructure);                           //初始化GPIO
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;                        //LED0对应IO口
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;                    //普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                   //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;               //100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                     //上拉
  GPIO_Init(GPIOB, &GPIO_InitStructure);                           //初始化GPIO
	 
/********************************************************************/
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_0);                                //LED0灭
	GPIO_ResetBits(GPIOD,GPIO_Pin_8);                                //LED1灭
	GPIO_ResetBits(GPIOA,GPIO_Pin_15);	                               //LED2灭

}


