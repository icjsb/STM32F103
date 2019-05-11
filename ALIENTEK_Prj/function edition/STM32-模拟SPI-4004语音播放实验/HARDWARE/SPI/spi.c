#include "spi.h"
#include "delay.h"

       //模拟SPI总线
void SPI1_Init(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );//PORTB时钟使能 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_13|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB
}   


//串行发送子程序，8位数据
void spi_send(u8 isdx)
{
	u8 isx_counter;	
	SS=0;//ss=0打开spi通信端
	SCLK=0;
	for(isx_counter=0;isx_counter<8;isx_counter++)
	{
		if((isdx&0x01)==1)	   //先发地位再发高位依次发送
		
			MOSI=1;
		else
			MOSI=0;
			isdx=isdx>>1;
			SCLK=1;
			delay_us(2);
			SCLK=0;
			delay_us(2);		
	}
}

//发送stop指令
void isd_stop(void)
{
	delay_us(10);
	spi_send(0x30);
	SS=1;    //两条指令之间为高电平，故所有指令之后都要将SS拉高
	delay_ms(50);
}

//发送上电指令并延时50毫秒,上电后要延时一段时间（约为25MS）后才能发送指令
void isd_pu(void)
{
	delay_us(10);
	SS=0;
	spi_send(0x20);
	SS=1;
	delay_ms(50);   //上电后要延时一段时间（约为25MS）后才能发送指令，这里给它50 MS

}

//发送掉电指令并延时50ms
void isd_pd(void)
{
	delay_us(10);
	spi_send(0x01);
	SS=1;
	delay_ms(50);

}

//发送play指令,播放
void isd_play(void)
{
// 	spi_send(0xe0);
// 	delay_ms(1);
	spi_send(0xf0);
	SS=1;

}

//发送setplay指令
void isd_setplay(u8 adl,u8 adh)
{
	delay_ms(1);
	spi_send(adl);//发送放音起始地址地位
	delay_us(2);
	spi_send(adh);// 发送放音起始地址高位
	delay_us(2);
	spi_send(0xe0);
	SS=1;

}

void bofan(u8 mun)
{
    u8 i;
    switch(mun)
	{
	   case 1:
			    
	        isd_setplay(0xa9,0xf0);//发送setplay指令，从0x0000地址开始放音
		      isd_play();            //发送放音指令 
			    i=5;					 //i为放音时间，单位为秒，可更改
	          while(i--)
	          {
			         delay_ms(500);
            }
			  
			  isd_stop();	
			  break;
	}
}





