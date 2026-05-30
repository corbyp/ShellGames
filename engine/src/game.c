#include "game.h"
#include "entity.h"
#include "input.h"

#include <bits/time.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static bool running = true;
static bool verbose = false;

static Game game = {.loop_delta = 0,
                    .timer = 0,
                    .rows = 21,
                    .cols = 41,
                    .top = '#',
                    .bottom = '#',
                    .left = '#',
                    .right = '#',
                    .top_left = '#',
                    .top_right = '#',
                    .bottom_left = '#',
                    .bottom_right = '#',
                    .background = ' '};

// user defined
extern void setup(Game *game);
extern void process(Game *game);
extern void teardown(Game *game);

void add_entities(char arr[game.rows][game.cols + 1]) {
  for (int i = 0; i < entity_count(); ++i) {
    Entity cur_entity = get_entity(i);
    arr[cur_entity.y][cur_entity.x] = cur_entity.icon;
  }
}

// grid

void fill_grid(char arr[game.rows][game.cols + 1], char c) {
  for (int i = 0; i < game.rows; ++i) {
    memset(arr[i], c, game.cols);
    arr[i][game.cols] = 0;
  }
}

void draw_grid(char arr[game.rows][game.cols + 1]) {
  char horizontal[game.cols + 3];
  memset(horizontal, game.top, game.cols + 1);
  horizontal[0] = game.top_left;
  horizontal[game.cols + 1] = game.top_right;
  horizontal[game.cols + 2] = 0;
  printf("%s\n", horizontal);

  for (int i = 0; i < game.rows; ++i)
    printf("%c%s%c\n", game.left, arr[i], game.right);

  memset(horizontal, game.bottom, game.cols + 1);
  horizontal[0] = game.bottom_left;
  horizontal[game.cols + 1] = game.bottom_right;
  horizontal[game.cols + 2] = 0;
  printf("%s\n", horizontal);
}

void draw_verbose(long elapsed) {
  static uint8_t count;
  static int old_timer;
  static uint8_t fps;

  ++count;
  if (old_timer != game.timer) {
    fps = count;
    count = 0;
    old_timer = game.timer;
  }
  if (verbose) {
    printf("time: %d seconds\n", game.timer);
    printf("Current FPS: %d\n", fps);
    printf("Elapsed: %ld µs\n", elapsed);
  }
}

void fix_fps(long elapsed) {
  // 60 fps = 60 frames / 1 s
  // 60 fps = 60 frames / 1000 ms
  // 60 fps = 60 frames / 1.000.000 µs
  // 60 fps = 6 frames / 100.000 µs

  // usleep(16667 - elapsed); // ~60fps
  usleep(10000 -
         elapsed); // 100fps since every loop waits for 10.000 µs and thus
  // there is a loop every 10 ms which equivilates to 100 loops / s
}

// external api

// game loop
void game_loop(void) {
  setup(&game);

  char arr[game.rows][game.cols + 1];
  set_bounds(0, game.rows, 0, game.cols);
  struct timespec start, end;
  time_t now = time(0);

  while (running) {
    clock_gettime(CLOCK_MONOTONIC, &start);
    printf("\e[H\e[J"); // deletes previous frame

    game.loop_delta =
        (game.loop_delta + 1) % 1000; // resets every 1000th loop or every ms
    game.timer = time(0) - now;

    process(&game);

    fill_grid(arr, game.background);
    add_entities(arr);

    draw_grid(arr);

    clock_gettime(CLOCK_MONOTONIC, &end);
    long elapsed = (end.tv_sec - start.tv_sec) * 1000000 +
                   (end.tv_nsec - start.tv_nsec) / 1000;
    draw_verbose(elapsed);
    fix_fps(elapsed);
  }

  teardown(&game);
}

// start stop

void start(void) {
  printf("\e[?25l");
  pthread_t input;
  pthread_create(&input, NULL, input_loop, NULL);

  game_loop();

  pthread_cancel(input);
  pthread_join(input, NULL);
}

void stop(void) {
  running = false;
  printf("\e[?25h");
}

void toggle_verbose(void) { verbose = !verbose; }
