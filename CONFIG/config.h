#ifndef _CONFIG_H
#define _CONFIG_H

#include "stm32f10x.h"
#include "commen.h"
#include "SYSstatus.h"



//充电桩各模块对应配置

//gpio struct
typedef struct Gpio_pin_parameter_all
{
	GPIO_TypeDef* c_gpio;
	uint16_t c_pin;
}Gpio_pin_parameter;


extern Gpio_pin_parameter m_InfraredLeft_Port,m_InfraredMiddle_Port,m_InfraredRight_Port; 
extern Gpio_pin_parameter m_Relay_Port;
extern Gpio_pin_parameter m_Key1_Port,m_Key2_Port;
extern Gpio_pin_parameter m_CurrentFlag_Port; 


void MySystem_init(void);      //系统设置
void ChargeBasePinDefine(void);//充电桩引脚定义
void pinOutputModeInit(Gpio_pin_parameter* Gpio_pin_para);//输出引脚定义模式
void pinInputModeInit(Gpio_pin_parameter* Gpio_pin_para);
void pinDigitalWrite( Gpio_pin_parameter* Gpio_pin_para, boolean fal );
uint8_t pinDigitalRead( Gpio_pin_parameter* Gpio_pin_para );
#endif


