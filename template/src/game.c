#include "input.h"

#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

bool running = true;
extern const int ROWS;
extern const int COLS;
extern void update(char arr[ROWS][COLS + 1]);

void print_grid(char arr[ROWS][COLS + 1]) {
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
  printf("\033[H\033[J");
  fill_grid(arr, ' ');
}

void *game_loop(void *_) {
  char arr[ROWS][COLS + 1];

  while (running) {
    update(arr);

    print_grid(arr);
    usleep(10 * 1000);
    reset(arr);
  }

  return NULL;
}

void start_game(void) {
  pthread_t input;
  pthread_t game;
  pthread_create(&input, NULL, input_loop, NULL);
  pthread_create(&game, NULL, game_loop, NULL);

  pthread_join(input, NULL);
  pthread_join(game, NULL);
}
