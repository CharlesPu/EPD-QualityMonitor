// sgp30.h 文件，存放SGP30相关的宏定义和函数声明

#ifndef SGP30_H
#define SGP30_H

#include "stm32f1xx_hal.h"

// 定义SGP30的IIC从机地址
#define SGP30_ADDR 0x58

// 定义SGP30结构体，存储CO2和TVOC的值
typedef struct {
  uint16_t co2;
  uint16_t tvoc;
} sgp30_data_t;

// 声明SGP30初始化函数，发送初始化命令
void sgp30_init(void);

// 声明SGP30读取数据函数，返回CO2和TVOC的值
sgp30_data_t sgp30_read(void);

// 声明SGP30计算CRC校验值的函数，使用多项式x8 + x5 + x4 + x0
uint8_t sgp30_crc(uint8_t *data, uint8_t len);

#endif
