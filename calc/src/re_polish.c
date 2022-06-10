
#include <stdio.h>
#include <stdlib.h>
#include "re_polish.h"
#include "type_def.h"
#include <math.h>

struct StackNode *postFix[STACK_SIZE_MAX];
struct StackNode *operatorStack[STACK_SIZE_MAX];

static uint8_t operator_pri(void *value_p);
static float operation_plus(float addend1, float addend2);
static float operation_minus(float minuend, float subtrahend);
static float operation_multiple(float multiplier1, float multiplier2);
static float operation_divide(float dividend, float divisor);
static float operation_squareroot(float oprand);

static uint8_t err_exist=0;


char* err_code[]={
    "/ by 0",
    "Missing ()",
    "Syntax err",
    "root of neg"
};

char * error_Check(void){
    if(err_exist == 0)return NULL;
    else{
        uint8_t num = err_exist;
        err_exist=0;
        return err_code[num-1];
    }
    return NULL;//无错误
}





uint8_t infixToPostfix(struct StackNode *head)
{
    struct StackNode *node_ptr = head;
    uint8_t postFix_stack_offset = 0;
    uint8_t opeartor_stack_offset = 0;
    do
    {
        if (node_ptr->data_type == DATA_TYPE_OPERATOR)
        {
            //操作符
            switch (*(enum OPERATOR_TYPE *)(node_ptr->value_p))
            {
            case OPERATOR_LEFT_PAREN:
                //左括号
                operatorStack[opeartor_stack_offset] = node_ptr;
                opeartor_stack_offset++;
                break;
            case OPERATOR_RIGHT_PAREN:
                while (opeartor_stack_offset != 0 &&
                       *(enum OPERATOR_TYPE *)operatorStack[opeartor_stack_offset - 1]->value_p !=
                           OPERATOR_LEFT_PAREN) //括号之间不断出符号栈
                {
                    postFix[postFix_stack_offset] = operatorStack[opeartor_stack_offset - 1];
                    opeartor_stack_offset--;
                    postFix_stack_offset++;
                }
                if (opeartor_stack_offset == 0 ||
                    *(enum OPERATOR_TYPE *)operatorStack[opeartor_stack_offset - 1]->value_p !=
                        OPERATOR_LEFT_PAREN)
                {
                    //表达式有误
                    err_exist=2;
                    return 0;
                }
                else
                {
                    opeartor_stack_offset--; //弹出左括号
                }

                break;
            case OPERATOR_MINUS:
                if((node_ptr == head) && (node_ptr->previous->data_type == DATA_TYPE_FLOAT)){
                    //如果是开头的负号，后面跟着数
                    //跳过负号节点、操作数节点取负
                    float * previous_oprand_p = (float *)node_ptr->previous->value_p;
                    node_ptr=node_ptr->previous;
                    *previous_oprand_p = 0-(*previous_oprand_p);
                    break;
                }else if((node_ptr != head) && (node_ptr->previous->data_type == DATA_TYPE_FLOAT) && (node_ptr->next->data_type == DATA_TYPE_OPERATOR)){
                    enum OPERATOR_TYPE previous_opr = *(enum OPERATOR_TYPE *)(node_ptr->next->value_p);
                    if((previous_opr != CHAR_FOR_SROOT) && (previous_opr != CHAR_FOR_SQUARE)){
                        //在中间做负号
                        //删除负号节点、操作数节点取负
                        float * previous_oprand_p = (float *)node_ptr->previous->value_p;
                        node_ptr=node_ptr->previous;
                        *previous_oprand_p = 0-(*previous_oprand_p);
                        break;
                    }
                }
                
            default:
                //其他运算符,比较与符号栈中的优先级
                while (opeartor_stack_offset != 0 &&
                       operator_pri(operatorStack[opeartor_stack_offset - 1]->value_p) >=
                           operator_pri(node_ptr->value_p))
                { //如果优先级低于已有，则不断出栈,括号优先级最低，不会影响。
                    postFix[postFix_stack_offset] = operatorStack[opeartor_stack_offset - 1];
                    opeartor_stack_offset--;
                    postFix_stack_offset++;
                }
                operatorStack[opeartor_stack_offset] = node_ptr; //放入
                opeartor_stack_offset++;
                break;
            }
        }
        if(node_ptr->data_type == DATA_TYPE_FLOAT)
        {
            //操作数
            postFix[postFix_stack_offset] = node_ptr;
            postFix_stack_offset++;
        }

        node_ptr = node_ptr->previous;
    } while (node_ptr != head);

    while (opeartor_stack_offset != 0)
    { //剩余的操作符出栈进入后缀栈
        if(*(enum OPERATOR_TYPE *)operatorStack[opeartor_stack_offset - 1]->value_p ==
                        OPERATOR_LEFT_PAREN ){
                            err_exist=2;//缺少右括号
                            return 0;
                        }
        postFix[postFix_stack_offset] = operatorStack[opeartor_stack_offset - 1];
        opeartor_stack_offset--;
        postFix_stack_offset++;
    }
    return postFix_stack_offset;
}



float calcPostfix(uint16_t notation_length)
{
    float calc_stack[STACK_SIZE_MAX / 4] = {0};
    uint8_t postFix_stack_offset = 0;
    uint8_t calc_stack_offset = 0;
	uint16_t i;
    for (i = 0; i < notation_length; i++)
    {
        enum DATA_TYPE_E node_type = postFix[i]->data_type;
        switch (node_type)
        {
        case DATA_TYPE_OPERATOR:
        {
            //运算符,区分单目双目、前后缀
            enum OPERATOR_TYPE operator_type = *(enum OPERATOR_TYPE *)postFix[i]->value_p;

            switch (operator_type)
            {
            case OPERATOR_SQUARE:
                calc_stack[calc_stack_offset - 1] = operation_multiple(calc_stack[calc_stack_offset - 1], calc_stack[calc_stack_offset - 1]);
                break;
            case OPERATOR_SQUARE_ROOT:
                if(calc_stack[calc_stack_offset - 1]<0){
                    err_exist=4;
                    return 0;
                }
                calc_stack[calc_stack_offset - 1] = operation_squareroot(calc_stack[calc_stack_offset - 1]);
                break;

            default:
                if (calc_stack_offset > 1)
                {
                    switch (operator_type)
                    {
                    case OPERATOR_PLUS:
                        calc_stack[calc_stack_offset - 2] = operation_plus(calc_stack[calc_stack_offset - 1], calc_stack[calc_stack_offset - 2]);
                        calc_stack_offset--;
                        break;
                    case OPERATOR_MINUS:
                        calc_stack[calc_stack_offset - 2] = operation_minus(calc_stack[calc_stack_offset - 2], calc_stack[calc_stack_offset - 1]);
                        calc_stack_offset--;
                        break;
                    case OPERATOR_MULTI:
                        calc_stack[calc_stack_offset - 2] = operation_multiple(calc_stack[calc_stack_offset - 2], calc_stack[calc_stack_offset - 1]);
                        calc_stack_offset--;
                        break;
                    case OPERATOR_DIVDE:
                        if(calc_stack[calc_stack_offset - 1] == 0){//除0错误
                            err_exist=1;
                            return 0;
                        }
                        calc_stack[calc_stack_offset - 2] = operation_divide(calc_stack[calc_stack_offset - 2], calc_stack[calc_stack_offset - 1]);
                        calc_stack_offset--;
                        break;
                    default:
                        //未知运算符
                        break;
                    }
                }
                else
                {
                    //运算栈内少于两个，表达式有误
                    err_exist=3;
                    return 0;
                }
            }

            break;
        }
        default:
            //数字直接压入计算栈
            calc_stack[calc_stack_offset] = *(float *)(postFix[i]->value_p);
            calc_stack_offset++;
            break;
        }
    }
    if (calc_stack_offset == 1)
        return calc_stack[0];
    else
    { //错误
        err_exist=3;
        return 0;
    }
}



static uint8_t operator_pri(void *value_p)
{
    enum OPERATOR_TYPE *operator_p = value_p;
    if(*operator_p <=OPERATOR_RIGHT_PAREN)
        return 0;
    if (*operator_p <= OPERATOR_MINUS)
        return 1;
    if (*operator_p <= OPERATOR_DIVDE)
        return 2;
    if (*operator_p <= OPERATOR_SQUARE)
        return 3;
}

static float operation_plus(float addend1, float addend2)
{
    //加法
    return addend1 + addend2;
}

static float operation_minus(float minuend, float subtrahend)
{
    return minuend - subtrahend;
}

static float operation_multiple(float multiplier1, float multiplier2)
{
    return multiplier1 * multiplier2;
}

static float operation_divide(float dividend, float divisor)
{
    return dividend / divisor;
}

static float operation_squareroot(float oprand)
{
    return sqrt(oprand);
}



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
            if (decimal){
                err_exist=3;
                return NULL;
            }
            else
                decimal = ptr;
        }
    }
    if (!decimal)
    { //如果没有小数点则为整数
        decimal = tail + 1;
        for (i = 0; i < decimal - head ; i++)
        {
            //计算整数部分
            result += (*(head + i) - '0') * pow(10, decimal - head - 1 - i);
        }
        if((ceil(result)-result) < 0.5)result=ceil(result);

    }
    else
    {
        //如果有小数点,约束小数位数
        tail = (tail - decimal) > MAX_DECIMAL_PLACES ? decimal + 6 : tail;
				
        for (i = 0; i < tail - decimal; i++)
        {
            result += (*(decimal + i + 1) - '0') * pow(0.1, (i + 1));
            
        }

        for (i = 0; i < decimal - head ; i++)
        {
            //计算整数部分
            result += (*(head + i) - '0') * pow(10, decimal - head - 1 - i);
        }
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
            err_exist=3;
            break;
        }
        //生成操作符节点并挂载
        push(root, newNode(DATA_TYPE_OPERATOR, &operator_type));
        ptr++;
    }
}

