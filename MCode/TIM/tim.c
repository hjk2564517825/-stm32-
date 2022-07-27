#include "tim.h"


void TIM6_Init(void)
{
	TIM_TimeBaseInitTypeDef TIMInitStruct;
	NVIC_InitTypeDef NVICInitStruct;
	
	//ʹ��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);
	
	//���ö�ʱ��ʱ����Ԫ
//	TIM6->PSC = 8400-1;//84Mhz/8400 =10000Hz 1/10ms 0.1ms/���� 
//	TIM6->ARR = 65536-1;//10000*0.1ms = 1000ms = 1s
	TIMInitStruct.TIM_Prescaler     = 8400-1;//PSC
	TIMInitStruct.TIM_Period        = 10000-1;//ARR 0.2s
	TIMInitStruct.TIM_CounterMode   = TIM_CounterMode_Up;//����
	TIMInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//����Ƶ
	TIM_TimeBaseInit(TIM6,&TIMInitStruct);
	
	
	//����TIM6 �����ж�
	TIM6->DIER |= 0x01;
	
	//NVIC�жϹ���---���ոտ������жϵ����ȼ�����
	NVICInitStruct.NVIC_IRQChannel = TIM6_DAC_IRQn;//54��
	NVICInitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVICInitStruct.NVIC_IRQChannelSubPriority        = 0x00;
	NVICInitStruct.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init(&NVICInitStruct);
	
	//������ʱ��������
	TIM6->CR1 |= 0x01;
}

void TIM6_DAC_IRQHandler(void)
{
	
	if(TIM6->SR & 0x01) //�жϸ��±�־λ�Ƿ���λ
	{
		//ʱ�䵽��
		GPIO_ToggleBits(GPIOF,GPIO_Pin_9);
		TIM6->SR = 0;//д0����  CNT���Զ�����Ϊ��ֵ0
	}

}



void TIM7_Init(void)
{
	TIM_TimeBaseInitTypeDef TIMInitStruct;
	//ʹ��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);
	
	//���ö�ʱ��ʱ����Ԫ
//	TIM6->PSC = 8400-1;//84Mhz/8400 =10000Hz 1/10ms 0.1ms/���� 
//	TIM6->ARR = 65536-1;//10000*0.1ms = 1000ms = 1s
	TIMInitStruct.TIM_Prescaler     = 8400-1;//PSC
	TIMInitStruct.TIM_Period        = 10000-1;//ARR
	TIMInitStruct.TIM_CounterMode   = TIM_CounterMode_Up;//����
	TIMInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//����Ƶ
	TIM_TimeBaseInit(TIM7,&TIMInitStruct);
	
	
	//������ʱ��������
	TIM7->CR1 |= 0x01;
}

