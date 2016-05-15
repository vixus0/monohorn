#include "ws2811.h"

int matrix_init();
void matrix_end();
int matrix_render();
void matrix_set(int x, int y, ws2811_led_t colour);
int matrix_width();
int matrix_height();

