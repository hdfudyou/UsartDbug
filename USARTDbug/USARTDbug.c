#include "USARTDbug.h"
#include "stdlib.h"

extern UART_HandleTypeDef huart2;//����
extern DMA_HandleTypeDef hdma_usart2_rx;//���ڽ���
extern DMA_HandleTypeDef hdma_usart2_tx;//���ڷ���
extern DMA_HandleTypeDef hdma_memtomem_dma1_channel1;//���ݰ���

Node *pReceive;
Node *pNode;//����ͷ�ڵ�
float Head;//֡ͷ
//char Head;//֡ͷ
char End[4];//֡β
uint8_t verify;

DMA_HandleTypeDef* hdma_usart_rx;//���ڽ���DMA
DMA_HandleTypeDef* hdma_usart_tx;//���ڷ���DMA
DMA_HandleTypeDef*hdma_memtomem;//�ڴ浽�ڴ�DMA
UART_HandleTypeDef*huart;//����

void USARTDbug_Config(void)//��Ҫ��������DMA�ĵ�ַ��һ���ǽ��մ��ڵ�DMA��һ���Ƿ��ʹ��ڵ�DMA
{
   //3��DMA��1�����ڳ�ʼ��
   huart=&huart2;
   hdma_memtomem=&hdma_memtomem_dma1_channel1;
}


void SetSendReceiveData(Node *pNode,void *Address,uint32_t Size/*,uint8_t SendOrReceive*/)
{
    Node *pInsert;
	pInsert=InsertHeadList(pNode);
    pInsert->Address=Address;
    pInsert->Size=Size;
}
/*����HAL�����ú�DMA�������ַ���䣬�ڴ��ַ�����
�ֻ�������DMA_CNDTRx�����ô������������DMA_CMARx�����ڴ��ַ
��������DMA_CCRx������ͨ��*/
//�˴�Ӧ�ð����Դ���DMA�ĳ�ʼ��,��������DMA�ķ��������
void USARTSendReceiveList(Node *pNode,uint8_t SendOrReceive)//�����������������Ԫ�أ��β�������ͷ,����ͷ����Ķ���Ҳ�ᷢ��
{
    /*У��λ*/
	//   uint8_t* Address;
    /*��У��λ*/
    Node *pMove;
    pMove = pNode;
    if(SendOrReceive==Send)
    {
	    while (pMove != NULL) 
        {
    /*У��λ�����*/
//          if((pMove->Address!=pNode->Address)&&(pMove->Address!=&verify))
//          {
//			        Address=(uint8_t*)(pMove->Address);
//              for(uint32_t i=0;i<pMove->Size;i++)
//              {
//                 verify+=Address[i];
//              }
//          }
//          else if(pMove->Address==pNode->Address)
//          {
//            verify=0;
//          }
        /*������У��λ�Ĵ����*/
          while(!(huart2.Instance->SR&0x00000040));//TCλ
          HAL_UART_Transmit_DMA(huart, (uint8_t *)pMove->Address,pMove->Size);
           pMove = pMove->next;
        }
    }
    else
    {
        while (pMove != NULL)//����ʱ 
        {
            pMove = pMove->next;
            
        }
    }
//        printf("\n%s����ִ�У���ӡ��ͷ���ĵ�����ɹ�\n",__FUNCTION__);
}
//ʹ�÷���������InitialList(Node *pNode)��Ŀ����г�ʼ��
void USARTDbug_Init(void)
{
    verify=0;
	  Head=00;
//	  Head=0xA5;
//	  End=0x5A;
	  {
			End[0]=0x00;
			End[1]=0x00;
			End[2]=0x80;
			End[3]=0x7F;
		}
    Node *pMove;//֡β
    pNode=InitialList(pNode);
    CreateList(pNode,2);
	pMove=pNode->next;//�м�ڵ�ճ���
    pNode->Address=&Head;
    pNode->Size=sizeof(Head);
    /*��������ҪУ��λʱ���*/
//    pMove->Address=&verify;
//    pMove->Size=sizeof(verify);
//    pMove=pMove->next;
    /*��������ҪУ��λʱ���*/
    pMove->Address=&End;
    pMove->Size=sizeof(End);
}

void USARTDReceive_Init(void)
{
   pReceive=InitialList(pReceive);
   pReceive->Address=NULL;
}

void Calculate(Node *pNode)//�������������Ԫ�أ�������Ҫ���Ļ�������������һ�λ�����
{//�ÿ����жϴ�
	uint32_t HeadAddress;//���ڼ�¼ͷ�ڵ��ʼλ�������ͷ�
    Node *pMove;
    pMove = pNode;
	HeadAddress=(uint32_t)pNode->Address;  
    while (pMove != NULL)//����ʱ 
    {
       pMove = pMove->next;
       if(pNode->Address!=NULL)//������һ֡
			 {
				 HAL_DMA_Start_IT(hdma_memtomem,(uint32_t)pNode->Address,(uint32_t)pMove->Address,pMove->Size);
				 pNode->Address=(void*)((uint32_t)pNode->Address+pMove->Size);
			 }				 
       pNode->Size+=pMove->Size; 
    }
    free((void*)HeadAddress);
    pNode->Address=(void*)malloc(pNode->Size);
    HAL_UART_Receive_DMA(huart,pNode->Address,(pNode->Size+3));//����һ��У��λ��֡ͷ֡β
}

void DataTransmit(Node *pNode,uint8_t* Sourse)//���ݰ���
{
    uint32_t i=0;
    Node *pMove;
    pMove = pNode;
    while ((pMove != NULL)&&(pMove->next!=NULL))//����ʱ 
    {
       pMove = pMove->next;
    //    HAL_DMA_Start_IT(hdma_memtomem,(uint32_t)pNode->Address,(uint32_t)pMove->Address,pMove->Size);
		HAL_DMA_Start_IT(hdma_memtomem,(uint32_t)&(Sourse[i]),(uint32_t)pMove->Address,pMove->Size);
		HAL_DMA_PollForTransfer(hdma_memtomem,HAL_DMA_FULL_TRANSFER,20);
         i+=pMove->Size;
	// 	pNode->Address=(void*)((uint32_t)pNode->Address+pMove->Size);				 
    //    pNode->Size+=pMove->Size;
    }
}

//�����ֶ����ã�
/*        //�ȼ���Ƿ������,�ڽ�������
        //while(!(huart2.Instance->SR&0x00000080);//TXEλ
        //while(!(huart2.Instance->SR&0x00000040));//TCλ
        //huart2.Instance->CR3&=0xFFFFFFBF;//�رմ�������DMA��ͨ��
        //__HAL_DMA_DISABLE(&hdma_usart2_tx);
        //hdma_usart2_tx.Instance->CNDTR=pMove->Size;//���ô�������С
        //hdma_usart2_tx.Instance->CMAR=(uint32_t)pMove->Address;//���ô洢����ַ
        //hdma_usart2_tx.Instance->CPAR=(uint32_t)&(huart2.Instance->DR);
        //__HAL_DMA_ENABLE(&hdma_usart2_tx);
        //huart2.Instance->CR3|=0x00000040;//�������ڵĿ���*/



