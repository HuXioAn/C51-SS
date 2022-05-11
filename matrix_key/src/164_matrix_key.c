

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
    //向行写低
}
