#include "mp3player.h"
#include "vs10xx.h"	 
#include "delay.h"
#include "led.h"
#include "key.h"
#include "lcd.h"		 
#include "malloc.h"
#include "text.h"
#include "string.h"
#include "exfuns.h"  
#include "ff.h"   
#include "usart.h"	
#include "SendData.h"

#include "tpad.h"	
#include "mrrlcd.h"

extern u16 get_txtnum(u8 *path);


u8 DataSendInit(void)
{
	u8 res;u8 rval;
	
	
 	DIR mp3dir;	 		//Ŀ¼
	FILINFO mp3fileinfo;//�ļ���Ϣ
	u8 *fn;   			//���ļ���
	u8 *pname;			//��·�����ļ���
	u16 totmp3num; 		//�����ļ�����
	u16 curindex;		//ͼƬ��ǰ����
//	u8 key;				//��ֵ		  
 	u16 temp;
	u16 *mp3indextbl;	//���������� 
 	while(f_opendir(&mp3dir,"0:/MUSIC"))//�������ļ���
 	{	    
		Show_Str(30,190,240,16,"MUSIC�ļ��д���!",16,0);
		delay_ms(200);				  
		LCD_Fill(30,190,240,206,WHITE);//�����ʾ	     
		delay_ms(200);				  
	} 									  
	totmp3num=mp3_get_tnum("0:/MUSIC"); //�õ�����Ч�ļ���
  	while(totmp3num==NULL)//�����ļ�����Ϊ0		
 	{	    
		Show_Str(30,190,240,16,"û�������ļ�!",16,0);
		delay_ms(200);				  
		LCD_Fill(30,190,240,146,WHITE);//�����ʾ	     
		delay_ms(200);				  
	}										   
  	mp3fileinfo.lfsize=_MAX_LFN*2+1;						//���ļ�����󳤶�
	mp3fileinfo.lfname=mymalloc(SRAMIN,mp3fileinfo.lfsize);	//Ϊ���ļ������������ڴ�
 	pname=mymalloc(SRAMIN,mp3fileinfo.lfsize);				//Ϊ��·�����ļ��������ڴ�
 	mp3indextbl=mymalloc(SRAMIN,2*totmp3num);				//����2*totmp3num���ֽڵ��ڴ�,���ڴ�������ļ�����
 	while(mp3fileinfo.lfname==NULL||pname==NULL||mp3indextbl==NULL)//�ڴ�������
 	{	    
		Show_Str(30,190,240,16,"�ڴ����ʧ��!",16,0);
		delay_ms(200);				  
		LCD_Fill(30,190,240,146,WHITE);//�����ʾ	     
		delay_ms(200);				  
	}  	
//��¼����
    res=f_opendir(&mp3dir,"0:/MUSIC"); //��Ŀ¼
	if(res==FR_OK)
	{
		curindex=0;//��ǰ����Ϊ0
		while(1)//ȫ����ѯһ��
		{
			temp=mp3dir.index;								//��¼��ǰindex
	        res=f_readdir(&mp3dir,&mp3fileinfo);       		//��ȡĿ¼�µ�һ���ļ�
	        if(res!=FR_OK||mp3fileinfo.fname[0]==0)break;	//������/��ĩβ��,�˳�		  
     		fn=(u8*)(*mp3fileinfo.lfname?mp3fileinfo.lfname:mp3fileinfo.fname);			 
			res=f_typetell(fn);	
			if((res&0XF0)==0X40)//ȡ����λ,�����ǲ��������ļ�	
			{
				mp3indextbl[curindex]=temp;//��¼����
				curindex++;
			}	    
		} 
	}   
	f_closedir(&mp3dir);
   	curindex=0;											//��0��ʼ��ʾ
   	res=f_opendir(&mp3dir,(const TCHAR*)"0:/MUSIC"); 	//��Ŀ¼
	while(res==FR_OK)//�򿪳ɹ�
	{	
		dir_sdi(&mp3dir,mp3indextbl[curindex]);			//�ı䵱ǰĿ¼����	   
        res=f_readdir(&mp3dir,&mp3fileinfo);       		//��ȡĿ¼�µ�һ���ļ�
		
        if(res!=FR_OK||mp3fileinfo.fname[0]==0)break;	//������/��ĩβ��,�˳�
     	fn=(u8*)(*mp3fileinfo.lfname?mp3fileinfo.lfname:mp3fileinfo.fname);	

		
		strcpy((char*)pname,"0:/MUSIC/");				//����·��(Ŀ¼)
		strcat((char*)pname,(const char*)fn);  			//���ļ������ں���
 		LCD_Fill(30,190,240,190+16,WHITE);				//���֮ǰ����ʾ
		Show_Str(30,190,240-30,16,fn,16,0);				//��ʾ�������� 
		mp3_index_show(curindex+1,totmp3num);
		
		//Show_Str(30,450,310,24,pname,24,0);
		//Show_Str(30,480,310,24,fn,24,0);
		
		if(DataSend(0,pname,1))
		{	LCD_ShowString(30,300,310,16,24,"music send fail!");
			rval =1;
			return rval;
		}
		else
		{
			LCD_ShowString(30,300,310,16,24,"music send succeed!");
			break;
		}
		
			


	}
		f_closedir(&mp3dir);
		myfree(SRAMIN,mp3fileinfo.lfname);	//�ͷ��ڴ�			    
	myfree(SRAMIN,pname);				//�ͷ��ڴ�			    
	myfree(SRAMIN,mp3indextbl);			//�ͷ��ڴ�	 
	rval=0;
	return rval;
}





//rval=0XFF ;//ʧ��.
//rval = 0;  //�ɹ�
//type=0 ��������
//type=1 ����¼���ļ�
//type=2 �����ı��ļ�
u8 DataSend(u8 type,u8 *pname,u16 rate)//usΪ��λ
{		
	u8 *rpname=0;
	FIL* fmp3;
	FIL* fmw;
    u16 br;
	u16 bw;
	u8 res,res1,rval;	  
	u8 *databuf,*p,tmp;	   		   
	u16 j=0;
	//Show_Str(30,510,310,24,pname,24,0);	

	rval=0;	    
	fmp3=(FIL*)mymalloc(SRAMIN,sizeof(FIL));//�����ڴ�
	fmw=(FIL*)mymalloc(SRAMIN,sizeof(FIL));//�����ڴ�
	
	rpname=mymalloc(SRAMIN,30);
	
	databuf=(u8*)mymalloc(SRAMIN,4096);		//����4096�ֽڵ��ڴ�����
	if(databuf==NULL||fmp3==NULL)rval=0XFF ;//�ڴ�����ʧ��.
	//Show_Str(30,5,310,24,pname,24,0);	 //����
	//LCD_ShowNum(30,5,rval,2,16);  						//��ʾһ������
	if(rval==0)
	{	
		//create_new_pathname(rpname);
		if(type==2)
		{
			create_nsendtxt_pathname(rpname);
		}
		else if(type==1)
		{
			create_nsendrec_pathname(rpname);
		}else
		{
			create_nsendmsc_pathname(rpname);
		}
		
		
		//LCD_ShowNum(30,5,rval,2,16); 
		//Show_Str(30,50,310,24,pname,24,0);	 //����
		res1 = f_open(fmw,(const TCHAR*)rpname,FA_CREATE_ALWAYS|FA_WRITE);
		
		res=f_open(fmp3,(const TCHAR*)pname,FA_READ);//���ļ�	
		
		
		//Show_Str(30,110,310,24,pname,24,0);	 //����

 		if(res==0)//�򿪳ɹ�.
		{ 
		
			
				while(res1 == FR_OK && fmp3->fptr < fmp3->fsize)//�ļ��Ƿ������
					{

						res=f_read(fmp3,databuf,4096,(UINT*)&br);//����4096���ֽ�  
						p = databuf;
				
						f_write(fmw,databuf,br,(UINT*)&bw);//4096���ֽ�
						tmp=*p;
						
						//LCD_ShowString(30,360,310,16,24,&tmp);
						//LCD_ShowxNum(30,360,tmp,sizeof(tmp),16,0);
						for(j=0;j<8;j++)//OOK���� ��λMSB�ȷ�
						{
							if(tmp&0x80)		//�ߵ�ƽ
							{
								LED1=0;
								LED0=0;
								/***************************/
								//�������Ų�������
								
								
								
								/***************************/
								//LCD_ShowString(30,380,310,16,24,"1");
							}
							else				//�͵�ƽ
							{
								LED1=1;
								LED0=1;
								/***************************/
								//�������Ų�������
								
								
								
								/***************************/
								
								//LCD_ShowString(30,380,310,16,24,"0");
							}
							tmp<<=1;
							delay_us(rate); //���Ƶ������� 
						}
							p++;
						if(TPAD_Scan(0))//ͷ�� ������һ��
						{
							rval=0xFF;
							f_close(fmp3);	
							f_close(fmw);
							
							myfree(SRAMIN,databuf);	  	 		  	    
							myfree(SRAMIN,fmp3);
							myfree(SRAMIN,fmw);
							return rval;
						}
 					 }		
						rval = 0;
		}	
		else 	
		rval=0XFF;//���ִ���	   	  
	}	
	//mp3_msg_show(fmp3->fsize);//��ʾ��Ϣ	  	
	f_close(fmp3);	
	f_close(fmw);
	
	myfree(SRAMIN,databuf);	  	 		  	    
	myfree(SRAMIN,fmp3);
	myfree(SRAMIN,fmw);
	return rval;

}




//ͨ��ʱ���ȡ�ļ���
//������SD������,��֧��FLASH DISK����
//��ϳ�:����"0:RECORDER/REC20120321210633.wav"���ļ���
void create_new_pathname(u8 *pname)
{	 
	u8 res;					 
	u16 index=0;
	while(index<0XFFFF)
	{
		sprintf((char*)pname,"0:RECEIVE/REV%05d.wav",index);
		res=f_open(ftemp,(const TCHAR*)pname,FA_READ);//���Դ�����ļ�
		if(res==FR_NO_FILE)break;		//���ļ���������=����������Ҫ��.
		index++;
	}
}
void create_nsendmsc_pathname(u8 *pname)
{	 
	u8 res;					 
	u16 index=0;
	while(index<0XFFFF)
	{
		sprintf((char*)pname,"0:RECEIVE/SENDMSC%03d.wav",index);
		res=f_open(ftemp,(const TCHAR*)pname,FA_READ);//���Դ�����ļ�
		if(res==FR_NO_FILE)break;		//���ļ���������=����������Ҫ��.
		index++;
	}
}
void create_nsendrec_pathname(u8 *pname)
{	 
	u8 res;					 
	u16 index=0;
	while(index<0XFFFF)
	{
		sprintf((char*)pname,"0:RECEIVE/SENDREC%03d.wav",index);
		res=f_open(ftemp,(const TCHAR*)pname,FA_READ);//���Դ�����ļ�
		if(res==FR_NO_FILE)break;		//���ļ���������=����������Ҫ��.
		index++;
	}
}

void create_nsendtxt_pathname(u8 *pname)
{	 
	u8 res;					 
	u16 index=0;
	FIL* fp;
	
		while(index<0XFFFF)
	{
		sprintf((char*)pname,"0:RECEIVE/SENDtxt%03d.txt",index);
		//Show_Str(30,0,310,24,pname,24,0);	 //����
		res=f_open(fp,(const TCHAR*)pname,FA_READ);//���Դ�����ļ�
		
		//Show_Str(30,0,310,24,pname,24,0);	 //����
		if(res==FR_NO_FILE)break;		//���ļ���������=����������Ҫ��.
		index++;
	}
}







//���ַ��ͳ���
//rval= 0���ͳɹ�
u8 musictransfer(u8 number,u16 rate)
{
			u8 rval;
			u8 res;
			u8 *fn;   			//���ļ���
			u8 *pname;			//��·�����ļ���
			u16 curindex;		//ͼƬ��ǰ����
			u16 temp;
			u16 *mp3indextbl;	//���������� 
			DIR mp3dir;	 		//Ŀ¼
			FILINFO mp3fileinfo;//�ļ���Ϣ
		u16 totmp3num; 		//�����ļ�����
		totmp3num=0; 
		totmp3num=mp3_get_tnum("0:/MUSIC"); //�õ�����Ч�ļ���
		mp3fileinfo.lfsize=_MAX_LFN*2+1;						//���ļ�����󳤶�
		mp3fileinfo.lfname=mymalloc(SRAMIN,mp3fileinfo.lfsize);	//Ϊ���ļ������������ڴ�
		pname=mymalloc(SRAMIN,mp3fileinfo.lfsize);				//Ϊ��·�����ļ��������ڴ�
			
		mp3indextbl=mymalloc(SRAMIN,2*totmp3num);	
		res=f_opendir(&mp3dir,"0:/MUSIC"); //��Ŀ¼

			if(res==FR_OK)
			{
				curindex=0;//��ǰ����Ϊ0
				while(1)//ȫ����ѯһ��
				{
					temp=mp3dir.index;								//��¼��ǰindex
					res=f_readdir(&mp3dir,&mp3fileinfo);       		//��ȡĿ¼�µ�һ���ļ�
					if(res!=FR_OK||mp3fileinfo.fname[0]==0)break;	//������/��ĩβ��,�˳�		  
					fn=(u8*)(*mp3fileinfo.lfname?mp3fileinfo.lfname:mp3fileinfo.fname);			 
					res=f_typetell(fn);	
					if((res&0XF0)==0X40)//ȡ����λ,�����ǲ��������ļ�	//��������exfuns.c�ļ�
					{
						mp3indextbl[curindex]=temp;//��¼����
						curindex++;
					}	    
				} 
			}   
			f_closedir(&mp3dir);
			
			curindex=number;											//��0��ʼ��ʾ
			res=f_opendir(&mp3dir,(const TCHAR*)"0:/MUSIC"); 	//��Ŀ¼													
			dir_sdi(&mp3dir,mp3indextbl[curindex]);			//�ı䵱ǰĿ¼����	   
			res=f_readdir(&mp3dir,&mp3fileinfo);       		//��ȡĿ¼�µ�һ���ļ�
			//if(res!=FR_OK||mp3fileinfo.fname[0]==0)break;	//������/��ĩβ��,�˳�
			fn=(u8*)(*mp3fileinfo.lfname?mp3fileinfo.lfname:mp3fileinfo.fname);																
			strcpy((char*)pname,"0:/MUSIC/");				//����·��(Ŀ¼)
			strcat((char*)pname,(const char*)fn);  			//���ļ������ں���
			
			Show_Str(30,730,240,16,pname,16,0); //����
			
			
			
			// ���Ϳ�ʼ������ʾ��Ϣ
			LCD_Fill(30,750,240,800,WHITE);				//���֮ǰ����ʾ
			Show_Str(30,750,240,16,"���ݷ���......",16,0);
			
			//u8 DataSend(u8 *pname,rate);//usΪ��λ�ĵ�������
			//1KH��������
			if(DataSend(0,pname,rate)==0) //���ͳɹ�
			{
				// ���ʹ��������ʾ��Ϣ
				LCD_Fill(30,750,240,800,WHITE);				//���֮ǰ����ʾ
				Show_Str(30,750,240,16,"���ݷ������",16,0);
				rval = 0;
			
			}else
			{
			
				// ���ʹ�����ֹ/ʧ����ʾ��Ϣ
				LCD_Fill(30,750,240,800,WHITE);				//���֮ǰ����ʾ
				Show_Str(30,750,240,16,"���ݷ���ʧ��/��ֹ",16,0);
				f_closedir(&mp3dir);
				myfree(SRAMIN,mp3fileinfo.lfname);	//�ͷ��ڴ�			    
				myfree(SRAMIN,pname);				//�ͷ��ڴ�		
				myfree(SRAMIN,mp3indextbl);			//�ͷ��ڴ�
				delay_ms(3000);
				rval = 0xff;
			}
			
			
			f_closedir(&mp3dir);
			myfree(SRAMIN,mp3fileinfo.lfname);	//�ͷ��ڴ�			    
			myfree(SRAMIN,pname);				//�ͷ��ڴ�		
			myfree(SRAMIN,mp3indextbl);			//�ͷ��ڴ�
			return rval;

}

//¼���ļ����ͳ���
//rval= 0���ͳɹ�
u8 recordertransfer(u8 number,u16 rate)
{
			u8 rval;
			u8 res;
			u8 *fn;   			//���ļ���
			u8 *pname;			//��·�����ļ���
			u16 curindex;		//ͼƬ��ǰ����
			u16 temp;
			u16 *mp3indextbl;	//���������� 
			DIR mp3dir;	 		//Ŀ¼
			FILINFO mp3fileinfo;//�ļ���Ϣ
		u16 totmp3num; 		//�����ļ�����
		totmp3num=0; 
		totmp3num=mp3_get_tnum("0:/RECORDER"); //�õ�����Ч�ļ���
		mp3fileinfo.lfsize=_MAX_LFN*2+1;						//���ļ�����󳤶�
		mp3fileinfo.lfname=mymalloc(SRAMIN,mp3fileinfo.lfsize);	//Ϊ���ļ������������ڴ�
		pname=mymalloc(SRAMIN,mp3fileinfo.lfsize);				//Ϊ��·�����ļ��������ڴ�
			
		mp3indextbl=mymalloc(SRAMIN,2*totmp3num);	
		res=f_opendir(&mp3dir,"0:/RECORDER"); //��Ŀ¼

			if(res==FR_OK)
			{
				curindex=0;//��ǰ����Ϊ0
				while(1)//ȫ����ѯһ��
				{
					temp=mp3dir.index;								//��¼��ǰindex
					res=f_readdir(&mp3dir,&mp3fileinfo);       		//��ȡĿ¼�µ�һ���ļ�
					if(res!=FR_OK||mp3fileinfo.fname[0]==0)break;	//������/��ĩβ��,�˳�		  
					fn=(u8*)(*mp3fileinfo.lfname?mp3fileinfo.lfname:mp3fileinfo.fname);			 
					res=f_typetell(fn);	
					if((res&0XF0)==0X40)//ȡ����λ,�����ǲ��������ļ�	//��������exfuns.c�ļ�
					{
						mp3indextbl[curindex]=temp;//��¼����
						curindex++;
					}	    
				} 
			}   
			f_closedir(&mp3dir);
			
			curindex=number;											//��0��ʼ��ʾ
			res=f_opendir(&mp3dir,(const TCHAR*)"0:/RECORDER"); 	//��Ŀ¼													
			dir_sdi(&mp3dir,mp3indextbl[curindex]);			//�ı䵱ǰĿ¼����	   
			res=f_readdir(&mp3dir,&mp3fileinfo);       		//��ȡĿ¼�µ�һ���ļ�
			//if(res!=FR_OK||mp3fileinfo.fname[0]==0)break;	//������/��ĩβ��,�˳�
			fn=(u8*)(*mp3fileinfo.lfname?mp3fileinfo.lfname:mp3fileinfo.fname);																
			strcpy((char*)pname,"0:/RECORDER/");				//����·��(Ŀ¼)
			strcat((char*)pname,(const char*)fn);  			//���ļ������ں���
			
			Show_Str(30,730,240,16,pname,16,0); //����
			
			
			
			// ���Ϳ�ʼ������ʾ��Ϣ
			LCD_Fill(30,750,240,800,WHITE);				//���֮ǰ����ʾ
			Show_Str(30,750,240,16,"���ݷ���......",16,0);
			
			//u8 DataSend(u8 *pname,rate);//usΪ��λ�ĵ�������
			//1KH��������
			if(DataSend(1,pname,rate)==0) //���ͳɹ�
			{
				// ���ʹ��������ʾ��Ϣ
				LCD_Fill(30,750,240,800,WHITE);				//���֮ǰ����ʾ
				Show_Str(30,750,240,16,"���ݷ������",16,0);
				rval = 0;
			
			}else
			{
			
				// ���ʹ�����ֹ/ʧ����ʾ��Ϣ
				LCD_Fill(30,750,240,800,WHITE);				//���֮ǰ����ʾ
				Show_Str(30,750,240,16,"���ݷ���ʧ��/��ֹ",16,0);
				f_closedir(&mp3dir);
				myfree(SRAMIN,mp3fileinfo.lfname);	//�ͷ��ڴ�			    
				myfree(SRAMIN,pname);				//�ͷ��ڴ�		
				myfree(SRAMIN,mp3indextbl);			//�ͷ��ڴ�
				delay_ms(3000);
				rval = 0xff;
			}
			
			
			f_closedir(&mp3dir);
			myfree(SRAMIN,mp3fileinfo.lfname);	//�ͷ��ڴ�			    
			myfree(SRAMIN,pname);				//�ͷ��ڴ�		
			myfree(SRAMIN,mp3indextbl);			//�ͷ��ڴ�
			return rval;

}

//�ı����ͳ���
//rval= 0���ͳɹ�
u8 txttransfer(u8 number,u16 rate)
{
			u8 rval;
			u8 res;
			u8 *fn;   			//���ļ���
			u8 *pname;			//��·�����ļ���
			u16 curindex;		//ͼƬ��ǰ����
			u16 temp;
			u16 *mp3indextbl;	//���������� 
			DIR mp3dir;	 		//Ŀ¼
			FILINFO mp3fileinfo;//�ļ���Ϣ
		u16 totmp3num; 		//�����ļ�����
		totmp3num=0; 
		totmp3num=get_txtnum("0:/TEXT"); //�õ�����Ч�ļ���
		mp3fileinfo.lfsize=_MAX_LFN*2+1;						//���ļ�����󳤶�
		mp3fileinfo.lfname=mymalloc(SRAMIN,mp3fileinfo.lfsize);	//Ϊ���ļ������������ڴ�
		pname=mymalloc(SRAMIN,mp3fileinfo.lfsize);				//Ϊ��·�����ļ��������ڴ�
			
		mp3indextbl=mymalloc(SRAMIN,2*totmp3num);	
		res=f_opendir(&mp3dir,"0:/TEXT"); //��Ŀ¼

			if(res==FR_OK)
			{
				curindex=0;//��ǰ����Ϊ0
				while(1)//ȫ����ѯһ��
				{
					temp=mp3dir.index;								//��¼��ǰindex
					res=f_readdir(&mp3dir,&mp3fileinfo);       		//��ȡĿ¼�µ�һ���ļ�
					if(res!=FR_OK||mp3fileinfo.fname[0]==0)break;	//������/��ĩβ��,�˳�		  
					fn=(u8*)(*mp3fileinfo.lfname?mp3fileinfo.lfname:mp3fileinfo.fname);			 
					res=f_typetell(fn);	
					if((res&0XF0)==0X30)//ȡ����λ,�����ǲ��������ļ�	//��������exfuns.c�ļ�
					{
						mp3indextbl[curindex]=temp;//��¼����
						curindex++;
					}	    
				} 
			}   
			f_closedir(&mp3dir);
			
			curindex=number;											//��0��ʼ��ʾ
			res=f_opendir(&mp3dir,(const TCHAR*)"0:/TEXT"); 	//��Ŀ¼													
			dir_sdi(&mp3dir,mp3indextbl[curindex]);			//�ı䵱ǰĿ¼����	   
			res=f_readdir(&mp3dir,&mp3fileinfo);       		//��ȡĿ¼�µ�һ���ļ�
			//if(res!=FR_OK||mp3fileinfo.fname[0]==0)break;	//������/��ĩβ��,�˳�
			fn=(u8*)(*mp3fileinfo.lfname?mp3fileinfo.lfname:mp3fileinfo.fname);																
			strcpy((char*)pname,"0:/TEXT/");				//����·��(Ŀ¼)
			strcat((char*)pname,(const char*)fn);  			//���ļ������ں���
			
			Show_Str(30,730,240,16,pname,16,0); //����
			
			
			
			// ���Ϳ�ʼ������ʾ��Ϣ
			LCD_Fill(30,750,240,800,WHITE);				//���֮ǰ����ʾ
			Show_Str(30,750,240,16,"���ݷ���......",16,0);
			
			//u8 DataSend(u8 *pname,rate);//usΪ��λ�ĵ�������
			//1KH��������
			if(DataSend(2,pname,rate)==0) //���ͳɹ�
			{
				// ���ʹ��������ʾ��Ϣ
				LCD_Fill(30,750,240,800,WHITE);				//���֮ǰ����ʾ
				Show_Str(30,750,240,16,"���ݷ������",16,0);
				rval = 0;
			
			}else
			{
			
				// ���ʹ�����ֹ/ʧ����ʾ��Ϣ
				LCD_Fill(30,750,240,800,WHITE);				//���֮ǰ����ʾ
				Show_Str(30,750,240,16,"���ݷ���ʧ��/��ֹ",16,0);
				f_closedir(&mp3dir);
				myfree(SRAMIN,mp3fileinfo.lfname);	//�ͷ��ڴ�			    
				myfree(SRAMIN,pname);				//�ͷ��ڴ�		
				myfree(SRAMIN,mp3indextbl);			//�ͷ��ڴ�
				delay_ms(3000);
				rval = 0xff;
			}
			
			
			f_closedir(&mp3dir);
			myfree(SRAMIN,mp3fileinfo.lfname);	//�ͷ��ڴ�			    
			myfree(SRAMIN,pname);				//�ͷ��ڴ�		
			myfree(SRAMIN,mp3indextbl);			//�ͷ��ڴ�
			return rval;

}


//u8 DataSend(u8 *pname)
//{
//			
//	FIL* fmp3;
//	FIL* fmw;
//    u16 br;
//	u16 bw;
//	u8 res,rval;	  
//	u8 *databuf,*p,tmp;	   		   
//	u16 i=0,j=0;
//	Show_Str(30,510,310,24,pname,24,0);	

//	rval=0;	    
//	fmp3=(FIL*)mymalloc(SRAMIN,sizeof(FIL));//�����ڴ�
//	fmw=(FIL*)mymalloc(SRAMIN,sizeof(FIL));//�����ڴ�
//	databuf=(u8*)mymalloc(SRAMIN,4096);		//����4096�ֽڵ��ڴ�����
//	if(databuf==NULL||fmp3==NULL)rval=0XFF ;//�ڴ�����ʧ��.
//	if(rval==0)
//	{	

//		
//		f_open(fmw,(const TCHAR*)"0:/TEXT/test.mp3",FA_CREATE_ALWAYS|FA_WRITE);
//		res=f_open(fmp3,(const TCHAR*)pname,FA_READ);//���ļ�	

// 		if(res==0)//�򿪳ɹ�.
//		{ 
//				res=f_read(fmp3,databuf,4096,(UINT*)&br);//����4096���ֽ�  
//				p = databuf;
//				
//				f_write(fmw,databuf,br,(UINT*)&bw);//4096���ֽ�
//			
//			
//				i=0;
//				do
//				{
//					tmp=*p;
//					//LCD_ShowString(30,360,310,16,24,&tmp);
//					//LCD_ShowxNum(30,360,tmp,sizeof(tmp),16,0);
//					for(j=0;j<8;j++)
//					{
//						if(tmp&0x80)
//						{
//							LED1=0;
//							LED0=0;
//							//LCD_ShowString(30,380,310,16,24,"1");
//						}
//						else
//						{
//							LED1=1;
//							LED0=1;
//							//LCD_ShowString(30,380,310,16,24,"0");
//						}
//						tmp<<=1;
//						delay_us(1);
//					}
//					
//					i++;
//					p++;
//					
//				}while(i<4096);//ѭ������4096���ֽ� 
//				
//				if(br!=4096||res!=0)
//				{
//					rval=0;	  
//				} 							 
//		}	
//		else 	
//		rval=0XFF;//���ִ���	   	  
//	}	
//	//mp3_msg_show(fmp3->fsize);//��ʾ��Ϣ	  	
//	f_close(fmp3);	
//	f_close(fmw);
//	
//	myfree(SRAMIN,databuf);	  	 		  	    
//	myfree(SRAMIN,fmp3);
//	myfree(SRAMIN,fmw);
//	return rval;

//}
/************************************************************************************/













///*-----------------------------------------------------------------------*/
///* ʾ�����룺���ݴ��亯��������f_forward��������                                 */
///*-----------------------------------------------------------------------*/
// 
//UINT out_stream (   /* �����ѷ����ֽ�������״̬ */
//    const BYTE *p,  /* �������͵����ݿ��ָ�� */
//    UINT btf        /* >0: �������(�������͵��ֽ���)��0: ������ */
//)
//{
//    UINT cnt = 0;
// 
//    if (btf == 0) {     /* ������ */
//        /* ������״̬(0: æ��1: ����) */
//        /* ��������ʱ��һ�������ؾ�������ô�ں����Ĵ������ʱ���������������һ���ֽڣ�����f_forward����FR_INT_ERROR��ʧ�ܡ� */
//        if (FIFO_READY) cnt = 1;
//    }
//    else {              /* ������� */
//        do {    /* ��������Ҫ���Ͳ���������ʱ�ظ� */
//            FIFO_PORT = *p++;
//            cnt++;
//        } while (cnt < btf && FIFO_READY);
//    }
// 
//    return cnt;
//}


///*-----------------------------------------------------------------------*/
///* ʾ�����룺ʹ��f_forward����                                                    */
///*-----------------------------------------------------------------------*/
// 
//FRESULT play_file (
//    char *fn        /* �����ŵ���Ƶ�ļ�����ָ�� */
//)
//{
//    FRESULT rc;
//    FIL fil;
//    UINT dmy;
// 
//    /* ��ֻ��ģʽ����Ƶ�ļ� */
//    rc = f_open(&fil, fn, FA_READ);
// 
//    /* �ظ���ֱ���ļ�ָ�뵽���ļ�����λ�� */
//    while (rc == FR_OK && fil.fptr < fil.fsize) {
// 
//        /* �κ���������... */
// 
//        /* ���ڻ�����ʽ�������� */
//        rc = f_forward(&fil, out_stream, 1000, &dmy);
//    }
// 
//    /* ��ֻ������Ƶ�ļ�������Ҫ�رվͿ��Ա����� */
//    return rc;
//}
//	


