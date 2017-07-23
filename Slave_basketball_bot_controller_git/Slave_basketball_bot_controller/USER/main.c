#include "data.h"

System_flag System;
volatile Queue rx_queue;
robot basketballbot;  
Angle_PID ANGLE;
u8  buffer[MAX_DATA_LEN] = {0};
enum Recstate								   	
{
		 RECFF1,RECFF2,SENDID,RECID,RECLEN1,RECLEN2,RECSEL,RECCHECK
}Recstate = RECFF1;
		
		
		
/***************速度赋值**************/
void BASE_Init(void);

		
int main(void)


  {	  
	  static uint16_t checksum;
    u8 datalen1;
    short data_len;
    int cur=0;
    u8 data;
	
		BASE_Init();
	
		while(1){
		//	NRF24L01_RX();
      if(queue_empty(rx_queue))
							continue;

		data = (uint8_t)queue_get(&rx_queue);
  
		switch (Recstate) 
		{		case RECFF1:
			  LED2=!LED2;		
				if (data == 0xff)	  
				{	
					Recstate = RECFF2;	
					checksum=0;		
					cur = 0;								//校验位清零
				}
	
				break;
	
			case RECFF2:
				if (data == 0xff)
					Recstate = RECID;
				else
					Recstate = RECFF1;
				break;
	
	
			case RECID:				 					
			  if(data==0x01)	       //设备编号0x01底盘
			    {
					checksum += data;
					Recstate = RECLEN1;
				  }
				else
					Recstate = RECFF1;	  
				break;
	
			case RECLEN1:				 				
				checksum += data;
				datalen1 = data;
				Recstate = RECLEN2;	  
				break;
		
			case RECLEN2:	
				checksum += data;			 				
				data_len = (short)datalen1 << 8	 | data;
				Recstate = RECSEL;	  
				break;
	
			case RECSEL:
				checksum += data;
				buffer[cur++] = data;
				if(cur >= data_len)
					Recstate = RECCHECK;
				break;
	
			case RECCHECK:
				checksum=checksum%255;
				if(data == checksum)
				{					
					Receive_Handle();			
					checksum=0;	
					Recstate = RECFF1;	 
				}
				else
					Recstate = RECFF1;
				break;
			 default:
					Recstate = RECFF1;
		 }
			 
		}
}

void BASE_Init()
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);               //设置系统中断优先级分组2
	  delay_init(168);                                              //初始化延时函数
    USART1_Init();                                  	        //初始化串口1波特率为115200,GPIOA的始终在此被使能
	  USART3_Init(115200);                                          //初始化串口3波特率为115200,GPIOB的始终在此被使能   
	  LED_Init();					                                          //初始化LED ,GPIOC、GPIOD、GPIOE的始终在此被使能   		                                      //初始化NRF24L01,GPIOG的始终在此被使能
	  CAN1_Mode_Init(CAN_SJW_1tq,CAN_BS2_6tq,CAN_BS1_7tq,6,0); //CAN1初始化正常模式,波特率500Kbps    
	  DRIVER_Init();                                            		//驱动盒复位并进入速度模式
		TIMER_Init();
    LED2=!LED2;
	  PID_Init();
	
}

