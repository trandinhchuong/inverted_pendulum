#include "button.h"

#include "sys_dbg.h"

#include "app.h"
#include "app_bsp.h"
#include "app_dbg.h"
#include "app_if.h"

#include "task_list.h"
#include "task_list_if.h"

button_t btn_mode;
button_t btn_up;
button_t btn_down;

void btn_mode_callback(void* b) {
	button_t* me_b = (button_t*)b;
	switch (me_b->state) {
	case BUTTON_SW_STATE_PRESSED: {
		APP_DBG("[btn_mode_callback] BUTTON_SW_STATE_PRESSED\n");  //nhan
		task_post_pure_msg(AC_TASK_INVERTERPENDULUM,AC_INVERTERPENDULUM_STOP);
		task_post_pure_msg(AC_TASK_DISPLAY_ID,AC_DISPLAY_SHOW_STOP);
	}
		break;

	case BUTTON_SW_STATE_LONG_PRESSED: {
		APP_DBG("[btn_mode_callback] BUTTON_SW_STATE_LONG_PRESSED\n");//NHAN GIU
		task_post_pure_msg(AC_TASK_INVERTERPENDULUM,AC_INVERTERPENDULUM_START);
		task_post_pure_msg(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_START);

	}
		break;

	case BUTTON_SW_STATE_RELEASED: {
		APP_DBG("[btn_mode_callback] BUTTON_SW_STATE_RELEASED\n");   //nha
		task_post_pure_msg(AC_TASK_DISPLAY_ID,AC_DISPLAY_SHOW_VALUE);

	}
		break;

	default:
		break;
	}
}

void btn_up_callback(void* b) {
	button_t* me_b = (button_t*)b;
	switch (me_b->state) {
	case BUTTON_SW_STATE_PRESSED: {
		APP_DBG("[btn_up_callback] BUTTON_SW_STATE_PRESSED\n");
		task_post_pure_msg(AC_TASK_INVERTERPENDULUM,AC_INVERTERPENDULUM_SET_DOWN);
		//task_post_pure_msg(AC_TASK_DISPLAY_ID,AC_DISPLAY_SHOW_VALUE);

	}
		break;

	case BUTTON_SW_STATE_LONG_PRESSED: {
		APP_DBG("[btn_up_callback] BUTTON_SW_STATE_LONG_PRESSED\n");
		task_post_pure_msg(AC_TASK_INVERTERPENDULUM,AC_INVERTERPENDULUM_FLASH_READ);

	}
		break;

	case BUTTON_SW_STATE_RELEASED: {
		APP_DBG("[btn_up_callback] BUTTON_SW_STATE_RELEASED\n");

	}
		break;

	default:
		break;
	}
}

void btn_down_callback(void* b) {
	button_t* me_b = (button_t*)b;
	switch (me_b->state) {
	case BUTTON_SW_STATE_PRESSED: {
		APP_DBG("[btn_down_callback] BUTTON_SW_STATE_PRESSED\n");
		task_post_pure_msg(AC_TASK_INVERTERPENDULUM,AC_INVERTERPENDULUM_SET_UP);

	}
		break;

	case BUTTON_SW_STATE_LONG_PRESSED: {
		APP_DBG("[btn_down_callback] BUTTON_SW_STATE_LONG_PRESSED\n");
		task_post_pure_msg(AC_TASK_INVERTERPENDULUM,AC_INVERTERPENDULUM_FLASH_WRITE);

	}
		break;

	case BUTTON_SW_STATE_RELEASED: {
		APP_DBG("[btn_down_callback] BUTTON_SW_STATE_RELEASED\n");
	}
		break;

	default:
		break;
	}
}
