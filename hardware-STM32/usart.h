#ifndef __USART2_H_
#define __USART2_H_

#include <stdio.h>
#include <stdarg.h>
#include "stm32f4xx.h"
#include "misc.h"
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

// Define for USART2
#define USART2_BUFFER_SIZE 512
#define USART2_BAUDRATE_921600 0x0000002D
#define USART2_BAUDRATE_460800 0x0000005B
#define USART2_BAUDRATE_115200 0x0000016C
#define USART2_BAUDRATE_9600 0x00001117

// Define for USART3
#define USART3_BUFFER_SIZE 512
#define USART3_BAUDRATE_921600 0x0000002D
#define USART3_BAUDRATE_460800 0x0000005B
#define USART3_BAUDRATE_115200 0x0000016C
#define USART3_BAUDRATE_9600 0x00001117

// functions for USART2 print/read
void initUSART2(uint32_t baudrate);
void putcharUSART2(uint8_t data);
void printUSART2(char *str, ...);
void sprintUSART2(uint8_t *str);
uint8_t getcharUSART2();

// USART 2 Interrupts:
void chkRxBuffUSART2(void);
void enIrqUSART2(void);
void disIrqUSART2(void);

extern volatile uint8_t g_usart2_buffer[USART2_BUFFER_SIZE];
extern volatile uint16_t g_usart2_widx;
extern volatile uint16_t g_usart2_ridx;

// functions for usart3
void initUSART3(uint32_t baudrate);
void putcharUSART3(uint8_t data);
void printUSART3(char *str, ...);
void sprintUSART3(uint8_t *str);
uint8_t getcharUSART3(void);

void chkRxBuffUSART3(void);
void enIrqUSART3(void);

void SplitString(char c, char *toSplit);

extern volatile uint8_t g_usart3_buffer[USART3_BUFFER_SIZE];
extern volatile uint16_t g_usart3_widx;
extern volatile uint16_t g_usart3_ridx;

#endif