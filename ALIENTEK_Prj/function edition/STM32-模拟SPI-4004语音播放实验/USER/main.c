#include "delay.h"
#include "sys.h"
#include "usart.h"	 
#include "spi.h"
#include "led.h"				 	


int main(void)
{
	u8 mun;
	delay_init();	    	 //延时函数初始化	  
	NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(9600);	 	//串口初始化为9600
	SPI1_Init();  		//SPI 初始化 

	delay_ms(200);
	isd_pu();//AN键按下ISD上电并延时50ms
	isd_pd();
	isd_pu();
	while(1)
	{
		mun=1;
    bofan(mun);

	}	
}

