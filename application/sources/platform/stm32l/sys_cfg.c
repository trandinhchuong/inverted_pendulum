/**
 ******************************************************************************
 * @author: ThanNT
 * @date:   05/09/2016
 ******************************************************************************
**/
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>

#include "sys_cfg.h"
#include "system.h"

#include "stm32l1xx.h"
#include "stm32l1xx_conf.h"
#include "system_stm32l1xx.h"
#include "core_cm3.h"
#include "core_cmFunc.h"

#include "xprintf.h"

#include "sys_dbg.h"
#include "sys_def.h"
#include "sys_ctrl.h"
#include "sys_io.h"
#include "sys_dbg.h"
#include "ak.h"

/* Private define */
static uint32_t delay_coeficient = 0;
static void xputchar(uint8_t c);

/******************************************************************************
* system configure function
*******************************************************************************/
void sys_cfg_clock() {
	/* Enable the HSI oscillator */
	RCC_HSICmd(ENABLE);
	while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);

	SystemCoreClockUpdate();

	/* NVIC configuration */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
}

void sys_cfg_tick() {
	volatile uint32_t ticks = SystemCoreClock / 1000;
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = SysTick_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	SysTick->LOAD  = ticks - 1;                                  /* set reload register */

	NVIC_Init(&NVIC_InitStructure);

	SysTick->VAL   = 0;                                          /* Load the SysTick Counter Value */
	SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
			SysTick_CTRL_TICKINT_Msk   |
			SysTick_CTRL_ENABLE_Msk;                             /* Enable SysTick IRQ and SysTick Timer */
}

void sys_cfg_console() {
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Enable GPIO clock */
	RCC_AHBPeriphClockCmd(USARTx_TX_GPIO_CLK | USARTx_RX_GPIO_CLK, ENABLE);

	/* Enable USART clock */
	RCC_APB2PeriphClockCmd(USARTx_CLK, ENABLE);

	/* Connect PXx to USARTx_Tx */
	GPIO_PinAFConfig(USARTx_TX_GPIO_PORT, USARTx_TX_SOURCE, USARTx_TX_AF);

	/* Connect PXx to USARTx_Rx */
	GPIO_PinAFConfig(USARTx_RX_GPIO_PORT, USARTx_RX_SOURCE, USARTx_RX_AF);

	/* Configure USART Tx and Rx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_400KHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = USARTx_TX_PIN;
	GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = USARTx_RX_PIN;
	GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStructure);

	/* USARTx configuration */
	USART_InitStructure.USART_BaudRate = SYS_CONSOLE_BAUDRATE;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USARTx, &USART_InitStructure);

	/* Enable the USARTx Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USARTx_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_ClearITPendingBit(USARTx,USART_IT_RXNE);
	USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);

	/* Enable USART */
	USART_Cmd(USARTx, ENABLE);

	xfunc_out = xputchar;
}

void sys_cfg_svc() {
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = SVC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void sys_cfg_pendsv() {
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = PendSV_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0xF;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void sys_cfg_update_info() {
	extern uint32_t _start_flash;
	extern uint32_t _end_flash;
	extern uint32_t _start_ram;
	extern uint32_t _end_ram;
	extern uint32_t _data;
	extern uint32_t _edata;
	extern uint32_t _bss;
	extern uint32_t _ebss;
	extern uint32_t __heap_start__;
	extern uint32_t __heap_end__;
	extern uint32_t _estack;

	RCC_ClocksTypeDef RCC_Clocks;

	RCC_GetClocksFreq(&RCC_Clocks);

	system_info.cpu_clock = RCC_Clocks.HCLK_Frequency;
	system_info.tick      = 1;
	system_info.console_baudrate = SYS_CONSOLE_BAUDRATE;
	system_info.flash_used = ((uint32_t)&_end_flash - (uint32_t)&_start_flash) + ((uint32_t)&_edata - (uint32_t)&_data);
	system_info.ram_used = (uint32_t)&_estack - (uint32_t)&_start_ram;
	system_info.data_init_size = (uint32_t)&_edata - (uint32_t)&_data;
	system_info.data_non_init_size = (uint32_t)&_ebss - (uint32_t)&_bss;
	system_info.stack_avail = (uint32_t)&_estack - (uint32_t)&_end_ram;
	system_info.heap_avail = (uint32_t)&__heap_end__ - (uint32_t)&__heap_start__;
	system_info.ram_other = system_info.ram_used - (system_info.heap_avail + system_info.stack_avail + system_info.data_non_init_size + system_info.data_init_size);

	delay_coeficient = system_info.cpu_clock /1000000;

	/* kernel banner */
	SYS_PRINT("\n");
	SYS_PRINT("   __    _  _ \n");
	SYS_PRINT("  /__\\  ( )/ )\n");
	SYS_PRINT(" /(__)\\ (   ( \n");
	SYS_PRINT("(__)(__)(_)\\_)\n");
	SYS_PRINT("Wellcome to Active Kernel %s\n", AK_VERSION);
	SYS_PRINT("\n");

#if 0
	/* system banner */
	SYS_PRINT("system information:\n");
	SYS_PRINT("\tFLASH used:\t%d bytes\n", system_info.flash_used);
	SYS_PRINT("\tSRAM used:\t%d bytes\n", system_info.ram_used);
	SYS_PRINT("\t\tdata init size:\t\t%d bytes\n", system_info.data_init_size);
	SYS_PRINT("\t\tdata non_init size:\t%d bytes\n", system_info.data_non_init_size);
	SYS_PRINT("\t\tstack avail:\t\t%d bytes\n", system_info.stack_avail);
	SYS_PRINT("\t\theap avail:\t\t%d bytes\n", system_info.heap_avail);
	SYS_PRINT("\t\tother:\t\t\t%d bytes\n", system_info.ram_other);
	SYS_PRINT("\n");
	SYS_PRINT("\tcpu clock:\t%d Hz\n", system_info.cpu_clock);
	SYS_PRINT("\ttime tick:\t%d ms\n", system_info.tick);
	SYS_PRINT("\tconsole:\t%d bps\n", system_info.console_baudrate);
	SYS_PRINT("\n\n");
#endif
}


/******************************************************************************
* system utilities function
*******************************************************************************/
void xputchar(uint8_t c) {
	/* wait last transmission completed */
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);

	/* put transnission data */
	USART_SendData(USARTx, (uint8_t)c);

	/* wait transmission completed */
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
}

void sys_ctrl_shell_put_char(uint8_t c) {
	/* wait last transmission completed */
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);

	/* put transnission data */
	USART_SendData(USARTx, (uint8_t)c);

	/* wait transmission completed */
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
}

uint8_t sys_ctrl_shell_get_char() {
	volatile uint8_t c;

	if (USART_GetITStatus(USARTx, USART_IT_RXNE) == SET) {
		c = (uint8_t)USART_ReceiveData(USARTx);
		USART_ClearITPendingBit(USARTx, USART_IT_RXNE);
	}

	return c;
}

void sys_ctrl_reset() {
	NVIC_SystemReset();
}

void timer9_int() {

	ENTRY_CRITICAL();
	TIM_TimeBaseInitTypeDef  timer9;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* ---------------------------------------------------------------------------
	  TIM9 Configuration: generate a PWM signal:
	  The TIM11CLK frequency is set to SystemCoreClock (Hz).
	  SystemCoreClock is set to 32 MHz for Ultra Low Power Medium-Density Devices.
	  TIM11 prescaler is set to 0
	  The TIM11 is running at 32 MHz Frequency = TIM9 counter clock/(ARR + 1)
													  = 32 MHz / 320000/8 = 0.1 KHz=100hz=>10us
	  TIM11 Channel1 duty cycle = (TIM11_CCR1/ (TIM11_ARR+1))* 100 = 333 / (665+1) = 50 %
	---------------------------------------------------------------------------- */
	/* timer 50us to polling receive IR signal */
	RCC_APB1PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);
	timer9.TIM_Period = 1592;
	timer9.TIM_Prescaler = 0;
	timer9.TIM_ClockDivision = 0;
	timer9.TIM_CounterMode = TIM_CounterMode_Down;
	TIM_TimeBaseInit(TIM9, &timer9);

	NVIC_InitStructure.NVIC_IRQChannel = TIM9_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	TIM_ITConfig(TIM9, TIM_IT_Update, ENABLE);
	ENTRY_CRITICAL();
	TIM_Cmd(TIM9, DISABLE);
}

void sys_ctrl_delay_us(volatile uint32_t count) {
	volatile uint32_t delay_value = 0;
	delay_value = count*delay_coeficient / 8;
	while(delay_value--);
}

void sys_ctrl_delay_ms(volatile uint32_t count) {
#if 0
	volatile uint32_t current_time = 0;
	volatile uint32_t current;
	volatile int32_t start = sys_ctrl_millis();

	while(current_time < count) {
		current = sys_ctrl_millis();

		if (current < start) {
			current_time += ((uint32_t)0xFFFFFFFF - start) + current;
		}
		else {
			current_time += current - start;
		}

		start = current;
	}
#else
	volatile uint32_t delay = 1000 * count;
	sys_ctrl_delay_us(delay);
#endif
}

void sys_ctr_sleep_wait_for_irq() {
	__WFI();
}

uint32_t sys_ctr_get_exception_number() {
	volatile uint32_t exception_number = (uint32_t)__get_IPSR();
	return exception_number;
}

void sys_ctr_restart_app() {
	volatile uint32_t normal_stack_pointer	=	(uint32_t) *(volatile uint32_t*)(APP_START_ADDR);
	volatile uint32_t normal_jump_address	=	(uint32_t) *(volatile uint32_t*)(APP_START_ADDR + 4);


	p_jump_func jump_to_normal = (p_jump_func)normal_jump_address;

	/* Disable interrupt */
	DISABLE_INTERRUPTS();

	__DMB();

	/* update interrupt vertor table */
	SCB->VTOR = APP_START_ADDR;

	/* set stack pointer */
	__asm volatile ("MSR msp, %0\n" : : "r" (normal_stack_pointer) : "sp");

	__DSB();

	/* jump to normal program */
	jump_to_normal();

	while(1);
}

void sys_ctrl_independent_watchdog_init() {
	/* enable PCLK1 for watchdog timer */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);

	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(IWDG_Prescaler_256);		/* IWDG counter clock: 37KHz(LSI) / 256 = 0.144 KHz */
	IWDG_SetReload(0xFFF);						/* Set counter reload, T = (1/IWDG counter clock) * Reload_counter  = 30s  */
	IWDG_ReloadCounter();
	IWDG_Enable();
}

void sys_ctrl_independent_watchdog_reset() {
	ENTRY_CRITICAL();
	IWDG_ReloadCounter();
	EXIT_CRITICAL();
}

static uint32_t sys_ctrl_soft_counter = 0;
static uint32_t sys_ctrl_soft_time_out;

void sys_ctrl_soft_watchdog_init(uint32_t time_out) {
	TIM_TimeBaseInitTypeDef  timer_100us;
	NVIC_InitTypeDef NVIC_InitStructure;
	sys_ctrl_soft_time_out = time_out;

	/* timer 10ms to polling receive IR signal */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
	timer_100us.TIM_Period = 3196;		/* HCLK=8Mhz and 3196 respective HCLK=32Mhz;*/
	timer_100us.TIM_Prescaler = 1000;
	timer_100us.TIM_ClockDivision = 0;
	timer_100us.TIM_CounterMode = TIM_CounterMode_Down;

	TIM_TimeBaseInit(TIM7, &timer_100us);

	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	/* highest priority level */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM7, ENABLE);
}

void sys_ctrl_soft_watchdog_reset() {
	ENTRY_CRITICAL();
	sys_ctrl_soft_counter = 0;
	EXIT_CRITICAL();
}

void sys_ctrl_soft_watchdog_enable() {
	ENTRY_CRITICAL();
	TIM_Cmd(TIM7, ENABLE);
	EXIT_CRITICAL();
}

void sys_ctrl_soft_watchdog_disable() {
	ENTRY_CRITICAL();
	TIM_Cmd(TIM7, DISABLE);
	EXIT_CRITICAL();
}

void sys_ctrl_soft_watchdog_increase_counter() {
	sys_ctrl_soft_counter++;
	if (sys_ctrl_soft_counter >= sys_ctrl_soft_time_out) {
		TIM_Cmd(TIM7, DISABLE);
		FATAL("SWDG", 0x01);
	}
}

void sys_ctrl_get_firmware_info(firmware_header_t* header) {
	extern uint32_t _start_flash;
	extern uint32_t _end_flash;
	extern uint32_t _data;
	extern uint32_t _edata;

	uint32_t check_sum = 0;
	uint32_t len_of_flash = (uint32_t)&_end_flash - (uint32_t)&_start_flash + ((uint32_t)&_edata - (uint32_t)&_data) + sizeof(uint32_t);

	for (uint32_t index = (uint32_t)&(_start_flash); index <= ((uint32_t)&(_start_flash) + len_of_flash); index += sizeof(uint32_t)) {
		check_sum += *((uint32_t*)index);
	}

	header->psk = FIRMWARE_PSK;
	header->checksum = (check_sum & 0xFFFF);
	header->bin_len = len_of_flash;
}

void ak_irq_io_entry_trigger() {
#if defined(AK_IO_IRQ_ANALYZER)
	GPIO_ResetBits(LED_LIFE_IO_PORT, LED_LIFE_IO_PIN);
#endif
}

void ak_irq_io_exit_trigger() {
#if defined(AK_IO_IRQ_ANALYZER)
	GPIO_SetBits(LED_LIFE_IO_PORT, LED_LIFE_IO_PIN);
#endif
}
