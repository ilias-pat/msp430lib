#include "spi.h"
#include "types.h"
#include "clock.h"

#include <msp430.h>

#define SPI_CLK_SRC		SMCLK
#define DUMMY			( 0xFF )

void SPI_init( uint8_t spi_port, uint8_t mode, uint32_t clock_rate, uint16_t clock_source )
{
	/* Currently only port 1 is supported. */
	( void )spi_port;

	/* configure pins for USART1 SPI:
	 * P5.0: STE1 (unused), P5.1: SIMO1, P5.2: SOMI1, P5.3: UCLK */
	P5SEL |= ( 1 << 1 ) | ( 1 << 2 ) | ( 1 << 3 );

    /* Keep in reset while configuring. */
	UCTL1 = SWRST;

	/* Enable USART1 SPI mode. */
	ME2 |= USPIE1;

	/* Master mode, 8bit dword. */
	UCTL1 |= SYNC | MM | CHAR;

	/* 3-pin SPI mode | CPOL/CPHA conf */
	UTCTL1 = STC | mode ;

	/* Configure Clock Source */
	UTCTL1 &= ~( SSEL0 | SSEL1 );
	if( clock_source == SMCLK )
		UTCTL1 |= SSEL1;
	else
		UTCTL1 |= SSEL0;	/* defaults to ACLK */

	/* Set the baudrate dividers and modulation */
	uint32_t spi_clk = clock_get( clock_source );
	unsigned int N_div;
	N_div = spi_clk / clock_rate;

	float N_div_f;
	N_div_f = (float)spi_clk / (float)clock_rate;

	UBR01 = ( unsigned char )( N_div & 0x00FF );
	UBR11 = ( unsigned char )( ( N_div & 0xFF00 ) >> 8 );
	UMCTL1 = ( unsigned char )( ( ( N_div_f - ( int )( N_div_f ) ) ) * 8.0f ) << 1; // Set BRS

    /* Remove reset */
    UCTL1 &= ~SWRST;
}

void SPI_uninit( uint8_t spi_port )
{
	/* Currently only port 1 is supported. */
	( void )spi_port;

    UCTL1 = SWRST;
    UTCTL1 = SYNC;
    IE2 = 0;
}

void SPI_configClock( uint8_t spi_port, uint32_t clock_rate, uint16_t clock_source )
{
	/* Currently only port 1 is supported. */
	( void )spi_port;

    /* Keep in reset while configuring. */
	UCTL1 |= SWRST;

	/* Configure Clock Source */
	UTCTL1 &= ~( SSEL0 | SSEL1 );
	if( clock_source == SMCLK )
		UTCTL1 |= SSEL1;
	else
		UTCTL1 |= SSEL0;	/* defaults to ACLK */

	/* Set the baudrate dividers and modulation */
	uint32_t spi_clk = clock_get( SPI_CLK_SRC );
	unsigned int N_div;
	N_div = spi_clk / clock_rate;

	float N_div_f;
	N_div_f = (float)spi_clk / (float)clock_rate;

	UBR01 = ( unsigned char )( N_div & 0x00FF );
	UBR11 = ( unsigned char )( ( N_div & 0xFF00 ) >> 8 );
	UMCTL1 = ( unsigned char )( ( ( N_div_f - ( int )( N_div_f ) ) ) * 8.0f ) << 1; // Set BRS

    /* Remove reset */
    UCTL1 &= ~SWRST;
}

uint8_t SPI_transferByte( uint8_t spi_port, uint8_t byte )
{
	/* Currently only port 1 is supported. */
	( void )spi_port;

	/* wait for TX buf empty. */
	while( ( IFG2 & UTXIFG1 ) == 0 );

	/* Tx buf empty interrupt is automatically reset
	 * when a character is written to Tx buf. */
    TXBUF1 = byte;

    /* wait for RX buf full. */
    while( ( IFG2 & URXIFG1 ) == 0 );

	/* Rx buf full interrupt is automatically reset
	 * when a character is read from RX buf. */
    return RXBUF1;
}

void SPI_receiveFrame( uint8_t spi_port, uint8_t* buffer, uint16_t size )
{
	/* Currently only port 1 is supported. */
	( void )spi_port;

    while( size )
    {
    	/* Write dummy byte to clock data in. */
    	TXBUF1 = DUMMY;
        while ( ( IFG2 & URXIFG1 ) == 0 );
        *buffer++ = RXBUF1;
        size--;
    }
}

void SPI_transmitFrame( uint8_t spi_port, const uint8_t* buffer, uint16_t size)
{
	/* Currently only port 1 is supported. */
	( void )spi_port;

    while( size )
    {
    	while( ( IFG2 & UTXIFG1 ) == 0 );
    	TXBUF1 = *buffer++;
        size--;
    }

    /* clear the rx flag. Not reset automatically
     * as no char read from RX buf. */
    IFG2 &= ~URXIFG1;
}

void SPI_transferFrame( uint8_t spi_port, uint8_t* in_buffer, const uint8_t* out_buffer, uint16_t size )
{
	/* Currently only port 1 is supported. */
	( void )spi_port;

    while( size )
    {
    	while( ( IFG2 & UTXIFG1) == 0 );
        TXBUF1 = *out_buffer++;
        while( ( IFG2 & URXIFG1 ) == 0 );
        *in_buffer++ = RXBUF1;
        size--;
    }
}
