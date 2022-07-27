#include "delay.h"

//����ʱ
void delay(int tim)
{
	int i;
	while(tim--)
		for(i=38400;i>0;i--);
}

//�ο�SysTickʵ�ֵ�us��׼��ʱ
void delay_us(int n)
{
	//ѡ��SysTickʱ��Դ---21Mhz 1/21us
	SysTick->CTRL &= ~(0x01<<2);
	
	//������Ҫ��ʱ��us�õ�Ҫ�Ƶ��������
	SysTick->LOAD  = 21*n-1;
	
	//���������ֵ����VAL��  VALд���κ�����������-->�Զ���LOAD����ֵ����
	//�ò���Ҳ���Խ�CTRL�ı�־λ����
	SysTick->VAL  = 0;
	
	//����SysTick������
	SysTick->CTRL |= 0x01;
	
	//�ȴ�ָ��ʱ�䵽��----����ʱ����--ÿ�ζ��궼�����־λ
	while(!(SysTick->CTRL & (0x01<<16)));
	
	//�ر�SysTick
	SysTick->CTRL &= ~0x01;
}

//LOAD���16777216/21000 >= n    n<=798ms
void delay_ms(int n)
{
	//ѡ��SysTickʱ��Դ---21Mhz 1/21us
	SysTick->CTRL &= ~(0x01<<2);
	
	//������Ҫ��ʱ��us�õ�Ҫ�Ƶ��������
	SysTick->LOAD  = 21000*n-1;
	
	//���������ֵ����VAL��  VALд���κ�����������-->�Զ���LOAD����ֵ����
	//�ò���Ҳ���Խ�CTRL�ı�־λ����
	SysTick->VAL  = 0;
	
	//����SysTick������
	SysTick->CTRL |= 0x01;
	
	//�ȴ�ָ��ʱ�䵽��----����ʱ����--ÿ�ζ��궼�����־λ
	while(!(SysTick->CTRL & (0x01<<16)));
	
	//�ر�SysTick
	SysTick->CTRL &= ~0x01;

}

void delay_s(int n)
{
	while(n--)
	{
		delay_ms(500);
		delay_ms(500);
	}
}

//void delay_ms(int n)
//{
//	TIM_TimeBaseInitTypeDef TIMInitStruct;
//	
//	//ʹ��ʱ��
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);
//	
//	//���ö�ʱ��ʱ����Ԫ
//	TIMInitStruct.TIM_Prescaler     = 8400-1;//PSC 0.1ms/����  10�����幻1ms
//	TIMInitStruct.TIM_Period        = 65536-1;//���ǲ�ϣ��CNT������׼��ʱʱ����ǰ���
//	TIMInitStruct.TIM_CounterMode   = TIM_CounterMode_Up;//����
//	TIMInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//����Ƶ
//	TIM_TimeBaseInit(TIM6,&TIMInitStruct);
//	
//	TIM6->CNT = 0;//������
//	
//	//����������
//	TIM6->CR1 |= 0x01;
//	
//	//10*n<=65536  n<= 6553.6ms
//	while(TIM6->CNT != (10*n-1));//�ȴ�CNT�Ƶ�ָ������ֵ---��ʱһ��ʱ��

//}




////��TIM6ʵ��us������ʱ�����Ը��ݴ��ξ�����ʱ����us
//void delay_us(int n)
//{
//	
//	
//	
//	
//}


////ʵ��s������ʱ�����Ը��ݴ��ξ�����ʱ����us
//void delay_s(int n)
//{
//	
//	
//	
//	
//}


