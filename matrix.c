#include "matrix.h"

#define TARGET_FREQ                              WS2811_TARGET_FREQ
#define GPIO_PIN                                 18
#define DMA                                      5

#define WIDTH                                    8
#define HEIGHT                                   8
#define LED_COUNT                                (WIDTH * HEIGHT)

ws2811_t leds =
{
    .freq = TARGET_FREQ,
    .dmanum = DMA,
    .channel =
    {
        [0] =
        {
            .gpionum = GPIO_PIN,
            .count = LED_COUNT,
            .invert = 0,
            .brightness = 80,
        },
        [1] =
        {
            .gpionum = 0,
            .count = 0,
            .invert = 0,
            .brightness = 0,
        },
    },
};

int get_pixel_pos(uint8_t x, uint8_t y) {
	int map[8][8] = {
		{7 ,6 ,5 ,4 ,3 ,2 ,1 ,0 },
		{8 ,9 ,10,11,12,13,14,15},
		{23,22,21,20,19,18,17,16},
		{24,25,26,27,28,29,30,31},
		{39,38,37,36,35,34,33,32},
		{40,41,42,43,44,45,46,47},
		{55,54,53,52,51,50,49,48},
		{56,57,58,59,60,61,62,63}
	};

	return map[x][y];
}
int matrix_init() {
  return ws2811_init(&leds);
}

void matrix_end() {
  ws2811_fini(&leds);
}

int matrix_render() {
  return ws2811_render(&leds);
}

void matrix_set(int x, int y, ws2811_led_t colour) {
  leds.channel[0].leds[get_pixel_pos(x,y)] = colour;
}

void matrix_clear() {
  int x, y;
  for (x = 0; x < WIDTH; x++) for (y = 0; y < HEIGHT; y++)
    matrix_set(x, y, 0);
  matrix_render();
}

int matrix_width() { return WIDTH; }
int matrix_height() { return HEIGHT; }
