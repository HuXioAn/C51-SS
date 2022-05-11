#ifndef _H_DELAY_
#define _H_DELAY_


#define Crystal_Clock 22118400L

#define US_PER_TICK (12000000L/Crystal_Clock)

#define TICK_PER_US (Crystal_Clock/12000000.0)

#include "type_def.h"

//使用tim3，16bit，在22mhz左右下十二分频最多能计时35ms

void delay_us(int16_t time_us);

void delay_ms(int16_t time_ms);



#endif