#ifndef _MYADC_H 
#define _MYADC_H

#include "stm32f10x_adc.h"

void  MyAdc_Init(void);
u16 Get_Adc(u8 ch);
u16 Get_Adc_Average(u8 ch,u8 times);
void  MyAdcA_Init(void);
#endif

