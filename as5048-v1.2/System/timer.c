#include "timer.h"

#define ENCODER_TIMER TIM4

volatile u16 IC1Value = 0;	
volatile u16 IC2Value = 0;

void encoder_timer_Init(void)
{	 
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;    
	TIM_ICInitTypeDef  TIM_ICInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
	
	GPIO_PinRemapConfig(GPIO_Remap_TIM4,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,GPIO_Pin_6);			
	
	TIM_TimeBaseStructure.TIM_Period = 0xfffe; //设定计数器自动重装值 
	TIM_TimeBaseStructure.TIM_Prescaler =72-1; 	//预分频器   
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;  
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 	
	TIM_TimeBaseInit(ENCODER_TIMER,&TIM_TimeBaseStructure); 
		 
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICFilter = 0x0;
	//TIM_ICInit(ENCODER_TIMER, &TIM_ICInitStructure);
	TIM_PWMIConfig(ENCODER_TIMER, &TIM_ICInitStructure);
	/* Select the TIM2 Input Trigger: TI2FP2 */
	TIM_SelectInputTrigger(ENCODER_TIMER, TIM_TS_TI1FP1);

	/* Select the slave Mode: Reset Mode */
	TIM_SelectSlaveMode(ENCODER_TIMER, TIM_SlaveMode_Reset);
	TIM_SelectMasterSlaveMode(ENCODER_TIMER,TIM_MasterSlaveMode_Enable);
	/* TIM enable counter */
	TIM_Cmd(ENCODER_TIMER, ENABLE);
	
	/* Enable the CC2 Interrupt Request */
	TIM_ITConfig(ENCODER_TIMER, TIM_IT_CC1, ENABLE);
}


static void encoder_singnal_capture(void)
{  
	/* Get the Input Capture value */
	IC1Value = TIM_GetCapture1(ENCODER_TIMER);

	if(IC1Value != 0)
	{
		/* Duty cycle computation */
		IC2Value = TIM_GetCapture2(ENCODER_TIMER);
	}
	else
	{
		IC1Value = 0;
		IC2Value = 0;			
	}
}

void TIM4_IRQHandler(void)
{ 
		if (TIM_GetITStatus(ENCODER_TIMER, TIM_IT_CC1))//捕获1发生捕获事件	
		{	
			TIM_ClearITPendingBit(ENCODER_TIMER, TIM_IT_CC1);
			encoder_singnal_capture();
		}			     	    					    
}

s16  Calculate_Electric_Angle(void)
{
	s16 ThetaEst = 0;
	u32 Q32_Mechanical_To_Electric_Angle_Temp;

	float AS5600_Falling_Count_Temp = 0;
	float AS5600_Rising_Count_Temp = 0;
	AS5600_Rising_Count_Temp = IC1Value;
	AS5600_Falling_Count_Temp = IC2Value;

	//pwm  4119
	Q32_Mechanical_To_Electric_Angle_Temp = (AS5600_Falling_Count_Temp*4119/AS5600_Rising_Count_Temp-16);
	
	ThetaEst = (s16)Q32_Mechanical_To_Electric_Angle_Temp;

	return ThetaEst;
}

