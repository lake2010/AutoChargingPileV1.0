#include "ChargeBase.h"
#include "config.h"
#include "commen.h"
#include "usart.h"
#include "delay.h"
#include "myadc.h"
#include "stm32f10x.h"
#include "delay.h"
#include "Led.h"

CChargeBase_parameter ChargeBase;//充电桩状态量总定义
static void TIM1_GPIO_Config(void) 
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void CChargeBase_PWMStatus(u8 CChargeBase_TimerPWMStatus)
{
	if(CChargeBase_TimerPWMStatus == TIMER1_PWM_STATUS_ON)
	{
		TIM_CtrlPWMOutputs(TIM1, ENABLE);
	}
	else
	{
		TIM_CtrlPWMOutputs(TIM1, DISABLE);
		GPIO_WriteBit(GPIOB,GPIO_Pin_15,TIMER1_PWM_STATUS_OFF);
	}
}

static void TIM1_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;	
	TIM_TimeBaseStructure.TIM_Period = 1871;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);	
	TIM_ARRPreloadConfig(TIM1, ENABLE);
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
	TIM_Cmd(TIM1, ENABLE);
	CChargeBase_PWMStatus(TIMER1_PWM_STATUS_OFF);
}

void CChargeBaseInfrared_init(void)
{
	TIM1_GPIO_Config();
	TIM1_Mode_Config();	
}

void Timer1_PWM_SetDutyCycle(u8 Timer1_PWM_DutyCycle)
{
	TIM_SetCompare3(TIM1,Timer1_PWM_DutyCycle *1872 /100);
}

void CChargeBase_InfraredSendStatus(u8 CChargeBase_InfraredSendStatus)
{
	if(CChargeBase_InfraredSendStatus == INFRARED_SEND_STATUS_ON)
	{
		CChargeBase_PWMStatus(TIMER1_PWM_STATUS_ON);
	}
	else
	{
		CChargeBase_PWMStatus(TIMER1_PWM_STATUS_OFF);
	}
}
void CChargeBase_InfraredSend(u32 CChargeBase_Data)  
{  
	u8 i = 0;   
	Timer1_PWM_SetDutyCycle(50);  
	CChargeBase_InfraredSendStatus(INFRARED_SEND_STATUS_OFF);    	
	//9ms起始信号低电平  
	CChargeBase_InfraredSendStatus(INFRARED_SEND_STATUS_ON);  
	delay50_us(180);
	//4.5ms起始信号高电平
	CChargeBase_InfraredSendStatus(INFRARED_SEND_STATUS_OFF);  
	delay50_us(90);
	//循环发送32bit的数据  
	for(i = 0; i < 32; i++)  
	{  
		//通用的0.56ms低电平  
		CChargeBase_InfraredSendStatus(INFRARED_SEND_STATUS_ON);  
		delay50_us(11); 
		//数值0  
		if((CChargeBase_Data & 0x80000000) == 0)  
		{  
			//0.565ms高电平  
			CChargeBase_InfraredSendStatus(INFRARED_SEND_STATUS_OFF); 
			delay50_us(11);  
		}  
		//数值1  
		else  
		{  
			//1.69ms高电平
			CChargeBase_InfraredSendStatus(INFRARED_SEND_STATUS_OFF); 
			delay50_us(34);
		}  
		CChargeBase_Data <<= 1;  
	}
//	0.263ms低电平作为结束位  
	CChargeBase_InfraredSendStatus(INFRARED_SEND_STATUS_ON);  
	delay50_us(5);   
//	关闭红外  
	CChargeBase_InfraredSendStatus(INFRARED_SEND_STATUS_OFF);    
} 

void CChargeBase_JudgeKeyState(char* flag,GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	if(!(*flag))
	{
		if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == 0)
		{
			delay_ms(10);
			if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == 0)
				*flag = 1;
		}
	}
	else
	{
		if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) != 0)
		{
			delay_ms(10);
			if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) != 0)
				*flag = 0;
		}
	}
}

void AutoCharging_loop(CChargeBase_parameter *CChargeBase_para)
{
	static u8 c;
	CChargeBase_JudgeKeyState(&(CChargeBase_para->m_SYSstatus.key1_status),m_Key1_Port.c_gpio,m_Key1_Port.c_pin);
	CChargeBase_JudgeKeyState(&(CChargeBase_para->m_SYSstatus.key2_status),m_Key2_Port.c_gpio,m_Key2_Port.c_pin);
	if(CChargeBase_para->m_SYSstatus.key1_status || CChargeBase_para->m_SYSstatus.key2_status)
	{
		CChargeBase_para->m_LED.m_chargeLedBlink = true;
		GPIO_WriteBit(m_Relay_Port.c_gpio,m_Relay_Port.c_pin,Bit_SET);
	}
	//激光发射非充电状态
	else
	{
		GPIO_WriteBit(m_Relay_Port.c_gpio,m_Relay_Port.c_pin,Bit_RESET);
		switch(c)
		{
			case 0:
				if(millis() - CChargeBase_para->m_SYSstatus.tim2 > 100)
				{
					GPIO_WriteBit(m_InfraredLeft_Port.c_gpio,m_InfraredLeft_Port.c_pin,Bit_SET);//left
					GPIO_WriteBit(m_InfraredMiddle_Port.c_gpio,m_InfraredMiddle_Port.c_pin,Bit_RESET);//middle
					GPIO_WriteBit(m_InfraredRight_Port.c_gpio,m_InfraredRight_Port.c_pin,Bit_RESET);//right
					CChargeBase_InfraredSend(0x406F6F63);
					c = 1;
				}
				break;
			case 1:
				if(millis() - CChargeBase_para->m_SYSstatus.tim2 > 200)
				{
					GPIO_WriteBit(m_InfraredLeft_Port.c_gpio,m_InfraredLeft_Port.c_pin,Bit_RESET);
					GPIO_WriteBit(m_InfraredMiddle_Port.c_gpio,m_InfraredMiddle_Port.c_pin,Bit_SET);
					GPIO_WriteBit(m_InfraredRight_Port.c_gpio,m_InfraredRight_Port.c_pin,Bit_RESET);
					CChargeBase_InfraredSend(0x426F6F63);
					c = 2;
				}
				break;
			case 2:
				if(millis() - CChargeBase_para->m_SYSstatus.tim2 > 300)
				{
					CChargeBase_para->m_SYSstatus.tim2 = millis();
					GPIO_WriteBit(m_InfraredLeft_Port.c_gpio,m_InfraredLeft_Port.c_pin,Bit_RESET);
					GPIO_WriteBit(m_InfraredMiddle_Port.c_gpio,m_InfraredMiddle_Port.c_pin,Bit_RESET);
					GPIO_WriteBit(m_InfraredRight_Port.c_gpio,m_InfraredRight_Port.c_pin,Bit_SET);
					CChargeBase_InfraredSend(0x446F6F63);
					c = 0;
				}
				break;
		}
	}
}



void AutoCharging_setup(CChargeBase_parameter *CChargeBase_para)
{
	delay_init();
	ChargeBasePinDefine();
	ConfigClass_USART1Init(UART1_BAUD); //USART1_TX GPIOA.9 USART1_RX   GPIOA.10
	CChargeBaseInfrared_init();	
	CLED_setup( &CChargeBase_para->m_LED, &m_CurrentFlag_Port );
	CChargeBase_IWDGInit(4,3125);//与分频数为64,重载值为625,溢出时间为1s	
	//TIM3_PWM_Init(1871,0);	                                            //不分频。PWM频率=72000000/900=80Khz	重映射到B5 不分频频率是38K1871 
	//MyAdc_Init();															//PA1引脚作为AD输入
}


