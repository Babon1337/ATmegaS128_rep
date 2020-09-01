/*
 * TIMERdrv.h
 *
 * Created: 01.09.2020 15:32:00
 * Author : Babon
 */ 

#ifndef TIMERDEV_H_
#define TIMERDEV_H_

#ifdef __cplusplus
	extern "C"{
#endif

	int InitTIMER(void);
	int InitWatchDog(void);
	int DisableWatchDog(void);

#ifdef __cplusplus
	}
#endif

#endif
