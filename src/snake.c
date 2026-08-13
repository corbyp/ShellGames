#include "engine.h"
#include "entity.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_SCORE 100

static Entity player[MAX_SCORE] = {
    {0, 0, '>', RIGHT, PLAYER, true, true},
};
static Entity coin = {0, 0, 'o', NONE, ITEM, true, true};

static uint8_t score = 0;
static Direction next_direction = RIGHT;
static Direction last_direction = RIGHT;

static double move_cooldown = 0.15;
static double move_timer = 0.0;

void move_trail(int prev_x, int prev_y) {
  for (int i = score; i > 1; --i) {
    player[i].x = player[i - 1].x;
    player[i].y = player[i - 1].y;
  }

  player[1].x = prev_x;
  player[1].y = prev_y;
}

void input_hook(char c) {
  Direction requested;
  char icon;

  switch (c) {
  case 'w':
    requested = UP;
    icon = '^';
    break;
  case 'a':
    requested = LEFT;
    icon = '<';
    break;
  case 's':
    requested = DOWN;
    icon = 'v';
    break;
  case 'd':
    requested = RIGHT;
    icon = '>';
    break;
  default:
    return;
  }

  if ((requested == UP && last_direction == DOWN) ||
      (requested == DOWN && last_direction == UP) ||
      (requested == LEFT && last_direction == RIGHT) ||
      (requested == RIGHT && last_direction == LEFT)) {
    return;
  }

  next_direction = requested;
  player[0].icon = icon;
}

void move_coin(int rows, int cols) {
  coin.y = rand() % rows;
  coin.x = rand() % cols;
}

void update(Game *game) {
  printf("score: %d\n", score);
  printf("time: %d seconds\n", (*game).timer);
  printf("x: %d y: %d\n", coin.x, coin.y);
  static int prev_x;
  static int prev_y;

  move_timer += (*game).time_delta;

  if (move_timer >= move_cooldown) {
    move_timer -= move_cooldown;

    prev_x = player[0].x;
    prev_y = player[0].y;

    player[0].direction = next_direction;
    last_direction = next_direction;

    switch (move(&player[0])) {
    case ITEM:
      move_coin((*game).rows, (*game).cols);
      if (score < MAX_SCORE - 1) {
        player[score + 1] = (Entity){player[score].x,
                                     player[score].y,
                                     'O',
                                     NONE,
                                     PLAYER,
                                     true,
                                     true};
        add_entity(&player[score + 1]);
      }
      ++score;
      break;
    case PLAYER:
      player[0].icon = 'X';
      stop();
      return;
    case WALL:
      player[0].icon = 'X';
      stop();
      return;
    default:
      break;
    }

    move_trail(prev_x, prev_y);
  }
}

void setup(Game *game) {
  move_coin((*game).rows, (*game).cols);
  add_entity(&player[0]);
  add_entity(&coin);
}

void teardown(Game *game) {
  printf("You scored %d points in %d seconds\n", score, (*game).timer);
}
