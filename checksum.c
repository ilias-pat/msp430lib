#include "checksum.h"
#include "types.h"

uint8_t calculate_checksum( const uint8_t* buffer, int size )
{
	uint8_t checksum = 0;
	int i = 0;
	for( ; i<size; i++ )
		checksum += buffer[i];

	return checksum;
}
