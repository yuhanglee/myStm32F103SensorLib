#include "beep/beep.h"
#include "Timer/Timer.h"


static void Beep_Init(BEEP_TYPE *me)
{
	me->gpio.GPIOInitTypeStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	me->gpio.GPIOInitTypeStruct.GPIO_Speed = GPIO_Speed_50MHz;
	me->gpio.IntValue = 1-(me->highLevel);
	GPIOX_Init(me->gpio);
}

static void Beep_On(BEEP_TYPE *me)
{
	GPIOX_SetPinBit(me->gpio, me->highLevel != RESET);
}


static void Beep_Off(BEEP_TYPE *me)
{
	GPIOX_SetPinBit(me->gpio, me->highLevel == RESET);
}

static void Beep_Write(BEEP_TYPE *me, uint8_t status) {
	if (status != RESET) {
		me->On(me);
	} else {
		me->Off(me);
	}
}

static uint8_t Beep_Read(BEEP_TYPE *me) {
	uint8_t level = GPIOX_GetPinBit(me->gpio);
	return  level == me->highLevel;
}



void BEEP_CreateDefaultFuncBEEP(BEEP_TYPE *me) {
	me->Init = Beep_Init;
	me->Write = Beep_Write;
	me->Read = Beep_Read;
	me->On = Beep_On;
	me->Off = Beep_Off;
}



static BEEP_TYPE beep = {0};
void TimerHandler(void) {
	beep.Write(&beep, 1 - beep.Read(&beep));
}
void BEEP_Demo(void) {
	TIMER_TYPE timer = {0};

	timer.TIMER = TIM2;

	Timer_CreateDefaultTIMER(&timer);

	timer.Init(&timer);

	timer.WriteMs(&timer, 1000);
	timer.SetIRQHandler(&timer, TimerHandler);


	BEEP_CreateDefaultFuncBEEP(&beep);

	beep.gpio.Port = GPIOB;
	beep.gpio.GPIOInitTypeStruct.GPIO_Pin = GPIO_Pin_8;
	beep.highLevel = SET;
	beep.Init(&beep);



	timer.Open(&timer);

	while (1);
}




/*************************END**************************/

