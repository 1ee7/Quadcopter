/*=====================================================================================================*/
/*=====================================================================================================*/
#include "stm32f4_system.h"
#include "module_keyBoard.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
vu16 KeyCtrl[KeyNums];

extern vu8 fTime_Sec;
extern vu8 fTime_Min;
/*=====================================================================================================*/
/*=====================================================================================================*
**��� : LED_Config
**�\�� : LED �]�w & �t�m
**��J : None
**��X : None
**�ϥ� : LED_Config();
**=====================================================================================================*/
/*=====================================================================================================*/
void LED_Config( void )
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB |
												 RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4  | RCC_APB1Periph_TIM5,  ENABLE);

  GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_TIM5);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_TIM5);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_TIM4);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_TIM4);

	/* LED3 PA4 */	/* LED1 PA8 */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_8;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOA, &GPIO_InitStruct);
	/* LED4 PB5 */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOB, &GPIO_InitStruct);
	/* LED2 PD2 */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOD, &GPIO_InitStruct);

	/* LED6 PA2 */	/* LED8 PA3 */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(GPIOA, &GPIO_InitStruct);
	/* LED7 PB8 */	/* LED5 PB9 */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOB, &GPIO_InitStruct);

	TIM_DeInit(TIM4);
	TIM_DeInit(TIM5);

/************************** PWM Output **************************************/
	/* �]�w TIM4 TIM5 Time Base */
	TIM_TimeBaseStruct.TIM_Period = (2000-1);										// �g�� = 20ms
	TIM_TimeBaseStruct.TIM_Prescaler = (840-1);									// ���W8400 = 100kHz ( 10us )
	TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;		// �W��
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStruct);
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStruct);

	/* �]�w TIM4 TIM5 OC */
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;							// �t�m�� PWM1 �Ҧ�
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;	// �P�� OC
	TIM_OCInitStruct.TIM_Pulse = 2000;													// �]�m���ܭ�
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;			// ��p�ƭȤp�� MOTOR_PWM_Min �ɬ����q��
	TIM_OC3Init(TIM4, &TIM_OCInitStruct);												// ��l�� TIM4 OC3
	TIM_OC4Init(TIM4, &TIM_OCInitStruct);												// ��l�� TIM4 OC4
	TIM_OC3Init(TIM5, &TIM_OCInitStruct);												// ��l�� TIM5 OC3
	TIM_OC4Init(TIM5, &TIM_OCInitStruct);												// ��l�� TIM5 OC4
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);						// �P�� TIM4 OC3 �w�˸�
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);						// �P�� TIM4 OC4 �w�˸�
	TIM_OC3PreloadConfig(TIM5, TIM_OCPreload_Enable);						// �P�� TIM5 OC3 �w�˸�
	TIM_OC4PreloadConfig(TIM5, TIM_OCPreload_Enable);						// �P�� TIM5 OC4 �w�˸�

	/* �Ұ� TIM4 TIM5 */
	TIM_ARRPreloadConfig(TIM4, ENABLE);													// �P�� TIM4 �����H�s��ARR
	TIM_ARRPreloadConfig(TIM5, ENABLE);													// �P�� TIM5 �����H�s��ARR
	TIM_Cmd(TIM4, ENABLE);																			// �P�� TIM4
	TIM_Cmd(TIM5, ENABLE);																			// �P�� TIM5
}
/*=====================================================================================================*/
/*=====================================================================================================*
**��� : KEY_Config
**�\�� : KEY �]�w & �t�m
**��J : None
**��X : None
**�ϥ� : KEY_Config();
**=====================================================================================================*/
/*=====================================================================================================*/
void KEY_Config( void )
{
	GPIO_InitTypeDef GPIO_InitStruct;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	/* KEY PA5 */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(GPIOA, &GPIO_InitStruct);

//   // TEST
//   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
// 	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14;
//   GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
//   GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
//   GPIO_Init(GPIOC, &GPIO_InitStruct);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**��� : KeyBoard_Read
**�\�� : Ū������
**��J : KeyBoardBuf
**��X : None
**�ϥ� : KeyBoard_Read(RxBuf);
**=====================================================================================================*/
/*=====================================================================================================*/
void KeyBoard_Read( u8* KeyBoardBuf )
{
	KEYR_J    = (u16)KeyBoardBuf[0];
	KEYR_U    = (u16)KeyBoardBuf[1];
	KEYR_D    = (u16)KeyBoardBuf[2];
	KEYR_L    = (u16)KeyBoardBuf[3];
	KEYR_R    = (u16)KeyBoardBuf[4];
	KEYR_S1   = (u16)KeyBoardBuf[5];
	KEYR_S2   = (u16)KeyBoardBuf[6];
	KEYL_J    = (u16)KeyBoardBuf[7];
	KEYL_U    = (u16)KeyBoardBuf[8];
	KEYL_D    = (u16)KeyBoardBuf[9];
	KEYL_L    = (u16)KeyBoardBuf[10];
	KEYL_R    = (u16)KeyBoardBuf[11];
	KEYL_S1   = (u16)KeyBoardBuf[12];
	KEYL_S2   = (u16)KeyBoardBuf[13];
	Exp_Pitch = (u16)((KeyBoardBuf[15] << 8) | KeyBoardBuf[14]);
	Exp_Roll  = (u16)((KeyBoardBuf[17] << 8) | KeyBoardBuf[16]);
// 	JSR_X     = (u16)((KeyBoardBuf[15] << 8) | KeyBoardBuf[14]);
// 	JSR_Y     = (u16)((KeyBoardBuf[17] << 8) | KeyBoardBuf[16]);
	JSR_Z     = (u16)((KeyBoardBuf[19] << 8) | KeyBoardBuf[18]);
  Exp_Yaw   = (u16)((KeyBoardBuf[21] << 8) | KeyBoardBuf[20]);
  Exp_Thr   = (u16)((KeyBoardBuf[23] << 8) | KeyBoardBuf[22]);
// 	JSL_X     = (u16)((KeyBoardBuf[21] << 8) | KeyBoardBuf[20]);
// 	JSL_Y     = (u16)((KeyBoardBuf[23] << 8) | KeyBoardBuf[22]);
	JSL_Z     = (u16)((KeyBoardBuf[25] << 8) | KeyBoardBuf[24]);
//   Exp_Pitch = (u16)((KeyBoardBuf[27] << 8) | KeyBoardBuf[26]);
//   Exp_Roll  = (u16)((KeyBoardBuf[29] << 8) | KeyBoardBuf[28]);
//   Exp_Thr = (u16)((KeyBoardBuf[31] << 8) | KeyBoardBuf[30]);
  fTime_Sec = (u8)(KeyBoardBuf[30]);
  fTime_Min = (u8)(KeyBoardBuf[31]);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**��� : LED_Shine
**�\�� : LED�{�{ 
**��J : LED_Sel, LED_ShineNum, LED_Delay1ms
**��X : None
**�ϥ� : LED_Shine(0x01, 5, 200)
**=====================================================================================================*/
/*=====================================================================================================*/
void LED_Shine( u16 LED_Sel, u16 LED_ShineNum, u16 LED_Delay1ms )
{
	while(LED_ShineNum--) {
		LED1 = ((LED_Sel&0x01) == 0x01) ? 0 : 1;
		LED2 = ((LED_Sel&0x02) == 0x02) ? 0 : 1;
		LED3 = ((LED_Sel&0x04) == 0x04) ? 0 : 1;
		LED4 = ((LED_Sel&0x08) == 0x08) ? 0 : 1;
		LED5 = ((LED_Sel&0x10) == 0x10) ? LED_PWM_Max : LED_PWM_Min;
		LED6 = ((LED_Sel&0x20) == 0x20) ? LED_PWM_Max : LED_PWM_Min;
		LED7 = ((LED_Sel&0x40) == 0x40) ? LED_PWM_Max : LED_PWM_Min;
		LED8 = ((LED_Sel&0x80) == 0x80) ? LED_PWM_Max : LED_PWM_Min;
		Delay_1ms(LED_Delay1ms);
		LED1 = 1;
		LED2 = 1;
		LED3 = 1;
		LED4 = 1;
		LED5 = LED_PWM_Min;
		LED6 = LED_PWM_Min;
		LED7 = LED_PWM_Min;
		LED8 = LED_PWM_Min;
		Delay_1ms(LED_Delay1ms);
	}
}
/*=====================================================================================================*/
/*=====================================================================================================*
**��� : LED_PWM
**�\�� : 
**��J : LED_ShineNum, LED_DelayTime
**��X : 
**�ϥ� : 
**=====================================================================================================*/
/*=====================================================================================================*/
void LED_PWM( u8 LED_Mode , u16 LED_Sel, u16 LED_DelayTime )
{
	u16 i = 0;

	if(LED_Mode == LED_ON) {
		for(i=LED_PWM_Min; i<LED_PWM_Max-10; i=i+2) {
			if((LED_Sel&0x01) == 0x01)	LED5 = i;
			if((LED_Sel&0x02) == 0x02)	LED6 = i;
			if((LED_Sel&0x04) == 0x04)	LED7 = i;
			if((LED_Sel&0x08) == 0x08)	LED8 = i;
			Delay_1us(LED_DelayTime);
		}
	}
	else {
		for(i=LED_PWM_Max; i>LED_PWM_Min+10; i=i-2) {
			if((LED_Sel&0x01) == 0x01)	LED5 = i;
			if((LED_Sel&0x02) == 0x02)	LED6 = i;
			if((LED_Sel&0x04) == 0x04)	LED7 = i;
			if((LED_Sel&0x08) == 0x08)	LED8 = i;
			Delay_1us(LED_DelayTime);
		}
	}
}
/*=====================================================================================================*/
/*=====================================================================================================*
**��� : LED_Shine
**�\�� : 
**��J : LED_Sel, LED_ShineNum, LED_DelayTime
**��X : 
**�ϥ� : 
**=====================================================================================================*/
/*=====================================================================================================*/
void LED_PWM_Shine( u16 LED_Sel, u16 LED_ShineNum, u16 LED_DelayTime )
{
	u16 Tmp_LED_Sel = 0;

	Tmp_LED_Sel = (u8)(LED_Sel>>4);

	while(LED_ShineNum--) {
		LED_PWM(LED_ON, Tmp_LED_Sel, LED_DelayTime);
		LED1 = ((LED_Sel&0x01) == 0x01) ? 0 : 1;
		LED2 = ((LED_Sel&0x02) == 0x02) ? 0 : 1;
		LED3 = ((LED_Sel&0x04) == 0x04) ? 0 : 1;
		LED4 = ((LED_Sel&0x08) == 0x08) ? 0 : 1;
		LED_PWM(LED_OFF, Tmp_LED_Sel, LED_DelayTime);
		LED1 = 1;
		LED2 = 1;
		LED3 = 1;
		LED4 = 1;
	}
}
/*=====================================================================================================*/
/*=====================================================================================================*/
