#ifndef _H_LCD1602A_
#define _H_LCD1602A_

#include "type_def.h"

#define DISPLAY_BUFFER_LEN 64
#define LCD1602_WIDTH 16

#define CTR_PORT P3
#define RS_BIT 0
#define RW_BIT 1
#define EN_BIT 2

#define DATA_PORT P2

#define RS(x)                        \
    do                               \
    {   int a=0;                     \
        if (x)                       \
        {                            \
            CTR_PORT |= 0x01 << RS_BIT;    \
        }                            \
        else                         \
        {                            \
            CTR_PORT &= ~(0x01 << RS_BIT); \
        } \
        while(a<10)a++;              \
    } while (0)

#define RW(x)                        \
    do                               \
    {   int a=0;                             \
        if (x)                       \
        {                            \
            CTR_PORT |= 0x01 << RW_BIT;    \
        }                            \
        else                         \
        {                            \
            CTR_PORT &= ~(0x01 << RW_BIT); \
        }\
        while(a<10)a++;                \
    } while (0)

#define EN(x)                        \
    do                               \
    {   int a=0;                             \
        if (x)                       \
        {                            \
            CTR_PORT |= 0x01 << EN_BIT;    \
            while(a<10)a++;             \
        }                            \
        else                         \
        {   while(a<10)a++;                         \
            CTR_PORT &= ~(0x01 << EN_BIT); \
        }                            \
    } while (0)
	
	
	
	void WriteInsWithoutFlagCheck(uint8_t ins);
	void lcd1602_WriteIns(uint8_t ins);
	uint8_t waitWhileBusy(void);
	void lcd1602_Init(void);
    void lcd1602_WriteData(uint8_t data_value);
	uint8_t lcd1602_printChar(char * str,uint8_t line,int8_t pos);
    void lcd1602_dispStructInit(display_t * disp,char * buffer,uint8_t buffer_len);
    uint8_t lcd1602_dispKeyValue(char keyvalue, display_t * disp );
    void lcd1602_displayFromStruct(display_t * disp);




#endif