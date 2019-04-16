#include "gpio.h"

/*******************************************************************************
 * Function Name  : GPIO_Init
 * Description    : GPIO初始化
 * Input          : GPIO_Pin(GPIO_Pin_0-GPIO_Pin_15,GPIO_Pin_All) 
                    GPIO_Mode(GPIO_Mode_AIN,
                              GPIO_Mode_IN_FLOATING,
                              GPIO_Mode_IPD,
                              GPIO_Mode_IPU,
                              GPIO_Mode_Out_OD,
                              GPIO_Mode_Out_PP,
                              GPIO_Mode_AF_OD,
                              GPIO_Mode_AF_PP)
                    Init_Value(true, false)
 * Output         : None
 * Return         : uint8_t
 *******************************************************************************/
uint8_t GPIOX_Init(GPIO_TYPE gpio)
{
	GPIO_TypeDef *Port = gpio.Port;
    GPIO_InitTypeDef GPIO_InitStructure = gpio.GPIOInitTypeStruct;
	GPIOMode_TypeDef GPIO_Mode = GPIO_InitStructure.GPIO_Mode;
	uint16_t GPIO_Pin = GPIO_InitStructure.GPIO_Pin;
	uint8_t Init_Value = gpio.IntValue;
	
    if (Port == GPIOA) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    } else if (Port == GPIOB){
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    } else if (Port == GPIOC){
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    } else if (Port == GPIOD){
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    } else if (Port == GPIOE){
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
    } else if (Port == GPIOF){
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
    } else if (Port == GPIOG){
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
    } else {
       return 1; 
    }

	GPIO_Init(Port, &GPIO_InitStructure);
	
    if ((GPIO_Mode == GPIO_Mode_AIN) || 
        (GPIO_Mode == GPIO_Mode_IN_FLOATING) || 
        (GPIO_Mode == GPIO_Mode_IPD) || 
        (GPIO_Mode == GPIO_Mode_IPU)) {
            return 0;
    }
    if (Init_Value) {
        GPIO_SetBits(Port, GPIO_Pin);
    } else {
        GPIO_ResetBits(Port, GPIO_Pin);
    }
    return 0;
}
/*******************************************************************************
 * Function Name  : GPIOX_GetPinBit
 * Description    : 获取GPIO的电平值
 * Input          : gpio(GPIO_TYPE: 其中包含GPIO_PORT和GPIO_PIN) 
 * Output         : None
 * Return         : uint8_t
 *******************************************************************************/
uint8_t GPIOX_GetPinBit(GPIO_TYPE gpio) {
	GPIO_TypeDef * Port = gpio.Port;
	GPIOMode_TypeDef GPIO_Mode = gpio.GPIOInitTypeStruct.GPIO_Mode;
	uint16_t GPIO_Pin = gpio.GPIOInitTypeStruct.GPIO_Pin;
	
	if ((GPIO_Mode == GPIO_Mode_AIN) || 
        (GPIO_Mode == GPIO_Mode_IN_FLOATING) || 
        (GPIO_Mode == GPIO_Mode_IPD) || 
        (GPIO_Mode == GPIO_Mode_IPU)) {
		return GPIO_ReadInputDataBit(Port, GPIO_Pin);
	} else {
		return GPIO_ReadOutputDataBit(Port, GPIO_Pin);
	}
}

/*******************************************************************************
 * Function Name  : GPIOX_SetPinBit
 * Description    : 设置GPIO的电平值
 * Input          : gpio(GPIO_TYPE: 其中包含GPIO_PORT和GPIO_PIN) 
                    status(1, 0)
 * Output         : None
 * Return         : None
 *******************************************************************************/
void GPIOX_SetPinBit(GPIO_TYPE gpio, uint8_t status) {
	GPIO_TypeDef * Port = gpio.Port;
	GPIOMode_TypeDef GPIO_Mode = gpio.GPIOInitTypeStruct.GPIO_Mode;
	uint16_t GPIO_Pin = gpio.GPIOInitTypeStruct.GPIO_Pin;
	
    // 如果不是输入模式
	if ((GPIO_Mode == GPIO_Mode_Out_OD) || 
        (GPIO_Mode == GPIO_Mode_Out_PP) || 
        (GPIO_Mode == GPIO_Mode_AF_OD) || 
        (GPIO_Mode == GPIO_Mode_AF_PP)) {
		GPIO_WriteBit(Port, GPIO_Pin, status);
	}
}


/*******************************************************************************
 * Function Name  : GPIOX_TogglePinBit
 * Description    : 切换GPIO的电平值
 * Input          : gpio(GPIO_TYPE: 其中包含GPIO_PORT和GPIO_PIN)
 * Output         : None
 * Return         : None
 *******************************************************************************/
void GPIOX_TogglePinBit(GPIO_TYPE gpio) {
    GPIOX_SetPinBit(gpio, !GPIOX_GetPinBit(gpio));
}

/*
---------------------------------------------------
*/

static GPIO_TYPE GPIOArray[] = {
	{GPIOD, {GPIO_Pin_13, GPIO_Speed_50MHz, GPIO_Mode_Out_PP}, RESET},
   // {GPIOG, {GPIO_Pin_14, GPIO_Speed_50MHz, GPIO_Mode_Out_PP}, SET},
};
void delay_1us(uint16_t i)
{
    uint16_t j, k;
    
    for(j = 0;j < i;j++) {
        __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
    }
}
void delay_ms(uint16_t ms) {
    while (ms--) {
        delay_1us(1000);
    }
}

void GPIOTestDemo(void) {
	uint8_t i;

    // 初始化
    for (i = 0; i < sizeof(GPIOArray)/sizeof(GPIO_TYPE); i++) {
        GPIOX_Init(GPIOArray[i]);
    }
    
    while (1) {
        for (i = 0; i < sizeof(GPIOArray)/sizeof(GPIO_TYPE); i++) {
            // 切换gpio
            GPIOX_TogglePinBit(GPIOArray[i]);
            delay_ms(500);
        }
    }
}

/*
--------------------------------------------------
*/

