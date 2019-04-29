#ifndef __I2C_H__
#define __I2C_H__

#include "stm32f10x.h"
#include "gpio/gpio.h"



typedef struct IIC{
	GPIO_TYPE		GPIO_WP;
	GPIO_TYPE		GPIO_SCL;
	GPIO_TYPE		GPIO_SDA;

	void (*Init)(struct IIC *);
	void (*Start)(struct IIC);
	void (*Stop)(struct IIC);
	FlagStatus (*WaitAck)(struct IIC);
	void (*SendAck)(struct IIC);
	void (*SendNoAck)(struct IIC);
	
	void (*WriteDataOneByte)(struct IIC, uint8_t);
	void (*WriteDataOnesByte)(struct IIC, uint8_t *, uint16_t len);
	uint8_t (*ReadDataOneByte)(struct IIC, uint8_t *);
	uint8_t (*ReadDataOnesByte)(struct IIC, uint8_t *, uint8_t len);
} IIC_TYPE;


void IIC_CreateDefaultIIC(IIC_TYPE *iic);

#endif
