#include "W25QXX/w25qxx.h" 
#include "string.h"
//ָ���
#define W25X_WriteEnable		0x06 
#define W25X_WriteDisable		0x04 
#define W25X_ReadStatusReg		0x05 
#define W25X_WriteStatusReg		0x01 
#define W25X_ReadData			0x03 
#define W25X_FastReadData		0x0B 
#define W25X_FastReadDual		0x3B 
#define W25X_PageProgram		0x02 
#define W25X_BlockErase			0xD8 
#define W25X_SectorErase		0x20 
#define W25X_ChipErase			0xC7 
#define W25X_PowerDown			0xB9 
#define W25X_ReleasePowerDown	0xAB 
#define W25X_DeviceID			0xAB 
#define W25X_ManufactDeviceID	0x90 
#define W25X_JedecDeviceID		0x9F 


void SPI_CS_WRITE_BIT(W25QXX_TYPE * _flash, uint8_t _bit) {
	GPIOX_SetPinBit(_flash->SPI.GPIO_CS, _bit);
}


// uint16_t W25QXX_TYPE=W25Q64;//Ĭ�Ͼ���25Q64

//4KbytesΪһ��Sector
//16������Ϊ1��Block
//W25X16
//����Ϊ2M�ֽ�,����32��Block,512��Sector 

//��ʼ��SPI FLASH��IO��
static void W25QXX_Init(W25QXX_TYPE *me) {
	SPI_CreateDefaultSPI(&(me->SPI));
	
	me->SPI.Init(&(me->SPI));
	me->SPI.Open(&(me->SPI));

	me->type = me->ReadID(me);//��ȡFLASH ID.
}  

//��ȡW25QXX��״̬�Ĵ���
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:Ĭ��0,״̬�Ĵ�������λ,���WPʹ��
//TB,BP2,BP1,BP0:FLASH����д��������
//WEL:дʹ������
//BUSY:æ���λ(1,æ;0,����)
//Ĭ��:0x00
static uint8_t W25QXX_ReadSR(W25QXX_TYPE *me) {
	uint8_t byte=0;   

	SPI_CS_WRITE_BIT(me, RESET);
	me->SPI.Write(&(me->SPI), W25X_ReadStatusReg);    //���Ͷ�ȡ״̬�Ĵ�������    
	byte = me->SPI.Read(&(me->SPI), 0xFF);             //��ȡһ���ֽ�  
	SPI_CS_WRITE_BIT(me, SET);

	return byte;   
} 
//дW25QXX״̬�Ĵ���
//ֻ��SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)����д!!!
static void W25QXX_Write_SR(W25QXX_TYPE *me, uint8_t sr) {
	SPI_CS_WRITE_BIT(me, RESET);                            //ʹ������   
	me->SPI.Write(&(me->SPI), W25X_WriteStatusReg);   //����дȡ״̬�Ĵ�������    
	me->SPI.Write(&(me->SPI), sr);               //д��һ���ֽ�  
	SPI_CS_WRITE_BIT(me, SET);                            //ȡ��Ƭѡ     	      
}   
//W25QXXдʹ��	
//��WEL��λ   
static void W25QXX_Write_Enable(W25QXX_TYPE *me)   
{
	SPI_CS_WRITE_BIT(me, RESET);                            //ʹ������   
    me->SPI.Write(&(me->SPI), W25X_WriteEnable);      //����дʹ��  
	SPI_CS_WRITE_BIT(me, SET);                            //ȡ��Ƭѡ     	      
} 
//W25QXXд��ֹ	
//��WEL����  
static void W25QXX_Write_Disable(W25QXX_TYPE *me)   
{  
	SPI_CS_WRITE_BIT(me, RESET);                            //ʹ������   
    me->SPI.Write(&(me->SPI), W25X_WriteDisable);     //����д��ָֹ��    
	SPI_CS_WRITE_BIT(me, SET);                            //ȡ��Ƭѡ     	      
} 			    
//��ȡоƬID W25X16��ID:0XEF14
static uint16_t W25QXX_ReadID(W25QXX_TYPE *me)
{
	uint16_t Temp = 0;

	SPI_CS_WRITE_BIT(me, RESET);				    
	me->SPI.Write(&(me->SPI), 0x90);//���Ͷ�ȡID����	    
	me->SPI.Write(&(me->SPI), 0x00); 	    
	me->SPI.Write(&(me->SPI), 0x00); 	    
	me->SPI.Write(&(me->SPI), 0x00);
	Temp |= me->SPI.Read(&(me->SPI), 0xFF) << 8;  
	Temp |= me->SPI.Read(&(me->SPI), 0xFF);
	SPI_CS_WRITE_BIT(me, SET);				    
	
	return Temp;
}   		    
//��ȡSPI FLASH  
//��ָ����ַ��ʼ��ȡָ�����ȵ�����
//pBuffer:���ݴ洢��
//ReadAddr:��ʼ��ȡ�ĵ�ַ(24bit)
//NumByteToRead:Ҫ��ȡ���ֽ���(���65535)
static void W25QXX_Read(W25QXX_TYPE *me, uint8_t* pBuf, uint32_t addr, uint16_t len)   
{ 
 	uint16_t i;
	
	SPI_CS_WRITE_BIT(me, RESET);                            //ʹ������   
    me->SPI.Write(&(me->SPI), W25X_ReadData);         //���Ͷ�ȡ����   
    me->SPI.Write(&(me->SPI), (uint8_t)((addr)>>16));  //����24bit��ַ    
    me->SPI.Write(&(me->SPI), (uint8_t)((addr)>>8));   
    me->SPI.Write(&(me->SPI), (uint8_t)addr);   
    for (i = 0; i < len; i++) { 
        pBuf[i] = me->SPI.Read(&(me->SPI), 0XFF);   //ѭ������  
    }
	SPI_CS_WRITE_BIT(me, SET);                            //ȡ��Ƭѡ     	      
}  
//SPI��һҳ(0~65535)��д������256���ֽڵ�����
//��ָ����ַ��ʼд�����256�ֽڵ�����
//pBuffer:���ݴ洢��
//addr:��ʼд��ĵ�ַ(24bit)
//len:Ҫд����ֽ���(���256),������Ӧ�ó�����ҳ��ʣ���ֽ���!!!	 
static void W25QXX_Write_Page(W25QXX_TYPE *me, uint8_t* pBuf, uint32_t addr, uint16_t len)
{
 	uint16_t i;

	me->Open(me);                  //SET WEL 

	SPI_CS_WRITE_BIT(me, RESET);                            //ʹ������   
    me->SPI.Write(&(me->SPI), W25X_PageProgram);      //����дҳ����   
    me->SPI.Write(&(me->SPI), (uint8_t)((addr)>>16)); //����24bit��ַ    
    me->SPI.Write(&(me->SPI), (uint8_t)((addr)>>8));   
    me->SPI.Write(&(me->SPI), (uint8_t)addr);   
    for (i = 0; i < len; i++) {
		me->SPI.Write(&(me->SPI), pBuf[i]);//ѭ��д�� 
    }
	me->WaitBusy(me);					   //�ȴ�д�����
	SPI_CS_WRITE_BIT(me, SET);                            //ȡ��Ƭѡ 
} 
//�޼���дSPI FLASH 
//����ȷ����д�ĵ�ַ��Χ�ڵ�����ȫ��Ϊ0XFF,�����ڷ�0XFF��д������ݽ�ʧ��!
//�����Զ���ҳ���� 
//��ָ����ַ��ʼд��ָ�����ȵ�����,����Ҫȷ����ַ��Խ��!
//pBuffer:���ݴ洢��
//addr:��ʼд��ĵ�ַ(24bit)
//len:Ҫд����ֽ���(���65535)
//CHECK OK
static void W25QXX_Write_NoCheck(W25QXX_TYPE *me, uint8_t* pBuf, uint32_t addr, uint16_t len)   
{ 			 		 
	uint16_t pageremain = 0;
	
	pageremain = 256 - addr % 256; //��ҳʣ����ֽ���		 	 
	
	if (len <= pageremain) {
		pageremain = len;//������256���ֽ�
	}
	while (1) {	   
		W25QXX_Write_Page(me, pBuf, addr, pageremain);
		
		if (len == pageremain) {
			break;//д�������
	 	} else {  //len>pageremain
			pBuf += pageremain;
			addr += pageremain;	

			len -= pageremain;			  //��ȥ�Ѿ�д���˵��ֽ���
			if (len > 256) {
				pageremain=256; //һ�ο���д��256���ֽ�
			} else { 
				pageremain = len; 	  //����256���ֽ���
			}
		}
	};	    
} 
//дSPI FLASH  
//��ָ����ַ��ʼд��ָ�����ȵ�����
//�ú�������������!
//pBuffer:���ݴ洢��
//addr:��ʼд��ĵ�ַ(24bit)
//len:Ҫд����ֽ���(���65535)  		   
uint8_t W25QXX_BUF[4096];
static void W25QXX_Write(W25QXX_TYPE *me, uint8_t* pBuf, uint32_t addr, uint16_t len)   
{ 
	uint32_t secpos;
	uint16_t secoff;
	uint16_t secremain;	   
 	uint16_t i;    

	secpos = addr / 4096;//������ַ 0~511 for w25x16
	secoff = addr % 4096;//�������ڵ�ƫ��
	secremain=4096-secoff;//����ʣ��ռ��С   

	if (len <= secremain) {
		secremain=len;//������4096���ֽ�
	}
	while (1) {
		me->Read(me, W25QXX_BUF,secpos*4096,4096);//������������������

		for(i = 0; i < secremain; i++) {  //У������
			if (W25QXX_BUF[secoff+i] != 0XFF) {//��Ҫ����
				break;
			}
		}
		if (i < secremain) {  //��Ҫ����
			me->EraseSector(me, secpos);//�����������
			for (i = 0; i < secremain; i++) {	   //����
				W25QXX_BUF[i+secoff] = pBuf[i];	  
			}
			me->NoCheck(me, W25QXX_BUF, secpos * 4096, 4096);//д����������  

		} else {
			me->NoCheck(me, pBuf, addr, secremain);//д�Ѿ������˵�,ֱ��д������ʣ������. 				   
		}
		if (len == secremain) {
			break;//д�������
		} else {//д��δ����
			secpos++;//������ַ��1
			secoff = 0;//ƫ��λ��Ϊ0 	 

		   	pBuf += secremain;  //ָ��ƫ��
			addr += secremain;//д��ַƫ��	   
		   	len -= secremain;				//�ֽ����ݼ�
			if (len > 4096) {
				secremain = 4096;	//��һ����������д����
			} else {
				secremain = len;			//��һ����������д����
			}	 
		}
	};	 	 
}
//��������оƬ
//��Ƭ����ʱ��:
//W25X16:25s 
//W25X32:40s 
//W25X64:40s 
//�ȴ�ʱ�䳬��...
static void W25QXX_Erase_Chip(W25QXX_TYPE *me)   
{                                             
	me->Open(me);
  	SPI_CS_WRITE_BIT(me, RESET);                            //ʹ������   
	me->WaitBusy(me);   
    me->SPI.Write(&(me->SPI), W25X_ChipErase);        //����Ƭ��������       
	me->WaitBusy(me);   				   //�ȴ�оƬ��������
	SPI_CS_WRITE_BIT(me, SET);                            //ȡ��Ƭѡ     	 
}   
//����һ������
//Dst_Addr:������ַ 0~511 for w25x16
//����һ��ɽ��������ʱ��:150ms
static void W25QXX_Erase_Sector(W25QXX_TYPE *me, uint32_t sectorAddr)   
{   
	sectorAddr *= 4096;
    me->Open(me);                  //SET WEL 	 
  	SPI_CS_WRITE_BIT(me, RESET);                            //ʹ������   
    me->WaitBusy(me);   
    me->SPI.Write(&(me->SPI), W25X_SectorErase);      //������������ָ�� 
    me->SPI.Write(&(me->SPI), (uint8_t)((sectorAddr)>>16));  //����24bit��ַ    
    me->SPI.Write(&(me->SPI), (uint8_t)((sectorAddr)>>8));   
    me->SPI.Write(&(me->SPI), (uint8_t)sectorAddr);     
    me->WaitBusy(me);   				   //�ȴ��������
	SPI_CS_WRITE_BIT(me, SET);                            //ȡ��Ƭѡ     	   
}  
//�ȴ�����
static void W25QXX_Wait_Busy(W25QXX_TYPE *me)   
{   
	while ((me->ReadSR(me) & 0x01) == 0x01);   // �ȴ�BUSYλ���
}  
//�������ģʽ
static void W25QXX_PowerDown(W25QXX_TYPE *me)   
{ 
  	SPI_CS_WRITE_BIT(me, RESET);                            //ʹ������   
    me->SPI.Write(&(me->SPI), W25X_PowerDown);        //���͵�������  
	SPI_CS_WRITE_BIT(me, SET);                            //ȡ��Ƭѡ     	      
}   
//����
static void W25QXX_WAKEUP(W25QXX_TYPE *me)   
{  
  	SPI_CS_WRITE_BIT(me, RESET);                            //ʹ������   
    me->SPI.Write(&(me->SPI), W25X_ReleasePowerDown);   //  send W25X_PowerDown command 0xAB    
	SPI_CS_WRITE_BIT(me, SET);                            //ȡ��Ƭѡ     	      
}   


static void W25QXX_CreateDefaultW25QXX(W25QXX_TYPE *me) {
	me->Init = W25QXX_Init;
	me->ReadID = W25QXX_ReadID;
	me->ReadSR = W25QXX_ReadSR;
	me->WriteSR = W25QXX_Write_SR;
	me->Open = W25QXX_Write_Enable;
	me->Close = W25QXX_Write_Disable;
	me->NoCheck = W25QXX_Write_NoCheck;
	me->Read = W25QXX_Read;
	me->Write = W25QXX_Write;
	me->EraseChip = W25QXX_Erase_Chip;
	me->EraseSector = W25QXX_Erase_Sector;
	me->WaitBusy = W25QXX_Wait_Busy;
	me->PowerDown = W25QXX_PowerDown;
	me->WakeUp = W25QXX_WAKEUP;
}



void W25QXX_Demo(void) {
	W25QXX_TYPE w25qxx = {0};

	// ʹ��ģ��SPI
	memset(&w25qxx, 0, sizeof(w25qxx));
	w25qxx.SPI.SPI_MODE = SPI_MODE_MODEL;
	w25qxx.SPI.SPI_CPHA = SPI_CPHA_2Edge;
	w25qxx.SPI.SPI_CPOL = SPI_CPOL_High;
	w25qxx.SPI.InitStru.SPI_FirstBit = SPI_FirstBit_MSB;
	

	w25qxx.SPI.GPIO_CS.Port = GPIOB;
	w25qxx.SPI.GPIO_CS.GPIOInitTypeStruct.GPIO_Pin = GPIO_Pin_12;

	w25qxx.SPI.GPIO_CLK.Port = GPIOB;
	w25qxx.SPI.GPIO_CLK.GPIOInitTypeStruct.GPIO_Pin = GPIO_Pin_13;

	w25qxx.SPI.GPIO_MISO.Port = GPIOB;
	w25qxx.SPI.GPIO_MISO.GPIOInitTypeStruct.GPIO_Pin = GPIO_Pin_14;

	w25qxx.SPI.GPIO_MOSI.Port = GPIOB;
	w25qxx.SPI.GPIO_MOSI.GPIOInitTypeStruct.GPIO_Pin = GPIO_Pin_15;


	W25QXX_CreateDefaultW25QXX(&w25qxx);

	w25qxx.Init(&w25qxx);

	printf("FLASH ID:[%X]\r\n", w25qxx.type);

	

	// ʹ���ڲ�SPI
	memset(&w25qxx, 0, sizeof(w25qxx));
	w25qxx.SPI.SPIx = SPI2;
	w25qxx.SPI.SPI_CPHA = SPI_CPHA_2Edge;
	w25qxx.SPI.SPI_CPOL = SPI_CPOL_High;
	w25qxx.SPI.SPI_MODE = SPI_MODE_INSIDE;

	W25QXX_CreateDefaultW25QXX(&w25qxx);
	
	w25qxx.Init(&w25qxx);

	printf("FLASH ID:[%X]\r\n", w25qxx.type);
}














