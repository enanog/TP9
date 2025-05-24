/* ---------------------------------------------------
 * nstdio.h
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

#ifndef _NSTDIO_H_
#define _NSTDIO_H_

typedef void (* scanCallback) (const char *segment, void *dest);

void nscaf(scanCallback *callbacks, void **destinations, uint8_t count);
uint8_t readInt(const char *str, void *dest);
uint8_t readFloat(const char *str, void *dest);
uint8_t readHex(const char *str, void *dest);
uint8_t readChar(const char *str, void *dest);
void readString(const char *str, void *dest);



#endif /* LIBS_NSTDIO_NSTDIO_H_ */
