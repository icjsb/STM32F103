#ifndef __SPI_H
#define __SPI_H
#include "sys.h"

#define	SS          PCout(0)  //	Ƭѡ�ź�

void CS_Init(void);        //Ƭѡ�źų�ʼ������
void SPI2_Init(void);

void ISD4004(unsigned int ISD4004_order);       //ISD4004�����
void PLAY(unsigned int PLAY_address);          //��������  
void stop(void);                               //ֹͣ����
void REC(unsigned int REC_address);            //¼������

#endif

