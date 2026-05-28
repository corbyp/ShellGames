#include "game.h"
#include "entity.h"
#include "input.h"

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

static uint8_t count = 0;
static int old_timer = 0;
static uint8_t fps;
static Map map = {21, 41, '#', '#', '#', '#', '#', '#', '#', '#', ' '};

// user defined
extern void setup(void);
extern void process(char arr[map.ROWS][map.COLS], Game game);
extern void teardown(char arr[map.ROWS][map.COLS], Game game);

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
  ++count;
  if (old_timer != game.timer) {
    fps = count;
    count = 0;
    old_timer = game.timer;
  }
  if (verbose)
    printf("Current FPS: %d\n", fps);
}

// external api

// game loop
void game_loop(void) {
  setup();

  Game game = {0, 0};
  char arr[map.ROWS][map.COLS + 1];
  set_bounds(0, map.ROWS, 0, map.COLS);
  time_t now = time(0);

  while (running) {
    printf("\e[H\e[J"); // deletes previous frame

    game.loop_delta = (game.loop_delta + 1) % 60; // 0 every 60th loop
    game.timer = time(0) - now;

    fill_grid(arr, map.BACKGROUND);
    add_entities(arr);

    process(arr, game);

    draw_grid(arr);
    draw_verbose(game);
    usleep(16450); // ~60fps
  }

  teardown(arr, game);
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

void set_char(char c, int x, int y) {}

void set_map_dims(const int rows, const int cols) {
  map.ROWS = rows;
  map.COLS = cols;
}

void set_map_chars(const char top, const char bottom, const char left, const char right, const char top_left, const char top_right, const char bottom_left, const char bottom_right, const char background) {
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
