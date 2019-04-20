#ifndef __24CXX_H__
#define __24CXX_H__

#include "I2C/i2c.h"
#include "usart/uart.h"
#include "config.h"

typedef enum {
	AT24C01 = 7,
	AT24C02,
	AT24C04,
	AT24C08,
	AT24C16,
	AT24C32,
	AT24C64,
	AT24C128,
	AT24C256,
} AT24CXX_TYPE_ENUM;

#define AT24CXX_GET_BYTE_SIZE(type)		(GET_BIT_UINT_MAX_NUM(type))

typedef struct __AT24CXX_TYPE {
	IIC_TYPE 		IIC;
	uint8_t 		adddress;
	uint8_t 		checkAddress;
	uint8_t         checkValue;
	AT24CXX_TYPE_ENUM    type;
	GPIO_TYPE		GPIO_SCL;
	GPIO_TYPE		GPIO_SDA;
	GPIO_TYPE		GPIO_WP;

	
	void 			(*Init)(struct __AT24CXX_TYPE *me);
	void 			(*Open)(struct __AT24CXX_TYPE *me);
	void 			(*Close)(struct __AT24CXX_TYPE *me);
	uint8_t         (*CheckChip)(struct __AT24CXX_TYPE *me);
	uint32_t		(*WriteOneByte)(struct __AT24CXX_TYPE *me, uint32_t addr, uint8_t _byte);
	uint32_t		(*WriteOneWord)(struct __AT24CXX_TYPE *me, uint32_t addr, uint16_t _word);
	uint32_t		(*WriteOneInt)(struct __AT24CXX_TYPE *me, uint32_t addr, uint32_t _intager);
	uint32_t		(*ReadOneByte)(struct __AT24CXX_TYPE *me, uint32_t addr, uint8_t *_byte);
	uint32_t		(*ReadOneWord)(struct __AT24CXX_TYPE *me, uint32_t addr, uint16_t *_word);
	uint32_t		(*ReadOneInt)(struct __AT24CXX_TYPE *me, uint32_t addr, uint32_t *_intager);
	uint32_t		(*WriteMultiByte)(struct __AT24CXX_TYPE *me, uint32_t addr, uint8_t *_byte, uint32_t len);
	uint32_t		(*WriteMultiWord)(struct __AT24CXX_TYPE *me, uint32_t addr, uint16_t *_word, uint32_t len);
	uint32_t		(*WriteMultiInt)(struct __AT24CXX_TYPE *me, uint32_t addr, uint32_t *_intager, uint32_t len);
	uint32_t		(*ReadMultiByte)(struct __AT24CXX_TYPE *me, uint32_t addr, uint8_t *_byte, uint32_t len);
	uint32_t		(*ReadMultiWord)(struct __AT24CXX_TYPE *me, uint32_t addr, uint16_t *_word, uint32_t len);
	uint32_t		(*ReadMultiInt)(struct __AT24CXX_TYPE *me, uint32_t addr, uint32_t *_intager, uint32_t len);
} AT24CXX_TYPE;
/*
#define AT24C01		127
#define AT24C02		255
#define AT24C04		511
#define AT24C08		1023
#define AT24C16		2047
#define AT24C32		4095
#define AT24C64	    8191
#define AT24C128	16383
#define AT24C256	32767  

//Mini STM32开发板使用的是24c02，所以定义EE_TYPE为AT24C02
#define EE_TYPE 	AT24C64
					  
u8 AT24CXX_ReadOneByte(u16 ReadAddr);							//指定地址读取一个字节
void AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite);		//指定地址写入一个字节
void AT24CXX_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite);	//从指定地址开始写入指定长度的数据
void AT24CXX_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead);   	//从指定地址开始读出指定长度的数据

u8 AT24CXX_Check(void);  //检查器件
void AT24CXX_Init(void); //初始化IIC
*/
void AT24CXXTestDemo(void);

#endif
















