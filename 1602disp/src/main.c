#include <c8051f020.h>     
#include "delay.h"
#include "lcd1602a.h"


#define Crystal_Clock 22118400L

#define uchar unsigned char
#define DELAY_LCD 100



void ExtCrystalOsc_Init (void);
void SYSCLK_Init (void);

//void Lcd1602_init(void);
char isLcdBusy(void);
void Lcd1602_Write_Command(uchar Command_Code);
void Lcd1602_Write_Data(uchar Data);
void Lcd1602_Write_Char(uchar row, uchar column, uchar Data);
void Lcd1602_Write_String(uchar row, uchar column, uchar *dis_buffer);

uchar   table1[]={0x10,0x06,0x09,0x08,0x08,0x09,0x06,0x00};//字符℃
uchar   table2[]={0x04,0x1F,0x00,0x0A,0x0A,0x0A,0x1B,0x00};//字符六

void Delay(int time){
	int i;
	for (i = time; i > 0; i--); 
	
}
/*
void Lcd1602_init(void)
{
    Lcd1602_Write_Command(0x38);  // 8bit data, two lines, 5x7 font
    Lcd1602_Write_Command(0x08);  // display off
    Lcd1602_Write_Command(0x01);  // clear screen
    Lcd1602_Write_Command(0x06);  // cursor shift left
    Lcd1602_Write_Command(0x0c);  // display on
    Lcd1602_Write_Command(0x80);  // set data address
    Lcd1602_Write_Command(0x02);  // clear data pointer
}

*/

/* Check where LCD is busy */
char isLcdBusy(void)
{
    P5 = 0xFF;
    P6 = 0x02; // RS=0, RW=1, EN=0
    Delay(DELAY_LCD);
    P6 = 0x03; // RS=0, RW=1, EN=1
    Delay(DELAY_LCD);
    return (P5 & 0x80);
}

void Lcd1602_Write_Command(uchar Command_Code)
{
    while(isLcdBusy());
    
    P6 = 0x00;         // RS=0,RW=0
	Delay(DELAY_LCD);
	P6 = 0x01;         // EN=1
    Delay(DELAY_LCD);
    P5 = Command_Code;
    Delay(DELAY_LCD);
    P6 = 0x00;
}

void Lcd1602_Write_Data(uchar Data)
{
    while(isLcdBusy());
    P6 = 0x04;                  // RS=1,RW=0
    Delay(DELAY_LCD);
    P6 = 0x05;                  // EN=1
    Delay(DELAY_LCD);
	P5 = Data;
	Delay(DELAY_LCD);
    P6 = 0x04;
}

void Lcd1602_Write_Char(uchar row, uchar column, uchar Data)
{
    while(isLcdBusy());

    if (row == 1)
        column |= 0xC0;
    else
        column |= 0x80;
    Lcd1602_Write_Command(column);
    Lcd1602_Write_Data(Data);
}

void Lcd1602_Write_String(uchar row, uchar column, uchar *dis_buffer)
{
    uchar i = 0;
    if (row >= 2) return; // is a 1602
    while (dis_buffer[i]!='\0') {
        Lcd1602_Write_Char(row, column + i, dis_buffer[i]);
        i++;
    }
}

void main(void)
{	
	int a=0;
	
	WDTCN = 0xde;
    WDTCN = 0xad; // Disable watchdog
	SYSCLK_Init();
	
	//while(a<10)a++;
	
	
	lcd1602_Init();
    lcd1602_WriteIns(0x81);
    lcd1602_WriteData(0x30);
	
	
	
	
	
	/*
    uchar i, j;
    WDTCN = 0xde;
    WDTCN = 0xad; // Disable watchdog
    SYSCLK_Init();
    P74OUT = 0x30;  // Port6 is push-pull
    Lcd1602_init();
 //   Lcd1602_Write_String(1, 8, "567");
    Lcd1602_Write_String(1, 1, "123");
    for (j = 0; j < 255; j++)
        Delay(10000);
    Lcd1602_Write_Command(0x40);
    for (i = 0; i < 8; i++) Lcd1602_Write_Data(table1[i]);
    for (i = 0; i < 8; i++) Lcd1602_Write_Data(table2[i]);
    for (i = 0; i < 8; i++) {
        Lcd1602_Write_Command(0x80);
        for (j = 0; j < 16; j++)
            Lcd1602_Write_Data(i * 32 + j);
        Lcd1602_Write_Command(0xC0);
        for (j = 0; j < 16; j++)
            Lcd1602_Write_Data(i * 32 + j + 16);
        for (j = 0; j < 255; j++)
            Delay(10000);
    }
	
	
	*/
    while(1);
}



void SYSCLK_Init (void)
{
   OSCICN |= 0x03;                     // Configure internal oscillator for
                                       // its highest frequency (16 MHz)

   OSCICN |= 0x80;                     // Enable missing clock detector

   ExtCrystalOsc_Init ();
}


void ExtCrystalOsc_Init (void)
{

   // Set the appropriate XFCN bits for the crystal frequency
   //
   //   XFCN     Crystal (XOSCMD = 11x)
   //   000      f <= 32 kHz
   //   001      32 kHz < f <= 84 kHz
   //   010      84 kHz < f <= 225 kHz
   //   011      225 kHz < f <= 590 kHz
   //   100      590 kHz < f <= 1.5 MHz
   //   101      1.5 MHz < f <= 4 MHz
   //   110      4 MHz < f <= 10 MHz
   //   111      10 MHz < f <= 30 MHz
   #if (Crystal_Clock <= 32000)
      #define XFCN 0
   #elif (Crystal_Clock <= 84000L)
      #define XFCN 1
   #elif (Crystal_Clock <= 225000L)
      #define XFCN 2
   #elif (Crystal_Clock <= 590000L)
      #define XFCN 3
   #elif (Crystal_Clock <= 1500000L)
      #define XFCN 4
   #elif (Crystal_Clock <= 4000000L)
      #define XFCN 5
   #elif (Crystal_Clock <= 10000000L)
      #define XFCN 6
   #elif (Crystal_Clock <= 30000000L)
      #define XFCN 7
   #else
      #error "Defined Crystal Frequency outside allowable range!"
      #define XFCN 0
   #endif

   unsigned int i;

   // Step 1. Enable the external oscillator.
   OSCXCN = 0x60;                      // External Oscillator is an external
                                       // crystal (no divide by 2 stage)

   OSCXCN |= XFCN;


   // Step 2. Wait at least 1 ms.
   for (i = 9000; i > 0; i--);         // at 16 MHz, 1 ms = 16000 SYSCLKs
                                       // DJNZ = 2 SYSCLKs


   // Step 3. Poll for XTLVLD => �1�.
   while ((OSCXCN & 0x80) != 0x80);


   // Step 4. Switch the system clock to the external oscillator.
   OSCICN |= 0x08;
}
