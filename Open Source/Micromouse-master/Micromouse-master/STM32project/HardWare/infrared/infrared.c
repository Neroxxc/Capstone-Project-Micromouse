#include "infrared.h"
#include "delay.h"

u16 ADC_MeasureBuf[6] = {0,0,0,0,0,0};

void Dma_Init(void)
{	
	
	RCC->AHBENR |= 1;                         //open adc1 clock
	DMA1_Channel1->CPAR =(u32)&ADC1->DR;      //Peripheral address is dr of adc1 
	DMA1_Channel1->CMAR =(u32)&ADC_MeasureBuf; //Memory address is buf
	
	DMA1_Channel1->CCR |= (1<<7);             //Memory address is increment
	DMA1_Channel1->CCR |= (1<<12);            //priority is centre
	DMA1_Channel1->CCR |= (1<<8);             //Peripheral address is 16-bit
	DMA1_Channel1->CCR |= (1<<10);            //Memory 16-bit
  DMA1_Channel1->CCR &=~(1<<4);             //DAta transfer deriction is read from the Peripheral
} 

void Dma_Start(void)
{
	DMA1_Channel1->CCR &= ~0x01;              //disnable DMA1
	DMA1_Channel1->CNDTR |= 6;                //CNDTR = 6;
	DMA1_Channel1->CCR |= 1;   	              //enable DMA1
}


void Adc_Init(void)
{
	RCC->APB2ENR |= (1<<2);                   //open GPIOA clock
	RCC->APB2ENR |= (1<<4);                   //open GPIOC clock
	
	GPIOA->CRL &= 0xffffff0ff;
	GPIOA->CRL |= 0x000000000;                //analog input 
	
	GPIOC->CRL &= 0xff0fff00;
	GPIOC->CRL |= 0x00000000;
	
	
	RCC->APB2ENR |= (1<<9);                   //open adc1 clock
	RCC->CFGR |= (0x3<<14);                   //ADC clock = PCLK2 / 8;
	RCC->APB2RSTR |= (1<<9);                  //ADC1 RSTR
	RCC->APB2RSTR &=~(1<<9);                  //ADC1 end of reset
	
	ADC1->CR1 &= ~(0xf<<16);                  //ADC is independent mode
	ADC1->CR1 |= (0x1<<8);                    //open scan mode
	ADC1->CR2 |= (0x7<<17);                   //SWSTART
	ADC1->CR2 &=~(1<<11);                     //flush right
	ADC1->CR2 |= (1<<8);                      //enable dma
	
	ADC1->SMPR2 |= (0x7<<6);                  //ͨ��2����239.5������
	ADC1->SMPR1 |= 0x7;                       //ͨ��1����239.5������
	ADC1->SMPR1 |= (0x7<<3);                  //ͨ��11����239.5������
	ADC1->SMPR1 |= (0x7<<15);                 //ͨ��15����239.5������
	ADC1->SMPR1 |= (0x7<<9);                  //ͨ��13����239.5������
	ADC1->SMPR2 |= (0x7<<12);                 //ͨ��4����239.5������
	
  ADC1->SQR3 |=  0x02;                      //ת��1ͨ��һΪ IN2
	ADC1->SQR3 |= (0x0a<<5);                  //ת��2ͨ��һΪ IN10
	ADC1->SQR3 |= (0x0f<<10);                 //ת��3ͨ��һΪ IN15
	ADC1->SQR3 |= (0x0b<<15);                 //ת��4ͨ��һΪ IN11
	ADC1->SQR3 |= (0x0d<<20);                 //ת��5ͨ��һΪ IN13
	ADC1->SQR3 |= (0x04<<25);                 //ת��6ͨ��һΪ IN4
	
	ADC1->SQR1 |= (0x6<<20);                  //6��ת��
	ADC1->CR2 |= 1;                           //enable ADC1
	ADC1->CR2 |= (1<<3);                      //��λУ׼
	while( (ADC1->CR2 & (1<<3)) );            //�ȴ�У׼����
	ADC1->CR2 |= (1<<2);                      //ʹ��adУ׼
	while( (ADC1->CR2 & (1<<2)) );            //�ȴ�adУ׼����
	ADC1->CR2 &= ~1;                          //ADC enable
	
	
}

void Infrared_Send_Init(void)
{
	RCC->APB2ENR |= (1<<2);
	RCC->APB2ENR |= (1<<4);                  //open clock
	
	
	GPIOA->CRL &= ~(0xf<<12);
	GPIOA->CRL &= ~(0xf<<20);
	GPIOA->CRL |= (0x3<<12);
	GPIOA->CRL |= (0x3<<20);                 //PA3 5 push_pull output
	
	GPIOC->CRL &= ~(0xf<<8);
	GPIOC->CRH &= ~(0xf<<20); 
	GPIOC->CRH |= (0x3<<20);
	GPIOC->CRL |= (0x3<<8);                  //PC2 13 push_pull output
	
	GPIOA->ODR &= ~(1<<3);
	GPIOA->ODR &= ~(1<<5);
	
	GPIOC->ODR &= ~(1<<2);
	GPIOC->ODR &= ~(1<<13);                  // low
}


void Infrared_Send_Control(u8 led)
{	
  Out_A(5) = (led & 0x01);
	Out_C(13) = ((led>>1) & 0x01);
	Out_A(3) = ((led>>2) & 0x01);
	Out_C(2) = ((led>>3) & 0x01);
}	

void Adc_Transition()                     
{
	ADC1->CR2 |= 1;                         //ADC�ϵ�
	delay_us(50);           
	ADC1->CR2 |= 1;                         //ADC����
	Dma_Start(); 
	ADC1->CR2 |= (1<<22);                   //��ʼ����ͨ��ת��
	while(!(DMA1->ISR&0x2));               //�ȴ�DMAת������
	DMA1->IFCR |= 0x7;
	ADC1->SR &= ~0x12;         
	ADC1->CR2 &= ~1; 
}
