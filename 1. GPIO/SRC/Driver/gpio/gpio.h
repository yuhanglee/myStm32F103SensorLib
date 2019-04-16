#ifndef __GPIO_H__
#define __GPIO_H__

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"


typedef struct {
	GPIO_TypeDef     *Port;
	GPIO_InitTypeDef GPIOInitTypeStruct;
	uint8_t          IntValue;
} GPIO_TYPE;

uint8_t GPIOX_Init(GPIO_TYPE gpio);
uint8_t GPIOX_GetPinBit(GPIO_TYPE gpio);

void GPIOTestDemo(void);

#endif
