#include "gpio.h"
#include "types.h"

#include <msp430.h>

const volatile struct
{
	const volatile unsigned char* PIN;
	volatile unsigned char* POUT;
	volatile unsigned char* PDIR;
	volatile unsigned char* PSEL;
} GPIO_portTable[] =
{
	{ .PIN  = &P1IN, .POUT  = &P1OUT, .PDIR = &P1DIR, .PSEL = &P1SEL },
	{ .PIN  = &P2IN, .POUT  = &P2OUT, .PDIR = &P2DIR, .PSEL = &P2SEL },
	{ .PIN  = &P3IN, .POUT  = &P3OUT, .PDIR = &P3DIR, .PSEL = &P3SEL },
	{ .PIN  = &P4IN, .POUT  = &P4OUT, .PDIR = &P4DIR, .PSEL = &P4SEL },
	{ .PIN  = &P5IN, .POUT  = &P5OUT, .PDIR = &P5DIR, .PSEL = &P5SEL },
	{ .PIN  = &P6IN, .POUT  = &P6OUT, .PDIR = &P6DIR, .PSEL = &P6SEL }
};

void GPIO_set( uint8_t port, uint8_t bit, uint8_t value )
{
	if( value )
		*GPIO_portTable[port-1].POUT |= ( 1 << bit );
	else
		*GPIO_portTable[port-1].POUT &= ~( 1 << bit );
}

uint8_t GPIO_get( uint8_t port, uint8_t bit )
{
	if( *GPIO_portTable[port-1].PIN & ( 1 << bit ) )
		return HIGH;
	else
		return LOW;
}

void GPIO_toggle( uint8_t port, uint8_t bit )
{
	*GPIO_portTable[port-1].POUT ^= ( 1 << bit );
}

void GPIO_mode( uint8_t port, uint8_t bit, uint8_t mode )
{
	/* Select GPIO function */
	*GPIO_portTable[port-1].PSEL &= ( 1 << bit );

	if( mode == OUTPUT )
		*GPIO_portTable[port-1].PDIR |= ( 1 << bit );
	else
		*GPIO_portTable[port-1].PDIR &= ~( 1 << bit );
}

// TODO
//void GPIO_pull( uint8_t port, uint8_t bit, uint8_t pull );
//void GPIO_attachInterrupt( uint8_t port, uint8_t bit, void ( *callback )( unsigned int ), uint8_t mode );
//void GPIO_detachInterrupt( uint8_t port, uint8_t bit );

// Arduino interface
#include "pin_map.h"

void digitalWrite( int pin, int value )
{
	uint8_t port = mapPinToPort( pin );
	uint8_t bit  = mapPinToBit( pin );
	GPIO_set( port, bit, value );
}

int digitalRead( int pin )
{
	uint8_t port = mapPinToPort( pin );
	uint8_t bit  = mapPinToBit( pin );
	return GPIO_get( port, bit );
}

void pinMode( int pin, int mode )
{
	uint8_t port = mapPinToPort( pin );
	uint8_t bit  = mapPinToBit( pin );
	GPIO_mode( port, bit, mode );
}

// TODO
//void attachInterrupt( int pin, void ( *callback )( unsigned int ), int mode );
//void detachInterrupt( int pin );
