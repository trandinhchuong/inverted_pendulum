#include "PID.h"
/*
PID::PID(float p, float i, float d)
{
    kp = p;
    ki = i;
    kd = d;
}

void PID::compute(float Setpoint, float Input, float* Output)
{
    static float errSum = 0.0f;
    static float lastErr = 0.0f;

    float error = Setpoint - Input;
    errSum += error;
    float dErr = (error - lastErr);

	if (Kp<0 || Ki<0 || Kd<0) return;

	pOn = POn;
	pOnE = POn == P_ON_E;

	dispKp = Kp; dispKi = Ki; dispKd = Kd;

	double SampleTimeInSec = ((double)SampleTime)/1000;
	kp = Kp;
	ki = Ki * SampleTimeInSec;
	kd = Kd / SampleTimeInSec;

   if(controllerDirection ==REVERSE)
	{
	   kp = (0 - kp);
	   ki = (0 - ki);
	   kd = (0 - kd);
	}
 }

    lastErr = error;
}
	  *Output = kp * error + ki * errSum + kd * dErr;

*/
