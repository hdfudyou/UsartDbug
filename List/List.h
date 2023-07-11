#ifndef __LIST_H
#define __LIST_H

#include "stm32f1xx_hal.h"

typedef int elemType;
typedef struct NodeList
{
    void *Address;//��ַ
    uint32_t Size;//���ݿ��С
    int element;//δ֪
    struct NodeList *next;
}Node;

Node * InitialList(Node *pNode);
void CreateList(Node *pNode,uint8_t Num);
void PrintList(Node *pNode);
void ClearList(Node *pNode);
uint8_t SizeList(Node *pNode);
uint8_t IsEmptyList(Node *pNode);
elemType* GetElemAddr(Node *pNode,int x);
Node* ModifyElem(Node *pNode,int pos,int x);
Node *InsertHeadList(Node *pNode);
Node *InsertTailList(Node *pNode);


#endif

