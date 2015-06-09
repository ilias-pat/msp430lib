#include "systick.h"
#include "timer.h"
#include "types.h"

volatile unsigned long _system_ticks;
int _system_tick_interval_ms;

static SystickCallback_t _systick_callback;
static timer_t _systick_timer;

static void systick_handler( void* user );

int systick_init( int tick_interval_ms, SystickCallback_t callback )
{
	_system_ticks = 0;
	_systick_callback = callback;
	//_system_tick_interval_ms = tick_interval_ms;

	_systick_timer.mode = TIMER_MODE_PERIODIC;
	_systick_timer.period_msec = tick_interval_ms;
	_systick_timer.callback = systick_handler;
	_systick_timer.user = NULL;
	timer_start( &_systick_timer );

	return 1;
}

static void systick_handler( void* user )
{
	_system_ticks++;

	if( _systick_callback )
		_systick_callback( );
}
