

#include "C8051F020.h"
#include "lcd1602a.h"
#include "type_def.h"
#include "delay.h"
#include <string.h>



void WriteInsWithoutFlagCheck(uint8_t ins)
{
    RS(0);
    RW(0);
    EN(1);
    DATA_PORT = ins;
    EN(0);
}

void lcd1602_WriteIns(uint8_t ins)
{
    waitWhileBusy();
    WriteInsWithoutFlagCheck(ins);
}

uint8_t waitWhileBusy(void)
{
    //检查数据口最高位
    DATA_PORT = 0xff;
    RS(0);
    RW(1);
    EN(1);
    while (DATA_PORT & 0x80);
    EN(0);
}



void lcd1602_Init(void){
	XBR2  |= 0x40;//P0123不开只能做输入
    //延时40ms
    delay_ms(50);
    WriteInsWithoutFlagCheck(0x38);//功能设定指令，8bit总线、两行显示、字体5*8
    //延时37us
    delay_us(40);
    WriteInsWithoutFlagCheck(0x38);
    //延时37us
    delay_us(40);
    lcd1602_WriteIns(0x0f);//显示开关，显示开启、光标开启、光标闪烁开启
    //延时37us
    delay_us(40);
    lcd1602_WriteIns(0x01);//清屏，全显示空格
    //延时1.52ms
    delay_us(1700);
    lcd1602_WriteIns(0x06);//Entry mode,光标向右自增，不开启整个屏幕的位移
    //延时37us
    delay_us(40);
    lcd1602_WriteIns(0x80);//AC内容设定，清零AC
    //延时37us
    delay_us(40);
    lcd1602_WriteIns(0x02);//回家？？这个得研究和上一个的区别，貌似是清零AC回正光标
    //延时1.52ms
    delay_us(1700);
}

void lcd1602_WriteData(uint8_t data_value){
    waitWhileBusy();

    RS(1);
    RW(0);
    EN(1);
    DATA_PORT=data_value;
    EN(0);

}


uint8_t lcd1602_printChar(char * str,uint8_t line,int8_t pos){
    if(pos>=0 && pos<16){//设定位置
        uint8_t ins = line == 1?0x80|pos:0xc0|pos;
        lcd1602_WriteIns(ins);
	    delay_us(40); 
    }
    while(*str){
        lcd1602_WriteData(*str);
		delay_us(40);
        str++;
    }
}

void lcd1602_dispStructInit(display_t * disp,char * buffer,uint8_t buffer_len){
    //初始化显示结构体
    disp->buffer=buffer;
    disp->buffer_len=buffer_len;
    disp->lcd_head=buffer;
    disp->cursor=buffer;
    disp->lcd_tail=buffer+LCD1602_WIDTH;
    disp->str_end=buffer;
    //*(buffer+buffer_len-1)=0;
}


uint8_t lcd1602_dispKeyValue(char keyvalue, display_t * disp ){
    //根据键值命令显示
    switch (keyvalue)
    {//先判断控制字符，显示字符统一处理
    case 'l':
        if(disp->cursor == disp->buffer){//在算式最左边，跳转最右边
            disp->cursor = disp->str_end;
            if(disp->cursor >= disp->lcd_head && disp->cursor < disp->lcd_tail){
                break;
            }
            disp->lcd_tail= disp->cursor+1;
            disp->lcd_head=disp->lcd_tail-LCD1602_WIDTH;
            break;
        }
        disp->cursor--;
        if(disp->cursor == (disp->lcd_head-1)){//如果到最左了，显示区域位移
            disp->lcd_head--;
            disp->lcd_tail--;
        }

        break;
    case 'r':
        if(disp->cursor == disp->str_end){//在算式最右边，跳转最左边
            disp->cursor = disp->buffer;
            if(disp->cursor >= disp->lcd_head && disp->cursor < disp->lcd_tail){
                break;
            }
            disp->lcd_head=disp->buffer;
            disp->lcd_tail=disp->buffer+LCD1602_WIDTH;
            break;
        }
        disp->cursor++;
        if(disp->cursor == (disp->lcd_tail)){//如果到最左了，显示区域位移
            disp->lcd_head++;
            disp->lcd_tail++;
        }
        break;
    case '=':
        return 1;//返回通知主程序开始计算
        break;
    case '\e'://清零
        lcd1602_dispStructInit(disp,disp->buffer,disp->buffer_len);
        lcd1602_WriteIns(0x01);
        delay_us(1700);
        break;
    case '\b':
        if(disp->cursor == disp->buffer)break;//如果已空，就无任何反应
        if(disp->cursor == disp->str_end){//在最后面删除，没有影响
            disp->cursor--;
            disp->str_end--;
        }else{//在内容中删除
            strncpy(disp->cursor-1,disp->cursor,disp->str_end-disp->cursor);
            disp->cursor--;
            disp->str_end--;
        }
        if(disp->cursor == (disp->lcd_head-1)){//如果到最左了，显示区域位移
            disp->lcd_head--;
            disp->lcd_tail--;
        }

        break;
    default:
        //要显示的字符
        if(disp->cursor == (disp->buffer+disp->buffer_len-2))break;//如果已满，就无任何反应
        *(disp->cursor++)=keyvalue;
        disp->str_end++;
        if(disp->cursor == disp->lcd_tail){//如果到右边了，显示区域位移
            disp->lcd_head++;
            disp->lcd_tail++;
        }
        break;
    }

    //更新显示
    lcd1602_displayFromStruct(disp);

    return 0;
}


void lcd1602_displayFromStruct(display_t * disp){
    char content[LCD1602_WIDTH+1];
    
    *(disp->str_end)='\0';
    strncpy(content,disp->lcd_head,LCD1602_WIDTH);
    content[LCD1602_WIDTH]='\0';

    lcd1602_printChar(content,1,0);//发送内容

    lcd1602_WriteIns(0x80|(disp->cursor-disp->lcd_head));//设定光标
	delay_us(40); 



}







