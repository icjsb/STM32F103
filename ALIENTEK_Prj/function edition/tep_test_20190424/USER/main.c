#include "stm32f10x.h"


/************************************************
 ALIENTEK ս��STM32F103������ʵ��0
 ����ģ��
 ע�⣬�����ֲ��е��½������½�ʹ�õ�main�ļ� 
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

void My_UART1_Init(void){
    
    GPIO_InitTypeDef GPIO_InitStruct;
    USART_InitTypeDef USART_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 , ENABLE);
    
    USART_DeInit(USART1);
    
    
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    USART_InitStruct.USART_BaudRate = 115200;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b ;
    USART_InitStruct.USART_StopBits = USART_StopBits_1 ;
    USART_InitStruct.USART_Parity = USART_Parity_No ;
    USART_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None ;
    
    USART_Init(USART1, &USART_InitStruct);
    
    USART_Cmd(USART1,ENABLE);//ʹ�ܴ���1
    
    USART_ITConfig(USART1, USART_IT_RXNE , ENABLE);//���������ж�
    
    NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn ;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
}

void USART1_IRQHandler(void){
    uint16_t data;
    if(USART_GetITStatus(USART1,USART_IT_RXNE))
    {
        data=USART_ReceiveData(USART1);
        USART_SendData(USART1,data);
    }

}
    
 int main(void)
 {	
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    My_UART1_Init();
    while(1);
 }
