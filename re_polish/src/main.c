

#include "stdio.h"
#include "C8051F020.h"
#include "stack.h"
#include "re_polish.h"
#include <math.h>
#include <stdlib.h>

//#include "si_toolchain.h"
//#include "C8051F020_def"


#define MAX_DECIMAL_PLACES 6
#define CHAR_FOR_SROOT '#'
#define CHAR_FOR_SQUARE '^'



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
	
    struct StackNode *root = NULL;
    char notation[] = "23.4+20*15";
	init_mempool (&malloc_mempool, sizeof(malloc_mempool));
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

    
	
	return 0;
}