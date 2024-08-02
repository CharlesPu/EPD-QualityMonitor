/******************************************************************************/
#include "epaper.h"
#include "EPD_2IN9.h"
#include "EPD_2IN9BC.h"
#include "EPD_4in2bc.h"

#include "DEV_Config.h"
#include "GUI_Paint.h"
#include "Debug.h"
#include <stdlib.h> // malloc() free()
#include <stdint.h>

#ifdef EPD_2IN9BC
wave_t wave_temp;
wave_t wave_rh;
wave_t wave_co2;
wave_t wave_tvoc;

int EPD_test_2in9bc(void)
{
		printf("EPD_2IN9BC_test Demo\r\n");
//    DEV_Module_Init();

    printf("e-Paper Init and Clear...\r\n");
    EPD_2IN9BC_Init();
    EPD_2IN9BC_Clear();
    DEV_Delay_ms(500);

    //Create a new image cache named IMAGE_BW and fill it with white
    UBYTE *BlackImage, *RYImage; // Red or Yellow
#ifndef EPD_BUFF_STACK
    UWORD Imagesize = ((EPD_2IN9BC_WIDTH % 8 == 0)? (EPD_2IN9BC_WIDTH / 8 ): (EPD_2IN9BC_WIDTH / 8 + 1)) * EPD_2IN9BC_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        return -1;
    }
    if((RYImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for red memory...\r\n");
        return -1;
    }
#else	
		UBYTE DisplayBuf1[(EPD_2IN9BC_WIDTH / 8)*EPD_2IN9BC_HEIGHT]={0};
		BlackImage = DisplayBuf1;
		UBYTE DisplayBuf2[(EPD_2IN9BC_WIDTH / 8)*EPD_2IN9BC_HEIGHT]={0};
		RYImage = DisplayBuf2;
#endif	
		
    printf("NewImage:BlackImage and RYImage\r\n");
    Paint_NewImage(BlackImage, EPD_2IN9BC_WIDTH, EPD_2IN9BC_HEIGHT, 270, WHITE);
    Paint_NewImage(RYImage, EPD_2IN9BC_WIDTH, EPD_2IN9BC_HEIGHT, 270, WHITE);

    //Select Image
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
    Paint_SelectImage(RYImage);
    Paint_Clear(WHITE);

#if 1   // show image for array    
    printf("show image for array\r\n");
    EPD_2IN9BC_Display(gImage_2in9bc_b, gImage_2in9bc_ry);
    DEV_Delay_ms(2000);
#endif

#if 1   // Drawing on the image
    /*Horizontal screen*/
    //1.Draw black image
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
    Paint_DrawPoint(10, 80, BLACK, DOT_PIXEL_1X1, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 90, BLACK, DOT_PIXEL_2X2, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 100, BLACK, DOT_PIXEL_3X3, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 110, BLACK, DOT_PIXEL_3X3, DOT_STYLE_DFT);
    Paint_DrawLine(20, 70, 70, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawLine(70, 70, 20, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawRectangle(20, 70, 70, 120, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawRectangle(80, 70, 130, 120, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawString_EN(10, 0, "waveshare", &Font16, BLACK, WHITE);
    Paint_DrawString_CN(130, 20, "΢ѩ    ", &Font24CN, WHITE, BLACK);
    Paint_DrawNum(10, 50, 987654321, &Font16, WHITE, BLACK);

    //2.Draw red image
    Paint_SelectImage(RYImage);
    Paint_Clear(WHITE);
    Paint_DrawCircle(160, 95, 20, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawCircle(210, 95, 20, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawLine(85, 95, 125, 95, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawLine(105, 75, 105, 115, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawString_CN(130, 0,"   abc", &Font12CN, BLACK, WHITE);
    Paint_DrawString_EN(10, 20, "hello world", &Font12, WHITE, BLACK);
    Paint_DrawNum(10, 33, 123456789, &Font12, BLACK, WHITE);

    printf("EPD_Display\r\n");
    EPD_2IN9BC_Display(BlackImage, RYImage);
    DEV_Delay_ms(2000);
#endif

    printf("Clear...\r\n");
    EPD_2IN9BC_Clear();

    printf("Goto Sleep...\r\n");
    EPD_2IN9BC_Sleep();
    free(BlackImage);
    free(RYImage);
    BlackImage = NULL;
    RYImage = NULL;

    // close 5V
    printf("close 5V, Module enters 0 power consumption ...\r\n");
    DEV_Module_Exit();
    
    return 0;
}

int EPD_2in9bc_aqi(sgp30_data_t* sgp30_data, dht11_data_t* dht11_data)
{
	printf("EPD_2IN9BC air quality!\r\n");
//	DEV_Module_Init();

	EPD_2IN9BC_Init();
	//EPD_2IN9BC_Clear();
	DEV_Delay_ms(500);
	printf("e-Paper Init and Clear done\r\n");

	//Create a new image cache named IMAGE_BW and fill it with white
	UBYTE *BlackImage, *RYImage; // Red or Yellow
#ifndef EPD_BUFF_STACK
	UWORD Imagesize = ((EPD_2IN9BC_WIDTH % 8 == 0)? (EPD_2IN9BC_WIDTH / 8 ): (EPD_2IN9BC_WIDTH / 8 + 1)) * EPD_2IN9BC_HEIGHT;
	if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
			printf("Failed to apply for black memory...\r\n");
			return -1;
	}
	if((RYImage = (UBYTE *)malloc(Imagesize)) == NULL) {
			printf("Failed to apply for red memory...\r\n");
			return -1;
	}
#else	
	UBYTE DisplayBuf1[(EPD_2IN9BC_WIDTH / 8)*EPD_2IN9BC_HEIGHT]={0};
	BlackImage = DisplayBuf1;
	UBYTE DisplayBuf2[(EPD_2IN9BC_WIDTH / 8)*EPD_2IN9BC_HEIGHT]={0};
	RYImage = DisplayBuf2;
#endif	
	
//	printf("NewImage:BlackImage and RYImage\r\n");
	Paint_NewImage(BlackImage, EPD_2IN9BC_WIDTH, EPD_2IN9BC_HEIGHT, 270, WHITE);
	Paint_NewImage(RYImage, EPD_2IN9BC_WIDTH, EPD_2IN9BC_HEIGHT, 270, WHITE);

	//Select Image
	Paint_SelectImage(BlackImage);
	Paint_Clear(WHITE);
	Paint_SelectImage(RYImage);
	Paint_Clear(WHITE);

#if 0   // show image for array    
	printf("show image for array\r\n");
	EPD_2IN9BC_Display(gImage_aqi, gImage_aqi);
	DEV_Delay_ms(2000);
#endif

#if 1   // Drawing on the image
	/*Horizontal screen*/
	//1.Draw black image
	Paint_SelectImage(RYImage);
	Paint_Clear(WHITE);
	Paint_DrawBitMap(gImage_aqi);
	
	// ..............296.............
	
	// ..............................
	// 128...........................
	// ..............................
	// ..............................
	// (10,10)----------------------
	// |             ....           |
	//  --------------------(280,120)
//	Paint_DrawRectangle(5, 5, 290, 125, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY); // 外框
//	Paint_DrawLine(5, 65, 290, 65, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
//	Paint_DrawLine(147, 5, 147, 125, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
	
//	Paint_DrawString_EN(6, 20, "Temp: ", &Font20, WHITE, BLACK);
//	Paint_DrawString_EN(150, 20, "RH: ", &Font20, WHITE, BLACK);
//	Paint_DrawString_EN(6, 80, "CO2: ", &Font20, WHITE, BLACK);
//	Paint_DrawString_EN(150, 80, "TVOC: ", &Font20, WHITE, BLACK);

	//2.Draw red image
	Paint_SelectImage(BlackImage);
	Paint_Clear(WHITE);
	
	char buff1[10] = {0};
	sprintf(buff1, "%d.%d", dht11_data->temp_H, dht11_data->temp_L);
	Paint_DrawString_EN(50, 10, buff1, &Font24, WHITE, BLACK);
	char buff2[10] = {0};
	sprintf(buff2, "%d.%d", dht11_data->rh_H, dht11_data->rh_L);
	Paint_DrawString_EN(195, 10, buff2, &Font24, WHITE, BLACK);
	Paint_DrawNum(57, 75, sgp30_data->co2, &Font24, BLACK, WHITE);
	Paint_DrawNum(202, 75, sgp30_data->tvoc, &Font24, BLACK, WHITE);
	// waves
	EPD_wave_refreshPoints(EPD_wave_genPointDiff_Y(dht11_data->temp_H*10+dht11_data->temp_L, 200, 400),&wave_temp);
	EPD_wave_paint_dot(40,62,&wave_temp);
	
	EPD_wave_refreshPoints(EPD_wave_genPointDiff_Y(dht11_data->rh_H, 40, 80),&wave_rh);
	EPD_wave_paint_dot(187,62,&wave_rh);
	
	EPD_wave_refreshPoints(EPD_wave_genPointDiff_Y(sgp30_data->co2, 400, 700),&wave_co2);
	EPD_wave_paint_line(40,125,&wave_co2);
	
	EPD_wave_refreshPoints(EPD_wave_genPointDiff_Y(sgp30_data->tvoc, 0, 100),&wave_tvoc);
	EPD_wave_paint_line(187,125,&wave_tvoc);
	
	printf("EPD_Display\r\n");
	EPD_2IN9BC_Display(BlackImage, RYImage);
#endif

//	printf("Clear...\r\n");
//	EPD_2IN9BC_Clear();

	printf("Goto Sleep...\r\n");
	EPD_2IN9BC_Sleep();
#ifndef EPD_BUFF_STACK
	free(BlackImage);
	free(RYImage);
	BlackImage = NULL;
	RYImage = NULL;
#endif

	// close 5V
//	printf("close 5V, Module enters 0 power consumption ...\r\n");
//	DEV_Module_Exit();
	
	return 0;
}
#endif

////***************************************************************************************************************************///////////
////***************************************************************************************************************************///////////
#ifdef EPD_2IN9
int EPD_2in9_aqi(sgp30_data_t* sgp30_data, dht11_data_t* dht11_data){
	printf("EPD_2IN9_test Demo\r\n");
	//DEV_Module_Init();

	printf("e-Paper Init and Clear...\r\n");
	EPD_2IN9_Init(EPD_2IN9_FULL);
	//EPD_2IN9_Clear();
	DEV_Delay_ms(500);

	//Create a new image cache
	UBYTE *BlackImage;
#ifndef EPD_BUFF_STACK	
	/* you have to edit the startup_stm32fxxx.s file and set a big enough heap size */
	UWORD Imagesize = ((EPD_2IN9_WIDTH % 8 == 0)? (EPD_2IN9_WIDTH / 8 ): (EPD_2IN9_WIDTH / 8 + 1)) * EPD_2IN9_HEIGHT;
	if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
			printf("Failed to apply for black memory...\r\n");
			return -1;
	}
#else		
		UBYTE DisplayBuf1[(EPD_2IN9_WIDTH / 8)*EPD_2IN9_HEIGHT]={0};
		BlackImage = DisplayBuf1;
#endif		
	printf("Paint_NewImage\r\n");
	Paint_NewImage(BlackImage, EPD_2IN9_WIDTH, EPD_2IN9_HEIGHT, 270, WHITE);

#if 1   //show image for array    
	printf("show image for array\r\n");
	Paint_SelectImage(BlackImage);
	Paint_Clear(WHITE);
	Paint_DrawBitMap(gImage_aqi);

	EPD_2IN9_Display(BlackImage);
	DEV_Delay_ms(2000);
#endif

#if 1   // Drawing on the image
	printf("Drawing\r\n");
	//1.Select Image
	Paint_SelectImage(BlackImage);
	Paint_Clear(WHITE);

	// 2.Drawing on the image
	printf("Drawing:BlackImage\r\n");
	Paint_DrawPoint(10, 80, BLACK, DOT_PIXEL_1X1, DOT_STYLE_DFT);
	Paint_DrawPoint(10, 90, BLACK, DOT_PIXEL_2X2, DOT_STYLE_DFT);
	Paint_DrawPoint(10, 100, BLACK, DOT_PIXEL_3X3, DOT_STYLE_DFT);

	Paint_DrawLine(20, 70, 70, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
	Paint_DrawLine(70, 70, 20, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);

	Paint_DrawRectangle(20, 70, 70, 120, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
	Paint_DrawRectangle(80, 70, 130, 120, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);

	Paint_DrawCircle(45, 95, 20, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
	Paint_DrawCircle(105, 95, 20, WHITE, DOT_PIXEL_1X1, DRAW_FILL_FULL);

	Paint_DrawLine(85, 95, 125, 95, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
	Paint_DrawLine(105, 75, 105, 115, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);

	Paint_DrawString_EN(10, 0, "waveshare", &Font16, BLACK, WHITE);
	Paint_DrawString_EN(10, 20, "hello world", &Font12, WHITE, BLACK);

	Paint_DrawNum(10, 33, 123456789, &Font12, BLACK, WHITE);
	Paint_DrawNum(10, 50, 987654321, &Font16, WHITE, BLACK);

	Paint_DrawString_CN(130, 0,"   abc", &Font12CN, BLACK, WHITE);
	Paint_DrawString_CN(130, 20, "΢ѩ    ", &Font24CN, WHITE, BLACK);

	EPD_2IN9_Display(BlackImage);
	DEV_Delay_ms(2000);
#endif

#if 1   //Partial refresh, example shows time    		
	printf("Partial refresh\r\n");
	EPD_2IN9_Init(EPD_2IN9_PART);
	Paint_SelectImage(BlackImage);
	PAINT_TIME sPaint_time;
	sPaint_time.Hour = 12;
	sPaint_time.Min = 34;
	sPaint_time.Sec = 56;
	UBYTE num = 20;
	for (;;) {
			sPaint_time.Sec = sPaint_time.Sec + 1;
			if (sPaint_time.Sec == 60) {
					sPaint_time.Min = sPaint_time.Min + 1;
					sPaint_time.Sec = 0;
					if (sPaint_time.Min == 60) {
							sPaint_time.Hour =  sPaint_time.Hour + 1;
							sPaint_time.Min = 0;
							if (sPaint_time.Hour == 24) {
									sPaint_time.Hour = 0;
									sPaint_time.Min = 0;
									sPaint_time.Sec = 0;
							}
					}
			}
			Paint_ClearWindows(150, 80, 150 + Font20.Width * 7, 80 + Font20.Height, WHITE);
			Paint_DrawTime(150, 80, &sPaint_time, &Font20, WHITE, BLACK);

			num = num - 1;
			if(num == 0) {
					break;
			}
			EPD_2IN9_Display(BlackImage);
			DEV_Delay_ms(500);//Analog clock 1s
	}

#endif
//	printf("Clear...\r\n");
//	EPD_2IN9_Init(EPD_2IN9_FULL);
//	EPD_2IN9_Clear();

	printf("Goto Sleep...\r\n");
	EPD_2IN9_Sleep();
#ifndef EPD_BUFF_STACK	
	free(BlackImage);
	BlackImage = NULL;
#endif
	// close 5V
	//printf("close 5V, Module enters 0 power consumption ...\r\n");
	//DEV_Module_Exit();
	
	return 0;
}
#endif

////***************************************************************************************************************************///////////
////***************************************************************************************************************************///////////
#ifdef EPD_4IN2BC
int EPD_test_4in2bc(void)
{
	  printf("EPD_4IN2BC_test Demo\r\n");
//    DEV_Module_Init();

    printf("e-Paper Init and Clear...\r\n");
    EPD_4IN2BC_Init();
    EPD_4IN2BC_Clear();
    DEV_Delay_ms(500);

    //Create a new image cache named IMAGE_BW and fill it with white
    UBYTE *BlackImage, *RYImage; // Red or Yellow
#ifndef EPD_BUFF_STACK
    UWORD Imagesize = ((EPD_4IN2BC_WIDTH % 8 == 0)? (EPD_4IN2BC_WIDTH / 8 ): (EPD_4IN2BC_WIDTH / 8 + 1)) * EPD_4IN2BC_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        return -1;
    }
    if((RYImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for red memory...\r\n");
        return -1;
    }
#else		
		UBYTE DisplayBuf1[(EPD_4IN2BC_WIDTH / 8)*EPD_4IN2BC_HEIGHT]={0};
		BlackImage = DisplayBuf1;
		UBYTE DisplayBuf2[(EPD_4IN2BC_WIDTH / 8)*EPD_4IN2BC_HEIGHT]={0};
		RYImage = DisplayBuf2;
#endif		
		
    printf("NewImage:BlackImage and RYImage\r\n");
    Paint_NewImage(BlackImage, EPD_4IN2BC_WIDTH, EPD_4IN2BC_HEIGHT, 270, WHITE);
    Paint_NewImage(RYImage, EPD_4IN2BC_WIDTH, EPD_4IN2BC_HEIGHT, 270, WHITE);

    //Select Image
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
    Paint_SelectImage(RYImage);
    Paint_Clear(WHITE);

#if 1   // show image for array    
    printf("show image for array\r\n");
    EPD_4IN2BC_Display(gImage_4in2bc_b, gImage_4in2bc_ry);
    DEV_Delay_ms(2000);
#endif

#if 1   // Drawing on the image
    /*Horizontal screen*/
    //1.Draw black image
    printf("Draw black image\r\n");
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
    Paint_DrawPoint(10, 80, BLACK, DOT_PIXEL_1X1, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 90, BLACK, DOT_PIXEL_2X2, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 100, BLACK, DOT_PIXEL_3X3, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 110, BLACK, DOT_PIXEL_3X3, DOT_STYLE_DFT);
    Paint_DrawLine(20, 70, 70, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawLine(70, 70, 20, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);      
    Paint_DrawRectangle(20, 70, 70, 120, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawRectangle(80, 70, 130, 120, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawString_EN(10, 0, "waveshare", &Font16, BLACK, WHITE);    
    Paint_DrawString_CN(130, 20, "΢ѩ    ", &Font24CN, WHITE, BLACK);
    Paint_DrawNum(10, 50, 987654321, &Font16, WHITE, BLACK);
    
    //2.Draw red image
    printf("Draw red image\r\n");
    Paint_SelectImage(RYImage);
    Paint_Clear(WHITE);
    Paint_DrawCircle(160, 95, 20, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawCircle(210, 95, 20, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawLine(85, 95, 125, 95, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawLine(105, 75, 105, 115, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);  
    Paint_DrawString_CN(130, 0,"   abc", &Font12CN, BLACK, WHITE);
    Paint_DrawString_EN(10, 20, "hello world", &Font12, WHITE, BLACK);
    Paint_DrawNum(10, 33, 123456789, &Font12, BLACK, WHITE);

    printf("EPD_Display\r\n");
    EPD_4IN2BC_Display(BlackImage, RYImage);
    DEV_Delay_ms(2000);
#endif

    printf("Clear...\r\n");
    EPD_4IN2BC_Clear();

    printf("Goto Sleep...\r\n");
    EPD_4IN2BC_Sleep();
#ifndef EPD_BUFF_STACK
		free(BlackImage);
		free(RYImage);
		BlackImage = NULL;
		RYImage = NULL;
#endif

    // close 5V
    printf("close 5V, Module enters 0 power consumption ...\r\n");
    DEV_Module_Exit();
    
    return 0;
}
#endif


int EPD_wave_genPointDiff_Y(int data, int from, int to) {
	if (data >= to) {
		return EPD_WAVE_H;
	}else if (data <= from){
		return 0;
	}
	int interval = to - from;
	return EPD_WAVE_H*(data-from)/interval;
}

int EPD_wave_refreshPoints(int point, wave_t* w){
	for(int i = 0; i < wave_points_len; i++) {
		printf("%d ", w->wave_points[i]);
	}
	printf(" %d\r\n", w->wave_points_idx_next);
	
	if (w->wave_points_idx_next <= wave_points_len-1) {
		w->wave_points[ w->wave_points_idx_next] = point;
		w->wave_points_idx_next++;
		return 0;
	}
	for(int i = 0; i < wave_points_len-1; i++) {
		w->wave_points[i] = w->wave_points[i+1];
	}
	w->wave_points[wave_points_len-1] = point;
	
	return 0;
}

int EPD_wave_paint_dot(UWORD x, UWORD y, wave_t* w){
	for(int i = 0; i < w->wave_points_idx_next; i++) {
		printf("paint %d %d\r\n",x+i*EPD_WAVE_W_step, y-w->wave_points[i]);
		Paint_DrawPoint(x+i*EPD_WAVE_W_step, y-w->wave_points[i], BLACK, DOT_PIXEL_2X2, DOT_STYLE_DFT);
	}
	return 0;
}

int EPD_wave_paint_line(UWORD x, UWORD y, wave_t* w){
	for(int i = 0; i < w->wave_points_idx_next; i++) {
		printf("paint %d %d\r\n",x+i*EPD_WAVE_W_step, y-w->wave_points[i]);
		Paint_DrawLine(x+i*EPD_WAVE_W_step, y, x+i*EPD_WAVE_W_step, y-w->wave_points[i], BLACK, DOT_PIXEL_2X2, LINE_STYLE_SOLID);
	}
	return 0;
}
