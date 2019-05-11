#include "ISD4004.h"


//////////////////////////////////////////////////////////////////////////////////	 
//ISD4004��������	   
//�޸�����:2019/5/2
//�汾��V1.0								  
////////////////////////////////////////////////////////////////////////////////// 

/*****************************************************************
�������ƣ�ISD4004_Init(void)
��ڲ�������
���ڲ�������
���ܣ���ʼ��ʱ���Լ���ISD4004�����ӵ�IO��
******************************************************************/
void ISD4004_Init(void)
{
	
	//��ʼ��PC�ڣ�����CS��INT��RAC
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB, ENABLE);	 //ʹ��PB,PE�˿�ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_4|GPIO_Pin_5;				 //
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
//	
//	GPIO_SetBits(GPIOB,GPIO_Pin_12); 
//	GPIO_SetBits(GPIOC,GPIO_Pin_0); 
	
	W25QXX_CS=1;				//SPI FLASH��ѡ��
	ISD4004_CS = 1;
	
	//SPI2_SetSpeed(SPI_BaudRatePrescaler_256);//����Ϊ18Mʱ��,����ģʽ;APB2ʱ���ٶ�Ϊ36M
	SPI2_Init();		   	//��ʼ��SPI
}

/********************************************************************
�������ƣ�ISD4004_PwrUp()
��ڲ�������
���ڲ�������
���ܣ�ISD4004���ϵ�ָ��
********************************************************************/
void ISD4004_PwrUp(void)
{
	ISD4004_CS = 1;
	ISD4004_CS = 0;	
	//ISD4004(ISD4004_PwrUpEnable);
	ISD4004_noCS(ISD4004_PwrUpEnable);
	ISD4004_CS = 1;

}

/*****************************************************************
�������ƣ�ISD4004_StopMode(void)
��ڲ�������
���ڲ�������
���ܣ�ֹͣ��ǰ����
********************************************************************/
void ISD4004_StopMode(void)
{	

	ISD4004(ISD4004_StopCrtWr);
	delay_ms(50);
}
void ISD4004_StopPWRDN(void)
{
	ISD4004(ISD4004_StopWrPwrDn);
	delay_ms(50);
}
/*****************************************************************
�������ƣ�ISD4004_Rec(u16 addr)
��ڲ�����u16 addr
���ڲ�������
���ܣ���address����ʼ¼��
********************************************************************/
void ISD4004_RecAddr(u16 addr)
{

	
	ISD4004_PwrUp();
	delay_ms(25);
	ISD4004_PwrUp();
	delay_ms(50);

	ISD4004_CS = 1;
	ISD4004_CS = 0;	
	delay_ms(5);
	ISD4004_noCS(addr);
	ISD4004_noCS(ISD4004_SetRecAddr);
	//ISD4004(ISD4004_RecAtCrtAddr);
	ISD4004_CS = 1;	
	delay_ms(5);
}	

void Rec(void) //��ʼ¼��
{
	ISD4004_PwrUp();
	delay_ms(25);
	ISD4004_PwrUp();
	delay_ms(50);
	

	ISD4004(ISD4004_RecAtCrtAddr);
	
}

/*****************************************************************
�������ƣ�ISD4004_PlayRec(u16 addr)
��ڲ�����u16 addr
���ڲ�������
���ܣ���address����ʼ����
********************************************************************/

void ISD4004_PlayAddr(u16 addr)
{
	ISD4004_PwrUp();
	delay_ms(25);

	ISD4004_CS = 1;
	ISD4004_CS = 0;	
	delay_ms(5);
	ISD4004_noCS(addr);
	ISD4004_noCS(ISD4004_SetPlayAddr);
	ISD4004_noCS(ISD4004_PlayAtCrtAddr);	
	ISD4004_CS = 1;
	delay_ms(5);
		
}

void Play(void)
{
	ISD4004_PwrUp();
	delay_ms(25);


	ISD4004(ISD4004_PlayAtCrtAddr);	
}
/*****************************************************************
�������ƣ�ISD4004
��ڲ�����u16 
���ڲ�������
���ܣ���������
********************************************************************/

void ISD4004(u16 ISD4004_order)
{	
	ISD4004_CS = 0;	
	delay_ms(5);
	SPI_I2S_SendData(SPI2,ISD4004_order);								//��������
	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_BSY)!=RESET);	//�ȴ��������
	ISD4004_CS = 1;	
}


void ISD4004_noCS(u16 ISD4004_order)
{
		SPI_I2S_SendData(SPI2,ISD4004_order);								//��������
	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_BSY)!=RESET);	//�ȴ��������
}


