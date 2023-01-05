void display_line(uint16_t x0,uint16_t y0,uint16_t x1, uint16_t y1, uint16_t color);
void display_rectangle(uint16_t x0, uint16_t y0, uint16_t length, uint16_t width, uint16_t color);
void display_frectangle(uint16_t x0, uint16_t y0, uint16_t length, uint16_t width, uint16_t color);
void display_circle(int x0, int y0, int r, uint16_t color);
void display_fcircle(int x0, int y0, int r, uint16_t color);
void display_triangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void display_ftriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
void display_char(uint8_t ascii, uint16_t x0, uint16_t y0,uint16_t size, uint16_t fgcolor);
void display_print(char *string, uint16_t x0, uint16_t y0, uint16_t size, uint16_t fgcolor);
void display_int(uint32_t n, uint16_t x, uint16_t y, uint16_t size, uint16_t fgcolor);
void display_hex(uint32_t n, uint16_t x, uint16_t y, uint16_t size, uint16_t fgcolor);
void display_float(float n, uint16_t x, uint16_t y, uint16_t size, uint16_t fgcolor);
void display_background(uint16_t color);

#define _swap_int16_t(a, b)	\
{				\
	int16_t t = a;		\
	a = b;			\
	b = t;			\
}
