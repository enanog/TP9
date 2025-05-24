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

// Maximum size for an input segment
#define SEGMENT_SIZE 100

// Prototypes for internal validation and conversion functions
/**
 * @brief Checks whether the given string represents a valid integer number.
 *
 * @param str Null-terminated string to validate.
 * @return 1 if valid integer format, 0 otherwise.
 */
static uint8_t isInt(const char *str);

/**
 * @brief Checks whether the given string represents a valid floating-point number.
 *
 * @param str Null-terminated string to validate.
 * @return 1 if valid float format, 0 otherwise.
 */
static uint8_t isFloat(const char *str);

/**
 * @brief Checks whether the given string represents a valid hexadecimal number.
 *
 * @param str Null-terminated string to validate.
 * @return 1 if valid hex format, 0 otherwise.
 */
static uint8_t isHex(const char *str);

/**
 * @brief Converts a valid integer string to its int value.
 *
 * @param str Null-terminated valid integer string.
 * @return Parsed integer value.
 */
static int strToInt(const char *str);

/**
 * @brief Converts a valid float string to its double value.
 *
 * @param str Null-terminated valid float string.
 * @return Parsed floating-point value.
 */
static double strToFloat(const char *str);

/**
 * @brief Converts a valid hexadecimal string (with "0x" prefix) to uint32_t.
 *
 * @param str Null-terminated valid hex string (e.g., "0x1A3F").
 * @return Parsed hexadecimal value as uint32_t.
 */
static uint32_t strToHex(const char *str);

/**
 * @brief Computes x raised to the power of n.
 *
 * @param x Base value.
 * @param n Non-negative integer exponent.
 * @return Result of x raised to the power n.
 */
static double double_pow(double x, uint8_t n);

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void ClearInputBuffer(void)
{
	char c;
	while((c = getchar()) != '\n' && c != EOF);
}

void nscanf(scanCallback *callbacks, void **destinations, uint8_t count)
{
	char segment[SEGMENT_SIZE];
	uint8_t segment_index;
	uint32_t arg_index;
	int character;
	uint8_t in_quotes ;
	uint8_t valid_input;

	do
	{
		segment_index = 0;
		arg_index = 0;
		in_quotes = 0;
		valid_input = 1;

		printf("> ");

		while((character = getchar()) != '\n' && character != EOF)
		{
			if (character == '"')
			{
				// Toggle quotes mode
				in_quotes = !in_quotes;
			}
			else if(character == ' ' && !in_quotes)
			{
				// End of a segment
				if(segment_index > 0 && arg_index < count)
				{
					segment[segment_index] = '\0';
					if (!callbacks[arg_index](segment, destinations[arg_index]))
					{
						valid_input = 0;            // Mark input as invalid
					}
					arg_index++;
					segment_index = 0;
				}
			}
			else
			{
				if(segment_index < sizeof(segment) -1)
				{
					segment[segment_index++] = (char) character;
				}
			}
		}

		// Process the last segment
		if (segment_index > 0 && arg_index < count) {
			segment[segment_index] = '\0';
			if (!callbacks[arg_index](segment, destinations[arg_index]))
			{
				valid_input = 0;            // Mark input as invalid
			}
			arg_index++;
		}

		 // Check if all arguments were parsed
		if (arg_index < count) {
			printf("[ERROR] Missing arguments. Expected:%d, Read: %d\n", count, arg_index);
		}
	}while(!valid_input || arg_index < count);	// Retry if there was an error
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

uint8_t readString(const char *str, void *dest)
{
	char *ptr = (char *)dest;

	while((*ptr++ = *str++) != '\0');
	return 1;
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
		if(!isDigit(*str))
		{
			return 0;
		}
		str++;
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
		}
		else if(!isDigit(*str))
		{
			return 0;
		}
		str++;
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
		if(!isNibble(*str))
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
	int num = 0;
	int8_t sign = 1;

	if(str[i] == '+' || str[i] == '-')
	{
		sign = (str[i++] == '+')? 1: -1;
	}
	for(; str[i] != '\0'; i++)
	{
		num = num*10 + TODIGIT(str[i]);
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
		num = num*10 + TODIGIT(str[i]);
	}

	if(str[i++] == '\0')
	{
		return sign * num;
	}

	uint8_t dot_index;
	for(dot_index = i - 1; str[i] != '\0'; i++)
	{
		num = num + TODIGIT(str[i]) * double_pow(0.1, i-dot_index);
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
			num += TODIGIT(str[i]);
			continue;
		}

		num += TOUPPER(str[i]) - 'A' + 10;
	}

	return num;
}

static double double_pow(double x, uint8_t n)
{
	double result = 1;
	for(uint8_t i = 0; i < n; i++)
	{
		result *= x;
	}
	return result;
}

