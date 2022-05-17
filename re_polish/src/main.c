

#include "stdio.h"
#include "C8051F020.h"
#include "stack.h"
#include "re_polish.h"
#include <math.h>
#include <stdlib.h>

//#include "si_toolchain.h"
//#include "C8051F020_def"






unsigned char xdata malloc_mempool [0x1000];


extern struct StackNode *postFix[STACK_SIZE_MAX];



int main(void){
	
	int l,i;
    char * err_code;
	float ans=0;
    struct StackNode *root = NULL;
    char notation[] = "(23.4/2*15";
	
	
	if(NULL){
		while(1);
		
	}
	
	
	err_code = NULL;
	
	if(err_code){
		while(1);
		
	}
	
	
	init_mempool (&malloc_mempool, sizeof(malloc_mempool));
    strToList(&root, notation);
    if(err_code=error_Check()){
        
        while(1);
    }
    l=infixToPostfix(root->previous);
    if(err_code=error_Check()){
        
        while(1);
    }


    /*
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
*/
    
    
    
    ans = calcPostfix(l);
	if(err_code=error_Check()){
        
        while(1);
    }
	
	return 0;
}