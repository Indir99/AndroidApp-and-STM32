#ifndef __DELAY_H
#define __DELAY_H

#include "stm32f4xx.h"

#define SYSTIMER_TIMEOUT						0
#define SYSTIMER_KEEP_ALIVE						1

void 		delay_ms(uint32_t ms);				// 													
void 		delay_us(uint32_t ms);				// 


void 		initSTOPWATCH(void);
void 		startSTOPWATCH(void);
uint32_t 	stopSTOPWATCH(void);

void 		initSYSTIMER(void);
uint32_t 	getSYSTIMER(void);
uint8_t 	chk4TimeoutSYSTIMER(uint32_t btime, uint32_t period);

#endif 
