#include "stm32f10x.h"
#include "usart.h"
#include "as5600.h"
#include "delay.h"
#include "I2C.h"
#include "timer.h"

extern u8  TIM3CH1_CAPTURE_STA;	  				
extern u16	TIM3CH1_CAPTURE_VAL;	
extern u16 IC1Value;
extern u16 IC2Value;
extern u32 dutyCycle;

/*
WD FTH SF PWMF OUTS HYST PM
0  000 00 00   10   00   00   
          11   10   00   00
          10   10   00   00 

conf 0xe0 ,mang 0xfff

1:write conf and mang
2:set burn_setting
3:verify zpos/mpos/conf/mang
4:read conf and mang
5:set start position(pull the out pin to gnd,then float)
6:set stop position
7:set burn_angle
8:read zpos and mpos(before exeture 3)
9:read position
*/




int main(void)
{
  uart_init(115200);
	delay_init();
	printf("\r\nstart...");
  I2C_Simulation_Init();
//	encoder_timer_Init();
	
  while (1)
  {
		printf("\r\nPWM- position:%d",AS5600_Read_12(AS5600_RAW_ANGLE_H)*(360/4096));
		delay_ms(1000);
  }
}





