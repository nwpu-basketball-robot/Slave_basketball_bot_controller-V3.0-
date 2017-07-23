#include "data.h"


/********************************************

    此部分代码对SPI2进行初始化以完成NRF24L01的初始化工作

********************************************/


u8 msg[8];                                                                         //定义此数组用于存放CAN需要向驱动盒发送的数据                          
extern robot basketballbot;


//对驱动盒初始化操作使其进入速度模式
void DRIVER_Init()
{
     int cnt;
	
	   for(cnt=0;cnt<8;cnt++)
         msg[cnt]=0x55;  
	   CAN1_Send_Msg(msg  ,8, 0x00);                                                 //复位驱动盒，广播命令
     delay_ms(600);                                                                //延时等待驱动盒复位完成
	
	   msg[0]=0x03;
     CAN1_Send_Msg(msg  ,8, 0x01);                                                 //驱动盒进入速度模式，广播命令
     delay_ms(500);
	     Set_Speed_Robot_To_Motor();
	    DRIVER_Set_Speed();
}


void DRIVER_Set_Speed()
{ 
	    msg[0]=0x10;                                                                   //PWM上限设置为5000
	   msg[1]=0x00;
	   msg[2]=(unsigned char)((basketballbot.motor_speed.M4>>8)&0xff);       //设置电机1转速
	   msg[3]=(unsigned char)(basketballbot.motor_speed.M4&0xff);
     CAN1_Send_Msg(msg  ,8, 0x44);                                                  //向驱动盒1发送电1机目标转速
     delay_us(100);
     msg[0]=0x10;                                                                   //PWM上限设置为5000
	   msg[1]=0x00;
	   msg[2]=(unsigned char)((basketballbot.motor_speed.M1>>8)&0xff);       //设置电机1转速
	   msg[3]=(unsigned char)(basketballbot.motor_speed.M1&0xff);
     CAN1_Send_Msg(msg  ,8, 0x14);                                                  //向驱动盒1发送电1机目标转速
     delay_us(100);
     msg[0]=0x10;                                                                   //PWM上限设置为5000
	   msg[1]=0x00;
	   msg[2]=(unsigned char)((basketballbot.motor_speed.M2>>8)&0xff);       //设置电机1转速
	   msg[3]=(unsigned char)(basketballbot.motor_speed.M2&0xff);
     CAN1_Send_Msg(msg  ,8, 0x24);                                                  //向驱动盒1发送电1机目标转速
	   delay_us(100);
	   msg[0]=0x10;                                                                   //PWM上限设置为5000
	   msg[1]=0x00;
	   msg[2]=(unsigned char)((basketballbot.motor_speed.M3>>8)&0xff);       //设置电机1转速
	   msg[3]=(unsigned char)(basketballbot.motor_speed.M3&0xff);	         
     CAN1_Send_Msg(msg  ,8, 0x34);       
     CAN1_Send_Msg(msg  ,8, 0x34);   		 //向驱动盒1发送电1机目标转速
     delay_us(100);

}
