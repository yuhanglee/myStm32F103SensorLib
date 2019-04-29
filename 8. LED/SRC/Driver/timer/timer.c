#include "timer/timer.h"
#include "gpio/gpio.h"
#include "string.h"
#include "config.h"

static void (*Timer2_IRQHandler)(void) = NULL;
static void (*Timer3_IRQHandler)(void) = NULL;
static void (*Timer4_IRQHandler)(void) = NULL;
static void (*Timer5_IRQHandler)(void) = NULL;
static void (*Timer6_IRQHandler)(void) = NULL;
static void (*Timer7_IRQHandler)(void) = NULL;

static TIMER_TYPE __timer_type_init = {0};
static void Timer_Init(TIMER_TYPE *me) {
	NVIC_InitTypeDef NVICInitStruct;
	TIM_TimeBaseInitTypeDef TIMBase;
	uint32_t RCC_Periph = 0;
	TIM_TypeDef * timer;
	IRQn_Type TIMER_IRQn;

	timer = me->TIMER;

	switch ((uint32_t)timer) {
		case (uint32_t)TIM2: {
			RCC_Periph = RCC_APB1Periph_TIM2;
			TIMER_IRQn = TIM2_IRQn;
			Timer2_IRQHandler = me->TimerIRQHandler;
		}
		break;

		case (uint32_t)TIM3: {
			RCC_Periph = RCC_APB1Periph_TIM3;
			TIMER_IRQn = TIM3_IRQn;
			Timer3_IRQHandler = me->TimerIRQHandler;
		}
		break;


		case (uint32_t)TIM4: {
			RCC_Periph = RCC_APB1Periph_TIM4;
			TIMER_IRQn = TIM4_IRQn;
			Timer4_IRQHandler = me->TimerIRQHandler;
		}
		break;
		
		case (uint32_t)TIM5: {
			RCC_Periph = RCC_APB1Periph_TIM5;
			TIMER_IRQn = TIM5_IRQn;
			Timer5_IRQHandler = me->TimerIRQHandler;
		}
		break;
		
		case (uint32_t)TIM6: {
			RCC_Periph = RCC_APB1Periph_TIM6;
			TIMER_IRQn = TIM6_IRQn;
			Timer6_IRQHandler = me->TimerIRQHandler;
		}
		break;

		case (uint32_t)TIM7: {
			RCC_Periph = RCC_APB1Periph_TIM7;
			TIMER_IRQn = TIM7_IRQn;
			Timer7_IRQHandler = me->TimerIRQHandler;
		}
		break;
	}

	TIM_DeInit(timer);

	RCC_APB1PeriphClockCmd(RCC_Periph, ENABLE);
	
	if ((me->TIMER_InitStruct.TIM_Period == 0) && 
		(me->TIMER_InitStruct.TIM_Prescaler == 0)) {
		TIMBase.TIM_CounterMode = TIM_CounterMode_Up;
		TIMBase.TIM_ClockDivision = TIM_CKD_DIV1;
		TIMBase.TIM_Period = me->Period - 1;
		TIMBase.TIM_Prescaler = me->Prescaler - 1;
		me->TIMER_InitStruct = TIMBase;
	} else {
		TIMBase = me->TIMER_InitStruct;
	}

	TIM_TimeBaseInit(timer, &TIMBase);

	NVICInitStruct.NVIC_IRQChannel = TIMER_IRQn;
	NVICInitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVICInitStruct.NVIC_IRQChannelSubPriority = 10;
	NVICInitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVICInitStruct);
	
    TIM_SetCounter(timer, 0);
    
	TIM_ClearFlag(timer,TIM_FLAG_Update);
    TIM_ITConfig(timer, TIM_IT_Update, ENABLE);
	TIM_Cmd(timer, DISABLE);
}


static void Timer_Open(TIMER_TYPE *me) {
	TIM_ClearFlag(me->TIMER,TIM_FLAG_Update);
	TIM_Cmd(me->TIMER, ENABLE);
}

static void Timer_Close(TIMER_TYPE *me) {
	TIM_ClearFlag(me->TIMER, TIM_FLAG_Update);
	TIM_Cmd(me->TIMER, DISABLE);
}

static uint32_t Timer_ReadCount(TIMER_TYPE *me) {
	return TIM_GetCounter(me->TIMER);
}

static uint32_t Timer_ReadMs(TIMER_TYPE *me) {
	return me->ReadCount(me) / 2;
}


static void Timer_Write(TIMER_TYPE *me, uint32_t Period, uint32_t Prescaler) {
	me->TIMER_InitStruct.TIM_Period = Period - 1;
	me->TIMER_InitStruct.TIM_Prescaler = Prescaler - 1;
	TIM_TimeBaseInit(me->TIMER, &(me->TIMER_InitStruct));
}

static void TImer_WriteMs(TIMER_TYPE *me, uint32_t ms) {
	me->Write(me, ms * 2, SystemCoreClock / 2000);	
}

static void Timer_ChangePeriod(TIMER_TYPE *me, uint32_t Period) {
	me->TIMER_InitStruct.TIM_Period = Period - 1;
	TIM_TimeBaseInit(me->TIMER, &(me->TIMER_InitStruct));
}

static void Timer_ChangePrescaler(TIMER_TYPE *me, uint32_t Prescaler) {
	me->TIMER_InitStruct.TIM_Prescaler = Prescaler - 1;
	TIM_TimeBaseInit(me->TIMER, &(me->TIMER_InitStruct));
}

static void Timer_SetIRQHandler(TIMER_TYPE *me, void (*IRQHandler)(void)) {
	TIM_TypeDef * timer;

	timer = me->TIMER;

	switch ((uint32_t)timer) {
		case (uint32_t)TIM2: {
			Timer2_IRQHandler = IRQHandler;
		}
		break;

		case (uint32_t)TIM3: {
			Timer3_IRQHandler = IRQHandler;
		}
		break;


		case (uint32_t)TIM4: {
			Timer4_IRQHandler = IRQHandler;
		}
		break;
		
		case (uint32_t)TIM5: {
			Timer5_IRQHandler = IRQHandler;
		}
		break;
		
		case (uint32_t)TIM6: {
			Timer6_IRQHandler = IRQHandler;
		}
		break;

		case (uint32_t)TIM7: {
			Timer7_IRQHandler = IRQHandler;
		}
		break;
	}
}


void Timer_CreateDefaultTIMER(TIMER_TYPE *me) {
	me->Init = Timer_Init;
	me->Open = Timer_Open;
	me->Close = Timer_Close;
	me->ReadCount = Timer_ReadCount;
	me->ReadMs = Timer_ReadMs;
	me->WriteMs = TImer_WriteMs;
	me->Write = Timer_Write;
	me->ChangePeriod = Timer_ChangePeriod;
	me->ChangePrescaler = Timer_ChangePrescaler;
	me->SetIRQHandler = Timer_SetIRQHandler;
}


void TIM2_IRQHandler(void)                                          
{       
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {  // interrupt handler
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        if (Timer2_IRQHandler != NULL) {
            (*Timer2_IRQHandler)();
        }        
    }  
}

void TIM3_IRQHandler(void)                                          
{   
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {  // interrupt handler
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
        
        if (Timer3_IRQHandler != NULL) {
            (*Timer3_IRQHandler)();
        }        
    }  
}

void TIM4_IRQHandler(void)                                          
{   
    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) {  // interrupt handler
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
        
        if (Timer4_IRQHandler != NULL) {
            (*Timer4_IRQHandler)();
        }        
    }  
}

void TIM5_IRQHandler(void)                                          
{   
    if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET) {  // interrupt handler
        TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
        
        if (Timer5_IRQHandler != NULL) {
            (*Timer5_IRQHandler)();
        }        
    }  
}

void TIM6_IRQHandler(void)                                          
{   
    if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET) {  // interrupt handler
        TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
        
        if (Timer6_IRQHandler != NULL) {
            (*Timer6_IRQHandler)();
        }        
    }  
}

void TIM7_IRQHandler(void)                                          
{   
    if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET) {  // interrupt handler
        TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
        
        if (Timer7_IRQHandler != NULL) {
            (*Timer7_IRQHandler)();
        }        
    }  
}

GPIO_TYPE gpio = {0};
static void testLed(void) {

	gpio.Port = GPIOB;
	gpio.GPIOInitTypeStruct.GPIO_Pin = GPIO_Pin_5;
	gpio.GPIOInitTypeStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio.GPIOInitTypeStruct.GPIO_Speed = GPIO_Speed_50MHz;

	GPIOX_Init(gpio);
}


static void testIRQ(void) {
	GPIOX_TogglePinBit(gpio);
}


void Timer_Demo(void) {
	TIMER_TYPE timer = {0};
	int i = 0;

	testLed();

	timer.TIMER = TIM7;
	timer.TimerIRQHandler = testIRQ;
	timer.Period = 1000;
	timer.Prescaler = 9000;
	
	Timer_CreateDefaultTIMER(&timer);

	timer.Init(&timer);
	timer.Open(&timer);


	while (1);
}

