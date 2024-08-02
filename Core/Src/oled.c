#include "oled.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "spi.h"
//#include "dma.h"

uint8_t u8x8_byte_4wire_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int,void *arg_ptr)
{
    switch (msg)
    {
        case U8X8_MSG_BYTE_SEND: /*Í¨¹ıSPI·¢ËÍarg_int¸ö×Ö½ÚÊı¾İ*/
//          HAL_SPI_Transmit_DMA(&hspi1, (uint8_t *)arg_ptr, arg_int);while(hspi1.TxXferCount);
			//Ê¹ÓÃDMA¿ÉÒÔ½«ÉÏÃæµÄ×¢ÊÍ½â³ı£¬°ÑÏÂÃæ²»´øDMAµÄ¸ø×¢ÊÍµô
			      HAL_SPI_Transmit(&hspi1,(uint8_t *)arg_ptr,arg_int,200);
            break;
        case U8X8_MSG_BYTE_INIT: /*³õÊ¼»¯º¯Êı*/
            break;
        case U8X8_MSG_BYTE_SET_DC: /*ÉèÖÃDCÒı½Å,±íÃ÷·¢ËÍµÄÊÇÊı¾İ»¹ÊÇÃüÁî*/
			      HAL_GPIO_WritePin(OLED_DC_GPIO_Port,OLED_DC_Pin,arg_int==0?GPIO_PIN_RESET:GPIO_PIN_SET);
            break;
        case U8X8_MSG_BYTE_START_TRANSFER: 
            u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_enable_level);
            u8x8->gpio_and_delay_cb(u8x8, U8X8_MSG_DELAY_NANO, u8x8->display_info->post_chip_enable_wait_ns, NULL);
            break;
        case U8X8_MSG_BYTE_END_TRANSFER: 
            u8x8->gpio_and_delay_cb(u8x8, U8X8_MSG_DELAY_NANO, u8x8->display_info->pre_chip_disable_wait_ns, NULL);
            u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_disable_level);
            break;
        default:
            return 0;
    }
    return 1;
}
 
uint8_t u8x8_stm32_gpio_and_delay(U8X8_UNUSED u8x8_t *u8x8,
    U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int,
    U8X8_UNUSED void *arg_ptr) 
{
    switch (msg)
    {
        case U8X8_MSG_GPIO_AND_DELAY_INIT: /*delayºÍGPIOµÄ³õÊ¼»¯£¬ÔÚmainÖĞÒÑ¾­³õÊ¼»¯Íê³ÉÁË*/
            break;
        case U8X8_MSG_DELAY_MILLI: /*ÑÓÊ±º¯Êı*/
            HAL_Delay(arg_int);     //µ÷ÓÃË­stm32ÏµÍ³ÑÓÊ±º¯Êı
            break;
        case U8X8_MSG_GPIO_CS: /*Æ¬Ñ¡ĞÅºÅ*/ //ÓÉÓÚÖ»ÓĞÒ»¸öSPIÉè±¸£¬ËùÒÔÆ¬Ñ¡ĞÅºÅÔÚ³õÊ¼»¯Ê±ÒÑ¾­ÉèÖÃÎªÎª³£ÓĞĞ§
					HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, arg_int==0?GPIO_PIN_RESET:GPIO_PIN_SET);
            break;
        case U8X8_MSG_GPIO_DC: /*ÉèÖÃDCÒı½Å,±íÃ÷·¢ËÍµÄÊÇÊı¾İ»¹ÊÇÃüÁî*/
            HAL_GPIO_WritePin(OLED_DC_GPIO_Port,OLED_DC_Pin,arg_int==0?GPIO_PIN_RESET:GPIO_PIN_SET);
            break;
        case U8X8_MSG_GPIO_RESET:
					if(arg_int)MD_OLED_RST_Set();
            else MD_OLED_RST_Clr();
            break;
    }
    return 1;
}
 
 
void u8g2Init(u8g2_t *u8g2)
{
	MD_OLED_RST_Set(); //ÏÔÊ¾Æ÷¸´Î»À­¸ß
	u8g2_Setup_ssd1327_ws_128x128_f(u8g2, U8G2_R0, u8x8_byte_4wire_hw_spi,             
    u8x8_stm32_gpio_and_delay);  // ³õÊ¼»¯0.96´çOLED u8g2 ½á¹¹Ìå
	u8g2_InitDisplay(u8g2);     //³õÊ¼»¯ÏÔÊ¾
	u8g2_SetPowerSave(u8g2, 0); //¿ªÆôÏÔÊ¾
}

void myDraw(u8g2_t *u8g2) {
	u8g2_ClearBuffer(u8g2);
	
	//u8g2_SetFontMode(u8g2, 1);
	 u8g2_SetFontDirection(u8g2,0);
//	 u8g2_SetFont(u8g2,u8g2_font_inb16_mf); 
//	 
//	 u8g2_DrawStr(u8g2,0, 20, "U8G2_TEST");
//	 u8g2_DrawStr(u8g2,0, 60, "lao wang");
//	 u8g2_DrawStr(u8g2,0+x, 22+y, "U");
//	 
//	 u8g2_SetFontDirection(u8g2,1);
//	 u8g2_SetFont(u8g2,u8g2_font_inb19_mn); 
//	 u8g2_DrawStr(u8g2,15+x, 8+y, "8");
//	 
//	 u8g2_SetFontDirection(u8g2,0);
//	 u8g2_SetFont(u8g2,u8g2_font_inb19_mf); 
//	 u8g2_DrawStr(u8g2,36+x, 22+y, "g");
//	 u8g2_DrawStr(u8g2,48+x, 22+y, "\xb2");
//	 
//	 u8g2_DrawHLine(u8g2,2+x, 25+y, 34);
//	 u8g2_DrawHLine(u8g2,3+x, 26+y, 34);
//	 u8g2_DrawVLine(u8g2,32+x, 22+y, 12);
//	 u8g2_DrawVLine(u8g2,33+x, 23+y, 12);
//	 u8g2_SendBuffer(u8g2);LaoWangChineseFont_16
	
	
	 u8g2_SetFont(u8g2, u8g2_font_health_lamp); 		//Ê¹ÓÃ×Ô¶¨Òå×Ö¿â£¬ÏÂÃæÊ¹ÓÃU8G2¹Ù·½GB2312×Ö¿â£¬Ğ¡ÈİÁ¿µ¥Æ¬»úÎŞ·¨ÊÔÓÃ
//	 u8g2_DrawUTF8(u8g2,0,20,"---|×ÖÌå|----");
//	 u8g2_DrawUTF8(u8g2,0,50,"ÀÏÍõµç×ÓÊıÂëDIY");
	 
	 // u8g2_SetFont(u8g2,u8g2_font_health_lamp); 
	 u8g2_DrawUTF8(u8g2,0,70,"æµ¦ç¿");
	 u8g2_DrawUTF8(u8g2,0,89,"wangjiao.....");
	 u8g2_DrawUTF8(u8g2,0,108,"18868105905.....");
	 u8g2_SendBuffer(u8g2);
	 HAL_Delay(2500);
	 
	 u8g2_ClearBuffer(u8g2);
	 u8g2_DrawUTF8(u8g2,0,70,"ç‹å¨‡");
	 u8g2_DrawUTF8(u8g2,0,89,"purui.....");
	 u8g2_DrawUTF8(u8g2,0,108,"18101907896.....");
	 u8g2_SendBuffer(u8g2);
	 HAL_Delay(1000);
	 u8g2_ClearBuffer(u8g2);

	  /*
	 u8g2_SetFont(u8g2,u8g2_font_wqy12_t_gb2312b); 
	 u8g2_ClearBuffer(u8g2);
	 u8g2_DrawUTF8(u8g2,0,10,"MCU£ºSTM32F407ZG");
	 u8g2_DrawUTF8(u8g2,0,24,"Èí¼ş£ºU8G2");
	 u8g2_DrawUTF8(u8g2,0,38,"BSD License:");
	 u8g2_DrawUTF8(u8g2,0,52,"U8g2lib Code");
	 u8g2_SendBuffer(u8g2);
	 
	 HAL_Delay(2500);
	 
	 u8g2_ClearBuffer(u8g2);
   u8g2_SetFont(u8g2,u8g2_font_wqy16_t_gb2312);
   u8g2_DrawUTF8(u8g2,0, 20,"------»­Í¼²âÊÔ-------");
   u8g2_SendBuffer(u8g2);
	 */
    
   HAL_Delay(200);
   u8g2_DrawBox(u8g2, 15, 45, 20, 20);
   u8g2_SendBuffer(u8g2);

   HAL_Delay(500 );
   u8g2_DrawFrame(u8g2, 40, 45, 20, 20);
   u8g2_SendBuffer(u8g2);

   HAL_Delay(500);
   u8g2_DrawRFrame(u8g2, 65, 45, 20, 20,3);
   u8g2_SendBuffer(u8g2);
	 
	 HAL_Delay(500);
   u8g2_DrawCircle(u8g2, 100, 55, 10,U8G2_DRAW_ALL);
   u8g2_SendBuffer(u8g2);
   HAL_Delay(500);
   u8g2_DrawEllipse(u8g2, 64, 95, 50,20,U8G2_DRAW_ALL);
   u8g2_SendBuffer(u8g2);
   HAL_Delay(2000);
	 
	  u8g2_ClearBuffer(u8g2);
    HAL_Delay(1000);
        
    u8g2_ClearBuffer(u8g2);
    u8g2_DrawXBMP(u8g2,0,0,128,128,gImage_1);
    u8g2_SendBuffer(u8g2);
    HAL_Delay(2000);   

    u8g2_ClearBuffer(u8g2);
    u8g2_DrawXBMP(u8g2,0,0,128,128,gImage_2);
    u8g2_SendBuffer(u8g2);
    HAL_Delay(2000); 
       
    u8g2_ClearBuffer(u8g2);
    u8g2_DrawXBMP(u8g2,0,0,128,128,gImage_4);
    u8g2_SendBuffer(u8g2);
    HAL_Delay(2000); 

    u8g2_ClearBuffer(u8g2);
    u8g2_DrawXBMP(u8g2,0,0,128,128,gImage_5);
    u8g2_SendBuffer(u8g2);
		
    HAL_Delay(2000); 
		
//		HAL_GPIO_WritePin(GPIOF,GPIO_PIN_9,GPIO_PIN_RESET);
//		HAL_GPIO_WritePin(GPIOF,GPIO_PIN_10,GPIO_PIN_RESET);
//		HAL_Delay(500);
//		HAL_GPIO_WritePin(GPIOF,GPIO_PIN_9,GPIO_PIN_SET);
//		HAL_GPIO_WritePin(GPIOF,GPIO_PIN_10,GPIO_PIN_SET);
//		HAL_Delay(500);
}

void myDraw_o(u8g2_t *u8g2, sgp30_data_t *sgp30, dht11_data_t *dht11) {
	u8g2_ClearBuffer(u8g2);
	u8g2_SetFontDirection(u8g2,0);
	
	u8g2_SetFont(u8g2, LaoWangChineseFont_16); 		//Ê¹ÓÃ×Ô¶¨Òå×Ö¿â£¬ÏÂÃæÊ¹ÓÃU8G2¹Ù·½GB2312×Ö¿â£¬Ğ¡ÈİÁ¿µ¥Æ¬»úÎŞ·¨ÊÔÓÃ
	char buff[50];
	
	sprintf(buff, "TEMP: %d.%d oC",dht11->temp_H, dht11->temp_L);
	u8g2_DrawUTF8(u8g2,0,30,buff);
	
	memset(buff, 0, sizeof(buff));
	sprintf(buff, "RH: %d.%d %%",dht11->rh_H, dht11->rh_L);
	u8g2_DrawUTF8(u8g2,0,50,buff);
	
	memset(buff, 0, sizeof(buff));
	sprintf(buff, "CO2: %d ppm",sgp30->co2);
	u8g2_DrawUTF8(u8g2,0,70,buff);
	
	memset(buff, 0, sizeof(buff));
	sprintf(buff, "TVOC: %d ppd",sgp30->tvoc);
	u8g2_DrawUTF8(u8g2,0,90,buff);
	
	u8g2_DrawHLine(u8g2,0, 100, 128);
	
	u8g2_SetFont(u8g2, u8g2_font_health_lamp); 
	u8g2_DrawUTF8(u8g2,0,120,"ç‹å¨‡&æµ¦ç¿");

	u8g2_SendBuffer(u8g2);
}

void myDraw_pic(u8g2_t *u8g2) {
	u8g2_ClearBuffer(u8g2);
	u8g2_SetFontDirection(u8g2,0);
	
	//u8g2_SetFont(u8g2, LaoWangChineseFont_16); 		//Ê¹Óƒ×”Â¶Â¨Ò¥×–Â¿â£¬Ï‚Ã¦Ê¹ÓƒU8G2Â¹Ù·Â½GB2312×–Â¿â£¬Ğ¡ÈÃÂ¿ÂµÂ¥Æ¬Â»ÃºÎÂ·Â¨Ê”Óƒ

	u8g2_DrawXBMP(u8g2,0,0,128,128,gImage_missw);

	u8g2_SendBuffer(u8g2);
}
