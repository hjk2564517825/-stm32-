#include "led.h"


/*
�������ܣ�LED���ų�ʼ��
����1��viod
����ֵ��void
*/
void LED_PinConfig(void)
{
	GPIO_InitTypeDef p;
	
	//��������ٽ�--�ȿ�����GPIOE  GPIPF�Ĵ���ʱ�ӹ���ʹ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
	
	//ʹ�ÿ⺯����ɶ�GPIO������
	p.GPIO_Pin   = GPIO_Pin_9 | GPIO_Pin_10;
	p.GPIO_Mode  = GPIO_Mode_OUT;//���
	p.GPIO_OType = GPIO_OType_PP;//����
	p.GPIO_Speed = GPIO_Low_Speed;//���� 
	p.GPIO_PuPd  = GPIO_PuPd_NOPULL;//����
	GPIO_Init(GPIOF,&p);
	
	p.GPIO_Pin   = GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_Init(GPIOE,&p);
	
	//�Ƴ�ʼ����ȫ��
	GPIO_WriteBit(GPIOF,GPIO_Pin_9 |GPIO_Pin_10,(BitAction)1);
	GPIO_WriteBit(GPIOE,GPIO_Pin_13|GPIO_Pin_14,(BitAction)1);
}

/*
�������ܣ�LED���ſ��ƺ���
����1��LEDn ָ��LED���� 0~3 ��LED0~LED3
����2��Status ָ����LED��״̬ 0-DISABLE:��  1-ENABLE:��
����ֵ��void
*/
void LED_Control(int LEDn,char Status)
{
	switch(LEDn)
	{
		case 0:
			GPIO_WriteBit(GPIOF,GPIO_Pin_9,(BitAction)!Status);
			break;
		case 1:
			GPIO_WriteBit(GPIOF,GPIO_Pin_10,(BitAction)!Status);
			break;
		case 2:
			GPIO_WriteBit(GPIOE,GPIO_Pin_13,(BitAction)!Status);
			break;
		case 3:
			GPIO_WriteBit(GPIOE,GPIO_Pin_14,(BitAction)!Status);
			break;
	}
}



void TIM14CH1_Init(void)
{
	GPIO_InitTypeDef        GPIOInitStruct;
	TIM_TimeBaseInitTypeDef TIMInitStruct;
	TIM_OCInitTypeDef       OCInitStruct;
	//ʹ��ʱ��GPIOF  TIM14
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14,ENABLE);
	
	//����GPIOF 14����Ϊ����AFģʽ
	GPIOInitStruct.GPIO_Pin   = GPIO_Pin_9;
	GPIOInitStruct.GPIO_Mode  = GPIO_Mode_AF;//����ģʽ
	GPIOInitStruct.GPIO_OType = GPIO_OType_PP;//�������
	GPIOInitStruct.GPIO_Speed = GPIO_Low_Speed;//�����㹻
	GPIOInitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;//Ƭ���������� Ƭ�ڿ��Բ���
	GPIO_Init(GPIOF,&GPIOInitStruct);
	
	//��PF9����Ϊָ������---TIM14
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource9,GPIO_AF_TIM14);
	
	//��ʱ��ʱ����Ԫ
	TIMInitStruct.TIM_Prescaler     = 84-1;//PSC 1us
	TIMInitStruct.TIM_Period        = 1000-1;//ARR 1ms����
	TIMInitStruct.TIM_CounterMode   = TIM_CounterMode_Up;//����
	TIMInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//����Ƶ
	TIM_TimeBaseInit(TIM14,&TIMInitStruct);
	
	//��ʱ��������Ƚ�����
	OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;//PWM1  CNT<CCR �����Ч  ���������Ч
	OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_Low;//�͵�ƽ��Ч
	OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;//PWM���ʹ��
	OCInitStruct.TIM_Pulse       = 0;//Ĭ��100%��  ��ȫ��  
	TIM_OC1Init(TIM14,&OCInitStruct);
	
	//������ʱ��
	TIM_Cmd(TIM14,ENABLE);
}
	























