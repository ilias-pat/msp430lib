#ifndef SYSTICK_H_
#define SYSTICK_H_

#define SYSTICK_INTERRUPT_PRIORITY	0

typedef void ( *SystickCallback_t )( void );

extern volatile unsigned long _system_ticks;
extern int _system_tick_interval_ms;

// minimum is 1ms, maximum is 1000ms
// callback can be NULL
int systick_init( int tick_interval_ms, SystickCallback_t callback );

#endif
