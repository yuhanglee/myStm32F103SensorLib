#ifndef __LED_H__
#define __LED_H__

#include "stm32f10x.h"
#include "gpio/gpio.h"


#define LED_MAX 4

typedef struct __LED_TYPE LED_TYPE;

struct __LED_TYPE {
	GPIO_TYPE gpio;
	uint8_t  id;
	uint8_t  HighLevel;
	uint16_t Period;
	uint16_t Count;
	uint16_t Light;
	uint16_t (*Init)(LED_TYPE*);
	void (*SetLight)(LED_TYPE *, FlagStatus);
	uint8_t (*GetLight)(LED_TYPE *);
	void (*SetPeriod)(LED_TYPE *, int, int);
};

void LED_CreateDefaultInit(LED_TYPE *me);
void LED_Demo(void);


#endif
