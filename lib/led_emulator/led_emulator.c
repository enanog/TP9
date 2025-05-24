/* ---------------------------------------------------
 * leds_emulator.c
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

#include "led_emulator.h"
#include <unistd.h>			// Include for usleep function
#include <stdio.h>

// Macro to get the value of bit n from variable x
#define _bitGet(x, n) (((x) >> (n)) & 1)

// Macro to move the cursor to a specific row and column on the terminal using ANSI escape codes
#define GOTOXY(row, col) printf("\033[%d;%dH", row, col)

/**
 * @brief Draws the outline of a single LED using ASCII characters at the given position.
 *
 * @param row Starting row for LED drawing.
 * @param col Starting column for LED drawing.
 */
static void print_led_outline(int row, int col);


/**
 * @brief Fills a previously drawn LED with red color to indicate it's ON.
 *
 * @param row Starting row of the LED.
 * @param col Starting column of the LED.
 */
static void fill_led_on(int row, int col);


/**
 * @brief Clears the fill of a previously drawn LED to indicate it's OFF.
 *
 * @param row Starting row of the LED.
 * @param col Starting column of the LED.
 */
static void fill_led_off(int row, int col);


/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


void display_leds(uint8_t cant, uint32_t mask)
{
	CLEAR_SCREEN();	// Clears the terminal screen before drawing

	// Draw the outline for each LED
	for (int i = 0; i < cant; i++) {
		int col = 2 + i * 10;
		print_led_outline(2, col);
	}

	usleep(20000);

	// Fill each LED based on the bitmask (ON or OFF)
	for (int i = 0; i < cant; i++)
	{
		int col = 2 + i * 10;

		if (_bitGet(mask, i))
			fill_led_on(2, col);
		else
			fill_led_off(2, col);
	}

	GOTOXY(15, 1); // Move cursor out of the LED area, below them
}

/*******************************************************************************
 *******************************************************************************
                             LOCAL DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

static void print_led_outline(int row, int col) {
	GOTOXY(row, col);     printf("  ____  ");
	GOTOXY(row + 1, col); printf(" /    \\ ");
	GOTOXY(row + 2, col); printf("|      |");
	GOTOXY(row + 3, col); printf("|      |");
	GOTOXY(row + 4, col); printf("|      |");
    GOTOXY(row + 5, col); printf("|      |");
    GOTOXY(row + 6, col); printf("|______|");
    GOTOXY(row + 7, col); printf("  |  |  ");
    GOTOXY(row + 8, col); printf("  |  |  ");
}

static void fill_led_on(int row, int col) {
	GOTOXY(row + 1, col + 2);
	printf("\033[41m    \033[0m");
    for (int i = 2; i <= 6; i++) {
    	GOTOXY(row + i, col + 1);
        printf("\033[41m      \033[0m");
    }
}

static void fill_led_off(int row, int col) {
	GOTOXY(row + 1, col + 2);
	printf("    ");
    for (int i = 2; i <= 5; i++) {
    	GOTOXY(row + i, col + 1);
        printf("      ");
    }
}
