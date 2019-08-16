#include "stm32f10x.h"

extern u8 USART_RX_BUF[64];
extern u8 USART_RX_STA;   

void USART1_Init(void);

void USART1_Config(void);

void NVIC_USART1_Config(void);
