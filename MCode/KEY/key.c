#include "key.h"

void KEY_PinConfig(void)
{
	GPIO_InitTypeDef GPIOInitStruct;
	
	//GPIOAʱ��ʹ��  Ӧ�ý�PA0����Ϊ����
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	
	//PA0--KEY0��������
	GPIOInitStruct.GPIO_Pin  = GPIO_Pin_2;
	GPIOInitStruct.GPIO_Mode = GPIO_Mode_IN;//����ģʽ
	GPIOInitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;//Ƭ���������� Ƭ�ڿ��Բ���
	GPIO_Init(GPIOE,&GPIOInitStruct);

}

//�ⲿ�ж�EXTI0����
void KEY_EXTIInit(void)
{
	EXTI_InitTypeDef EXTIInitStruct;
	NVIC_InitTypeDef NVICInitStruct;
	
	//��ʹ��ʱ�� GPIOA SYSCFG 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);

	//�ڽ�����PA0����Ϊ��������
	KEY_PinConfig();

	//��ѡ��ָ���������ӵ�ָ�����ⲿ�ж���
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource2);

	//������EXTI
	EXTIInitStruct.EXTI_Line = EXTI_Line2;
	EXTIInitStruct.EXTI_Mode = EXTI_Mode_Interrupt;//�ж�ģʽ
	EXTIInitStruct.EXTI_Trigger = EXTI_Trigger_Falling;//�½��ش���---���´���
	EXTIInitStruct.EXTI_LineCmd = ENABLE;//�����ж�
	EXTI_Init(&EXTIInitStruct);
	
	//��NVIC�жϹ���---���ոտ������жϵ����ȼ�����
	NVICInitStruct.NVIC_IRQChannel = EXTI2_IRQn;//8��
	NVICInitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVICInitStruct.NVIC_IRQChannelSubPriority        = 0x00;
	NVICInitStruct.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init(&NVICInitStruct);

}

//EXTI2 ���жϷ�����--�ú������жϴ���ʱ��CPUֱ����ת����
void EXTI2_IRQHandler(void)
{
	delay(50);//����
	if(EXTI->PR & (0x01<<2))
	{
		//����ÿ����һ�� LED0��תһ��
		GPIO_ToggleBits(GPIOF,GPIO_Pin_9);
		
		EXTI->PR = (0x01<<2);//��λд1���� д0 ��Ӱ��  ��������
		//EXTI_PR(2) = 1;//д1����
	}
}














