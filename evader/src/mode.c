#include "entity.h"
#include "game.h"

#include <stdbool.h> // included in entity.h
#include <stdio.h>
#include <stdlib.h> // for rand

#define MAX_ENEMIES 5000

static const int COLS = 41;
static const int ROWS = 21;
static int score = 0;
static int count = 0;

Entity player = {COLS / 2, ROWS - 2, NONE, '@', true, true, true};
Entity enemies[MAX_ENEMIES];

void move_enemies() {
  for (int i = 0; i < count; ++i) {
    bool wall_collision = safe_move(&enemies[i]);

    if (wall_collision) {
      score++;
      enemies[i].y = 0;
      enemies[i].x = rand() % (COLS - 1);
    }
  }
}

// necessary definitions

void setup() { add_entity(&player); }

void teardown(char arr[ROWS][COLS + 1], Game game) {
  (void) arr;
  printf("You scored %d points in %d seconds\n", score, game.timer);
}

void process(char arr[ROWS][COLS + 1], Game game) {
  printf("score: %d\n", score);
  printf("timer: %d\n", game.timer);

  if (game.loop_delta % 5 == 0) {
    move_enemies();
  }

  if (game.loop_delta % 60 == 0) {
    if (count < MAX_ENEMIES) {
      enemies[count] = (Entity){rand() % (COLS - 1), 0, DOWN, 'o', true, true, true};
      add_entity(&enemies[count]);
      count++;
    }
  }

  for (int i = 0; i < count; ++i) {
    if (collide(&enemies[i], &player)) {
      arr[player.y][player.x] = 'x';
      stop();
    }
  }
}

void input_hook(char c) {
  switch (c) {
  case 'a':
    player.direction = LEFT;
    break;
  case 'd':
    player.direction = RIGHT;
    break;
  }

  safe_move(&player);
}
