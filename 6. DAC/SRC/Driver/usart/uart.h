#ifndef __UART4_H__
#define __UART4_H__

#include "stm32f10x.h"
#include "stdio.h"
#include "gpio/gpio.h"


typedef void(*UartCallbackFunc)(uint8_t); 

typedef struct UART_TYPE{
	USART_InitTypeDef USART_InitType;
	USART_TypeDef * USARTX;
	GPIO_TYPE GPIO_RX;
	GPIO_TYPE GPIO_TX;
	
	void(*Init)(struct UART_TYPE);		// 初始化
	void(*Reset)(struct UART_TYPE);		// 管脚设置为输入下拉，关闭串口时钟
	void(*CallbackFunc)(uint8_t);			// 回调函数
	void(*Open)(USART_TypeDef *);		// 使能串口中断
	void(*Close)(USART_TypeDef *);		// 失能串口中断

	void(*SendChar)(USART_TypeDef *, uint8_t);
	void(*SendData)(USART_TypeDef *, uint8_t *, uint16_t);
	void(*SendString)(USART_TypeDef *, uint8_t *);
} UART_TYPE;


void USART1_DefaultInit(UART_TYPE * usart, uint32_t baudrate);
void USART2_DefaultInit(UART_TYPE * usart, uint32_t baudrate);
void USART3_DefaultInit(UART_TYPE * usart, uint32_t baudrate);
void UART4_DefaultInit(UART_TYPE * usart, uint32_t baudrate);
void UART5_DefaultInit(UART_TYPE * usart, uint32_t baudrate);


void USARTX_CreateDefaultUsart(UART_TYPE * usart);
void UartTestDemo(void);


#endif 


