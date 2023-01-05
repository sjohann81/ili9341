#include "ili9341.h"
#include "gdi.h"
#include "IBM_VGA_8x8.h"

void display_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
{
	int x = x1 - x0;
	int y = y1 - y0;
	int dx = abs(x);
	int sx = x0 < x1 ? 1 : -1;
	int dy = -abs(y);
	int sy = y0 < y1 ? 1 : -1;
	int err = dx + dy, e2;
	
	for (;;) {
		ili9341_pixel(x0, y0, color);
		e2 = 2 * err;
		if (e2 >= dy) {
			if (x0 == x1)
				break;
			err += dy;
			x0 += sx;
		}
		if (e2 <= dx) {
			if (y0 == y1) break;
			err += dx;
			y0 += sy;
		}
	}
}

void display_rectangle(uint16_t x0, uint16_t y0, uint16_t length, uint16_t width, uint16_t color)
{
	ili9341_hline(x0, y0, length, color);
	ili9341_hline(x0, y0 + width, length, color);
	ili9341_vline(x0, y0 + 1, width - 1, color);
	ili9341_vline(x0 + length, y0, width + 1, color);
}

void display_frectangle(uint16_t x0, uint16_t y0, uint16_t length, uint16_t width, uint16_t color)
{
	int i;
	
	for (i = 0; i < width; i++)
		ili9341_hline(x0, y0 + i, length, color);
}

void display_circle(int16_t x0, int16_t y0, int16_t r, uint16_t color)
{
	int x = -r, y = 0, err = 2 - 2 * r, e2;
	
	do {
		ili9341_pixel(x0 - x, y0 + y, color);
		ili9341_pixel(x0 + x, y0 + y, color);
		ili9341_pixel(x0 + x, y0 - y, color);
		ili9341_pixel(x0 - x, y0 - y, color);
		
		e2 = err;
		if (e2 <= y) {
			err += ++y * 2 + 1;
			if (-x == y && e2 <= x)
				e2 = 0;
		}
		if (e2 > x)
			err += ++x * 2 + 1;
	} while (x <= 0);
}

void display_fcircle(int16_t x0, int16_t y0, int16_t r, uint16_t color)
{
	int x = -r, y = 0, err = 2 - 2 * r, e2;
	
	do {
		ili9341_vline(x0 - x, y0 - y, 2 * y, color);
		ili9341_vline(x0 + x, y0 - y, 2 * y, color);

		e2 = err;
		if (e2 <= y) {
			err += ++y * 2 + 1;
			if (-x == y && e2 <= x)
				e2 = 0;
		}
		if (e2 > x)
			err += ++x * 2 + 1;
	} while (x <= 0);
}

void display_triangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
	display_line(x0, y0, x1, y1, color);
	display_line(x1, y1, x2, y2, color);
	display_line(x2, y2, x0, y0, color);
}


void display_ftriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color)
{
	int16_t a, b, y, last;
	int16_t dx01, dy01, dx02, dy02, dx12, dy12;
	int32_t sa = 0, sb = 0;

	if (y0 > y1) {
		_swap_int16_t(y0, y1);
		_swap_int16_t(x0, x1);
	}
	if (y1 > y2) {
		_swap_int16_t(y2, y1);
		_swap_int16_t(x2, x1);
	}
	if (y0 > y1) {
		_swap_int16_t(y0, y1);
		_swap_int16_t(x0, x1);
	}

	if (y0 == y2) {
		a = b = x0;
		if (x1 < a)
			a = x1;
		else if (x1 > b)
			b = x1;
		if (x2 < a)
			a = x2;
		else if (x2 > b)
			b = x2;
		ili9341_hline(a, y0, b - a + 1, color);

		return;
	}

	dx01 = x1 - x0;
	dy01 = y1 - y0;
	dx02 = x2 - x0;
	dy02 = y2 - y0;
	dx12 = x2 - x1;
	dy12 = y2 - y1;

	last = y1 == y2 ? y1 : y1 - 1;

	for (y = y0; y <= last; y++) {
		a = x0 + sa / dy01;
		b = x0 + sb / dy02;
		sa += dx01;
		sb += dx02;
		if (a > b)
			_swap_int16_t(a, b);
		ili9341_hline(a, y, b - a + 1, color);
	}

	sa = dx12 * (y - y1);
	sb = dx02 * (y - y0);
	for (; y <= y2; y++) {
		a = x1 + sa / dy12;
		b = x0 + sb / dy02;
		sa += dx12;
		sb += dx02;
		if (a > b)
			_swap_int16_t(a, b);
		ili9341_hline(a, y, b - a + 1, color);
	}
}

void display_char(uint8_t ascii, uint16_t x0, uint16_t y0, uint16_t size, uint16_t fgcolor)
{
	uint8_t c, b;
	int i;
	
	for (i = 0; i < FONT_Y; i++) {
		c = pgm_read_byte(&ibm_8x8[ascii][i]);
		for (b = 0; b < 8; b++) {
			if ((c >> b) & 0x01)
				display_frectangle(x0 + (8 - b) * size, y0 + i * size, size, size, fgcolor);
		}
	}
}

void display_print(char *string, uint16_t x0, uint16_t y0, uint16_t size, uint16_t fgcolor)
{
	while (*string) {
		display_char(*string, x0, y0, size, fgcolor);
		string++;

		if (x0 < max_x - 1 - (FONT_X + FONT_X) * size)
			x0 += FONT_X * size;
	}
}

static int32_t ftoa(float f, char *outbuf, int32_t precision)
{
	int32_t mantissa, int_part, frac_part, exp2, i;
	char *p;
	union float_long {
		float f;
		int32_t l;
		uint32_t u;
	} fl;

	p = outbuf;

	if (f < 0.0) {
		*p = '-';
		f = -f;
		p++;
	}

	fl.f = f;

	exp2 = (fl.l >> 23) - 127;
	mantissa = (fl.l & 0xffffff) | 0x800000;
	frac_part = 0;
	int_part = 0;

	if (exp2 >= 31) {
		return -1;	/* too large */
	} else {
		if (exp2 >= -23) {
			if (exp2 >= 23) {
				int_part = mantissa << (exp2 - 23);
			} else {
				if (exp2 >= 0){
					int_part = mantissa >> (23 - exp2);
					frac_part = (mantissa << (exp2 + 1)) & 0xffffff;
				} else {
					frac_part = (mantissa & 0xffffff) >> (-(exp2 + 1));
				}
			}
		}
	}

	if (int_part == 0) {
		*p = '0';
		p++;
	} else {
		itoa(int_part, p, 10);
		while (*p) p++;
	}
	*p = '.';
	p++;

	for (i = 0; i < precision; i++) {
		frac_part = (frac_part << 3) + (frac_part << 1);
		*p = (frac_part >> 24) + '0';
		p++;
		frac_part = frac_part & 0xffffff;
	}

	*p = 0;

	return 0;
}

void display_int(uint32_t n, uint16_t x, uint16_t y, uint16_t size, uint16_t fgcolor)
{
	char buf[30];

	itoa(n, buf, 10);
	display_print(buf, x, y, size, fgcolor);
}

void display_hex(uint32_t n, uint16_t x, uint16_t y, uint16_t size, uint16_t fgcolor)
{
	char buf[30];

	itoa(n, buf, 16);
	display_print(buf, x, y, size, fgcolor);
}

void display_float(float n, uint16_t x, uint16_t y, uint16_t size, uint16_t fgcolor)
{
	char buf[30];

	ftoa(n, buf, 3);
	display_print(buf, x, y, size, fgcolor);
}

void display_background(uint16_t color)
{
	display_frectangle(0,0,max_x,max_y,color);
}
