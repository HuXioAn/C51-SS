

#include "C8051F020.h"
#include "lcd1602a.h"
#include "type_def.h"
#include "delay.h"

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


