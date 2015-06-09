#ifndef CHECKSUM_H_
#define CHECKSUM_H_

#include "types.h"

/**
 * Calculates an 8-bit sum of all the bytes in the supplied buffer.
 * @param[in] buffer	Buffer that holds the data.
 * @param[in] size		Buffer size
 * @return				8-bit checksum.
 */
uint8_t calculate_checksum( const uint8_t* buffer, int size );

#endif
