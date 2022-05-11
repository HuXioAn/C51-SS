

#include <c8051f020.h>     
#include "delay.h"
#include "164_matrix_key.h"

sbit hc164_b = HC164_B;
sbit hc164_clk = HC164_CLK;


void hc164_write(uint8_t data){
    uint8_t i=0;
    uint8_t a=0;
    for(;i<8;i++){
        hc164_clk = 0;
        while(a<10)a++;
        hc164_b = 0x01 & (data>>i);
        while(a<10)a++;
        hc164_clk = 1;
        while(a<10)a++;
    }
}


int8_t matrix_key_get(void){
    //向行全写低
    hc164_write(0x00);
    //查询列有无低电平
    if((MATRIX_INPUT_PORT&0x0f) != 0x0f){
        //有则记录列，再轮流拉高行确定行
        uint8_t col,row;
        uint8_t colume = MATRIX_INPUT_PORT&0x0f;
        delay_ms(20);
        if((MATRIX_INPUT_PORT&0x0f)!=colume)return -1;
        //通过去抖
        for(col=0;col<4;col++){
            if((colume>>col)&0x01)continue;
            for(row=0;row<5;row++){
                hc164_write((0xff)^(0x01<<row));
                if((MATRIX_INPUT_PORT&0x0f)=colume)return((row<<2)+col);

            }
        }

    }else{
        return -1;
    }
    return -1;
    
}
