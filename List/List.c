#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "List.h"


//��ֻ��ʼ��ͷ�ڵ�
Node * InitialList(Node *pNode)
{
    //���˽���ÿһ��malloc�����ڴ�ռ�󣬶�Ҫ�����жϷ����Ƿ�ɹ���Ҳ���ж��Ƿ�Ϊ�գ�
    //��ʱ�����pNode����һ��ͷ��㣻
    //��ʼ���ɹ�����ʵ�൱����һ�������������ˣ�
    pNode = (Node *)malloc(sizeof(Node));//ͷ�ڵ�
    if (pNode == NULL) 
    {
        return NULL;
        // printf("%s����ִ�У��ڴ����ʧ�ܣ���ʼ��������ʧ��\n",__FUNCTION__);
    }
    else
    {
        pNode->next =NULL;
        return pNode;
        // printf("%s����ִ�У���ͷ���ĵ������ʼ�����\n",__FUNCTION__);
    }
}

//2.������ͷ���ĵ�����
void CreateList(Node *pNode,uint8_t Num)//������Num���ڵ�(����ͷ)������
{
    Node *pInsert;
    Node *pMove;
    pInsert = (Node *)malloc(sizeof(Node));//��Ҫ�������ڴ��Ƿ�ɹ� pInsert == NULL  ?
    memset(pInsert, 0, sizeof(Node));
    pInsert->next = NULL;

//    scanf("%d",&(pInsert->element));
    pMove = pNode;
    for(uint8_t i=0;i<Num;i++)
    {

        pMove->next = pInsert;
        pMove = pInsert;

        pInsert = (Node *)malloc(sizeof(Node)); //��Ҫ�������ڴ��Ƿ�ɹ� pInsert == NULL  ?
        memset(pInsert, 0, sizeof(Node));
        pInsert->next = NULL;

//        scanf("%d",&(pInsert->element));
    }

//    printf("%s����ִ��,��ͷ���ĵ��������ɹ�\n",__FUNCTION__);
}

// //3.��ӡ��ͷ���ĵ�����
// void PrintList(Node *pNode)
// {
//     /**
//      *  ע��������������Ϊ�գ�ֻ��һ��ͷ��㣩����Ҳ������ӡ����ӡ�ɹ�����ֻ������û��Ԫ�أ�������ǿյĶ���,�����ڿ���̨�Ͼ���һ�пյģ�
//      */
//         Node *pMove;
//         pMove = pNode->next;
//         while (pMove != NULL) {
//             printf("%d ",pMove->element);
//             pMove = pMove->next;
//         }

// //        printf("\n%s����ִ�У���ӡ��ͷ���ĵ�����ɹ�\n",__FUNCTION__);
// }

//4.������Ա��е�����Ԫ�أ����ͷ����нڵ㣨����ͷ��㣩��ʹ֮��Ϊһ���ձ�
void ClearList(Node *pNode)
{
    Node *pMove;
    pMove = pNode->next;
    while (pMove != NULL) {

        pNode->next = pMove->next;
        free(pMove);
        pMove = pNode->next;
    }

//    printf("%s����ִ�У���մ�ͷ��������ɹ�\n",__FUNCTION__);
}

//5.���ش�ͷ���ĵ�����ĳ���,�˳��Ȳ�����β�ڵ�
uint8_t SizeList(Node *pNode)
{
    /**
     *  ��ֻ��һ��ͷ����ʱ��size = 0��ͷ�ڵ㲻���㵽�������С�
     */
    uint8_t i = 0;
    Node *pMove;
    pMove = pNode->next;
    while (pMove->next != NULL) 
	{
       i++;
       pMove = pMove->next;
    }

//    printf("%s����ִ�У���ͷ���ĵ�����ĳ���Ϊ��%d\n",__FUNCTION__,i);

    return i;
}

//6.�жϴ�ͷ���ĵ������Ƿ�Ϊ�գ�Ϊ���򷵻�1�����򷵻�0
uint8_t IsEmptyList(Node *pNode)
{
    /**
     *  ��ֻ��һ��ͷ����ʱ�򣬸������Ϊ��
     */
    if (pNode->next == NULL) 
    {
//        printf("%s����ִ�У���ͷ��������Ϊ��\n",__FUNCTION__);
        return 1;
    }

//    printf("%s����ִ�У���ͷ��������ǿ�\n",__FUNCTION__);

    return 0;
}

//7.���ص������е�pos������е�Ԫ�أ�������-1����ʾû���ҵ�
int GetElement(Node *pNode,uint8_t pos)
{
    int i = 1;
    Node *pMove;
    pMove = pNode->next;

    while (pMove != NULL) {
        if (i == pos) {

//            printf("%s����ִ�У�pos=%dλ�õ�ֵ��%d\n",__FUNCTION__,pos,pMove->element);
            return pMove->element;
        }

        i++;
        pMove = pMove->next;
    }

//    printf("%s����ִ�У���pos=%dλ��û���ҵ�ֵ\n",__FUNCTION__,pos);
    return -1;
}

// //8.�ӵ������в��Ҿ��и���ֵx�ĵ�һ��Ԫ�أ������ҳɹ��򷵻ظý��data��Ĵ洢��ַ�����򷵻�NULL
// elemType* GetElemAddr(Node *pNode,int x)
// {
//     Node *pMove;
//     pMove = pNode->next;

//     while (pMove != NULL) {
//         if (pMove->element == x) {
// //            printf("%s����ִ�У����ҵ�x=%d���ڴ��ַΪ:0x%x\n",__FUNCTION__,x,&(pMove->element));
//             return &(pMove->element);
//         }
//         pMove = pMove->next;
//     }

// //    printf("%s����ִ�У��ڴ�ͷ���ĵ�������û���ҵ�x=%d��ֵ���޷�����ڴ��ַ\n",__FUNCTION__,x);

//     return NULL;
// }

//9.�ѵ������е�pos������ֵ�޸�Ϊx��ֵ
// Node* ModifyElem(Node *pNode,int pos,int x)
// {
//     int i = 1;
//     Node *pMove;
//     pMove = pNode->next;
//     while (pMove != NULL) {
//         if (i == pos) {
//             pMove->element = x;
// //            printf("%s����ִ�У���pos=%dλ�õ�ֵ��Ϊ%d�ɹ�\n",__FUNCTION__,pos,x);
//             return pNode;
//         }
//         i++;
//         pMove = pMove->next;
//     }
// //    printf("%s����ִ�У�����Ϊ�ջ�������posֵ�Ƿ����޸�ֵʧ��\n",__FUNCTION__);

//     return pNode;
// }

//10.������ı�ͷ����һ��Ԫ��
Node *InsertHeadList(Node *pNode)//x�������ֵ
{
    Node *pInsert;
    pInsert = (Node *)malloc(sizeof(Node));
    memset(pInsert, 0, sizeof(Node));
//    pInsert->element = x;

    pInsert->next = pNode->next;
    pNode->next = pInsert;

//    printf("%s����ִ�У��ڱ�ͷ����Ԫ��%d�ɹ�\n",__FUNCTION__,x);
    return pInsert;
}

// 11.������ĩβ��ǰһ���ڵ����һ��Ԫ��
Node *InsertTailList(Node *pNode)//x�������ֵ
{
    Node *pMove;
    Node *pInsert;
    pInsert = (Node *)malloc(sizeof(Node));
    memset(pInsert, 0, sizeof(Node));
//    pInsert->next = NULL;

    pMove = pNode;
    while ((pMove->next->next != NULL)&&(pMove->next!= NULL)) 
    {
        pMove = pMove->next;
    }
		pInsert->next=pMove->next;
    pMove->next = pInsert;

//    printf("%s����ִ�У��ڱ�β����Ԫ��%d�ɹ�\n",__FUNCTION__,x);

    return pInsert;
}

//// int main(int argc, const char * argv[]) {

////     Node *pList;

////     InitialList(&pList);

//// CreateList(pList);
//    // PrintList(pList);

//    // SizeList(pList);

////     IsEmptyList(pList);

////     GetElement(pList, 3);

////     GetElemAddr(pList, 5);

////     ModifyElem(pList, 2, 111);
////     PrintList(pList);

////     InsertHeadList(pList,1234);
////     PrintList(pList);
////     SizeList(pList);

////     InsertTailList(pList,9999);
////     PrintList(pList);
////     SizeList(pList);

////     ClearList(pList);
////     PrintList(pList);

////     return 0;
//// }

