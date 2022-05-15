#ifndef _H_RE_POLISH_
#define _H_RE_POLISH_

#include "stack.h"

#define STACK_SIZE_MAX 128

enum OPERATOR_TYPE{
    //优先级除了括号之外由低到高
    OPERATOR_LEFT_PAREN=0,
    OPERATOR_RIGHT_PAREN,
    OPERATOR_PLUS,
    OPERATOR_MINUS,
    OPERATOR_MULTI,
    OPERATOR_DIVDE,
    OPERATOR_SQUARE_ROOT,
    OPERATOR_SQUARE,
};


uint8_t infixToPostfix(struct StackNode *head);

float  calcPostfix(uint16_t notation_length);

char * error_Check(void);







#endif