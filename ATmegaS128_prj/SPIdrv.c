/*
 * SPIdrv.c
 *
 * Created: 09.09.2020 10:18:10
 *  Author: Babon
 */ 

#include <avr/io.h>
#include <stdint.h>
#include "SPIdrv.h"

#define CS_ENABLE		PORTB &=~ (1 << 0)
#define CS_DISABLE		PORTB |= (1 << 0)
#define SPI_WAITING_DR	1000

/* Инициализация SPI0 в режиме мастер */
// TODO: доработать функцию для работы в режиме slave, а так же различными настройками
int InitSPI(const uint32_t baudrate) {
	
	// Проверка входных параметров
	if(baudrate > 4e6)
		return -1;

	// Конфигурация GPIO
	// PB0 - SS
	// PB1 - SCK
	// PB2 - MOSI
	// PB3 - MISO
	DDRB = (1 << DDB0) | (1 << DDB1) | (1 << DDB2);
	CS_DISABLE;

	//SPR0 - делитель частоты шины см. таблицу 22-5 даташита
	uint8_t val = 8e6 / baudrate;
	if(val == 2)
	{
		SPCR = 0;
		SPSR = 1;	// SPI2X = 1
	}
	else if(val <= 4)
	{
		SPCR = 0;
		SPSR = 0;	// SPI2X = 0
	}
	else if(val <= 8)
	{
		SPCR = 1;
		SPSR = 1;
	}
	else if(val <= 16)
	{
		SPCR = 1;
		SPSR = 0;		
	}
	else if(val <= 32)
	{
		SPCR = 2;
		SPSR = 1;
	}
	else if(val <= 64)
	{
		SPCR = 2;
		SPSR = 0;
	}
	else if(val <= 128)
	{
		SPCR = 3;
		SPSR = 0;
	}
	// SPE - включение SPI, MSTR - включение режима мастер
	SPCR |= (1 << SPE) | (1 << MSTR);

	return 0;
}

int SPIM_write(const uint8_t* data, const unsigned int size)
{
	unsigned int i;
	uint16_t val;
	CS_ENABLE;
	for(i = 0; i < size; i++) {
		SPDR = data[i];
		// Ожидаем пока данные передадутся
		for(val = 0; val < SPI_WAITING_DR; val++)
			if((SPSR & (1 << 6)) == (1 << 6))
				break;
	}
	CS_DISABLE;
	return i;
}
