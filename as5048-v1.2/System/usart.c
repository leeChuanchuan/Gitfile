#include "sys.h"
#include "usart.h"	  
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_UCOS
#include "includes.h"					//ucos 使用	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//串口1初始化		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/8/18
//版本：V1.5
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved
//********************************************************************************
//V1.3修改说明 
//支持适应不同频率下的串口波特率设置.
//加入了对printf的支持
//增加了串口接收命令功能.
//修正了printf第一个字符丢失的bug
//V1.4修改说明
//1,修改串口初始化IO的bug
//2,修改了USART_RX_STA,使得串口最大接收字节数为2的14次方
//3,增加了USART_REC_LEN,用于定义串口最大允许接收的字节数(不大于2的14次方)
//4,修改了EN_USART1_RX的使能方式
//V1.5修改说明
//1,增加了对UCOSII的支持
////////////////////////////////////////////////////////////////////////////////// 	  
 

//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET); 
    USART_SendData(USART1,(uint8_t)ch);   
	return ch;
}
#endif 

/*使用microLib的方法*/
 /* 
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (uint8_t) ch);

	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}	
   
    return ch;
}
int GetKey (void)  { 

    while (!(USART1->SR & USART_FLAG_RXNE));

    return ((int)(USART1->DR & 0x1FF));
}
*/
 
#if EN_USART1_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
//u16 USART_RX_STA=0;       //接收状态标记	  
u8 BmqDataRight;                    //Res为接受的数据8位；BmqDataRight为数据接收正确标志
u8 BmqOverFlag,BmqStartFlag;            //BmqOverFlag为结束接受数据标志；BmqStartFlag为开始接受数据标志
u8 BmqDataBuff[11],BmqDataCount;        //BmqDataBuff为存放数据的数组；BmqDataCount为数据计数变量


//初始化IO 串口1 
//bound:波特率
void uart_init(u32 bound){
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	//PA9  PA10
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);//重映射
 	//GPIO_PinRemapConfig(GPIO_Remap_USART1,ENABLE);	
	
	
	USART_DeInit(USART1);  //复位串口1
	
  //USART1_TX   PA.9    PB.6
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PA9
 
	//USART1_RX	  PA.10  PB.7
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化PA10


//  //PB6  PB7
//	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);//重映射
// 	GPIO_PinRemapConfig(GPIO_Remap_USART1,ENABLE);	
//	
//	USART_DeInit(USART1);  //复位串口1
//  //USART1_TX   PA.9    PB.6
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //PA.9
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
//	GPIO_Init(GPIOB, &GPIO_InitStructure); //初始化PA9
// 
//	//USART1_RX	  PA.10  PB.7
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
//	GPIO_Init(GPIOB, &GPIO_InitStructure);  //初始化PA10


  //USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(USART1, &USART_InitStructure); //初始化串口
#if EN_USART1_RX		  //如果使能了接收  
  //Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
   
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启中断
#endif
  USART_Cmd(USART1, ENABLE);//使能串口 
}





void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	u8 i,Res,BmqDataRight;                    //Res为接受的数据8位；BmqDataRight为数据接收正确标志
	u8 BmqOverFlag,BmqStartFlag;            //BmqOverFlag为结束接受数据标志；BmqStartFlag为开始接受数据标志
	u8 BmqDataBuff[11],BmqDataCount;        //BmqDataBuff为存放数据的数组；BmqDataCount为数据计数变量
	//实现代码：接收数据
	Res = USART_ReceiveData(USART1);
	BmqStartFlag = 0;
	BmqStartFlag = 0;
	if(BmqStartFlag == 0){    //缺帧首的情况，如果发现帧首标志变量为0，代表帧首缺失，则去判断这次接收的数据是否是
	   if(Res == 0xFE){       //帧首头字节，如果发现时帧首头字节，则将计数器COUNT赋值0，同时将该数据存入0号元素
		    BmqDataCount = 0;
			  BmqDataBuff[BmqDataCount++] = Res;
		 }
		 if(BmqDataCount == 1){  //再已经检测到帧首头字节的情况下（用COUNT是否为1去判断），再去判断下一次接受到的数据是否为0xFB
		    if(Res == 0xFB){     //如果是，则将计数器COUNT+1，同时将Res存入数组的第二个元素，以及将帧首缺失变量置1，代表帧首不再缺失
				BmqDataBuff[BmqDataCount++] = Res;
				BmqStartFlag = 1;
				}
				else{
				BmqDataCount = 0;    //如果发现下一次接受到的数据不是0xFB,则说明数据有问题，将上一次接受到的数组0号元素清零。
				BmqDataBuff[0] = 0;
				}
		 } 
	}
	else{   //当BmqStartFlag为1时，才将进行else操作
	   BmqDataRight = 1;
		 BmqDataBuff[BmqDataCount++] = Res;
		 if(BmqDataCount == 11){
		    BmqStartFlag = 0;
			  BmqDataCount = 0;
			  if(BmqDataBuff[2] != 0x03) BmqDataRight = 0;
			  if(BmqDataBuff[9] != 0xFE) BmqDataRight = 0;
			  if(BmqDataBuff[10]!= 0xFA) BmqDataRight = 0;
		 }
		 if(BmqDataRight == 0){
		    for(int i = 0;i<11,i++){
				    BmqDataCount[i] = 0;
					  BmqDataCount = 0;
					  BmqStartFlag = 0;
					  BmqOverFlag = 0;
				}
		 }
		 else{
		    BmqOverFlag = 1;
		 }
	   
	
	}
}
		
#ifdef OS_TICKS_PER_SEC	 	//如果时钟节拍数定义了,说明要使用ucosII了.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
		Res =USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据
		
		  		 
     } 
#ifdef OS_TICKS_PER_SEC	 	//如果时钟节拍数定义了,说明要使用ucosII了.
	OSIntExit();  											 
#endif
} 
#endif	

