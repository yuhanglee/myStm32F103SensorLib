#include "SPI/spi.h"
#include "string.h"

static SPI_InitTypeDef __SPI_InitZero = {0};


/*******************************************************************************
 * Function Name  : Spi_Delay
 * Description    : ģ��SPI��ʱ
 * Input          : None 
 * Output         : None
 * Return         : None
 *******************************************************************************/
static void Spi_Delay(void)
{
    int j = 0;
    
    for(j = 0; j < 360; j++);
}

/*ʹ���ڲ�*/
/*******************************************************************************
 * Function Name  : SPI_Inside_Init
 * Description    : �ڲ�SPI��ʼ��
 * Input          : me ��Ҫ���г�ʼ���Ľṹ���ַ 
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

	// û�����ò���
	if (memcmp(&(me->InitStru), &__SPI_InitZero, sizeof(__SPI_InitZero)) == 0) {
		// ���δ���г�ʼ���������SPI���ó�ʼ��
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
		// �Ѿ����й��ⲿ��ʼ��
		SPI_InitStructure = me->InitStru;
	}
	SPI_Init(SPIx, &SPI_InitStructure);
}

/*******************************************************************************
 * Function Name  : SPI_Inside_Open
 * Description    : �ڲ�SPI����
 * Input          : me ��Ҫ���г�ʼ���Ľṹ���ַ 
 * Output         : None
 * Return         : None
 *******************************************************************************/
static void SPI_Inside_Open(SPI_TYPE const *me) {
	SPI_Cmd(me->SPIx, ENABLE);
}

/*******************************************************************************
 * Function Name  : SPI_Inside_Close
 * Description    : �ڲ�SPI�ر�
 * Input          : me ��Ҫ���г�ʼ���Ľṹ���ַ 
 * Output         : None
 * Return         : None
 *******************************************************************************/
static void SPI_Inside_Close(SPI_TYPE const *me) {
	SPI_Cmd(me->SPIx, DISABLE);
}

/*******************************************************************************
 * Function Name  : SPI_Inside_Write
 * Description    : ��������
 * Input          : me ��Ҫ���г�ʼ���Ľṹ���ַ 
 *                : data (��Ҫ���͵�����) 
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
 * Description    : ��ȡ����
 * Input          : me ��Ҫ���г�ʼ���Ľṹ���ַ 
 * Output         : None
 * Return         : ��ȡ��������
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

/* ģ��SPI */

/*******************************************************************************
 * Function Name  : SPI_Inside_Init
 * Description    : �ڲ�SPI��ʼ��
 * Input          : me ��Ҫ���г�ʼ���Ľṹ���ַ 
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
 * Description    : �ڲ�SPI����
 * Input          : me ��Ҫ���г�ʼ���Ľṹ���ַ 
 * Output         : None
 * Return         : None
 *******************************************************************************/
static void SPI_Model_Open(SPI_TYPE const *me) {
	
}

/*******************************************************************************
 * Function Name  : SPI_Inside_Close
 * Description    : �ڲ�SPI�ر�
 * Input          : me ��Ҫ���г�ʼ���Ľṹ���ַ 
 * Output         : None
 * Return         : None
 *******************************************************************************/
static void SPI_Model_Close(SPI_TYPE const *me) {

}

/*******************************************************************************
 * Function Name  : SPI_Inside_Write
 * Description    : ��������
 * Input          : me ��Ҫ���г�ʼ���Ľṹ���ַ 
 *                : data (��Ҫ���͵�����) 
 * Output         : None
 * Return         : None
 *******************************************************************************/
static void SPI_Model_Write(SPI_TYPE const *me, uint8_t const data) {
	uint8_t i = 0;
	uint8_t _data = data;
	// ����CS����
	GPIOX_SetPinBit(me->GPIO_CS, RESET);

	// ��һ�����ط�������
	if (me->SPI_CPHA == RESET) {
		for (i = 0; i < 8; i++) {
			// ��λ����
			if (me->InitStru.SPI_FirstBit == SPI_FirstBit_MSB) {
				GPIOX_SetPinBit(me->GPIO_MISO, (_data & 0x80) != 0);
				_data <<= 1;
			// ��λ����
			} else {
				GPIOX_SetPinBit(me->GPIO_MISO, (_data & 0x01) != 0);
				_data >>= 1;
			}
			// ��ת��ƽ
			GPIOX_TogglePinBit(me->GPIO_MISO);
			Spi_Delay();
			// ��ת��ƽ
			GPIOX_TogglePinBit(me->GPIO_MISO);
			Spi_Delay();
		}
	// �ڶ������ط�������
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
	
	// �ͷ�CS����
	GPIOX_SetPinBit(me->GPIO_CS, SET);
}

/*******************************************************************************
 * Function Name  : SPI_Inside_Read
 * Description    : ��ȡ����
 * Input          : me ��Ҫ���г�ʼ���Ľṹ���ַ 
 * Output         : None
 * Return         : ��ȡ��������
 *******************************************************************************/
static uint8_t SPI_Model_Read(SPI_TYPE const *me, uint8_t const data) {
	uint8_t ret = 0;
	uint8_t i = 0, _data = 0;
	
	// ����CS����
	GPIOX_SetPinBit(me->GPIO_CS, RESET);

	// ��һ�����ط�������
	if (me->SPI_CPHA == RESET) {
		for (i = 0; i < 8; i++) {
			// ��λ����
			if (me->InitStru.SPI_FirstBit == SPI_FirstBit_MSB) {
				GPIOX_SetPinBit(me->GPIO_MOSI, (_data & 0x80) != 0);
				_data <<= 1;
			// ��λ����
			} else {
				GPIOX_SetPinBit(me->GPIO_MOSI, (_data & 0x01) != 0);
				_data >>= 1;
			}
			// ��ת��ƽ
			GPIOX_TogglePinBit(me->GPIO_CLK);
			Spi_Delay();
			if (me->InitStru.SPI_FirstBit == SPI_FirstBit_MSB) {
				ret |= (GPIOX_GetPinBit(me->GPIO_MISO) ? 0x01 : 0x00);
				ret <<= 1;
			} else {
				ret |= (GPIOX_GetPinBit(me->GPIO_MISO) ? 0x80 : 0x00);
				ret >>= 1;
			}
			// ��ת��ƽ
			GPIOX_TogglePinBit(me->GPIO_CLK);
			Spi_Delay();
		}
	// �ڶ������ط�������
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

	
	// �ͷ�CS����
	GPIOX_SetPinBit(me->GPIO_CS, SET);
	return ret;
}

/* END MODEL SPI */

/*
 * �ĸ����ŵĳ�ʼ��
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
 * Description    : �������ݣ��Լ���ȡ����
 * Input          : dat(��Ҫ���͵�����) 
 * Output         : None
 * Return         : ��ȡ��������
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
		// TODO:  ��Ҫ�ڽ��б�д����û���Թ�
		//        ��Ҫ�ر�JTAG���ܺ�SWD����
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
	
	// �����ڲ�SPI
	_SPI.SPI_MODE = SPI_MODE_INSIDE;     // ����Ϊ�ڲ�ģʽ
	SPI_CreateDefaultSPI(&_SPI);         // ����Ĭ��SPI
	_SPI.Init(&_SPI);                    // ��ʼ��
	_SPI.Open(&_SPI);                    // ��SPI

	_SPI.Write(&_SPI, 0x30);			 // д0X30
	printf("read:%x\r\n", _SPI.Read(&_SPI, 0xFF)); // ������
	
	_SPI.Close(&_SPI);                   // �ر�SPI
	// ����GPIOģ��SPI
	_SPI.SPI_MODE = SPI_MODE_MODEL;	     // ����Ϊģ��ģʽ
	SPI_CreateDefaultSPI(&_SPI);         // ����Ĭ��SPI
	_SPI.Init(&_SPI);                    // ��ʼ��
	_SPI.Open(&_SPI);					 // ��
	_SPI.Write(&_SPI, 0x44);			 // SPI д
	printf("read:%x\r\n", _SPI.Read(&_SPI, 0xFF)); // SPI ��
	_SPI.Close(&_SPI);					 // �ر�
}


