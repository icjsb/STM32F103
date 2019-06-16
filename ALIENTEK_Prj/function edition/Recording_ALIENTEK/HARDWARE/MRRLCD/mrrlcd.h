#ifndef __MRRLCD_H
#define __MRRLCD_H
#include "sys.h"	


//电容触摸屏专有部分
//画水平线
//x0,y0:坐标
//len:线长度
//color:颜色
void gui_draw_hline(u16 x0,u16 y0,u16 len,u16 color);

//画实心圆
//x0,y0:坐标
//r:半径
//color:颜色
void gui_fill_circle(u16 x0,u16 y0,u16 r,u16 color);

//两个数之差的绝对值 
//x1,x2：需取差值的两个数
//返回值：|x1-x2|
u16 my_abs(u16 x1,u16 x2);

//画一条粗线
//(x1,y1),(x2,y2):线条的起始坐标
//size：线条的粗细程度
//color：线条的颜色
void lcd_draw_bline(u16 x1, u16 y1, u16 x2, u16 y2,u8 size,u16 color);

//电容触摸屏测试函数
void ctp_test(void);

//清空屏幕并在右上角显示"RST"
void Load_Drow_Dialog(void);


//显示毕设基本信息
void mrrlcd_msg_show(void);

//基于MEMS逆向光调制系统界面设计
void Recording_Dialog(void);

//扫描触摸屏，判断
void touch_swtich(void);

//绘制控制按钮
void draw_button(u16 y,u16 width,u8*str,u8 size);

void draw_button_dir(u16 x,u16 y,u16 width,u8*str,u8 size);


//显示目录
void draw_dir(u16 y,u8 checkkey);
void draw_dir_record(u16 y,u8 checkkey);
void draw_dir_txt(u16 y,u8 checkkey);
void draw_dir_receive(u16 y,u8 checkkey);
//统计目录文本数量
u16 get_txtnum(u8 *path);

#endif
