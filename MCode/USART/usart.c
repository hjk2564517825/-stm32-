#include "usart.h"



void USART1_Init(unsigned int Baud)
{
	GPIO_InitTypeDef        GPIOInitStruct;
	USART_InitTypeDef       USARTInitStructure;
	NVIC_InitTypeDef        NVICInitStruct;
	
	//��ʱ��ʹ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	//�ڽ�ָ�����Ÿ���ΪUSART1����  PA9 PA10
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
	
	//������PA9 PA10ΪAF���ù���
	GPIOInitStruct.GPIO_Pin   = GPIO_Pin_9 | GPIO_Pin_10;
	GPIOInitStruct.GPIO_Mode  = GPIO_Mode_AF;//����ģʽ
	GPIOInitStruct.GPIO_OType = GPIO_OType_PP;//�������
	GPIOInitStruct.GPIO_Speed = GPIO_Low_Speed;//�����㹻 2Mhz
	GPIOInitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;//Ƭ���������� Ƭ�ڿ��Բ���
	GPIO_Init(GPIOA,&GPIOInitStruct);
	
	//�ܴ��ڵĹ���������ѡ��  ENABLE--8��  DISABLE--16��
	USART_OverSampling8Cmd(USART1, ENABLE);  
	
	//�����ô���1
	USARTInitStructure.USART_BaudRate   = Baud;//����������
	USARTInitStructure.USART_WordLength = USART_WordLength_8b;
	USARTInitStructure.USART_StopBits   = USART_StopBits_1;
	USARTInitStructure.USART_Parity     = USART_Parity_No;//��У��
	USARTInitStructure.USART_Mode       = USART_Mode_Tx | USART_Mode_Rx;//�շ�ģʽ
	USARTInitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������
	USART_Init(USART1, &USARTInitStructure);
	
	//�������ڵĽ����ж�
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	//�ж����ȼ�����
	NVICInitStruct.NVIC_IRQChannel = USART1_IRQn;//37��
	NVICInitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVICInitStruct.NVIC_IRQChannelSubPriority        = 0x00;
	NVICInitStruct.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init(&NVICInitStruct);
	

	//�޿�������
	USART_Cmd(USART1,ENABLE);
}




void USART1_SendString(char *str)
{
	while(*str != '\0')
	{
		USART1->DR = *str;
		str++;
		//�ȵ�ǰ�ַ�����  �������ݼĴ���Ϊ�ձ�־ ---- ��λ����
		while(!(USART1->SR & (0x01<<7)));
	}
}


//�ض���printf�ײ㺯�� fputc
int fputc(int ch, FILE * fp)
{
	USART1->DR = (u8)ch;
	while(!(USART1->SR & (0x01<<7)));//�ȴ���ǰ�ַ�����
	
	return ch;//����ɶ����
}


