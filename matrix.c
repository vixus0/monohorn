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
            .brightness = 255,
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
  leds.channel[0].leds[(y * WIDTH) + x] = colour;
}

void matrix_clear() {
  int x, y;
  for (x = 0; x < WIDTH; x++) for (y = 0; y < HEIGHT; y++)
    matrix_set(x, y, 0);
  matrix_render();
}

int matrix_width() { return WIDTH; }
int matrix_height() { return HEIGHT; }
