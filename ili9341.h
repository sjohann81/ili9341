#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <stdlib.h>

#define ILI9341_HEIGHT	240 
#define ILI9341_WIDTH	320

#define FONT_X		8
#define FONT_Y		8

#define PORTRAIT	0
#define LANDSCAPE	1

#define ABS(a) ((a) < 0 ? -(a) : (a))

/* R:00001 G:000001 B:00001 */
#define RGB(red, green, blue)	((uint16_t)(((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3)))

/* https://www.rapidtables.com/web/color/RGB_Color.html */
#define BLACK		RGB(0x00, 0x00, 0x00)
#define WHITE		RGB(0xff, 0xff, 0xff)
#define RED		RGB(0xff, 0x00, 0x00)
#define LIME		RGB(0x00, 0xff, 0x00)
#define BLUE		RGB(0x00, 0x00, 0xff)
#define YELLOW		RGB(0xff, 0xff, 0x00)
#define CYAN		RGB(0x00, 0xff, 0xff)
#define MAGENTA		RGB(0xff, 0x00, 0xff)
#define SILVER		RGB(0xc0, 0xc0, 0xc0)
#define GRAY		RGB(0x80, 0x80, 0x80)
#define MAROON		RGB(0x80, 0x00, 0x00)
#define OLIVE		RGB(0x80, 0x80, 0x00)
#define GREEN		RGB(0x00, 0x80, 0x00)
#define PURPLE		RGB(0x80, 0x00, 0x80)
#define TEAL		RGB(0x00, 0x80, 0x80)
#define NAVY		RGB(0x00, 0x00, 0x80)

extern uint16_t max_x, max_y;
extern uint8_t orientation;

void ili9341_init(void);
void ili9341_orientation(char orientation);
void ili9341_pixel(uint16_t x0, uint16_t y0, uint16_t color);
void ili9341_hline(uint16_t x0, uint16_t y0, uint16_t length, uint16_t color);
void ili9341_vline(uint16_t x0, uint16_t y0, uint16_t length, uint16_t color);
