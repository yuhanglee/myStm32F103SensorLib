#include "W25QXX/w25qxx.h" 
#include "string.h"
//指令表
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


// uint16_t W25QXX_TYPE=W25Q64;//默认就是25Q64

//4Kbytes为一个Sector
//16个扇区为1个Block
//W25X16
//容量为2M字节,共有32个Block,512个Sector 

//初始化SPI FLASH的IO口
static void W25QXX_Init(W25QXX_TYPE *me) {
	SPI_CreateDefaultSPI(&(me->SPI));
	
	me->SPI.Init(&(me->SPI));
	me->SPI.Open(&(me->SPI));

	me->type = me->ReadID(me);//读取FLASH ID.
}  

//读取W25QXX的状态寄存器
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:默认0,状态寄存器保护位,配合WP使用
//TB,BP2,BP1,BP0:FLASH区域写保护设置
//WEL:写使能锁定
//BUSY:忙标记位(1,忙;0,空闲)
//默认:0x00
static uint8_t W25QXX_ReadSR(W25QXX_TYPE *me) {
	uint8_t byte=0;   

	SPI_CS_WRITE_BIT(me, RESET);
	me->SPI.Write(&(me->SPI), W25X_ReadStatusReg);    //发送读取状态寄存器命令    
	byte = me->SPI.Read(&(me->SPI), 0xFF);             //读取一个字节  
	SPI_CS_WRITE_BIT(me, SET);

	return byte;   
} 
//写W25QXX状态寄存器
//只有SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)可以写!!!
static void W25QXX_Write_SR(W25QXX_TYPE *me, uint8_t sr) {
	SPI_CS_WRITE_BIT(me, RESET);                            //使能器件   
	me->SPI.Write(&(me->SPI), W25X_WriteStatusReg);   //发送写取状态寄存器命令    
	me->SPI.Write(&(me->SPI), sr);               //写入一个字节  
	SPI_CS_WRITE_BIT(me, SET);                            //取消片选     	      
}   
//W25QXX写使能	
//将WEL置位   
static void W25QXX_Write_Enable(W25QXX_TYPE *me)   
{
	SPI_CS_WRITE_BIT(me, RESET);                            //使能器件   
    me->SPI.Write(&(me->SPI), W25X_WriteEnable);      //发送写使能  
	SPI_CS_WRITE_BIT(me, SET);                            //取消片选     	      
} 
//W25QXX写禁止	
//将WEL清零  
static void W25QXX_Write_Disable(W25QXX_TYPE *me)   
{  
	SPI_CS_WRITE_BIT(me, RESET);                            //使能器件   
    me->SPI.Write(&(me->SPI), W25X_WriteDisable);     //发送写禁止指令    
	SPI_CS_WRITE_BIT(me, SET);                            //取消片选     	      
} 			    
//读取芯片ID W25X16的ID:0XEF14
static uint16_t W25QXX_ReadID(W25QXX_TYPE *me)
{
	uint16_t Temp = 0;

	SPI_CS_WRITE_BIT(me, RESET);				    
	me->SPI.Write(&(me->SPI), 0x90);//发送读取ID命令	    
	me->SPI.Write(&(me->SPI), 0x00); 	    
	me->SPI.Write(&(me->SPI), 0x00); 	    
	me->SPI.Write(&(me->SPI), 0x00);
	Temp |= me->SPI.Read(&(me->SPI), 0xFF) << 8;  
	Temp |= me->SPI.Read(&(me->SPI), 0xFF);
	SPI_CS_WRITE_BIT(me, SET);				    
	
	return Temp;
}   		    
//读取SPI FLASH  
//在指定地址开始读取指定长度的数据
//pBuffer:数据存储区
//ReadAddr:开始读取的地址(24bit)
//NumByteToRead:要读取的字节数(最大65535)
static void W25QXX_Read(W25QXX_TYPE *me, uint8_t* pBuf, uint32_t addr, uint16_t len)   
{ 
 	uint16_t i;
	
	SPI_CS_WRITE_BIT(me, RESET);                            //使能器件   
    me->SPI.Write(&(me->SPI), W25X_ReadData);         //发送读取命令   
    me->SPI.Write(&(me->SPI), (uint8_t)((addr)>>16));  //发送24bit地址    
    me->SPI.Write(&(me->SPI), (uint8_t)((addr)>>8));   
    me->SPI.Write(&(me->SPI), (uint8_t)addr);   
    for (i = 0; i < len; i++) { 
        pBuf[i] = me->SPI.Read(&(me->SPI), 0XFF);   //循环读数  
    }
	SPI_CS_WRITE_BIT(me, SET);                            //取消片选     	      
}  
//SPI在一页(0~65535)内写入少于256个字节的数据
//在指定地址开始写入最大256字节的数据
//pBuffer:数据存储区
//addr:开始写入的地址(24bit)
//len:要写入的字节数(最大256),该数不应该超过该页的剩余字节数!!!	 
static void W25QXX_Write_Page(W25QXX_TYPE *me, uint8_t* pBuf, uint32_t addr, uint16_t len)
{
 	uint16_t i;

	me->Open(me);                  //SET WEL 

	SPI_CS_WRITE_BIT(me, RESET);                            //使能器件   
    me->SPI.Write(&(me->SPI), W25X_PageProgram);      //发送写页命令   
    me->SPI.Write(&(me->SPI), (uint8_t)((addr)>>16)); //发送24bit地址    
    me->SPI.Write(&(me->SPI), (uint8_t)((addr)>>8));   
    me->SPI.Write(&(me->SPI), (uint8_t)addr);   
    for (i = 0; i < len; i++) {
		me->SPI.Write(&(me->SPI), pBuf[i]);//循环写数 
    }
	me->WaitBusy(me);					   //等待写入结束
	SPI_CS_WRITE_BIT(me, SET);                            //取消片选 
} 
//无检验写SPI FLASH 
//必须确保所写的地址范围内的数据全部为0XFF,否则在非0XFF处写入的数据将失败!
//具有自动换页功能 
//在指定地址开始写入指定长度的数据,但是要确保地址不越界!
//pBuffer:数据存储区
//addr:开始写入的地址(24bit)
//len:要写入的字节数(最大65535)
//CHECK OK
static void W25QXX_Write_NoCheck(W25QXX_TYPE *me, uint8_t* pBuf, uint32_t addr, uint16_t len)   
{ 			 		 
	uint16_t pageremain = 0;
	
	pageremain = 256 - addr % 256; //单页剩余的字节数		 	 
	
	if (len <= pageremain) {
		pageremain = len;//不大于256个字节
	}
	while (1) {	   
		W25QXX_Write_Page(me, pBuf, addr, pageremain);
		
		if (len == pageremain) {
			break;//写入结束了
	 	} else {  //len>pageremain
			pBuf += pageremain;
			addr += pageremain;	

			len -= pageremain;			  //减去已经写入了的字节数
			if (len > 256) {
				pageremain=256; //一次可以写入256个字节
			} else { 
				pageremain = len; 	  //不够256个字节了
			}
		}
	};	    
} 
//写SPI FLASH  
//在指定地址开始写入指定长度的数据
//该函数带擦除操作!
//pBuffer:数据存储区
//addr:开始写入的地址(24bit)
//len:要写入的字节数(最大65535)  		   
uint8_t W25QXX_BUF[4096];
static void W25QXX_Write(W25QXX_TYPE *me, uint8_t* pBuf, uint32_t addr, uint16_t len)   
{ 
	uint32_t secpos;
	uint16_t secoff;
	uint16_t secremain;	   
 	uint16_t i;    

	secpos = addr / 4096;//扇区地址 0~511 for w25x16
	secoff = addr % 4096;//在扇区内的偏移
	secremain=4096-secoff;//扇区剩余空间大小   

	if (len <= secremain) {
		secremain=len;//不大于4096个字节
	}
	while (1) {
		me->Read(me, W25QXX_BUF,secpos*4096,4096);//读出整个扇区的内容

		for(i = 0; i < secremain; i++) {  //校验数据
			if (W25QXX_BUF[secoff+i] != 0XFF) {//需要擦除
				break;
			}
		}
		if (i < secremain) {  //需要擦除
			me->EraseSector(me, secpos);//擦除这个扇区
			for (i = 0; i < secremain; i++) {	   //复制
				W25QXX_BUF[i+secoff] = pBuf[i];	  
			}
			me->NoCheck(me, W25QXX_BUF, secpos * 4096, 4096);//写入整个扇区  

		} else {
			me->NoCheck(me, pBuf, addr, secremain);//写已经擦除了的,直接写入扇区剩余区间. 				   
		}
		if (len == secremain) {
			break;//写入结束了
		} else {//写入未结束
			secpos++;//扇区地址增1
			secoff = 0;//偏移位置为0 	 

		   	pBuf += secremain;  //指针偏移
			addr += secremain;//写地址偏移	   
		   	len -= secremain;				//字节数递减
			if (len > 4096) {
				secremain = 4096;	//下一个扇区还是写不完
			} else {
				secremain = len;			//下一个扇区可以写完了
			}	 
		}
	};	 	 
}
//擦除整个芯片
//整片擦除时间:
//W25X16:25s 
//W25X32:40s 
//W25X64:40s 
//等待时间超长...
static void W25QXX_Erase_Chip(W25QXX_TYPE *me)   
{                                             
	me->Open(me);
  	SPI_CS_WRITE_BIT(me, RESET);                            //使能器件   
	me->WaitBusy(me);   
    me->SPI.Write(&(me->SPI), W25X_ChipErase);        //发送片擦除命令       
	me->WaitBusy(me);   				   //等待芯片擦除结束
	SPI_CS_WRITE_BIT(me, SET);                            //取消片选     	 
}   
//擦除一个扇区
//Dst_Addr:扇区地址 0~511 for w25x16
//擦除一个山区的最少时间:150ms
static void W25QXX_Erase_Sector(W25QXX_TYPE *me, uint32_t sectorAddr)   
{   
	sectorAddr *= 4096;
    me->Open(me);                  //SET WEL 	 
  	SPI_CS_WRITE_BIT(me, RESET);                            //使能器件   
    me->WaitBusy(me);   
    me->SPI.Write(&(me->SPI), W25X_SectorErase);      //发送扇区擦除指令 
    me->SPI.Write(&(me->SPI), (uint8_t)((sectorAddr)>>16));  //发送24bit地址    
    me->SPI.Write(&(me->SPI), (uint8_t)((sectorAddr)>>8));   
    me->SPI.Write(&(me->SPI), (uint8_t)sectorAddr);     
    me->WaitBusy(me);   				   //等待擦除完成
	SPI_CS_WRITE_BIT(me, SET);                            //取消片选     	   
}  
//等待空闲
static void W25QXX_Wait_Busy(W25QXX_TYPE *me)   
{   
	while ((me->ReadSR(me) & 0x01) == 0x01);   // 等待BUSY位清空
}  
//进入掉电模式
static void W25QXX_PowerDown(W25QXX_TYPE *me)   
{ 
  	SPI_CS_WRITE_BIT(me, RESET);                            //使能器件   
    me->SPI.Write(&(me->SPI), W25X_PowerDown);        //发送掉电命令  
	SPI_CS_WRITE_BIT(me, SET);                            //取消片选     	      
}   
//唤醒
static void W25QXX_WAKEUP(W25QXX_TYPE *me)   
{  
  	SPI_CS_WRITE_BIT(me, RESET);                            //使能器件   
    me->SPI.Write(&(me->SPI), W25X_ReleasePowerDown);   //  send W25X_PowerDown command 0xAB    
	SPI_CS_WRITE_BIT(me, SET);                            //取消片选     	      
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

	// 使用模拟SPI
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

	

	// 使用内部SPI
	memset(&w25qxx, 0, sizeof(w25qxx));
	w25qxx.SPI.SPIx = SPI2;
	w25qxx.SPI.SPI_CPHA = SPI_CPHA_2Edge;
	w25qxx.SPI.SPI_CPOL = SPI_CPOL_High;
	w25qxx.SPI.SPI_MODE = SPI_MODE_INSIDE;

	W25QXX_CreateDefaultW25QXX(&w25qxx);
	
	w25qxx.Init(&w25qxx);

	printf("FLASH ID:[%X]\r\n", w25qxx.type);
}














