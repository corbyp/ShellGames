#include "game.h"
#include "entity.h"
#include "input.h"

#include <bits/time.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

typedef struct Map {
  int ROWS;
  int COLS;

  char TOP;
  char BOTTOM;
  char LEFT;
  char RIGHT;
  char TOP_LEFT;
  char TOP_RIGHT;
  char BOTTOM_LEFT;
  char BOTTOM_RIGHT;

  char BACKGROUND;
} Map;

static bool running = true;
static bool verbose = false;

static Map map = {21, 41, '#', '#', '#', '#', '#', '#', '#', '#', ' '};

// user defined
extern void setup(void);
extern void process(Game game);
extern void teardown(Game game);

void signalHandler(int sig) {
  (void)sig;
  stop();
}

void add_entities(char arr[map.ROWS][map.COLS + 1]) {
  for (int i = 0; i < entity_count(); ++i) {
    Entity cur_entity = get_entity(i);
    arr[cur_entity.y][cur_entity.x] = cur_entity.icon;
  }
}

// grid

void fill_grid(char arr[map.ROWS][map.COLS + 1], char c) {
  for (int i = 0; i < map.ROWS; ++i) {
    memset(arr[i], c, map.COLS);
    arr[i][map.COLS] = 0;
  }
}

void draw_grid(char arr[map.ROWS][map.COLS + 1]) {
  char horizontal[map.COLS + 3];
  memset(horizontal, map.TOP, map.COLS + 1);
  horizontal[0] = map.TOP_LEFT;
  horizontal[map.COLS + 1] = map.TOP_RIGHT;
  horizontal[map.COLS + 2] = 0;
  printf("%s\n", horizontal);

  for (int i = 0; i < map.ROWS; ++i)
    printf("%c%s%c\n", map.LEFT, arr[i], map.RIGHT);

  memset(horizontal, map.BOTTOM, map.COLS + 1);
  horizontal[0] = map.BOTTOM_LEFT;
  horizontal[map.COLS + 1] = map.BOTTOM_RIGHT;
  horizontal[map.COLS + 2] = 0;
  printf("%s\n", horizontal);
}

void draw_verbose(Game game) {
  static uint8_t count;
  static int old_timer;
  static uint8_t fps;

  ++count;
  if (old_timer != game.timer) {
    fps = count;
    count = 0;
    old_timer = game.timer;
  }
  if (verbose)
    printf("Current FPS: %d\n", fps);
}

void fix_fps(struct timespec start, struct timespec end) {
  long elapsed = (end.tv_sec - start.tv_sec) * 1000000 +
                 (end.tv_nsec - start.tv_nsec) / 1000;
  printf("Elapsed: %ld µs\n", elapsed);

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
  setup();

  Game game = {0, 0};
  char arr[map.ROWS][map.COLS + 1];
  set_bounds(0, map.ROWS, 0, map.COLS);
  struct timespec start, end;
  time_t now = time(0);

  while (running) {
    clock_gettime(CLOCK_MONOTONIC, &start);

    game.loop_delta =
        (game.loop_delta + 1) % 1000; // resets every 1000th loop or every ms
    game.timer = time(0) - now;

    process(game);

    fill_grid(arr, map.BACKGROUND);
    add_entities(arr);

    draw_grid(arr);
    draw_verbose(game);

    clock_gettime(CLOCK_MONOTONIC, &end);
    fix_fps(start, end);
    printf("\e[H\e[J"); // deletes previous frame
  }

  teardown(game);
  draw_grid(arr);
  draw_verbose(game);
}

// start stop

void start(void) {
  signal(SIGINT, signalHandler);
  printf("\e[?25l");
  pthread_t input;
  pthread_create(&input, NULL, input_loop, NULL);

  game_loop();

  pthread_cancel(input);
  pthread_join(input, NULL);
  printf("\e[?25h");
}

void stop(void) { running = false; }

void toggle_verbose(void) { verbose = !verbose; }

void set_map_dims(const int rows, const int cols) {
  map.ROWS = rows;
  map.COLS = cols;
}

void set_map_chars(const char top, const char bottom, const char left,
                   const char right, const char top_left, const char top_right,
                   const char bottom_left, const char bottom_right,
                   const char background) {
  map.TOP = top;
  map.BOTTOM = bottom;
  map.LEFT = left;
  map.RIGHT = right;
  map.TOP_LEFT = top_left;
  map.TOP_RIGHT = top_right;
  map.BOTTOM_LEFT = bottom_left;
  map.BOTTOM_RIGHT = bottom_right;
  map.BACKGROUND = background;
}
