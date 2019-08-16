#include "as5600.h"
#include "I2C.h"
#include "usart.h"
#include "delay.h"

void AS5600_WriteReg(u8 reg_add,u8 reg_dat)
{
	u8 a,b,c;
	printf("\r\nwrite---");
	I2C_Start();                                                   //启动总线
	I2C_Send_Byte(AS5600_WRITE);										//写从器件地址
  a=I2C_Wait_Ack();
	I2C_Send_Byte(reg_add);											            //写数据地址
	b=I2C_Wait_Ack();
	I2C_Send_Byte(reg_dat);									               //写数据
	c=I2C_Wait_Ack();
	I2C_Stop();   
	
	if(a==0&&b==0&&c==0)
	  printf("\r\nwrite success");
	else
		printf("\r\nwrite failed");
}


u8 AS5600_ReadData(u8 reg_add)
{
	u8 rev;
	u8 a,b,c;
	printf("\r\nread---");
	I2C_Start();
	I2C_Send_Byte(AS5600_WRITE);
	a=I2C_Wait_Ack();
	I2C_Send_Byte(reg_add);
	b=I2C_Wait_Ack();
	I2C_Start();
	I2C_Send_Byte(AS5600_READ);
	c=I2C_Wait_Ack();	   
	//rev=I2C_Read_Byte(1);	
	rev=I2C_Read_Byte(0);	//stable
	I2C_Stop();
	
	if(a==0&&b==0&&c==0)
	  printf("\r\nread success");
	else
		printf("\r\nread failed");
	return rev;
}

//写12位的寄存器   ZPOS/MPOS/MANG
void AS5600_Write_12(u8 reg_add,u16 set_value)
{
//	u16 read_h,read_l,get_value; 
	set_value=set_value&0xFFF;
  //write
	delay_ms(100);
	AS5600_WriteReg(reg_add,set_value>>8);
	delay_ms(100);
	AS5600_WriteReg(reg_add+1,set_value&0xFF);//L
	//read
	AS5600_Read_12(reg_add);
}

//读12位的寄存器   ZPOS/MPOS/MANG/RAW_ANGLE/ANGLE/MAGNITUDE
u16 AS5600_Read_12(u8 reg_add)
{
	u16 read_h,read_l,get_value; 
	//read
	delay_ms(100);
	read_h = AS5600_ReadData(reg_add)&0xF;
	printf("\r\nH..=%d",read_h);
	delay_ms(100);
	read_l = AS5600_ReadData(reg_add+1);//L
	printf("\r\nL..=%d",read_l);
  get_value=(read_h<<8)+read_l;
  printf("\r\nget_value..=%d",get_value);
	return get_value;
}

//写14位的寄存器   CONF
void AS5600_Write_CONF(u8 reg_add,u16 set_value)
{
//	u16 read_h,read_l,get_value; 
	set_value=set_value&0x3FFF;
  //write
	delay_ms(100);
	AS5600_WriteReg(reg_add,set_value>>8);
	delay_ms(100);
	AS5600_WriteReg(reg_add+1,set_value&0xFF);//L
	//read
	AS5600_Read_CONF(reg_add);
}

//读14位的寄存器   CONF
u16 AS5600_Read_CONF(u8 reg_add)
{
	u16 read_h,read_l,get_value; 
	//read
	delay_ms(100);
	read_h = AS5600_ReadData(reg_add);
	printf("\r\nH..=%d",read_h);
	delay_ms(100);
	read_l = AS5600_ReadData(reg_add+1);//L
	printf("\r\nL..=%d",read_l);
  get_value=(read_h<<8)+read_l;
  printf("\r\nget_value..=%d",get_value);
	return get_value;
}


