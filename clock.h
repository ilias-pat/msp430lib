#ifndef CLOCK_H_
#define CLOCK_H_

#include "types.h"

/* Prevent clashing with clock_t in sys/types.h */
#define __clock_t_defined

/**
 * DCO pre-defined frequencies
 * @attention	The frequencies are for 3V3 operation and may vary
 * depending on room temperature and internal DCO resistance of the
 * specific device.
 */
#define DCO_OFF				0			/**< DCO not used. */
#define DCO_FREQ_750KHz		750000		/**< 750 KHz. */
#define DCO_FREQ_1300KHz	1300000		/**< 1.3 MHz. */
#define DCO_FREQ_2000KHz	2000000		/**< 2 MHz. */
#define DCO_FREQ_3200KHz	3200000		/**< 1.3 MHz. */
#define DCO_FREQ_4900KHz	4900000		/**< 4.9 MHz. */

/** Clock Source */
typedef enum
{
	XT1,		/**< XT1 Crystal Oscillator. */
	XT2,		/**< XT2 Crystal Oscillator. Not available on all devices. */
	DCO			/**< Internal Digitally Controlled Oscillator. */
} clock_source_t;

/** MCU Clock */
typedef enum
{
	ACLK	= 0x01,		/**< Auxiliary Clock. Can only be clocked from XT1. */
	MCLK	= 0x04,		/**< Master Clock. Can be clocked from XT1, XT2 or DCO. */
	SMCLK	= 0x02		/**< Sub-main Clock. Can be clocked from XT1, XT2 or DCO. */
} clock_t;

/**
 * Initialises the clock module.
 * @param[in] xt1_freq	Frequency of XT1 Crystal Oscillator, in Hz.
 * @param[in] xt2_freq	Frequency of XT2 Crystal Oscillator, in Hz.
 * @param[in] dco_freq	Frequency of DCO, in Hz.*
 */
void clock_init( uint32_t xt1_freq, uint32_t xt2_freq, uint32_t dco_freq );

/**
 * Configures the MCU clocks: ACLK, MCLK, SMCLK.
 * @param[in] clock 	One of ACLK, MCLK, SMCLK.
 * @param[in] source	The source of the clock, one of XT1, XT2, DCO.
 * @param[in] divider	The clock source divider, one of 1, 2, 4, 8.
 * @attention ACLK only supports XT1 clock source.
 */
void clock_set( clock_t clk, clock_source_t source, uint8_t divider );

/**
 * Returns the frequency of the specified clock.
 * @param[in] clock 	One of ACLK, MCLK, SMCLK.
 * @return	Frequency in Hz.
 */
uint32_t clock_get( clock_t clk );

#endif
