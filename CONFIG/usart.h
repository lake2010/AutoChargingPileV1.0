#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 
#include "stdarg.h"
#include "commen.h"

void ConfigClass_USART1Init(uint32_t baud);
void ConfigClass_USART3Init(uint32_t baud);
void myprintfUSART1( const char* fmt, ... );
void myprintfUSART3( const char* fmt, ... );
void mySerialWriteUSART1( const byte* sendBuff, int sendLen );
void mySerialWriteUSART3( const byte* sendBuff, int sendLen );
#endif


