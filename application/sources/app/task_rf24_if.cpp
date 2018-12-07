#include <stdbool.h>
#include <stdint.h>

#include "app.h"
#include "app_dbg.h"
#include "app_data.h"

#include "task_rf24_if.h"
#include "task_if.h"
#include "task_list.h"

#include "fsm.h"
#include "port.h"
#include "message.h"
#include "timer.h"

#include "utils.h"
#include "fifo.h"

#include "eeprom.h"


#include "hal_nrf.h"
#include "nrf_data.h"

void task_rf24_if(ak_msg_t* msg) {
	switch (msg->sig) {
	case AC_RF24_IF_INIT_NETWORK: {
		//APP_DBG_SIG("AC_RF24_IF_INIT_NETWORK\n");

		/* init network address */
	//	nrf_set_static_nwk_addr(IF_TYPE_RF24_ME);

		/* init network state machine */
		//FSM(&nrf_nwk_fsm, nrf_nwk_fsm_init);

		/* post message init network request */
	//	task_post_pure_msg(RF24_NWK_ID, RF24_NWK_INIT);
	}
		break;
	default:
		break;
	}
}
