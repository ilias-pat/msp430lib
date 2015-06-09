#ifndef DELAY_H_
#define DELAY_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Returns the number of milliseconds since power-up.
 * @return		Milliseconds count.
 */
unsigned long millis( void );

/**
 * Calculates the elapsed number of milliseconds since
 * @ref start. This funtion will account for any overflow
 * that may have occured since start.
 * @param[in]	start	Initial timestamp in milliseconds.
 * @return		Elapsed milliseconds.
 */
unsigned long elapsed_millis( unsigned long start );

/**
 * Delays for the specified number of milliseconds.
 * @param[in]	msec	Number of msecs to delay.
 */
void delay( unsigned long msec );

#ifdef __cplusplus
}
#endif

#endif
