#include "entity.h"
#include "game.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_SCORE 100

static Entity player[MAX_SCORE] = {
    {0, 0, RIGHT, '@', true, true},
};
static Entity coin = {0, 0, NONE, 'o', true, true};

static uint8_t score = 0;

void move_trail(int prev_x, int prev_y) {
  for (int i = score; i > 1; --i) {
    player[i].x = player[i - 1].x;
    player[i].y = player[i - 1].y;
  }

  player[1].x = prev_x;
  player[1].y = prev_y;
}

char last_c;

void input_hook(char c) { last_c = c; }

void move_coin(int rows, int cols) {
  coin.y = rand() % rows;
  coin.x = rand() % cols;

  // for (int i = 0; i < score; ++i) {
  // }
}

bool trail_collision(void) {
  if (score < 2)
    return false;
  for (int i = 1; i < score + 1; ++i)
    if (collide(&player[0], &player[i]))
      return true;

  return false;
}

void process(Game *game) {
  printf("score: %d\n", score);
  printf("time: %d seconds\n", (*game).timer);
  printf("x: %d y: %d\n", coin.x, coin.y);
  static int prev_x;
  static int prev_y;

  if ((*game).loop_delta % 20 == 0) {
    switch (last_c) {
    case 'w':
      if (player[0].direction != DOWN) {
        player[0].icon = '^';
        player[0].direction = UP;
      }
      break;
    case 's':
      if (player[0].direction != UP) {
        player[0].icon = 'v';
        player[0].direction = DOWN;
      }
      break;
    case 'a':
      if (player[0].direction != RIGHT) {
        player[0].icon = '<';
        player[0].direction = LEFT;
      }
      break;
    case 'd':
      if (player[0].direction != LEFT) {
        player[0].icon = '>';
        player[0].direction = RIGHT;
      }
      break;
    }
    prev_x = player[0].x;
    prev_y = player[0].y;

    if (trail_collision() || safe_move(&player[0])) {
      player[score + 1] = (Entity){
          player[0].x, player[0].y, NONE, 'X', true, true};
      add_entity(&player[score + 1]);
      stop();
      return;
    }

    move_trail(prev_x, prev_y);
  }

  if (collide(&player[0], &coin)) {
    move_coin((*game).rows, (*game).cols);
    if (score < MAX_SCORE - 1) {
      player[score + 1] = (Entity){
          player[score].x, player[score].y, NONE, 'O', true, true};
      add_entity(&player[score + 1]);
    }
    ++score;
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
