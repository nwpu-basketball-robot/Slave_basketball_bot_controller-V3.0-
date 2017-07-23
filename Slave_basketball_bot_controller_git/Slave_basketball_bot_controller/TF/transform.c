#include <math.h>
#include "data.h"

extern robot basketballbot;

//机器人坐标系速度转换为电机转速
void TF_Robot_To_Motor(double Speed_goal[3],double *motor)
{
     *motor=Radius * Speed_goal[2] - Speed_goal[1] * pow(2.0,0.5)/2  + Speed_goal[0] * pow(2.0,0.5)/2 ;
     *(motor+1)=Radius * Speed_goal[2] - Speed_goal[1] * pow(2.0,0.5)/2 - Speed_goal[0] * pow(2.0,0.5)/2;
     *(motor+2)=Radius * Speed_goal[2] + Speed_goal[1] * pow(2.0,0.5)/2 - Speed_goal[0] * pow(2.0,0.5)/2;
     *(motor+3)=Radius * Speed_goal[2] + Speed_goal[1] * pow(2.0,0.5)/2 + Speed_goal[0] * pow(2.0,0.5)/2;
}


//世界坐标系速度转换为电机转速
void TF_Global_To_Motor(double Speed_goal[3],double *motor,double Angle_included)//a为机器人坐标系与世界坐标系的夹角
{
     *motor=Radius*Speed_goal[2] - Speed_goal[0]*((pow(2.0,0.5)*cos(Angle_included))/2 + (pow(2.0,0.5)*sin(Angle_included))/2) + Speed_goal[1]*((pow(2.0,0.5)*cos(Angle_included))/2 - (pow(2.0,0.5)*sin(Angle_included))/2);
     *(motor+1)=Radius*Speed_goal[2] - Speed_goal[0]*((pow(2.0,0.5)*cos(Angle_included))/2 - (pow(2.0,0.5)*sin(Angle_included))/2) - Speed_goal[1]*((pow(2.0,0.5)*cos(Angle_included))/2 + (pow(2.0,0.5)*sin(Angle_included))/2);
     *(motor+2)=Radius*Speed_goal[2] + Speed_goal[0]*((pow(2.0,0.5)*cos(Angle_included))/2 + (pow(2.0,0.5)*sin(Angle_included))/2) - Speed_goal[1]*((pow(2.0,0.5)*cos(Angle_included))/2 - (pow(2.0,0.5)*sin(Angle_included))/2);
     *(motor+3)=Radius*Speed_goal[2] + Speed_goal[0]*((pow(2.0,0.5)*cos(Angle_included))/2 - (pow(2.0,0.5)*sin(Angle_included))/2) + Speed_goal[1]*((pow(2.0,0.5)*cos(Angle_included))/2 + (pow(2.0,0.5)*sin(Angle_included))/2);
}


//机器人自身速度转换为全局速度
void TF_Robot_To_Global_Speed()   
{
	   basketballbot.globel_expected_speed.w= basketballbot.robot_expected_speed.w;
	   basketballbot.globel_expected_speed.x= basketballbot.robot_expected_speed.x*cos(basketballbot.real_position_radius.w)+ basketballbot.robot_expected_speed.y*sin(basketballbot.real_position_radius.w);
	   basketballbot.globel_expected_speed.y=-basketballbot.robot_expected_speed.x*sin(basketballbot.real_position_radius.w)+ basketballbot.robot_expected_speed.y*cos(basketballbot.real_position_radius.w);
}

void TF_Global_To_Robot_Speed()
{
     basketballbot.robot_expected_speed.w= basketballbot.globel_expected_speed.w;
	   basketballbot.robot_expected_speed.x= basketballbot.globel_expected_speed.x*cos(basketballbot.real_position_radius.w)- basketballbot.globel_expected_speed.y*sin(basketballbot.real_position_radius.w);
	   basketballbot.robot_expected_speed.y= basketballbot.globel_expected_speed.x*sin(basketballbot.real_position_radius.w)+ basketballbot.globel_expected_speed.y*cos(basketballbot.real_position_radius.w);
}


//设置世界坐标系速度
void Set_Speed_Global_To_Motor()
{
	double Speed_goal[3],motor[4];
	Speed_goal[0]=basketballbot.globel_expected_speed.x;
	Speed_goal[1]=basketballbot.globel_expected_speed.y;
	Speed_goal[2]=basketballbot.globel_expected_speed.w;
	
  TF_Global_To_Motor(Speed_goal,motor,basketballbot.real_position.w+(double)pi/2.0);
	
	basketballbot.motor_speed.M1=(motor[0]/ wheel_R ) /pi *30.0*28.5;// r/min
	basketballbot.motor_speed.M2=(motor[1]/ wheel_R ) /pi *30.0*28.5;// r/min
	basketballbot.motor_speed.M3=(motor[2]/ wheel_R )  /pi *30.0*28.5;// r/min
	basketballbot.motor_speed.M4=(motor[3]/ wheel_R )  /pi *30.0*28.5;// r/min
}


//设置机器人坐标系速度
void Set_Speed_Robot_To_Motor()
{
	double Speed_goal[3],motor[4];
	Speed_goal[0]=basketballbot.robot_expected_speed.x;
	Speed_goal[1]=basketballbot.robot_expected_speed.y;
	Speed_goal[2]=basketballbot.robot_expected_speed.w;
	Speed_goal[2]=Speed_goal[2] / 60*2*pi;
  TF_Robot_To_Motor(Speed_goal,motor);
	
	basketballbot.motor_speed.M1=(motor[0]/ wheel_R ) /pi *30.0*28.5;// r/min
	basketballbot.motor_speed.M2=(motor[1]/ wheel_R ) /pi *30.0*28.5;// r/min
	basketballbot.motor_speed.M3=(motor[2]/ wheel_R )  /pi *30.0*28.5;// r/min
	basketballbot.motor_speed.M4=(motor[3]/ wheel_R )  /pi *30.0*28.5;// r/min	
}




