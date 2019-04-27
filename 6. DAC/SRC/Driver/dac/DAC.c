#include "DAC/DAC.h"
#include "string.h"

static const uint32_t dacChannelConst[3] = {0, DAC_Channel_1, DAC_Channel_2};
static uint32_t DAC_GetChannel(uint32_t num) {
	if (num > 2) {
		num = 0;
	}
	
	return dacChannelConst[num];
}

static const DAC_InitTypeDef __dac_init_type = {0};
static void DACx_Init(DAC_TYPE *me) {
	DAC_InitTypeDef DAC_InitStructure;
	uint32_t DAC_Channel;

	if (me->channelNum == 1) {
		me->gpio.GPIOInitTypeStruct.GPIO_Pin = GPIO_Pin_4;  // DAC_OUT1
	} else if (me->channelNum == 2) {
		me->gpio.GPIOInitTypeStruct.GPIO_Pin = GPIO_Pin_5;  // DAC_OUT2
	}
	me->gpio.Port = GPIOA;
	me->gpio.GPIOInitTypeStruct.GPIO_Speed = GPIO_Speed_50MHz;
	me->gpio.GPIOInitTypeStruct.GPIO_Mode = GPIO_Mode_AIN;  // DAC 需要将模式调至模拟输入(AIN)模式
	me->gpio.IntValue = SET;
	GPIOX_Init(me->gpio);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

	if (memcmp(&(me->DAC_InitStru), &__dac_init_type, sizeof(__dac_init_type)) == 0) {
		DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;		// 关闭缓存输出
		DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;				// 软件触发
		DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;  // 三角赋值等于1
		DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;		// 关闭波形生成
	} else {
		DAC_InitStructure = me->DAC_InitStru;
	}
	
	DAC_Channel = DAC_GetChannel(me->channelNum);
	DAC_Init(DAC_Channel, &DAC_InitStructure);
	
}


static void DAC_Open(DAC_TYPE *me) {
	uint32_t DAC_Channel;

	DAC_Channel = DAC_GetChannel(me->channelNum);
	
	DAC_Cmd(DAC_Channel, ENABLE);
}


static void DAC_Close(DAC_TYPE *me) {
	uint32_t DAC_Channel;

	DAC_Channel = DAC_GetChannel(me->channelNum);
	
	DAC_Cmd(DAC_Channel, DISABLE);
}


static void DAC_WriteValue(DAC_TYPE *me, uint16_t value) {
	if (me->channelNum == 1) {
		DAC_SetChannel1Data(DAC_Align_12b_R, value);
	} else if (me->channelNum == 2) {
		DAC_SetChannel2Data(DAC_Align_12b_R, value);
	}
}

static void DAC_WriteVoltage(DAC_TYPE *me, float voltage) {
	uint32_t value = voltage * 4096 / 3300;

	me->WriteValue(me, value);
}

static uint32_t DAC_ReadValue(DAC_TYPE *me) {
	uint32_t channel = DAC_GetChannel(me->channelNum);

	return DAC_GetDataOutputValue(channel);
}

static float DAC_ReadVoltage(DAC_TYPE *me) {
	return me->ReadValue(me) * 3300 / 4096;
}

void DAC_CreateDefaultDAC(DAC_TYPE *me) {
	me->Init = DACx_Init;
	me->Open = DAC_Open;
	me->Close = DAC_Close;
	me->WriteValue = DAC_WriteValue;
	me->WriteVoltage = DAC_WriteVoltage;
	me->ReadValue = DAC_ReadValue;
	me->ReadVoltage = DAC_ReadVoltage;
}




static void delay_us(uint32_t us) {
	int i;

	while (us--) {
		for (i = 0; i < 2; i++) {
			;
		}
	}
}

static void delay_ms(uint32_t ms) {
	while (ms--) {
		delay_us(1000);
	}
}


void DAC_Demo(void) {
	DAC_TYPE dac[2] = {0};
	int i = 0;
	int vol = 100;
	
	for (i = 0; i < 2; i++) {
		dac[i].channelNum = i+1;
		DAC_CreateDefaultDAC(&(dac[i]));
		dac[i].Init(&(dac[i]));
		dac[i].Open(&(dac[i]));
	}
	while (1) {
		for (vol = 0; vol < 100; vol++) {
			for (i = 0; i < 2; i++) {
				dac[i].WriteVoltage(&(dac[i]), vol*(i+1));
				delay_ms(1000);
			}
		}
	}
}

