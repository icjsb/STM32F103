//#include "stm32f10x.h"
//#include "delay.h"
//#include "sys.h"
#include  "stm32f10x_lib.h"		//包含所有的头文件
#include	"stm32f10x_map.h"
#include <stdio.h>

/*********************************************************************/
//第14讲 位绑定的封装
#define GPIOA_ODR_ADDR (GPIOA_BASE+0X0C)		//0x4001080C  
#define GPIOA_IDR_ADDR (GPIOA_BASE+0X08)		
#define GPIOB_ODR_ADDR (GPIOB_BASE+0X0C)		//0x40010C0C
#define GPIOB_IDR_ADDR (GPIOB_BASE+0X08)		
#define GPIOC_ODR_ADDR (GPIOC_BASE+0X0C)		//0x4001100C
#define GPIOC_IDR_ADDR (GPIOC_BASE+0X08)	
#define GPIOD_ODR_ADDR (GPIOD_BASE+0X0C)		//0x4001140C
#define GPIOD_IDR_ADDR (GPIOD_BASE+0X08)
#define GPIOE_ODR_ADDR (GPIOE_BASE+0X0C)		//0x4001180C
#define GPIOE_IDR_ADDR (GPIOE_BASE+0X08)

#define BitBand(Addr,BitNum)  ((Addr & 0xF0000000) + 0x2000000 + ((Addr & 0xfffff) << 5 )+ (BitNum << 2))		//Aliase位别名地址
#define MeM_Addr(addr)    *((volatile unsigned long *)(addr))		//添加volatile为安全起见
																																//volatile关键字 告诉编译器不要优化当前代码

#define PAout(n)	MeM_Addr(BitBand(GPIOA_ODR_ADDR,n))		//PA口的输出
#define PAin(n)		MeM_Addr(BitBand(GPIOA_IDR_ADDR,n))			//PA口的输入
#define PBout(n)	MeM_Addr(BitBand(GPIOB_ODR_ADDR,n))		//PB口的输出
#define PBin(n)		MeM_Addr(BitBand(GPIOB_IDR_ADDR,n))			//PB口的输入
#define PCout(n)	MeM_Addr(BitBand(GPIOC_ODR_ADDR,n))		//PC口的输出
#define PCin(n)		MeM_Addr(BitBand(GPIOC_IDR_ADDR,n))			//PC口的输入
#define PDout(n)	MeM_Addr(BitBand(GPIOD_ODR_ADDR,n))		//PD口的输出
#define PDin(n)		MeM_Addr(BitBand(GPIOD_IDR_ADDR,n))			//PD口的输入
#define PEout(n)	MeM_Addr(BitBand(GPIOE_ODR_ADDR,n))		//PE口的输出
#define PEin(n)		MeM_Addr(BitBand(GPIOE_IDR_ADDR,n))			//PE口的输入

/*************************函数声明*************************************/
void Delay_MS(u16 dly);
void LS74164_Send(u8 data);
void RCC_Configuration(void);
void GPIO_Configuration(void);
void USART_Configuration(u32 BaudRate);
int fputc(int ch,FILE* f);
/**********************************************************
** 函数名: void All_GPIO_Config
** 功能描述: 所有的GPIO口配置放在这里
** 输入参数: 无
** 输出参数: 无
***********************************************************/
//void All_GPIO_Config(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;  //定义GPIO结构体	
//	/*允许总线CLOCK,在使用GPIO之前必须允许相应端的时钟.
//	从STM32的设计角度上说,没被允许的端将不接入时钟,也就不会耗能,
//	这是STM32节能的一种技巧,*/
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能GPIOA口时钟
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//使能GPIOB口时钟

//	/* PA1,2输出*/
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2; //PA1,PA2配置
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//推挽输出
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50M时钟速度
//	GPIO_Init(GPIOA, &GPIO_InitStructure); //根据以上参数初始化结构体
//	
//	/*PB1,输出*/
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;//PB1
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//推挽输出
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50M时钟速度
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
//}

/**********************************************************
** 函数名: main
** 功能描述: 使用系统文件使PA1,PA2,PB1口引脚输出方波
** 输入参数: 无
** 输出参数: 无
** 说明：下面介绍三种实现方式
	1.使用STM32的位段功能
	2.直接操作寄存器
	3.使用库函数操作
	使用位段功能与直接操作寄存器速度差不多，使用库函数速度明显慢得多
	可以使用软件仿真调试分别查看三种方式的执行时间
***********************************************************/
int main(void){
	return(1);
}	
	

////int main(void)
////{
//    //SystemInit(); //系统时钟初始化，72MHz
//	Stm32_Clock_Init(12); //系统时钟设置为12倍频
//	delay_init(96);//系统SysTick初始化
//	All_GPIO_Config();//所有GPIO配置
//  	for(;;)//循环
//  	{
//      PAout(1)=1;//PA1=1
//	  PAout(2)=1;//PA2=1
//	  PBout(1)=1;//PB1=1
//	  delay_ms(1); //使用SysTick(系统滴嗒定时器)延时1ms
//	  PAout(1)=0;//PA1=0
//	  PAout(2)=0;//PA2=0
//	  PBout(1)=0;//PB1=0
//      delay_ms(1); //使用SysTick相对精确延时1ms	

//	  //或者直接使用寄存器操作
//	  /*GPIOA->BSRR=GPIO_Pin_1;//PA1=1
//	  GPIOA->BSRR=GPIO_Pin_2;//PA2=1
//	  GPIOB->BSRR=GPIO_Pin_1;//PB1=1
//	  delay_ms(1); //使用SysTick延时1ms
//	  GPIOA->BRR=GPIO_Pin_1;//PA1=0
//	  GPIOA->BRR=GPIO_Pin_2;//PA2=0
//	  GPIOB->BRR=GPIO_Pin_1;//PB1=0
//      delay_ms(1); //使用SysTick相对精确延时1ms	 */

//	  //或者使用库函数操作
//	  /*GPIO_SetBits(GPIOA, GPIO_Pin_1);//PA1=1
//	  GPIO_SetBits(GPIOA, GPIO_Pin_2);//PA2=1
//	  GPIO_SetBits(GPIOB, GPIO_Pin_1);//PB1=1
//	  delay_ms(1); //延时1ms
//	  GPIO_ResetBits(GPIOA, GPIO_Pin_1);//PA1=0
//	  GPIO_ResetBits(GPIOA, GPIO_Pin_2);//PA2=0
//	  GPIO_ResetBits(GPIOB, GPIO_Pin_1);//PB1=0
//      delay_ms(1); //延时1ms */

//  	}
//}

/*******************************************************************************
* Function Name  : printf-foutc
* Description    : 重新定向这个C库（studio）prontf函数 文件流->串口usart1.
* Input          : ch,*f
* Output         : None
* Return         : None
*******************************************************************************/
int fputc(int ch,FILE* f)
{
	//ch送给USART1
	USART_SendData(USART1,ch);
	//等待发送完毕
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) ;
	//返回ch
	return (ch);
}


/*******************************************************************************
* Function Name  : RCC_Configuration
* Description    : Configures the different system clocks.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RCC_Configuration(void)
{
	//----------使用外部RC晶振-----------
	RCC_DeInit();			//初始化为缺省值
	RCC_HSEConfig(RCC_HSE_ON);	//使能外部的高速时钟 
	while(RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET);	//等待外部高速时钟使能就绪
	
	//FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);	//Enable Prefetch Buffer
	//FLASH_SetLatency(FLASH_Latency_2);		//Flash 2 wait state
	
	RCC_HCLKConfig(RCC_SYSCLK_Div1);		//HCLK = SYSCLK
	RCC_PCLK2Config(RCC_HCLK_Div1);			//PCLK2 =  HCLK
	RCC_PCLK1Config(RCC_HCLK_Div2);			//PCLK1 = HCLK/2
	RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_9);	//PLLCLK = 8MHZ * 9 =72MHZ
	RCC_PLLCmd(ENABLE);			//Enable PLLCLK

	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);	//Wait till PLLCLK is ready
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);	//Select PLL as system clock
	while(RCC_GetSYSCLKSource()!=0x08);		//Wait till PLL is used as system clock source
	
	//---------打开相应外设时钟--------------------
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	//使能APB2外设的GPIOA的时钟	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);	//使能APB2外设的GPIOC的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	//GPIO_PinRemapConfig(GPIO_Remap_USART1,ENABLE);	 	 
}

/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : 初始化GPIO外设
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/ 
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Configure USARTx_Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Configure USARTx_Rx as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

}

void USART_Configuration(u32 BaudRate)
{
	USART_InitTypeDef USART_InitStructure; 
	USART_InitStructure.USART_BaudRate = BaudRate; 
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; 
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 
	USART_InitStructure.USART_Parity = USART_Parity_No; 
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;  
	USART_Init(USART1, &USART_InitStructure);
	USART_Cmd(USART1, ENABLE);
}


void LS74164_Send(u8 data)
{
		u8 i;
	data = 0xf2;
	PBout(0)=1;
	for(i=0;i<8;i++)
	{
		 
		Delay_MS(1);
		if((data&0x01)==0x01)
		{
			PAout(0)=1;
		}
		else
		{
			PAout(0)=0;
		}
		PAout(1)=0;
		Delay_MS(1);
		PAout(1)=1;
		data>>=1;
	}	
}

/*******************************************************************************
* Function Name  : Delay_Ms
* Description    : delay 1 ms.
* Input          : dly (ms)
* Output         : None
* Return         : None
*******************************************************************************/
void Delay_MS(u16 dly)
{
	u16 i,j;
	for(i=0;i<dly;i++)
		for(j=1000;j>0;j--);
}

