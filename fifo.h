#ifndef FIFO_H_
#define FIFO_H_

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

/** Number of bytes that can be stored in the FIFO */
#define FIFO_BUFFER_SIZE	128

/**
 * FIFO structure.
 */
typedef struct
{
	uint8_t buffer[FIFO_BUFFER_SIZE];
	volatile uint16_t wpos;
	volatile uint16_t rpos;
} Fifo_t;

/**
 * Initialise the FIFO.
 * @param[in] fifo		The FIFO structure.
 */
void Fifo_init( Fifo_t* fifo );

/**
 * Write a byte at the end of the FIFO.
 * @param[in] fifo		The FIFO structure.
 * @param[in] byte		Byte to write.
 */
void Fifo_push( Fifo_t* fifo, uint8_t byte );

/**
 * Read a byte from the start of the FIFO.
 * @param[in] fifo		The FIFO structure.
 * @return				Byte
 */
uint8_t Fifo_pop( Fifo_t* fifo );

/**
 * Returns the number of bytes currently stored in the FIFO.
 * @return				FIFO size in bytes.
 */
uint16_t Fifo_size( Fifo_t* fifo );

/** Returns 1 if FIFO empty, 0 otherwise. */
#define Fifo_empty( fifo )		( Fifo_size( ( fifo ) ) != 0 )

/** Returns 1 if FIFO full, 0 otherwise. */
#define Fifo_full( fifo )		( Fifo_size( ( fifo ) ) != 128 )

/** Returns the number of bytes available in the FIFO. */
#define Fifo_available( fifo )	( Fifo_size( ( fifo ) ) )

#ifdef __cplusplus
}
#endif

#endif
