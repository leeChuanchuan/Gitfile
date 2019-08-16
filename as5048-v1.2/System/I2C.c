#include "delay.h"
#include "I2C.h"

void I2C_Simulation_Init(u8 Num_Port)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);						//开gpio管脚时钟
	
	GPIO_InitStructure.GPIO_Pin=I2C_SCL|I2C_SDA|I2C_SCL_2|I2C_SDA_2|I2C_SDA_3|I2C_SCL_3|I2C_SDA_4|I2C_SCL_4;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	C_I2C_SCL_H(Num_Port);
	C_I2C_SDA_H(Num_Port);
}

void C_I2C_SDA_H(u8 Num){
     switch(Num){
			 case 1:I2C_SDA_H;
		   case 2:I2C_SDA_H_2;
			 case 3:I2C_SDA_H_3;
			 case 4:I2C_SDA_H_4;
		 }
}

void C_I2C_SDA_L(u8 Num){
     switch(Num){
		 case 1:I2C_SDA_L;
		 case 2:I2C_SDA_L_2;
		 case 3:I2C_SDA_L_3;
		 case 4:I2C_SDA_L_4;
		 }

}

void C_I2C_SCL_H(u8 Num){
   switch(Num){
			 case 1:I2C_SCL_H;
		   case 2:I2C_SCL_H_2;
			 case 3:I2C_SCL_H_3;
			 case 4:I2C_SCL_H_4;
		 }
}

void C_I2C_SCL_L(u8 Num){
   switch(Num){
			 case 1:I2C_SCL_L;
		   case 2:I2C_SCL_L_2;
			 case 3:I2C_SCL_L_3;
			 case 4:I2C_SCL_L_4;
		 }
}

void I2C_SDA_OUT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;	
	
	GPIO_InitStructure.GPIO_Pin=I2C_SDA|I2C_SDA_2|I2C_SDA_3|I2C_SDA_4;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}

void I2C_SDA_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	
	GPIO_InitStructure.GPIO_Pin=I2C_SDA|I2C_SDA_2|I2C_SDA_3|I2C_SDA_4;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}

//产生起始信号
void I2C_Start(u8 Num_Port)
{
    I2C_SDA_OUT();

	C_I2C_SDA_H(Num_Port);
	C_I2C_SCL_H(Num_Port);
	delay_us(5);
	C_I2C_SDA_L(Num_Port);
	delay_us(6);
	C_I2C_SCL_L(Num_Port);
}

//产生停止信号
void I2C_Stop(u8 Num_Port)
{
   I2C_SDA_OUT();

   C_I2C_SCL_L(Num_Port);
   C_I2C_SDA_L(Num_Port);
   C_I2C_SCL_H(Num_Port);
   delay_us(6);
   C_I2C_SDA_H(Num_Port);
   delay_us(6);
}

//主机产生应答信号ACK
void I2C_Ack(u8 Num_Port)
{
   C_I2C_SCL_L(Num_Port);          //初始条件，持续一段时间，跳转状态，持续一段时间，回到初始状态
   C_I2C_SDA_OUT();
   C_I2C_SDA_L(Num_Port);
   delay_us(2);
   C_I2C_SCL_H(Num_Port);
   delay_us(5);
   C_I2C_SCL_L(Num_Port);          //有点像开关
}

//主机不产生应答信号NACK
void I2C_NAck(u8 Num_Port)
{
   C_I2C_SCL_L(Num_Port);
   I2C_SDA_OUT();
   C_I2C_SDA_H(Num_Port);
   delay_us(2);
   C_I2C_SCL_H(Num_Port);
   delay_us(5);
   C_I2C_SCL_L(Num_Port);
}
//等待从机应答信号
//返回值：1 接收应答失败
//		  0 接收应答成功
u8 I2C_Wait_Ack(u8 Num_Port)
{
	u8 tempTime=0;

	C_I2C_SDA_IN();

	C_I2C_SDA_H(Num_Port);
	delay_us(1);
	C_I2C_SCL_H(Num_Port);
	delay_us(1);//上面给出两条线上的初始状态SDA=1,SCL=1;

	while(GPIO_ReadInputDataBit(GPIO_I2C,I2C_SDA))
	{
		tempTime++;
		if(tempTime>250)
		{
			I2C_Stop(Num_Port);
			return 1;
		}	 
	}

	C_I2C_SCL_L(Num_Port);
	return 0;
}
//I2C 发送一个字节
void I2C_Send_Byte(u8 txd,u8)
{
	u8 i=0;

	I2C_SDA_OUT();
	C_I2C_SCL_L(Num_Port);//拉低时钟开始数据传输

	for(i=0;i<8;i++)
	{
		if((txd&0x80)>0) //0x80  1000 0000
			C_I2C_SDA_H(Num_Port);
		else
			C_I2C_SDA_L(Num_Port);

		txd<<=1;
		C_I2C_SCL_H(Num_Port);
		delay_us(2); //发送数据
		C_I2C_SCL_L(Num_Port);
		delay_us(2);
	}
}

//I2C 读取一个字节，ack=1时，发送ACK，ack=0，发送nACK 

u8 I2C_Read_Byte(u8 ack,u8 Num_Port)
{
   u8 i=0,receive=0;
	 
   I2C_SDA_IN();
   for(i=0;i<8;i++)
   {
   		C_I2C_SCL_L(Num_Port);
		delay_us(2);
		C_I2C_SCL_H(Num_Port);
		receive<<=1;
		delay_us(2);
		if(GPIO_ReadInputDataBit(GPIO_I2C,I2C_SDA))
		   receive++;
		delay_us(1);	
   }

   	if(ack==0)
	   	I2C_NAck(Num_Port);
	else
		I2C_Ack(Num_Port);

	return receive;
}



