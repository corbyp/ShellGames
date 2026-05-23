#include "game.h"
#include "player.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

extern Player player;
extern unsigned long timer;
const int ROWS = 20;
const int COLS = 2 * ROWS;
const char PLAYER_CHAR = 'x';
const unsigned long time_limit = 10;

int score = 0;

void input_hook(char c) {
  switch (c) {
  case 'w':
    player.up(&player);
    break;
  case 's':
    player.down(&player);
    break;
  case 'a':
    player.left(&player);
    break;
  case 'd':
    player.right(&player);
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
  coin.x = rand() % COLS;
  coin.y = rand() % ROWS;

  coin.exists = true;
}

void game_hook(char arr[ROWS][COLS + 1]) {
  if (timer > time_limit)
    stop();

  printf("score: %d\n", score);
  printf("%lu seconds remaining\n", time_limit - timer);
  arr[player.y][player.x] = PLAYER_CHAR;

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

void teardown() {
  printf("You scored %d points in %lu seconds\n", score, time_limit);
}
