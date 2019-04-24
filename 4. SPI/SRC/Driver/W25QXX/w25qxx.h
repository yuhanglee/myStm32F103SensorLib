#ifndef __W25QXX_H__
#define __W25QXX_H__

#include "SPI/spi.h"
#include "stm32f10x.h"

//#define W25QXX_1Mb_UNIT		(1024*1024/8)
//#define W25Q16				(16*W25QXX_1Mb_UNIT)
//#define W25Q32				(32*W25QXX_1Mb_UNIT)
//#define W25Q64				(64*W25QXX_1Mb_UNIT)
//#define W25Q128				(128*W25QXX_1Mb_UNIT)
//#define W25Q256				(256*W25QXX_1Mb_UNIT)

//#define W25QXX_TYPE			W25Q64
//#define W25QXX_SIZE			(W25QXX_TYPE)	// BYTE 字节数

typedef enum {
	W25Q80 = 0XEF13,
	W25Q16 = 0XEF14,
	W25Q32 = 0XEF15,
	W25Q64 = 0XEF16,
} W25QXX_TYPE_ENUM;

typedef struct __W25QXX_TYPE {
	SPI_TYPE SPI;
	GPIO_TYPE GPIO_CS;
	GPIO_TYPE GPIO_CLK;
	GPIO_TYPE GPIO_MISO;
	GPIO_TYPE GPIO_MOSI;
    uint16_t SPI_CPOL;
    uint16_t SPI_CPHA;
    uint8_t SPI_MODE;
	W25QXX_TYPE_ENUM type;

	void 			(*Init)(struct __W25QXX_TYPE *me);
	void			(*Open)(struct __W25QXX_TYPE *me);
	void 			(*Close)(struct __W25QXX_TYPE *me);
	uint16_t 		(*ReadID)(struct __W25QXX_TYPE *me);
	uint8_t   		(*ReadSR)(struct __W25QXX_TYPE *me);
	void 			(*WriteSR)(struct __W25QXX_TYPE *me, uint8_t SR);
	void 			(*NoCheck)(struct __W25QXX_TYPE *me, uint8_t *pBuf, uint32_t addr, uint16_t len);
	void 			(*Read)(struct __W25QXX_TYPE *me, uint8_t *pBuf, uint32_t addr, uint16_t len);
	void			(*Write)(struct __W25QXX_TYPE *me, uint8_t *PBuf, uint32_t addr, uint16_t len);
	void 			(*EraseChip)(struct __W25QXX_TYPE *me);
	void 			(*EraseSector)(struct __W25QXX_TYPE *me, uint32_t sectorAddr);
	void 			(*WaitBusy)(struct __W25QXX_TYPE *me);
	void 			(*PowerDown)(struct __W25QXX_TYPE *me);
	void 			(*WakeUp)(struct __W25QXX_TYPE *me);
} W25QXX_TYPE;

void W25QXX_Demo(void);
/*

void SPI_Flash_Init(void);
u16  SPI_Flash_ReadID(void);  	    //读取FLASH ID
u8	 SPI_Flash_ReadSR(void);        //读取状态寄存器 
void SPI_FLASH_Write_SR(u8 sr);  	//写状态寄存器
void SPI_FLASH_Write_Enable(void);  //写使能 
void SPI_FLASH_Write_Disable(void);	//写保护
void SPI_Flash_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);
void SPI_Flash_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead);   //读取flash
void SPI_Flash_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);//写入flash
void SPI_Flash_Erase_Chip(void);    	  //整片擦除
void SPI_Flash_Erase_Sector(u32 Dst_Addr);//扇区擦除
void SPI_Flash_Wait_Busy(void);           //等待空闲
void SPI_Flash_PowerDown(void);           //进入掉电模式
void SPI_Flash_WAKEUP(void);			  //唤醒


void SPI_Flash_Init(void);
//W25X系列/Q系列芯片列表	   
//W25Q80 ID  0XEF13
//W25Q16 ID  0XEF14
//W25Q32 ID  0XEF15
//W25Q32 ID  0XEF16	
#define W25Q80 	0XEF13 	
#define W25Q16 	0XEF14
#define W25Q32 	0XEF15
#define W25Q64 	0XEF16

extern u16 SPI_FLASH_TYPE;//定义我们使用的flash芯片型号
	

#define	SPI_FLASH_CS PBout(12)  //选中FLASH	
#define SPI_FLASH_CS_SET GPIO_SetBits(GPIOB,GPIO_Pin_12)
#define SPI_FLASH_CS_CLR GPIO_ResetBits(GPIOB,GPIO_Pin_12)
*/			 
////////////////////////////////////////////////////////////////////////////
 

#endif

