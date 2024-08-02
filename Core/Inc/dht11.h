#ifndef __DHT11_H_
#define __DHT11_H_
#include "main.h"

typedef struct {
  unsigned char rh_H;
	unsigned char rh_L;
	unsigned char temp_H;
	unsigned char temp_L;
} dht11_data_t;

void DHT_GPIO_Init_IN(void);
void DHT_GPIO_Init_OUT(void);

void HAL_Delay_us(uint16_t time);
dht11_data_t DHT11_REC_Data(void);



#endif

