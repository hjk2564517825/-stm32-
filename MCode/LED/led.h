#ifndef __LED_H //��ֹͬһ��.c�ļ����ظ�����
#define __LED_H

#include "stm32f4xx.h"


//���������б�
void LED_PinConfig(void);
void LED_Control(int LEDn,char Status);

//��ʱ�����PWM�صƳ�ʼ������
void TIM14CH1_Init(void);

#endif

