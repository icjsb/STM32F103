#include "sys.h"
#include "delay.h"
#include "usart.h"
 
/************************************************
 ʵ��0��ALIENTEK STM32F103�����幤��ģ��
 ע�⣬�����ֲ��еĵ����½�ʹ�õ�main�ļ�
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

#include "led.h"



int main(void)
{	
    delay_init();
    LED_Init();
//    GPIO_ResetBits(GPIOE,GPIO_Pin_5);
//    GPIO_ResetBits(GPIOB,GPIO_Pin_5);
//    delay_ms(1000);
//    GPIO_SetBits(GPIOB,GPIO_Pin_5);
//    delay_ms(100);
    while(1)
    {
        PEout(5)=1;
        PBout(5)=1;
        delay_ms(500);
        PEout(5)=0;
        PBout(5)=0;
        delay_ms(500);
    }
	 
} 
