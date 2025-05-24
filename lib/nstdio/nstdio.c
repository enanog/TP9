/* ---------------------------------------------------
 * nstdio.c
 * ---------------------------------------------------
 * GRUPO 1:
 * 	CASTRO, Tomás
 *	FRIGERIO, Dylan
 * 	VALENZUELA, Agustín
 * 	YAGGI, Lucca
 *
 * Profesores:
 * 	MAGLIIOLA, Nicolas
 * 	JACOBY, Daniel
 * 	VACATELLO, Pablo
 *
 * fecha: 21/05/2025
 * ---------------------------------------------------*/

#include <stdio.h>
#include "nstdio.h"

#define TOUPPER(c) (((c) >= 'a' && (c) <= 'z')? 'A'+(c)-'a' : (c))
#define TOLOWER(c) (((c) >= 'A' && (c) <= 'Z')? 'a'+(c)-'A' : (c))

#define isDigit(c) ((c) >= '0' && (c) <= '9')
#define isByte(c) (isDigit(c) || (TOUPPER(c) >= 'A' && TOUPPER(c) <= 'F'))
#define isLetter(c) (TOUPPER(c) >= 'A' && TOUPPER(c) <= 'Z')

static uint8_t isInt(const char *str);
static uint8_t isFloat(const char *str);
static uint8_t isHex(const char *str);

static int strToInt(const char *str);
static double strToFloat(const char *str);
static uint32_t strToHex(const char *str);

static double double_pow(double x, uint8_t n);

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void nscaf(scanCallback *callbacks, void **destinations, uint8_t count)
{

}

uint8_t readInt(const char *str, void *dest)
{
	if(!isInt(str))
	{
		printf("[ERROR] '%s' is not a valid int.\n", str);
		return 0;
	}

	*((int *)dest) = strToInt(str);
	return 1;
}

uint8_t readFloat(const char *str, void *dest)
{
	if(!isFloat(str))
	{
		printf("[ERROR] '%s' is not a valid float.\n", str);
		return 0;
	}

	*((float *)dest) = strToFloat(str);
	return 1;
}

uint8_t readHex(const char *str, void *dest)
{
	if(!isHex(str))
	{
		printf("[ERROR] '%s' is not a valid hex format.\n", str);
		return 0;
	}

	*((uint32_t *)dest) = strToHex(str);
	return 1;
}

uint8_t readChar(const char *str, void *dest)
{
	if(*(str+1) != '\0')
	{
		printf("[ERROR] '%s' has more than one character.\n", str);
		return 0;
	}

	*((char *)dest) = *str;
	return 1;
}

void readString(const char *str, void *dest)
{
	char *ptr = (char *)dest;

	while((*ptr++ = *str++) != '\0');
}



/*******************************************************************************
 *******************************************************************************
                             LOCAL DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

static uint8_t isInt(const char *str)
{
	if(*str == '+' || *str == '-')
	{
		str++;
	}
	while(*str)
	{
		if(!isDigit(*str++))
		{
			return 0;
		}
	}
	return 1;
}

static uint8_t isFloat(const char *str)
{
	uint8_t has_dot = 0;
	if(*str == '+' || *str == '-')
	{
		str++;
	}
	while(*str)
	{
		if(*str == '.' || *str == ',')
		{
			if(has_dot)
			{
				return 0;
			}
			has_dot = 1;
			str++;
		}
		else if(!isDigit(*str++))
		{
			return 0;
		}
	}
	return 1;
}

static uint8_t isHex(const char *str)
{
	if(*str++ != '0')
	{
		return 0;
	}
	if(TOUPPER(*str) != 'X')
	{
		return 0;
	}
	str++;
	while(*str)
	{
		if(!isByte(*str))
		{
			return 0;
		}
		str++;
	}
	return 1;
}

static int strToInt(const char *str)
{
	uint8_t i = 0;
	int num;
	int8_t sign = 1;

	if(str[i] == '+' || str[i] == '-')
	{
		sign = (str[i++] == '+')? 1: -1;
	}
	for(num = 0; str[i] != '\0'; i++)
	{
		num = num*10 + (str[i]  - '0');
	}

	return sign * num;
}

static double strToFloat(const char *str)
{
	uint8_t i = 0;
	double num;
	double sign = 1;

	if(str[i] == '+' || str[i] == '-')
	{
		sign = (str[i++] == '+')? 1: -1;
	}
	for(num = 0; str[i] != '\0' && str[i] != '.' && str[i] != ','; i++)
	{
		num = num*10 + (str[i]  - '0');
	}

	if(str[i++] == '\0')
	{
		return sign * num;
	}

	uint8_t dot_index;
	for(dot_index = i - 1; str[i] != '\0'; i++)
	{
		num = num + (str[i]  - '0') * double_pow(0.1, i-dot_index);
	}

	return sign * num;
}

static uint32_t strToHex(const char *str)
{
	uint8_t i;
	uint32_t num;

	for(i = 2, num = 0; str[i] != '\0'; i++)
	{
		num <<= 4;
		if(isDigit(str[i]))
		{
			num += str[i] - '0';
			continue;
		}

		num += TOUPPER(str[i]) - 'A' + 10;
	}

	return num;
}

static double double_pow(double x, uint8_t n)
{
	double result = 1;
	for(uint8_t i = 0, result = 1; i < n; i++)
	{
		result *= x;
	}
	return result;
}

