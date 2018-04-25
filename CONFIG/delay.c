#include "delay.h"
#include "usart.h"

static volatile u32 MillisTime = 0;

void delay_init()
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//选择外部时钟  HCLK/8
	SysTick->VAL = 0x00;
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;   	//开启SYSTICK中断
	SysTick->LOAD=SystemCoreClock/8000000;  	//每1us中断一次	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;   	//开启SYSTICK   
	NVIC_InitStructure.NVIC_IRQChannel = SysTick_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}			

void delay_ms(u32 delayms)
{
	u32 StartTime  = millis();
	while(millis() - StartTime <= delayms);
}
void delay_us(u32 delayus)
{
	u32 StartTime = MillisTime;
	while(MillisTime - StartTime <= delayus);
}

void delay50_us(u32 delayus)
{
	u32 StartTime = MillisTime;
	while(MillisTime - StartTime <= delayus * 50);
}


void SysTick_Handler(void)
{
	MillisTime++;
}

u32 millis(void)
{
	return MillisTime/1000;
}


