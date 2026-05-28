#include "entity.h"
#include "game.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

const int ROWS = 21, COLS = 41;
static const uint8_t score_limit = 10;
static Entity player = {0, 0, NONE, '@', true, true, true};
static Entity coin = {0, 0, NONE, 'o', true, true, true};
static uint8_t score = 0;

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
  safe_move(&player);
}

void move_coin() {
  coin.x = rand() % COLS;
  coin.y = rand() % ROWS;
}

void process(char arr[ROWS][COLS + 1], Game game) {
  printf("score: %d / %d\n", score, score_limit);
  printf("time: %d seconds\n", game.timer);

  if (score >= score_limit)
    stop();

  if (collide(&player, &coin)) {
    move_coin();
    ++score;
  }
}

void setup() {
  add_entity(&player);
  add_entity(&coin);
}

void teardown(char arr[ROWS][COLS + 1], Game game) {
  printf("You scored %d points in %d seconds\n", score, game.timer);
}
