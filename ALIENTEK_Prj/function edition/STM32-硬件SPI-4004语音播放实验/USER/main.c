#include "delay.h"
#include "sys.h"
#include "usart.h"	 
#include "spi.h"
			 	
int main(void)
{
  u8 i=5;
	delay_init();	    	     //��ʱ������ʼ��	  
	NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(9600);	 	     //���ڳ�ʼ��Ϊ9600
  CS_Init();               //SPIƬѡ�źų�ʼ��
	SPI2_Init();  		       //SPI ��ʼ�� 


	while(1)
	{

     PLAY(0xc0);
	   while(i--)
	       {
			      delay_ms(1000);
         }    	            
     stop();
	}	
}

