#include "data.h"

extern robot basketballbot;
int cnt;
extern uint8_t radBufferRS485[99]; 
extern Angle_PID ANGLE;

void TIMER_Init()
{   
	  TIM2_Init(85,8400-1);                       //5ms
	  TIM14_Init(100-1,8400-1);                 //10ms进一次中断

}



void TIM2_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  ///使能TIM3时钟
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);//初始化TIM3
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	TIM_Cmd(TIM2,ENABLE); //先不使能用的时候再使能
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn; //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x03; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x01; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

void TIM14_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14,ENABLE);  ///使能TIM3时钟
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM14,&TIM_TimeBaseInitStructure);//初始化TIM3
	
	TIM_ITConfig(TIM14,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	TIM_Cmd(TIM14,ENABLE); //先不使能用的时候再使能
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM8_TRG_COM_TIM14_IRQn; //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x02; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}


void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET) //溢出中断
	{
	  if(basketballbot.Control_Moder_robot==1)	TF_Robot_To_Global_Speed();  
	  if(basketballbot.Control_Moder_globel==1)  TF_Global_To_Robot_Speed();
		Set_Speed_Robot_To_Motor();
	  DRIVER_Set_Speed();
	}
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);  //清除中断标志位
}


void TIM8_TRG_COM_TIM14_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM14,TIM_IT_Update)==SET) //溢出中断
	{
		 if( basketballbot.pid_expected_speed.w==0)
		   { 
				 ANGLE.ACTUAL_Angle=basketballbot.real_position.w;
				 if(fabs(ANGLE.SET_Angle_now-ANGLE.SET_Angle_last)<1)
   					ANGLE.SET_Angle_now=ANGLE.SET_Angle_last;
				 else ANGLE.SET_Angle_now=basketballbot.real_position.w;			 
				 ANGLE.SET_Angle_last=ANGLE.SET_Angle_now;
			      ANGLE.Kp=1;
				    ANGLE.Kd=0.2;
				    ANGLE.Ki=0.015;
			   ANGLE_ADJUST();
			 }
		 else PID_Init();
		  
	}
	TIM_ClearITPendingBit(TIM14,TIM_IT_Update);  //清除中断标志位
}

void ANGLE_ADJUST()
{
	   ANGLE.ERROR_Angle= ANGLE.SET_Angle_now-ANGLE.ACTUAL_Angle;
	   if(ANGLE.ERROR_Angle>180) ANGLE.ERROR_Angle=ANGLE.ERROR_Angle-360;
	   if(ANGLE.ERROR_Angle<-180) ANGLE.ERROR_Angle=ANGLE.ERROR_Angle+360;
     ANGLE.INTEGRAL+=ANGLE.ERROR_Angle;
	   ANGLE.SPEED_W_PID=ANGLE.Kp*ANGLE.ERROR_Angle+ANGLE.Ki*ANGLE.INTEGRAL+ANGLE.Kd*(ANGLE.ERROR_Angle-ANGLE.ERROR_LAST_Angle);
	   ANGLE.ERROR_LAST_Angle=ANGLE.ERROR_Angle;
	   ANGLE.SPEED_W_PID=ANGLE.SPEED_W_PID*(-1);
	   basketballbot.robot_expected_speed.w=ANGLE.SPEED_W_PID;
	
	



}
