#include "usart.h"

//����NVIC����
//NVIC_Group:NVIC���� 0~4 �ܹ�5�� 		   
void MY_NVIC_PriorityGroupConfig(u8 NVIC_Group)	 
{ 
	u32 temp,temp1;	  
	temp1=(~NVIC_Group)&0x07;//ȡ����λ
	temp1<<=8;
	temp=SCB->AIRCR;  //��ȡ��ǰ������
	temp&=0X0000F8FF; //�����ǰ����
	temp|=0X05FA0000; //д��Կ��
	temp|=temp1;	   
	SCB->AIRCR=temp;  //���÷���	    	  				   
}
//����NVIC 
//NVIC_PreemptionPriority:��ռ���ȼ�
//NVIC_SubPriority       :��Ӧ���ȼ�
//NVIC_Channel           :�жϱ��
//NVIC_Group             :�жϷ��� 0~4
//ע�����ȼ����ܳ����趨����ķ�Χ!����������벻���Ĵ���
//�黮��:
//��0:0λ��ռ���ȼ�,4λ��Ӧ���ȼ�
//��1:1λ��ռ���ȼ�,3λ��Ӧ���ȼ�
//��2:2λ��ռ���ȼ�,2λ��Ӧ���ȼ�
//��3:3λ��ռ���ȼ�,1λ��Ӧ���ȼ�
//��4:4λ��ռ���ȼ�,0λ��Ӧ���ȼ�
//NVIC_SubPriority��NVIC_PreemptionPriority��ԭ����,��ֵԽС,Խ����	   
void MY_NVIC_Init(u8 NVIC_PreemptionPriority,u8 NVIC_SubPriority,u8 NVIC_Channel,u8 NVIC_Group)	 
{ 
	u32 temp;	
	//MY_NVIC_PriorityGroupConfig(NVIC_Group);//���÷���
	temp=NVIC_PreemptionPriority<<(4-NVIC_Group);	  
	temp|=NVIC_SubPriority&(0x0f>>NVIC_Group);
	temp&=0xf;								//ȡ����λ  
	NVIC->ISER[NVIC_Channel/32]|=(1<<NVIC_Channel%32);//ʹ���ж�λ(Ҫ����Ļ�,�෴������OK) 
	NVIC->IP[NVIC_Channel]|=temp<<4;		//������Ӧ���ȼ����������ȼ�   	    	  				   
} 
void UsartInit(u32 baud)
{
  float a = 0;
	int b = 0,c = 0;
//	u32 temp = 0;
	
	RCC->APB1ENR |= 1<<18;      //USART3ʱ��ʹ��													
	
	GPIOB->CRH &= 0xffff00ff;   //B10,B11����
	GPIOB->CRH |= 0x00004b00;   //B10�����������,B11��������
	RCC->APB1RSTR |= (1<<18);   //��λUSART3
	RCC->APB1RSTR &= ~(1<<18);  //USART��λ����
	USART3->CR1 &= ~(1<<12);    //һ����ʼλ��8������λ
	USART3->CR2 &= ~(3<<12);    //1��ֹͣλ
  //USART1->CR1|=(1<<10);     //��У�� żУ��
	USART1->CR1|=0X200C;  //1λֹͣ,��У��λ.
  a=36000000.0/(16*baud);     //���㲨����
	b=a;                        //ȡ������
	c=((a-b)*16);               //ȡ��С��
	c=(b<<4)+c;                 //�ϲ�
	USART3->BRR|=c;             //������ֵ��BRR;
	USART3->CR1|=(1<<3);        //����ʹ��
  USART3->CR1|=(1<<2);        //����ʹ��
	USART3->CR1|=(1<<13);       //USARTʹ��
	
	//ʹ�ܽ����ж� 
	USART3->CR1|=1<<5;    //���ջ������ǿ��ж�ʹ��	    	
	MY_NVIC_Init(0,0,USART3_IRQn,2);//��2
	
//	temp = USART3->SR;
}

//���ڷ���
void UsartSendData(u8 * tmp)
{
	while(*tmp)
	{
		USART3->DR = *tmp++;
		while(!(USART3->SR&(1<<6)));  //�ȴ��������
	}
}

//���ڽ��ܺ���
void (* g_usartRxFunc)(uint8_t data,USART_TypeDef* USARTx);

//���ڽ��պ�������
void USART_RxFuncConfig(void (* func)(uint8_t data,USART_TypeDef* USARTx))
{
		g_usartRxFunc = func;
}


//���ڽ����ж�
void USART3_IRQHandler(void)
{
	uint8_t bt;
	if(USART3->SR&(1<<5))
	{	 
		bt = (USART3->DR&(uint8_t)0x00FF);
		g_usartRxFunc(bt,USART3);
	}
}

