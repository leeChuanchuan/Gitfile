#include "usart1.h"
#include "stdio.h"


u8 USART_RX_BUF[64];

u8 USART_RX_STA=0;
u8 count=0;

//int fputc(int ch, FILE *f)
//{
//   USART_SendData(USART1, (u8) ch);
//   while(!(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == SET))
//   {
//   }
//   return ch;
//}

//int fgetc(FILE *f)
//{
//   while(!(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET))
//   {
//   }
//   return (USART_ReceiveData(USART1));
//}

#if 1
#pragma import(__use_no_semihosting)

struct __FILE
{
int handle;
};
FILE __stdout;

_sys_exit(int x)
{
x = x;
}

int fputc(int ch, FILE *f)
{
while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);
 USART_SendData(USART1,(uint8_t)ch);
return ch;
}
#endif

void USART1_Init(void)
{
    USART1_Config();
    NVIC_USART1_Config();
}


void USART1_Config(void)
{
    //GPIO  init
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
    // USART init
   
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity =  USART_Parity_No ;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
 
  USART_Init(USART1, &USART_InitStructure);
 
  USART_Cmd(USART1, ENABLE);
}

void NVIC_USART1_Config(void)
{
    //NVICs
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
    //USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); //??????
}


void USART1_IRQHandler(void)
{
    u8 res;
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        res = USART_ReceiveData(USART1);
        //printf("%c",ch);
        USART_RX_BUF[count]=res;
    count++;
        if(res!=0x0a)USART_RX_STA=0;
                else USART_RX_STA=1;
        //USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
}



    

 
//void USART1_IRQHandler(void)
//{
//    u8 res;       
//    if(USART1->SR&(1<<5))//?????
//    {     
//        res=USART1->DR;
//        if((USART_RX_STA&0x80)==0)//?????
//        {
//            if(USART_RX_STA&0x40)//????0x0d
//            {
//                if(res!=0x0a)USART_RX_STA=0;//????,????
//                else USART_RX_STA|=0x80;    //?????
//            }else //????0X0D
//            {   
//                if(res==0x0d)USART_RX_STA|=0x40;
//                else
//                {
//                    //USART_RX_BUF[USART_RX_STA&0X3F]=res;
//                    USART_RX_BUF[count]=res;
//                    USART_RX_STA++;
//                    count++;
//                    if(USART_RX_STA>63)USART_RX_STA=0;//??????,??????     
//                }         
//            }
//        }                                                   
//    }     
//    USART_ClearITPendingBit(USART1, USART_IT_RXNE);  //??????
//} 