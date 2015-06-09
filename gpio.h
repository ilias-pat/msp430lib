#ifndef GPIO_H_
#define GPIO_H_

#include "types.h"

/** GPIO Mode */
#define  INPUT 		0
#define  OUTPUT 	1

/** GPIO pull */
#define  NOPULL  	0
#define  PULLUP  	1
#define  PULLDOWN  	2

/** GPIO value */
#define  LOW		0
#define  HIGH		1

/** GPIO interrupt mode */
#define LEVEL		0
#define RISING		1
#define FALLING		2

/**
 * Sets the GPIO pin direction.
 * @param[in] port		GPIO port, 1 to 6
 * @param[in] bit		GPIO port bit, 0 to 7
 * @param[in] mode		@ref INPUT or @ref OUTPUT
 */
void GPIO_mode( uint8_t port, uint8_t bit, uint8_t mode );

/**
 * Sets the state of the specified GPIO pin.
 * @param[in] port		GPIO port, 1 to 6
 * @param[in] bit		GPIO port bit, 0 to 7
 * @param[in] value		@ref LOW or @ref HIGH
 */
void GPIO_set( uint8_t port, uint8_t bit, uint8_t value );

/**
 * Returns the state of the specified GPIO  pin.
 * @param[in] port		GPIO port, 1 to 6
 * @param[in] bit		GPIO port bit, 0 to 7
 * @return 				@ref LOW or @ref HIGH
 */
uint8_t GPIO_get( uint8_t port, uint8_t bit );

/**
 * Toggles the state of the specified GPIO  pin.
 * @param[in] port		GPIO port, 1 to 6
 * @param[in] bit		GPIO port bit, 0 to 7
 */
void GPIO_toggle( uint8_t port, uint8_t bit );

/**
 * Sets the internal Pull-up/Pull-down resistors of
 * the specified GPIO pin.
 * @param[in] port		GPIO port, 1 to 6
 * @param[in] bit		GPIO port bit, 0 to 7
 * @param[in] pull		@ref NOPULL, @ref PULLUP or @ref PULLDOWN
 */
void GPIO_pull( uint8_t port, uint8_t bit, uint8_t pull );

/**
 * Enables the GPIO interrupt of the specified GPIO pin.
 * @param[in] port		GPIO port, 1 to 6
 * @param[in] bit		GPIO port bit, 0 to 7
 * @param[in] callback	Function to be called on interrupt. Can be NULL.
 * @param[in] mode		@ref LEVEL, @ref RISING edge or @ref FALLING  edge.
 */
void GPIO_attachInterrupt( uint8_t port, uint8_t bit, void ( *callback )( unsigned int ), uint8_t mode );

/**
 * Disables the GPIO interrupt of the specified GPIO pin.
 * @param[in] port		GPIO port, 1 to 6
 * @param[in] bit		GPIO port bit, 0 to 7
 */
void GPIO_detachInterrupt( uint8_t port, uint8_t bit );


//////////////////////////////////////////////////////////////////////////////////////////////////
// Arduino interface
//////////////////////////////////////////////////////////////////////////////////////////////////
#include "pin_map.h"

/**
 * Sets the GPIO pin direction.
 * @param[in] pin		GPIO pin as specified in @ref pin_map.h
 * @param[in] mode		@ref INPUT or @ref OUTPUT
 */
void pinMode( int pin, int mode );

/**
 * Sets the state of the specified GPIO pin.
 * @param[in] pin		GPIO pin as specified in @ref pin_map.h
 * @param[in] value		@ref LOW or @ref HIGH
 */
void digitalWrite( int pin, int value );

/**
 * Returns the state of the specified GPIO pin.
 * @param[in] pin		GPIO pin as specified in @ref pin_map.h
 * @param[in] value		@ref LOW or @ref HIGH
 */
int digitalRead( int pin );

/**
 * Enables the GPIO interrupt of the specified GPIO pin.
 * @param[in] pin		GPIO pin as specified in @ref pin_map.h
 * @param[in] callback	Function to be called on interrupt. Can be NULL.
 * @param[in] mode		@ref LEVEL, @ref RISING edge or @ref FALLING  edge.
 */
void attachInterrupt( int pin, void ( *callback )( unsigned int ), int mode );

/**
 * Disables the GPIO interrupt of the specified GPIO pin.
 * @param[in] pin		GPIO pin as specified in @ref pin_map.h
 */
void detachInterrupt( int pin );

#endif
