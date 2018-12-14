#include "fsm.h"
#include "port.h"
#include "message.h"

#include "io_cfg.h" // TIM_GetCouonter(TIM3);
#include "view_render.h"

#include "sys_ctrl.h"
#include "sys_dbg.h"
#include "flash.h"
#include "app.h"
#include "app_dbg.h"

#include "task_list.h"
#include "task_life.h"
#include "task_encoder.h"
//#include "PID.h"
#include "eeprom.h"



__IO int16_t Encoder = 0,Encoder_val = 0;
__IO int16_t Encoder800=0,Encoder800_val=0;

float s=0.0, s1 = 0.0,Pos = 0.0, theta = 0.0, theta1 = 0.0;\
float k1, k2, k3, k4;

float setpoint=20;
char st[6];
__IO int16_t error =0;
__IO int16_t Output=0;
__IO int16_t  upp=9000;
__IO int16_t  downn=-9000;
/*****************Set_value**************/
int Set=0;
int run=0;
/*****************timer10us*****************/
uint16_t micro_us=0;
uint16_t timePrint=100;
uint16_t timeCount=0;
uint16_t microus()
{
	micro_us= micro_us+1;
	timeCount=timeCount+1;
	return  micro_us;

}

void PWM(__IO int16_t val);

void canbang()
{
	Encoder=TIM_GetCounter(TIM3);//200prr
	s=(Encoder*0.00479);//0.047909
	s=s;
	Encoder800=TIM_GetCounter(TIM2);
	Pos=(Encoder800*0.157);//0.00157

	theta =Pos;
	if(run==0)
	{
		Output=0;
		PWM((__IO int16_t )Output);
		//21.5023 13.5548 . 59.5012 9.7365
		k1 =33.5242;//5.5023;//33.5242;//82.3814;//37.0716//
		k2 =17.5696;//1.5548; //16.5696;//16.5696;//54.4133;//-24.4860//
		k3 =126.6244;//126.6244;//-56.9810//800
		k4 =30.9240;//28.9240;//-13.0158//50
		timeCount=0;
	}//if
	else
	{
//		Output = pOut + dOut;  100 0.5 500 5
//		Output = (int16_t) (-(k1*s + k2*100*(s-s1) + k3*theta +k4*100*(theta-theta1)));


		Output = (int16_t) (- ( -(k1*s + k2*100*(s-s1) ) + k3*theta +k4*100*(theta-theta1)) );

		if(Output>1023)Output=1023;
		if(Output<=(-1023))Output=-1023;

		PWM((__IO int16_t )Output);

		s1 = s;
		theta1 = theta;
		if((theta-theta1)>(setpoint*5))
			run=0;
		else if(((theta-theta1)*-1)> (setpoint*5))
			run=0;
	}//else
if(micro_us >=(uint32_t)1)
{
if(timeCount<40000)
{

	LOGIN_PRINT("\n");
	//LOGIN_PRINT("s: %d\t",(int)(s*100));

	LOGIN_PRINT("theta: %d\t",(int)(theta*100));
	LOGIN_PRINT("output: %d\t",(int)(Output));
	LOGIN_PRINT("dOut: %d\t",(int)upp);
	LOGIN_PRINT("iOut: %d\t",(int)downn);

//	LOGIN_PRINT("encoder800: %d\t",(uint16_t)Encoder800);
//	LOGIN_PRINT("POut:%d\t",(int)pOut);
	//LOGIN_PRINT("microus:%d\t",(uint16_t)micro_us);

	micro_us=0;
}
}//if
	task_post_pure_msg(AC_TASK_INVERTERPENDULUM,AC_INVERTERPENDULUM);
}

void PWM( int16_t val)
{
	if(val < 0){
		val=val*(-1);//105
		val=120+val;
		if(val>1023)val=1023;
		TIM11_PWM((__IO int16_t)val);
		GPIO_ResetBits(GPIOB, GPIO_Pin_8);
	}
	else if(val > 1) {
		val=120 + val;//80
		if(val>1023)val=1023;
		TIM11_PWM((__IO int16_t)val);
		GPIO_SetBits(GPIOB, GPIO_Pin_8);
	}
	else
	{
		val=0;
		TIM11_PWM((uint8_t)val);
	}

}


void display()
{	view_render.clear();
	view_render.setTextSize(2);
	view_render.setTextColor(WHITE,BLACK);
	view_render.setCursor(0, 10);
	view_render.print("K1:");
	view_render.setTextSize(1);
	view_render.print((int)k1);

	view_render.setTextSize(2);
	view_render.setTextColor(WHITE,BLACK);
	view_render.setCursor(0, 30); //encoder
	view_render.print("K2:");
	view_render.setTextSize(1);
	view_render.print((int)k2);

	view_render.setTextSize(2);
	view_render.setTextColor(WHITE,BLACK);
	view_render.setCursor(0, 50); //pos
	view_render.print(Pos);\

	view_render.setTextSize(2);
	view_render.setTextColor(WHITE,BLACK);
	view_render.setCursor(65, 10); //encoder
	view_render.print("K3:");
	view_render.setTextSize(1);
	view_render.print((int)k3);

	view_render.setTextSize(2);
	view_render.setTextColor(WHITE,BLACK);
	view_render.setCursor(65, 30); //encoder
	view_render.print("K4:");
	view_render.setTextSize(1);
	view_render.print((int)k4);

	view_render.setTextSize(1);
	view_render.setTextColor(WHITE,BLACK);
	view_render.setCursor(65, 50); //pos
	view_render.print(s);

	view_render.update();
}
void display_encoder()
{

	Set=Set+1;
	if(Set>5)Set=1;
	view_render.display_on();
	view_render.clear();
	switch (Set) {
	case SHOW:{

		view_render.setTextSize(2);
		view_render.setTextColor(WHITE,BLACK);
		view_render.setCursor(0, 10);
		view_render.print("K1:");
		view_render.setTextSize(1);
		view_render.print((int)k1);

		view_render.setTextSize(2);
		view_render.setTextColor(WHITE,BLACK);
		view_render.setCursor(0, 30); //encoder
		view_render.print("K2:");
		view_render.setTextSize(1);
		view_render.print((int)k2);

		view_render.setTextSize(2);
		view_render.setTextColor(WHITE,BLACK);
		view_render.setCursor(0, 50); //pos
		view_render.print(Pos);\

		view_render.setTextSize(2);
		view_render.setTextColor(WHITE,BLACK);
		view_render.setCursor(65, 10); //encoder
		view_render.print("K3:");
		view_render.setTextSize(1);
		view_render.print((int)k3);

		view_render.setTextSize(2);
		view_render.setTextColor(WHITE,BLACK);
		view_render.setCursor(65, 30); //encoder
		view_render.print("K4:");
		view_render.setTextSize(1);
		view_render.print((int)k4);

		view_render.setTextSize(1);
		view_render.setTextColor(WHITE,BLACK);
		view_render.setCursor(65, 50); //pos
		view_render.print(s);
	}
		break;
	case SET_KP:{
		view_render.setTextSize(2);
		view_render.setTextColor(WHITE,BLACK);
		view_render.setCursor(0, 10);
		view_render.print("K1:");
		view_render.print(k1);
	}
		break;
	case SET_KD:{
		view_render.setTextSize(3);
		view_render.setTextColor(WHITE,BLACK);
		view_render.setCursor(0, 10);
		view_render.print("K2:");
		view_render.print(k2);
	}
		break;
	case SET_xKP:{
		view_render.setTextSize(3);
		view_render.setTextColor(WHITE,BLACK);
		view_render.setCursor(0, 10);
		view_render.print("K3:");
		view_render.print(k3);
	}
		break;
	case SET_xKD:{
		view_render.setTextSize(3);
		view_render.setTextColor(WHITE,BLACK);
		view_render.setCursor(0, 10);
		view_render.print("K4:");
		view_render.print(k4);
	}
		break;
	default:
		break;
	}
	view_render.update();
}
void task_inverterPendulum(ak_msg_t* msg) {
	switch (msg->sig) {
	case AC_INVERTERPENDULUM:{//inverterPendulum
		canbang();
	}
		break;
	case AC_INVERTERPENDULUM_START:{
		run=start;

	}
		break;
	case AC_INVERTERPENDULUM_STOP:{
		run=stop;

	}
		break;
	case AC_INVERTERPENDULUM_SET_UP:{
		if(Set==SET_KP)k1=k1-10;
		if(Set==SET_KD)k2=k2-10;
		if(Set==SET_xKP)k3=k3-10;
		if(Set==SET_xKD)k4=k4-10;
		LOGIN_PRINT("X: %d %d %d\t",(int)k1,(int)k2,k3);
		Set=Set-1;
		display_encoder();
	}
		break;
	case AC_INVERTERPENDULUM_SET_DOWN:{

		if(Set==SET_KP)k1=k1+10;
		if(Set==SET_KD)k2=k2+10;
		if(Set==SET_xKP)k3=k3+10;
		if(Set==SET_xKD)k4=k4+10;
		LOGIN_PRINT("X: %d %d %d\t",(int)k1,(int)k2,k3);
		Set=Set-1;
		display_encoder();
		s=0;

	}
		break;
	case AC_INVERTERPENDULUM_FLASH_READ:{

		//		sector writre_RSQ;
		//		memset((uint8_t *)&writre_RSQ, 0, sizeof(sector));
		//		flash_read(KP_FLASH_ADD, (uint8_t*)&writre_RSQ, sizeof(sector));
		//		k1= writre_RSQ.value;

		task_post_pure_msg(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_FLASH_READ);
	}
		break;
	case AC_INVERTERPENDULUM_FLASH_WRITE:{
		//		sector writre_RSQ;
		//		memset((uint8_t *)&writre_RSQ, 0, sizeof(sector));
		//		writre_RSQ.value=k1;
		//		flash_write(KP_FLASH_ADD, (uint8_t*)&writre_RSQ, sizeof(sector));

		task_post_pure_msg(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_FLASH_WRITE);

	}
		break;


	default:
		break;
	}
}
