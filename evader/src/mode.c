#include "game.h"
#include "player.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_ENEMIES 100

extern Player player;
extern int loop_delta;

const int ROWS = 20;
const int COLS = 2 * ROWS;
const char PLAYER_CHAR = 'x';
const unsigned long time_limit = 10;
const int level = 100;

unsigned long timer;
time_t now;

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
} Enemy;

Enemy enemies[MAX_ENEMIES];
int cur = 0;

void spawn_enemy(void) {
  for (int i = 0; i < cur; ++i) {
    enemies[i].y++;
    if (enemies[i].y >= ROWS) {
      enemies[i].y = 0;
      enemies[i].x = rand() % COLS;
    }
  }

  Enemy enemy = {rand() % COLS, 0};
  enemies[cur++] = enemy;
}

void game_hook(char arr[ROWS][COLS + 1]) {
  printf("score: %d\n", score);

  arr[player.y][player.x] = PLAYER_CHAR;

  for (int i = 0; i < cur; ++i) {
    Enemy cur_enemy = enemies[i];
    arr[cur_enemy.y][cur_enemy.x] = 'o';

    if (cur_enemy.y == player.y && cur_enemy.x == player.x)
      stop();
  }

  int level;
  if (score > 20) {
    level = 100;
  } else if (score > 10) {
    level = 500;
  } else {
    level = 1000;
  }

  if (loop_delta % level == 0) {
    spawn_enemy();

    for (int i = 0; i < cur; ++i) {
      Enemy cur_enemy = enemies[i];

      if (cur_enemy.y == player.y && cur_enemy.x != player.x)
        score++;
    }
  }
}

void setup() {
  player.x = COLS / 2;
  player.y = ROWS - 3;
}

void teardown() {
  printf("You scored %d points in %lu seconds\n", score, time_limit);
}
