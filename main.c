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



int main() {
    GPIO_MaskInit(PORTA, 0xFF, OUTPUT);
	GPIO_SetMaskedOutput(PORTA, 0xFF, OUTPUT);

	uint8_t prog = RUNNING;

	scanCallback callback[] = { readInt, readFloat, readHex, readChar, readString};
	int a;
	float b;
	uint32_t c;
	char d;
	char e[20];
	void *destinations[]={ &a, &b, &c, &d, e};

	nscanf(callback, destinations, 5);
	printf("%d %f %X %c %s\n", a, b, c, d, e);

//	showMenu();
//
//	display_leds(15, 0xff);
//	showMenu();
    return 0;
}

void showMenu() {
    printf("\t\t\t=========== LED SIMULATOR MENU ===========\n");
    printf("\t\t\t [0-7]  Turn ON a specific LED\n");
    printf("\t\t\t [t]    Toggle all LEDs\n");
    printf("\t\t\t [c]    Clear (turn off) all LEDs\n");
    printf("\t\t\t [s]    Set (turn on) all LEDs\n");
    printf("\t\t\t [q]    Quit program\n");
    printf("\t\t\t==========================================\n");
    printf("\n\n\n\t\t\t\t\tPress a key to continue...");
    ClearInputBuffer();
    CLEAR_SCREEN();
}

//int main(void)
//{
////	uint8_t prog = RUNNING;
//	GPIO_MaskInit(PORTA, 0xFF, OUTPUT);
//	GPIO_SetMaskedOutput(PORTA, 0xFF, OUTPUT);
//	/*while(prog)
//	{
//
//		switch(prog)
//		{
//			case LED_SELECTOR:
//				break;
//
//			case NOT_LED:
//				break;
//
//			case OFF_ALL:
//				break;
//
//			case ON_ALL:
//				break;
//
//			case FINISH:
//				prog = STOP;
//				break;
//
//			default:
//				printf("Se ingreso un comando erroneo.\n");
//				break;
//		}
//	}*/
//
//	return 0;
//}

