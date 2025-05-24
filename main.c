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

#define MAX_LEDS 16

enum
{
	LED_SELECTOR = 0,
	NOT_LED,
	OFF_ALL,
	ON_ALL,
	FINISH
};

enum
{
	STOP = 0,
	RUNNING,
};

void showMenu1(int *ledCount, uint32_t *ledMask);
void showMenu2(int *ledCount);

int main(void)
{
	uint8_t prog = RUNNING;
	int ledCount;
	uint32_t ledMask;
	showMenu1(&ledCount, &ledMask);
	for(uint8_t i = 0; i < ledCount; i++)
	{
		GPIO_PinInit(PORTD, i, OUTPUT);
	}
	GPIO_SetMaskedOutput(PORTD, ledMask, HIGH);
	display_leds(ledCount, ledMask);

	while(prog)
	{
		showMenu2(ledCount);

		uint8_t selection = readMenuOption();

		switch(selection)
		{
			case LED_SELECTOR:
				break;

			case NOT_LED:
				for(int i = 0; i < ledCount; i++)
				{
					GPIO_SetPinState(PORTD, i, TOGGLE);
				}
				display_leds(ledCount, 0x);
				break;

			case OFF_ALL:
				ledMask = 1;
				for(int i = 0; i < ledCount; i++)
				{
					ledMask <<= 1;
					GPIO_SetPinState(PORTD, i, LOW);
					ledMask += 1;
				}
				display_leds(ledCount, 0x00);
				break;

			case ON_ALL:
				for(int i = 0; i < ledCount; i++)
				{
					GPIO_SetPinState(PORTD, i, HIGH);
				}
				display_leds(ledCount, 0xFF);
				break;

			case FINISH:
				prog = STOP;
				break;

			default:
				printf("\t\t\t[ERROR] Invalid command.\n");
				break;
		}
	}

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

		if ((*ledMask & ~allowedMask) != 0)
		{
			printf("\t\t\t[ERROR] Mask contains bits outside the range of %d LEDs.\n", *ledCount);
			printf("\t\t\tAllowed mask: 0x%02X\n", allowedMask);
			validation_led = 0;
		}
	}while(!validation_led);
}

void showMenu2(int *ledCount) {
    printf("\t\t\t=========== LED SIMULATOR MENU ===========\n");
    printf("\t\t\t [0-%d]  Turn ON a specific LED\n", *ledCount);
    printf("\t\t\t [t]    Toggle all LEDs\n");
    printf("\t\t\t [c]    Clear (turn off) all LEDs\n");
    printf("\t\t\t [s]    Set (turn on) all LEDs\n");
    printf("\t\t\t [q]    Quit program\n");
    printf("\t\t\t==========================================\n");
    ClearInputBuffer();
    CLEAR_SCREEN();
}
