#ifndef __MRRLCD_H
#define __MRRLCD_H
#include "sys.h"	


//���ݴ�����ר�в���
//��ˮƽ��
//x0,y0:����
//len:�߳���
//color:��ɫ
void gui_draw_hline(u16 x0,u16 y0,u16 len,u16 color);

//��ʵ��Բ
//x0,y0:����
//r:�뾶
//color:��ɫ
void gui_fill_circle(u16 x0,u16 y0,u16 r,u16 color);

//������֮��ľ���ֵ 
//x1,x2����ȡ��ֵ��������
//����ֵ��|x1-x2|
u16 my_abs(u16 x1,u16 x2);

//��һ������
//(x1,y1),(x2,y2):��������ʼ����
//size�������Ĵ�ϸ�̶�
//color����������ɫ
void lcd_draw_bline(u16 x1, u16 y1, u16 x2, u16 y2,u8 size,u16 color);

//���ݴ��������Ժ���
void ctp_test(void);

//�����Ļ�������Ͻ���ʾ"RST"
void Load_Drow_Dialog(void);


//��ʾ���������Ϣ
void mrrlcd_msg_show(void);

//����MEMS��������ϵͳ�������
void Recording_Dialog(void);

//ɨ�败�������ж�
void touch_swtich(void);

//���ƿ��ư�ť
void draw_button(u16 y,u16 width,u8*str,u8 size);

void draw_button_dir(u16 x,u16 y,u16 width,u8*str,u8 size);


//��ʾĿ¼
void draw_dir(u16 y,u8 checkkey);
void draw_dir_record(u16 y,u8 checkkey);
void draw_dir_txt(u16 y,u8 checkkey);
void draw_dir_receive(u16 y,u8 checkkey);
//ͳ��Ŀ¼�ı�����
u16 get_txtnum(u8 *path);

#endif
