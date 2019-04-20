#include "24cxx/24cxx.h"


//��ʼ��IIC�ӿ�
static void AT24CXX_Init(AT24CXX_TYPE *me)
{
	// ����һ��IIC
	IIC_CreateDefaultIIC(&(me->IIC));

	me->IIC.GPIO_WP = me->GPIO_WP;
	me->IIC.GPIO_SCL = me->GPIO_SCL;
	me->IIC.GPIO_SDA = me->GPIO_SDA;
	
	me->IIC.Init(&(me->IIC));
}

static void AT24CXX_Open(AT24CXX_TYPE *me) {
	GPIOX_SetPinBit(me->IIC.GPIO_WP, RESET);
}

static void AT24CXX_Close(AT24CXX_TYPE *me) {
	GPIOX_SetPinBit(me->IIC.GPIO_WP, SET);
}

//��AT24CXXָ����ַ����һ������
//ReadAddr:��ʼ�����ĵ�ַ  
//����ֵ  :����������
static uint32_t AT24CXX_ReadOneByte(AT24CXX_TYPE *me, uint32_t addr, uint8_t *_byte)
{
	uint8_t writeCmd = 0xA0 | ((me->adddress & 0x07) << 1); // ���е�ַѡ��
	uint8_t readCmd = writeCmd + 1;
	IIC_TYPE _IIC = me->IIC;
	AT24CXX_TYPE_ENUM type = me->type;

	
	_IIC.Start(_IIC);
	
	if (type > AT24C16) {
		_IIC.WriteDataOneByte(_IIC, writeCmd);
		_IIC.WaitAck(_IIC);
		_IIC.WriteDataOneByte(_IIC, addr >> 8);
		_IIC.WaitAck(_IIC);
	} else {
		_IIC.WriteDataOneByte(_IIC, 0xA0 + ((addr >> 8) << 1));
		_IIC.WaitAck(_IIC);
	}
	
	_IIC.WriteDataOneByte(_IIC, addr & 0XFF);
	_IIC.WaitAck(_IIC);
	_IIC.Start(_IIC);
	_IIC.WriteDataOneByte(_IIC, readCmd);
	_IIC.WaitAck(_IIC);
	_IIC.ReadDataOneByte(_IIC, _byte);

	_IIC.SendNoAck(_IIC);
	_IIC.Stop(_IIC);

	return 1;
}
//��AT24CXXָ����ַд��һ������
//WriteAddr  :д�����ݵ�Ŀ�ĵ�ַ    
//DataToWrite:Ҫд�������
static uint32_t AT24CXX_WriteOneByte(AT24CXX_TYPE *me, uint32_t addr, uint8_t _byte)
{				  
	uint8_t writeCmd = 0xA0 | ((me->adddress & 0x07) << 1); // ���е�ַѡ��
	IIC_TYPE _IIC = me->IIC;
	AT24CXX_TYPE_ENUM type = me->type;

	_IIC.Start(_IIC);

	if (type > AT24C16) {
		_IIC.WriteDataOneByte(_IIC, writeCmd);
		_IIC.WaitAck(_IIC);
		_IIC.WriteDataOneByte(_IIC, addr >> 8);
		_IIC.WaitAck(_IIC);
	} else {
		_IIC.WriteDataOneByte(_IIC, 0xA0 + ((addr >> 8) << 1));
		_IIC.WaitAck(_IIC);
	}

	_IIC.WriteDataOneByte(_IIC, addr & 0xFF);
	_IIC.WaitAck(_IIC);
	_IIC.WriteDataOneByte(_IIC, _byte);
	_IIC.WaitAck(_IIC);
	_IIC.Stop(_IIC);

	return 1;
}


static uint32_t AT24CXX_WriteMultiByte(struct __AT24CXX_TYPE *me, uint32_t addr, uint8_t *_byte, uint32_t len) {
	uint8_t writeCmd = 0xA0 | ((me->adddress & 0x07) << 1); // ���е�ַѡ��
	IIC_TYPE _IIC = me->IIC;
	AT24CXX_TYPE_ENUM type = me->type;
	uint32_t i = 0;

	
	_IIC.Start(_IIC);

	if (type > AT24C16) {
		_IIC.WriteDataOneByte(_IIC, writeCmd);
		_IIC.WaitAck(_IIC);
		_IIC.WriteDataOneByte(_IIC, addr >> 8);
		_IIC.WaitAck(_IIC);
	} else {
		_IIC.WriteDataOneByte(_IIC, 0xA0 + ((addr >> 8) << 1));
		_IIC.WaitAck(_IIC);
	}
	
	_IIC.WriteDataOneByte(_IIC, addr & 0xFF);
	_IIC.WaitAck(_IIC);
	
	for (i = 0; i < len; i++) {
		_IIC.WriteDataOneByte(_IIC, *_byte);
		_IIC.WaitAck(_IIC);
		_byte++;
	}
	
	_IIC.Stop(_IIC);
	
	return i;
}

static uint32_t AT24CXX_ReadMultiByte(AT24CXX_TYPE *me, uint32_t addr, uint8_t *_byte, uint32_t len) {	
	uint8_t writeCmd = 0xA0 | ((me->adddress & 0x07) << 1); // ���е�ַѡ��
	uint8_t readCmd = writeCmd + 1;
	IIC_TYPE _IIC = me->IIC;
	uint32_t i = 0;
	AT24CXX_TYPE_ENUM type = me->type;


	_IIC.Start(_IIC);

	if (type > AT24C16) {
		_IIC.WriteDataOneByte(_IIC, writeCmd);
		_IIC.WaitAck(_IIC);
		_IIC.WriteDataOneByte(_IIC, addr >> 8);
		_IIC.WaitAck(_IIC);
	} else {
		_IIC.WriteDataOneByte(_IIC, 0xA0 + ((addr >> 8) << 1));
		_IIC.WaitAck(_IIC);
	}

	_IIC.WriteDataOneByte(_IIC, addr & 0XFF);
	_IIC.WaitAck(_IIC);
	_IIC.Start(_IIC);
	_IIC.WriteDataOneByte(_IIC, readCmd);
	_IIC.WaitAck(_IIC);

	for (i = 0; i < len; i++) {
		_IIC.ReadDataOneByte(_IIC, _byte);		
		_IIC.SendNoAck(_IIC);
		_byte++;
	}
	
	_IIC.Stop(_IIC);

	return 1;
}

static uint32_t AT24CXX_WriteOneWord(struct __AT24CXX_TYPE *me, uint32_t addr, uint16_t _word) {
	return me->WriteMultiWord(me, addr, &_word, 1);
}
static uint32_t AT24CXX_WriteOneInt(struct __AT24CXX_TYPE *me, uint32_t addr, uint32_t _intager) {
	return me->WriteMultiInt(me, addr, &_intager, 1);
}
static uint32_t AT24CXX_ReadOneWord(struct __AT24CXX_TYPE *me, uint32_t addr, uint16_t *_word) {
	return me->ReadMultiWord(me, addr, _word, 1);
}
static uint32_t AT24CXX_ReadOneInt(struct __AT24CXX_TYPE *me, uint32_t addr, uint32_t *_intager) {
	return me->ReadMultiInt(me, addr, _intager, 1);
}

static uint32_t AT24CXX_WriteMultiWord(struct __AT24CXX_TYPE *me, uint32_t addr, uint16_t *_word, uint32_t len) {
	uint8_t *bytePtr = (uint8_t *)_word;

	me->WriteMultiByte(me, addr, bytePtr, len * 2);

	return len*2;
}
static uint32_t AT24CXX_WriteMultiInt(struct __AT24CXX_TYPE *me, uint32_t addr, uint32_t *_intager, uint32_t len) {
	uint8_t *bytePtr = (uint8_t *)_intager;

	me->WriteMultiByte(me, addr, bytePtr, len * sizeof(int));

	return len*sizeof(int);
}
static uint32_t AT24CXX_ReadMultiWord(struct __AT24CXX_TYPE *me, uint32_t addr, uint16_t *_word, uint32_t len) {
	uint8_t *bytePtr = (uint8_t *)_word;

	me->ReadMultiByte(me, addr, bytePtr, len * 2);

	return len*2;
}
static uint32_t AT24CXX_ReadMultiInt(struct __AT24CXX_TYPE *me, uint32_t addr, uint32_t *_intager, uint32_t len) {
	uint8_t *bytePtr = (uint8_t *)_intager;

	me->ReadMultiByte(me, addr, bytePtr, len * 4);

	return len * 4;
}




//���AT24CXX�Ƿ�����
//��������24XX�����һ����ַ(255)���洢��־��.
//���������24Cϵ��,�����ַҪ�޸�
//����1:���ʧ��
//����0:���ɹ�
static uint8_t AT24CXX_CheckChip(AT24CXX_TYPE *me) {
	uint8_t temp = 0;
	
	//����ÿ�ο�����дAT24CXX
	me->ReadOneByte(me, me->checkAddress, &temp);
	
	if(temp == me->checkValue) {
		return 0;
 	} else {
		me->WriteOneByte(me, me->checkAddress, me->checkValue);
	    me->ReadOneByte(me, me->checkAddress, &temp);
		printf("temp:%x\r\n", temp);
		if (temp == me->checkValue) {
			return 0;
		}
	}
	
	return 1;											  
}
/*
//��AT24CXX�����ָ����ַ��ʼ����ָ������������
//ReadAddr :��ʼ�����ĵ�ַ ��24c02Ϊ0~255
//pBuffer  :���������׵�ַ
//NumToRead:Ҫ�������ݵĸ���
void AT24CXX_Read(uint16_t ReadAddr,uint8_t *pBuffer,uint16_t NumToRead)
{
	while(NumToRead--)
	{
		*pBuffer = AT24CXX_ReadOneByte(ReadAddr);
		pBuffer++;
		ReadAddr++;
	}
}  
//��AT24CXX�����ָ����ַ��ʼд��ָ������������
//WriteAddr :��ʼд��ĵ�ַ ��24c02Ϊ0~255
//pBuffer   :���������׵�ַ
//NumToWrite:Ҫд�����ݵĸ���
void AT24CXX_Write(uint16_t WriteAddr,uint8_t *pBuffer,uint16_t NumToWrite)
{
	while(NumToWrite--)
	{
		AT24CXX_WriteOneByte(WriteAddr,*pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}
*/

void AT24CXX_CreateDefaultAT24CXX(AT24CXX_TYPE *AT24CXX) {
	AT24CXX->Init = AT24CXX_Init;
	AT24CXX->Open = AT24CXX_Open;
	AT24CXX->Close = AT24CXX_Close;

	AT24CXX->CheckChip = AT24CXX_CheckChip;
	AT24CXX->ReadOneByte = AT24CXX_ReadOneByte;
	AT24CXX->WriteOneByte = AT24CXX_WriteOneByte;
	AT24CXX->ReadOneWord = AT24CXX_ReadOneWord;
	AT24CXX->WriteOneWord = AT24CXX_WriteOneWord;
	AT24CXX->ReadOneInt = AT24CXX_ReadOneInt;
	AT24CXX->WriteOneInt = AT24CXX_WriteOneInt;
	AT24CXX->ReadMultiByte = AT24CXX_ReadMultiByte;
	AT24CXX->WriteMultiByte = AT24CXX_WriteMultiByte;
	AT24CXX->ReadMultiWord = AT24CXX_ReadMultiWord;
	AT24CXX->WriteMultiWord = AT24CXX_WriteMultiWord;
	AT24CXX->ReadMultiInt = AT24CXX_ReadMultiInt;
	AT24CXX->WriteMultiInt = AT24CXX_WriteMultiInt;
}





/*
-------------------------------------------
*/

void AT24CXX_Delay(void) {
	int i, j;

	for (i = 0; i < 1000; i++) {
		for (j = 0; j < 1000; j++) {
			;
		}
	}
}

void AT24CXXTestDemo(void) {
	AT24CXX_TYPE AT24CXX;
	GPIO_TYPE gpio;

	
	AT24CXX_CreateDefaultAT24CXX(&AT24CXX);

	gpio.Port = GPIOB;
	gpio.GPIOInitTypeStruct.GPIO_Pin = GPIO_Pin_10;
	AT24CXX.GPIO_SCL = gpio;

	gpio.Port = GPIOB;
	gpio.GPIOInitTypeStruct.GPIO_Pin = GPIO_Pin_11;
	AT24CXX.GPIO_SDA = gpio;
	
	AT24CXX.Init(&AT24CXX);

	
	while (AT24CXX.CheckChip(&AT24CXX)) {
		AT24CXX_Delay();
		printf("24cxx check faild\r\n");
	}
    
    printf("24CXX check chip success\r\n");
	
	
}


/*
-------------------------------------------
*/



