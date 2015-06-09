#include "serial.h"
#include "fifo.h"
#include "types.h"
#include "clock.h"

#include <msp430.h>
#include <signal.h>

static void serial_setBaud( int uart, uint32_t baud_rate, uint16_t clock_source );
static void serial_setMode( int uart, uint8_t mode );

Fifo_t serial_rxFifo;

int serial_init( int uart, uint8_t mode, uint32_t baud_rate, uint16_t clock_source )
{
	Fifo_init( &serial_rxFifo );

	/* select UART pins TX=P3.4, RX=P3.5 */
    P3SEL |= ( 1 << 4 ) | ( 1 << 5 );

	/* keep in soft reset while configuring */
	UCTL0  = SWRST;

	/* select clock source */
	if( clock_source == SMCLK )
		UTCTL0 = SSEL1;
	else
		UTCTL0 = SSEL0;	/* Defaults to ACLK */

	/* configure Rx Ctrl Register. */
	URCTL0 = 0;

	serial_setMode( uart, mode );
    serial_setBaud( uart, baud_rate, clock_source );

    /* enable transmit and receive */
    ME1 |= URXE0 | UTXE0;

    /* remove reset */
    UCTL0 &= ~SWRST;

    /* enable receive interrupt for USART0 */
    IE1 |= URXIE0;

	return 1;
}

int serial_uninit( int uart )
{
 	/* keep in soft reset */
	UCTL0 |= SWRST;
	/* disable transmit and receive */
    ME1   &= ~( URXE0 | UTXE0 );
	return 1;
}

int serial_available( int uart )
{
    return Fifo_available( &serial_rxFifo );
}

int serial_read( int uart )
{
	return Fifo_pop( &serial_rxFifo );
}


int serial_write( int uart, char c )
{
	/* Wait until prev char transmited. */
	while( ( IFG1 & UTXIFG0 ) == 0 );
	U0TXBUF = c;

	return 1;
}

int serial_flush( int uart )
{
	Fifo_init( &serial_rxFifo );
	return 1;
}

int serial_putstr( int uart, const char* str )
{
	while( *str )
		serial_write( uart, *str++ );

	return 1;
}


static void serial_setBaud( int uart, uint32_t baud_rate, uint16_t clock_source )
{
    // Assumes uart 0
	( void )uart;

	/* Set the baudrate dividers and modulation */
	uint32_t clk = clock_get( clock_source );
	unsigned int N_div;
	N_div = clk / baud_rate;

	float N_div_f;
	N_div_f = (float)clk / (float)baud_rate;

	UBR00 = ( unsigned char )( N_div & 0x00FF );
	UBR10 = ( unsigned char )( ( N_div & 0xFF00 ) >> 8 );
	UMCTL0 = ( unsigned char )( ( ( N_div_f - ( int )( N_div_f ) ) ) * 8.0f ) << 1; // Set BRS
}

static void serial_setMode( int uart, uint8_t mode )
{
	// Assumes uart 0.
	( void )uart;

	uint8_t uctl = 0;

	/* Data Word Length */
	if( mode & CHAR_8BIT )
		uctl |= CHAR;

	/* Stop Bits */
	if( mode & SPB_TWO )
		uctl |= SPB;

	/* Parity */
	if( mode & PAR_EVEN )
	{
		uctl |= PEV;
		uctl |= PENA;
	}
	else if( mode & PAR_ODD )
	{
		uctl &= ~PEV;
		uctl |= PENA;
	}

	UCTL0 |= uctl;
}

__attribute__( ( __interrupt__( USART0RX_VECTOR ) ) )
void Serial_UART0_IRQ(void)
{
	uint8_t byte = RXBUF0;
	Fifo_push( &serial_rxFifo, byte );
	__bic_SR_register_on_exit(LPM3_bits);
}

