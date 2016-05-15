#include <stdio.h>
#include <unistd.h>
#include <signal.h>

#include "ws2811.h"
#include "lo/lo.h"

#define MONOHORN_URL "osc.unix://localhost/tmp/monohorn.socket"
#define ARRAY_SIZE(stuff) (sizeof(stuff) / sizeof(stuff[0]))
#define WIDTH 8
#define HEIGHT 8

ws2811_led_t matrix[WIDTH][HEIGHT];

void rb_render(lo_address ad)
{
  int x, y;

  for (x = 0; x < WIDTH; x++)
    for (y = 0; y < HEIGHT; y++)
      lo_send(ad, "/set", "iii", x, y, matrix[x][y]);
}

void rb_raise(void)
{
    int x, y;

    for (y = 0; y < (HEIGHT - 1); y++)
      for (x = 0; x < WIDTH; x++)
        matrix[x][y] = matrix[x][y + 1];
}

int dotspos[] = { 0, 1, 2, 3, 4, 5, 6, 7 };
ws2811_led_t dotcolors[] =
{
    0x200000,  // red
    0x201000,  // orange
    0x202000,  // yellow
    0x002000,  // green
    0x002020,  // lightblue
    0x000020,  // blue
    0x100010,  // purple
    0x200010,  // pink
};

void rb_bottom(void)
{
    int i;

    for (i = 0; i < ARRAY_SIZE(dotspos); i++)
    {
      dotspos[i]++;

      if (dotspos[i] > (WIDTH - 1))
      {
        dotspos[i] = 0;
      }

      matrix[dotspos[i]][HEIGHT - 1] = dotcolors[i];
    }
}

int main(int argc, char *argv[])
{
  lo_address ad;

  if (argc > 1) {
    printf("Connecting to %s\n", argv[1]);
    ad = lo_address_new_from_url(argv[1]);
  } else {
    printf("Connecting to %s\n", MONOHORN_URL);
    ad = lo_address_new_from_url(MONOHORN_URL);
  }

  if (lo_address_errno(ad) < 0) {
    fprintf(stderr, "LO Error: %s\n", lo_address_errstr(ad));
    return 1;
  }

  int ret = 0;

  while (1)
  {
    rb_raise();
    rb_bottom();
    rb_render(ad);

    if (lo_send(ad, "/render", NULL) < 0)
    {
      fprintf(stderr, "LO Error: %s\n", lo_address_errstr(ad));
      ret = 1;
      break;
    }

    // 15 frames /sec
    usleep(1000000 / 15);
  }

  lo_send(ad, "/clear", NULL);

  return ret;
}

