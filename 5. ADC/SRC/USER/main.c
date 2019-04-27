// ϵͳͷ�ļ�
#include "stdio.h"

// ����ͷ�ļ�
#include "gpio/gpio.h"
#include "usart/uart.h"
#include "I2C/i2c.h"

void assert_failed(uint8_t *file, uint32_t line) {
	printf("[%s]:%u, error\r\n", file, line);
	while (1);
}


int main(void) {
	UART_TYPE USART1_TYPE;
	USARTX_CreateDefaultUsart(&USART1_TYPE);
	USART1_DefaultInit(&USART1_TYPE, 115200);
    
    printf("init\r\n");
    ADC_Demo();
	
    // ����ѭ��
    while (1) ;
    
    return 0;
}