#include "beep.h"

//�������������ú���
void Beep_PinConfig(void)
{
	GPIO_InitTypeDef GPIOInitStruct;
	
	//GPIOFʱ��ʹ��  Ӧ�ý�PF8����Ϊ���
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
	
	//PA0--KEY0��������
	GPIOInitStruct.GPIO_Pin   = GPIO_Pin_8;
	GPIOInitStruct.GPIO_Mode  = GPIO_Mode_OUT;//���ģʽ
	GPIOInitStruct.GPIO_OType = GPIO_OType_PP;//�������
	GPIOInitStruct.GPIO_Speed = GPIO_Speed_2MHz;//����
	GPIOInitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;//Ƭ���������� Ƭ�ڿ��Բ���
	GPIO_Init(GPIOF,&GPIOInitStruct);
}

