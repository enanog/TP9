/* ---------------------------------------------------
 * leds_emulator.h
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

#ifndef LED_EMULATOR_H
#define LED_EMULATOR_H

#define CLEAR_SCREEN(void) printf("\033[2J")

/**
 * @brief: Displays a number of LEDs on the screen with states indicated by a bitmask.
 *
 * @param cant Number of LEDs to render (usually 8).
 * @param mask Bitmask representing which LEDs should be ON (1) or OFF (0).
 */
void display_leds(uint8_t cant, uint32_t mask);

#endif /* LEDS_EMULATOR_H */
