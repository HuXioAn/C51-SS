#include <c8051f020.h>     
#include "delay.h"
#include "lcd1602a.h"
#include "164_matrix_key.h"


#define Crystal_Clock 22118400L



void ExtCrystalOsc_Init (void);
void SYSCLK_Init (void);

char buf[DISPLAY_BUFFER_LEN];


void main(void)
{	
	int a=0;
   char ch;
	char *not="0.33+2*(2/5)";
	display_t disp;
	char *notation = not;
	
	WDTCN = 0xde;
   WDTCN = 0xad; // Disable watchdog
	SYSCLK_Init();
	lcd1602_Init();
	lcd1602_dispStructInit(&disp,buf,DISPLAY_BUFFER_LEN);

	
	/*
	while(1){
	ch=matrix_key_get();
	while( ch == -1)ch=matrix_key_get();

   lcd1602_WriteData(ch);
	
	while( ch != -1)ch=matrix_key_get();
	
	}
	
	*/
	
	
	
	
	while(1)
	{
		ch=matrix_key_wait();
		if(1==lcd1602_dispKeyValue(ch,&disp))break;
		
		
	}
	
	
	
	
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
