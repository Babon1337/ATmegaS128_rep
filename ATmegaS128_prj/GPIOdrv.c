/*
 * GPIOdrv.c
 *
 * Created: 01.09.2020 15:40:19
 * Author : Babon
 */ 

#include "GPIOdrv.h"
#include <avr/io.h>
#include <avr/cpufunc.h>

int InitGPIO(void) {
	// Устанавливаем пины 0-4 порта А как выходы
	DDRA = 0xF;
	// Устанавливаем значение по выходу - 0
	PORTA = 0x0;
	// Нужен NOP для синхронизации (возможно нужно добавлять только в режиме вход, для чтения состояния регистра)
	_NOP();

	return 0;
}
