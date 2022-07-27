#include "hcsr04.h"

#define TRIG PBout(10)
#define ECHO PBin(11)

void HCSR04_Init(void)
{
	//GPIO  SysTick�ڲ��ʱ������
	GPIO_InitTypeDef GPIOInitStruct;
	
	//GPIOAʱ��ʹ��  Ӧ�ý�PA0����Ϊ����
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	//PB10--TRIG ���
	GPIOInitStruct.GPIO_Pin  = GPIO_Pin_10;
	GPIOInitStruct.GPIO_Mode = GPIO_Mode_OUT;//����ģʽ
	GPIOInitStruct.GPIO_OType = GPIO_OType_PP;//�������
	GPIOInitStruct.GPIO_Speed = GPIO_Low_Speed;//������ȫ�㹻
	GPIOInitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;//Ƭ���������� Ƭ�ڿ��Բ���
	GPIO_Init(GPIOB,&GPIOInitStruct);
	
	GPIOInitStruct.GPIO_Pin  = GPIO_Pin_11;
	GPIOInitStruct.GPIO_Mode = GPIO_Mode_IN;//����ģʽ
	GPIO_Init(GPIOB,&GPIOInitStruct);
}

static void SysTick_HCSR04Init(void)
{
	//ѡ��SysTickʱ��Դ---21Mhz 1/21us
	SysTick->CTRL &= ~(0x01<<2);
	
	//�ȹر�
	SysTick->CTRL &= ~0x01;
	
	//LOAD
	SysTick->LOAD = 1000000;
	SysTick->VAL  = 0;
	
}

//��ຯ��
float HCSR04_GetDistance(void)
{
	//������������������
	TRIG = 1;
	delay_us(15);//TRIG��������10us
	TRIG = 0;
	
	//����SysTick
	SysTick_HCSR04Init();//��1000000��ʼ�ݼ�
	
	//�ȴ�ECHo����
	while(ECHO == 0);
	//��ʼ��ʱ--�����δ�ʱ��
	SysTick->CTRL |= 0x01;
	
	//�ȵ�ECHO������
	while(ECHO == 1);
	//�ر�SysTick---SysTick->VAL����
	SysTick->CTRL &= ~0x01;

	//����VAL  340m/s  0.34mm/us
	//�Ƶ����������(1000000-(SysTick->VAL)) ��λ����
	//t = (1000000-(SysTick->VAL))*(1/21)    ��λ��us
	//s = t*v = t*0.34
	//s = s/2  ��Ϊ�������˫��
	return ((1000000-(SysTick->VAL))/21)*0.17;
}


