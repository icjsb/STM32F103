#include "delay.h"
#include "sys.h"
#include "usart.h"	 
#include "spi.h"
			 	
int main(void)
{
  u8 i=5;
	delay_init();	    	     //延时函数初始化	  
	NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(9600);	 	     //串口初始化为9600
  CS_Init();               //SPI片选信号初始化
	SPI2_Init();  		       //SPI 初始化 


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

