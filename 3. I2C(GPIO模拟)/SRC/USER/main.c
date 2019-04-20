// 系统头文件
#include "stdio.h"
#include "stm32f10x.h"


// 驱动头文件
#include "24cxx/24cxx.h"
#include "usart/uart.h"

void assert_failed(uint8_t *file, uint32_t line) {
	printf("[%s]:%u, error\r\n", file, line);
	while (1);
}


int main(void) {
	UART_TYPE USART1_TYPE;
	USARTX_CreateDefaultUsart(&USART1_TYPE);
	USART1_DefaultInit(&USART1_TYPE, 115200);
    
    AT24CXXTestDemo();
    
    
    // 开启循环
    while (1) ;
    
    return 0;
}
