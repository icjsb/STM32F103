#include "spi.h"
#include "delay.h"

 
/***********************************
 功能：SPI片选信号初始化
 说明：如果SPI_NSS配置为：软件模式（SPI_NSS_Soft）
       则SPI的片选信号可作为通用I/O
       如果SPI_NSS配置为：硬件模式（SPI_NSS_Hard）
       则SPI的片选信号为SPI_NSS		 
************************************/

void CS_Init(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOC, ENABLE );    //PORTB时钟使能 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		       //推挽输出 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);                     //初始化GPIOB
}


/***********************************
 功能：SPI初始化
 说明：SPI片选信号：PB12
       SPI_SCK:     PB13
       SPI_MOSI:    PB15
************************************/

void SPI2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;
	
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );//PORTB时钟使能 
	RCC_APB1PeriphClockCmd(	RCC_APB1Periph_SPI2,  ENABLE );//SPI2时钟使能 	 
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 |GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //PB13/15复用推挽输出 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB


	SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;             //SPI设置为单线双向发送 
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		                      //设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;		                //设置SPI的数据大小:SPI发送接收16位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low ;		                        //串行同步时钟的空闲状态为低电平  (空闲低电平意思是正脉冲有效)
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge ;	                        //串行同步时钟的第二个跳变沿（上升或下降）数据被采样
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		                          //软件模式
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;	 //定义波特率预分频的值:波特率预分频值为4（9MHZ）
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_LSB;	                  //数据传输从 LSB 位开始

	SPI_Init(SPI2, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
  
	SPI_Cmd(SPI2, ENABLE); //使能SPI外设
		 
} 


/****************************************
		 函数功能： ISD4004命令函数
*****************************************/

void ISD4004(unsigned int ISD4004_order)
{
	SS=0;																			          //拉低片选
	SPI_I2S_SendData(SPI2,ISD4004_order);								//发送命令
	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_BSY)!=RESET);	//等待发送完成
	SS=1;																			          //释放器件
}


/*******************************
函数功能：放音函数   
说明：放音地址 PLAY_address        
********************************/

void PLAY(unsigned int PLAY_address)
{
	ISD4004(0x2000);						   //ISD4004上电
	delay_ms(25);							     //延时25ms
	ISD4004(0xF000|PLAY_address);	//ISD4004设置放音地址
  ISD4004(0xE000);						  //ISD4004从指定地址设置处放音
	ISD4004(0xF000);						  //ISD4004放音
}

/********************************
函数功能：录音函数   
说明：录音地址  REC_address 
           
*********************************/

void REC(unsigned int REC_address)
{
	ISD4004(0x2000);					//ISD4004上电
	delay_ms(25);						  //延时25ms
	ISD4004(0x2000);					//ISD4004上电
	delay_ms(50);						  //延时50ms
  ISD4004(0xA000|REC_address);	//ISD4004录音地址
  ISD4004(0xB000);					//ISD4004从地址设置处录音
}

/***********************
函数功能：停止函数  
*************************/

void stop(void)
{
	ISD4004(0x1000);		//停止并掉电函数
	delay_ms(50);			//延时50ms
}





