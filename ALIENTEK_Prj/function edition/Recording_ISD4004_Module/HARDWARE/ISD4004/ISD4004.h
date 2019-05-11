#ifndef __ISD4004_H
#define __ISD4004_H	 
#include "sys.h"
#include "spi.h"
#include "delay.h"

//////////////////////////////////////////////////////////////////////////////////	 
//ISD4004驱动代码	   
//修改日期:2019/5/2
//版本：V1.0								  
////////////////////////////////////////////////////////////////////////////////// 

/********************************************************************************/
//指令表
//快进只能在放音操作开始时选择。
#define ISD4004_PwrUpEnable 0x20  //上电:等待TPUD 后器件可以工作
#define ISD4004_SetPlayAddr 0xE0 //从指定地址开始放音。后跟PLAY 指令可使放音继续进行下去
#define ISD4004_PlayAtCrtAddr 0xF0 //从当前地址开始放音(直至EOM 或OVF)
#define ISD4004_SetRecAddr 0xA0 //从指定地址开始录音。后跟REC 指令可使录音继续进行下去
#define ISD4004_RecAtCrtAddr 0xB0  //从当前地址开始录音(直至OVF 或停止)
#define ISD4004_SetMc 0xE8 //从指定地址开始快进。后跟MC 指令可使快进继续进行下去
#define ISD4004_Mc 0xF8 //执行快进,直到EOM.若再无信息,则进入OVF 状态
#define ISD4004_StopCrtWr 0x30 //停止当前操作
#define ISD4004_StopWrPwrDn 0x10 //停止当前操作并掉电
//#define 4004_ReadStaus 0x30 //读状态:OVF 和EOM



/********************************************************************************/

// PC0 CS_4004
// PC1 SCLK_4004
// PC2 MOSI_4004
// PC3 MISO_4004
// PC4 INT_4004
// PC5 RAC_4004

//#define CS_4004_H PCout(0)=1 // PC0=1
//#define CS_4004_L PCout(0)=0 // PC0=0

//#define SCLK_4004_H PCout(1)=1 // PC1=1
//#define SCLK_4004_L PCout(1)=0 // PC1=0

//#define MOSI_4004_H PCout(2)=1 // PC2=1
//#define MOSI_4004_L PCout(2)=0 // PC2=0

//#define MISO_4004_H PCout(3)=1 // PC3=1
//#define MISO_4004_L PCout(3)=0 // PC3=0

#define ISD4004_CS PCout(0) // PC0 SS输出口
#define ISD4004_INT PCout(4) // PC4 INT 输出口
#define ISD4004_RAC_ PCout(5) // PC5 RAC 输出口

#define	W25QXX_CS 	PBout(12)  		//W25QXX的片选信号


void ISD4004_Init(void);//初始化
void ISD4004_PwrUp(void);//PWRUP
void ISD4004_StopMode(void); //停止录音
void ISD4004_StopPWRDN(void);

void ISD4004_RecAddr(u16 addr);  //设置开始录音地址
void Rec(void);  //开始录音


void ISD4004_PlayAddr(u16 addr); //设置播放录音地址
void Play(void);//播放录音

void ISD4004(u16 ISD4004_order);//发送数据



void ISD4004_noCS(u16 ISD4004_order);


		 				    
#endif
