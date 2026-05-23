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
  case 'a':
    player.left(&player);
    break;
  case 'd':
    player.right(&player);
    break;
  }
}

typedef struct Enemy {
  int x;
  int y;
  bool exists;
} Enemy;

Enemy enemies[255];

void spawn_enemy(void) {
  
}

void game_hook(char arr[ROWS][COLS + 1]) {
  printf("score: %d\n", score);
  printf("%lu seconds remaining\n", time_limit - timer);
  arr[player.y][player.x] = PLAYER_CHAR;

  if (player.y == coin.y && player.x == coin.x) {
    coin.exists = false;
    score++;
  }
  
  arr[coin.y][coin.x] = 'o';
}

void setup() {
  player.x = COLS / 2;
  player.y = ROWS - 3;  
}

void teardown() {
  printf("You scored %d points in %lu seconds\n", score, time_limit);
}
