#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "tpad.h"
#include "beep.h"
#include "myiic.h"
#include "timer.h"
#include "ISD4004.H"


/************************************************
 ALIENTEK 战舰STM32F103开发板实验0
 工程模板
 注意，这是手册中的新建工程章节使用的main文件 
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/
//void test(u32 x)
//{	
//	u32 y;
//	 GPIO_InitTypeDef  GPIO_InitStructure;
// 	
// RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE, ENABLE);	 //使能PB,PE端口时钟
//	
// GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;				 //LED0-->PB.5 端口配置
// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
// GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
// GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5

//	y=((u8)((x)>>16));
//	GPIOB->BRR=y;
//	
//}


 int main(void)
 {	
	//变量定义
	vu8 key=0;

	u8 times=2;
	 
	//函数初始化
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 //串口初始化为115200
 	LED_Init();			     //LED端口初始化
    TPAD_Init(72);			//初始化触摸按键
	BEEP_Init();	//蜂鸣器初始化
	TIM3_Int_Init(4999,7199);//10Khz的计数频率，计数到5000为500ms 
	KEY_Init();//按键初始化
	 
	ISD4004_Init();
	 

	//测试工程代码
	//测试LED0,LED1和蜂鸣器硬件状态
	TIM_Cmd(TIM3, DISABLE);  //失能TIMx
	LED0=1;//关
	LED1=1;
	BEEP=0;
	delay_ms(1000);
	LED0=0;//开
	LED1=0;
	BEEP=1;
	delay_ms(500);
	printf("\r\n战舰STM32开发板 串口实验\r\n");  //测试串口通信状态
	printf("正点原子@ALIENTEK\r\n\r\n");
	BEEP=0; //关
	LED0=1;
	LED1=1;
	
	//测试定时器的代码
	//TIM_Cmd(TIM3, ENABLE);  //使能TIMx
	
	
	while(1)
	{
//				//测试触摸按键硬件状态
//		if(TPAD_Scan(0))
//		{
//			LED0=0;
//			LED1=0;
//			delay_ms(100);
//			ISD4004_Rec(0); //开始录音
//			delay_ms(100);
//		}
		
		//测试三个按键硬件状态
 		key=KEY_Scan(1);	//得到键值
	   	if(key)
		{						   
			switch(key)
			{				 
				case WKUP_PRES:	//控制蜂鸣器
					//ISD4004_RecAddr(0x10);  					
					ISD4004_PlayAddr(0xC0);
				delay_ms(200);
				break;
				
				case KEY2_PRES:	//控制LED0翻转
					LED0=!LED0;
				
				//ISD4004_StopMode(); 
					ISD4004_StopPWRDN();
				delay_ms(200);
					break;
				
				case KEY1_PRES:	//控制LED1翻转	 
					LED1=!LED1;
				
					Play();//播放录音
					BEEP=0;
				delay_ms(200);
					break;
				
				case KEY0_PRES:	//同时控制LED0,LED1翻转 
					LED0=!LED0;
					LED1=!LED1;
					
					Rec();
					BEEP=0;
					delay_ms(200);
					break;
			}
		}else 
		{
			
		}
		while(key==WKUP_PRES)
		{
				
				BEEP=1;
				delay_ms(500);
				BEEP=0;
				delay_ms(200);
			times--;
			if(times==0)
			{
				times = 4;
				break;
			}
			
		}
		Play();
	}
 }
