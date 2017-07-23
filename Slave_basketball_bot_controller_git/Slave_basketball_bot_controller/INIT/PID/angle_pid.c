#include "data.h"

extern robot basketballbot;
extern Angle_PID ANGLE;

void PID_Init()
{
     ANGLE.ACTUAL_Angle=0;
	   ANGLE.ERROR_Angle=0;
	   ANGLE.ERROR_LAST_Angle=0;
	   ANGLE.INTEGRAL=0;
	   ANGLE.Kd=0;
	   ANGLE.Ki=0;
	   ANGLE.Kp=0;
	   ANGLE.SET_Angle_now=0;
	   ANGLE.SET_Angle_last=0;
	   ANGLE.SPEED_W_PID=0;
}