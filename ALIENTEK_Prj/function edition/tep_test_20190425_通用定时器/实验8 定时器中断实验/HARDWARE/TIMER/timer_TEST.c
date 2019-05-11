#include "timer.h"
#include "led.h"

//ͨ�ö�ʱ��3�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!
void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
    
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_Period = arr;
    TIM_TimeBaseInitStruct.TIM_Prescaler = psc;
    //TIM_TimeBaseInitStruct.TIM_RepetitionCounter = ;//This parameter is valid only for TIM1 and TIM8
    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);
    
    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
    
    NVIC_InitStruct.NVIC_IRQChannel = TIM3_IRQn ;
    NVIC_InitStruct.NVIC_IRQChannelCmd =ENABLE ;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
    NVIC_Init(&NVIC_InitStruct);
    
    TIM_Cmd(TIM3,ENABLE);
    
}




void TIM3_IRQHandler(void)   
{
 	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
		{
			TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx�����жϱ�־  
			//LED0=!LED0;
			LED1=!LED1;
		}
}

//PWMʵ��
void TIM3_PWM_Init(u16 arr,u16 psc,u16 pulse)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOB ,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 //LED0-->PB.5 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5

	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);
	
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_Period = arr;
    TIM_TimeBaseInitStruct.TIM_Prescaler = psc;
    //TIM_TimeBaseInitStruct.TIM_RepetitionCounter = ;//This parameter is valid only for TIM1 and TIM8
    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);
	

	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM2 ;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_Pulse = pulse;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable ;
	TIM_OC2Init(TIM3, &TIM_OCInitStruct);
	
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	TIM_Cmd(TIM3, ENABLE);
	
	
}

