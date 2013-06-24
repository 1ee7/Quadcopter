/* #include "module_motor.h" */

#ifndef __MODULE_MOTOR_H
#define __MODULE_MOTOR_H

#include "stm32f4xx.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
#define PWM_MOTOR_MAX 2000  // 32~80%
#define PWM_MOTOR_MED 1400
#define PWM_MOTOR_MIN 800

#define PWM_SEVRO_MAX 2500
#define PWM_SEVRO_MED 1500
#define PWM_SEVRO_MIN 500

#define PWM_Motor1 TIM3->CCR4	// �L�� PWM
#define PWM_Motor2 TIM3->CCR3	// �L�� PWM
#define PWM_Motor3 TIM3->CCR2	// �L�� PWM
#define PWM_Motor4 TIM3->CCR1	// �L�� PWM
#define PWM_SevroA TIM2->CCR1	// ���A PWM
#define PWM_SevroB TIM2->CCR2	// ���A PWM
/*=====================================================================================================*/
/*=====================================================================================================*/
void Motor_Config( void );
void Motor_Control( u16, u16, u16, u16 );
void Sevro_Control( u16, u16 );
/*=====================================================================================================*/
/*=====================================================================================================*/
#endif
