#ifndef __DAC_H__
#define __DAC_H__

#include "stm32f10x.h"
#include "gpio/gpio.h"
#include "usart/uart.h"


typedef struct __DAC_TYPE {
	uint16_t     channelNum;
	DAC_InitTypeDef DAC_InitStru;
	GPIO_TYPE   gpio;

	void        (*Init)(struct __DAC_TYPE *me);
	void        (*Open)(struct __DAC_TYPE *me);
	void        (*Close)(struct __DAC_TYPE *me);
	void        (*WriteValue)(struct __DAC_TYPE *me, uint16_t Value);
	void        (*WriteVoltage)(struct __DAC_TYPE *me, float Voltage);
	uint32_t    (*ReadValue)(struct __DAC_TYPE *me);
	float       (*ReadVoltage)(struct __DAC_TYPE *me);
} DAC_TYPE;


void DAC_CreateDefaultDAC(DAC_TYPE *me);
void DAC_Demo(void);

#endif
