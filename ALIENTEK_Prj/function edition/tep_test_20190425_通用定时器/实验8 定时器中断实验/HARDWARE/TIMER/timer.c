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
//��ʱ��3�жϷ������
void TIM3_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM3,TIM_IT_Update)!=RESET)
    {
        
        TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
        LED1!=LED1;
    }
}





