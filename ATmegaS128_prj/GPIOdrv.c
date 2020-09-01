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
	// ������������� ���� 0-4 ����� � ��� ������
	DDRA = 0xF;
	// ������������� �������� �� ������ - 0
	PORTA = 0x0;
	// ����� NOP ��� ������������� (�������� ����� ��������� ������ � ������ ����, ��� ������ ��������� ��������)
	_NOP();

	return 0;
}
