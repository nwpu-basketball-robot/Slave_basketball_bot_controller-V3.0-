#ifndef __DATA_H
#define __DATA_H
#include "sys.h"
#include "led.h"
#include "usart.h"
#include "usart3.h"
#include "delay.h"
#include "can.h"
#include "driver.h"
#include "stm32f4xx.h"
#include "transform.h"
#include "queue.h"
#include "timer.h"
#include "math.h"
#include "stdlib.h"
#include "stdio.h"
#include "math.h"
#include "receive_handle.h"
#include "angle_pid.h"


#define  Motor_Num               4 
#define  MAX_DATA_LEN           100   //数据接收缓冲区大小
#define  wheel_R                0.075000f
#define  wheel_D                0.150000f
#define  pi                     3.141592f
#define  encoder_barrier        512*4
#define  Radius                 0.28281f              //轮子到质心的半径  需要修改
#define  TEM_PWM                5000
#define  Reduction_ratio        28.5


typedef struct System{   // 系统各种标志
	      int8_t Identity ;        //ID 号
	      int8_t Signal_Update;    //通信更新标志
	      int8_t Signal_error;     //通信错误标志
	      int8_t S_Initialize;     //系统状态  0  系统没初始化   |1   系统初始化成功
	      float  Sys_Time;         //系统时钟 
	      float  CPU_T;            //CPU温度
	      float  Eledat;           //电流大小	
	      int8_t Elemoder;         //过流标志   1为过流
        int8_t Control_Moder;    // 控制模式
	      int16_t S_cnt1 ;
	      int16_t S_cnt2 ;
	      int16_t S_cnt3 ;
        }System_flag;      // 系统时间

				
typedef struct Angle_PID{
        float SET_Angle_now;
	      float SET_Angle_last;
	      float ACTUAL_Angle;
	      float ERROR_Angle;
	      float ERROR_LAST_Angle;
	      float Kp,Ki,Kd;
        float SPEED_W_PID;
	      float INTEGRAL;
        }Angle_PID;

				
typedef struct motor{
	int M1;
	int M2;
	int M3;
	int M4;
} motor;


typedef struct vector{
	float x;
	float y;
	float w;
}vector;


typedef struct robot{

  motor motor_speed;
	
  vector globel_real_speed;
	vector robot_real_speed;
	vector real_position;
	vector real_position_radius;
	vector pid_expected_speed;
	vector globel_expected_speed;	
	vector robot_expected_speed;
	int Control_Moder_globel;
	int Control_Moder_robot;
 
}robot;

#endif
				
				
		
				
				
				
