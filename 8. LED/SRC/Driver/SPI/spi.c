#include "SPI/spi.h"
#include "string.h"

static SPI_InitTypeDef __SPI_InitZero = {0};


/*******************************************************************************
 * Function Name  : Spi_Delay
 * Description    : 模拟SPI延时
 * Input          : None 
 * Output         : None
 * Return         : None
 *******************************************************************************/
static void Spi_Delay(void)
{
    int j = 0;
    
    for(j = 0; j < 360; j++);
}

/*使用内部*/
/*******************************************************************************
 * Function Name  : SPI_Inside_Init
 * Description    : 内部SPI初始化
 * Input          : me 需要进行初始化的结构体地址 
 * Output         : None
 * Return         : None
 *******************************************************************************/
static void SPI_Inside_Init(SPI_TYPE *me)
{
	SPI_TypeDef      * SPIx;
	SPI_InitTypeDef  SPI_InitStructure; 
	

	SPIx = (SPI_TypeDef *)(me->SPIx);
	
    SPI_Cmd(SPIx, DISABLE);

	if (SPIx == SPI1) {
    	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE); 
	} else if (SPIx == SPI2) {
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE); 	
	}
	GPIOX_Init(me->GPIO_CS);
	GPIOX_Init(me->GPIO_CLK);
	GPIOX_Init(me->GPIO_MOSI);
	GPIOX_Init(me->GPIO_MISO);

	// 没有设置参数
	if (memcmp(&(me->InitStru), &__SPI_InitZero, sizeof(__SPI_InitZero)) == 0) {
		// 如果未进行初始化，则进行SPI设置初始化
	    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	    SPI_InitStructure.SPI_CPOL = me->SPI_CPOL; 
	    SPI_InitStructure.SPI_CPHA = me->SPI_CPHA;
	    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	    SPI_InitStructure.SPI_CRCPolynomial = 7;
	} else {
		// 已经进行过外部初始化
		SPI_InitStructure = me->InitStru;
	}
	SPI_Init(SPIx, &SPI_InitStructure);
}

/*******************************************************************************
 * Function Name  : SPI_Inside_Open
 * Description    : 内部SPI开启
 * Input          : me 需要进行初始化的结构体地址 
 * Output         : None
 * Return         : None
 *******************************************************************************/
static void SPI_Inside_Open(SPI_TYPE const *me) {
	SPI_Cmd(me->SPIx, ENABLE);
}

/*******************************************************************************
 * Function Name  : SPI_Inside_Close
 * Description    : 内部SPI关闭
 * Input          : me 需要进行初始化的结构体地址 
 * Output         : None
 * Return         : None
 *******************************************************************************/
static void SPI_Inside_Close(SPI_TYPE const *me) {
	SPI_Cmd(me->SPIx, DISABLE);
}

/*******************************************************************************
 * Function Name  : SPI_Inside_Write
 * Description    : 发送数据
 * Input          : me 需要进行初始化的结构体地址 
 *                : data (需要发送的数据) 
 * Output         : None
 * Return         : None
 *******************************************************************************/
static void SPI_Inside_Write(SPI_TYPE const *me, uint8_t const data) {
	SPI_TypeDef * SPIx = (SPI_TypeDef *)(me->SPIx);
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPIx, data); 

	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET); 
	SPI_I2S_ReceiveData(SPIx); 

}

/*******************************************************************************
 * Function Name  : SPI_Inside_Read
 * Description    : 读取数据
 * Input          : me 需要进行初始化的结构体地址 
 * Output         : None
 * Return         : 读取到的数据
 *******************************************************************************/
static uint8_t SPI_Inside_Read(SPI_TYPE const *me, uint8_t const data) {
	SPI_TypeDef *SPIx = (SPI_TypeDef *)(me->SPIx);
	uint8_t ret = 0;
	
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPIx, data); 

	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET); 
	ret = SPI_I2S_ReceiveData(SPIx); 

	return ret;
}
/* EDN INSIDE SPI*/

/* 模拟SPI */

/*******************************************************************************
 * Function Name  : SPI_Inside_Init
 * Description    : 内部SPI初始化
 * Input          : me 需要进行初始化的结构体地址 
 * Output         : None
 * Return         : None
 *******************************************************************************/
static void SPI_Model_Init(SPI_TYPE *me)
{
	GPIOX_Init(me->GPIO_CS);
	me->GPIO_CLK.IntValue = me->SPI_CPOL;
	GPIOX_Init(me->GPIO_CLK);
	GPIOX_Init(me->GPIO_MOSI);
	GPIOX_Init(me->GPIO_MISO);
}

/*******************************************************************************
 * Function Name  : SPI_Inside_Open
 * Description    : 内部SPI开启
 * Input          : me 需要进行初始化的结构体地址 
 * Output         : None
 * Return         : None
 *******************************************************************************/
static void SPI_Model_Open(SPI_TYPE const *me) {
	
}

/*******************************************************************************
 * Function Name  : SPI_Inside_Close
 * Description    : 内部SPI关闭
 * Input          : me 需要进行初始化的结构体地址 
 * Output         : None
 * Return         : None
 *******************************************************************************/
static void SPI_Model_Close(SPI_TYPE const *me) {

}

/*******************************************************************************
 * Function Name  : SPI_Inside_Write
 * Description    : 发送数据
 * Input          : me 需要进行初始化的结构体地址 
 *                : data (需要发送的数据) 
 * Output         : None
 * Return         : None
 *******************************************************************************/
static void SPI_Model_Write(SPI_TYPE const *me, uint8_t const data) {
	uint8_t i = 0;
	uint8_t _data = data;
	// 拉低CS引脚
	GPIOX_SetPinBit(me->GPIO_CS, RESET);

	// 第一个边沿发送数据
	if (me->SPI_CPHA == RESET) {
		for (i = 0; i < 8; i++) {
			// 高位优先
			if (me->InitStru.SPI_FirstBit == SPI_FirstBit_MSB) {
				GPIOX_SetPinBit(me->GPIO_MISO, (_data & 0x80) != 0);
				_data <<= 1;
			// 低位优先
			} else {
				GPIOX_SetPinBit(me->GPIO_MISO, (_data & 0x01) != 0);
				_data >>= 1;
			}
			// 反转电平
			GPIOX_TogglePinBit(me->GPIO_MISO);
			Spi_Delay();
			// 反转电平
			GPIOX_TogglePinBit(me->GPIO_MISO);
			Spi_Delay();
		}
	// 第二个边沿发送数据
	} else { 
		for (i = 0;i < 8; i++) {
			GPIOX_TogglePinBit(me->GPIO_MISO);
			Spi_Delay();
			if (me->InitStru.SPI_FirstBit == SPI_FirstBit_MSB) {
				GPIOX_SetPinBit(me->GPIO_MISO, (_data & 0x80) != 0);
				_data <<= 1;
			} else {
				GPIOX_SetPinBit(me->GPIO_MISO, (_data & 0x01) != 0);
				_data >>= 1;
			}
			GPIOX_TogglePinBit(me->GPIO_MISO);
			Spi_Delay();
		}
	}
	
	// 释放CS引脚
	GPIOX_SetPinBit(me->GPIO_CS, SET);
}

/*******************************************************************************
 * Function Name  : SPI_Inside_Read
 * Description    : 读取数据
 * Input          : me 需要进行初始化的结构体地址 
 * Output         : None
 * Return         : 读取到的数据
 *******************************************************************************/
static uint8_t SPI_Model_Read(SPI_TYPE const *me, uint8_t const data) {
	uint8_t ret = 0;
	uint8_t i = 0, _data = 0;
	
	// 拉低CS引脚
	GPIOX_SetPinBit(me->GPIO_CS, RESET);

	// 第一个边沿发送数据
	if (me->SPI_CPHA == RESET) {
		for (i = 0; i < 8; i++) {
			// 高位优先
			if (me->InitStru.SPI_FirstBit == SPI_FirstBit_MSB) {
				GPIOX_SetPinBit(me->GPIO_MOSI, (_data & 0x80) != 0);
				_data <<= 1;
			// 低位优先
			} else {
				GPIOX_SetPinBit(me->GPIO_MOSI, (_data & 0x01) != 0);
				_data >>= 1;
			}
			// 反转电平
			GPIOX_TogglePinBit(me->GPIO_CLK);
			Spi_Delay();
			if (me->InitStru.SPI_FirstBit == SPI_FirstBit_MSB) {
				ret |= (GPIOX_GetPinBit(me->GPIO_MISO) ? 0x01 : 0x00);
				ret <<= 1;
			} else {
				ret |= (GPIOX_GetPinBit(me->GPIO_MISO) ? 0x80 : 0x00);
				ret >>= 1;
			}
			// 反转电平
			GPIOX_TogglePinBit(me->GPIO_CLK);
			Spi_Delay();
		}
	// 第二个边沿发送数据
	} else { 
		for (i = 0;i < 8; i++) {
			GPIOX_TogglePinBit(me->GPIO_MISO);
			Spi_Delay();
			if (me->InitStru.SPI_FirstBit == SPI_FirstBit_MSB) {
				GPIOX_SetPinBit(me->GPIO_MISO, (_data & 0x80) != 0);
				_data <<= 1;
			} else {
				GPIOX_SetPinBit(me->GPIO_MISO, (_data & 0x01) != 0);
				_data >>= 1;
			}
			GPIOX_TogglePinBit(me->GPIO_MISO);
			Spi_Delay();
			if (me->InitStru.SPI_FirstBit == SPI_FirstBit_MSB) {
				ret |= (GPIOX_GetPinBit(me->GPIO_MISO) ? 0x01 : 0x00);
				ret <<= 1;
			} else {
				ret |= (GPIOX_GetPinBit(me->GPIO_MISO) ? 0x80 : 0x00);
				ret >>= 1;
			}
		}
	}

	
	// 释放CS引脚
	GPIOX_SetPinBit(me->GPIO_CS, SET);
	return ret;
}

/* END MODEL SPI */

/*
 * 四个引脚的初始化
 */
static const GPIO_TYPE __CS = {
	GPIOA, {GPIO_Pin_0, GPIO_Speed_50MHz, GPIO_Mode_Out_PP}, SET,
};
static const GPIO_TYPE __CLK = {
	GPIOA, {GPIO_Pin_0, GPIO_Speed_50MHz, GPIO_Mode_Out_PP}, RESET,
};
static const GPIO_TYPE __MOSI = {
	GPIOA, {GPIO_Pin_0, GPIO_Speed_50MHz, GPIO_Mode_Out_PP}, RESET,
};
static const GPIO_TYPE __MISO = {
	GPIOA, {GPIO_Pin_0, GPIO_Speed_50MHz, GPIO_Mode_IPU}, RESET,
};

/*******************************************************************************
 * Function Name  : SPI1_RW
 * Description    : 发送数据，以及读取数据
 * Input          : dat(需要发送的数据) 
 * Output         : None
 * Return         : 读取到的数据
 *******************************************************************************/
static void SPI_CreateDefaultSPI(SPI_TYPE *me) {
    if ((SPI_TypeDef *)(me->SPIx) == SPI1) { // SPI1
		me->GPIO_CS = __CS;
		me->GPIO_CS.Port = GPIOA;
		me->GPIO_CS.GPIOInitTypeStruct.GPIO_Pin = GPIO_Pin_4;

		me->GPIO_CLK = __CLK;
		me->GPIO_CLK.Port = GPIOA;
		me->GPIO_CLK.GPIOInitTypeStruct.GPIO_Pin = GPIO_Pin_5;

		me->GPIO_MISO = __MISO;
		me->GPIO_MISO.Port = GPIOA;
		me->GPIO_MISO.GPIOInitTypeStruct.GPIO_Pin = GPIO_Pin_6;

		me->GPIO_MISO = __MOSI;
		me->GPIO_MOSI.Port = GPIOA;
		me->GPIO_MOSI.GPIOInitTypeStruct.GPIO_Pin = GPIO_Pin_7;
	} else if ((SPI_TypeDef *)(me->SPIx) == SPI2) { // SPI2
		me->GPIO_CS = __CS;
		me->GPIO_CS.Port = GPIOB;
		me->GPIO_CS.GPIOInitTypeStruct.GPIO_Pin = GPIO_Pin_12;

		me->GPIO_CLK = __CLK;
		me->GPIO_CLK.Port = GPIOB;
		me->GPIO_CLK.GPIOInitTypeStruct.GPIO_Pin = GPIO_Pin_13;

		me->GPIO_MISO = __MISO;
		me->GPIO_MISO.Port = GPIOB;
		me->GPIO_MISO.GPIOInitTypeStruct.GPIO_Pin = GPIO_Pin_14;

		me->GPIO_MISO = __MOSI;
		me->GPIO_MOSI.Port = GPIOB;
		me->GPIO_MOSI.GPIOInitTypeStruct.GPIO_Pin = GPIO_Pin_15;
	} else if ((SPI_TypeDef *)(me->SPIx) == SPI3) { // SOI3
		// TODO:  需要在进行编写，还没有试过
		//        需要关闭JTAG功能和SWD功能
		me->GPIO_CS = __CS;
		me->GPIO_CS.Port = GPIOA;
		me->GPIO_CS.GPIOInitTypeStruct.GPIO_Pin = GPIO_Pin_15;

		me->GPIO_CLK = __CLK;
		me->GPIO_CLK.Port = GPIOB;
		me->GPIO_CLK.GPIOInitTypeStruct.GPIO_Pin = GPIO_Pin_3;

		me->GPIO_MISO = __MISO;
		me->GPIO_MISO.Port = GPIOB;
		me->GPIO_MISO.GPIOInitTypeStruct.GPIO_Pin = GPIO_Pin_4;

		me->GPIO_MISO = __MOSI;
		me->GPIO_MOSI.Port = GPIOB;
		me->GPIO_MOSI.GPIOInitTypeStruct.GPIO_Pin = GPIO_Pin_5;
	}

	if (me->SPI_MODE == SPI_MODE_INSIDE) {
		me->Init = SPI_Inside_Init;
		me->Open = SPI_Inside_Open;
		me->Close = SPI_Inside_Close;
		me->Write = SPI_Inside_Write;
		me->Read = SPI_Inside_Read;	
	} else if (me->SPI_MODE == SPI_MODE_MODEL) {
		me->Init = SPI_Model_Init;
		me->Open = SPI_Model_Open;
		me->Close = SPI_Model_Close;
		me->Write = SPI_Model_Write;
		me->Read = SPI_Model_Read;		
	}
}



/* DEMO */

void SPI_Demo(void) {
	SPI_TYPE _SPI;
	
	// 测试内部SPI
	_SPI.SPI_MODE = SPI_MODE_INSIDE;     // 设置为内部模式
	SPI_CreateDefaultSPI(&_SPI);         // 创建默认SPI
	_SPI.Init(&_SPI);                    // 初始化
	_SPI.Open(&_SPI);                    // 打开SPI

	_SPI.Write(&_SPI, 0x30);			 // 写0X30
	printf("read:%x\r\n", _SPI.Read(&_SPI, 0xFF)); // 读数据
	
	_SPI.Close(&_SPI);                   // 关闭SPI
	// 测试GPIO模拟SPI
	_SPI.SPI_MODE = SPI_MODE_MODEL;	     // 设置为模拟模式
	SPI_CreateDefaultSPI(&_SPI);         // 创建默认SPI
	_SPI.Init(&_SPI);                    // 初始化
	_SPI.Open(&_SPI);					 // 打开
	_SPI.Write(&_SPI, 0x44);			 // SPI 写
	printf("read:%x\r\n", _SPI.Read(&_SPI, 0xFF)); // SPI 读
	_SPI.Close(&_SPI);					 // 关闭
}


