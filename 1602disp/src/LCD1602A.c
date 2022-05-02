//-----------------------------------------------------------------------------
//程序描述：
//  这个程序可以在该液晶模块上显示字符:"  BT F020 V1.0  www.xhl.com.cn"
//  此程序用4位数据线来驱动液晶
//  液晶接线方法 :1脚接地;2脚接+5V;3脚通过1个10K电阻接+5v，通过1个430欧电阻接地；
//  15脚接地;16脚通过1个15R电阻接+5v。液晶别的引脚直接接相应的数据或控制信号线。
//作者:ZDP
//时间:2005-11-30
//版本:V1.0
//-----------------------------------------------------------------------------
// 头文件包含
//-----------------------------------------------------------------------------
#include <c8051f020.h>     



#define Crystal_Clock 22118400L


                                 
unsigned long x;                 //"www.xhl.com.cn"
unsigned char xdata Netdata[16]={0x77,0x77,0x77,0x2e,0x78,0x68,0x6c,0x2e,0x63,0x6f,
                                0x6d,0x2e,0x63,0x6e};                                
                               
                                 //"  BT F020 V1.0  "
unsigned char xdata NCDdata[17]={0x20,0x20,0x42,0x54,0x20,0x46,0x30,0x32,0x30,0x20,
                                 0x56,0x31,0x2e,0x30,0x20,0x20};
                                                              
unsigned char *lcdpoint;        //指向 lcddata数组的指针
unsigned char lcd_data_count;

void SYSCLK_Init (void);
void PORT_Init (void);
void LCD_Init(void);
void ExtCrystalOsc_Init (void);
void SYSCLK_Init (void);




void main (void)
 {
	static unsigned char data1;
    WDTCN = 0xde;
    WDTCN = 0xad;                       	//禁止看门狗定时器

    SYSCLK_Init ();                        	// 系统时钟初始化
                                          
    PORT_Init ();                          	// 交叉开关和通用I/O口初始化

 	while(1){
		LCD_Init();                         //LCD初始化
	    P2  = 0xA0;                         //准备送数据
	    for(x=0;x<5000;x++);
	    lcdpoint=&NCDdata;					//取地址
	    for(lcd_data_count=14;lcd_data_count>0;lcd_data_count--)//显示第一行字符
	    { 
		    data1=*lcdpoint;				//读出数据				
		    P3 = data1; 					//写数据到端口
			P2 = 0X20;
	        P2 = 0XA0;						//控制LCD	
	        lcdpoint++;
			for(x=0;x<5000;x++);
		}
		P2 = 0X80;
	   	P3 = 0xc0;
   		P2 = 0x00;		
   		P2 = 0x80; 		
   		for(x=0;x<1000;x++);
	    lcdpoint=&Netdata;
	    for(lcd_data_count=14;lcd_data_count>0;lcd_data_count--)//显示第一行字符
	    { 
		    data1=*lcdpoint;
		    P3 = data1; 
//-----------------------------------------------------
	//        P2 = 0x20;                   /*原来的LCD程序*/ 
	//		P2 = 0xA0;
//-----------------------------------------------------
		    P2 = 0xA0;                   //修改后的LCD程序
            P2 = 0x20;
//------------------------------------------------------/
	        lcdpoint++;
			for(x=0;x<5000;x++);
		}
	}
}




void PORT_Init (void)
{

   XBR0     = 0x00;                       // 没有选择数字外设
   XBR2     = 0x40;
   P2MDOUT  = 0xe0;                       // P2口设为推挽方式
}



void LCD_Init(void)
{
   P2 = 0X80;
   for(x=0;x<50000;x++);
   //P7 = 0x30;				/*一行显示*/
   P3 = 0x38;				/*两行显示*/
   P2 = 0X00;//0x08;
   P2 = 0X80;//0x09; 
   for(x=0;x<1000;x++);
   P3 = 0x0e;
   P2 = 0x00;
   P2 = 0x80;
   for(x=0;x<1000;x++);
   P3=  0x06;
   P2 = 0x00;
   P2 = 0x80;
   for(x=0;x<5000;x++);
   P3 = 0x01;
   P2 = 0x00;
   P2 = 0x80; 
   for(x=0;x<5000;x++);
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
