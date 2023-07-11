#include "USARTDbug.h"
#include "stdlib.h"

extern UART_HandleTypeDef huart2;//串口
extern DMA_HandleTypeDef hdma_usart2_rx;//串口接收
extern DMA_HandleTypeDef hdma_usart2_tx;//串口发送
extern DMA_HandleTypeDef hdma_memtomem_dma1_channel1;//数据搬运

Node *pReceive;
Node *pNode;//链表头节点
float Head;//帧头
//char Head;//帧头
char End[4];//帧尾
uint8_t verify;

DMA_HandleTypeDef* hdma_usart_rx;//串口接收DMA
DMA_HandleTypeDef* hdma_usart_tx;//串口发送DMA
DMA_HandleTypeDef*hdma_memtomem;//内存到内存DMA
UART_HandleTypeDef*huart;//串口

void USARTDbug_Config(void)//需要配置两个DMA的地址，一个是接收串口的DMA，一个是发送串口的DMA
{
   //3个DMA，1个串口初始化
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
/*经过HAL库配置后，DMA的外设地址不变，内存地址会递增
现还需配置DMA_CNDTRx来设置传输的数据量和DMA_CMARx设置内存地址
还有配置DMA_CCRx来启动通道*/
//此处应该包含对串口DMA的初始化,包括串口DMA的发送与接收
void USARTSendReceiveList(Node *pNode,uint8_t SendOrReceive)//遍历链表里面的所有元素，形参是链表头,链表头里面的东西也会发送
{
    /*校验位*/
	//   uint8_t* Address;
    /*无校验位*/
    Node *pMove;
    pMove = pNode;
    if(SendOrReceive==Send)
    {
	    while (pMove != NULL) 
        {
    /*校验位代码块*/
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
        /*以上是校验位的代码块*/
          while(!(huart2.Instance->SR&0x00000040));//TC位
          HAL_UART_Transmit_DMA(huart, (uint8_t *)pMove->Address,pMove->Size);
           pMove = pMove->next;
        }
    }
    else
    {
        while (pMove != NULL)//接收时 
        {
            pMove = pMove->next;
            
        }
    }
//        printf("\n%s函数执行，打印带头结点的单链表成功\n",__FUNCTION__);
}
//使用方法：先用InitialList(Node *pNode)对目标进行初始化
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
    Node *pMove;//帧尾
    pNode=InitialList(pNode);
    CreateList(pNode,2);
	pMove=pNode->next;//中间节点空出来
    pNode->Address=&Head;
    pNode->Size=sizeof(Head);
    /*以下是需要校验位时添加*/
//    pMove->Address=&verify;
//    pMove->Size=sizeof(verify);
//    pMove=pMove->next;
    /*以上是需要校验位时添加*/
    pMove->Address=&End;
    pMove->Size=sizeof(End);
}

void USARTDReceive_Init(void)
{
   pReceive=InitialList(pReceive);
   pReceive->Address=NULL;
}

void Calculate(Node *pNode)//遍历数组的所有元素，计算需要多大的缓冲区，设置下一次缓冲区
{//置空闲中断处
	uint32_t HeadAddress;//用于记录头节点初始位置用于释放
    Node *pMove;
    pMove = pNode;
	HeadAddress=(uint32_t)pNode->Address;  
    while (pMove != NULL)//接收时 
    {
       pMove = pMove->next;
       if(pNode->Address!=NULL)//丢掉第一帧
			 {
				 HAL_DMA_Start_IT(hdma_memtomem,(uint32_t)pNode->Address,(uint32_t)pMove->Address,pMove->Size);
				 pNode->Address=(void*)((uint32_t)pNode->Address+pMove->Size);
			 }				 
       pNode->Size+=pMove->Size; 
    }
    free((void*)HeadAddress);
    pNode->Address=(void*)malloc(pNode->Size);
    HAL_UART_Receive_DMA(huart,pNode->Address,(pNode->Size+3));//包含一个校验位和帧头帧尾
}

void DataTransmit(Node *pNode,uint8_t* Sourse)//数据搬运
{
    uint32_t i=0;
    Node *pMove;
    pMove = pNode;
    while ((pMove != NULL)&&(pMove->next!=NULL))//接收时 
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

//串口手动配置：
/*        //先检查是否传输完毕,在进行配置
        //while(!(huart2.Instance->SR&0x00000080);//TXE位
        //while(!(huart2.Instance->SR&0x00000040));//TC位
        //huart2.Instance->CR3&=0xFFFFFFBF;//关闭串口允许DMA的通道
        //__HAL_DMA_DISABLE(&hdma_usart2_tx);
        //hdma_usart2_tx.Instance->CNDTR=pMove->Size;//配置传输量大小
        //hdma_usart2_tx.Instance->CMAR=(uint32_t)pMove->Address;//配置存储器地址
        //hdma_usart2_tx.Instance->CPAR=(uint32_t)&(huart2.Instance->DR);
        //__HAL_DMA_ENABLE(&hdma_usart2_tx);
        //huart2.Instance->CR3|=0x00000040;//开启串口的开关*/



