#ifndef __TIMER_H__
#define __TIMER_H__

#include "stm32f10x.h"

typedef struct __TIMER_TYPE {
	TIM_TypeDef * TIMER;
	TIM_TimeBaseInitTypeDef TIMER_InitStruct;
	NVIC_InitTypeDef NVICInitStruct;
	uint32_t    us;
	uint32_t    value;
	uint32_t    Period;
	uint32_t    Prescaler;
	void 	   (*TimerIRQHandler)(void);


	void       (*Init)(struct __TIMER_TYPE *me);
	void       (*Open)(struct __TIMER_TYPE *me);
	void       (*Close)(struct __TIMER_TYPE *me);
	void       (*Write)(struct __TIMER_TYPE *me, uint32_t Period, uint32_t Prescaler);
	void       (*WriteMs)(struct __TIMER_TYPE *me, uint32_t ms);
	uint32_t   (*ReadCount)(struct __TIMER_TYPE *me);
	uint32_t   (*ReadMs)(struct __TIMER_TYPE *me);
	void       (*ChangePeriod)(struct __TIMER_TYPE *me, uint32_t Period);
	void       (*ChangePrescaler)(struct __TIMER_TYPE *me, uint32_t Prescaler);
	void       (*SetIRQHandler)(struct __TIMER_TYPE *me, void(*)(void));
} TIMER_TYPE;


void Timer_Demo(void);
void Timer_CreateDefaultTIMER(TIMER_TYPE *me);


#endif
/*********************************** END **************************************/


