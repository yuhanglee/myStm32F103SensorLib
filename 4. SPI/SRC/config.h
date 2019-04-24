#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "stm32f10x.h"
#include "USER/BitOper.h"

/*
 *	调试串口
 * USARTX_1
 * USARTX_2
 * USARTX_3
 * USARTX_4
 * USARTX_5
 */
#if !((defined USARTX_1) || (defined USARTX_2) || (defined USARTX_3) || (defined USARTX_4) || (defined USARTX_5)) // 没有定义调试串口
	#define USARTX_1
#endif

// 获取RCC时钟的地址
#define RCC_GET_APB1Periph(periph)				(1 << (((periph) - APB1PERIPH_BASE)/0x400))		
#define RCC_GET_APB2Periph(periph)				(1 << (((periph) - APB2PERIPH_BASE)/0x400))		

// 定义16进制数据
typedef union {
	uint16_t All;
	struct {
		uint16_t Bit0:1;
		uint16_t Bit1:1;
		uint16_t Bit2:1;
		uint16_t Bit3:1;
		uint16_t Bit4:1;
		uint16_t Bit5:1;
		uint16_t Bit6:1;
		uint16_t Bit7:1;
		uint16_t Bit8:1;
		uint16_t Bit9:1;
		uint16_t Bit10:1;
		uint16_t Bit11:1;
		uint16_t Bit12:1;
		uint16_t Bit13:1;
		uint16_t Bit14:1;
		uint16_t Bit15:1;
	} BitStatus;
} u16_union;



#endif

