#include "data.h"


/********************************************

    �˲��ִ����SPI2���г�ʼ�������NRF24L01�ĳ�ʼ������

********************************************/


u8 msg[8];                                                                         //������������ڴ��CAN��Ҫ�������з��͵�����                          
extern robot basketballbot;


//�������г�ʼ������ʹ������ٶ�ģʽ
void DRIVER_Init()
{
     int cnt;
	
	   for(cnt=0;cnt<8;cnt++)
         msg[cnt]=0x55;  
	   CAN1_Send_Msg(msg  ,8, 0x00);                                                 //��λ�����У��㲥����
     delay_ms(600);                                                                //��ʱ�ȴ������и�λ���
	
	   msg[0]=0x03;
     CAN1_Send_Msg(msg  ,8, 0x01);                                                 //�����н����ٶ�ģʽ���㲥����
     delay_ms(500);
	     Set_Speed_Robot_To_Motor();
	    DRIVER_Set_Speed();
}


void DRIVER_Set_Speed()
{ 
	    msg[0]=0x10;                                                                   //PWM��������Ϊ5000
	   msg[1]=0x00;
	   msg[2]=(unsigned char)((basketballbot.motor_speed.M4>>8)&0xff);       //���õ��1ת��
	   msg[3]=(unsigned char)(basketballbot.motor_speed.M4&0xff);
     CAN1_Send_Msg(msg  ,8, 0x44);                                                  //��������1���͵�1��Ŀ��ת��
     delay_us(100);
     msg[0]=0x10;                                                                   //PWM��������Ϊ5000
	   msg[1]=0x00;
	   msg[2]=(unsigned char)((basketballbot.motor_speed.M1>>8)&0xff);       //���õ��1ת��
	   msg[3]=(unsigned char)(basketballbot.motor_speed.M1&0xff);
     CAN1_Send_Msg(msg  ,8, 0x14);                                                  //��������1���͵�1��Ŀ��ת��
     delay_us(100);
     msg[0]=0x10;                                                                   //PWM��������Ϊ5000
	   msg[1]=0x00;
	   msg[2]=(unsigned char)((basketballbot.motor_speed.M2>>8)&0xff);       //���õ��1ת��
	   msg[3]=(unsigned char)(basketballbot.motor_speed.M2&0xff);
     CAN1_Send_Msg(msg  ,8, 0x24);                                                  //��������1���͵�1��Ŀ��ת��
	   delay_us(100);
	   msg[0]=0x10;                                                                   //PWM��������Ϊ5000
	   msg[1]=0x00;
	   msg[2]=(unsigned char)((basketballbot.motor_speed.M3>>8)&0xff);       //���õ��1ת��
	   msg[3]=(unsigned char)(basketballbot.motor_speed.M3&0xff);	         
     CAN1_Send_Msg(msg  ,8, 0x34);       
     CAN1_Send_Msg(msg  ,8, 0x34);   		 //��������1���͵�1��Ŀ��ת��
     delay_us(100);

}
