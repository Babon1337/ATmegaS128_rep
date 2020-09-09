/*
 * SPIdrv.c
 *
 * Created: 09.09.2020 10:18:10
 *  Author: Babon
 */ 

#include <avr/io.h>
#include <stdint.h>
#include "SPIdrv.h"
#include <avr/interrupt.h>

// Определения для передачи по SPI в режиме ожидания
#define CS_ENABLE		PORTB &=~ (1 << 0)
#define CS_DISABLE		PORTB |= (1 << 0)
#define SPI_WAITING_DR	1000

// Определения для передачи по SPI внутри прерваний
//static uint8_t SPI_FIFO_RX[128];	// временно реализована только передача
#define SPI_FIFO_TX_SIZE	128
static uint8_t SPI_FIFO_TX[SPI_FIFO_TX_SIZE];
static uint16_t SPI_FIFO_TX_Tail = 0;
static uint16_t SPI_FIFO_TX_Head = 0;

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
	SPCR |= (1 << SPE) | (1 << MSTR) | (1 << SPIE);

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

int SPIM_irq_write(const uint8_t* data, const unsigned int size) {
	unsigned int i;
	// Проверяем размер входного буфера
	if(size >= SPI_FIFO_TX_SIZE)
		return -1;
	CS_ENABLE;
	SPDR = data[0];
	for(i = 1; i < size; i++) {
		SPI_FIFO_TX[SPI_FIFO_TX_Head] = data[i];
		SPI_FIFO_TX_Head++;
		SPI_FIFO_TX_Head &= (SPI_FIFO_TX_SIZE-1);
	}
	return i;
}

// Обработчик прерывания
// TODO: разобраться почему объявления _VECTOR в коде ядра и в даташите не совпадают
ISR(_VECTOR(0x22)) {
	// Проверяем статус передачи
	if((SPSR & (1 << 6)) == (1 << 6)) {
		// Проверяем что в буфере еще есть данные
		if(SPI_FIFO_TX_Tail == SPI_FIFO_TX_Head) {
			CS_DISABLE;
			return;
		}
		// Передаем данные
		SPDR = SPI_FIFO_TX[SPI_FIFO_TX_Tail];
		SPI_FIFO_TX_Tail++;
		SPI_FIFO_TX_Tail &= (SPI_FIFO_TX_SIZE-1);	
	}
}
