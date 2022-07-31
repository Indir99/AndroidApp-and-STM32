#ifndef __MISC_H_
#define __MISC_H_

#include "stm32f4xx.h"

#define MAX_PRINT_STRING_SIZE					1024

#define PRINT_ARG_TYPE_BINARY_BYTE				0x0001	
#define PRINT_ARG_TYPE_BINARY_HALFWORD			0x0002	
#define PRINT_ARG_TYPE_BINARY_WORD				0x0004			
#define PRINT_ARG_TYPE_DECIMAL_BYTE				0x0008		
#define PRINT_ARG_TYPE_DECIMAL_HALFWORD			0x0010		
#define PRINT_ARG_TYPE_DECIMAL_WORD				0x0020		
#define PRINT_ARG_TYPE_CHARACTER				0x0040			
#define PRINT_ARG_TYPE_STRING					0x0080			
#define PRINT_ARG_TYPE_FLOAT					0x0100			
#define PRINT_ARG_TYPE_HEXADECIMAL_BYTE			0x0200		
#define PRINT_ARG_TYPE_HEXADECIMAL_HALFWORD		0x0400		
#define PRINT_ARG_TYPE_HEXADECIMAL_WORD			0x0800
#define PRINT_ARG_TYPE_UNKNOWN					0x0000

#define PRINT_ARG_TYPE_MASK_CHAR_STRING			~((PRINT_ARG_TYPE_CHARACTER)|(PRINT_ARG_TYPE_STRING))


void getDNumMISC(uint8_t * dnum, uint32_t num);
void putDNumMISC(uint8_t * dnum, uint16_t * m, uint8_t * r_str);
void getStr4NumMISC(uint16_t type, uint32_t  * num, uint8_t * r_str);

#endif 
