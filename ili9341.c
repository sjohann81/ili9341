#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include "ili9341.h"
#include "spi.h"
#include "pins.h"

uint16_t max_x, max_y;
uint8_t orientation;

static void ili9341_sendcmd(uint8_t data)
{
	ILI_PORT &= ~ILI_DC;
	spi_start();
	spi_xfer_hw(data);
	spi_stop();
}

static void ili9341_senddata(uint8_t data)
{
	ILI_PORT |= ILI_DC;
	spi_start();
	spi_xfer_hw(data);
	spi_stop();
}

static void ili9341_senddata16(uint16_t data)
{
	uint8_t data1 = data >> 8;
	uint8_t data2 = data & 0xff;
	
	ILI_PORT |= ILI_DC;
	spi_start();
	spi_xfer_hw(data1);
	spi_xfer_hw(data2);
	spi_stop();
}

void ili9341_init(void)
{
	ILI_DDR |= ILI_DC | ILI_RST;
	ILI_PORT |= ILI_DC;
	
	spi_setup(1);
	ILI_PORT |= ILI_DC;
	ILI_PORT &= ~ILI_RST;
	_delay_ms(10);
	ILI_PORT |= ILI_RST;

	ili9341_orientation(PORTRAIT);

	/* power control A */
	ili9341_sendcmd(0xcb);
	ili9341_senddata(0x39);
	ili9341_senddata(0x2c);
	ili9341_senddata(0x00);
	ili9341_senddata(0x34);
	ili9341_senddata(0x02);

	/* power control B */
	ili9341_sendcmd(0xcf);
	ili9341_senddata(0x00);
	ili9341_senddata(0xc1);
	ili9341_senddata(0x30);

	/* driver timing control A */
	ili9341_sendcmd(0xe8);
	ili9341_senddata(0x85);
	ili9341_senddata(0x00);
	ili9341_senddata(0x78);

	/* driver timing control B */
	ili9341_sendcmd(0xea);
	ili9341_senddata(0x00);
	ili9341_senddata(0x00);

	/* power on sequence control */
	ili9341_sendcmd(0xed);
	ili9341_senddata(0x64);
	ili9341_senddata(0x03);
	ili9341_senddata(0x12);
	ili9341_senddata(0x81);

	/* pump ratio control */
	ili9341_sendcmd(0xf7);
	ili9341_senddata(0x20);

	/* power control,VRH[5:0] */
	ili9341_sendcmd(0xc0);
	ili9341_senddata(0x23);

	/* Power control,SAP[2:0];BT[3:0] */
	ili9341_sendcmd(0xc1);
	ili9341_senddata(0x10);

	/* vcm control */
	ili9341_sendcmd(0xc5);
	ili9341_senddata(0x3e);
	ili9341_senddata(0x28);

	/* vcm control 2 */
	ili9341_sendcmd(0xc7);
	ili9341_senddata(0x86);

	/* memory access control */
	ili9341_sendcmd(0x36);
	ili9341_senddata(0x48);

	/* pixel format */
	ili9341_sendcmd(0x3a);
	ili9341_senddata(0x55);

	/* frame ratio control, normal mode full colours */
	ili9341_sendcmd(0xb1);
	ili9341_senddata(0x00);
	ili9341_senddata(0x1b);

	/* gamma function disable */
	ili9341_sendcmd(0xf2);
	ili9341_senddata(0x00);

	/* gamma curve selected */
	ili9341_sendcmd(0x26);
	ili9341_senddata(0x01);

	/* set positive gamma correction */
	ili9341_sendcmd(0xe0);
	ili9341_senddata(0x0f);
	ili9341_senddata(0x31);
	ili9341_senddata(0x2b);
	ili9341_senddata(0x0c);
	ili9341_senddata(0x0e);
	ili9341_senddata(0x08);
	ili9341_senddata(0x4e);
	ili9341_senddata(0xf1);
	ili9341_senddata(0x37);
	ili9341_senddata(0x07);
	ili9341_senddata(0x10);
	ili9341_senddata(0x03);
	ili9341_senddata(0x0e);
	ili9341_senddata(0x09);
	ili9341_senddata(0x00);

	/* set negative gamma correction */
	ili9341_sendcmd(0xe1);
	ili9341_senddata(0x00);
	ili9341_senddata(0x0e);
	ili9341_senddata(0x14);
	ili9341_senddata(0x03);
	ili9341_senddata(0x11);
	ili9341_senddata(0x07);
	ili9341_senddata(0x31);
	ili9341_senddata(0xc1);
	ili9341_senddata(0x48);
	ili9341_senddata(0x08);
	ili9341_senddata(0x0f);
	ili9341_senddata(0x0c);
	ili9341_senddata(0x31);
	ili9341_senddata(0x36);
	ili9341_senddata(0x0f);

	/* exit sleep */
	ili9341_sendcmd(0x11);
	_delay_ms(150);
	
	/* display on*/
	ili9341_sendcmd(0x29);
}

static void ili9341_setX(uint16_t x0,uint16_t x1)
{
	ili9341_sendcmd(0x2a);
	ili9341_senddata16(x0);
	ili9341_senddata16(x1);
}

static void ili9341_setY(uint16_t y0,uint16_t y1)
{
	ili9341_sendcmd(0x2b);
	ili9341_senddata16(y0);
	ili9341_senddata16(y1);
}

void ili9341_orientation(char val)
{
	orientation = val;
	if (orientation == PORTRAIT) {
		max_x = ILI9341_HEIGHT;
		max_y = ILI9341_WIDTH;
	} else if (orientation == LANDSCAPE) {
		max_x = ILI9341_WIDTH;
		max_y = ILI9341_HEIGHT;
	}
}

void ili9341_pixel(uint16_t x0, uint16_t y0, uint16_t color)
{
	if (orientation == PORTRAIT) {
		ili9341_setX(x0, x0);
		ili9341_setY(y0, y0);
		ili9341_sendcmd(0x2c);
		ili9341_senddata16(color);
	} else if (orientation == LANDSCAPE) {
		ili9341_setX(y0, y0);
		ili9341_setY(max_x-x0, max_x-x0);
		ili9341_sendcmd(0x2c);
		ili9341_senddata16(color);
	}
}

void ili9341_hline(uint16_t x0, uint16_t y0, uint16_t length, uint16_t color)
{
	int i;

	if (orientation == PORTRAIT) {
		ili9341_setX(x0, x0 + length);
		ili9341_setY(y0, y0);
		ili9341_sendcmd(0x2c);
		for (i = 0; i < length; i++)
			ili9341_senddata16(color);
	} else if (orientation ==  LANDSCAPE) {
		ili9341_setX(y0, y0);
		ili9341_setY(max_x - x0 - length, max_x - x0);
		ili9341_sendcmd(0x2c);
		for (i = 0; i < length; i++)
			ili9341_senddata16(color);
	}
}

void ili9341_vline(uint16_t x0, uint16_t y0, uint16_t length, uint16_t color)
{
	int i;
	
	if (orientation == PORTRAIT) {
		ili9341_setX(x0, x0);
		ili9341_setY(y0, y0 + length);
		ili9341_sendcmd(0x2c);
		for (i = 0; i < length; i++)
			ili9341_senddata16(color);
	} else if (orientation ==  LANDSCAPE) {
		ili9341_setX(y0, y0 + length);
		ili9341_setY(max_x - x0, max_x - x0);
		ili9341_sendcmd(0x2c);
		for (i = 0; i < length; i++)
			ili9341_senddata16(color);
	}
}
