/*
 *  Uart.c - Serial Interface Driver ($vision: V1.0$)
 *
 *  Copyright (C) 2001, 2002 Will Liu <liushenglin@cryo-service.com.cn>
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * @file    uart.c
 * @author  sqdtek team
 * @version V1.0.0
 * @date    2017-06-07
 *
 * This program developed by Beijing SQD Technology CO.,LTD.;
 * @brief   This file defines usart1,2,3\uart4,5 hardware drivers.
 *
 *   This file is confi dential. Recipient(s) named above is(are) obligated
 * to maintain secrecy and is(are) not permitted to disclose the contents
 * of this communication to others. Thanks!
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */


#include "usart/uart.h"

#include "stdlib.h"
#include "string.h"
#include "stdio.h"


// 定义printf库函数使用的串口号
#define PRINT_UARTX		USART1


static UartCallbackFunc Usart1_Handler = NULL;
static UartCallbackFunc Usart2_Handler = NULL;
static UartCallbackFunc Usart3_Handler = NULL;
static UartCallbackFunc Uart4_Handler = NULL;
static UartCallbackFunc Uart5_Handler = NULL;

void USART1_IRQHandler(void);
void USART2_IRQHandler(void);
void USART3_IRQHandler(void);
void UART4_IRQHandler(void);
void UART5_IRQHandler(void);



void USARTX_SetNVIC(UART_TYPE me) {
	NVIC_InitTypeDef NVIC_InitStructure = {0};
	uint8_t IRQChannel = 0;

	switch ((uint32_t)me.USARTX) {
		case (uint32_t)(USART1): {
			IRQChannel = USART1_IRQn;
		}
		break;

		case (uint32_t)(USART2): {
			IRQChannel = USART2_IRQn;
		}
		break;

		case (uint32_t)(USART3): {
			IRQChannel = USART3_IRQn;
		}
		break;
		
		case (uint32_t)(UART4): {
			IRQChannel = UART4_IRQn;
		}
		break;

		case (uint32_t)(UART5): {
			IRQChannel = UART5_IRQn;
		}
		break;
}
	NVIC_InitStructure.NVIC_IRQChannel = IRQChannel;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void USARTX_SetCallbackFunc(USART_TypeDef * UART,        UartCallbackFunc func) {
	switch ((uint32_t)UART) {
		case (uint32_t)USART1: {
			Usart1_Handler = func;
		}
		break;

		case (uint32_t)USART2: {
			Usart2_Handler = func;
		}
		break;

		case (uint32_t)USART3: {
			Usart3_Handler = func;
		}
		break;

		case (uint32_t)UART4: {
			Uart4_Handler = func;		
		}
		break;

		case (uint32_t)UART5: {
			Uart5_Handler = func;
		}
		break;
	}
}

void USARTX_SendChar(USART_TypeDef * X, uint8_t c) {
	USART_SendData(X, c);
	while (!(X->SR & USART_FLAG_TXE));
}

void USARTX_SendData(USART_TypeDef * X, uint8_t * data, uint16_t len) {
	uint16_t i;

	for (i = 0; i < len; i++) {
		USARTX_SendChar(X, data[i]);
	}
}

void USARTX_SendString(USART_TypeDef * X, uint8_t * str) {
	USARTX_SendData(X, str, strlen(str));
}

void USARTX_Open(USART_TypeDef * X) {
	USART_Cmd(X, ENABLE);

    USART_ClearITPendingBit(X, USART_IT_TXE);
    USART_ClearITPendingBit(X, USART_IT_TC);
    USART_ClearITPendingBit(X, USART_IT_RXNE);

	USART_ITConfig(X, USART_IT_TXE, ENABLE);
    USART_ITConfig(X, USART_IT_RXNE, ENABLE);
    USART_ITConfig(X, USART_IT_TC, ENABLE);
}

void USARTX_Colse(USART_TypeDef * X) {
	USART_Cmd(X, DISABLE);

	USART_ITConfig(X, USART_IT_TXE, DISABLE);
    USART_ITConfig(X, USART_IT_RXNE, DISABLE);
    USART_ITConfig(X, USART_IT_TC, DISABLE);

    USART_ClearITPendingBit(X, USART_IT_TXE);
    USART_ClearITPendingBit(X, USART_IT_TC);
    USART_ClearITPendingBit(X, USART_IT_RXNE);
}

void USARTX_Init(UART_TYPE me) {
	USART_InitTypeDef USART_InitStructure = me.USART_InitType;
	USART_TypeDef * UART = me.USARTX;

	
	switch ((uint32_t)me.USARTX) {
		case (uint32_t)USART1: {
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
		}
		break;

		case (uint32_t)USART2: {
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
		}
		break;

		case (uint32_t)USART3: {
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
		}
		break;
		
		case (uint32_t)UART4: {
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
		}
		break;

		case (uint32_t)UART5: {
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
		}
		break;
	}
	
	GPIOX_Init(me.GPIO_TX);
	GPIOX_Init(me.GPIO_RX);
    USART_Init(UART, &USART_InitStructure);	
	
    /********Enable Usart1****/
    USART_Cmd(UART, ENABLE);
    
    /*******Open IT*********/
    USART_ITConfig(UART, USART_IT_RXNE, ENABLE);
    /*******Set NVIC***********/
	USARTX_SetNVIC(me);


	USARTX_SetCallbackFunc(UART, me.CallbackFunc);
}

void USARTX_Reset(UART_TYPE me) {
	GPIO_TYPE gpio;

	gpio = me.GPIO_RX;
	gpio.GPIOInitTypeStruct.GPIO_Mode = GPIO_Mode_IPD;
	GPIOX_Init(gpio);

	gpio = me.GPIO_TX;
	gpio.GPIOInitTypeStruct.GPIO_Mode = GPIO_Mode_IPD;
	GPIOX_Init(gpio);

	
	switch ((uint32_t)me.USARTX) {
		case (uint32_t)USART1: {
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, DISABLE);
		}
		break;

		case (uint32_t)USART2: {
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, DISABLE);
		}
		break;

		case (uint32_t)USART3: {
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, DISABLE);
		}
		break;
		
		case (uint32_t)UART4: {
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, DISABLE);
		}
		break;

		case (uint32_t)UART5: {
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, DISABLE);
		}
		break;
	}
		
}


void USARTX_CreateDefaultUsart(UART_TYPE * usart) {
	memset(usart, 0, sizeof(UART_TYPE));

	usart->Open = USARTX_Open;
	usart->Close = USARTX_Colse;
	usart->Init = USARTX_Init;
	usart->Reset = USARTX_Reset;
	usart->SendChar = USARTX_SendChar;
	usart->SendString = USARTX_SendString;
	usart->SendData = USARTX_SendData;
}



void USART1_DefaultInit(UART_TYPE * usart, uint32_t baudrate) {
	USART_InitTypeDef USART_InitTypeStruct = {0};
	GPIO_TYPE gpio;

	usart->USARTX = USART1;

	USART_InitTypeStruct.USART_BaudRate = baudrate;
	USART_InitTypeStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitTypeStruct.USART_Parity = USART_Parity_No;
	USART_InitTypeStruct.USART_StopBits = USART_StopBits_1;
	USART_InitTypeStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitTypeStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	usart->USART_InitType = USART_InitTypeStruct;

	gpio.Port = GPIOA;
	gpio.GPIOInitTypeStruct.GPIO_Pin = GPIO_Pin_9;
	gpio.GPIOInitTypeStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio.GPIOInitTypeStruct.GPIO_Speed = GPIO_Speed_50MHz;
	gpio.IntValue = RESET;
	
	usart->GPIO_TX = gpio;

	gpio.Port = GPIOA;
	gpio.GPIOInitTypeStruct.GPIO_Pin = GPIO_Pin_10;
	gpio.GPIOInitTypeStruct.GPIO_Mode = GPIO_Mode_IPU;
	gpio.IntValue = RESET;
	usart->GPIO_RX = gpio;

	usart->Init(*usart);
}


void USART2_DefaultInit(UART_TYPE * usart, uint32_t baudrate) {
	USART_InitTypeDef USART_InitTypeStruct = {0};
	GPIO_TYPE gpio;


	usart->USARTX = USART2;

	USART_InitTypeStruct.USART_BaudRate = baudrate;
	USART_InitTypeStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitTypeStruct.USART_Parity = USART_Parity_No;
	USART_InitTypeStruct.USART_StopBits = USART_StopBits_1;
	USART_InitTypeStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitTypeStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	usart->USART_InitType = USART_InitTypeStruct;

	gpio.Port = GPIOA;
	gpio.GPIOInitTypeStruct.GPIO_Pin = GPIO_Pin_2;
	gpio.GPIOInitTypeStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio.GPIOInitTypeStruct.GPIO_Speed = GPIO_Speed_50MHz;
	gpio.IntValue = SET;
	usart->GPIO_TX = gpio;

	gpio.Port = GPIOA;
	gpio.GPIOInitTypeStruct.GPIO_Pin = GPIO_Pin_3;
	gpio.GPIOInitTypeStruct.GPIO_Mode = GPIO_Mode_IPU;
	gpio.IntValue = RESET;
	usart->GPIO_RX = gpio;

	usart->Init(*usart);
}


void USART3_DefaultInit(UART_TYPE * usart, uint32_t baudrate) {
	USART_InitTypeDef USART_InitTypeStruct = {0};
	GPIO_TYPE gpio;
	
	usart->USARTX = USART3;

	USART_InitTypeStruct.USART_BaudRate = baudrate;
	USART_InitTypeStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitTypeStruct.USART_Parity = USART_Parity_No;
	USART_InitTypeStruct.USART_StopBits = USART_StopBits_1;
	USART_InitTypeStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitTypeStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	usart->USART_InitType = USART_InitTypeStruct;

	gpio.Port = GPIOB;
	gpio.GPIOInitTypeStruct.GPIO_Pin = GPIO_Pin_10;
	gpio.GPIOInitTypeStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio.GPIOInitTypeStruct.GPIO_Speed = GPIO_Speed_50MHz;
	gpio.IntValue = SET;
	usart->GPIO_TX = gpio;

	gpio.Port = GPIOB;
	gpio.GPIOInitTypeStruct.GPIO_Pin = GPIO_Pin_11;
	gpio.GPIOInitTypeStruct.GPIO_Mode = GPIO_Mode_IPU;
	gpio.IntValue = RESET;
	usart->GPIO_RX = gpio;

	usart->Init(*usart);
}

void UART4_DefaultInit(UART_TYPE * usart, uint32_t baudrate) {
	USART_InitTypeDef USART_InitTypeStruct = {0};
	GPIO_TYPE gpio;
	
	usart->USARTX = UART4;

	USART_InitTypeStruct.USART_BaudRate = baudrate;
	USART_InitTypeStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitTypeStruct.USART_Parity = USART_Parity_No;
	USART_InitTypeStruct.USART_StopBits = USART_StopBits_1;
	USART_InitTypeStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitTypeStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	usart->USART_InitType = USART_InitTypeStruct;

	gpio.Port = GPIOC;
	gpio.GPIOInitTypeStruct.GPIO_Pin = GPIO_Pin_10;
	gpio.GPIOInitTypeStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio.GPIOInitTypeStruct.GPIO_Speed = GPIO_Speed_50MHz;
	gpio.IntValue = SET;
	usart->GPIO_TX = gpio;

	gpio.Port = GPIOC;
	gpio.GPIOInitTypeStruct.GPIO_Pin = GPIO_Pin_11;
	gpio.GPIOInitTypeStruct.GPIO_Mode = GPIO_Mode_IPU;
	gpio.IntValue = RESET;
	usart->GPIO_RX = gpio;

	usart->Init(*usart);
}


void UART5_DefaultInit(UART_TYPE * usart, uint32_t baudrate) {
	USART_InitTypeDef USART_InitTypeStruct = {0};
	GPIO_TYPE gpio;
	

	usart->USARTX = UART5;

	USART_InitTypeStruct.USART_BaudRate = baudrate;
	USART_InitTypeStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitTypeStruct.USART_Parity = USART_Parity_No;
	USART_InitTypeStruct.USART_StopBits = USART_StopBits_1;
	USART_InitTypeStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitTypeStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	usart->USART_InitType = USART_InitTypeStruct;

	gpio.Port = GPIOC;
	gpio.GPIOInitTypeStruct.GPIO_Pin = GPIO_Pin_12;
	gpio.GPIOInitTypeStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio.GPIOInitTypeStruct.GPIO_Speed = GPIO_Speed_50MHz;
	gpio.IntValue = SET;
	usart->GPIO_TX = gpio;

	gpio.Port = GPIOD;
	gpio.GPIOInitTypeStruct.GPIO_Pin = GPIO_Pin_2;
	gpio.GPIOInitTypeStruct.GPIO_Mode = GPIO_Mode_IPU;
	gpio.IntValue = RESET;
	usart->GPIO_RX = gpio;

	usart->Init(*usart);
}



void USART1_IRQHandler(void)
{
	char ch = 0;
	
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
		ch = USART_ReceiveData(USART1);
		
        if (Usart1_Handler != NULL) {
            (*Usart1_Handler)(ch);
        }  
    }
    
    USART_ClearFlag(USART1, USART_FLAG_RXNE);
}


void USART2_IRQHandler(void)
{
	char ch = 0;
	
	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
		ch = USART_ReceiveData(USART2);
		
		if (Usart2_Handler != NULL) {
            (*Usart2_Handler)(ch);
        }  
    }
    
    USART_ClearFlag(USART2, USART_FLAG_RXNE);
}


void USART3_IRQHandler(void)
{
	char ch = 0; 
	
    if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) {
		ch = USART_ReceiveData(USART3);
		
		if (Usart3_Handler != NULL) {
            (*Usart3_Handler)(ch);
        }  
    }
    
    USART_ClearFlag(USART3, USART_FLAG_RXNE);
}

void UART4_IRQHandler(void)
{
	char ch = 0;
	
    if (USART_GetITStatus(UART4, USART_IT_RXNE) != RESET) {
		ch = USART_ReceiveData(UART4);
		
		if (Uart4_Handler != NULL) {
            (*Uart4_Handler)(ch);
        }  
    }
    
    USART_ClearFlag(UART4, USART_FLAG_RXNE);
}


void UART5_IRQHandler(void)
{
	char ch = 0;
	
    if (USART_GetITStatus(UART5, USART_IT_RXNE) != RESET) {
		ch = USART_ReceiveData(UART5);
		
		if (Uart5_Handler != NULL) {
            (*Uart5_Handler)(ch);
        }  
    }
    
    USART_ClearFlag(UART5, USART_FLAG_RXNE);
}


// 由于使用库函数printf  所以需要重定义库函数fputc
int fputc(int ch, FILE *f)  
{
    PRINT_UARTX->DR = (u8) ch;
    /* Loop until the end of transmission */  
    while (USART_GetFlagStatus(PRINT_UARTX, USART_FLAG_TXE) == RESET) {
    }
    
    return ch;  
}


/*
-----------------------------------------------------------------------------------------------------------
*/
	

static void __delay_1us(uint16_t i)
{
    uint16_t j;
    
    for(j = 0;j < i;j++) {
        __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
    }
}
static void __delay_ms(uint16_t ms) {
    while (ms--) {
        __delay_1us(1000);
    }
}

static uint8_t u1buff[100];
static uint8_t u2buff[100];
static uint8_t u3buff[100];
static uint8_t u4buff[100];
static uint8_t u5buff[100];

static uint16_t u1i = 0;
static uint16_t u2i = 0;
static uint16_t u3i = 0;
static uint16_t u4i = 0;
static uint16_t u5i = 0;

void Usart1CallbackFunc(uint8_t ch) {
	u1buff[u1i] = ch;
	u1i++;
}

void Usart2CallbackFunc(uint8_t ch) {
	u2buff[u2i] = ch;
	u2i++;
}

void Usart3CallbackFunc(uint8_t ch) {
	u3buff[u3i] = ch;
	u3i++;
}

void Uart4CallbackFunc(uint8_t ch) {
	u4buff[u4i] = ch;
	u4i++;
}

void Uart5CallbackFunc(uint8_t ch) {
	u5buff[u5i] = ch;
	u5i++;
}
void UartTestDemo(void) {
	int i = 0;
	UART_TYPE USART1_TYPE;
	UART_TYPE USART2_TYPE;
	UART_TYPE USART3_TYPE;
	UART_TYPE UART4_TYPE;
	UART_TYPE UART5_TYPE;

    // 创建默认结构体
	USARTX_CreateDefaultUsart(&USART1_TYPE);
	USARTX_CreateDefaultUsart(&USART2_TYPE);
	USARTX_CreateDefaultUsart(&USART3_TYPE);
	USARTX_CreateDefaultUsart(&UART4_TYPE);
	USARTX_CreateDefaultUsart(&UART5_TYPE);


    // 配置回调函数
	USART1_TYPE.CallbackFunc = Usart1CallbackFunc;
	USART1_DefaultInit(&USART1_TYPE, 115200);
	USART1_TYPE.Init(USART1_TYPE);
    
	USART2_TYPE.CallbackFunc = Usart2CallbackFunc;
	USART2_DefaultInit(&USART2_TYPE, 115200);
	USART1_TYPE.Init(USART2_TYPE);

	USART3_TYPE.CallbackFunc = Usart3CallbackFunc;
	USART3_DefaultInit(&USART3_TYPE, 115200);
	USART1_TYPE.Init(USART3_TYPE);

	UART4_TYPE.CallbackFunc = Uart4CallbackFunc;
	UART4_DefaultInit(&UART4_TYPE, 115200);
	USART1_TYPE.Init(UART4_TYPE);

	UART5_TYPE.CallbackFunc = Uart5CallbackFunc;
	UART5_DefaultInit(&UART5_TYPE, 115200);
	USART1_TYPE.Init(UART5_TYPE);


    // 测试发送字符
	USART1_TYPE.SendChar(USART1_TYPE.USARTX , '1');
	USART2_TYPE.SendChar(USART2_TYPE.USARTX, '2');
	USART3_TYPE.SendChar(USART3_TYPE.USARTX, '3');
	UART4_TYPE.SendChar(UART4_TYPE.USARTX, '4');
	UART5_TYPE.SendChar(UART5_TYPE.USARTX, '5');


    // 测试发送字符串
	USART1_TYPE.SendString(USART1_TYPE.USARTX, "usart1 string test\r\n");
	USART2_TYPE.SendString(USART2_TYPE.USARTX, "usart2 string test\r\n");
	USART3_TYPE.SendString(USART3_TYPE.USARTX, "usart3 string test\r\n");
	UART4_TYPE.SendString(UART4_TYPE.USARTX, "uart4 string test\r\n");
	UART5_TYPE.SendString(UART5_TYPE.USARTX, "uart5 string test\r\n");
    
    
    // 测试发送数组
	USART1_TYPE.SendData(USART1_TYPE.USARTX, "usart1 data test\r\n", strlen("usart1 data test\r\n"));
	USART2_TYPE.SendData(USART2_TYPE.USARTX, "usart2 data test\r\n", strlen("usart2 data test\r\n"));
	USART3_TYPE.SendData(USART3_TYPE.USARTX, "usart3 data test\r\n", strlen("usart3 data test\r\n"));
	UART4_TYPE.SendData(UART4_TYPE.USARTX, "uart4 data test\r\n", strlen("uart4 data test\r\n"));
	UART5_TYPE.SendData(UART5_TYPE.USARTX, "uart5 data test\r\n", strlen("uart5 data test\r\n"));
    

	while (i < 3) {
		if (u1i) {
            // 发送数据到串口1
			USART1_TYPE.SendData(USART1_TYPE.USARTX, u1buff, u1i);
			u1i = 0;
			i++;
		}

		
		if (u2i) {
            // 发送数据到串口2
			USART2_TYPE.SendData(USART2_TYPE.USARTX, u2buff, u2i);
			u2i = 0;
			i++;
		}

		
		if (u3i) {
            // 发送数据到串口3
			USART3_TYPE.SendData(USART3_TYPE.USARTX, u3buff, u3i);
			u3i = 0;
			i++;
		}

		if (u4i) {
            // 发送数据到串口4
			UART4_TYPE.SendData(UART4_TYPE.USARTX, u4buff, u4i);
			u4i = 0;
			i++;
		}

		
		if (u5i) {
            // 发送数据到串口5
			UART5_TYPE.SendData(UART5_TYPE.USARTX, u5buff, u5i);
			u5i = 0;
			i++;
		}
        
        __delay_ms(100);
	}
    
    printf("lib printf function：const string test.\r\n");
    printf("lib printf function: int test.  3=%d\r\n", 3);
    printf("lib printf function: hex test.  3=%x\r\n", 3);
    printf("lib printf function: oct test.  3=%o\r\n", 3);
    printf("lib printf function: float test.  3.0=%f\r\n", 3.0);
    printf("lib printf function: string test.  string=%s\r\n", "string");
}


/*
-----------------------------------------------------------------------------------------------------------
*/



/*************************END**************************/

