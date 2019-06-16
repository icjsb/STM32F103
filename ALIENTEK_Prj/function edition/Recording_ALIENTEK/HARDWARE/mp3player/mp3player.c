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
#include "flac.h"	
#include "usart.h"	
#include "tpad.h"	

#include "mrrlcd.h"
#include "touch.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
//ALIENTEKս��STM32������V3
//MP3�������� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/1/20
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) �������������ӿƼ����޹�˾ 2009-2019
//All rights reserved								  						    								  
//////////////////////////////////////////////////////////////////////////////////


//static u16 looknumb;

//��ʾ��Ŀ����
//index:��ǰ����
//total:���ļ���
void mp3_index_show(u16 index,u16 total)
{
	//��ʾ��ǰ��Ŀ������,������Ŀ��
	LCD_ShowxNum(30+0,530+220,index,3,16,0X80);		//����
	LCD_ShowChar(30+24,530+220,'/',16,0);
	LCD_ShowxNum(30+32,530+220,total,3,16,0X80); 	//����Ŀ				  	  
}
//��ʾ��ǰ����
void mp3_vol_show(u8 vol)
{			    
	LCD_ShowString(30+110,530+220,200,16,16,"VOL:");	  	  
	LCD_ShowxNum(30+142,530+220,vol,2,16,0X80); 	//��ʾ����	 
}
u16 f_kbps=0;//�����ļ�λ��	
//��ʾ����ʱ��,������ ��Ϣ 
//lenth:�����ܳ���
void mp3_msg_show(u32 lenth)
{	
	static u16 playtime=0;//����ʱ����	     
 	u16 time=0;// ʱ�����
	u16 temp=0;	  
	if(f_kbps==0xffff)//δ���¹�
	{
		playtime=0;
		f_kbps=VS_Get_HeadInfo();	   //��ñ�����
	}	 	 
	time=VS_Get_DecodeTime(); //�õ�����ʱ��
	if(playtime==0)playtime=time;
	else if((time!=playtime)&&(time!=0))//1sʱ�䵽,������ʾ����
	{
		playtime=time;//����ʱ�� 	 				    
		temp=VS_Get_HeadInfo(); //��ñ�����	   				 
		if(temp!=f_kbps)
		{
			f_kbps=temp;//����KBPS	  				     
		}			 
		//��ʾ����ʱ��			 
		LCD_ShowxNum(30,510+220,time/60,2,16,0X80);		//����
		LCD_ShowChar(30+16,510+220,':',16,0);
		LCD_ShowxNum(30+24,510+220,time%60,2,16,0X80);	//����		
 		LCD_ShowChar(30+40,510+220,'/',16,0); 	    	 
		//��ʾ��ʱ��
		if(f_kbps)time=(lenth/f_kbps)/125;//�õ�������   (�ļ�����(�ֽ�)/(1000/8)/������=����������    	  
		else time=0;//�Ƿ�λ��	  
 		LCD_ShowxNum(30+48,510+220,time/60,2,16,0X80);	//����
		LCD_ShowChar(30+64,510+220,':',16,0);
		LCD_ShowxNum(30+72,510+220,time%60,2,16,0X80);	//����	  		    
		//��ʾλ��			   
   		LCD_ShowxNum(30+110,510+220,f_kbps,3,16,0X80); 	//��ʾλ��	 
		LCD_ShowString(30+134,510+220,200,16,16,"Kbps");	  	  
		LED0=!LED0;		//DS0��ת
	}   		 
}			  		 
//�õ�path·����,Ŀ���ļ����ܸ���
//path:·��		    
//����ֵ:����Ч�ļ���
u16 mp3_get_tnum(u8 *path)
{	  
	u8 res;
	u16 rval=0;
 	DIR tdir;	 		//��ʱĿ¼
	FILINFO tfileinfo;	//��ʱ�ļ���Ϣ		
	u8 *fn; 			 			   			     
    res=f_opendir(&tdir,(const TCHAR*)path); //��Ŀ¼
  	tfileinfo.lfsize=_MAX_LFN*2+1;						//���ļ�����󳤶�
	tfileinfo.lfname=mymalloc(SRAMIN,tfileinfo.lfsize);	//Ϊ���ļ������������ڴ�
	if(res==FR_OK&&tfileinfo.lfname!=NULL)
	{
		while(1)//��ѯ�ܵ���Ч�ļ���
		{
	        res=f_readdir(&tdir,&tfileinfo);       		//��ȡĿ¼�µ�һ���ļ�
	        if(res!=FR_OK||tfileinfo.fname[0]==0)break;	//������/��ĩβ��,�˳�		  
     		fn=(u8*)(*tfileinfo.lfname?tfileinfo.lfname:tfileinfo.fname);			 
			res=f_typetell(fn);	
			if((res&0XF0)==0X40)//ȡ����λ,�����ǲ��������ļ�	
			{
				rval++;//��Ч�ļ�������1
			}	    
		}  
	} 
	myfree(SRAMIN,tfileinfo.lfname);
	return rval;
}
//��������
void mp3_play(void)
{
	u8 res;
 	DIR mp3dir;	 		//Ŀ¼
	FILINFO mp3fileinfo;//�ļ���Ϣ
	u8 *fn;   			//���ļ���
	u8 *pname;			//��·�����ļ���
	u16 totmp3num; 		//�����ļ�����
	u16 curindex;		//ͼƬ��ǰ����
	u8 key;				//��ֵ		  
 	u16 temp;
	u16 *mp3indextbl;	//���������� 
	//u8 looknumb=0;
	
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
	VS_HD_Reset();
	VS_Soft_Reset();
	vsset.mvol=220;						//Ĭ����������Ϊ220.
	mp3_vol_show((vsset.mvol-100)/5);	//����������:100~250,��ʾ��ʱ��,���չ�ʽ(vol-100)/5,��ʾ,Ҳ����0~30   
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
 		LCD_Fill(30,490+220,480,190+16,WHITE);				//���֮ǰ����ʾ
		Show_Str(30,490+220,480-30,16,fn,16,0);				//��ʾ�������� 
		//Show_Str(30,760,480,16,fn,16,0);
		mp3_index_show(curindex+1,totmp3num);
		key=mp3_play_song(pname); 				 		//�������MP3  
	
		
		if(key==2)		//��һ��
		{
			if(curindex)
			{
				curindex--;
		
			}
			else curindex=totmp3num-1;
 		}else if(key<=1)//��һ��
		{
			curindex++;		   	
			if(curindex>=totmp3num)curindex=0;//��ĩβ��ʱ��,�Զ���ͷ��ʼ
 		}else if(key == 3)
		{
		  goto endplay;
		}
		else
			 break;	//�����˴��� 	 
	}
endplay:	
	f_closedir(&mp3dir);
	myfree(SRAMIN,mp3fileinfo.lfname);	//�ͷ��ڴ�			    
	myfree(SRAMIN,pname);				//�ͷ��ڴ�			    
	myfree(SRAMIN,mp3indextbl);			//�ͷ��ڴ�	
	
}


void mp3play(u8 playindex)
{
	
	u8 firsttime=0;
	u8 lastupdate=0;
	
	u8 res;
 	DIR mp3dir;	 		//Ŀ¼
	FILINFO mp3fileinfo;//�ļ���Ϣ
	u8 *fn;   			//���ļ���
	u8 *pname;			//��·�����ļ���
	u16 totmp3num; 		//�����ļ�����
	u16 curindex;		//ͼƬ��ǰ����
	u8 key;				//��ֵ		  
 	u16 temp;
	u16 *mp3indextbl;	//���������� 
	//u8 looknumb=0;
	
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
	VS_HD_Reset();
	VS_Soft_Reset();
	vsset.mvol=220;						//Ĭ����������Ϊ220.
	mp3_vol_show((vsset.mvol-100)/5);	//����������:100~250,��ʾ��ʱ��,���չ�ʽ(vol-100)/5,��ʾ,Ҳ����0~30   
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
   	curindex=playindex;											//��0��ʼ��ʾ
   	res=f_opendir(&mp3dir,(const TCHAR*)"0:/MUSIC"); 	//��Ŀ¼
	while(res==FR_OK)//�򿪳ɹ�
	{	
				if(!firsttime)
		{
			firsttime=1;
			draw_dir(540,curindex);//��ʾ����Ŀ¼��y,Ŀ¼����
		}
		if(lastupdate!=curindex)
		{
			draw_dir(540,curindex);//��ʾ����Ŀ¼��y,Ŀ¼����
		}
		lastupdate=curindex-1;	
		
		
		dir_sdi(&mp3dir,mp3indextbl[curindex]);			//�ı䵱ǰĿ¼����	   
        res=f_readdir(&mp3dir,&mp3fileinfo);       		//��ȡĿ¼�µ�һ���ļ�
        if(res!=FR_OK||mp3fileinfo.fname[0]==0)break;	//������/��ĩβ��,�˳�
     	fn=(u8*)(*mp3fileinfo.lfname?mp3fileinfo.lfname:mp3fileinfo.fname);		
	
		strcpy((char*)pname,"0:/MUSIC/");				//����·��(Ŀ¼)
		strcat((char*)pname,(const char*)fn);  			//���ļ������ں���
 		LCD_Fill(30,490+220,480,190+16,WHITE);				//���֮ǰ����ʾ
		Show_Str(30,490+220,480-30,16,fn,16,0);				//��ʾ�������� 
		//Show_Str(30,760,480,16,fn,16,0);
		
		key=mp3_play_song(pname); 				 		//�������MP3  
		
	
		mp3_index_show(curindex+1,totmp3num);
		if(key==2)		//��һ��
		{
			if(curindex)
			{
				curindex--;
		
			}
			else curindex=totmp3num-1;
 		}else if(key<=1)//��һ��
		{
			curindex++;		   	
			if(curindex>=totmp3num)curindex=0;//��ĩβ��ʱ��,�Զ���ͷ��ʼ
 		}else if(key == 3)
		{	
			
			//Show_Str(450-16*5-80-16*5,540+80,240,16,"������½�ͷ�񷵻���һ��/ֹͣ����",16,0);
			LCD_Fill(450-16*5-80-16*5,540+80,500,540+80+16,WHITE);				//���֮ǰ����ʾ
			LCD_Fill(30,400+220,250,800,WHITE);				//���֮ǰ����ʾ
			LCD_Fill(450-16*5-80-16*5,540+60,500,540+60+16,WHITE);				//���֮ǰ����ʾ
			
			LCD_Fill(450-16*5-16*5,540+60,500,540+60+16,WHITE);				//���֮ǰ����ʾ
			Show_Str(450-16*5-80-16*5,540+60,240,16,"���سɹ�",16,0);
			delay_ms(3000);
			LCD_Fill(450-16*5-80-16*5,540+60,500,540+60+16,WHITE);				//���֮ǰ����ʾ
			Show_Str(450-16*5-80-16*5,540+60,240,16,"��ѡ����",16,0);
		  goto endplay;
		}
		else
			 break;	//�����˴��� 	 
	}
endplay:	
	f_closedir(&mp3dir);
	myfree(SRAMIN,mp3fileinfo.lfname);	//�ͷ��ڴ�			    
	myfree(SRAMIN,pname);				//�ͷ��ڴ�			    
	myfree(SRAMIN,mp3indextbl);			//�ͷ��ڴ�	
	
}

void mp3playREC(u8 playindex)
{
	
	u8 firsttime=0;
	u8 lastupdate=0;
	
	u8 res;
 	DIR mp3dir;	 		//Ŀ¼
	FILINFO mp3fileinfo;//�ļ���Ϣ
	u8 *fn;   			//���ļ���
	u8 *pname;			//��·�����ļ���
	u16 totmp3num; 		//�����ļ�����
	u16 curindex;		//ͼƬ��ǰ����
	u8 key;				//��ֵ		  
 	u16 temp;
	u16 *mp3indextbl;	//���������� 
	//u8 looknumb=0;
	
 	while(f_opendir(&mp3dir,"0:/RECORDER"))//�������ļ���
 	{	    
		Show_Str(30,190,240,16,"RECORDER�ļ��д���!",16,0);
		delay_ms(200);				  
		LCD_Fill(30,190,240,206,WHITE);//�����ʾ	     
		delay_ms(200);				  
	} 									  
	totmp3num=mp3_get_tnum("0:/RECORDER"); //�õ�����Ч�ļ���
  	while(totmp3num==NULL)//�����ļ�����Ϊ0		
 	{	    
		Show_Str(30,190,240,16,"û��¼���ļ�!",16,0);
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
	VS_HD_Reset();
	VS_Soft_Reset();
	vsset.mvol=220;						//Ĭ����������Ϊ220.
	mp3_vol_show((vsset.mvol-100)/5);	//����������:100~250,��ʾ��ʱ��,���չ�ʽ(vol-100)/5,��ʾ,Ҳ����0~30   
	//��¼����
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
			if((res&0XF0)==0X40)//ȡ����λ,�����ǲ��������ļ�	
			{
				mp3indextbl[curindex]=temp;//��¼����
				curindex++;
			}	    
		} 
	}  
	f_closedir(&mp3dir);
   	curindex=playindex;											//��0��ʼ��ʾ
   	res=f_opendir(&mp3dir,(const TCHAR*)"0:/RECORDER"); 	//��Ŀ¼
	while(res==FR_OK)//�򿪳ɹ�
	{	
				if(!firsttime)
		{
			firsttime=1;
			draw_dir_record(540,curindex);//��ʾ����Ŀ¼��y,Ŀ¼����
		}
		if(lastupdate!=curindex)
		{
			draw_dir_record(540,curindex);//��ʾ����Ŀ¼��y,Ŀ¼����
		}
		lastupdate=curindex-1;	
		
		
		dir_sdi(&mp3dir,mp3indextbl[curindex]);			//�ı䵱ǰĿ¼����	   
        res=f_readdir(&mp3dir,&mp3fileinfo);       		//��ȡĿ¼�µ�һ���ļ�
        if(res!=FR_OK||mp3fileinfo.fname[0]==0)break;	//������/��ĩβ��,�˳�
     	fn=(u8*)(*mp3fileinfo.lfname?mp3fileinfo.lfname:mp3fileinfo.fname);		
	
		strcpy((char*)pname,"0:/RECORDER/");				//����·��(Ŀ¼)
		strcat((char*)pname,(const char*)fn);  			//���ļ������ں���
 		LCD_Fill(30,490+220,480,190+16,WHITE);				//���֮ǰ����ʾ
		Show_Str(30,490+220,480-30,16,fn,16,0);				//��ʾ�������� 
		//Show_Str(30,760,480,16,fn,16,0);
		
		key=mp3_play_song(pname); 				 		//�������MP3  
		
	
		mp3_index_show(curindex+1,totmp3num);
		if(key==2)		//��һ��
		{
			if(curindex)
			{
				curindex--;
		
			}
			else curindex=totmp3num-1;
 		}else if(key<=1)//��һ��
		{
			curindex++;		   	
			if(curindex>=totmp3num)curindex=0;//��ĩβ��ʱ��,�Զ���ͷ��ʼ
 		}else if(key == 3)
		{	
			
			//Show_Str(450-16*5-80-16*5,540+80,240,16,"������½�ͷ�񷵻���һ��/ֹͣ����",16,0);
			LCD_Fill(450-16*5-80-16*5,540+80,500,540+80+16,WHITE);				//���֮ǰ����ʾ
			LCD_Fill(30,400+220,250,800,WHITE);				//���֮ǰ����ʾ
			LCD_Fill(450-16*5-80-16*5,540+60,500,540+60+16,WHITE);				//���֮ǰ����ʾ
			
			LCD_Fill(450-16*5-16*5,540+60,500,540+60+16,WHITE);				//���֮ǰ����ʾ
			Show_Str(450-16*5-80-16*5,540+60,240,16,"���سɹ�",16,0);
			delay_ms(3000);
			LCD_Fill(450-16*5-80-16*5,540+60,500,540+60+16,WHITE);				//���֮ǰ����ʾ
			Show_Str(450-16*5-80-16*5,540+60,240,16,"��ѡ����",16,0);
		  goto endplay;
		}
		else
			 break;	//�����˴��� 	 
	}
endplay:	
	f_closedir(&mp3dir);
	myfree(SRAMIN,mp3fileinfo.lfname);	//�ͷ��ڴ�			    
	myfree(SRAMIN,pname);				//�ͷ��ڴ�			    
	myfree(SRAMIN,mp3indextbl);			//�ͷ��ڴ�	
	
}


void mp3playRECEIVE(u8 playindex)
{
	
	u8 firsttime=0;
	u8 lastupdate=0;
	
	u8 res;
 	DIR mp3dir;	 		//Ŀ¼
	FILINFO mp3fileinfo;//�ļ���Ϣ
	u8 *fn;   			//���ļ���
	u8 *pname;			//��·�����ļ���
	u16 totmp3num; 		//�����ļ�����
	u16 curindex;		//ͼƬ��ǰ����
	u8 key;				//��ֵ		  
 	u16 temp;
	u16 *mp3indextbl;	//���������� 
	//u8 looknumb=0;
	
 	while(f_opendir(&mp3dir,"0:/RECEIVE"))//�������ļ���
 	{	    
		Show_Str(30,190,240,16,"RECEIVE�ļ��д���!",16,0);
		delay_ms(200);				  
		LCD_Fill(30,190,240,206,WHITE);//�����ʾ	     
		delay_ms(200);				  
	} 									  
	totmp3num=mp3_get_tnum("0:/RECEIVE"); //�õ�����Ч�ļ���
  	while(totmp3num==NULL)//�����ļ�����Ϊ0		
 	{	    
		Show_Str(30,190+650,240,16,"û�н����ļ�!",16,0);
		delay_ms(200);				  
		LCD_Fill(30,190+650,240,146,WHITE);//�����ʾ	     
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
	VS_HD_Reset();
	VS_Soft_Reset();
	vsset.mvol=220;						//Ĭ����������Ϊ220.
	mp3_vol_show((vsset.mvol-100)/5);	//����������:100~250,��ʾ��ʱ��,���չ�ʽ(vol-100)/5,��ʾ,Ҳ����0~30   
	//��¼����
    res=f_opendir(&mp3dir,"0:/RECEIVE"); //��Ŀ¼
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
   	curindex=playindex;											//��0��ʼ��ʾ
   	res=f_opendir(&mp3dir,(const TCHAR*)"0:/RECEIVE"); 	//��Ŀ¼
	while(res==FR_OK)//�򿪳ɹ�
	{	
				if(!firsttime)
		{
			firsttime=1;
			draw_dir_record(540,curindex);//��ʾ����Ŀ¼��y,Ŀ¼����
		}
		if(lastupdate!=curindex)
		{
			draw_dir_record(540,curindex);//��ʾ����Ŀ¼��y,Ŀ¼����
		}
		lastupdate=curindex-1;	
		
		
		dir_sdi(&mp3dir,mp3indextbl[curindex]);			//�ı䵱ǰĿ¼����	   
        res=f_readdir(&mp3dir,&mp3fileinfo);       		//��ȡĿ¼�µ�һ���ļ�
        if(res!=FR_OK||mp3fileinfo.fname[0]==0)break;	//������/��ĩβ��,�˳�
     	fn=(u8*)(*mp3fileinfo.lfname?mp3fileinfo.lfname:mp3fileinfo.fname);		
	
		strcpy((char*)pname,"0:/RECEIVE/");				//����·��(Ŀ¼)
		strcat((char*)pname,(const char*)fn);  			//���ļ������ں���
 		LCD_Fill(30,490+220,480,190+16,WHITE);				//���֮ǰ����ʾ
		Show_Str(30,490+220,480-30,16,fn,16,0);				//��ʾ�������� 
		//Show_Str(30,760,480,16,fn,16,0);
		
		key=mp3_play_song(pname); 				 		//�������MP3  
		
	
		mp3_index_show(curindex+1,totmp3num);
		if(key==2)		//��һ��
		{
			if(curindex)
			{
				curindex--;
		
			}
			else curindex=totmp3num-1;
 		}else if(key<=1)//��һ��
		{
			curindex++;		   	
			if(curindex>=totmp3num)curindex=0;//��ĩβ��ʱ��,�Զ���ͷ��ʼ
 		}else if(key == 3)
		{	
			
			//Show_Str(450-16*5-80-16*5,540+80,240,16,"������½�ͷ�񷵻���һ��/ֹͣ����",16,0);
			LCD_Fill(450-16*5-80-16*5,540+80,500,540+80+16,WHITE);				//���֮ǰ����ʾ
			LCD_Fill(30,400+220,250,800,WHITE);				//���֮ǰ����ʾ
			LCD_Fill(450-16*5-80-16*5,540+60,500,540+60+16,WHITE);				//���֮ǰ����ʾ
			
			LCD_Fill(450-16*5-16*5,540+60,500,540+60+16,WHITE);				//���֮ǰ����ʾ
			Show_Str(450-16*5-80-16*5,540+60,240,16,"���سɹ�",16,0);
			delay_ms(3000);
			LCD_Fill(450-16*5-80-16*5,540+60,500,540+60+16,WHITE);				//���֮ǰ����ʾ
			Show_Str(450-16*5-80-16*5,540+60,240,16,"��ѡ����",16,0);
		  goto endplay;
		}
		else
			 break;	//�����˴��� 	 
	}
endplay:	
	f_closedir(&mp3dir);
	myfree(SRAMIN,mp3fileinfo.lfname);	//�ͷ��ڴ�			    
	myfree(SRAMIN,pname);				//�ͷ��ڴ�			    
	myfree(SRAMIN,mp3indextbl);			//�ͷ��ڴ�	
	
}
//����һ��ָ���ĸ���				     	   									    	 
//����ֵ:0,�����������
//		 1,��һ��
//       2,��һ��
//       0XFF,���ִ�����
u8 mp3_play_song(u8 *pname)
{	 
 	FIL* fmp3;
    u16 br;
	u8 res,rval;	  
	u8 *databuf;	   		   
	u16 i=0; 
	u8 key;  	
	
//	u8 looknumb=0;
//	u16 totmp3num; 		//�����ļ�����

			   
	rval=0;	    
	fmp3=(FIL*)mymalloc(SRAMIN,sizeof(FIL));//�����ڴ�
	databuf=(u8*)mymalloc(SRAMIN,4096);		//����4096�ֽڵ��ڴ�����
	if(databuf==NULL||fmp3==NULL)rval=0XFF ;//�ڴ�����ʧ��.
	if(rval==0)
	{	  
	  	VS_Restart_Play();  					//�������� 
		VS_Set_All();        					//������������Ϣ 			 
		VS_Reset_DecodeTime();					//��λ����ʱ�� 	  
		res=f_typetell(pname);	 	 			//�õ��ļ���׺	 			  	 						 
		if(res==0x4c)//�����flac,����patch
		{	
			VS_Load_Patch((u16*)vs1053b_patch,VS1053B_PATCHLEN);
		}  				 		   		 						  
		res=f_open(fmp3,(const TCHAR*)pname,FA_READ);//���ļ�	 
 		if(res==0)//�򿪳ɹ�.
		{ 
			VS_SPI_SpeedHigh();	//����						   
			while(rval==0)
			{
				res=f_read(fmp3,databuf,4096,(UINT*)&br);//����4096���ֽ�  
				i=0;
				do//������ѭ��
			    {  	
					if(VS_Send_MusicData(databuf+i)==0)//��VS10XX������Ƶ����
					{
						i+=32;
					}else   
					{
						key=KEY_Scan(0);
						if(TPAD_Scan(0))
						{
							 rval = 3;
							return rval;
							
						}
						
						
//						tp_dev.scan(0);
//						if((tp_dev.sta)&(1<<0)) //
//						{
//								//�鿴Ŀ¼��һ����Ϣ
//							//draw_button(430,16*5,"�鿴��һ��",16);
//							//LCD_DrawRectangle((450-16*5-100),(y-10),450-100+20,(y+size+10));
//							//LCD_DrawRectangle((450-16*5-100),(430-10),(450-100+20),(430+16+10));
//							if(tp_dev.x[0]>(450-16*5-100)&&tp_dev.y[0]>(430-10)&&tp_dev.x[0]<(450-100+20)&&tp_dev.y[0]<(430+16+10))
//							{	
//									
//								if(looknumb)
//								{
//									looknumb--;
//							
//								}
//								else 
//								{
//									totmp3num=mp3_get_tnum("0:/MUSIC"); //�õ�����Ч�ļ���
//									looknumb=totmp3num-1;
//								}

//							}
//								//draw_button(500,16*5,"�鿴��һ��",16);
//								//LCD_DrawRectangle((450-16*5-100),(500-10),(450-100+20),(500+16+10));
//								if(tp_dev.x[0]>(450-16*5-100)&&tp_dev.y[0]>(500-10)&&tp_dev.x[0]<(450-100+20)&&tp_dev.y[0]<(500+16+10))
//							{
//								looknumb++;
//							}
//						}
//						draw_dir(540,looknumb);//��ʾ����Ŀ¼��y,Ŀ¼����
//			
						
						
						switch(key)
						{
							case KEY0_PRES:
								rval=1;		//��һ��
							//return 0;
								break;
							case KEY2_PRES:
								rval=2;		//��һ��
								break;
							case WKUP_PRES:	//��������
								if(vsset.mvol<250)
								{
									vsset.mvol+=5;
						 			VS_Set_Vol(vsset.mvol);	
								}else vsset.mvol=250;
								mp3_vol_show((vsset.mvol-100)/5);	//����������:100~250,��ʾ��ʱ��,���չ�ʽ(vol-100)/5,��ʾ,Ҳ����0~30   
								break;
							case KEY1_PRES:	//������
								if(vsset.mvol>100)
								{
									vsset.mvol-=5;
						 			VS_Set_Vol(vsset.mvol);	
								}else vsset.mvol=100;
								mp3_vol_show((vsset.mvol-100)/5);	//����������:100~250,��ʾ��ʱ��,���չ�ʽ(vol-100)/5,��ʾ,Ҳ����0~30   
								break;
						}
						mp3_msg_show(fmp3->fsize);//��ʾ��Ϣ	    
					}	    	    
				}while(i<4096);//ѭ������4096���ֽ� 
				if(br!=4096||res!=0)
				{
					rval=0;
					break;//������.		  
				} 							 
			}
			f_close(fmp3);
		}else rval=0XFF;//���ִ���	   	  
	}						     	 
	myfree(SRAMIN,databuf);	  	 		  	    
	myfree(SRAMIN,fmp3);
	return rval;	  	 		  	    
}



























