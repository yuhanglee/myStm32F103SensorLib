#ifndef __BEEP_H__
#define __BEEP_H__


#include "gpio/gpio.h"
#include "stm32f10x.h"


typedef struct __BEEP_TYPE {
	GPIO_TYPE gpio;
	uint8_t   highLevel;

	void    (*Init)(struct __BEEP_TYPE *me);
	void    (*On)(struct __BEEP_TYPE *me);
	void    (*Off)(struct __BEEP_TYPE *me);
	void    (*Write)(struct __BEEP_TYPE *me, uint8_t status);
	uint8_t    (*Read)(struct __BEEP_TYPE *me);
} BEEP_TYPE;

void BEEP_CreateDefaultFuncBEEP(BEEP_TYPE *me);
void BEEP_Demo(void);


#endif
/*********************END*****************/


