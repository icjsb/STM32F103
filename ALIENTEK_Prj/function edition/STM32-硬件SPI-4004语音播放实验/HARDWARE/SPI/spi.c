#include "spi.h"
#include "delay.h"

 
/***********************************
 ���ܣ�SPIƬѡ�źų�ʼ��
 ˵�������SPI_NSS����Ϊ�����ģʽ��SPI_NSS_Soft��
       ��SPI��Ƭѡ�źſ���Ϊͨ��I/O
       ���SPI_NSS����Ϊ��Ӳ��ģʽ��SPI_NSS_Hard��
       ��SPI��Ƭѡ�ź�ΪSPI_NSS		 
************************************/

void CS_Init(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOC, ENABLE );    //PORTBʱ��ʹ�� 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		       //������� 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);                     //��ʼ��GPIOB
}


/***********************************
 ���ܣ�SPI��ʼ��
 ˵����SPIƬѡ�źţ�PB12
       SPI_SCK:     PB13
       SPI_MOSI:    PB15
************************************/

void SPI2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;
	
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );//PORTBʱ��ʹ�� 
	RCC_APB1PeriphClockCmd(	RCC_APB1Periph_SPI2,  ENABLE );//SPI2ʱ��ʹ�� 	 
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 |GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //PB13/15����������� 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB


	SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;             //SPI����Ϊ����˫���� 
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		                      //����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;		                //����SPI�����ݴ�С:SPI���ͽ���16λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low ;		                        //����ͬ��ʱ�ӵĿ���״̬Ϊ�͵�ƽ  (���е͵�ƽ��˼����������Ч)
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge ;	                        //����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		                          //���ģʽ
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;	 //���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ4��9MHZ��
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_LSB;	                  //���ݴ���� LSB λ��ʼ

	SPI_Init(SPI2, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
  
	SPI_Cmd(SPI2, ENABLE); //ʹ��SPI����
		 
} 


/****************************************
		 �������ܣ� ISD4004�����
*****************************************/

void ISD4004(unsigned int ISD4004_order)
{
	SS=0;																			          //����Ƭѡ
	SPI_I2S_SendData(SPI2,ISD4004_order);								//��������
	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_BSY)!=RESET);	//�ȴ��������
	SS=1;																			          //�ͷ�����
}


/*******************************
�������ܣ���������   
˵����������ַ PLAY_address        
********************************/

void PLAY(unsigned int PLAY_address)
{
	ISD4004(0x2000);						   //ISD4004�ϵ�
	delay_ms(25);							     //��ʱ25ms
	ISD4004(0xF000|PLAY_address);	//ISD4004���÷�����ַ
  ISD4004(0xE000);						  //ISD4004��ָ����ַ���ô�����
	ISD4004(0xF000);						  //ISD4004����
}

/********************************
�������ܣ�¼������   
˵����¼����ַ  REC_address 
           
*********************************/

void REC(unsigned int REC_address)
{
	ISD4004(0x2000);					//ISD4004�ϵ�
	delay_ms(25);						  //��ʱ25ms
	ISD4004(0x2000);					//ISD4004�ϵ�
	delay_ms(50);						  //��ʱ50ms
  ISD4004(0xA000|REC_address);	//ISD4004¼����ַ
  ISD4004(0xB000);					//ISD4004�ӵ�ַ���ô�¼��
}

/***********************
�������ܣ�ֹͣ����  
*************************/

void stop(void)
{
	ISD4004(0x1000);		//ֹͣ�����纯��
	delay_ms(50);			//��ʱ50ms
}





