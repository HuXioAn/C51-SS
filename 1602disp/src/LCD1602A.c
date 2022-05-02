

#include "C8051F020.h"
#include "lcd1602a.h"
#include "type_def.h"

void WriteInsWithoutFlagCheck(uin8_t ins)
{
    RS(0);
    RW(0);
    EN(1);
    DATA_PORT = ins;
    EN(0);
}

void WriteIns(uint8_t ins)
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
