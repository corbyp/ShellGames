#include "engine.h"
#include "entity.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static const uint8_t score_limit = 10;
static Entity player = {0, 0, '@', NONE, PLAYER, true, true};
static Entity coin = {0, 0, 'o', NONE, ITEM, true, true};
static uint8_t score = 0;
static double move_cooldown = 0.15;
static double move_timer = 0.0;

void input_hook(char c) {
  switch (c) {
  case 'w':
    player.direction = UP;
    break;
  case 's':
    player.direction = DOWN;
    break;
  case 'a':
    player.direction = LEFT;
    break;
  case 'd':
    player.direction = RIGHT;
    break;
  }
}

void move_coin(int rows, int cols) {
  coin.y = rand() % rows;
  coin.x = rand() % cols;
}

void update(Game *game) {
  printf("score: %d / %d\n", score, score_limit);
  printf("time: %d seconds\n", (*game).timer);

  move_timer += (*game).time_delta;

  if (move_timer >= move_cooldown) {
    move_timer -= move_cooldown;
    switch (move(&player)) {
    case ITEM:
      move_coin((*game).rows, (*game).cols);
      ++score;
      break;
    default:
      break;
    }

    if (score >= score_limit)
      stop();
  }
}

void setup(Game *game) {
  (void)game;
  move_coin((*game).rows, (*game).cols);
  add_entity(&player);
  add_entity(&coin);
}

void teardown(Game *game) {
  printf("You scored %d points in %d seconds\n", score, (*game).timer);
}
