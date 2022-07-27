#include "main.h"

uint8_t dht_buf[5];  //�����ȡ����ʪ������
uint8_t hjk_temp;//���¶�����
uint8_t hjk_humi;//��ʪ������

float	temperature; //�����¶�

__IO uint8_t  RTC_TimeFlag = 0;  //��ʾ��ȡʱ���־  __IO ��ʾvolatile �ױ��


RTC_TimeTypeDef   RTC_TimeStructure;

int hour_num=0;
int minute_num=0;
int second_num=0;

int decimal_bcd_code(int decimal);

//#define DHT11_SET(n)  n ? GPIO_WriteBit(GPIOG,GPIO_Pin_9,Bit_SET): GPIO_WriteBit(GPIOG,GPIO_Pin_9,Bit_RESET)
//#define DHT11_READ	  GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_9)

////��ʪ�ȴ������ĳ�ʼ��
//void  DHT11_Init(void)
//{
//	GPIO_InitTypeDef GPIOInitStruct;
//	
//	//GPIOFʱ��ʹ��  Ӧ�ý�PG9����Ϊ���
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE);
//	
//	//PA0--KEY0��������
//	GPIOInitStruct.GPIO_Pin   = GPIO_Pin_9;
//	GPIOInitStruct.GPIO_Mode  = GPIO_Mode_OUT;		//���ģʽ
//	GPIOInitStruct.GPIO_OType = GPIO_OType_PP;		//�������
//	GPIOInitStruct.GPIO_Speed = GPIO_Speed_100MHz;	//�������
//	GPIOInitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;	//Ƭ���������� Ƭ�ڿ��Բ���
//	GPIO_Init(GPIOG,&GPIOInitStruct);
//	
//	//PG��������ߵ�ƽ ��ʾ����
//	DHT11_SET(1);
//}

////����DHT11����Ϊ����ģʽ
//void DHT11_SetInputMode(void)
//{
//	GPIO_InitTypeDef GPIOInitStruct;
//	
//	//GPIOGʱ��ʹ��  Ӧ�ý�PG9����Ϊ����
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE);
//	
//	//��������
//	GPIOInitStruct.GPIO_Pin  = GPIO_Pin_9;
//	GPIOInitStruct.GPIO_Mode = GPIO_Mode_IN;//����ģʽ
//	GPIOInitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;//Ƭ���������� Ƭ�ڿ��Բ���
//	GPIO_Init(GPIOG,&GPIOInitStruct);
//}

////DHT11��ʼ�ź�
//void DHT11_Start(void)
//{
//	//1.����DHT11����Ϊ���ģʽ
//	DHT11_Init();
//	
//	//2.��PG9�������� ����18ms
//	DHT11_SET(0);
//	delay_ms(20);
//	
//	//3.��PG9��������
//	DHT11_SET(1);
//	delay_us(30);
//}

////DHT11����Ӧ�ź�  ����0 ��ʾδ��Ӧ  
//uint8_t  DHT11_WaitAck(void)
//{
//	int i = 0; //Ϊ�˼�¼��ʱʱ��
//	
//	//1.����PG9����Ϊ����ģʽ
//	DHT11_SetInputMode();
//	
//	//2.������ȡDHT11�ĵ͵�ƽ�ź� �ȴ��͵�ƽ����  ��ʱ���� 100us
//	while( DHT11_READ == 1 && i < 100)
//	{
//		delay_us(1);
//		i++;
//	}
//	
//	if(i>=100)
//	{
//		return 0; //δ��Ӧ 
//	}
//	
//	i = 0; 
//	
//	//3.�ж�DHT11�����Ƿ�����80us
//	while( DHT11_READ == 0 && i < 100)
//	{
//		delay_us(1);
//		i++;
//	}
//	
//	if(i>=100)
//	{
//		return 0; //δ��Ӧ 
//	}
//	else
//		return 1; //����Ӧ
//}

////DHT11��ȡ1bit����
//uint8_t DHT11_ReadBit(void)
//{
//	int  i = 0;  
//	
//	//1.�ȴ����ŵ͵�ƽ����
//	while( DHT11_READ == 1 && i < 100)
//	{
//		delay_us(1);
//		i++;
//	}
//	
//	i = 0;
//	//2.�ȴ����Ÿߵ�ƽ����
//	while( DHT11_READ == 0 && i < 100)
//	{
//		delay_us(1);
//		i++;
//	}
//	
//	//3.��ʱһ��ʱ��  27us < �� < 74us
//	delay_us(40);
//	
//	//4.�ж���ʱ֮��ĵ�ƽ״̬
//	if( DHT11_READ == 1)
//		return 1;  //����1  8'b0000_0001
//	else
//		return 0;  //����0  8'b0000_0000
//	
//}

////��ȡDHT11��1���ֽ�
//uint8_t DHT11_ReadByte(void)
//{
//	uint8_t i = 0;
//	uint8_t data = 0; //dataΪ�˱�������  data = 8'b0000_0000
//	
//	//��λ�ȳ�
//	for(i=0;i<8;i++)
//	{
//		data <<= 1;	
//		data |= DHT11_ReadBit(); 			
//	}

//	return data;
//}

////��ȡ��ʪ������  ����0 ��ʾ��������
//uint8_t DHT11_GetVal(uint8_t *pbuf)
//{
//	uint8_t i = 0;
//	
//	//1.���Ϳ�ʼ�ź�
//	DHT11_Start();
//	
//	//2.�ȴ���Ӧ�ź�
//	if( DHT11_WaitAck() == 1 ) //��ʾ��Ӧ
//	{
//		//ѭ�����յ�5���ֽ� 40bit
//		for(i=0;i<5;i++)
//		{
//			pbuf[i] = DHT11_ReadByte();		
//		}
//		//��������У��
//		if( pbuf[0] + pbuf[1] + pbuf[2] + pbuf[3] != pbuf[4] )
//		{
//			return 0;	//��ȡʧ��  ԭ����У������
//		}
//	}
//	else
//		return 0;	//��ȡʧ��  ԭ����δ��Ӧ
//	
//	return 1;

//}

//�������Ź��ĳ�ʼ��
void IWDG_Init(void)
{
	/* Enable write access to IWDG_PR and IWDG_RLR registers */
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	
	/* IWDG counter clock: LSI/32 LSI��32KHZ/32 = 1KHZ --->�൱��1ms����1�� */
	IWDG_SetPrescaler(IWDG_Prescaler_32);
	
	//��������ֵ �൱��250ms���ǳ�ʱʱ��
	IWDG_SetReload(250-1);
	
	/* Reload IWDG counter */
	IWDG_ReloadCounter();
	
	/* Enable IWDG (the LSI oscillator will be enabled by hardware) */
	IWDG_Enable();
}

//RTC������
void RTC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef  EXTI_InitStructure;
	RTC_InitTypeDef   RTC_InitStructure;
	
	/* Enable the PWR APB1 Clock Interface ��PWR�����ʱ�� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

	/* Allow access to BKP Domain ���RTC�Ĵ�����д����*/
	PWR_BackupAccessCmd(ENABLE);
	
	/* Enable the RTC Interrupt ����RTCʱ�ӵĻ����ж����ȼ��Ĳ���*/
	NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn;			//����ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* EXTI configuration ����RTC�Զ����ѵ��ⲿ�ж��ߵĲ���*/
	EXTI_ClearITPendingBit(EXTI_Line22);
	EXTI_InitStructure.EXTI_Line = EXTI_Line22;				//�ж��߱��
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;		//�ж�ģʽ
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;	//��������Ч
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	/* Enable the LSE OSC ʹ��LSE�����ⲿʱ��*/
	RCC_LSEConfig(RCC_LSE_ON);
	
	/* Wait till LSE is ready �ȴ�LSE׼�����*/  
	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);
	
	/* Select the RTC Clock Source ѡ��LSE��ΪRTCʱ��Դ*/
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

	/* Enable the RTC Clock ʹ��RTCʱ��*/
	RCC_RTCCLKCmd(ENABLE);
	
	/* Wait for RTC APB registers synchronisation �ȴ�RTCʱ��ͬ�����*/
	RTC_WaitForSynchro();
	
	
	/*��RTCʱ�ӽ��г�ʼ�� ck_spre(1Hz) = RTCCLK(LSE) /(uwAsynchPrediv + 1)*(uwSynchPrediv + 1)  ����RTC�ڲ�ʱ�� 32768HZ / (127+1) / (255+1) = 1HZ */
	RTC_InitStructure.RTC_AsynchPrediv = 128-1;				//�����첽Ԥ��Ƶ��  32768HZ / 128 = 256HZ 
	RTC_InitStructure.RTC_SynchPrediv  = 256-1;				//����ͬ��Ԥ��Ƶ��  256 / 256 = 1HZ
	RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;	//����24Сʱ��
	RTC_Init(&RTC_InitStructure);

	
	/* Set the Time ����RTC��ʱ��*/
	RTC_TimeStructure.RTC_Hours   = 0x08;	//����16����   ʹ��BCD���ʽ    16���ƣ� 0x29+1 = 0x2A   BCD��:  0x29+1 = 0x30  10:00:00  0x10 --> 10
	RTC_TimeStructure.RTC_Minutes = 0x00;
	RTC_TimeStructure.RTC_Seconds = 0x00;
	RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure);  
	
	/* Disable Wakeup Counter ��ֹ���Ѷ�ʱ��*/
	RTC_WakeUpCmd(DISABLE);
	
	/* Configure the RTC Wakeup Clock source and Counter (Wakeup event each 1 second) ����RTC��������  ��ʾ�����������ڲ�ʱ�ӵ�����  1HZ---> 1��1�� */
	RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_16bits);
	
	/*�����Զ�����ֵ*/
	RTC_SetWakeUpCounter(0);  //RTCCLK = 1HZ  ���Լ���1�λ���1s
	 
	/* Enable the Wakeup Interrupt ��RTC�����ж�*/
	RTC_ITConfig(RTC_IT_WUT, ENABLE);

	/* Enable Wakeup Counter ��RTC���Ѷ�ʱ��*/
	RTC_WakeUpCmd(ENABLE); 
	
	/*���RTCʱ�ӵĻ��ѱ�־ */
	RTC_ClearITPendingBit(RTC_IT_WUT);
	
	/*���EXTI22�ⲿ�ж��ߵı�־ */
	EXTI_ClearITPendingBit(EXTI_Line22);
}

int main()
{
	char tbuf[40]={0};//ʱ��
	char tbuf_temp_humi[20]={0};//��ʪ��
	char tbuf_red_Temp[10]={0};//����
	
	//�����жϵ����ȼ�����  ��2���ʾ2bit��ռ���ȼ���0~3����2bit��Ӧ���ȼ���0~3�� �ú�����Ӧ��Ƶ������  Ӧ�÷�������������ĳ�ʼ��֮ǰ
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	USART1_Init(9600);	//����1�ĳ�ʼ��

	RTC_Config();		//RTCʱ�ӵ�����
	
	OLED_Init();			//��ʼ��OLED  
	OLED_Clear() ; 
	//printf("mcu hardware init ok\r\n");
	DHT11_Init();
	IIC_red_Init();

	while(1)
	{
		temperature=Get_Tem_DATA(0x07);					//��ȡ�¶�ֵ
		DHT11_GetVal(dht_buf); //��ȡһ����ʪ������
		hjk_temp = dht_buf[2];
		hjk_humi = dht_buf[0];
		//�������������
		if(RTC_TimeFlag == 1)
		{
			RTC_TimeFlag = 0; 
			
			/* Get the current Time ��ȡʱ�� BCD��ʽ  (0x10 / 16) * 10 + (0x10%16) = 10   */
			RTC_GetTime(RTC_Format_BCD, &RTC_TimeStructure);
			sprintf(tbuf,"%02x:%02x:%02x",RTC_TimeStructure.RTC_Hours,RTC_TimeStructure.RTC_Minutes,RTC_TimeStructure.RTC_Seconds);	
			OLED_ShowString(1,0,tbuf,16);
			
			sprintf(tbuf_temp_humi,"temp:%d,humi:%d",hjk_temp,hjk_humi);
			OLED_ShowString(1,3,tbuf_temp_humi,16);
			
			sprintf(tbuf_red_Temp,"%2.2f",temperature);
			OLED_ShowString(1,6,tbuf_red_Temp,16);
			OLED_ShowCHinese(45,6,3);
			//printf("%02x:%02x:%02x \r\n",RTC_TimeStructure.RTC_Hours,RTC_TimeStructure.RTC_Minutes,RTC_TimeStructure.RTC_Seconds);
		}
		

	}
	
	//return 0;
}

//RTCʱ�ӵĻ����жϷ�����  ÿ��1s����һ��
void RTC_WKUP_IRQHandler(void)
{
  if(RTC_GetITStatus(RTC_IT_WUT) != RESET)
  {  
		RTC_TimeFlag = 1;
	  
		RTC_ClearITPendingBit(RTC_IT_WUT);
		EXTI_ClearITPendingBit(EXTI_Line22);
  }
}

void USART1_IRQHandler(void)
{
	char buf1[10] = {0};
	char buf2[10] = {0};
	char buf3[10] = {0};
	char buf4[10] = {0};
	char buf5[10] = {0};
	char* p = "+=:";
	static char Rdata[18]={0};//ÿ�ν���һ���ַ���������жϺ����������Ҫ����Ϊ��̬��ȫ��
	static int i = 0;
	if(USART1->SR & (0x01<<5))
	{
		Rdata[i] = USART1->DR;
		i++;
		if(i==17)
		{
			i=0;
			
			//set
			char* ret = strtok(Rdata, p);//�ҵ������еĵ�һ����ǲ���Ϊ'\0'��strtok���������������ַ������е�λ��
			printf("%s\n", ret);//���س�ʼλ��
			strcpy(buf1, ret);
			
			//time
			ret = strtok(NULL, p);//�ҵ���һ����Ǹ�Ϊ'\0',����λ��
			printf("%s\n", ret);//������һ����ǵ�λ��
			strcpy(buf2, ret);
			
			//ʱ
			ret = strtok(NULL, p);
			printf("%s\n", ret);
			strcpy(buf3, ret);
			
			//��
			ret = strtok(NULL, p);
			printf("%s\n", ret);
			strcpy(buf4, ret);
			
			//��
			ret = strtok(NULL, p);
			printf("%s\n", ret);
			strcpy(buf5, ret);
			 
			if(strcmp(buf2,"time")==0)
			{
				int hour_set;
				int min_set;
				int sed_set;
				
				hour_set=atoi(buf3);
				min_set=atoi(buf4);
				sed_set=atoi(buf5);
				
				hour_num=hour_set;
				minute_num=min_set;
				second_num=sed_set;
				
				hour_num=decimal_bcd_code(hour_num);
				minute_num=decimal_bcd_code(minute_num);
				second_num=decimal_bcd_code(second_num);
				
				/* Set the Time ����RTC��ʱ��*/
				RTC_TimeStructure.RTC_Hours   = hour_num;	//����16����   ʹ��BCD���ʽ    16���ƣ� 0x29+1 = 0x2A   BCD��:  0x29+1 = 0x30  10:00:00  0x10 --> 10
				RTC_TimeStructure.RTC_Minutes = minute_num;
				RTC_TimeStructure.RTC_Seconds = second_num;
				RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure);  
					
			}
			
		}			
			
	}
}

int decimal_bcd_code(int decimal)//ʮ����תBCD��
{
	int sum = 0, i;//i����������sum���ص�BCD��
	for ( i = 0; decimal > 0; i++)
	{
		sum |= ((decimal % 10 ) << ( 4*i));
		decimal /= 10;
	}
	return sum;
}

