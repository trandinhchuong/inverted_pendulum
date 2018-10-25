#include "fsm.h"
#include "port.h"
#include "message.h"

#include "io_cfg.h" // TIM_GetCouonter(TIM3);
#include "view_render.h"

#include "sys_ctrl.h"
#include "sys_dbg.h"

#include "app.h"
#include "app_dbg.h"

#include "task_list.h"
#include "task_life.h"
#include "task_encoder.h"
//#include "PID.h"
#include <PID_v1.h>
#include "Arduino.h"

void display_encoder();
void PWM(double val);
__IO int16_t Encoder = 0,Encoder800=0;
__IO int16_t Encoder_val = 0,Encoder800_val=0;
float Pos = 0;
float Speed=0;

double Setpoint, Input, Output;


int run=0;

//Specify the links and initial tuning parameters
double Kp=	10, Ki=0, Kd=0;
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);


void encoder_read ()
{
	Encoder=TIM_GetCounter(TIM3) -Encoder_val;
	Encoder_val=TIM_GetCounter(TIM3);
	Pos=Pos+ (Encoder*0.45);
	//vong=Pos/360
	Speed=(Encoder/10);
	//display_encoder();
	Setpoint=180;
	Input=(double)Speed;
	myPID.SetSampleTime(1);
	myPID.SetOutputLimits(-255, 255);

	//myPID.compute(200, Speed, &Output);


	TIM11_PWM((__IO int16_t)Output);

	display_encoder();
	Encoder=0;
	LOGIN_PRINT("Output : %d\n",(__IO int16_t)Output);
	/*
 *
 * Tốc độ được tính như sau:
 *
 * vong     xung.1000000us
 * -------=----------------------
 * phut     10us.60.800
 * = xung*0.45
 *
 */
}

void canbang()
{
	Encoder=TIM_GetCounter(TIM3) -Encoder_val;
	Encoder_val=TIM_GetCounter(TIM3);
	Speed=(Encoder/10);
	Kp= (TIM_GetCounter(TIM3));
	Encoder800=TIM_GetCounter(TIM2) -Encoder800_val;
	Encoder800_val=TIM_GetCounter(TIM2);
	Pos=Pos+ (Encoder800*0.225);

	//LOGIN_PRINT("milis %d \n",millis());


	if(run==0){
		//LOGIN_PRINT("run==0");

/*
		if(Pos >= Setpoint)run=1;
		Setpoint =180 ;

		for(uint32_t i = 0; i < EEPROM_END_ADDR; i++) {
			eeprom_read(i, &Kp, sizeof(uint8_t));
		}
		if(button_t btn_up()==1)
			for(uint32_t i = 0; i < EEPROM_END_ADDR; i++) {
			eeprom_write(i, &Kp, sizeof(uint8_t));
		}
		*/

		//if(button_pressed) encoder_angle = 180;
		//if(abs(encoder_angle)<5
		//	display("READY")
		//else
		//	display("NOT READY");
		myPID.SetSampleTime(1);
		myPID.SetMode(AUTOMATIC);
		myPID.SetOutputLimits(-255,255);
		Output=0;
	}
	else
		if(run==1)
		{
			Input=Pos;
			LOGIN_PRINT("pos: %d \n",(uint8_t)Pos);
			Setpoint =180 ;
			myPID.laymau();
			PWM(Output);
			//	LOGIN_PRINT("Output %d \n",Output);
		}

	display_encoder();



}

void PWM(double val)
{

	if(val < 0){
		val=abs(val);
		//	 LOGIN_PRINT("     abs : %d\n",val);
		TIM11_PWM((__IO int16_t)val);
		GPIO_SetBits(GPIOB, GPIO_Pin_8);
	}

	else {

		TIM11_PWM((__IO int16_t)val);

		GPIO_ResetBits(GPIOB, GPIO_Pin_8);
	}


}


void display()
{
	APP_DBG_SIG("AC_DISPLAY_SHOW_ON_LOGO\n");
	view_render.initialize();
	view_render.display_on();
	view_render.clear();
	/* ak logo */
	view_render.setTextSize(1);
	view_render.setTextColor(WHITE);
	view_render.setCursor(1, 1);
	view_render.print("_START_");

	view_render.setTextSize(1);
	view_render.setTextColor(WHITE);
	view_render.setCursor(1, 30);
	view_render.print("ENCODER:");

	view_render.setTextSize(1);
	view_render.setTextColor(WHITE);
	view_render.setCursor(1, 50);
	view_render.print("POS:   ");

	view_render.update();
}

void display_encoder()
{

	view_render.setTextSize(1);
	view_render.setTextColor(WHITE,BLACK);
	view_render.setCursor(50, 10);
	view_render.print(Kp);

	view_render.setTextSize(1);
	view_render.setTextColor(WHITE,BLACK);
	view_render.setCursor(50, 30); //encoder
	view_render.print(Output);
	view_render.setCursor(90, 30); //encoder
	view_render.print(Output);


	view_render.setTextSize(1);
	view_render.setTextColor(WHITE,BLACK);
	view_render.setCursor(50, 50); //pos
	view_render.print(Pos);
	view_render.update();


}
void task_encoder(ak_msg_t* msg) {
	switch (msg->sig) {
	case AC_LIFE_SYSTEM_CHECK:
		/* reset watchdog */
		sys_ctrl_independent_watchdog_reset();
		sys_ctrl_soft_watchdog_reset();

#if defined(AK_IO_IRQ_ANALYZER)
#else
	{      /* toggle led indicator */
		//led_toggle(&led_life);
		canbang();

	}
#endif
		break;

	default:
		break;
	}
}
