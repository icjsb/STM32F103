#include "delay.h"
#include "sys.h"
#include "usart.h"	 
#include "spi.h"
#include "led.h"				 	


int main(void)
{
	u8 mun;
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(9600);	 	//���ڳ�ʼ��Ϊ9600
	SPI1_Init();  		//SPI ��ʼ�� 

	delay_ms(200);
	isd_pu();//AN������ISD�ϵ粢��ʱ50ms
	isd_pd();
	isd_pu();
	while(1)
	{
		mun=1;
    bofan(mun);

	}	
}

