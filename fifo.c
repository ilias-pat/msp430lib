#include "fifo.h"
#include "types.h"

void Fifo_init( Fifo_t* fifo )
{
	fifo->rpos = 0;
	fifo->wpos = 0;
}

void Fifo_push( Fifo_t* fifo, uint8_t byte )
{
	 if( ( fifo->wpos + 1 ) % FIFO_BUFFER_SIZE == fifo->rpos )
		 return;

	 fifo->buffer[fifo->wpos] = byte;
	 fifo->wpos = ( fifo->wpos + 1 ) % FIFO_BUFFER_SIZE;
}

uint8_t Fifo_pop( Fifo_t* fifo )
{
	if( fifo->rpos == fifo->wpos )
		return EOF;

	char ch = fifo->buffer[fifo->rpos];
	fifo->rpos = ( fifo->rpos + 1 ) % FIFO_BUFFER_SIZE;

	return ch;
}

uint16_t Fifo_size( Fifo_t* fifo )
{
	return ( fifo->wpos + FIFO_BUFFER_SIZE - fifo->rpos ) % FIFO_BUFFER_SIZE;
}
