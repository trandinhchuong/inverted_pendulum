#include "fsm.h"
#include "port.h"
#include "message.h"
#include "timer.h"

#include "sys_ctrl.h"
#include "sys_dbg.h"

#include "view_render.h"

#include "app.h"
#include "app_dbg.h"

#include "task_list.h"
#include "task_display.h"

void task_display(ak_msg_t* msg) {
    switch (msg->sig) {
    case AC_DISPLAY_INITIAL: {
        APP_DBG_SIG("AC_DISPLAY_INITIAL\n");
        view_render.initialize();
        task_post_pure_msg(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_ON_LOGO);
    }
        break;

    case AC_DISPLAY_SHOW_ON_LOGO: {
        APP_DBG_SIG("AC_DISPLAY_SHOW_ON_LOGO\n");
        view_render.display_on();
        view_render.clear();

        /* ak logo */
        view_render.setTextSize(1);
        view_render.setTextColor(WHITE);
        view_render.setCursor(24, 3);
        view_render.print("   __    _  _ ");
        view_render.setCursor(24, 10);
        view_render.print("  /__\\  ( )/ )");
        view_render.setCursor(24, 20);
        view_render.print(" /(__)\\ (   (");
        view_render.setCursor(24, 30);
        view_render.print("(__)(__)(_)\\_)");
        view_render.setCursor(24, 45);
        view_render.print("Active Kernel");
        view_render.update();

        timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_OFF, AC_DISPLAY_LOGO_INTERVAL, TIMER_ONE_SHOT);
    } break;

    case AC_DISPLAY_SHOW_DO_AN: {
        APP_DBG_SIG("AC_DISPLAY_SHOW_ON_LOGO\n");
        view_render.display_on();
        view_render.clear();

        /* ak logo */
        view_render.setTextSize(3);
        view_render.setTextColor(WHITE);
        view_render.setCursor(1, 1);
        view_render.print("_START_");

        view_render.setTextSize(1);
        view_render.setTextColor(WHITE);
        view_render.setCursor(5, 45);
        view_render.print("waitting..");

        view_render.update();

        timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_OFF, AC_DISPLAY_LOGO_INTERVAL, TIMER_ONE_SHOT);
    }
        break;

    case AC_DISPLAY_SHOW_OFF: {
        APP_DBG_SIG("AC_DISPLAY_SHOW_OFF\n");
        view_render.display_off();
    }
        break;

    default:
        break;
    }
}
