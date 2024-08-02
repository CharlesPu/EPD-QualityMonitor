#include "dht11.h"
#include "main.h"
#include "stdio.h"
#include "gpio.h"
#include "tim.h"


//粗延时函数，微秒
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
//  __HAL_TIM_SET_COUNTER(&htim2,differ); //设定TIM2计数器起始值
//  HAL_TIM_Base_Start(&htim2);   //启动定时器 
//  
//  while(differ < 0xffff-5){ //判断
//    differ = __HAL_TIM_GET_COUNTER(&htim2);   //查询计数器的计数值
//  }
//  HAL_TIM_Base_Stop(&htim2);
//}


void DHT11_Start()
{
	DHT_GPIO_Init_OUT();                                             //GPIOB GPIO_PIN_8 配置为推挽输出
	HAL_GPIO_WritePin(DHT11_DI_GPIO_Port, DHT11_DI_Pin, GPIO_PIN_SET);            //先拉高
	HAL_Delay(1);                                                   //延时（随便，只是拉高）
	HAL_GPIO_WritePin(DHT11_DI_GPIO_Port, DHT11_DI_Pin, GPIO_PIN_RESET);          //拉低，起始信号
	HAL_Delay(20);                                                  //至少18ms
	HAL_GPIO_WritePin(DHT11_DI_GPIO_Port, DHT11_DI_Pin, GPIO_PIN_SET);            //然后拉高
	HAL_Delay_us(30);                                               //等待20-40us
	DHT_GPIO_Init_IN();                                                 //GPIOB GPIO_PIN_8 配置为浮空输入 
}

char DHT11_Rec_Byte(void)
{
	unsigned char i = 0;
	unsigned char data;
	
	for(i=0;i<8;i++)                                                //1个数据就是1个字节byte，1个字节byte有8位bit
	{
		while( HAL_GPIO_ReadPin(DHT11_DI_GPIO_Port, DHT11_DI_Pin) == RESET );       //从1bit开始，低电平变高电平，等待低电平结束
		HAL_Delay_us(30);                                             //延迟30us是为了区别数据0和数据1，0只有26~28us
		
		data <<= 1;                                                   //左移一位
		
		if( HAL_GPIO_ReadPin(DHT11_DI_GPIO_Port, DHT11_DI_Pin) == 1 )               //如果过了30us还是高电平的话就是数据1
		{
			data |= 1;                                                  //数据+1
		}
		
		while( HAL_GPIO_ReadPin(DHT11_DI_GPIO_Port, DHT11_DI_Pin) == 1 );           //高电平变低电平，等待高电平结束
	}
	
	return data;
}

//获取数据

dht11_data_t DHT11_REC_Data(void)
{
	unsigned char R_Z,R_X,T_Z,T_X;
	unsigned char rh_H,rh_L,temp_H,temp_L,CHECK;

	DHT11_Start();                                                  //主机发送信号
	
	if( HAL_GPIO_ReadPin(DHT11_DI_GPIO_Port, DHT11_DI_Pin) == RESET )             //判断DHT11是否响应
	{
		while( HAL_GPIO_ReadPin(DHT11_DI_GPIO_Port, DHT11_DI_Pin) == 0);            //低电平变高电平，等待低电平结束
		while( HAL_GPIO_ReadPin(DHT11_DI_GPIO_Port, DHT11_DI_Pin) == 1);            //高电平变低电平，等待高电平结束
		
		R_Z   =   DHT11_Rec_Byte();
		R_X   =   DHT11_Rec_Byte();
		T_Z   =   DHT11_Rec_Byte();
		T_X   =   DHT11_Rec_Byte();
		CHECK =   DHT11_Rec_Byte();                                   //接收5个数据
		 
		DHT_GPIO_Init_OUT();
		HAL_GPIO_WritePin(DHT11_DI_GPIO_Port, DHT11_DI_Pin, GPIO_PIN_RESET);        //当最后一bit数据传送完毕后，DHT11拉低总线 50us
		HAL_Delay_us(55);                                             //这里延时55us
		HAL_GPIO_WritePin(DHT11_DI_GPIO_Port, DHT11_DI_Pin, GPIO_PIN_SET);          //随后总线由上拉电阻拉高进入空闲状态。
		
		if(R_Z + R_X + T_Z + T_X == CHECK)                            //和检验位对比，判断校验接收到的数据是否正确
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
//	oled_show_chinese(0, 1, 0);                                     //温
//	oled_show_chinese(20, 1, 1);                                    //度
//	oled_show_chinese(0, 4, 2);                                     //湿
//	oled_show_chinese(20, 4, 1);                                    //度
//	oled_show_string(40, 1, ":", 2);                                //:
//	oled_show_string(40, 4, ":", 2);                                //:
//	oled_show_string(76, 1, ".", 2);                                //.
//	oled_show_string(76, 4, ".", 2);                                //.
//	oled_show_char(60, 1, TZ/10+48, 16);                            //温度整数第十位
//	oled_show_char(68, 1, TZ%10+48, 16);                            //温度整数第个位
//	oled_show_char(84, 1, TX+48, 16);																//温度小数
//	oled_show_chinese(92, 1, 3);                                    //℃
//	oled_show_char(60, 4, RZ/10+48, 16);                            //温度整数第十位
//	oled_show_char(68, 4, RZ%10+48, 16);                            //温度整数第个位
//	oled_show_char(84, 4, RX+48, 16);																//温度小数
//	oled_show_string(96, 4, "%", 2);                                //.
//	rec_data[0] = RH;
//	rec_data[1] = RL;
//	rec_data[2] = TH;
//	rec_data[3] = TL;
}

void DHT_GPIO_Init_IN(void)                   //DHT11_DI_GPIO_Port PIN_8 配置为推挽输ru 
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
void DHT_GPIO_Init_OUT(void)                   //GPIOB PIN_8 配置为推挽输出 上拉
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
