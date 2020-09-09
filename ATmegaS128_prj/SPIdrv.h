/*
 * SPIdrv.h
 *
 * Created: 09.09.2020 10:19:45
 *  Author: Babon
 */ 

#ifndef SPIDRV_H_
#define SPIDRV_H_

#include <stdint.h>

#ifdef __cplusplus
	extern "C" {
#endif

	int InitSPI(const uint32_t baudrate);
	int SPIM_write(const uint8_t* data, const unsigned int size);
	int SPIM_irq_write(const uint8_t* data, const unsigned int size);

#ifdef __cplusplus
	}
#endif

#endif /* SPIDRV_H_ */
