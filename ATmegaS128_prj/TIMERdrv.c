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
	// ������������� �������� �������� �������, ��. ������� 19-6 �������� (��� ��������� ���� ������� �� ���� ������ ��� ������ PWM)
	TCCR0 |= 7;	// �������� ����� �� 1024 (8e6 / 1024 = 128 ���)
	// ������������� ������ �� ����������� ��� ������������
	TIMSK |= 1;
	return 0;
}

int InitWatchDog(void) {
	// ������������� �������� watchdog �������, ��. ������� 13-2
	WDTCR = (1 << 3) | 2;
	return 0;
}

// ����. ������� ��� ���������� watchdog'a
int DisableWatchDog(void) {
	 /* Reset WDT*/
	 wdt_reset();
	 /* Write logical one to WDCE and WDE */
	 WDTCR |= (1<<WDCE) | (1<<WDE);
	 /* Turn off WDT */
	 WDTCR = 0x00;
	return 0;
}

// ���������� ����������, ���������� ������ 32.768 ��
// Note: ��� ������������� TIMER0_OVF0_vect �������� warning
ISR(_VECTOR(TIMER0_OVF0_vect_num)) {
	static unsigned int cnt = 0;
	static uint8_t data[4] = {0xAA, 0xBB, 0xCC, 0xDD};
	// ��������� ������ ������������ �������
	if((TIFR & 1) == 1) {	
		cnt++;
	
		// ��� ��������� ������ ���������� GPIO
		//...

		// ��� ��������� ������ ���������� SPI
		SPIM_irq_write(data, 4);
	
		// ���������� ������� �������������� ����� �� ����� ������������ uint32_t
		if(cnt == 1e9)
			cnt = 0;
		// ������������� ���������� ���� ��������� ����������
		TIFR = 1;
	}
}
