#include "input.h"

#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>

bool running = true;
bool verbose = false;
time_t now;
unsigned long timer;
extern void setup(void);
extern void teardown(void);
extern const int ROWS;
extern const int COLS;
extern void game_hook(char arr[ROWS][COLS + 1]);

void print_grid(char arr[ROWS][COLS + 1]) {
  if (verbose) {
    printf("%lu seconds passed\n", timer);
  }

  char bounds[COLS + 3];
  bounds[COLS + 2] = 0;
  memset(bounds, '=', COLS + 2);
  printf("%s\n", bounds);
  
  for (int i = 0; i < ROWS; ++i) {
    printf("|%s|\n", arr[i]);

  }
  printf("%s\n", bounds);
}

void fill_grid(char arr[ROWS][COLS + 1], char c) {
  for (int i = 0; i < ROWS; ++i) {
    memset(arr[i], c, COLS);
    arr[i][COLS] = 0;
  }
}

void reset(char arr[ROWS][COLS + 1]) {
  printf("\e[H\e[J");
  fill_grid(arr, ' ');
}

void *game_loop(void *_) {
  setup();

  char arr[ROWS][COLS + 1];
  now = time(0);

  while (running) {
    timer = time(0) - now;
    game_hook(arr);

    print_grid(arr);
    usleep(1000);
    reset(arr);
  }

  teardown();

  return NULL;
}

void start(void) {
  printf("\e[?25l");
  pthread_t input;
  pthread_t game;
  pthread_create(&input, NULL, input_loop, NULL);
  pthread_create(&game, NULL, game_loop, NULL);

  pthread_join(game, NULL);
  pthread_cancel(input);
  pthread_join(input, NULL);
  printf("\e[?25h");
}

void stop(void) {
  running = false;
}
