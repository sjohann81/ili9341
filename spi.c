#include <avr/io.h>
#include <util/delay.h>
#include "spi.h"
#include "pins.h"

void spi_setup(uint8_t hw)
{
	SPI_DDR = SPI_CS | SPI_SCK | SPI_MOSI;
	if (!hw) {
		SPI_PORT = SPI_CS | SPI_MOSI;
	} else {
		SPCR = (1 << SPE) | (1 << MSTR);
		SPSR = (1 << SPI2X);
	}
}

void spi_start(void)
{
	SPI_PORT &= ~SPI_CS;
}

void spi_stop(void)
{
	SPI_PORT |= SPI_CS;
}

/* SPI mode 0 */
char spi_xfer(char data)
{
	char i;
	char newdata = 0;

	for (i = 0; i < 8; i++) {
		if (data & 0x80) {
			SPI_PORT |= SPI_MOSI;
		} else {
			SPI_PORT &= ~SPI_MOSI;
		}
		data <<= 1;
		SPI_PORT |= SPI_SCK;
		newdata <<= 1;
		newdata |= ((SPI_PIN & SPI_MISO) ? 1 : 0);
		SPI_PORT &= ~SPI_SCK;
	}
	
	return newdata;
}

char spi_xfer_hw(char data)
{
	SPDR = data;
	while (!(SPSR & (1 << SPIF)));
	
	return SPDR;
}
