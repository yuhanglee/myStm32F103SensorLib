// 系统头文件
#include "stdio.h"

// 驱动头文件
#include "gpio/gpio.h"
#include "usart/uart.h"
#include "dac/dac.h"

void assert_failed(uint8_t *file, uint32_t line) {
	printf("[%s]:%u, error\r\n", file, line);
	while (1);
}


int main(void) {
	UART_TYPE USART1_TYPE;
	USARTX_CreateDefaultUsart(&USART1_TYPE);
	USART1_DefaultInit(&USART1_TYPE, 115200);
    
    DAC_Demo();
    
    // 开启循环
    while (1) ;
    
    return 0;
}
