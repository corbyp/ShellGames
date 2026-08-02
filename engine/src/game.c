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

extern void input_hook(char c);

static bool running = true;
static bool verbose = false;
static double move_timer = 0.0;

static Game game = {.time_delta = 0,
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
                    .background = ' ',
                    .client = 0,
                    .server = 0};

// user defined
extern void setup(Game *game);
extern void update(Game *game);
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

void handle_input(void) {
  char c = 0;
  getch(&c);

  switch (c) {
  case 'q':
    stop();
    break;
  case 'v':
    toggle_verbose();
    break;
  }

  input_hook(c);
}

// external api

// game loop
void game_loop(void) {
  setup(&game);

  char arr[game.rows][game.cols + 1];
  set_bounds(0, game.rows, 0, game.cols);
  struct timespec prev, cur;
  clock_gettime(CLOCK_MONOTONIC, &prev);
  time_t now = time(0);

  while (running) {
    printf("\e[H\e[J"); // deletes previous frame

    // game.loop_delta =
    //     (game.loop_delta + 1) % 1000; // resets every 1000th loop or every ms
    game.timer = time(0) - now;
    move_timer += game.time_delta;

    handle_input();
    update(&game);

    fill_grid(arr, game.background);
    add_entities(arr);

    draw_grid(arr);

    clock_gettime(CLOCK_MONOTONIC, &cur);
    draw_verbose(game.time_delta);
    game.time_delta =
        (cur.tv_sec - prev.tv_sec) + (cur.tv_nsec - prev.tv_nsec) / 1e9;
    prev = cur;
    fix_fps(game.time_delta);
  }

  teardown(&game);
}

// start stop

void start(int client, int server) {
  enable_raw_mode();
  printf("\e[?25l");

  game.client = client;
  game.server = server;
  
  game_loop();
}

void stop(void) {
  running = false;
  printf("\e[?25h");
}

void toggle_verbose(void) { verbose = !verbose; }
