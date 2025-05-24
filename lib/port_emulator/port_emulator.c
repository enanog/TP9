/* ---------------------------------------------------
 * port_emulator.c
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

#include <stdint.h>
#include "port_emulator.h"

// GPIO Register Macros
#define _PDDR portD.PDDR.WORD	// Access the full 16-bit Port Data Direction Register
#define _PDIR portD.PDIR.WORD	// Access the full 16-bit Port Data Input Register
#define _PDOR portD.PDOR.WORD	// Access the full 16-bit Port Data Output Register

#define sizePort 8

// Port offset calculation for 16-bit emulated port system.
#define _offsetPort(port) ((!(port))? 8: 0)

// Bit manipulation macros
#define _bitSet(x, n) ((x) | (1 << (n)))
#define _bitClr(x, n) ((x) & ~(1 << (n)))
#define _bitGet(x, n) (((x) >> (n)) & 1)
#define _bitToggle(x, n) ((x) ^ (1 << (n)))

// Mask manipulation macros
#define _maskOn(x, mask) ((x) | (mask))
#define _maskOff(x, mask) ((x) & ~(mask))
#define _maskToggle(x, mask) ((x) ^ (mask))
#define _maskGet(x, mask) ((x) & (mask))

// Error codes for GPIO operations
enum {
    GPIO_ERR_NOTERROR = 0,               // No error
    GPIO_ERR_INVALID_DIRECTION,          // Invalid direction (INPUT/OUTPUT)
    GPIO_ERR_INVALID_STATE,              // Invalid output state (HIGH/LOW/TOGGLE)
    GPIO_ERR_WRITE_TO_INPUT,             // Writing to pin configured as input
    GPIO_ERR_READ_OUTPUT_AS_INPUT        // Reading from a pin configured as output
};

typedef union
{
	uint8_t byte[2];	// Port A and Port B
	uint16_t WORD;
}gpio_port_d_t;

typedef struct
{
	gpio_port_d_t PDDR;	// Port Data Direction Register
	gpio_port_d_t PDIR;	// Port Data Input Register
	gpio_port_d_t PDOR;	// Port Data Output Register
}gpio_register_t;

/**
 * @brief Handles and reports GPIO-related errors based on the given error code.
 *
 * @param port  Port identifier where the error occurred (PORTA, PORTB, PORTD).
 * @param pin   Pin number involved in the error (0–7 for PORTA and PORTB, 0–15 for PORTD, or 0xFF if not applicable).
 * @param code  Error code indicating the type of GPIO error (e.g., invalid port, pin, direction, etc.).
 *
 * @return 1 if an error was detected and reported, 0 otherwise.
 */
static uint8_t GPIO_Error(uint8_t port, uint8_t pin, uint8_t code);

// Instance simulating the GPIO registers of PORTD
static gpio_register_t portD = {
	.PDDR.WORD = 0,			// All pins default to INPUT
	.PDIR.WORD = 0x1AAD,	// Simulated input values for all pins
	.PDOR.WORD = 0			// All outputs initialized to LOW
};

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void GPIO_PinInit(uint8_t port, uint8_t pin, uint8_t state)
{
	// Check for invalid port or pin index
	if(GPIO_Error(port, pin, 0))
	{
		return;
	}
	// Check for invalid direction state
	else if(state > 1)
	{
		GPIO_Error(port, pin, GPIO_ERR_INVALID_DIRECTION);
		return;
	}

	uint8_t offset = _offsetPort(port);
	// Configure pin as OUTPUT or INPUT by setting or clearing its corresponding bit in _PDDR
	_PDDR = (state)? _bitSet(_PDDR , pin + offset): _bitClr(_PDDR , pin + offset);
}

void GPIO_MaskInit(uint8_t port, uint16_t mask, uint8_t state)
{
	// Validate port only (mask applies to all pins)
	if(GPIO_Error(port, 0, 0))
	{
		return;
	}
	else if (state != INPUT && state != OUTPUT)
    {
        GPIO_Error(port, 0, GPIO_ERR_INVALID_DIRECTION);
        return;
    }
    // Ensure mask does not exceed 8 bits for PORTA or PORTB
    else if((mask > 0xFF && (port == PORTA || port == PORTB)))
    {
		GPIO_Error(port, 0XFF, 0);
		return;
	}

	uint8_t offset = _offsetPort(port);
	// Set or clear multiple pins' directions using the mask
	_PDDR = (state)? _maskOn(_PDDR , mask << offset): _maskOff(_PDDR , mask << offset);
}

void GPIO_SetPinState(uint8_t port, uint8_t pin, uint8_t state)
{
	uint8_t offset = _offsetPort(port);

	if(GPIO_Error(port, pin, 0))
	{
		return;
	}
	// Ensure the pin is configured as OUTPUT before modifying its state
	else if(_bitGet(_PDDR, pin + offset) != OUTPUT)
	{
		GPIO_Error(port, pin, GPIO_ERR_WRITE_TO_INPUT);
		return;
	}

	// Set the output state based on input
	switch(state)
	{
		case HIGH:
			_PDOR = _bitSet(_PDOR, pin + offset);
			break;

		case LOW:
			_PDOR = _bitClr(_PDOR, pin + offset);
			break;

		case TOGGLE:
			_PDOR = _bitToggle(_PDOR, pin + offset);
			break;

		default:
			GPIO_Error(port, pin, GPIO_ERR_INVALID_STATE);
			break;
	}
}

void GPIO_SetMaskedOutput(uint8_t port, uint16_t mask, uint8_t state)
{
	uint8_t offset = _offsetPort(port);

	if(GPIO_Error(port, 0, 0))
	{
		return;
	}

	// Validate that each bit in the mask refers to a pin configured as OUTPUT
	uint8_t range = (port == PORTA)? 2*sizePort: sizePort;
    for (uint8_t i = offset; i < range; i++)
    {
        if (_bitGet(_PDDR, i) != OUTPUT && _bitGet(mask, i - offset))
        {
            GPIO_Error(port, i - offset, GPIO_ERR_WRITE_TO_INPUT);
            return;
        }
    }

    // Perform batch state operation on all masked pins
	switch(state)
	{
		case HIGH:
			_PDOR = _maskOn(_PDOR, mask << offset);
			break;

		case LOW:
			_PDOR = _maskOff(_PDOR, mask << offset);
			break;

		case TOGGLE:
			_PDOR = _maskToggle(_PDOR, mask << offset);
			break;

		default:
			GPIO_Error(port, 0, GPIO_ERR_INVALID_STATE); // 0x0 indicates don't care about pin
			break;
	}
}

uint8_t GPIO_ReadPin(uint8_t port, uint8_t pin)
{
	uint8_t offset = _offsetPort(port);

	if(GPIO_Error(port, pin, 0))
	{
		return GPIO_ERR_INPUT;
	}
	// Ensure the pin is configured as INPUT before reading
	else if(_bitGet(_PDDR, pin + offset) != INPUT)
	{
		GPIO_Error(port, pin, GPIO_ERR_READ_OUTPUT_AS_INPUT);
		return GPIO_ERR_INPUT;
	}

	// Read and return input pin state
	uint8_t state = _bitGet(_PDIR, pin + offset);
	return state;
}

uint32_t GPIO_MaskRead(uint8_t port, uint16_t mask)
{
	uint8_t offset = _offsetPort(port);

	if(GPIO_Error(port, 0, 0))
	{
		return GPIO_ERR_INPUT_MASK;
	}

	// Validate all masked bits refer to INPUT pins
	uint8_t range = (port == PORTA)? 2*sizePort: sizePort;
    for (uint8_t i = offset; i < range; i++)
    {
        if (_bitGet(_PDDR, i) != INPUT && _bitGet(mask, i - offset))
        {
            GPIO_Error(port, i - offset, GPIO_ERR_READ_OUTPUT_AS_INPUT);
            return GPIO_ERR_INPUT_MASK;
        }
    }

	// Return the input state of all pins in the mask
	uint32_t state = _maskGet(_PDIR, mask << offset);
	return (!port)? state >> offset: state;
}

/*******************************************************************************
 *******************************************************************************
                             LOCAL DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
static uint8_t GPIO_Error(uint8_t port, uint8_t pin, uint8_t code)
{
	uint8_t error = 0;

	// Validate port and pin only if no specific code is passed
	if(!code)
	{
		if(port > 2)
		{
			error = 1;
			printf("\nError: specified port does not exist.\n");
		}
		else if((pin >= sizePort && (port == PORTA || port == PORTB)) || (pin >= 2*sizePort && port == PORTD))
		{
			if(pin != 0xFF)
			{
				error = 1;
				printf("\nError: attempting to access pin %d on port %c, which exceeds port size.", pin, 'A'+port);
			}
			else
			{
				error = 1;
				printf("\nError: attempting to access a pin of port %c which exceeds port size.", 'A'+port);
			}
		}
	}

	// Report specific error based on error code
	switch (code)
	{
		case GPIO_ERR_NOTERROR:
			break;
		case GPIO_ERR_INVALID_DIRECTION:
			error = 1;
			printf("\nError: invalid direction mode specified. Use INPUT (0) or OUTPUT (1).");
			break;
		case GPIO_ERR_INVALID_STATE:
	    	error = 1;
	    	printf("\nError: invalid output state specified. Use HIGH (1), LOW (0), or TOGGLE (2).");
	    	break;
	    case GPIO_ERR_WRITE_TO_INPUT:
	    	error = 1;
	    	printf("\nWarning: trying to write to a pin configured as input. Operation ignored.");
	    	break;
	    case GPIO_ERR_READ_OUTPUT_AS_INPUT:
	    	error = 1;
	    	printf("\nWarning: attempting to read an output pin as input. Returning 0xFF or 0xFFFFFFFF as error value.");
	    	break;
	    default:
	    	error = 1;
	    	printf("Error: unknown GPIO error.\n");
	    	break;
    }

    return error;
}
