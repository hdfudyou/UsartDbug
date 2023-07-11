#ifndef __USARTDbug_H
#define __USARTDbug_H

#include "List.h"

#define Receive 1
#define Send 2

extern Node *pReceive;
extern Node *pNode;//����ͷ�ڵ�

void SetSendReceiveData(Node *pNode,void *Address,uint32_t Size/*,uint8_t SendOrReceive*/);
void USARTSendReceiveList(Node *pNode,uint8_t SendOrReceive);
void USARTDbug_Init(void);
void DataTransmit(Node *pNode,uint8_t* Sourse);//���ݰ���
void USARTDReceive_Init(void);

#endif

