#include "entity.h"
#include "game.h"

#include <stdbool.h> // included in entity.h
#include <stdio.h>
#include <stdlib.h> // for rand

#define MAX_ENEMIES 5000

static int score = 0;
static int count = 0;

Entity player = {0, 0, NONE, '@', true, true};
Entity enemies[MAX_ENEMIES];

void move_enemies(int cols) {
  for (int i = 0; i < count; ++i) {
    bool wall_collision = safe_move(&enemies[i]);

    if (wall_collision) {
      score++;
      enemies[i].y = 0;
      enemies[i].x = rand() % (cols - 1);
    }
  }
}

// necessary definitions

void setup(Game *game) {
  player.x = (*game).cols / 2;
  player.y = (*game).rows - 2;
  add_entity(&player);
}

void teardown(Game *game) {
  printf("You scored %d points in %d seconds\n", score, (*game).timer);
}

void process(Game *game) {
  printf("score: %d\n", score);
  printf("timer: %d\n", (*game).timer);

  if ((*game).loop_delta % 5 == 0) {
    move_enemies((*game).cols);
  }

  if ((*game).loop_delta % 60 == 0) {
    if (count < MAX_ENEMIES) {
      enemies[count] = (Entity){rand() % ((*game).cols - 1), 0, DOWN, 'o', true, true};
      add_entity(&enemies[count]);
      count++;
    }
  }

  for (int i = 0; i < count; ++i) {
    if (collide(&enemies[i], &player)) {
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
  default:
    player.direction = NONE;
    break;
  }

  safe_move(&player);
}
