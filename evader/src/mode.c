#include "entity.h"
#include "game.h"
#include "map.h"

#include <stdbool.h> // included in entity.h
#include <stdio.h>
#include <stdlib.h> // for rand

#define MAX_ENEMIES 5000

// necessary definitions
const Map map = (Map){21, 41, '#', '#', '#', '#', '#', '#', '#', '#', ' '};

// own logic
int score = 0;
int count = 0;

Entity player = {map.COLS / 2, map.ROWS - 2, '@', true, true, true};
Entity enemies[MAX_ENEMIES];

void move_enemies() {
  for (int i = 0; i < count; ++i) {
    bool wall_collision = move_down(&enemies[i], map.ROWS);

    if (wall_collision) {
      score++;
      enemies[i].y = 0;
      enemies[i].x = rand() % (map.COLS - 1);
    }
  }
}

// necessary definitions

void setup() { add_entity(&player); }

void teardown(char arr[map.ROWS][map.COLS + 1], Game game) {
  (void) arr;
  printf("You scored %d points in %lu seconds\n", score, game.timer);
}

void process(char arr[map.ROWS][map.COLS + 1], Game game) {
  printf("score: %d\n", score);
  printf("timer: %lu\n", game.timer);

  if (game.loop_delta % 5 == 0) {
    move_enemies();
  }

  if (game.loop_delta % 60 == 0) {
    if (count < MAX_ENEMIES) {
      enemies[count] = (Entity){rand() % (map.COLS - 1), 0, 'o', true, true, true};
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
    move_left(&player, 0);
    break;
  case 'd':
    move_right(&player, map.COLS);
    break;
  }
}
