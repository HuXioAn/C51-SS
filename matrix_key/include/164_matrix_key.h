#ifndef _H_164_MATRIX_KEY_
#define _H_164_MATRIX_KEY_

#include "type_def.h"  

#define HC164_B (P3^6)
#define HC164_CLK (P3^7)


#define MATRIX_INPUT_PORT P4

#define TIME_OF_LONGPRESS 40


void hc164_write(uint8_t value);
int8_t matrix_key_get(void);
char matrix_key_wait(void);







#endif