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

/**
 * @brief Defines a callback function type for parsing input segments.
 * The callback receives a string and a generic pointer to the destination variable.
 */
typedef uint8_t (* scanCallback) (const char *str, void *dest);

/**
 * @brief Clears the input buffer (stdin) to remove any remaining characters after reading.
 */
void ClearInputBuffer(void);

/**
 * @brief Reads and parses input from stdin using an array of callbacks.
 * Each callback is responsible for converting one argument from string to its proper type.
 *
 * @param callbacks      Array of functions to convert each argument.
 * @param destinations   Array of pointers to the destination variables.
 * @param count          Number of expected arguments (callbacks/destinations).
 */
void nscanf(scanCallback *callbacks, void **destinations, uint8_t count);

/**
 * @brief Callback to convert a string to an integer.
 *
 * @param str   Input string to convert.
 * @param dest  Pointer to an integer variable to store the result.
 * @return 1 if successful, 0 otherwise.
 */
uint8_t readInt(const char *str, void *dest);

/**
 * @brief Callback to convert a string to a floating-point number.
 *
 * @param str   Input string to convert.
 * @param dest  Pointer to a float variable to store the result.
 * @return 1 if successful, 0 otherwise.
 */
uint8_t readFloat(const char *str, void *dest);

/**
 * @brief Callback to convert a string in hexadecimal format to a uint32_t.
 *
 * @param str   Input string to convert.
 * @param dest  Pointer to a uint32_t variable to store the result.
 * @return 1 if successful, 0 otherwise.
 */
uint8_t readHex(const char *str, void *dest);

/**
 * @brief Callback to read a single character from a string.
 *
 * @param str   Input string (should contain only one character).
 * @param dest  Pointer to a char variable to store the result.
 * @return 1 if successful, 0 otherwise.
 */
uint8_t readChar(const char *str, void *dest);

/**
 * @brief Callback to copy a string into a destination buffer.
 *
 * @param str   Input string.
 * @param dest  Pointer to the destination character array.
 */
uint8_t readString(const char *str, void *dest);



#endif /* _NSTDIO_H_ */
