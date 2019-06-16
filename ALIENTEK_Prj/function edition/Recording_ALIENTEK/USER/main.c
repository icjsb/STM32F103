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
 ALIENTEK战舰STM32开发板实验44
 录音机 实验
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/


 int main(void)
 {	 
 
	delay_init();	    	 //延时函数初始化	  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
	usmart_dev.init(72);		//初始化USMART
	 LED_Init();		  			//初始化与LED连接的硬件接口
	KEY_Init();					//初始化按键
	TPAD_Init(6);				//初始化触摸按键	  
	LCD_Init();			   		//初始化LCD     
	W25QXX_Init();				//初始化W25Q128
 	VS_Init();	  				//初始化VS1053 
 	my_mem_init(SRAMIN);		//初始化内部内存池
	exfuns_init();				//为fatfs相关变量申请内存  
 	f_mount(fs[0],"0:",1); 		//挂载SD卡 
 	f_mount(fs[1],"1:",1); 		//挂载FLASH. 
	 
	 tp_dev.init(); //触摸屏初始化
	 
 	while(font_init()) 				//检查字库
	{	    
		LCD_ShowString(30,50,200,16,16,"Font Error!");
		delay_ms(200);				  
		LCD_Fill(30,50,800,480,WHITE);//清除显示	   
		
	}
	mrrlcd_msg_show();
	
 	
		POINT_COLOR=RED;//设置字体为红色
   		
		LED0=0;
		LED1=0;
		Recording_Dialog();
		while(1)
		{
			//tp_dev.scan(0);

			touch_swtich();
		}



}




















