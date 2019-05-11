#include "spi.h"
#include "delay.h"

       //ģ��SPI����
void SPI1_Init(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );//PORTBʱ��ʹ�� 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_13|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //������� 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB
}   


//���з����ӳ���8λ����
void spi_send(u8 isdx)
{
	u8 isx_counter;	
	SS=0;//ss=0��spiͨ�Ŷ�
	SCLK=0;
	for(isx_counter=0;isx_counter<8;isx_counter++)
	{
		if((isdx&0x01)==1)	   //�ȷ���λ�ٷ���λ���η���
		
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

//����stopָ��
void isd_stop(void)
{
	delay_us(10);
	spi_send(0x30);
	SS=1;    //����ָ��֮��Ϊ�ߵ�ƽ��������ָ��֮��Ҫ��SS����
	delay_ms(50);
}

//�����ϵ�ָ���ʱ50����,�ϵ��Ҫ��ʱһ��ʱ�䣨ԼΪ25MS������ܷ���ָ��
void isd_pu(void)
{
	delay_us(10);
	SS=0;
	spi_send(0x20);
	SS=1;
	delay_ms(50);   //�ϵ��Ҫ��ʱһ��ʱ�䣨ԼΪ25MS������ܷ���ָ��������50 MS

}

//���͵���ָ���ʱ50ms
void isd_pd(void)
{
	delay_us(10);
	spi_send(0x01);
	SS=1;
	delay_ms(50);

}

//����playָ��,����
void isd_play(void)
{
// 	spi_send(0xe0);
// 	delay_ms(1);
	spi_send(0xf0);
	SS=1;

}

//����setplayָ��
void isd_setplay(u8 adl,u8 adh)
{
	delay_ms(1);
	spi_send(adl);//���ͷ�����ʼ��ַ��λ
	delay_us(2);
	spi_send(adh);// ���ͷ�����ʼ��ַ��λ
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
			    
	        isd_setplay(0xa9,0xf0);//����setplayָ���0x0000��ַ��ʼ����
		      isd_play();            //���ͷ���ָ�� 
			    i=5;					 //iΪ����ʱ�䣬��λΪ�룬�ɸ���
	          while(i--)
	          {
			         delay_ms(500);
            }
			  
			  isd_stop();	
			  break;
	}
}





