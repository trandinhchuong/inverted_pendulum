#ifndef __TASK_ENCODER__
#define __TASK_ENCODER__

#include "led.h"

#define start 1
#define stop 0

#define SHOW 1
#define SET_KP 2
#define SET_KD 3
#define SET_xKP 4
#define SET_xKD 5

#define START_ADD			(0x08080000)
#define STOP_ADD			(0x08080000)
#define KP_FLASH_ADD		(0X00)
#define KD_FLASH_ADD		(0x0f)
#define XKD_FLASH_ADD		(0x080805FF)
#define XKP_FLASH_ADD		(0x080806FF)

#define POSKP 0

void display_encoder();
void encoder_read ();
void canbang();
void flashRead(float* value ,uint32_t sector );
void flashWrite(float* value ,uint32_t sector );

//extern uint8_t  eeprom_read(uint32_t, uint8_t*, uint32_t);
//extern uint8_t  eeprom_write(uint32_t, uint8_t*, uint32_t);
//extern uint8_t  eeprom_erase(uint32_t address, uint32_t len);

void PWM(double val);
void display();
uint16_t microus();
#endif //__TASK_ENCODER__
