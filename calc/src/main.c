

#include "stdio.h"
#include "C8051F020.h"
#include "stack.h"
#include "re_polish.h"
#include "lcd1602a.h"
#include "delay.h"
#include "164_matrix_key.h"
#include <math.h>
#include <stdlib.h>

//#include "si_toolchain.h"
//#include "C8051F020_def"


#define MAX_DECIMAL_PLACES 6
#define CHAR_FOR_SROOT '#'
#define CHAR_FOR_SQUARE '^'

#define Crystal_Clock 22118400L



void ExtCrystalOsc_Init (void);
void SYSCLK_Init (void);

unsigned char xdata malloc_mempool [0x1000];


extern struct StackNode *postFix[STACK_SIZE_MAX];





struct StackNode *strToOprandNode(char *head, char *tail)
{	uint8_t i;
    char *ptr = head;
    char *decimal = NULL;
    float result = 0;
    //小数点个数检查
    for (; ptr <= tail; ptr++)
    {
        if (*ptr < '0' || *ptr > '9')
        {
            if (decimal)
                return NULL;
            else
                decimal = ptr;
        }
    }
    if (!decimal)
    { //如果没有小数点则为整数
        decimal = tail + 1;
    }
    else
    {
        //如果有小数点,约束小数位数
        tail = (tail - decimal) > MAX_DECIMAL_PLACES ? decimal + 6 : tail;
				
        for (i = 0; i < tail - decimal; i++)
        {
            result += (*(decimal + i + 1) - '0') * pow(0.1, (i + 1));
            
        }
    }
    for (i = 0; i < decimal - head ; i++)
    {
        //计算整数部分
        result += (*(head + i) - '0') * pow(10, decimal - head - 1 - i);
    }

    return newNode(DATA_TYPE_FLOAT, &result);
}



void strToList(struct StackNode **root, char *notation)
{
    char *ptr = notation;
    char *head_figure = NULL;
    char *tail_figure = NULL;
    while (*ptr)
    {
		enum OPERATOR_TYPE operator_type;
        if (*ptr >= '0' && *ptr <= '9')
        {
            //是数字,生成运算数节点
            head_figure = ptr;
            while ((*ptr >= '0' && *ptr <= '9') || *ptr == '.')
            {
                ptr++; //找到下一个运算符
                if (!*ptr)
                    break;
            }
            tail_figure = ptr - 1;
            push(root, strToOprandNode(head_figure, tail_figure));
            continue;
        }
        
        switch (*ptr)
        {
        case '+':
            operator_type = OPERATOR_PLUS;
            break;
        case '-':
            operator_type = OPERATOR_MINUS;
            break;
        case '*':
            operator_type = OPERATOR_MULTI;
            break;
        case '/':
            operator_type = OPERATOR_DIVDE;
            break;
        case '(':
            operator_type = OPERATOR_LEFT_PAREN;
            break;
        case ')':
            operator_type = OPERATOR_RIGHT_PAREN;
            break;
        case CHAR_FOR_SQUARE:
            operator_type = OPERATOR_SQUARE;
            break;
        case CHAR_FOR_SROOT:
            operator_type = OPERATOR_SQUARE_ROOT;
            break;
        default:
            //表达式有误

            break;
        }
        //生成操作符节点并挂载
        push(root, newNode(DATA_TYPE_OPERATOR, &operator_type));
        ptr++;
    }
}



int main(void){
	
	int l,i;
	float ans=0;
	char answer[10],ch;

    struct StackNode *root = NULL;
    char notation[40] ;//= "23.4+20*15";
	init_mempool (&malloc_mempool, sizeof(malloc_mempool));

    
    WDTCN = 0xde;
    WDTCN = 0xad; // Disable watchdog
	SYSCLK_Init();

    lcd1602_Init();

    lcd1602_WriteIns(0x80);
    delay_us(40);

    i=0;

    while(1){
        ch=matrix_key_get();
        while( ch == -1)ch=matrix_key_get();

        if('=' == ch){*(notation+i)='\0';break;}
        lcd1602_WriteData(ch);
        *(notation+i)=ch;
        i++;

        while( ch != -1)ch=matrix_key_get();
        
        }









    //lcd1602_printChar(notation,1,0);

    strToList(&root, notation);

    l=infixToPostfix(root->previous);

    for(i=0;i<l;i++){
        if(postFix[i]->data_type == DATA_TYPE_FLOAT){
            //printf("%f",*(float*)postFix[i]->value_p);
        }else{
            enum OPERATOR_TYPE operator_type=*(enum OPERATOR_TYPE*)postFix[i]->value_p;
            char operator=0;
            switch (operator_type)
            {
            case OPERATOR_PLUS:
                operator = '+';
                break;
            case OPERATOR_MINUS:
                operator = '-';
                break;
            case OPERATOR_MULTI:
                operator = '*';
                break;
            case OPERATOR_DIVDE:
                operator = '/';
                break;
            case OPERATOR_LEFT_PAREN:
                operator = '(';
                break;
            case OPERATOR_RIGHT_PAREN:
                operator = ')';
                break;
            case OPERATOR_SQUARE:
                operator = CHAR_FOR_SQUARE;
                break;
            case OPERATOR_SQUARE_ROOT:
                operator = CHAR_FOR_SROOT;
                break;
            default:
                //表达式有误

                break;
            }
            //printf("%c",operator);
        }
    }
    //printf("\n");

    ans = calcPostfix(l);

    //printf("ans is %f \n",ans);
    sprintf(answer,"%.4f",ans);

    lcd1602_printChar(answer,2,0);

    while(1);
	
	return 0;
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
