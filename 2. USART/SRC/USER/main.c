// ϵͳͷ�ļ�
#include "stdio.h"
#include "usart/uart.h"
#include "stm32f10x.h"

// ����ͷ�ļ�
#include "gpio/gpio.h"


int main(void) {
    UartTestDemo();
    
    // ����ѭ��
    while (1) ;
    
    return 0;
}
