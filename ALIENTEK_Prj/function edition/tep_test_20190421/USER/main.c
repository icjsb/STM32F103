#include "sys.h"
#include "delay.h"
#include "usart.h"
 
/************************************************
 实验0：ALIENTEK STM32F103开发板工程模板
 注意，这是手册中的调试章节使用的main文件
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
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
