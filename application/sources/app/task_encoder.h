#ifndef __TASK_ENCODER__
#define __TASK_ENCODER__

#include "led.h"


void display_encoder();
void encoder_read ();
void PWM(double val);
void canbang();
void display();
void display_encoder();

void task_encoder(ak_msg_t* msg);

#endif //__TASK_ENCODER__
