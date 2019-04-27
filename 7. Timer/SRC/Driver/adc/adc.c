/*
 *  ADC.c - Analog to Digital Converter  ($vision: V1.0$)
 *
 *  Copyright (C) 2001, 2002 Will Liu <liushenglin@cryo-service.com.cn>
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * @file    ADC.c
 * @author  sqdtek team
 * @version V1.0.0
 * @date    2017-06-07
 *
 * This program developed by Beijing SQD Technology CO.,LTD.;
 * @brief   This file defines ADC1,2 hardware drivers.
 *
 *   This file is confi dential. Recipient(s) named above is(are) obligated
 * to maintain secrecy and is(are) not permitted to disclose the contents
 * of this communication to others. Thanks!
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

#include "ADC/adc.h"
#include "string.h"

static ADC_InitTypeDef _adc_init_struct = {0};
static void ADCx_Init(ADC_TYPE *me) {
    ADC_InitTypeDef ADC_InitStructure;
    GPIO_TYPE gpio;
	ADC_TypeDef *ADCx = me->ADC;
	uint16_t ADCx_Channel = me->channelNum;
	
	if (!(ADCx == ADC1 || ADCx == ADC2 || ADCx == ADC3)) {
		printf("ADC Type error\r\n");
		return ;
	} 
	
	if (ADCx_Channel > 15) {
		printf("ADC channel error\r\n");
		return ;
	} 
	
	

	if (ADCx == ADC1) {
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	} else if (ADCx == ADC2) {
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
	} else if (ADCx == ADC3) {
		if ((ADCx_Channel <= 3) || (ADCx_Channel >= 10 && ADCx_Channel <= 13)) {
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);
		} else {
			printf("ADC3 Channel Num Error\r\n");
			printf("0-3, 10-13\r\n");
		}
	} 
	
	 
	if (ADCx_Channel <= 7) {
		/* A0...A7  ==>  CH0...CH7 */
		gpio.Port = GPIOA;
		gpio.GPIOInitTypeStruct.GPIO_Pin = ADCx_Channel - 0 + GPIO_Pin_0;
	} else if (ADCx_Channel <= 9){
		/* B0...B1  ==>  CH8...CH9 */
		gpio.Port = GPIOB;
	 	gpio.GPIOInitTypeStruct.GPIO_Pin = ADCx_Channel - 8 + GPIO_Pin_0;
	} else if (ADCx_Channel <= 15){
		gpio.Port = GPIOC;
		gpio.GPIOInitTypeStruct.GPIO_Pin = ADCx_Channel - 10 + GPIO_Pin_0;
	}
	gpio.GPIOInitTypeStruct.GPIO_Mode = GPIO_Mode_AIN;
	me->gpio = gpio;
	GPIOX_Init(gpio);
	
	
	if (memcmp(&(me->ADC_InitStru), &_adc_init_struct, sizeof(_adc_init_struct)) == 0) {
		ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
		ADC_InitStructure.ADC_ScanConvMode = DISABLE;
		ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
		ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
		ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
		ADC_InitStructure.ADC_NbrOfChannel = 1;
	} else {
		ADC_InitStructure = me->ADC_InitStru;
	}

	ADC_Init(ADCx, &ADC_InitStructure);

    RCC_ADCCLKConfig(RCC_PCLK2_Div6);//
}

static void ADC_Open(ADC_TYPE const *me) {	
	ADC_TypeDef * ADCx = me->ADC;

	
	ADC_Cmd(ADCx, ENABLE);

	ADC_ResetCalibration(ADCx);
	while(ADC_GetResetCalibrationStatus(ADCx));

	ADC_StartCalibration(ADCx);
	while(ADC_GetCalibrationStatus(ADCx));
}

static void ADC_Close(ADC_TYPE const *me) {
	ADC_Cmd(me->ADC, DISABLE);
}



static uint16_t ADC_GetValue(ADC_TypeDef* ADCx, uint16_t ADCx_Channel) {
    __IO uint16_t DataValue;   
	
	ADC_RegularChannelConfig(ADCx, ADCx_Channel, 1, ADC_SampleTime_55Cycles5); // (28.5+12.5) / 12 = 41/12 = 3.42uS

	ADC_SoftwareStartConvCmd(ADCx, ENABLE);
    
    while(!ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC));
	
	DataValue = ADC_GetConversionValue(ADCx); 
	
	return DataValue; 
}

static uint16_t ADC_ReadValue(ADC_TYPE const *me) {
	return ADC_GetValue(me->ADC, me->channelNum);
}
static float ADC_ReadVoltage(ADC_TYPE const *me) {
	return me->ReadValue(me) * 3300 / 4096;   // 最高电压3300mV，12bits精度
}

void ADC_CreateDefaultADC(ADC_TYPE *me) {
	me->Init = ADCx_Init;
	me->Open = ADC_Open;
	me->Close = ADC_Close;
	me->ReadValue = ADC_ReadValue;
	me->ReadVoltage = ADC_ReadVoltage;
}



static void delay_us(uint32_t us) {
	uint8_t i;

	while (us--) {
		for (i = 0; i < 10; i++) {
			;
		}
	}
}

static void delay_ms(uint32_t ms) {
	while (ms--) {
		delay_us(1000);
	}
}

void ADC_Demo(void) {
    ADC_TYPE adc[16] = {0};
	int i = 0;
	
	for (i = 0; i < 16; i++) {
		adc[i].ADC = ADC1;
		adc[i].channelNum = ADC_Channel_0 + i;
		ADC_CreateDefaultADC(&(adc[i]));
		adc[i].Init(&(adc[i]));
		adc[i].Open(&(adc[i]));
	}
	
	
	
	while (1) {
		delay_ms(1000);
		for (i = 0; i < 16; i++) {
			printf("adc[%02d]: %04x   voltage:%f mV\r\n", i, adc[i].ReadValue(&(adc[i])), adc[i].ReadVoltage(&(adc[i])));
		}
	}
}
