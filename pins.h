#define SPI_DDR		DDRB
#define SPI_PIN		PINB
#define SPI_PORT	PORTB

#define SPI_CS		(1 << PB0)
#define SPI_SCK		(1 << PB1)
#define SPI_MOSI	(1 << PB2)
#define SPI_MISO	(1 << PB3)

#define ILI_DDR		DDRE
#define ILI_PORT	PORTE

#define ILI_DC		(1 << PE6)
#define ILI_RST		(1 << PE7)
