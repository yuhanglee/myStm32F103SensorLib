#include "led.h"
#include "gpio/gpio.h"
#include "Timer/Timer.h"

static uint16_t g_led_id;


static LED_TYPE Leds[LED_MAX];
void LED_TimerInit(void);

void LED_SetLight(LED_TYPE *me, FlagStatus light);


void Led_IRQhandle(void) {
	int i = 0;
	
	for (i = 0;i < LED_MAX;i++) {
		// 初始化完成
		if (Leds[i].id != 0x00) {
			Leds[i].Count++;
			
			if (Leds[i].Count >= Leds[i].Period) {
				Leds[i].Count = 0;
			}
			
			if (Leds[i].Count < Leds[i].Light) {
				Leds[i].SetLight(&(Leds[i]), SET);
			} else {
				Leds[i].SetLight(&(Leds[i]), RESET);
			}
		}
	}
}
void LED_TimerInit(void) {
	TIMER_TYPE timer = {0};

	timer.TIMER = TIM2;

	Timer_CreateDefaultTIMER(&timer);

	
	timer.Init(&timer);
	timer.SetIRQHandler(&timer, Led_IRQhandle);
	timer.WriteMs(&timer, 10);
	timer.Open(&timer);
}


/*******************************************************************************
 * Function Name  : Led_Init
 * Description    : This function Init LED.
 * Input          : Led_num(0: heartheat led)
                    light(true, false)         
 * Output         : bool
 * Return         : true, false
 *******************************************************************************/
uint16_t Led_Init(LED_TYPE *me) {
	GPIO_TYPE *gpio = &(me->gpio);

	gpio->GPIOInitTypeStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio->GPIOInitTypeStruct.GPIO_Speed = GPIO_Speed_50MHz;

	GPIOX_Init(*gpio);
	g_led_id++;
	me->id = g_led_id;
	return g_led_id;
}

/*******************************************************************************
 * Function Name  : LED_SetLight
 * Description    : This function set Led port.
 * Input          : Led_num(0: heartheat led)
                    light(true, false)         
 * Output         : bool
 * Return         : true, false
 *******************************************************************************/
void LED_SetLight(LED_TYPE *me, FlagStatus light) {
	GPIO_TypeDef * port = me->gpio.Port;
	uint16_t gpio_pin = me->gpio.GPIOInitTypeStruct.GPIO_Pin;
	if (light == RESET) { // 灭
		GPIO_WriteBit(port, gpio_pin, (BitAction)(me->HighLevel == RESET));
	} else { // 亮
		GPIO_WriteBit(port, gpio_pin, (BitAction)(me->HighLevel == SET));
	}
	
}

/*******************************************************************************
 * Function Name  : LED_SetLight
 * Description    : This function get Led port.
 * Input          : Led_num(0: heartheat led)      
 * Output         : bool
 * Return         : true, false
 *******************************************************************************/
uint8_t LED_GetLight(LED_TYPE *me) {
	uint8_t gpioBitValue = GPIOX_GetPinBit(me->gpio);

	return gpioBitValue == me->HighLevel;
}

void LED_SetPeriod(LED_TYPE *me,int light, int period) {
	me->Light = light;
	me->Period = period;
	Leds[me->id - 1] = *me;
}


void LED_CreateDefaultInit(LED_TYPE *me) {
	// 开启定时器
	LED_TimerInit();

	
	me->Period = 0;
	me->Count = 0;
	me->Light = 0;

	me->Init = Led_Init;
	me->SetLight = LED_SetLight;
	me->GetLight = LED_GetLight;
	me->SetPeriod = LED_SetPeriod;
}




void LED_Demo(void) {
	LED_TYPE led = {0};

	LED_CreateDefaultInit(&led);

	led.gpio.Port = GPIOE;
	led.gpio.GPIOInitTypeStruct.GPIO_Pin = GPIO_Pin_5;
	led.gpio.IntValue = RESET;

	// 设置LED亮的电平状态
	led.HighLevel = RESET;
	
	led.Init(&led);
	// 20*10ms 亮  80*10ms灭
	led.SetPeriod(&led, 20, 100);
	
	while (1);	
}

