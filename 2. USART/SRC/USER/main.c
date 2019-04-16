// 系统头文件
#include "stdio.h"
#include "usart/uart.h"
#include "stm32f10x.h"

// 驱动头文件
#include "gpio/gpio.h"


int main(void) {
    UartTestDemo();
    
    // 开启循环
    while (1) ;
    
    return 0;
}
