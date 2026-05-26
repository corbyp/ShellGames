#include "game.h"
#include "entity.h"
#include "map.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

const Map map = (Map) {21, 41, '#', '#', '#', '#', '#', '#', '#', '#', ' '};
static Entity player = {0, 0, '@', true, true, true};
static const uint8_t time_limit = 10;
static int score = 0;

void input_hook(char c) {
  switch (c) {
  case 'w':
    move_up(&player, 0);
    break;
  case 's':
    move_down(&player, map.ROWS);
    break;
  case 'a':
    move_left(&player, 0);
    break;
  case 'd':
    move_right(&player, map.COLS);
    break;
  }
}

typedef struct Coin {
  int x;
  int y;
  bool exists;
} Coin;

Coin coin = {0, 0, false};

void spawn_coin() {
  coin.x = rand() % map.COLS;
  coin.y = rand() % map.ROWS;

  coin.exists = true;
}

void process(char arr[map.ROWS][map.COLS + 1], Game game) {
  if (game.timer > time_limit)
    stop();

  printf("score: %d\n", score);
  printf("%lu seconds remaining\n", time_limit - game.timer);
  arr[player.y][player.x] = player.icon;

  if (player.y == coin.y && player.x == coin.x) {
    coin.exists = false;
    score++;
  }
  
  if (!coin.exists)
    spawn_coin();
  
  arr[coin.y][coin.x] = 'o';
}

void setup() {
  
}

void teardown(Game game) {
  (void)game;
  printf("You scored %d points in %d seconds\n", score, time_limit);
}
