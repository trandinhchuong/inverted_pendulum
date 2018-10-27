#include "PID.h"

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


    *Output = kp * error + ki * errSum + kd * dErr;


    lastErr = error;
}
