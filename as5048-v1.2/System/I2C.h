#ifndef _I2C_H
#define _I2C_H
#include "sys.h"

//�����ֲ����ʱֻҪ��һ�������ط�������
#define GPIO_I2C GPIOB

#define I2C_SCL GPIO_Pin_8    //��һ��I/O��
#define I2C_SDA GPIO_Pin_9

#define I2C_SCL_2 GPIO_Pin_6  //�ڶ���I/O��
#define I2C_SDA_2 GPIO_Pin_7

#define I2C_SCL_3 GPIO_Pin_4  //������I/O��
#define I2C_SDA_3 GPIO_Pin_5

#define I2C_SCL_4 GPIO_Pin_3   //������I/O��
#define I2C_SDA_4 GPIO_Pin_2

#define I2C_SCL_H GPIO_SetBits(GPIO_I2C,I2C_SCL)   //��һ��I/O��ʱ���ߺ������߿���
#define I2C_SCL_L GPIO_ResetBits(GPIO_I2C,I2C_SCL)
#define I2C_SDA_H GPIO_SetBits(GPIO_I2C,I2C_SDA)
#define I2C_SDA_L GPIO_ResetBits(GPIO_I2C,I2C_SDA)

#define I2C_SCL_H_2 GPIO_SetBits(GPIO_I2C,I2C_SCL_2)   //�ڶ���I/O��ʱ���ߺ������߿���
#define I2C_SCL_L_2 GPIO_ResetBits(GPIO_I2C,I2C_SCL_2)
#define I2C_SDA_H_2 GPIO_SetBits(GPIO_I2C,I2C_SDA_2)
#define I2C_SDA_L_2 GPIO_ResetBits(GPIO_I2C,I2C_SDA_2)

#define I2C_SCL_H_3 GPIO_SetBits(GPIO_I2C,I2C_SCL_3)   //������I/O��ʱ���ߺ������߿���
#define I2C_SCL_L_3 GPIO_ResetBits(GPIO_I2C,I2C_SCL_3)
#define I2C_SDA_H_3 GPIO_SetBits(GPIO_I2C,I2C_SDA_3)
#define I2C_SDA_L_3 GPIO_ResetBits(GPIO_I2C,I2C_SDA_3)

#define I2C_SCL_H_4 GPIO_SetBits(GPIO_I2C,I2C_SCL_4)   //������I/O��ʱ���ߺ������߿���
#define I2C_SCL_L_4 GPIO_ResetBits(GPIO_I2C,I2C_SCL4)
#define I2C_SDA_H_4 GPIO_SetBits(GPIO_I2C,I2C_SDA_4)
#define I2C_SDA_L_4 GPIO_ResetBits(GPIO_I2C,I2C_SDA_4)

void I2C_Simulation_Init(Num_Port);
void I2C_SDA_OUT(void);
void I2C_SDA_IN(void);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_Ack(void);
void I2C_NAck(void);
u8   I2C_Wait_Ack(void);
void I2C_Send_Byte(u8 txd);
u8   I2C_Read_Byte(u8 ack);

void C_I2C_SDA_H(Num_Port);
void C_I2C_SDA_L(Num_Port);
void C_I2C_SCL_H(Num_Port);
void C_I2C_SCL_L(Num_Port);
#endif
