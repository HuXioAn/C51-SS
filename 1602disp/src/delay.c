
#include "delay.h"
#include "C8051F020.h"

sfr16 TMR3     = 0x94;

void delay_us(int16_t time_us){
	int16_t ticks_to_go;
	
	time_us=time_us>50?time_us-40:10;

    ticks_to_go =(int16_t) (time_us*TICK_PER_US);
    TMR3CN =0X00;//清空溢出、关闭计数、选择主时钟十二分频
    TMR3 = (-ticks_to_go);
    TMR3CN |= 0x04;//开始计数
    while(!(TMR3CN & 0x80));
    TMR3CN =0X00;
}

void delay_ms(int16_t time_ms){
	int i;
	for(i=0;i<time_ms;i++)delay_us(1000);
	
}