#include "engine.h"
#include "entity.h"

#include <stdbool.h> // included in entity.h
#include <stdio.h>
#include <stdlib.h> // for rand

#define MAX_ENEMIES 100

static int score = 0;
static int count = 0;
static double move_cooldown = 0.15;
static double move_timer = 0.0;

Entity player = {0, 0, '@', NONE, PLAYER, true, true};
Entity enemies[MAX_ENEMIES];

// necessary definitions

void setup(Game *game) {
  player.x = (*game).cols / 2;
  player.y = (*game).rows - 2;
  add_entity(&player);
}

void teardown(Game *game) {
  printf("You scored %d points in %d seconds\n", score, (*game).timer);
}

void update(Game *game) {
  printf("score: %d\n", score);
  printf("timer: %d\n", (*game).timer);

  move(&player);

  move_timer += (*game).time_delta;
  if (move_timer >= move_cooldown) {
    move_timer -= move_cooldown;

    for (int i = 0; i < count; ++i) {
      switch (move(&enemies[i])) {
      case WALL:
        enemies[i].x = rand() % ((*game).cols - 1);
        enemies[i].y = 0;
        break;
      case PLAYER:
        player.icon = 'X';
        stop();
        break;
      default:
        break;
      }
    }

    if (count < MAX_ENEMIES) {
      enemies[count] =
          (Entity){rand() % ((*game).cols - 1), 0, 'o', DOWN, ENEMY, true, true};
      add_entity(&enemies[count]);
      count++;
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
}
