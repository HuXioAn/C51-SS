

#include <c8051f020.h>     
#include "delay.h"
#include "164_matrix_key.h"


#define CHAR_FOR_SROOT '#'
#define CHAR_FOR_SQUARE '^'


sbit hc164_b = HC164_B;
sbit hc164_clk = HC164_CLK;

char key_value_short[]={'0','1','2','3','4','5','6','7','8','9','.',
                '=','+','-','*','/','(',')',
                '\b','\e'};

char key_value_long[]={'0','1','2','3','4','5','6','7','8','9','.',
                '=','+','-',CHAR_FOR_SQUARE,CHAR_FOR_SROOT,'l','r',
                '\b','\e'};


void hc164_write(uint8_t value){
    uint8_t i=0;
    uint8_t a=0;
    for(;i<8;i++){
        hc164_clk = 0;
        while(a<10)a++;
        hc164_b = 0x01 & (value>>(7-i));
        while(a<10)a++;
        hc164_clk = 1;
        while(a<10)a++;
    }
}


int8_t matrix_key_get(void){//当前按下的按键，检测到第一个即返回
    //向行全写低
    hc164_write(0x00);
    //查询列有无低电平
    if((MATRIX_INPUT_PORT&0x0f) != 0x0f){
        //有则记录列，再轮流拉高行确定行
        uint8_t col,row;
        uint8_t colume;
        MATRIX_INPUT_PORT =0xff;
        colume = MATRIX_INPUT_PORT&0x0f;
        delay_ms(20);
        MATRIX_INPUT_PORT =0xff;
        if((MATRIX_INPUT_PORT&0x0f)!=colume)return -1;
        //通过去抖
        for(col=0;col<4;col++){
            if((colume>>col)&0x01)continue;
            for(row=0;row<5;row++){
                hc164_write((0xff)^(0x01<<row));
                MATRIX_INPUT_PORT =0xff;
                if((MATRIX_INPUT_PORT&0x0f)==colume)return((row<<2)+col);

            }
        }

    }else{
        return -1;
    }
    return -1;
    
}

char matrix_key_value(int8_t key,char* value_table){
    return value_table[key];
}


char matrix_key_wait(void){//阻塞查询按键，应区分长按短按
    char ch,key;
    uint16_t time_count=0;
    static char continue_press=-1;
    
    while(1){

	ch=matrix_key_get();
    
	while( ch == -1 ){
        ch=matrix_key_get();//有无按键按下检测
        continue_press=-1;
    }
    key=ch;
	while( ch != -1){
        ch=matrix_key_get();//按键抬起检测
        time_count++;
        if(ch == continue_press && time_count == TIME_OF_CONTINUEPRESS){
            return matrix_key_value(key,key_value_long);
        }
        if(time_count>TIME_OF_LONGPRESS && continue_press == -1 && (key == 17 || key ==16)){
            //初次连按设定连按变量
            continue_press=key;
            return matrix_key_value(key,key_value_long);
        }
        
    }
    
    if(time_count<TIME_OF_LONGPRESS && continue_press==-1)return matrix_key_value(key,key_value_short);
    if(continue_press==-1)return matrix_key_value(key,key_value_long);
    
    continue_press=-1;

    }

	
	
}


