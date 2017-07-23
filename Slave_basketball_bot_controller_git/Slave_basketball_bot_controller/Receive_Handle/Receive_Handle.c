#include "data.h"

u8 One_handle[20];
int noselect;
extern u8 buffer[MAX_DATA_LEN];
u16 checksumsend;
extern robot basketballbot;
extern Angle_PID ANGLE;

void Receive_Handle()
{
	   int flag;
     flag=First_handle();
	   if(flag==1) ;
     else Second_handle();
}


int First_handle()
{
	  int cnt=0;	
	
		switch(buffer[0])
		{	
			case 1: noselect = 0x01 ;break;		   //机器人世界坐标系速度	
			case 2: noselect = 0x02 ;break;		   //读取三个电机的角速度
			case 3: noselect = 0x03 ;break;	     //重置编码器数据		 
      case 4: noselect = 0x04 ;break;	     //读取x,y,w坐标
      case 5: noselect = 0x05 ;break;      //控制机器人坐标系速度			
			default:break;
		}
		
		
		if(noselect == 0x01 )			             //机器人世界坐标系速度信息转移到radBufferRS485[i]
		{						
			for(cnt=0;cnt< 12 ; cnt++)												
			{
				One_handle[cnt] = buffer[cnt+1] ;							
			}		
      //System.Signal_Update = 1 ;    //控制信号更新
		  Third_handle_global();									
			noselect=0;			
			return 1;
		}
		
		
		if(noselect == 0x05 )			             //电机速度信息转移到radBufferRS485[i]
		{						
			for(cnt=0;cnt< 12 ;cnt++)												
			{
				One_handle[cnt] = buffer[cnt+1] ;	
			}		
      //System.Signal_Update = 1 ;    //控制信号更新
			Third_handle_robot();										
			noselect=0;			
			return 1;
		}
		return 0;
}



void Second_handle()
{	
	if(USART_GetFlagStatus(USART3, USART_FLAG_TC)!=RESET)
	{	
	 	if(noselect == 0x02)			 // 读取速度 
		{	
		  Palstance_back();
			noselect=0;			  					
	 	}

	  else if(noselect==0x03)			   //重置编码器的数据
		{	
		 
			Robot_clear();		
			noselect=0;
		}

		else if(noselect == 0x04)		   //  返回FLOAT 的X Y W坐标
		{
			Coordinate_back();				
			noselect=0;		  				
	 	}
  }
   while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET);				
	
}


void Third_handle_global()
{	
		if(noselect==0x01)
		{

		//System.Control_Moder = 2;    //添加contro_mode,避免和local控制干扰

    memcpy(&basketballbot.globel_expected_speed, One_handle , 12);	  //
	  basketballbot.globel_expected_speed.x=Speed_limit(basketballbot.globel_expected_speed.x, 0.5);  //防止速度过大，通信很难说的。
	  basketballbot.globel_expected_speed.y=Speed_limit(basketballbot.globel_expected_speed.y, 0.5);  
		basketballbot.globel_expected_speed.w=Speed_limit(basketballbot.globel_expected_speed.w, 10);
    basketballbot.pid_expected_speed.w=basketballbot.globel_expected_speed.w;
		TF_Global_To_Robot_Speed();  
		}
}


void Third_handle_robot()
{
	
		if(noselect==0x05)   
		{
	  //	System.Control_Moder = 3;    //添加contro_mode,避免和local控制干扰
    
		memcpy(&basketballbot.robot_expected_speed  , One_handle , 12);	 
	  basketballbot.robot_expected_speed.x=Speed_limit(basketballbot.robot_expected_speed.x, 2.0);  
	  basketballbot.robot_expected_speed.y=Speed_limit(basketballbot.robot_expected_speed.y, 2.0);  
		basketballbot.robot_expected_speed.w=Speed_limit(basketballbot.robot_expected_speed.w, 20);
	  basketballbot.pid_expected_speed.w=basketballbot.robot_expected_speed.w;
	  TF_Robot_To_Global_Speed();  
		}
}

void Robot_clear()
{
      basketballbot.globel_expected_speed.x=0;
	    basketballbot.globel_expected_speed.y=0;
      basketballbot.globel_expected_speed.w=0;
	
      basketballbot.real_position.x=0;
	    basketballbot.real_position.y=0;
	    basketballbot.real_position.w=0;
	
	    basketballbot.robot_expected_speed.x=0;
	    basketballbot.robot_expected_speed.y=0;
	    basketballbot.robot_expected_speed.w=0;

	    basketballbot.motor_speed.M1=0;
	    basketballbot.motor_speed.M2=0;
	    basketballbot.motor_speed.M3=0;
	    basketballbot.motor_speed.M4=0;
      
	
	    USART3_SendChar(0xff);		
			USART3_SendChar(0xff);			
			USART3_SendChar(0x01);   
			USART3_SendChar(0x00);		 
			USART3_SendChar(0x02);	   
			USART3_SendChar(0x03);    
			USART3_SendChar(0x00);		 
			USART3_SendChar(0x06);
}


void Palstance_back()
{
	    u8 Palstance[12];
	    int cnt;

     	USART3_SendChar(0xff);
			USART3_SendChar(0xff);			
			USART3_SendChar(0x01);									 										   
			USART3_SendChar(0x00);											   		
			USART3_SendChar(0x0d);											   
			USART3_SendChar(0x02);
			
			checksumsend = 0x10;     
	    memcpy( Palstance, &basketballbot.globel_real_speed , 12 );		   
			for(cnt=0 ;cnt < 12 ; cnt++)
			{
				USART3_SendChar( Palstance[cnt] );
			  checksumsend = checksumsend + Palstance[cnt];
			}
				
			checksumsend = checksumsend%255 ;
			USART3_SendChar(checksumsend) ;
}

void Coordinate_back()
{
      
	    u8 Coordinate[12];
	    int cnt;
	
      USART3_SendChar(0xff);					
			USART3_SendChar(0xff);
			USART3_SendChar(0x01);		 						
			USART3_SendChar(0x00);				
			USART3_SendChar(0x0d);		 
			USART3_SendChar(0x04);     

			checksumsend = 0x12;
    
  		memcpy( Coordinate , &basketballbot.real_position_radius , 12 );

			for(cnt=0 ; cnt < 12 ; cnt++)
			{
				USART3_SendChar( Coordinate[cnt] );
				checksumsend=checksumsend + Coordinate[cnt] ;
			}		 
	
			checksumsend =checksumsend % (0xff);
		
			USART3_SendChar(checksumsend) ;				
			noselect=0;		  				

}


float Speed_limit(float speed,float limitation)
{
     if(abs(speed)<limitation)  return speed;
	   else if (speed<0)  return -limitation;
	   else return limitation;
}


