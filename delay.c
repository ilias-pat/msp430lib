#include "delay.h"
#include "timer.h"
#include "types.h"

unsigned long millis( void )
{
	//return _system_ticks * _system_tick_interval_ms;
	return timer_millis( );
}

unsigned long elapsed_millis( unsigned long start )
{
	unsigned long current = millis( );
	unsigned long elapsed = 0;

	if( current < start ) // overflow
		elapsed = UINT32_MAX - start + current;
	else
		elapsed = current - start;

	return elapsed;
}

void delay( unsigned long msec )
{
	unsigned long start = millis();
	while( elapsed_millis( start ) < msec );
}
