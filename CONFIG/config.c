#include "config.h"
#include "delay.h"

//外设 m--》machien
Gpio_pin_parameter m_InfraredLeft_Port,m_InfraredMiddle_Port,m_InfraredRight_Port; 
Gpio_pin_parameter m_Relay_Port;
Gpio_pin_parameter m_Key1_Port,m_Key2_Port;
Gpio_pin_parameter m_CurrentFlag_Port; 

//外设引脚设置定义
void ChargeBasePinDefine(void)
{
	//红外
	m_InfraredLeft_Port.c_gpio = GPIOC; 
	m_InfraredLeft_Port.c_pin = GPIO_Pin_15;
	m_InfraredMiddle_Port.c_gpio = GPIOC;
	m_InfraredMiddle_Port.c_pin = GPIO_Pin_13;
	m_InfraredRight_Port.c_gpio = GPIOC;
	m_InfraredRight_Port.c_pin = GPIO_Pin_14;
	//继电器设置
	m_Relay_Port.c_gpio = GPIOB;
	m_Relay_Port.c_pin = GPIO_Pin_14;
	//开关设置
	m_Key1_Port.c_gpio =  GPIOA;
	m_Key1_Port.c_pin = GPIO_Pin_6;
	m_Key2_Port.c_gpio =  GPIOA;
	m_Key2_Port.c_pin = GPIO_Pin_0;
	
	//充电头告知控制板状态信息
	m_CurrentFlag_Port.c_gpio = GPIOB;
	m_CurrentFlag_Port.c_pin = GPIO_Pin_9;
	
	pinOutputModeInit(&m_Relay_Port); 
	//红外初始化
	pinOutputModeInit(&m_InfraredLeft_Port);
	pinOutputModeInit(&m_InfraredMiddle_Port);
	pinOutputModeInit(&m_InfraredRight_Port);
	//按键初始化
	pinInputModeInit(&m_Key1_Port);
	pinInputModeInit(&m_Key2_Port);
	pinOutputModeInit(&m_CurrentFlag_Port);
}


void pinInputModeInit(Gpio_pin_parameter* Gpio_pin_para)
{
	GPIO_InitTypeDef GPIO_InitStructrue;
	if(Gpio_pin_para->c_gpio == GPIOA)
		RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA,ENABLE);
	else if(Gpio_pin_para->c_gpio == GPIOB)
		RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOB,ENABLE);
	else if(Gpio_pin_para->c_gpio == GPIOC)
		RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOC,ENABLE);
	else if(Gpio_pin_para->c_gpio == GPIOD)
		RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOD,ENABLE);
	else if(Gpio_pin_para->c_gpio == GPIOE)
		RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOE,ENABLE);
	else if(Gpio_pin_para->c_gpio == GPIOF)
		RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOF,ENABLE);
	else if(Gpio_pin_para->c_gpio == GPIOG)
		RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOG,ENABLE);
	
	GPIO_InitStructrue.GPIO_Pin = Gpio_pin_para->c_pin;
	GPIO_InitStructrue.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_InitStructrue.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(Gpio_pin_para->c_gpio,&GPIO_InitStructrue);   //g根据设定参数配置GPIO
}


void pinOutputModeInit(Gpio_pin_parameter* Gpio_pin_para)
{
	GPIO_InitTypeDef GPIO_InitStructrue;
	if(Gpio_pin_para->c_gpio == GPIOA)
		RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA,ENABLE);
	else if(Gpio_pin_para->c_gpio == GPIOB)
		RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOB,ENABLE);
	else if(Gpio_pin_para->c_gpio == GPIOC)
		RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOC,ENABLE);
	else if(Gpio_pin_para->c_gpio == GPIOD)
		RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOD,ENABLE);
	else if(Gpio_pin_para->c_gpio == GPIOE)
		RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOE,ENABLE);
	else if(Gpio_pin_para->c_gpio == GPIOF)
		RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOF,ENABLE);
	else if(Gpio_pin_para->c_gpio == GPIOG)
		RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOG,ENABLE);
	
	GPIO_InitStructrue.GPIO_Pin = Gpio_pin_para->c_pin;
	GPIO_InitStructrue.GPIO_Mode = GPIO_Mode_Out_PP; //通用推挽输出模式
	GPIO_InitStructrue.GPIO_Speed = GPIO_Speed_50MHz;//输出模式最大速度
	GPIO_Init(Gpio_pin_para->c_gpio,&GPIO_InitStructrue);//g根据设定参数配置GPIO
}

void pinDigitalWrite( Gpio_pin_parameter* Gpio_pin_para, boolean fal )
{
	if( fal == false )
		GPIO_ResetBits(Gpio_pin_para->c_gpio,Gpio_pin_para->c_pin);
	else if( fal == true )
		GPIO_SetBits(Gpio_pin_para->c_gpio,Gpio_pin_para->c_pin);
	else
		GPIO_ResetBits(Gpio_pin_para->c_gpio,Gpio_pin_para->c_pin);
}

uint8_t pinDigitalRead( Gpio_pin_parameter* Gpio_pin_para )
{
	return GPIO_ReadInputDataBit( Gpio_pin_para->c_gpio, Gpio_pin_para->c_pin );
}

//系统时钟设置 SYSCLK AHB APB2 PLL为72M APB1为36M
void RCC_SystemClkInit(void)
{
	RCC_DeInit();              
	RCC_HSEConfig(RCC_HSE_ON);   
	while(RCC_WaitForHSEStartUp() != SUCCESS);
	FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable); 
	FLASH_SetLatency(FLASH_Latency_2);
	RCC_HCLKConfig(RCC_SYSCLK_Div1); //HCLK = SYSCLK 
	RCC_PCLK2Config(RCC_HCLK_Div1);  //PCLK2 = HCLK
	RCC_PCLK1Config(RCC_HCLK_Div2);  //PCLK1 = HCLK/2 
	RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);//PLLCLK = 8MHz * 9 = 72 MHz 
	RCC_PLLCmd(ENABLE);
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    while(RCC_GetSYSCLKSource() != 0x08);
}

void MySystem_init(void)
{
	RCC_SystemClkInit();	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
}

