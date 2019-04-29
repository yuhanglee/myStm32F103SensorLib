#ifndef __ADC_H__
#define __ADC_H__

#include "stm32f10x.h"
#include "gpio/gpio.h"
#include "usart/uart.h"

typedef struct __ADC_TYPE {
	ADC_TypeDef * ADC;
	ADC_InitTypeDef ADC_InitStru;
	GPIO_TYPE     gpio;
	uint16_t      channelNum;

	void     (*Init)(struct __ADC_TYPE *me);
	void     (*Open)(struct __ADC_TYPE const *me);
	void     (*Close)(struct __ADC_TYPE const *me);
	uint16_t (*ReadValue)(struct __ADC_TYPE const *me);
	float    (*ReadVoltage)(struct __ADC_TYPE const *me);
} ADC_TYPE;

void ADC_Demo(void);
void ADC_CreateDefaultADC(ADC_TYPE *me);


#endif


