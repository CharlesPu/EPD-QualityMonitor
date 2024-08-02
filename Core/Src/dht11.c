#include "dht11.h"
#include "main.h"
#include "stdio.h"
#include "gpio.h"
#include "tim.h"


//����ʱ������΢��
void HAL_Delay_us(uint16_t nus)
{    
 __HAL_TIM_SET_COUNTER(&htim2, 0);  // set the counter value a 0
 __HAL_TIM_ENABLE(&htim2);          // start the counter
  // wait for the counte r to reach the us input in the parameter
 while (__HAL_TIM_GET_COUNTER(&htim2) < nus);
 __HAL_TIM_DISABLE(&htim2);         // stop the counter
}

//void delay_us(uint32_t us)
//{
//  uint16_t differ = 0xffff-us-5;        
//  __HAL_TIM_SET_COUNTER(&htim2,differ); //�趨TIM2��������ʼֵ
//  HAL_TIM_Base_Start(&htim2);   //������ʱ�� 
//  
//  while(differ < 0xffff-5){ //�ж�
//    differ = __HAL_TIM_GET_COUNTER(&htim2);   //��ѯ�������ļ���ֵ
//  }
//  HAL_TIM_Base_Stop(&htim2);
//}


void DHT11_Start()
{
	DHT_GPIO_Init_OUT();                                             //GPIOB GPIO_PIN_8 ����Ϊ�������
	HAL_GPIO_WritePin(DHT11_DI_GPIO_Port, DHT11_DI_Pin, GPIO_PIN_SET);            //������
	HAL_Delay(1);                                                   //��ʱ����㣬ֻ�����ߣ�
	HAL_GPIO_WritePin(DHT11_DI_GPIO_Port, DHT11_DI_Pin, GPIO_PIN_RESET);          //���ͣ���ʼ�ź�
	HAL_Delay(20);                                                  //����18ms
	HAL_GPIO_WritePin(DHT11_DI_GPIO_Port, DHT11_DI_Pin, GPIO_PIN_SET);            //Ȼ������
	HAL_Delay_us(30);                                               //�ȴ�20-40us
	DHT_GPIO_Init_IN();                                                 //GPIOB GPIO_PIN_8 ����Ϊ�������� 
}

char DHT11_Rec_Byte(void)
{
	unsigned char i = 0;
	unsigned char data;
	
	for(i=0;i<8;i++)                                                //1�����ݾ���1���ֽ�byte��1���ֽ�byte��8λbit
	{
		while( HAL_GPIO_ReadPin(DHT11_DI_GPIO_Port, DHT11_DI_Pin) == RESET );       //��1bit��ʼ���͵�ƽ��ߵ�ƽ���ȴ��͵�ƽ����
		HAL_Delay_us(30);                                             //�ӳ�30us��Ϊ����������0������1��0ֻ��26~28us
		
		data <<= 1;                                                   //����һλ
		
		if( HAL_GPIO_ReadPin(DHT11_DI_GPIO_Port, DHT11_DI_Pin) == 1 )               //�������30us���Ǹߵ�ƽ�Ļ���������1
		{
			data |= 1;                                                  //����+1
		}
		
		while( HAL_GPIO_ReadPin(DHT11_DI_GPIO_Port, DHT11_DI_Pin) == 1 );           //�ߵ�ƽ��͵�ƽ���ȴ��ߵ�ƽ����
	}
	
	return data;
}

//��ȡ����

dht11_data_t DHT11_REC_Data(void)
{
	unsigned char R_Z,R_X,T_Z,T_X;
	unsigned char rh_H,rh_L,temp_H,temp_L,CHECK;

	DHT11_Start();                                                  //���������ź�
	
	if( HAL_GPIO_ReadPin(DHT11_DI_GPIO_Port, DHT11_DI_Pin) == RESET )             //�ж�DHT11�Ƿ���Ӧ
	{
		while( HAL_GPIO_ReadPin(DHT11_DI_GPIO_Port, DHT11_DI_Pin) == 0);            //�͵�ƽ��ߵ�ƽ���ȴ��͵�ƽ����
		while( HAL_GPIO_ReadPin(DHT11_DI_GPIO_Port, DHT11_DI_Pin) == 1);            //�ߵ�ƽ��͵�ƽ���ȴ��ߵ�ƽ����
		
		R_Z   =   DHT11_Rec_Byte();
		R_X   =   DHT11_Rec_Byte();
		T_Z   =   DHT11_Rec_Byte();
		T_X   =   DHT11_Rec_Byte();
		CHECK =   DHT11_Rec_Byte();                                   //����5������
		 
		DHT_GPIO_Init_OUT();
		HAL_GPIO_WritePin(DHT11_DI_GPIO_Port, DHT11_DI_Pin, GPIO_PIN_RESET);        //�����һbit���ݴ�����Ϻ�DHT11�������� 50us
		HAL_Delay_us(55);                                             //������ʱ55us
		HAL_GPIO_WritePin(DHT11_DI_GPIO_Port, DHT11_DI_Pin, GPIO_PIN_SET);          //��������������������߽������״̬��
		
		if(R_Z + R_X + T_Z + T_X == CHECK)                            //�ͼ���λ�Աȣ��ж�У����յ��������Ƿ���ȷ
		{
			rh_H = R_Z;
			rh_L = R_X;
			temp_H = T_Z;
			temp_L = T_X;
		}else {
				printf("error check dht");
		}
	}
	
	dht11_data_t rt;
	rt.rh_H=rh_H;
	rt.rh_L=rh_L;
	rt.temp_H=temp_H;
	rt.temp_L=temp_L;
	return rt;
//	oled_show_chinese(0, 1, 0);                                     //��
//	oled_show_chinese(20, 1, 1);                                    //��
//	oled_show_chinese(0, 4, 2);                                     //ʪ
//	oled_show_chinese(20, 4, 1);                                    //��
//	oled_show_string(40, 1, ":", 2);                                //:
//	oled_show_string(40, 4, ":", 2);                                //:
//	oled_show_string(76, 1, ".", 2);                                //.
//	oled_show_string(76, 4, ".", 2);                                //.
//	oled_show_char(60, 1, TZ/10+48, 16);                            //�¶�������ʮλ
//	oled_show_char(68, 1, TZ%10+48, 16);                            //�¶������ڸ�λ
//	oled_show_char(84, 1, TX+48, 16);																//�¶�С��
//	oled_show_chinese(92, 1, 3);                                    //��
//	oled_show_char(60, 4, RZ/10+48, 16);                            //�¶�������ʮλ
//	oled_show_char(68, 4, RZ%10+48, 16);                            //�¶������ڸ�λ
//	oled_show_char(84, 4, RX+48, 16);																//�¶�С��
//	oled_show_string(96, 4, "%", 2);                                //.
//	rec_data[0] = RH;
//	rec_data[1] = RL;
//	rec_data[2] = TH;
//	rec_data[3] = TL;
}

void DHT_GPIO_Init_IN(void)                   //DHT11_DI_GPIO_Port PIN_8 ����Ϊ������ru 
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();


  /*Configure GPIO pin : PB8 */
  GPIO_InitStruct.Pin = DHT11_DI_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(DHT11_DI_GPIO_Port, &GPIO_InitStruct);

}
void DHT_GPIO_Init_OUT(void)                   //GPIOB PIN_8 ����Ϊ������� ����
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
	// NOTE!!!!! must set because first level is high, to avoid REST to SET
  HAL_GPIO_WritePin(DHT11_DI_GPIO_Port, DHT11_DI_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : PB12 */
  GPIO_InitStruct.Pin = DHT11_DI_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(DHT11_DI_GPIO_Port, &GPIO_InitStruct);

}
