#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/stat.h>
#include "lo/lo.h"
#include "matrix.h"

#define SOCKET_PATH "/tmp/monohorn.socket"

static void mh_cleanup() {
  matrix_clear();
  matrix_end();
}

static void mh_sigint(int sig) {
  mh_cleanup();
}

int chmod_socket(const char *path) {
  return chmod(path, S_IWGRP | S_IWOTH);
}

void mh_osc_error(int num, const char *msg, const char *path);
int mh_osc_set(
    const char *path, const char *types, lo_arg **argv, int argc,
    void *data, void *user_data
    );
int mh_osc_render(
    const char *path, const char *types, lo_arg **argv, int argc,
    void *data, void *user_data
    );
int mh_osc_clear(
    const char *path, const char *types, lo_arg **argv, int argc,
    void *data, void *user_data
    );

int main(int argc, char *argv[]) {
  // Catch signals
  signal(SIGINT, mh_sigint);
  signal(SIGTERM, mh_sigint);

  // Initialise LEDs
  if (matrix_init() < 0) return 1;

  // Set up OSC server
  lo_server_thread st;
  int chmod_err = 0;

  if (argc > 1) {
    st = lo_server_thread_new(argv[1], mh_osc_error);
    chmod_err = chmod_socket(argv[1]);
  } else {
    st = lo_server_thread_new(SOCKET_PATH, mh_osc_error);
    chmod_err = chmod_socket(SOCKET_PATH);
  }

  if (chmod_err != 0) {
    fprintf(stderr, "Error changing socket permissions\n");
    return 1;
  }

  // Set up OSC message handlers
  lo_server_thread_add_method(st, "/set", "iii", mh_osc_set, NULL);
  lo_server_thread_add_method(st, "/render", NULL, mh_osc_render, NULL);
  lo_server_thread_add_method(st, "/clear", NULL, mh_osc_clear, NULL);
  lo_server_thread_start(st);

  while (1) {
    usleep(1000);
  }

  // Cleanup
  lo_server_thread_free(st);
  mh_cleanup();

  return 0;
}

void mh_osc_error(int num, const char *msg, const char *path) {
  fprintf(stderr, "mh_osc_error [%d] (%s) %s\n", num, path, msg);
  fflush(stderr);
}

int mh_osc_set(
    const char *path, const char *types, lo_arg **argv, int argc,
    void *data, void *user_data
    )
{
  int x = argv[0]->i;
  int y = argv[1]->i;
  ws2811_led_t colour = (ws2811_led_t)(argv[2]->i);

  matrix_set(x, y, colour);
  return 0;
}

int mh_osc_render(
    const char *path, const char *types, lo_arg **argv, int argc,
    void *data, void *user_data
    )
{
  matrix_render();
  return 0;
}

int mh_osc_clear(
    const char *path, const char *types, lo_arg **argv, int argc,
    void *data, void *user_data
    )
{
  matrix_clear();
  return 0;
}
