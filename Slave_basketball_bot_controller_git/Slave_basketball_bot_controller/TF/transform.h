#ifndef __TRANSFORM_H
#define __TRANSFORM_H
#include "data.h"




void TF_Robot_To_Motor(double Speed_goal[3],double *motor);
void TF_Global_To_Motor(double Speed_goal[3],double *motor,double Angle_included);
void TF_Robot_To_Global_Speed(void);
void TF_Global_To_Robot_Speed(void);
void Set_Speed_Global_To_Motor(void);
void Set_Speed_Robot_To_Motor(void);
void Angle_control(void);
#endif
