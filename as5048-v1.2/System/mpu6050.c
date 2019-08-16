#include "mpu6050.h"
#include "I2C.h"
/************************************************************************/
//函数名称：void MPU6050_WriteReg(u8 reg_add,u8 reg_dat)
//输入参剩：u8 reg_add		数据地址
//					u8 reg_dat		数据
//返回参数：void
//功能：写入数据到MPU6050制定位置
/************************************************************************/
void MPU6050_WriteReg(u8 reg_add,u8 reg_dat)
{
	I2C_Start();                                                   //启动总线
	I2C_Send_Byte(MPU6050_SLAVE_ADDRESS);										//写从器件地址
	I2C_Wait_Ack();                                                  //等待应答
	I2C_Send_Byte(reg_add);											            //写数据地址
	I2C_Wait_Ack(); 
	I2C_Send_Byte(reg_dat);									               //写数据
	I2C_Wait_Ack();                                              //等待应答
	I2C_Stop();          
}
/************************************************************************/
//函数名称：void MPU6050_ReadData(u8 reg_add,unsigned char*Read,u8 num)
//输入参剩：u8 reg_add				//数据地址
//					unsigned char*Read	//返回数据指针地址
//					u8 num				//数据长度
//返回参数：读到的数据，指针形式
//功能：		读取制定寄存器的num个数据
/************************************************************************/
void MPU6050_ReadData(u8 reg_add,unsigned char*Read,u8 num)
{
	unsigned char i;
	
	I2C_Start();
	I2C_Send_Byte(MPU6050_SLAVE_ADDRESS);
	I2C_Wait_Ack();
	I2C_Send_Byte(reg_add);
	I2C_Wait_Ack();
	
	I2C_Start();
	I2C_Send_Byte(MPU6050_SLAVE_ADDRESS+1);
	I2C_Wait_Ack();
	
	for(i=0;i<(num-1);i++){
		*Read=I2C_Read_Byte(1);
		Read++;
	}
	*Read=I2C_Read_Byte(0);
	I2C_Stop();
}
/************************************************************************/
//函数名称oid MPU6050_Init(void)
//输入参剩：void
//返回参数：void
//功能：	初始化6050
/************************************************************************/
void MPU6050_Init(void)
{ 
	MPU6050_WriteReg(MPU6050_RA_PWR_MGMT_1, 0x03);	     //解除休眠状态
	MPU6050_WriteReg(MPU6050_RA_SMPLRT_DIV , 0x00);	    //陀螺仪采样率
	MPU6050_WriteReg(MPU6050_RA_CONFIG , 0x03);					//低通滤波频率	42hz
	MPU6050_WriteReg(MPU6050_RA_ACCEL_CONFIG , 0x01);	  //配置加速度传感器工作在2G模式
	MPU6050_WriteReg(MPU6050_RA_GYRO_CONFIG, 0x18);     //陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
}
/************************************************************************/
//函数名称8 MPU6050ReadID(void)
//输入参剩：void
//返回参数：void
//功能：	读取6050默认地址，也用于检测6050是否安装或IIC正确与否
/************************************************************************/
u8 MPU6050ReadID(void)
{
	unsigned char Re = 0;
    MPU6050_ReadData(MPU6050_RA_WHO_AM_I,&Re,1);    //读器件地址
    return Re;
}
/************************************************************************/
//函数名称：void MPU6050ReadAcc(short *accData)
//输入参剩：short *accData
//返回参数：void
//功能：	读取加速度xyz值
/************************************************************************/
void MPU6050ReadAcc(short *accData)
{
    u8 buf[6];
    MPU6050_ReadData(MPU6050_ACC_OUT, buf, 6);
    accData[0] = (buf[0] << 8) | buf[1];
    accData[1] = (buf[2] << 8) | buf[3];
    accData[2] = (buf[4] << 8) | buf[5];
}
/************************************************************************/
//函数名称：void MPU6050ReadGyro(short *gyroData)
//输入参剩：short *gyroData
//返回参数：void
//功能：	读取陀螺仪xyz值
/************************************************************************/
void MPU6050ReadGyro(short *gyroData)
{
    u8 buf[6];
    MPU6050_ReadData(MPU6050_GYRO_OUT,buf,6);
    gyroData[0] = (buf[0] << 8) | buf[1];
    gyroData[1] = (buf[2] << 8) | buf[3];
    gyroData[2] = (buf[4] << 8) | buf[5];
}
/************************************************************************/
//函数名称：void MPU6050_ReturnTemp(short*Temperature)
//输入参剩：short*Temperature
//返回参数：void
//功能：		读取温度
/************************************************************************/
void MPU6050_ReturnTemp(short*Temperature)
{
	short temp3;
	u8 buf[2];
	
	MPU6050_ReadData(MPU6050_RA_TEMP_OUT_H,buf,2);     //读取温度值
  temp3= (buf[0] << 8) | buf[1];
	*Temperature=(((double) (temp3 + 13200)) / 280)-13;
}
