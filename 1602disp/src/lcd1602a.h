#ifndef _H_LCD1602A_
#define _H_LCD1602A_

#include "type_def.h"

#define CTR_PORT P6
#define RS_BIT 2
#define RW_BIT 1
#define EN_BIT 0

#define DATA_PORT P5

#define RS(x)                        \
    do                               \
    {                                \
        if (x)                       \
        {                            \
            P6 |= 0x01 << RS_BIT;    \
        }                            \
        else                         \
        {                            \
            P6 &= ~(0x01 << RS_BIT); \
        }                            \
    } while (0)

#define RW(x)                        \
    do                               \
    {                                \
        if (x)                       \
        {                            \
            P6 |= 0x01 << RW_BIT;    \
        }                            \
        else                         \
        {                            \
            P6 &= ~(0x01 << RW_BIT); \
        }                            \
    } while (0)

#define EN(x)                        \
    do                               \
    {                                \
        if (x)                       \
        {                            \
            P6 |= 0x01 << EN_BIT;    \
        }                            \
        else                         \
        {                            \
            P6 &= ~(0x01 << EN_BIT); \
        }                            \
    } while (0)
	
	
	
	void WriteInsWithoutFlagCheck(uint8_t ins);
	void lcd1602_WriteIns(uint8_t ins);
	uint8_t waitWhileBusy(void);
	void lcd1602_Init(void);
#endif