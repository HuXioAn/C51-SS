
#include "stdio.h"
#include "stack.h"
#include "re_polish.h"
#include <stdlib.h>






struct StackNode* newNode(enum DATA_TYPE_E type,void * data_p)
{
	struct StackNode* stackNode =
	(struct StackNode*)
	malloc(sizeof(struct StackNode));
    if(stackNode==NULL)return NULL;

    switch (type)
    {
    case DATA_TYPE_INT:
        stackNode->value_p=malloc(sizeof(uint32_t));
        if(stackNode->value_p==NULL){free(stackNode); return NULL;}
        *(uint32_t*)(stackNode->value_p)=*(uint32_t*)data_p;
        break;
    case DATA_TYPE_FLOAT:
        stackNode->value_p=malloc(sizeof(float));
        if(stackNode->value_p==NULL){free(stackNode); return NULL;}
        *(float*)(stackNode->value_p)=*(float*)data_p;
        break;
    case DATA_TYPE_OPERATOR:
        stackNode->value_p=malloc(sizeof(enum OPERATOR_TYPE));
        if(stackNode->value_p==NULL){free(stackNode); return NULL;}
        *(enum OPERATOR_TYPE*)(stackNode->value_p)=*(enum OPERATOR_TYPE*)data_p;
        break;
    default:
        //ERROR
        break;
    }


    stackNode->data_type=type;
    stackNode->previous = stackNode;//为了形成回环
	stackNode->next = NULL;
	return stackNode;
}

uint32_t isEmpty(struct StackNode* root)
{
	return !root;
}

void push(struct StackNode** root, struct StackNode* node)
{   

	node->next = *root;

    if(*root){node->previous=(*root)->previous;(*root)->previous=node;}
	*root = node;
	//printf("%d pushed to stack\n", value_p);
}

struct StackNode* pop(struct StackNode** root)
{
	struct StackNode *temp;
	
	if (isEmpty(*root)){return NULL;}

	
	temp	= (*root);
	*root = (*root)->next;
  if(*root){(*root)->previous=temp->previous;}

	return temp;
}

//从双向链接栈中移除节点，并不会释放节点！
void removeNode(struct StackNode* node){
    node->next->previous=node->previous;
    node->previous->next=node->next;
}

//删除释放一个节点
void deleteNode(struct StackNode* node){
    free(node->value_p);
    free(node);
}


//删除释放整个栈
void deleteStack(struct StackNode** root){
    while((*root)->next!=NULL){
        free((*root)->value_p);
        (*root)=(*root)->next;
        free((*root)->previous);
    }
    free((*root)->value_p);
    free((*root));
    *root=NULL;
}




