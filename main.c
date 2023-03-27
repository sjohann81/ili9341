#include "ili9341.h"
#include "gdi.h"

void display_test1()
{
	ili9341_pixel(10, 10, YELLOW);
	display_line(0, 0, max_x, max_y, GREEN);
	display_line(max_x, 0, 0, max_y, GREEN);
	display_rectangle(0, 0, 240, 240, LIME);
	display_frectangle(20, 10, 100, 20, RED);
	display_circle(95,170,15, WHITE);
	display_fcircle(25, 65, 20, BLUE);
	display_char('a', 5, 80, 4, RED);
	display_print("abcdefgh", 180, 0, 1, GREEN);
	display_float(-123.56, 120, 60, 2, BLUE);
	display_int(456, 200, 80, 2, WHITE);
	display_hex(0xdeadbeef, 60, 100, 3, OLIVE);
}

void display_test2(int size)
{
	int i, j, k;
	
	for (j = 10, k = 0; k < 255; j += FONT_Y * size) {
		for (i = 1; i < max_x - 1 - FONT_X * size && k < 255; i += FONT_X * size, k++) {
			display_char(k, i, j, size, YELLOW);
		}
	}

}

int dot_demo()
{
	uint16_t k, l, c;
	int i = 0;
	
	for (;;) {
		k = rand() % max_x;
		l = rand() % max_y;
		c = RGB((rand() % 255), (rand() % 255), (rand() % 255));

		ili9341_pixel(k, l, c);
		
		if (i > 1000)
			break;
			
		i++;
	}
	
	_delay_ms(2000);
	
	return i;
}

int line_demo()
{
	uint16_t k, l, m, n, c;
	int i = 0;
	
	for (;;) {
		k = rand() % max_x;
		l = rand() % max_y;
		m = rand() % max_x;
		n = rand() % max_y;
		c = RGB((rand() % 255), (rand() % 255), (rand() % 255));

		display_line(k, l, m, n, c);
			
		if (i > 500)
			break;
			
		i++;
	}
	
	_delay_ms(2000);
	
	return i;
}


int rectangle_demo()
{
	uint16_t k, l, m, n, c;
	int i = 0;
	
	for (;;) {
		do {
			m = rand() % (max_x / 4);
		} while (m < 5);
		do {
			n = rand() % (max_y / 4);
		} while (n < 5);
		
		k = rand() % (max_x - m);
		l = rand() % (max_y - n);
		c = RGB((rand() % 255), (rand() % 255), (rand() % 255));

		if (i & 1)
			display_rectangle(k, l, m, n, c);
		else
			display_frectangle(k, l, m, n, c);
		
		if (i > 500)
			break;
			
		i++;
	}
	
	_delay_ms(2000);
	
	return i;
}


int circle_demo()
{
	uint16_t k, l, r, c;
	int i = 0;
	
	for (;;) {
		do {
			r = rand() % (max_y / 4);
		} while (r < 5);
		
		k = rand() % (max_x - r * 2) + r;
		l = rand() % (max_y - r * 2) + r;
		c = RGB((rand() % 255), (rand() % 255), (rand() % 255));

		if (i & 1)
			display_circle(k, l, r, c);
		else
			display_fcircle(k, l, r, c);
		
		if (i > 500)
			break;
			
		i++;
	}
	
	_delay_ms(2000);
	
	return i;
}


int triangle_demo()
{
	uint16_t k, l, m, n, o, p, c;
	int i = 0;
	
	for (;;) {
		k = rand() % max_x;
		l = rand() % max_y;
		m = rand() % max_x;
		n = rand() % max_y;
		o = rand() % max_x;
		p = rand() % max_y;
		c = RGB((rand() % 255), (rand() % 255), (rand() % 255));
			
		if (i & 1)
			display_triangle(k, l, m, n, o, p, c);
		else
			display_ftriangle(k, l, m, n, o, p, c);
		
		if (i > 500)
			break;
			
		i++;
	}
	
	_delay_ms(2000);
	
	return i;
}

void display_test3()
{
//	while (1) {
		display_background(BLACK);
		dot_demo();
		display_background(BLACK);
		line_demo();
		display_background(BLACK);
		rectangle_demo();
		display_background(BLACK);
		circle_demo();
		display_background(BLACK);
		triangle_demo();
//	}
}

int main(void) {
	ili9341_init();

	while (1) {
		ili9341_orientation(LANDSCAPE);
		display_test1();
		_delay_ms(2000);
		display_background(BLACK);

		ili9341_orientation(PORTRAIT);
		display_test1();
		_delay_ms(2000);
		display_background(BLACK);

		ili9341_orientation(LANDSCAPE);
		display_test2(2);
		_delay_ms(4000);
		display_background(BLUE);

		ili9341_orientation(PORTRAIT);
		display_test2(2);
		_delay_ms(4000);
		display_background(BLACK);

		ili9341_orientation(LANDSCAPE);
		display_test2(1);
		_delay_ms(4000);
		display_background(BLUE);

		ili9341_orientation(PORTRAIT);
		display_test2(1);
		_delay_ms(4000);
		display_background(BLACK);
		
		ili9341_orientation(LANDSCAPE);
		display_test3();
		display_background(BLACK);
	}
	
	return 0;
}
