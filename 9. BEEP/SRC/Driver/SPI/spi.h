#ifndef __SPI_H__
#define __SPI_H__

#include "stm32f10x.h"
#include "gpio/gpio.h"
#include "usart/uart.h"

#define SPI_MODE_INSIDE     0
#define SPI_MODE_MODEL      1


typedef struct _SPI_TYPE {
    void        * SPIx;
	SPI_InitTypeDef InitStru;
    GPIO_TYPE   GPIO_CS;
    GPIO_TYPE   GPIO_CLK;
    GPIO_TYPE   GPIO_MOSI;
    GPIO_TYPE   GPIO_MISO;
    uint16_t    SPI_CPOL;
    uint16_t    SPI_CPHA;
    uint8_t     SPI_MODE;

	void       (*Init)(struct _SPI_TYPE *me);
	void       (*Open)(struct _SPI_TYPE const *me);
	void       (*Close)(struct _SPI_TYPE const *me);
	void       (*Write)(struct _SPI_TYPE const *me, uint8_t const data);
	uint8_t    (*Read)(struct _SPI_TYPE const *me, uint8_t const data);
} SPI_TYPE;


void SPICreateDefaultFunction(SPI_TYPE *SPI);


#endif
