#include "misc.h"

void getDNumMISC(uint8_t *dnum, uint32_t num)
{ /// calculate decimal digits from integer number 'num' and store them as ASCII char in dnum array
    uint8_t k;
    uint32_t step = 1000000000;

    for (k = 0; k < 10; k++)
    {                     // convert integer value in hex format to decimal format
        dnum[9 - k] = 48; // set char value
        while (num >= step)
        {
            dnum[9 - k]++;
            num -= step;
        }
        step /= 10;
    }
}

void putDNumMISC(uint8_t *dnum, uint16_t *m, uint8_t *r_str)
{ /// add unsigned integer number digits into string r_str
    uint8_t k = 0, flag = 0;

    for (k = 0; k < 10; k++)
    { // convert integer value in hex format to decimal format
        if (flag == 0)
        {
            if ((dnum[9 - k] != 48) || (k == 9)) // include the 0x00000000 number
            {
                flag = 1;
                r_str[(*m)] = dnum[9 - k];
                (*m)++;
            }
        }
        else
        {
            r_str[(*m)] = dnum[9 - k];
            (*m)++;
        }
    }
     r_str[(*m)] = 0x00;
}

void getStr4NumMISC(uint16_t type, uint32_t  * num, uint8_t * rstr)
{   /// print text and one signed integer or float number
    // the 'num' number is not modified!
    int32_t * p_int32;
    uint8_t dnum[11];
    uint8_t k;
    uint16_t m = 0;
    rstr[0] = 0x00;
    
    switch(type)
    {
		case(PRINT_ARG_TYPE_BINARY_BYTE):
		{// 8 bit binary representation
			for(k=0;k<8;k++)
			{
				if((*num) & 0x00000080)
				{
					rstr[k] = '1';
				}
				else
				{
					rstr[k] = '0';
				}
				*num = (*num)<<1;
			}
			rstr[k] = 0x00;
			break;
		}
		case(PRINT_ARG_TYPE_BINARY_HALFWORD):
		{// 16 bit binary representation
			for(k=0;k<16;k++)
			{
				if((*num) & 0x00008000)
				{
					rstr[k] = '1';
				}
				else
				{
					rstr[k] = '0';
				}
				*num = (*num)<<1;
			}
			rstr[k] = 0x00;
			break;
		}
		case(PRINT_ARG_TYPE_BINARY_WORD):
		{// 32 bit binary representation
			for(k=0;k<32;k++)
			{
				if((*num) & 0x80000000)
				{
					rstr[k] = '1';
				}
				else
				{
					rstr[k] = '0';
				}
				*num = (*num)<<1;
			}
			rstr[k] = 0x00;
			break;
		}
		case(PRINT_ARG_TYPE_DECIMAL_BYTE):
		case(PRINT_ARG_TYPE_DECIMAL_HALFWORD):
		case(PRINT_ARG_TYPE_DECIMAL_WORD):
		{// conver integer number into appropriate string
			p_int32 = (int32_t *)num;
            if (*p_int32 < 0)
            { // print the sign character
                rstr[m] = '-';
                m++;
                *p_int32 = (*p_int32)*(-1);
            }

            getDNumMISC(dnum, (uint32_t)(*p_int32));
            putDNumMISC(dnum, &m, rstr);
			break;
		}
		case(PRINT_ARG_TYPE_CHARACTER):
		{
			break;
		}
		case(PRINT_ARG_TYPE_STRING):
		{
			break;
		}
		case(PRINT_ARG_TYPE_FLOAT):
		{
			//int32_t exp;
			//uint32_t man;
			//// extract sign
			//if((*num)&0x80000000)
			//{
				//rstr[m] = '-';
				//m++;
			//}
			
			//// extract exponent
			//exp = (((*num)&0x7F800000)>>23);
			//exp -= 127;
			
			//// extract mantisa
			//man = ~((*num)&0xFF800000);
			
			////exp = (*num);
			//getDNumMISC(dnum, exp);
            //putDNumMISC(dnum, &m, rstr);
			
			
			float f_step;
			float f_num = *((float *)num); // convert pointer
			uint8_t sige = ' ';
			uint16_t e_part;
			uint32_t d_part, i_part;
			
            if (f_num < 0)
            { // negative float number
                rstr[m] = '-';
                m++;
                f_num = (-1) * f_num;
            }

            if (f_num < 1)
            {
                sige = '-';
                f_step = 0.1;
                e_part = 1;
                while (f_num < f_step)
                {
                    f_step = f_step * 0.1;
                    e_part++;
                }
            }
            else
            {
                f_step = 1.0;
                e_part = 0;
                while (f_num > f_step)
                {
                    f_step = f_step * 10;
                    e_part++;
                }
                f_step = f_step / 10;
                e_part--;
            }

            f_num = f_num / f_step;
            i_part = (uint32_t)f_num;

            f_num = f_num - (float)i_part;
            d_part = (uint32_t)(1000 * f_num);

            getDNumMISC(dnum, i_part);
            putDNumMISC(dnum, &m, rstr);

            getDNumMISC(dnum, d_part);
            dnum[3] = '.';
            putDNumMISC(dnum, &m, rstr);

            rstr[m] = 'e';
            m++;

            if (sige == '-')
            {
                rstr[m] = '-';
                m++;
            }

            getDNumMISC(dnum, e_part);
            putDNumMISC(dnum, &m, rstr);
				
			break;
		}
		case(PRINT_ARG_TYPE_HEXADECIMAL_BYTE):
		{// 8 bit hex string representation
			uint8_t t_rez;
            for (k=0;k<2;k++)
            {
                t_rez = ((*num) & 0x000000F0) >> 4;
                if (t_rez < 0x0A)
                {
                    rstr[m] = t_rez + 0x30;
                }
                else
                {
                    rstr[m] = t_rez + 0x41 - 0x0A;
                }
                (*num) = (*num) << 4;
                m++;
            }
            rstr[m] = 0x00;
			break;
		}
		case(PRINT_ARG_TYPE_HEXADECIMAL_HALFWORD):
		{// 16 bit hex string representation
			uint8_t t_rez;
            for (k=0;k<4;k++)
            {
                t_rez = ((*num) & 0x0000F000) >> 12;
                if (t_rez < 0x0A)
                {
                    rstr[m] = t_rez + 0x30;
                }
                else
                {
                    rstr[m] = t_rez + 0x41 - 0x0A;
                }
                (*num) = (*num) << 4;
                m++;
            }
            rstr[m] = 0x00;
			break;
		}
		case(PRINT_ARG_TYPE_HEXADECIMAL_WORD):
		{// 32 bit hex string representation
            uint8_t t_rez;
            for (k=0;k<8;k++)
            {
                t_rez = ((*num) & 0xF0000000) >> 28;
                if (t_rez < 0x0A)
                {
                    rstr[m] = t_rez + 0x30;
                }
                else
                {
                    rstr[m] = t_rez + 0x41 - 0x0A;
                }
                (*num) = (*num) << 4;
                m++;
            }
            rstr[m] = 0x00;
			break;
		}
		default:
		{
			rstr[0] = 0x00;
			break;
		}
	}
}


