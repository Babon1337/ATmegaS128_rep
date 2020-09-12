/*
 * TIMERdrv.c
 *
 * Created: 01.09.2020 15:40:19
 * Author : Babon
 */ 

#include "TIMERdrv.h"
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include "SPIdrv.h"

int InitTIMER(void) {
	// Устанавливаем основной делитель таймера, см. таблицу 19-6 даташита (все остальные биты отличны от нуля только для режима PWM)
	TCCR0 |= 7;	// делитель клока на 1024 (8e6 / 1024 = 128 мкс)
	// Устанавливаем работу по прерываниям при переполнении
	TIMSK |= 1;
	return 0;
}

int InitWatchDog(void) {
	// Устанавливаем делитель watchdog таймера, см. таблицу 13-2
	WDTCR = (1 << 3) | 2;
	return 0;
}

// Спец. функция для выключения watchdog'a
int DisableWatchDog(void) {
	 /* Reset WDT*/
	 wdt_reset();
	 /* Write logical one to WDCE and WDE */
	 WDTCR |= (1<<WDCE) | (1<<WDE);
	 /* Turn off WDT */
	 WDTCR = 0x00;
	return 0;
}

// Обработчик прерывания, вызывается каждые 32.768 мс
// Note: при использовании TIMER0_OVF0_vect получаем warning
ISR(_VECTOR(TIMER0_OVF0_vect_num)) {
	static unsigned int cnt = 0;
	static uint8_t data[4] = {0xAA, 0xBB, 0xCC, 0xDD};
	// Проверяем статус переполнения таймера
	if((TIFR & 1) == 1) {	
		cnt++;
	
		// Тут реализуем логику управления GPIO
		//...

		// Тут реализуем логику управления SPI
		SPIM_irq_write(data, 4);
	
		// Сбрасываем счетчик самостоятельно чтобы не ждать переполнения uint32_t
		if(cnt == 1e9)
			cnt = 0;
		// Принудительно сбрасываем флаг обработки прерывания
		TIFR = 1;
	}
}
