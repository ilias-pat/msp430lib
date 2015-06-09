#include "timer.h"
#include "types.h"
#include "clock.h"
#include <signal.h>
#include <msp430.h>

static timer_t* _timer_list_head;
static volatile uint32_t _timer_ticks;

static int timer_list_exists( timer_t* timer );
static void timer_list_add( timer_t* new );
static void timer_list_remove( timer_t* timer );
static timer_t* timer_list_find_previous( timer_t* timer );
static timer_t* timer_list_find_last( void );

void timer_init( uint16_t clock_source, uint8_t divider )
{
	/* Use TimerA to create periodic interrupts */
	/* Clear TimerA */
	TACTL = TACLR;

	/* Set clock divider */
	switch( divider )
	{
	default:
	case 1: TACTL |= ID_0; break;
	case 2: TACTL |= ID_1; break;
	case 4: TACTL |= ID_2; break;
	case 8: TACTL |= ID_3; break;
	}

	/* Set clock source */
	if( clock_source == SMCLK )
		TACTL |= TASSEL1;
	else
		TACTL |= TASSEL0;	/* Default to ACLK */

	/* Set Capture/Compare Register */
	uint32_t tmr_clk = clock_get( clock_source ) / divider;
	TACCR0 = ( TIMER_RESOLUTION_MSEC * tmr_clk ) / 1000 ;

	/* Enable Capture/Compare Interrupt */
	TACCTL0 = CCIE;

	/* Start timer in up to CCR0 mode */
	TACTL |= MC_1;
}

void timer_uninit( void )
{
	TACTL |= MC_0;
}

void timer_start( timer_t* timer )
{
	if( timer && timer_list_exists( timer ) )
		return;

	timer->next = NULL;
	timer->ticks_remaining = timer->period_msec / TIMER_RESOLUTION_MSEC;

	timer_list_add( timer );
}

void timer_stop( timer_t* timer )
{
	if( timer && timer_list_exists( timer ) )
		timer_list_remove( timer );
}

void timer_reset( timer_t* timer )
{
	timer->ticks_remaining = timer->period_msec / TIMER_RESOLUTION_MSEC;
}

uint32_t timer_millis( void )
{
	return ( _timer_ticks * TIMER_RESOLUTION_MSEC );
}

static int timer_list_exists( timer_t* timer )
{
	timer_t* it = _timer_list_head;

	for( ; it ; it = it->next )
	{
		if( it == timer )
			return 1;
	}

	return 0;
}

static void timer_list_add( timer_t* new )
{
	if( !_timer_list_head )
	{
		/* This is the first timer to be added. */
		_timer_list_head = new;
	}
	else
	{
		/* Add new Timer at the end of the list. */
		timer_t* last = timer_list_find_last( );
		last->next = new;
	}
}

static void timer_list_remove( timer_t* timer )
{
	if( _timer_list_head == timer )
	{
		/* Removing first element of list. */
		_timer_list_head = timer->next;
		timer->next = 0;
	}
	else
	{
		timer_t* prev = timer_list_find_previous( timer );
		prev->next = timer->next;
		timer->next = 0;
	}
}

static timer_t* timer_list_find_previous( timer_t* timer )
{
	timer_t* it = _timer_list_head;

	for( ; it ; it = it->next )
	{
		if( it->next == timer )
			return it;
	}

	return NULL;
}

static timer_t* timer_list_find_last( void )
{
	timer_t* it = _timer_list_head;

	for( ; it ; it = it->next )
	{
		if( it->next == NULL )
			return it;
	}

	return NULL;
}

__attribute__ ( ( interrupt ( TIMERA0_VECTOR ) ) )
void TIMERA_IRQHandler( void )
{
	timer_t* it = _timer_list_head;

	/* Used for delay function. */
	_timer_ticks++;

	for( ; it ; it = it->next )
	{
		it->ticks_remaining--;
		if( it->ticks_remaining <= 0 )
		{
			/* Timer has expired. Invoke callback
			 * and reset/remove as necessary.
			 */
			if( it->callback != NULL )
				it->callback( it->user );

			if( it->mode == TIMER_MODE_PERIODIC )
			{
				timer_reset( it );
			}
			else
			{
				timer_list_remove( it );
			}
		}
	}
}

