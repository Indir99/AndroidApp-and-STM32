#include "usart.h"

//************************************************ USART2 **************************

volatile uint8_t g_usart2_buffer[USART2_BUFFER_SIZE];
volatile uint16_t g_usart2_widx = 0;
volatile uint16_t g_usart2_ridx = 0;

void initUSART2(uint32_t baudrate)
{
	// wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww
	//  USART2: PA2 -> TX & PA3 -> RX
	//------------------------------------------------------------------

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA->MODER &= ~((GPIO_MODER_MODER2) | (GPIO_MODER_MODER3));
	GPIOA->MODER |= (GPIO_MODER_MODER2_1) | (GPIO_MODER_MODER3_1);
	GPIOA->AFR[0] |= 0x00007700;

	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	USART2->BRR = baudrate;
	USART2->CR1 = USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;
}

uint8_t getcharUSART2()
{ /// print one character to USART2
	while (!(USART2->SR & USART_SR_RXNE))
		;
	return USART2->DR;
}

void putcharUSART2(uint8_t data)
{ /// print one character to USART2

	while (!(USART2->SR & USART_SR_TC))
		;
	USART2->DR = data;
}

void printUSART2(char *str, ...)
{					  /// print text and up to 10 arguments!
	uint8_t rstr[40]; // 33 max -> 32 ASCII for 32 bits and NULL
	uint16_t k = 0;
	uint16_t arg_type;
	uint32_t utmp32;
	uint32_t *p_uint32;
	char *p_char;
	va_list vl;

	// va_start(vl, 10);													// always pass the last named parameter to va_start, for compatibility with older compilers
	va_start(vl, str); // always pass the last named parameter to va_start, for compatibility with older compilers
	while (str[k] != 0x00)
	{
		if (str[k] == '%')
		{
			if (str[k + 1] != 0x00)
			{
				switch (str[k + 1])
				{
				case ('b'):
				{ // binary
					if (str[k + 2] == 'b')
					{ // byte
						utmp32 = va_arg(vl, int);
						arg_type = (PRINT_ARG_TYPE_BINARY_BYTE);
					}
					else if (str[k + 2] == 'h')
					{ // half word
						utmp32 = va_arg(vl, int);
						arg_type = (PRINT_ARG_TYPE_BINARY_HALFWORD);
					}
					else if (str[k + 2] == 'w')
					{ // word
						utmp32 = va_arg(vl, uint32_t);
						arg_type = (PRINT_ARG_TYPE_BINARY_WORD);
					}
					else
					{ // default word
						utmp32 = va_arg(vl, uint32_t);
						arg_type = (PRINT_ARG_TYPE_BINARY_WORD);
						k--;
					}

					k++;
					p_uint32 = &utmp32;
					break;
				}
				case ('d'):
				{ // decimal
					if (str[k + 2] == 'b')
					{ // byte
						utmp32 = va_arg(vl, int);
						arg_type = (PRINT_ARG_TYPE_DECIMAL_BYTE);
					}
					else if (str[k + 2] == 'h')
					{ // half word
						utmp32 = va_arg(vl, int);
						arg_type = (PRINT_ARG_TYPE_DECIMAL_HALFWORD);
					}
					else if (str[k + 2] == 'w')
					{ // word
						utmp32 = va_arg(vl, uint32_t);
						arg_type = (PRINT_ARG_TYPE_DECIMAL_WORD);
					}
					else
					{ // default word
						utmp32 = va_arg(vl, uint32_t);
						arg_type = (PRINT_ARG_TYPE_DECIMAL_WORD);
						k--;
					}

					k++;
					p_uint32 = &utmp32;
					break;
				}
				case ('c'):
				{ // character
					char tchar = va_arg(vl, int);
					putcharUSART2(tchar);
					arg_type = (PRINT_ARG_TYPE_CHARACTER);
					break;
				}
				case ('s'):
				{ // string
					p_char = va_arg(vl, char *);
					sprintUSART2((uint8_t *)p_char);
					arg_type = (PRINT_ARG_TYPE_STRING);
					break;
				}
				case ('f'):
				{											// float
					uint64_t utmp64 = va_arg(vl, uint64_t); // convert double to float representation IEEE 754
					uint32_t tmp1 = utmp64 & 0x00000000FFFFFFFF;
					tmp1 = tmp1 >> 29;
					utmp32 = utmp64 >> 32;
					utmp32 &= 0x07FFFFFF;
					utmp32 = utmp32 << 3;
					utmp32 |= tmp1;
					if (utmp64 & 0x8000000000000000)
						utmp32 |= 0x80000000;

					if (utmp64 & 0x4000000000000000)
						utmp32 |= 0x40000000;

					p_uint32 = &utmp32;

					arg_type = (PRINT_ARG_TYPE_FLOAT);
					// arg_type = (PRINT_ARG_TYPE_HEXADECIMAL_WORD);
					// arg_type = (PRINT_ARG_TYPE_BINARY_WORD);
					break;
				}
				case ('x'):
				{ // hexadecimal
					if (str[k + 2] == 'b')
					{ // byte
						utmp32 = (uint32_t)va_arg(vl, int);
						arg_type = (PRINT_ARG_TYPE_HEXADECIMAL_BYTE);
					}
					else if (str[k + 2] == 'h')
					{ // half word
						utmp32 = (uint32_t)va_arg(vl, int);
						arg_type = (PRINT_ARG_TYPE_HEXADECIMAL_HALFWORD);
					}
					else if (str[k + 2] == 'w')
					{ // word
						utmp32 = va_arg(vl, uint32_t);
						arg_type = (PRINT_ARG_TYPE_HEXADECIMAL_WORD);
					}
					else
					{
						utmp32 = va_arg(vl, uint32_t);
						arg_type = (PRINT_ARG_TYPE_HEXADECIMAL_WORD);
						k--;
					}

					k++;
					p_uint32 = &utmp32;
					break;
				}
				default:
				{
					utmp32 = 0;
					p_uint32 = &utmp32;
					arg_type = (PRINT_ARG_TYPE_UNKNOWN);
					break;
				}
				}

				if (arg_type & (PRINT_ARG_TYPE_MASK_CHAR_STRING))
				{
					getStr4NumMISC(arg_type, p_uint32, rstr);
					sprintUSART2(rstr);
				}
				k++;
			}
		}
		else
		{ // not a '%' char -> print the char
			putcharUSART2(str[k]);
			if (str[k] == '\n')
				putcharUSART2('\r');
		}
		k++;
	}

	va_end(vl);
	return;
}

void sprintUSART2(uint8_t *str)
{
	uint16_t k = 0;

	while (str[k] != '\0')
	{
		putcharUSART2(str[k]);
		if (str[k] == '\n')
			putcharUSART2('\r');
		k++;

		if (k == MAX_PRINT_STRING_SIZE)
			break;
	}
}

void enIrqUSART2(void)
{
	USART2->CR1 &= ~(USART_CR1_UE);

	NVIC_EnableIRQ(USART2_IRQn);
	USART2->CR1 |= (USART_CR1_UE) | (USART_CR1_RE) | (USART_CR1_RXNEIE);
}

void disIrqUSART2(void)
{
	USART2->CR1 &= ~((USART_CR1_UE) | (USART_CR1_RXNEIE));

	NVIC_DisableIRQ(USART2_IRQn);
	USART2->CR1 |= (USART_CR1_UE);
}

#ifdef USART_ECHO
void USART2_IRQHandler(void)
{
	uint8_t data;

	if (USART2->SR & (USART_SR_RXNE))
	{
		data = USART2->DR;
		// USART3->SR &= ~(USART_SR_RXNE);
		USART2->DR = data;
	}
}
#else

void USART2_IRQHandler(void)
{

	if (USART2->SR & (USART_SR_RXNE))
	{
		g_usart2_buffer[g_usart2_widx] = USART2->DR;
		g_usart2_widx++;
		if (g_usart2_widx >= (USART2_BUFFER_SIZE))
		{
			g_usart2_widx = 0;
		}
	}
}

#endif
void chkRxBuffUSART2(void)
{

	if (g_usart2_ridx != g_usart2_widx)
	{
		if ((g_usart2_buffer[g_usart2_widx - 1] == '#'))
		{
			for (int i = g_usart2_ridx; i < g_usart2_widx - 1; i++)
			{
				putcharUSART2(g_usart2_buffer[i]);
			}
			printUSART2("\n");
			g_usart2_ridx = 0;
			g_usart2_widx = 0;
		}

		// USART2->DR = g_usart2_buffer[g_usart2_ridx++];
		if (g_usart2_ridx >= (USART2_BUFFER_SIZE))
		{
			g_usart2_ridx = 0;
		}
	}
}

// ************************************** USART3 ************************************

volatile uint8_t g_usart3_buffer[USART3_BUFFER_SIZE];
volatile uint16_t g_usart3_widx = 0;
volatile uint16_t g_usart3_ridx = 0;

void initUSART3(uint32_t baudrate)
{

	// wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww
	//  USART3: PD8 -> TX & PD9 -> RX
	//------------------------------------------------------------------

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
	GPIOD->MODER |= (GPIO_MODER_MODER8_1) | (GPIO_MODER_MODER9_1);
	GPIOD->AFR[1] |= 0x00000077;

	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR2_1;
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR3_1;

	USART3->BRR = baudrate;
	USART3->CR1 = (USART_CR1_UE) | (USART_CR1_TE) | (USART_CR1_RE);
}

void putcharUSART3(uint8_t data)
{ /// print one character to USART2
	while (!(USART3->SR & USART_SR_TC))
		;

	USART3->DR = data;
}

uint8_t getcharUSART3(void)
{ /// get one character from USART1
	uint8_t data;
	USART3->CR1 |= USART_CR1_UE | USART_CR1_RE; // enable usart	and Rx
	while ((USART3->SR & USART_SR_RXNE) != USART_SR_RXNE)
		; // wait until data ready

	data = USART3->DR;				// send data
	USART3->SR &= ~(USART_SR_RXNE); // clear Rx data ready flag
	USART3->CR1 &= ~(USART_CR1_RE);
	return data;
}

void printUSART3(char *str, ...)
{					  /// print text and up to 10 arguments!
	uint8_t rstr[40]; // 33 max -> 32 ASCII for 32 bits and NULL
	uint16_t k = 0;
	uint16_t arg_type;
	uint32_t utmp32;
	uint32_t *p_uint32;
	char *p_char;
	va_list vl;

	// va_start(vl, 10);													// always pass the last named parameter to va_start, for compatibility with older compilers
	va_start(vl, str); // always pass the last named parameter to va_start, for compatibility with older compilers
	while (str[k] != 0x00)
	{
		if (str[k] == '%')
		{
			if (str[k + 1] != 0x00)
			{
				switch (str[k + 1])
				{
				case ('b'):
				{ // binary
					if (str[k + 2] == 'b')
					{ // byte
						utmp32 = va_arg(vl, int);
						arg_type = (PRINT_ARG_TYPE_BINARY_BYTE);
					}
					else if (str[k + 2] == 'h')
					{ // half word
						utmp32 = va_arg(vl, int);
						arg_type = (PRINT_ARG_TYPE_BINARY_HALFWORD);
					}
					else if (str[k + 2] == 'w')
					{ // word
						utmp32 = va_arg(vl, uint32_t);
						arg_type = (PRINT_ARG_TYPE_BINARY_WORD);
					}
					else
					{ // default word
						utmp32 = va_arg(vl, uint32_t);
						arg_type = (PRINT_ARG_TYPE_BINARY_WORD);
						k--;
					}

					k++;
					p_uint32 = &utmp32;
					break;
				}
				case ('d'):
				{ // decimal
					if (str[k + 2] == 'b')
					{ // byte
						utmp32 = va_arg(vl, int);
						arg_type = (PRINT_ARG_TYPE_DECIMAL_BYTE);
					}
					else if (str[k + 2] == 'h')
					{ // half word
						utmp32 = va_arg(vl, int);
						arg_type = (PRINT_ARG_TYPE_DECIMAL_HALFWORD);
					}
					else if (str[k + 2] == 'w')
					{ // word
						utmp32 = va_arg(vl, uint32_t);
						arg_type = (PRINT_ARG_TYPE_DECIMAL_WORD);
					}
					else
					{ // default word
						utmp32 = va_arg(vl, uint32_t);
						arg_type = (PRINT_ARG_TYPE_DECIMAL_WORD);
						k--;
					}

					k++;
					p_uint32 = &utmp32;
					break;
				}
				case ('c'):
				{ // character
					char tchar = va_arg(vl, int);
					putcharUSART3(tchar);
					arg_type = (PRINT_ARG_TYPE_CHARACTER);
					break;
				}
				case ('s'):
				{ // string
					p_char = va_arg(vl, char *);
					sprintUSART3((uint8_t *)p_char);
					arg_type = (PRINT_ARG_TYPE_STRING);
					break;
				}
				case ('f'):
				{											// float
					uint64_t utmp64 = va_arg(vl, uint64_t); // convert double to float representation IEEE 754
					uint32_t tmp1 = utmp64 & 0x00000000FFFFFFFF;
					tmp1 = tmp1 >> 29;
					utmp32 = utmp64 >> 32;
					utmp32 &= 0x07FFFFFF;
					utmp32 = utmp32 << 3;
					utmp32 |= tmp1;
					if (utmp64 & 0x8000000000000000)
						utmp32 |= 0x80000000;

					if (utmp64 & 0x4000000000000000)
						utmp32 |= 0x40000000;

					p_uint32 = &utmp32;

					arg_type = (PRINT_ARG_TYPE_FLOAT);
					// arg_type = (PRINT_ARG_TYPE_HEXADECIMAL_WORD);
					// arg_type = (PRINT_ARG_TYPE_BINARY_WORD);
					break;
				}
				case ('x'):
				{ // hexadecimal
					if (str[k + 2] == 'b')
					{ // byte
						utmp32 = (uint32_t)va_arg(vl, int);
						arg_type = (PRINT_ARG_TYPE_HEXADECIMAL_BYTE);
					}
					else if (str[k + 2] == 'h')
					{ // half word
						utmp32 = (uint32_t)va_arg(vl, int);
						arg_type = (PRINT_ARG_TYPE_HEXADECIMAL_HALFWORD);
					}
					else if (str[k + 2] == 'w')
					{ // word
						utmp32 = va_arg(vl, uint32_t);
						arg_type = (PRINT_ARG_TYPE_HEXADECIMAL_WORD);
					}
					else
					{
						utmp32 = va_arg(vl, uint32_t);
						arg_type = (PRINT_ARG_TYPE_HEXADECIMAL_WORD);
						k--;
					}

					k++;
					p_uint32 = &utmp32;
					break;
				}
				default:
				{
					utmp32 = 0;
					p_uint32 = &utmp32;
					arg_type = (PRINT_ARG_TYPE_UNKNOWN);
					break;
				}
				}

				if (arg_type & (PRINT_ARG_TYPE_MASK_CHAR_STRING))
				{
					getStr4NumMISC(arg_type, p_uint32, rstr);
					sprintUSART3(rstr);
				}
				k++;
			}
		}
		else
		{ // not a '%' char -> print the char
			putcharUSART3(str[k]);
			if (str[k] == '\n')
				putcharUSART3('\r');
		}
		k++;
	}

	va_end(vl);
	return;
}

void sprintUSART3(uint8_t *str)
{
	uint16_t k = 0;

	while (str[k] != '\0')
	{
		putcharUSART3(str[k]);
		if (str[k] == '\n')
			putcharUSART3('\r');
		k++;

		if (k == MAX_PRINT_STRING_SIZE)
			break;
	}
}

void enIrqUSART3(void)
{
	USART3->CR1 &= ~(USART_CR1_UE);

	NVIC_EnableIRQ(USART3_IRQn);
	USART3->CR1 |= (USART_CR1_UE) | (USART_CR1_RE) | (USART_CR1_RXNEIE);
}

void disIrqUSART3(void)
{
	USART3->CR1 &= ~((USART_CR1_UE) | (USART_CR1_RXNEIE));

	NVIC_DisableIRQ(USART2_IRQn);
	USART3->CR1 |= (USART_CR1_UE);
}

#ifdef USART_ECHO
void USART3_IRQHandler(void)
{
	uint8_t data;

	if (USART3->SR & (USART_SR_RXNE))
	{
		data = USART3->DR;
		// USART3->SR &= ~(USART_SR_RXNE);
		USART3->DR = data;
	}
}
#else

void USART3_IRQHandler(void)
{
	// usart3_state = 0;

	if (USART3->SR & (USART_SR_RXNE))
	{
		g_usart3_buffer[g_usart3_widx] = USART3->DR;
		g_usart3_widx++;
		if (g_usart3_widx >= (USART3_BUFFER_SIZE))
		{
			g_usart3_widx = 0;
		}
		if (g_usart3_ridx >= (USART3_BUFFER_SIZE))
		{
			g_usart3_ridx = 0;
		}
	}
}

#endif
void chkRxBuffUSART3(void)
{
	if (g_usart3_ridx != g_usart3_widx)
	{
		if ((g_usart3_buffer[g_usart3_widx - 1] == '\n'))
		{
			char str[g_usart2_widx];
			for (int i = g_usart2_ridx; i < g_usart3_widx - 1; i++)
			{
				str[i] = g_usart3_buffer[i];
				// putcharUSART2(g_usart3_buffer[i]);
			}
			str[g_usart3_widx - 1] = '\0';
			// putcharUSART2('\n');
			g_usart3_ridx = 0;
			g_usart3_widx = 0;
			printUSART3("Message recived\n");
			// printUSART2("%s\n", str);
			SplitString('#', str);
		}

		// USART3->DR = g_usart3_buffer[g_usart3_ridx++];
		if (g_usart3_ridx >= (USART3_BUFFER_SIZE))
		{
			g_usart3_ridx = 0;
		}
	}
}

void SplitString(char c, char *toSplit)
{
	uint8_t char_index = 0;
	for (int i = 0; i < strlen(toSplit); i++)
	{
		if (toSplit[i] == c)
		{
			char_index = i;
		}
	}
	char left[char_index];
	char right[strlen(toSplit) - char_index];
	for (int i = 0; i < char_index; i++)
	{
		left[i] = toSplit[i];
	}
	left[char_index] = '\0';
	for (int i = char_index + 1; i < strlen(toSplit); i++)
	{
		right[i - char_index - 1] = toSplit[i];
	}
	right[strlen(toSplit) - char_index] = '\0';

	uint8_t value = (uint8_t)strtol(right, NULL, 10);

	printUSART2("Received: Left: %s  Right: %d\n", left, value);
	if (!strcmp(left, "PD12") && value == 1)
	{
		printUSART2("Diode PD12 control\n");
		GPIOD->ODR = 0x8000;
	}
	if (!strcmp(left, "PD13") && value == 1)
	{
		printUSART2("Diode PD13 control\n");
		GPIOD->ODR = 0x4000;
	}
	if (!strcmp(left, "PD14") && value == 1)
	{
		printUSART2("Diode PD14 control\n");
		GPIOD->ODR = 0x2000;
	}
	if (!strcmp(left, "PD15") && value == 1)
	{
		printUSART2("Diode PD15 control\n");
		GPIOD->ODR = 0x1000;
	}
}
