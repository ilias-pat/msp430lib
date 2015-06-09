#ifndef SERIAL_H_
#define SERIAL_H_

#include "Types.h"

#ifdef __cplusplus
extern "C" {
#endif

/** Serial Mode definitions */
#define CHAR_7BIT		( 0x01 )	/**< 7-bit character length */
#define CHAR_8BIT		( 0x02 )	/**< 8-bit character length */
#define SPB_ONE			( 0x00 )	/**< 1 stop bit */
#define SPB_TWO			( 0x04 )	/**< 2 stop bits */
#define PAR_NONE		( 0x00 )	/**< No parity */
#define PAR_ODD			( 0x08 )	/**< Odd parity */
#define PAR_EVEN		( 0x10 )	/**< Even parity */

/**
 * Initializes the UART hardware.
 * @param[in] uart			Specifies the MCU USART port to operate on.
 * @param[in] mode 			This is a bitfield that specified the Serial configuration.
 * @param[in] baud_rate		The serial baud rate.
 * @param[in] clock_source 	The clock source of the MCU USART, one of ACLK or SMCLK.
 * @return	Returns 1.
 * @attention The clock source must operate at a sufficient frequency to support the
 * requested baud rate.
 */
int serial_init( int uart, uint8_t mode, uint32_t baud_rate, uint16_t clock_source );

/**
 * Stops the UART hardware.
 * @param[in] uart			Specifies the MCU USART port to operate on.
 * @return	Returns 1.
 */
int serial_uninit( int uart );

/**
 * Tests if there are any characters available in the receive FIFO.
 * @param[in] uart			Specifies the MCU USART port to operate on.
 * @return	Returns the number of bytes available in the receive FIFO.
 */
int serial_available( int uart );

/**
 * Reads a character.
 * @param[in] uart			Specifies the MCU USART port to operate on.
 * @return	The received character or EOF if not character available.
 */
int serial_read( int uart );

/**
 * Writes a character.
 * @param[in] uart			Specifies the MCU USART port to operate on.
 * @param[in] c				The character.
 * @return	Returns 1.
 */
int serial_write( int uart, char c );

/**
 * Flushes the UART and discards all the characters received.
 * @param[in] uart			Specifies the MCU USART port to operate on.
 * @return	Returns 1.
 */
int serial_flush( int uart );

/**
 * Writes a null-terminated string.
 * @param[in] uart			Specifies the MCU USART port to operate on.
 * @param[in] c				The string.
 * @return Returns 1.
 */
int serial_putstr( int uart, const char* str );

/** Writes a character. Alias for @ref serial_write( ). */
#define serial_putchar( uart, c )		serial_write( uart, c )

/** Reads a character. Alias for @ref serial_read( ). */
#define serial_getchar( uart )			serial_read( uart )

#ifdef __cplusplus
}
#endif

#endif
