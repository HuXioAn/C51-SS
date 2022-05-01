#ifndef _H_STACK_
#define _H_STACK_

#include "type_def.h"


#define STACK_NODE_MAX 128



enum DATA_TYPE_E{
    DATA_TYPE_INT=0,
    DATA_TYPE_FLOAT,
    DATA_TYPE_OPERATOR
};




struct StackNode {
    enum DATA_TYPE_E data_type;
		void * value_p;
    struct StackNode* previous;
		struct StackNode* next;
};

struct StackNode* newNode(enum DATA_TYPE_E type,void* data_p);

uint32_t isEmpty(struct StackNode* root);

void push(struct StackNode** root, struct StackNode* node);

struct StackNode* pop(struct StackNode** root);

void deleteNode(struct StackNode* node);

void deleteStack(struct StackNode** root);






#endif