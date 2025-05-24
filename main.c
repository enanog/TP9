/* ---------------------------------------------------
 * TP9:Tipos de datos avanzados y Herramientas de
 * desarrollo.
 * ---------------------------------------------------
 * GRUPO 1:
 * 	CASTRO, Tomás
 * 	FRIGERIO, Dylan
 * 	VALENZUELA, Agustín
 * 	YAGGI, Lucca
 *
 * Profesores:
 * 	MAGLIIOLA, Nicolas
 * 	JACOBY, Daniel
 * 	VACATELLO, Pablo
 *
 * fecha: 26/05/2025
 * ---------------------------------------------------*/

#include <stdio.h>
#include <stdint.h>

#include "lib/led_emulator/led_emulator.h"
#include "lib/nstdio/nstdio.h"
#include "lib/port_emulator/port_emulator.h"

// Maximum number of LEDs the program supports
#define MAX_LEDS 16
#define BUFFER 100

// Enumeration to represent the user's input option
enum
{
	LED_SELECTOR = 0,	// User entered a number to turn on a specific LED
	NOT_LED,			// User chose to toggle all LEDs
	OFF_ALL,			// User chose to turn all LEDs off
	ON_ALL,				// User chose to turn all LEDs on
	FINISH				 // User chose to quit the program
};

enum
{
	STOP = 0,
	RUNNING,
};

/**
 * @brief Displays the LED configuration menu and reads user input.
 * @param ledCount Pointer to store the number of LEDs selected by the user.
 * @param ledMask Pointer to store the initial bitmask of LEDs to turn on.
 */
void showMenu1(int *ledCount, uint32_t *ledMask);

/**
 * @brief Displays the LED control menu.
 *
 * @param ledCount Number of LEDs configured (used to display valid index range).
 */
void showMenu2(int ledCount);

/**
 * @brief Reads and interprets the user's menu selection.
 *
 * @param ledSelector Pointer to store the index of the LED selected by the user.
 * @param ledCount Number of LEDs configured, used to validate index.
 * @return One of the predefined menu actions (LED_SELECTOR, NOT_LED, OFF_ALL, ON_ALL, FINISH), or 0xFF if invalid.
 */
uint8_t readMenuOption(int *ledSelector, int ledCount);

int main(void)
{
	uint8_t prog = RUNNING;
	int ledCount;
	uint32_t ledMaskOn;
	uint32_t ledInitMask = 0;

	showMenu1(&ledCount, &ledMaskOn);

	// Initialize each LED pin and build the initialization mask
	for(uint8_t i = 0; i < ledCount; i++)
	{
		ledInitMask <<= 1;
		GPIO_PinInit(PORTD, i, OUTPUT);
		ledInitMask += 1;
	}

	// Set initial LED states using the provided mask
	GPIO_SetMaskedOutput(PORTD, ledMaskOn, HIGH);
	display_leds(ledCount, ledMaskOn);

	do
	{
		showMenu2(ledCount);

		int ledSelector = 0;
		uint8_t selection = readMenuOption(&ledSelector, ledCount);

		switch(selection)
		{
			case LED_SELECTOR:
				ledMaskOn |= (1 << ledSelector);
				GPIO_SetPinState(PORTD, ledSelector, HIGH);
				display_leds(ledCount, ledMaskOn);
				break;

			case NOT_LED:
				ledMaskOn = (~ledMaskOn) & ledInitMask;
				GPIO_SetMaskedOutput(PORTD, ledInitMask, TOGGLE);
				display_leds(ledCount, ledMaskOn);
				break;

			case OFF_ALL:
				ledMaskOn = 0;
				GPIO_SetMaskedOutput(PORTD, ledInitMask, LOW);
				display_leds(ledCount, ledMaskOn);
				break;

			case ON_ALL:
				ledMaskOn = ledInitMask;
				GPIO_SetMaskedOutput(PORTD, ledInitMask, HIGH);
				display_leds(ledCount, ledMaskOn);
				break;

			case FINISH:
				prog = STOP;
				break;

			default:
				printf("\t\t\t[ERROR] Invalid command.\n");
				break;
		}
	}while(prog);

	return 0;
}

void showMenu1(int *ledCount, uint32_t *ledMask)
{
	scanCallback callbacks[] = { readInt, readHex};
	void *destinations[] = { ledCount, ledMask };

	printf("\n\n\t\t\t===========  LED CONFIGURATION =========== \n");
	printf("\t\t\tEnter the number of LEDs to configure (1-%d), and initial state mask (hex, e.g. 0xAF):\n", MAX_LEDS);

	uint8_t validation_led = 1;
	do
	{
		validation_led = 1;
		printf("\t\t\t");
		nscanf(callbacks, destinations, 2);

		if (*ledCount < 1 || *ledCount > MAX_LEDS) {
			printf("\t\t\t[ERROR] Invalid number of LEDs. Must be between 1 and %d.\n", MAX_LEDS);
			validation_led = 0;
			continue;
		}

		uint16_t allowedMask = (1 << *ledCount) - 1;

		// Check that the user's mask doesn't exceed allowed LEDs
		if ((*ledMask & ~allowedMask) != 0)
		{
			printf("\t\t\t[ERROR] Mask contains bits outside the range of %d LEDs.\n", *ledCount);
			printf("\t\t\tAllowed mask: 0x%02X\n", allowedMask);
			validation_led = 0;
		}
	}while(!validation_led);
}

void showMenu2(int ledCount) {
    printf("\t\t\t=========== LED SIMULATOR MENU ===========\n");
    printf("\t\t\t [0-%d]  Turn ON a specific LED\n", ledCount - 1);
    printf("\t\t\t [t]    Toggle all LEDs\n");
    printf("\t\t\t [c]    Clear (turn off) all LEDs\n");
    printf("\t\t\t [s]    Set (turn on) all LEDs\n");
    printf("\t\t\t [q]    Quit program\n");
    printf("\t\t\t==========================================\n");
}

uint8_t readMenuOption(int * ledSelector, int ledCount)
{
	int ch[BUFFER];
	printf("\n\n\t\t\tEnter an option: ");

	ch[0] = getchar();

	if(isDigit(ch[0]))
	{
		uint8_t i;
		// Read all numeric digits
		for(i = 0; isDigit(ch[i]); i++, ch[i] = getchar())
		{
			*ledSelector = (*ledSelector) * 10 + TODIGIT(ch[i]);
		}
		if(ch[i] != '\n')
		{
			ClearInputBuffer();
		}

		// Validate LED index
		if (*ledSelector >= 0 && *ledSelector < ledCount)
		{
			return LED_SELECTOR;
		}
		else
		{
			printf("\t\t\t[ERROR] Invalid LED index.\n");
			return 0xFF;
		}
	}

	// If not a digit, convert to lowercase and check command
	ch[0] = TOLOWER(ch[0]);
	ClearInputBuffer();
	switch(ch[0])
	{
		case 't':
			return NOT_LED;
		case 'c':
			return OFF_ALL;
		case 's':
			return ON_ALL;
		case 'q':
			return FINISH;
		default:
			return 0xFF;	 // Invalid character
	}
}
