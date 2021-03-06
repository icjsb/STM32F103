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
 
/************************************************
 ALIENTEK战舰STM32开发板实验39
 FATFS文件系统实验 
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/



 int main(void)
 {	 
 	u32 total,free;
	u8 t=0;	
	u8 res=0;	    	

	FIL fp1,fp2; FRESULT state01; UINT btw1,btw2;u8 buff1[50],buff2[50];	
		u8  *pp,tmp,i,L;


	delay_init();	    	 //延时函数初始化	  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
	usmart_dev.init(72);		//初始化USMART		
 	LED_Init();		  			//初始化与LED连接的硬件接口
	KEY_Init();					//初始化按键
	LCD_Init();			   		//初始化LCD   
	W25QXX_Init();				//初始化W25Q128
 	my_mem_init(SRAMIN);		//初始化内部内存池
 	POINT_COLOR=RED;			//设置字体为红色 
	LCD_ShowString(30,50,200,16,16,"WarShip STM32");	
	LCD_ShowString(30,70,200,16,16,"FATFS TEST");	
	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110,200,16,16,"2015/1/20"); 
	LCD_ShowString(30,130,200,16,16,"Use USMART for test");   
	while(SD_Init())//检测不到SD卡
	{
		LCD_ShowString(30,150,200,16,16,"SD Card Error!");
		delay_ms(500);					
		LCD_ShowString(30,150,200,16,16,"Please Check! ");
		delay_ms(500);
		LED0=!LED0;//DS0闪烁
	}
	
	
 	exfuns_init();							//为fatfs相关变量申请内存				 
  	f_mount(fs[0],"0:",1); 					//挂载SD卡 
 	res=f_mount(fs[1],"1:",1); 				//挂载FLASH.	
	if(res==0X0D)//FLASH磁盘,FAT文件系统错误,重新格式化FLASH
	{
		LCD_ShowString(30,150,200,16,16,"Flash Disk Formatting...");	//格式化FLASH
		res=f_mkfs("1:",1,4096);//格式化FLASH,1,盘符;1,不需要引导区,8个扇区为1个簇
		if(res==0)
		{
			f_setlabel((const TCHAR *)"1:ALIENTEK");	//设置Flash磁盘的名字为：ALIENTEK
			LCD_ShowString(30,150,200,16,16,"Flash Disk Format Finish");	//格式化完成
		}else LCD_ShowString(30,150,200,16,16,"Flash Disk Format Error ");	//格式化失败
		delay_ms(1000);
	}													    
	LCD_Fill(30,150,240,150+16,WHITE);		//清除显示			  
	while(exf_getfree("0",&total,&free))	//得到SD卡的总容量和剩余容量
	{
		LCD_ShowString(30,150,200,16,16,"SD Card Fatfs Error!");
		delay_ms(200);
		LCD_Fill(30,150,240,150+16,WHITE);	//清除显示			  
		delay_ms(200);
		LED0=!LED0;//DS0闪烁
	}													  			    
 	POINT_COLOR=BLUE;//设置字体为蓝色	   
	LCD_ShowString(30,150,200,16,16,"FATFS OK!");	 
	LCD_ShowString(30,170,200,16,16,"SD Total Size:     MB");	 
	LCD_ShowString(30,190,200,16,16,"SD  Free Size:     MB"); 	    
 	LCD_ShowNum(30+8*14,170,total>>10,5,16);				//显示SD卡总容量 MB
 	LCD_ShowNum(30+8*14,190,free>>10,5,16);		

	
	for(i=0;i<=50;i++)
	{
			 buff1[i]=0;
		 buff2[i]=0;
	}

		L = sizeof("123486 hello world!!");
	state01= f_open(&fp1,"0:/RECORDER/testt.txt",FA_OPEN_ALWAYS|FA_WRITE);
	if(state01)
{LCD_ShowString(30,280,300,16,24,"TEXT CREAT fail!");}
	else
	{
		f_write(&fp1,(const void* )"123486 hello world!!",L,&btw1);
		//f_write(&fp1,(const void* )"goodbye!!",10,&btw1);
	}
		f_close(&fp1);
//	f_open(&fp1,"0:/RECORDER/testt.txt",FA_WRITE);
//	f_write(&fp1,(const void* )"goodbye!!",10,&btw1);
//	f_close(&fp1);
	
	
//	f_open(&fp1,"0:/RECORDER/testt1.txt",FA_CREATE_ALWAYS|FA_WRITE);
//	f_write(&fp1,(const void* )"file1!!",10,&btw1);
//	f_close(&fp1);
//	f_open(&fp2,"0:/RECORDER/testt2.txt",FA_CREATE_ALWAYS|FA_WRITE);
//	f_write(&fp2,(const void* )"file2!!",10,&btw2);
	
	
	f_close(&fp2);
	
	
	
	
	
	state01= f_open(&fp1,"0:/RECORDER/testt.txt",FA_READ);
	
	if(state01)
{LCD_ShowString(30,300,300,16,24,"read fail!");}
	else
	{
		f_read(&fp1,buff1,5,&btw1);
		LCD_ShowString(30,480,310,16,24,buff1);
		f_read(&fp1,buff1,5,&btw1);
		LCD_ShowString(30,500,310,16,24,buff1);
		
		f_close(&fp1);
		LCD_ShowString(30,280,310,16,24,"Write success!");
		f_open(&fp2,"0:/RECORDER/revv.txt", FA_OPEN_ALWAYS|FA_WRITE);
		f_write(&fp2,(const void* )buff1,L,&btw2);
		f_close(&fp2);
		
	}	
		LCD_ShowString(30,300,310,16,24,buff1);
	state01= f_open(&fp2,"0:/RECORDER/revv.txt", FA_READ);
	
		if(state01)LCD_ShowString(30,330,300,16,24,"revv read fail!");
	else
	{

		f_read(&fp2,buff2,L,&btw2);
		f_close(&fp2);
		pp = buff2;
		while(*pp)
		{
			tmp=*pp;
			for(i=0;i<8;i++)
			{
				if(tmp&0x80)
				{
					LED1=1;
					LED0=1;
				}
				else
				{
					LED1=0;
					LED0=0;
				}
				tmp<<=1;
				delay_ms(100);
			}

			pp++;
		}
		LCD_ShowString(30,330,310,16,24,"LED send over!");
	}
	
	
	

	//显示SD卡剩余容量 MB			    
	while(1)
	{
		t++; 
		delay_ms(200);		 			   
		LED0=0;
	} 
}
















