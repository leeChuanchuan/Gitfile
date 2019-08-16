#include "stm32f10x.h"

#define AS5600_ADDRESS 0x36
#define AS5600_READ 0x6D    //01101101
#define AS5600_WRITE 0x6C   //01101100

#define AS5600_ZMCO 0x00   //R  default£º0
#define AS5600_ZPOS_H 0x01 //R/W  default£º0  
//#define AS5600_ZPOS_L 0x02 //R/W  default£º0   
#define AS5600_MPOS_H 0x03 //R/W  default£º0   
//#define AS5600_MPOS_L 0x04 //R/W  default£º0  
#define AS5600_MANG_H 0x05 //R/W  default£º0   
//#define AS5600_MANG_L 0x06 //R/W  default£º0   
#define AS5600_CONF_H 0x07 //R/W  default£º0    
//#define AS5600_CONF_L 0x08 //R/W  default£º0
#define AS5600_RAW_ANGLE_H 0x0C //R  default£º0
//#define AS5600_RAW_ANGLE_L 0x0D //R
#define AS5600_ANGLE_H 0x0E //R
//#define AS5600_ANGLE_L 0x0F //R
#define AS5600_STATUS 0x0B //R  default£º0
#define AS5600_AGC 0x1A //R  default£º128
#define AS5600_MAGNITUDE_H 0x1B //R  default£º0
#define AS5600_MAGNITUDE_L 0x1C //R
#define AS5600_BURN 0xFF   //W   Burn_Angle=0x80;Burn_Setting=0x40    //R  default£º0
#define Burn_Angle 0x80
#define Burn_Setting 0x40

u8 AS5600_ReadData(u8 reg_add);
void AS5600_WriteReg(u8 reg_add,u8 reg_dat);
void AS5600_Write_12(u8 reg_add,u16 set_value);
u16 AS5600_Read_12(u8 reg_add);
void AS5600_Write_CONF(u8 reg_add,u16 set_value);
u16 AS5600_Read_CONF(u8 reg_add);
