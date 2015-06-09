/**
 * @Brief Implements a software timer.
 *
 * The software timer (or just a 'timer') allows a function to
 * be executed at a set time in the future. The function executed
 * by the timer is the timer’s callback function. The time between
 * a timer being started, and its callback function being executed,
 * is the timer’s period. Put simply, the timer's callback function
 * is executed when the timer's period expires.
 *
 * This module maintains a linked list of the software timers that
 * have been started. Hardware Timer0 is used to keep track of when
 * a software timer expires. This minimizes the hardware resources
 * used in the micro-controller to only one hardware timer.
 *
 * The software timer supports two modes:
 * 1) Periodic: the timer expires and restarts.
 * 2) One-shot: the timer expires and stops.
 *
 * This module also provides a facility for microsecond delay.
 *
 * @warning For the microsecond delay to work correctly, sufficient
 * timer resolution must be configured in the @ref TIMER_RESOLUTION_USEC
 *
 * @warning The software timer structure must not be destroyed after the
 * timer is started. It is usually a global variable. This module does not
 * maintain copies of the structures passed to it.
 *
 * @Author iliaspat
 *
 */
#ifndef TIMER_H_
#define TIMER_H_

#include "types.h"

/* Prevent clashing with timer_t in sys/types.h */
#define __timer_t_defined

/**
 * Periodic Timer Mode: the timer expires and restarts.
 */
#define TIMER_MODE_PERIODIC		0x00

/**
 * One-shot Timer Mode: the timer expires and stops.
 */
#define TIMER_MODE_ONESHOT		0x01

/**
 * This macro controls the resolution of the timer.
 * The value is in milliseconds.
 */
#define TIMER_RESOLUTION_MSEC	1

/**
 * The software timer structure.
 * mode, period_usec, callback and user must be
 * set by the user.
 */
typedef struct _timer
{
	int mode;						/**< Timer mode, one of @ref TIMER_MODE_PERIODIC or @ref TIMER_MODE_ONESHOT */
	int period_msec;				/**< The period of the timer in microsec. This is the period before the timer expires. */
	void ( *callback )( void* );	/**< The callback fucntion to be called when the timer expires. Can be NULL. */
	void* user;						/**< A user provided variable that is passed in the callback function. */

	// private - do not use.
	int ticks_remaining;
	struct _timer* next;
} timer_t;

/**
 * Initialize the hardware Timer. Must be called first,
 * before any software timer is started by calling @ref
 * timer_start( ).
 * @param[in] clock_source	The clock source used to clock the timer module (SMCLK or ACLK).
 * @param[in] divider		Clock source divider, one of 1, 2, 4, 8.
 */
void timer_init( uint16_t clock_source, uint8_t divider );

/**
 * Stop hardware Timer0 to conserve power.
 */
void timer_uninit( void );

/**
 * Start a software timer.
 * @param[in] timer		The timer to be started.
 */
void timer_start( timer_t* timer );

/**
 * Stop a software timer.
 * @param[in] timer		The timer to be stopped.
 */
void timer_stop( timer_t* timer );

/**
 * Reset a software timer.
 * @param[in] timer		The timer to be reset.
 */
void timer_reset( timer_t* timer );

/**
 * Returns number of milliseconds since start of
 * timer.
 * @return milliseconds.
 */
unsigned long timer_millis( void );

#endif
