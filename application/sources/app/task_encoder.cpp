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
#include "eeprom.h"

#include <PID_v1.h>

__IO int16_t Encoder = 0,Encoder800=0;
__IO int16_t Encoder_val = 0,Encoder800_val=0;


float s=0.0, s1 = 0.0,Pos = 0.0, theta = 0.0, theta1 = 0.0;

float Speed=0.0;
char st[6];
__IO int16_t error =0;
__IO int16_t Output=0,xOutput=0,Setpoint=0,xSetpoint=0;

/**************PID_pos value*********************/
__IO int32_t Old_Pulse=0;                       // = pre_Pulse
__IO int32_t Err=0;
__IO int32_t Old_Err=0;                         // = pre_Err
float Kp=1500, Kd=5, Ki=0;						// kp30 kd 20
__IO int32_t pOut=0;
__IO int32_t dOut=0;
__IO int32_t iOut=0;
__IO int32_t Out_Put=0;

__IO int32_t Inv_Sampling_Time=10;
__IO int32_t Sampling_Time=10;
/******************END*************************/
/**************PID_x value*********************/
__IO int32_t xOld_Pulse=0;                       // = pre_Pulse
__IO int32_t xErr=0;
__IO int32_t xOld_Err=0;                         // = pre_Err
float xKp=0.1, xKd=0, xKi=0;
__IO int32_t xpOut=0;
__IO int32_t xdOut=0;
__IO int32_t xiOut=0;
__IO int32_t xOut_Put=0;

/****************END PID_x value***************/


/*****************Set_value**************/

int Set=0;
int run=0;
/*****************timer10us*****************/
uint16_t micro_us=0;
uint16_t microus()
{
	micro_us= micro_us+1;
	return  micro_us;
}

//*****************sample_value**************
int sample_timer=0;
int sample=0;


void PWM(__IO int16_t val);

void flashRead(float* value ,uint32_t sector ){

	//flash_write(KP_FLASH_ADD, (float*)value , sizeof(float));

	//flash_read(sector , (uint8_t*)value, sizeof(float));
	io_eeprom_read(sector,(uint8_t*)value,sizeof(float));

}

void flashWrite(float* value ,uint32_t sector ){


	//io_eeprom_erase(START_ADD,1000	);
	//flash_write(sector, (uint8_t*)&value , sizeof(float));
	io_eeprom_write(sector,(uint8_t*)value,sizeof(float));
}


void canbang()
{
	float k1, k2, k3, k4;
	Encoder=TIM_GetCounter(TIM3) -Encoder_val;
	Encoder_val=TIM_GetCounter(TIM3);
	s=s+ (Encoder*0.000125);

	Encoder800=TIM_GetCounter(TIM2) -Encoder800_val;
	Encoder800_val=TIM_GetCounter(TIM2);
	Pos=Pos+ (Encoder800*0.225) ;
	theta = Pos + 180;
	//Setpoint=0;
	if(run==0){
//		LOGIN_PRINT("STOP:\t");
		Setpoint=0;
		Output=0;
		PWM((__IO int16_t )Output);
		//task_post_pure_msg(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_RUN);

		//task_post(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_RUN);
	}
	else
	{
//		LOGIN_PRINT("RUN :\n");

		/*************x_comper*************************/
		//xErr=s-xSetpoint;
		//xpOut=xKp*xErr;

	//	xdOut=xKd*(xErr-xOld_Err)*Inv_Sampling_Time;

		//xOld_Err=xErr;
		//Setpoint=xpOut + xdOut;

		//if(Setpoint>1.5)Setpoint=1.5;
		//if(Setpoint<=(-1.5))Setpoint=-1.5;


		/*************pos_comper*************************/
		//Err=Pos-Setpoint;
		//pOut=Kp*Err;
		//		dOut=Kd*(Err-Old_Err)*Inv_Sampling_Time;

		//iOut+=(Ki*Sampling_Time*Err)/10000;

		//Old_Err=Err;


		//Output = pOut + dOut;
		k1 = 100;
		k2 = 0.5;
		k3 = 500;
		k4 = 5;

		Output = (int16_t) (-(k1*s + k2*100*(s-s1) + k3*theta +k4*100*(theta-theta1)));

		if(Output>1023)Output=1023;
		if(Output<=(-1023))Output=-1023;

//		if(abs(s)<40)
//		PWM((__IO int16_t )Output);
//		else
//		PWM((__IO int16_t )0);

		PWM((__IO int16_t )Output);
		s1 = s;
		theta1 = theta;

		/*************END_pos_comper*************************/



		/*************PWM*************************/


	}



	LOGIN_PRINT("\n");
	LOGIN_PRINT("\nX: %d\t",(int)(s*100));
	LOGIN_PRINT("Pos: %d\t",(int)(theta*100));
//	LOGIN_PRINT("POut:%d\t",(int)pOut);
//	LOGIN_PRINT("dOut:%d\t",(int)dOut);
//	LOGIN_PRINT("iOut:%d\t",(int)iOut);
	LOGIN_PRINT("output: %d\t",(int)Output);
//		LOGIN_PRINT("microus():%d\t",microus());


	micro_us=0;
	task_post_pure_msg(AC_TASK_INVERTERPENDULUM,AC_INVERTERPENDULUM);
}

void PWM(__IO int16_t val)
{
	if(val < 0){
		val=abs(val);

		TIM11_PWM((__IO int16_t)val);
		GPIO_ResetBits(GPIOB, GPIO_Pin_8);
	}

	else {
		TIM11_PWM((uint8_t)val);
		GPIO_SetBits(GPIOB, GPIO_Pin_8);

	}
}


void display()
{
	view_render.setTextSize(2);
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
		view_render.print("Kp:");
		view_render.setTextSize(2);
		view_render.print((int)Kp);

		view_render.setTextSize(2);
		view_render.setTextColor(WHITE,BLACK);
		view_render.setCursor(0, 30); //encoder
		view_render.print("Kd:");
		view_render.setTextSize(2);
		view_render.print((int)Kd);

		view_render.setTextSize(2);
		view_render.setTextColor(WHITE,BLACK);
		view_render.setCursor(0, 50); //pos
		view_render.print(Pos);\

		view_render.setTextSize(2);
		view_render.setTextColor(WHITE,BLACK);
		view_render.setCursor(65, 10); //encoder
		view_render.print("xKp:");
		view_render.setTextSize(2);
		view_render.print((int)xKp);

		view_render.setTextSize(2);
		view_render.setTextColor(WHITE,BLACK);
		view_render.setCursor(65, 30); //encoder
		view_render.print("xKd:");
		view_render.setTextSize(2);
		view_render.print((int)xKd);

		view_render.setTextSize(2);
		view_render.setTextColor(WHITE,BLACK);
		view_render.setCursor(65, 50); //pos
		view_render.print(s);
	}
		break;
	case SET_KP:{
		view_render.setTextSize(3);
		view_render.setTextColor(WHITE,BLACK);
		view_render.setCursor(0, 10);
		view_render.print("Kp:");
		view_render.print(Kp);
	}
		break;
	case SET_KD:{
		view_render.setTextSize(3);
		view_render.setTextColor(WHITE,BLACK);
		view_render.setCursor(0, 10);
		view_render.print("Kd:");
		view_render.print(Kd);
	}
		break;
	case SET_xKP:{
		view_render.setTextSize(3);
		view_render.setTextColor(WHITE,BLACK);
		view_render.setCursor(0, 10);
		view_render.print("xKp:");
		view_render.print(xKp);
	}
		break;
	case SET_xKD:{
		view_render.setTextSize(3);
		view_render.setTextColor(WHITE,BLACK);
		view_render.setCursor(0, 10);
		view_render.print("xKd:");
		view_render.print(xKd);
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
		if(Set==SET_KP)Kp=Kp-0.5;
		if(Set==SET_KD)Kd=Kd-0.5;
		if(Set==SET_xKP)xKp=xKp-0.5;
		if(Set==SET_xKD)xKd=xKd-0.5;
		LOGIN_PRINT("X: %d %d %d\t",(int)Kp,(int)Kd,(int)Ki);
		Set=Set-1;
		display_encoder();
	}
		break;
	case AC_INVERTERPENDULUM_SET_DOWN:{

		if(Set==SET_KP)Kp=Kp+0.5;
		if(Set==SET_KD)Kd=Kd+0.5;
		if(Set==SET_xKP)xKp=xKp+0.5;
		if(Set==SET_xKD)xKd=xKd+0.5;
		LOGIN_PRINT("X: %d %d %d\t",(int)Kp,(int)xKp,xKi);
		Set=Set-1;
		display_encoder();
		s=0;

	}
		break;
	case AC_INVERTERPENDULUM_FLASH_READ:{

		//LOGIN_PRINT("Write_Flash\t");

		task_post_pure_msg(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_FLASH_READ);
		eeprom_read(KP_FLASH_ADD,(uint8_t*) &Kp,sizeof(float));
		//io_eeprom_read(KP_FLASH_ADD,(uint8_t*) &Kp,2);
		//flashRead(&Kp,KP_FLASH_ADD);
		//		flashRead(&Kd,KP_FLASH_ADD);
		//		flashRead(&xKp,XKP_FLASH_ADD);
		//		flashRead(&xKp,XKP_FLASH_ADD);

	}
		break;
	case AC_INVERTERPENDULUM_FLASH_WRITE:{
		//LOGIN_PRINT("Write_Flash\t");
		task_post_pure_msg(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_FLASH_WRITE);
		eeprom_erase(KP_FLASH_ADD,sizeof(uint32_t));
		eeprom_write(KP_FLASH_ADD,(uint8_t*)&Kp,sizeof(float));
		//		flashWrite(&Kd,KD_FLASH_ADD);
		//		flashWrite(&xKp,XKP_FLASH_ADD);
		//		flashWrite(&xKp,XKP_FLASH_ADD);

	}
		break;


	default:
		break;
	}
}
