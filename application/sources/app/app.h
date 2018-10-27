/**
 ******************************************************************************
 * @author: ThanNT
 * @date:   13/08/2016
 ******************************************************************************
**/

#ifndef __APP_H__
#define __APP_H__

#ifdef __cplusplus
extern "C"
{
#endif
#include "ak.h"

#include "nrf_nwk_sig.h"

#include "app_if.h"
#include "app_eeprom.h"
#include "app_data.h"

/*****************************************************************************/
/* FIRMWARE task define
 */
/*****************************************************************************/
/* define timer */
#define FW_PACKED_TIMEOUT_INTERVAL			(5000)
#define FW_CHECKING_INTERVAL				(500)

/* define signal */
enum {
	FW_CRENT_APP_FW_INFO_REQ = AK_USER_DEFINE_SIG,
	FW_CRENT_BOOT_FW_INFO_REQ,
	FW_UPDATE_REQ,
	FW_UPDATE_SM_OK,
	FW_TRANSFER_REQ,
	FW_INTERNAL_UPDATE_APP_RES_OK,
	FW_INTERNAL_UPDATE_BOOT_RES_OK,
	FW_SAFE_MODE_RES_OK,
	FW_UPDATE_SM_BUSY,
	FW_PACKED_TIMEOUT,
	FW_CHECKING_REQ
};

/*****************************************************************************/
/*  LIFE task define
 */
/*****************************************************************************/
/* define timer */
#define AC_LIFE_TASK_TIMER_LED_LIFE_INTERVAL		(1000)

/* define signal */
enum {
	AC_LIFE_SYSTEM_CHECK = AK_USER_DEFINE_SIG,
};

/*****************************************************************************/
/*  SHELL task define
 */
/*****************************************************************************/
/* define timer */

/* define signal */
enum {
	AC_SHELL_LOGIN_CMD = AK_USER_DEFINE_SIG,
	AC_SHELL_REMOTE_CMD,
};

/*****************************************************************************/
/*  RF24 task define
 */
/*****************************************************************************/
/* private define */
/* define timer */
/* define signal */
enum {
	AC_RF24_IF_INIT_NETWORK = AK_USER_DEFINE_SIG,
	AC_RF24_IF_PURE_MSG_OUT,
	AC_RF24_IF_COMMON_MSG_OUT,
	AC_RF24_IF_PURE_MSG_IN,
	AC_RF24_IF_COMMON_MSG_IN,
};

/*****************************************************************************/
/* IF task define
 */
/*****************************************************************************/
/* define timer */
#define AC_IF_TIMER_PACKET_TIMEOUT_INTERVAL			(500)

/* define signal */
enum {
	AC_IF_PURE_MSG_IN = AK_USER_DEFINE_SIG,
	AC_IF_PURE_MSG_OUT,
	AC_IF_COMMON_MSG_IN,
	AC_IF_COMMON_MSG_OUT,
	AC_IF_DYNAMIC_MSG_IN,
	AC_IF_DYNAMIC_MSG_OUT,
};

/*****************************************************************************/
/* UART_IF task define
 */
/*****************************************************************************/
/* timer signal */
#define AC_UART_IF_FRAME_TO_INTERVAL		20

/* define signal */
enum {
	AC_UART_IF_PURE_MSG_OUT = AK_USER_DEFINE_SIG,
	AC_UART_IF_COMMON_MSG_OUT,
	AC_UART_IF_DYNAMIC_MSG_OUT,
	AC_UART_IF_FRAME_TO,
};

/*****************************************************************************/
/*  LIFE task define
 */
/*****************************************************************************/
/* define timer */
#define AC_DISPLAY_LOGO_INTERVAL			(10000)

/* define signal */
enum {
	AC_DISPLAY_INITIAL = AK_USER_DEFINE_SIG,
	AC_DISPLAY_SHOW_ON_LOGO,
	AC_DISPLAY_SHOW_ON_RTOS,
    AC_DISPLAY_SHOW_DO_AN,
	AC_DISPLAY_SHOW_VALUE,
	AC_DISPLAY_SHOW_START,
	AC_DISPLAY_SHOW_STOP,
	AC_DISPLAY_SHOW_RUN,
	AC_DISPLAY_SHOW_SET_KP,
	AC_DISPLAY_SHOW_SET_KD,
	AC_DISPLAY_SHOW_SET_XKP,
	AC_DISPLAY_SHOW_SET_XKD,
	AC_DISPLAY_SHOW_FLASH_WRITE,
	AC_DISPLAY_SHOW_FLASH_READ,
	AC_DISPLAY_SHOW_OFF,
};

/*****************************************************************************/
/* DBG task define
 */
/*****************************************************************************/
/* define timer */
/* define signal */
enum {
	AC_DBG_TEST_1 = AK_USER_DEFINE_SIG,
	AC_DBG_TEST_2,
	AC_DBG_TEST_3
};

/*****************************************************************************/
/*  global define variable
 */
/*****************************************************************************/
#define APP_OK									(0x00)
#define APP_NG									(0x01)

#define APP_FLAG_OFF							(0x00)
#define APP_FLAG_ON								(0x01)

/*****************************************************************************/
/*  app function declare
 */
/*****************************************************************************/

enum {
	AC_INVERTERPENDULUM ,
	AC_INVERTERPENDULUM_START,
	AC_INVERTERPENDULUM_STOP,
	AC_INVERTERPENDULUM_SET_UP,
	AC_INVERTERPENDULUM_SET_DOWN,
	AC_INVERTERPENDULUM_FLASH_WRITE,
	AC_INVERTERPENDULUM_FLASH_READ,



};




#define APP_VER		"0.0.1"

extern const char* app_version;

extern void* app_get_boot_share_data();
extern int  main_app();

#ifdef __cplusplus
}
#endif

#endif //__APP_H__
