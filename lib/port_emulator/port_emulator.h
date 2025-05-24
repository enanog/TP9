/* ---------------------------------------------------
 * port_emulator.h
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

#ifndef PORT_EMULATOR_H
#define PORT_EMULATOR_H

// Error value returned when reading an individual pin configured incorrectly
#define GPIO_ERR_INPUT 0xFF
// Error value returned when reading a masked group of pins with an invalid configuration
#define GPIO_ERR_INPUT_MASK 0xFFFFFFFF

// Uncomment to switch to Big Endian pin mapping
//#define BIG_ENDIAN;

// Pin direction modes.
enum
{
	INPUT = 0,
	OUTPUT,
};

//Output states for a pin or group of pins.
enum
{
	HIGH = 0,
	LOW,
	TOGGLE,
};

/**
 * @brief Port identifiers used to select which virtual GPIO port to access.
 *
 * If BIG_ENDIAN is not defined (default little endian configuration):
 *   - PORTA index is 0 (occupies bits 8-15)
 *   - PORTB index is 1 (occupies bits 0-7)
 *   - PORTD index is 2 (occupies bits 0-15)
 *
 * If BIG_ENDIAN is defined:
 *   - PORTB index is 0 (occupies bits 0-7)
 *   - PORTA index is 1 (occupies bits 8-15)
 *   - PORTD index is 2 (occupies bits 0-15)
 */
#ifndef BIG_ENDIAN
	enum
	{
		PORTA = 0,
		PORTB,
		PORTD,
	};
#else
	enum
	{
		PORTB = 0,
		PORTA,
		PORTD,
	};
#endif


/**
 * @brief Initializes the mode (input or output) of a specific pin on a given port.
 *
 * This function validates the port and pin, and then sets the corresponding bit
 * in the direction register to configure the pin's mode.
 *
 * @param port   Port to which the pin belongs (PORTA, PORTB, PORTD)
 * @param pin    Pin number within the port (0–7 for PORTA && PORTB, 0-15 for PORTD).
 * @param state  Desired mode for the pin: INPUT (0) or OUTPUT (1).
 */
void GPIO_PinInit(uint8_t port, uint8_t pin, uint8_t state);


/**
 * @brief Initializes the mode (input or output) of multiple pins on a given port using a bitmask.
 *
 * This function allows configuring a group of pins simultaneously. It shifts the mask
 * to the proper location based on the port and updates the direction register accordingly.
 *
 * @param port   Port to which the pins belong (PORTA, PORTB, PORTD).
 * @param mask   Bitmask representing the pins to initialize.
 * @param state  Desired mode: INPUT (0) or OUTPUT (1).
 */
void GPIO_MaskInit(uint8_t port, uint16_t mask, uint8_t state);


/**
 * @brief Sets the output state of a specific pin on a given port.
 *
 * This function writes a logic value (HIGH/LOW) to the selected pin or toggles it
 * if TOGGLE is selected. Only works if the pin is configured as an output.
 *
 * @param port   Port to which the pin belongs (PORTA, PORTB, PORTD).
 * @param pin    Pin number within the port (0–7 for PORTA and PORTB, 0–15 for PORTD).
 * @param state  Output state: LOW (0), HIGH (1), or TOGGLE (2).
 */
void GPIO_SetPinState(uint8_t port, uint8_t pin, uint8_t state);


/**
 * @brief Sets the output state of multiple pins on a given port using a bitmask.
 *
 * This function modifies multiple output pins simultaneously based on a bitmask.
 * Pins must be previously configured as outputs. Invalid pins will trigger warnings.
 *
 * @param port   Port to which the pins belong (PORTA, PORTB, PORTD).
 * @param mask   Bitmask representing the pins to modify.
 * @param state  Output state to apply: LOW (0), HIGH (1), or TOGGLE (2).
 */
void GPIO_SetMaskedOutput(uint8_t port, uint16_t mask, uint8_t state);


/**
 * @brief Reads the current logic level of a specific pin on a given port.
 *
 * Only works correctly for pins configured as input. Reading from an output-configured
 * pin will issue a warning and return an error code.
 *
 * @param port   Port to which the pin belongs (PORTA, PORTB, PORTD).
 * @param pin    Pin number within the port (0–7 for PORTA and PORTB, 0–15 for PORTD).
 *
 * @return Logic level of the pin: 0 (LOW), 1 (HIGH), or 0xFF on error.
 */
uint8_t GPIO_ReadPin(uint8_t port, uint8_t pin);

/**
 * @brief Reads the logic levels of multiple pins on a given port using a bitmask.
 *
 * This function retrieves the current logic states of a group of pins using a mask.
 * Returns 0xFFFFFFFF if any of the selected pins are configured as outputs.
 *
 * @param port   Port to which the pins belong (PORTA, PORTB, PORTD).
 * @param mask   Bitmask representing the pins to read.
 *
 * @return Bitfield with the logic levels of the selected pins, or 0xFFFFFFFF on error.
 */
uint32_t GPIO_MaskRead(uint8_t port, uint16_t mask);

#endif /* PORT_EMULATOR_H */
