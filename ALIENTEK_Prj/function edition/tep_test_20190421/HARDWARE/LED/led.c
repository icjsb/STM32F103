#include "led.h"

void LED_Init(void)
{

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE,ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;     
    GPIO_DeInit(GPIOE);
    GPIO_DeInit(GPIOB);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    GPIO_SetBits(GPIOB,GPIO_Pin_5);	
    GPIO_SetBits(GPIOE,GPIO_Pin_5);	
    
    
}
