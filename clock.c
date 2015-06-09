#include "clock.h"
#include <msp430.h>

#define WAIT_CRYSTAL( )														\
	do {																	\
		int i;																\
		do {																\
			IFG1 &= ~OFIFG;                  /* Clear OSCFault flag  */		\
			for( i = 0xff; i > 0; i-- )      /* Time for flag to set */		\
				__nop();													\
		}  while( ( IFG1 & OFIFG ) != 0 );   /* OSCFault flag still set? */	\
	} while( 0 )

static uint32_t clock_XT1FreqHz;
static uint32_t clock_XT2FreqHz;
static uint32_t clock_DCOFreqHz;

static void clock_configACLK( uint8_t divider );
static void clock_configMCLK( clock_source_t source, uint8_t divider );
static void clock_configSMCLK( clock_source_t source, uint8_t divider );
static uint32_t clock_getACLK( void );
static uint32_t clock_getMCLK( void );
static uint32_t clock_getSMCLK( void );

void clock_init( uint32_t xt1_freq, uint32_t xt2_freq, uint32_t dco_freq )
{
	clock_XT1FreqHz = xt1_freq;
	clock_XT2FreqHz = xt2_freq;
	clock_DCOFreqHz = dco_freq;

	/* Enable High Freq mode for XT1. */
	if( clock_XT1FreqHz >= 450000 )
		BCSCTL1 |= XTS;

	/* Disable XT2 if not used. */
	if( clock_XT2FreqHz == 0 )
		BCSCTL1 |= XT2OFF;
	else
		BCSCTL1 &= ~XT2OFF;

	switch( dco_freq )
	{
	/* Turn off DCOCLK */
	case DCO_OFF:
		__bis_SR_register( SCG0 );
		break;

	/* 750 KHz */
	default:
	case DCO_FREQ_750KHz:
		DCOCTL   = ( ( 0x3 << 5 ) & 0xE0 );
		DCOCTL  |= ( 0x00 & 0x1F );
		BCSCTL1 |= ( 0x04 & 0x07 );
		break;

	/* 1.3 MHz */
	case DCO_FREQ_1300KHz:
		DCOCTL   = ( ( 0x3 << 5 ) & 0xE0 );
		DCOCTL  |= ( 0x00 & 0x1F );
		BCSCTL1 |= ( 0x05 & 0x07 );
		break;

	/* 2 MHz */
	case DCO_FREQ_2000KHz:
		DCOCTL   = ( ( 0x3 << 5 ) & 0xE0 );
		DCOCTL  |= ( 0x00 & 0x1F );
		BCSCTL1 |= ( 0x06 & 0x07 );
		break;

	/* 3.2 MHz */
	case DCO_FREQ_3200KHz:
		DCOCTL   = ( ( 0x3 << 5 ) & 0xE0 );
		DCOCTL  |= ( 0x00 & 0x1F );
		BCSCTL1 |= ( 0x07 & 0x07 );
		break;

	/* 4.9 MHz */
	case DCO_FREQ_4900KHz:
		DCOCTL   = ( ( 0x7 << 5 ) & 0xE0 );
		DCOCTL  |= ( 0x00 & 0x1F );
		BCSCTL1 |= ( 0x07 & 0x07 );
		break;
	}
}

void clock_set( clock_t clk, clock_source_t source, uint8_t divider )
{
	switch( clk )
	{
	case ACLK:
		clock_configACLK( divider );
		break;

	case MCLK:
		clock_configMCLK( source, divider );
		break;

	case SMCLK:
		clock_configSMCLK( source, divider );
		break;

	default:
		break;
	}

	WAIT_CRYSTAL( );
}

uint32_t clock_get( clock_t clk )
{
	switch( clk )
	{
	case ACLK:
		return clock_getACLK( );
	case MCLK:
		return clock_getMCLK( );
	case SMCLK:
		return clock_getSMCLK( );
	default:
		return 0;
	}
}

static void clock_configACLK( uint8_t divider )
{
	int diva = 0;

    switch( divider )
    {
    default:
    case 1:  diva = DIVA_0;	break;
    case 2:  diva = DIVA_1;	break;
    case 4:  diva = DIVA_2;	break;
    case 8:  diva = DIVA_3;	break;
    }

    BCSCTL1  &= ~(DIVA_3);
    BCSCTL1  |= diva;
}

static void clock_configMCLK( clock_source_t source, uint8_t divider )
{
	uint8_t selm = 0;
	switch( source )
	{
	default:
	case DCO: selm = SELM_0; break;
	case XT2: selm = SELM_2; break;
	case XT1: selm = SELM_3; break;
	}

	BCSCTL2 &= ~SELM_3;
	BCSCTL2 |= selm;

	uint8_t divm = 0;
	switch( divider )
	{
	default:
	case 1:  divm = DIVM_0;	break;
	case 2:  divm = DIVM_1;	break;
	case 4:  divm = DIVM_2;	break;
	case 8:  divm = DIVM_3;	break;
	}

	BCSCTL2  &= ~(DIVM_3);
	BCSCTL2  |= divm;
}

static void clock_configSMCLK( clock_source_t source, uint8_t divider )
{
	if( source == XT2 )
	{
		BCSCTL2 |= SELS;
	}
	else if( source == DCO )
	{
		BCSCTL2 &= ~SELS;
	}

	uint8_t divs = 0;
	switch( divider )
	{
	case 1:  divs = DIVS_0;	break;
	case 2:  divs = DIVS_1;	break;
	case 4:  divs = DIVS_2;	break;
	case 8:  divs = DIVS_3;	break;
	default: divs = DIVS_0;	break;
	}

	BCSCTL2  &= ~(DIVS_3);
	BCSCTL2  |= divs;
}

static uint32_t clock_getACLK( void )
{
	uint32_t freq = clock_XT1FreqHz;

	uint8_t diva = BCSCTL1 & 0x30;
	switch( diva )
	{
	default:
	case DIVA_0: break;
	case DIVA_1: freq /= 2; break;
	case DIVA_2: freq /= 4; break;
	case DIVA_3: freq /= 8; break;
	}

	return freq;
}

static uint32_t clock_getMCLK( void )
{
	uint32_t freq = clock_DCOFreqHz;

	if( ( BCSCTL2 & 0xC0 ) == SELM_2 )
	{
		freq = clock_XT2FreqHz;
	}
	else if( ( BCSCTL2 & 0xC0 ) == SELM_3 )
	{
		freq = clock_XT1FreqHz;
	}

	uint8_t divm = BCSCTL2 & 0x30;
	switch( divm )
	{
	default:
	case DIVM_0: break;
	case DIVM_1: freq /= 2; break;
	case DIVM_2: freq /= 4; break;
	case DIVM_3: freq /= 8; break;
	}

	return freq;
}

static uint32_t clock_getSMCLK( void )
{
	uint32_t freq = clock_DCOFreqHz;

	if( ( BCSCTL2 & SELS ) == SELS )
		freq = clock_XT2FreqHz;	/* XT2 present on F149 */

	uint8_t divs = BCSCTL2 & 0x06;
	switch( divs )
	{
	default:
	case DIVS_0: break;
	case DIVS_1: freq /= 2; break;
	case DIVS_2: freq /= 4; break;
	case DIVS_3: freq /= 8; break;
	}

	return freq;
}

