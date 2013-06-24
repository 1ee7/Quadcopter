/*=====================================================================================================*/
/*=====================================================================================================*/
#include "stm32f4_system.h"
#include "stm32f4_spi.h"
#include "module_ssd1963.h"
#include "module_xpt2046.h"
#include "algorithm_bubblesort.h"
/*=====================================================================================================*/
/*=====================================================================================================*
**��� : Touch_Init
**�\�� : Ĳ����l��
**��J : *PEN
**��X : None
**�ϥ� : Touch_Init(&PEN);
**=====================================================================================================*/
/*=====================================================================================================*/
void Touch_Init( PEN_Struct *PEN )
{
	PEN->Sta = 0;
	PEN->X = 0;
	PEN->Y = 0;
	PEN->ADC_X = 0;
	PEN->ADC_Y = 0;
	PEN->ADC_XMax = 31500;
	PEN->ADC_XMin = 1100;
	PEN->ADC_YMax = 30000;
	PEN->ADC_YMin = 2200;
}
/*=====================================================================================================*/
/*=====================================================================================================*
**��� : XPT2046_Config
**�\�� : XPT2046 �]�w & �t�m
**��J : None
**��X : None
**�ϥ� : XPT2046_Config;
**=====================================================================================================*/
/*=====================================================================================================*/
void XPT2046_Config( void )
{
	GPIO_InitTypeDef GPIO_InitStruct;
	SPI_InitTypeDef SPI_InitStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
  NVIC_InitTypeDef NVIC_InitStruct;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource13,  GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource14,  GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource15,  GPIO_AF_SPI2);

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource12);
	EXTI_InitStruct.EXTI_Line = EXTI_Line12;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);

	NVIC_InitStruct.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x0F;	// �u���ų̧C
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x0E;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);

	/* TPCS PB12 */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	/* SCK PB13 */	/* MISO PB14 */	/* MOSI PB15 */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	/* PEN PA12 */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	TFT_TPCS = 1;

	SPI_I2S_DeInit(SPI2);
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;		// ���u�����u
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;												// �D�Ҧ�
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;										// 8��ƾڼҦ�
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;														// �Ŷ��� SCK = 0
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;													// ��1����u���ġA�W�ɪu�����ˮɨ�
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;														// NSS�H���ѳn�󲣥�
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;	// 
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;										// ����b�e
	SPI_InitStruct.SPI_CRCPolynomial = 7;
	SPI_Init(SPI2, &SPI_InitStruct);

	SPI_Cmd(SPI2, ENABLE);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**��� : Touch_Read
**�\�� : 
**��J : WriteCom
**��X : ReadData
**�ϥ� : 
**=====================================================================================================*/
/*=====================================================================================================*/
u16 Touch_Read( u8 WriteCom )
{
	u8 Temp = 0;
	u16 ReadData = 0;

	TFT_TPCS = 0;

	SPI_RW(SPI2, WriteCom);
	Delay_1us(6);
	Temp = SPI_RW(SPI2, 0x00);
	Delay_1us(1);
	ReadData = Temp << 8;
	Temp = SPI_RW(SPI2, 0x00);
	Delay_1us(1);
	ReadData |= Temp;

	TFT_TPCS = 1;

	return (ReadData);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**��� : Touch_ReadXY
**�\�� : 
**��J : PEN
**��X : None
**�ϥ� : 
**=====================================================================================================*/
/*=====================================================================================================*/
void Touch_ReadXY( PEN_Struct *PEN )
{
	u16 i = 0;
	u16 TouchBuf[2][READ_TIMES] = {0};
	u32 AveSum[2] = {0};

	for(i=0; i<READ_TIMES; i++) {
		TouchBuf[0][i] = Touch_Read(Touch_ReadX);
		TouchBuf[1][i] = Touch_Read(Touch_ReadY);
	}

	BubbleSortU16(READ_TIMES, TouchBuf[0]);
	BubbleSortU16(READ_TIMES, TouchBuf[1]);

	for(i=READ_DISCARD; i<(READ_TIMES-READ_DISCARD); i++) {
		AveSum[0] += TouchBuf[0][i];
		AveSum[1] += TouchBuf[1][i];
	}

	i = READ_TIMES-2*READ_DISCARD;

	PEN->ADC_X = AveSum[0]/i;
	PEN->ADC_Y = AveSum[1]/i;
}
/*=====================================================================================================*/
/*=====================================================================================================*
**��� : TouchTransform
**�\�� : 
**��J : PEN
**��X : None
**�ϥ� : 
**=====================================================================================================*/
/*=====================================================================================================*/
void Touch_Transform( PEN_Struct *PEN )
{
	double TouchL = 0;
	double TouchW = 0;

	TouchL = ((double)fabs(PEN->ADC_X - PEN->ADC_XMin))/((double)(PEN->ADC_XMax - PEN->ADC_XMin));
	TouchW = ((double)fabs(PEN->ADC_Y - PEN->ADC_YMin))/((double)(PEN->ADC_YMax - PEN->ADC_YMin));

	TouchL = TouchL*((double)TFT_L)+0.5;
	TouchW = TouchW*((double)TFT_W)+0.5;

	if(TouchL>=TFT_L)
		TouchL = TFT_L;
	else if(TouchL<1)
		TouchL = 1;
	if(TouchW>=TFT_W)
		TouchW = TFT_W;
	else if(TouchW<1)
		TouchW = 1;

	PEN->X = TFT_L - (u16)(TouchL);
	PEN->Y = TFT_W - (u16)(TouchW);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**��� : Touch_CorrectPoint
**�\�� : Draw Correct Point
**��J : CoordiX, CoordiY
**��X : None
**�ϥ� : Touch_CorrectPoint(CoordiX, CoordiY)
**=====================================================================================================*/
/*=====================================================================================================*/
void Touch_CorrectPoint( u16 CoordiX, u16 CoordiY, u32 Color )
{
	TFT_DrawRectangle(CoordiX-3, CoordiY-3, 6, 6, Color);
	TFT_DrawRectangle(CoordiX-6, CoordiY-6, 12, 12, Color);
	TFT_DrawLineX(CoordiX-9, CoordiY, 19, Color);
	TFT_DrawLineY(CoordiX, CoordiY-9, 19, Color);
}
/*=====================================================================================================*/
/*=====================================================================================================*/
