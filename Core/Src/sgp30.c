#include "sgp30.h"
#include "i2c.h"

// ����SGP30��ʼ�����������ͳ�ʼ������
void sgp30_init(void) {
  uint8_t cmd[2] = {0x20, 0x03}; // ��ʼ������Ϊ0x2003
  HAL_I2C_Master_Transmit(&hi2c2, SGP30_ADDR << 1, cmd, 2, 100); // ���ͳ�ʼ������
  HAL_Delay(20); // �ȴ�10ms
}

// ����SGP30��ȡ���ݺ���������CO2��TVOC��ֵ
sgp30_data_t sgp30_read(void) {
  uint8_t cmd[2] = {0x20, 0x08}; // ��ȡ����Ϊ0x2008
  uint8_t data[6]; // �洢���ص�6���ֽ�����
  uint8_t crc; // �洢CRCУ��ֵ
  sgp30_data_t result; // �洢���

  HAL_I2C_Master_Transmit(&hi2c2, SGP30_ADDR << 1, cmd, 2, 100); // ���Ͷ�ȡ����
  HAL_Delay(25); // �ȴ�12ms
  HAL_I2C_Master_Receive(&hi2c2, SGP30_ADDR << 1 | 0x01, data, 6, 100); // ����6���ֽ�����

  crc = sgp30_crc(data, 2); // ����ǰ�����ֽڵ�CRCУ��ֵ
  if (crc == data[2]) { // ����͵������ֽ���ͬ��˵��CO2������Ч
    result.co2 = (data[0] << 8) | data[1]; // ��ǰ�����ֽںϲ�ΪCO2ֵ
  } else {
    result.co2 = 0; // ���򣬷���0��ʾ��Ч
  }

  crc = sgp30_crc(data + 3, 2); // ������ĺ͵�����ֽڵ�CRCУ��ֵ
  if (crc == data[5]) { // ����͵������ֽ���ͬ��˵��TVOC������Ч
    result.tvoc = (data[3] << 8) | data[4]; // �����ĺ͵�����ֽںϲ�ΪTVOCֵ
  } else {
    result.tvoc = 0; // ���򣬷���0��ʾ��Ч
  }

  return result; // ���ؽ��
}

// ����SGP30����CRCУ��ֵ�ĺ�����ʹ�ö���ʽx8 + x5 + x4 + x0
uint8_t sgp30_crc(uint8_t *data, uint8_t len) {
  uint8_t crc = 0xFF; // ��ʼֵΪ0xFF
  uint8_t bit; // �洢λ����

  for (uint8_t i = 0; i < len; i++) { // ����ÿ���ֽ�
    crc ^= data[i]; // ���ǰ�ֽ�
    for (bit = 8; bit > 0; bit--) { // ����ÿ��λ
      if (crc & 0x80) { // ������λΪ1�������Ʋ����0x31
        crc = (crc << 1) ^ 0x31;
      } else { // ���򣬾�����
        crc = (crc << 1);
      }
    }
  }

  return crc; // ����CRCУ��ֵ
}
