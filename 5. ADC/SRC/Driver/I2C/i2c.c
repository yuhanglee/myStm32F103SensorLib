#include "I2C/i2c.h"


#include "stdlib.h"
#include "string.h"
#include "stdio.h"





static void IIC_DelayUs(u16 t2)
{
	u16 i, j;
	for(i = 0; i < 12; i++)
		for(j = 0; j < t2; j++);
}

static void IIC_GPIO_RESET(GPIO_TYPE gpio) {
	if (gpio.Port != 0x00000000) {
		GPIO_ResetBits(gpio.Port, gpio.GPIOInitTypeStruct.GPIO_Pin);
	}
}


static void IIC_GPIO_SET(GPIO_TYPE gpio) {
	if (gpio.Port != 0x00000000) {
		GPIO_SetBits(gpio.Port, gpio.GPIOInitTypeStruct.GPIO_Pin);
	}
}

static void IIC_SdaMode(GPIO_TYPE *gpio, GPIOMode_TypeDef mode) {
	gpio->GPIOInitTypeStruct.GPIO_Mode = mode;
	GPIOX_Init(*gpio);
}

static void IIC_Init(IIC_TYPE * iic) {
	GPIO_TYPE * gpio;

	gpio = &(iic->GPIO_WP);
	gpio->GPIOInitTypeStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio->GPIOInitTypeStruct.GPIO_Speed = GPIO_Speed_50MHz;
	gpio->IntValue = RESET;
	GPIOX_Init(*gpio);

	gpio = &(iic->GPIO_SCL);
	gpio->GPIOInitTypeStruct.GPIO_Mode = GPIO_Mode_Out_OD;
	gpio->GPIOInitTypeStruct.GPIO_Speed = GPIO_Speed_50MHz;
	gpio->IntValue = RESET;
	GPIOX_Init(*gpio);

	gpio = &(iic->GPIO_SDA);
	gpio->GPIOInitTypeStruct.GPIO_Mode = GPIO_Mode_Out_OD;
	gpio->GPIOInitTypeStruct.GPIO_Speed = GPIO_Speed_50MHz;
	gpio->IntValue = RESET;
	GPIOX_Init(*gpio);

	IIC_GPIO_RESET(iic->GPIO_WP);
}

static void IIC_Start(IIC_TYPE iic) {
	IIC_SdaMode(&(iic.GPIO_SDA), GPIO_Mode_Out_OD);
	IIC_GPIO_SET(iic.GPIO_SDA);
	IIC_GPIO_SET(iic.GPIO_SCL);
	IIC_DelayUs(4);
	IIC_GPIO_RESET(iic.GPIO_SDA);
	IIC_DelayUs(4);
	IIC_GPIO_RESET(iic.GPIO_SCL);
}

static void IIC_Stop(IIC_TYPE iic) {
	IIC_SdaMode(&(iic.GPIO_SDA), GPIO_Mode_Out_OD);
	IIC_GPIO_RESET(iic.GPIO_SCL);
	IIC_GPIO_RESET(iic.GPIO_SDA);
	IIC_DelayUs(4);
	IIC_GPIO_SET(iic.GPIO_SCL);
	IIC_GPIO_SET(iic.GPIO_SDA);
	IIC_DelayUs(4);
}

static FlagStatus IIC_WaitAck(IIC_TYPE iic) {
	uint8_t errTime = 0;

	IIC_SdaMode(&(iic.GPIO_SDA), GPIO_Mode_IN_FLOATING);

	IIC_GPIO_SET(iic.GPIO_SDA);
	IIC_DelayUs(4);
	IIC_GPIO_SET(iic.GPIO_SCL);
	IIC_DelayUs(4);

	while (GPIOX_GetPinBit(iic.GPIO_SDA)) {
		errTime++;

		if (errTime > 250) {
			iic.Stop(iic);
			return SET;
		}
	}

	IIC_GPIO_RESET(iic.GPIO_SCL);
	return RESET;
}


static void IIC_SendAck(IIC_TYPE iic) {
	IIC_SdaMode(&(iic.GPIO_SDA), GPIO_Mode_Out_OD);
	
	IIC_GPIO_RESET(iic.GPIO_SCL);
	IIC_GPIO_RESET(iic.GPIO_SDA);
	IIC_DelayUs(2);
	IIC_GPIO_SET(iic.GPIO_SCL);
	IIC_DelayUs(2);
	IIC_GPIO_RESET(iic.GPIO_SCL);
}

static void IIC_SendNoAck(IIC_TYPE iic) {
	IIC_SdaMode(&(iic.GPIO_SDA), GPIO_Mode_Out_OD);

	IIC_GPIO_RESET(iic.GPIO_SCL);
	IIC_GPIO_SET(iic.GPIO_SDA);
	IIC_DelayUs(2);
	IIC_GPIO_SET(iic.GPIO_SDA);
	IIC_DelayUs(2);
	IIC_GPIO_RESET(iic.GPIO_SCL);
}

static void IIC_SendByte(IIC_TYPE iic, uint8_t data) {
	uint8_t i;

	IIC_SdaMode(&(iic.GPIO_SDA), GPIO_Mode_Out_OD);

	IIC_GPIO_RESET(iic.GPIO_SCL);
	
	for (i = 0; i < 8; i++) {
		if ((data & 0x80) >> 7) {
			IIC_GPIO_SET(iic.GPIO_SDA);
		} else {
			IIC_GPIO_RESET(iic.GPIO_SDA);
		}

		data <<= 1;
		IIC_DelayUs(2);
		IIC_GPIO_SET(iic.GPIO_SCL);
		IIC_DelayUs(2);
		IIC_GPIO_RESET(iic.GPIO_SCL);
		IIC_DelayUs(2);
	}
}

static uint8_t IIC_ReadByte(IIC_TYPE iic, uint8_t * data) {
	uint8_t i, receive = 0;

	IIC_SdaMode(&(iic.GPIO_SDA), GPIO_Mode_IN_FLOATING);

	for (i = 0; i < 8; i++) {
		IIC_GPIO_RESET(iic.GPIO_SCL);
		IIC_DelayUs(2);
		IIC_GPIO_SET(iic.GPIO_SCL);
		receive <<= 1;
		
		if (GPIOX_GetPinBit(iic.GPIO_SDA)) {
			receive++;
		}
		IIC_DelayUs(1);
	}
	
	*data = receive;
    
	return receive;
}
void IIC_CreateDefaultIIC(IIC_TYPE *iic) {
	memset(iic, 0, sizeof(IIC_TYPE));

	
	iic->Init = IIC_Init;
	iic->Start = IIC_Start;
	iic->Stop = IIC_Stop;
	iic->WaitAck = IIC_WaitAck;
	iic->SendNoAck = IIC_SendNoAck;
	iic->SendAck = IIC_SendAck;
	iic->WriteDataOneByte = IIC_SendByte;
	iic->ReadDataOneByte = IIC_ReadByte;
}


/*
----------------------------------------
*/

// 需要使用驱动进行测量，单独测量，实验现象不好确定
	
/*
----------------------------------------
*/

