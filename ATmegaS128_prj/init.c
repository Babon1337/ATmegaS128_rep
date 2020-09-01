/*
 * init.c
 *
 * Created: 01.09.2020 15:32:00
 * Author : Babon
 */ 

#include "init.h"
#include "GPIOdrv.h"
#include "TIMERdrv.h"
#include <avr/interrupt.h>

int User_System_Init(void) {

	if(InitGPIO() != 0)
		return -1;

	if(InitTIMER() != 0)
		return -2;

	if(InitWatchDog() != 0)
		return -3;

	// –азрешаем обработку вектора прерываний
	sei();

	return 0;
}