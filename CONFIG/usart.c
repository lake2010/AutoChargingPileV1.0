#include "sys.h"
#include "usart.h"	  
void ConfigClass_USART1Init(uint32_t baud)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
	//USART1_TX   GPIOA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9
   
	//USART1_RX	  GPIOA.10初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10  

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置 
	USART_InitStructure.USART_BaudRate = baud;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

	USART_Init(USART1, &USART_InitStructure); //初始化串口1
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接受中断
	USART_Cmd(USART1, ENABLE);                    //使能串口1 
}

//调试串口
void ConfigClass_USART3Init(uint32_t baud)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//使能USART1，GPIOA时钟
	//USART1_TX   GPIOA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOA.9
   
	//USART1_RX	  GPIOA.10初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOA.10  

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置 
	USART_InitStructure.USART_BaudRate = baud;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

	USART_Init(USART3, &USART_InitStructure); //初始化串口1
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启串口接受中断
	USART_Cmd(USART3, ENABLE);                    //使能串口1 
}
uint16_t ConfigClass_readUSART(USART_TypeDef* USARTx)
{

	if( USART_GetFlagStatus(USARTx,USART_FLAG_RXNE))
		return USART_ReceiveData(USARTx);
	else
		return 0xFFFF;
}
void USART1_IRQHandler(void)
{
	if (USART_GetFlagStatus(USART1, USART_FLAG_PE) != RESET){
		USART_ReceiveData(USART1);
		USART_ClearFlag(USART1, USART_FLAG_PE);
	}  
	if (USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET){
		USART_ReceiveData(USART1);
		USART_ClearFlag(USART1, USART_FLAG_ORE);
	}
	if (USART_GetFlagStatus(USART1, USART_FLAG_FE) != RESET){
		USART_ReceiveData(USART1);
		USART_ClearFlag(USART1, USART_FLAG_FE);
	}
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){
		//ConfigClass_readUSART(USART1);
		//CMySerial_addCmdtoOrignal_buff(&ChargeBase.m_ccm, ConfigClass_readUSART(USART1));
		USART_ClearFlag(USART1, USART_FLAG_RXNE);
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}

/*对外串口*/
void USART3_IRQHandler(void)
{
	if (USART_GetFlagStatus(USART3, USART_FLAG_PE) != RESET){
		USART_ReceiveData(USART3);
		USART_ClearFlag(USART3, USART_FLAG_PE);
	}  
	if (USART_GetFlagStatus(USART3, USART_FLAG_ORE) != RESET){
		USART_ReceiveData(USART3);
		USART_ClearFlag(USART3, USART_FLAG_ORE);
	}
	if (USART_GetFlagStatus(USART3, USART_FLAG_FE) != RESET){
		USART_ReceiveData(USART3);
		USART_ClearFlag(USART3, USART_FLAG_FE);
	}
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET){
		//CMySerial_addCmdtoOrignal_buff(&ChargeBase.m_ccm, ConfigClass_readUSART(USART3));
//		m_message.m_transmissionBuffStruct.m_UWBUARTtoWIFIandW5500.m_currentReceiveTime = millis();
		USART_ClearFlag(USART3, USART_FLAG_RXNE);
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
	}
}

void myprintfUSART1( const char* fmt, ... )
{
	static char buf[256];
	va_list ap;
	va_start( ap, fmt );
	vsprintf( buf, fmt, ap );
	va_end( ap );
	for(int i = 0; i < strlen(buf); i++){
		USART_SendData(USART1, buf[i]);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);		
	}
}

void myprintfUSART3( const char* fmt, ... )
{
	static char buf[256];
	va_list ap;
	va_start( ap, fmt );
	vsprintf( buf, fmt, ap );
	va_end( ap );
	for(int i = 0; i < strlen(buf); i++){
		USART_SendData(USART3, buf[i]);
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);		
	}
}

void mySerialWriteUSART1( const byte* sendBuff, int sendLen )
{
	for( int i = 0; i < sendLen; i++ ){
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
		USART_SendData(USART1, sendBuff[i]);		
	}
}
void mySerialWriteUSART3( const byte* sendBuff, int sendLen )
{
	for( int i = 0; i < sendLen; i++ ){
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
		USART_SendData(USART3, sendBuff[i]);		
	}
}
