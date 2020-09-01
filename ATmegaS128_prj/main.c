/*
 * main.c
 *
 * Created: 01.09.2020 15:29:19
 * Author : Babon
 */ 

#include <avr/wdt.h>
#include "init.h"

int main(void) {

    User_System_Init();

    while (1) 
		wdt_reset();
}
