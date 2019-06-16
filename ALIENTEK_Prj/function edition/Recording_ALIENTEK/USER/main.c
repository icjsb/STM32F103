#include "sys.h"
#include "delay.h"
#include "usart.h" 
#include "led.h" 		 	 
#include "lcd.h"  
#include "key.h"     
#include "usmart.h" 
#include "malloc.h"
#include "sdio_sdcard.h"  
#include "w25qxx.h"    
#include "ff.h"  
#include "exfuns.h"   
#include "text.h" 
#include "vs10xx.h"
#include "tpad.h"	   
#include "recorder.h"	 
#include "24cxx.h"
#include "touch.h"

#include "mrrlcd.h"
#include "vs10xx.h"
#include "mp3player.h"	
#include "SendData.h"
#include "sram.h"	 

 
/************************************************
 ALIENTEKս��STM32������ʵ��44
 ¼���� ʵ��
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/


 int main(void)
 {	 
 
	delay_init();	    	 //��ʱ������ʼ��	  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
	usmart_dev.init(72);		//��ʼ��USMART
	 LED_Init();		  			//��ʼ����LED���ӵ�Ӳ���ӿ�
	KEY_Init();					//��ʼ������
	TPAD_Init(6);				//��ʼ����������	  
	LCD_Init();			   		//��ʼ��LCD     
	W25QXX_Init();				//��ʼ��W25Q128
 	VS_Init();	  				//��ʼ��VS1053 
 	my_mem_init(SRAMIN);		//��ʼ���ڲ��ڴ��
	exfuns_init();				//Ϊfatfs��ر��������ڴ�  
 	f_mount(fs[0],"0:",1); 		//����SD�� 
 	f_mount(fs[1],"1:",1); 		//����FLASH. 
	 
	 tp_dev.init(); //��������ʼ��
	 
 	while(font_init()) 				//����ֿ�
	{	    
		LCD_ShowString(30,50,200,16,16,"Font Error!");
		delay_ms(200);				  
		LCD_Fill(30,50,800,480,WHITE);//�����ʾ	   
		
	}
	mrrlcd_msg_show();
	
 	
		POINT_COLOR=RED;//��������Ϊ��ɫ
   		
		LED0=0;
		LED1=0;
		Recording_Dialog();
		while(1)
		{
			//tp_dev.scan(0);

			touch_swtich();
		}



}




















