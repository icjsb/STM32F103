#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "tpad.h"
#include "beep.h"
#include "myiic.h"
#include "timer.h"


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


 int main(void)
 {	
	//��������
	vu8 key=0;
	//u8 timecount=10;


	//������ʼ��
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
 	LED_Init();			     //LED�˿ڳ�ʼ��
    TPAD_Init(72);			//��ʼ����������
	BEEP_Init();	//��������ʼ��
	TIM3_Int_Init(4999,7199);//10Khz�ļ���Ƶ�ʣ�������5000Ϊ500ms 
	KEY_Init();//������ʼ��
	 

	//���Թ��̴���
	//����LED0,LED1�ͷ�����Ӳ��״̬
	TIM_Cmd(TIM3, DISABLE);  //ʧ��TIMx
	LED0=1;//��
	LED1=1;
	BEEP=0;
	delay_ms(1000);
	LED0=0;//��
	LED1=0;
	BEEP=1;
	delay_ms(1000);
	printf("\r\nս��STM32������ ����ʵ��\r\n");  //���Դ���ͨ��״̬
	printf("����ԭ��@ALIENTEK\r\n\r\n");
	BEEP=0; //��
	LED0=1;
	LED1=1;
	
	//���Զ�ʱ���Ĵ���
	//TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx
	
	
	while(1)
	{
		//���Դ�������Ӳ��״̬
		if(TPAD_Scan(0))
		{
			//LED0=0;
			LED1=0;
			BEEP=0;
			delay_ms(250);
			//LED0=1;
			LED1=1;
			BEEP=1;
			delay_ms(250);
			
		}
		
		//������������Ӳ��״̬
 		key=KEY_Scan(1);	//�õ���ֵ
	   	if(key)
		{						   
			switch(key)
			{				 
				case WKUP_PRES:	//���Ʒ�����
					BEEP=1;
                delay_ms(200); 
					break;
				case KEY2_PRES:	//����LED0��ת
					LED0=!LED0;
                delay_ms(200); 
					break;
				case KEY1_PRES:	//����LED1��ת	 
					LED1=!LED1;
                delay_ms(200); 
					break;
				case KEY0_PRES:	//ͬʱ����LED0,LED1��ת 
					LED0=!LED0;
					LED1=!LED1;
                delay_ms(200); 
					break;
			}
		}else 
		{
		}
		BEEP=0;
	}
 }
