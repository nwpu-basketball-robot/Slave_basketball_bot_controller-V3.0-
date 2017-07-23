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
		
		
		
/***************�ٶȸ�ֵ**************/
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
					cur = 0;								//У��λ����
				}
	
				break;
	
			case RECFF2:
				if (data == 0xff)
					Recstate = RECID;
				else
					Recstate = RECFF1;
				break;
	
	
			case RECID:				 					
			  if(data==0x01)	       //�豸���0x01����
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
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);               //����ϵͳ�ж����ȼ�����2
	  delay_init(168);                                              //��ʼ����ʱ����
    USART1_Init();                                  	        //��ʼ������1������Ϊ115200,GPIOA��ʼ���ڴ˱�ʹ��
	  USART3_Init(115200);                                          //��ʼ������3������Ϊ115200,GPIOB��ʼ���ڴ˱�ʹ��   
	  LED_Init();					                                          //��ʼ��LED ,GPIOC��GPIOD��GPIOE��ʼ���ڴ˱�ʹ��   		                                      //��ʼ��NRF24L01,GPIOG��ʼ���ڴ˱�ʹ��
	  CAN1_Mode_Init(CAN_SJW_1tq,CAN_BS2_6tq,CAN_BS1_7tq,6,0); //CAN1��ʼ������ģʽ,������500Kbps    
	  DRIVER_Init();                                            		//�����и�λ�������ٶ�ģʽ
		TIMER_Init();
    LED2=!LED2;
	  PID_Init();
	
}

