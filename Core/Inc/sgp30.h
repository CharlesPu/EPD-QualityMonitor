// sgp30.h �ļ������SGP30��صĺ궨��ͺ�������

#ifndef SGP30_H
#define SGP30_H

#include "stm32f1xx_hal.h"

// ����SGP30��IIC�ӻ���ַ
#define SGP30_ADDR 0x58

// ����SGP30�ṹ�壬�洢CO2��TVOC��ֵ
typedef struct {
  uint16_t co2;
  uint16_t tvoc;
} sgp30_data_t;

// ����SGP30��ʼ�����������ͳ�ʼ������
void sgp30_init(void);

// ����SGP30��ȡ���ݺ���������CO2��TVOC��ֵ
sgp30_data_t sgp30_read(void);

// ����SGP30����CRCУ��ֵ�ĺ�����ʹ�ö���ʽx8 + x5 + x4 + x0
uint8_t sgp30_crc(uint8_t *data, uint8_t len);

#endif
