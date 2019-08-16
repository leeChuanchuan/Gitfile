#include "mpu6050.h"
#include "I2C.h"
/************************************************************************/
//�������ƣ�void MPU6050_WriteReg(u8 reg_add,u8 reg_dat)
//�����ʣ��u8 reg_add		���ݵ�ַ
//					u8 reg_dat		����
//���ز�����void
//���ܣ�д�����ݵ�MPU6050�ƶ�λ��
/************************************************************************/
void MPU6050_WriteReg(u8 reg_add,u8 reg_dat)
{
	I2C_Start();                                                   //��������
	I2C_Send_Byte(MPU6050_SLAVE_ADDRESS);										//д��������ַ
	I2C_Wait_Ack();                                                  //�ȴ�Ӧ��
	I2C_Send_Byte(reg_add);											            //д���ݵ�ַ
	I2C_Wait_Ack(); 
	I2C_Send_Byte(reg_dat);									               //д����
	I2C_Wait_Ack();                                              //�ȴ�Ӧ��
	I2C_Stop();          
}
/************************************************************************/
//�������ƣ�void MPU6050_ReadData(u8 reg_add,unsigned char*Read,u8 num)
//�����ʣ��u8 reg_add				//���ݵ�ַ
//					unsigned char*Read	//��������ָ���ַ
//					u8 num				//���ݳ���
//���ز��������������ݣ�ָ����ʽ
//���ܣ�		��ȡ�ƶ��Ĵ�����num������
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
//�������ƣvoid MPU6050_Init(void)
//�����ʣ��void
//���ز�����void
//���ܣ�	��ʼ��6050
/************************************************************************/
void MPU6050_Init(void)
{ 
	MPU6050_WriteReg(MPU6050_RA_PWR_MGMT_1, 0x03);	     //�������״̬
	MPU6050_WriteReg(MPU6050_RA_SMPLRT_DIV , 0x00);	    //�����ǲ�����
	MPU6050_WriteReg(MPU6050_RA_CONFIG , 0x03);					//��ͨ�˲�Ƶ��	42hz
	MPU6050_WriteReg(MPU6050_RA_ACCEL_CONFIG , 0x01);	  //���ü��ٶȴ�����������2Gģʽ
	MPU6050_WriteReg(MPU6050_RA_GYRO_CONFIG, 0x18);     //�������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)
}
/************************************************************************/
//�������ƣu8 MPU6050ReadID(void)
//�����ʣ��void
//���ز�����void
//���ܣ�	��ȡ6050Ĭ�ϵ�ַ��Ҳ���ڼ��6050�Ƿ�װ��IIC��ȷ���
/************************************************************************/
u8 MPU6050ReadID(void)
{
	unsigned char Re = 0;
    MPU6050_ReadData(MPU6050_RA_WHO_AM_I,&Re,1);    //��������ַ
    return Re;
}
/************************************************************************/
//�������ƣ�void MPU6050ReadAcc(short *accData)
//�����ʣ��short *accData
//���ز�����void
//���ܣ�	��ȡ���ٶ�xyzֵ
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
//�������ƣ�void MPU6050ReadGyro(short *gyroData)
//�����ʣ��short *gyroData
//���ز�����void
//���ܣ�	��ȡ������xyzֵ
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
//�������ƣ�void MPU6050_ReturnTemp(short*Temperature)
//�����ʣ��short*Temperature
//���ز�����void
//���ܣ�		��ȡ�¶�
/************************************************************************/
void MPU6050_ReturnTemp(short*Temperature)
{
	short temp3;
	u8 buf[2];
	
	MPU6050_ReadData(MPU6050_RA_TEMP_OUT_H,buf,2);     //��ȡ�¶�ֵ
  temp3= (buf[0] << 8) | buf[1];
	*Temperature=(((double) (temp3 + 13200)) / 280)-13;
}
