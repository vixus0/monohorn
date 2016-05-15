/*
 * main.c
 *
 * Copyright (c) 2014 Jeremy Garff <jer @ jers.net>
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted
 * provided that the following conditions are met:
 *
 *     1.  Redistributions of source code must retain the above copyright notice, this list of
 *         conditions and the following disclaimer.
 *     2.  Redistributions in binary form must reproduce the above copyright notice, this list
 *         of conditions and the following disclaimer in the documentation and/or other materials
 *         provided with the distribution.
 *     3.  Neither the name of the owner nor the names of its contributors may be used to endorse
 *         or promote products derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */


#include <unistd.h>
#include <signal.h>

#include "ws2811.h"
#include "matrix.h"

#define ARRAY_SIZE(stuff) (sizeof(stuff) / sizeof(stuff[0]))
#define WIDTH 8
#define HEIGHT 8

ws2811_led_t matrix[WIDTH][HEIGHT];

void rb_render()
{
    int x, y;

    for (x = 0; x < WIDTH; x++)
    {
        for (y = 0; y < HEIGHT; y++)
        {
            matrix_set(x, y, matrix[x][y]);
        }
    }
}

void rb_raise(void)
{
    int x, y;

    for (y = 0; y < (HEIGHT - 1); y++)
    {
        for (x = 0; x < WIDTH; x++)
        {
            matrix[x][y] = matrix[x][y + 1];
        }
    }
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

static void ctrl_c_handler(int signum)
{
  matrix_end();
}

static void setup_handlers(void)
{
    signal(SIGINT, ctrl_c_handler);
    signal(SIGTERM, ctrl_c_handler);
}

int main(int argc, char *argv[])
{
    int ret = 0;

    setup_handlers();

    if (matrix_init() < 0)
    {
        return 1;
    }

    while (1)
    {
        rb_raise();
        rb_bottom();
        rb_render();

        if (matrix_render() < 0)
        {
            ret = 1;
            break;
        }

        // 15 frames /sec
        usleep(1000000 / 15);
    }

    matrix_end();

    return ret;
}

