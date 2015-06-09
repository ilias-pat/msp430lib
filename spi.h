#ifndef SPI_H
#define SPI_H

#include "types.h"
#include <msp430.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * SPI Modes.
 * @image html http://upload.wikimedia.org/wikipedia/commons/6/6b/SPI_timing_diagram2.svg "Various SPI modes"
 */
#define SPI_MODE0    ( 0 )              /**< CPOL = 0, CPHA = 0 */
#define SPI_MODE1    ( CKPH )         	/**< CPOL = 0, CPHA = 1 */
#define SPI_MODE2    ( CKPL )         	/**< CPOL = 1, CPHA = 0 */
#define SPI_MODE3    ( CKPL | CKPH )	/**< CPOL = 1, CPHA = 1 */

/**
 * Initializes the SPI hardware, in master mode.
 * @param[in] spi_port		Specifies the MCU USART port to operate on.
 * @param[in] mode 			Specifies the SPI CLK polarity and phase.
 * @param[in] clock_rate 	Clock rate in Hz.
 * @param[in] clock_source 	Source of SPI clock, one of ACLK or SMCLK.
 * @attention The clock source must operate at a sufficient frequency to support the
 * requested clock rate.
 */
void SPI_init( uint8_t spi_port, uint8_t mode, uint32_t clock_rate, uint16_t clock_source );

/**
 * Uninitializes the SPI hardware.
 * @param[in] spi_port	Specifies the SPI port to operate on.
 */
void SPI_uninit( uint8_t spi_port );

/**
 * Sends and receives a single byte.
 * @param[in] spi_port	Specifies the SPI port to operate on.
 * @param[in] data 		Byte to be sent.
 * @returns Byte received.
 */
uint8_t SPI_transferByte( uint8_t spi_port, uint8_t data );

/**
 * Sends and receives a frame of specified size.
 * @param[in] spi_port		Specifies the SPI port to operate on.
 * @param [in] size 		Number of bytes to transfer.
 * @param [out] in_buffer 	Stores received data.
 * @param [in] src 			Stores data to be transmitted.
 */
void SPI_transferFrame( uint8_t spi_port, uint8_t* in_buffer, const uint8_t* out_buffer, uint16_t size );

/**
 * Receive a frame of specified size.
 * @param[in] spi_port	Specifies the SPI port to operate on.
 * @param[out] buffer	Stores received data.
 * @param[in] size		Number of bytes to receive.
 */
void SPI_receiveFrame( uint8_t spi_port, uint8_t* buffer, uint16_t size );

/**
 * Transmit a frame of specified size.
 * @param[in] spi_port	Specifies the SPI port to operate on.
 * @param [in] buffer	Stores data to be transmitted.
 * @param [in] size		Number of bytes to transmit.
 */
void SPI_transmitFrame( uint8_t spi_port, const uint8_t* buffer, uint16_t size );


#ifdef __cplusplus
}
#endif

#endif
