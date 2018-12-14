/**
 ******************************************************************************
 * @author: ThanNT
 * @date:   13/08/2016
 ******************************************************************************
**/

/* kernel include */
#include "ak.h"
#include "message.h"
#include "timer.h"
#include "fsm.h"

/* driver include */
#include "led.h"
#include "button.h"
#include "flash.h"
//#include "hs1101.h"

/* app include */
#include "app.h"
#include "app_dbg.h"
#include "app_bsp.h"
#include "app_flash.h"
#include "app_non_clear_ram.h"

#include "task_list.h"
#include "task_shell.h"
#include "task_life.h"
#include "task_if.h"
#include "task_rf24_if.h"


/* sys include */
#include "sys_boot.h"
#include "sys_irq.h"
#include "sys_io.h"
#include "sys_ctrl.h"
#include "sys_dbg.h"


/* arduino include */
#include "SPI.h"
#include "WString.h"

/* common include */
#include "utils.h"
#include "screen_manager.h"
#include "task_encoder.h"
#include "io_cfg.h"
//#include <PID_v1.h>

const char* app_version = APP_VER;

static boot_app_share_data_t boot_app_share_data;


static void app_start_timer();
static void app_init_state_machine();
static void app_task_init();

/*****************************************************************************/
/* app main function.
 */
/*****************************************************************************/
int main_app() {
	APP_PRINT("app version: %s\n", app_version);

	APP_DBG_SIG("AC_DISPLAY_SHOW_ON_LOGO\n");


//	view_render.initialize();
//	view_render.display_on();
//	view_render.clear();

//	/* ak logo */
//	view_render.setTextSize(1);
//	view_render.setTextColor(WHITE);
//	view_render.setCursor(1, 1);
//	view_render.print("_START_");

//	view_render.setTextSize(1);
//	view_render.setTextColor(WHITE);
//	view_render.setCursor(1, 10);
//	view_render.print("Kp: ");

//	view_render.setTextSize(1);
//	view_render.setTextColor(WHITE);
//	view_render.setCursor(1, 30);
//	view_render.print("ERR");

//	view_render.setTextSize(1);
//	view_render.setTextColor(WHITE);
//	view_render.setCursor(1, 50);
//	view_render.print("POS:   ");

//	view_render.update();



	/* button init */
	button_init(&btn_mode,	10,	BUTTON_MODE_ID,	io_button_mode_init,	io_button_mode_read,	btn_mode_callback);
	button_init(&btn_up,	10,	BUTTON_UP_ID,	io_button_up_init,		io_button_up_read,		btn_up_callback);
	button_init(&btn_down,	10,	BUTTON_DOWN_ID,	io_button_down_init,	io_button_down_read,	btn_down_callback);

	button_enable(&btn_mode);
	button_enable(&btn_up);
	button_enable(&btn_down);


	sys_soft_reboot_counter++;

	/******************************************************************************
	* init active kernel
	*******************************************************************************/
	ENTRY_CRITICAL();
	task_init();
    TIM3_Encoder_Config();
    TIM2_Encoder_Config();
	PWM_int();
    Dir_int() ;
	//timer9_int();
	timer_50us_init();
	timer_50us_enable();
    GPIO_ResetBits(GPIOB, GPIO_Pin_8);

    task_create(app_task_table);
	EXIT_CRITICAL();


	sys_ctrl_independent_watchdog_init();	/* 32s */
	sys_ctrl_soft_watchdog_init(200);		/* 20s */

	led_init(&led_life, led_life_init, led_life_on, led_life_off);

	/* button init */
	button_init(&btn_mode,	50,	BUTTON_MODE_ID,	io_button_mode_init,	io_button_mode_read,	btn_mode_callback);
	button_init(&btn_up,	50,	BUTTON_UP_ID,	io_button_up_init,		io_button_up_read,		btn_up_callback);
	button_init(&btn_down,	50,	BUTTON_DOWN_ID,	io_button_down_init,	io_button_down_read,	btn_down_callback);

	button_enable(&btn_mode);
	button_enable(&btn_up);
	button_enable(&btn_down);


	//task_post_pure_msg(AC_TASK_DISPLAY_ID,AC_DISPLAY_INITIAL);
	//task_post_pure_msg(AC_TASK_INVERTERPENDULUM,AC_INVERTERPENDULUM_FLASH_READ);

	task_post_pure_msg(AC_TASK_INVERTERPENDULUM,AC_INVERTERPENDULUM);

	EXIT_CRITICAL();


	app_init_state_machine();
	app_start_timer();
	sys_irq_timer_50us();
	sys_irq_pid();
	/******************************************************************************
	* app task initial
	*******************************************************************************/
	app_task_init();
	/******************************************************************************
	* run applications
	*******************************************************************************/
	return task_run();
}

void app_start_timer() {

//	timer_set(AC_TASK_INVERTERPENDULUM, AC_INVERTERPENDULUM, 10, TIMER_PERIODIC);
	timer_set(AC_TASK_LIFE_ID, AC_LIFE_SYSTEM_CHECK, 1000, TIMER_PERIODIC);

}


void app_init_state_machine() {
}


void app_task_init() {


}

/*****************************************************************************/
/* app common function
 */
/*****************************************************************************/

/* hardware timer interrupt 10ms
 * used for led, button polling
 */
void sys_irq_timer_10ms() {
    button_timer_polling(&btn_mode);
	button_timer_polling(&btn_up);
	button_timer_polling(&btn_down);
}

/* hardware timer interrupt 50ms
 * used for encode and decode ir
 */
void sys_irq_timer_50us() {

}

/* hardware rtc interrupt alarm
 * used for internal rtc alarm
 */
void sys_irq_timer_hs1101() {
}
void sys_irq_pid(){
//led_toggle(&led_life);
//LOGIN_PRINT("%d\n",microus());
	microus();
}

/* hardware io interrupt at rev ir pin
 * used for decode ir
 */
void sys_irq_ir_io_rev() {
}

void sys_irq_usb_recv(uint8_t* data, uint32_t len) {
	(void)data;
	(void)len;
}

/* init non-clear RAM objects
 */
void app_power_on_reset() {
    sys_soft_reboot_counter = 0;
}

void* app_get_boot_share_data() {
	return (void*)&boot_app_share_data;
}
